import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Drawer {
    id: operationDrawer
    width: parent.width
    height: parent.height / 2
    edge: Qt.BottomEdge
    modal: true

    property var operationModel    // Оголошуємо property для прийому моделі

    background: Rectangle {
        anchors.fill: parent
        color: "#2C3E50"
    }

    Label {
        id: operationTitle
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 10
        text: "Операції"
        color: "white"
        font.pixelSize: 14
    }

    Flickable {
        anchors.top: operationTitle.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        contentWidth: parent.width
        contentHeight: columnOperations.height
        clip: true

        Column {
            id: columnOperations
            width: parent.width
            spacing: 10
            anchors.margins: 20

            Repeater {
                model: operationModel

                delegate: Rectangle {
                    width: parent.width * 0.9
                    height: 40
                    color: model.type === "in" ? "#e8f5e9" : "#ffebee"
                    border.color: "#ccc"
                    radius: 8
                    anchors.horizontalCenter: parent.horizontalCenter

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 10

                        Text {
                            text: model.name
                            color: "#333"
                            font.pixelSize: 16
                            verticalAlignment: Text.AlignVCenter
                            Layout.preferredWidth: 120
                        }

                        Text {
                            text: model.amount
                            color: model.type === "in" ? "green" : "red"
                            font.pixelSize: 16
                            verticalAlignment: Text.AlignVCenter
                        }

                        Item {
                            Layout.fillWidth: true
                        }

                        Text {
                            text: model.date
                            color: "#666"
                            font.pixelSize: 14
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignRight
                            Layout.preferredWidth: 100
                        }
                    }
                }
            }
        }
    }
}
