import QtQuick
import QtQuick.Controls

Drawer {
    id: addIncomeDrawer
    width: parent.width
    height: parent.height / 2
    edge: Qt.BottomEdge

    property var operationModel
    property string selectedMonth: ""  // Передавай звідси бажаний місяць, напр. "06-2025"
    signal accepted()

    // Очистка полів після закриття
    onClosed: {
        addIncomeAmount.text = "";
        addIncomeDescription.text = "";
        customCategoryField.text = "";
        addIncomeCategory.currentIndex = 0;
        if (selectedMonth !== "") {
            // selectedMonth у форматі "MM-YYYY", потрібно зробити "dd-mm-yyyy"
            addIncomeDate.text = "01-" + selectedMonth;
        } else {
            let today = new Date();
            let day = String(today.getDate()).padStart(2, '0');
            let month = String(today.getMonth() + 1).padStart(2, '0');
            let year = today.getFullYear();
            addIncomeDate.text = day + "-" + month + "-" + year;
        }
    }

    background: Rectangle {
        anchors.fill: parent
        color: "#2C3E50"
    }

    Item {
        id: addIncomeTitle
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 10
        width: 180
        height: 30

        Rectangle {
            color: "white"
            radius: 8
            anchors.fill: parent
            anchors.margins: 4
        }

        Text {
            text: "Додати надходження"
            color: "black"
            font.pixelSize: 14
            font.bold: true
            anchors.centerIn: parent
        }
    }

    Item {
        id: addIncomeForm
        anchors.top: addIncomeTitle.bottom
        anchors.topMargin: 30
        width: parent.width / 2
        anchors.horizontalCenter: parent.horizontalCenter

        // 1. Дата
        TextField {
            Label {
                text: "Дата"
                color: "white"
                anchors.left: parent.left
                anchors.bottom: parent.top
                anchors.bottomMargin: 5
            }
            id: addIncomeDate
            width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderTextColor: "white"
            text: selectedMonth !== "" ? ("01-" + selectedMonth) :
                  (function() {
                      var today = new Date();
                      var day = String(today.getDate()).padStart(2, '0');
                      var month = String(today.getMonth() + 1).padStart(2, '0');
                      var year = today.getFullYear();
                      return day + "-" + month + "-" + year;
                  })()

            readOnly: false
        }

        // 2. Сума
        TextField {
            Label {
                text: "Сума"
                color: "white"
                anchors.left: parent.left
                anchors.bottom: parent.top
                anchors.bottomMargin: 5
            }
            id: addIncomeAmount
            width: parent.width
            anchors.top: addIncomeDate.bottom
            anchors.topMargin: 40
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: "00.00"
            placeholderTextColor: "white"
        }

        // 3. Категорія
        ComboBox {
            Label {
                text: "Обрати категорію"
                color: "white"
                anchors.left: parent.left
                anchors.bottom: parent.top
                anchors.bottomMargin: 5
            }
            id: addIncomeCategory
            width: parent.width
            anchors.top: addIncomeAmount.bottom
            anchors.topMargin: 40
            anchors.horizontalCenter: parent.horizontalCenter
            model: ["Зарплата", "Депозит", "Заощадження", "Інше"]
        }

        // 3.1. Інша категорія (текстове поле)
        TextField {
            Label {
                text: "Інша категорія"
                color: "white"
                anchors.left: parent.left
                anchors.bottom: parent.top
                anchors.bottomMargin: 5
                visible: addIncomeCategory.currentText === "Інше"
            }
            id: customCategoryField
            width: parent.width
            anchors.top: addIncomeCategory.bottom
            anchors.topMargin: 40
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: "Введіть назву"
            placeholderTextColor: "white"
            visible: addIncomeCategory.currentText === "Інше"
        }

        // 4. Опис
        TextField {
            Label {
                text: "Опис"
                color: "white"
                anchors.left: parent.left
                anchors.bottom: parent.top
                anchors.bottomMargin: 5
            }
            id: addIncomeDescription
            width: parent.width
            anchors.top: customCategoryField.visible ? customCategoryField.bottom : addIncomeCategory.bottom
            anchors.topMargin: 40
            anchors.horizontalCenter: parent.horizontalCenter
        }

        // Кнопка "Прийняти"
        RoundButton {
            id: addIncomeSubmit
            anchors.top: addIncomeDescription.bottom
            anchors.topMargin: 25
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Прийняти"

            onClicked: {
                var category = addIncomeCategory.currentText === "Інше"
                                ? customCategoryField.text.trim()
                                : addIncomeCategory.currentText;

                if (category.length === 0) {
                    console.log("Категорія не може бути порожньою");
                    return;
                }

                var dateStr = addIncomeDate.text.trim();
                if (!dateStr.match(/^\d{2}-\d{2}-\d{4}$/)) {
                    console.log("Невірний формат дати, очікується dd-mm-yyyy");
                    return;
                }

                var amountStr = "+" + addIncomeAmount.text.trim() + " грн";
                if (addIncomeAmount.text.trim() === "") {
                    console.log("Сума не може бути порожньою");
                    return;
                }

                operationModel.append({
                    name: category,
                    amount: amountStr,
                    type: "in",
                    date: dateStr,
                    description: addIncomeDescription.text.trim()
                });

                accepted();
                addIncomeDrawer.close();
            }
        }
    }
}
