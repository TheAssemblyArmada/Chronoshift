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
#pragma once

#ifndef GBUFFER_H
#define GBUFFER_H

#include "always.h"
#include "buffer.h"
#include "tpoint.h"

#ifdef PLATFORM_WINDOWS
#include "ddraw.h"
#endif

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
#endif

// Flags for using DD surfaces in GBC
enum GBCEnum
{
    GBC_NONE = 0,
    GBC_VIDEO_MEM = 1,
    GBC_VISIBLE = 2,
};

#define BLIT_GET_PITCH(vp) (vp.Get_Full_Pitch())

class GraphicBufferClass;

class BitmapClass
{
public:
    BitmapClass(int w, int h, void *bitmap) : m_width(w), m_height(h), m_data(bitmap) {}
    BitmapClass(const BitmapClass &that) : m_width(that.m_width), m_height(that.m_height), m_data(that.m_data){};

    void Set_Dimensions(int w, int h)
    {
        m_width = w;
        m_height = h;
    }
    int Get_Width() { return m_width; }
    int Get_Height() { return m_height; }
    void *Get_Bitmap() { return m_data; }

private:
    int m_width;
    int m_height;
    void *m_data;
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
    void *Get_Offset() { return m_offset; }
    int Get_XAdd() { return m_xAdd; }
    int Get_XPos() { return m_xPos; }
    int Get_YPos() { return m_yPos; }
    int Get_Width() { return m_width; }
    int Get_Height() { return m_height; }
    int Get_Pitch() { return m_pitch; }
    int Get_LockCount() { return m_lockCount; }
    int Get_Full_Pitch() { return Get_Pitch() + Get_XAdd() + Get_Width(); }
    BOOL In_Video_Memory() { return m_inVideoMemory; }
    GraphicBufferClass *Get_Graphic_Buffer() { return m_graphicBuff; }

    void Set_XY_Pos(int x, int y)
    {
        m_xPos = x;
        m_yPos = y;
    }
    void Put_Pixel(int x, int y, unsigned char value);
    unsigned Get_Pixel(int x, int y);
    void Remap(int x, int y, int w, int h, unsigned char *fading_table);
    void Draw_Rect(int x, int y, int w, int h, unsigned char color = 0);
    void Draw_Line(int x1, int y1, int x2, int y2, unsigned char color = 0);
    void Fill_Rect(int x, int y, int w, int h, unsigned char color);
    // void Overlay_Fill_Rect(int x_pos, int y_pos, int width, int height, unsigned char color);
    unsigned Print(const char *string, int x, int y, int fground, int bground);
    void Clear(unsigned char color = 0);
    void From_Buffer(int x, int y, int w, int h, void *buffer);
    void To_Buffer(int x, int y, int w, int h, void *buffer, int size);
    void Scale(GraphicViewPortClass &vp, int src_x, int src_y, int dst_x, int dst_y, int src_w, int src_h, int dst_w,
        int dst_h, bool use_keysrc, void *fade);
    int Blit(GraphicViewPortClass &vp, int src_x, int src_y, int dst_x, int dst_y, int w, int h, BOOL use_keysrc = false);
    void Draw_Stamp(void *tileset, int icon, int x, int y, void const *remapper, int left, int top, int right, int bottom);
    // int Full_Blit(GraphicViewPortClass &viewport, BOOL use_keysrc = false);

#ifndef CHRONOSHIFT_STANDALONE
    static void Hook_Me();
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
    // This one should only be called from Blit or Full_Blit and should probably be a protected function
    int DD_Linear_Blit_To_Linear(
        GraphicViewPortClass &viewport, int src_x, int src_y, int dst_x, int dst_y, int w, int h, BOOL use_keysrc = false);

protected:
    void *m_offset;
    int m_width;
    int m_height;
    int m_xAdd;
    int m_xPos;
    int m_yPos;
    int m_pitch;
    GraphicBufferClass *m_graphicBuff;
    BOOL m_inVideoMemory;
    int m_lockCount;
};

class GraphicBufferClass : public GraphicViewPortClass
{
public:
    GraphicBufferClass();
    GraphicBufferClass(int w, int h, void *buffer, int size);
    GraphicBufferClass(int w, int h, void *buffer);
    GraphicBufferClass(int w, int h, GBCEnum mode = GBC_NONE);
    ~GraphicBufferClass();

    BufferClass *Get_GBuffer() { return &m_graphicBuffer; }

    void DD_Init(GBCEnum mode);
#ifdef PLATFORM_WINDOWS
    LPDIRECTDRAWSURFACE Get_DD_Surface() { return m_videoSurface; }
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
    BufferClass m_graphicBuffer;
#ifdef PLATFORM_WINDOWS
    LPDIRECTDRAWSURFACE m_videoSurface;
    DDSURFACEDESC m_surfaceInfo;
#endif
};

void Wait_Blit();

GraphicViewPortClass *Set_Logic_Page(GraphicViewPortClass *vp);
GraphicViewPortClass *Set_Logic_Page(GraphicViewPortClass &vp);

#ifndef CHRONOSHIFT_STANDALONE
inline void GraphicViewPortClass::Hook_Me() {}

extern GraphicViewPortClass *&g_logicPage;
extern LPDIRECTDRAWSURFACE &g_paletteSurface;
extern GraphicViewPortClass &g_seenBuff;
extern GraphicViewPortClass &g_hidPage;
#else
extern GraphicViewPortClass *g_logicPage;
extern LPDIRECTDRAWSURFACE g_paletteSurface;
extern GraphicViewPortClass g_seenBuff;
extern GraphicViewPortClass g_hidPage;
#endif

#endif // GBUFFER_H
