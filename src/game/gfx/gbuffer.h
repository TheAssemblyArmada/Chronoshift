/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Wrapper class for platform drawing engine.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef GBUFFER_H
#define GBUFFER_H

#include "always.h"
#include "buffer.h"
#include "tpoint.h"

#ifdef BUILD_WITH_DDRAW
#define DIRECTDRAW_VERSION 0x300
#include <ddraw.h>
#endif

// Flags for using DD surfaces in GBC
enum GBCEnum
{
    GBC_NONE = 0,
    GBC_VIDEO_MEM = 1,
    GBC_VISIBLE = 2,
};
DEFINE_ENUMERATION_BITWISE_OPERATORS(GBCEnum);

#define BLIT_GET_PITCH(view) (view.Get_Full_Pitch())

class GraphicBufferClass;

class BitmapClass
{
public:
    BitmapClass(int w, int h, void *bitmap) : m_Width(w), m_Height(h), m_Data(bitmap) {}
    BitmapClass(const BitmapClass &that) : m_Width(that.m_Width), m_Height(that.m_Height), m_Data(that.m_Data){};

    void Set_Dimensions(int w, int h)
    {
        m_Width = w;
        m_Height = h;
    }
    int Get_Width() { return m_Width; }
    int Get_Height() { return m_Height; }
    void *Get_Bitmap() { return m_Data; }

private:
    int m_Width;
    int m_Height;
    void *m_Data;
};

class GraphicViewPortClass
{
public:
    GraphicViewPortClass() {}
    GraphicViewPortClass(GraphicBufferClass *buffer, int x, int y, int w, int h);
    ~GraphicViewPortClass();

    void Attach(GraphicBufferClass *buffer, int x, int y, int w, int h);
    BOOL Change(int x, int y, int w, int h);
    BOOL Lock();
    BOOL Unlock();
    void *Get_Offset() { return m_Offset; }
    int Get_XAdd() { return m_XAdd; }
    int Get_XPos() { return m_XPos; }
    int Get_YPos() { return m_YPos; }
    int Get_Width() { return m_Width; }
    int Get_Height() { return m_Height; }
    int Get_Pitch() { return m_Pitch; }
    int Get_LockCount() { return m_LockCount; }
    int Get_Full_Pitch() { return Get_Pitch() + Get_XAdd() + Get_Width(); }
    BOOL In_Video_Memory() { return m_InVideoMemory; }
    GraphicBufferClass *Get_Graphic_Buffer() { return m_GraphicBuff; }

    void Set_XY_Pos(int x, int y)
    {
        m_XPos = x;
        m_YPos = y;
    }
    void Put_Pixel(int x, int y, unsigned char value);
    void Put_Fat_Pixel(int x, int y, int size, unsigned char value);
    unsigned Get_Pixel(int x, int y);
    void Remap(int x, int y, int w, int h, unsigned char *fading_table);
    void Draw_Rect(int x, int y, int w, int h, unsigned char color = 0);
    void Draw_Line(int x1, int y1, int x2, int y2, unsigned char color = 0);
    void Fill_Rect(int x, int y, int w, int h, unsigned char color);
    unsigned Print(const char *string, int x, int y, int fground, int bground);
    void Clear(unsigned char color = 0);
    void From_Buffer(int x, int y, int w, int h, void *buffer);
    void To_Buffer(int x, int y, int w, int h, void *buffer, int size);
    void Scale(GraphicViewPortClass &view, BOOL use_keysrc = false, void *fade = nullptr);
    void Scale(GraphicViewPortClass &view, int src_x, int src_y, int dst_x, int dst_y, int src_w, int src_h, int dst_w,
        int dst_h, BOOL use_keysrc = false, void *fade = nullptr);
    int Blit(GraphicViewPortClass &view, BOOL use_keysrc = false);
    int Blit(GraphicViewPortClass &view, int src_x, int src_y, int dst_x, int dst_y, int w, int h, BOOL use_keysrc = false);
    void Draw_Stamp(void *tileset, int icon, int x, int y, const void *remapper, int left, int top, int right, int bottom);

#ifdef GAME_DLL
    // static BOOL &AllowHardwareBlitFills;
    // static BOOL &AllowStretchBlits;
    static int &ScreenWidth;
    static int &ScreenHeight;
#else
    static BOOL AllowHardwareBlitFills;
    static BOOL AllowStretchBlits;
    static int ScreenWidth;
    static int ScreenHeight;
#endif

private:
    int DD_Linear_Blit_To_Linear(
        GraphicViewPortClass &view, int src_x, int src_y, int dst_x, int dst_y, int w, int h, BOOL use_keysrc = false);

protected:
    void *m_Offset;
    int m_Width;
    int m_Height;
    int m_XAdd;
    int m_XPos;
    int m_YPos;
    int m_Pitch;
    GraphicBufferClass *m_GraphicBuff;
    BOOL m_InVideoMemory;
    int m_LockCount;
};

class GraphicBufferClass : public GraphicViewPortClass
{
public:
    GraphicBufferClass();
    GraphicBufferClass(int w, int h, void *buffer, int size);
    GraphicBufferClass(int w, int h, void *buffer);
    GraphicBufferClass(int w, int h, GBCEnum mode = GBC_NONE);
    ~GraphicBufferClass();

    BufferClass *Get_GBuffer() { return &m_GraphicBuffer; }

    void DD_Init(GBCEnum mode);
#ifdef BUILD_WITH_DDRAW
    LPDIRECTDRAWSURFACE Get_DD_Surface() { return m_VideoSurface; }
#endif

    void Attach_DD_Surface(GraphicBufferClass *buffer);

    // needs defualt angle
    void Scale_Rotate(BitmapClass &bitmap, TPoint2D<int> &pivot, int scale, uint8_t angle);

    void Init(int width, int height, void *buffer, int size, GBCEnum mode = GBC_NONE);
    void Un_Init();

    BOOL Lock();
    BOOL Unlock();

    static int TotalLocks;

private:
    BufferClass m_GraphicBuffer;
#ifdef BUILD_WITH_DDRAW
    LPDIRECTDRAWSURFACE m_VideoSurface;
    DDSURFACEDESC m_SurfaceInfo;
#endif
};

void Wait_Blit();

GraphicViewPortClass *Set_Logic_Page(GraphicViewPortClass *view);
GraphicViewPortClass *Set_Logic_Page(GraphicViewPortClass &view);

#ifdef GAME_DLL
extern GraphicViewPortClass *&g_logicPage;
extern GraphicViewPortClass &g_seenBuff;
extern GraphicViewPortClass &g_hidPage;
extern GraphicBufferClass &g_visiblePage;
extern GraphicBufferClass &g_hiddenPage;
extern GraphicBufferClass &g_sysMemPage;
extern GraphicBufferClass &g_modeXBuff;
#else
extern GraphicViewPortClass *g_logicPage;
extern GraphicViewPortClass g_seenBuff;
extern GraphicViewPortClass g_hidPage;
extern GraphicBufferClass g_visiblePage;
extern GraphicBufferClass g_hiddenPage;
extern GraphicBufferClass g_sysMemPage;
extern GraphicBufferClass g_modeXBuff;
#endif

#endif // GBUFFER_H
