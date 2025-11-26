#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <QObject>
#include <QGeoCoordinate>
#include <vector>
#include <queue>
#include <tuple> // NEW: For std::tuple
#include "Inventory.h"
#include "Order.h"
#include "Truck.h"
#include "City.h"

class QTimer; // Forward declaration for QTimer

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
    std::vector<std::tuple<Order, int>> ordersInProgress; 
    
    QList<Truck*> dockedTrucks;
    std::queue<Designar::Path<CityGraph>> truckRoutes;

    QGeoCoordinate m_coordinate;
    QTimer *processingTimer = nullptr;

private slots:
    void advanceProcessing();

signals:
    void trucksChanged();

public:
    explicit Warehouse(double latitude, double longitude, int totalEmployees, int numTrucks, const Inventory& initialInventory, QObject *parent = nullptr);
    ~Warehouse();

    QGeoCoordinate getCoordinate() const;
    QList<QObject*> getVisualTrucks() const;

    int getTotalEmployees() const;
    int getBusyEmployees() const;
    int getAvailableEmployees() const;
    const QList<Truck*> getDockedTrucks();
    const std::vector<Order>& getPendingOrders() const;
    const std::vector<Order>& getReadyToShipOrders() const;
    void dockTruck(Truck *truck);
    const Inventory& getInventory() const;

    float getCurrentLoad() const;

    void addOrder(const Order& order);
    bool fulfillNextOrder();
    void fullfillAllPossibleOrders();
    std::pair<int, float> shipOrders(CityGraph& city);
    std::pair<int, float> assignRoutes();
    std::vector<Designar::Path<CityGraph>> planTruckRoutes(CityGraph& city);
};

#endif // WAREHOUSE_H