import QtQuick 2.6

import App 1.0

Item {
    property var window
    property Project project
    property TileCanvas canvas

    Shortcut {
        sequence: settings.quitShortcut
        onActivated: Qt.quit()
    }

    Shortcut {
        id: s
        sequence: settings.newShortcut
        onActivated: window.newProjectPopup.open()
        enabled: !canvas.modalPopupsOpen
    }

    Shortcut {
        sequence: settings.openShortcut
        onActivated: window.openProjectDialog.open()
        enabled: !canvas.modalPopupsOpen
    }

    Shortcut {
        sequence: settings.saveShortcut
        onActivated: project.saveOrSaveAs()
        enabled: !canvas.modalPopupsOpen && project.canSave
    }

    Shortcut {
        sequence: settings.closeShortcut
        onActivated: project.close()
        enabled: !canvas.modalPopupsOpen
    }

    Shortcut {
        sequence: settings.undoShortcut
        onActivated: project.undoStack.undo()
        enabled: !canvas.modalPopupsOpen
    }

    Shortcut {
        sequence: settings.redoShortcut
        onActivated: project.undoStack.redo()
        enabled: !canvas.modalPopupsOpen
    }

    Shortcut {
        sequence: settings.gridVisibleShortcut
        onActivated: settings.gridVisible = !settings.gridVisible
    }

    Shortcut {
        sequence: settings.splitScreenShortcut
        onActivated: settings.splitScreen = !settings.splitScreen
    }

    Shortcut {
        sequence: settings.penToolShortcut
        onActivated: canvas.tool = TileCanvas.PenTool
    }

    Shortcut {
        sequence: settings.eyeDropperToolShortcut
        onActivated: canvas.tool = TileCanvas.EyeDropperTool
    }

    Shortcut {
        sequence: settings.eraserToolShortcut
        onActivated: canvas.tool = TileCanvas.EraserTool
    }

    Shortcut {
        sequence: settings.toolModeShortcut
        onActivated: canvas.mode = (canvas.mode === TileCanvas.TileMode ? TileCanvas.PixelMode : TileCanvas.TileMode)
    }

    Shortcut {
        sequence: settings.decreaseToolSizeShortcut
        onActivated: --canvas.toolSize
    }

    Shortcut {
        sequence: settings.increaseToolSizeShortcut
        onActivated: ++canvas.toolSize
    }

    Shortcut {
        sequence: settings.swatchLeftShortcut
        onActivated: canvas.swatchLeft()
    }

    Shortcut {
        sequence: settings.swatchRightShortcut
        onActivated: canvas.swatchRight()
    }

    Shortcut {
        sequence: settings.swatchUpShortcut
        onActivated: canvas.swatchUp()
    }

    Shortcut {
        sequence: settings.swatchDownShortcut
        onActivated: canvas.swatchDown()
    }
}
