/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Low level palette handling.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "pal.h"
#include "initvideo.h"
#include <cstring>

using std::memcpy;

#ifndef GAME_DLL
uint8_t g_currentPalette[768];
#endif

void Set_Palette(void *pal)
{
    memcpy(g_currentPalette, pal, 768);
    Set_Video_Palette(pal);
}
