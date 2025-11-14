#include "Warehouse.h"
#include <iostream>
#include <algorithm> // For std::remove_if or vector manipulation

Warehouse::Warehouse(int totalEmployees, const Inventory& initialInventory)
    : totalEmployees(totalEmployees), busyEmployees(0), inventory(initialInventory) {}

int Warehouse::getTotalEmployees() const {
    return totalEmployees;
}

int Warehouse::getBusyEmployees() const {
    return busyEmployees;
}

int Warehouse::getAvailableEmployees() const {
    return totalEmployees - busyEmployees;
}

const std::vector<Order>& Warehouse::getPendingOrders() const {
    return pendingOrders;
}

const std::vector<Order>& Warehouse::getReadyToShipOrders() const {
    return readyToShipOrders;
}

const Inventory& Warehouse::getInventory() const {
    return inventory;
}

void Warehouse::addOrder(const Order& order) {
    pendingOrders.push_back(order);
}

bool Warehouse::fulfillNextOrder() {
    if (getAvailableEmployees() <= 0) {
        return false;
    }

    if (pendingOrders.empty()) {
        return false;
    }

    Order& nextOrder = pendingOrders.front();

    if (inventory.canFulfillOrder(nextOrder)) {
        busyEmployees++;
        
        for (const auto& pair : nextOrder.getProductQuantities()) {
            inventory.removeStock(pair.first, pair.second);
        }

        readyToShipOrders.push_back(std::move(nextOrder));
        
        pendingOrders.erase(pendingOrders.begin());
        
        busyEmployees--;

        return true;
    }
    else {
        return false;
    }
}

void Warehouse::shipNextOrder() {
    if (!readyToShipOrders.empty()) {
        readyToShipOrders.erase(readyToShipOrders.begin());
    }
}