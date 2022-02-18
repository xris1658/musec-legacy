import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2

QQC2.SplitView {
    property color handleColor: "#FFFFFF"
    property real handleOpacity: 0.0
    handle: Rectangle {
        implicitWidth: 5
        implicitHeight: 5
        color: handleColor
        opacity: handleOpacity
    }
}
