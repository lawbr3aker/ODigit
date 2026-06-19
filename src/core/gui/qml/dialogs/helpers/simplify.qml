import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.3 as ControlsV1
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3
import QtGraphicalEffects 1.15

import "qrc:Components/Controls" as Components_Controls

import scripts 1.0 as Scripts

Popup {
    id: root
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    property var globalW

    property alias a: a.input
    property alias b: b.input
    property alias c: c.input

    property var after

    Scripts.Config {
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

    ColumnLayout {
        id: content

        RowLayout {
            Components_Controls.Input {
                id: a

                Layout.fillWidth: false
                Layout.alignment: Qt.AlignRight

                         title.text: `${globalW.globalTranslator.tr('I29X')}`
                input.implicitWidth: 70
            }
        }

        RowLayout {
            Layout.preferredWidth: parent.width

            Components_Controls.Input {
                id: b

                Layout.alignment: Qt.AlignRight
                Layout.fillWidth: false

                         title.text: `${globalW.globalTranslator.tr('Ow19')}`
                input.implicitWidth: 70
            }
        }

        RowLayout {
            Layout.preferredWidth: parent.width

            Components_Controls.Spinbox {
                id: c

                Layout.alignment: Qt.AlignRight
                Layout.fillWidth: false

                         title.text: `${globalW.globalTranslator.tr('O9JD')}`
                input.implicitWidth: 70
            }
        }

        RowLayout {
            Components_Controls.Button {
                title.text: `${globalW.globalTranslator.tr('Uqb2')}`

                Layout.alignment: Qt.AlignRight
                Layout.fillWidth: true

                onClicked: {
                    root.after()
                }
            }
        }
    }
}
