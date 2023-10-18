import QtQuick 2.15
import QtQuick.Layouts 1.15

import "qrc:components/buttons" as Buttons

RowLayout {
    ColumnLayout {
        Layout.fillHeight: true

        RowLayout {
            Layout.fillHeight: true

            Buttons.IconButton {
                Layout.fillHeight: true
                Layout.preferredWidth: 65

                icon.source: 'qrc:assets/icons/move'
                text.text: "Move"
            }

            Buttons.IconButton {
                Layout.fillHeight: true
                Layout.preferredWidth: 55

                icon.source: 'qrc:assets/icons/eraser'
                text.text: "Erase"
            }
        }

        Text {
            Layout.alignment: Qt.AlignBottom
            Layout.preferredWidth: parent.width

            text: 'Utility'
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
