import QtQml.Models 2.15

ListModel {
    ListElement {
        parameterId: 1
        name: "A Continuous Parameter"
        shortName: "Cont."
        discrete: false
        periodic: false
        hidden: false
        readonly: false
        automatable: true
        minValue: 0
        maxValue: 1
        defaultValue: 0.8
        value: 0.5
        step: 0
        showAsList: false
        showAsSwitch: false
        list: null
    }
}
