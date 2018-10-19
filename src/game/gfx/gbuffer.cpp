/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Wrapper class for platform drawing engine.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "gbuffer.h"
#include "gamedebug.h"
#include "blitters.h"
#include "minmax.h"
#include "textprint.h"
#include "tileset.h"

#ifndef CHRONOSHIFT_STANDALONE
// BOOL &GraphicViewPortClass::AllowHardwareBlitFills;
// BOOL &GraphicViewPortClass::AllowStretchBlits;
int &GraphicViewPortClass::ScreenWidth = Make_Global<int>(0x006016B0);
int &GraphicViewPortClass::ScreenHeight = Make_Global<int>(0x006016B4);
GraphicViewPortClass *&g_logicPage = Make_Global<GraphicViewPortClass *>(0x006AC274);
LPDIRECTDRAWSURFACE &g_paletteSurface = Make_Global<LPDIRECTDRAWSURFACE>(0x006B18A4);
GraphicViewPortClass &g_seenBuff = Make_Global<GraphicViewPortClass>(0x006807A4);
GraphicViewPortClass &g_hidPage = Make_Global<GraphicViewPortClass>(0x006807CC);
GraphicBufferClass &g_visiblePage = Make_Global<GraphicBufferClass>(0x0068065C);
GraphicBufferClass &g_hiddenPage = Make_Global<GraphicBufferClass>(0x00680700);
#else
BOOL GraphicViewPortClass::AllowHardwareBlitFills;
BOOL GraphicViewPortClass::AllowStretchBlits;
int GraphicViewPortClass::ScreenWidth = 640;
int GraphicViewPortClass::ScreenHeight = 400;
GraphicViewPortClass *g_logicPage = nullptr;
LPDIRECTDRAWSURFACE g_paletteSurface = nullptr;
GraphicViewPortClass g_seenBuff;
GraphicViewPortClass g_hidPage;
GraphicBufferClass g_visiblePage;
GraphicBufferClass g_hiddenPage;
#endif

void Wait_Blit()
{
    DWORD result;
    do {
        result = g_paletteSurface->GetBltStatus(DDGBS_ISBLTDONE);
    } while (result != 0 && result != DDERR_SURFACELOST);
}

GraphicViewPortClass *Set_Logic_Page(GraphicViewPortClass *vp)
{
    GraphicViewPortClass *old = g_logicPage;
    g_logicPage = vp;

    return old;
}

GraphicViewPortClass *Set_Logic_Page(GraphicViewPortClass &vp)
{
    GraphicViewPortClass *old = g_logicPage;
    g_logicPage = &vp;

    return old;
}

GraphicViewPortClass::GraphicViewPortClass(GraphicBufferClass *buffer, int x, int y, int w, int h) :
    m_graphicBuff(nullptr),
    m_lockCount(0)
{
    Attach(buffer, x, y, w, h);
}

GraphicViewPortClass::~GraphicViewPortClass()
{
    m_offset = nullptr;
    m_width = 0;
    m_height = 0;
    m_xAdd = 0;
    m_xPos = 0;
    m_yPos = 0;
    m_pitch = 0;
    m_inVideoMemory = false;
    m_lockCount = 0;
    m_graphicBuff = nullptr;
}

void GraphicViewPortClass::Attach(GraphicBufferClass *buffer, int x, int y, int w, int h)
{
    if (buffer != this) {
        x = Clamp(x, 0, buffer->Get_Width() - 1); // Min(Max(0, x), buffer->Get_Width() - 1);
        y = Clamp(y, 0, buffer->Get_Height() - 1); // Min(Max(0, y), buffer->Get_Height() - 1);
        w = x + w > buffer->Get_Width() ? buffer->Get_Width() - x : w;
        h = y + h > buffer->Get_Height() ? buffer->Get_Height() - y : h;

        int scanline = y * (buffer->Get_Width() + buffer->Get_Pitch());

        m_xPos = x;
        m_yPos = y;

        m_offset = x + scanline + static_cast<char *>(buffer->Get_Offset());
        m_width = w;
        m_xAdd = buffer->Get_Width() - w;
        m_height = h;
        m_pitch = buffer->Get_Pitch();
        m_graphicBuff = buffer;
        m_inVideoMemory = buffer->In_Video_Memory();
    }
}

BOOL GraphicViewPortClass::Change(int x, int y, int w, int h)
{
    if (Get_Graphic_Buffer() == this) {
        return false;
    }

    Attach(Get_Graphic_Buffer(), x, y, w, h);

    return true;
}

