import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.3 as ControlsV1
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3

import "qrc:Components/Controls" as Components_Controls
import "qrc:Components/Dialogs/Message" as Components_Dialogs_Message

import scripts 1.0

ControlsV1.ApplicationWindow {
    id: window

    required property var globalW

    title: "Registering application"

    minimumWidth: content.width
    minimumHeight: content.height

    flags: Qt.Dialog

    property int direction

    property bool succeed: false

    Config {
        id: config
    }

    Registration {
        id: registration
    }

    Components_Dialogs_Message.Info {
        id: failedMessage

         modality: Qt.ApplicationModal
        context.text: "License key is invalid!"
        context.width: 230
    }

    Components_Dialogs_Message.Info {
        id: successfulMessage

         modality: Qt.ApplicationModal
        context.text: "Program successfully registered"
        context.width: 270

        ok.onClicked: {
            window.close()
        }
    }

    ColumnLayout {
        id: content

        anchors.fill: parent
        anchors.margins: 10

        RowLayout {
            Layout.fillWidth: false
            layoutDirection: window.direction

            Text {
                text: "Registration key:"
            }

            RowLayout {
                Layout.fillWidth: false
                spacing: 0

                Components_Controls.Input {
                    id: k1
                    input.      maximumLength: 4
                    input.     font.pixelSize: 19
                    input.      implicitWidth: input.maximumLength * (input.font.pixelSize * 0.7)
                    input.horizontalAlignment: TextInput.AlignHCenter

                    //input.text: "6002"

                    input.onLengthChanged:
                        if (input.length == input.maximumLength) k2.input.focus = true
                }

                Text {
                    Layout.leftMargin: 4

                    text: "-"
                }

                Components_Controls.Input {
                    id: k2
                    input.      maximumLength: 4
                    input.     font.pixelSize: 19
                    input.      implicitWidth: input.maximumLength * (input.font.pixelSize * 0.7)
                    input.horizontalAlignment: TextInput.AlignHCenter

                    //input.text: "021B"

                    input.onLengthChanged:
                        if (input.length == input.maximumLength) k3.input.focus = true
                }

                Text {
                    Layout.leftMargin: 4

                    text: "-"
                }

                Components_Controls.Input {
                    id: k3
                    input.      maximumLength: 4
                    input.     font.pixelSize: 19
                    input.      implicitWidth: input.maximumLength * (input.font.pixelSize * 0.7)
                    input.horizontalAlignment: TextInput.AlignHCenter

                    //input.text: "7L00"

                    input.onLengthChanged:
                        if (input.length == input.maximumLength) k4.input.focus = true
                }

                Text {
                    Layout.leftMargin: 4

                    text: "-"
                }

                Components_Controls.Input {
                    id: k4
                    input.      maximumLength: 6
                    input.     font.pixelSize: 19
                    input.      implicitWidth: input.maximumLength * (input.font.pixelSize * 0.7)
                    input.horizontalAlignment: TextInput.AlignHCenter

                    //input.text: "7X9I12"

                    input.onLengthChanged:
                        if (input.length == input.maximumLength) k5.input.focus = true
                }

                Text {
                    Layout.leftMargin: 4

                    text: "-"
                }

                Components_Controls.Input {
                    id: k5
                    input.      maximumLength: 4
                    input.     font.pixelSize: 19
                    input.      implicitWidth: input.maximumLength * (input.font.pixelSize * 0.7)
                    input.horizontalAlignment: TextInput.AlignHCenter

                    //input.text:d "22M0"
                }
            }
        }

        Row {
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 5

            Text {
                width: 510

                    text: "If you don't have any registration license, Submit (<a href='requests.txt'>REQUEST.txt</a>) file to your administration to get one."
                wrapMode: Text.WordWrap
                font.pixelSize: 13

                MouseArea {
                    z: -1
                    anchors.fill: parent
                    cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor

                    onClicked: {
                        if (parent.hoveredLink === 'requests.txt') {
                            registration.write_id()
                        }
                    }
                }
            }
        }

        RowLayout {
            Layout.topMargin: 5
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignRight

            Components_Controls.Button {
                Layout.preferredWidth: 90

                icon.height: 0
                title.text: "Apply"

                onClicked: {
                    const key  = k1.input.text + k2.input.text + k3.input.text + k5.input.text
                    const seed = k4.input.text
                    if (registration.check_key(key, seed)) {
                        config.global.set('license/key' , key)
                        config.global.set('license/seed', seed)
                        config.global.save()
                        succeed = true

                        successfulMessage.show()
                    } else {
                        failedMessage.show()
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        direction = globalW.globalConfig.value('interface/appearance/direction') == 'rtl' ? Qt.RightToLeft : Qt.LeftToRight
    }
}