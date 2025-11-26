#include "warehouseedit.h"
#include "ui_warehouseedit.h"
#include <QDebug>

warehouseEdit::warehouseEdit(Warehouse *warehouseToEdit, Simulation *sim, QWidget *parent)
    : QWidget(parent), ui(new Ui::warehouseEdit), simulation(sim), currentWarehouse(warehouseToEdit)
{
    ui->setupUi(this);

    // Inicializar el modelo y la tabla
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

    ui->tableView->setItemDelegateForColumn(3, new Stockspinboxdelegate(this));

    // Configuración visual
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
        int currentStock = inventory.getStockLevel(productID);

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

void warehouseEdit::on_pushButton_clicked()
{
    // 1. Obtener el índice de la fila seleccionada
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, tr("Advertencia"), tr("Seleccione un producto para añadirle stock"));
        return;
    }

    // 2. Usar la primera fila seleccionada (índice 0 en la lista)
    QModelIndex selectedIndex = selectedRows.at(0);

    // 3. Obtener el ID del producto (está en la columna 0)
    bool ok;
    // Usamos productModel directamente ya que es el modelo de ui->tableView
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

    if (!ok) return; // Usuario canceló

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

    if (!ok) return; // Usuario canceló

    currentWarehouse->getDockedTrucks().clear();

    for (int i = 1; i <= quantity; ++i)
    {
        currentWarehouse->dockTruck(new Truck(i, currentWarehouse->getCoordinate(), currentWarehouse));
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

    if (!ok) return; // Usuario canceló

    currentWarehouse->setTotalEmployees(quantity);

    QString employeeString;

    employeeString.append(QString::fromStdString("Cantidad de empleados: "));
    employeeString.append(QString::number(currentWarehouse->getTotalEmployees()));

    ui->employeeLabel->setText(employeeString);
}

