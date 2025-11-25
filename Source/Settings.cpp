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
    //productModel = new QStandardItemModel(this);

    // Cargar el JSON desde la ruta de recursos
    //loadProductCatalog(":/ProductCatalog.json");

    // Enlazar el modelo a QTableView
    //ui->tableView->setModel(productModel);
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

void Settings::loadProductCatalog(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Error: No se pudo abrir el archivo JSON:" << filePath << file.errorString();
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qCritical() << "Error al parsear JSON:" << parseError.errorString();
        return;
    }

    if (!doc.isArray()) {
        qCritical() << "Error: El JSON raíz no es un array (lista de productos).";
        return;
    }

    QJsonArray productArray = doc.array();

    // 1. Establecer las cabeceras de la tabla (las claves del JSON)
    QStringList headers = {"ID", "Nombre", "Descripción"};
    productModel->setHorizontalHeaderLabels(headers);
    productModel->setColumnCount(headers.size());

    // 2. Iterar sobre el array de productos y llenar el modelo
    for (const QJsonValue &value : productArray) {
        if (value.isObject()) {
            QJsonObject obj = value.toObject();
            QList<QStandardItem*> rowItems;

            // Crear el ítem para cada columna. Nota: Usar QString::number para convertir a texto.
            QStandardItem *idItem = new QStandardItem(QString::number(obj["id"].toInt()));
            QStandardItem *nameItem = new QStandardItem(obj["nombre"].toString());
            QStandardItem *descItem = new QStandardItem(obj["descripcion"].toString());

            // Bloquear la edición de ciertas celdas si es un catálogo fijo
            idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);

            rowItems << idItem << nameItem << descItem;

            // Añadir la fila completa al modelo
            productModel->appendRow(rowItems);
        }
    }
}
