import QtQuick 2.6
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0

import App 1.0

Popup {
    id: popup
    objectName: "canvasSizePopup"
    modal: true
    dim: false
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnReleaseOutside

    property Project project

    onVisibleChanged: {
        if (visible) {
            widthSpinBox.value = project.tilesWide;
            heightSpinBox.value = project.tilesHigh;
        }
    }

    contentItem: ColumnLayout {
        Label {
            id: titleLabel
            text: qsTr("Choose a size for the canvas")
        }

        Item {
            Layout.preferredHeight: 4
        }

        GridLayout {
            columns: 2

            Label {
                text: qsTr("Canvas width")
            }

            SpinBox {
                id: widthSpinBox
                objectName: "changeCanvasWidthSpinBox"
                from: 1
                to: 1000
                stepSize: 1

                ToolTip.text: qsTr("Number of horizontal tiles")
            }
            Label {
                text: qsTr("Canvas height")
            }

            SpinBox {
                id: heightSpinBox
                objectName: "changeCanvasHeightSpinBox"
                from: 1
                to: 1000
                stepSize: 1

                ToolTip.text: qsTr("Number of vertical tiles")
            }

            Item {
                Layout.minimumHeight: 10
                Layout.columnSpan: 2
            }

            RowLayout {
                Layout.columnSpan: 2

                Item {
                    Layout.fillWidth: true
                }

                Button {
                    objectName: "canvasSizePopupOkButton"
                    text: "OK"
                    onClicked: {
                        project.size = Qt.size(widthSpinBox.value, heightSpinBox.value);
                        popup.visible = false;
                    }
                }
                Button {
                    objectName: "canvasSizePopupCancelButton"
                    text: "Cancel"
                    onClicked: popup.visible = false
                }
            }
        }
    }
}
