import QtQuick 2.15
import QtQuick.Controls 1.2 as ControlsV1
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ControlsV1.Button {
  id: root

  implicitHeight: 34

  readonly property alias icon : icon
  readonly property alias title: title
  readonly property alias hint : hint

  ColumnLayout {
    anchors.fill: parent

    spacing: 0

    Item {
      Layout. fillWidth: true
      Layout.fillHeight: true

      Layout.margins: icon.parent.anchors.margins

      Item {
        width: parent.width

        anchors.margins: 10

        Image {
          id: icon

          anchors.horizontalCenter: parent.horizontalCenter

           width: parent.width / implicitWidth * implicitHeight < parent.height ? parent.width : parent.height / implicitHeight * implicitWidth
          height: width / implicitWidth * implicitHeight
        }
      }

      Component.onCompleted: {
        if (icon.implicitHeight === 0) {
          Layout.maximumHeight = 0
          Layout.margins = 0
        }
      }
    }

    Text {
      id: title

      Layout.preferredWidth: parent.width
      Layout.  bottomMargin: 4
                leftPadding: 5
               rightPadding: leftPadding

                 wrapMode: Text.WordWrap
      horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignBottom

      Component.onCompleted: {
        if (title.text == '') {
            Layout.maximumHeight = 0
        }
        if (icon.implicitHeight === 0) {
            Layout.bottomMargin = 0
            verticalAlignment = Text.AlignVCenter
        }
      }
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
