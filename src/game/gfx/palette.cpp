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

#ifdef GAME_DLL
PaletteClass &GamePalette = Make_Global<PaletteClass>(0x00669C5C);
PaletteClass &OriginalPalette = Make_Global<PaletteClass>(0x0066A55C);
PaletteClass &CCPalette = Make_Global<PaletteClass>(0x0066995C);
PaletteClass &BlackPalette = Make_Global<PaletteClass>(0x00668F5C);
PaletteClass &WhitePalette = Make_Global<PaletteClass>(0x0066925C);
#else
PaletteClass GamePalette;
PaletteClass OriginalPalette;
PaletteClass CCPalette;
PaletteClass BlackPalette(RGBClass::BlackColor);
PaletteClass WhitePalette(RGBClass::WhiteColor);
#endif

PaletteClass &PaletteClass::CurrentPalette = *reinterpret_cast<PaletteClass *>(g_currentPalette);

PaletteClass::PaletteClass(const RGBClass &rgb)
{
    for (int index = 0; index < ARRAY_SIZE(PaletteClass::m_palette); ++index) {
        m_palette[index].Set_Red(rgb.Get_Red());
        m_palette[index].Set_Green(rgb.Get_Green());
        m_palette[index].Set_Blue(rgb.Get_Blue());
    }
}

bool PaletteClass::operator==(PaletteClass &pal) const
{
    if (this != &pal) {
        return memcmp(m_palette, pal.m_palette, sizeof(m_palette)) == 0;
    }

    return true;
}

PaletteClass &PaletteClass::operator=(PaletteClass &pal)
{
    if (this != &pal) {
        memcpy(m_palette, pal.m_palette, sizeof(m_palette));
    }

    return *this;
}

void PaletteClass::Adjust(int adj)
{
    for (int index = 0; index < ARRAY_SIZE(PaletteClass::m_palette); ++index) {
        m_palette[index].Adjust(adj, RGBClass::BlackColor);
    }
}

void PaletteClass::Adjust(int adj, PaletteClass &pal)
{
    for (int index = 0; index < ARRAY_SIZE(PaletteClass::m_palette); ++index) {
        m_palette[index].Adjust(adj, (pal.m_palette[index]));
    }
}

void PaletteClass::Partial_Adjust(int adj, char *data)
{
    for (int index = 0; index < ARRAY_SIZE(PaletteClass::m_palette); ++index) {
        if (data[index]) {
            m_palette[index].Adjust(adj, RGBClass::BlackColor);
        }
    }
}

void PaletteClass::Partial_Adjust(int adj, PaletteClass &pal, char *data)
{
    for (int index = 0; index < ARRAY_SIZE(PaletteClass::m_palette); ++index) {
        if (data[index]) {
            m_palette[index].Adjust(adj, pal.m_palette[index]);
        }
    }
}

int PaletteClass::Closest_Color(RGBClass &rgb) const
{
    int difference;
    int index;
    int prevdiff = -1;

    for (index = 0; index < ARRAY_SIZE(PaletteClass::m_palette); ++index) {
        difference = rgb.Difference(m_palette[index]);

        if (prevdiff == -1 || difference < prevdiff) {
            prevdiff = difference;
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
        adjpal.Adjust(((fading_steps - remaining) * ARRAY_SIZE(PaletteClass::m_palette)) / fading_steps, *this);
        remaining = timer.Time();
        Set_Palette((uint8_t *)adjpal.m_palette);

        int i = timer.Time();
        while (i == remaining && remaining > 0) {
            if (callback != nullptr) {
                callback();
            }

            i = timer.Time();
        }
    }

    Set_Palette((uint8_t *)m_palette);
}
