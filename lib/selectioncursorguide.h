/*
    Copyright 2020, Mitch Curtis

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

#ifndef SELECTIONCURSORGUIDE_H
#define SELECTIONCURSORGUIDE_H

#include <QQuickPaintedItem>

#include "slate-global.h"

class ImageCanvas;
class CanvasPane;

class SLATE_EXPORT SelectionCursorGuide : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(ImageCanvas *canvas READ canvas WRITE setCanvas NOTIFY canvasChanged)
    Q_PROPERTY(CanvasPane *pane READ pane WRITE setPane NOTIFY paneChanged)
    Q_PROPERTY(int paneIndex READ paneIndex WRITE setPaneIndex NOTIFY paneIndexChanged)
    Q_MOC_INCLUDE("canvaspane.h")
    Q_MOC_INCLUDE("imagecanvas.h")

public:
    explicit SelectionCursorGuide(QQuickItem *parent = nullptr);
    ~SelectionCursorGuide() override;

    ImageCanvas *canvas() const;
    void setCanvas(ImageCanvas *newCanvas);

    CanvasPane *pane() const;
    void setPane(CanvasPane *newPane);

    int paneIndex() const;
    void setPaneIndex(int paneIndex);

    void paint(QPainter *painter) override;

signals:
    void canvasChanged();
    void paneChanged();
    void paneIndexChanged();

private:
    ImageCanvas *mCanvas = nullptr;
    CanvasPane *mPane = nullptr;
    int mPaneIndex = -1;
};

#endif
