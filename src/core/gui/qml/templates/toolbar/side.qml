import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3

import "qrc:Components/Controls" as Components_Controls
import "qrc:Components/Dialogs/Helpers" as Components_Dialogs_Helpers

import scripts 1.0

ColumnLayout {
    property var globalW

    spacing: 5


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
        hint.text: globalW.globalTranslator.tr('29aj')

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
                if (!globalW.currentProcess.editor.input.callbacks.includes(callback))
                    globalW.currentProcess.editor.input.callbacks.push(callback)
            }
        }

        function callback(key, active) {
            if (!toolSelect.enable)
                return

            if (key === Qt.LeftButton && !globalW.currentProcess.editor.status['selected']) {
                if (active) {
                    if (!callback.s) {
                        callback.s = {
                            x: globalW.currentProcess.editor.input.mouseRealX,
                            y: globalW.currentProcess.editor.input.mouseRealY
                        }

                        callback.active = true

                        globalW.currentProcess.editor.callbacks.push(
                            function rect(ctx, px, py, z) {
                                if (!callback.active)
                                    return

                                callback.e = {
                                    x: globalW.currentProcess.editor.input.mouseRealX,
                                    y: globalW.currentProcess.editor.input.mouseRealY
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
                                    Math.min(callback.s.x, callback.e.x) * globalW.currentProcess.editor.zoom + globalW.currentProcess.editor.pan.x,
                                    Math.min(callback.s.y, callback.e.y) * globalW.currentProcess.editor.zoom + globalW.currentProcess.editor.pan.y,
                                    callback.size.w * globalW.currentProcess.editor.zoom,
                                    callback.size.h * globalW.currentProcess.editor.zoom
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

                        for (const point of globalW.currentProcess.editor.points) {
                            if (globalW.currentProcess.editor.input.checkPoint(
                                    point,
                                    Math.min(callback.s.x, callback.e.x), Math.min(callback.s.y, callback.e.y),
                                    Math.max(callback.s.x, callback.e.x), Math.max(callback.s.y, callback.e.y),
                                )) {
                                if (!globalW.currentProcess.editor.input.activePoints.includes(point)) {
                                    point.status = 'active'
                                    globalW.currentProcess.editor.input.activePoints.push(point)
                                }
                            }
                        }

                        for (const polyline of globalW.currentProcess.editor.polylines) {
                            for (const segment of polyline.segments) {
                                const midpoint = {
                                    x: (segment.s.x + segment.e.x) / 2,
                                    y: (segment.s.y + segment.e.y) / 2
                                }

                                if (globalW.currentProcess.editor.input.checkPoint(
                                        midpoint,
                                        Math.min(callback.s.x, callback.e.x), Math.min(callback.s.y, callback.e.y),
                                        Math.max(callback.s.x, callback.e.x), Math.max(callback.s.y, callback.e.y),
                                    )) {
                                    if (!globalW.currentProcess.editor.input.activeSegments.includes(segment)) {
                                        segment.status = 'active'
                                        globalW.currentProcess.editor.input.activeSegments.push(segment)
                                    }
                                }
                            }
                        }

                        globalW.currentProcess.editor.update()
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
        hint.text: `${globalW.globalTranslator.tr('4jdn')}`

        onClicked: {
            resetAll()
            globalW.currentProcess.editor.home()
            globalW.currentProcess.editor.update()
        }
    }

    Components_Controls.Button {
        Layout.fillWidth: true
        Layout.preferredHeight: width

        icn.source: 'qrc:Assets/Images/Icons/zoom-in'
        icn.parent.height: width
        icn.parent.anchors.margins: 6
        hint.text: `${globalW.globalTranslator.tr('Oi20')}`

        onClicked: {
            globalW.currentProcess.editor.zoom += 0.03
            if (globalW.currentProcess.editor.zoom > 2.2)
                globalW.currentProcess.editor.zoom = 2.2
            globalW.currentProcess.editor.update()
        }
    }

    Components_Controls.Button {
        Layout.fillWidth: true
        Layout.preferredHeight: width

        icn.source: 'qrc:Assets/Images/Icons/zoom-out'
        icn.parent.height: width
        icn.parent.anchors.margins: 6
        hint.text: `${globalW.globalTranslator.tr('Oi21')}`

        onClicked: {
            globalW.currentProcess.editor.zoom -= 0.03
            if (globalW.currentProcess.editor.zoom < 0.03)
                globalW.currentProcess.editor.zoom = 0.03
            globalW.currentProcess.editor.update()
        }
    }

    Components_Controls.Button {
        id: toolPoint
        Layout.fillWidth: true
        Layout.preferredHeight: width

        icn.source: 'qrc:Assets/Images/Icons/point'
        icn.parent.height: width
        icn.parent.anchors.margins: 6
        hint.text: `${globalW.globalTranslator.tr('Oi22')}`

        property bool enable: false

        onClicked: {
            if (!enable) {
                resetAll(true)
                enable = true

                if (!globalW.currentProcess.editor.input.callbacks.includes(callback))
                    globalW.currentProcess.editor.input.callbacks.push(callback)
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

            if (key === Qt.LeftButton && !active && !globalW.currentProcess.editor.status['selected'] && !toolSelect.selecting) {
                const x = (globalW.currentProcess.editor.input.mouseX - globalW.currentProcess.editor.pan.x) / globalW.currentProcess.editor.zoom
                const y = (globalW.currentProcess.editor.input.mouseY - globalW.currentProcess.editor.pan.y) / globalW.currentProcess.editor.zoom

                globalW.currentProcess.editor.add_point(x, y)
                globalW.currentProcess.editor.update()
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
        hint.text: `${globalW.globalTranslator.tr('Oi23')}`

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
                if (!globalW.currentProcess.editor.input.callbacks.includes(callback))
                    globalW.currentProcess.editor.input.callbacks.push(callback)

                globalW.currentProcess.editor.callbacks.push(
                    function ruler(ctx, px, py, z) {
                        if (!toolLine.enable)
                            return

                        if (!callback.s)
                            return true

                        const x = (globalW.currentProcess.editor.input.mouseX - px) / z
                        const y = (globalW.currentProcess.editor.input.mouseY - py) / z

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
                let hovered = globalW.currentProcess.editor.input.hoveredPoint
                if (!hovered) {
                    const x = (globalW.currentProcess.editor.input.mouseX - globalW.currentProcess.editor.pan.x) / globalW.currentProcess.editor.zoom
                    const y = (globalW.currentProcess.editor.input.mouseY - globalW.currentProcess.editor.pan.y) / globalW.currentProcess.editor.zoom

                    hovered = globalW.currentProcess.editor.add_point(x, y)
                    globalW.currentProcess.editor.update()
                }

                if (callback.s) {
                    if (hovered !== callback.s) {
                        globalW.currentProcess.editor.add_segment(callback.s, hovered)
                        globalW.currentProcess.editor.update()
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
        hint.text: `${globalW.globalTranslator.tr('Oi24')}`

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
                if (!globalW.currentProcess.editor.input.callbacks.includes(callback))
                    globalW.currentProcess.editor.input.callbacks.push(callback)
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
                        x: (globalW.currentProcess.editor.input.mouseX - globalW.currentProcess.editor.pan.x) / globalW.currentProcess.editor.zoom,
                        y: (globalW.currentProcess.editor.input.mouseY - globalW.currentProcess.editor.pan.y) / globalW.currentProcess.editor.zoom
                    }

                    globalW.currentProcess.editor.callbacks.push(
                        function ruler(ctx, px, py, z) {
                            if (!callback.active)
                                return

                            callback.e = {
                                x: (globalW.currentProcess.editor.input.mouseX - px) / z,
                                y: (globalW.currentProcess.editor.input.mouseY - py) / z
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

                    globalW.currentProcess.editor.callbacks.push(
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
                                distance.text    = `${(distance.value / globalW.currentProcess.config.value('cm_pixels', 'int')).toFixed(2)}CM`
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
                    globalW.currentProcess.editor.add_text(
                        (toolAddText.callback.s.x - globalW.currentProcess.editor.pan.x) / globalW.currentProcess.editor.zoom,
                        (toolAddText.callback.s.y - globalW.currentProcess.editor.pan.y + addText.fontSize * row) / globalW.currentProcess.editor.zoom,
                        toolAddText.callback.size.w / globalW.currentProcess.editor.zoom,
                        toolAddText.callback.size.h / globalW.currentProcess.editor.zoom,
                        addText.value.text.substr(i, toolAddText.callback.columns),
                        "Consolas",
                        addText.fontSize / globalW.currentProcess.editor.zoom,
                        addText.fontBold,
                        addText.fontItalic
                    )
                }
                globalW.currentProcess.editor.update()
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
        hint.text: `${globalW.globalTranslator.tr('Oi25')}`

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
                if (!globalW.currentProcess.editor.input.callbacks.includes(callback))
                    globalW.currentProcess.editor.input.callbacks.push(callback)
            }
        }

        function callback(key, active) {
            if (key === Qt.LeftButton && active) {
                if (!callback.s) {
                    callback.s = {
                        x: globalW.currentProcess.editor.input.mouseX,
                        y: globalW.currentProcess.editor.input.mouseY
                    }

                    callback.active = true

                    globalW.currentProcess.editor.callbacks.push(
                        function rect(ctx, px, py, z) {
                            if (!callback.active)
                                return

                            callback.e = {
                                x: globalW.currentProcess.editor.input.mouseX,
                                y: globalW.currentProcess.editor.input.mouseY
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

                    addText.parent = globalW.currentProcess.editor
                    addText.x = callback.s.x
                    addText.y = Math.max(callback.s.y, callback.e.y)
                    addText.open()

                    callback.alive = true

                    globalW.currentProcess.editor.callbacks.push(
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
