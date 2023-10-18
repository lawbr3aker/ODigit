import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.3 as ControlsV1
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3

import "qrc:components/buttons" as Buttons
import "qrc:components/others" as Others
import "qrc:components/views" as Views

import "qrc:components/static/toolbar" as Toolbar

import scripts 1.0 as Scripts

ControlsV1.Tab {
    id: root

    anchors.margins: 3
    anchors.fill: parent

    title: 'Untitled'

    readonly property Scripts.Process data: Scripts.Process {
    }

    Item {
        anchors.fill: parent

        GridLayout {
            anchors.fill: parent

            columnSpacing: 0
            rowSpacing: 0

            columns: 2
            rows: 2

            Others.Ruller {
                Layout.column: 0
                Layout.row: 1
                Layout.fillHeight: true
                width: 25
                offset: p.canvas.pan.y
                unitPixels: 56

                orientation: Qt.Vertical
            }

            Others.Ruller {
                Layout.column: 1
                Layout.row: 0
                Layout.fillWidth: true
                height: 25
                offset: p.canvas.pan.x
                unitPixels: 56

                orientation: Qt.Horizontal
            }

            Views.PolygonEditor {
                id: p

                Layout.column: 1
                Layout.row: 1
                Layout.fillHeight: true
                Layout.fillWidth: true
                visible: true

                Component.onCompleted: {
                    console.log(p.canvas.points)
                    root.data.editor = p.canvas
                }
            }
        }
    }
}
