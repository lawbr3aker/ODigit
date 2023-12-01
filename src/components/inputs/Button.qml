import QtQuick 2.4
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.1 as ControlsV1

ControlsV1.Button {
    implicitHeight: 37

    Rectangle {
        anchors.fill: parent
        color: 'black'
        opacity: 0.3
        visible: checked
        radius: 4
    }
}
