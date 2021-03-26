import qbs

Group {
    id: root
    // The qrc prefix to assign the files.
    files: [path + "/main.qml"]
    Qt.core.resourcePrefix: "/qml"
    Qt.core.resourceSourceBase: path
    fileTags: ["qt.qml.qml", "qt.core.resource_data"]

    Group {
        // The directory the files are in; saves writing it for each line.
        // The qrc prefix to assign the files.
        Qt.core.resourcePrefix: "/qml"
        Qt.core.resourceSourceBase: path
        fileTags: ["qt.qml.qml", "qt.core.resource_data"]
        files: [
            "ui/qmldir",
            "ui/AboutDialog.qml",
            "ui/AnimationDelegate.qml",
            "ui/AnimationPanel.qml",
            "ui/AnimationPreviewSettingsPopup.qml",
            "ui/AnimationSettingsPopup.qml",
            "ui/Theme.qml",
            "ui/CanvasContainer.qml",
            "ui/CanvasPaneRepeater.qml",
            "ui/CanvasSizePopup.qml",
            "ui/ColourButton.qml",
            "ui/ColourPanel.qml",
            "ui/ColourSelector.qml",
            "ui/CrosshairCursor.qml",
            "ui/DialogButton.qml",
            "ui/DoubleTextField.qml",
            "ui/ErrorLabel.qml",
            "ui/ErrorPopup.qml",
            "ui/FillToolMenu.qml",
            "ui/FpsCounter.qml",
            "ui/HexColourRowLayout.qml",
            "ui/HorizontalGradientRectangle.qml",
            "ui/HslSimplePicker.qml",
            "ui/HueGradient.qml",
            "ui/HueSaturationDialog.qml",
            "ui/HueSlider.qml",
            "ui/IconRectangle.qml",
            "ui/IconToolButton.qml",
            "ui/ImageSizePopup.qml",
            "ui/ImageTypeCanvas.qml",
            "ui/LayerDelegate.qml",
            "ui/LayerPanel.qml",
            "ui/LayeredImageTypeCanvas.qml",
            "ui/MenuBar.qml",
            "ui/MoveContentsDialog.qml",
            "ui/NewImageProjectPopup.qml",
            "ui/NewLayeredImageProjectPopup.qml",
            "ui/NewProjectPopup.qml",
            "ui/NewTilesetProjectPopup.qml",
            "ui/NoteContextMenu.qml",
            "ui/NoteDialog.qml",
            "ui/OpacityDialog.qml",
            "ui/OptionsDialog.qml",
            "ui/Panel.qml",
            "ui/ProjectTemplateButton.qml",
            "ui/RenameSwatchColourDialog.qml",
            "ui/RowActionButton.qml",
            "ui/SaturationLightnessPicker.qml",
            "ui/SaveChangesDialog.qml",
            "ui/ShortcutRow.qml",
            "ui/Shortcuts.qml",
            "ui/SplitterBar.qml",
            "ui/SpriteImageContainer.qml",
            "ui/StatusBar.qml",
            "ui/SwatchContextMenu.qml",
            "ui/SwatchFocusRectangle.qml",
            "ui/SwatchGridView.qml",
            "ui/SwatchPanel.qml",
            "ui/SwatchSettingsContextMenu.qml",
            "ui/TexturedFillSettingsDialog.qml",
            "ui/TexturedFillSwatchDelegate.qml",
            "ui/TexturedFillVarianceRangedSlider.qml",
            "ui/TilesetSwatchPanel.qml",
            "ui/TilesetTypeCanvas.qml",
            "ui/ToolBar.qml",
            "ui/ToolButton.qml",
            "ui/ToolButtonMenuIndicator.qml",
            "ui/ToolSizePopup.qml",
            "ui/UiStateSerialisation.qml",
            "ui/UiConstants.qml",
            "ui/VerticalSeparator.qml",
            "ui/ViewBorder.qml",
            "ui/ZoomIndicator.qml"
        ]

        Group {
            Qt.core.resourcePrefix: "/qml"
            Qt.core.resourceSourceBase: path
            fileTags: ["qt.qml.qml", "qt.core.resource_data"]
            files: [
                "ui/+Material/DialogButton.qml",
                "ui/+Material/IconRectangle.qml",
                "ui/+Material/SwatchFocusRectangle.qml",
                "ui/+Material/Theme.qml",
                "ui/+Imagine/Theme.qml",
                "ui/+nativemenubar/MenuBar.qml",
                "ui/+Universal/IconRectangle.qml",
                "ui/+Universal/SwatchFocusRectangle.qml",
                "ui/+Universal/Theme.qml",
                "ui/+Universal/VerticalSeparator.qml"
            ]
        }
    }
}
