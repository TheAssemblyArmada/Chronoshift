/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class for holding RGB color model data.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "rgb.h"
#include "gamedebug.h"
#include "hsv.h"
#include "palette.h"
#include <algorithm>

RGBClass const RGBClass::BlackColor(RGB_MIN, RGB_MIN, RGB_MIN);
RGBClass const RGBClass::WhiteColor(RGB_MAX, RGB_MAX, RGB_MAX);

void RGBClass::Adjust(int adjustment, const RGBClass &that)
{
    int tmp;

    tmp = adjustment * (that.m_red - m_red);
    m_red += (((tmp & 0xFF000000) >> 24) + tmp) >> 8;

    tmp = adjustment * (that.m_grn - m_grn);
    m_grn += (((tmp & 0xFF000000) >> 24) + tmp) >> 8;

    tmp = adjustment * (that.m_blu - m_blu);
    m_blu += (((tmp & 0xFF000000) >> 24) + tmp) >> 8;
}

int const RGBClass::Difference(RGBClass const &that) const
{
    int red = m_red - that.m_red;
    int green = m_grn - that.m_grn;
    int blue = m_grn - that.m_blu;

    red = std::abs(red);
    green = std::abs(green);
    blue = std::abs(blue);

    return blue * blue + green * green + red * red;
}

RGBClass RGBClass::Average(RGBClass const &that) const
{
    int red = m_red + that.m_red;
    int green = m_grn + that.m_grn;
    int blue = m_blu + that.m_blu;

    RGBClass ret((red / 2), (green / 2), (blue / 2));

    return ret;
}

void RGBClass::Set(int index)
{
    // DOS I/O stuff, for updating the screen palette directly
    //__outbyte(0x3C8u, index);
    //__outbyte(0x3C9u, m_red);
    //__outbyte(0x3C9u, m_grn);
    //__outbyte(0x3C9u, m_blu);

    PaletteClass::CurrentPalette[index] = *this;
}

RGBClass::operator HSVClass()
{
    int red = Expand_VGA(m_red);
    int green = Expand_VGA(m_grn);
    int blue = Expand_VGA(m_blu);

    int saturation = 0;
    unsigned hue = 0;
    int value = std::max(blue, std::max(green, red));

    int min_value = std::min(blue, std::min(green, red));

    int delta = value - min_value;

    if (value) {
        saturation = 255 * (delta) / value;
    }

    if (saturation) {
        unsigned finalhue;

        if (value == red) {
            if (min_value == green) {
                finalhue = (255 * (value - blue) / (value - min_value)) + 1280;
            } else {
                finalhue = 256 - (255 * (value - green) / (value - min_value));
            }
        } else if (value == green) {
            if (min_value == blue) {
                finalhue = (255 * (value - red) / (value - min_value)) + 256;
            } else {
                finalhue = 768 - (255 * (value - blue) / (value - min_value));
            }
        } else if (min_value == red) {
            finalhue = (255 * (value - green) / (value - min_value)) + 768;
        } else {
            finalhue = 1280 - (255 * (value - red) / (value - min_value));
        }

        hue = finalhue / 6;
    }

    return HSVClass(hue, saturation, value);
}
