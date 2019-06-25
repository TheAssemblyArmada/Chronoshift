/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Fixed point math class.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef FIXED_H
#define FIXED_H

#include "always.h"
#include "endiantype.h"

class fixed_t
{
public:
    friend int operator*(const int &a, const fixed_t &b);
    friend int operator*(const fixed_t &a, const int &b);
    friend bool operator>(const fixed_t &a, const int &b);
    friend bool operator>(const int &a, const fixed_t &b);
    friend bool operator>=(const fixed_t &a, const int &b);
    friend bool operator>=(const int &a, const fixed_t &b);
    friend bool operator==(const fixed_t &a, const int &b);
    friend bool operator==(const int &a, const fixed_t &b);
    friend bool operator!=(const fixed_t &a, const int &b);
    friend bool operator!=(const int &a, const fixed_t &b);

    fixed_t() {}
    explicit fixed_t(uint8_t byte) { m_number = byte; }

    fixed_t(int a, int b = 1)
    {
        if (b) {
            m_number = (a << 8) / b;
        } else {
            m_number = 0;
        }
    }

    fixed_t(const char *string);
    fixed_t(const fixed_t &that) : m_number(that.m_number) {}
    ~fixed_t() {}

    //operator int() const;
    int To_Int() const;
    fixed_t &operator=(const int &num);
    fixed_t &operator=(const fixed_t &that);
    fixed_t operator+(const fixed_t &that) const;
    fixed_t operator-(const fixed_t &that) const;
    fixed_t operator/(const fixed_t &that) const;
    fixed_t operator*(const fixed_t &that) const;
    fixed_t &operator+=(const fixed_t &that) { return *this = *this + that; }
    fixed_t &operator-=(const fixed_t &that) { return *this = *this - that; }
    fixed_t &operator*=(const fixed_t &that) { return *this = *this * that; }
    fixed_t &operator/=(const fixed_t &that) { return *this = *this / that; }
    bool operator==(const fixed_t &that) const { return m_number == that.m_number; }
    bool operator!=(const fixed_t &that) const { return m_number != that.m_number; }
    bool operator<(const fixed_t &that) const { return m_number < that.m_number; }
    bool operator>(const fixed_t &that) const { return that < *this; }
    bool operator<=(const fixed_t &that) const { return !(*this > that); }
    bool operator>=(const fixed_t &that) const { return !(*this < that); }

    int To_ASCII(char *string, int size) const;
    char *const As_ASCII() const;
    int Round_To_Int() const { return (m_number + 128) >> 8; }
    int Chop_To_Int() const { return m_number >> 8; }
    int Get_Word() const { return m_number; }
    void Set_Word(uint16_t word) { m_number = word; }

    static const fixed_t _0_1; // 0.0    0
    static const fixed_t _1_1; // 1.0    256
    static const fixed_t _1_2; // 0.5
    static const fixed_t _1_3; // 0.33 ish
    static const fixed_t _1_4; // 0.25
    static const fixed_t _3_4; // 0.75
    static const fixed_t _2_3; // 0.66 ish

private:
    int16_t m_number;
};

//inline fixed_t::operator int() const
inline int fixed_t::To_Int() const
{
    int16_t tmp;
    tmp = m_number + 128;
    return tmp >> 8;
}

inline fixed_t &fixed_t::operator=(const int &num)
{
    m_number = num << 8;

    return *this;
}

inline fixed_t &fixed_t::operator=(const fixed_t &that)
{
    m_number = that.m_number;

    return *this;
}

inline fixed_t fixed_t::operator+(const fixed_t &that) const
{
    fixed_t tmp;
    tmp.m_number = m_number + that.m_number;

    return tmp;
}

inline fixed_t fixed_t::operator-(const fixed_t &that) const
{
    fixed_t tmp;
    tmp.m_number = m_number - that.m_number;

    return tmp;
}

inline fixed_t fixed_t::operator/(const fixed_t &that) const
{
    fixed_t tmp = *this;

    if (that != 0 && that != 1) {
        tmp.m_number = ((int)m_number << 8) / that.m_number;
    }

    return tmp;
}

inline fixed_t fixed_t::operator*(const fixed_t &that) const
{
    // Multiplication can overflow, largest whole value multi supported
    // by 8:8 fixed point is around <16.0 * <16.0
    fixed_t tmp;
    tmp.m_number = ((int)m_number * that.m_number) / 256;

    return tmp;
}

inline int operator*(const int &a, const fixed_t &b)
{
    return (unsigned)(a * b.m_number + 128) / 256;
}

inline int operator*(const fixed_t &a, const int &b)
{
    return (unsigned)(b * a.m_number + 128) / 256;
}

inline bool operator>(const fixed_t &a, const int &b)
{
    return a.m_number > (b << 8);
}

inline bool operator>(const int &a, const fixed_t &b)
{
    return (a << 8) > b.m_number;
}

inline bool operator>=(const fixed_t &a, const int &b)
{
    return a.m_number >= (b << 8);
}

inline bool operator>=(const int &a, const fixed_t &b)
{
    return (a << 8) >= b.m_number;
}

inline bool operator==(const fixed_t &a, const int &b)
{
    return a.m_number == (b << 8);
}

inline bool operator==(const int &a, const fixed_t &b)
{
    return (a << 8) == b.m_number;
}

inline bool operator!=(const fixed_t &a, const int &b)
{
    return a.m_number != (b << 8);
}

inline bool operator!=(const int &a, const fixed_t &b)
{
    return (a << 8) != b.m_number;
}

inline bool operator<(const fixed_t &a, const int &b)
{
    return b > a;
}

inline bool operator<(const int &a, const fixed_t &b)
{
    return b > a;
}

inline bool operator<=(const fixed_t &a, const int &b)
{
    return b >= a;
}

inline bool operator<=(const int &a, const fixed_t &b)
{
    return b >= a;
}

#endif // FIXED_H
