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

#include "applypixelerasercommand.h"

#include <QLoggingCategory>

#include "commands.h"

Q_LOGGING_CATEGORY(lcApplyPixelEraserCommand, "app.undo.applyPixelEraserCommand")

ApplyPixelEraserCommand::ApplyPixelEraserCommand(ImageCanvas *canvas, int layerIndex, const QVector<QPoint> &scenePositions,
    const QVector<QColor> &previousColours, QUndoCommand *parent) :
    QUndoCommand(parent),
    mCanvas(canvas),
    mLayerIndex(layerIndex)
{
    mScenePositions = scenePositions;
    mPreviousColours = previousColours;

    qCDebug(lcApplyPixelEraserCommand) << "constructed" << this;
}

void ApplyPixelEraserCommand::undo()
{
    qCDebug(lcApplyPixelEraserCommand) << "undoing" << this;
    for (int i = 0; i < mScenePositions.size(); ++i) {
        mCanvas->applyPixelPenTool(mLayerIndex, mScenePositions.at(i), mPreviousColours.at(i));
    }
}

void ApplyPixelEraserCommand::redo()
{
    qCDebug(lcApplyPixelEraserCommand) << "redoing" << this;
    for (int i = 0; i < mScenePositions.size(); ++i) {
        mCanvas->applyPixelPenTool(mLayerIndex, mScenePositions.at(i), QColor(Qt::transparent));
    }
}

int ApplyPixelEraserCommand::id() const
{
    return ApplyPixelEraserCommandId;
}

bool ApplyPixelEraserCommand::mergeWith(const QUndoCommand *other)
{
    const ApplyPixelEraserCommand *otherCommand = dynamic_cast<const ApplyPixelEraserCommand*>(other);
    if (!otherCommand) {
        return false;
    }

    // Duplicate pixel; we can just discard the other command.
    if (otherCommand->mScenePositions.size() == 1 && mScenePositions.contains(otherCommand->mScenePositions.first())) {
        return true;
    }

    // A unique pixel that we haven't touched yet; add it.
    qCDebug(lcApplyPixelEraserCommand) << "\nmerging:\n    " << otherCommand << "\nwith:\n    " << this;
    mScenePositions.append(otherCommand->mScenePositions);
    mPreviousColours.append(otherCommand->mPreviousColours);
    return true;
}

QDebug operator<<(QDebug debug, const ApplyPixelEraserCommand *command)
{
    debug.nospace() << "(ApplyPixelEraserCommand"
        << " layerIndex=" << command->mLayerIndex
        << ", scenePositions=" << command->mScenePositions
        << ", previousColours=" << command->mPreviousColours
        << ")";
    return debug.space();
}
