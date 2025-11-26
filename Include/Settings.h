#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include "Simulation.h"
#include <QObject>
#include <QList>
#include <QQuickItem>
#include <QtQuickWidgets/QQuickWidget>
#include <QStandardItemModel>
#include "warehouseedit.h"

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
    void on_addProduct_clicked();
    void on_deleteProduct_clicked();
    void on_editButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::Settings *ui;
    QQuickWidget *mapQuickWidget;
    Simulation *simulation;
    QStandardItemModel *productModel;
    QStandardItemModel *warehousesModel;

    void setupMapTab();
    void setupProductCatalogView();
    void loadWarehouses();
};

#endif // SETTINGS_H
