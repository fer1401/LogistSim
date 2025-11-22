#include "Include/mainwindow.h"
#include "ui_mainwindow.h"
#include <QQmlContext>
#include <graphalgorithms.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), simulation(Simulation(this))
{
    ui->setupUi(this);
    QQuickWidget *map = ui->centralwidget->findChild<QQuickWidget*>("mapWidget");

    map->setSource(QUrl(QStringLiteral("qrc:/MapQuickWidget.qml")));

    simulation.startClock();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
}

