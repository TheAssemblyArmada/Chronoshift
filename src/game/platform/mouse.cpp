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
#include "mouse.h"
#include "gamedebug.h"
#include "gbuffer.h"
#include "globals.h"
#include "mouseshape.h"
#include <cstring>
#include <algorithm>

using std::memcpy;

#ifdef PLATFORM_WINDOWS
// extern HWND MainWindow;
#endif

#ifndef GAME_DLL
MouseClass *g_mouse = nullptr;
MouseClass *g_wwmouse = nullptr;
#endif
//
// Frequency to update the mouse in ms
//
static int const MOUSE_UPDATE_FREQ = 16;

// This callback is called by a timer thread periodically and asyncronously,
// it causes the mouse instance Process_Mouse() function to be called provided
// it isn't already in the middle of it.
#ifdef PLATFORM_WINDOWS
void CALLBACK Process_Mouse(UINT uDelay, UINT uResolution, DWORD_PTR fptc, DWORD_PTR dwUser, DWORD_PTR fuEvent)
{
    static bool InMouseCallback;

    if (g_mouse != nullptr && !InMouseCallback) {
        InMouseCallback = true;
        g_mouse->Process_Mouse();
        InMouseCallback = false;
    }
}
#endif

MouseClass::MouseClass(GraphicViewPortClass *scr, int width, int height) :
    m_MouseCursor(new uint8_t[width * height]),
    m_MouseHotX(0),
    m_MouseHotY(0),
    m_CursorWidth(0),
    m_CursorHeight(0),
    m_MouseBuffer(new uint8_t[width * height]),
    m_BuffX(-1),
    m_BuffY(-1),
    m_MaxWidth(width),
    m_MaxHeight(height),
    m_MouseCXLeft(0),
    m_MouseCYUpper(0),
    m_MouseCXRight(0),
    m_MouseCYLower(0),
    m_CFlags(0),
    m_CCount(0),
    m_Screen(scr),
    m_PrevCursor(nullptr),
    m_MouseUpdate(0),
    m_State(1),
    m_EraseBuffer(new uint8_t[width * height]),
    m_EraseBuffX(-1),
    m_EraseBuffY(-1),
    m_EraseBuffHotX(-1),
    m_EraseBuffHotY(-1),
    m_EraseFlags(0)
{
#ifdef PLATFORM_WINDOWS
    InitializeCriticalSection(&m_MouseCritSec);
    timeBeginPeriod(MOUSE_UPDATE_FREQ);
    m_TimerHandle = timeSetEvent(MOUSE_UPDATE_FREQ, 1, ::Process_Mouse, 0, TIME_PERIODIC);
#endif
    Set_Cursor_Clip();
}

MouseClass::~MouseClass()
{
    ++m_MouseUpdate;
    delete[] m_MouseCursor;
    delete[] m_MouseBuffer;
    delete[] m_EraseBuffer;

#ifdef PLATFORM_WINDOWS
    if (m_TimerHandle != 0) {
        timeKillEvent(m_TimerHandle);
    }

    timeEndPeriod(MOUSE_UPDATE_FREQ);
#endif

    Clear_Cursor_Clip();
}

int MouseClass::Get_Mouse_X()
{
#if defined GAME_DLL
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
#else
    return 0;
#endif
}

int MouseClass::Get_Mouse_Y()
{
#if defined GAME_DLL
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
#else
    return 0;
#endif
}

