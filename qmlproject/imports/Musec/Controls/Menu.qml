import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2
import Musec 1.0
import Musec.Controls 1.0 as MCtrl

QQC2.Menu {
    delegate: MCtrl.MenuItem {}
    background: Rectangle {
        color: Constants.menuBackgroundColor
        border.color: Constants.borderColor
    }
}
