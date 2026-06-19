import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.3 as ControlsV1
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3

import "qrc:Components/Controls" as Components_Controls
import "qrc:Components/Dialogs/Message" as Components_Dialogs_Message

import scripts 1.0

ApplicationWindow {
    id: window

    required property var globalW

    title: "Registering application"

    minimumWidth: content.width + 20
    minimumHeight: content.height + 20

    flags: Qt.Dialog

    property int direction

    property bool succeed: false

    required property Registration registration

    Rectangle {
        anchors.fill: parent

        color: globalW.globalThemes.value('colors/IlPR')
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

        anchors.centerIn: parent

        function onPaste(event, i) {
            onPaste.t = [k1, k2, k3, k4, k5]

            if ((event.key === Qt.Key_V) && (event.modifiers & Qt.ControlModifier)) {
                let t = ''
                for (let c of Clipboard.getText())
                    if (/\w/.test(c))
                        t += c

                let p = onPaste.t[i].input.text.length
                let c = 0
                for (; i < 5; ++i) {
                    const e = onPaste.t[i].input
                    e.text = t.substring(c, c += e.maximumLength - p)
                    p = 0
                }
            }
        }

        RowLayout {
            Layout.fillWidth: false
            layoutDirection: window.direction

            Text {
                text: "Registration key:"
                color: globalW.globalThemes.value('colors/GFeV')
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

                    onPressed: function (event) { content.onPaste(event, 0) }
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

                    onPressed: function (event) { content.onPaste(event, 1) }
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

                    onPressed: function (event) { content.onPaste(event, 2) }
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

                    onPressed: function (event) { content.onPaste(event, 3) }
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

                    onPressed: function (event) { content.onPaste(event, 4) }
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
                color: globalW.globalThemes.value('colors/GFeV')
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
                        globalW.globalConfig.set('license/key' , key)
                        globalW.globalConfig.set('license/seed', seed)
                        globalW.globalConfig.save()
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