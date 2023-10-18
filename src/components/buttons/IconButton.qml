import QtQuick 2.15
import QtQuick.Controls 1.1 as ControlsV1
import QtQuick.Layouts 1.15

ControlsV1.Button {
    readonly property alias icon: icon
    readonly property alias text: text

    property int iconheight: 50

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
            Layout.topMargin: 5
            Layout.preferredHeight: Math.min(50, parent.height - 2 * Layout.topMargin)
            Layout.preferredWidth: implicitWidth / implicitHeight * Layout.preferredHeight
        }

        Text {
            id: text

            Layout.preferredWidth: parent.width
            Layout.fillHeight: true

            font.pixelSize: Qt.application.font.pixelSize * 0.9
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignBottom
            bottomPadding: 3
            leftPadding: 5
            rightPadding: 5
        }
    }
}
