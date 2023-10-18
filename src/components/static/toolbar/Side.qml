import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

import "qrc:components/buttons" as Buttons

ColumnLayout {
    spacing: 5

    Buttons.IconButton {
        Layout.fillWidth: true
        Layout.preferredHeight: width

        icon.source: 'qrc:assets/icons/cursor'
    }

    Buttons.IconButton {
        Layout.fillWidth: true
        Layout.preferredHeight: width

        icon.source: 'qrc:assets/icons/point'
    }

    Buttons.IconButton {
        Layout.fillWidth: true
        Layout.preferredHeight: width

        icon.source: 'qrc:assets/icons/line'
    }
}
