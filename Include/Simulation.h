#ifndef SIMULATION_H
#define SIMULATION_H

#include <City.h>
#include <Order.h>
#include <Product.h>
#include <Warehouse.h>
#include <QTimer>
#include <QObject>
#include <QList>

struct Stats
{
    int totalOrdersReceived = 0;
    int totalOrdersFulfilled = 0;
    int totalOrdersShipped = 0;

    double totalDistanceTraveled = 0.0;
    int totalTripsMade = 0;
};

inline void printStats(const Stats &stats)
{
    std::cout << "Simulation Statistics:\n";
    std::cout << "Total Orders Received: " << stats.totalOrdersReceived << "\n";
    std::cout << "Total Orders Fulfilled: " << stats.totalOrdersFulfilled << "\n";
    std::cout << "Total Orders Shipped: " << stats.totalOrdersShipped << "\n";
    std::cout << "Average Fulfillment Rate: "
              << (stats.totalOrdersReceived > 0 ? (static_cast<double>(stats.totalOrdersFulfilled) / stats.totalOrdersReceived) * 100.0 : 0.0)
              << "%\n";
    std::cout << "Shipped Package %: "
              << (stats.totalOrdersFulfilled > 0 ? (static_cast<double>(stats.totalOrdersShipped) / stats.totalOrdersFulfilled) * 100.0 : 0.0)
              << "%\n";
    std::cout << "-----------------------------------\n";
    std::cout << "Total Distance Traveled: " << stats.totalDistanceTraveled << " units\n";
    std::cout << "Total Trips Made: " << stats.totalTripsMade << "\n";
    std::cout << "Average Distance per Trip: "
              << (stats.totalTripsMade > 0 ? stats.totalDistanceTraveled / stats.totalTripsMade : 0.0)
              << " units\n";
}

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
