/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Initialisation functions for graphics API.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef INITVIDEO_H
#define INITVIDEO_H

#include "always.h"

#ifdef BUILD_WITH_DDRAW
#define DIRECTDRAW_VERSION 0x300
#include <ddraw.h>
#endif

BOOL Set_Video_Mode(uintptr_t handle, int w, int h, int bpp);
void Reset_Video_Mode();
BOOL Init_Video();
void Set_Video_Palette(void *pal);

#ifdef GAME_DLL
#ifdef BUILD_WITH_DDRAW
extern LPDIRECTDRAW &g_directDrawObject;
extern LPDIRECTDRAWSURFACE &g_paletteSurface;
#endif
#else
#ifdef BUILD_WITH_DDRAW
extern LPDIRECTDRAW g_directDrawObject;
extern LPDIRECTDRAWSURFACE g_paletteSurface;
#endif
#endif

#endif // INITVIDEO_H
