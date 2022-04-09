import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Item {
    property alias midiEditorSnapUnit: editorSnapUnit
    property alias triplets: tripletsButton.triplets
    property alias showKeyScale: showKeyScaleButton.showKeyScale
    property int verticalLineSpacing: 20
    property int horizontalLineSpacing: 15
    property int barCount: 20
    id: root
    clip: true
    Item {
        id: _left
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 75
        Rectangle {
            id: lu
            anchors.left: parent.left
            anchors.top: parent.top
            width: parent.width
            height: 40
            z: 3
            color: Constants.backgroundColor
            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 1
                color: Constants.borderColor
            }
            MCtrl.ComboBox {
                id: editorSnapUnit
                anchors.left: parent.left
                anchors.top: parent.top
//                color: Constants.mouseOverElementColor
//                border.width: 0
                width: parent.width - height
                height: parent.height / 2
                font.family: Constants.font
                model: ["1/64", "1/32", "1/16", "1/8", "1/4", "1/2", "1/1"]
                currentIndex: 4
                MCtrl.ToolTip {
                    visible: parent.hovered
                    text: qsTr("对齐单位")
                }
            }
            Button {
                id: tripletsButton
                property bool triplets: false
                anchors.right: parent.right
                anchors.top: parent.top
                width: 20
                height: 20
                contentItem: Text {
                    anchors.centerIn: parent
                    text: "3"
                    color: triplets? Constants.backgroundColor: Constants.contentColor1
                    font.family: "Noto Sans Mono Condensed"
                    font.styleName: "SemiBold"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                background: Rectangle {
                    anchors.fill: parent
                    color: triplets? Constants.currentElementColor: Constants.backgroundColor
                }
                MCtrl.ToolTip {
                    visible: parent.hovered
                    text: qsTr("三连音")
                }
                onClicked: {
                    triplets = !triplets;
                }
            }
            Button {
                id: showKeyScaleButton
                property bool showKeyScale: false
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.bottomMargin: parent.border.width
                width: 40
                height: 20 - anchors.bottomMargin
                contentItem: Text {
                    anchors.centerIn: parent
                    text: "SCALE"
                    font.family: "Noto Sans Mono Condensed"
                    font.styleName: "SemiBold"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: showKeyScaleButton.showKeyScale? Constants.backgroundColor: Constants.contentColor1
                }
                background: Rectangle {
                    anchors.fill: parent
                    color: showKeyScaleButton.showKeyScale? Constants.currentElementColor: Constants.backgroundColor
                }
                MCtrl.ToolTip {
                    visible: parent.hovered
                    text: qsTr("显示音阶")
                }
                onClicked: {
                    showKeyScaleButton.showKeyScale = !(showKeyScaleButton.showKeyScale);
                }
            }
        }
        Rectangle {
            id: ld
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            width: lu.width
            height: Constants.midiValueRange
            z: 3
            color: Constants.backgroundColor
            MCtrl.ComboBox {
                id: expressionSelector
                anchors.left: parent.left
                anchors.top: parent.top
                border.width: 0
                width: parent.width
                height: 20
                model: ["力度", "声像", "变调"]
                font.family: Constants.font
                currentIndex: 0
            }
        }
        //PianoKey
        Rectangle {
            anchors.top: lu.bottom
            anchors.bottom: ld.top
            width: parent.width
            anchors.left: parent.left
//            anchors.right: parent.right
            color: "#000000"
            clip: true
            PianoKeyBackup {
                id: pianoKey
                width: parent.width
                anchors.left: parent.left
                y: -vbar.position * height
                keyHeight: horizontalLineSpacing
                MouseArea {
                    anchors.fill: parent
                    onWheel: {
                        vbar.position += 0.1 * wheel.angleDelta.y / 1080.0 * (wheel.inverted? 1: -1);
                        if(vbar.position > 1 - vbar.size)
                        {
                            vbar.position = 1 - vbar.size;
                        }
                        else if(vbar.position < 0)
                        {
                            vbar.position = 0;
                        }
                    }
                }
            }
        }
    }
    Item {
        id: _right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 15
        Rectangle {
            id: ru
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: lu.height
            z: 3
            color: Constants.backgroundColor
        }
        Rectangle {
            id: rd
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: parent.width
            height: ld.height
            z: 3
            color: Constants.backgroundColor
        }
        ScrollBar {
            id: vbar
            anchors.right: parent.right
            width: 15
            anchors.top: ru.bottom
            height: ld.y - y
            orientation: Qt.Vertical
            background: Rectangle {
                anchors.fill: parent
                color: Constants.backgroundColor
            }
            contentItem: Rectangle {
                radius: 2
                color: Constants.mouseOverElementColor
            }
            size: height / pianoKey.height
            active: hovered || pressed
            position: 0.5
            // 备用
            Rectangle {
                anchors.left: parent.left
                width: 1
                anchors.top: parent.top
                height: parent.height
                color: "transparent"
            }
        }
    }
    Item {
        id: _medium
        anchors.left: _left.right
        width: parent.width - _left.width - _right.width
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        Item {
            id: timelineAndHbar
            anchors.top: parent.top
            height: lu.height
            anchors.left: parent.left
            width: parent.width
            z: 1
            clip: true
            ScrollBar {
                id: hbar
                anchors.left: parent.left
                width: parent.width
                anchors.top: parent.top
                height: 15
                background: Rectangle {
                    anchors.fill: parent
                    color: timeline.color
                }
                contentItem: Rectangle {
                    radius: 2
                    color: Constants.mouseOverElementColor
                }
                size: parent.width / timeline.width
                orientation: Qt.Horizontal
                active: hovered || pressed
                position: 0.0
            }
            Timeline {
                id: timeline
                barWidth: numerator * verticalLineSpacing
                barCount: root.barCount
                anchors.bottom: parent.bottom
                height: parent.height - hbar.height
                x: -hbar.position * width
                Rectangle {
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    width: parent.width
                    height: 1
                    color: Constants.borderColor
                }
                MouseArea {
                    anchors.fill: parent
                    onWheel: {
                        hbar.position -= 0.1 * wheel.angleDelta.y / 540.0 * (wheel.inverted? -1: 1);
                        if(hbar.position > 1 - hbar.size)
                        {
                            hbar.position = 1 - hbar.size;
                        }
                        else if(hbar.position < 0)
                        {
                            hbar.position = 0;
                        }
                    }
                }
            }
        }
        Rectangle {
            id: expressionAreaRect
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: ld.height
            clip: true
            ContentArea {
                id: expressionArea
                anchors.top: parent.top
                height: parent.height
                x: timeline.x
                verLineCount: timeline.barCount * timeline.numerator
                verLineSpacing: verticalLineSpacing
                horLineCount: 1
                horLineSpacing: height
            }
        }
        Rectangle {
            id: contentAreaRect
            anchors.left: parent.left
            width: parent.width
//            y:lu.height
            height: expressionAreaRect.y
            clip: true
            ContentArea {
                id: contentArea
                x: timeline.x
                y: pianoKey.y + timelineAndHbar.height
                verLineCount: timeline.barCount * timeline.numerator
                verLineSpacing: verticalLineSpacing
                horLineCount: Constants.midiValueRange
                horLineSpacing: horizontalLineSpacing
                Column { // 用于区分钢琴键黑白的背景
                    anchors.fill: parent
                    z: 1
                    Repeater {
                        model: Constants.midiValueRange
                        Rectangle {
                            width: parent.width
                            height: pianoKey.keyHeight
                            color: Constants.blackKey(Constants.midiValueRange - 1 - index)? Constants.backgroundColor2 : Constants.backgroundColor
                        }
                    }
                }
                ListView {
                    id: noteView
                    // 未完成
                }
                MouseArea {
                    anchors.fill: parent
                    scrollGestureEnabled: true
                    onWheel: {
                        if(wheel.modifiers == Qt.NoModifier)
                        {
                            vbar.position += 0.1 * wheel.angleDelta.y / 1080.0 * (wheel.inverted? 1: -1);
                            if(vbar.position > 1 - vbar.size)
                            {
                                vbar.position = 1 - vbar.size;
                            }
                            else if(vbar.position < 0)
                            {
                                vbar.position = 0;
                            }
                            hbar.position += 0.1 * wheel.angleDelta.x / 540.0 * (wheel.inverted? 1: -1);
                            if(hbar.position > 1 - hbar.size)
                            {
                                hbar.position = 1 - hbar.size;
                            }
                            else if(hbar.position < 0)
                            {
                                hbar.position = 0;
                            }
                        }
                        else if(wheel.modifiers & Qt.ShiftModifier)
                        {
                            hbar.position += 0.1 * wheel.angleDelta.y / 540.0 * (wheel.inverted? 1: -1);
                            if(hbar.position > 1 - hbar.size)
                            {
                                hbar.position = 1 - hbar.size;
                            }
                            else if(hbar.position < 0)
                            {
                                hbar.position = 0;
                            }
                            vbar.position += 0.1 * wheel.angleDelta.x / 1080.0 * (wheel.inverted? 1: -1);
                            if(vbar.position > 1 - vbar.size)
                            {
                                vbar.position = 1 - vbar.size;
                            }
                            else if(vbar.position < 0)
                            {
                                vbar.position = 0;
                            }
                        }
                    }
                    onDoubleClicked: {
                        var note = 127 - (mouseY - mouseY % pianoKey.keyHeight) / pianoKey.keyHeight;
                        var item = Qt.createQmlObject("import Musec 1.0; Note {}",
                            contentArea, null
                        );
                        var _x = mouseX - contentArea.x + contentAreaRect.x;
                        item.x = _x - _x % verticalLineSpacing + 1;
                        item.y = mouseY - mouseY % pianoKey.keyHeight + 1;
                        item.width = verticalLineSpacing - 1;
                        item.height = pianoKey.keyHeight - 1;
                        item.noteMidiNum = note;
                        item.z = 3;
                    }
                }
            }
        }
    }
}
