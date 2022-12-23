import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

import Musec 1.0
import Musec.Controls 1.0 as MCtrl
import Musec.Models 1.0 as MModel

Window {
    id: root
    width: 400
    height: 400
    color: Constants.backgroundColor
    property MModel.PluginParameterListModel parameterListModel: null
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
                text: (parameterListModel.parameterCount()? parameterListModel.parameterCount(): 0) + qsTr(" parameters")
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
                model: root.parameterListModel
                ScrollBar.vertical: ScrollBar {
                    width: 15
                }
                delegate: Item {
                    id: delegate
                    width: root.width
                    height: 20
                    Item {
                        id: left_
                        width: parent.width * 0.75
                        height: parent.height
                        onWidthChanged: {
                            paramText.getDisplayText();
                        }
                        Text {
                            id: paramText
                            function getDisplayText() {
                                text = name;
                                if(contentWidth > parent.width) {
                                    text = shortName;
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
                        id: right_
                        width: delegate.width - left_.width
                        anchors.right: parent.right
                        height: parent.height
                        Slider {
                            anchors.fill: parent
                            anchors.rightMargin: 2
                            visible: true
                            snapMode: discrete? Slider.SnapAlways: Slider.NoSnap
                            stepSize: discrete? 1.0 / step: 0.0
                            handle.width: 10
                            handle.height: 16
                            leftPadding: 0
                            rightPadding: 0
                        }
                        MCtrl.ComboBox {
                            anchors.fill: parent
                            anchors.rightMargin: 2
                            visible: false
                            model: step
                        }
                        Item {
                            anchors.fill: parent
                            anchors.rightMargin: 2
                            visible: false
                            MCtrl.Switch {
                                anchors.right: parent.right
                                height: parent.height
                                width: height * 2
                            }
                        }
                    }
                }
            }
        }
    }
}
