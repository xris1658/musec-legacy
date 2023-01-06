import QtQml.Models 2.15

ListModel {
    property double maxDecibel: 6
    property double minDecibel: -144
    function insertPoint(position: double, decibel: double) {
        //
    }
    function removePoint(index: int) {
        //
    }
    function decibelFromPosition(position: double) {
        if(position <= 0) {
            return minDecibel;
        }
        if(position >= 1) {
            return maxDecibel;
        }
        if(count == 0) {
            return minDecibel + (maxDecibel - minDecibel) * position;
        }
        // TODO
        for(let i = 0; i < count; ++i) {
            //
        }
    }
    function positionFromDecibel(decibel: double) {
        //
    }

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
