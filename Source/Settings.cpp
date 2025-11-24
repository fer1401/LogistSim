#include "Settings.h"
#include "ui_settings.h"
#include <QQmlContext>
#include <QVBoxLayout>

Settings::Settings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Settings)
{
    ui->setupUi(this);
    setupMapTab();
}

Settings::~Settings()
{
    delete ui;
}

void Settings::setupMapTab()
{
    // 1. Crear el contenedor QQuickWidget
    mapQuickWidget = new QQuickWidget(this);

    // 3. Cargar tu archivo QML
    // Asegúrate de que la ruta sea correcta (asumo que está en recursos Qt, 'qrc:/')
    mapQuickWidget->setSource(QUrl("qrc:/WarehouseSetupMap.qml"));

    // 4. Integrar en el QTabWidget
    QWidget *tab = ui->tab; // Asumiendo que 'tab_1' es el QWidget en la primera pestaña

    // Usar un layout para asegurar que el mapa QML se estire para llenar la pestaña
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->addWidget(mapQuickWidget);
    layout->setContentsMargins(0, 0, 0, 0); // Quitar márgenes
    tab->setLayout(layout);

    // Ajustar el modo de redimensionamiento para que el QML ocupe todo el widget
    mapQuickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
}
