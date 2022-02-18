import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2
import Musec 1.0

QQC2.Button {
    property alias border: _background.border
    property alias color: _background.color
    id: control
    text: ""
    contentItem: Text {
        text: control.text.indexOf('&')>=0?
                  control.text.split('&')[0] + control.text.split('&')[1].substring(0, 1) + control.text.split('&')[1].substring(1, control.text.split('&')[1].length)
                : control.text
        font: Constants.font
        opacity: enabled? 1.0 : 0.5
        color: Constants.contentColor1
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
    background: Rectangle {
        id: _background
        anchors.fill: parent
        opacity: enabled? 1 : 0.5
        border.width: 1
        border.color: Constants.mouseOverElementColor
        color: control.down? Qt.darker(Constants.backgroundColor, 1.25): control.hovered? Constants.mouseOverElementColor: Constants.backgroundColor
    }
}
