#include "Simulation.h"
#include "Inventory.h"
#include "graphalgorithms.hpp"
#include <limits>

const int simulationClockInterval = 200;

Simulation::Simulation(QObject *parent) : QObject{parent}
{
    // Initialize city and warehouses
    city = City();

    Inventory initialInventory;
    initialInventory.addStock(1, 50);
    initialInventory.addStock(2, 100);
    initialInventory.addStock(3, 75);

    Warehouse *warehouseA = new Warehouse(8.571765, -71.179717, 10, initialInventory, this);
    Warehouse *warehouseB = new Warehouse(8.5962673, -71.1518601, 5, initialInventory, this);

    warehouses.append(warehouseA);
    warehouses.append(warehouseB);
    
    // Initialize products
    productCatalog.emplace_back(Product(1, "Laptop", "HP Spectre x360"));
    productCatalog.emplace_back(Product(2, "Smartphone", "Samsung Galaxy S21"));
    productCatalog.emplace_back(Product(3, "Tablet", "iPad Pro"));

    simulationClock = new QTimer(this);
    QTimer::connect(simulationClock, SIGNAL(timeout()), this, SLOT(simulationTick()));
    simulationClock->setInterval(simulationClockInterval);

    for (int i = 0; i < 20; ++i)
    {
        generateOrder();
    }

    run();

    emit warehousesChanged();
}

Simulation::~Simulation()
{
    qDeleteAll(warehouses);
    delete simulationClock;
}

void Simulation::generateOrder()
{
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()

    std::uniform_real_distribution<> latDist(8.56837, 8.61000);
    std::uniform_real_distribution<> lonDist(-71.16609, -71.12145);

    Order newOrder("Client_" + std::to_string(incomingOrders.size() + 1), 
                   latDist(gen), 
                   lonDist(gen));
    
    std::uniform_int_distribution<> productSelect(1, productCatalog.size());
    std::uniform_int_distribution<> numProducts(1, 2);
    std::uniform_int_distribution<> quantity(1, 10);
    
    int numberOfProducts = numProducts(gen);
    for (int i = 0; i < numberOfProducts; ++i)
    {
        int productIndex = productSelect(gen) - 1; // Adjust for 0-based index
        int qty = quantity(gen);
        newOrder.addProduct(productCatalog[productIndex], qty);
    }

    incomingOrders.push_back(newOrder);
}

void Simulation::run()
{
    // A* solver instance
    Designar::Astar<CityGraph, CityEdgeDistance, CityHeuristic> astar_solver;
    // Process incoming orders
    for (auto& order : incomingOrders)
    {
        CityGraph::Node *customerNode = find_nearest_node(city.getGraph(), order.getLatitude(), order.getLongitude());
        
        // Variables to track the best (closest) fulfilling warehouse found so far
        Warehouse* bestWarehouse = nullptr;
        double minDistance = std::numeric_limits<double>::max(); // Initialize with maximum possible value
    
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
                double currentDistance = path_distance(p_w_c) + path_distance(p_c_w);
                // Check if this warehouse is closer than the current best
                if (currentDistance < minDistance)
                {
                    minDistance = currentDistance;
                    bestWarehouse = warehouse;
                }
                
                
            }

            
        }

        if (bestWarehouse != nullptr)
        {
            bestWarehouse->addOrder(order);
        }
    }
    incomingOrders.clear();

    // Update warehouses
    for (auto& warehouse : warehouses)
    {
        warehouse->fullfillAllPossibleOrders();
        warehouse->shipOrders(city.getGraph());
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

void Simulation::simulationTick()
{
    for (const auto &warehouse : warehouses)
    {
        for (const auto &truck : warehouse->getDockedTrucks())
        {
            truck->updateRoutePosition();
        }
    }

    emit trucksChanged();
}
