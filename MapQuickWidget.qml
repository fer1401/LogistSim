import QtQuick 2.15
import QtLocation 6.1
import QtPositioning 5.15

Item
{
    width: 600
    height: 400

    Map
    {
        id: simulationMap
        anchors.fill: parent
        plugin: Plugin { name: "osm" }
        center: QtPositioning.coordinate(8.5945770, -71.1598334)
        zoomLevel: 14

        DragHandler
        {
            id: mapDragHandler
            target: simulationMap // Target the Map itself
            onTranslationChanged: (delta) => simulationMap.pan(-delta.x, -delta.y) // Pan the map
        }

        // 2. Control del Zoom con la Rueda del Ratón y doble click
        MouseArea
        {
            anchors.fill: parent
            hoverEnabled: true

            // Lógica para el zoom con la rueda del ratón
            onWheel: function(mouse) {
                if (mouse.angleDelta.y !== 0)
                {
                    let newZoom = simulationMap.zoomLevel + (mouse.angleDelta.y > 0 ? 0.5 : -0.5);
                    simulationMap.zoomLevel = Math.max(2, Math.min(20, newZoom));
                }
                mouse.accepted = true;
            }

            // Lógica para zoom con doble click
            onDoubleClicked: {
                // Calcula el centro de zoom en la posición del ratón
                simulationMap.zoomTo(simulationMap.zoomLevel + 1, mouse.x, mouse.y);
            }
        }
    }
}
