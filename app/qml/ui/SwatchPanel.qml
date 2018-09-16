/*
    Copyright 2018, Mitch Curtis

    This file is part of Slate.

    Slate is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Slate is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Slate. If not, see <http://www.gnu.org/licenses/>.
*/

import QtQuick 2.9
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.4
import QtQuick.Window 2.0

import App 1.0

import "." as Ui

Panel {
    id: root
    objectName: "swatchesPanel"
    title: qsTr("Swatches")
    topPadding: 0
    leftPadding: 0
    rightPadding: 0
    bottomPadding: 6

    property ImageCanvas canvas
    property Project project

//    property int draggedSwatchIndex: -1

    contentItem: ColumnLayout {
        visible: root.expanded
        spacing: 0

        // We use RowLayouts here because we want the ScrollBar to be positioned
        // outside of the view.
        RowLayout {
            id: autoSwatchRowLayout
            spacing: 0

            // Match the gap that the scrollbar leaves on the right.
            Layout.leftMargin: 16
            Layout.preferredHeight: 100

            Loader {
                active: settings.autoSwatchEnabled

                Layout.fillWidth: true
                Layout.fillHeight: true

                sourceComponent: SwatchGridView {
                    id: autoSwatchGridView
                    objectName: "autoSwatchGridView"
                    readOnly: true

                    property real contentYBeforeModelReset
                    property bool lockContentY

                    // Try to maintain the same contentY between model changes
                    // so that the view doesn't fly back to the top every time a new colour is added.
                    Binding {
                        target: autoSwatchGridView
                        property: "contentY"
                        value: autoSwatchGridView.contentYBeforeModelReset
                        when: autoSwatchGridView.lockContentY
                    }

                    Connections {
                        target: autoSwatchGridView.model
                        onModelAboutToBeReset: {
                            autoSwatchGridView.contentYBeforeModelReset = autoSwatchGridView.contentY
                            autoSwatchGridView.lockContentY = true
                        }
                        onModelReset: {
                            Qt.callLater(function() {
                                autoSwatchGridView.lockContentY = false
                                autoSwatchGridView.contentY = autoSwatchGridView.contentYBeforeModelReset
                            });
                        }
                    }

                    ScrollBar.vertical: ScrollBar {
                        parent: autoSwatchRowLayout

                        Layout.fillHeight: true
                    }

                    model: AutoSwatchModel {
                        canvas: root.canvas
                    }
                }
            }
        }
        MenuSeparator {
            visible: settings.autoSwatchEnabled
            leftPadding: 12
            topPadding: 0
            rightPadding: 12
            bottomPadding: 0

            Layout.fillWidth: true
        }
        RowLayout {
            id: swatchRowLayout
            spacing: 0

            Layout.leftMargin: 16
            Layout.preferredHeight: 100

            SwatchGridView {
                id: swatchGridView
                objectName: "swatchGridView"
                readOnly: false
                swatchContextMenu: contextMenu

                Layout.fillWidth: true
                Layout.fillHeight: true

                ScrollBar.vertical: ScrollBar {
                    parent: swatchRowLayout

                    Layout.fillHeight: true
                }

                model: SwatchModel {
                    project: root.project
                }
            }
        }
    }

    footer: ColumnLayout {
        visible: root.expanded
        spacing: 0

        MenuSeparator {
            padding: 6
            topPadding: 0
            bottomPadding: 0

            Layout.fillWidth: true
        }

        RowLayout {
            id: footerRowLayout

            Button {
                objectName: "newSwatchColourButton"
                text: "+"
                flat: true
                focusPolicy: Qt.NoFocus
                hoverEnabled: true

                Layout.maximumWidth: implicitHeight
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.leftMargin: 6

                ToolTip.text: qsTr("New swatch colour")
                ToolTip.visible: hovered
                ToolTip.delay: toolTipDelay
                ToolTip.timeout: toolTipTimeout

                onClicked: project.swatch.addColour("", canvas.penForegroundColour)
            }

            Item {
                Layout.fillWidth: true
            }

            // TODO: need to fix drag and drop to be able to use this
            /*Button {
                objectName: "deleteSwatchColourButton"
                text: "\uf1f8"
                font.family: "FontAwesome"
                flat: true
                focusPolicy: Qt.NoFocus
                enabled: project && dropArea.containsDrag
                hoverEnabled: true

                Layout.maximumWidth: implicitHeight
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.rightMargin: 6

                DropArea {
                    id: dropArea
                    anchors.fill: parent
                    onContainsDragChanged: print("containsDrag", containsDrag)
                }

//                onClicked: project.swatch.removeColour(draggedSwatchIndex)
            }*/
        }
    }

    Menu {
        id: contextMenu
        objectName: "swatchContextMenu"
        x: rightClickedColourIndex !== -1 ? rightClickedColourPos.x : 0
        y: rightClickedColourIndex !== -1 ? rightClickedColourPos.y : 0
        modal: true
        dim: false

        property int rightClickedColourIndex
        property string rightClickedColourName
        property int rightClickedColourX
        property int rightClickedColourY

        readonly property point rightClickedColourPos: rightClickedColourIndex !== -1 ? swatchGridView.contentItem.mapToItem(
            root, rightClickedColourX, rightClickedColourY) : Qt.point(0, 0)

        onClosed: {
            rightClickedColourIndex = -1
            rightClickedColourName = ""
        }

        MenuItem {
            id: renameMenuItem
            objectName: "renameSwatchColourMenuItem"
            text: qsTr("Rename")
            onTriggered: {
                renameDialog.colourIndex = contextMenu.rightClickedColourIndex
                renameDialog.oldName = contextMenu.rightClickedColourName
                renameDialog.open()
            }
        }

        MenuItem {
            objectName: "deleteSwatchColourMenuItem"
            text: qsTr("Delete")
            onTriggered: project.swatch.removeColour(contextMenu.rightClickedColourIndex)
        }
    }

    Dialog {
        id: renameDialog
        title: qsTr("Rename colour swatch")
        modal: true
        dim: false
        focus: true
        standardButtons: Dialog.Ok | Dialog.Cancel
        parent: Overlay.overlay
        anchors.centerIn: parent

        property string oldName
        property int colourIndex

        onAboutToShow: {
            nameTextField.text = oldName
            nameTextField.selectAll()
            nameTextField.forceActiveFocus()
        }
        onAccepted: project.swatch.renameColour(colourIndex, nameTextField.text)
        onClosed: {
            colourIndex = -1
            oldName = ""
        }

        TextField {
            id: nameTextField
            width: parent.width
            onAccepted: renameDialog.accept()
        }
    }
}
