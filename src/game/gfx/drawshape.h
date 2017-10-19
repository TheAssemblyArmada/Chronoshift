/**
* @file
*
* @author CCHyper
* @author OmniBlade
*
* @brief Low level functions for loading and rendering C&C sprite files.
*
* @copyright Redalert++ is free software: you can redistribute it and/or
*            modify it under the terms of the GNU General Public License
*            as published by the Free Software Foundation, either version
*            2 of the License, or (at your option) any later version.
*            A full copy of the GNU General Public License can be found in
*            LICENSE
*/
#pragma once

#ifndef DRAWSHAPE_H
#define DRAWSHAPE_H

//#include "facing.h"
#include "dialog.h"
#include "shape.h"

// TODO move to facing.h when implemented.
enum DirType
{
    DIR_NORTH = 0,                      //0°
    DIR_NORTH_NORTH_EAST = 16,
    DIR_NORTH_EAST = 32,                //45°
    DIR_EAST_NORTH_EAST = 48,
    DIR_EAST = 64,                      //90°
    DIR_EAST_SOUTH_EAST = 80,
    DIR_SOUTH_EAST = 96,                //135°
    DIR_SOUTH_SOUTH_EAST = 114,
    DIR_SOUTH = 128,                    //180°
    DIR_SOUTH_SOUTH_WEST = 144,
    DIR_SOUTH_WEST = 160,               //225°
    DIR_WEST_SOUTH_WEST = 176,
    DIR_WEST = 192,                     //270°
    DIR_WEST_NORTH_WEST = 208,
    DIR_NORTH_WEST = 224,               //315°
    DIR_NORTH_WEST_NORTH = 240,
    DIR_LAST = 255,
};

void CC_Draw_Shape(void *shape, int frame, int draw_x, int draw_y, WindowNumberType window_num = WINDOW_0,
    ShapeFlags flags = SHAPE_NORMAL, void *fading_table = nullptr, void *ghost_table = nullptr,
    DirType angle = DIR_NORTH, int scale = 256);

#endif // DRAWSHAPE_H
