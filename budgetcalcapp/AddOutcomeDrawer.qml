import QtQuick
import QtQuick.Controls

Drawer {
    id: addOutcomeDrawer
    width: parent.width
    height: parent.height / 2
    edge: Qt.BottomEdge

    property var operationModel
    property string selectedMonth: "06-2025"  // формат MM-YYYY, передавай ззовні
    signal accepted()

    // Універсальна функція встановлення поточної дати
    function setTodayDate() {
        var today = new Date();
        function pad(n) { return n < 10 ? "0" + n : n; }
        var dd = pad(today.getDate());
        var mm = pad(today.getMonth() + 1);
        var yyyy = today.getFullYear();
        addOutcomeDate.text = dd + "-" + mm + "-" + yyyy;
    }

    onOpened: {
        setTodayDate();
    }

    onClosed: {
        addOutcomeAmount.text = "";
        addOutcomeDescription.text = "";
        customOutcomeCategoryField.text = "";
        addOutcomeCategory.currentIndex = 0;
        setTodayDate();
    }

    background: Rectangle {
        anchors.fill: parent
        color: "#2C3E50"
    }

    Item {
        id: addOutcomeTitle
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 10
        width: 150
        height: 30

        Rectangle {
            color: "white"
            radius: 8
            anchors.fill: parent
            anchors.margins: 4
        }

        Text {
            text: "Додати витрати"
            color: "black"
            font.pixelSize: 14
            font.bold: true
            anchors.centerIn: parent
        }
    }

    Item {
        id: addOutcomeForm
        anchors.top: addOutcomeTitle.bottom
        anchors.topMargin: 30
        width: parent.width / 2
        anchors.horizontalCenter: parent.horizontalCenter

        // 1. Date
        TextField {
            Label {
                text: "Дата"
                color: "white"
                anchors.left: parent.left
                anchors.bottom: parent.top
                anchors.bottomMargin: 5
            }
            id: addOutcomeDate
            width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderTextColor: "white"
            readOnly: false
        }

        // 2. Amount
        TextField {
            Label {
                text: "Сума"
                color: "white"
                anchors.left: parent.left
                anchors.bottom: parent.top
                anchors.bottomMargin: 5
            }
            id: addOutcomeAmount
            width: parent.width
            anchors.top: addOutcomeDate.bottom
            anchors.topMargin: 40
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: "00.00"
            placeholderTextColor: "white"
            inputMethodHints: Qt.ImhFormattedNumbersOnly
        }

        // 3. Category
        ComboBox {
            Label {
                text: "Обрати категорію"
                color: "white"
                anchors.left: parent.left
                anchors.bottom: parent.top
                anchors.bottomMargin: 5
            }
            id: addOutcomeCategory
            width: parent.width
            anchors.top: addOutcomeAmount.bottom
            anchors.topMargin: 40
            anchors.horizontalCenter: parent.horizontalCenter
            model: ["Продукти", "Транспорт", "Комунальні", "Одяг", "Інше"]
        }

        // 3.1. Custom category field
        TextField {
            Label {
                text: "Інша категорія"
                color: "white"
                anchors.left: parent.left
                anchors.bottom: parent.top
                anchors.bottomMargin: 5
                visible: addOutcomeCategory.currentText === "Інше"
            }
            id: customOutcomeCategoryField
            width: parent.width
            anchors.top: addOutcomeCategory.bottom
            anchors.topMargin: 40
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: "Введіть назву"
            placeholderTextColor: "white"
            visible: addOutcomeCategory.currentText === "Інше"
        }

        // 4. Description
        TextField {
            Label {
                text: "Опис"
                color: "white"
                anchors.left: parent.left
                anchors.bottom: parent.top
                anchors.bottomMargin: 5
            }
            id: addOutcomeDescription
            width: parent.width
            anchors.top: customOutcomeCategoryField.visible ? customOutcomeCategoryField.bottom : addOutcomeCategory.bottom
            anchors.topMargin: 40
            anchors.horizontalCenter: parent.horizontalCenter
        }

        // Submit button
        RoundButton {
            id: addOutcomeSubmit
            anchors.top: addOutcomeDescription.bottom
            anchors.topMargin: 25
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Прийняти"

            onClicked: {
                var category = addOutcomeCategory.currentText === "Інше"
                                ? customOutcomeCategoryField.text.trim()
                                : addOutcomeCategory.currentText;

                if (category.length === 0) {
                    console.log("Вкажіть категорію");
                    return;
                }

                var dateStr = addOutcomeDate.text.trim();
                if (!dateStr.match(/^\d{2}-\d{2}-\d{4}$/)) {
                    console.log("Невірний формат дати, очікується DD-MM-YYYY");
                    return;
                }

                var amountVal = parseFloat(addOutcomeAmount.text);
                if (isNaN(amountVal) || amountVal <= 0) {
                    console.log("Вкажіть правильну суму > 0");
                    return;
                }

                var amountStr = "-" + amountVal.toFixed(2) + " грн";

                operationModel.append({
                    name: category,
                    amount: amountStr,
                    type: "out",
                    date: dateStr
                });

                accepted();
                addOutcomeDrawer.close();
            }
        }
    }
}
