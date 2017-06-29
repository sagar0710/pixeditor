import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.2
import QtQuick.Window 2.0

import App 1.0

import "." as Ui

Page {
    id: root
    implicitWidth: 200
    background: Rectangle {
        color: Ui.CanvasColours.panelColour
    }

    padding: 0

    property LayeredImageCanvas layeredImageCanvas
    property LayeredImageProject project

    ButtonGroup {
        buttons: listView.contentItem.children
    }

    ListView {
        id: listView
        objectName: "layerListView"
        anchors.fill: parent
        boundsBehavior: ListView.StopAtBounds
        visible: project && project.loaded
        clip: true

        ScrollBar.vertical: ScrollBar {
            id: verticalScrollBar
        }
        ScrollBar.horizontal: ScrollBar {
            id: horizontalScrollBar
        }

        model: LayerModel {
            layeredImageProject: project
        }

        delegate: ItemDelegate {
            objectName: model.layer.name
            checkable: true
            checked: project.currentLayerIndex === index
            width: listView.width
            leftPadding: visibilityCheckBox.width + 18
            focusPolicy: Qt.NoFocus

            onClicked: project.currentLayerIndex = index
            onDoubleClicked: layerNameTextField.forceActiveFocus()

            CheckBox {
                id: visibilityCheckBox
                objectName: "layerVisibilityCheckBox"
                x: 14
                text: model.layer.visible ? "\uf06e" : "\uf070"
                font.family: "FontAwesome"
                focusPolicy: Qt.NoFocus
                indicator: null
                anchors.verticalCenter: parent.verticalCenter

                onClicked: project.setLayerVisible(index, !model.layer.visible)
            }

            TextField {
                id: layerNameTextField
                objectName: "layerNameTextField"
                x: parent.leftPadding
                text: model.layer.name
                font.family: "FontAwesome"
                activeFocusOnPress: false
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 6
                background.visible: false
                font.pixelSize: 12
                visible: false

                Keys.onEscapePressed: {
                    text = model.layer.name;
                    layeredImageCanvas.forceActiveFocus();
                }
                onAccepted: {
                    project.setLayerName(index, text);
                    layeredImageCanvas.forceActiveFocus();
                }
            }

            // We don't want TextField's editable cursor to be visible,
            // so we set visible: false to disable the cursor, and instead
            // render it via this.
            ShaderEffectSource {
                sourceItem: layerNameTextField
                anchors.fill: layerNameTextField
            }

            // Apparently the one above only works for the top level control item,
            // so we also need one for the background.
            ShaderEffectSource {
                sourceItem: layerNameTextField.background
                x: layerNameTextField.x + layerNameTextField.background.x
                y: layerNameTextField.y + layerNameTextField.background.y
                width: layerNameTextField.background.width
                height: layerNameTextField.background.height
                visible: layerNameTextField.activeFocus
            }

            Rectangle {
                id: focusRect
                width: 2
                height: parent.height
                color: Ui.CanvasColours.focusColour
                visible: parent.checked
            }
        }
    }

    MenuSeparator {
        width: parent.width
        y: footer.y - height
        padding: 6
        topPadding: 0
        bottomPadding: 0
    }

    footer: RowLayout {
        Button {
            objectName: "newLayerButton"
            text: "+"
            flat: true
            focusPolicy: Qt.NoFocus
            hoverEnabled: true

            Layout.maximumWidth: implicitHeight
            Layout.fillWidth: true
            Layout.leftMargin: 6

            onClicked: project.addNewLayer()
        }

        Button {
            objectName: "moveLayerDownButton"
            text: "\uf107"
            font.family: "FontAwesome"
            flat: true
            focusPolicy: Qt.NoFocus
            hoverEnabled: true
            enabled: project && project.currentLayerIndex < project.layerCount - 1

            Layout.maximumWidth: implicitHeight
            Layout.fillWidth: true

            onClicked: project.moveCurrentLayerDown()
        }

        Button {
            objectName: "moveLayerUpButton"
            text: "\uf106"
            font.family: "FontAwesome"
            flat: true
            focusPolicy: Qt.NoFocus
            hoverEnabled: true
            enabled: project && project.currentLayerIndex > 0

            Layout.maximumWidth: implicitHeight
            Layout.fillWidth: true

            onClicked: project.moveCurrentLayerUp()
        }

        Item {
            Layout.fillWidth: true
        }

        Button {
            objectName: "deleteLayerButton"
            text: "\uf1f8"
            font.family: "FontAwesome"
            flat: true
            focusPolicy: Qt.NoFocus
            enabled: project && project.currentLayer && project.layerCount > 1
            hoverEnabled: true

            Layout.maximumWidth: implicitHeight
            Layout.fillWidth: true
            Layout.rightMargin: 6

            onClicked: project.deleteCurrentLayer()
        }
    }
}
