#ifndef TRUCK_H
#define TRUCK_H

#include <QObject>
#include <QGeoCoordinate>

class Truck : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QGeoCoordinate coordinate READ getCoordinate WRITE setCoordinate NOTIFY coordinateChanged FINAL)

public:
    explicit Truck(int id, QObject *parent = nullptr);

    QGeoCoordinate getCoordinate();

    int getId() const;

    void setId(int newId);

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
};

#endif // TRUCK_H
