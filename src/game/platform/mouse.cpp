/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class for handling the low level mouse IO and drawing.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "mouse.h"
#include "gamedebug.h"
#include "gbuffer.h"
#include "globals.h"
#include "minmax.h"
#include "mouseshape.h"

#ifdef PLATFORM_WINDOWS
// extern HWND MainWindow;
#endif

#ifndef RAPP_STANDALONE
WWMouseClass *&g_mouse = Make_Global<WWMouseClass *>(0x006AC284);

void WWMouseClass::Hook_Low_Hide(WWMouseClass *ptr)
{
    ptr->WWMouseClass::Low_Hide_Mouse();
}

void WWMouseClass::Hook_Low_Show(WWMouseClass *ptr, int x, int y)
{
    ptr->WWMouseClass::Low_Show_Mouse(x, y);
}
#else
WWMouseClass *g_mouse = nullptr;
#endif
//
// Frequency to update the mouse in ms
//
static int const MOUSE_UPDATE_FREQ = 16;

// This callback is called by a timer thread periodically and asyncronously,
// it causes the mouse instance Process_Mouse() function to be called provided
// it isn't already in the middle of it.
#ifdef PLATFORM_WINDOWS
void __stdcall Process_Mouse(
    unsigned int uDelay, unsigned int uResolution, unsigned long fptc, unsigned long dwUser, unsigned long fuEvent)
#endif
{
    static bool InMouseCallback;

    if (g_mouse != nullptr && !InMouseCallback) {
        InMouseCallback = true;
        g_mouse->Process_Mouse();
        InMouseCallback = false;
    }
}

WWMouseClass::WWMouseClass(GraphicViewPortClass *scr, int width, int height) :
    m_mouseCursor(new uint8_t[width * height]),
    m_mouseHotX(0),
    m_mouseHotY(0),
    m_cursorWidth(0),
    m_cursorHeight(0),
    m_mouseBuffer(new uint8_t[width * height]),
    m_buffX(-1),
    m_buffY(-1),
    m_maxWidth(width),
    m_maxHeight(height),
    m_mouseCXLeft(0),
    m_mouseCYUpper(0),
    m_mouseCXRight(0),
    m_mouseCYLower(0),
    m_cFlags(0),
    m_cCount(0),
    m_screen(scr),
    m_prevCursor(nullptr),
    m_mouseUpdate(0),
    m_state(1),
    m_eraseBuffer(new uint8_t[width * height]),
    m_eraseBuffX(-1),
    m_eraseBuffY(-1),
    m_eraseBuffHotX(-1),
    m_eraseBuffHotY(-1),
    m_eraseFlags(0)
{
#ifdef PLATFORM_WINDOWS
    InitializeCriticalSection(&m_mouseCritSec);
    timeBeginPeriod(MOUSE_UPDATE_FREQ);
    m_timerHandle = timeSetEvent(MOUSE_UPDATE_FREQ, 1, ::Process_Mouse, 0, TIME_PERIODIC);
#endif
    Set_Cursor_Clip();
}

WWMouseClass::~WWMouseClass()
{
    ++m_mouseUpdate;
    delete[] m_mouseCursor;
    delete[] m_mouseBuffer;
    delete[] m_eraseBuffer;

#ifdef PLATFORM_WINDOWS
    if (m_timerHandle != 0) {
        timeKillEvent(m_timerHandle);
    }

    timeEndPeriod(MOUSE_UPDATE_FREQ);
#endif

    Clear_Cursor_Clip();
}

int WWMouseClass::Get_Mouse_X()
{
#if !defined RAPP_STANDALONE
    // This convolution is needed as it seems this function is detoured in ddraw.dll replacements
    // to allow windowed mode, so we can't use the one rapp.dll links to.
    typedef  BOOL(__stdcall *HookedGetCursorPos_t)(LPPOINT lpPoint);
    HookedGetCursorPos_t HookedGetCursorPos = *reinterpret_cast<HookedGetCursorPos_t*>(0x005E6848);
    POINT pos;
    HookedGetCursorPos(&pos);
    return pos.x;
#elif defined PLATFORM_WINDOWS
    POINT pos;
    GetCursorPos(&pos);
    return pos.x;
#endif
}

int WWMouseClass::Get_Mouse_Y()
{
#if !defined RAPP_STANDALONE
    // This convolution is needed as it seems this function is detoured in ddraw.dll replacements
    // to allow windowed mode, so we can't use the one rapp.dll links to.
    typedef  BOOL(__stdcall *HookedGetCursorPos_t)(LPPOINT lpPoint);
    HookedGetCursorPos_t HookedGetCursorPos = *reinterpret_cast<HookedGetCursorPos_t*>(0x005E6848);
    POINT pos;
    HookedGetCursorPos(&pos);
    return pos.y;
#elif defined PLATFORM_WINDOWS
    POINT pos;
    GetCursorPos(&pos);
    return pos.y;
#endif
}

