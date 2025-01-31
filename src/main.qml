import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.3 as ControlsV1
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls.Styles 1.4
import Qt.labs.settings 1.0
import QtQml 2.15

import "qrc:Dialogs" as Dialogs
import "qrc:/Components/Controls" as Components_Controls
import "qrc:/Components/Templates" as Components_Templates
import "qrc:/Components/Templates/Toolbar" as Components_Templates_Toolbar

import scripts 1.0

ApplicationWindow {
    id: window
    minimumWidth: 850
    minimumHeight: 650
    visible: true
    title: "ODigit 25.1"
    modality: Qt.ApplicationModal

    property int direction

    property var currentProcess

    property Config globalConfig: Config {
        id: rootConfig

        group: "config-main"
    }

    property Config globalThemes: Config {
        id: rootThemes

        group: "config-themes"
    }

    property Config globalTranslator: Config {
        id: rootTranslator

        function tr(path) {
            return value(path)
        }
    }

    Loader {
        id: mainLoader

        active: false

        anchors.fill: parent

        sourceComponent: Item  {
            id: context

            Dialogs.Registration {
                id: registrationDialog

                globalW: window

                modality: Qt.ApplicationModal

                Registration {
                    id: registration
                }

                onClosing: {
                    if (!succeed)
                        Qt.quit()
                }

                Timer {
                    id: fps

                    interval: 1000 * 60 * 3
                     running: true
                      repeat: false

                    onTriggered: {
                        const key  = rootConfig.value('license/key')
                        const seed = rootConfig.value('license/seed')

                        if (!registration.check_key(key, seed))
                            registrationDialog.show()
                    }
                }
            }

            Rectangle {
                anchors.fill: parent

                color: rootThemes.value('colors/IlPR')
            }

            ColumnLayout {
                spacing: 7

                anchors.fill: parent
                anchors.margins: 10

                ControlsV1.TabView {
                    style: TabViewStyle {
                        frameOverlap: 1
                        tab: Rectangle {
                            y: styleData.selected ? 4 : 6
                            color: rootThemes.value('colors/uK6i')
                            border.width: 1
                            border.color: rootThemes.value('colors/iIyV')

                            implicitWidth: 100
                            height: 33

                            radius: styleData.selected ? 4 : 2

                            Text {
                                id: text
                                anchors.centerIn: parent
                                text: styleData.title
                                font.pixelSize: 13
                                anchors.bottomMargin: 2
                                color: rootThemes.value('colors/GFeV')
                            }
                        }

                        frame: Rectangle {
                            border.width: 1
                            border.color: rootThemes.value('colors/iIyV')
                            color: rootThemes.value('colors/uK6i')
                        }
                    }

                    Layout.fillWidth: true
                    Layout.preferredHeight: 170

                    ControlsV1.Tab {
                        anchors.leftMargin: 7
                        anchors.topMargin: 7
                        anchors.rightMargin: 7
                        anchors.fill: parent

                        title: "Home"

                        ColumnLayout {
                            anchors.fill: parent

                            Components_Templates_Toolbar.Home {
                                z: 1
                                height: parent.height

                                globalW: window
                            }
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    spacing: parent.spacing

                    ColumnLayout {
                        z: 2
                        Layout.alignment: Qt.AlignTop
                        Layout.fillHeight: true
                        Layout.minimumHeight: 370
                        Layout.maximumWidth: 43
                        Layout.minimumWidth: Layout.maximumWidth

                        Components_Templates_Toolbar.Side {
                            id: sidebar
                            z: 5

                            height: 300
                            Layout.alignment: Qt.AlignTop
                            Layout.fillWidth: true
                            Layout.margins: 5

                            globalW: window

                            Connections {
                                target: window.currentProcess.editor.input

                                function onKeyChanged(key, state) {
                                    if ((key === Qt.RightButton || key === Qt.Key_Escape) && state === 0) {
                                        sidebar.resetAll()
                                    }
                                }
                            }
                        }

                        Rectangle {
                            width: parent.width
                            height: parent.height

                            border.width: 1
                            border.color: rootThemes.value('colors/iIyV')
                            color: rootThemes.value('colors/uK6i')
                        }
                    }

                    ColumnLayout {
                        z: 1
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        ControlsV1.TabView {
                            id: processes

                            style: TabViewStyle {
                                frameOverlap: 1
                                tab: Rectangle {
                                    y: styleData.selected ? 4 : 6

                                    color: styleData.selected ? rootThemes.value('colors/uK6i') : rootThemes.value('colors/UrkI')
                                    border.width: 1
                                    border.color: rootThemes.value('colors/iIyV')

                                    implicitWidth: 100
                                    height: styleData.selected ? 33 : 29

                                    radius: styleData.selected ? 4 : 2

                                    Text {
                                        id: text
                                        anchors.centerIn: parent
                                        text: styleData.title
                                        font.pixelSize: 13
                                        anchors.bottomMargin: 2
                                        color: rootThemes.value('colors/GFeV')
                                    }
                                }

                                frame: Rectangle {
                                    border.width: 1
                                    border.color: rootThemes.value('colors/iIyV')
                                    color: rootThemes.value('colors/uK6i')
                                }
                            }

                            property bool addProcess: false

                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            ControlsV1.Tab {
                                title: '+'
                            }

                            Component {
                                id: defaultProcess

                                Components_Templates.Process {
                                    config: rootConfig
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
        }
    }

    Component.onCompleted: {
        rootTranslator.load(`:/Translations/${rootConfig.value('interface/language')}`)
        rootThemes.load(`:/Assets/Themes/${rootConfig.value('interface/appearance/theme')}`)

        mainLoader.active = true

        direction = rootConfig.value('interface/appearance/direction', 'string') == 'rtl' ? Qt.RightToLeft : Qt.LeftToRight
    }
}
