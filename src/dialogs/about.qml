import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.3 as ControlsV1
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3

ControlsV1.ApplicationWindow {
    title: "About"
    
    property var globalW

     minimumWidth: 271
     maximumWidth: minimumWidth
    minimumHeight: mainContainer.implicitHeight
    maximumHeight: minimumHeight

    Rectangle {
        anchors.fill: parent
        color: globalW.globalThemes.value('colors/IlPR')
    }

    ColumnLayout {
        id: mainContainer
        width: parent.width
        spacing: 0

        ColumnLayout {
            id: banner

            Layout.      alignment: Qt.AlignHCenter
            Layout.      fillWidth: true
            height: 210

            Image {
                Layout.      alignment: Qt.AlignHCenter
                Layout.      topMargin: 10
                Layout. preferredWidth: 100
                Layout.preferredHeight: 100

                source: 'qrc:/Assets/Images/logo'
            }

            Text {
                Layout.topMargin: 10
                Layout.bottomMargin: 10

                              color: globalW.globalThemes.value('colors/GFeV')
                               text: globalW.globalTranslator.tr('UlcH')
                     font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
            }
        }

        ColumnLayout {
            id: temp
            Layout. fillWidth: true

            Rectangle {
                anchors.fill: parent
                color: globalW.globalThemes.value('colors/uK6i')
            }

            Text {
                id: description

                Layout.fillWidth: true
                Layout.margins: 17

                         color: globalW.globalThemes.value('colors/GFeV')

                    textFormat: Text.RichText
                      wrapMode: Text.WordWrap
                font.pixelSize: 14
                horizontalAlignment: Text.AlignHCenter

                text: globalW.globalTranslator.tr('MV2O')
            }

            Text {
                id: license

                Layout.fillWidth: true
                Layout.margins: 17

                         color: globalW.globalThemes.value('colors/GFeV')

                    textFormat: Text.RichText
                      wrapMode: Text.WordWrap
                font.pixelSize: 14

                Component.onCompleted: {
                    function formatString(template, ...args) {
                        var i = 0;
                        return template.replace(/{}/g, function() {
                            return typeof args[i] !== 'undefined' ? args[i++] : '{}';
                        });
                    }

                    //globalW.globalConfig.value('scanner/fixer/min_area')

                    text = formatString(globalW.globalTranslator.tr('I2nu'), "<span style='color: green;'>Activated</span>", "Unlimited")
                }
            }
        }
    }
}