void WWMouseClass::Get_Mouse_XY(int &x_pos, int &y_pos)
{
#if !defined RAPP_STANDALONE
    // This convolution is needed as it seems this function is detoured in ddraw.dll replacements
    // to allow windowed mode, so we can't use the one rapp.dll links to.
    typedef  BOOL(__stdcall *HookedGetCursorPos_t)(LPPOINT lpPoint);
    HookedGetCursorPos_t HookedGetCursorPos = *reinterpret_cast<HookedGetCursorPos_t*>(0x005E6848);
    POINT pos;
    HookedGetCursorPos(&pos);
    x_pos = pos.x;
    y_pos = pos.y;
#elif defined PLATFORM_WINDOWS
    POINT pos;
    GetCursorPos(&pos);
    x_pos = pos.x;
    y_pos = pos.y;
#endif
}

void WWMouseClass::Set_Mouse_X(int x_pos)
{
#ifdef PLATFORM_WINDOWS
    SetCursorPos(x_pos, Get_Mouse_Y());
#endif
}

void WWMouseClass::Set_Mouse_Y(int y_pos)
{
#ifdef PLATFORM_WINDOWS
    SetCursorPos(Get_Mouse_X(), y_pos);
#endif
}

BOOL WWMouseClass::Set_Mouse_XY(int x_pos, int y_pos)
{
#ifdef PLATFORM_WINDOWS
    return SetCursorPos(x_pos, y_pos);
#endif
}

void WWMouseClass::Block_Mouse(GraphicBufferClass *gbuffer)
{
    if (m_screen->Get_Graphic_Buffer() == gbuffer) {
#ifdef PLATFORM_WINDOWS
        EnterCriticalSection(&m_mouseCritSec);
#endif
    }
}

void WWMouseClass::Unblock_Mouse(GraphicBufferClass *gbuffer)
{
    if (m_screen->Get_Graphic_Buffer() == gbuffer) {
#ifdef PLATFORM_WINDOWS
        LeaveCriticalSection(&m_mouseCritSec);
#endif
    }
}

void WWMouseClass::Set_Cursor_Clip()
{
#ifdef PLATFORM_WINDOWS
    RECT cliprect;

    if (m_screen) {
        cliprect.left = 0;
        cliprect.top = 0;
        cliprect.right = m_screen->Get_Width();
        cliprect.bottom = m_screen->Get_Height();

        ClipCursor(&cliprect);
    }
#endif
}

void WWMouseClass::Clear_Cursor_Clip()
{
#ifdef PLATFORM_WINDOWS
    ClipCursor(nullptr);
#endif
}

void WWMouseClass::Process_Mouse()
{
    if (m_screen != nullptr && m_state <= 0 && !m_mouseUpdate && !m_eraseFlags && g_gameInFocus && m_screen->Get_LockCount() == 0) {
        int cur_x;
        int cur_y;

        Get_Mouse_XY(cur_x, cur_y);
       
        if (cur_x != m_buffX || cur_y != m_buffY) {
            if (m_screen->Lock()) {
                Low_Hide_Mouse();

                if ((m_cFlags & 1) && cur_x >= m_mouseCXLeft && cur_x <= m_mouseCXRight && cur_y >= m_mouseCYUpper
                    && cur_y <= m_mouseCYLower) {
                    m_cFlags |= 2;
                }

                if (!(m_cFlags & 2)) {
                    Low_Show_Mouse(cur_x, cur_y);
                }

                m_screen->Unlock();
            }
        }
    }
}

void *WWMouseClass::Set_Cursor(int hotspot_x, int hotspot_y, void *cursor)
{
    if (cursor && cursor != m_prevCursor) {
        ++m_mouseUpdate;
        Hide_Mouse();
        void *retval = Mouse_Set_Cursor(*this, hotspot_x, hotspot_y, cursor);
        Show_Mouse();
        --m_mouseUpdate;

        return retval;
    }

    return nullptr;
}

void WWMouseClass::Low_Hide_Mouse()
{
    if (m_state == 0) {
        if (m_buffX != -1 || m_buffY != -1) {
            if (m_screen->Lock()) {
                // DEBUG_LOG("Erasing mouse at X %d, Y %d, HotX %d, HotY %d\n", m_buffX, m_buffY, m_mouseHotX,
                // m_mouseHotY);
                Mouse_Shadow_Buffer(*this, *m_screen, m_mouseBuffer, m_buffX, m_buffY, m_mouseHotX, m_mouseHotY, false);
                m_screen->Unlock();
            }
        }

        m_buffX = -1;
        m_buffY = -1;
    }

    ++m_state;
}

void WWMouseClass::Hide_Mouse()
{
    // DEBUG_LOG("WWMouseClass::Hide_Mouse()\n");
    ++m_mouseUpdate;
    Low_Hide_Mouse();
    --m_mouseUpdate;
}

