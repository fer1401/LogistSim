#include "Warehouse.h"
#include <algorithm> // For std::remove_if or vector manipulation
#include <cmath>
#include <tuple>
#include <limits>
#include <graphalgorithms.hpp>

Warehouse::Warehouse(double latitude, double longitude, int totalEmployees, const Inventory &initialInventory, QObject *parent)
    : QObject{parent}, m_coordinate(latitude, longitude), totalEmployees(totalEmployees), busyEmployees(0), inventory(initialInventory)
{
    dockedTrucks.append(new Truck(1, m_coordinate, this));
}

Warehouse::~Warehouse()
{
    qDeleteAll(dockedTrucks);
}

QGeoCoordinate Warehouse::getCoordinate() const
{
    return m_coordinate;
}

QList<QObject *> Warehouse::getVisualTrucks() const
{
    return reinterpret_cast<const QList<QObject*>&>(dockedTrucks);
}

int Warehouse::getTotalEmployees() const
{
    return totalEmployees;
}

int Warehouse::getBusyEmployees() const
{
    return busyEmployees;
}

int Warehouse::getAvailableEmployees() const
{
    return totalEmployees - busyEmployees;
}

const QList<Truck *> Warehouse::getDockedTrucks()
{
    return dockedTrucks;
}

const std::vector<Order> &Warehouse::getPendingOrders() const
{
    return pendingOrders;
}

const std::vector<Order> &Warehouse::getReadyToShipOrders() const
{
    return readyToShipOrders;
}

void Warehouse::dockTruck(Truck *truck)
{
    dockedTrucks.append(truck);
}

const Inventory &Warehouse::getInventory() const
{
    return inventory;
}

void Warehouse::addOrder(const Order &order)
{
    pendingOrders.push_back(order);
}

bool Warehouse::fulfillNextOrder()
{
    if (getAvailableEmployees() <= 0)
    {
        return false;
    }

    if (pendingOrders.empty())
    {
        return false;
    }

    Order &nextOrder = pendingOrders.front();

    if (inventory.canFulfillOrder(nextOrder))
    {
        busyEmployees++;

        for (const auto &pair : nextOrder.getProductQuantities())
        {
            inventory.removeStock(pair.first, pair.second);
        }

        readyToShipOrders.push_back(std::move(nextOrder));

        pendingOrders.erase(pendingOrders.begin());

        busyEmployees--;

        return true;
    }
    else
    {
        return false;
    }
}

void Warehouse::fullfillAllPossibleOrders()
{
    bool possibleToFulfill = true;
    while (possibleToFulfill)
    {
        possibleToFulfill = fulfillNextOrder();
    }
}

// Collect active routes and convert to Designar::Path placeholders
// Define distance (edge cost) and heuristic for A*
struct CityEdgeDistance
{
    using Type = double;
    static constexpr Type ZERO = 0.0;
    Type operator()(const CityGraph::Arc *a) const
    {
        return a->get_info().getLength();
    }
};

