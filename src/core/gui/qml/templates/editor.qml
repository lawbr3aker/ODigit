import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import scripts 1.0 as Scripts

Scripts.Editor {
    id: root

    property alias input: input
    property var callbacks: []

    property var status: {}

    Component.onCompleted: {
        zoom = 1
        status = {}
    }

    Rectangle {
        z: -1
        anchors.fill: parent

        color: 'white'
    }

    Canvas {
        anchors.fill: parent

        onPaint: {
            const ctx = getContext('2d')

            ctx.clearRect(0, 0, root.width, root.height)

            const deleteCallbacks = []
            for (const callback of root.callbacks) {
                const result = callback(ctx, root.pan.x, root.pan.y, root.zoom)

                if (result !== true)
                    deleteCallbacks.push(callback)
            }

            if (deleteCallbacks.length)
                root.callbacks = root.callbacks.filter(c => !deleteCallbacks.includes(c))
        }

        Timer {
            id: fps
            interval: 20
            running: root.process.active
            repeat: true

            onTriggered: {
                parent.requestPaint()
            }
        }
    }

    MouseArea {
        id: input

        anchors.fill: parent

        signal keyChanged(int key, int state)

        readonly property int cursorDefault: Qt.CrossCursor
        readonly property int cursorHover  : Qt.OpenHandCursor

        property int mouseCurrentX: -1
        property int mouseLastX   : -1
        property int mouseRealX   : -1
        property int mouseCurrentY: -1
        property int mouseLastY   : -1
        property int mouseRealY   : -1

        property var callbacks: []

        property var hoveredPoint  : null
        property var hoveredSegment: null

        property var activePoints  : []
        property var activeSegments: []

        property var keys: {}

            cursorShape: cursorDefault
           hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.MiddleButton | Qt.RightButton

        function checkPoint(point, lx, ly, rx, ry) {
            return lx <= point.x && point.x <= rx && ly <= point.y && point.y <= ry
        }

        onPositionChanged: {
            forceActiveFocus()

            mouseCurrentX = mouseX
            mouseRealX    = (mouseCurrentX - root.pan.x) / root.zoom
            mouseCurrentY = mouseY
            mouseRealY    = (mouseCurrentY - root.pan.y) / root.zoom

            function checkPoints() {
                let hovered
                const threshold = 8 / root.zoom
                for (const point of root.points) {
                    if (checkPoint(
                            point,
                            mouseRealX - threshold, mouseRealY - threshold,
                            mouseRealX + threshold, mouseRealY + threshold
                        )) {
                        hovered = point
                        break
                    }
                }

                if (hovered) {
                    if (hovered != hoveredPoint) {
                        if (hoveredPoint)
                            hoveredPoint.status = activePoints.includes(hoveredPoint) ? 'active' : 'default'
                        hoveredPoint = hovered
                        //
                        cursorShape = cursorHover
                        root.update()
                    }
                    hoveredPoint.status = 'hover'
                } else if (hoveredPoint) {
                    hoveredPoint.status = activePoints.includes(hoveredPoint) ? 'active' : 'default'
                    hoveredPoint = undefined
                    //
                    cursorShape = cursorDefault
                    root.update()
                }

                return hovered
            }

            function checkSegments() {
                let hovered = undefined
                const threshold = 5 / root.zoom
                for (const polyline of root.polylines) {
                    for (const segment of polyline.segments) {
                        const a = segment.e.x - segment.s.x
                        const b = segment.e.y - segment.s.y
                        const distance = Math.abs(((mouseRealX - segment.s.x) * b - (mouseRealY - segment.s.y) * a) / Math.sqrt(a * a + b * b))

                        if (distance < threshold && Math.min(segment.s.x, segment.e.x) <= mouseRealX && mouseRealX <= Math.max(segment.s.x, segment.e.x)) {
                            hovered = segment
                            break
                        }
                    }
                }

                if (hovered) {
                    if (hovered != hoveredSegment) {
                        if (hoveredSegment)
                            hoveredSegment.status = activeSegments.includes(hoveredSegment) ? 'active' : 'default'
                        hoveredSegment = hovered
                        //
                        cursorShape = cursorHover
                        root.update()
                    }
                    hoveredSegment.status = 'hover'
                } else if (hoveredSegment) {
                    hoveredSegment.status = activeSegments.includes(hoveredSegment) ? 'active' : 'default'
                    hoveredSegment = undefined
                    //
                    cursorShape = cursorDefault
                    root.update()
                }

                return hovered
            }

            if (checkPoints()) {
                root.status['hovered'] = true

                if (hoveredSegment) {
                    hoveredSegment.status = activeSegments.includes(hoveredSegment) ? 'active' : 'default'
                    hoveredSegment = undefined
                }
            } else if (checkSegments()) {
                root.status['hovered'] = true

                if (hoveredPoint) {
                    hoveredPoint.status = activePoints.includes(hoveredPoint) ? 'active' : 'default'
                    hoveredPoint = undefined
                }
            } else {
                root.status['hovered'] = false
            }


            if (mouseCurrentX != mouseLastX || mouseCurrentY != mouseLastY) {
                if (root.status['selected']) {
                    root.status['moving'] = true
                }

                if (root.status['moving']) {
                    let deltaX = (mouseCurrentX - mouseLastX) / root.zoom,
                        deltaY = (mouseCurrentY - mouseLastY) / root.zoom

                    let points = activePoints
                    for (const segment of activeSegments) {
                        if (!points.includes(segment.s))
                            points.push(segment.s)
                        if (!points.includes(segment.e))
                            points.push(segment.e)
                    }
                    for (const point of activePoints) {
                        point.x += deltaX
                        point.y += deltaY
                    }
                    root.update()
                }

                if (keys[Qt.MiddleButton]) {
                    let deltaX = mouseCurrentX - mouseLastX,
                        deltaY = mouseCurrentY - mouseLastY

                    root.pan.x += deltaX
                    root.pan.y += deltaY
                    root.update()
                }
            }

            mouseLastX = mouseCurrentX
            mouseLastY = mouseCurrentY
        }

        onPressed: {
            input.keys[mouse.button] = true

            keyChanged(mouse.button, true)
        }

        onReleased: {
            delete input.keys[mouse.button]

            keyChanged(mouse.button, false)
        }

        Keys.onPressed: {
            input.keys[event.key] = true

            keyChanged(event.key, true)
        }

        Keys.onReleased: {
            delete input.keys[event.key]

            keyChanged(event.key, false)
        }

        onKeyChanged: function (key, active) {
            if (active) {
                switch (key) {
                    case Qt.LeftButton: {
                        if (hoveredPoint) {
                            if (!activePoints.includes(hoveredPoint)) {
                                if (!keys[Qt.Key_Shift]) {
                                    for (const point of activePoints)
                                        point.status = 'default'
                                    activePoints = []
                                    for (const segment of activeSegments)
                                        segment.status = 'default'
                                    activeSegments = []
                                }
                                activePoints.push(hoveredPoint)
                                hoveredPoint.status = 'active'

                                onKeyChanged.selected = true
                            }
                        } else if (!keys[Qt.Key_Shift] && !root.status['hovered']) {
                            for (const point of activePoints)
                                point.status = 'default'
                            activePoints = []
                        }
                        if (hoveredSegment) {
                            if (!activeSegments.includes(hoveredSegment)) {
                                if (!keys[Qt.Key_Shift]) {
                                    for (const Segment of activeSegments)
                                        Segment.status = 'default'
                                    activeSegments = []
                                    for (const point of activePoints)
                                        point.status = 'default'
                                    activePoints = []
                                }
                                activeSegments.push(hoveredSegment)
                                hoveredSegment.status = 'active'

                                onKeyChanged.selected = true
                            }
                        } else if (!keys[Qt.Key_Shift] && !root.status['hovered']) {
                            for (const segment of activeSegments)
                                segment.status = 'default'
                            activeSegments = []
                        }
                        if (root.status['hovered'])
                            root.status['selected'] = true
                    } break
                }
            } else {
                switch (key) {
                    case Qt.Key_Delete: {
                        if (keys[Qt.Key_X]) {
                            for (const segment of input.activeSegments)
                                root.dissolve_segment(segment)
                            for (const point of input.activePoints)
                                root.dissolve_point(point)
                        } else {
                            for (const segment of input.activeSegments)
                                root.remove_segment(segment)
                            for (const point of input.activePoints)
                                root.remove_point(point)
                        }
                        root.update()
                    } break
                    case Qt.LeftButton: {
                        if (hoveredPoint && !onKeyChanged.selected && !root.status['moving']) {
                            if (activePoints.includes(hoveredPoint)) {
                                if (keys[Qt.Key_Shift]) {
                                    activePoints.splice(activePoints.indexOf(hoveredPoint), 1)
                                } else {
                                    for (const point of activePoints)
                                        point.status = 'default'
                                    activePoints = [hoveredPoint]
                                    for (const segment of activeSegments)
                                        segment.status = 'default'
                                    activeSegments = []
                                }
                            }
                        }
                        if (hoveredSegment && !onKeyChanged.selected && !root.status['moving']) {
                            if (activeSegments.includes(hoveredSegment)) {
                                if (keys[Qt.Key_Shift]) {
                                    activeSegments.splice(activeSegments.indexOf(hoveredSegment), 1)
                                } else {
                                    for (const segment of activeSegments)
                                        segment.status = 'default'
                                    activeSegments = [hoveredSegment]
                                    for (const point of activePoints)
                                        point.status = 'default'
                                    activePoints = []
                                }
                            } else {
                            }
                        }

                        root.status['moving'] = false
                        root.status['selected'] = false

                        onKeyChanged.selected = false
                    }
                }
            }

            const deleteCallbacks = []
            for (const callback of input.callbacks) {
                const result = callback(key, active)

                if (result !== true)
                    deleteCallbacks.push(callback)
            }

            if (deleteCallbacks.length)
                input.callbacks = input.callbacks.filter(callback => !deleteCallbacks.includes(callback))
        }

        WheelHandler {
            acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad
            orientation: Qt.Vertical
            property: "y"

            onWheel: (wheel) => {
                let mx0 = (input.mouseX - root.pan.x) / root.zoom
                let my0 = (input.mouseY - root.pan.y) / root.zoom

                const new_zoom = root.zoom * (1 + (wheel.angleDelta.y / 120 / 10))
                if (new_zoom > 2.2 || new_zoom < 0.03)
                    return

                root.zoom = new_zoom

                mx0 = root.pan.x + (mx0 * root.zoom)
                my0 = root.pan.y + (my0 * root.zoom)

                root.pan.x += input.mouseX - mx0
                root.pan.y += input.mouseY - my0
                //
                root.update()
            }
        }

        Component.onCompleted: {
            keys = {}
        }
    }
}
