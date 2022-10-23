import QtQuick 2.15

import Musec 1.0
import Musec.Controls 1.0 as MCtrl

import Musec.Models 1.0 as MModel

MCtrl.Slider {
    id: control
    orientation: Qt.Vertical
    property MModel.VolumeFaderScaleModel model: MModel.VolumeFaderScaleModel {
        maxDecibel: 6
        minDecibel: -144
    }
    Repeater {
        id: repeater
        model: control.model
        Rectangle {
            parent: control
            anchors.right: parent.right
            width: parent.width
            height: 1
            color: Constants.borderColor
            y: repeater.model.position * control.height
        }
    }
}
