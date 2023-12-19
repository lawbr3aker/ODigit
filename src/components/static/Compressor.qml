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
    width: 180
    height: 49
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    property alias value: ratio.input

    property var after

    background: Item {
        anchors.fill: parent

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

            RowLayout {
                spacing: 4
                Layout.fillWidth: true
                layoutDirection: Qt.LeftToRight

                Inputs.Input {
                    id: ratio
                    spacing: 0
                    input.implicitWidth: 70
                    input.text: '1.0'
                }

                Inputs.Button {
                    text: '✔'
                    Layout.maximumHeight: value.height
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
