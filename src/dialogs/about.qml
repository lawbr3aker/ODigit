import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.3 as ControlsV1
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3

ControlsV1.ApplicationWindow {
    title: "About"
    
    property var globalW

     minimumWidth: 280
     maximumWidth: minimumWidth
    minimumHeight: description.height + banner.height
    maximumHeight: minimumHeight

    Rectangle {
        anchors.fill: parent
        color: globalW.globalThemes.value('colors/IlPR')
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        ColumnLayout {
            id: banner

            Layout.      alignment: Qt.AlignHCenter
            Layout.      fillWidth: true
            Layout.preferredHeight: 210

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

        Rectangle {
            Layout. fillWidth: true
            Layout.fillHeight: true

            color: globalW.globalThemes.value('colors/uK6i')

            Text {
                id: description

                           width: parent.width
                anchors.centerIn: parent
                         padding: 17

                         color: globalW.globalThemes.value('colors/GFeV')
                          text: globalW.globalTranslator.tr('MV2O')
                    textFormat: Text.RichText
                      wrapMode: Text.WordWrap
                font.pixelSize: 14
            }
        }
    }
}
