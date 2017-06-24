import App 1.0

// For access to CanvasColours Singleton
import "."

ImageCanvas {
    id: imageCanvas
    objectName: "imageCanvas"
    focus: true
    // Items in Loaders are deleteLater()'d, so we have to make sure
    // that we're still the active item before setting a project that might
    // now be something different than what we're designed for.
    project: loader.item === imageCanvas ? canvasContainer.project : null
    backgroundColour: CanvasColours.backgroundColour
    gridVisible: settings.gridVisible
    gridColour: "#55000000"
    splitScreen: settings.splitScreen
    splitColour: CanvasColours.splitColour
    splitter.enabled: settings.splitScreen && !settings.splitterLocked
    splitter.width: 32
    scrollZoom: settings.scrollZoom
    anchors.fill: parent

    readonly property int currentPaneZoomLevel: imageCanvas.currentPane ? imageCanvas.currentPane.zoomLevel : 1
    readonly property point currentPaneOffset: imageCanvas.currentPane ? imageCanvas.currentPane.offset : Qt.point(0, 0)
    readonly property bool useCrosshairCursor: imageCanvas.tool === TileCanvas.SelectionTool
        || (imageCanvas.toolSize < 4 && imageCanvas.currentPaneZoomLevel <= 3)
    readonly property bool useIconCursor: imageCanvas.tool === TileCanvas.EyeDropperTool

    onErrorOccurred: errorPopup.showError(errorMessage)
}
