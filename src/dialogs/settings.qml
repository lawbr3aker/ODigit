import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.3 as ControlsV1
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3

import "qrc:Components/Controls" as Components_Controls

import scripts 1.0

ControlsV1.ApplicationWindow {
    id: window

    title: "Settings"

    minimumWidth: 750
    minimumHeight: 520

    property var master

    property int direction

    Config {
        id: config
    }
    
    Translator {
        id: translator
    }

    MessageDialog {
        id: restart

        title: "Require restart"
        icon: StandardIcon.Question
        text: translator.global.tr('LQNW')
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
                color: 'black'
            }
            Text {
                id: title
            }
            Rectangle {
                height: 2
                Layout.fillWidth: true
                color: 'black'
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

                                    title.text: translator.global.tr('P4wQ')

                                    input.model: [
                                        { text: translator.global.tr('K8bT'), value: 'en' },
                                        { text: translator.global.tr('V1nE') , value: 'fa' }
                                    ]

                                    input.onCurrentIndexChanged: tabs.inputsChanged(true)
                                }
                            }

                            Loader {
                                Layout.fillWidth: true
                                Layout.topMargin: 10
                                sourceComponent: sectionHeader
                                onLoaded: {
                                    item.title.text = translator.global.tr('H7pK')
                                }
                            }

                            Row {
                                spacing: 50
                                Layout.fillWidth: true
                                layoutDirection: window.direction

                                Components_Controls.Choice {
                                    id: interfaceAppearanceStyle
                                    layoutDirection: window.direction

                                    title.text: translator.global.tr('C7oF')

                                    input.model: [
                                        { text: translator.global.tr('W3pG'), value: 'fusion' }
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

                                    title.text: translator.global.tr('B9qH')
                                    input.implicitWidth: 100

                                    input.onTextChanged: tabs.inputsChanged()
                                }

                                Components_Controls.Input {
                                    id: mainPlaneHeight
                                    layoutDirection: window.direction

                                    title.text: translator.global.tr('X4rI')
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

                                    title.text: translator.global.tr('A8sJ')
                                    input.implicitWidth: 70

                                    input.onValueChanged: tabs.inputsChanged()
                                }
                            }

                            Loader {
                                Layout.fillWidth: true
                                Layout.topMargin: 10
                                sourceComponent: sectionHeader
                                onLoaded: {
                                    item.title.text = translator.global.tr('F8iN')
                                }
                            }

                            Row {
                                spacing: 50
                                Layout.fillWidth: true
                                layoutDirection: window.direction

                                Components_Controls.Spinbox {
                                    id: detectorThreshold1
                                    layoutDirection: window.direction

                                    title.text: translator.global.tr('L2YP')
                                    input.implicitWidth: 100

                                    input.onValueChanged: tabs.inputsChanged()
                                }

                                Components_Controls.Spinbox {
                                    id: detectorThreshold2
                                    layoutDirection: window.direction

                                    title.text: translator.global.tr('P4PL')
                                    input.implicitWidth: 100

                                    input.onValueChanged: tabs.inputsChanged()
                                }
                            }

                            Row {
                                spacing: 60
                                Layout.fillWidth: true
                                layoutDirection: window.direction

                                Components_Controls.Spinbox {
                                    id: detectorEDSize
                                    layoutDirection: window.direction

                                    title.text: translator.global.tr('BujH')
                                    input.implicitWidth: 50

                                    input.onValueChanged: tabs.inputsChanged()
                                }
                            }

                            Loader {
                                Layout.fillWidth: true
                                Layout.topMargin: 10
                                sourceComponent: sectionHeader
                                onLoaded: {
                                    item.title.text = translator.global.tr('R4wM')
                                }
                            }

                            Row {
                                spacing: 60
                                Layout.fillWidth: true
                                layoutDirection: window.direction

                                Components_Controls.Spinbox {
                                    id: stabilizerBlurSize
                                    layoutDirection: window.direction

                                    title.text: translator.global.tr('SENH')
                                    input.implicitWidth: 50

                                    input.onValueChanged: tabs.inputsChanged()
                                }
                            }

                            Loader {
                                Layout.fillWidth: true
                                Layout.topMargin: 10
                                sourceComponent: sectionHeader
                                onLoaded: {
                                    item.title.text = translator.global.tr('A2tL')
                                }
                            }

                            Row {
                                spacing: 50
                                Layout.fillWidth: true
                                layoutDirection: window.direction

                                Components_Controls.Spinbox {
                                    id: fixerMinArea
                                    layoutDirection: window.direction

                                    title.text: translator.global.tr('FxhI')
                                    input.implicitWidth: 100

                                    input.onValueChanged: tabs.inputsChanged()
                                }

                                Components_Controls.Spinbox {
                                    id: fixerMaxDistance
                                    layoutDirection: window.direction

                                    title.text: translator.global.tr('YcEF')
                                    input.implicitWidth: 70

                                    input.onValueChanged: tabs.inputsChanged()
                                }

                                Components_Controls.Input {
                                    id: fixerEpsilon
                                    layoutDirection: window.direction

                                    title.text: translator.global.tr('D1aX')
                                    input.implicitWidth: 50

                                    input.onTextChanged: tabs.inputsChanged()
                                }
                            }

                            Loader {
                                Layout.fillWidth: true
                                Layout.topMargin: 10
                                sourceComponent: sectionHeader
                                onLoaded: {
                                    item.title.text = translator.global.tr('H3RJ')
                                }
                            }

                            Row {
                                spacing: 50
                                Layout.fillWidth: true
                                layoutDirection: window.direction

                                Components_Controls.Spinbox {
                                    id: simplifyThA
                                    layoutDirection: window.direction

                                    title.text: translator.global.tr('A92d')
                                    input.implicitWidth: 100

                                    input.onValueChanged: tabs.inputsChanged()
                                }

                                Components_Controls.Spinbox {
                                    id: simplifyThB
                                    layoutDirection: window.direction

                                    title.text: translator.global.tr('Nj1D')
                                    input.implicitWidth: 70

                                    input.onValueChanged: tabs.inputsChanged()
                                }
                            }
                        }
                    }
                }
            }

            function inputsLoad() {
                interfaceLanguage .input.currentIndex = interfaceLanguage.input.model.findIndex(x => x.value === config.global.value('interface/language'))
                interfaceAppearanceStyle.input.currentIndex = interfaceAppearanceStyle.input.model.findIndex(x => x.value === config.global.value('interface/appearance/style'))
                mainPlaneWidth    .input.value = config.global.value('plane_width')
                mainPlaneHeight   .input.value = config.global.value('plane_height')
                mainCMPixels      .input.value = config.global.value('cm_pixels')
                detectorThreshold1.input.value = config.global.value('scanner/threshold_1')
                detectorThreshold2.input.value = config.global.value('scanner/threshold_2')
                detectorEDSize    .input.value = config.global.value('scanner/fixer/erode_dilate_size')
                stabilizerBlurSize.input.value = config.global.value('scanner/blur_size')
                fixerMinArea      .input.value = config.global.value('scanner/fixer/min_area')
                fixerMaxDistance  .input.value = config.global.value('scanner/fixer/max_gap')
                fixerEpsilon      .input.value = config.global.value('scanner/fixer/epsilon')
                simplifyThA       .input.value = config.global.value('scanner/simplify/default_threshold_a')
                simplifyThB       .input.value = config.global.value('scanner/simplify/default_threshold_b')
            }

            function inputsSave() {
                const language = interfaceLanguage.input.model[interfaceLanguage.input.currentIndex].value
                config.global.set('interface/language'                      , language)
                config.global.set('interface/appearance/style'              , interfaceAppearanceStyle.input.model[interfaceAppearanceStyle.input.currentIndex].value)
                config.global.set('interface/appearance/direction'          , language === 'fa' ? 'rtl' : 'ltr')
                config.global.set('plane_width'                             , mainPlaneWidth            .input.value)
                config.global.set('plane_height'                            , mainPlaneHeight           .input.value)
                config.global.set('cm_pixels'                               , mainCMPixels              .input.value)
                config.global.set('scanner/threshold_1'                     , detectorThreshold1        .input.value)
                config.global.set('scanner/threshold_2'                     , detectorThreshold2        .input.value)
                config.global.set('scanner/fixer/erode_dilate_size'         , detectorEDSize            .input.value)
                config.global.set('scanner/blur_size'                       , stabilizerBlurSize        .input.value)
                config.global.set('scanner/fixer/min_area'                  , fixerMinArea              .input.value)
                config.global.set('scanner/fixer/max_gap'                   , fixerMaxDistance          .input.value)
                config.global.set('scanner/fixer/epsilon'                   , fixerEpsilon              .input.value)
                config.global.set('scanner/simplify/default_threshold_a'    , simplifyThA               .input.value)
                config.global.set('scanner/simplify/default_threshold_b'    , simplifyThB               .input.value)
                config.global.save()
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

                text: translator.global.tr('OeZG')

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
                text: translator.global.tr('NtjB')

                onClicked: {
                    tabs.inputsLoad()
                    window.close()
                }
            }
        }
    }

    Component.onCompleted: {
        direction = config.global.value('interface/appearance/direction', 'string') == 'rtl' ? Qt.RightToLeft : Qt.LeftToRight
    }
}
