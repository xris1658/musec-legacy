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
    Grid {
        columns: 2
        columnSpacing: 10
        rowSpacing: 5
        verticalItemAlignment: Grid.AlignVCenter
        horizontalItemAlignment: Grid.AlignLeft
        Text {
            width: 100
            height: 20
            text: qsTr("区域")
            font.family: Constants.font
            color: Constants.contentColor2
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Item {
            width: 320
            height: 20
        }
        Text {
            width: 100
            text: qsTr("语言")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
        MCtrl.ComboBox {
            id: comboBoxLanguage
            model: ["简体中文", "繁體中文", "English"]
            font.family: Constants.font
            width: 100
        }
        Text {
            width: 100
            height: 20
            text: qsTr("外观和交互")
            font.family: Constants.font
            color: Constants.contentColor2
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Item {
            width: 320
            height: 20
        }
        Text {
            width: 100
            text: qsTr("颜色")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
        MCtrl.ComboBox {
            id: comboBoxColor
            model: ["深色", "浅色"]
            font.family: Constants.font
            width: 100
        }
        Text {
            width: 100
            text: qsTr("缩放比例")
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
            width: 100
        }
        Text {
            width: 100
            text: qsTr("触控")
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
            width: 100
            text: qsTr("单实例模式")
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
            width: 100
            text: qsTr("使用系统文字渲染")
            font.family: Constants.font
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
                    text: qsTr("如果您的计算机上安装了 MacType 等改善系统文字渲染的程序，\n请勾选此项。否则通常不建议勾选。")
//                    visible: systemTextRenderingInfoMouseArea.containsMouse
                    visible: systemTextRenderingInfoButton.hovered | systemTextRenderingInfoButton.activeFocus
                }
            }
        }
    }
}
