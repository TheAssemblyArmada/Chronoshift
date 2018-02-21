/**
 * @file
 *
 * @Author CCHyper, OmniBlade
 *
 * @brief Low level software blitters.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "blitters.h"
#include "gamedebug.h"
#include "minmax.h"

namespace
{
// Clipping functions for line drawing.
unsigned Line_Get_Clipping(GraphicViewPortClass &view, int x, int y)
{
    unsigned flags = 0;

    if (y < view.Get_YPos()) {
        flags |= 0x1;
    }

    if (y > view.Get_Height() - 1) {
        flags |= 0x2;
    }

    if (x < view.Get_XPos()) {
        flags |= 0x4;
    }

    if (x > view.Get_Width() - 1) {
        flags |= 0x8;
    }

    return flags;
}

void Line_Clip_Top(GraphicViewPortClass &view, int &x1, int &y1, int x2, int y2)
{
    x1 += (x2 - x1) * (view.Get_YPos() - y1) / (y2 - y1);
    y1 = view.Get_YPos();
}

void Line_Clip_Bottom(GraphicViewPortClass &view, int &x1, int &y1, int x2, int y2)
{
    x1 += (x2 - x1) * (y1 - view.Get_Height() - 1) / (y1 - y2);
    y1 = view.Get_Height() - 1;
}

void Line_Clip_Left(GraphicViewPortClass &view, int &x1, int &y1, int x2, int y2)
{
    y1 += (y2 - y1) * (view.Get_XPos() - x1) / (x2 - x1);
    x1 = view.Get_XPos();
}

void Line_Clip_Right(GraphicViewPortClass &view, int &x1, int &y1, int x2, int y2)
{
    y1 += (y2 - y1) * (x1 - view.Get_Width() - 1) / (x1 - x2);
    x1 = view.Get_Width() - 1;
}

} // namespace

void __cdecl Buffer_Draw_Line(GraphicViewPortClass &vp, int x1, int y1, int x2, int y2, uint8_t color)
{
    uint8_t *screen = static_cast<uint8_t *>(vp.Get_Offset());
    int increment = 1;

    // Do we need to do any clipping
    if (x1 < 0 || x1 >= vp.Get_Width() || y1 < 0 || y1 >= vp.Get_Height() || x2 < 0 || x2 >= vp.Get_Width() || y2 < 0
        || y2 >= vp.Get_Height()) {
        while (true) {
            unsigned clip1 = Line_Get_Clipping(vp, x1, y1);
            unsigned clip2 = Line_Get_Clipping(vp, x2, y2);

            // no more clipping to be done
            if (clip1 == 0 && clip2 == 0) {
                break;
            }

            // if both are flagged to clip the same, no need to draw the line
            if ((clip1 & clip2) != 0) {
                return;
            }

            switch (clip1) {
                case 1:
                case 9:
                    Line_Clip_Top(vp, x1, y1, x2, y2);
                    break;

                case 2:
                case 6:
                    Line_Clip_Bottom(vp, x1, y1, x2, y2);
                    break;

                case 4:
                case 5:
                    Line_Clip_Left(vp, x1, y1, x2, y2);
                    break;

                case 8:
                case 10:
                    Line_Clip_Right(vp, x1, y1, x2, y2);
                    break;

                default:
                    switch (clip2) {
                        case 1:
                        case 9:
                            Line_Clip_Top(vp, x2, y2, x1, y1);
                            break;

                        case 2:
                        case 6:
                            Line_Clip_Bottom(vp, x2, y2, x1, y1);
                            break;

                        case 4:
                        case 5:
                            Line_Clip_Left(vp, x2, y2, x1, y1);
                            break;

                        case 8:
                        case 10:
                            Line_Clip_Right(vp, x2, y2, x1, y1);
                            break;

                        default:
                            break;
                    }
            }
        }
    }

    y2 -= y1;

    if (y2 == 0) {
        if (x1 >= x2) {
            int x = x1;
            x1 = x2;
            x2 = x;
        }

        x2 -= x1 - 1;
        screen += y1 * vp.Get_Full_Pitch() + x1;
        memset(screen, color, x2);

        return;
    }

    if (y2 < 0) {
        int x = x1;
        x1 = x2;
        x2 = x;
        y2 = -y2;
        y1 -= y2;
    }

    screen += y1 * vp.Get_Full_Pitch();

    x2 -= x1;
    if (x2 == 0) {
        screen += x1;

        while (y2-- != 0) {
            *screen = color;
            screen += vp.Get_Full_Pitch();
        }

        return;
    }

    if (x2 < 0) {
        x2 = -x2;
        increment = -1;
    }

    if (x2 < y2) {
        int full = y2;
        int half = y2 / 2;
        screen += x1;

        while (true) {
            *screen = color;

            if (y2-- == 0) {
                return;
            }

            screen += vp.Get_Full_Pitch();
            half -= x2;

            if (half < 0) {
                half += full;
                screen += increment;
            }
        }
    } else {
        int full = x2;
        int half = x2 / 2;
        screen += x1;

        while (true) {
            *screen = color;

            if (x2-- == 0) {
                return;
            }

            screen += increment;
            half -= y2;

            if (half < 0) {
                half += full;
                screen += vp.Get_Full_Pitch();
            }
        }
    }
}

void __cdecl Buffer_Fill_Rect(GraphicViewPortClass &vp, int x, int y, int w, int h, uint8_t color)
{
    uint8_t *offset = static_cast<uint8_t *>(vp.Get_Offset());

    if ((x >= vp.Get_Width()) || (y >= vp.Get_Height()) || (x + w < 0) || (y + h < 0)) {
        return;
    }

    x = Max(0, x);
    y = Max(0, y);

    int width = w >= vp.Get_Width() ? vp.Get_Width() - x : w - x + 1;
    int height = h >= vp.Get_Height() ? vp.Get_Height() - y : h - y + 1;

    // DEBUG_LOG("Actual Fill_Rect(%u, %d, %d, %d, %d, %d)\n", view, x_pos, y_pos, width, height, color);

    offset += y * vp.Get_Full_Pitch() + x;

    for (int h = 0; h < height; ++h) {
        memset(offset, color, width);
        offset += vp.Get_Full_Pitch();
    }
}

void __cdecl Buffer_Remap(GraphicViewPortClass &vp, int x, int y, int w, int h, uint8_t *fading_table)
{
    if (fading_table == nullptr) {
        return;
    }

    int xstart = x;
    int ystart = y;
    int xend = x + w - 1;
    int yend = y + h - 1;

    int xoffset = 0;
    int yoffset = 0;

    // If we aren't drawing within the viewport, return
    if (xstart >= vp.Get_Width() || ystart >= vp.Get_Height() || xend < 0 || yend < 0) {
        return;
    }

    // Clipping
    if (xstart < 0) {
        xoffset = -xstart;
        xstart = 0;
    }

    if (ystart < 0) {
        yoffset += h * (-ystart);
        ystart = 0;
    }

    xend = Min(xend, (int)vp.Get_Width() - 1);
    yend = Min(yend, (int)vp.Get_Height() - 1);

    // Setup parameters for blit
    int pitch = vp.Get_Pitch() + vp.Get_Width() + vp.Get_XAdd();
    uint8_t *offset = y * pitch + x + static_cast<uint8_t *>(vp.Get_Offset());
    int lines = yend - ystart + 1;
    int blit_width = xend - xstart + 1;

    // remap blit
    while (lines--) {
        for (int i = 0; i < blit_width; ++i) {
            offset[i] = fading_table[offset[i]];
        }

        offset += pitch;
    }
}

int __cdecl Buffer_Get_Pixel(GraphicViewPortClass &vp, unsigned x, unsigned y)
{
    if (x < (unsigned)vp.Get_Width() && y < (unsigned)vp.Get_Height()) {
        return *(static_cast<uint8_t *>(vp.Get_Offset()) + x + y * vp.Get_Full_Pitch());
    }

    return 0;
}

void __cdecl Buffer_Put_Pixel(GraphicViewPortClass &vp, unsigned x, unsigned y, uint8_t val)
{
    if (x < (unsigned)vp.Get_Width() && y < (unsigned)vp.Get_Height()) {
        *(static_cast<uint8_t *>(vp.Get_Offset()) + x + y * vp.Get_Full_Pitch()) = val;
    }
}

void __cdecl Linear_Blit_To_Linear(GraphicViewPortClass &src_vp, GraphicViewPortClass &dst_vp, int src_x, int src_y,
    int dst_x, int dst_y, int w, int h, bool use_key)
{
    uint8_t *src = static_cast<uint8_t *>(src_vp.Get_Offset()) + src_vp.Get_XAdd();
    uint8_t *dst = static_cast<uint8_t *>(dst_vp.Get_Offset()) + dst_vp.Get_XAdd();
    int s_scanline = (src_vp.Get_Pitch() + src_vp.Get_XAdd() + src_vp.Get_Width());
    int d_scanline = (dst_vp.Get_Pitch() + dst_vp.Get_XAdd() + dst_vp.Get_Width());

    if (src_x >= src_vp.Get_Width() || src_y >= src_vp.Get_Height() || dst_x >= dst_vp.Get_Width()
        || dst_y >= dst_vp.Get_Height() || h < 0 || w < 1) {
        return;
    }

    src_x = Max(0, src_x);
    src_y = Max(0, src_y);
    dst_x = Max(0, dst_x);
    dst_y = Max(0, dst_y);

    h = (dst_y + h) > dst_vp.Get_Height() ? dst_vp.Get_Height() - 1 - dst_y : h;
    w = (dst_x + w) > dst_vp.Get_Width() ? dst_vp.Get_Width() - 1 - dst_x : w;

    // move our pointers to the start locations
    src += src_x + src_y * s_scanline;
    dst += dst_x + dst_y * d_scanline;

    // If src is before dst, we run the risk of overlapping memory regions so we
    // need to move src and dst to the last line and work backwards
    if (src < dst) {
        uint8_t *esrc = src + (h - 1) * s_scanline;
        uint8_t *edst = dst + (h - 1) * d_scanline;
        if (use_key) {
            char key_colour = 0;
            while (h-- != 0) {
                // Can't optimize as we need to check every pixel against key colour :(
                for (int i = w - 1; i >= 0; --i) {
                    if (esrc[i] != key_colour) {
                        edst[i] = esrc[i];
                    }
                }

                edst -= d_scanline;
                esrc -= s_scanline;
            }
        } else {
            while (h-- != 0) {
                memmove(edst, esrc, w);
                edst -= d_scanline;
                esrc -= s_scanline;
            }
        }
    } else {
        if (use_key) {
            uint8_t key_colour = 0;
            while (h-- != 0) {
                // Can't optimize as we need to check every pixel against key colour :(
                for (int i = 0; i < w; ++i) {
                    if (src[i] != key_colour) {
                        dst[i] = src[i];
                    }
                }

                dst += d_scanline;
                src += s_scanline;
            }
        } else {
            while (h-- != 0) {
                memmove(dst, src, w);
                dst += d_scanline;
                src += s_scanline;
            }
        }
    }
}

void __cdecl Buffer_Clear(GraphicViewPortClass &vp, uint8_t color)
{
    uint8_t *offset = static_cast<uint8_t *>(vp.Get_Offset());

    for (int h = 0; h < vp.Get_Height(); ++h) {
        memset(offset, color, vp.Get_Width());
        offset += vp.Get_Full_Pitch();
    }
}

void __cdecl Buffer_To_Buffer(GraphicViewPortClass &vp, int x, int y, int w, int h, void *buffer, int length)
{
    int xstart = x;
    int ystart = y;
    int xend = x + w - 1;
    int yend = y + h - 1;

    int xoffset = 0;
    int yoffset = 0;

    // If we aren't drawing within the viewport, return
    if (!buffer || length <= 0 || xstart >= vp.Get_Width() || ystart >= vp.Get_Height() || xend < 0 || yend < 0) {
        return;
    }

    // Clipping
    if (xstart < 0) {
        xoffset = -xstart;
        xstart = 0;
    }

    if (ystart < 0) {
        yoffset += h * (-ystart);
        ystart = 0;
    }

    xend = Min(xend, (int)vp.Get_Width() - 1);
    yend = Min(yend, (int)vp.Get_Height() - 1);

    // Setup parameters for blit
    int pitch = vp.Get_Pitch() + vp.Get_Width() + vp.Get_XAdd();
    uint8_t *src = y * pitch + x + static_cast<uint8_t *>(vp.Get_Offset());
    uint8_t *dst = xoffset + w * yoffset + static_cast<uint8_t *>(buffer);
    // int src_pitch = x_pos + pitch - xend + 1;
    // int dst_pitch = x_pos + width - xend + 1;
    int lines = yend - ystart + 1;
    int blit_width = xend - xstart + 1;

    // Is our buffer large enough?
    if (lines * w <= length) {
        // blit
        while (lines--) {
            memcpy(dst, src, blit_width);
            src += pitch;
            dst += w;
        }
    }
}

void __cdecl Buffer_To_Page(int x, int y, int w, int h, void *buffer, GraphicViewPortClass &vp)
{
    int xstart = x;
    int ystart = y;
    int xend = x + w - 1;
    int yend = y + h - 1;

    int xoffset = 0;
    int yoffset = 0;

    // If we aren't drawing within the viewport, return
    if (!buffer || xstart >= vp.Get_Width() || ystart >= vp.Get_Height() || xend < 0 || yend < 0) {
    }

    // Clipping
    if (xstart < 0) {
        xoffset = -xstart;
        xstart = 0;
    }

    if (ystart < 0) {
        yoffset += h * (-ystart);
        ystart = 0;
    }

    xend = Min(xend, vp.Get_Width() - 1);
    yend = Min(yend, vp.Get_Height() - 1);

    int pitch = vp.Get_Pitch() + vp.Get_Width() + vp.Get_XAdd();
    uint8_t *dst = y * pitch + x + static_cast<uint8_t *>(vp.Get_Offset());
    uint8_t *src = xoffset + w * yoffset + static_cast<uint8_t *>(buffer);
    // int dst_pitch = x_pos + pitch - xend;
    // int src_pitch = x_pos + width - xend;
    int lines = yend - ystart + 1;
    int blit_width = xend - xstart + 1;

    // blit
    while (lines--) {
        memcpy(dst, src, blit_width);
        src += w;
        dst += pitch;
    }
}

void __cdecl Linear_Scale_To_Linear(GraphicViewPortClass &src_vp, GraphicViewPortClass &dst_vp, int src_x, int src_y,
    int dst_x, int dst_y, int src_w, int src_h, int dst_w, int dst_h, bool use_keysrc, void *fade)
{
    // If there is nothing to scale, just return.
    if (src_w <= 0 || src_h <= 0 || dst_w <= 0 || dst_h <= 0) {
        return;
    }

    int sx = src_x;
    int sy = src_y;
    int dx = dst_x;
    int dy = dst_y;
    int dw = dst_w + dst_x;
    int dh = dst_h + dst_y;

    // These ifs are all for clipping purposes incase coords are outside
    // the expected area.
    if (src_x < 0) {
        sx = 0;
        dx = dst_x + ((dst_w * -src_x) / src_w);
    }

    if (src_y < 0) {
        sy = 0;
        dy = dst_y + ((dst_h * -src_y) / src_h);
    }

    if (src_x + src_w > src_vp.Get_Width() + 1) {
        dw = dst_x + (dst_w * (src_vp.Get_Width() - src_x) / src_w);
    }

    if (src_y + src_h > src_vp.Get_Height() + 1) {
        dh = dst_y + (dst_h * (src_vp.Get_Height() - src_y) / src_h);
    }

    if (dx < 0) {
        dx = 0;
        sx = src_x + ((src_w * -dst_x) / dst_w);
    }

    if (dy < 0) {
        dy = 0;
        sy = src_y + ((src_h * -dst_y) / dst_h);
    }

    if (dw > dst_vp.Get_Width() + 1) {
        dw = dst_vp.Get_Width();
    }

    if (dh > dst_vp.Get_Height() + 1) {
        dh = dst_vp.Get_Height();
    }

    if (dy > dh || dx > dw) {
        return;
    }

    uint8_t *src = sy * src_vp.Get_Full_Pitch() + sx + static_cast<uint8_t *>(src_vp.Get_Offset());
    uint8_t *dst = dy * dst_vp.Get_Full_Pitch() + dx + static_cast<uint8_t *>(dst_vp.Get_Offset());
    dw -= dx;
    dh -= dy;
    int x_ratio = ((src_w << 16) / dw) + 1;
    int y_ratio = ((src_h << 16) / dh) + 1;
    
    // keysrc basically means do we skip index 0 entries, thus treating them as
    // transparent?
    if (use_keysrc) {
        if (fade != nullptr) {
            for (int i = 0; i < dh; ++i) {
                uint8_t *d = dst + i * dst_vp.Get_Full_Pitch();
                uint8_t *s = src + ((i * y_ratio) >> 16) * src_vp.Get_Full_Pitch();
                int xrat = 0;

                for (int j = 0; j < dw; ++j) {
                    uint8_t tmp = s[xrat >> 16];

                    if (tmp != 0) {
                        *d = static_cast<uint8_t *>(fade)[tmp];
                    }

                    ++d;
                    xrat += x_ratio;
                }
            }
        } else {
            for (int i = 0; i < dh; ++i) {
                uint8_t *d = dst + i * dst_vp.Get_Full_Pitch();
                uint8_t *s = src + ((i * y_ratio) >> 16) * src_vp.Get_Full_Pitch();
                int xrat = 0;

                for (int j = 0; j < dw; ++j) {
                    uint8_t tmp = s[xrat >> 16];

                    if (tmp != 0) {
                        *d = tmp;
                    }

                    ++d;
                    xrat += x_ratio;
                }
            }
        }
    } else {
        if (fade != nullptr) {
            for (int i = 0; i < dh; ++i) {
                uint8_t *d = dst + i * dst_vp.Get_Full_Pitch();
                uint8_t *s = src + ((i * y_ratio) >> 16) * src_vp.Get_Full_Pitch();
                int xrat = 0;

                for (int j = 0; j < dw; ++j) {
                    *d++ = static_cast<uint8_t *>(fade)[s[xrat >> 16]];
                    xrat += x_ratio;
                }
            }
        } else {
            for (int i = 0; i < dh; ++i) {
                uint8_t *d = dst + i * dst_vp.Get_Full_Pitch();
                uint8_t *s = src + ((i * y_ratio) >> 16) * src_vp.Get_Full_Pitch();
                int xrat = 0;

                for (int j = 0; j < dw; ++j) {
                    *d++ = s[xrat >> 16];
                    xrat += x_ratio;
                }
            }
        }
    }
}
