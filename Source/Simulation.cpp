#include "Simulation.h"
#include "Inventory.h"
#include "graphalgorithms.hpp"
#include <limits>

const int simulationClockInterval = 200;

Simulation::Simulation(QObject *parent) : QObject{parent}, city{City()}
{
    Inventory initialInventory;
    initialInventory.addStock(1, 250);
    initialInventory.addStock(2, 300);
    initialInventory.addStock(3, 475);

    Warehouse *warehouseA = new Warehouse(8.571765, -71.179717, 10, 3, initialInventory, this);
    Warehouse *warehouseB = new Warehouse(8.5962673, -71.1518601, 5, 2, initialInventory, this);

    warehouses.append(warehouseA);
    warehouses.append(warehouseB);
    
    // Initialize products
    productCatalog.emplace_back(Product(1, "Laptop", "HP Spectre x360", 1));
    productCatalog.emplace_back(Product(2, "Smartphone", "Samsung Galaxy S21", 3));
    productCatalog.emplace_back(Product(3, "Tablet", "iPad Pro", 5));

    simulationClock = new QTimer(this);
    QTimer::connect(simulationClock, SIGNAL(timeout()), this, SLOT(simulationTick()));
    simulationClock->setInterval(simulationClockInterval);

    for (int i = 0; i < 75; ++i)
    {
        generateOrder();
    }

    emit warehousesChanged();
}

Simulation::~Simulation()
{
    qDeleteAll(warehouses);
    delete simulationClock;
}

void Simulation::generateOrder()
{
    Order newOrder("Client_" + std::to_string(incomingOrders.size() + 1),
                   rng.randomLatitude(),
                   rng.randomLongitude());

    int numberOfProducts = rng.randomNumProducts();
    for (int i = 0; i < numberOfProducts; ++i)
    {
        int productIndex = rng.selectProduct(static_cast<int>(productCatalog.size())) - 1; // Adjust for 0-based index
        int qty = rng.randomQuantity() + 1; // preserve previous +1 behavior
        newOrder.addProduct(productCatalog[productIndex], qty);
    }

    incomingOrders.push_back(newOrder);
}

void Simulation::assignOrdersToWarehouses()
{
    // A* solver instance
    Designar::Astar<CityGraph, CityEdgeDistance, CityHeuristic> astar_solver;
    // Process incoming orders
    for (int i = 0; i < incomingOrders.size(); ++i)
    {
        simulationStats.incrementTotalOrdersReceived();

        Order order = incomingOrders[i];

        CityGraph::Node *customerNode = find_nearest_node(city.getGraph(), order.getLatitude(), order.getLongitude());
        
        // Variables to track the best (closest) fulfilling warehouse found so far
        Warehouse* bestWarehouse = nullptr;
        double bestCost = std::numeric_limits<double>::max();

        for (auto& warehouse : warehouses)
        {
            CityGraph::Node *warehouseNode = find_nearest_node(city.getGraph(), warehouse->getCoordinate().latitude(), warehouse->getCoordinate().longitude());
            Designar::Path<CityGraph> p_w_c = astar_solver.search_min_path(city.getGraph(), warehouseNode, customerNode);
            Designar::Path<CityGraph> p_c_w = astar_solver.search_min_path(city.getGraph(), customerNode, warehouseNode);

            if (p_w_c.size() == 0 || p_c_w.size() == 0)
            {
                continue; // can't route from this warehouse to customer
            }
            
            if (warehouse->getInventory().canFulfillOrder(order))
            {
                double cost = path_distance(p_w_c) + path_distance(p_c_w) + warehouse->getCurrentLoad();
                // Check if this warehouse is closer than the current best
                if (cost < bestCost)
                {
                    bestCost = cost;
                    bestWarehouse = warehouse;
                }
            }            
        }

        if (bestWarehouse != nullptr)
        {
            bestWarehouse->addOrder(order);
            visualOrders.append(QVariant::fromValue(QGeoCoordinate(customerNode->get_info().getLatitude(), customerNode->get_info().getLongitude())));
            incomingOrders.erase(incomingOrders.begin() + i);
            i--; // Adjust index after erasure
            simulationStats.incrementTotalOrdersFulfilled();
        }
    }
}

void Simulation::shipOrders()
{
    for (auto& warehouse : warehouses)
    {
        auto [assignedTrips, distanceTravelled] = warehouse->shipOrders(city.getGraph());
        simulationStats.addDistanceTraveled(distanceTravelled);
        simulationStats.addTripsMade(assignedTrips);
    }
}

QTimer *Simulation::getSimulationClock()
{
    return simulationClock;
}

void Simulation::startClock()
{
    simulationClock->start();
}

void Simulation::stopClock()
{
    simulationClock->stop();
}

QList<QObject *> Simulation::getVisualWarehouses()
{
    return reinterpret_cast<const QList<QObject*>&>(warehouses);
}

QList<QObject *> Simulation::getVisualTrucks()
{
    QList<QObject*> result;

    for (const auto &warehouse : warehouses)
    {
        for (const auto &truck : warehouse->getVisualTrucks())
        {
            result.append(truck);
        }
    }

    return result;
}

QVariantList Simulation::getVisualOrders()
{
    return visualOrders;
}

void Simulation::simulationTick()
{
    simulationTime++;

    for (const auto &warehouse : warehouses)
    {
        for (const auto &truck : warehouse->getDockedTrucks())
        {
            truck->updateRoutePosition();

            auto newEnd = std::remove_if(visualOrders.begin(), visualOrders.end(), [&truck](QVariant a) { return truck->getCoordinate() == a.value<QGeoCoordinate>(); });
            int elementsDeleted = std::distance(newEnd, visualOrders.end());
            visualOrders.erase(newEnd, visualOrders.end());
            if (elementsDeleted > 0)
                simulationStats.addOrdersShipped(elementsDeleted);
        }
        // Assign new routes if trucks are available
        warehouse->assignRoutes();
        // Fulfill as many orders as possible
        warehouse->fullfillAllPossibleOrders();
    }

    if(simulationTime % 10 == 0) // Generate a new order
    {
        generateOrder();
        assignOrdersToWarehouses();
    }

    if(simulationTime % 300 == 0) // Deliver orders
    {
        shipOrders();
    }

    emit trucksChanged();
    emit ordersChanged();
}
