import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15
import QtQml.Models 2.15

Item {
    id: root
    width: 400
    height: 100
    property double timeUnitWidth: 1
    property double defaultValue: 0.5
    property ListModel valueList
    Repeater {
        id: points
        model: valueList
        delegate: Rectangle {
            width: 8
            height: width
            radius: width / 2
            x: time - width / 2
            y: (1 - value) * root.height - (height / 2)
            color: Constants.automationColor
            border.color: color
        }
    }
    Repeater {
        id: paths
        model: valueList
        delegate: Shape {
            id: shape
            visible: lastToEnd.visible
            width: root.width
            height: root.height
            layer.enabled: true
            layer.smooth: true
            // 2x SSAA
            layer.textureSize: Qt.size(shape.width * 2, shape.height * 2)
            // 4x MSAA
//            layer.samples: 4
            ShapePath {
                id: shapePath
                strokeColor: Constants.automationColor
                strokeWidth: 2
                fillColor: "transparent"
                startX: time * timeUnitWidth
                startY: (1 - value) * root.height
                PathQuad {
                    x: index == 0? 0: paths.model.get(index - 1).time * timeUnitWidth
                    y: index == 0? shapePath.startY: (1 - paths.model.get(index - 1).value) * root.height
                    controlX: (shapePath.startX + x) / 2
                    controlY: index == 0? shapePath.startY: Math.max(shapePath.startY, y) - Math.abs(shapePath.startY - y) * (curve * 0.5 + 0.5)
                }
            }
        }
    }
    Shape {
        id: lastToEnd
        width: parent.width
        height: parent.height
        visible: valueList != null && valueList.count != 0
        ShapePath {
            id: lastToEndShapePath
            strokeColor: Constants.automationColor
            strokeWidth: 2
            startX: valueList.get(valueList.count - 1).time * timeUnitWidth
            startY: (1 - valueList.get(valueList.count - 1).value) * root.height
            PathLine {
                x: root.width
                y: lastToEndShapePath.startY
            }
        }
    }

    Shape {
        id: noPoint
        width: parent.width
        height: parent.height
        visible: !lastToEnd.visible
        ShapePath {
            id: noPointShapePath
            strokeColor: Constants.automationColor
            strokeWidth: 2
            strokeStyle: ShapePath.DashLine
            startX: 0
            startY: (1 - root.defaultValue) * root.height
            PathLine {
                x: root.width
                y: noPointShapePath.startY
            }
        }
    }
}