void WWMouseClass::Low_Show_Mouse(int x_pos, int y_pos)
{
    if (m_state != 0 && (--m_state) == 0 && m_screen->Lock()) {
        Mouse_Shadow_Buffer(*this, *m_screen, m_mouseBuffer, x_pos, y_pos, m_mouseHotX, m_mouseHotY, true);
        Mouse_Draw(*this, *m_screen, x_pos, y_pos);
        m_buffX = x_pos;
        m_buffY = y_pos;
        m_screen->Unlock();
    }
}

void WWMouseClass::Show_Mouse()
{
    // DEBUG_LOG("WWMouseClass::Show_Mouse()\n");
    int x_pos;
    int y_pos;

    Get_Mouse_XY(x_pos, y_pos);

    --m_mouseUpdate;
    Low_Show_Mouse(x_pos, y_pos);
    ++m_mouseUpdate;
}

void WWMouseClass::Conditional_Hide_Mouse(int x_pos, int y_pos, int width, int height)
{
    ++m_mouseUpdate;

    // this looks like the hotspot calculation
    x_pos = Max(0, x_pos - m_cursorWidth - m_mouseHotX);
    y_pos = Max(0, y_pos - m_cursorHeight - m_mouseHotY);
    width = Min(width + m_mouseHotX, m_screen->Get_Width());
    height = Min(height + m_mouseHotY, m_screen->Get_Height());

    if (m_cCount) {
        m_mouseCXLeft = Min(x_pos, m_mouseCXLeft);
        m_mouseCYUpper = Min(y_pos, m_mouseCYUpper);
        m_mouseCXRight = Max(width, m_mouseCXRight);
        m_mouseCYLower = Max(height, m_mouseCYLower);
    } else {
        m_mouseCXLeft = x_pos;
        m_mouseCYUpper = y_pos;
        m_mouseCXRight = width;
        m_mouseCYLower = height;
    }

    if (!(m_cFlags & 2)) {
        if (m_buffX >= m_mouseCXLeft && m_buffX <= m_mouseCXRight && m_buffY >= m_mouseCYUpper
            && m_buffY <= m_mouseCYLower) {
            Low_Hide_Mouse();
            m_cFlags |= 2;
        }
    }

    m_cFlags |= 1;
    ++m_cCount;
    --m_mouseUpdate;
}

void WWMouseClass::Conditional_Show_Mouse()
{
    // open dune has a idle sleep here waiting.
    // https://github.com/OpenDUNE/OpenDUNE/blob/master/src/gui/gui.c#L3849
    ++m_mouseUpdate;

    if (m_cCount) {
        --m_cCount;

        if (!m_cCount) {
            if (m_cFlags & 2) {
                Show_Mouse();
            }

            m_cFlags = 0;
        }
    }

    --m_mouseUpdate;
}

void WWMouseClass::Draw_Mouse(GraphicViewPortClass &viewport)
{
    if (!m_state) {
        ++m_mouseUpdate;

        int cur_x;
        int cur_y;

        Get_Mouse_XY(cur_x, cur_y);

        if (m_cFlags & 1 && cur_x >= m_mouseCXLeft && cur_x <= m_mouseCXRight && cur_y >= m_mouseCYUpper && cur_y <= m_mouseCYLower) {
            Hide_Mouse();
            m_cFlags |= 2;
        } else {
            m_eraseFlags = 1;

            if (viewport.Lock()) {
                Mouse_Shadow_Buffer(*this, viewport, m_eraseBuffer, cur_x, cur_y, m_mouseHotX, m_mouseHotY, true);
                memcpy(m_mouseBuffer, m_eraseBuffer, m_maxHeight * m_maxWidth);
                Mouse_Draw(*this, viewport, cur_x, cur_y);

                m_eraseBuffX = cur_x;
                m_buffX = cur_x;
                m_eraseBuffY = cur_y;
                m_buffY = cur_y;
                m_eraseBuffHotX = m_mouseHotX;
                m_eraseBuffHotY = m_mouseHotY;

                viewport.Unlock();
            }
        }

        --m_mouseUpdate;
    }
}

void WWMouseClass::Erase_Mouse(GraphicViewPortClass &viewport, bool erase)
{
    if (!erase || m_eraseBuffX != -1 || m_eraseBuffY != -1) {
        ++m_mouseUpdate;

        if (erase && (m_eraseBuffX != -1 || m_eraseBuffY != -1)) {
            if (viewport.Lock()) {
                Mouse_Shadow_Buffer(
                    *this, viewport, m_eraseBuffer, m_eraseBuffX, m_eraseBuffY, m_eraseBuffHotX, m_eraseBuffHotY, false);
                viewport.Unlock();
            }

            m_eraseBuffX = -1;
            m_eraseBuffY = -1;
        }

        --m_mouseUpdate;
        m_eraseFlags = 0;
    }
}
