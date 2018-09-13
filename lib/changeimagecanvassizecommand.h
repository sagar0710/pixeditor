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

#ifndef CHANGEIMAGECANVASSIZECOMMAND_H
#define CHANGEIMAGECANVASSIZECOMMAND_H

#include <QDebug>
#include <QImage>
#include <QUndoCommand>

#include "slate-global.h"

class ImageProject;

class SLATE_EXPORT ChangeImageCanvasSizeCommand : public QUndoCommand
{
public:
    ChangeImageCanvasSizeCommand(ImageProject *project, const QImage &previousImage, const QImage &newImage,
        QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;

private:
    friend QDebug operator<<(QDebug debug, const ChangeImageCanvasSizeCommand *command);

    ImageProject *mProject;
    QImage mPreviousImage;
    QImage mNewImage;
};

#endif // CHANGEIMAGECANVASSIZECOMMAND_H
