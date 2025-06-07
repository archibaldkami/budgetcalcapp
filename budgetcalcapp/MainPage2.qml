import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: _item
    width: parent.width
    height: parent.height

    ListModel {
        id: operationModel
        ListElement { name: "Продукти"; amount: "-200 грн"; type: "out" }
        ListElement { name: "Зарплата"; amount: "+1500 грн"; type: "in" }
        ListElement { name: "Одяг"; amount: "-100 грн"; type: "out" }
    }

    function updateCategories() {
        var map = {};
        var colors = ["#4CAF50", "#2196F3", "#FFC107", "#FF5722", "#9C27B0", "#E91E63", "#00BCD4", "#8BC34A"];
        var colorIndex = 0;

        for (var i = 0; i < operationModel.count; i++) {
            var op = operationModel.get(i);
            if (op.type === "out") {
                var name = op.name;
                var amountStr = op.amount.replace(/[^\d\-+]/g, "");
                var value = parseInt(amountStr);

                if (!map[name]) {
                    map[name] = { name: name, amount: 0, color: colors[colorIndex % colors.length] };
                    colorIndex++;
                }

                map[name].amount += Math.abs(value);
            }
        }

        pieCanvas.categories = Object.values(map);
        pieCanvas.requestPaint();
    }

    Rectangle {
        id: header
        width: parent.width
        height: 50
        color: "#2C3E50"
        anchors.top: parent.top

        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 10

            // Ліворуч: логотип або назва
            Text {
                text: "Budgetka"
                color: "white"
                font.pixelSize: 20
                font.bold: true
                verticalAlignment: Text.AlignVCenter
                Layout.alignment: Qt.AlignVCenter
            }

            // Заповнювач простору
            Item {
                Layout.fillWidth: true
            }

            // Бургер меню справа
            Item {
                id: burgerMenu
                width: 40
                height: 30
                Layout.alignment: Qt.AlignVCenter

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: rightDrawer.open()
                }

                Column {
                    anchors.centerIn: parent
                    spacing: 5

                    Rectangle { width: 30; height: 4; color: "white"; radius: 2 }
                    Rectangle { width: 30; height: 4; color: "white"; radius: 2 }
                    Rectangle { width: 30; height: 4; color: "white"; radius: 2 }
                }
            }
        }
    }


    Rectangle {
        id: bgRec
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color: "tan"
    }

    Item {
        id: dateDisplay
        width: parent.width
        height: 50
        anchors.top: header.bottom
        anchors.topMargin: 20

        Text {
            id: dateText
            anchors.centerIn: parent
            color: "black"
            font.pixelSize: 20
            font.bold: true
            text: {
                let today = new Date();
                let day = String(today.getDate()).padStart(2, '0');
                let month = String(today.getMonth() + 1).padStart(2, '0');
                let year = today.getFullYear();
                return `${year}-${month}-${day}`;
            }
        }
    }

    Item {
        id: budgetSection
        width: parent.width
        height: parent.height / 2
        anchors.top: dateDisplay.bottom

        Item {
            width: 250
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            height: pieCanvas.height + balanceContainer.height + 10

            Canvas {
                id: pieCanvas
                width: 250
                height: 250
                property var categories: []

                onPaint: {
                    var ctx = getContext("2d");
                    ctx.clearRect(0, 0, width, height);

                    if (categories.length === 0)
                        return;

                    var total = 0;
                    for (var i = 0; i < categories.length; i++)
                        total += categories[i].amount;

                    var centerX = width / 2;
                    var centerY = height / 2;
                    var radius = Math.min(width, height) / 2 * 0.9;
                    var startAngle = -Math.PI / 2;

                    for (var i = 0; i < categories.length; i++) {
                        var cat = categories[i];
                        var sliceAngle = 2 * Math.PI * (cat.amount / total);

                        ctx.beginPath();
                        ctx.moveTo(centerX, centerY);
                        ctx.arc(centerX, centerY, radius, startAngle, startAngle + sliceAngle);
                        ctx.closePath();

                        ctx.fillStyle = cat.color;
                        ctx.fill();

                        var midAngle = startAngle + sliceAngle / 2;
                        var labelRadius = radius * 0.6;
                        var labelX = centerX + Math.cos(midAngle) * labelRadius;
                        var labelY = centerY + Math.sin(midAngle) * labelRadius;

                        ctx.fillStyle = "white";
                        ctx.font = "bold 12px sans-serif";
                        ctx.textAlign = "center";
                        ctx.textBaseline = "middle";
                        ctx.fillText(cat.name, labelX, labelY);

                        startAngle += sliceAngle;
                    }
                }

                Component.onCompleted: _item.updateCategories()
            }

            Rectangle {
                id: balanceContainer
                anchors.top: pieCanvas.bottom
                anchors.topMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
                width: 200
                height: 50
                radius: 12
                color: "white"
                border.color: "#BDC3C7"
                border.width: 1

                Text {
                    id: balanceText
                    anchors.centerIn: parent
                    color: "#2C3E50"
                    font.pixelSize: 18
                    font.bold: true
                    text: {
                        var sum = 0;
                        for (var i = 0; i < operationModel.count; i++) {
                            var amountStr = operationModel.get(i).amount;
                            var numStr = amountStr.replace(/[^\d\-+]/g, "");
                            sum += parseInt(numStr);
                        }
                        return "Баланс: " + sum + " ₴";
                    }
                }
            }
        }
    }


    Item {
        id: operationSection
        width: parent.width
        anchors.top: budgetSection.bottom
        height: 70

        Item {
            id: buttonContainer
            width: 300
            height: parent.height
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            RoundButton {
                id: addOutcome
                width: 60
                height: 60
                radius: width / 2
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                onClicked: addOutcomeDrawer.open()
                contentItem: Item {
                    anchors.fill: parent
                    Text {
                        text: "-"
                        color: "red"
                        font.pixelSize: 30
                        anchors.centerIn: parent
                    }
                }
                background: Rectangle {
                    color: "#f0f0f0"
                    radius: parent.radius
                    border.color: "red"
                    border.width: 2
                }
            }

            Button {
                text: "Переглянути операції"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                background: Rectangle { color: "#3498db"; radius: 6 }
                onClicked: operationDrawer.open()
                width: 150
                height: 40
            }

            RoundButton {
                id: addIncome
                width: 60
                height: 60
                radius: width / 2
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                onClicked: addIncomeDrawer.open()
                contentItem: Item {
                    anchors.fill: parent
                    Text {
                        text: "+"
                        color: "green"
                        font.pixelSize: 30
                        anchors.centerIn: parent
                    }
                }
                background: Rectangle {
                    color: "#f0f0f0"
                    radius: parent.radius
                    border.color: "green"
                    border.width: 2
                }
            }
        }
    }

    AddOutcomeDrawer {
        id: addOutcomeDrawer
        operationModel: operationModel
        onAccepted: _item.updateCategories()
    }

    AddIncomeDrawer {
        id: addIncomeDrawer
        operationModel: operationModel
    }

    OperationDrawer {
        id: operationDrawer
        operationModel: operationModel
    }

    RightDrawer {
        id: rightDrawer
    }
}
