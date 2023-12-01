import QtQuick 2.4
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.1 as ControlsV1

RowLayout {
    property alias title: title
    property alias input: input

    Text {
        id: title
    }

    ControlsV1.TextField {
        id: input

        Layout.fillWidth: true

        property alias value: input.text
    }
}
