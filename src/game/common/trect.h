/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class representing a rectangle.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
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
#include "minmax.h"

template<typename T>
class TRect
{
public:
    TRect() {};
    TRect(const TRect &that) : m_left(that.m_left), m_top(that.m_top), m_right(that.m_right), m_bottom(that.m_bottom) {}
    TRect(T left, T top, T right, T bottom) : m_left(left), m_top(top), m_right(right), m_bottom(bottom) {}

    int const Is_Valid() const { return m_right > 0 && m_bottom > 0; }

    T Get_Left() const { return m_left; }
    T Get_Top() const { return m_top; }
    T Get_Right() const { return m_right; }
    T Get_Bottom() const { return m_bottom; }

    void Set_Left(T left) { m_left = left; }
    void Set_Top(T top) { m_top = top; }
    void Set_Right(T right) { m_right = right; }
    void Set_Bottom(T bottom) { m_bottom = bottom; }
    T Get_Width() const { return m_right - m_left; }
    T Get_Height() const { return m_bottom - m_top; }
    TRect &operator*=(T k) { return Scale(k); }
    TRect &operator/=(T k) { return Scale(1 / k); }

    TRect &Scale(T k)
    {
        m_left *= k;
        m_top *= k;
        m_right *= k;
        m_bottom *= k;
        return *this;
    }

    TRect &operator+=(const TRect<T> &that)
    {
        m_left = Min(m_left, that.m_left);
        m_top = Min(m_top, that.m_top);
        m_right = Max(m_right, that.m_right);
        m_bottom = Max(m_bottom, that.m_bottom);
        return *this;
    }

    // Equality
    bool operator==(const TRect<T> &that) const
    {
        return (that.m_left == m_left) && (that.m_right == m_right) && (that.m_top == m_top) && (that.m_bottom == m_bottom);
    }

    bool operator!=(const TRect<T> &that) const
    {
        return (that.m_left != m_left) || (that.m_right != m_right) || (that.m_top != m_top) || (that.m_bottom != m_bottom);
    }

    // Assign
    TRect &operator=(const TRect &that)
    {
        if (this != &that) {
            m_left = that.m_left;
            m_top = that.m_top;
            m_right = that.m_right;
            m_bottom = that.m_bottom;
        }

        return *this;
    }

    bool Contains(const TRect &that) const
    {
        return that.m_left >= m_left && that.m_right <= m_right && that.m_top >= m_top && that.m_bottom <= m_bottom;
    }

    TRect const Intersect(const TRect &that, T *xposthing = NULL, T *yposthing = NULL) const;
    TRect const Union(TRect &that) const;

    void Add_Pos(TRect &that)
    {
        m_left += that->m_left;
        m_top += that->m_top;
    }

    bool Intersects(const TRect &rectangle) const;
    bool Intersects(const TRect &rectangle, TRect &intersection) const;

    char *To_String() const;

public:
    T m_left; // Left coordinate of the rectangle.
    T m_top; // Top coordinate of the rectangle.
    T m_right; // Width of the rectangle.
    T m_bottom; // Height of the rectangle.

};

template<typename T>
TRect<T> const TRect<T>::Union(TRect<T> &that) const
{
    if (Is_Valid()) {
        if (that.Is_Valid()) {
            int left = m_left;
            int top = m_top;
            int right = m_right;
            int bottom = m_bottom;

            if (m_left > that.m_left) {
                right += left - that.m_left;
                left = that.m_left;
            }
            if (top > that.m_top) {
                bottom += top - that.m_top;
                top = that.m_top;
            }

            int rightside = that.m_right + that.m_left;

            if (right + left < rightside) {
                right = rightside - left + 1;
            }

            int bottomside = that.m_bottom + that.m_top;

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
    int x1 = Max(m_left, that.m_left);
    int x2 = Min(m_right, that.m_right);
    int y1 = Max(m_top, that.m_top);
    int y2 = Min(m_bottom, that.m_bottom);
    return TRect(x1, y1, Max(0, x2 - x1 + 1), Max(0, y2 - y1 + 1));
}
*/

template<typename T>
TRect<T> const TRect<T>::Intersect(const TRect<T> &that, T *xposthing, T *yposthing) const
{
    T left = that.m_left;
    T top = that.m_top;
    T right = that.m_right;
    T bottom = that.m_bottom;

    // Do we have 2 valid rects?
    if (!Is_Valid() || !that.Is_Valid()) {
        return TRect();
    }

    if (left < m_left) {
        right += left - m_left;
        left = m_left;
    }

    if (right < 1) {
        return TRect();
    }

    if (top < m_top) {
        bottom += top - m_top;
        top = m_top;
    }

    if (bottom < 1 || (left + right <= m_left + m_right ? right : (right = m_right + m_left - left), right < 1)) {
        return TRect();
    }

    if (bottom + top > m_bottom + m_top) {
        bottom = m_bottom + m_top - top;
    }

    if (bottom < 1) {
        return TRect();
    }

    // If these int or T pointers have been passed in, increment by difference
    // in position
    if (xposthing) {
        *xposthing += that.m_left - left;
    }

    if (yposthing) {
        *yposthing += that.m_top - top;
    }

    // Returned rect should reflect area that this rect and passed rect intersect
    return TRect(left, top, right, bottom);
}

#endif // TRECT_H
