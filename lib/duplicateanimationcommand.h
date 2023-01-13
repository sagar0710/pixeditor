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

#ifndef DUPLICATEANIMATIONCOMMAND_H
#define DUPLICATEANIMATIONCOMMAND_H

#include <memory>

#include <QDebug>
#include <QSize>
#include <QUndoCommand>

#include "slate-global.h"
#include "undocommand.h"

class Animation;
class AnimationSystem;
class ImageLayer;

class SLATE_EXPORT DuplicateAnimationCommand : public UndoCommand
{
public:
    DuplicateAnimationCommand(AnimationSystem *animationSystem, int targetIndex, int duplicateIndex, const QString &name,
        UndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;

private:
    friend QDebug operator<<(QDebug debug, const DuplicateAnimationCommand *command);

    AnimationSystem *mAnimationSystem = nullptr;
    // The index of the animation we want to duplicate.
    int mTargetIndex = -1;
    // The index we want the duplicate to have.
    int mDuplicateIndex = -1;
    std::unique_ptr<Animation> mAnimationGuard;
};


#endif // DUPLICATEANIMATIONCOMMAND_H
