import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15
import QtQml.Models 2.15

import Musec.Controls 1.0 as MCtrl
import Musec.Models 1.0 as MModels

Item {
    id: root
    property double timeUnitWidth: 1
    property double defaultValue: 0.5
    property double pointRadius: 4
    property MModels.AutomationModel model
    clip: false
    function modelEmpty() {
        return (!model) || (model.empty());
    }
    function timeToX(time: int) {
        return time * timeUnitWidth;
    }
    function xToTime(x: real) {
        return x / timeUnitWidth;
    }
    function normalizeValue(value: double) {
        if(model.maxValue() == model.minValue()) {
            return 0;
        }
        return (value - model.minValue()) / (model.maxValue() - model.minValue());
    }
    function valueToY(value: double) {
        return (1 - normalizeValue(value)) * (height - pointRadius * 2) + pointRadius;
    }
    function yToValue(y: double) {
        if(model.minValue() == model.maxValue()) {
            return 0;
        }
        var normalizedValue =  1 - (y - pointRadius) / (height - pointRadius * 2);
        return model.minValue() + normalizedValue * (model.maxValue() - model.minValue());
    }
    function timeOfPoint(index: int) {
        if(index < 0 || index >= model.count()) {
            console.log("Invalid index: ", index);
            return 0;
        }
        return model.timeOfPoint(index);
    }
    function valueOfPoint(index: int) {
        if(index < 0 || index >= model.count()) {
            console.log("Invalid index: ", index);
            return 0;
        }
        return model.valueOfPoint(index);
    }
    function repressPoint(newIndex, mouseEvent) {
        while(points.itemAt(newIndex) == null) {
        }
        points.itemAt(newIndex).onPressed(mouseEvent);
    }
    MouseArea {
        anchors.fill: parent
        onDoubleClicked: {
            root.model.insertPoint(xToTime(mouseX), yToValue(mouseY), 0, 0);
        }
    }
    Repeater {
        id: points
        model: root.model
        delegate: MouseArea {
            id: mouseArea
            width: root.pointRadius * 2
            height: width
            x: timeToX(time) - width / 2
            y: valueToY(value) - height / 2
            property Item rootItem: root
            hoverEnabled: true
            property int initialX: 0
            property int initialY: 0
            Rectangle {
                anchors.fill: parent
                color: Constants.automationColor
                border.color: color
                radius: root.pointRadius
            }
            onDoubleClicked: {
                var oldIndex = index;
                var oldCount = root.model.count();
                root.model.deletePoint(index);
                if(oldIndex >= oldCount - 1) {
                    lastToEnd.updateStartX();
                    lastToEnd.updateStartY();
                }
                else {
                    paths.itemAt(oldIndex).updateX();
                    paths.itemAt(oldIndex).updateY();
                }
            }
            onPressed: {
                initialX = mouseX;
                initialY = mouseY;
            }
            onMouseXChanged: {
                if(pressed) {
                    console.log(mouseX);
                    var delta = mouseX - initialX;
                    var newTime = root.xToTime(x + width / 2 + delta);
                    if(newTime < 0) {
                        newTime = 0;
                    }
                    var newIndex = root.model.setTimeOfPoint(index, newTime, 0);
                    if(newIndex != index) {
                        rootItem.repressPoint(newIndex, mouse);
                    }
                }
            }
            onMouseYChanged: {
                if(pressed) {
                    var delta = mouseY - initialY;
                    var newValue = root.yToValue(y + delta + height / 2);
                    if(newValue > root.model.maxValue()) {
                        newValue = root.model.maxValue();
                    }
                    if(newValue < root.model.minValue()) {
                        newValue = root.model.minValue();
                    }
                    root.model.setValueOfPoint(index, newValue);
                }
            }
        }
    }
    // 线向左
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
            signal updateX()
            onUpdateX: {
                shapePath.updateX();
            }
            signal updateY()
            onUpdateY: {
                shapePath.updateY();
            }
            Rectangle {
                width: root.pointRadius
                height: width
                x: pathQuad.controlX - width / 2
                y: pathQuad.controlY - height / 2
                color: Constants.automationColor
            }

            ShapePath {
                id: shapePath
                strokeColor: Constants.automationColor
                strokeWidth: 2
                fillColor: "transparent"
                startX: root.timeToX(time)
                startY: root.valueToY(value)
                onStartXChanged: {
                    if(index == root.model.count() - 1) {
                        lastToEnd.updateStartX();
                    }
                    else {
                        if(paths.itemAt(index + 1) != null) {
                            paths.itemAt(index + 1).updateX();
                        }
                    }
                }
                onStartYChanged: {
                    if(index == root.model.count() - 1) {
                        lastToEnd.updateStartY();
                    }
                    else {
                        if(paths.itemAt(index + 1) != null) {
                            paths.itemAt(index + 1).updateY();
                        }
                    }
                }
                function updateX() {
                    pathQuad.x = (index <= 0)? 0: root.timeToX(root.timeOfPoint(index - 1));
                }
                function updateY() {
                    pathQuad.y = (index <= 0)? shapePath.startY: root.valueToY(root.valueOfPoint(index - 1))
                }
                PathQuad {
                    id: pathQuad
                    x: (index <= 0)? 0: root.timeToX(root.timeOfPoint(index - 1))
                    y: (index <= 0)? shapePath.startY: root.valueToY(root.valueOfPoint(index - 1))
                    controlX: (shapePath.startX + x) / 2
                    controlY: (index <= 0)? shapePath.startY: Math.max(shapePath.startY, y) - Math.abs(shapePath.startY - y) * (curve * 0.5 + 0.5)
                    onControlYChanged: {
                        if(Number.isNaN(controlY)) {
                            console.log("controlY is NaN, index:", index);
                        }
                    }
                }
            }
        }
    }
    // 线向右
    Shape {
        id: lastToEnd
        width: parent.width
        height: parent.height
        function updateStartX() {
            lastToEndShapePath.startX = (root.modelEmpty() || root.model.count() <= 0)? 0: root.timeToX(root.timeOfPoint(root.model.count() - 1));
        }
        function updateStartY() {
            lastToEndShapePath.startY = (root.modelEmpty() || root.model.count() <= 0)? root.valueToY(root.defaultValue): root.valueToY(root.valueOfPoint(root.model.count() - 1));
        }
        ShapePath {
            id: lastToEndShapePath
            strokeColor: Constants.automationColor
            strokeWidth: 2
            strokeStyle: root.modelEmpty()? ShapePath.DashLine: ShapePath.SolidLine
            startX: (root.modelEmpty() || root.model.count() <= 0)? 0: root.timeToX(root.timeOfPoint(root.model.count() - 1))
            startY: (root.modelEmpty() || root.model.count() <= 0)? root.valueToY(root.defaultValue): root.valueToY(root.valueOfPoint(root.model.count() - 1))
            PathLine {
                x: root.width
                y: lastToEndShapePath.startY
            }
        }
    }
}
