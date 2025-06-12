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

        initialItem: MainPage {
            stackView: contentFrame
        }
    }
}
