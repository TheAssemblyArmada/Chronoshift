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
#include "drawshape.h"
#include "display.h"
#include "gamefile.h"
#include "gamedebug.h"
#include "gbuffer.h"
#include "globals.h"

/**
 * @brief Main function used to draw sprites to the screen, wraps lower level handling.
 */
void CC_Draw_Shape(void *shape_ptr, int frame, int draw_x, int draw_y, WindowNumberType window_num, ShapeFlags flags,
    void *fading_table, void *ghost_table, DirType angle, int scale)
{
    static uint8_t *_xbuffer;

    void *shape_frame;
    int shape_height;
    int shape_width;
    int curr_frame;

    if (shape_ptr != nullptr) {
        if (flags & SHAPE_FADING && fading_table == nullptr) {
            fading_table = DisplayClass::FadingShade;
        }

        if (flags & SHAPE_GHOST && ghost_table == nullptr) {
            ghost_table = DisplayClass::SpecialGhost;
        }

        if (_xbuffer == nullptr) {
            _xbuffer = new uint8_t[SHAPE_BUFFER_SIZE];
        }

        if (frame != -1) {
            shape_width = Get_Build_Frame_Width(shape_ptr);
            shape_height = Get_Build_Frame_Height(shape_ptr);
            shape_frame = Build_Frame(shape_ptr, frame, g_shapeBuffer);

            if (shape_frame != nullptr) {
                if (Get_Last_Frame_Length() > g_shapeBufferSize) {
                    DEBUG_LOG("Attempt to use shape buffer for size %d buffer is only size %d",
                        Get_Last_Frame_Length(),
                        g_shapeBufferSize);
                }

                GraphicViewPortClass vp(g_logicPage->Get_Graphic_Buffer(),
                    WindowList[window_num].X + g_logicPage->Get_XPos(),
                    WindowList[window_num].Y + g_logicPage->Get_YPos(),
                    WindowList[window_num].W,
                    WindowList[window_num].H);

                g_useOldShapeDraw = false;

                // As all shapes are drawn as they are, we ignore DIR_NORTH as it represents this.
                // Scale of 256 is 100%
                if (angle != DIR_NORTH || scale != 256) {
                    g_useOldShapeDraw = true;
                    BitmapClass data_bitmap(shape_width, shape_height, Get_Shape_Header_Data(shape_frame));
                    shape_width *= 2;
                    shape_height *= 2;
                    memset(_xbuffer, 0, SHAPE_BUFFER_SIZE);
                    GraphicBufferClass xform_buff(shape_width, shape_height, _xbuffer);
                    TPoint2D<int> point(shape_width / 2, shape_height / 2);
                    xform_buff.Scale_Rotate(data_bitmap, point, scale, -(char)(angle - 64));
                    shape_frame = _xbuffer;
                }

                if ((flags & (SHAPE_FADING | SHAPE_PREDATOR)) == (SHAPE_FADING | SHAPE_PREDATOR)) {
                    ghost_table = reinterpret_cast<void*>(0x006580E4); // TODO these should both be from DisplayClass
                    flags &= ~(SHAPE_FADING | SHAPE_PREDATOR);
                    flags |= SHAPE_GHOST;
                }

                curr_frame = g_GameFrame;
                if (WindowList[window_num].X >> 1 < draw_x) {
                    curr_frame = -curr_frame;
                }

                if (vp.Lock()) {
                    if ((flags & (SHAPE_GHOST | SHAPE_FADING)) == (SHAPE_GHOST | SHAPE_FADING)) {
                        Buffer_Frame_To_Page(draw_x,
                            draw_y,
                            shape_width,
                            shape_height,
                            shape_frame,
                            vp,
                            flags | SHAPE_TRANSPARENT,
                            ghost_table,
                            fading_table,
                            1,
                            curr_frame);
                    } else if ((flags & SHAPE_FADING) != 0) {
                        Buffer_Frame_To_Page(draw_x,
                            draw_y,
                            shape_width,
                            shape_height,
                            shape_frame,
                            vp,
                            flags | SHAPE_TRANSPARENT,
                            fading_table,
                            1,
                            curr_frame);
                    } else if ((flags & SHAPE_PREDATOR) != 0) {
                        Buffer_Frame_To_Page(draw_x,
                            draw_y,
                            shape_width,
                            shape_height,
                            shape_frame,
                            vp,
                            flags | SHAPE_TRANSPARENT,
                            curr_frame);
                    } else {
                        Buffer_Frame_To_Page(draw_x,
                            draw_y,
                            shape_width,
                            shape_height,
                            shape_frame,
                            vp,
                            flags | SHAPE_TRANSPARENT,
                            ghost_table,
                            curr_frame);
                    }

                    vp.Unlock();
                }
            }
        }
    }
}
