import QtQuick 2.15
import QtQuick.Controls 2.15
import Musec 1.0
import Musec.Controls 1.0 as MCtrl

Item {
    id: root
    width: 628
    height: 54
    property int itemSpacing: 3
    property int cpu: 4
    Rectangle {
        id: bigClock
        anchors.fill: parent
        color: Constants.bigClockColor
        radius: 5
        Row {
            id: bigClockItems
            anchors.centerIn: parent
            spacing: 9
            Gradient {
                id: separatorGradient
                GradientStop {
                    position: 0
                    color: Constants.bigClockColor
                }
                GradientStop {
                    position: 0.5
                    color: Constants.bigClockTextColor
                }
                GradientStop {
                    position: 1
                    color: Constants.bigClockColor
                }
            }
            Column {
                id: cpuIndicator
                anchors.verticalCenter: parent.verticalCenter
                spacing: 2
                Item {
                    id: cpuIndicatorEntity
                    width: 50
                    height: 30
                    Rectangle {
                        id: cpuIndicatorBackground
                        anchors.bottom: parent.bottom
                        width: 50
                        height: 18
                        color: Constants.backgroundColor
                        z: 0
                        Rectangle {
                            id: cpuIndicatorForeground
                            width: cpu / 100 * cpuIndicatorBackground.width
                            height: parent.height
                            color: Constants.bigClockCpuBackColor
                            z: 1
                        }
                        Text {
                            id: cpuIndicatorText
                            text: cpu + qsTr("%")
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.family: "Noto Sans Mono"
                            font.styleName: "Condensed"
                            font.bold: true
                            font.pointSize: 12
                            color: Constants.contentColor1
                            z: 2
                        }
                    }
                }
                Text {
                    height: 19
                    text: qsTr("CPU")
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: Constants.bigClockAltTextColor
                    font.family: "Noto Sans Mono"
                    font.styleName: "Condensed"
                    font.hintingPreference: Font.PreferNoHinting
                    font.kerning: true
                    font.pointSize: 8
                    font.capitalization: Font.AllUppercase
                }
            }
            Rectangle {
                width: 2
                anchors.verticalCenter: parent.verticalCenter
                height: root.height - 16
                gradient: separatorGradient
            }
            Column {
                id: keyIndicator
                anchors.verticalCenter: parent.verticalCenter
                spacing: 2
                Item {
                    id: keyIndicatorItem
                    width: 50
                    height: 30
                    Item {
                        width: parent.width
                        height: parent.height - 10
                        anchors.bottom: parent.bottom
                        Row {
                            spacing: 2
                            Row {
                                Text {
                                    id: keyIndicatorMain
                                    text: qsTr("D")
                                    font.family: "Noto Sans Mono"
                                    font.styleName: "Condensed SemiBold"
                                    height: 20
                                    font.pointSize: 17
                                    color: Constants.bigClockTextColor
                                    verticalAlignment: Text.AlignBottom
                                    bottomPadding: font.pointSize * -0.4
                                }
                                Text {
                                    id: keyIndicatorSharp
                                    text: qsTr("\u266d")
                                    font.family: "Noto Sans Mono"
                                    font.styleName: "Condensed SemiBold"
                                    height: font.pointSize
                                    font.pointSize: 12
                                    color: Constants.bigClockTextColor
                                    verticalAlignment: Text.AlignBottom
                                    bottomPadding: font.pointSize * -0.4
                                }
                            }
                            Column {
                                anchors.top: parent.top
                                anchors.topMargin: -3
                                spacing: -3
                                Text {
                                    text: qsTr("MAJOR")
                                    width: contentWidth
                                    height: contentHeight - 2
                                    color: Constants.bigClockTextColor
                                    font.family: "Noto Sans Mono"
                                    font.styleName: "Condensed SemiBold"
                                    font.pointSize: 8
                                    verticalAlignment: Text.AlignBottom
                                    bottomPadding: font.pointSize * -0.4
                                    font.capitalization: Font.AllUppercase
                                }
                                Text {
                                    text: qsTr("minor")
                                    height: contentHeight - 2
                                    color: Constants.bigClockAltTextColor
                                    font.family: "Noto Sans Mono"
                                    font.styleName: "Condensed SemiBold"
                                    font.pointSize: 8
                                    verticalAlignment: Text.AlignBottom
                                    bottomPadding: font.pointSize * -0.4
                                    font.capitalization: Font.AllLowercase
                                }
                            }
                        }
                    }
                }
                Text {
                    height: 19
                    text: qsTr("KEY")
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: Constants.bigClockAltTextColor
                    font.family: "Noto Sans Mono"
                    font.styleName: "Condensed"
                    font.hintingPreference: Font.PreferNoHinting
                    font.kerning: false
                    font.pointSize: 8
                    font.capitalization: Font.AllUppercase
                }
            }
            Rectangle {
                width: 2
                anchors.verticalCenter: parent.verticalCenter
                height: parent.parent.height - 18
                gradient: separatorGradient
            }
            Column {
                id: loopIndicator
                anchors.verticalCenter: parent.verticalCenter
                spacing: 2
                Item {
                    id: loopIndicatorEntity
                    width: 50
                    height: 30
                    Item {
                        width: parent.width
                        height: 20
                        anchors.bottom: parent.bottom
                        Text {
                            anchors.top: parent.top
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: qsTr("0001:00.00")
                            color: Constants.bigClockTextColor
                            font.family: "Noto Sans Mono"
                            font.styleName: "Condensed SemiBold"
                            font.pointSize: 8
                            font.capitalization: Font.AllUppercase
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignTop
                            topPadding: -3
                        }
                        Text {
                            anchors.bottom: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: qsTr("0005:00.00")
                            color: Constants.bigClockTextColor
                            font.family: "Noto Sans Mono"
                            font.styleName: "Condensed SemiBold"
                            font.pointSize: 8
                            font.capitalization: Font.AllUppercase
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignBottom
                            bottomPadding: -3
                        }
                    }
                }
                Text {
                    height: 19
                    text: qsTr("LOOP")
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: Constants.bigClockAltTextColor
                    font.family: "Noto Sans Mono"
                    font.styleName: "Condensed"
                    font.hintingPreference: Font.PreferNoHinting
                    font.pointSize: 8
                    font.capitalization: Font.AllUppercase
                }
            }
            Rectangle {
                width: 2
                anchors.verticalCenter: parent.verticalCenter
                height: root.height - 16
                gradient: separatorGradient
            }
            Column {
                id: timeIndicator
                anchors.verticalCenter: parent.verticalCenter
                spacing: 2
                Item {
                    width: textTime.contentWidth
                    height: 30
                    Text {
                        id: textTime
                        height: font.pointSize
                        text: qsTr("0001:03.00")
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 1
                        font.family: "Noto Sans Mono"
                        font.styleName: "Condensed SemiBold"
                        font.pointSize: 22
                        font.capitalization: Font.AllUppercase
                        color: Constants.bigClockTextColor
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                Text {
                    width: textTime.width
                    height: 19
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: Constants.bigClockAltTextColor
                    horizontalAlignment: Text.AlignHCenter
                    ListModel {
                        id: textTimeFormat
                        ListElement {
                            name: qsTr("BAR:BEAT.STEP \u25BC")
                        }
                        ListElement {
                            name: qsTr("HH:MM:SS.mmm \u25BC")
                        }
                        ListElement {
                            name: qsTr("SAMPLE \u25BC")
                        }
                    }
                    text: textTimeFormat.get(comboTimeFormat.currentIndex).name
                    font.family: "Noto Sans Mono"
                    font.styleName: "Condensed"
                    font.hintingPreference: Font.PreferNoHinting
                    font.pointSize: 8
                    elide: Text.ElideMiddle
                    MCtrl.ComboBox {
                        id: comboTimeFormat
                        anchors.fill: parent
                        opacity: 0
                        font.family: Constants.font
                        model: ["节拍", "十进制", "采样"]
                        currentIndex: 0
                    }
                }
            }
            Rectangle {
                width: 2
                anchors.verticalCenter: parent.verticalCenter
                height: parent.parent.height - 16
                gradient: separatorGradient
            }
            Item {
                width: tempoIndicator.width
                height: 30
                anchors.verticalCenter: parent.verticalCenter
                Column {
                    id: tempoIndicator
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 2
                    Row {
                        id: tempoIndicatorEntity
                        spacing: -1
                        Text {
                            id: tempoIndicatorBeforePoint
                            anchors.bottom: parent.bottom
                            text: qsTr("128")
                            color: Constants.bigClockTextColor
                            font.family: "Noto Sans Mono"
                            font.styleName: "Condensed SemiBold"
                            font.pointSize: 17
                            verticalAlignment: Text.AlignBottom
                            bottomPadding: font.pointSize * -0.4
                        }
                        Text {
                            id: tempoIndicatorPoint
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 1
                            text: qsTr(".")
                            color: Constants.bigClockTextColor
                            font.family: "Noto Sans Mono"
                            font.styleName: "Condensed SemiBold"
                            font.pointSize: 12
                            verticalAlignment: Text.AlignBottom
                            bottomPadding: font.pointSize * -0.4 - 1
                        }
                        Text {
                            id: tempoIndicatorAfterPoint
                            anchors.bottom: parent.bottom
                            text: qsTr("000")
                            color: Constants.bigClockTextColor
                            font.family: "Noto Sans Mono"
                            font.styleName: "Condensed SemiBold"
                            font.pointSize: 8
                            verticalAlignment: Text.AlignBottom
                            bottomPadding: font.pointSize * -0.4
                        }
                    }
                    Text {
                        height: 19
                        text: qsTr("TEMPO")
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: Constants.bigClockAltTextColor
                        font.family: "Noto Sans Mono"
                        font.styleName: "Condensed"
                        font.hintingPreference: Font.PreferNoHinting
                        font.pointSize: 8
                        font.capitalization: Font.AllUppercase
                    }
                }
            }
            Rectangle {
                width: 2
                anchors.verticalCenter: parent.verticalCenter
                height: parent.parent.height - 18
                gradient: separatorGradient
            }
            Column {
                id: timeSignatureIndicator
                anchors.verticalCenter: parent.verticalCenter
                spacing: 2
                Item {
                    width: 45
                    height: 30
                    Item {
                        id: timeSignatureIndicatorEntity
                        width: 45
                        height: 20
                        anchors.bottom: parent.bottom
                        Text {
                            id: timeSignatureIndicatorNumerator
                            anchors.top: parent.top
                            anchors.topMargin: -4
                            anchors.right: timeSignatureIndicatorSlash.left
                            anchors.rightMargin: 2
                            text: qsTr("4")
                            color: Constants.bigClockTextColor
                            font.family: "Noto Sans Mono"
                            font.styleName: "Condensed"
                            font.bold: true
                            font.pointSize: 12
                        }
                        Text {
                            id: timeSignatureIndicatorSlash
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: qsTr("/")
                            color: Constants.bigClockTextColor
                            font.family: "Noto Sans Mono"
                            font.styleName: "Condensed"
                            font.pointSize: 17
                        }
                        Text {
                            id: timeSignatureIndicatorDenominator
                            anchors.left: timeSignatureIndicatorSlash.right
                            anchors.rightMargin: 0
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: -6
                            text: qsTr("4")
                            color: Constants.bigClockTextColor
                            font.family: "Noto Sans Mono"
                            font.styleName: "Condensed"
                            font.bold: true
                            font.pointSize: 12
                        }
                    }
                }

                Text {
                    height: 19
                    text: qsTr("TIME SIG.")
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: Constants.bigClockAltTextColor
                    font.family: "Noto Sans Mono"
                    font.styleName: "Condensed"
                    font.hintingPreference: Font.PreferNoHinting
                    font.pointSize: 8
                    font.capitalization: Font.AllUppercase
                }
            }
        }
    }
}
