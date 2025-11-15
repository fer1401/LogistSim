#include "Truck.h"

Truck::Truck(QObject *parent)
    : QObject{parent},
    m_coordinate(8.5945770,-71.1598334)// Coordenada inicial
{}

QGeoCoordinate Truck::getCoordinate()
{
    return m_coordinate;
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
    QGeoCoordinate newCoord(longitude, latitude);
    setCoordinate(newCoord);
}
