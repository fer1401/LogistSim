#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include "Simulation.h"
#include <QObject>
#include <QList>
#include <QQuickItem>
#include <QtQuickWidgets/QQuickWidget>
#include <QStandardItemModel> // Para el modelo de la tabla
#include <QJsonDocument>      // Para manejar el JSON
#include <QJsonObject>        // Para manejar objetos dentro del JSON
#include <QJsonArray>         // Para manejar la lista de productos

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

    // modelo de datos para QTableView
    QStandardItemModel *productModel;

    // Función para encapsular la lógica de configuración del mapa
    void setupMapTab();
    void loadProductCatalog(const QString &filePath); // Función para cargar JSON
};

#endif // SETTINGS_H
