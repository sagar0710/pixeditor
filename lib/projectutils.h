/*
    Copyright 2021, Mitch Curtis

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

#ifndef PROJECTUTILS_H
#define PROJECTUTILS_H

#include <QVector>

#include "guide.h"
#include "slate-global.h"

class Project;

namespace ProjectUtils {
    SLATE_EXPORT void addGuidesForSpacing(const Project *project, QVector<Guide> &guides, int horizontalSpacing, int verticalSpacing);
    SLATE_EXPORT QVector<Guide> uniqueGuides(const Project *project, const QVector<Guide> &guidesToAdd);
}

#endif // PROJECTUTILS_H
