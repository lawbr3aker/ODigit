import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

import "qrc:Components/Controls" as Components_Controls
import "qrc:Components/Dialogs/Helpers" as Components_Dialogs_Helpers

ColumnLayout {
    property var window

    spacing: 5

    function resetAll() {
        toolMeasure.enable = false
        toolPoint.enable = false
        toolLine.enable = false
    }

    Components_Controls.Button {
        Layout.fillWidth: true
        Layout.preferredHeight: width

        icon.source: 'qrc:Assets/Images/Icons/cursor'
        hint.text: "Select"

        onClicked: {
            resetAll()
            window.currentProcess.editor.input.callbacks = []
        }
    }

    Components_Controls.Button {
        Layout.fillWidth: true
        Layout.preferredHeight: width

        icon.source: 'qrc:Assets/Images/Icons/home'
        hint.text: "Home"

        onClicked: {
            resetAll()
            window.currentProcess.editor.home()
            window.currentProcess.editor.update()
        }
    }

    Components_Controls.Button {
        Layout.fillWidth: true
        Layout.preferredHeight: width

        icon.source: 'qrc:Assets/Images/Icons/zoom-in'
        hint.text: "Zoom in"

        onClicked: {
            window.currentProcess.editor.zoom += 0.15
            window.currentProcess.editor.update()
        }
    }

    Components_Controls.Button {
        Layout.fillWidth: true
        Layout.preferredHeight: width

        icon.source: 'qrc:Assets/Images/Icons/zoom-out'
        hint.text: "Zoom out"

        onClicked: {
            window.currentProcess.editor.zoom -= 0.15
            window.currentProcess.editor.update()
        }
    }

    Components_Controls.Button {
        id: toolPoint
        Layout.fillWidth: true
        Layout.preferredHeight: width

        icon.source: 'qrc:Assets/Images/Icons/point'
        hint.text: "Draw point"

        property bool enable: false

        onClicked: {
            if (!enable) {
                resetAll()
                enable = true

                window.currentProcess.editor.input.callbacks.push(
                    function callback(key, active) {
                        if (key === Qt.LeftButton && active) {
                            const x = (window.currentProcess.editor.input.mouseX - window.currentProcess.editor.pan.x) / window.currentProcess.editor.zoom
                            const y = (window.currentProcess.editor.input.mouseY - window.currentProcess.editor.pan.y) / window.currentProcess.editor.zoom

                            window.currentProcess.editor.add_point(x, y)
                            window.currentProcess.editor.update()

                            toolPoint.enable = false
                        }

                        return toolPoint.enable
                    }
                )
            }
        }
    }

    Components_Controls.Button {
        id: toolLine

        Layout.      fillWidth: true
        Layout.preferredHeight: width

        hint.  text: "Draw line"
        icon.source: 'qrc:Assets/Images/Icons/line'

        property bool enable: false

        onClicked: {
            if (!enable) {
                resetAll()
                enable = true

                window.currentProcess.editor.input.callbacks.push(callback)
            }
        }

        function callback(key, active) {
            if (!toolLine.enable)
                return

            if (key === Qt.LeftButton && active) {
                let hovered = window.currentProcess.editor.input.hoveredPoint
                if (!hovered) {
                    const x = (window.currentProcess.editor.input.mouseX - window.currentProcess.editor.pan.x) / window.currentProcess.editor.zoom
                    const y = (window.currentProcess.editor.input.mouseY - window.currentProcess.editor.pan.y) / window.currentProcess.editor.zoom

                    hovered = window.currentProcess.editor.add_point(x, y)
                    window.currentProcess.editor.update()
                }

                if (callback.s) {
                    if (hovered !== callback.s) {
                        window.currentProcess.editor.add_line(callback.s, hovered)
                        window.currentProcess.editor.update()

                        toolLine.enable = false

                        window.currentProcess.editor.input.callbacks.push(
                            function (key, active) {
                                if (key === Qt.LeftButton) {
                                    if (window.currentProcess.editor.input.hoveredPoint) {
                                        window.currentProcess.editor.input.hoveredPoint = undefined

                                        return false
                                    }
                                }

                                return true
                            }
                        )
                    }
                } else
                    callback.s = hovered
            }

            return true
        }
    }

    Components_Controls.Button {
        id: toolMeasure
        Layout.fillWidth: true
        Layout.preferredHeight: width

        icon.source: 'qrc:Assets/Images/Icons/measure'
        hint.text: "Measure"

        property bool enable: false

        onClicked: {
            if (!enable) {
                resetAll()
                enable = true

                callback.active = true
                window.currentProcess.editor.input.callbacks.push(callback)
            } else {
                delete callback.s
                callback.alive = false
                callback.active = true
            }
        }

        onEnableChanged: {
            if (!enable) {
                delete callback.s
                callback.alive = false
                callback.active = false
            }
        }

        function callback(key, active) {
            if (key === Qt.LeftButton && active) {
                if (!callback.s) {
                    callback.s = {
                        x: (window.currentProcess.editor.input.mouseX - window.currentProcess.editor.pan.x) / window.currentProcess.editor.zoom,
                        y: (window.currentProcess.editor.input.mouseY - window.currentProcess.editor.pan.y) / window.currentProcess.editor.zoom
                    }

                    window.currentProcess.editor.callbacks.push(
                        function ruler(ctx, px, py, z) {
                            if (!callback.active)
                                return

                            callback.e = {
                                x: (window.currentProcess.editor.input.mouseX - px) / z,
                                y: (window.currentProcess.editor.input.mouseY - py) / z
                            }

                            ctx.lineWidth   = 3
                            ctx.strokeStyle = "#FF7D7D"
                            ctx.beginPath()
                            ctx.moveTo(callback.s.x * z + px, callback.s.y * z + py)
                            ctx.lineTo(callback.e.x * z + px, callback.e.y * z + py)
                            ctx.stroke()

                            return true
                        }
                    )
                } else {
                    callback.active = false
                    callback.alive = true

                    window.currentProcess.editor.callbacks.push(
                        function distance(ctx, px, py, z) {
                            if (!callback.alive)
                                return

                            ctx.lineWidth   = 3
                            ctx.strokeStyle = "#FF7D7D"
                            ctx.fillStyle   = "red"
                            ctx.font        = '23px Arial Bold'
                            ctx.beginPath()
                            ctx.moveTo(callback.s.x * z + px, callback.s.y * z + py)
                            ctx.lineTo(callback.e.x * z + px, callback.e.y * z + py)
                            ctx.stroke()
                            if (distance.angle === undefined) {
                                distance.angle   = Math.atan((callback.e.y - callback.s.y) / (callback.e.x - callback.s.x))
                                distance.value   = Math.sqrt(Math.pow(callback.e.y - callback.s.y, 2) + Math.pow(callback.e.x - callback.s.x, 2))
                                distance.text    = `${(distance.value / window.currentProcess.config.value('cm_pixels', 'int')).toFixed(2)}CM`
                                distance.metrics = ctx.measureText(distance.text)
                                distance.point   = {
                                    x: (callback.s.x + callback.e.x) / 2 - distance.metrics.width,
                                    y: (callback.s.y + callback.e.y) / 2
                                }
                            }
                            ctx.fillText(distance.text,
                                distance.point.x * z + px,
                                distance.point.y * z + py
                            )

                            return true
                        }
                    )
                }
            }

            return toolMeasure.enable
        }
    }

    Components_Dialogs_Helpers.AddText {
        id: addText

        onVisibleChanged: {
            if (!visible) {
                toolAddText.enable = false
            } else {
                value.text = ''
            }
        }

        after:
            function() {
                for (let i = 0, row = 1; i < addText.value.text.length; i += toolAddText.callback.columns, ++row) {
                    window.currentProcess.editor.addText(
                        toolAddText.callback.s.x - window.currentProcess.editor.pan.x, toolAddText.callback.s.y + addText.fontSize * row - window.currentProcess.editor.pan.y,
                        toolAddText.callback.size.w, toolAddText.callback.size.h, window.currentProcess.editor.zoom,
                        addText.value.text.substr(i, toolAddText.callback.columns), "Consolas", addText.fontSize, addText.fontBold, addText.fontItalic
                    )
                }
                window.currentProcess.editor.update()
                toolAddText.enable = false
                addText.close()
            }
    }

    Components_Controls.Button {
        id: toolAddText
        Layout.fillWidth: true
        Layout.preferredHeight: width

        icon.source: 'qrc:Assets/Images/Icons/text'
        hint.text: "Add text"

        property bool enable: false

        onClicked: {
            if (!enable) {
                resetAll()
                enable = true

                window.currentProcess.editor.input.callbacks.push(callback)
            } else {
            }
        }

        onEnableChanged: {
            if (!enable) {
                delete callback.s
                callback.alive = false
                callback.active = false
            }
        }

        function callback(key, active) {
            if (key === Qt.LeftButton && active) {
                if (!callback.s) {
                    callback.s = {
                        x: window.currentProcess.editor.input.mouseX,
                        y: window.currentProcess.editor.input.mouseY
                    }

                    callback.active = true

                    window.currentProcess.editor.callbacks.push(
                        function rect(ctx, px, py, z) {
                            if (!callback.active)
                                return

                            callback.e = {
                                x: window.currentProcess.editor.input.mouseX,
                                y: window.currentProcess.editor.input.mouseY
                            }

                            callback.size = {
                                w: Math.abs(callback.e.x - callback.s.x),
                                h: Math.abs(callback.e.y - callback.s.y)
                            }

                            ctx.strokeStyle = "#FF7D7D"

                            ctx.lineWidth = 3
                            ctx.beginPath()
                            ctx.strokeRect(
                                Math.min(callback.s.x, callback.e.x), Math.min(callback.s.y, callback.e.y),
                                callback.size.w, callback.size.h)
                            ctx.stroke()

                            return true
                        }
                    )
                } else {
                    callback.active = false

                    const sx = callback.s.x, sy = callback.s.y,
                          ex = callback.e.x, ey = callback.e.y

                    callback.s = {
                        x: Math.min(sx, ex),
                        y: Math.min(sy, ey)
                    }
                    callback.e = {
                        x: Math.max(sx, ex),
                        y: Math.max(sy, ey)
                    }

                    addText.parent = window.currentProcess.editor
                    addText.x = callback.s.x
                    addText.y = Math.max(callback.s.y, callback.e.y)
                    addText.open()

                    callback.alive = true

                    window.currentProcess.editor.callbacks.push(
                        function distance(ctx, px, py, z) {
                            if (!callback.alive)
                                return

                            ctx.strokeStyle = "#FF7D7D"
                            ctx.fillStyle   = "red"
                            ctx.font        = `${addText.fontItalic ? 'italic' : ''} ${addText.fontBold ? 'bold' : ''} ${addText.fontSize}px Consolas`.trim()
                            ctx.strokeRect(callback.s.x, callback.s.y, callback.size.w, callback.size.h)
                            ctx.stroke()

                            const text = addText.value.text
                            const metrics = ctx.measureText(text)
                            callback.columns = Math.floor(callback.size.w / (metrics.width / text.length))
                            for (let i = 0, row = 1; i < text.length; i += callback.columns, ++row) {
                                ctx.fillText(addText.value.text.substr(i, callback.columns),
                                    callback.s.x,
                                    callback.s.y + addText.fontSize * row
                                )
                            }
                            ctx.stroke()

                            return true
                        }
                    )
                }
            }

            return toolAddText.enable
        }
    }
}
