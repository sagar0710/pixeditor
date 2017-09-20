import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import App 1.0

RowLayout {
    id: zoomLevelIndicator

    property CanvasPane pane
    property FontMetrics fontMetrics

    TextMetrics {
        id: maxZoomTextMetrics
        text: pane ? pane.maxZoomLevel.toString() : "999"
    }

    Label {
        text: "\uf002"
        font.family: "FontAwesome"
        color: "#ffffff"
    }

    Label {
        text: "x"
        color: "#ffffff"
    }

    Label {
        id: zoomLevelText
        text: pane ? pane.integerZoomLevel : ""
        color: "#ffffff"

        Layout.minimumWidth: maxZoomTextMetrics.width
        Layout.maximumWidth: maxZoomTextMetrics.width
    }
}
