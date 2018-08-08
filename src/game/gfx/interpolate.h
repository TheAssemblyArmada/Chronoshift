/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Interpolation tables and functions for scaling up 320 x 200 image content.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef INTERPOLATE_H
#define INTERPOLATE_H

#include "always.h"

class GraphicViewPortClass;
class GraphicBufferClass;

enum CopyEnum
{
    COPY_NONE = -1,
    COPY_INTERLEAVE,
    COPY_LINE_DOUBLE,
    COPY_LINE_INTERPOLATE,
};

#define INTERPOL_PAL_SIZE 256 * 256 //256x256 = 65536 pixels

void Create_Palette_Interpolation_Table();
void Read_Interpolation_Palette(char const *filename);
void Write_Interpolation_Palette(char const *filename);
void Rebuild_Interpolated_Palette(void *ipalette);
int Load_Interpolated_Palettes(char const *filename, BOOL append);
void Free_Interpolated_Palettes();

void Interpolate_2X_Scale(
    GraphicBufferClass &src, GraphicViewPortClass &dst, char const *filename);
void __cdecl Interpolate_Interleave(void *src, void *dst, int src_height, int src_width, int dst_pitch);
void __cdecl Interpolate_Line_Double(void *src, void *dst, int src_height, int src_width, int dst_pitch);
void __cdecl Interpolate_Line_Interpolate(void *src, void *dst, int src_height, int src_width, int dst_pitch);

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"

extern uint8_t *g_paletteInterpolationTable;
extern BOOL &g_palettesRead;
extern int &g_paletteCounter;
extern int &g_interpolationMode;

namespace Interpolate {

inline void Hook_Me()
{
    Hook_Function(0x005B2DD0, Create_Palette_Interpolation_Table);
    Hook_Function(0x005B2CE0, Read_Interpolation_Palette);
    Hook_Function(0x005B2D5C, Write_Interpolation_Palette);
    Hook_Function(0x004A8704, Load_Interpolated_Palettes);
    Hook_Function(0x004A8874, Free_Interpolated_Palettes);
    Hook_Function(0x005B2FCC, Interpolate_2X_Scale);
}

}
#else
extern uint8_t g_paletteInterpolationTable[INTERPOL_PAL_SIZE];
extern BOOL g_palettesRead;
extern int g_paletteCounter;
extern int g_interpolationMode;
#endif

#endif // INTERPOLATE_H
