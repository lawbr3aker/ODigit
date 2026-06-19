import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

import "qrc:Dialogs" as Dialogs
import "qrc:Components/Controls" as Components_Controls
import "qrc:Components/Dialogs/Helpers" as Components_Dialogs_Helpers

import scripts 1.0

RowLayout {
    id: root

    property var globalW

    Dialogs.About {
        id: about

        globalW: root.globalW

        modality: Qt.ApplicationModal
    }

    Dialogs.Settings {
        id: settings

        globalW: root.globalW

        modality: Qt.ApplicationModal
    }

    ColumnLayout {
        Layout.fillHeight: true

        spacing: 3

        RowLayout {
            Layout.fillHeight: true

            Components_Controls.Button {
                Layout.    fillHeight: true
                Layout.preferredWidth: 75

                font.family: ":/Assets/Fonts/Default-EN"

                title.text: `${globalW.globalTranslator.tr('M9sY')}`
                icn.source: 'qrc:Assets/Images/Icons/open-file'
                icn.parent.height: 40

                onClicked: {
                    globalW.currentProcess.step_path()
                    globalW.currentProcess.step_detect()
                    for (const point of globalW.currentProcess.editor.input.activePoints)
                        point.status = 'default'
                    globalW.currentProcess.editor.input.activePoints = []
                    for (const segment of globalW.currentProcess.editor.input.activeSegments)
                        segment.status = 'default'
                    globalW.currentProcess.editor.input.activeSegments = []
                    globalW.currentProcess.editor.update()
                }
            }

            //Components_Controls.Button {
            //    Layout.    fillHeight: true
            //    Layout.preferredWidth: 55
//
            //    title. text: `${globalW.globalTranslator.tr('N2cU')}`
            //    icn.source: 'qrc:Assets/Images/Icons/save-file'
            //    icn.parent.height: 40
            //}
        }

        Text {
            Layout.     alignment: Qt.AlignBottom
            Layout.preferredWidth: parent.width
            Layout. minimumHeight: 23

                          color: globalW.globalThemes.value('colors/GFeV')
                           text: `${globalW.globalTranslator.tr('S8zG')}`
            horizontalAlignment: Text.AlignHCenter
              verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle {
        Layout.    fillHeight: true
        Layout.preferredWidth: 1
        Layout.    leftMargin: 5
        Layout.   rightMargin: 5

        color: globalW.globalThemes.value('colors/iIyV')
    }

    ColumnLayout {
        Layout.fillHeight: true

        spacing: 3

        RowLayout {
            Layout.fillHeight: true

            Components_Controls.Button {
                id: render

                Layout.    fillHeight: true
                Layout.preferredWidth: 75

                title. text: `${globalW.globalTranslator.tr('H6eV')}`
                icn.source: 'qrc:Assets/Images/Icons/render'
                icn.parent.height: 40

                onClicked: {
                    globalW.currentProcess.step_process()
                    for (const point of globalW.currentProcess.editor.input.activePoints)
                        point.status = 'default'
                    globalW.currentProcess.editor.input.activePoints = []
                    for (const segment of globalW.currentProcess.editor.input.activeSegments)
                        segment.status = 'default'
                    globalW.currentProcess.editor.input.activeSegments = []
                    globalW.currentProcess.editor.update()
                }
            }

            Components_Dialogs_Helpers.ContoursSimplify {
                id: simplifyDialog

                globalW: root.globalW

                onVisibleChanged: {
                    parent = globalW.currentProcess
                    x = 40
                    y = 40
                }

                after:
                    function() {
                        globalW.currentProcess.editor.simplify(parseFloat(simplifyDialog.a.text), parseFloat(simplifyDialog.b.text), simplifyDialog.c.value)
                        globalW.currentProcess.editor.update()
                        simplifyDialog.close()
                    }
            }

            Components_Controls.Button {
                id: simplify

                Layout.    fillHeight: true
                Layout.preferredWidth: 75

                title. text: `${globalW.globalTranslator.tr('DSe1')}`
                icn.source: 'qrc:Assets/Images/Icons/compress'
                icn.parent.height: 40

                onClicked: {
                    simplifyDialog.a.text  = globalW.currentProcess.config.value('scanner/simplify/default_threshold_a')
                    simplifyDialog.b.text  = globalW.currentProcess.config.value('scanner/simplify/default_threshold_b')
                    simplifyDialog.c.value = globalW.currentProcess.config.value('scanner/simplify/default_iterations')
                    simplifyDialog.open()
                }
            }

            Components_Controls.Button {
                id: exportDXF

                Layout.    fillHeight: true
                Layout.preferredWidth: 65

                title. text: `${globalW.globalTranslator.tr('D5hY')}`
                icn.source: 'qrc:Assets/Images/Icons/dxf'
                icn.parent.height: 40

                onClicked: {
                    globalW.currentProcess.step_export_dxf()
                }
            }
        }

        Text {
            Layout.     alignment: Qt.AlignBottom
            Layout.preferredWidth: parent.width
            Layout. minimumHeight: 23

                          color: globalW.globalThemes.value('colors/GFeV')
                           text: `${globalW.globalTranslator.tr('G4jA')}`
            horizontalAlignment: Text.AlignHCenter
              verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle {
        Layout.    fillHeight: true
        Layout.preferredWidth: 1
        Layout.    leftMargin: 5
        Layout.   rightMargin: 5

        color: globalW.globalThemes.value('colors/iIyV')
    }

    ColumnLayout {
        Layout.fillHeight: true

        spacing: 3

        RowLayout {
            Layout.fillHeight: true

            Components_Controls.Button {
                Layout.    fillHeight: true
                Layout.preferredWidth: 65

                title. text: `${globalW.globalTranslator.tr('Q8kB')}`
                icn.source: 'qrc:Assets/Images/Icons/setting'
                icn.parent.height: 40

                onClicked: {
                    settings.show()
                }
            }

            Components_Controls.Button {
                Layout.    fillHeight: true
                Layout.preferredWidth: 55

                 title.text: `${globalW.globalTranslator.tr('T2lC')}`
                icn.source: 'qrc:Assets/Images/Icons/about'
                icn.parent.height: 40

                onClicked: {
                    about.show()
                }
            }
        }

        Text {
            Layout.     alignment: Qt.AlignBottom
            Layout.preferredWidth: parent.width
            Layout. minimumHeight: 23

                          color: globalW.globalThemes.value('colors/GFeV')
                           text: `${globalW.globalTranslator.tr('IhyC')}`
            horizontalAlignment: Text.AlignHCenter
              verticalAlignment: Text.AlignVCenter
        }
    }
}
