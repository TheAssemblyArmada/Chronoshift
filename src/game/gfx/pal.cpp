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
#include <cstring>

using std::memcpy;

#ifdef GAME_DLL
uint8_t *const g_currentPalette = reinterpret_cast<uint8_t *>(0x0060CE90);
#else
uint8_t g_currentPalette[768];
#endif

void Set_Palette(void *pal)
{
    memcpy(g_currentPalette, pal, 768);

#ifdef GAME_DLL
    static void (*hook_set_dd_pal)(void *) = reinterpret_cast<void(*)(void *)>(0x005CA070);
    hook_set_dd_pal(pal);
#else
    // TODO implement low level palette set.
#endif
}