BOOL GraphicViewPortClass::Lock()
{
    if (m_graphicBuff->Lock()) {
        if (m_graphicBuff != this) {
            Attach(m_graphicBuff, m_xPos, m_yPos, m_width, m_height);
        }

        return true;
    }

    return false;
}

BOOL GraphicViewPortClass::Unlock()
{
    if (m_graphicBuff->Unlock()) {
        if (m_graphicBuff != this && m_inVideoMemory && !m_graphicBuff->Get_LockCount()) {
            m_offset = nullptr;
        }

        return true;
    }

    return false;
}

void GraphicViewPortClass::Put_Pixel(int x, int y, unsigned char value)
{
    if (Lock()) {
        Buffer_Put_Pixel(*this, x, y, value);
    }

    Unlock();
}

unsigned GraphicViewPortClass::Get_Pixel(int x, int y)
{
    unsigned val = 0;

    if (Lock()) {
        val = Buffer_Get_Pixel(*this, x, y);
    }

    Unlock();

    return val;
}

void GraphicViewPortClass::Remap(int x, int y, int w, int h, unsigned char *fading_table)
{
    if (Lock()) {
        Buffer_Remap(*this, x, y, w, h, fading_table);
    }

    Unlock();
}

void GraphicViewPortClass::Draw_Rect(int x, int y, int w, int h, unsigned char color)
{
    if (Lock()) {
        Draw_Line(x, y, w, y, color);
        Draw_Line(x, h, w, h, color);
        Draw_Line(x, y, x, h, color);
        Draw_Line(w, y, w, h, color);
    }

    Unlock();
}

void GraphicViewPortClass::Draw_Line(int x1, int y1, int x2, int y2, unsigned char color)
{
    if (Lock()) {
        Buffer_Draw_Line(*this, x1, y1, x2, y2, color);
    }

    Unlock();
}

void GraphicViewPortClass::Fill_Rect(int x, int y, int w, int h, unsigned char color)
{
    if (Lock()) {
        Buffer_Fill_Rect(*this, x, y, w, h, color);
    }

    Unlock();
}

void GraphicViewPortClass::Clear(unsigned char color)
{
    if (Lock()) {
        Buffer_Clear(*this, color);
    }

    Unlock();
}

int GraphicViewPortClass::Blit(
    GraphicViewPortClass &vp, int src_x, int src_y, int dst_x, int dst_y, int w, int h, BOOL use_keysrc)
{
    if (Lock()) {
        if (vp.Lock()) {
            Linear_Blit_To_Linear(*this, vp, src_x, src_y, dst_x, dst_y, w, h, use_keysrc);
        }

        vp.Unlock();
    }

    Unlock();

    return 0;
}

void GraphicViewPortClass::Draw_Stamp(void *tileset, int icon, int x, int y, const void *remapper, int left, int top, int right, int bottom)
{
    if (Lock()) {
        Buffer_Draw_Stamp_Clip(*this, (IconControlType*)tileset, icon, x, y, remapper, left, top, right, bottom);
    }

    Unlock();
}

void GraphicViewPortClass::Scale(GraphicViewPortClass &vp, int src_x, int src_y, int dst_x, int dst_y, int src_w, int src_h,
    int dst_w, int dst_h, bool use_keysrc, void *fade)
{
    if (Lock()) {
        if (vp.Lock()) {
            Linear_Scale_To_Linear(*this, vp, src_x, src_y, dst_x, dst_y, src_w, src_h, dst_w, dst_h, use_keysrc, fade);
        }

        vp.Unlock();
    }

    Unlock();
}

int GraphicViewPortClass::DD_Linear_Blit_To_Linear(
    GraphicViewPortClass &viewport, int src_x, int src_y, int dst_x, int dst_y, int w, int h, BOOL use_keysrc)
{
#ifdef PLATFORM_WINDOWS
    int keysrcflag = 0;

    // Flags if we are keying based on source key colour
    if (use_keysrc) {
        keysrcflag = DDBLT_KEYSRC;
    }

    RECT srect;
    RECT drect;

    srect.left = src_x;
    srect.top = src_y;
    srect.right = src_x + w;
    srect.bottom = src_y + h;
    drect.left = dst_x;
    drect.top = dst_y;
    drect.right = dst_x + w;
    drect.bottom = dst_y + h;

    return viewport.Get_Graphic_Buffer()->Get_DD_Surface()->Blt(
        &drect, Get_Graphic_Buffer()->Get_DD_Surface(), &srect, keysrcflag | DDBLT_WAIT | DDCKEY_DESTBLT, nullptr);
#else
    return 0;
#endif
}

