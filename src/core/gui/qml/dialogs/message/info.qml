import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.3 as ControlsV1
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3

import "qrc:Components/Controls" as Components_Controls

import scripts 1.0

ControlsV1.ApplicationWindow {
    id: window

    minimumWidth: content.width
    minimumHeight: content.height

    flags: Qt.Dialog

    property var callback

    property alias context: text
    property alias ok: button

    ColumnLayout {
        id: content

        anchors.fill: parent
        anchors.margins: 10

        Row {
            Layout.alignment: Qt.AlignHCenter

            Text {
                id: text

                width: 300
                horizontalAlignment: Text.AlignHCenter

                wrapMode: Text.WordWrap
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