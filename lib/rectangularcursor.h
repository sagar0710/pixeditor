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

#ifndef RECTANGULARCURSOR_H
#define RECTANGULARCURSOR_H

#include <QQuickPaintedItem>

#include "slate-global.h"

class SLATE_EXPORT RectangularCursor : public QQuickPaintedItem
{
    Q_OBJECT

public:
    RectangularCursor();

    void paint(QPainter *painter) override;
};

#endif // RECTANGULARCURSOR_H
