#ifndef SIMULATION_H
#define SIMULATION_H

#include <City.h>
#include <Order.h>
#include <Product.h>
#include <Warehouse.h>
#include <Stats.h>
#include <QTimer>
#include <QObject>
#include <QList>
#include <QQuickItem>
#include <algorithm>
#include <QInputDialog>
#include <QMessageBox>
#include "RandomGenerator.h"



class Simulation : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<QObject*> warehouses READ getVisualWarehouses NOTIFY warehousesChanged FINAL)

    Q_PROPERTY(QList<QObject*> trucks READ getVisualTrucks NOTIFY trucksChanged FINAL)

    Q_PROPERTY(QGeoCoordinate newCoordinate READ getNewCoordinate WRITE setNewCoordinate NOTIFY newCoordinateChanged FINAL)
    Q_PROPERTY(QVariantList orders READ getVisualOrders NOTIFY ordersChanged FINAL)

public:
    explicit Simulation(QObject *parent = nullptr);
    ~Simulation();
    void generateOrder();
    void assignOrdersToWarehouses();
    void shipOrders();
    QTimer *getSimulationClock();
    void startClock();
    void stopClock();
    QList<QObject*> getVisualWarehouses();
    QList<QObject*> getVisualTrucks();
    Q_INVOKABLE bool addNewWarehouse(double latitude, double longitude);
    Q_INVOKABLE void setNewCoord(const QGeoCoordinate &coord);
    const std::vector<Product>& getProductCatalog() const { return productCatalog; } //get para obtener el catalogo de profuctos
    bool addNewProduct(int id, const QString &name, const QString &description);
    bool deleteProduct(int id);
    QList<Warehouse*> getWarehouses();
    QGeoCoordinate getNewCoordinate();
    void setNewCoordinate(QGeoCoordinate coord);
    QVariantList getVisualOrders();

private slots:

    void simulationTick();

signals:

    void warehousesChanged();
    void trucksChanged();
    void warehouseLimitReached();
    void newCoordinateChanged();
    void ordersChanged();


private:
    City city;
    QList<Warehouse*> warehouses;
    const int MAX_WAREHOUSES = 10;
    RandomGenerator rng;
    QList<QObject*> visualTrucks;
    std::vector<Order> incomingOrders;
    std::vector<Product> productCatalog;
    QTimer *simulationClock;
    QGeoCoordinate newCoordinate;
    QVariantList visualOrders;

    Stats simulationStats;
    int simulationTime = 0;
    int nextOrderTime = 0;
};

#endif // SIMULATION_H
