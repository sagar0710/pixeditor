/*
    Copyright 2016, Mitch Curtis

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

#ifndef TILECANVAS_H
#define TILECANVAS_H

#include <QObject>
#include <QImage>
#include <QQuickPaintedItem>
#include <QStack>
#include <QWheelEvent>

#include <QtUndo/undostack.h>

#include "imagecanvas.h"

class Project;
class Tile;
class Tileset;
class TilesetProject;

class TileCanvas : public ImageCanvas
{
    Q_OBJECT
    Q_PROPERTY(int cursorTilePixelX READ cursorTilePixelX NOTIFY cursorTilePixelXChanged)
    Q_PROPERTY(int cursorTilePixelY READ cursorTilePixelY NOTIFY cursorTilePixelYChanged)
    Q_PROPERTY(Mode mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(Tile *penTile READ penTile WRITE setPenTile NOTIFY penTileChanged)

public:
    enum Mode {
        PixelMode,
        TileMode
    };

    Q_ENUM(Mode)

    TileCanvas();
    ~TileCanvas();

    int cursorTilePixelX() const;
    void setCursorTilePixelX(int cursorTilePixelX);

    int cursorTilePixelY() const;
    void setCursorTilePixelY(int cursorTilePixelY);

    Mode mode() const;
    void setMode(const Mode &mode);

    Tile *penTile() const;
    void setPenTile(Tile *penTile);

    QPoint scenePosToTilePixelPos(const QPoint &scenePos) const;

    void paint(QPainter *painter) override;

signals:
    void cursorTilePixelXChanged();
    void cursorTilePixelYChanged();
    void modeChanged();
    void penTileChanged();

public slots:
//    void createNew(int width, int height, const QColor &penBackgroundColour);
//    void save(QUrl url = QUrl());
//    void saveAs(const QUrl &url);
    void swatchLeft();
    void swatchRight();
    void swatchUp();
    void swatchDown();
    void onTilesetChanged(Tileset *oldTileset, Tileset *newTileset);

protected slots:
    void reset() override;

protected:
    void connectSignals() override;
    void disconnectSignals() override;
    void toolChange() override;
    virtual bool supportsSelectionTool() const;

    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;

    void onLoadedChanged() override;

private:
    friend class ApplyTilePenCommand;
    friend class ApplyTileEraserCommand;
    friend class ApplyTileFillCommand;
    friend class ApplyTileCanvasPixelFillCommand;

    PixelCandidateData penEraserPixelCandidates(Tool tool) const override;
    PixelCandidateData fillPixelCandidates() const;
    PixelCandidateData greedyFillPixelCandidates() const;

    struct TileCandidateData
    {
        QVector<QPoint> tilePositions;
        int previousTile;
        int newTileId;

        TileCandidateData() : previousTile(-1), newTileId(-1) {}
    };
    TileCandidateData fillTileCandidates() const;

    void applyCurrentTool() override;
    void applyPixelPenTool(int layerIndex, const QPoint &scenePos, const QColor &colour, bool markAsLastRelease = false) override;
    void applyTilePenTool(const QPoint &tilePos, int id);

    void updateCursorPos(const QPoint &eventPos) override;
    void error(const QString &message);
    QColor penColour() const;
    void setHasBlankCursor(bool hasBlankCursor);
    void updateTilePenPreview();
    void setTilePenPreview(bool tilePenPreview);

    void drawPane(QPainter *painter, const CanvasPane &pane, int paneIndex);

    void hoverLeaveEvent(QHoverEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

    TilesetProject *mTilesetProject;

    // The position of the cursor in scene coordinates relative to the tile that it's in.
    int mCursorTilePixelX;
    int mCursorTilePixelY;
    Mode mMode;
    Tile *mPenTile;
    bool mTilePenPreview;
};

#endif // TILECANVAS_H
