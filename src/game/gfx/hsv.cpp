/**
 * @file
 *
 * @Author CCHyper, OmniBlade
 *
 * @brief Class for holding HSV color model data.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "hsv.h"
#include "abs.h"
#include "rgb.h"

HSVClass const HSVClass::s_blackColor(0, 0, 0);
HSVClass const HSVClass::s_whiteColor(0, 0, 100);

void HSVClass::Adjust(int adjust, HSVClass const &that)
{
    int tmp;

    tmp = adjust * (that.m_val - m_val);
    m_val += (tmp - (tmp >> 31 << 8)) >> 8;
    tmp = adjust * (that.m_sat - m_sat);
    m_sat += (tmp - (tmp >> 31 << 8)) >> 8;
    tmp = adjust * (that.m_hue - m_hue);
    m_hue += (tmp - (tmp >> 31 << 8)) >> 8;
}

int const HSVClass::Difference(HSVClass const &that) const
{
    int hue = m_hue - that.m_hue;
    int saturation = m_sat - that.m_sat;
    int value = m_val - that.m_val;

    hue = Abs(hue);
    saturation = Abs(saturation);
    value = Abs(value);

    return hue * hue + saturation * saturation + value * value;
}

HSVClass::operator RGBClass() const
{
    if (m_sat == 0) {
        // Achromatic (grey)
        return RGBClass(RGBClass::Contract_VGA(m_val), RGBClass::Contract_VGA(m_val), RGBClass::Contract_VGA(m_val));
    }

    int saturation = m_sat;
    int value = m_val;
    int hue = m_hue * 6; // scale to full range.
    int q = (hue / 255);
    int tmp[7];

    tmp[1] = value;
    tmp[2] = value;
    tmp[3] = value * (255 - hue % 255 * saturation / 255) / 255;
    tmp[4] = value * (255 - saturation) / 255;
    tmp[5] = tmp[4];
    tmp[6] = (255 - saturation * (255 - hue % 255) / 255) * value / 255;

    int red;
    int green;
    int blue;

    if (q > 4) {
        q = q - 4;
    } else {
        q = q + 2;
    }

    red = tmp[q];

    if (q > 4) {
        q = q - 4;
    } else {
        q = q + 2;
    }

    blue = tmp[q];

    if (q > 4) {
        q = q - 4;
    } else {
        q = q + 2;
    }

    green = tmp[q];

    return RGBClass(RGBClass::Contract_VGA(red), RGBClass::Contract_VGA(green), RGBClass::Contract_VGA(blue));
}

////////////////////////////////////////////////////////////////////////////////
//  NAME:
//    <function name>
//
//  DESCRIPTION:
//    <function overview>
//
//  PARAMETERS:
//    none
//
//  RETURN:
//    none
//
//  WARNINGS:
//    none
//
////////////////////////////////////////////////////////////////////////////////
void const HSVClass::Set(uint8_t index) const
{
    RGBClass rgb;

    rgb = *this; // should call operator RGBClass;
    rgb.Set(index);
}
