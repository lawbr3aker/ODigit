import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

import "qrc:components/buttons" as Buttons
import "qrc:/components/static" as Static
import "qrc:/"

import scripts 1.0

RowLayout {
    id: root

    property var window

    Translator {
        id: translator
    }

    About {
        id: about
        modality: Qt.ApplicationModal
    }

    Settings {
        id: settings
        modality: Qt.ApplicationModal

        master: window
    }

    ColumnLayout {
        Layout.fillHeight: true

        spacing: 3

        RowLayout {
            Layout.fillHeight: true

            Buttons.IconButton {
                Layout.fillHeight: true
                Layout.preferredWidth: 75

                icon.source: 'qrc:assets/icons/open-file'
                title.text: translator.global.tr('M9sY')

                onClicked: {
                    window.currentProcess.data.readPath()
                }
            }

            Buttons.IconButton {
                Layout.fillHeight: true
                Layout.preferredWidth: 55

                icon.source: 'qrc:assets/icons/save-file'
                title.text: translator.global.tr('N2cU')
            }
        }

        Text {
            Layout.alignment: Qt.AlignBottom
            Layout.preferredWidth: parent.width
            Layout.minimumHeight: 23

            text: translator.global.tr('S8zG')
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle {
        Layout.fillHeight: true
        Layout.preferredWidth: 1
        Layout.leftMargin: 5
        Layout.rightMargin: 5

        color: '#AAA'
    }

    ColumnLayout {
        Layout.fillHeight: true

        spacing: 3

        RowLayout {
            Layout.fillHeight: true

            Buttons.IconButton {
                id: render

                Layout.fillHeight: true
                Layout.preferredWidth: 75

                icon.source: 'qrc:assets/icons/render'
                title.text: translator.global.tr('H6eV')

                onClicked: {
                    window.currentProcess.data.process()
                    window.currentProcess.data.editor.update()
                }
            }

            Buttons.IconButton {
                id: exportDXF

                Layout.fillHeight: true
                Layout.preferredWidth: 65

                icon.source: 'qrc:assets/icons/dxf'
                title.text: translator.global.tr('D5hY')

                onClicked: {
                    window.currentProcess.data.exportDXF()
                }
            }

            Buttons.IconButton {
                id: exportPDF

                Layout.fillHeight: true
                Layout.preferredWidth: 65

//                icon.source: 'qrc:assets/icons/render'
                title.text: translator.global.tr('J9iZ')

                onClicked: {
                    window.currentProcess.data.exportPDF()
                }
            }
        }

        Text {
            Layout.alignment: Qt.AlignBottom
            Layout.preferredWidth: parent.width
            Layout.minimumHeight: 23

            text: translator.global.tr('G4jA')
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle {
        Layout.fillHeight: true
        Layout.preferredWidth: 1
        Layout.leftMargin: 5
        Layout.rightMargin: 5

        color: '#AAA'
    }

    ColumnLayout {
        Layout.fillHeight: true

        spacing: 3

        RowLayout {
            Layout.fillHeight: true

            Buttons.IconButton {
                Layout.fillHeight: true
                Layout.preferredWidth: 65

                icon.source: 'qrc:assets/icons/setting'
                title.text: translator.global.tr('Q8kB')

                onClicked: {
                    settings.show()
                }

            }

            Buttons.IconButton {
                Layout.fillHeight: true
                Layout.preferredWidth: 55

                icon.source: 'qrc:assets/icons/about'
                title.text: translator.global.tr('T2lC')

                onClicked: {
                    about.show()
                }
            }
        }

        Text {
            Layout.alignment: Qt.AlignBottom
            Layout.preferredWidth: parent.width
            Layout.minimumHeight: 23

            text: translator.global.tr('IhyC')
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
