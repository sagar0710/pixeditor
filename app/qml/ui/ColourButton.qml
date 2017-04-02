import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Button {
    id: control
    checkable: true
    hoverEnabled: true
    focusPolicy: Qt.NoFocus

    property alias color: rect.color

    background: Rectangle {
        id: rect

        Rectangle {
            anchors.fill: parent
            color: "transparent"
            border.color: "#444444"
        }

        Rectangle {
            anchors.fill: parent
            anchors.margins: 1
            color: "transparent"
            border.color: "#dddddd"
        }

        SwatchFocusRectangle {
            x: -1
            width: 1
            height: parent.height
            color: focusColour
            visible: control.checked
        }
    }
}
