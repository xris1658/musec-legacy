import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import QtQuick.Window 2.15
import Qt.labs.platform 1.1 as Labs

import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Item {
    id: root
    clip: true
    enum ShowMasterTrack {
        Hide,
        Top,
        Bottom
    }
    property alias timelineNumerator: timeline.numerator
    property int showMasterTrack: Arrangement.ShowMasterTrack.Hide
    readonly property alias showMasterTrackModel: showMasterTrackButton.model
    property int masterTrackHeight: 60
    property bool loop: false
    property ListModel tracks
    property alias arrangementSnapUnit: editorSnapUnit
    property int barCount: 20
    // Qt 6 中移除了 QtQuick.Dialogs 中的 ColorDialog 和 MessageDialog,
    // 此处使用 Qt.labs.platform 中的 ColorDialog 以保证向后兼容.
    // 讲个笑话: Types in Qt.labs modules are not guaranteed to remain
    // compatible in future versions. - Qt Documentation
    property Labs.ColorDialog colorDialog: Labs.ColorDialog {
        property TrackHeader colorDest1
        color: Constants.backgroundColor
        title: qsTr("选择颜色")
        modality: Qt.WindowModal
//        showAlphaChannel: false
        onAccepted: {
            tracks.get(colorDest1.trackIndex - 1).color = color;
        }
    }

    MCtrl.Menu {
        id: arrangementOptions
        title: qsTr("编排视图操作")
        delegate: MCtrl.MenuItem {}
        width: 200
        implicitHeight: 20
        height: contentHeight
        MCtrl.Action {
            text: qsTr("添加 MIDI / 乐器轨道(&M)")
            onTriggered: {
                var newTrack = {
                    'name': 'MIDI',
                    'color': Qt.rgba(Math.random(), Math.random(), Math.random()),
                    'mute': false,
                    'solo': false,
                    'armRecord': false,
                    'selected': false,
                    'trackHeight': 60,
                    'selected': false
                };
                tracks.append(newTrack);
            }
        }
        MCtrl.Action {
            text: qsTr("添加音频轨道(&A)")
            onTriggered: {
                var newTrack = {
                    'name': '音频',
                    'color': Qt.rgba(Math.random(), Math.random(), Math.random()),
                    'mute': 'false',
                    'solo': 'false',
                    'armRecord': 'false',
                    'selected': 'false',
                    'trackHeight': '60',
                    'selected': 'false'
                };
                tracks.append(newTrack);
            }
        }
    }
    MCtrl.Menu {
        id: trackOptions
        property int trackIndex
        property TrackHeader trackHeader
        title: qsTr("轨道操作")
        delegate: MCtrl.MenuItem {}
        width: 200
        implicitHeight: 20
        height: contentHeight
       MCtrl.Action {
            text: qsTr("剪切(&T)")
            shortcut: "Ctrl+X"
        }
        MCtrl.Action {
            text: qsTr("复制(&C)")
            shortcut: "Ctrl+C"
        }
        MCtrl.Action {
            text: qsTr("创建副本(&L)")
            shortcut: "Ctrl+D"
        }
        MCtrl.Action {
            text: qsTr("粘贴(&P)")
            shortcut: "Ctrl+V"
        }
        MCtrl.Action {
            text: qsTr("删除(&D)")
            shortcut: "Delete"
            onTriggered: {
                tracks.remove(trackOptions.trackIndex - 1);
            }
        }
        MCtrl.MenuSeparator {}
        MCtrl.Action {
            text: qsTr("重命名(&R)")
            shortcut: "F2"
            onTriggered: {
                trackOptions.trackHeader.rename();
            }
        }
        MCtrl.Action {
            text: qsTr("颜色(&L)")
            onTriggered: {
                colorDialog.open();
            }
        }
    }

    MCtrl.SplitView {
        id: headerContentSplitView
        anchors.fill: parent
        orientation: Qt.Horizontal
        handle: Item {
            implicitWidth: 6
            implicitHeight: 5
            Rectangle {
                anchors.right: parent.right
                width: 2
                height: parent.height
                color: Constants.gridColor
            }
        }
        Item {
            id: _left
            MCtrl.SplitView.minimumWidth: 150
            MCtrl.SplitView.maximumWidth: 500
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            Rectangle {
                id: lu
                anchors.left: parent.left
                anchors.top: parent.top
                width: parent.width + 4
                height: 40
                z: 3
                color: Constants.backgroundColor
                Rectangle {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 1
                    color: Constants.borderColor
                    z: 2
                }
                MCtrl.ComboBox {
                    id: editorSnapUnit
                    anchors.left: parent.left
                    anchors.top: parent.top
                    width: 60
                    height: parent.height / 2
                    font.family: Constants.font
                    model: ["1/64", "1/32", "1/16", "1/8", "1/4", "1/2", "1/1", "2", "4", "8", "16"]
                    currentIndex: 4
                    color: Constants.mouseOverElementColor
                    border.width: 0
                    z: 1
                    MCtrl.ToolTip {
                        visible: parent.hovered
                        text: qsTr("对齐单位")
                    }
                }
                    MCtrl.Button {
                        id: automationButton
                        property bool automationEnabled: false
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        width: height
                        height: editorSnapUnit.height
                        background: Rectangle {
                            anchors.fill: parent
                            color: automationButton.automationEnabled? Constants.currentElementColor: Constants.backgroundColor
                        }
                        Item {
                            anchors.centerIn: parent
                            width: 12
                            height: 12
                            Rectangle {
                                anchors.left: parent.left
                                anchors.bottom: parent.bottom
                                width: 5
                                height: width
                                radius: width / 2
                                color: automationButton.automationEnabled? Constants.contentColor1: Constants.contentColor2
                                border.color: color
                            }
                            Rectangle {
                                anchors.right: parent.right
                                anchors.top: parent.top
                                width: 5
                                height: width
                                radius: width / 2
                                color: automationButton.automationEnabled? Constants.contentColor1: Constants.contentColor2
                                border.color: color
                            }
                            Shape {
                                width: parent.width
                                height: parent.height
                                antialiasing: true
                                smooth: true
                                ShapePath {
                                    startX: 3
                                    startY: 9
                                    PathLine {
                                        x: 9
                                        y: 3
                                    }
                                    strokeWidth: 1.5
                                    strokeColor: automationButton.automationEnabled? Constants.contentColor1: Constants.contentColor2
                                }
                            }
                        }
                        border.width: 0
                        z: 1
                        MCtrl.ToolTip {
                            visible: parent.hovered
                            text: qsTr("自动化包络")
                        }
                        onClicked:  {
                            automationEnabled = !automationEnabled;
                        }
                    }
                    MCtrl.ComboBox {
                        id: showMasterTrackButton
                        width: height * 3
                        height: editorSnapUnit.height
                        anchors.left: automationButton.right
                        anchors.top: automationButton.top
                        model: ["不显示", "上方", "下方"]
                        currentIndex: showMasterTrack
                        MCtrl.ToolTip {
                            visible: parent.hovered
                            text: qsTr("显示主轨道")
                        }
                    }
            }
            Rectangle {
                id: headers
                anchors.top: lu.bottom
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                width: parent.width + 4
                color: Constants.backgroundColor
                clip: true
                MouseArea {
                    acceptedButtons: Qt.RightButton
                    anchors.fill: parent
                    anchors.margins: 0
                    anchors.rightMargin: 6
                    onPressed: {
                        if(mouse.buttons == Qt.RightButton) {
                            arrangementOptions.x = mouseX;
                            arrangementOptions.y = mouseY + arrangementOptions.height;
                            arrangementOptions.open();
                        }
                    }
                }
                TrackHeader {
                    id: masterTrack
                    trackName: qsTr("Master")
                    parent: showMasterTrackButton.currentIndex == 1? masterTrackHeader: masterTrackFooter
                    height: showMasterTrackButton.currentIndex ? parent.height: 0
                    Rectangle {
                        width: parent.width
                        height: 1
                        x: 0
                        y: showMasterTrackButton.currentIndex == 1? parent.height - 1: 0
                        color: Constants.borderColor
                    }
                }
                Item {
                    id: masterTrackHeader
                    width: parent.width
                    height: showMasterTrackButton.currentIndex == 1? masterTrackHeight: 0
                    anchors.top: parent.top
                    z: 2
                }
                ListView {
                    id: trackHeaderList
                    orientation: Qt.Vertical
//                    anchors.fill: parent
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: masterTrackHeader.bottom
                    anchors.bottom: masterTrackFooter.top
                    model: tracks
                    interactive: false
                    property int minimumBlankHeight: 50
//                    property int blankHeight: height < contentHeight + minimumBlankHeight? contentHeight - height: minimumBlankHeight
                    z: 1
                    clip: true
                    ScrollBar.vertical: ScrollBar {
                        id: vbar
                        parent: vbarPlaceholder
                        width: parent.width
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        background: Rectangle {
                            color: Constants.backgroundColor
                        }
                        contentItem: Rectangle {
                            radius: 2
                            color: Constants.mouseOverElementColor
                            opacity: trackHeaderList.height < trackHeaderList.contentHeight? 1: 0
                        }
                    }
                    delegate: TrackHeader {
                        id: trackHeader
                        trackName: name
                        trackColor: color
                        width: headers.width
                        height: trackHeight
                        trackIndex: index + 1
                        trackSelected: false
                        Rectangle {
                            width: parent.width
                            height: 1
                            x: 0
                            y: parent.height - 1
                            color: Constants.borderColor
                        }
                        MouseArea {
                            id: trackHeaderMouseArea
                            width: parent.width - 3 * 20
                            height: 20
//                            width: parent.width
//                            height: 20
                            acceptedButtons: Qt.LeftButton | Qt.RightButton
                            onClicked: {
                                if(mouse.button == Qt.RightButton) {
                                    trackOptions.parent = trackHeader;
                                    trackOptions.trackIndex = trackHeader.trackIndex;
                                    trackOptions.trackHeader = trackHeader;
                                    colorDialog.colorDest1 = trackHeader;
                                    colorDialog.title = "选择颜色 - 轨道 " + trackHeader.trackIndex + ": " + trackHeader.trackName
                                    colorDialog.color = trackHeader.trackColor;
                                    trackOptions.x = mouseX;
                                    trackOptions.y = mouseY;
                                    trackOptions.open();
                                }
                            }
                        }
                    }
                }
                Item {
                    id: masterTrackFooter
                    width: parent.width
                    height: showMasterTrackButton.currentIndex == 2? masterTrackHeight: 0
                    anchors.bottom: parent.bottom
                    z: 2
                }
            }
        }
        Item {
            id: contentAndVBar
            Item {
                id: _right
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: 15
                Rectangle {
                    id: ru
                    anchors.top: parent.top
                    anchors.left: parent.left
                    width: parent.width
                    height: lu.height
                    z: 3
                    color: Constants.backgroundColor
                }
                Rectangle {
                    id: vbarPlaceholder
                    width: 15
                    anchors.top: ru.bottom
                    anchors.right: parent.right
                    height: headers.height
                    color: Constants.mouseOverElementColor
                }
            }
            Rectangle {
                id: _medium
//                anchors.left: _left.right
                width: parent.width - _right.width
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                color: Constants.backgroundColor2
                Item {
                    id: timelineAndHbar
                    anchors.top: parent.top
                    height: lu.height
                    anchors.left: parent.left
                    width: parent.width
                    z: 1
                    clip: true
                    ScrollBar {
                        id: hbar
                        anchors.left: parent.left
                        width: parent.width
                        anchors.top: parent.top
                        height: 15
                        background: Rectangle {
                            anchors.fill: parent
                            color: timeline.color
                        }
                        contentItem: Rectangle {
                            radius: 2
                            color: Constants.mouseOverElementColor
                        }
                        size: parent.width / timeline.width
                        orientation: Qt.Horizontal
                        active: hovered || pressed
                        position: 0.0
                    }
                    Timeline {
                        id: timeline
                        barCount: root.barCount
                        anchors.bottom: parent.bottom
                        height: parent.height - hbar.height
                        x: -hbar.position * width
                        Rectangle {
                            anchors.bottom: parent.bottom
                            anchors.left: parent.left
                            width: parent.width
                            height: 1
                            color: Constants.borderColor
                        }

                        MouseArea {
                            anchors.fill: parent
                            onWheel: {
                                hbar.position -= 0.1 * wheel.angleDelta.y / 540.0 * (wheel.inverted? -1: 1);
                                if(hbar.position > 1 - hbar.size)
                                {
                                    hbar.position = 1 - hbar.size;
                                }
                                else if(hbar.position < 0)
                                {
                                    hbar.position = 0;
                                }
                            }
                        }
                    }
                }
                Item {
                    id: contentAreaRect
                    anchors.left: parent.left
                    width: parent.width
                    y: headers.y
                    height: root.height - timelineAndHbar.height
                    clip: true
                    MouseArea {
                        anchors.fill: parent
                        scrollGestureEnabled: true
                        onWheel: {
                            if(wheel.modifiers == Qt.NoModifier)
                            {
                                vbar.position += 0.1 * wheel.angleDelta.y / 360.0 * (wheel.inverted? 1: -1);
                                if(vbar.position > 1 - vbar.size)
                                {
                                    vbar.position = 1 - vbar.size;
                                }
                                else if(vbar.position < 0)
                                {
                                    vbar.position = 0;
                                }
                                hbar.position += 0.1 * wheel.angleDelta.x / 540.0 * (wheel.inverted? 1: -1);
                                if(hbar.position > 1 - hbar.size)
                                {
                                    hbar.position = 1 - hbar.size;
                                }
                                else if(hbar.position < 0)
                                {
                                    hbar.position = 0;
                                }
                            }
                            else if(wheel.modifiers & Qt.ShiftModifier)
                            {
                                hbar.position += 0.1 * wheel.angleDelta.y / 540.0 * (wheel.inverted? 1: -1);
                                if(hbar.position > 1 - hbar.size)
                                {
                                    hbar.position = 1 - hbar.size;
                                }
                                else if(hbar.position < 0)
                                {
                                    hbar.position = 0;
                                }
                                vbar.position += 0.1 * wheel.angleDelta.x / 360.0 * (wheel.inverted? 1: -1);
                                if(vbar.position > 1 - vbar.size)
                                {
                                    vbar.position = 1 - vbar.size;
                                }
                                else if(vbar.position < 0)
                                {
                                    vbar.position = 0;
                                }
                            }
                        }
                    }
                    Rectangle {
                        id: contentAreaMaster
                        width: timeline.width
                        x: timeline.x
                        y: showMasterTrackButton.currentIndex == 1?
                            0:
                            masterTrackFooter.y
                        z: 2
                        height: masterTrack.height
                        color: Constants.backgroundColor2
                        Item {
                            anchors.fill: parent
                            Rectangle {
                                y: showMasterTrackButton.currentIndex == 1?
                                    parent.height - 1: 0
                                anchors.left: parent.left
                                width: parent.width
                                height: 1
                                color: Constants.gridColor
                            }
                        }
                        ListView {
                            model: timeline.barCount * timeline.numerator
                            orientation: Qt.Horizontal
                            interactive: false
                            anchors.fill: parent
                            delegate: Item {
                                width: timeline.barWidth / timeline.numerator
                                height: masterTrack.height
                                Rectangle {
                                    anchors.left: parent.left
                                    anchors.top: parent.top
                                    width: 1
                                    height: parent.height
                                    color: Constants.gridColor
                                }
                            }
                        }
                    }
                    Rectangle {
                        id: contentArea
                        width: timeline.width
                        height: contentAreaRect.height > trackHeaderList.contentHeight? contentAreaRect.height: trackHeaderList.contentHeight
                        x: timeline.x
                        y: -(trackHeaderList.contentY) + masterTrackHeader.height
                        z: 1
                        color: Constants.backgroundColor2
                        clip: true
                        ListView {
                            model: tracks
                            orientation: Qt.Vertical
                            interactive: false
                            anchors.fill: parent
                            delegate: Item {
                                width: contentArea.width
                                height: trackHeight
                                Rectangle {
                                    anchors.bottom: parent.bottom
                                    anchors.left: parent.left
                                    width: parent.width
                                    height: 1
                                    color: Constants.gridColor
                                }
                            }
                        }
                        ListView {
                            model: timeline.barCount * timeline.numerator
                            orientation: Qt.Horizontal
                            height: contentAreaRect.height
                            interactive: false
                            anchors.fill: parent
                            delegate: Item {
                                width: timeline.barWidth / timeline.numerator
                                height: contentAreaRect.height > trackHeaderList.contentHeight? contentAreaRect.height: trackHeaderList.contentHeight
                                Rectangle {
                                    anchors.left: parent.left
                                    anchors.top: parent.top
                                    width: 1
                                    height: parent.height
                                    color: Constants.gridColor
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
