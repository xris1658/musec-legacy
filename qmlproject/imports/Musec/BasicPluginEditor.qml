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
    property string pluginName: "此处显示插件名称"
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
                text: qsTr("可调参数数量：") + parameterListView.count
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
                        parameterName: "连续的参数"
                        shortParameterName: "连参"
                        continuous: true
                    }
                    ListElement {
                        parameterId: 2
                        parameterName: "离散的参数（10 格，显示为水平推子）"
                        shortParameterName: "离参 10 推子"
                        continuous: false
                        stepCount: 10
                        showAsComboBox: false
                    }
                    ListElement {
                        parameterId: 3
                        parameterName: "离散的参数（10 格，显示为下拉列表）"
                        shortParameterName: "离参 10 下拉"
                        continuous: false
                        stepCount: 10
                        showAsComboBox: true
                    }
                    ListElement {
                        parameterId: 4
                        parameterName: "离散的参数 (开关)"
                        shortParameterName: "离参 开关"
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
