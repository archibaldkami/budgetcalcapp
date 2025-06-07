import QtQuick 2.15

Item {
    id: root
    width: 200
    height: 200

    property real income: 900
    property real outcome: 100
    property real balance: income - outcome

    Canvas {
        id: canvas
        anchors.fill: parent
        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)

            var total = income + outcome
            if (total === 0) return

            var startAngle = 0
            var incomeAngle = 2 * Math.PI * income / total
            var outcomeAngle = 2 * Math.PI * outcome / total

            // Draw income (green)
            ctx.beginPath()
            ctx.moveTo(width/2, height/2)
            ctx.arc(width/2, height/2, width/2, startAngle, startAngle + incomeAngle)
            ctx.closePath()
            ctx.fillStyle = "green"
            ctx.fill()

            // Draw outcome (red)
            ctx.beginPath()
            ctx.moveTo(width/2, height/2)
            ctx.arc(width/2, height/2, width/2, startAngle + incomeAngle, startAngle + incomeAngle + outcomeAngle)
            ctx.closePath()
            ctx.fillStyle = "red"
            ctx.fill()

            // Optional: white circle in center for donut effect
            ctx.beginPath()
            ctx.arc(width/2, height/2, width/4, 0, 2 * Math.PI)
            ctx.fillStyle = "white"
            ctx.fill()
        }

        Component.onCompleted: requestPaint()
    }

    // Text in center
    Text {
        anchors.centerIn: parent
        text: "₴" + root.balance
        font.pixelSize: 20
        color: "black"
    }
}
