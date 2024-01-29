import QtQuick 2.15
import QtQuick.Controls 1.2 as ControlsV1
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ControlsV1.Button {
  id: root

  readonly property alias icon : icon
  readonly property alias title: title
  readonly property alias hint : hint

  ColumnLayout {
    anchors.fill: parent

    spacing: 0

    Image {
      id: icon

      Layout.      alignment: Qt.AlignTop | Qt.AlignHCenter
      Layout.preferredHeight: Math.min(45, parent.height - 2 * Layout.topMargin)
      Layout. preferredWidth: implicitWidth / implicitHeight * Layout.preferredHeight
      Layout.      topMargin: 7
    }

    Text {
      id: title

      property string text

      Layout.preferredWidth: parent.width
      Layout.    fillHeight: true
      Layout.  bottomMargin: 3
                leftPadding: 5
               rightPadding: leftPadding

                 wrapMode: Text.WordWrap
      horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignBottom
           font.pixelSize: Qt.application.font.pixelSize * 0.9
    }
  }

  MouseArea {
    id: hintMouse

    anchors.fill: parent

       hoverEnabled: true
    acceptedButtons: Qt.RightButton

    onPositionChanged: {
      if (hint.text) {
        hintContainer.visible = false
        hintTimer.restart()
      }
      mouse.accepted = false
    }

    onExited: {
      if (hint.text) {
        hintContainer.visible = false
        hintTimer.stop()
      }
    }
    
    Timer {
      id: hintTimer
  
      interval: 400
      running: false
  
      onTriggered: {
        hintContainer.x = hintMouse.mouseX
        hintContainer.y = hintMouse.mouseY - hintContainer.height
        hintContainer.visible = true
      }
    }
    
    Rectangle {
      id: hintContainer
  
      visible: false
  
      color: '#444'
  
      width: hint.width + 16
      height: hint.height + 6
  
      Label {
        id: hint
  
        enabled: false
        anchors.centerIn: parent
  
        color: '#F8F8F8'
  
        onTextChanged: {
          enabled = true
        }
      }
    }
  }

  onPressedChanged: {
    if (pressed)
      forceActiveFocus()
  }
}
