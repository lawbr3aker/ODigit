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

    readonly property Scripts.Process data: Scripts.Process {
    }

    property GridLayout container: GridLayout {
        parent: root
        anchors.fill: parent

        columnSpacing: 0
        rowSpacing: 0

        columns: 2
        rows: 2

        Scripts.Ruler {
            id: rulerV
            Layout.column: 0
            Layout.row: 1
            Layout.fillHeight: true
            width: 25
            offset: editor.canvas.pan.y

            orientation: Qt.Vertical
        }

        Scripts.Ruler {
            id: rulerH
            Layout.column: 1
            Layout.row: 0
            Layout.fillWidth: true
            height: 25
            offset: editor.canvas.pan.x

            orientation: Qt.Horizontal
        }

        Timer {
            id: fps
            interval: 16
            running: true
            repeat: true
            onTriggered: {
                rulerH.update()
                rulerV.update()
            }
        }

        Views.PolygonEditor {
            id: editor
            parent: container

            Layout.column: 1
            Layout.row: 1
            Layout.fillHeight: true
            Layout.fillWidth: true
            visible: true

            Component.onCompleted: {
                root.data.editor = editor.canvas
            }

            canvas.onZoomChanged: {
                const pixels = root.data.config.value('cm_pixels', 'int')
                rulerV.unitPixels = canvas.zoom * pixels
                rulerH.unitPixels = canvas.zoom * pixels
            }
        }
    }

    property alias editor: editor
}
