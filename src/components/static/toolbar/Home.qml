import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

import "qrc:components/buttons" as Buttons

import "qrc:/components/static" as Static

RowLayout {
    property var currentProcess

    ColumnLayout {
        Layout.fillHeight: true

        RowLayout {
            Layout.fillHeight: true

            Buttons.IconButton {
                Layout.fillHeight: true
                Layout.preferredWidth: 75

                icon.source: 'qrc:assets/icons/open-file'
                text.text: "Open file"

                onClicked: {
                    console.log('clicked', currentProcess.data.readPath())
                }
            }

            Buttons.IconButton {
                Layout.fillHeight: true
                Layout.preferredWidth: 55

                icon.source: 'qrc:assets/icons/save-file'
                text.text: "Save file"
            }

            Buttons.IconButton {
                Layout.fillHeight: true
                Layout.preferredWidth: 55

                icon.source: 'qrc:assets/icons/save-as'
                text.text: "Save as"
            }
        }

        Text {
            Layout.alignment: Qt.AlignBottom
            Layout.preferredWidth: parent.width

            text: 'Files'
            horizontalAlignment: Text.AlignHCenter
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

        RowLayout {
            Layout.fillHeight: true

            Buttons.IconButton {
                Layout.fillHeight: true
                Layout.preferredWidth: 75

                icon.source: 'qrc:assets/icons/render'
                text.text: "Render"
            }

            Buttons.IconButton {
                Layout.fillHeight: true
                Layout.preferredWidth: 65

//                icon.source: 'qrc:assets/icons/render'
                text.text: "UnKnown"
            }
        }

        Text {
            Layout.alignment: Qt.AlignBottom
            Layout.preferredWidth: parent.width

            text: 'Proccesssing'
            horizontalAlignment: Text.AlignHCenter
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

        RowLayout {
            Layout.fillHeight: true

            Buttons.IconButton {
                Layout.fillHeight: true
                Layout.preferredWidth: 65

                icon.source: 'qrc:assets/icons/setting'
                text.text: "Setting"
            }

            Buttons.IconButton {
                Layout.fillHeight: true
                Layout.preferredWidth: 55

                icon.source: 'qrc:assets/icons/about'
                text.text: "About"
            }
        }

        Text {
            Layout.alignment: Qt.AlignBottom
            Layout.preferredWidth: parent.width

            text: '...'
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