void GraphicViewPortClass::From_Buffer(int x, int y, int w, int h, void *buffer)
{
    if (Lock()) {
        Buffer_To_Page(x, y, w, h, buffer, *this);
    }

    Unlock();
}

void GraphicViewPortClass::To_Buffer(int x, int y, int w, int h, void *buffer, int size)
{
    if (Lock()) {
        Buffer_To_Buffer(*this, x, y, w, h, buffer, size);
    }

    Unlock();
}

unsigned GraphicViewPortClass::Print(const char *string, int x, int y, int fground, int bground)
{
    unsigned ret = 0;

    if (Lock()) {
        ret = Buffer_Print(*this, string, x, y, fground, bground);
    }

    Unlock();

    return ret;
}

GraphicBufferClass::GraphicBufferClass() : m_videoSurface(nullptr)
{
    memset(&m_surfaceInfo, 0, sizeof(m_surfaceInfo));
}

GraphicBufferClass::GraphicBufferClass(int w, int h, void *buffer, int size)
{
    Init(w, h, buffer, size);
}

GraphicBufferClass::GraphicBufferClass(int w, int h, void *buffer)
{
    Init(w, h, buffer, w * h);
}

GraphicBufferClass::GraphicBufferClass(int w, int h, GBCEnum mode)
{
    Init(w, h, nullptr, w * h, mode);
}

GraphicBufferClass::~GraphicBufferClass()
{
    Un_Init();
}

void GraphicBufferClass::DD_Init(GBCEnum mode)
{
#ifndef CHRONOSHIFT_STANDALONE
    static void (*call_ddinit)(GraphicBufferClass *, GBCEnum) =
        reinterpret_cast<void (*)(GraphicBufferClass *, GBCEnum)>(0x005C0AF4);
    call_ddinit(this, mode);
#else
    return 0;
#endif
}

void GraphicBufferClass::Attach_DD_Surface(GraphicBufferClass *buffer)
{
    m_videoSurface->AddAttachedSurface(buffer->m_videoSurface);
}

