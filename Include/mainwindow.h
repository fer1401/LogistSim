#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "City.h"
#include "Truck.h"
#include "array.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    City city;
    Truck *exampleTruck;
    Designar::DynArray<MapPoint> examplePath;
    Designar::nat_t pathPosition;
};
#endif // MAINWINDOW_H
