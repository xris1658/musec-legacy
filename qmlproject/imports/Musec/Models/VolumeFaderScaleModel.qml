import QtQml.Models 2.15

ListModel {
    property double maxDecibel: 6
    property double minDecibel: -144
    ListElement {
        position: 0.9
        decibel: 0
    }
    ListElement {
        position: 0.8
        decibel: -6
    }
    ListElement {
        position: 0.7
        decibel: -12
    }
    ListElement {
        position: 0.6
        decibel: -18
    }
    ListElement {
        position: 0.5
        decibel: -24
    }
    ListElement {
        position: 0.4
        decibel: -36
    }
    ListElement {
        position: 0.3
        decibel: -54
    }
    ListElement {
        position: 0.2
        decibel: -78
    }
    ListElement {
        position: 0.1
        decibel: -108
    }
}