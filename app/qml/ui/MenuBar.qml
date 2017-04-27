import Qt.labs.platform 1.0 as Platform

import App 1.0

Platform.MenuBar {
    property ImageCanvas canvas
    property ProjectManager projectManager
    property Project project: projectManager.project

    Platform.Menu {
        id: fileMenu
        objectName: "fileMenu"
        title: qsTr("File")

        Platform.MenuItem {
            objectName: "newMenuButton"
            text: qsTr("New")
            onTriggered: doIfChangesDiscarded(function() { newProjectPopup.open() })
        }

        Platform.MenuItem {
            objectName: "openMenuButton"
            text: qsTr("Open")
            onTriggered: doIfChangesDiscarded(function() { openProjectDialog.open() }, true)
        }

        Platform.MenuItem {
            objectName: "saveMenuButton"
            text: qsTr("Save")
            enabled: project ? project.canSave : false
            onTriggered: projectManager.saveOrSaveAs()
        }

        Platform.MenuItem {
            objectName: "saveAsMenuButton"
            text: qsTr("Save As")
            enabled: project ? project.loaded : false
            onTriggered: saveAsDialog.open()
        }

        Platform.MenuItem {
            objectName: "closeMenuButton"
            text: qsTr("Close")
            enabled: project ? project.loaded : false
            onTriggered: doIfChangesDiscarded(function() { project.close() })
        }

        Platform.MenuItem {
            objectName: "revertMenuButton"
            text: qsTr("Revert")
            enabled: project ? project.loaded && project.unsavedChanges : false
            onTriggered: project.revert()
        }
    }

    Platform.Menu {
        id: editMenu
        objectName: "editMenu"
        title: qsTr("Edit")

        Platform.MenuItem {
            objectName: "undoMenuButton"
            text: qsTr("Undo")
            onTriggered: project.undoStack.undo()
            enabled: project ? project.undoStack.canUndo : false
        }

        Platform.MenuItem {
            objectName: "redoMenuButton"
            text: qsTr("Redo")
            onTriggered: project.undoStack.redo()
            enabled: project ? project.undoStack.canRedo : false
        }
    }

    Platform.Menu {
        id: viewMenu
        objectName: "viewMenu"
        title: qsTr("View")

        Platform.MenuItem {
            objectName: "centreMenuButton"
            text: qsTr("Centre")
            enabled: canvas
            onTriggered: canvas.centreView()
        }

        Platform.MenuSeparator {}

        Platform.MenuItem {
            objectName: "showGridMenuButton"
            text: qsTr("Show Grid")
            enabled: canvas
            checkable: true
            checked: settings.gridVisible
            onTriggered: settings.gridVisible = checked
        }

        Platform.MenuItem {
            objectName: "splitScreenMenuButton"
            text: qsTr("Split Screen")
            enabled: canvas
            checkable: true
            checked: settings.splitScreen
            onTriggered: settings.splitScreen = checked
        }

        Platform.MenuItem {
            objectName: "splitterLockedMenuButton"
            text: qsTr("Lock Splitter")
            enabled: canvas && settings.splitScreen
            checkable: true
            checked: settings.splitterLocked
            onTriggered: settings.splitterLocked = checked
        }
    }

    Platform.Menu {
        id: settingsMenu
        title: qsTr("Tools")

        Platform.MenuItem {
            objectName: "settingsMenuButton"
            text: qsTr("Options")
            onTriggered: optionsDialog.open()
        }
    }
}
