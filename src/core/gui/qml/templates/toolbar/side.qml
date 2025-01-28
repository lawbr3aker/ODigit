import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3

import "qrc:Components/Controls" as Components_Controls
import "qrc:Components/Dialogs/Helpers" as Components_Dialogs_Helpers

import scripts 1.0

ColumnLayout {
    property var window

    spacing: 5

    Translator {
        id: translator
    }

    function resetAll() {
        toolMeasure.enable = false
        toolPoint  .enable = false
        toolLine   .enable = false
        toolAddText.enable = false
        //
        toolSelect .enable = true
    }

    Components_Controls.Button {
        id: toolSelect
        Layout.fillWidth: true
        Layout.preferredHeight: width

        icn.source: 'qrc:Assets/Images/Icons/cursor'
        icn.parent.height: width
        icn.parent.anchors.margins: 6
        hint.text: translator.global.tr('29aj')

        property bool enable: false
        property bool selecting: false

        Component.onCompleted: {
            enable = true
        }

        onClicked: {
            resetAll()
            enable = true
        }

        onEnableChanged: {
            if (!enable) {
                delete callback.s
                callback.active = false
            } else {
                if (!window.currentProcess.editor.input.callbacks.includes(callback))
                    window.currentProcess.editor.input.callbacks.push(callback)
            }
        }

        function callback(key, active) {
            if (!toolSelect.enable)
                return

            if (key === Qt.LeftButton && !window.currentProcess.editor.status['selected']) {
                if (active) {
                    if (!callback.s) {
                        callback.s = {
                            x: window.currentProcess.editor.input.mouseRealX,
                            y: window.currentProcess.editor.input.mouseRealY
                        }

                        callback.active = true

                        window.currentProcess.editor.callbacks.push(
                            function rect(ctx, px, py, z) {
                                if (!callback.active)
                                    return

                                callback.e = {
                                    x: window.currentProcess.editor.input.mouseRealX,
                                    y: window.currentProcess.editor.input.mouseRealY
                                }

                                callback.size = {
                                    w: Math.abs(callback.e.x - callback.s.x),
                                    h: Math.abs(callback.e.y - callback.s.y)
                                }

                                toolSelect.selecting = callback.size.w > 0 && callback.size.h > 0

                                ctx.strokeStyle = "#FF0000"

                                ctx.lineWidth = 2
                                ctx.beginPath()
                                ctx.strokeRect(
                                    Math.min(callback.s.x, callback.e.x) * window.currentProcess.editor.zoom + window.currentProcess.editor.pan.x,
                                    Math.min(callback.s.y, callback.e.y) * window.currentProcess.editor.zoom + window.currentProcess.editor.pan.y,
                                    callback.size.w * window.currentProcess.editor.zoom,
                                    callback.size.h * window.currentProcess.editor.zoom
                                )
                                ctx.stroke()

                                return true
                            }
                        )
                    }
                } else {
                    //toolSelect.selecting = false

                    if (callback.active) {
                        callback.active = false

                        for (const point of window.currentProcess.editor.points) {
                            if (window.currentProcess.editor.input.checkPoint(
                                    point,
                                    Math.min(callback.s.x, callback.e.x), Math.min(callback.s.y, callback.e.y),
                                    Math.max(callback.s.x, callback.e.x), Math.max(callback.s.y, callback.e.y),
                                )) {
                                if (!window.currentProcess.editor.input.activePoints.includes(point)) {
                                    point.status = 'active'
                                    window.currentProcess.editor.input.activePoints.push(point)
                                }
                            }
                        }

                        for (const polyline of window.currentProcess.editor.polylines) {
                            for (const segment of polyline.segments) {
                                const midpoint = {
                                    x: (segment.s.x + segment.e.x) / 2,
                                    y: (segment.s.y + segment.e.y) / 2
                                }

                                if (window.currentProcess.editor.input.checkPoint(
                                        midpoint,
                                        Math.min(callback.s.x, callback.e.x), Math.min(callback.s.y, callback.e.y),
                                        Math.max(callback.s.x, callback.e.x), Math.max(callback.s.y, callback.e.y),
                                    )) {
                                    if (!window.currentProcess.editor.input.activeSegments.includes(segment)) {
                                        segment.status = 'active'
                                        window.currentProcess.editor.input.activeSegments.push(segment)
                                    }
                                }
                            }
                        }

                        window.currentProcess.editor.update()
                    }

                    delete callback.s
                    callback.active = false
                }
            }

            return true
        }
    }

    Components_Controls.Button {
        Layout.fillWidth: true
        Layout.preferredHeight: width

        icn.source: 'qrc:Assets/Images/Icons/home'
        icn.parent.height: width
        icn.parent.anchors.margins: 6
        hint.text: translator.global.tr('4jdn')

        onClicked: {
            resetAll()
            window.currentProcess.editor.home()
            window.currentProcess.editor.update()
        }
    }

    Components_Controls.Button {
        Layout.fillWidth: true
        Layout.preferredHeight: width

        icn.source: 'qrc:Assets/Images/Icons/zoom-in'
        icn.parent.height: width
        icn.parent.anchors.margins: 6
        hint.text: translator.global.tr('Oi20')

        onClicked: {
            window.currentProcess.editor.zoom += 0.03
            if (window.currentProcess.editor.zoom > 2.2)
                window.currentProcess.editor.zoom = 2.2
            window.currentProcess.editor.update()
        }
    }

    Components_Controls.Button {
        Layout.fillWidth: true
        Layout.preferredHeight: width

        icn.source: 'qrc:Assets/Images/Icons/zoom-out'
        icn.parent.height: width
        icn.parent.anchors.margins: 6
        hint.text: translator.global.tr('Oi21')

        onClicked: {
            window.currentProcess.editor.zoom -= 0.03
            if (window.currentProcess.editor.zoom < 0.03)
                window.currentProcess.editor.zoom = 0.03
            window.currentProcess.editor.update()
        }
    }

    Components_Controls.Button {
        id: toolPoint
        Layout.fillWidth: true
        Layout.preferredHeight: width

        icn.source: 'qrc:Assets/Images/Icons/point'
        icn.parent.height: width
        icn.parent.anchors.margins: 6
        hint.text: translator.global.tr('Oi22')

        property bool enable: false

        onClicked: {
            if (!enable) {
                resetAll(true)
                enable = true

                if (!window.currentProcess.editor.input.callbacks.includes(callback))
                    window.currentProcess.editor.input.callbacks.push(callback)
            }
        }

        onEnableChanged: {
            if (enabled) {
            } else {
            }
        }

        function callback(key, active) {
            if (!toolPoint.enable)
                return

            if (key === Qt.LeftButton && !active && !window.currentProcess.editor.status['selected'] && !toolSelect.selecting) {
                const x = (window.currentProcess.editor.input.mouseX - window.currentProcess.editor.pan.x) / window.currentProcess.editor.zoom
                const y = (window.currentProcess.editor.input.mouseY - window.currentProcess.editor.pan.y) / window.currentProcess.editor.zoom

                window.currentProcess.editor.add_point(x, y)
                window.currentProcess.editor.update()
            }

            return true
        }
    }

    Components_Controls.Button {
        id: toolLine

        Layout.      fillWidth: true
        Layout.preferredHeight: width

        icn.source: 'qrc:Assets/Images/Icons/line'
        icn.parent.height: width
        icn.parent.anchors.margins: 6
        hint.text: translator.global.tr('Oi23')

        property bool enable: false

        onClicked: {
            if (!enable) {
                resetAll()
                toolSelect.enable = false
                enable = true
            }
        }

        onEnableChanged: {
            if (!enable) {
                delete callback.s
            } else {
                if (!window.currentProcess.editor.input.callbacks.includes(callback))
                    window.currentProcess.editor.input.callbacks.push(callback)

                window.currentProcess.editor.callbacks.push(
                    function ruler(ctx, px, py, z) {
                        if (!toolLine.enable)
                            return

                        if (!callback.s)
                            return true

                        const x = (window.currentProcess.editor.input.mouseX - px) / z
                        const y = (window.currentProcess.editor.input.mouseY - py) / z

                        ctx.lineWidth   = 3
                        ctx.strokeStyle = "#37AD76"
                        ctx.beginPath()
                        ctx.moveTo(callback.s.x * z + px, callback.s.y * z + py)
                        ctx.lineTo(x * z + px, y * z + py)
                        ctx.stroke()

                        return true
                    }
                )
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
                        window.currentProcess.editor.add_segment(callback.s, hovered)
                        window.currentProcess.editor.update()
                    }
                }
                callback.s = hovered
            }

            return true
        }
    }

    Components_Controls.Button {
        id: toolMeasure
        Layout.fillWidth: true
        Layout.preferredHeight: width

        icn.source: 'qrc:Assets/Images/Icons/measure'
        icn.parent.height: width
        icn.parent.anchors.margins: 6
        hint.text: translator.global.tr('Oi24')

        property bool enable: false

        onClicked: {
            resetAll()
            toolSelect.enable = false
            enable = true
        }

        onEnableChanged: {
            if (!enable) {
                delete callback.s
                callback.alive = false
                callback.active = false
            } else {
                if (!window.currentProcess.editor.input.callbacks.includes(callback))
                    window.currentProcess.editor.input.callbacks.push(callback)
            }
        }

        function callback(key, active) {
            if (!toolMeasure.enable)
                return

            if (key === Qt.LeftButton && active) {
                if (callback.alive) {
                    delete callback.s

                    callback.alive = false
                }

                if (!callback.s) {
                    callback.alive = false
                    callback.active = true

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

            return true
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
                    window.currentProcess.editor.add_text(
                        (toolAddText.callback.s.x - window.currentProcess.editor.pan.x) / window.currentProcess.editor.zoom,
                        (toolAddText.callback.s.y - window.currentProcess.editor.pan.y + addText.fontSize * row) / window.currentProcess.editor.zoom,
                        toolAddText.callback.size.w / window.currentProcess.editor.zoom,
                        toolAddText.callback.size.h / window.currentProcess.editor.zoom,
                        addText.value.text.substr(i, toolAddText.callback.columns),
                        "Consolas",
                        addText.fontSize / window.currentProcess.editor.zoom,
                        addText.fontBold,
                        addText.fontItalic
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

        icn.source: 'qrc:Assets/Images/Icons/text'
        icn.parent.height: width
        icn.parent.anchors.margins: 6
        hint.text: translator.global.tr('Oi25')

        property bool enable: false

        onClicked: {
            if (!enable) {
                resetAll()
                enable = true
            }
        }

        onEnableChanged: {
            if (!enable) {
                delete callback.s
                callback.alive = false
                callback.active = false
            } else {
                if (!window.currentProcess.editor.input.callbacks.includes(callback))
                    window.currentProcess.editor.input.callbacks.push(callback)
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
                    console.log("aded")

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
            console.log("returne", toolAddText.enable)

            return toolAddText.enable
        }
    }
}
