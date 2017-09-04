/**
 * @file
 *
 * @Author CCHyper, OmniBlade
 *
 * @brief Class for holding RGB color model data.
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

#ifndef RGB_H
#define RGB_H

#include "always.h"

#define RGB_MIN 0
#define RGB_MAX 63 // palette is clipped to 6 bits, so highest is 63

class HSVClass;

class RGBClass
{
public:
    RGBClass(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0) : m_red(red), m_grn(green), m_blu(blue) {}
    RGBClass(RGBClass const &that) : m_red(that.m_red), m_grn(that.m_grn), m_blu(that.m_blu) {}

    bool operator==(RGBClass const &that) const;
    bool operator!=(RGBClass const &that) const { return !(*this == that); }
    RGBClass &operator=(RGBClass const &that);
    operator HSVClass();

    uint8_t Get_Red() const { return m_red; }
    uint8_t Get_Green() const { return m_grn; }
    uint8_t Get_Blue() const { return m_blu; }
    void Set_Red(uint8_t red) { m_red = red; }
    void Set_Green(uint8_t green) { m_grn = green; }
    void Set_Blue(uint8_t blue) { m_blu = blue; }

    void Adjust(int adjustment, const RGBClass &that);
    int const Difference(RGBClass const &that) const;
    RGBClass Average(RGBClass const &that) const;
    void Set(int index);

    static uint8_t Expand_VGA(uint8_t value) { return (value >> 6) | (value << 2); }
    static uint8_t Contract_VGA(uint8_t value) { return value >> 2; }

public:
    static RGBClass const BlackColor;
    static RGBClass const WhiteColor;

private:
    uint8_t m_red;
    uint8_t m_grn;
    uint8_t m_blu;
};

inline RGBClass &RGBClass::operator=(RGBClass const &that)
{
    if (this != &that) {
        m_red = that.m_red;
        m_grn = that.m_grn;
        m_blu = that.m_blu;
    }

    return *this;
}

inline bool RGBClass::operator==(RGBClass const &that) const
{
    return (m_red == that.m_red && m_grn == that.m_grn && m_grn == that.m_grn);
}

#endif // RGB_H
