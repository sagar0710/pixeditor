/*
    Copyright 2017, Mitch Curtis

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

#include "applypixellinecommand.h"

#include <QLoggingCategory>
#include <QPainter>
#include <QImage>

#include "commands.h"

Q_LOGGING_CATEGORY(lcApplyPixelLineCommand, "app.undo.applyPixelLineCommand")

// The undo command for lines needs the project image before and after
// the line was drawn on it.
ApplyPixelLineCommand::ApplyPixelLineCommand(ImageCanvas *canvas, int layerIndex, QImage &currentProjectImage, const QPointF point1, const QPointF point2,
                                             const QPointF &newLastPixelPenReleaseScenePos, const QPointF &oldLastPixelPenReleaseScenePos,
                                             const QPainter::CompositionMode mode, QUndoCommand *parent) :
    QUndoCommand(parent),
    mCanvas(canvas),
    mLayerIndex(layerIndex),
    mNewLastPixelPenReleaseScenePos(newLastPixelPenReleaseScenePos),
    mOldLastPixelPenReleaseScenePos(oldLastPixelPenReleaseScenePos),
    subImageDatas()
{
    const QRect lineRect = mCanvas->normalisedLineRect(point1, point2);
    const QList<ImageCanvas::SubImage> subImages = canvas->subImagesInBounds(lineRect);
    for (auto const &subImage : subImages) {
        const QRect subImageLineRect = lineRect.translated(-subImage.offset).intersected(subImage.bounds);
        SubImageData subImageData;
        subImageData.subImage = subImage;
        subImageData.lineRect = subImageLineRect;
        subImageData.imageWithoutLine = currentProjectImage.copy(subImageLineRect);

        QPainter painter(&currentProjectImage);
        painter.setClipRect(subImageLineRect);
        mCanvas->drawLine(&painter, point1 - subImage.offset, point2 - subImage.offset, mode);
        painter.end();

        subImageData.imageWithLine = currentProjectImage.copy(subImageLineRect);
        subImageDatas.append(subImageData);
    }

    qCDebug(lcApplyPixelLineCommand) << "constructed" << this;
}

ApplyPixelLineCommand::~ApplyPixelLineCommand()
{
    qCDebug(lcApplyPixelLineCommand) << "destructed" << this;
}

void ApplyPixelLineCommand::undo()
{
    qCDebug(lcApplyPixelLineCommand) << "undoing" << this;
    for (auto const &subImageData : subImageDatas) {
        mCanvas->applyPixelLineTool(mLayerIndex, subImageData.imageWithoutLine, subImageData.lineRect, mOldLastPixelPenReleaseScenePos);
    }
}

void ApplyPixelLineCommand::redo()
{
    qCDebug(lcApplyPixelLineCommand) << "redoing" << this;
    for (auto const &subImageData : subImageDatas) {
        mCanvas->applyPixelLineTool(mLayerIndex, subImageData.imageWithLine, subImageData.lineRect, mNewLastPixelPenReleaseScenePos);
    }
}

int ApplyPixelLineCommand::id() const
{
    return ApplyPixelLineCommandId;
}

bool ApplyPixelLineCommand::mergeWith(const QUndoCommand *)
{
    return false;
}

QDebug operator<<(QDebug debug, const ApplyPixelLineCommand *command)
{
    debug.nospace() << "(ApplyPixelLineCommand"
        << " layerIndex=" << command->mLayerIndex
//        << ", lineRect" << command->mLineRect
        << ", newLastPixelPenReleaseScenePos=" << command->mNewLastPixelPenReleaseScenePos
        << ", oldLastPixelPenReleaseScenePos=" << command->mOldLastPixelPenReleaseScenePos
        << ")";
    return debug.space();
}
