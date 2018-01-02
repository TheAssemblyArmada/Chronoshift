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
#pragma once

#ifndef PALETTE_H
#define PALETTE_H

#include "always.h"
#include "fileclass.h"
#include "rgb.h"

class PaletteClass
{
public:
    enum
    {
        PALETTE_ENTRIES = 256,
    };

public:
    PaletteClass(const RGBClass &rgb = RGBClass::BlackColor);

    RGBClass &operator[](int index) { return m_palette[index]; }
    const RGBClass &operator[](int index) const { return m_palette[index]; }

    bool operator==(PaletteClass &pal) const;
    PaletteClass &operator=(PaletteClass &pal);

    void Adjust(int adj);
    void Adjust(int adj, PaletteClass &pal);

    void Partial_Adjust(int adj, char *data);
    void Partial_Adjust(int adj, PaletteClass &pal, char *data);

    int Closest_Color(RGBClass &rgb) const;

    void Set(int fading_steps = 0, void (*callback)() = nullptr);

    static PaletteClass &CurrentPalette;

private:
    RGBClass m_palette[PALETTE_ENTRIES];
};

extern PaletteClass WhitePalette;
extern PaletteClass BlackPalette;

#ifndef RAPP_STANDALONE
#include "hooker.h"

extern PaletteClass &GamePalette;
extern PaletteClass &OriginalPalette;
extern PaletteClass &CCPalette;
#else
extern PaletteClass GamePalette;
extern PaletteClass OriginalPalette;
extern PaletteClass CCPalette;
#endif

#endif // PALETTE_H
