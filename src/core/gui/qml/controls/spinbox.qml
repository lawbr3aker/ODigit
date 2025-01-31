import QtQuick 2.4
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.1 as ControlsV1
import QtQuick.Controls.Styles 1.4

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
        minimumValue: -9999
        maximumValue: 9999

        style: SpinBoxStyle {
            textColor: configThemes.value('colors/GFeV')
            horizontalAlignment: Qt.AlignLeft

            background: Rectangle {
                property string bg: ""
                property string bd: ""

                radius: 3

                implicitWidth: 100
                implicitHeight: 25

                border {
                  color: bd ? bd : configThemes.value('colors/0LOj')
                  width: 1
                }

                color: bg ? bg : ((input.hovered | input.focus) ? configThemes.value('colors/HVvi') : configThemes.value('colors/5v8I'))
            }
        }
    }
}
