import QtQuick
import QtQuick.Controls

Window {
    width: 400
    height: 720
    visible: true
    title: "Budget Calculation App"

    StackView {
        id: contentFrame
        anchors.fill: parent
        width: parent.width
        height: parent.height
        initialItem: Qt.resolvedUrl("LoadPage.qml")
        Component.onCompleted: {
            contentFrame.replace("MainPage.qml")
        }
    }
}
