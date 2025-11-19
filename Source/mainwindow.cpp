#include "Include/mainwindow.h"
#include "ui_mainwindow.h"
#include <QQmlContext>
#include <graphalgorithms.hpp>
#include "Inventory.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    exampleTruck = new Truck(this);
    ui->setupUi(this);
    QQuickWidget *map = ui->centralwidget->findChild<QQuickWidget*>("mapWidget");
    map->rootContext()->setContextProperty("exampleTruck", exampleTruck);

    Inventory initialInventory;

    warehouseA = new Warehouse(8.60000, -71.16500, 10, initialInventory, this);

    warehouseB = new Warehouse(8.58000, -71.14000, 5, initialInventory, this);

    map->rootContext()->setContextProperty("warehouseA", warehouseA);
    map->rootContext()->setContextProperty("warehouseB", warehouseB);

    map->setSource(QUrl(QStringLiteral("qrc:/MapQuickWidget.qml")));
    city = City();

    auto src = city.getGraph().search_node([&](auto node){ return (node->get_info().getId() == 693979862);});

    examplePath.insert(src->get_info());

    for (int i = 0; i < 26; i++)
    {
        auto adjacent_arcs = city.getGraph().adjacent_arcs(src);

        examplePath.append(adjacent_arcs.get_first()->get_tgt_node()->get_info());

        src = adjacent_arcs.get_first()->get_tgt_node();
    }

    pathPosition = 0;
}

MainWindow::~MainWindow()
{
    delete exampleTruck;
    delete warehouseA;
    delete warehouseB;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(examplePath.is_empty())
    {
        return;
    }

    if ((pathPosition + 1) == examplePath.size())
    {
        return;
    }

    exampleTruck->updatePosition(examplePath.at(pathPosition).getLongitude(), examplePath.at(pathPosition).getLatitude());
    pathPosition++;
}