void GraphicBufferClass::Scale_Rotate(BitmapClass & bitmap, TPoint2D<int>& pivot, int scale, uint8_t angle)
{
    static const int _SineTab[256] = {
        0, 6, 12, 18, 25, 31, 37, 43, 49, 56, 62, 68, 74, 80, 86, 92,
        97, 103, 109, 115, 120, 126, 131, 136, 142, 147, 152, 157, 162, 167, 171, 176,
        180, 185, 189, 193, 197, 201, 205, 209, 212, 216, 219, 222, 225, 228, 231, 233,
        236, 238, 240, 243, 244, 246, 248, 249, 251, 252, 253, 254, 254, 255, 255, 255,
        255, 255, 255, 255, 254, 254, 253, 252, 251, 249, 248, 246, 245, 243, 241, 238,
        236, 234, 231, 228, 225, 222, 219, 216, 213, 209, 205, 201, 198, 194, 189, 185,
        181, 176, 172, 167, 162, 157, 152, 147, 142, 137, 131, 126, 120, 115, 109, 104,
        98, 92, 86, 80, 74, 68, 62, 56, 50, 44, 37, 31, 25, 19, 12, 6,
        0, -5, -12, -18, -24, -30, -37, -43, -49, -55, -61, -67, -73, -79, -85, -91,
        -97, -103, -109, -114, -120, -125, -131, -136, -141, -147, -152, -157, -162, -166, -171, -176,
        -180, -185, -189, -193, -197, -201, -205, -208, -212, -215, -219, -222, -225, -228, -231, -233,
        -236, -238, -240, -242, -244, -246, -248, -249, -250, -252, -253, -254, -254, -255, -255, -255,
        -255, -255, -255, -255, -254, -254, -253, -252, -251, -249, -248, -246, -245, -243, -241, -239,
        -236, -234, -231, -228, -226, -223, -219, -216, -213, -209, -206, -202, -198, -194, -190, -185,
        -181, -177, -172, -167, -162, -158, -153, -148, -142, -137, -132, -126, -121, -115, -110, -104,
        -98, -92, -86, -81, -75, -69, -62, -56, -50, -44, -38, -32, -25, -19, -13, -7
    };

    static const int _CosineTab[256] = {
        256, 255, 255, 255, 254, 254, 253, 252, 251, 249, 248, 246, 244, 243, 241, 238,
        236, 234, 231, 228, 225, 222, 219, 216, 212, 209, 205, 201, 197, 193, 189, 185,
        181, 176, 171, 167, 162, 157, 152, 147, 142, 137, 131, 126, 120, 115, 109, 103,
        98, 92, 86, 80, 74, 68, 62, 56, 50, 43, 37, 31, 25, 19, 12, 6,
        0, -6, -12, -18, -24, -31, -37, -43, -49, -55, -61, -68, -74, -80, -86, -91,
        -97, -103, -109, -114, -120, -125, -131, -136, -141, -147, -152, -157, -162, -166, -171, -176,
        -180, -185, -189, -193, -197, -201, -205, -209, -212, -216, -219, -222, -225, -228, -231, -233,
        -236, -238, -240, -242, -244, -246, -248, -249, -251, -252, -253, -254, -254, -255, -255, -255,
        -255, -255, -255, -255, -254, -254, -253, -252, -251, -249, -248, -246, -245, -243, -241, -239,
        -236, -234, -231, -228, -225, -222, -219, -216, -213, -209, -205, -202, -198, -194, -190, -185,
        -181, -176, -172, -167, -162, -157, -152, -147, -142, -137, -132, -126, -121, -115, -109, -104,
        -98, -92, -86, -80, -74, -68, -62, -56, -50, -44, -38, -31, -25, -19, -13, -6,
        0, 5, 11, 18, 24, 30, 36, 43, 49, 55, 61, 67, 73, 79, 85, 91,
        97, 103, 108, 114, 120, 125, 131, 136, 141, 146, 151, 156, 161, 166, 171, 176,
        180, 184, 189, 193, 197, 201, 205, 208, 212, 215, 219, 222, 225, 228, 231, 233,
        236, 238, 240, 242, 244, 246, 248, 249, 250, 252, 253, 253, 254, 255, 255, 255
    };

    // Calculate transformation values using fixed point maths.
    int cos_val = _CosineTab[angle];
    int sin_val = _SineTab[angle];
    int bmp_w_scale = (bitmap.Get_Width() * scale) / 2;
    int bmp_h_scale = (bitmap.Get_Height() * scale) / 2;
    int x_dst_start = ((((cos_val * bmp_h_scale) >> 8) - ((sin_val * bmp_w_scale) >> 8)) >> 8) + pivot.x;
    int y_dst_start = (((-(sin_val * bmp_h_scale) >> 8) - ((cos_val * bmp_w_scale) >> 8)) >> 8) + pivot.y;
    int x_tran_two = ((((cos_val * bmp_h_scale) >> 8) + ((sin_val * bmp_w_scale) >> 8)) >> 8) + pivot.x;
    int y_tran_two = (((-(sin_val * bmp_h_scale) >> 8) + ((cos_val * bmp_w_scale) >> 8)) >> 8) + pivot.y;
    int x_tran_three = (((-(cos_val * bmp_h_scale) >> 8) - ((sin_val * bmp_w_scale) >> 8)) >> 8) + pivot.x;
    int y_tran_three = ((((sin_val * bmp_h_scale) >> 8) - ((cos_val * bmp_w_scale) >> 8)) >> 8) + pivot.y;
    int x_diff_one = x_tran_two - x_dst_start;
    int y_diff_one = y_tran_two - y_dst_start;
    int x_diff_two = x_tran_three - x_dst_start;
    int y_diff_two = y_tran_three - y_dst_start;
    int adj_w_one = m_width;
    int adj_w_two = m_width;
    int x_inc_one = 1;
    int x_inc_two = 1;
    int inc_counter_one = 0;
    int inc_counter_two = 0;
    int pos_one = 0;
    int pos_two = 0;
    int src_pos = 0;
    int dst_pos = 0;
    uint8_t *src_ptr = static_cast<uint8_t*>(bitmap.Get_Bitmap());
    uint8_t *dst_ptr = m_graphicBuffer.Get_Buffer();

    // Perform adjustments different rotation directions?
    if (y_diff_one < 0) {
        y_diff_one = -y_diff_one;
        adj_w_one = -m_width;
    }

    if (x_diff_one < 0) {
        x_diff_one = -x_diff_one;
        x_inc_one = -1;
    }

    if (y_diff_two < 0) {
        y_diff_two = -y_diff_two;
        adj_w_two = -m_width;
    }

    if (x_diff_two < 0) {
        x_diff_two = -x_diff_two;
        x_inc_two = -1;
    }

    dst_pos = y_dst_start * m_width + x_dst_start;
    
    // Decide which way we are rotating?
    if (x_diff_one > y_diff_one) {
        for (int k = 0; ; ++k) {
            if (k >= y_diff_two) {
                break;
            }

            int saved_dst_pos = dst_pos;

            for (int l = 0; l < x_diff_one; ++l) {
                uint8_t tmp = src_ptr[src_pos];

                if (tmp != 0) {
                    dst_ptr[dst_pos] = tmp;
                }

                for (inc_counter_one += bitmap.Get_Width(); inc_counter_one > x_diff_one; inc_counter_one -= x_diff_one) {
                    ++src_pos;
                }

                dst_pos += x_inc_one;
                pos_one += y_diff_one;

                if (pos_one > x_diff_one) {
                    if (tmp) {
                        dst_ptr[dst_pos] = tmp;
                    }

                    pos_one -= x_diff_one;
                    dst_pos += adj_w_one;
                }
            }

            inc_counter_one = 0;
            src_pos -= bitmap.Get_Width() - 1;

            for (inc_counter_two += bitmap.Get_Height(); inc_counter_two > y_diff_two; inc_counter_two -= y_diff_two) {
                src_pos += bitmap.Get_Width();
            }

            pos_one = 0;
            dst_pos = adj_w_two + saved_dst_pos;
            pos_two += x_diff_two;

            if (pos_two > y_diff_two) {
                pos_two -= y_diff_two;
                dst_pos += x_inc_two;
            }
        }
    } else {
        for (int i = 0; ; ++i) {
            if (i >= x_diff_two) {
                break;
            }

            int saved_dst_pos = dst_pos;

            for (int j = 0; j < y_diff_one; ++j) {
                uint8_t tmp = src_ptr[src_pos];

                if (tmp) {
                    dst_ptr[dst_pos] = tmp;
                }

                for (inc_counter_one += bitmap.Get_Width(); inc_counter_one > y_diff_one; inc_counter_one -= y_diff_one) {
                    ++src_pos;
                }

                dst_pos += adj_w_one;
                pos_one += x_diff_one;

                if (pos_one > y_diff_one) {
                    if (tmp) {
                        dst_ptr[dst_pos] = tmp;
                    }

                    pos_one -= y_diff_one;
                    dst_pos += x_inc_one;
                }
            }

            inc_counter_one = 0;
            src_pos -= bitmap.Get_Width() - 1;

            for (inc_counter_two += bitmap.Get_Height(); inc_counter_two > x_diff_two; inc_counter_two -= x_diff_two) {
                src_pos += bitmap.Get_Width();
            }

            dst_pos = x_inc_two + saved_dst_pos;
            pos_two += y_diff_two;
            pos_one = 0;

            if (pos_two > x_diff_two) {
                pos_two -= x_diff_two;
                dst_pos += adj_w_two;
            }
        }
    }
}

