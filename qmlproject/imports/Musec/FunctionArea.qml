import QtQuick 2.15
import QtQuick.Layouts 1.15
//import Musec 1.0

Item {
    id: root
    property string title
    property alias contentArea: contentArea
    anchors.fill: parent
    Rectangle
    {
        id: titleArea
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        height: 20
        color: Constants.borderColor
        clip: true
        Text {
            id: titleText
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: (parent.height - contentHeight)
            text: title
            font.family: Constants.font
            font.pointSize: Constants.titleTextPointSize
            color: Constants.contentColor1
        }
    }
    Rectangle {
        id: contentArea
        anchors.top: titleArea.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        border.color: Constants.borderColor
        color: Constants.backgroundColor
        clip: true
    }
}
