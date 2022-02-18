import QtQuick 2.15
import QtQml.Models 2.15

Item {
    ListModel {
        id: valueList
        ListElement {
            time: 0
            value: 0.0
        }
        ListElement {
            time: 44100
            value: 1.0
        }
    }
    ListModel {
        id: lineList
        ListElement {
            curve: 0
        }
    }

    ListView {
        model: valueList
        delegate: ItemDelegate {
            //
        }
    }
}
