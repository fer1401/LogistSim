#include "Warehouseedit.h"
#include "ui_Warehouseedit.h"
#include <QDebug>

Warehouseedit::Warehouseedit(Warehouse *warehouseToEdit, Simulation *sim, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Warehouseedit), simulation(sim), currentWarehouse(warehouseToEdit)
{
    ui->setupUi(this);

    // Inicializar el modelo y la tabla
    inventoryModel = new QStandardItemModel(this);
    ui->inventoryTableView->setModel(inventoryModel);

    setupInventoryTable();

    ui->tableView->setItemDelegateForColumn(3, new StockspinboxDelegate(this));

    // Configuración visual
    ui->inventoryTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->inventoryTableView->verticalHeader()->hide();
    ui->inventoryTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void Warehouseedit::setupInventoryTable()
{
    inventoryModel->clear();

    // 1. Establecer las cabeceras
    QStringList headers = {"ID", "Nombre", "Descripción", "Stock"};
    inventoryModel->setHorizontalHeaderLabels(headers);
    inventoryModel->setColumnCount(headers.size());

    // Referencias
    const auto& catalog = simulation->getProductCatalog();
    const Inventory& inventory = currentWarehouse->getInventory();

    // 2. Llenar la tabla
    for (const auto& product : catalog)
    {
        int productID = product.getId();
        int currentStock = inventory.getStock(productID);

        QList<QStandardItem*> rowItems;

        // Columna 0: ID (No editable)
        QStandardItem *idItem = new QStandardItem(QString::number(productID));
        idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);

        // Columna 1: Nombre (No editable)
        QStandardItem *nameItem = new QStandardItem(QString::fromStdString(product.getName()));
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);

        // Columna 2: Descripción (No editable)
        QStandardItem *descItem = new QStandardItem(QString::fromStdString(product.getDescription()));
        descItem->setFlags(descItem->flags() & ~Qt::ItemIsEditable);

        // Columna 3: Stock (Editable por el delegado)
        QStandardItem *stockItem = new QStandardItem(QString::number(currentStock));
        // ⭐ Clave: Hacer que sea editable para que el delegado funcione
        stockItem->setFlags(stockItem->flags() | Qt::ItemIsEditable);

        // Guardar el ID del producto en el rol de datos para saber qué producto editar
        stockItem->setData(productID, Qt::UserRole + 1);

        rowItems << idItem << nameItem << descItem << stockItem;
        inventoryModel->appendRow(rowItems);
    }
}
