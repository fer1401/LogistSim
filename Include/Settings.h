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
    explicit Settings(Simulation *s, QWidget *parent = nullptr);
    ~Settings();

private slots:
    void on_addProduct_clicked();//para agregar un producto
    void on_deleteProduct_clicked();//para eliminar un producto

private:
    Ui::Settings *ui;
    QQuickWidget *mapQuickWidget;
    Simulation *simulation;

    // modelo de datos para QTableView
    QStandardItemModel *productModel;
    QStandardItemModel *warehousesModel;

    // Función para encapsular la lógica de configuración del mapa
    void setupMapTab();
    void setupProductCatalogView(); // Función para cargar los productos desde la simulacion
    void loadWarehouses();
    void loadProductCatalog(const QString &filePath);// Función para cargar JSON
};

#endif // SETTINGS_H
