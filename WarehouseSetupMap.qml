import QtQuick 2.15
import QtLocation 6.1
import QtPositioning 5.15

Map // Usamos Map como el componente raíz
{
    id: setupMap
    anchors.fill: parent
    plugin: Plugin { name: "osm" }

    // **Ajusta estas coordenadas** para que tu área de simulación sea visible
    center: QtPositioning.coordinate(8.5945770, -71.1598334)
    zoomLevel: 14

    // --- Visualización de Almacenes Existentes ---
    Repeater
    {
        model: simulation.visualWarehouses // Usamos la propiedad expuesta desde C++

        delegate: MapQuickItem {
            // Asegúrate de que tu clase Warehouse tenga Q_PROPERTY para latitude y longitude
            coordinate: model.coordinate
            width: 50
            height: 50
            sourceItem: Image {
                source: "qrc:/almacen.png" // La imagen del pin del almacén
                width: 50
                height: 50
            }
        }
    }

    // --- Lógica para Añadir Almacén con Clic ---
    MouseArea
    {
        anchors.fill: parent
        // Solo para detectar el clic
        onClicked: (mouse) => {
            // 1. Convertir coordenadas de píxeles (mouse.x, mouse.y) a coordenadas geográficas (lat, lon)
            var newCoordinate = setupMap.toCoordinate(mouse.x, mouse.y, false);
            var lat = newCoordinate.latitude;
            var lon = newCoordinate.longitude;

            // 2. Llamar al método de C++ para crear el almacén
            var success = simulation.addNewWarehouse(lat, lon);

            if (!success) {
                // Notificar al usuario (puedes usar un componente de notificación QML)
                console.log("Error: No se pueden agregar más de 10 almacenes.");
            }

            mouse.accepted = true; // Consumir el evento de clic
        }

        // Mantener la funcionalidad de arrastre y zoom de tu mapa original
        // Esto previene que el clic para añadir almacén interfiera con el arrastre del mapa
        onPressAndHold: {
            // No hacer nada en 'onPressAndHold' o implementar arrastre si lo deseas aquí
        }
    }

    // --- Funcionalidad de Arrastre y Zoom (de tu mapa original) ---
    DragHandler
    {
        id: setupMapDragHandler
        target: setupMap
        onTranslationChanged: (delta) => setupMap.pan(-delta.x, -delta.y)
    }

    MouseArea
    {
        anchors.fill: parent
        hoverEnabled: true

        onWheel: function(mouse) {
            if (mouse.angleDelta.y !== 0)
            {
                let newZoom = setupMap.zoomLevel + (mouse.angleDelta.y > 0 ? 0.5 : -0.5);
                setupMap.zoomLevel = Math.max(2, Math.min(20, newZoom));
            }
            mouse.accepted = true;
        }

        onDoubleClicked: {
            setupMap.zoomTo(setupMap.zoomLevel + 1, mouse.x, mouse.y);
        }
    }
}
