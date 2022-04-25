import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root
    property int minimumHeight: 20
    property int _height
    property string name
    property color color
    height: _height < minimumHeight? minimumHeight: _height
    Rectangle {
        width: parent.width
        height: minimumHeight
        color: root.color
        Text {
            text: name.length? name: qsTr("未命名片段")
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width
            color: Constants.contentColorOnBackground(root.color)
            font.family: Constants.font
        }
    }
}
