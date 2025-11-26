#include "Include/mainwindow.h"
#include "ui_mainwindow.h"
#include <QQmlContext>
#include <graphalgorithms.hpp>

MainWindow::MainWindow(Simulation *sim, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), simulation(sim)
{
    ui->setupUi(this);
    QQuickWidget *map = ui->centralwidget->findChild<QQuickWidget*>("mapWidget");

    map->rootContext()->setContextProperty("simulation", simulation);
    map->setSource(QUrl(QStringLiteral("qrc:/MapQuickWidget.qml")));

    simulation->startClock();
}

MainWindow::~MainWindow()
{
    delete simulation;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
}

