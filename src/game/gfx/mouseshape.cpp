/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Low level sprite drawing functions for Dune2 sprite format (used for mouse in early C&C games).
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "mouseshape.h"
#include "mouse.h"
#include "gbuffer.h"
#include "lcw.h"
#include <cstring>
#include <algorithm>

using std::memcpy;

uint8_t g_mouseShapeBuffer[65000]; // Original code shares the global ShapeBuffer with other sprite drawing.

int Get_Shape_Uncomp_Size(void *shape)
{
    return static_cast<MouseShapeFrameHeader *>(shape)->uncompressed_size;
}

int Extract_Shape_Count(void *shape)
{
    return static_cast<MouseShapeHeader *>(shape)->frame_count;
}

void *Extract_Shape(void *shape, int frame)
{
    char *data = static_cast<char *>(shape);
    MouseShapeHeader *header = static_cast<MouseShapeHeader *>(shape);

    if (shape && frame >= 0 && header->frame_count > frame) {
        return data + (&header->offset)[frame] + 2;
    }

    return nullptr;
}

int Get_Shape_Width(void *shape)
{
    // return static_cast<ShapeHeaderStruct*>(shape)->Width;
    return static_cast<MouseShapeFrameHeader *>(shape)->width;
}

int Get_Shape_Height(void *shape)
{
    return static_cast<MouseShapeFrameHeader *>(shape)->lines;
}

int Restore_Shape_Height(void *shape)
{
    int oldheight = static_cast<MouseShapeFrameHeader *>(shape)->lines;
    static_cast<MouseShapeFrameHeader *>(shape)->lines = static_cast<MouseShapeFrameHeader *>(shape)->height;
    return oldheight;
}

int Get_Shape_Original_Height(void *shape)
{
    return static_cast<MouseShapeFrameHeader *>(shape)->height;
}

void __cdecl Mouse_Shadow_Buffer(WWMouseClass &mouse, GraphicViewPortClass &viewport, void *buffer, int x_pos, int y_pos, int hspot_x, int hspot_y, BOOL save)
{
    int xstart = x_pos - hspot_x;
    int ystart = y_pos - hspot_y;
    int yend = mouse.Get_Cursor_Height() + ystart - 1;
    int xend = mouse.Get_Cursor_Width() + xstart - 1;
    int ms_img_offset = 0;
    uint8_t *mouse_img = static_cast<uint8_t*>(buffer);

    //If we aren't drawing within the viewport, return
    if (xstart >= viewport.Get_Width() ||
        ystart >= viewport.Get_Height() ||
        xend <= 0 || yend <= 0) {
        return;
    }

    if (xstart < 0) {
        ms_img_offset = -xstart;
        xstart = 0;
    }

    if (ystart < 0) {
        mouse_img += mouse.Get_Cursor_Width() * (-ystart);
        ystart = 0;
    }

    xend = std::min(xend, viewport.Get_Width() - 1);
    yend = std::min(yend, viewport.Get_Height() - 1);

    int blit_height = yend - ystart + 1;
    int blit_width = xend - xstart + 1;
    int src_pitch;
    int dst_pitch;
    uint8_t *src;
    uint8_t *dst;

    // Bool will control if we are copying to or from shadow buffer, set vars here
    if (save) {
        src_pitch = (viewport.Get_Pitch() + viewport.Get_XAdd() + viewport.Get_Width());
        dst_pitch = mouse.Get_Cursor_Width();
        src = static_cast<uint8_t*>(viewport.Get_Offset()) + src_pitch * ystart + xstart;
        dst = mouse_img + ms_img_offset;
    } else {
        src_pitch = mouse.Get_Cursor_Width();
        dst_pitch = (viewport.Get_Pitch() + viewport.Get_XAdd() + viewport.Get_Width());
        src = mouse_img + ms_img_offset;
        dst = static_cast<uint8_t*>(viewport.Get_Offset()) + dst_pitch * ystart + xstart;
    }

    while (blit_height--) {
        memcpy(dst, src, blit_width);
        src += src_pitch;
        dst += dst_pitch;
    }
}

