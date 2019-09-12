/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Low level software blitters.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef BLITTERS_H
#define BLITTERS_H

#include "always.h"
#include "gbuffer.h"

void __cdecl Buffer_Draw_Line(GraphicViewPortClass &vp, int x1, int y1, int x2, int y2, uint8_t color);
void __cdecl Buffer_Fill_Rect(GraphicViewPortClass &vp, int x, int y, int w, int h, uint8_t color);
void __cdecl Buffer_Remap(GraphicViewPortClass &vp, int x, int y, int w, int h, uint8_t *fading_table);
int __cdecl Buffer_Get_Pixel(GraphicViewPortClass &vp, unsigned x, unsigned y);
void __cdecl Buffer_Put_Pixel(GraphicViewPortClass &vp, unsigned x, unsigned y, uint8_t val);
void __cdecl Linear_Blit_To_Linear(GraphicViewPortClass &src_vp, GraphicViewPortClass &dst_vp, int src_x, int src_y,
    int dst_x, int dst_y, int w, int h, bool use_key);
void __cdecl Buffer_Clear(GraphicViewPortClass &vp, uint8_t color);
void __cdecl Buffer_To_Buffer(GraphicViewPortClass &vp, int x, int y, int w, int h, void *buffer, int length);
void __cdecl Buffer_To_Page(int x, int y, int w, int h, void *buffer, GraphicViewPortClass &vp);
void __cdecl Linear_Scale_To_Linear(GraphicViewPortClass &src_vp, GraphicViewPortClass &dst_vp, int src_x, int src_y,
    int dst_x, int dst_y, int src_w, int src_h, int dst_w, int dst_h, bool use_keysrc, void *fade);

#endif // BLITTERS_H
