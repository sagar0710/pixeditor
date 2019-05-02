/*
    Copyright 2019, Mitch Curtis

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

import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import App 1.0

import "." as Ui

Dialog {
    id: root
    objectName: "opacityDialog"
    title: qsTr("Opacity")
    modal: true
    dim: false
    focus: true

    property Project project
    property ImageCanvas canvas

    property real hslAlpha

    readonly property real sliderStepSize: 0.001

    TextMetrics {
        id: valueTextMetrics
        font: opacityTextField.font
        text: "-0.0001"
    }

    function modifySelectionHsl() {
        var flags = ImageCanvas.DefaultAlphaAdjustment
        if (doNotModifyFullyTransparentPixelsCheckBox.checked)
            flags |= ImageCanvas.DoNotModifyFullyTransparentPixels
        if (doNotModifyFullyOpaquePixelsCheckBox.checked)
            flags |= ImageCanvas.DoNotModifyFullyOpaquePixels
        canvas.modifySelectionHsl(0, 0, 0, hslAlpha, flags)
    }

    onAboutToShow: {
        if (project) {
            hslAlpha = 0

            opacityTextField.forceActiveFocus()

            canvas.beginModifyingSelectionHsl()
        }
    }

    onClosed: {
        canvas.forceActiveFocus()
        if (canvas.adjustingImage) {
            // The dialog can be closed in two ways, so it's easier just to discard any adjustment here.
            canvas.endModifyingSelectionHsl(ImageCanvas.RollbackAdjustment)
        }
    }

    contentItem: GridLayout {
        columns: 3
        columnSpacing: 24
        rowSpacing: 0

        Label {
            text: qsTr("Opacity")

            Layout.fillWidth: true
        }
        Slider {
            id: opacitySlider
            objectName: root.objectName + "OpacitySlider"
            from: -1
            to: 1
            value: hslAlpha
            stepSize: sliderStepSize
            leftPadding: 0
            rightPadding: 0

            ToolTip.text: qsTr("Changes the opacity of the image")
            ToolTip.visible: hovered
            ToolTip.delay: toolTipDelay

            onMoved: {
                hslAlpha = value
                modifySelectionHsl()
            }

            background: Item {
                // Default Material values.
                implicitWidth: 200
                implicitHeight: 48

                HorizontalGradientRectangle {
                    width: parent.width
                    gradient: Gradient {
                        GradientStop {
                            position: 0
                            color: "transparent"
                        }
                        GradientStop {
                            position: 1
                            color: Ui.CanvasColours.focusColour
                        }
                    }
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
        DoubleTextField {
            id: opacityTextField
            objectName: root.objectName + "OpacityTextField"
            propertySource: root
            propertyName: "hslAlpha"
            // If the maximum length matches the text metrics text, the sign
            // character will sometimes be slightly cut off, so account for that.
            maximumLength: valueTextMetrics.text.length - 1

            Layout.maximumWidth: valueTextMetrics.width
            Layout.fillWidth: true

            // We call this here instead of just doing it in e.g. onHslHueChanged
            // because that would require us to block changes that occur when the
            // HSL property values are reset upon showing the dialog. Modifying the
            // selection only on user interaction instead feels nicer.
            onValueModified: modifySelectionHsl()
        }

        CheckBox {
            id: doNotModifyFullyTransparentPixelsCheckBox
            objectName: "doNotModifyFullyTransparentPixelsCheckBox"
            text: qsTr("Do not modify fully transparent pixels")
            checked: true

            Layout.columnSpan: 3

            ToolTip.text: qsTr("Only change the alpha if it's non-zero to prevent fully transparent pixels from gaining opacity.")
            ToolTip.visible: hovered
            ToolTip.delay: toolTipDelay

            onClicked: modifySelectionHsl()
        }

        CheckBox {
            id: doNotModifyFullyOpaquePixelsCheckBox
            objectName: "doNotModifyFullyOpaquePixelsCheckBox"
            text: qsTr("Do not modify fully opaque pixels")
            checked: true

            Layout.columnSpan: 3

            ToolTip.text: qsTr("Only change the alpha if it's less than one to prevent fully opaque pixels from losing opacity.")
            ToolTip.visible: hovered
            ToolTip.delay: toolTipDelay

            onClicked: modifySelectionHsl()
        }
    }

    footer: DialogButtonBox {
        Button {
            objectName: "opacityDialogOkButton"
            text: qsTr("OK")

            DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole

            onClicked: {
                canvas.endModifyingSelectionHsl(ImageCanvas.CommitAdjustment)
                root.close()
            }
        }
        Button {
            objectName: "opacityDialogCancelButton"
            text: qsTr("Cancel")

            DialogButtonBox.buttonRole: DialogButtonBox.DestructiveRole

            onClicked: root.close()
        }
    }
}
