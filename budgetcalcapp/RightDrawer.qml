import QtQuick 2.15
import QtQuick.Controls 2.15

Drawer {
    id: drawer
    width: parent ? parent.width / 3 : 300
    height: parent ? parent.height : 600
    edge: Qt.RightEdge
    modal: true

    property bool showAccountsSubmenu: false

    signal goToMainPage()
    signal goToMainPage2()

    background: Rectangle {
        color: "#2C3E50"
    }

    onClosed: showAccountsSubmenu = false

    Column {
        spacing: 16
        anchors.fill: parent
        anchors.margins: 20

        Label {
            text: "Меню"
            font.pixelSize: 22
            font.bold: true
            color: "white"
        }

        Button {
            text: showAccountsSubmenu ? "Рахунки ▲" : "Рахунки ▼"
            onClicked: showAccountsSubmenu = !showAccountsSubmenu
        }

        Column {
            visible: showAccountsSubmenu
            spacing: 8
            anchors.left: parent.left
            anchors.leftMargin: 16

            Button {
                text: "Готівка"
                onClicked: {
                    drawer.close()
                    goToMainPage()
                }
            }

            Button {
                text: "Картка"
                onClicked: {
                    drawer.close()
                    goToMainPage2()
                }
            }
        }

        Button {
            text: "Налаштування"
            onClicked: drawer.close()
        }
    }
}
