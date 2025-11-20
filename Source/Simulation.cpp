#include "Simulation.h"
#include "Inventory.h"

Simulation::Simulation()
{
    // Initialize city and warehouses
    city = City();

    Inventory initialInventory;
    initialInventory.addStock(1, 50);
    initialInventory.addStock(2, 100);
    initialInventory.addStock(3, 75);

    warehouses.push_back(std::make_unique<Warehouse>(8.60000, -71.16500, 10, initialInventory, nullptr));
    warehouses.push_back(std::make_unique<Warehouse>(8.58000, -71.14000, 5, initialInventory, nullptr));
    
    // Initialize products
    productCatalog.emplace_back(Product(1, "Laptop", "HP Spectre x360"));
    productCatalog.emplace_back(Product(2, "Smartphone", "Samsung Galaxy S21"));
    productCatalog.emplace_back(Product(3, "Tablet", "iPad Pro"));
}

void Simulation::generateOrder()
{
    Order newOrder("Client_" + std::to_string(incomingOrders.size() + 1), 
                   8.59000 + static_cast<double>(rand()) / RAND_MAX * 0.02, 
                   -71.15000 + static_cast<double>(rand()) / RAND_MAX * 0.02);
    
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
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
        while (warehouse->getAvailableEmployees() > 0 && !warehouse->getPendingOrders().empty())
        {
            warehouse->fulfillNextOrder();
        }
        while (!warehouse->getReadyToShipOrders().empty())
        {
            warehouse->shipNextOrder();
        }
    }
}