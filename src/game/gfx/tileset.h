/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Low level functions for loading and rendering C&C terrain tilesets.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef TILESET_H
#define TILESET_H

#include "always.h"

class GraphicViewPortClass;

#define TD_TILESET_CHECK 0x20

/**
* @brief union is to handle the parts of the header which vary between TD and RA format tiles.
*/
#pragma pack(push, 1)
struct IconControlType
{
    int16_t width; // always 24 (ICON_WIDTH)
    int16_t height; // always 24 (ICON_HEIGHT)
    int16_t count; // count of cells in set, not same as images
    int16_t allocated; // is treated like a bool, always 0 in the file?

    union
    {
        struct
        {
            int32_t size; // filesize
            int32_t icons; // always 0x00000020
            int32_t palettes; // seems to always be 0x00000000
            int32_t remaps; // unknown, bitfield?
            int32_t trans_flag; // array of images length, unknown
            int32_t map; // image index for each cell
        } td;

        struct
        {
            int16_t map_width; // tile width in cells
            int16_t map_height; // tile height in cells
            int32_t size; // filesize
            int32_t icons; // always 0x00000028
            int32_t palettes; // seems to always be 0x00000000
            int32_t remaps; // unknown, bitfield?
            int32_t trans_flag; // array of images length, unknown
            int32_t color_map; // terrain type index, ra only
            int32_t map; // image index for each cell
        } ra;
    };
};
#pragma pack(pop)

void __cdecl Buffer_Draw_Stamp(
    GraphicViewPortClass &viewport, IconControlType *tileset, int icon, int x, int y, const void *remapper);
void __cdecl Buffer_Draw_Stamp_Clip(GraphicViewPortClass &viewport, IconControlType *tileset, int icon, int x, int y,
    const void *remapper, int left, int top, int right, int bottom);
uint8_t *Get_Icon_Set_Map(void *temp);

#endif // TILESET_H
