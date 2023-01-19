import QtQuick 2.15
import QtQuick.Controls 2.15

import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Rectangle {
    id: root
    width: 500
    height: 500
    clip: true
    color: Constants.backgroundColor
    property int firstColumnWidth: 100
    property int secondColumnWidth: 320
    Grid {
        columns: 2
        columnSpacing: 10
        rowSpacing: 5
        verticalItemAlignment: Grid.AlignVCenter
        horizontalItemAlignment: Grid.AlignLeft
        Text {
            width: firstColumnWidth
            height: 20
            text: qsTr("Automation")
            font.family: Constants.font
            color: Constants.contentColor2
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Item {
            width: secondColumnWidth
            height: 20
        }
        Text {
            width: firstColumnWidth
            text: qsTr("Moving point behavior")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
        MCtrl.ComboBox {
            id: comboBoxAutomationMovingPointBehavior
            width: secondColumnWidth
            model: [qsTr("Limit"), qsTr("Leap"), qsTr("Erase")]
        }
        Text {
            width: firstColumnWidth
            height: 20
            text: qsTr("Grid")
            font.family: Constants.font
            color: Constants.contentColor2
            horizontalAlignment: Text.AlignLeft
            leftPadding: 5
            verticalAlignment: Text.AlignVCenter
        }
        Item {
            width: secondColumnWidth
            height: 20
        }
        Text {
            width: firstColumnWidth
            text: qsTr("Linear scale unit")
            font.family: Constants.font
            color: Constants.contentColor1
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
        MCtrl.ComboBox {
            id: comboBoxColor
            model: [qsTr("Time"), qsTr("Beat")]
            font.family: Constants.font
            width: secondColumnWidth
        }
    }
}
