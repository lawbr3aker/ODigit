import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.1 as ControlsV1
import QtQuick.Controls 2.15
import QtQuick.Controls.Styles 1.4

import scripts 1.0

RowLayout {
    property alias title: title
    property alias input: input

    Config {
        id: configThemes

        group: 'config-themes'
    }

    Text {
        id: title

        color: configThemes.value('colors/GFeV')
    }

    ComboBox {
        id: input

        property var currentItem

        onCurrentIndexChanged: {
            currentItem = model[currentIndex]
        }

        delegate: ItemDelegate {
            width: input.width
            height: 25

            contentItem: Text {
                text: modelData.text
                color: configThemes.value('colors/GFeV')
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                implicitWidth: parent.width
                implicitHeight: parent.height

                color: highlighted ? configThemes.value('colors/HVvi') : configThemes.value('colors/5v8I')
            }

            highlighted: input.highlightedIndex === index
        }

        contentItem: Text {
            leftPadding: 5
            rightPadding: leftPadding

            text: input.currentItem.text
            color: configThemes.value('colors/GFeV')

            verticalAlignment: Text.AlignVCenter

            elide: Text.ElideRight
        }

        background: Rectangle {
            radius: 3

            implicitWidth: 140
            implicitHeight: 25

            border {
              color: configThemes.value('colors/0LOj')
              width: 1
            }

            color: (input.hovered | input.focus) ? configThemes.value('colors/HVvi') : configThemes.value('colors/5v8I')
        }

        popup: Popup {
            y: input.height - 1
            width: input.width
            implicitHeight: contentItem.implicitHeight
            padding: 1

            contentItem: ListView {
                clip: true
                implicitHeight: contentHeight
                model: input.popup.visible ? input.delegateModel : null
                currentIndex: input.highlightedIndex

                ScrollIndicator.vertical: ScrollIndicator { }
            }

            background: Rectangle {
                radius: 3

                implicitWidth: 140
                implicitHeight: input.delegateModel.height

                border {
                  color: configThemes.value('colors/0LOj')
                  width: 1
                }

                color: (input.hovered | input.focus) ? configThemes.value('colors/HVvi') : configThemes.value('colors/5v8I')
            }
        }
    }
}
