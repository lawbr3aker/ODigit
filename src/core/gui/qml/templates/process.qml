import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.3 as ControlsV1
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3

import "qrc:Components/Controls" as Components_Controls
import "qrc:Components/Templates" as Components_Templates

import scripts 1.0 as Scripts

Scripts.Process {
    id: root

    anchors.margins: 3
    anchors.   fill: parent

    property Scripts.Editor editor

    GridLayout {
        parent: root
        anchors.fill: parent

        columnSpacing: 0
           rowSpacing: 0

           rows: 2
        columns: 2

        Scripts.Ruler {
            id: rulerV

                        width: 25
            Layout.       row: 1
            Layout.    column: 0
            Layout.fillHeight: true

                 offset: editor.pan.y
            orientation: Qt.Vertical
        }

        Scripts.Ruler {
            id: rulerH

                      height: 25
            Layout.      row: 0
            Layout.   column: 1
            Layout.fillWidth: true

                 offset: editor.pan.x
            orientation: Qt.Horizontal
        }

        Timer {
            id: fps

            interval: 20
             running: root.active
              repeat: true

            onTriggered: {
                rulerH.update()
                rulerV.update()
            }
        }

        Components_Templates.Editor {
            id: editor

            Layout.       row: 1
            Layout.    column: 1
            Layout. fillWidth: true
            Layout.fillHeight: true

            process: root

            Component.onCompleted: {
                this.set_process(root)

                root.editor = this
                root.set_editor(this)
            }

            onZoomChanged: {
                const pixels = root.config.value('cm_pixels', 'int')

                rulerV.unit_pixels = zoom * pixels
                rulerH.unit_pixels = zoom * pixels
            }
        }
    }
}
