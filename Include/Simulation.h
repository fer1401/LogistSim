#ifndef SIMULATION_H
#define SIMULATION_H

#include <City.h>
#include <Order.h>
#include <Product.h>
#include <Warehouse.h>
#include <QTimer>
#include <QObject>
#include <QList>
#include <QQuickItem>
#include <algorithm>
#include <QInputDialog>
#include <QMessageBox>

class Simulation : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<QObject*> warehouses READ getVisualWarehouses NOTIFY warehousesChanged FINAL)

    Q_PROPERTY(QList<QObject*> trucks READ getVisualTrucks NOTIFY trucksChanged FINAL)

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
    Q_INVOKABLE bool addNewWarehouse(double latitude, double longitude);
    const std::vector<Product>& getProductCatalog() const { return productCatalog; } //get para obtener el catalogo de profuctos
    bool addNewProduct(int id, const QString &name, const QString &description);
    bool deleteProduct(int id);
    QList<Warehouse*> getWarehouses();

private slots:

    void simulationTick();

signals:

    void warehousesChanged();
    void trucksChanged();
    void warehouseLimitReached();

private:
    City city;
    QList<Warehouse*> warehouses;
    const int MAX_WAREHOUSES = 10;
    QList<QObject*> visualTrucks;
    std::vector<Order> incomingOrders;
    std::vector<Product> productCatalog;
    QTimer *simulationClock;
};

#endif // SIMULATION_H
