/**
 * @file
 *
 * @Author CCHyper, OmniBlade
 *
 * @brief Wrapper class for platform drawing engine.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "gbuffer.h"
#include "gamedebug.h"
#include "blitters.h"
#include "minmax.h"
#include "textprint.h"

#ifndef RAPP_STANDALONE
// BOOL &GraphicViewPortClass::AllowHardwareBlitFills;
// BOOL &GraphicViewPortClass::AllowStretchBlits;
int &GraphicViewPortClass::ScreenWidth = Make_Global<int>(0x006016B0);
int &GraphicViewPortClass::ScreenHeight = Make_Global<int>(0x006016B4);
GraphicViewPortClass *&g_logicPage = Make_Global<GraphicViewPortClass *>(0x006AC274);
LPDIRECTDRAWSURFACE &g_paletteSurface = Make_Global<LPDIRECTDRAWSURFACE>(0x006B18A4);
GraphicViewPortClass &g_seenBuff = Make_Global<GraphicViewPortClass>(0x006807A4);
#else
BOOL GraphicViewPortClass::AllowHardwareBlitFills;
BOOL GraphicViewPortClass::AllowStretchBlits;
int GraphicViewPortClass::ScreenWidth = 640;
int GraphicViewPortClass::ScreenHeight = 400;
GraphicViewPortClass *g_logicPage = nullptr;
LPDIRECTDRAWSURFACE g_paletteSurface = nullptr;
GraphicViewPortClass g_seenBuff;
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

unsigned GraphicViewPortClass::Print(char *string, int x, int y, int fground, int bground)
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
#ifndef RAPP_STANDALONE
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
#ifndef RAPP_STANDALONE
    static void (*call_uninit)(GraphicBufferClass *) = reinterpret_cast<void (*)(GraphicBufferClass *)>(0x005C0D0F);
    call_uninit(this);
#else
    return 0;
#endif
}

// Lock and Unlock need WWMouse and SurfaceMonitorClass classes.
BOOL GraphicBufferClass::Lock()
{
#ifndef RAPP_STANDALONE
    static BOOL (*call_lock)(GraphicBufferClass *) = reinterpret_cast<BOOL (*)(GraphicBufferClass *)>(0x005C101A);
    return call_lock(this);
#else
    return 0;
#endif
}

BOOL GraphicBufferClass::Unlock()
{
#ifndef RAPP_STANDALONE
    static BOOL (*call_unlock)(GraphicBufferClass *) = reinterpret_cast<BOOL (*)(GraphicBufferClass *)>(0x005C1191);
    return call_unlock(this);
#else
    return 0;
#endif
}
