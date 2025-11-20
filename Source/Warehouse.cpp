#include "Warehouse.h"
#include <iostream>
#include <algorithm> // For std::remove_if or vector manipulation

Warehouse::Warehouse(double latitude, double longitude, int totalEmployees, const Inventory& initialInventory, QObject *parent)
    : QObject{parent}, m_coordinate(latitude, longitude), totalEmployees(totalEmployees), busyEmployees(0), inventory(initialInventory) {}

QGeoCoordinate Warehouse::getCoordinate() const
{
    return m_coordinate;
}

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

void Warehouse::dockTruck(std::unique_ptr<Truck> truck) {
    dockedTrucks.push_back(std::move(truck));
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
