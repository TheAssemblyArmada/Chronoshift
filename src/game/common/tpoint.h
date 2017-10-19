/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Wrapper class for platform drawing engine.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef TPOINT_H
#define TPOINT_H

#include "always.h"

template<typename T>
class TPoint2D
{
public:
    TPoint2D() {}
    TPoint2D(T x, T y) : x(x), y(y) {}
    TPoint2D(TPoint2D const &that) : x(that.x), y(that.y) {}

    TPoint2D &operator=(TPoint2D const &that)
    {
        if (this != &that) {
            x = that.x;
            y = that.y;
        }

        return *this;
    }

    bool operator==(TPoint2D const &that) const { return x == that.x && y == that.y; }
    bool operator!=(TPoint2D const &that) const { return x != that.x && y != that.y; }
    TPoint2D operator-(TPoint2D const &that) const { return TPoint2D(x - that.x, y - that.y); }
    TPoint2D operator+(TPoint2D const &that) const { return TPoint2D(x + that.x, y + that.y); }
    TPoint2D &operator-=(TPoint2D const &that) const { return TPoint2D(x - that.x, y - that.y); }
    TPoint2D &operator+=(TPoint2D const &that) const { return TPoint2D(x + that.x, y + that.y); }

    bool Is_Valid() const { return x > 0 || y > 0; }

    T x;
    T y;
};

#endif // TPOINT_H
