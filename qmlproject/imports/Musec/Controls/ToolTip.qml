import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2
import Musec 1.0

QQC2.ToolTip {
    id: control
    text: qsTr("工具提示文字")
    contentItem: Text {
        text: control.text
        font: Constants.font
        color: Constants.contentColor1
    }
    background: Rectangle {
        color: Constants.backgroundColor
        border.color: Constants.borderColor
    }
}
