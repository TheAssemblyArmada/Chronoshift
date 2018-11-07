/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief An abstraction to buffer keyboard and mouse events.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "keyboard.h"
#include "gamedebug.h"
#include <cstring>

using std::memset;

#ifndef CHRONOSHIFT_STANDALONE
KeyboardClass *&g_keyboard = Make_Global<KeyboardClass *>(0x00666904);
#else
KeyboardClass *g_keyboard = nullptr;
#endif

KeyboardClass::KeyboardClass() : 
    m_MouseQX(0),
    m_MouseQY(0),
    m_ElementGetPos(0),
    m_ElementPutPos(0),
    m_Initialised(false)
{
    memset(m_KeyboardState, 0, sizeof(m_KeyboardState));
    memset(m_Elements, 0, sizeof(m_Elements));
}

BOOL KeyboardClass::Is_Mouse_Key(uint16_t keycode)
{
    // Strip off any flags.
    keycode = keycode & 0xFF;

#ifdef PLATFORM_WINDOWS
    return keycode == VK_LBUTTON || keycode == VK_MBUTTON || keycode == VK_RBUTTON;
#else
    return false;
#endif
}

BOOL KeyboardClass::Mouse_Buttons_Swapped()
{
#ifdef PLATFORM_WINDOWS
    return GetSystemMetrics(SM_SWAPBUTTON) != 0;
#else
    return false;
#endif
}

uint16_t KeyboardClass::Check()
{
    Fill_Buffer_From_System();

    if (!Is_Buffer_Empty()) {
        return Peek_Element();
    }

    return 0;
}

uint16_t KeyboardClass::Get()
{
    return Buff_Get();
}

BOOL KeyboardClass::Put(uint16_t keycode)
{
    if (Is_Buffer_Full()) {
        //DEBUG_LOG("KeyboardClass::Put() buffer is full, unable to put key (%d) and returning %s.\n", keycode, "false");
        return false;
    }

    //DEBUG_LOG("KeyboardClass::Put() setting key (%d) and returning %s.\n", keycode, "true");
    Put_Element(keycode);

    return true;
}

BOOL KeyboardClass::Put_Key_Message(uint16_t keycode, BOOL release)
{
    // As we are putting a keyboard message, check that the input keycode is not a mouse key.
    if (!Is_Mouse_Key(keycode)) {
#ifdef PLATFORM_WINDOWS
        // Is the shift key pressed?
        if ((GetKeyState(VK_SHIFT) >> 8) & 0x80 || GetKeyState(VK_CAPITAL) & 8 || GetKeyState(VK_NUMLOCK) & 8) {
            keycode |= KN_SHIFT_BIT;
            //DEBUG_LOG("KeyboardClass::Put_Key_Message() SHIFT is held down\n");
        }

        // Is the ctrl key pressed?
        if ((GetKeyState(VK_CONTROL) >> 8) & 0x80) {
            keycode |= KN_CTRL_BIT;
            //DEBUG_LOG("KeyboardClass::Put_Key_Message() CONTROL is held down\n");
        }

        // Is the alt key pressed?
        if ((GetKeyState(VK_MENU) >> 8) & 0x80) {
            keycode |= KN_ALT_BIT;
            //DEBUG_LOG("KeyboardClass::Put_Key_Message() MENU is held down\n");
        }
#endif
    }

    // Mark if this is a keyup event rather than a key down event.
    if (release) {
        keycode |= KN_RLSE_BIT;
    }

    return Put(keycode);
}

BOOL KeyboardClass::Put_Mouse_Message(uint16_t keycode, int mouse_x, int mouse_y, BOOL release)
{
    if (Available_Buffer_Room() >= 3 && Is_Mouse_Key(keycode)) {
        /*DEBUG_LOG("KeyboardClass::Put_Mouse_Message() - key = %d, mouse_x = %d, mouse_y = %d, release = %d.\n",
            keycode,
            mouse_x,
            mouse_y,
            release);*/

        Put_Key_Message(keycode, release);

        Put(mouse_x);
        Put(mouse_y);

        return true;
    }

    return false;
}

KeyASCIIType KeyboardClass::To_ASCII(uint16_t keycode)
{
#if defined PLATFORM_WINDOWS
    KeyASCIIType character[2];

    if (keycode & KN_RLSE_BIT) {
        return KA_NONE;
    }

    if (keycode & KN_SHIFT_BIT) {
        m_KeyboardState[VK_SHIFT] = 0x80;
    }

    if (keycode & KN_CTRL_BIT) {
        m_KeyboardState[VK_CONTROL] = 0x80;
    }

    if (keycode & KN_ALT_BIT) {
        m_KeyboardState[VK_MENU] = 0x80;
    }

    int ascii_out = ToAscii(keycode, MapVirtualKeyA(keycode, MAPVK_VK_TO_VSC), m_KeyboardState, LPWORD(character), 0);

    if (keycode & KN_SHIFT_BIT) {
        m_KeyboardState[VK_SHIFT] = 0;
    }

    if (keycode & KN_CTRL_BIT) {
        m_KeyboardState[VK_CONTROL] = 0;
    }

    if (keycode & KN_ALT_BIT) {
        m_KeyboardState[VK_MENU] = 0;
    }

    if (ascii_out != 1) {
        return KA_NONE;
    }

    return character[0];
#endif
}

