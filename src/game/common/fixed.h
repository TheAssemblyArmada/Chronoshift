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

class fixed
{
public:
    friend int operator*(const int &a, const fixed &b);
    friend int operator*(const fixed &a, const int &b);
    friend bool operator>(const fixed &a, const int &b);
    friend bool operator>(const int &a, const fixed &b);
    friend bool operator>=(const fixed &a, const int &b);
    friend bool operator>=(const int &a, const fixed &b);
    friend bool operator==(const fixed &a, const int &b);
    friend bool operator==(const int &a, const fixed &b);
    friend bool operator!=(const fixed &a, const int &b);
    friend bool operator!=(const int &a, const fixed &b);

    struct Data
    {
        struct Composite
        {
#if defined(SYSTEM_BIG_ENDIAN)
            uint8_t hi;
            uint8_t low;
#elif defined(SYSTEM_LITTLE_ENDIAN)
            uint8_t low;
            uint8_t hi;
#endif //
        };

        // members
        union
        {
            uint16_t word;
            Composite ch;
        };
    };

    fixed() {}
    explicit fixed(uint8_t byte) { m_number.word = byte; }
    fixed(int a, int b = 1);
    fixed(const char *string);
    fixed(const fixed &that) : m_number(that.m_number) {}
    ~fixed() {}

    //operator int() const;
    int To_Int() const;
    fixed &operator=(const int &num);
    fixed &operator=(const fixed &that);
    fixed operator+(const fixed &that) const;
    fixed operator-(const fixed &that) const;
    fixed operator/(const fixed &that) const;
    fixed operator*(const fixed &that) const;
    fixed &operator+=(const fixed &that) { return *this = *this + that; }
    fixed &operator-=(const fixed &that) { return *this = *this - that; }
    fixed &operator*=(const fixed &that) { return *this = *this * that; }
    fixed &operator/=(const fixed &that) { return *this = *this / that; }
    bool operator==(const fixed &that) const { return m_number.word == that.m_number.word; }
    bool operator!=(const fixed &that) const { return m_number.word != that.m_number.word; }
    bool operator<(const fixed &that) const { return m_number.word < that.m_number.word; }
    bool operator>(const fixed &that) const { return that < *this; }
    bool operator<=(const fixed &that) const { return !(*this > that); }
    bool operator>=(const fixed &that) const { return !(*this < that); }

    int To_ASCII(char *string, int size) const;
    char *const As_ASCII() const;
    int Round_To_Int() const { return (m_number.word + 128) >> 8; }
    int Chop_To_Int() const { return m_number.word >> 8; }
    int Get_Word() const { return m_number.word; }
    void Set_Word(uint16_t word) { m_number.word = word; }

    static const fixed _0_1; // 0.0	0
    static const fixed _1_1; // 1.0	256
    static const fixed _1_2; // 0.5
    static const fixed _1_3; // 0.33 ish
    static const fixed _1_4; // 0.25
    static const fixed _3_4; // 0.75
    static const fixed _2_3; // 0.66 ish

private:
    // This can be treated as a word or two chars
    Data m_number;
};

//inline fixed::operator int() const
inline int fixed::To_Int() const
{
    Data tmp;
    tmp.word = m_number.word + 128;
    return tmp.ch.hi;
}

inline fixed &fixed::operator=(const int &num)
{
    m_number.ch.low = 0;
    m_number.ch.hi = num;

    return *this;
}

inline fixed &fixed::operator=(const fixed &that)
{
    m_number.word = that.m_number.word;

    return *this;
}

inline fixed fixed::operator+(const fixed &that) const
{
    fixed tmp;
    tmp.m_number.word = m_number.word + that.m_number.word;

    return tmp;
}

inline fixed fixed::operator-(const fixed &that) const
{
    fixed tmp;
    tmp.m_number.word = m_number.word - that.m_number.word;

    return tmp;
}

inline fixed fixed::operator/(const fixed &that) const
{
    fixed tmp = *this;

    if (that != 0 && that != 1) {
        tmp.m_number.word = ((int)m_number.word << 8) / that.m_number.word;
    }

    return tmp;
}

inline fixed fixed::operator*(const fixed &that) const
{
    // Multiplication can overflow, largest whole value multi supported
    // by 8:8 fixed point is around <16.0 * <16.0
    fixed tmp;
    tmp.m_number.word = ((int)m_number.word * that.m_number.word) >> 8;

    return tmp;
}

inline int operator*(const int &a, const fixed &b)
{
    return (unsigned)(a * b.m_number.word + 128) / 256;
}

inline int operator*(const fixed &a, const int &b)
{
    return (unsigned)(b * a.m_number.word + 128) / 256;
}

inline bool operator>(const fixed &a, const int &b)
{
    return a.m_number.word > (b << 8);
}

inline bool operator>(const int &a, const fixed &b)
{
    return (a << 8) > b.m_number.word;
}

inline bool operator>=(const fixed &a, const int &b)
{
    return a.m_number.word >= (b << 8);
}

inline bool operator>=(const int &a, const fixed &b)
{
    return (a << 8) >= b.m_number.word;
}

inline bool operator==(const fixed &a, const int &b)
{
    return a.m_number.word == (b << 8);
}

inline bool operator==(const int &a, const fixed &b)
{
    return (a << 8) == b.m_number.word;
}

inline bool operator!=(const fixed &a, const int &b)
{
    return a.m_number.word != (b << 8);
}

inline bool operator!=(const int &a, const fixed &b)
{
    return (a << 8) != b.m_number.word;
}

inline bool operator<(const fixed &a, const int &b)
{
    return b > a;
}

inline bool operator<(const int &a, const fixed &b)
{
    return b > a;
}

inline bool operator<=(const fixed &a, const int &b)
{
    return b >= a;
}

inline bool operator<=(const int &a, const fixed &b)
{
    return b >= a;
}

#endif // FIXED_H
