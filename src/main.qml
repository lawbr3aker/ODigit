import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.3 as ControlsV1
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls.Styles 1.4
import QtQml 2.15

import "qrc:Dialogs" as Dialogs
import "qrc:/Components/Controls" as Components_Controls
import "qrc:/Components/Templates" as Components_Templates
import "qrc:/Components/Templates/Toolbar" as Components_Templates_Toolbar

import scripts 1.0

ControlsV1.ApplicationWindow {
    id: window
    minimumWidth: 850
    minimumHeight: 650
    visible: true
    title: "ODigit 25.1"
    modality: Qt.ApplicationModal

    property int direction

    property var currentProcess

    Config {
        id: rootConfig
    }

    Translator {
        id: rootTranslator
    }

    Registration {
        id: registration
    }

    Dialogs.Registration {
        id: registrationDialog

        modality: Qt.ApplicationModal

        onClosing: {
            if (!succeed) {
                Qt.quit()
            }
        }

        Timer {
            id: fps

            interval: 1000 * 2 //  60 * 3
             running: true
              repeat: false

            onTriggered: {
                const key  = rootConfig.global.value('license/key')
                const seed = rootConfig.global.value('license/seed')

                if (!registration.check_key(key, seed)) {
                    registrationDialog.show()
                }
            }
        }
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

                    readonly property Components_Templates_Toolbar.Home toolbar: Components_Templates_Toolbar.Home {
                        z: 1
                        parent: tabHome.container
                        height: parent.height

                        window: window
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
                Layout.minimumHeight: 370
                Layout.maximumWidth: 43
                Layout.minimumWidth: Layout.maximumWidth

                readonly property Item container: Item {
                    id: ts
                    parent: tabSide
                    width: parent.width

                    Components_Templates_Toolbar.Side {
                        id: sidebar

                        parent: tabSide.container
                        anchors.fill: parent
                        width: parent.width
                        anchors.margins: 5

                        window: window

                        Connections {
                            target: window.currentProcess.editor.input

                            function onKeyChanged(key, state) {
                                if ((key === Qt.RightButton || key === Qt.Key_Escape) && state === 0) {
                                    sidebar.resetAll()
                                }
                            }
                        }
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

                    property bool addProcess: false

                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    ControlsV1.Tab {
                        title: '+'
                    }

                    Component {
                        id: defaultProcess

                        Components_Templates.Process {
                            config: rootConfig.global
                        }
                    }

                    onCurrentIndexChanged: {
                        const currentProcess = getTab(currentIndex).children[0]
                        if (currentProcess) {
                            if (window.currentProcess)
                                window.currentProcess.active = false
                            window.currentProcess = currentProcess
                            window.currentProcess.active = true
                        }

                        if ((currentIndex == count - 1) && !addProcess) {
                            addProcess = true
                            insertTab(0, "Untitled", defaultProcess)
                            currentIndex = 0
                        } else {
                            addProcess = false
                        }
                    }

                    Component.onCompleted: {
                        insertTab(0, "Untitled", defaultProcess)
                        window.currentProcess = getTab(0).children[0]
                        window.currentProcess.active = true
                        //window.currentProcess.step_path('E:/Qt/ODigit/tests/camera9.jpg')
                        //window.currentProcess.step_detect()
                        //window.currentProcess.step_process()
                        //window.currentProcess.editor.simplify(5, 20, 3)
                        currentIndex = 0
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        direction = rootConfig.global.value('interface/appearance/direction', 'string') == 'rtl' ? Qt.RightToLeft : Qt.LeftToRight
    }
}
