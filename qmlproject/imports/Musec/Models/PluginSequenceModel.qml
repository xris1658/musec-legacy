import QtQml.Models 2.15

ListModel {
    ListElement {
        valid: true
        processing: true
        plugin_name: "SC Not Exist"
        sidechain_exist: false
        sidechain_enabled: false
        window_visible: false
    }
    ListElement {
        valid: true
        processing: true
        plugin_name: "SC Not Enabled"
        sidechain_exist: true
        sidechain_enabled: false
        window_visible: false
    }
    ListElement {
        valid: true
        processing: true
        plugin_name: "SC Enabled"
        sidechain_exist: true
        sidechain_enabled: true
        window_visible: false
    }
}