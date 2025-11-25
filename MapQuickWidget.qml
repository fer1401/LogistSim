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

        Repeater
        {
            model: simulation.warehouses

            delegate: MapQuickItem{

                coordinate: model.coordinate
                width: 50
                height: 50
                sourceItem: Image {source: "qrc:/almacen.png"
                    width: 50
                    height: 50
                }
            }
        }

        Repeater
        {
            model: simulation.trucks

            delegate: MapQuickItem{

                coordinate: model.coordinate
                width: 50
                height: 50
                sourceItem: Image {source: "qrc:/camion.png"
                    width: 25
                    height: 25
                }
            }
        }

        MapItemView {
                model: simulation.orders // Aquí se enlaza a tu Q_PROPERTY

                delegate: MapQuickItem {

                    coordinate: modelData

                    anchorPoint.x: sourceItem.width / 2
                    anchorPoint.y: sourceItem.height / 2

                    sourceItem: Rectangle {
                        width: 16
                        height: 16
                        radius: 8
                        color: "red"
                        border.color: "white"
                        border.width: 2
                    }
                }
        }

        Repeater
        {
            model: simulation.trucks

            delegate: MapRoute{
                route: model.visualPath

                line.width: 3

                line.color: "Blue"
            }
        }


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
