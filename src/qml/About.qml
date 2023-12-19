import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.3 as ControlsV1
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3

import "qrc:components/buttons" as Buttons
import "qrc:components/others" as Others
import "qrc:components/views" as Views

import "qrc:components/static/toolbar" as Toolbar
import "qrc:components/static" as Static

ControlsV1.ApplicationWindow {
    title: "About"

    width: 280
    height: description.height + blockTop.height

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        ColumnLayout {
            id: blockTop
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            Layout.preferredHeight: 180

            Image {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 10
                Layout.preferredWidth: 100
                Layout.preferredHeight: 100

                source: 'qrc:/assets/logo'
            }

            Text {
                Layout.topMargin: 10
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                text: translator.global.tr('UlcH')
            }
        }

        Rectangle {
            id: blockBottom
            Layout.fillWidth: true
            Layout.fillHeight: true

            Text {
                id: description
                anchors.centerIn: parent
                padding: 17
                width: parent.width
                wrapMode: Text.WordWrap
                font.pixelSize: 14
                text: translator.global.tr('MV2O')
            }
        }
    }
}
