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
#pragma once

#ifndef PAL_H
#define PAL_H

#include "always.h"

#ifdef GAME_DLL
extern uint8_t *g_CurrentPalette;
#else
extern uint8_t g_CurrentPalette[];
#endif

void Set_Palette(void *pal);

#endif // PAL_H
