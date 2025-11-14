#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <vector>
#include "Inventory.h"
#include "Order.h"

class Warehouse
{
private:
    int totalEmployees;
    int busyEmployees;
    Inventory inventory;
    std::vector<Order> pendingOrders;
    std::vector<Order> readyToShipOrders;

public:
    Warehouse(int totalEmployees, const Inventory& initialInventory);

    int getTotalEmployees() const;
    int getBusyEmployees() const;
    int getAvailableEmployees() const;
    const std::vector<Order>& getPendingOrders() const;
    const std::vector<Order>& getReadyToShipOrders() const;
    const Inventory& getInventory() const;

    void addOrder(const Order& order);
    bool fulfillNextOrder();
    void shipNextOrder();
};

#endif // WAREHOUSE_H