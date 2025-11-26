#include "Simulation.h"
#include "Inventory.h"
#include "graphalgorithms.hpp"
#include <limits>

const int simulationClockInterval = 200;

Simulation::Simulation(QObject *parent) : QObject{parent}, city{City()}
{
    
    // Initialize products
    productCatalog.emplace_back(Product(1, "Laptop", "HP Spectre x360", 1));
    productCatalog.emplace_back(Product(2, "Smartphone", "Samsung Galaxy S21", 3));
    productCatalog.emplace_back(Product(3, "Tablet", "iPad Pro", 5));
    productCatalog.emplace_back(Product(4, "Headphones", "Sony WH-1000XM5 Noise Cancelling", 2));
    productCatalog.emplace_back(Product(5, "Smart Watch", "Apple Watch Series 8", 4));
    productCatalog.emplace_back(Product(6, "Gaming Console", "PlayStation 5 Digital Edition", 5));
    productCatalog.emplace_back(Product(7, "E-reader", "Kindle Paperwhite (Latest Gen)", 1));
    productCatalog.emplace_back(Product(8, "Coffee Maker", "Breville Barista Express Espresso Machine", 3));
    productCatalog.emplace_back(Product(9, "Book", "'The Midnight Library' by Matt Haig", 2));
    productCatalog.emplace_back(Product(10, "Robot Vacuum", "iRobot Roomba j7+ Self-Emptying", 4));
    
    Inventory initialInventory;
    initialInventory.addStock(1, 2500);
    initialInventory.addStock(2, 3000);
    initialInventory.addStock(3, 4750);
    initialInventory.addStock(4, 3800);
    initialInventory.addStock(5, 5100);
    initialInventory.addStock(6, 2050);
    initialInventory.addStock(7, 7500);
    initialInventory.addStock(8, 1900);
    initialInventory.addStock(9, 12000);
    initialInventory.addStock(10, 4500);

    Warehouse *warehouseA = new Warehouse(8.571765, -71.179717, 25, 3, "Orange", initialInventory, this);
    Warehouse *warehouseB = new Warehouse(8.5962673, -71.1518601, 15, 2, "Purple", initialInventory, this);

    warehouses.append(warehouseA);
    warehouses.append(warehouseB);

    simulationClock = new QTimer(this);
    QTimer::connect(simulationClock, SIGNAL(timeout()), this, SLOT(simulationTick()));
    simulationClock->setInterval(simulationClockInterval);

    for (int i = 0; i < 150; ++i)
    {
        generateOrder();
    }
    assignOrdersToWarehouses();
    shipOrders();

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
        int productIndex = rng.selectProduct(productCatalog) + 1; // Adjust for 0-based index
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

    if(simulationTime >= nextOrderTime) // Generate a new order
    {
        generateOrder();
        assignOrdersToWarehouses();
        nextOrderTime += rng.randomOrderInterval();
    }

    if(simulationTime % 200 == 0) // Deliver orders
    {
        shipOrders();
    }
    
    simulationTime++;
    simulationStats.print(std::cout);

    emit trucksChanged();
    emit ordersChanged();
}
