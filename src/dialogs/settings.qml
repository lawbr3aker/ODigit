import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.3 as ControlsV1
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls.Styles 1.4

import "qrc:Components/Controls" as Components_Controls

import scripts 1.0

ControlsV1.ApplicationWindow {
    id: window

    required property var globalW

    title: "Settings"

    minimumWidth: 750
    minimumHeight: 520

    property int direction

    Rectangle {
        anchors.fill: parent

        color: globalW.globalThemes.value('colors/IlPR')
    }

    MessageDialog {
        id: restart

        title: "Require restart"
        icon: StandardIcon.Question
        text: `${globalW.globalTranslator.tr('LQNW')}`
        standardButtons: StandardButton.Yes | StandardButton.No

        onYes: {
            tabs.inputsSave()
            window.close()
            currentProcess.data.restartProgram()
        }
        onNo: {
            close()
        }
    }

    Component {
        id: sectionHeader

        RowLayout {
            property alias title: title

            Layout.bottomMargin: 5
            layoutDirection: window.direction

            Rectangle {
                height: 2
                width: 40
                color: globalW.globalThemes.value('colors/Ijk1')
            }
            Text {
                id: title

                color: globalW.globalThemes.value('colors/Ijk1')
            }
            Rectangle {
                height: 2
                Layout.fillWidth: true
                color: globalW.globalThemes.value('colors/Ijk1')
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent

        spacing: 0

        ControlsV1.TabView {
            id: tabs

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 10

            style: TabViewStyle {
                id: st12
                frameOverlap: 1
                tab: Rectangle {
                    y: styleData.selected ? 4 : 6

                    color: styleData.selected ? globalW.globalThemes.value('colors/uK6i') : globalW.globalThemes.value('colors/UrkI')
                    border.width: 1
                    border.color: globalW.globalThemes.value('colors/iIyV')

                    implicitWidth: 100
                    height: styleData.selected ? 33 : 29

                    radius: styleData.selected ? 4 : 2

                    Text {
                        id: text
                        anchors.centerIn: parent
                        text: styleData.title
                        font.pixelSize: 13
                        anchors.bottomMargin: 2
                        color: globalW.globalThemes.value('colors/GFeV')
                    }
                }

                frame: Rectangle {
                    border.width: 1
                    border.color: globalW.globalThemes.value('colors/iIyV')
                    color: globalW.globalThemes.value('colors/uK6i')
                }
            }

            ControlsV1.Tab {
                id: tabInterface
                anchors.leftMargin: 7
                anchors.topMargin: 7
                anchors.rightMargin: 7
                anchors.fill: parent

                title: "Interface"

                readonly property Item container: Item {
                    parent: tabInterface

                    anchors.fill: parent
                    anchors.margins: 8

                    Item {
                        width: parent.width

                        ColumnLayout {
                            width: parent.width

                            Row {
                                spacing: 50
                                Layout.fillWidth: true
                                layoutDirection: window.direction

                                Components_Controls.Choice {
                                    id: interfaceLanguage
                                    layoutDirection: window.direction

                                    title.text: `${globalW.globalTranslator.tr('P4wQ')}`

                                    input.model: [
                                        { text: `${globalW.globalTranslator.tr('K8bT')}`, value: 'en' },
                                        { text: `${globalW.globalTranslator.tr('V1nE')}` , value: 'fa' }
                                    ]

                                    input.onCurrentIndexChanged: tabs.inputsChanged(true)
                                }
                            }

                            Loader {
                                Layout.fillWidth: true
                                Layout.topMargin: 10
                                sourceComponent: sectionHeader
                                onLoaded: {
                                    item.title.text = `${globalW.globalTranslator.tr('H7pK')}`
                                }
                            }

                            Row {
                                spacing: 50
                                Layout.fillWidth: true
                                layoutDirection: window.direction

                                Components_Controls.Choice {
                                    id: interfaceAppearanceStyle
                                    layoutDirection: window.direction

                                    title.text: `${globalW.globalTranslator.tr('C7oF')}`

                                    input.model: [
                                        { text: `${globalW.globalTranslator.tr('W3pG')}`, value: 'fusion' }
                                    ]

                                    input.onCurrentIndexChanged: tabs.inputsChanged(true)
                                }

                                Components_Controls.Choice {
                                    id: interfaceAppearanceTheme
                                    layoutDirection: window.direction

                                    title.text: `${globalW.globalTranslator.tr('ASJ1')}`

                                    input.model: [
                                        { text: `${globalW.globalTranslator.tr('Bdhj')}`, value: 'light' },
                                        { text: `${globalW.globalTranslator.tr('Sjz2')}`, value: 'dark' }
                                    ]

                                    input.onCurrentIndexChanged: tabs.inputsChanged(true)
                                }
                            }
                        }
                    }
                }
            }

            ControlsV1.Tab {
                id: tabScanner
                anchors.leftMargin: 7
                anchors.topMargin: 7
                anchors.rightMargin: 7
                anchors.fill: parent

                title: "Scanner"

                readonly property Item container: ScrollView {
                    parent: tabScanner

                    anchors.fill: parent
                    anchors.margins: 8

                    clip: true

                    Item {
                        width: parent.width

                        ColumnLayout {
                            width: parent.width

                            Row {
                                spacing: 50
                                Layout.fillWidth: true
                                layoutDirection: window.direction

                                Components_Controls.Input {
                                    id: mainPlaneWidth
                                    layoutDirection: window.direction

                                    title.text: `${globalW.globalTranslator.tr('B9qH')}`
                                    input.implicitWidth: 100

                                    input.onTextChanged: tabs.inputsChanged()
                                }

                                Components_Controls.Input {
                                    id: mainPlaneHeight
                                    layoutDirection: window.direction

                                    title.text: `${globalW.globalTranslator.tr('X4rI')}`
                                    input.implicitWidth: 100

                                    input.onTextChanged: tabs.inputsChanged()
                                }
                            }

                            Row {
                                spacing: 60
                                Layout.fillWidth: true
                                layoutDirection: window.direction

                                Components_Controls.Spinbox {
                                    id: mainCMPixels
                                    layoutDirection: window.direction

                                    title.text: `${globalW.globalTranslator.tr('A8sJ')}`
                                    input.implicitWidth: 70

                                    input.onValueChanged: tabs.inputsChanged()
                                }
                            }

                            Loader {
                                Layout.fillWidth: true
                                Layout.topMargin: 10
                                sourceComponent: sectionHeader
                                onLoaded: {
                                    item.title.text = `${globalW.globalTranslator.tr('F8iN')}`
                                }
                            }

                            Row {
                                spacing: 60
                                Layout.fillWidth: true
                                layoutDirection: window.direction

                                Components_Controls.Spinbox {
                                    id: detectorEDSize
                                    layoutDirection: window.direction

                                    title.text: `${globalW.globalTranslator.tr('BujH')}`
                                    input.implicitWidth: 50

                                    input.onValueChanged: tabs.inputsChanged()
                                }
                            }

                            Loader {
                                Layout.fillWidth: true
                                Layout.topMargin: 10
                                sourceComponent: sectionHeader
                                onLoaded: {
                                    item.title.text = `${globalW.globalTranslator.tr('A2tL')}`
                                }
                            }

                            Row {
                                spacing: 50
                                Layout.fillWidth: true
                                layoutDirection: window.direction

                                Components_Controls.Spinbox {
                                    id: fixerMinArea
                                    layoutDirection: window.direction

                                    title.text: `${globalW.globalTranslator.tr('FxhI')}`
                                    input.implicitWidth: 100

                                    input.onValueChanged: tabs.inputsChanged()
                                }

                                Components_Controls.Spinbox {
                                    id: fixerMaxDistance
                                    layoutDirection: window.direction

                                    title.text: `${globalW.globalTranslator.tr('YcEF')}`
                                    input.implicitWidth: 70

                                    input.onValueChanged: tabs.inputsChanged()
                                }

                                Components_Controls.Input {
                                    id: fixerEpsilon
                                    layoutDirection: window.direction

                                    title.text: `${globalW.globalTranslator.tr('D1aX')}`
                                    input.implicitWidth: 50

                                    input.onTextChanged: tabs.inputsChanged()
                                }
                            }

                            Loader {
                                Layout.fillWidth: true
                                Layout.topMargin: 10
                                sourceComponent: sectionHeader
                                onLoaded: {
                                    item.title.text = `${globalW.globalTranslator.tr('H3RJ')}`
                                }
                            }

                            Row {
                                spacing: 50
                                Layout.fillWidth: true
                                layoutDirection: window.direction

                                Components_Controls.Spinbox {
                                    id: simplifyThA
                                    layoutDirection: window.direction

                                    title.text: `${globalW.globalTranslator.tr('A92d')}`
                                    input.implicitWidth: 100

                                    input.onValueChanged: tabs.inputsChanged()
                                }

                                Components_Controls.Spinbox {
                                    id: simplifyThB
                                    layoutDirection: window.direction

                                    title.text: `${globalW.globalTranslator.tr('Nj1D')}`
                                    input.implicitWidth: 70

                                    input.onValueChanged: tabs.inputsChanged()
                                }
                            }

                            Row {
                                spacing: 50
                                Layout.fillWidth: true
                                layoutDirection: window.direction

                                Components_Controls.Spinbox {
                                    id: simplifyThIts
                                    layoutDirection: window.direction

                                    title.text: `${globalW.globalTranslator.tr('DAd5')}`
                                    input.implicitWidth: 70

                                    input.onValueChanged: tabs.inputsChanged()
                                }
                            }
                        }
                    }
                }
            }

            function inputsLoad() {
                interfaceLanguage .input.currentIndex = interfaceLanguage.input.model.findIndex(x => x.value === globalW.globalConfig.value('interface/language'))
                interfaceAppearanceStyle.input.currentIndex = interfaceAppearanceStyle.input.model.findIndex(x => x.value === globalW.globalConfig.value('interface/appearance/style'))
                interfaceAppearanceTheme.input.currentIndex = interfaceAppearanceTheme.input.model.findIndex(x => x.value === globalW.globalConfig.value('interface/appearance/theme'))
                mainPlaneWidth    .input.value = globalW.globalConfig.value('plane_width')
                mainPlaneHeight   .input.value = globalW.globalConfig.value('plane_height')
                mainCMPixels      .input.value = globalW.globalConfig.value('cm_pixels')
                detectorEDSize    .input.value = globalW.globalConfig.value('scanner/fixer/dilate_size')
                fixerMinArea      .input.value = globalW.globalConfig.value('scanner/fixer/min_area')
                fixerMaxDistance  .input.value = globalW.globalConfig.value('scanner/fixer/max_gap')
                fixerEpsilon      .input.value = globalW.globalConfig.value('scanner/fixer/epsilon')
                simplifyThA       .input.value = globalW.globalConfig.value('scanner/simplify/default_threshold_a')
                simplifyThB       .input.value = globalW.globalConfig.value('scanner/simplify/default_threshold_b')
                simplifyThIts     .input.value = globalW.globalConfig.value('scanner/simplify/default_iterations')
            }

            function inputsSave() {
                const language = interfaceLanguage.input.model[interfaceLanguage.input.currentIndex].value
                globalW.globalConfig.set('interface/language'                      , language)
                globalW.globalConfig.set('interface/appearance/style'              , interfaceAppearanceStyle.input.model[interfaceAppearanceStyle.input.currentIndex].value)
                globalW.globalConfig.set('interface/appearance/theme'              , interfaceAppearanceTheme.input.model[interfaceAppearanceTheme.input.currentIndex].value)
                globalW.globalConfig.set('interface/appearance/direction'          , language === 'fa' ? 'rtl' : 'ltr')
                globalW.globalConfig.set('plane_width'                             , mainPlaneWidth            .input.value)
                globalW.globalConfig.set('plane_height'                            , mainPlaneHeight           .input.value)
                globalW.globalConfig.set('cm_pixels'                               , mainCMPixels              .input.value)
                globalW.globalConfig.set('scanner/fixer/dilate_size'               , detectorEDSize            .input.value)
                globalW.globalConfig.set('scanner/fixer/min_area'                  , fixerMinArea              .input.value)
                globalW.globalConfig.set('scanner/fixer/max_gap'                   , fixerMaxDistance          .input.value)
                globalW.globalConfig.set('scanner/fixer/epsilon'                   , fixerEpsilon              .input.value)
                globalW.globalConfig.set('scanner/simplify/default_threshold_a'    , simplifyThA               .input.value)
                globalW.globalConfig.set('scanner/simplify/default_threshold_b'    , simplifyThB               .input.value)
                globalW.globalConfig.set('scanner/simplify/default_iterations'     , simplifyThIts             .input.value)
                globalW.globalConfig.save()
            }

            function inputsChanged(restart) {
                if (inputsChanged.active) {
                    applyButton.enabled = true
                    if (restart)
                        inputsChanged.restart = true
                }
            }

            Component.onCompleted: {
                inputsLoad()
                inputsChanged.active = true
            }
        }

        RowLayout {
            Layout.minimumHeight: applyButton.height
            Layout.margins: 10
            Layout.topMargin: 0
            Layout.alignment: Qt.AlignRight

            Components_Controls.Button {
                id: applyButton
                Layout.minimumWidth: 130

                title.text: `${globalW.globalTranslator.tr('OeZG')}`

                enabled: false

                onClicked: {
                    if (tabs.inputsChanged.restart) {
                        restart.open()
                    } else {
                        tabs.inputsSave()
                        window.close()
                    }
                }
            }

            Components_Controls.Button {
                Layout.minimumWidth: 80
                title.text: `${globalW.globalTranslator.tr('NtjB')}`

                onClicked: {
                    tabs.inputsLoad()
                    window.close()
                }
            }
        }
    }

    Component.onCompleted: {
        direction = globalW.globalConfig.value('interface/appearance/direction') == 'rtl' ? Qt.RightToLeft : Qt.LeftToRight
    }
}
