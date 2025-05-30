import QtQuick
import QtQuick.Controls

Item {
    id: _item
    width: parent.width
    height: parent.height

    Rectangle {
        id: bgRec
        anchors.fill: parent
        color: "#1b1c1d"
    }

    Item{
        id: budgetSection
        width: parent.width
        height: parent.height / 3
        Label{
            id: budgetTitle
            text: "Budget"
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: "white"
        }
        Label{
            id: incomeValue
            text: "+999999"
            color: "green"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: budgetTitle.top
            anchors.topMargin: 30
        }
        Label{
            id: outcomeValue
            text: "-99999"
            color: "red"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: incomeValue.top
            anchors.topMargin: 20
        }
    }
    Item{
        id: operationSection
        width: parent.width
        anchors.top: budgetSection.bottom
        height: 50

        RoundButton{
            id: addOutcome
            text: "-"
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.verticalCenter: parent.verticalCenter
            onClicked: {
                addOutcomeDrawer.open()
            }
        }

        Button{
            id: operation
            text: "Check operation"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }

        RoundButton{
            id: addIncome
            text: "+"
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.verticalCenter: parent.verticalCenter
            onClicked:{
                addIncomeDrawer.open()
            }
        }

    }
    AddOutcomeDrawer{
        id: addOutcomeDrawer
    }
    AddIncomeDrawer{
        id: addIncomeDrawer
    }

}
