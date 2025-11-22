#ifndef SIMULATION_H
#define SIMULATION_H

#include <City.h>
#include <Order.h>
#include <Product.h>
#include <Warehouse.h>
#include <memory>
#include <QTimer>
#include <QObject>

class Simulation : public QObject
{
    Q_OBJECT

public:
    explicit Simulation(QObject *parent = nullptr);
    ~Simulation();
    void generateOrder();
    void run();
    QTimer *getSimulationClock();
    void startClock();
    void stopClock();

private slots:

    void simulationTick();

private:
    City city;
    std::vector<std::unique_ptr<Warehouse>> warehouses;
    std::vector<Order> incomingOrders;
    std::vector<Product> productCatalog;
    QTimer *simulationClock;
};

#endif // SIMULATION_H
