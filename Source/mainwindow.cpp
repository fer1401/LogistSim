#include "Include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QQuickWidget *map = ui->centralwidget->findChild<QQuickWidget*>("mapWidget");
    map->setSource(QUrl(QStringLiteral("qrc:/MapQuickWidget.qml")));
}

MainWindow::~MainWindow()
{
    delete ui;
}
