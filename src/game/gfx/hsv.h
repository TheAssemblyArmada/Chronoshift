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
    HSVClass(uint8_t hue = 0, uint8_t saturation = 0, uint8_t value = 0) : m_hue(hue), m_sat(saturation), m_val(value) {}
    HSVClass(HSVClass const &that) : m_hue(that.m_hue), m_sat(that.m_sat), m_val(that.m_val) {}

    operator RGBClass() const;
    bool operator==(HSVClass const &that) const { return m_hue == that.m_hue && m_sat == that.m_sat && m_val == that.m_val; }
    bool operator!=(HSVClass const &that) const { return this != &that; }
    HSVClass &operator=(HSVClass const &that);

    void Reset() { m_hue = 0; m_sat = 0; m_val = 0; }
    void Adjust(int adjust, HSVClass const &that);
    void Adjust(fixed_t brightness, fixed_t saturation, fixed_t tint, fixed_t contrast);
    int const Difference(HSVClass const &that) const;
    void const Set(uint8_t index) const;

    uint8_t Get_Hue() { return m_hue; }
    uint8_t Get_Saturation() { return m_sat; }
    uint8_t Get_Value() { return m_val; }
    void Set_Hue(uint8_t hue) { m_hue = hue; }
    void Set_Saturation(uint8_t sat) { m_sat = sat; }
    void Set_Value(uint8_t val) { m_val = val; }

private:
    uint8_t m_hue; // The hue angle component. [range 0 to 255] (def = 0)
    uint8_t m_sat; // The saturation component. [range 0 to 255] (def = 0) 0 = white, 255 = pure color.
    uint8_t m_val; // The value component.[range 0 to 255](def = 0) 0 = black, 255 = full.

public:
    static HSVClass const s_blackColor;
    static HSVClass const s_whiteColor;
};

inline HSVClass &HSVClass::operator=(HSVClass const &that)
{
    if (this != &that) {
        m_hue = that.m_hue;
        m_sat = that.m_sat;
        m_val = that.m_val;
    }

    return *this;
}

#endif // HSV_H
