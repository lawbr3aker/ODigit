import QtQuick 2.15
import QtQuick.Layouts 1.15

Canvas {
    id: root

    required property int orientation
    property int unitPixels: 56
    property int offset: 0

    readonly property alias background: background

    antialiasing: true

    Rectangle {
        id: background

        z: -1
        anchors.fill: parent

        color: '#FCFCFC'
    }

    onPaint: {
        const ctx = getContext('2d')

        const step = unitPixels / 10

        ctx.clearRect(0, 0, width, height)
        ctx.beginPath()
        ctx.fillStyle = 'black'
        if (orientation === Qt.Horizontal) {
            ctx.font = `bold ${height / 2.3}px sans-serif`
        } else if (orientation === Qt.Vertical) {
            ctx.font = `bold ${width / 2.3}px sans-serif`
        }


        let s = Math.trunc(-offset / unitPixels) - 1
        let o = 0
        let c = true

        o += offset % unitPixels - unitPixels
        do {
            for (let i = 0; i < 10; ++i, o += step) {
                let x, y
                if (orientation === Qt.Horizontal) {
                    x = o
                    if (o >= width) {
                        c = false
                        break
                    }
                    ctx.moveTo(x, height)

                    if (i === 0) {
                        ctx.lineTo(x, 0)
                        ctx.fillText(s++, x + 3, height / 2.5);
                    }
                    else if (i === 3 || i === 7)
                        ctx.lineTo(x, height * 0.75)
                    else if (i === 5)
                        ctx.lineTo(x, height * 0.5)
                    else
                        ctx.lineTo(x, height * 0.85)
                } else if (orientation === Qt.Vertical) {
                    y = o
                    if (o >= height) {
                        c = false
                        break
                    }
                    ctx.moveTo(width, y)

                    if (i === 0) {
                        ctx.lineTo(0, y)
                        ctx.rotate(-Math.PI / 2);
                        ctx.fillText(s++, -y + 3, width / 2.5);
                        ctx.rotate(Math.PI / 2);
                    }
                    else if (i === 3 || i === 7)
                        ctx.lineTo(width * 0.75, y)
                    else if (i === 5)
                        ctx.lineTo(width * 0.5, y)
                    else
                        ctx.lineTo(width * 0.85, y)
                }
            }
        } while (c)

        ctx.lineWidth = 1
        ctx.strokeStyle = 'black'
        ctx.stroke()
    }

    Timer {
        id: fps
        interval: 200
        running: false
        repeat: true
        onTriggered: {
            root.requestPaint()
        }
    }

    Component.onCompleted: {
        // fps.running = true
    }
}
