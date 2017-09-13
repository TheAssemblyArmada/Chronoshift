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
#include "blitters.h"
#include "minmax.h"

#ifndef RAPP_STANDALONE
 //BOOL &GraphicViewPortClass::AllowHardwareBlitFills;
 //BOOL &GraphicViewPortClass::AllowStretchBlits;
int &GraphicViewPortClass::ScreenWidth = Make_Global<int>(0x006016B0);
int &GraphicViewPortClass::ScreenHeight = Make_Global<int>(0x006016B4);
#else
BOOL GraphicViewPortClass::AllowHardwareBlitFills;
BOOL GraphicViewPortClass::AllowStretchBlits;
int GraphicViewPortClass::ScreenWidth = 640;
int GraphicViewPortClass::ScreenHeight = 400;
#endif

GraphicViewPortClass::GraphicViewPortClass(GraphicBufferClass *buffer, int x, int y, int w, int h) : 
    GraphicBuff(nullptr),
    LockCount(0)
{
    Attach(buffer, x, y, w, h);
}

GraphicViewPortClass::~GraphicViewPortClass()
{
    Offset = nullptr;
    Width = 0;
    Height = 0;
    XAdd = 0;
    XPos = 0;
    YPos = 0;
    Pitch = 0;
    InVideoMemory = false;
    LockCount = 0;
    GraphicBuff = nullptr;
}

void GraphicViewPortClass::Attach(GraphicBufferClass *buffer, int x, int y, int w, int h)
{
    if (buffer != this) {
        x = Clamp(x, 0, buffer->Get_Width() - 1); //Min(Max(0, x), buffer->Get_Width() - 1);
        y = Clamp(y, 0, buffer->Get_Height() - 1); //Min(Max(0, y), buffer->Get_Height() - 1);
        w = x + w > buffer->Get_Width() ? buffer->Get_Width() - x : w;
        h = y + h > buffer->Get_Height() ? buffer->Get_Height() - y : h;

        int scanline = y * (buffer->Get_Width() + buffer->Get_Pitch());

        XPos = x;
        YPos = y;

        Offset = x + scanline + static_cast<char *>(buffer->Get_Offset());
        Width = w;
        XAdd = buffer->Get_Width() - w;
        Height = h;
        Pitch = buffer->Get_Pitch();
        GraphicBuff = buffer;
        InVideoMemory = buffer->In_Video_Memory();
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
    if (GraphicBuff->Lock()) {
        if (GraphicBuff != this) {
            Attach(GraphicBuff, XPos, YPos, Width, Height);
        }

        return true;
    }

    return false;
}

BOOL GraphicViewPortClass::Unlock()
{
    if (GraphicBuff->Unlock()) {
        if (GraphicBuff != this && InVideoMemory && !GraphicBuff->Get_LockCount()) {
            Offset = nullptr;
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

unsigned char GraphicViewPortClass::Get_Pixel(int x, int y)
{
    uint8_t val = 0;

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

int GraphicViewPortClass::Blit(GraphicViewPortClass &viewport, int src_x, int src_y, int dst_x, int dst_y, int w, int h,
    BOOL use_keysrc)
{
    return 0;
}
