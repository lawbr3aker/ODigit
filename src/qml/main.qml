import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.3 as ControlsV1
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3

import "qrc:components/buttons" as Buttons
import "qrc:components/others" as Others
import "qrc:components/views" as Views

import "qrc:components/static/toolbar" as Toolbar
import "qrc:components/static" as Static

ControlsV1.ApplicationWindow {
    id: window
    width: 800
    height: 600
    visible: true
    title: "%1 %2".arg(Qt.application.name)
                    .arg(Qt.application.version)

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10

        spacing: 7

        ControlsV1.TabView {
            Layout.fillWidth: true
            Layout.preferredHeight: 170

            ControlsV1.Tab {
                id: tabHome
                anchors.leftMargin: 7
                anchors.topMargin: 7
                anchors.rightMargin: 7
                anchors.fill: parent

                title: "Home"

                readonly property Item container: Item {
                    parent: tabHome
                    anchors.fill: parent

                    readonly property Toolbar.Home toolbar: Toolbar.Home {
                        parent: tabHome.container
                        height: parent.height
                    }
                }
            }

            ControlsV1.Tab {
                anchors.leftMargin: 7
                anchors.topMargin: 7
                anchors.rightMargin: 7
                anchors.fill: parent

                title: "Draw"

                Item {
                    Layout.fillHeight: true

                    Toolbar.Draw {
                        height: parent.height
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            spacing: parent.spacing

            Column {
                Layout.fillHeight: true

                Rectangle {
                    height: 150
                    width: 40

                    border.width: 1
                    border.color: '#AAAAAA'
                    color: '#eaeaea'

                    Rectangle {
                        width: parent.width - 4
                        height: parent.height - 4
                        anchors.centerIn: parent

                        color: 'white '
                    }

                    Toolbar.Side {
                        x: 5
                        y: x
                        width: parent.width - 2 * x
                    }
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true

                ControlsV1.TabView {
                    id: processes

                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Static.Process {
                        id: untitled
                        title: 'Untitled'
                    }

                    Static.Process {
                        title: '+'
                    }

                    onCurrentIndexChanged: {
                        tabHome.container.toolbar.currentProcess = getTab(currentIndex)
                    }

                    Component.onCompleted: {
                        tabHome.container.toolbar.currentProcess = untitled
                    }
                }
            }
        }
    }
}
