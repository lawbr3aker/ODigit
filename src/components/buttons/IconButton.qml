import QtQuick 2.15
import QtQuick.Controls 1.1 as ControlsV1
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ControlsV1.Button {
    id: root

    readonly property alias icon: icon
    readonly property alias title: title
    readonly property alias hint: hint

    Timer {
        id: timer

        interval: 500
        running: false

        onTriggered: {
            hintContainer.x = mouseHandler.mouseX
            hintContainer.y = mouseHandler.mouseY - hintContainer.height
            hintContainer.visible = true
        }
    }

    MouseArea {
        id: mouseHandler
        z: -1

        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents: true

        onPositionChanged: {
            if (hint.enabled) {
                if (timer.running) {
                    hintContainer.visible = false
                    timer.stop()
                }
                timer.start()
            }
            mouse.accepted = false
        }

        onHoveredChanged: {
            if (hint.enabled && !hovered) {
                hintContainer.visible = false
                timer.stop()
            }
        }
    }

    Rectangle {
        id: hintContainer

        visible: false

        color: '#444'

        width: hint.width + 16
        height: hint.height + 6

        Label {
            id: hint

            enabled: false
            anchors.centerIn: parent

            color: '#F8F8F8'

            onTextChanged: {
                enabled = true
            }
        }
    }

    onPressedChanged: {
        if (pressed)
            forceActiveFocus()
    }

    ColumnLayout {
        anchors.fill: parent

        spacing: 0

        Image {
            id: icon

            Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
            Layout.topMargin: 7
            Layout.preferredHeight: Math.min(45, parent.height - 2 * Layout.topMargin)
            Layout.preferredWidth: implicitWidth / implicitHeight * Layout.preferredHeight
        }

        Text {
            id: title

            Layout.preferredWidth: parent.width
            Layout.fillHeight: true

            Layout.bottomMargin: 3

            property string text

            font.pixelSize: Qt.application.font.pixelSize * 0.9
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignBottom
            leftPadding: 5
            rightPadding: 5
        }
    }
}
