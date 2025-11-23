#include "Simulation.h"
#include "Inventory.h"

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
    // Process incoming orders
    for (auto& order : incomingOrders)
    {
        bool fulfilled = false;
        for (auto& warehouse : warehouses)
        {
            if (warehouse->getInventory().canFulfillOrder(order))
            {
                warehouse->addOrder(order);
                fulfilled = true;
                break;
            }
        }
        if (!fulfilled)
        {
            // Handle unfulfillable order (e.g., log it, notify user, etc.)
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
