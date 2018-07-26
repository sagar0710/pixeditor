import QtQuick 2.11

import App 1.0

// For access to CanvasColours Singleton
import "."

ImageCanvas {
    id: imageCanvas
    objectName: "imageCanvas"
    focus: true
    project: args.project
    backgroundColour: CanvasColours.backgroundColour
    checkerColour1: settings.checkerColour1
    checkerColour2: settings.checkerColour2
    gridVisible: settings.gridVisible
    gridColour: "#55000000"
    rulersVisible: settings.rulersVisible
    guidesVisible: settings.guidesVisible
    guidesLocked: settings.guidesLocked
    splitColour: CanvasColours.splitColour
    splitter.width: 32
    scrollZoom: settings.scrollZoom
    anchors.fill: parent

    readonly property int currentPaneZoomLevel: imageCanvas.currentPane ? imageCanvas.currentPane.integerZoomLevel : 1
    readonly property point currentPaneOffset: imageCanvas.currentPane ? imageCanvas.currentPane.offset : Qt.point(0, 0)
    readonly property bool useCrosshairCursor: imageCanvas.tool === TileCanvas.SelectionTool
        || (imageCanvas.toolSize < 4 && imageCanvas.currentPaneZoomLevel <= 3)
    readonly property bool useIconCursor: imageCanvas.tool === TileCanvas.EyeDropperTool

    onErrorOccurred: errorPopup.showError(errorMessage)

    CanvasPaneItem {
        id: secondPane
        canvas: imageCanvas
        pane: imageCanvas.secondPane
        paneIndex: 1
        anchors.fill: parent
        visible: imageCanvas.splitScreen

        Rectangle {
            anchors.fill: parent
            color: CanvasColours.backgroundColour
            z: -1
        }
    }

    CanvasPaneItem {
        id: firstPane
        canvas: imageCanvas
        pane: imageCanvas.firstPane
        paneIndex: 0
        anchors.fill: parent

        Rectangle {
            anchors.fill: parent
            color: CanvasColours.backgroundColour
            z: -1
        }
    }

    Rectangle {
        x: imageCanvas.firstPane.size * parent.width
        width: 1
        height: parent.height
        color: CanvasColours.splitColour
    }
}
