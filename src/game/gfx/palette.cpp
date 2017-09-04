/**
 * @file
 *
 * @Author CCHyper, OmniBlade
 *
 * @brief Class for handling palette data for 8bit indexed image formats.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "palette.h"
#include "pal.h"

PaletteClass WhitePalette(RGBClass::WhiteColor);
PaletteClass BlackPalette(RGBClass::BlackColor);

PaletteClass &PaletteClass::CurrentPalette = *reinterpret_cast<PaletteClass *>(g_currentPalette);

PaletteClass::PaletteClass(RGBClass const &rgb)
{
    for (int index = 0; index < ARRAY_SIZE(PaletteClass::m_palette); ++index) {
        m_palette[index].Set_Red(rgb.Get_Red());
        m_palette[index].Set_Green(rgb.Get_Green());
        m_palette[index].Set_Blue(rgb.Get_Blue());
    }
}

bool PaletteClass::operator==(PaletteClass &pal) const
{
    if (&m_palette != &pal.m_palette) {
        return memcmp(m_palette, &pal, sizeof(PaletteClass)) == 0;
    }

    return false;
}

PaletteClass &PaletteClass::operator=(PaletteClass &pal)
{
    if (this != &pal) {
        memcpy(m_palette, &pal.m_palette, sizeof(PaletteClass));
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
#ifndef RAPP_STANDALONE
    static void (*hooked_set)(PaletteClass *, int, void (*)()) =
        reinterpret_cast<void (*)(PaletteClass *, int, void (*)())>(0x005BCF44);
    hooked_set(this, fading_steps, callback);
#endif
    // TODO requires timer classes and low level palette set function.
    /*
    PaletteClass tmppal;
    PaletteClass adjpal;
    TCountDownTimerClass<SystemTimerClass> timer;

    timer = fading_steps;

    memcpy(&tmppal, &PaletteClass::CurrentPalette, sizeof(PaletteClass::m_palette));

    while (timer.Time() > 0) {
        memcpy(&adjpal, &tmppal, sizeof(PaletteClass::m_palette));
        int remaining = timer.Time();
        adjpal.Adjust(((fading_steps - remaining) * ARRAY_SIZE(PaletteClass::m_palette)) / fading_steps, *this);
        remaining = timer.Time();
        Set_Palette((uint8_t *)&adjpal.m_palette);

        int i = timer.Time();
        while (i == remaining && remaining > 0) {
            if (callback != nullptr) {
                callback();
            }

            i = timer.Time();
        }
    }

    Set_Palette((uint8_t *)m_palette);
    */
}
