import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Rectangle {
    id: root
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
    }
}