// Helper: haversine distance (meters)
double haversine (double lat1, double lon1, double lat2, double lon2)
{
    static const double R = 6371000.0; // Earth radius in meters
    const double toRad = M_PI / 180.0;
    double dLat = (lat2 - lat1) * toRad;
    double dLon = (lon2 - lon1) * toRad;
    double a = std::sin(dLat / 2) * std::sin(dLat / 2) + std::cos(lat1 * toRad) * std::cos(lat2 * toRad) * std::sin(dLon / 2) * std::sin(dLon / 2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    return R * c;
};

struct CityHeuristic
{
    using Type = double;
    static constexpr Type ZERO = 0.0;
    Type operator()(const CityGraph::Node *current, const CityGraph::Node *target) const
    {
        double lat1 = current->get_info().getLatitude();
        double lon1 = current->get_info().getLongitude();
        double lat2 = target->get_info().getLatitude();
        double lon2 = target->get_info().getLongitude();
        return haversine(lat1, lon1, lat2, lon2);
    }
};

// Helper: find nearest graph node to given coordinates
CityGraph::Node* find_nearest_node(CityGraph &city, double lat, double lon)
{
    CityGraph::Node *best = nullptr;
    double bestd = std::numeric_limits<double>::infinity();
    for (const auto &node : city.nodes())
    {
        double d = haversine(lat, lon, node->get_info().getLatitude(), node->get_info().getLongitude());
        if (d < bestd)
        {
            bestd = d;
            best = node;
        }
    }
    return best;
}

// Initial routes: one customer per route
struct Route
{
    std::vector<size_t> orders;
    int load = 0;
    bool active = true;
};

void Warehouse::shipOrders(CityGraph &city)
{
    auto routes = planTruckRoutes(city);
    for (const auto &route : routes)
    {
        truckRoutes.push(route);
    }
    assignRoutes();
}

void Warehouse::assignRoutes()
{
    for (auto &truckPtr : dockedTrucks)
    {
        Truck *truck = truckPtr;
        if (!truck->isShipping() && !truckRoutes.empty())
        {
            Designar::Path<CityGraph> route = truckRoutes.front();
            truckRoutes.pop();
            truck->assignRoute(route);
        }
    }
}

std::vector<Designar::Path<CityGraph>> Warehouse::planTruckRoutes(CityGraph &city)
{
    // Clark-Wright Savings heuristic implementation
    std::vector<Designar::Path<CityGraph>> resultPaths;

    // If there are no pending orders, return empty
    if (readyToShipOrders.empty())
        return resultPaths;

    // A* solver instance
    Designar::Astar<CityGraph, CityEdgeDistance, CityHeuristic> astar_solver;


    const int DEFAULT_TRUCK_CAPACITY = 100; // units (items).

    const size_t n = readyToShipOrders.size();

    // Distances from warehouse to each customer and between customers computed via A* path lengths.
    // If a path can't be found, the distance is set to infinity.
    std::vector<double> d0(n, std::numeric_limits<double>::infinity());
    std::vector<std::vector<double>> dij(n, std::vector<double>(n, std::numeric_limits<double>::infinity()));

    // Find nearest graph nodes for the warehouse and each customer
    CityGraph::Node *warehouseNode = find_nearest_node(city, m_coordinate.latitude(), m_coordinate.longitude());

    std::vector<CityGraph::Node*> custNode(n, nullptr);
    for (size_t i = 0; i < n; ++i)
        custNode[i] = find_nearest_node(city, readyToShipOrders[i].getLatitude(), readyToShipOrders[i].getLongitude());

    // Helper to compute path distance by summing arc lengths
    auto path_distance = [](const Designar::Path<CityGraph> &p) -> double {
        double sum = 0.0;
        p.for_each([&sum](auto node, auto arc)
        {
            if (arc)
                sum += arc->get_info().getLength();
        });
        return sum;
    };

    // Cache A* paths so we don't recompute them when assembling final routes.
    // warehouse -> customer
    std::vector<Designar::Path<CityGraph>> path_w_cust;
    path_w_cust.reserve(n);

    // customer -> warehouse
    std::vector<Designar::Path<CityGraph>> path_cust_w;
    path_cust_w.reserve(n);

    // customer -> customer (all directions)
    std::vector<std::vector<Designar::Path<CityGraph>>> dijPaths(n, std::vector<Designar::Path<CityGraph>>(n, Designar::Path<CityGraph>(city)));

    // Compute warehouse->customer and customer->warehouse
    for (size_t i = 0; i < n; ++i)
    {
        // default empty paths (size()==0) correspond to unreachable
        Designar::Path<CityGraph> p_w_c(city);
        Designar::Path<CityGraph> p_c_w(city);

        p_w_c = astar_solver.search_min_path(city, warehouseNode, custNode[i]);
        if (p_w_c.size() == 0)
            d0[i] = std::numeric_limits<double>::infinity();
        else
            d0[i] = path_distance(p_w_c);

        p_c_w = astar_solver.search_min_path(city, custNode[i], warehouseNode);
        if (p_c_w.size() == 0)
            d0[i] = std::numeric_limits<double>::infinity();

        path_w_cust.push_back(std::move(p_w_c));
        path_cust_w.push_back(std::move(p_c_w));
    }

    // Compute customer->customer paths (all ordered pairs)
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            if (i == j)
            {
                dij[i][j] = 0.0;
                continue;
            }
            if (!custNode[i] || !custNode[j])
            {
                dij[i][j] = std::numeric_limits<double>::infinity();
                // leave dijPaths[i][j] as empty path
                continue;
            }

            auto p = astar_solver.search_min_path(city, custNode[i], custNode[j]);
            if (p.size() == 0)
            {
                dij[i][j] = std::numeric_limits<double>::infinity();
            }
            else
            {
                dij[i][j] = path_distance(p);
            }

            dijPaths[i][j] = std::move(p);
        }
    }

    std::vector<Route> routes;
    routes.reserve(n);
    for (size_t i = 0; i < n; ++i)
    {
        Route r;
        r.orders.push_back(i);
        r.load = readyToShipOrders[i].getTotalQuantity();
        routes.push_back(std::move(r));
    }

    // Savings list: (saving, i, j)
    std::vector<std::tuple<double, size_t, size_t>> savings;
    savings.reserve(n * (n - 1) / 2);
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = i + 1; j < n; ++j)
        {
            double s = d0[i] + d0[j] - dij[i][j];
            savings.emplace_back(s, i, j);
        }
    }

    // Sort savings in descending order
    std::sort(savings.begin(), savings.end(), [](const auto &a, const auto &b)
              { return std::get<0>(a) > std::get<0>(b); });

    // Map from order index to route index
    std::vector<int> orderRoute(n);
    for (size_t i = 0; i < n; ++i)
        orderRoute[i] = static_cast<int>(i);

    std::cout << "Starting Clark-Wright with " << n << " orders and " << routes.size() << " initial routes.\n";

    // Try to merge routes according to savings
    for (const auto &entry : savings)
    {
        double s;
        size_t i, j;
        std::tie(s, i, j) = entry;
        int ri = orderRoute[i];
        int rj = orderRoute[j];
        if (ri == rj)
            continue; // already same route
        if (!routes[ri].active || !routes[rj].active)
            continue;

        // Only allow merging if i is at an end of its route and j at an end of its route
        auto &Ri = routes[ri].orders;
        auto &Rj = routes[rj].orders;

        bool i_is_front = (Ri.front() == i);
        bool i_is_back = (Ri.back() == i);
        bool j_is_front = (Rj.front() == j);
        bool j_is_back = (Rj.back() == j);

        if (!((i_is_front || i_is_back) && (j_is_front || j_is_back)))
            continue;

        int combinedLoad = routes[ri].load + routes[rj].load;
        if (combinedLoad > DEFAULT_TRUCK_CAPACITY)
            continue;

        // Determine how to merge: we want i at one end and j at the matching end
        // Four cases to concatenate in proper orientation
        std::vector<size_t> newOrders;
        if (i_is_front && j_is_back)
        {
            // reverse Ri, then append Rj
            newOrders = Ri;
            std::reverse(newOrders.begin(), newOrders.end());
            newOrders.insert(newOrders.end(), Rj.begin(), Rj.end());
        }
        else if (i_is_back && j_is_front)
        {
            // Ri then Rj
            newOrders = Ri;
            newOrders.insert(newOrders.end(), Rj.begin(), Rj.end());
        }
        else if (i_is_front && j_is_front)
        {
            // reverse both then append
            newOrders = Ri;
            std::reverse(newOrders.begin(), newOrders.end());
            newOrders.insert(newOrders.end(), Rj.begin(), Rj.end());
        }
        else
        { // i_is_back && j_is_back
            // Ri then reverse Rj
            newOrders = Ri;
            auto revJ = Rj;
            std::reverse(revJ.begin(), revJ.end());
            newOrders.insert(newOrders.end(), revJ.begin(), revJ.end());
        }

        // Create new route in ri and deactivate rj
        routes[ri].orders = std::move(newOrders);
        routes[ri].load = combinedLoad;
        routes[rj].active = false;

        // Update orderRoute mapping
        for (auto idx : routes[ri].orders)
            orderRoute[idx] = ri;
    }

    std::cout << "Merged to " << std::count_if(routes.begin(), routes.end(), [](const Route &r){ return r.active; }) << " routes after savings.\n";

    
    for (const auto &r : routes)
    {
        if (!r.active)
            continue;

        // We already computed/located the warehouse node earlier (warehouseNode).
        if (!warehouseNode)
            continue; // can't route without warehouse node

        Designar::Path<CityGraph> fullPath(city);

        bool failed = false;
        int prevOrderIdx = -1; // -1 means warehouse
        for (auto orderIdx : r.orders)
        {
            // select cached segment: warehouse->cust for first, else prevCust->cust
            const Designar::Path<CityGraph> &segRef = (prevOrderIdx == -1) ? path_w_cust[orderIdx] : dijPaths[prevOrderIdx][orderIdx];

            if (segRef.size() == 0)
            {
                std::cout << "Failed to find cached path segment for route assembly (from " << prevOrderIdx << " to " << orderIdx << ")\n";
                failed = true;
                break;
            }

            // copy segment so we can remove last node without mutating cache
            Designar::Path<CityGraph> seg = segRef;
            if (seg.size() > 0)
                seg.remove_last_node(); // avoid duplication when appending

            seg.for_each([&fullPath](auto node, auto arc)
            {
                fullPath.append(node);
            });

            prevOrderIdx = static_cast<int>(orderIdx);
        }

        if (!failed)
        {
            // append last customer -> warehouse
            if (prevOrderIdx == -1)
            {
                // no orders? shouldn't happen because routes are non-empty
                continue;
            }

            const Designar::Path<CityGraph> &backRef = path_cust_w[prevOrderIdx];
            if (backRef.size() == 0)
            {
                std::cout << "Failed to find cached return path from customer " << prevOrderIdx << " to warehouse\n";
                continue;
            }

            // append full back path (no removal)
            backRef.for_each([&fullPath](auto node, auto arc)
            {
                fullPath.append(node);
            });

            resultPaths.push_back(std::move(fullPath));
        }
    }

    std::cout << "Planned " << resultPaths.size() << " routes for shipment.\n";

    return resultPaths;
}
