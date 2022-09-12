import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2
import Musec 1.0
import Musec.Controls 1.0 as MCtrl

QQC2.Menu {
    id: root
    implicitWidth: 200
    delegate: MCtrl.MenuItem {}
    background: Rectangle {
        color: Constants.menuBackgroundColor
        border.color: Constants.borderColor
    }
    Component.onCompleted: {
        var textWidth = 0;
        var shortcutWidth = 0;
        for(var i = 0; i < root.width; ++i) {
            var item = root.itemAt(i);
            if(item != null && item.mainTextWidth != null) {
                textWidth = Math.max(textWidth, item.mainTextWidth());
            }
            if(item != null && item.keyTextWidth != null) {
                shortcutWidth = Math.max(shortcutWidth, item.keyTextWidth());
            }
        }
        var menuFitWidth = textWidth + shortcutWidth + 2 * 20 + 30;
        root.width = menuFitWidth;
        for(var i = 0; i < root.width; ++i) {
            var item = root.itemAt(i);
            if(item != null) {
                item.width = menuFitWidth;
            }
        }
    }
}
