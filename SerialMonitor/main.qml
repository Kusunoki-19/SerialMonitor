import QtQuick 2.12
import QtQuick.Controls 2.0
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12

import SerialComponents 1.0

Window {
    width: 640
    height: 480
    visible: true
    Connections {
        target: SerialPortWrapper
        function onRecordsChanged() {
            graphCanvas.requestPaint()
        }
    }
    Canvas {
        anchors.fill: parent
        onWidthChanged: requestPaint()
        onHeightChanged: requestPaint()
        Component.onCompleted: requestPaint()
        onPaint: {
            var ctx = getContext('2d')
            ctx.clearRect(0,0,width,height)
            ctx.strokeStyle = "black"
            ctx.beginPath()
            ctx.moveTo(0,height/2)
            ctx.lineTo(width,height/2)
            ctx.stroke()
        }

    }
    Canvas {
        id:graphCanvas
        anchors.fill: parent
        onWidthChanged: requestPaint()
        onHeightChanged: requestPaint()
        Component.onCompleted: requestPaint()
        onPaint: {
            let plotData = SerialPortWrapper.records
            var ctx = getContext('2d')
            ctx.clearRect(0,0,width,height)

            ctx.strokeStyle = "red"
            ctx.beginPath()
            let i = 0
            for(i = 0; i < plotData.length; i++) {
                //console.log("i:",i,", gz:",plotData[i].gz)
                const x = i*(width/plotData.length)
                const y = plotData[i].gx * -1 + height/2
                ctx.lineTo(x,y)
            }
            ctx.stroke()
            ctx.closePath()

            ctx.strokeStyle = "green"
            ctx.beginPath()
            for(i = 0; i < plotData.length; i++) {
                //console.log("i:",i,", gz:",plotData[i].gz)
                const x = i*(width/plotData.length)
                const y = plotData[i].gy * -1 + height/2
                ctx.lineTo(x,y)
            }
            ctx.stroke()
            ctx.closePath()

            ctx.strokeStyle = "blue"
            ctx.beginPath()
            for(i = 0; i < plotData.length; i++) {
                //console.log("i:",i,", gz:",plotData[i].gz)
                const x = i*(width/plotData.length)
                const y = plotData[i].gz * -1 + height/2
                ctx.lineTo(x,y)
            }
            ctx.stroke()
            ctx.closePath()
        }

    }
}
