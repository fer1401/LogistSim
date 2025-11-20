#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <QObject>
#include <QGeoCoordinate>
#include <vector>
#include <queue>
#include "Inventory.h"
#include "Order.h"
#include "Truck.h"
#include "City.h"

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
    std::vector<std::unique_ptr<Truck>> dockedTrucks;
    std::queue<Designar::Path<CityGraph>> truckRoutes;

    QGeoCoordinate m_coordinate;

public:
    explicit Warehouse(double latitude, double longitude, int totalEmployees, const Inventory& initialInventory, QObject *parent = nullptr);

    QGeoCoordinate getCoordinate() const;

    int getTotalEmployees() const;
    int getBusyEmployees() const;
    int getAvailableEmployees() const;
    const std::vector<std::unique_ptr<Truck>>& getDockedTrucks();
    const std::vector<Order>& getPendingOrders() const;
    const std::vector<Order>& getReadyToShipOrders() const;
    void dockTruck(std::unique_ptr<Truck> truck);
    const Inventory& getInventory() const;

    void addOrder(const Order& order);
    bool fulfillNextOrder();
    void shipOrders(CityGraph& city);
    void assignRoutes();
    std::vector<Designar::Path<CityGraph>> planTruckRoutes(CityGraph& city);
};

#endif // WAREHOUSE_H
