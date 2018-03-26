/*
    Copyright 2018, Mitch Curtis

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

import QtQuick 2.10

Canvas {
    width: 4
    height: 4

    property string color

    onColorChanged: requestPaint()

    onPaint: {
        var ctx = getContext("2d")
        ctx.reset()

        ctx.moveTo(width, 0)
        ctx.lineTo(width, height)
        ctx.lineTo(0, height)
        ctx.closePath()
        ctx.fillStyle = color
        ctx.fill()
    }
}
