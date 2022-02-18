import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root
    property real aboveZero: 0.2
    property real aboveNegSix: 0.2
    property real level: 1.25
    property color backColor: Constants.levelBackColor
    Rectangle {
        anchors.fill: parent
        color: backColor
    }

    Column {
        Item {
            id: aboveZeroPlaceholder
            width: root.width
            height: root.height * root.aboveZero
            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                height: (level - 1) * parent.height
                color: Constants.above0dBColor
            }
        }
        Item {
            id: aboveNegSixPlaceholder
            width: root.width
            height: root.height * root.aboveNegSix
            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                height: level > 1? parent.height : level > 0.5? (2 * level - 1) * parent.height : 0
                color: Constants.below0dBColor
            }
        }
        Item {
            id: belowNegSixPlaceholder
            width: root.width
            height: root.height * (1 - root.aboveZero - root.aboveNegSix)
            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                height: level > 0.5? parent.height : level * 2 * parent.height
                color: Constants.belowNeg6dBColor
            }
        }
    }
}
