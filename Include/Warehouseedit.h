// WarehouseEdit.h
#ifndef WAREHOUSEEDIT_H
#define WAREHOUSEEDIT_H

#include <QWidget>
#include <QStandardItemModel>
#include "Simulation.h"
#include "Warehouse.h"
#include "Stockspinboxdelegate.h"

namespace Ui { class Warehouseedit; }

class Warehouseedit : public QWidget
{
    Q_OBJECT

public:
    // El constructor debe recibir el Almacén que se va a editar
    explicit Warehouseedit(Warehouse *warehouseToEdit, Simulation *sim, QWidget *parent = nullptr);
    ~Warehouseedit();

private:
    Ui::Warehouseedit *ui;
    Simulation *simulation;
    Warehouse *currentWarehouse; // El almacén actual
    QStandardItemModel *inventoryModel; // Modelo para la tabla de inventario

    void setupInventoryTable(); // Función para poblar la tabla
};

#endif // WAREHOUSEEDIT_H
