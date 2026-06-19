import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.3 as ControlsV1
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3

import "qrc:Components/Controls" as Components_Controls

import scripts 1.0

ApplicationWindow {
    id: window

    minimumWidth: content.width + 20
    minimumHeight: content.height + 20

    flags: Qt.Dialog

    Config {
        id: themes

        group: "config-themes"
    }

    Rectangle {
        anchors.fill: parent

        color: themes.value('colors/IlPR')
    }

    property var callback

    property alias context: text
    property alias ok: button

    ColumnLayout {
        id: content

        anchors.centerIn: parent

        Row {
            Layout.alignment: Qt.AlignHCenter

            Text {
                id: text

                width: 300
                horizontalAlignment: Text.AlignHCenter

                wrapMode: Text.WordWrap

                color: themes.value('colors/GFeV')
            }
        }

        RowLayout {
            Layout.topMargin: 5
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignRight

            Components_Controls.Button {
                id: button

                Layout.preferredWidth: 90

                icon.height: 0
                title.text: "Ok"

                onClicked: {
                    window.close()
                }
            }
        }
    }
}