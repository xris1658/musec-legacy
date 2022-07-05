import QtQuick.Window 2.15

import Musec 1.0
import Musec.Controls 1.0

Window {
    visible: true
    width: 900
    height: 300
    title: "AutomationTest"
    property alias automationModel: automation.model
    Automation {
        id: automation
        width: 900
        height: 50
        timeUnitWidth: 0.5 + slider.value * 5.5
    }
    Slider {
    id: slider
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter
        width: 100
        value: 0.0
    }
}