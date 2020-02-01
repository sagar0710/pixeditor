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

#ifndef DUPLICATELAYERCOMMAND_H
#define DUPLICATELAYERCOMMAND_H

#include <QDebug>
#include <QImage>
#include <QScopedPointer>
#include <QUndoCommand>

#include "slate-global.h"

class ImageLayer;
class LayeredImageProject;

class SLATE_EXPORT DuplicateLayerCommand : public QUndoCommand
{
public:
    DuplicateLayerCommand(LayeredImageProject *project,
        int layerIndex, ImageLayer *layer, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;

private:
    friend QDebug operator<<(QDebug debug, const DuplicateLayerCommand *command);

    LayeredImageProject *mProject;
    int mLayerIndex;
    ImageLayer *mLayer;
    QScopedPointer<ImageLayer> mLayerGuard;
    int mPreviousCurrentLayerIndex;
};

#endif // DUPLICATELAYERCOMMAND_H
