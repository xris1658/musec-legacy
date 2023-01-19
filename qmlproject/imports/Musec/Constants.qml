pragma Singleton
import QtQuick 2.15
import Qt.labs.platform 1.1

QtObject {
    readonly property int width: 1600
    readonly property int height: 900

    property QtObject myFont: QtObject { property string name: "Noto Sans CJK SC" }
    property QtObject myFontBold: QtObject { property string name: "Noto Sans CJK SC Bold" }
    property QtObject myMonoFont: QtObject { property string name: "Noto Sans Mono Condensed" }

    readonly property string font: myFont.name
    // General
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

    // Timeline
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

    readonly property color bigClockColor:                "#001F1F"
    readonly property color bigClockTextColor:            "#00FF00"
    readonly property color bigClockAltTextColor:         "#00A000"
    readonly property color bigClockCpuForeColor:         "#0088FF"
    readonly property color bigClockCpuOverloadForeColor: "#FF0000"
    readonly property color bigClockCpuBackColor:         Constants.backgroundColor
    readonly property color bigClockCpuOffBackColor:      "#800000"

    // Big clock
//    readonly property color bigClockColor:         "#18181F"
//    readonly property color bigClockTextColor:     "#00FFFF"
//    readonly property color bigClockAltTextColor:  "#00A0A0"

    // Track controls (mute, solo, invert phase, arm recording)
    readonly property color muteIndicatorColor:    "#F39700"
    readonly property color soloIndicatorColor:    "#007EFF"
    readonly property color invertIndicatorColor:  "#336633"
    readonly property color recordIndicatorColor:  "#FF0000"

    // Arrangement
    readonly property color automationColor:       "#EC6941"

    // Peak level > 0
    readonly property color peakAboveZeroColor:    "#F39700"

    // Indicates that a device is enabled or disabled
    readonly property color deviceEnabledColor:    "#F39700"
    readonly property color deviceDisabledColor:   "#505050"

    // dB meter
    readonly property color belowNeg6dBColor: "#00FF00"
    readonly property color below0dBColor:    "#FFFF00"
    readonly property color above0dBColor:    "#FF0000"
    readonly property color levelBackColor:   "#003300"

    // note
    readonly property color noteBackgroundColor: "#80C269"

    // Menu item
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

    readonly property string okText:                    qsTr("OK")
    readonly property string okTextWithMnemonic:        qsTr("&OK")
    readonly property string cancelText:                qsTr("Cancel")
    readonly property string cancelTextWithMnemonic:    qsTr("&Cancel")
    readonly property string yesText:                   qsTr("Yes")
    readonly property string yesTextWithMnemonic:       qsTr("&Yes")
    readonly property string noText:                    qsTr("No")
    readonly property string noTextWithMnemonic:        qsTr("&No")
    readonly property string yesToAll:                  qsTr("Yes to All")
    readonly property string yesToAllWithMnemonic:      qsTr("Yes to &All")
    readonly property string noToAll:                   qsTr("No to All")
    readonly property string noToAllWithMnemonic:       qsTr("N&o to All")
    readonly property string undoText:                  qsTr("Undo")
    readonly property string undoTextWithMnemonic:      qsTr("&Undo")
    readonly property string redoText:                  qsTr("Redo")
    readonly property string redoTextWithMnemonic:      qsTr("&Redo")
    readonly property string cutText:                   qsTr("Cut")
    readonly property string cutTextWithMnemonic:       qsTr("Cu&t")
    readonly property string copyText:                  qsTr("Copy")
    readonly property string copyTextWithMnemonic:      qsTr("&Copy")
    readonly property string pasteText:                 qsTr("Paste")
    readonly property string pasteTextWithMnemonic:     qsTr("&Paste")
    readonly property string deleteText:                qsTr("Delete")
    readonly property string deleteTextWithMnemonic:    qsTr("&Delete")
    readonly property string selectAllText:             qsTr("Select All")
    readonly property string selectAllTextWithMnemonic: qsTr("Select &All")
    readonly property string duplicateText:             qsTr("Duplicate")
    readonly property string duplicateTextWithMnemonic: qsTr("&Duplicate")
    readonly property string renameText:                qsTr("Rename")
    readonly property string renameTextWithMnemonic:    qsTr("Rena&me")

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
            description: qsTr("Low latency audio driver")
        }
        ListElement {
            name: "FFmpeg"
            homepage: "https://www.ffmpeg.org/"
            description: qsTr("Library for manipulating multimedia files")
        }
        ListElement {
            name: "Avcpp"
            homepage: "https://github.com/h4tr3d/avcpp"
            description: qsTr("C++ wrapper of FFmpeg")
        }
        ListElement {
            name: "Inno Setup"
            homepage: "https://jrsoftware.org/isinfo.php"
            description: qsTr("Installer framework")
        }
        ListElement {
            name: "LAME"
            homepage: "https://lame.sourceforge.io/"
            description: qsTr("MP3 audio encoder")
        }
        ListElement {
            name: "Qt"
            homepage: "https://www.qt.io/"
            description: qsTr("Cross-platform application development toolkit")
        }
        ListElement {
            name: "The SoX Resampler library"
            homepage: "https://sourceforge.net/p/soxr/wiki/Home/"
            description: qsTr("Audio resampler")
        }
        ListElement {
            name: "spdlog"
            homepage: "https://github.com/gabime/spdlog"
            description: qsTr("Logger")
        }
        ListElement {
            name: "{fmt}"
            homepage: "https://fmt.dev"
            description: qsTr("Library for text formatting, dependency of spdlog")
        }
        ListElement {
            name: "SQLite"
            homepage: "https://www.sqlite.org/index.html"
            description: qsTr("Light-weight, efficient database engine")
        }
        ListElement {
            name: "sqlite-modern-cpp"
            homepage: "https://github.com/aminroosta/sqlite_modern_cpp"
            description: qsTr("Modern C++ wrapper of SQLite")
        }
        ListElement {
            name: "VST"
            homepage: "https://www.steinberg.net/developers/"
            description: qsTr("Widely used audio plugin format")
        }
        ListElement {
            name: "CLAP"
            homepage: "https://cleveraudio.org"
            description: qsTr("Free audio plugin format")
        }
        ListElement {
            name: "yaml-cpp"
            homepage: "https://github.com/jbeder/yaml-cpp/"
            description: qsTr("YAML parser and emitter")
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
        // QML JS engine does NOT support `String.prototype.replaceAll`
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

    function contentColorOnBackground(color: color) {
        return color.r * 0.299 + color.g * 0.587 + color.b * 0.514 > 0.6? "#000000": "#FFFFFF"
    }
}
