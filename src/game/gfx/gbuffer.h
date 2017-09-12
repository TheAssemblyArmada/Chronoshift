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
#pragma once

#ifndef GBUFFER_H
#define GBUFFER_H

#include "always.h"
#include "buffer.h"

#ifdef PLATFORM_WINDOWS
#include "ddraw.h"
#endif

#ifndef RAPP_STANDALONE
#include "hooker.h"
#endif

// Flags for using DD surfaces in GBC
enum GBC_Enum
{
    GBC_DD_SOFTWARE_BUFF = 0,
    GBC_DD_SURFACE_BUFFER = 1,
    GBC_DD_PRIMARY_SURFACE = 2,
};

#define BLIT_GET_PITCH(vp) (vp.Get_Full_Pitch())

class GraphicBufferClass;

class GBufferBitmapClass
{
public:
    GBufferBitmapClass(int w, int h, void *bitmap) : Width(w), Height(h), Data(bitmap) {}
    GBufferBitmapClass(const GBufferBitmapClass &that);

    ~GBufferBitmapClass() {}

    void Set_Dimensions(int w, int h);

    int Get_Width() { return Width; }
    int Get_Height() { return Height; }
    void *Get_Bitmap() { return Data; }

protected:
    int Width;
    int Height;
    void *Data;
};

class GraphicViewPortClass
{
public:
    GraphicViewPortClass();
    GraphicViewPortClass(GraphicBufferClass *buffer, int x_pos, int y_pos, int width, int height);
    ~GraphicViewPortClass();

    void Attach(GraphicBufferClass *buffer, int x_pos, int y_pos, int width, int height);
    BOOL Change(int x_pos, int y_pos, int width, int height);
    BOOL Lock();
    BOOL Unlock();
    void *Get_Offset() { return Offset; }
    int Get_XAdd() { return XAdd; }
    int Get_XPos() { return XPos; }
    int Get_YPos() { return YPos; }
    int Get_Width() { return Width; }
    int Get_Height() { return Height; }
    int Get_Pitch() { return Pitch; }
    int Get_LockCount() { return LockCount; }
    int Get_Full_Pitch() { return Get_Pitch() + Get_XAdd() + Get_Width(); }
    BOOL In_Video_Memory() { return InVideoMemory; }
    GraphicBufferClass *Get_Graphic_Buffer() { return GraphicBuff; }

    void Set_XY_Pos(int x, int y) { XPos = x; YPos = y; }
    void Put_Pixel(int x, int y, unsigned char value);
    int Get_Pixel(int x, int y);
    void Remap(int x, int y, int w, int h, unsigned char *fading_table);
    void Draw_Rect(int x_pos, int y_pos, int width, int height, unsigned char color = 0);
    void Draw_Line(int x1_pos, int y1_pos, int x2_pos, int y2_pos, unsigned char color = 0);
    void Fill_Rect(int x_pos, int y_pos, int width, int height, unsigned char color);
    void Overlay_Fill_Rect(int x_pos, int y_pos, int width, int height, unsigned char color);
    unsigned int Print(char *string, int x_pos, int y_pos, int a4, int a5);
    void Clear(unsigned char color = 0);

    int Blit(GraphicViewPortClass &viewport, int src_x_pos, int src_y_pos, int dest_x_pos, int dest_y_pos, int width,
        int height, BOOL use_keysrc = false);

    int Full_Blit(GraphicViewPortClass &viewport, BOOL use_keysrc = false);

#ifndef RAPP_STANDALONE
    //static BOOL &AllowHardwareBlitFills;
    //static BOOL &AllowStretchBlits;
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
    int DD_Linear_Blit_To_Linear(GraphicViewPortClass &viewport, int src_x_pos, int src_y_pos, int dest_x_pos,
        int dest_y_pos, int width, int height, BOOL use_keysrc = false);

protected:
    void *Offset;
    int Width;
    int Height;
    int XAdd;
    int XPos;
    int YPos;
    int Pitch;
    GraphicBufferClass *GraphicBuff;
    BOOL InVideoMemory;
    int LockCount;
};

class GraphicBufferClass : public GraphicViewPortClass
{
public:
    GraphicBufferClass();
    GraphicBufferClass(int width, int height, void *buffer, int size);
    GraphicBufferClass(int width, int height, void *buffer);
    GraphicBufferClass(int width, int height, GBC_Enum mode = GBC_DD_SOFTWARE_BUFF);
    ~GraphicBufferClass();

    BufferClass *Get_GBuffer() { return &GBuffer; }

    void DD_Init(GBC_Enum mode);
#ifdef PLATFORM_WINDOWS
    LPDIRECTDRAWSURFACE Get_DD_Surface() { return DDSurface; }
#endif

    void Attach_DD_Surface(GraphicBufferClass *buffer);

    // needs defualt angle
    //void Scale_Rotate(GBufferBitmapClass &input_bitmap, TPoint2D<int> &input_point, int scale, char angle);

    void Init(int width, int height, void *buffer, int size, GBC_Enum = GBC_DD_SOFTWARE_BUFF);
    void Un_Init();

    BOOL Lock();
    BOOL Unlock();

    static int TotalLocks;

private:
    BufferClass GBuffer;
#ifdef PLATFORM_WINDOWS
    LPDIRECTDRAWSURFACE DDSurface;
    DDSURFACEDESC SurfaceInfo;
#endif
};

GraphicViewPortClass *Set_Logic_Page(GraphicViewPortClass *vp);
GraphicViewPortClass *Set_Logic_Page(GraphicViewPortClass &vp);

#endif // GBUFFER_H
