/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Functions for building the color lookup tables to simulate transparency.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef FADING_H
#define FADING_H

#include "always.h"
#include "palette.h"

struct TLucentType
{
    uint8_t PaletteIndex;
    uint8_t FadeColor;
    uint8_t FadeFraction;
    uint8_t __UnknownValue; // Probably padding as orignal was compiled with alignment 1
};

inline int Translucent_Table_Size(int table_count)
{
    return (table_count << 8) + 256;
}

void *Make_Fading_Table(const PaletteClass &palette, void *fade_table, int color, int frac);
void *__cdecl Build_Fading_Table(const PaletteClass &palette, void *fade_table, int color, int frac);
void *Conquer_Build_Fading_Table(const PaletteClass &palette, void *fade_table, int color, int frac);
void *Build_Translucent_Table(PaletteClass &palette, const TLucentType *type, int tab_count, void *table);
void *Conquer_Build_Translucent_Table(PaletteClass &palette, const TLucentType *type, int tab_count, void *table);

#ifndef RAPP_STANDALONE
#include "hooker.h"

namespace Fading {

inline void Hook_Me()
{
    Hook_Function(0x004FB914, Make_Fading_Table);
    Hook_Function(0x005CC4C0, Build_Fading_Table);
    Hook_Function(0x004FB994, Conquer_Build_Fading_Table);
    Hook_Function(0x004FB7C4, Build_Translucent_Table);
    Hook_Function(0x004FB870, Conquer_Build_Translucent_Table);
}

}
#endif

#endif // FADING_H
