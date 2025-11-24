#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include "Simulation.h"
#include <QObject>
#include <QList>
#include <QQuickItem>
#include <QtQuickWidgets/QQuickWidget>

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private:
    Ui::Settings *ui;
    QQuickWidget *mapQuickWidget;

    // Función para encapsular la lógica de configuración del mapa
    void setupMapTab();
};

#endif // SETTINGS_H
