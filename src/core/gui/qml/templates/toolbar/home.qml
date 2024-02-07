import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

import "qrc:Dialogs" as Dialogs
import "qrc:Components/Controls" as Components_Controls
import "qrc:Components/Dialogs/Helpers" as Components_Dialogs_Helpers

import scripts 1.0

RowLayout {
    id: root

    property var window

    Translator {
        id: translator
    }

    Dialogs.About {
        id: about

        modality: Qt.ApplicationModal
    }

    Dialogs.Settings {
        id: settings

        modality: Qt.ApplicationModal
          master: window
    }

    ColumnLayout {
        Layout.fillHeight: true

        spacing: 3

        RowLayout {
            Layout.fillHeight: true

            Components_Controls.Button {
                Layout.    fillHeight: true
                Layout.preferredWidth: 75

                title. text: translator.global.tr('M9sY')
                icon.source: 'qrc:Assets/Images/Icons/open-file'

                onClicked: {
                    window.currentProcess.step_path()
                    window.currentProcess.step_detect()
                    window.currentProcess.editor.update()
                }
            }

            Components_Controls.Button {
                Layout.    fillHeight: true
                Layout.preferredWidth: 55

                title. text: translator.global.tr('N2cU')
                icon.source: 'qrc:Assets/Images/Icons/save-file'
            }
        }

        Text {
            Layout.     alignment: Qt.AlignBottom
            Layout.preferredWidth: parent.width
            Layout. minimumHeight: 23

                           text: translator.global.tr('S8zG')
            horizontalAlignment: Text.AlignHCenter
              verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle {
        Layout.    fillHeight: true
        Layout.preferredWidth: 1
        Layout.    leftMargin: 5
        Layout.   rightMargin: 5

        color: '#AAA'
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

                title. text: translator.global.tr('H6eV')
                icon.source: 'qrc:Assets/Images/Icons/render'

                onClicked: {
                    window.currentProcess.step_process()
                    window.currentProcess.editor.update()
                }
            }

            Components_Dialogs_Helpers.ContoursSimplify {
                id: simplifyDialog

                onVisibleChanged: {
                    parent = window.currentProcess
                    x = 40
                    y = 40
                }

                after:
                    function() {
                        window.currentProcess.step_simplify(parseFloat(simplifyDialog.a.text), parseFloat(simplifyDialog.b.text))
                        window.currentProcess.editor.update()
                        simplifyDialog.close()
                    }
            }

            Components_Controls.Button {
                id: simplify

                Layout.    fillHeight: true
                Layout.preferredWidth: 75

                title. text: translator.global.tr('DSe1')
                icon.source: 'qrc:Assets/Images/Icons/compress'

                onClicked: {
                    simplifyDialog.a.text = window.currentProcess.config.value('scanner/simplify/default_threshold_a')
                    simplifyDialog.b.text = window.currentProcess.config.value('scanner/simplify/default_threshold_b')
                    simplifyDialog.open()
                }
            }

            Components_Controls.Button {
                id: exportDXF

                Layout.    fillHeight: true
                Layout.preferredWidth: 65

                title. text: translator.global.tr('D5hY')
                icon.source: 'qrc:Assets/Images/Icons/dxf'

                onClicked: {
                    window.currentProcess.step_export_dxf()
                }
            }
        }

        Text {
            Layout.     alignment: Qt.AlignBottom
            Layout.preferredWidth: parent.width
            Layout. minimumHeight: 23

                           text: translator.global.tr('G4jA')
            horizontalAlignment: Text.AlignHCenter
              verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle {
        Layout.    fillHeight: true
        Layout.preferredWidth: 1
        Layout.    leftMargin: 5
        Layout.   rightMargin: 5

        color: '#AAA'
    }

    ColumnLayout {
        Layout.fillHeight: true

        spacing: 3

        RowLayout {
            Layout.fillHeight: true

            Components_Controls.Button {
                Layout.    fillHeight: true
                Layout.preferredWidth: 65

                title. text: translator.global.tr('Q8kB')
                icon.source: 'qrc:Assets/Images/Icons/setting'

                onClicked: {
                    settings.show()
                }
            }

            Components_Controls.Button {
                Layout.    fillHeight: true
                Layout.preferredWidth: 55

                icon.source: 'qrc:Assets/Images/Icons/about'
                 title.text: translator.global.tr('T2lC')

                onClicked: {
                    about.show()
                }
            }
        }

        Text {
            Layout.     alignment: Qt.AlignBottom
            Layout.preferredWidth: parent.width
            Layout. minimumHeight: 23

                           text: translator.global.tr('IhyC')
            horizontalAlignment: Text.AlignHCenter
              verticalAlignment: Text.AlignVCenter
        }
    }
}
