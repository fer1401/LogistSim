#ifndef TRUCK_H
#define TRUCK_H

#include <QObject>
#include <QGeoCoordinate>
#include "City.h"
#include "array.hpp"

class Truck : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QGeoCoordinate coordinate READ getCoordinate WRITE setCoordinate NOTIFY coordinateChanged FINAL)

public:
    explicit Truck(int id, QGeoCoordinate initialCoord, QObject *parent = nullptr);

    QGeoCoordinate getCoordinate();

    int getId() const;
    bool isShipping() const;
    Designar::DynArray<MapPoint> getAssignedRoute() const;

    void setId(int newId);
    void assignRoute(const Designar::Path<CityGraph> &newRoute);
    void clearRoute();
    void updateRoutePosition();

    // Función que la simulación en C++ llamará para mover el camión
    Q_INVOKABLE void updatePosition(double longitude, double latitude);

public slots:
    void setCoordinate(const QGeoCoordinate &newCoordinate);

signals:
    // 2. Señal emitida cada vez que la posición cambie.
    void coordinateChanged();

private:
    QGeoCoordinate m_coordinate;
    int id;
    bool shippingState = false;
    Designar::DynArray<MapPoint> assignedRoute;
    Designar::nat_t routePosition;
};

#endif // TRUCK_H
