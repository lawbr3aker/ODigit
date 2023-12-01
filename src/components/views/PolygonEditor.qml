import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import scripts 1.0 as Scripts

Item {
    id: root

    property alias canvas: canvas
    property alias input: input

    Scripts.Editor {
        id: canvas
        anchors.fill: parent

        property var callbacks: []

        Component.onCompleted: {
            zoom = 1
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

                ctx.clearRect(0, 0, context.canvas.width, context.canvas.height)

                const deleteCallbacks = []
                for (const callback of canvas.callbacks) {
                    if (callback(ctx, canvas.pan.x, canvas.pan.y, canvas.zoom) !== true)
                        deleteCallbacks.push(callback)
                }

                if (deleteCallbacks.length)
                    canvas.callbacks = canvas.callbacks.filter(c => !deleteCallbacks.includes(c))
            }

            Timer {
                id: fps
                interval: 20
                repeat: true

                onTriggered: {
                    parent.requestPaint()
                }
            }

            Component.onCompleted: {
                fps.running = true
            }
        }
    }

    MouseArea {
        id: input

        anchors.fill: parent

        readonly property int cursorDefault: Qt.CrossCursor
        readonly property int cursorHover  : Qt.OpenHandCursor

        property int  mouseCurrentX: -1
        property int  mouseCurrentY: -1
        property int  mouseLastX: -1
        property int  mouseLastY: -1
        property bool mouseButtonLeft: false
        property bool mouseButtonMiddle: false

        property var callbacks: []

        property var hoveredPoint  : null
        property var hoveredSegment: null

        property var activePoints: []

        property var keys: {}

        cursorShape: cursorDefault
        hoverEnabled: true
        focus: true

        acceptedButtons: Qt.LeftButton | Qt.MiddleButton

        onPositionChanged: {
            forceActiveFocus()
            mouseCurrentX = mouseX
            mouseCurrentY = mouseY

            let hovered
            for (const point of canvas.points) {
                const x = point.x * canvas.zoom + canvas.pan.x
                const y = point.y * canvas.zoom + canvas.pan.y

                if (x - 8 <= mouseCurrentX && mouseCurrentX <= x + 8
                 && y - 8 <= mouseCurrentY && mouseCurrentY <= y + 8) {
                    hovered = point
                }
            }

            if ((!hovered || hovered === hoveredPoint) && hoveredPoint) {
                if (!keys[Qt.LeftButton]) {
                    if (hoveredPoint.status === 'hover')
                        hoveredPoint.status = 'default'
                    hoveredPoint = undefined
                    //
                    cursorShape = cursorDefault
                    canvas.update()
                }
            }
            if (hovered) {
                hoveredPoint = hovered
                if (hoveredPoint.status === 'default')
                    hoveredPoint.status = 'hover'
                //
                cursorShape = cursorHover
                canvas.update()
            }

            if (keys[Qt.MiddleButton]) {
                let deltaX = mouseCurrentX - mouseLastX,
                    deltaY = mouseCurrentY - mouseLastY

                canvas.pan.x += deltaX
                canvas.pan.y += deltaY
                canvas.update()
            } else
            if (keys[Qt.LeftButton] && activePoints.length > 0) {
                let deltaX = mouseCurrentX - mouseLastX,
                    deltaY = mouseCurrentY - mouseLastY

                for (const point of activePoints) {
                    point.x += deltaX / canvas.zoom
                    point.y += deltaY / canvas.zoom
                }
                canvas.update()
            }

            mouseLastX = mouseCurrentX
            mouseLastY = mouseCurrentY
        }

        onPressed: {
            keys[mouse.button] = true

            onKeysChanged(mouse.button, true)
        }

        onReleased: {
            delete keys[mouse.button]

            onKeysChanged(mouse.button, false)
        }

        Keys.onPressed: {
            keys[event.key] = true

            onKeysChanged(event.key, true)
        }

        Keys.onReleased: {
            delete keys[event.key]

            onKeysChanged(event.key, false)
        }

        function onKeysChanged(key, active) {
            const deleteCallbacks = []
            for (const callback of input.callbacks) {
                if (callback(key, active) !== true)
                    deleteCallbacks.push(callback)
            }

            if (deleteCallbacks.length)
                input.callbacks = input.callbacks.filter(callback => !deleteCallbacks.includes(callback))


            if (active) {
                switch (key) {
                    case Qt.Key_Delete: {
                        for (const point of input.activePoints) {
                            for (const line of canvas.lines)
                                if (line.s === point || line.e === point)
                                    canvas.removeLine(line)

                            canvas.removePoint(point)
                        }
                        canvas.update()
                    } break
                    case Qt.LeftButton: {
                        if (hoveredPoint) {
                            if (hoveredPoint.status !== 'active') {
                                hoveredPoint.status = 'active'

                                if (keys[Qt.Key_Shift]) {
                                    activePoints.push(hoveredPoint)
                                } else {
                                    for (const point of activePoints)
                                        point.status = 'default'

                                    activePoints = [hoveredPoint]
                                }
                            } else {
                                if (keys[Qt.Key_Shift]) {
                                    hoveredPoint.status = 'default'

                                    activePoints.splice(activePoints.indexOf(hoveredPoint), 1)
                                }
                            }
                        }
                    } break
                }
                switch (key) {
                    case Qt.LeftButton:
                    case Qt.MiddleButton:
                    case Qt.RightButton: {
                        if (!hoveredPoint && activePoints.length) {
                            for (const point of activePoints)
                                point.status = 'default'

                            canvas.update()

                            activePoints = []
                        }
                    }
                }
            }
        }

        Timer {
            id: checkCallbacks
            repeat: true
            interval: 100
            running: true
            onTriggered: {
                const deleteCallbacks = []
                for (const callback of input.callbacks) {
                    if (callback() !== true)
                        deleteCallbacks.push(callback)
                }

                if (deleteCallbacks.length)
                    input.callbacks = input.callbacks.filter(callback => !deleteCallbacks.includes(callback))
            }
        }

        WheelHandler {
            acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad
            orientation: Qt.Vertical
            property: "y"

            onWheel: (wheel) => {
                let mx0 = (input.mouseX - canvas.pan.x) / canvas.zoom
                let my0 = (input.mouseY - canvas.pan.y) / canvas.zoom

                canvas.zoom *= 1 + (wheel.angleDelta.y / 120 / 10)

                mx0 = canvas.pan.x + (mx0 * canvas.zoom)
                my0 = canvas.pan.y + (my0 * canvas.zoom)

                canvas.pan.x += input.mouseX - mx0
                canvas.pan.y += input.mouseY - my0
                //
                canvas.update()
            }
        }

        Component.onCompleted: {
            keys = {}
        }
    }
}
