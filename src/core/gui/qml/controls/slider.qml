import QtQuick 2.4
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.1 as ControlsV1

import scripts 1.0

RowLayout {
    property alias title: title
    property alias input: input

    Config {
        id: configThemes

        group: 'config-themes'
    }

    Text {
        id: title

        color: configThemes.value('colors/GFeV')
    }

    ControlsV1.SpinBox {
        id: input
        Layout.fillWidth: true
    }
}