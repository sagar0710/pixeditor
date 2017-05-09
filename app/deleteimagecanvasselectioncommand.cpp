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

#include "deleteimagecanvasselectioncommand.h"

#include "imagecanvas.h"
#include "imageproject.h"

Q_LOGGING_CATEGORY(lcDeleteImageCanvasSelectionCommand, "app.undo.deleteImageCanvasSelectionCommand")

DeleteImageCanvasSelectionCommand::DeleteImageCanvasSelectionCommand(ImageCanvas *canvas, const QRect &area, UndoCommand *parent) :
    UndoCommand(parent),
    mCanvas(canvas),
    mDeletedArea(area),
    mDeletedAreaImagePortion(canvas->mImageProject->image()->copy(area))
{
}

void DeleteImageCanvasSelectionCommand::undo()
{
    qCDebug(lcDeleteImageCanvasSelectionCommand) << "undoing" << this;
    mCanvas->paintImageOntoPortionOfImage(mDeletedArea, mDeletedAreaImagePortion);
}

void DeleteImageCanvasSelectionCommand::redo()
{
    qCDebug(lcDeleteImageCanvasSelectionCommand) << "redoing" << this;
    mCanvas->erasePortionOfImage(mDeletedArea);
    // This matches what mspaint does; deleting a selection also causes the selection to be cleared.
    mCanvas->clearSelection();
}

int DeleteImageCanvasSelectionCommand::id() const
{
    return -1;
}

QDebug operator<<(QDebug debug, const DeleteImageCanvasSelectionCommand &command)
{
    debug.nospace() << "(DeleteImageCanvasSelectionCommand area=" << command.mDeletedArea
        << ")";
    return debug.space();
}