BOOL KeyboardClass::Down(uint16_t keycode)
{
#if defined PLATFORM_WINDOWS
    if (keycode == VK_LBUTTON || keycode == VK_RBUTTON) {
        // Determine whether the buttons are swapped.
        if (Mouse_Buttons_Swapped()) {
            keycode = VK_RBUTTON - (keycode != VK_LBUTTON);
        }
    }

    return GetAsyncKeyState(keycode) != 0;
#endif
}

BOOL KeyboardClass::Up(uint16_t keycode)
{
    return !Down(keycode);
}

uint16_t KeyboardClass::Fetch_Element()
{
    if (m_ElementGetPos != m_ElementPutPos) {
        uint16_t val = m_Elements[m_ElementGetPos];
        m_ElementGetPos = (m_ElementGetPos + 1) % ARRAY_SIZE(m_Elements);

        return val;
    }

    return 0;
}

uint16_t KeyboardClass::Peek_Element() const
{
    if (!Is_Buffer_Empty()) {
        return m_Elements[m_ElementGetPos];
    }

    return 0;
}

uint16_t KeyboardClass::Peek_Element_Ahead(int steps)
{
    if (steps < 0 || steps >= ARRAY_SIZE(m_Elements) || Is_Buffer_Empty()) {
        return steps;
    }

    return m_Elements[steps + m_ElementGetPos];
}

BOOL KeyboardClass::Put_Element(uint16_t key)
{
    if (!Is_Buffer_Full()) {
        m_Elements[m_ElementPutPos] = key;
        m_ElementPutPos = (m_ElementPutPos + 1) % ARRAY_SIZE(m_Elements);

        return true;
    }

    return false;
}

#if defined PLATFORM_WINDOWS
BOOL KeyboardClass::Message_Handler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Has the message been handled?
    BOOL handled = false;

    // Get two shorts from lParam.
    // This gets mouse pos (for mouse events) relative to window.
    POINT point;
    point.x = lParam & 0xFFFF;
    point.y = (lParam >> 16) & 0xFFFF;

    // This converts relative window pos to absolute screen pos.
    // ClientToScreen(hWnd, &point);

    switch (message) {

        case WM_LBUTTONDOWN:
            // DEBUG_LOG("KeyboardClass::Message_Handler(WM_LBUTTONDOWN), %d, %d.\n", point.x, point.y);
            Put_Mouse_Message(VK_LBUTTON, point.x, point.y, false);
            handled = true;
            break;

        case WM_LBUTTONUP:
            // DEBUG_LOG("KeyboardClass::Message_Handler(WM_LBUTTONUP).\n");
            Put_Mouse_Message(VK_LBUTTON, point.x, point.y, true);
            handled = true;
            break;

        case WM_LBUTTONDBLCLK:
            // DEBUG_LOG("KeyboardClass::Message_Handler(WM_LBUTTONDBLCLK).\n");

            // register double click?
            Put_Mouse_Message(VK_LBUTTON, point.x, point.y, false);
            Put_Mouse_Message(VK_LBUTTON, point.x, point.y, true);
            Put_Mouse_Message(VK_LBUTTON, point.x, point.y, false);
            Put_Mouse_Message(VK_LBUTTON, point.x, point.y, true);
            handled = true;
            break;

        case WM_MBUTTONDOWN:
            // DEBUG_LOG("KeyboardClass::Message_Handler(WM_MBUTTONDOWN).\n");
            Put_Mouse_Message(VK_MBUTTON, point.x, point.y, false);
            handled = true;
            break;

        case WM_MBUTTONUP:
            // DEBUG_LOG("KeyboardClass::Message_Handler(WM_MBUTTONUP).\n");
            Put_Mouse_Message(VK_MBUTTON, point.x, point.y, true);
            handled = true;
            break;

        case WM_MBUTTONDBLCLK:
            // DEBUG_LOG("KeyboardClass::Message_Handler(WM_MBUTTONDBLCLK).\n");
            Put_Mouse_Message(VK_MBUTTON, point.x, point.y, false);
            Put_Mouse_Message(VK_MBUTTON, point.x, point.y, true);
            Put_Mouse_Message(VK_MBUTTON, point.x, point.y, false);
            Put_Mouse_Message(VK_MBUTTON, point.x, point.y, true);
            handled = true;
            break;

        case WM_RBUTTONDOWN:
            // DEBUG_LOG("KeyboardClass::Message_Handler(WM_RBUTTONDOWN).\n");
            Put_Mouse_Message(VK_RBUTTON, point.x, point.y, false);
            handled = true;
            break;

        case WM_RBUTTONUP:
            // DEBUG_LOG("KeyboardClass::Message_Handler(WM_RBUTTONUP).\n");
            Put_Mouse_Message(VK_RBUTTON, point.x, point.y, true);
            handled = true;
            break;

        case WM_RBUTTONDBLCLK:
            // DEBUG_LOG("KeyboardClass::Message_Handler(WM_RBUTTONDBLCLK).\n");
            Put_Mouse_Message(VK_RBUTTON, point.x, point.y, false);
            Put_Mouse_Message(VK_RBUTTON, point.x, point.y, true);
            Put_Mouse_Message(VK_RBUTTON, point.x, point.y, false);
            Put_Mouse_Message(VK_RBUTTON, point.x, point.y, true);
            handled = true;
            break;

        case WM_SYSKEYUP:
            // DEBUG_LOG("KeyboardClass::Message_Handler(WM_SYSKEYUP).\n");
            Put_Key_Message(wParam, true);
            handled = true;
            break;

        case WM_KEYUP:
            // DEBUG_LOG("KeyboardClass::Message_Handler(WM_KEYUP).\n");
            if (wParam != VK_SCROLL) {
                Put_Key_Message(wParam, true);
            }

            handled = true;
            break;

        case WM_SYSKEYDOWN:
            // DEBUG_LOG("KeyboardClass::Message_Handler(WM_SYSKEYDOWN).\n");
            Put_Key_Message(wParam, false);
            handled = true;
            break;

        // WM_KEYDOWN and WM_KEYFIRST have the same value so only KEYDOWN
        case WM_KEYDOWN:
            // DEBUG_LOG("KeyboardClass::Message_Handler(WM_KEYDOWN).\n");
            // Scroll lock ran Stop_Execution which appears empty, prob for debugging?
            if (wParam != VK_SCROLL) {
                Put_Key_Message(wParam, false);
            }

            handled = true;
            break;

        default:
            handled = false;
            break;
    };

    if (!handled) {
        return false;
    }

    // If handled, pass to the default handler for final processing
    // our WindowsProcedure will return when we return true
    DefWindowProcA(hWnd, message, wParam, lParam);

    return true;
}
#endif //_WIN32

