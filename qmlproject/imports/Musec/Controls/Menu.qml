import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2
import Musec 1.0
import Musec.Controls 1.0 as MCtrl

QQC2.Menu {
    id: root
    implicitWidth: 200
    property int gapWidth: 50
    delegate: MCtrl.MenuItem {}
    background: Rectangle {
        color: Constants.menuBackgroundColor
        border.color: Constants.borderColor
    }
    Component.onCompleted: {
        let fitWidth = 0;
        for(let i = 0; i < root.count; ++i) {
            let item = root.itemAt(i);
            if(item != null && item.mainTextWidth != null && item.keyTextWidth != null) {
                fitWidth = Math.max(fitWidth, item.mainTextWidth() + item.keyTextWidth() + 2 * 20 /*check box + arrow*/ + gapWidth);
            }
        }
        root.implicitWidth = fitWidth;
        for(let i = 0; i < root.count; ++i) {
            let item = root.itemAt(i);
            if(item != null) {
                item.implicitWidth = fitWidth;
            }
        }
    }
}
