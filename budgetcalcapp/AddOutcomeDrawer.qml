import QtQuick
import QtQuick.Controls

Drawer {
    id: addOutcomeDrawer
    width: parent.width
    height: parent.height / 2
    edge: Qt.BottomEdge

    background: Rectangle{
        anchors.fill: parent
        color: "#2C3E50"
    }

    Label {
        id: addOutcomeTitle
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 10
        text: "Add Outcome Operation"
        color: "white"
    }

    Item{
        id: addOutcomeForm
        anchors.top: addOutcomeTitle.bottom
        anchors.topMargin: 30
        width: parent.width / 2
        anchors.horizontalCenter: parent.horizontalCenter

        TextField{
            Label{
                id: outcomeDescriptionLabel
                text: "Description"
                color: "white"
                anchors.left: parent.left
                anchors.bottom: parent.top
                anchors.bottomMargin: 5
            }
            id: addOutcomeDescription
            width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter
        }

        ComboBox{
            Label{
                id: outcomeCategoryLabel
                text: "Category"
                color: "white"
                anchors.left: parent.left
                anchors.bottom: parent.top
                anchors.bottomMargin: 5
            }
            id: addOutcomeCategory
            width: parent.width
            anchors.top: addOutcomeDescription.bottom
            anchors.topMargin: 40
            anchors.horizontalCenter: parent.horizontalCenter
            model: ["First", "Second", "Third"]
        }

        TextField{
            Label{
                id: outcomeDateLabel
                text: "Date"
                color: "white"
                anchors.left: parent.left
                anchors.bottom: parent.top
                anchors.bottomMargin: 5
            }
            id: addOutcomeDate
            width: parent.width
            anchors.top: addOutcomeCategory.bottom
            anchors.topMargin: 40
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: "00-00-00"
            placeholderTextColor: "white"
        }

        TextField{
            Label{
                id: outcomeAmountLabel
                text: "Amount"
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
        }

        RoundButton{
            id: addOutcomeSubmit
            anchors.top: addOutcomeAmount.bottom
            anchors.topMargin: 25
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Submit"
        }
    }
}
