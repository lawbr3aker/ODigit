import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import scripts 1.0 as Scripts

Item {
    id: root

    property var points  : []
    property var segments: []

    property var selectedPoints  : []
    property var selectedSegments: []

    property alias canvas: canvas

    function addPoint(x, y) {
        const point = Qt.point(x, y)
        points.push(point)

        return point
    }

    function addSegment(s, e) {
        const segment = { s: s, e: e }
        segments.push(segment)

        return segment
    }

    Scripts.Editor {
        id: canvas
        z: 0
//
        anchors.fill: parent
//
//        readonly property int lineThickness: 4
//        readonly property int pointRadius  : 5
//
//        property double zoom: 1
        property point  pan : Qt.point(0, 0)

        Rectangle {
            z: -1
            anchors.fill: parent

            color: 'white'
        }

//        function drawGrid(ctx) {
//        }
//
//        function drawPoints(ctx, points, color) {
//            ctx.beginPath()
//            for (const point of points) {
//                ctx.ellipse(point.x * zoom - pointRadius + pan.x, point.y * zoom - pointRadius + pan.y, pointRadius * 2, pointRadius * 2)
//            }
//            ctx.fillStyle = color
//            ctx.fill()
//        }
//
//        function drawSegments(ctx, segments, color) {
//            ctx.beginPath()
//            for (const segment of segments) {
//                if (!segment.s) {
//                    continue
//                }
//
//                ctx.moveTo(segment.s.x * zoom + pan.x, segment.s.y * zoom + pan.y);
//                ctx.lineTo(segment.e.x * zoom + pan.x, segment.e.y * zoom + pan.y);
//            }
//            ctx.lineWidth = lineThickness
//            ctx.strokeStyle = color
//            ctx.stroke()
//        }
//
//        onPaint: {
//            const ctx = getContext('2d')
//            ctx.reset()
//            //
//            drawSegments(ctx, segments, '#6060FF')
//
//
//            //
//            drawPoints(ctx, points        , '#0006FF')
//            drawPoints(ctx, selectedPoints, '#FF0300')
//            if (input.hoveredPoint)
//                drawPoints(ctx, [input.hoveredPoint], '#880106')
//        }

        Timer {
            id: canvasTimer

            repeat: true
            interval: 1000 / 100

            onTriggered: {
                canvas.width = canvas.width - 0.0001;
            }
        }

        Component.onCompleted: {
            canvasTimer.running = true
        }
    }

//    MouseArea {
//        id: input
//
//        anchors.fill: parent
//
//        readonly property int cursorDefault: Qt.CrossCursor
//        readonly property int cursorHover  : Qt.OpenHandCursor
//
//        property int mouseCurrentX: -1
//        property int mouseCurrentY: -1
//        property int mouseLastX: -1
//        property int mouseLastY: -1
//
//        property var hoveredPoint  : null
//        property var hoveredSegment: null
//
//        property var keys: []
//
//        cursorShape: cursorDefault
//        hoverEnabled: true
//
//        acceptedButtons: Qt.LeftButton | Qt.MiddleButton
//
//        onPositionChanged: {
//            if (!focus) {
//                forceActiveFocus()
//                focus = true
//            }
//            mouseCurrentX = mouseX / canvas.zoom + canvas.pan.x
//            mouseCurrentY = mouseY / canvas.zoom + canvas.pan.y
//
//            if (input.isPressed(Qt.LeftButton)) {
//                let deltaX = mouseCurrentX - mouseLastX,
//                    deltaY = mouseCurrentY - mouseLastY
//
//                for (const point of root.selectedPoints) {
//                    point.x += deltaX
//                    point.y += deltaY
//                }
//            } else {
//                hoveredPoint = undefined
//                for (const point of root.points) {
//                    if ((point.x - canvas.pointRadius -4 <= mouseCurrentX && mouseCurrentX <= point.x + canvas.pointRadius +4)
//                     && (point.y - canvas.pointRadius -4 <= mouseCurrentY && mouseCurrentY <= point.y + canvas.pointRadius +4)) {
//                        hoveredPoint = point
//                        break
//                    }
//                }
//
//                if (hoveredPoint) {
//                    cursorShape = cursorHover
//                    //
//                    return
//                }
//
//                let hoveredSegment
//
//                cursorShape = cursorDefault
//            }
//
//            if (input.isPressed(Qt.MiddleButton) && input.isPressed(Qt.Key_Shift)) {
//                let deltaX = mouseCurrentX - mouseLastX,
//                    deltaY = mouseCurrentY - mouseLastY
//
//                canvas.pan.x += deltaX * canvas.scale / 2
//                canvas.pan.y += deltaY * canvas.scale / 2
//            }
//
//            mouseLastX = mouseCurrentX
//            mouseLastY = mouseCurrentY
//        }
//
//        onPressed: {
//            input.addKey(mouse.button)
//
//            if (input.isPressed(Qt.LeftButton)) {
//                root.selectedPoints = []
//                if (hoveredPoint) {
//                    root.selectedPoints.push(hoveredPoint)
//                } else {
//                    root.addSegment(root.points[root.points.length - 1], root.addPoint(mouseCurrentX, mouseCurrentY))
//                }
//            }
//        }
//        onReleased: {
//            input.removeKey(mouse.button)
//        }
//
//        WheelHandler {
//            acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad
//            orientation: Qt.Vertical
//            property: "y"
//
//            onWheel: (wheel) => {
//                if (input.isPressed(Qt.Key_Shift)) {
//                    const tX = input.mouseX * canvas.zoom
//                    const tY = input.mouseY * canvas.zoom
//                    canvas.zoom *= 1 + (wheel.angleDelta.y / 120 / 10)
//                    canvas.pan.x += (tX - input.mouseX * canvas.zoom)
//                    canvas.pan.y += (tY - input.mouseY * canvas.zoom)
//                }
//            }
//        }
//
//        onFocusChanged: if (!focus) forceActiveFocus()
//        Component.onCompleted: {
//            forceActiveFocus()
//        }
//
//        function addKey(key) {
//            keys.push(key)
//        }
//
//        function removeKey(key) {
//            keys.splice(keys.indexOf(key), 1)
//        }
//
//        function isPressed(key) {
//            return keys.indexOf(key) !== -1
//        }
//
//        Keys.onPressed: (event) => {
//            input.addKey(event.key)
//        }
//        Keys.onReleased: (event) => {
//            input.removeKey(event.key)
//        }
//    }
}