void MouseClass::Get_Mouse_XY(int &x_pos, int &y_pos)
{
#if defined GAME_DLL
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

void MouseClass::Set_Mouse_X(int x_pos)
{
#ifdef PLATFORM_WINDOWS
    SetCursorPos(x_pos, Get_Mouse_Y());
#endif
}

void MouseClass::Set_Mouse_Y(int y_pos)
{
#ifdef PLATFORM_WINDOWS
    SetCursorPos(Get_Mouse_X(), y_pos);
#endif
}

BOOL MouseClass::Set_Mouse_XY(int x_pos, int y_pos)
{
#ifdef PLATFORM_WINDOWS
    return SetCursorPos(x_pos, y_pos);
#else
    return true;
#endif
}

void MouseClass::Block_Mouse(GraphicBufferClass *gbuffer)
{
    if (m_Screen->Get_Graphic_Buffer() == gbuffer) {
#ifdef PLATFORM_WINDOWS
        EnterCriticalSection(&m_MouseCritSec);
#endif
    }
}

void MouseClass::Unblock_Mouse(GraphicBufferClass *gbuffer)
{
    if (m_Screen->Get_Graphic_Buffer() == gbuffer) {
#ifdef PLATFORM_WINDOWS
        LeaveCriticalSection(&m_MouseCritSec);
#endif
    }
}

void MouseClass::Set_Cursor_Clip()
{
#ifdef PLATFORM_WINDOWS
    RECT cliprect;

    if (m_Screen) {
        cliprect.left = 0;
        cliprect.top = 0;
        cliprect.right = m_Screen->Get_Width();
        cliprect.bottom = m_Screen->Get_Height();

        ClipCursor(&cliprect);
    }
#endif
}

void MouseClass::Clear_Cursor_Clip()
{
#ifdef PLATFORM_WINDOWS
    ClipCursor(nullptr);
#endif
}

void MouseClass::Process_Mouse()
{
    if (m_Screen != nullptr && m_State <= 0 && !m_MouseUpdate && !m_EraseFlags && g_gameInFocus && m_Screen->Get_LockCount() == 0) {
        int cur_x;
        int cur_y;

        Get_Mouse_XY(cur_x, cur_y);
       
        if (cur_x != m_BuffX || cur_y != m_BuffY) {
            if (m_Screen->Lock()) {
                Low_Hide_Mouse();

                if ((m_CFlags & 1) && cur_x >= m_MouseCXLeft && cur_x <= m_MouseCXRight && cur_y >= m_MouseCYUpper
                    && cur_y <= m_MouseCYLower) {
                    m_CFlags |= 2;
                }

                if (!(m_CFlags & 2)) {
                    Low_Show_Mouse(cur_x, cur_y);
                }

                m_Screen->Unlock();
            }
        }
    }
}

void *MouseClass::Set_Cursor(int hotspot_x, int hotspot_y, void *cursor)
{
    if (cursor && cursor != m_PrevCursor) {
        ++m_MouseUpdate;
        Hide_Mouse();
        void *retval = Mouse_Set_Cursor(*this, hotspot_x, hotspot_y, cursor);
        Show_Mouse();
        --m_MouseUpdate;

        return retval;
    }

    return nullptr;
}

void MouseClass::Low_Hide_Mouse()
{
    if (m_State == 0) {
        if (m_BuffX != -1 || m_BuffY != -1) {
            if (m_Screen->Lock()) {
                // DEBUG_LOG("Erasing mouse at X %d, Y %d, HotX %d, HotY %d\n", m_buffX, m_buffY, m_mouseHotX,
                // m_mouseHotY);
                Mouse_Shadow_Buffer(*this, *m_Screen, m_MouseBuffer, m_BuffX, m_BuffY, m_MouseHotX, m_MouseHotY, false);
                m_Screen->Unlock();
            }
        }

        m_BuffX = -1;
        m_BuffY = -1;
    }

    ++m_State;
}

void MouseClass::Hide_Mouse()
{
    // DEBUG_LOG("MouseClass::Hide_Mouse()\n");
    ++m_MouseUpdate;
    Low_Hide_Mouse();
    --m_MouseUpdate;
}

void MouseClass::Low_Show_Mouse(int x_pos, int y_pos)
{
    if (m_State != 0 && (--m_State) == 0 && m_Screen->Lock()) {
        Mouse_Shadow_Buffer(*this, *m_Screen, m_MouseBuffer, x_pos, y_pos, m_MouseHotX, m_MouseHotY, true);
        Mouse_Draw(*this, *m_Screen, x_pos, y_pos);
        m_BuffX = x_pos;
        m_BuffY = y_pos;
        m_Screen->Unlock();
    }
}

void MouseClass::Show_Mouse()
{
    // DEBUG_LOG("MouseClass::Show_Mouse()\n");
    int x_pos;
    int y_pos;

    Get_Mouse_XY(x_pos, y_pos);

    --m_MouseUpdate;
    Low_Show_Mouse(x_pos, y_pos);
    ++m_MouseUpdate;
}

void MouseClass::Conditional_Hide_Mouse(int x_pos, int y_pos, int width, int height)
{
    ++m_MouseUpdate;

    // this looks like the hotspot calculation
    x_pos = std::max(0, x_pos - m_CursorWidth - m_MouseHotX);
    y_pos = std::max(0, y_pos - m_CursorHeight - m_MouseHotY);
    width = std::min(width + m_MouseHotX, m_Screen->Get_Width());
    height = std::min(height + m_MouseHotY, m_Screen->Get_Height());

    if (m_CCount) {
        m_MouseCXLeft = std::min(x_pos, m_MouseCXLeft);
        m_MouseCYUpper = std::min(y_pos, m_MouseCYUpper);
        m_MouseCXRight = std::max(width, m_MouseCXRight);
        m_MouseCYLower = std::max(height, m_MouseCYLower);
    } else {
        m_MouseCXLeft = x_pos;
        m_MouseCYUpper = y_pos;
        m_MouseCXRight = width;
        m_MouseCYLower = height;
    }

    if (!(m_CFlags & 2)) {
        if (m_BuffX >= m_MouseCXLeft && m_BuffX <= m_MouseCXRight && m_BuffY >= m_MouseCYUpper
            && m_BuffY <= m_MouseCYLower) {
            Low_Hide_Mouse();
            m_CFlags |= 2;
        }
    }

    m_CFlags |= 1;
    ++m_CCount;
    --m_MouseUpdate;
}

void MouseClass::Conditional_Show_Mouse()
{
    // open dune has a idle sleep here waiting.
    // https://github.com/OpenDUNE/OpenDUNE/blob/master/src/gui/gui.c#L3849
    ++m_MouseUpdate;

    if (m_CCount) {
        --m_CCount;

        if (!m_CCount) {
            if (m_CFlags & 2) {
                Show_Mouse();
            }

            m_CFlags = 0;
        }
    }

    --m_MouseUpdate;
}

void MouseClass::Draw_Mouse(GraphicViewPortClass &viewport)
{
    if (!m_State) {
        ++m_MouseUpdate;

        int cur_x;
        int cur_y;

        Get_Mouse_XY(cur_x, cur_y);

        if (m_CFlags & 1 && cur_x >= m_MouseCXLeft && cur_x <= m_MouseCXRight && cur_y >= m_MouseCYUpper && cur_y <= m_MouseCYLower) {
            Hide_Mouse();
            m_CFlags |= 2;
        } else {
            m_EraseFlags = 1;

            if (viewport.Lock()) {
                Mouse_Shadow_Buffer(*this, viewport, m_EraseBuffer, cur_x, cur_y, m_MouseHotX, m_MouseHotY, true);
                memcpy(m_MouseBuffer, m_EraseBuffer, m_MaxHeight * m_MaxWidth);
                Mouse_Draw(*this, viewport, cur_x, cur_y);

                m_EraseBuffX = cur_x;
                m_BuffX = cur_x;
                m_EraseBuffY = cur_y;
                m_BuffY = cur_y;
                m_EraseBuffHotX = m_MouseHotX;
                m_EraseBuffHotY = m_MouseHotY;

                viewport.Unlock();
            }
        }

        --m_MouseUpdate;
    }
}

void MouseClass::Erase_Mouse(GraphicViewPortClass &viewport, bool erase)
{
    if (!erase || m_EraseBuffX != -1 || m_EraseBuffY != -1) {
        ++m_MouseUpdate;

        if (erase && (m_EraseBuffX != -1 || m_EraseBuffY != -1)) {
            if (viewport.Lock()) {
                Mouse_Shadow_Buffer(
                    *this, viewport, m_EraseBuffer, m_EraseBuffX, m_EraseBuffY, m_EraseBuffHotX, m_EraseBuffHotY, false);
                viewport.Unlock();
            }

            m_EraseBuffX = -1;
            m_EraseBuffY = -1;
        }

        --m_MouseUpdate;
        m_EraseFlags = 0;
    }
}
