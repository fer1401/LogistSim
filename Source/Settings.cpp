#include "Settings.h"
#include "ui_settings.h"
#include <QQmlContext>
#include <QVBoxLayout>
#include <QFile>
#include <QDebug>
#include <QStandardItem>

Settings::Settings(Simulation *s, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Settings), simulation(s)
{
    ui->setupUi(this);
    setupMapTab();

    productModel = new QStandardItemModel(this);

    setupProductCatalogView();
    warehousesModel = new QStandardItemModel(this);

    loadWarehouses();

    ui->tableView->setModel(productModel);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->warehousesTableView->setModel(warehousesModel);

    ui->warehousesTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->tableView->verticalHeader()->hide();

    connect(ui->AddProductButton, &QPushButton::clicked, this, &Settings::on_addProduct_clicked);
    connect(ui->DeleteButton, &QPushButton::clicked, this, &Settings::on_deleteProduct_clicked);
    ui->warehousesTableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->warehousesTableView->verticalHeader()->hide();

}

Settings::~Settings()
{
    delete ui;
}

void Settings::setupMapTab()
{
    QQuickWidget *map = ui->tab->findChild<QQuickWidget*>("warehousesMapWidget");

    map->rootContext()->setContextProperty("simulation", simulation);
    map->setSource(QUrl("qrc:/WarehouseSetupMap.qml"));
}

void Settings::loadWarehouses()
{
    warehousesModel->clear();

    QStringList headers = {"Latitud", "Longitud", "Nº de empleados"};
    warehousesModel->setHorizontalHeaderLabels(headers);
    warehousesModel->setColumnCount(headers.size());

    int i = 1;

    for (const auto &warehouse : simulation->getWarehouses())
    {
        QList<QStandardItem*> rowItems;

        QStandardItem *idItem = new QStandardItem(QString::number(i));
        QStandardItem *latitudItem = new QStandardItem(QString::fromStdString(std::to_string(warehouse->getCoordinate().latitude())));
        QStandardItem *longitudItem = new QStandardItem(QString::fromStdString(std::to_string(warehouse->getCoordinate().longitude())));
        QStandardItem *employees = new QStandardItem(QString::fromStdString(std::to_string(warehouse->getTotalEmployees())));

        idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
        latitudItem->setFlags(latitudItem->flags() & ~Qt::ItemIsEditable);
        longitudItem->setFlags(longitudItem->flags() & ~Qt::ItemIsEditable);
        employees->setFlags(employees->flags() & ~Qt::ItemIsEditable);

        rowItems << idItem << latitudItem << longitudItem << employees;

        warehousesModel->appendRow(rowItems);

        ++i;
    }
}

void Settings::setupProductCatalogView()
{
    productModel->clear();

    const auto& catalog = simulation->getProductCatalog();

    QStringList headers = {"ID", "Nombre", "Descripción"};
    productModel->setHorizontalHeaderLabels(headers);
    productModel->setColumnCount(headers.size());

    for (const auto& product : catalog) {

        QList<QStandardItem*> rowItems;

        QStandardItem *idItem = new QStandardItem(QString::number(product.getId()));
        QStandardItem *nameItem = new QStandardItem(QString::fromStdString(product.getName()));
        QStandardItem *descItem = new QStandardItem(QString::fromStdString(product.getDescription()));

        idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
        descItem->setFlags(descItem->flags() & ~Qt::ItemIsEditable);

        rowItems << idItem << nameItem << descItem;

        productModel->appendRow(rowItems);
    }
}

void Settings::on_addProduct_clicked()
{
    bool ok;
    int id = QInputDialog::getInt(this,
                                  tr("Nuevo Producto"),
                                  tr("Ingrese el ID único del producto:"),
                                  0, 1, 1000, 1, &ok);

    if (!ok) return;

    QString name = QInputDialog::getText(this,
                                         tr("Nuevo Producto"),
                                         tr("Ingrese el Nombre del producto:"),
                                         QLineEdit::Normal,
                                         "", &ok);
    if (!ok || name.isEmpty()) return;

    QString description = QInputDialog::getText(this,
                                                tr("Nuevo Producto"),
                                                tr("Ingrese la Descripción del producto:"),
                                                QLineEdit::Normal,
                                                "", &ok);
    if (!ok || description.isEmpty()) return;

    if (simulation->addNewProduct(id, name, description))
    {
        setupProductCatalogView();
        QMessageBox::information(this, tr("Éxito"), tr("Producto añadido correctamente."));
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), tr("No se pudo añadir el producto. El ID ya existe."));
    }
}

void Settings::on_deleteProduct_clicked()
{
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, tr("Advertencia"), tr("Seleccione un producto para eliminar."));
        return;
    }

    QModelIndex selectedIndex = selectedRows.at(0);

    bool ok;

    int idToDelete = productModel->data(selectedIndex.sibling(selectedIndex.row(), 0)).toInt(&ok);

    if (!ok) {
        QMessageBox::critical(this, tr("Error"), tr("No se pudo obtener el ID del producto."));
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Confirmar"),
                                  tr("¿Está seguro de que desea eliminar el producto con ID %1?").arg(idToDelete),
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        if (simulation->deleteProduct(idToDelete))
        {
            setupProductCatalogView();
            QMessageBox::information(this, tr("Éxito"), tr("Producto eliminado correctamente."));
        }
        else
        {
            QMessageBox::critical(this, tr("Error"), tr("No se pudo encontrar el producto con ese ID."));
        }
    }
}

void Settings::on_editButton_clicked()
{
    QModelIndexList selectedRows = ui->warehousesTableView->selectionModel()->selectedRows();

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, tr("Advertencia"), tr("Seleccione un almacen para editar."));
        return;
    }

    QModelIndex selectedIndex = selectedRows.at(0);

    bool ok;

    int idToEdit = warehousesModel->data(selectedIndex.sibling(selectedIndex.row(), 0)).toInt(&ok);

    if (!ok) {
        QMessageBox::critical(this, tr("Error"), tr("No se pudo obtener el ID del producto."));
        return;
    }

    int i = 1;
    Warehouse *warehousetoedit;


    for (auto warehouse : simulation->getWarehouses())
    {
        if (idToEdit == i)
        {
            warehousetoedit = warehouse;
            break;
        }

        ++i;
    }

    warehouseEdit *warehouseedit = new warehouseEdit(warehousetoedit, simulation, nullptr);

    warehouseedit->show();
}


void Settings::on_pushButton_clicked()
{
    simulation->addNewWarehouse(simulation->getNewCoordinate().latitude(), simulation->getNewCoordinate().longitude());

    loadWarehouses();
}

