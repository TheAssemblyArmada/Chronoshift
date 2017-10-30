/**
 * @file
 *
 * @Author CCHyper, OmniBlade
 *
 * @brief Fixed point math class.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *
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
    friend int operator*(int const &a, fixed const &b);
    friend int operator*(fixed const &a, int const &b);
    friend bool operator>(fixed const &a, int const &b);
    friend bool operator>(int const &a, fixed const &b);

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
    fixed(char const *string);
    fixed(fixed const &that) : m_number(that.m_number) {}
    ~fixed() {}

    //operator int() const;
    int To_Int() const;
    fixed &operator=(int const &num);
    fixed &operator=(fixed const &that);
    fixed operator+(fixed const &that) const;
    fixed operator-(fixed const &that) const;
    fixed operator/(fixed const &that) const;
    fixed operator*(fixed const &that) const;
    fixed &operator+=(fixed const &that) { return *this = *this + that; }
    fixed &operator-=(fixed const &that) { return *this = *this - that; }
    fixed &operator*=(fixed const &that) { return *this = *this * that; }
    fixed &operator/=(fixed const &that) { return *this = *this / that; }
    bool operator==(fixed const &that) const { return m_number.word == that.m_number.word; }
    bool operator!=(fixed const &that) const { return m_number.word != that.m_number.word; }
    bool operator<(fixed const &that) const { return m_number.word < that.m_number.word; }
    bool operator>(fixed const &that) const { return that < *this; }
    bool operator<=(fixed const &that) const { return !(*this > that); }
    bool operator>=(fixed const &that) const { return !(*this < that); }

    int const To_ASCII(char *string, int size) const;
    char *const As_ASCII() const;
    int Round_To_Int() const { return (m_number.word + 128) >> 8; }
    int Chop_To_Int() const { return m_number.word >> 8; }
    int Get_Word() const { return m_number.word; }

    static fixed const _0_1; // 0.0	0
    static fixed const _1_1; // 1.0	256
    static fixed const _1_2; // 0.5
    static fixed const _1_3; // 0.33 ish
    static fixed const _1_4; // 0.25
    static fixed const _3_4; // 0.75
    static fixed const _2_3; // 0.66 ish

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

inline fixed &fixed::operator=(int const &num)
{
    m_number.ch.low = 0;
    m_number.ch.hi = num;

    return *this;
}

inline fixed &fixed::operator=(fixed const &that)
{
    m_number.word = that.m_number.word;

    return *this;
}

inline fixed fixed::operator+(fixed const &that) const
{
    fixed tmp;
    tmp.m_number.word = m_number.word + that.m_number.word;

    return tmp;
}

inline fixed fixed::operator-(fixed const &that) const
{
    fixed tmp;
    tmp.m_number.word = m_number.word - that.m_number.word;

    return tmp;
}

inline fixed fixed::operator/(fixed const &that) const
{
    fixed tmp;
    tmp.m_number.word = ((int)m_number.word << 8) / that.m_number.word;

    return tmp;
}

inline fixed fixed::operator*(fixed const &that) const
{
    // Multiplication can overflow, largest whole value multi supported
    // by 8:8 fixed point is around <16.0 * <16.0
    fixed tmp;
    tmp.m_number.word = ((int)m_number.word * that.m_number.word) >> 8;

    return tmp;
}

inline int operator*(int const &a, fixed const &b)
{
    return (unsigned)(a * b.m_number.word + 128) / 256;
}

inline int operator*(fixed const &a, int const &b)
{
    return (unsigned)(b * a.m_number.word + 128) / 256;
}

inline bool operator>(fixed const &a, int const &b)
{
    return a.m_number.word > (b << 8);
}

inline bool operator>(int const &a, fixed const &b)
{
    return (a << 8) > b.m_number.word;
}

inline bool operator<(fixed const &a, int const &b)
{
    return b > a;
}

inline bool operator<(int const &a, fixed const &b)
{
    return b > a;
}

#endif // FIXED_H
