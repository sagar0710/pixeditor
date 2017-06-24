import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
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

    Keys.onEscapePressed: contextMenu.cancelCurrentAction()

    ListView {
        id: listView
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
            text: layerName
            checkable: true
            checked: project.currentLayerIndex === index
            width: listView.width
            onClicked: project.currentLayerIndex = index

            Rectangle {
                id: focusRect
                width: 2
                height: parent.height
                color: Ui.CanvasColours.focusColour
                visible: parent.checked
            }
        }
    }

    footer: RowLayout {
        Button {
            objectName: "newLayerButton"
            text: qsTr("+")
            flat: true
            focusPolicy: Qt.NoFocus
            hoverEnabled: true

            Layout.maximumWidth: implicitHeight
            Layout.fillWidth: true
            Layout.leftMargin: 6

            onClicked: project.addNewLayer()
        }

        Item {
            Layout.fillWidth: true
        }

        Button {
            objectName: "deleteLayerButton"
            text: qsTr("\uf1f8")
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

//    Menu {
//        id: contextMenu
//        objectName: "layerPanelContextMenu"
//        x: rightClickedTile ? rightClickedTileScenePos.x : 0
//        y: rightClickedTile ? rightClickedTileScenePos.y : 0
//        parent: root
//        modal: true
//        dim: false

//        // TODO: add these back when they're re-added
////        allowHorizontalFlip: true
////        allowVerticalFlip: true

//        property string currentAction
//        property var rightClickedTile
//        property point rightClickedTileScenePos: rightClickedTile ? tilesetImage.mapToItem
//            (root, rightClickedTile.sourceRect.x, rightClickedTile.sourceRect.y + project.tileHeight) : Qt.point(0, 0)

//        // There isn't a current context action, so either the user cancelled
//        // whatever they were going to do by closing the menu, or the menu item
//        // has an immediate effect.
//        onClosed: if (!currentAction) rightClickedTile = null

//        function cancelCurrentAction() {
//            contextMenu.rightClickedTile = null;
//            contextMenu.currentAction = "";
//        }

//        MenuItem {
//            id: duplicateMenuItem
//            objectName: "duplicateTileMenuButton"
//            text: qsTr("Duplicate")
//            onClicked: {
//                contextMenu.currentAction = text;
//                cursorIcon.text = "\uf112";
//            }
//        }

//        MenuItem {
//            objectName: "rotateTileLeftMenuButton"
//            text: qsTr("Rotate 90° Left")
//            onClicked: {
//                project.rotateTileCounterClockwise(contextMenu.rightClickedTile);
//                contextMenu.rightClickedTile = null;
//            }
//        }

//        MenuItem {
//            objectName: "rotateTileRightMenuButton"
//            text: qsTr("Rotate 90° Right")
//            onClicked: {
//                project.rotateTileClockwise(contextMenu.rightClickedTile);
//                contextMenu.rightClickedTile = null;
//            }
//        }
//    }
}
