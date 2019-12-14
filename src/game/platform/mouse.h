/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class for handling the low level mouse IO and drawing.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef MOUSE_H
#define MOUSE_H

#include "always.h"
class GraphicViewPortClass;
class GraphicBufferClass;

#ifdef PLATFORM_WINDOWS
#include <mmsystem.h>
#endif

class MouseClass
{
public:
    MouseClass(GraphicViewPortClass *scr, int mouse_max_width, int mouse_max_height);
    ~MouseClass();

    int Get_Mouse_X();
    int Get_Mouse_Y();
    void Get_Mouse_XY(int &x_pos, int &y_pos);
    GraphicViewPortClass *Get_Screen() { return m_Screen; }
    void Set_Mouse_X(int x);
    void Set_Mouse_Y(int y);
    BOOL Set_Mouse_XY(int x, int y);
    int Get_Mouse_State() { return m_State; }
    void Block_Mouse(GraphicBufferClass *gbuffer);
    void Unblock_Mouse(GraphicBufferClass *gbuffer);
    void Set_Cursor_Clip();
    void Clear_Cursor_Clip();
    void Process_Mouse();
    void *Set_Cursor(int xhotspot, int yhotspot, void *cursor);
    void Low_Hide_Mouse();
    void Hide_Mouse();
    void Low_Show_Mouse(int x, int y);
    void Show_Mouse();
    void Conditional_Hide_Mouse(int x1, int y1, int x2, int y2);
    void Conditional_Show_Mouse();
    void Draw_Mouse(GraphicViewPortClass &scr);
    void Erase_Mouse(GraphicViewPortClass &scr, bool forced = false);
    uint8_t *Get_Image_Buff() { return m_MouseCursor; }
    int Get_Max_Width() { return m_MaxWidth; }
    int Get_Max_Height() { return m_MaxHeight; }
    int Get_Cursor_Width() { return m_CursorWidth; }
    void Set_Cursor_Width(int width) { m_CursorWidth = width; }
    int Get_Cursor_Height() { return m_CursorHeight; }
    void Set_Cursor_Height(int height) { m_CursorHeight = height; }
    int Get_Hotspot_X() { return m_MouseHotX; }
    void Set_Hotspot_X(int x_pos) { m_MouseHotX = x_pos; }
    int Get_Hotspot_Y() { return m_MouseHotY; }
    void Set_Hotspot_Y(int y_pos) { m_MouseHotY = y_pos; }
    void *Get_Frame_Pointer() { return m_PrevCursor; }
    void Set_Frame_Pointer(void *frame) { m_PrevCursor = frame; }

private:
    uint8_t *m_MouseCursor;
    int m_MouseHotX;
    int m_MouseHotY;
    int m_CursorWidth;
    int m_CursorHeight;
    uint8_t *m_MouseBuffer;
    int m_BuffX;
    int m_BuffY;
    int m_MaxWidth;
    int m_MaxHeight;
    int m_MouseCXLeft;
    int m_MouseCYUpper;
    int m_MouseCXRight;
    int m_MouseCYLower;
    uint8_t m_CFlags;
    uint8_t m_CCount;
    GraphicViewPortClass *m_Screen;
    void *m_PrevCursor;
    int m_MouseUpdate;
    int m_State;
    uint8_t *m_EraseBuffer;
    int m_EraseBuffX;
    int m_EraseBuffY;
    int m_EraseBuffHotX;
    int m_EraseBuffHotY;
    int m_EraseFlags;
#ifdef PLATFORM_WINDOWS
    CRITICAL_SECTION m_MouseCritSec;
    MMRESULT m_TimerHandle;
#endif
};

#ifdef PLATFORM_WINDOWS
void __stdcall Process_Mouse(
    unsigned int uDelay, unsigned int uResolution, unsigned long fptc, unsigned long dwUser, unsigned long fuEvent);
#endif

// Global mouse instance.
#ifdef GAME_DLL
extern MouseClass *&g_Mouse;
extern MouseClass *&g_WWMouse;
#else
extern MouseClass *g_Mouse;
extern MouseClass *g_WWMouse;
#endif

#endif // MOUSE_H
