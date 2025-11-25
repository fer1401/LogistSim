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



class Simulation : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<QObject*> warehouses READ getVisualWarehouses NOTIFY warehousesChanged FINAL)

    Q_PROPERTY(QList<QObject*> trucks READ getVisualTrucks NOTIFY trucksChanged FINAL)

    Q_PROPERTY(QVariantList orders READ getVisualOrders NOTIFY ordersChanged FINAL)

public:
    explicit Simulation(QObject *parent = nullptr);
    ~Simulation();
    void generateOrder();
    void run();
    QTimer *getSimulationClock();
    void startClock();
    void stopClock();
    QList<QObject*> getVisualWarehouses();
    QList<QObject*> getVisualTrucks();
    QVariantList getVisualOrders();

private slots:

    void simulationTick();

signals:

    void warehousesChanged();
    void trucksChanged();
    void ordersChanged();


private:
    City city;
    QList<Warehouse*> warehouses;
    QList<QObject*> visualTrucks;
    std::vector<Order> incomingOrders;
    std::vector<Product> productCatalog;
    QTimer *simulationClock;
    QVariantList visualOrders;

    Stats simulationStats;
};

#endif // SIMULATION_H
