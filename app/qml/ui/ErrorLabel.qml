import QtQuick
import QtQuick.Controls

Label {
    text: "\uf06a"
    font.family: "FontAwesome"

    property bool valid: false
    property string errorMessage

    MouseArea {
        hoverEnabled: true
        anchors.fill: parent

        ToolTip.text: errorMessage
        ToolTip.visible: containsMouse
    }
}
