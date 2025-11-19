#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <QObject>
#include <QGeoCoordinate>
#include <vector>
#include "Inventory.h"
#include "Order.h"

class Warehouse : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QGeoCoordinate coordinate READ getCoordinate CONSTANT)
private:
    int totalEmployees;
    int busyEmployees;
    Inventory inventory;
    std::vector<Order> pendingOrders;
    std::vector<Order> readyToShipOrders;

    QGeoCoordinate m_coordinate;

public:
    explicit Warehouse(double latitude, double longitude, int totalEmployees, const Inventory& initialInventory, QObject *parent = nullptr);

    QGeoCoordinate getCoordinate() const;

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
