import QtQuick 2.6
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.3

import App 1.0

Dialog {
    id: dialog
    objectName: "imageSizePopup"
    title: qsTr("Choose a size for the image")
    modal: true
    dim: false
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnReleaseOutside

    property Project project

    onVisibleChanged: {
        if (visible && project) {
            widthSpinBox.value = project.size.width;
            heightSpinBox.value = project.size.height;
            smoothCheckBox.checked = false;
            widthSpinBox.contentItem.forceActiveFocus();
        }
    }

    contentItem: ColumnLayout {
        Item {
            Layout.preferredHeight: 4
        }

        GridLayout {
            columns: 3

            Item {
                id: preserveContainer

                Layout.row: 0
                Layout.rowSpan: 2
                Layout.column: 0
                Layout.preferredWidth: preserveAspectRatioButton.implicitWidth
                Layout.fillHeight: true

                Rectangle {
                    width: 1
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: preserveContainer.height / 4
                    anchors.bottom: parent.verticalCenter
                    anchors.bottomMargin: (preserveAspectRatioButton.height - 16) / 2 + 4

                    Rectangle {
                        width: preserveAspectRatioButton.width / 2 - 6
                        height: 1
                        anchors.left: parent.left
                    }
                }

                Rectangle {
                    width: 1
                    anchors.top: parent.verticalCenter
                    anchors.topMargin: (preserveAspectRatioButton.height - 16) / 2 + 4
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: preserveContainer.height / 4
                    anchors.horizontalCenter: parent.horizontalCenter

                    Rectangle {
                        width: preserveAspectRatioButton.width / 2 - 6
                        height: 1
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                    }
                }

                IconToolButton {
                    id: preserveAspectRatioButton
                    objectName: "preserveAspectRatioButton"
                    text: "\uf0c1"
                    checked: true
                    topPadding: 0
                    bottomPadding: 0
                    anchors.verticalCenter: parent.verticalCenter

                    ToolTip.text: qsTr("Preserve aspect ratio")
                    ToolTip.visible: hovered
                }
            }

            Label {
                text: qsTr("Image width")
                Layout.row: 0
                Layout.column: 1
            }

            SpinBox {
                id: widthSpinBox
                objectName: "changeImageWidthSpinBox"
                from: 1
                to: 5120
                editable: true
                stepSize: 1

                Layout.row: 0
                Layout.column: 2

                ToolTip.text: qsTr("Image width in pixels")

                onValueModified: valueUpdated(value)

                // https://bugreports.qt.io/browse/QTBUG-64151
//                Connections {
//                    target: widthSpinBox.contentItem
//                    onTextEdited: widthSpinBox.valueUpdated(widthSpinBox.valueFromText(widthSpinBox.contentItem.text, widthSpinBox.locale))
//                }

                function valueUpdated(value) {
                    if (preserveAspectRatioButton.checked) {
                        var aspectRatio = project.size.width / project.size.height
                        heightSpinBox.value = value / aspectRatio
                    }
                }
            }

            Label {
                text: qsTr("Image height")
                Layout.row: 1
                Layout.column: 1
            }

            SpinBox {
                id: heightSpinBox
                objectName: "changeImageHeightSpinBox"
                from: 1
                to: 5120
                editable: true
                stepSize: 1

                Layout.row: 1
                Layout.column: 2

                ToolTip.text: qsTr("Image height in pixels")

                onValueModified: valueUpdated(value)

                // https://bugreports.qt.io/browse/QTBUG-64151
//                Connections {
//                    target: heightSpinBox.contentItem
//                    onTextEdited: heightSpinBox.valueUpdated(heightSpinBox.valueFromText(heightSpinBox.contentItem.text, heightSpinBox.locale))
//                }

                function valueUpdated(value) {
                    if (preserveAspectRatioButton.checked) {
                        var aspectRatio = project.size.width / project.size.height
                        widthSpinBox.value = value * aspectRatio
                    }
                }
            }

            Label {
                text: qsTr("Smooth")
                Layout.row: 2
                Layout.column: 1
            }

            CheckBox {
                id: smoothCheckBox
                objectName: "smoothCheckBox"

                Layout.row: 2
                Layout.column: 2

                ToolTip.text: qsTr("Resize smoothly using bilinear filtering")
                ToolTip.visible: hovered
            }

            Item {
                Layout.row: 2
                Layout.rowSpan: 2
                Layout.column: 0
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }

        RowLayout {
            Button {
                objectName: "imageSizePopupOkButton"
                text: qsTr("OK")

                onClicked: {
                    project.resize(widthSpinBox.value, heightSpinBox.value, smoothCheckBox.checked);
                    dialog.visible = false;
                }
            }
            Button {
                objectName: "imageSizePopupCancelButton"
                text: qsTr("Cancel")

                onClicked: dialog.visible = false
            }
        }
    }

    // https://bugreports.qt.io/browse/QTBUG-64174
//    footer: DialogButtonBox {
//        Button {
//            objectName: "imageSizePopupOkButton"
//            text: qsTr("OK")

//            DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole

//            onClicked: {
//                project.resize(widthSpinBox.value, heightSpinBox.value, smoothCheckBox.checked);
//                dialog.visible = false;
//            }
//        }
//        Button {
//            objectName: "imageSizePopupCancelButton"
//            text: qsTr("Cancel")

//            DialogButtonBox.buttonRole: DialogButtonBox.DestructiveRole

//            onClicked: dialog.visible = false
//        }
//    }
}
