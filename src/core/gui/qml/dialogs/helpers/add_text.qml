import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.3 as ControlsV1
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3
import QtGraphicalEffects 1.15

import "qrc:Components/Controls" as Components_Controls

import scripts 1.0

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

    Config {
        id: configThemes

        group: 'config-themes'
    }

    background: Rectangle {
        id: background
        anchors.fill: parent
        radius: 5
        border.width: 1
        border.color: rootThemes.value('colors/iIyV')
        color: rootThemes.value('colors/IlPR')
    }

    Item {
        anchors.fill: parent
        ColumnLayout {
            width: parent.width

            Row {
                spacing: 50
                Layout.fillWidth: true
                layoutDirection: window.direction

                Components_Controls.Input {
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

                Components_Controls.Spinbox {
                    id: fontSize
                    spacing: 0
                    input.implicitWidth: 70
                    input.value: 20
                }

                Components_Controls.Button {
                    id: fontBold
                    title.text: '𝐁'
                    Layout.maximumHeight: fontSize.height
                    Layout.minimumWidth: 35

                    onClicked: {
                        checked = !checked
                    }
                }

                Components_Controls.Button {
                    id: fontItalic
                    title.text: '𝑖'
                    Layout.maximumHeight: fontSize.height
                    Layout.maximumWidth: 35

                    onClicked: {
                        checked = !checked
                    }
                }

                Components_Controls.Button {
                    title.text: '✔'
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