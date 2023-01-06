import QtQml.Models 2.15

ListModel {
    ListElement {
        path: "/path/to/en-us"
        language_name: "en_US"
        readable_language_name: "English (US)"
    }
    ListElement {
        path: "/path/to/zh-cn"
        language_name: "zh_CN"
        readable_language_name: "简体中文（中国）"
    }
    ListElement {
        path: "/path/to/pseudo"
        language_name: "pseudo"
        readable_language_name: "Pseudo-Localization"
    }
}