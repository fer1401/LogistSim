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

    // Inicializar el modelo
    productModel = new QStandardItemModel(this);

    setupProductCatalogView();

    // Enlazar el modelo a QTableView
    ui->tableView->setModel(productModel);

    // configuracion para seleccionar filas completas
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->tableView->verticalHeader()->hide();

    connect(ui->AddProductButton, &QPushButton::clicked, this, &Settings::on_addProduct_clicked);
    connect(ui->DeleteButton, &QPushButton::clicked, this, &Settings::on_deleteProduct_clicked);

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

void Settings::setupProductCatalogView()
{
    // 1. Limpiar el modelo antes de llenarlo
    productModel->clear();

    // 2. Obtener el catálogo directamente desde la Simulación (¡El paso clave!)
    const auto& catalog = simulation->getProductCatalog();

    // 3. Establecer las cabeceras de la tabla
    QStringList headers = {"ID", "Nombre", "Descripción"};
    productModel->setHorizontalHeaderLabels(headers);
    productModel->setColumnCount(headers.size());

    // 4. Iterar sobre el catálogo de productos de la simulación y llenar el modelo
    for (const auto& product : catalog) {

        QList<QStandardItem*> rowItems;

        // Crear el ítem para cada columna usando los datos de la clase Product
        QStandardItem *idItem = new QStandardItem(QString::number(product.getId()));
        QStandardItem *nameItem = new QStandardItem(QString::fromStdString(product.getName()));
        QStandardItem *descItem = new QStandardItem(QString::fromStdString(product.getDescription()));

        // Bloquear la edición
        idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
        descItem->setFlags(descItem->flags() & ~Qt::ItemIsEditable);

        rowItems << idItem << nameItem << descItem;

        // Añadir la fila completa al modelo
        productModel->appendRow(rowItems);
    }
}

void Settings::on_addProduct_clicked()
{
    // 1. Obtener ID (usamos un simple QInputDialog)
    bool ok;
    int id = QInputDialog::getInt(this,
                                  tr("Nuevo Producto"),
                                  tr("Ingrese el ID único del producto:"),
                                  0, 1, 1000, 1, &ok);

    if (!ok) return; // Usuario canceló

    // 2. Obtener Nombre
    QString name = QInputDialog::getText(this,
                                         tr("Nuevo Producto"),
                                         tr("Ingrese el Nombre del producto:"),
                                         QLineEdit::Normal,
                                         "", &ok);
    if (!ok || name.isEmpty()) return;

    // 3. Obtener Descripción
    QString description = QInputDialog::getText(this,
                                                tr("Nuevo Producto"),
                                                tr("Ingrese la Descripción del producto:"),
                                                QLineEdit::Normal,
                                                "", &ok);
    if (!ok || description.isEmpty()) return;

    // 4. Llamar al método de la Simulación
    // Se necesita un método addNewProduct en la clase Simulation
    if (simulation->addNewProduct(id, name, description))
    {
        // 5. Actualizar la tabla
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
    // 1. Obtener el índice de la fila seleccionada
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, tr("Advertencia"), tr("Seleccione un producto para eliminar."));
        return;
    }

    // 2. Usar la primera fila seleccionada (índice 0 en la lista)
    QModelIndex selectedIndex = selectedRows.at(0);

    // 3. Obtener el ID del producto (está en la columna 0)
    bool ok;
    // Usamos productModel directamente ya que es el modelo de ui->tableView
    int idToDelete = productModel->data(selectedIndex.sibling(selectedIndex.row(), 0)).toInt(&ok);

    if (!ok) {
        QMessageBox::critical(this, tr("Error"), tr("No se pudo obtener el ID del producto."));
        return;
    }

    // 4. Confirmación
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Confirmar"),
                                  tr("¿Está seguro de que desea eliminar el producto con ID %1?").arg(idToDelete),
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        // 5. Llamar al método de la Simulación
        // Se necesita un método deleteProduct en la clase Simulation
        if (simulation->deleteProduct(idToDelete))
        {
            // 6. Actualizar la tabla
            setupProductCatalogView();
            QMessageBox::information(this, tr("Éxito"), tr("Producto eliminado correctamente."));
        }
        else
        {
            QMessageBox::critical(this, tr("Error"), tr("No se pudo encontrar el producto con ese ID."));
        }
    }
}
