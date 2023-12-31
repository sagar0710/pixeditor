/*
    Copyright 2023, Mitch Curtis

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

#ifndef MOVELAYEREDIMAGECONTENTSCOMMAND_H
#define MOVELAYEREDIMAGECONTENTSCOMMAND_H

#include <QDebug>
#include <QImage>
#include <QVector>

#include "slate-global.h"
#include "undocommand.h"

class LayeredImageProject;

class SLATE_EXPORT MoveLayeredImageContentsCommand : public UndoCommand
{
public:
    MoveLayeredImageContentsCommand(LayeredImageProject *project, const QVector<QImage> &previousImages,
        const QVector<QImage> &newImages, UndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;

    bool modifiesContents() const override;

private:
    friend QDebug operator<<(QDebug debug, const MoveLayeredImageContentsCommand *command);

    LayeredImageProject *mProject;
    QVector<QImage> mPreviousImages;
    QVector<QImage> mNewImages;
};

#endif // MOVELAYEREDIMAGECONTENTSCOMMAND_H
