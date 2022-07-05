import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15
import QtQml.Models 2.15

import Musec.Models 1.0 as MModels

Item {
    id: root
    property double timeUnitWidth: 1
    property double defaultValue: 0.5
    property MModels.AutomationModel model
    function modelEmpty() {
        return (!model) || (model.empty());
    }
    function timeToX(time: int) {
        return time * timeUnitWidth;
    }
    function valueToY(value: double) {
        return (1 - value) * root.height;
    }
    clip: false

    Repeater {
        id: points
        model: root.model
        delegate: Rectangle {
            width: 8
            height: width
            radius: width / 2
            x: timeToX(time) - width / 2
            y: valueToY(value) - (height / 2)
            color: Constants.automationColor
            border.color: color
        }
    }
    Repeater {
        id: paths
        model: root.model
        delegate: Shape {
            id: shape
            visible: lastToEnd.visible
            width: root.width
            height: root.height
            layer.enabled: true
            layer.smooth: true
            // 4x MSAA
            layer.samples: 4
            ShapePath {
                id: shapePath
                strokeColor: Constants.automationColor
                strokeWidth: 2
                fillColor: "transparent"
                startX: root.timeToX(time)
                startY: root.valueToY(value)
                PathQuad {
                    x: index == 0? 0: root.timeToX(root.model.timeOfPoint(index - 1))
                    y: index == 0? shapePath.startY: valueToY(root.model.valueOfPoint(index - 1))
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
        ShapePath {
            id: lastToEndShapePath
            strokeColor: Constants.automationColor
            strokeWidth: 2
            strokeStyle: root.modelEmpty()? ShapePath.DashLine: ShapePath.SolidLine
            startX: root.modelEmpty()? 0: root.timeToX(root.model.timeOfPoint(root.model.count() - 1))
            startY: root.modelEmpty()? root.valueToY(root.defaultValue): root.valueToY(root.model.valueOfPoint(root.model.count() - 1))
            PathLine {
                x: root.width
                y: root.modelEmpty()? root.valueToY(root.defaultValue): lastToEndShapePath.startY
            }
        }
    }
}
