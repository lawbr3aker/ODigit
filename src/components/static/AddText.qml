import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.3 as ControlsV1
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3
import QtGraphicalEffects 1.15

import "qrc:components/buttons" as Buttons
import "qrc:components/others" as Others
import "qrc:components/views" as Views

import "qrc:components/static/toolbar" as Toolbar

import "qrc:components/inputs" as Inputs

import scripts 1.0 as Scripts

Popup {
    id: root
    width: 230
    height: 89
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

    property alias value: value.input
    property alias fontSize: fontSize.input.value
    property alias fontBold: fontBold.checked
    property alias fontItalic: fontItalic.checked

    property var after

    background: Item {
        anchors.fill: root

        Rectangle {
            id: background
            anchors.fill: parent
            radius: 5
            color: '#EDEDED'
            border.width: 1
            border.color: '#CACACA'
        }
    }

    Item {
        anchors.fill: parent
        ColumnLayout {
            width: parent.width

            Row {
                spacing: 50
                Layout.fillWidth: true
                layoutDirection: window.direction

                Inputs.Input {
                    id: value
                    spacing: 0
                    width: parent.width
                    layoutDirection: window.direction
                }
            }

            RowLayout {
                spacing: 4
                Layout.fillWidth: true
                layoutDirection: Qt.LeftToRight

                Inputs.SpinBox {
                    id: fontSize
                    spacing: 0
                    input.implicitWidth: 70
                    input.value: 20
                }

                Inputs.Button {
                    id: fontBold
                    //text: '𝐁'
                    Layout.maximumHeight: fontSize.height
                    Layout.maximumWidth: 25

                    onClicked: {
                        checked = !checked
                    }
                }

                Inputs.Button {
                    id: fontItalic
                    text: '𝑖'
                    Layout.maximumHeight: fontSize.height
                    Layout.maximumWidth: 25

                    onClicked: {
                        checked = !checked
                    }
                }

                Inputs.Button {
                    text: '✔'
                    Layout.maximumHeight: fontSize.height
                    Layout.fillWidth: true
                    Layout.leftMargin: 30

                    onClicked: {
                        root.after()
                    }
                }
            }
        }
    }
}