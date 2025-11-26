#ifndef TRUCK_H
#define TRUCK_H

#include <QObject>
#include <QGeoCoordinate>
#include <QGeoRoute>
#include "City.h"
#include "array.hpp"

class Truck : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QGeoCoordinate coordinate READ getCoordinate WRITE setCoordinate NOTIFY coordinateChanged FINAL)
    Q_PROPERTY(QGeoRoute visualPath READ getVisualPath WRITE setVisualPath NOTIFY visualPathChanged FINAL)
    Q_PROPERTY(QString color READ getColor WRITE setColor NOTIFY colorChanged);

public:
    explicit Truck(int id, QGeoCoordinate initialCoord, QString truckColor, QObject *parent = nullptr);

    QGeoCoordinate getCoordinate();
    QGeoRoute getVisualPath();

    int getId() const;
    bool isShipping() const;
    Designar::DynArray<MapPoint> getAssignedRoute() const;

    void setId(int newId);
    void assignRoute(const Designar::Path<CityGraph> &newRoute);
    void clearRoute();
    void updateRoutePosition();
    void setColor(QString truckColor);

    QString getColor();

    // Función que la simulación en C++ llamará para mover el camión
    Q_INVOKABLE void updatePosition(double longitude, double latitude);

public slots:
    void setCoordinate(const QGeoCoordinate &newCoordinate);
    void setVisualPath(const QGeoRoute &newVisualPath);

signals:
    // 2. Señal emitida cada vez que la posición cambie.
    void coordinateChanged();
    void visualPathChanged();
    void colorChanged();

private:
    QGeoCoordinate m_coordinate;
    int id;
    bool shippingState = false;
    Designar::DynArray<MapPoint> assignedRoute;
    Designar::nat_t routePosition;
    QList<QGeoCoordinate> visualPath;
    QString color;
};

#endif // TRUCK_H
