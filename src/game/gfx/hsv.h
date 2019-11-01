/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class for holding HSV color model data.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef HSV_H
#define HSV_H

#include "always.h"
#include "fixed.h"

class RGBClass;

class HSVClass
{
public:
    HSVClass(uint8_t hue = 0, uint8_t saturation = 0, uint8_t value = 0) : m_Hue(hue), m_Sat(saturation), m_Val(value) {}
    HSVClass(HSVClass const &that) : m_Hue(that.m_Hue), m_Sat(that.m_Sat), m_Val(that.m_Val) {}

    operator RGBClass() const;
    bool operator==(HSVClass const &that) const { return m_Hue == that.m_Hue && m_Sat == that.m_Sat && m_Val == that.m_Val; }
    bool operator!=(HSVClass const &that) const { return this != &that; }
    HSVClass &operator=(HSVClass const &that);

    void Reset() { m_Hue = 0; m_Sat = 0; m_Val = 0; }
    void Adjust(int adjust, HSVClass const &that);
    void Adjust(fixed_t brightness, fixed_t saturation, fixed_t tint, fixed_t contrast);
    int const Difference(HSVClass const &that) const;
    void const Set(uint8_t index) const;

    uint8_t Get_Hue() { return m_Hue; }
    uint8_t Get_Saturation() { return m_Sat; }
    uint8_t Get_Value() { return m_Val; }
    void Set_Hue(uint8_t hue) { m_Hue = hue; }
    void Set_Saturation(uint8_t sat) { m_Sat = sat; }
    void Set_Value(uint8_t val) { m_Val = val; }

private:
    uint8_t m_Hue; // The hue angle component. [range 0 to 255] (def = 0)
    uint8_t m_Sat; // The saturation component. [range 0 to 255] (def = 0) 0 = white, 255 = pure color.
    uint8_t m_Val; // The value component.[range 0 to 255](def = 0) 0 = black, 255 = full.

public:
    static HSVClass const s_blackColor;
    static HSVClass const s_whiteColor;
};

inline HSVClass &HSVClass::operator=(HSVClass const &that)
{
    if (this != &that) {
        m_Hue = that.m_Hue;
        m_Sat = that.m_Sat;
        m_Val = that.m_Val;
    }

    return *this;
}

#endif // HSV_H
