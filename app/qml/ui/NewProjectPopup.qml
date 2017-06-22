import QtGraphicalEffects 1.0
import QtQuick 2.6
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.1
import QtQuick.Templates 2.1 as T
import Qt.labs.platform 1.0 as Platform

import App 1.0

Dialog {
    id: popup
    objectName: "newProjectPopup"
    modal: true
    closePolicy: Popup.CloseOnEscape
    focus: true
    padding: 20
    contentWidth: 600
    contentHeight: 400

    onAboutToShow: {
        buttonGroup.checkedButton = null;
        tilesetProjectButton.forceActiveFocus();
    }

    signal choseTilesetProject
    signal choseImageProject
    signal choseLayeredImageProject

    onAccepted: {
        if (buttonGroup.checkedButton === tilesetProjectButton)
            choseTilesetProject()
        else if (buttonGroup.checkedButton === imageProjectButton)
            choseImageProject()
        else if (buttonGroup.checkedButton === layeredImageProjectButton)
            choseLayeredImageProject()
    }

    contentItem: ColumnLayout {
        spacing: 14

        ButtonGroup {
            id: buttonGroup
            buttons: popup.contentItem.children
        }

        ProjectTemplateButton {
            id: tilesetProjectButton
            objectName: "tilesetProjectButton"
            titleText: qsTr("New Tileset")
            descriptionText: qsTr("Creates a new tileset bitmap image for editing. "
               + "Paint tiles from an image onto a grid. "
               + "An accompanying project file is created to save the contents of the grid.")
            radius: popup.background.radius
            iconBackgroundColour: Qt.darker(popup.background.color, 1.15)

            onClicked: popup.accept()

            Layout.fillWidth: true
            Layout.fillHeight: true

            Label {
                text: "\uf00a"
                font.family: "FontAwesome"
                font.pixelSize: 200
                fontSizeMode: Label.Fit
                verticalAlignment: Label.AlignVCenter

                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Label {
                text: "\uf1c5"
                font.family: "FontAwesome"
                font.pixelSize: 200
                fontSizeMode: Label.Fit
                verticalAlignment: Label.AlignVCenter

                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }

        ProjectTemplateButton {
            id: imageProjectButton
            objectName: "imageProjectButton"
            titleText: qsTr("New Image")
            descriptionText: qsTr("Creates a new bitmap image for direct editing, with no layer support.")
            radius: popup.background.radius
            iconBackgroundColour: Qt.darker(popup.background.color, 1.15)

            onClicked: popup.accept()

            Layout.fillWidth: true
            Layout.fillHeight: true

            Label {
                text: "\uf1c5"
                font.family: "FontAwesome"
                font.pixelSize: 200
                fontSizeMode: Label.Fit
                verticalAlignment: Label.AlignVCenter

                Layout.preferredWidth: height
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignCenter
            }
        }

        ProjectTemplateButton {
            id: layeredImageProjectButton
            objectName: "layeredImageProjectButton"
            titleText: qsTr("New Layered Image")
            descriptionText: qsTr("Creates a new layered image project. Several images are drawn on "
                + "top of each other, and can be exported as a single image.")
            radius: popup.background.radius
            iconBackgroundColour: Qt.darker(popup.background.color, 1.15)

            onClicked: popup.accept()

            Layout.fillWidth: true
            Layout.fillHeight: true

            Repeater {
                model: 3

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Label {
                        text: "\uf1c5"
                        font.family: "FontAwesome"
                        font.pixelSize: 38
                        anchors.centerIn: parent
                    }
                }
            }
        }
    }
}
