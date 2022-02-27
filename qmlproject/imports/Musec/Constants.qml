pragma Singleton
import QtQuick 2.15
import Qt.labs.platform 1.1

QtObject {
    readonly property int width: 1600
    readonly property int height: 900

    readonly property FontLoader myFont: FontLoader { name: "Noto Sans CJK SC" }
    readonly property FontLoader myMonoFont: FontLoader { name: "Noto Sans Mono Condensed" }

    property alias fontDirectory: directoryFontLoader.fontDirectory
    property alias relativeFontDirectory: directoryFontLoader.relativeFontDirectory

    readonly property font font: Qt.font({
                                             family: myFont.name,
                                             pixelSize: Qt.application.font.pixelSize
                                         })
    // 通用颜色
    readonly property color backgroundColor:       "#333333"
    readonly property color borderColor:           "#666666"
    readonly property color gridColor:             "#151515"
    readonly property color backgroundColor2:      "#2A2A2A"
    readonly property color linkColor:             "#0A7AF4"
    readonly property color mouseOverElementColor: "#505050"
    readonly property color currentElementColor:   "#CCCCCC" // "#727272"
    readonly property color contentColor1:         "#FFFFFF"
    readonly property color contentColor2:         "#AFAFAF"
    readonly property color warningTextColor:      "#FFBB00"

    // 时间线颜色
    readonly property color timelineBackgroundColor: "#000000"
    readonly property color timelineForegroundColor: Constants.borderColor

    readonly property color exitFullscreenButtonBackgroundColor: "#FF5546"

//    readonly property color backgroundColor:       "#EEEEEE"
//    readonly property color borderColor:           "#666666"
//    readonly property color backgroundColor2:      "#CCCCCC"
//    readonly property color mouseOverElementColor: "#AAAAAA"
//    readonly property color currentElementColor:   "#666666"
//    readonly property color contentColor1:         "#000000"
//    readonly property color contentColor2:         "#505050"

    readonly property color bigClockColor:         "#001F1F"
    readonly property color bigClockTextColor:     "#00FF00"
    readonly property color bigClockAltTextColor:  "#00A000"
    readonly property color bigClockCpuBackColor:  "#EB6100"

    //顶部大号时钟颜色
//    readonly property color bigClockColor:         "#18181F"
//    readonly property color bigClockTextColor:     "#00FFFF"
//    readonly property color bigClockAltTextColor:  "#00A0A0"

    //轨道控制颜色（静音、独奏、反相、接入录制）
    readonly property color muteIndicatorColor:    "#F39700"
    readonly property color soloIndicatorColor:    "#007EFF"
    readonly property color invertIndicatorColor:  "#336633"
    readonly property color recordIndicatorColor:  "#FF0000"

    //轨道峰值电平 > 0
    readonly property color peakAboveZeroColor:    "#F39700"

    //设备启用指示器的颜色
    readonly property color deviceEnabledColor:    "#F39700"
    readonly property color deviceDisabledColor:   "#505050"

    //电平
    readonly property color belowNeg6dBColor: "#00FF00"
    readonly property color below0dBColor:    "#FFFF00"
    readonly property color above0dBColor:    "#FF0000"
    readonly property color levelBackColor:   "#003300"

    //音符
    readonly property color noteBackgroundColor: "#80C269"

    // 菜单项颜色
//    readonly property color menuBackgroundColor:           "#FFFFFF"
//    readonly property color menuSeparatorColor:            "#666666" // Constants.borderColor
//    readonly property color menuContentColor:              "#000000"
//    readonly property color menuHighlightBackgroundColor:  "#0078D7"
//    readonly property color menuHighlightBorderColor:      "#12B7F5"
//    readonly property color menuHighlightContentColor:     "#FFFFFF"

    readonly property color menuBackgroundColor:           Constants.backgroundColor
    readonly property color menuSeparatorColor:            Constants.borderColor
    readonly property color menuContentColor:              Constants.contentColor1
    readonly property color menuHighlightBackgroundColor:  Qt.lighter(Constants.backgroundColor)
    readonly property color menuHighlightBorderColor:      Constants.borderColor
    readonly property color menuHighlightContentColor:     Constants.contentColor1

    readonly property color listHighlightBackColor:        Constants.currentElementColor
    readonly property color listHighlightContentColor:     "#000000"

    readonly property ListModel dynamicKeyName: ListModel {
        ListElement {
            flat: "C"
            sharp: "C"
        }
        ListElement {
            flat: "D\u266d"
            sharp: "C\u266f"
        }
        ListElement {
            flat: "D"
            sharp: "D"
        }
        ListElement {
            flat: "E\u266d"
            sharp: "D\u266f"
        }
        ListElement {
            flat: "E"
            sharp: "E"
        }
        ListElement {
            flat: "F"
            sharp: "F"
        }
        ListElement {
            flat: "G\u266d"
            sharp: "F\u266f"
        }
        ListElement {
            flat: "G"
            sharp: "G"
        }
        ListElement {
            flat: "A\u266d"
            sharp: "G\u266f"
        }
        ListElement {
            flat: "A"
            sharp: "A"
        }
        ListElement {
            flat: "B\u266d"
            sharp: "A\u266f"
        }
        ListElement {
            flat: "B"
            sharp: "B"
        }
    }

    readonly property ListModel technicalSupport: ListModel {
        ListElement {
            name: "ASIO"
            homepage: "https://www.steinberg.net/developers/"
            description: qsTr("低延迟的音频驱动")
        }
        ListElement {
            name: "FFmpeg"
            homepage: "https://www.ffmpeg.org/"
            description: qsTr("用于操作多媒体文件的跨平台库")
        }
        ListElement {
            name: "Inno Setup"
            homepage: "https://jrsoftware.org/isinfo.php"
            description: qsTr("安装程序框架")
        }
        ListElement {
            name: "LAME"
            homepage: "https://lame.sourceforge.io/"
            description: qsTr("MP3 音频编码器")
        }
        ListElement {
            name: "Qt"
            homepage: "https://www.qt.io/"
            description: qsTr("跨平台应用开发工具")
        }
        ListElement {
            name: "SoX"
            homepage: "http://sox.sourceforge.net/Main/HomePage"
            description: qsTr("音频重采样工具")
        }
        ListElement {
            name: "SQLite"
            homepage: "https://www.sqlite.org/index.html"
            description: qsTr("轻量，高效的数据库引擎")
        }
        ListElement {
            name: "VST"
            homepage: "https://www.steinberg.net/developers/"
            description: qsTr("广泛使用的音频插件架构")
        }
        ListElement {
            name: "yaml-cpp"
            homepage: "https://github.com/jbeder/yaml-cpp/"
            description: qsTr("YAML 解析与生成工具")
        }
    }

    readonly property ListModel noteName: ListModel {
        ListElement {
            name: "\u266f"
            asciiName: "#"
        }
        ListElement {
            name: "\u266d"
            asciiName: "b"
        }
    }

    function blackKey(midiNote) {
        var mod = midiNote % 12;
        return mod == 1
             | mod == 3
             | mod == 6
             | mod == 8
             | mod == 10;
    }

    function directoryFromUrl(directory: string) {
        let ret = String(directory).slice(8); // file:///
        // QML JS 引擎不支持 String.prototype.replaceAll
        while (ret.indexOf('/') != -1) {
            ret = ret.replace('/', '\\');
        }
        return ret;
    }

    function urlFromDirectory(directory: string) {
        while(directory.indexOf("\\") != -1) {
            directory = directory.replace("\\", "/");
        }
        let ret = String("file:///").concat(directory);
        return ret;
    }

    enum NoteName { Sharp, Flat }
    readonly property int midiValueRange: 128
    readonly property int maxVelocity: midiValueRange - 1
    readonly property int minVelocity: 0
    property int titleTextPointSize: 8

    property DirectoryFontLoader directoryFontLoader: DirectoryFontLoader {
        id: directoryFontLoader
    }
}