uint16_t KeyboardClass::Buff_Get()
{
    // Wait until there is a valid key pressed.
    while (Check() == KN_NONE) {
        // empty
    }

    uint16_t key_val = Fetch_Element();

    if (Is_Mouse_Key(key_val)) {
        m_MouseQX = Fetch_Element();
        m_MouseQY = Fetch_Element();

        // DEBUG_LOG("Getting Mouse button event at %d, %d.\n", m_MouseQX, m_MouseQY);
    }

    return key_val;
}

BOOL KeyboardClass::Is_Buffer_Full() const
{
    return (m_ElementPutPos + 1) % KEYBOARD_BUFFER_SIZE == m_ElementGetPos;
}

BOOL KeyboardClass::Is_Buffer_Empty() const
{
    return m_ElementGetPos == m_ElementPutPos;
}

void KeyboardClass::Clear()
{
    Fill_Buffer_From_System();
    m_ElementGetPos = m_ElementPutPos;

    Fill_Buffer_From_System();
    m_ElementGetPos = m_ElementPutPos;
}

void KeyboardClass::Fill_Buffer_From_System()
{
#if defined PLATFORM_WINDOWS
    MSG msg;

    if (!Is_Buffer_Full()) {
        // Check to see if any messages are waiting in the queue
        while (PeekMessageA(&msg, nullptr, 0, 0, PM_NOREMOVE)) {
            int result = GetMessageA(&msg, 0, 0, 0);

            if (result == 0 || result == -1) {
                break;
            }

            // Translate the message and dispatch it to WindowProc()
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
    }
#endif
}

int KeyboardClass::Available_Buffer_Room() const
{
    if (m_ElementGetPos == m_ElementPutPos) {
        return KEYBOARD_BUFFER_SIZE;
    }

    if (m_ElementGetPos < m_ElementPutPos) {
        return m_ElementPutPos - m_ElementGetPos;
    }

    if (m_ElementGetPos > m_ElementPutPos) {
        return KEYBOARD_BUFFER_SIZE - (m_ElementPutPos - m_ElementGetPos);
    }

    return 0;
}

#ifndef CHRONOSHIFT_STANDALONE
BOOL KeyboardClass::Hook_Handler(KeyboardClass *ptr, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return ptr->Message_Handler(hWnd, message, wParam, lParam);
}

uint16_t KeyboardClass::Hook_Check(KeyboardClass *ptr)
{
    return ptr->Check();
}

uint16_t KeyboardClass::Hook_Get(KeyboardClass *ptr)
{
    return ptr->Get();
}

void KeyboardClass::Hook_Clear(KeyboardClass *ptr)
{
    ptr->Clear();
}
void KeyboardClass::Hook_Fill(KeyboardClass *ptr)
{
    ptr->Fill_Buffer_From_System();
}
#endif
