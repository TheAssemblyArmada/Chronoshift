/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class representing a rectangle.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef TRECT_H
#define TRECT_H

#include "always.h"

template<typename T>
class TRect
{
public:
    TRect(void) : Left(), Top(), Right(), Bottom(){};
    TRect(const TRect &that) : Left(that.Left), Top(that.Top), Right(that.Right), Bottom(that.Bottom) {}
    TRect(T left, T top, T right, T bottom) : Left(left), Top(top), Right(right), Bottom(bottom) {}

    int const Is_Valid(void) const { return Right > 0 && Bottom > 0; }

    T Get_Left(void) const { return Left; }
    T Get_Top(void) const { return Top; }
    T Get_Right(void) const { return Right; }
    T Get_Bottom(void) const { return Bottom; }

    void Set_Left(T left) { Left = left; }
    void Set_Top(T top) { Top = top; }
    void Set_Right(T right) { Right = right; }
    void Set_Bottom(T bottom) { Bottom = bottom; }
    T Get_Width(void) const { return Right - Left; }
    T Get_Height(void) const { return Bottom - Top; }
    TRect &operator*=(T k) { return Scale(k); }
    TRect &operator/=(T k) { return Scale(1 / k); }
    TRect &Scale(T k)
    {
        Left *= k;
        Top *= k;
        Right *= k;
        Bottom *= k;
        return *this;
    }
    TRect &operator+=(const TRect<T> &that)
    {
        Left = MIN(Left, that.Left);
        Top = MIN(Top, that.Top);
        Right = MAX(Right, that.Right);
        Bottom = MAX(Bottom, that.Bottom);
        return *this;
    }

    // Equality
    bool operator==(const TRect<T> &that) const
    {
        return (that.Left == Left) && (that.Right == Right) && (that.Top == Top) && (that.Bottom == Bottom);
    }
    bool operator!=(const TRect<T> &that) const
    {
        return (that.Left != Left) || (that.Right != Right) || (that.Top != Top) || (that.Bottom != Bottom);
    }

    // Assign
    TRect &operator=(const TRect &that)
    {
        Left = that.Left;
        Top = that.Top;
        Right = that.Right;
        Bottom = that.Bottom;
    }

    bool Contains(const TRect &that) const
    {
        return that.Left >= Left && that.Right <= Right && that.Top >= Top && that.Bottom <= Bottom;
    }
    TRect const Intersect(const TRect &that, T *xposthing = NULL, T *yposthing = NULL) const;
    TRect const Union(TRect &that) const;
    void Add_Pos(TRect &that)
    {
        Left += that->Left;
        Top += that->Top;
    }

    bool Intersects(const TRect &rectangle) const;
    bool Intersects(const TRect &rectangle, TRect &intersection) const;

    char *To_String() const;

public:
    T Left; // Left coordinate of the rectangle.
    T Top; // Top coordinate of the rectangle.
    T Right; // Width of the rectangle.
    T Bottom; // Height of the rectangle.
};

template<typename T>
TRect<T> const TRect<T>::Union(TRect<T> &that) const
{
    if (Is_Valid()) {
        if (that.Is_Valid()) {
            int left = Left;
            int top = Top;
            int right = Right;
            int bottom = Bottom;

            if (Left > that.Left) {
                right += left - that.Left;
                left = that.Left;
            }
            if (top > that.Top) {
                bottom += top - that.Top;
                top = that.Top;
            }

            int rightside = that.Right + that.Left;

            if (right + left < rightside) {
                right = rightside - left + 1;
            }

            int bottomside = that.Bottom + that.Top;

            if (bottom + top < bottomside) {
                bottom = bottomside - top + 1;
            }

            return TRect(left, top, right, bottom);
        } else {
            return *this;
        }
    } else {
        return that;
    }
    return TRect();
}

/*
TRect Intersect(const TRect &that) const
{
    int x1 = MAX(Left, that.Left);
    int x2 = MIN(Right, that.Right);
    int y1 = MAX(Top, that.Top);
    int y2 = MIN(Bottom, that.Bottom);
    return TRect(x1, y1, MAX(0, x2 - x1 + 1), MAX(0, y2 - y1 + 1));
}
*/

template<typename T>
TRect<T> const TRect<T>::Intersect(const TRect<T> &that, T *xposthing, T *yposthing) const
{
    T left = that.Left;
    T top = that.Top;
    T right = that.Right;
    T bottom = that.Bottom;

    // Do we have 2 valid rects?
    if (!Is_Valid() || !that.Is_Valid()) {
        return TRect();
    }

    if (left < Left) {
        right += left - Left;
        left = Left;
    }

    if (right < 1) {
        return TRect();
    }

    if (top < Top) {
        bottom += top - Top;
        top = Top;
    }

    if (bottom < 1 || (left + right <= Left + Right ? right : (right = Right + Left - left), right < 1)) {
        return TRect();
    }

    if (bottom + top > Bottom + Top) {
        bottom = Bottom + Top - top;
    }

    if (bottom < 1) {
        return TRect();
    }

    // If these int or T pointers have been passed in, increment by difference
    // in position
    if (xposthing) {
        *xposthing += that.Left - left;
    }

    if (yposthing) {
        *yposthing += that.Top - top;
    }

    // Returned rect should reflect area that this rect and passed rect intersect
    return TRect(left, top, right, bottom);
}

#endif // TRECT_H
