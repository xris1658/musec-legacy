import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Musec 1.0
import Musec.Controls 1.0 as MCtrl
import Musec.Dialogs 1.0 as MDlg

Rectangle {
    id: root
    property alias systemRender: checkBoxSystemTextRendering.checked
    width: 500
    height: 500
    clip: true
    color: Constants.backgroundColor
    property int firstColumnWidth: 150
    property int secondColumnWidth: 100
    Grid {
        columns: 2
        columnSpacing: 10
        rowSpacing: 5
        verticalItemAlignment: Grid.AlignVCenter
        horizontalItemAlignment: Grid.AlignLeft
        Text {
            width: firstColumnWidth
            height: 20
            text: qsTr("Region")
            font.family: Constants.font
            color: Constants.contentColor2
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Item {
            width: secondColumnWidth
            height: 20
        }
        Text {
            width: firstColumnWidth
            text: qsTr("Language")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
        MCtrl.ComboBox {
            id: comboBoxLanguage
            model: ["简体中文", "繁體中文", "English"]
            font.family: Constants.font
            width: secondColumnWidth
        }
        Text {
            width: firstColumnWidth
            height: 20
            text: qsTr("Appearance")
            font.family: Constants.font
            color: Constants.contentColor2
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Item {
            width: secondColumnWidth
            height: 20
        }
        Text {
            width: firstColumnWidth
            text: qsTr("Color")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
        MCtrl.ComboBox {
            id: comboBoxColor
            model: ["Dark", "Light"]
            font.family: Constants.font
            width: secondColumnWidth
        }
        Text {
            width: firstColumnWidth
            text: qsTr("Zoom")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
        MCtrl.ComboBox {
            id: comboBoxZoom
            model: ["50%", "75%", "100%", "125%", "150%", "175%", "200%"]
            currentIndex: 2
            font.family: Constants.font
            width: secondColumnWidth
        }
        Text {
            width: firstColumnWidth
            text: qsTr("Touch")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
        MCtrl.CheckBox {
            id: checkBoxTouchSupport
            width: 16
            height: 16
            tristate: false
            checked: false
        }
        Text {
            width: firstColumnWidth
            text: qsTr("Single instance")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
        MCtrl.CheckBox {
            id: checkBoxSingleInstance
            width: 16
            height: 16
            tristate: false
            checked: true
        }
        Text {
            width: firstColumnWidth
            text: qsTr("Use system font rendering")
            font.family: Constants.font
            font.italic: true
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
        Row {
            spacing: checkBoxSystemTextRendering.width / 2
            MCtrl.CheckBox {
                id: checkBoxSystemTextRendering
                width: 16
                height: 16
                tristate: false
                onCheckedChanged: {
                    EventBridge.systemTextRenderingChanged(checked);
                }
            }
            MCtrl.Button {
                id: systemTextRenderingInfoButton
                width: checkBoxSystemTextRendering.height
                height: width
                background: Rectangle {
                    color: Constants.backgroundColor
                }
                Rectangle {
                    id: systemTextRenderingInfo
                    anchors.fill: parent
                    radius: height / 2
                    color: Constants.backgroundColor2
                    border.color: Constants.borderColor
                    border.width: systemTextRenderingInfoToolTip.visible? 2: 1
                    Text {
                        anchors.centerIn: parent
                        font.family: "Noto Sans Mono Condensed"
                        font.bold: true
                        text: "?"
                        color: Constants.borderColor
                    }
                }
                MCtrl.ToolTip {
                    id: systemTextRenderingInfoToolTip
                    text: qsTr("If applications that improves your font rendering (e.g. MacType),\nplease check this. Otherwise this is not recommended to check.")
//                    visible: systemTextRenderingInfoMouseArea.containsMouse
                    visible: systemTextRenderingInfoButton.hovered | systemTextRenderingInfoButton.activeFocus
                }
            }
        }
    }
    Item {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10
        height: experimentalFunctionText.contentHeight
        Text {
            id: experimentalFunctionText
            anchors.left: parent.left
            text: qsTr("<i>Italic</i> items are experimental features.")
            font.family: Constants.font
            color: Constants.contentColor2
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
    }
}
