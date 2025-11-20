#include "Truck.h"

Truck::Truck(int id, QObject *parent)
    : QObject{parent},
    m_coordinate(8.5945770, -71.1598334), // Coordenada inicial
    id(id)
{}

QGeoCoordinate Truck::getCoordinate()
{
    return m_coordinate;
}

int Truck::getId() const
{
    return id;
}

Designar::Path<CityGraph> Truck::getAssignedRoute() const
{
    return assignedRoute;
}

bool Truck::isShipping() const
{
    return shippingState;
}

void Truck::assignRoute(const Designar::Path<CityGraph> &newRoute)
{
    assignedRoute = newRoute;
    shippingState = true;
}

void Truck::clearRoute()
{
    assignedRoute = Designar::Path<CityGraph>();
    shippingState = false;
}

void Truck::setId(int newId)
{
    id = newId;
}

void Truck::setCoordinate(const QGeoCoordinate &newCoordinate)
{
    if (m_coordinate == newCoordinate)
    {
        return;
    }

    m_coordinate = newCoordinate;
    // Emitir la señal para notificar a QML
    emit coordinateChanged();
}

void Truck::updatePosition(double longitude, double latitude)
{
    QGeoCoordinate newCoord(latitude, longitude);
    setCoordinate(newCoord);
}
