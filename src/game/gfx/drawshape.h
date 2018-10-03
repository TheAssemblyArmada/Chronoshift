/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Low level functions for loading and rendering C&C sprite files.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef DRAWSHAPE_H
#define DRAWSHAPE_H

#include "facing.h"
#include "dialog.h"
#include "shape.h"

void CC_Draw_Shape(void *shape, int frame, int draw_x, int draw_y, WindowNumberType window_num = WINDOW_0,
    ShapeFlags flags = SHAPE_NORMAL, void *fading_table = nullptr, void *ghost_table = nullptr, DirType angle = DIR_NORTH,
    int scale = 256);

#endif // DRAWSHAPE_H
