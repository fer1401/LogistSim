#include "Truck.h"

Truck::Truck(int id, QGeoCoordinate initialCoord, QObject *parent)
    : QObject{parent},
    m_coordinate(initialCoord), // Coordenada inicial
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

Designar::DynArray<MapPoint> Truck::getAssignedRoute() const
{
    return assignedRoute;
}

bool Truck::isShipping() const
{
    return shippingState;
}

void Truck::assignRoute(const Designar::Path<CityGraph> &newRoute)
{
    for (const auto &node : newRoute.nodes())
    {
        assignedRoute.append(node->get_info());
    }

    routePosition = 0;

    shippingState = true;
}

void Truck::clearRoute()
{
    assignedRoute.clear();
    routePosition = 0;

    shippingState = false;
}

void Truck::updateRoutePosition()
{
    if (assignedRoute.is_empty())
    {
        return;
    }

    if (routePosition == assignedRoute.size())
    {
        clearRoute();
        return;
    }

    updatePosition(assignedRoute.at(routePosition).getLongitude(), assignedRoute.at(routePosition).getLatitude());
    routePosition++;
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
