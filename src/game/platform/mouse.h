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
#include "gbuffer.h"

#ifdef PLATFORM_WINDOWS
#include <mmsystem.h>
#endif

class WWMouseClass
{
public:
    WWMouseClass(GraphicViewPortClass *scr, int mouse_max_width, int mouse_max_height);
    ~WWMouseClass();

    int Get_Mouse_X();
    int Get_Mouse_Y();
    void Get_Mouse_XY(int &x_pos, int &y_pos);
    GraphicViewPortClass *Get_Screen() { return m_screen; }
    void Set_Mouse_X(int x);
    void Set_Mouse_Y(int y);
    BOOL Set_Mouse_XY(int x, int y);
    int Get_Mouse_State() { return m_state; }
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
    uint8_t *Get_Image_Buff() { return m_mouseCursor; }
    int Get_Max_Width() { return m_maxWidth; }
    int Get_Max_Height() { return m_maxHeight; }
    int Get_Cursor_Width() { return m_cursorWidth; }
    void Set_Cursor_Width(int width) { m_cursorWidth = width; }
    int Get_Cursor_Height() { return m_cursorHeight; }
    void Set_Cursor_Height(int height) { m_cursorHeight = height; }
    int Get_Hotspot_X() { return m_mouseHotX; }
    void Set_Hotspot_X(int x_pos) { m_mouseHotX = x_pos; }
    int Get_Hotspot_Y() { return m_mouseHotY; }
    void Set_Hotspot_Y(int y_pos) { m_mouseHotY = y_pos; }
    void *Get_Frame_Pointer() { return m_prevCursor; }
    void Set_Frame_Pointer(void *frame) { m_prevCursor = frame; }
#ifndef CHRONOSHIFT_STANDALONE
    static void Hook_Me();
    static void Hook_Low_Hide(WWMouseClass *ptr);
    static void Hook_Low_Show(WWMouseClass *ptr, int x, int y);
#endif

private:
    uint8_t *m_mouseCursor;
    int m_mouseHotX;
    int m_mouseHotY;
    int m_cursorWidth;
    int m_cursorHeight;
    uint8_t *m_mouseBuffer;
    int m_buffX;
    int m_buffY;
    int m_maxWidth;
    int m_maxHeight;
    int m_mouseCXLeft;
    int m_mouseCYUpper;
    int m_mouseCXRight;
    int m_mouseCYLower;
    uint8_t m_cFlags;
    uint8_t m_cCount;
    GraphicViewPortClass *m_screen;
    void *m_prevCursor;
    int m_mouseUpdate;
    int m_state;
    uint8_t *m_eraseBuffer;
    int m_eraseBuffX;
    int m_eraseBuffY;
    int m_eraseBuffHotX;
    int m_eraseBuffHotY;
    int m_eraseFlags;
#ifdef PLATFORM_WINDOWS
    CRITICAL_SECTION m_mouseCritSec;
    MMRESULT m_timerHandle;
#endif
};

#ifdef PLATFORM_WINDOWS
void __stdcall Process_Mouse(
    unsigned int uDelay, unsigned int uResolution, unsigned long fptc, unsigned long dwUser, unsigned long fuEvent);
#endif

// Global mouse instance.
#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"

inline void WWMouseClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    //Hook_Function(0x005C2180, ::Process_Mouse);
    Hook_Function(0x005C1B10, Hook_Low_Hide);
    Hook_Function(0x005C1BF0, Hook_Low_Show);
#endif
}

extern WWMouseClass *&g_mouse;
extern WWMouseClass *&g_wwmouse;
#else
extern WWMouseClass *g_mouse;
extern WWMouseClass *g_wwmouse;
#endif

#endif // MOUSE_H
