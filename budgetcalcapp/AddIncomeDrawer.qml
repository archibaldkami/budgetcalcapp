import QtQuick
import QtQuick.Controls

Drawer {
    id: addIncomeDrawer
    width: parent.width
    height: parent.height / 2
    edge: Qt.BottomEdge

    background: Rectangle{
        anchors.fill: parent
        color: "#2C3E50"
    }

    Label {
        id: addIncomeTitle
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 10
        text: "Add Income Operation"
        color: "white"
    }

    Item{
        id: addIncomeForm
        anchors.top: addIncomeTitle.bottom
        anchors.topMargin: 30
        width: parent.width / 2
        anchors.horizontalCenter: parent.horizontalCenter

        TextField{
            Label{
                id: incomeDescriptionLabel
                text: "Description"
                color: "white"
                anchors.left: parent.left
                anchors.bottom: parent.top
                anchors.bottomMargin: 5
            }
            id: addIncomeDescription
            width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter
        }

        ComboBox{
            Label{
                id: incomeCategoryLabel
                text: "Category"
                color: "white"
                anchors.left: parent.left
                anchors.bottom: parent.top
                anchors.bottomMargin: 5
            }
            id: addIncomeCategory
            width: parent.width
            anchors.top: addIncomeDescription.bottom
            anchors.topMargin: 40
            anchors.horizontalCenter: parent.horizontalCenter
            model: ["First", "Second", "Third"]
        }

        TextField{
            Label{
                id: incomeDateLabel
                text: "Date"
                color: "white"
                anchors.left: parent.left
                anchors.bottom: parent.top
                anchors.bottomMargin: 5
            }
            id: addIncomeDate
            width: parent.width
            anchors.top: addIncomeCategory.bottom
            anchors.topMargin: 40
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: "00-00-00"
            placeholderTextColor: "white"
        }

        TextField{
            Label{
                id: incomeAmountLabel
                text: "Amount"
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
        RoundButton{
            id: addIncomeSubmit
            anchors.top: addIncomeAmount.bottom
            anchors.topMargin: 25
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Submit"
        }
    }
}
