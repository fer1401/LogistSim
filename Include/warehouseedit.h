#ifndef WAREHOUSEEDIT_H
#define WAREHOUSEEDIT_H

#include <QWidget>
#include <QWidget>
#include <QStandardItemModel>
#include "Simulation.h"
#include "Warehouse.h"
#include "Stockspinboxdelegate.h"

namespace Ui {
class warehouseEdit;
}

class warehouseEdit : public QWidget
{
    Q_OBJECT

public:
    // El constructor debe recibir el Almacén que se va a editar
    explicit warehouseEdit(Warehouse *warehouseToEdit, Simulation *sim, QWidget *parent = nullptr);
    ~warehouseEdit();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::warehouseEdit *ui;
    Simulation *simulation;
    Warehouse *currentWarehouse; // El almacén actual
    QStandardItemModel *inventoryModel; // Modelo para la tabla de inventario

    void setupInventoryTable(); // Función para poblar la tabla
};

#endif // WAREHOUSEEDIT_H
