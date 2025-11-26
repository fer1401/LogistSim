#include "warehouseedit.h"
#include "ui_warehouseedit.h"
#include <QDebug>

warehouseEdit::warehouseEdit(Warehouse *warehouseToEdit, Simulation *sim, QWidget *parent)
    : QWidget(parent), ui(new Ui::warehouseEdit), simulation(sim), currentWarehouse(warehouseToEdit)
{
    ui->setupUi(this);

    inventoryModel = new QStandardItemModel(this);
    ui->tableView->setModel(inventoryModel);

    QString trucksString;

    trucksString.append(ui->trucksLabel->text());
    trucksString.append(QString::number(currentWarehouse->getVisualTrucks().size()));

    ui->trucksLabel->setText(trucksString);

    QString employeeString;

    employeeString.append(ui->employeeLabel->text());
    employeeString.append(QString::number(currentWarehouse->getTotalEmployees()));

    ui->employeeLabel->setText(employeeString);

    setupInventoryTable();

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

warehouseEdit::~warehouseEdit()
{
    delete ui;
}

void warehouseEdit::setupInventoryTable()
{
    inventoryModel->clear();

    QStringList headers = {"ID", "Nombre", "Descripción", "Stock"};
    inventoryModel->setHorizontalHeaderLabels(headers);
    inventoryModel->setColumnCount(headers.size());

    const auto& catalog = simulation->getProductCatalog();
    const Inventory& inventory = currentWarehouse->getInventory();

    for (const auto& product : catalog)
    {
        int productID = product.getId();
        int currentStock = inventory.getStockLevel(productID);

        QList<QStandardItem*> rowItems;

        QStandardItem *idItem = new QStandardItem(QString::number(productID));
        idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);

        QStandardItem *nameItem = new QStandardItem(QString::fromStdString(product.getName()));
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);

        QStandardItem *descItem = new QStandardItem(QString::fromStdString(product.getDescription()));
        descItem->setFlags(descItem->flags() & ~Qt::ItemIsEditable);

        QStandardItem *stockItem = new QStandardItem(QString::number(currentStock));

        stockItem->setFlags(stockItem->flags() | Qt::ItemIsEditable);

        stockItem->setData(productID, Qt::UserRole + 1);

        rowItems << idItem << nameItem << descItem << stockItem;
        inventoryModel->appendRow(rowItems);
    }
}

void warehouseEdit::on_pushButton_clicked()
{
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, tr("Advertencia"), tr("Seleccione un producto para añadirle stock"));
        return;
    }

    QModelIndex selectedIndex = selectedRows.at(0);

    bool ok;

    int idToAdd = inventoryModel->data(selectedIndex.sibling(selectedIndex.row(), 0)).toInt(&ok);

    if (!ok) {
        QMessageBox::critical(this, tr("Error"), tr("No se pudo obtener el ID del producto."));
        return;
    }

    Inventory &inventory = currentWarehouse->getInventory();

    int quantity = QInputDialog::getInt(this,
                                  tr("Agregar stock"),
                                  tr("Ingrese la cantidad a agregar"),
                                  0, 1, 1000, 1, &ok);

    if (!ok) return;

    inventory.addStock(idToAdd, quantity);

    setupInventoryTable();
}

void warehouseEdit::on_pushButton_2_clicked()
{
    bool ok;

    int quantity = QInputDialog::getInt(this,
                                        tr("Cantidad de camiones"),
                                        tr("Ingrese la cantidad de camiones en el almacen"),
                                        0, 1, 20, 1, &ok);

    if (!ok) return;

    currentWarehouse->getDockedTrucks().clear();

    for (int i = 1; i <= quantity; ++i)
    {
        currentWarehouse->dockTruck(new Truck(i, currentWarehouse->getCoordinate(), "Blue", currentWarehouse));
    }

    QString trucksString;

    trucksString.append(QString::fromStdString("Cantidad de camiones: "));
    trucksString.append(QString::number(currentWarehouse->getVisualTrucks().size()));

    ui->trucksLabel->setText(trucksString);
}


void warehouseEdit::on_pushButton_3_clicked()
{
    bool ok;

    int quantity = QInputDialog::getInt(this,
                                        tr("Cantidad de empleados"),
                                        tr("Ingrese la cantidad de empleados en el almacen"),
                                        0, 1, 20, 1, &ok);

    if (!ok) return;

    currentWarehouse->setTotalEmployees(quantity);

    QString employeeString;

    employeeString.append(QString::fromStdString("Cantidad de empleados: "));
    employeeString.append(QString::number(currentWarehouse->getTotalEmployees()));

    ui->employeeLabel->setText(employeeString);
}

