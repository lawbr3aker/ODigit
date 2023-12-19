import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.3 as ControlsV1
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls.Styles 1.4

import "qrc:components/buttons" as Buttons
import "qrc:components/others" as Others
import "qrc:components/views" as Views

import "qrc:components/static/toolbar" as Toolbar
import "qrc:components/static" as Static

import scripts 1.0

ControlsV1.ApplicationWindow {
    id: window
    width: 800
    height: 600
    visible: true
    title: "ODigit"
    modality: Qt.ApplicationModal

    property int direction

    property var currentProcess

    Config {
        id: config
    }

    Translator {
        id: translator
    }

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

                readonly property Item container: ColumnLayout {
                    parent: tabHome
                    anchors.fill: parent

                    readonly property Toolbar.Home toolbar: Toolbar.Home {
                        z: 1
                        parent: tabHome.container
                        height: parent.height

                        window: window
                    }
                }
            }

            ControlsV1.Tab {
                id: tabDraw
                anchors.leftMargin: 7
                anchors.topMargin: 7
                anchors.rightMargin: 7
                anchors.fill: parent

                title: "Draw"

                readonly property Item container: ColumnLayout {
                    parent: tabDraw
                    anchors.fill: parent

                    readonly property Toolbar.Draw toolbar: Toolbar.Draw {
                        parent: tabDraw.container
                        height: parent.height

                        // window: window
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            spacing: parent.spacing

            ColumnLayout {
                id: tabSide
                z: 2
                Layout.alignment: Qt.AlignTop
                Layout.fillHeight: true
                Layout.maximumHeight: 370
                Layout.minimumWidth: 40
                Layout.maximumWidth: 40

                readonly property Item container: Item {
                    id: ts
                    parent: tabSide
                    width: parent.width

                    readonly property Toolbar.Side toolbar: Toolbar.Side {
                        parent: tabSide.container
                        anchors.fill: parent
                        width: parent.width
                        anchors.margins: 5

                        window: window
                    }
                }

                Rectangle {
                    width: parent.width
                    height: parent.height

                    border.width: 1
                    border.color: '#AAAAAA'
                    color: 'white'
                }
            }

            ColumnLayout {
                z: 1
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

                    Component {
                        id: defaultProcess

                        Static.Process {
                            title: '+'
                        }
                    }

                    onCurrentIndexChanged: {
                        const currentTab = getTab(currentIndex)

                        window.currentProcess = currentTab
                        window.currentProcess = currentTab

                        if (currentIndex == count - 1 && count) {
                            currentTab.title = 'Untitled'

                            addTab('+', defaultProcess)
                        }
                    }

                    Component.onCompleted: {
                        const currentTab = untitled

                        window.currentProcess = currentTab
                        window.currentProcess = currentTab
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        direction = config.global.value('interface/appearance/direction', 'string') == 'rtl' ? Qt.RightToLeft : Qt.LeftToRight
    }
}