void GraphicBufferClass::Init(int width, int height, void *buffer, int size, GBCEnum mode)
{
    m_graphicBuffer.Set_Size(size);
    m_width = width;
    m_height = height;

    if ((mode & GBC_VIDEO_MEM) != 0) {
        DD_Init(mode);
    } else {
        if (buffer) {
            m_graphicBuffer.Set_Buffer(buffer);
            m_graphicBuffer.Set_Allocated(false);
        } else {
            if (!m_graphicBuffer.Get_Size()) {
                m_graphicBuffer.Set_Size(width * height);
            }

            m_graphicBuffer.Set_Buffer(new char[m_graphicBuffer.Get_Size()]);
            m_graphicBuffer.Set_Allocated(true);
        }

        m_offset = m_graphicBuffer.Get_Buffer();
        m_inVideoMemory = false;
    }

    m_pitch = 0;
    m_xAdd = 0;
    m_xPos = 0;
    m_yPos = 0;
    m_graphicBuff = this;
}

void GraphicBufferClass::Un_Init()
{
#ifndef CHRONOSHIFT_STANDALONE
    static void (*call_uninit)(GraphicBufferClass *) = reinterpret_cast<void (*)(GraphicBufferClass *)>(0x005C0D0F);
    call_uninit(this);
#else
    return 0;
#endif
}

// Lock and Unlock need WWMouse and SurfaceMonitorClass classes.
BOOL GraphicBufferClass::Lock()
{
#ifndef CHRONOSHIFT_STANDALONE
    static BOOL (*call_lock)(GraphicBufferClass *) = reinterpret_cast<BOOL (*)(GraphicBufferClass *)>(0x005C101A);
    return call_lock(this);
#else
    return 0;
#endif
}

BOOL GraphicBufferClass::Unlock()
{
#ifndef CHRONOSHIFT_STANDALONE
    static BOOL (*call_unlock)(GraphicBufferClass *) = reinterpret_cast<BOOL (*)(GraphicBufferClass *)>(0x005C1191);
    return call_unlock(this);
#else
    return 0;
#endif
}
