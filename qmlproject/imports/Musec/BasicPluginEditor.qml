import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Window {
    id: root
    width: 400
    height: 400
    color: Constants.backgroundColor
//    property alias parameterListModel: parameterListView.model
    property string pluginName: "Plugin name here"
    Column {
        Item {
            id: titleAndParamCount
            width: root.width
            height: 30
            Text {
                width: root.width - paramCountText.width
                anchors.left: parent.left
                anchors.leftMargin: (parent.height - contentHeight) / 2
                anchors.verticalCenter: parent.verticalCenter
                text: root.pluginName
                font.family: Constants.font
                color: Constants.contentColor1
                font.pixelSize: parent.height * 0.6
                elide: Text.ElideRight
            }
            Text {
                id: paramCountText
                anchors.right: parent.right
                anchors.rightMargin: (parent.height - contentHeight) / 2
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("# of Adjustable parameters") + ":" + parameterListView.count
                font.family: Constants.font
                color: Constants.contentColor1
                font.pixelSize: parent.height * 0.6
            }
            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 1
                color: Constants.borderColor
            }
        }
        Item {
            width: root.width
            height: root.height - titleAndParamCount.height
            clip: true
            ListView {
                id: parameterListView
                spacing: 5
                anchors.fill: parent
                model: ListModel {
                    ListElement {
                        parameterId: 1
                        parameterName: "Continuous Parameter"
                        shortParameterName: "Cont."
                        continuous: true
                    }
                    ListElement {
                        parameterId: 2
                        parameterName: "Discrete Parameter (10 scales, display as horizontal fader)"
                        shortParameterName: "Disc. 10 fader"
                        continuous: false
                        stepCount: 10
                        showAsComboBox: false
                    }
                    ListElement {
                        parameterId: 3
                        parameterName: "Discrete Parameter (10 scales, display as combo box)"
                        shortParameterName: "Disc. 10 combo"
                        continuous: false
                        stepCount: 10
                        showAsComboBox: true
                    }
                    ListElement {
                        parameterId: 4
                        parameterName: "Discrete Parameter (Switch)"
                        shortParameterName: "Disc. Switch"
                        continuous: false
                        parameterIsSwitch: true
                    }
                }
                ScrollBar.vertical: ScrollBar {
                    width: 15
                }

                delegate: Item {
                    id: delegate
                    width: root.width
                    height: 20
                    Item {
                        id: left
                        width: parent.width * 0.75
                        height: parent.height
                        onWidthChanged: {
                            paramText.getDisplayText();
                        }
                        Text {
                            id: paramText
                            function getDisplayText() {
                                text = parameterName;
                                if(contentWidth > parent.width) {
                                    text = shortParameterName;
                                }
                            }
                            Component.onCompleted: {
                                getDisplayText();
                            }
                            anchors.left: parent.left
                            anchors.leftMargin: (parent.height - contentHeight) / 2
                            font.family: Constants.font
                            color: Constants.contentColor1
                        }
                    }
                    Item {
                        id: right
                        width: delegate.width - left.width
                        anchors.right: parent.right
                        height: parent.height
                        Slider {
                            anchors.fill: parent
                            anchors.rightMargin: 2
                            visible: continuous || (!parameterIsSwitch)
                            snapMode: continuous? Slider.NoSnap: Slider.SnapAlways
                            stepSize: continuous? 0.0: 1.0 / stepCount
                            handle.width: 10
                            handle.height: 16
                            leftPadding: 0
                            rightPadding: 0
                        }
                        MCtrl.ComboBox {
                            anchors.fill: parent
                            anchors.rightMargin: 2
                            visible: (!continuous) && showAsComboBox
                            model: stepCount
                        }
                        MCtrl.Button {
                            anchors.fill: parent
                            anchors.rightMargin: 2
                            visible: (!continuous) && parameterIsSwitch
                        }
                    }
                }
            }
        }
    }
}