void __cdecl Mouse_Draw(WWMouseClass &mouse, GraphicViewPortClass &viewport, int x_pos, int y_pos)
{
    int xstart = x_pos - mouse.Get_Hotspot_X();
    int ystart = y_pos - mouse.Get_Hotspot_Y();
    int yend = mouse.Get_Cursor_Height() + ystart - 1;
    int xend = mouse.Get_Cursor_Width() + xstart - 1;
    int ms_img_offset = 0;
    uint8_t *mouse_img = mouse.Get_Image_Buff();

    //If we aren't drawing within the viewport, return
    if (xstart >= viewport.Get_Width() ||
        ystart >= viewport.Get_Height() ||
        xend <= 0 || yend <= 0) {
        return;
    }

    if (xstart < 0) {
        ms_img_offset = -xstart;
        xstart = 0;
    }

    if (ystart < 0) {
        mouse_img += mouse.Get_Cursor_Width() * (-ystart);
        ystart = 0;
    }

    xend = std::min(xend, viewport.Get_Width() - 1);
    yend = std::min(yend, viewport.Get_Height() - 1);

    int pitch = viewport.Get_Pitch() + viewport.Get_XAdd() + viewport.Get_Width();
    uint8_t *dst = xstart + pitch * ystart + static_cast<uint8_t*>(viewport.Get_Offset());
    uint8_t *src = ms_img_offset + mouse_img;
    int blit_pitch = xend - xstart + 1;

    if ((xend > xstart) && (yend > ystart)) {
        int blit_height = yend - ystart + 1;
        int dst_pitch = pitch - blit_pitch;
        int src_pitch = mouse.Get_Cursor_Width() - blit_pitch;

        while (blit_height--) {
            int blit_width = blit_pitch;
            while (blit_width--) {
                uint8_t current = *src++;
                if (current) {
                    *dst = current;
                }

                ++dst;
                //--blit_width;
            }

            src += src_pitch;
            dst += dst_pitch;
            //--blit_height;
        }
    }
}

void *__cdecl Mouse_Set_Cursor(WWMouseClass &mouse, int hspot_x, int hspot_y, void *frame)
{
    uint8_t *frame_buff;
    uint8_t *data_buff;
    uint8_t *decmp_buff;
    uint8_t *lcw_buff;
    int16_t frame_flags;

    int height;
    int width;
    int uncompsz;

    MouseShapeFrameHeader *frame_header = static_cast<MouseShapeFrameHeader*>(frame);

    //Get the dimensions of our frame, mouse shp format can have variable
    //dimensions for each frame.
    uncompsz = Get_Shape_Uncomp_Size(frame);
    width = Get_Shape_Width(frame);
    height = Get_Shape_Original_Height(frame);

    if (width <= mouse.Get_Max_Width() && height <= mouse.Get_Max_Height()) {
        frame_buff = mouse.Get_Image_Buff();
        data_buff = static_cast<uint8_t*>(frame);
        frame_flags = frame_header->flags;

        //Flag bit 2 is flag for no compression on frame, decompress to
        //intermediate buffer if flag is clear
        if (!(frame_header->flags & 2)) {
            decmp_buff = g_mouseShapeBuffer;
            lcw_buff = reinterpret_cast<uint8_t*>(frame_header);
            frame_flags = frame_header->flags | 2;

            memcpy(decmp_buff, lcw_buff, sizeof(MouseShapeFrameHeader));
            decmp_buff += sizeof(MouseShapeFrameHeader);
            lcw_buff += sizeof(MouseShapeFrameHeader);

            //Copies a small lookup table if it exists, probably not in RA.
            if (frame_flags & 1) {
                memcpy(decmp_buff, lcw_buff, 16);
                decmp_buff += 16;
                lcw_buff += 16;
            }

            LCW_Uncomp(lcw_buff, decmp_buff, uncompsz);
            data_buff = g_mouseShapeBuffer;
        }

        if (frame_flags & 1) {
            uint8_t *data_start = data_buff + sizeof(MouseShapeFrameHeader);
            uint8_t *image_start = data_buff + sizeof(MouseShapeFrameHeader) + 16;
            int image_size = height * width;
            //uint8_t current_byte = 0;
            int run_len = 0;

            while (image_size) {
                uint8_t current_byte = *image_start++;
                if (current_byte) {
                    *frame_buff++ = data_start[current_byte];
                    --image_size;
                    continue;
                }

                if (!image_size) {
                    break;
                }

                run_len = *image_start;
                image_size -= run_len;
                ++image_start;

                while (run_len--) {
                    *frame_buff++ = 0;
                }
            }
        } else {
            uint8_t *data_start = data_buff + sizeof(MouseShapeFrameHeader);
            int image_size = height * width;
            int run_len = 0;

            while (image_size) {
                uint8_t current_byte = *data_start++;
                if (current_byte) {
                    *frame_buff++ = current_byte;
                    --image_size;
                    continue;
                }

                if (!image_size) {
                    break;
                }

                run_len = *data_start;
                image_size -= run_len;
                ++data_start;

                while (run_len--) {
                    *frame_buff++ = 0;
                }
            }
        }

        mouse.Set_Hotspot_X(hspot_x);
        mouse.Set_Hotspot_Y(hspot_y);
        mouse.Set_Cursor_Height(height);
        mouse.Set_Cursor_Width(width);
    }

    void *result = mouse.Get_Frame_Pointer();
    mouse.Set_Frame_Pointer(frame);

    return result;
}
