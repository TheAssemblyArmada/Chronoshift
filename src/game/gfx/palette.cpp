/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class for handling palette data for 8bit indexed image formats.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "palette.h"
#include "pal.h"
#include "ttimer.h"
#include <cstring>

using std::memcpy;
using std::memcmp;

#ifndef GAME_DLL
PaletteClass g_GamePalette;
PaletteClass g_OriginalPalette;
PaletteClass g_CCPalette;
PaletteClass g_BlackPalette(RGBClass::BlackColor);
PaletteClass g_WhitePalette(RGBClass::WhiteColor);
#endif

PaletteClass &PaletteClass::CurrentPalette = *reinterpret_cast<PaletteClass *>(g_CurrentPalette);

PaletteClass::PaletteClass(const RGBClass &rgb)
{
    for (int index = 0; index < ARRAY_SIZE(PaletteClass::m_Palette); ++index) {
        m_Palette[index].Set_Red(rgb.Get_Red());
        m_Palette[index].Set_Green(rgb.Get_Green());
        m_Palette[index].Set_Blue(rgb.Get_Blue());
    }
}

bool PaletteClass::operator==(PaletteClass &pal) const
{
    if (this != &pal) {
        return memcmp(m_Palette, pal.m_Palette, sizeof(m_Palette)) == 0;
    }

    return true;
}

PaletteClass &PaletteClass::operator=(PaletteClass &pal)
{
    if (this != &pal) {
        memcpy(m_Palette, pal.m_Palette, sizeof(m_Palette));
    }

    return *this;
}

void PaletteClass::Adjust(int adj)
{
    for (int index = 0; index < ARRAY_SIZE(PaletteClass::m_Palette); ++index) {
        m_Palette[index].Adjust(adj, RGBClass::BlackColor);
    }
}

void PaletteClass::Adjust(int adj, PaletteClass &pal)
{
    for (int index = 0; index < ARRAY_SIZE(PaletteClass::m_Palette); ++index) {
        m_Palette[index].Adjust(adj, (pal.m_Palette[index]));
    }
}

void PaletteClass::Partial_Adjust(int adj, char *data)
{
    for (int index = 0; index < ARRAY_SIZE(PaletteClass::m_Palette); ++index) {
        if (data[index]) {
            m_Palette[index].Adjust(adj, RGBClass::BlackColor);
        }
    }
}

void PaletteClass::Partial_Adjust(int adj, PaletteClass &pal, char *data)
{
    for (int index = 0; index < ARRAY_SIZE(PaletteClass::m_Palette); ++index) {
        if (data[index]) {
            m_Palette[index].Adjust(adj, pal.m_Palette[index]);
        }
    }
}

int PaletteClass::Closest_Color(RGBClass &rgb) const
{
    int difference;
    int index = 0;
    int prevdiff = -1;

    for (int i = 0; i < ARRAY_SIZE(PaletteClass::m_Palette); ++i) {
        difference = rgb.Difference(m_Palette[i]);

        if (prevdiff == -1 || difference < prevdiff) {
            prevdiff = difference;
            index = i;
        }
    }

    return index;
}

void PaletteClass::Set(int fading_steps, void (*callback)())
{
    PaletteClass tmppal;
    PaletteClass adjpal;
    TCountDownTimerClass<SystemTimerClass> timer;

    timer = fading_steps;
    tmppal = CurrentPalette;
    
    while (timer.Time() > 0) {
        adjpal = tmppal;
        int remaining = timer.Time();
        adjpal.Adjust(((fading_steps - remaining) * ARRAY_SIZE(PaletteClass::m_Palette)) / fading_steps, *this);
        remaining = timer.Time();
        Set_Palette((uint8_t *)adjpal.m_Palette);

        int i = timer.Time();
        while (i == remaining && remaining > 0) {
            if (callback != nullptr) {
                callback();
            }

            i = timer.Time();
        }
    }

    Set_Palette((uint8_t *)m_Palette);
}
