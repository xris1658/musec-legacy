import QtQuick 2.15

Rectangle {
    property int verLineCount: 20
    property int horLineCount: 20
    property real verLineSpacing: 20
    property real horLineSpacing: 20
    id: root
    width: verLineCount * verLineSpacing
    height: horLineCount * horLineSpacing
    color: Constants.backgroundColor
    clip: true
    //画竖线
    Row {
        spacing: verLineSpacing - 1
        anchors.fill: parent
        z: 2
        Repeater {
            model: verLineCount
            Rectangle {
                width: 1
                height: root.height
                color: Constants.gridColor
            }
        }
    }
    //画横线
    Column {
        spacing: horLineSpacing - 1
        anchors.fill: parent
        z: 2
        Repeater {
            model: horLineCount
            Rectangle {
                width: root.width
                height: 1
                color: Constants.gridColor
            }
        }
    }
}
