#include "Warehouse.h"
#include <iostream>
#include <algorithm> // For std::remove_if or vector manipulation
#include <cmath>
#include <map>
#include <tuple>
#include <graphalgorithms.hpp>

Warehouse::Warehouse(double latitude, double longitude, int totalEmployees, const Inventory &initialInventory, QObject *parent)
    : QObject{parent}, m_coordinate(latitude, longitude), totalEmployees(totalEmployees), busyEmployees(0), inventory(initialInventory) {}

QGeoCoordinate Warehouse::getCoordinate() const
{
    return m_coordinate;
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

const std::vector<Order> &Warehouse::getPendingOrders() const
{
    return pendingOrders;
}

const std::vector<Order> &Warehouse::getReadyToShipOrders() const
{
    return readyToShipOrders;
}

void Warehouse::dockTruck(std::unique_ptr<Truck> truck)
{
    dockedTrucks.push_back(std::move(truck));
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
        Truck *truck = truckPtr.get();
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
    if (pendingOrders.empty())
        return resultPaths;

    const int DEFAULT_TRUCK_CAPACITY = 100; // units (items).

    const size_t n = pendingOrders.size();

    // Distances from warehouse (warehouse) to each customer and between customers
    std::vector<double> d0(n);
    std::vector<std::vector<double>> dij(n, std::vector<double>(n, 0.0));

    for (size_t i = 0; i < n; ++i)
    {
        d0[i] = haversine(m_coordinate.latitude(), m_coordinate.longitude(), pendingOrders[i].getLatitude(), pendingOrders[i].getLongitude());
    }

    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = i + 1; j < n; ++j)
        {
            double d = haversine(pendingOrders[i].getLatitude(), pendingOrders[i].getLongitude(), pendingOrders[j].getLatitude(), pendingOrders[j].getLongitude());
            dij[i][j] = dij[j][i] = d;
        }
    }

    std::vector<Route> routes;
    routes.reserve(n);
    for (size_t i = 0; i < n; ++i)
    {
        Route r;
        r.orders.push_back(i);
        r.load = pendingOrders[i].getTotalQuantity();
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



    // A* solver instance
    Designar::Astar<CityGraph, CityEdgeDistance, CityHeuristic> astar_solver;

    for (const auto &r : routes)
    {
        if (!r.active)
            continue;

        // Build a full path starting at warehouse
            CityGraph::Node *warehouseNode = find_nearest_node(city, m_coordinate.latitude(), m_coordinate.longitude());
        if (!warehouseNode)
            continue; // can't route without warehouse node

        Designar::Path<CityGraph> fullPath;
        // insert node pointers
        fullPath.insert(warehouseNode);

        CityGraph::Node *currentNode = warehouseNode;
        bool failed = false;

        for (auto orderIdx : r.orders)
        {
            const Order &o = pendingOrders[orderIdx];
            CityGraph::Node *targetNode = find_nearest_node(city, o.getLatitude(), o.getLongitude());
            if (!targetNode)
            {
                failed = true;
                break;
            }

            auto segment = astar_solver.search_min_path(city, currentNode, targetNode);
            if (segment.size() == 0)
            {
                failed = true;
                break;
            }

            fullPath.insert(targetNode);

            currentNode = targetNode;
        }

        if (!failed)
        {
            auto backSeg = astar_solver.search_min_path(city, currentNode, warehouseNode);
            if (backSeg.size() > 0)
            {
                fullPath.insert(warehouseNode);
            }
            resultPaths.push_back(std::move(fullPath));
        }
    }

    return resultPaths;
}