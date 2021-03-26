import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Templates as T

import App

import "." as Ui

Page {
    id: root
    objectName: "panel"
    background: Rectangle {
        color: Ui.Theme.panelColour
    }
    topPadding: 0
    bottomPadding: 0

    property bool expanded: true
    property T.Popup settingsPopup: null
    property alias settingsPopupToolButton: settingsPopupToolButton

    header: RowLayout {
        objectName: root.objectName + "Header"
        spacing: 0

        Label {
            objectName: parent.objectName + "TitleLabel"
            text: root.title
            font.bold: true

            Layout.leftMargin: 16
        }

        Item {
            Layout.fillWidth: true
        }

        ToolButton {
            id: settingsPopupToolButton
            objectName: root.objectName + "SettingsToolButton"
            text: "\uf013"
            font.family: "FontAwesome"
            focusPolicy: Qt.NoFocus
            visible: settingsPopup

            Layout.preferredWidth: implicitHeight

            onClicked: settingsPopup.open()
        }

        ToolButton {
            objectName: root.objectName + "HideShowToolButton"
            text: expanded ? "\uf146" : "\uf0fe"
            font.family: "FontAwesome"
            focusPolicy: Qt.NoFocus

            Layout.leftMargin: -8
            Layout.preferredWidth: implicitHeight

            onClicked: expanded = !expanded
        }
    }
}
