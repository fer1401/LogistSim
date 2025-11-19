#ifndef SIMULATION_H
#define SIMULATION_H

#include <City.h>
#include <Order.h>
#include <Product.h>
#include <Warehouse.h>
#include <memory>

class Simulation
{
public:
    Simulation();
    void generateOrder();
    void run();

private:
    City city;
    std::vector<std::unique_ptr<Warehouse>> warehouses;
    std::vector<Order> incomingOrders;
    std::vector<Product> productCatalog;
};

#endif // SIMULATION_H