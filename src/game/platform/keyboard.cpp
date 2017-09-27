/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief An abstraction to buffer keyboard and mouse events.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "keyboard.h"
#include "gamedebug.h"

#ifndef RAPP_STANDALONE
KeyboardClass *&g_keyboard = Make_Global<KeyboardClass *>(0x00666904);
#else
KeyboardClass *g_keyboard = nullptr;
#endif

KeyboardClass::KeyboardClass()
{
    m_mouseQX = 0;
    m_mouseQY = 0;
    m_elementGetPos = 0;
    m_elementPutPos = 0;
    memset(m_keyboardState, 0, sizeof(m_keyboardState));
}

BOOL KeyboardClass::Is_Mouse_Key(KeyType keycode)
{
    return keycode == VK_LBUTTON || keycode == VK_MBUTTON || keycode == VK_RBUTTON;
}

BOOL KeyboardClass::Mouse_Buttons_Swapped()
{
    return GetSystemMetrics(SM_SWAPBUTTON) != 0;
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
    while (Check() == 0) {
        //empty
    }

    return Buff_Get();
}

BOOL KeyboardClass::Put(KeyType keycode)
{
    if (Is_Buffer_Full()) {
        DEBUG_LOG("KeyboardClass::Put() buffer is full, unable to put key (%d) and returning %s.\n", keycode, "false");

        return false;
    } else {
        DEBUG_LOG("KeyboardClass::Put() setting key (%d) and returning %s.\n", keycode, "true");
        return Put_Element(keycode);

        return true;
    }
}

BOOL KeyboardClass::Put_Key_Message(KeyType keycode, BOOL release)
{
    // as we are putting a keyboard message, check that the input keycode is not a mouse key
    if (!Is_Mouse_Key(keycode)) {
        // Is the shift key pressed?
        if ((GetKeyState(VK_SHIFT) >> 8) & 0x80 || GetKeyState(VK_CAPITAL) & 8 || GetKeyState(VK_NUMLOCK) & 8) {
            keycode |= SHIFT_KEY_PRESSED;
            DEBUG_LOG("KeyboardClass::Put_Key_Message() VK_SHIFT is held down\n");
        }

        // Is the ctrl key pressed?
        if ((GetKeyState(VK_CONTROL) >> 8) & 0x80) {
            keycode |= CTRL_KEY_PRESSED;
            DEBUG_LOG("KeyboardClass::Put_Key_Message() VK_CONTROL is held down\n");
        }

        // Is the alt key pressed?
        if ((GetKeyState(VK_MENU) >> 8) & 0x80) {
            keycode |= ALT_KEY_PRESSED;
            DEBUG_LOG("KeyboardClass::Put_Key_Message() VK_MENU is held down\n");
        }
    }

    // Mark if this is a keyup event rather than a key down event.
    if (release) {
        keycode |= 0x800;
    }

    return Put(keycode);
}

BOOL KeyboardClass::Put_Mouse_Message(KeyType keycode, int x, int y, BOOL release)
{
    if (Available_Buffer_Room() >= 3 && Is_Mouse_Key(keycode)) {
        DEBUG_LOG("KeyboardClass::Put_Mouse_Message() - key = %d, mouse_x = %d, mouse_y = %d, release = %d.\n",
            keycode,
            x,
            y,
            release);
        Put_Key_Message(keycode, release);
        Put(x);
        Put(y);

        return true;
    }

    return false;
}

char KeyboardClass::To_ASCII(KeyType keycode)
{
#if defined PLATFORM_WINDOWS
    int ascii_out;
    uint16_t charbuff[2];

    if (keycode & 0x800) {
        return 0;
    } else {
        if (keycode & 0x100) {
            m_keyboardState[16] = 0x80u;
        }

        if (keycode & 0x200) {
            m_keyboardState[17] = 0x80u;
        }

        if (keycode & 0x400) {
            m_keyboardState[18] = 0x80u;
        }

        ascii_out = ToAscii(keycode, MapVirtualKeyA(keycode, MAPVK_VK_TO_VSC), m_keyboardState, charbuff, 0);

        if (keycode & 0x100) {
            m_keyboardState[16] = 0;
        }

        if (keycode & 0x200) {
            m_keyboardState[17] = 0;
        }

        if (keycode & 0x400) {
            m_keyboardState[18] = 0;
        }

        if (ascii_out != 1) {
            return 0;
        }
    }

    return *charbuff;
#endif
}

BOOL KeyboardClass::Down(KeyType keycode)
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

BOOL KeyboardClass::Up(KeyType keycode)
{
    return !Down(keycode);
}

uint16_t KeyboardClass::Fetch_Element()
{
    if (m_elementGetPos != m_elementPutPos) {
        uint16_t val = m_elements[m_elementGetPos];
        m_elementGetPos = (m_elementGetPos + 1) % ARRAY_SIZE(m_elements);

        return val;
    }

    return 0;
}

uint16_t KeyboardClass::Peek_Element() const
{
    if (!Is_Buffer_Empty()) {
        return m_elements[m_elementGetPos];
    }

    return 0;
}

uint16_t KeyboardClass::Peek_Element_Ahead(int steps)
{
    if (steps < 0 || steps >= ARRAY_SIZE(m_elements) || Is_Buffer_Empty()) {
        return steps;
    }

    return m_elements[steps + m_elementGetPos];
}

BOOL KeyboardClass::Put_Element(KeyType key)
{
    if (!Is_Buffer_Full()) {
        m_elements[m_elementPutPos] = key;
        m_elementPutPos = (m_elementPutPos + 1) % ARRAY_SIZE(m_elements);

        return true;
    }

    return false;
}

#if defined PLATFORM_WINDOWS
BOOL KeyboardClass::Message_Handler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    BOOL handled = false;

    POINT point;

    // get two shorts from lParam.
    // This gets mouse pos (for mouse events) relative to window
    point.x = lParam & 0xFFFF;
    point.y = (lParam >> 16) & 0xFFFF;

    // this converts relative window pos to absolute screen pos
    // ClientToScreen(hWnd, &point);

    switch (message) {
        case WM_LBUTTONDOWN:
            DEBUG_LOG("KeyboardClass::Message_Handler(WM_LBUTTONDOWN), %d, %d.\n", point.x, point.y);
            Put_Mouse_Message(VK_LBUTTON, point.x, point.y, false);
            handled = true;
            break;

        case WM_LBUTTONUP:
            DEBUG_LOG("KeyboardClass::Message_Handler(WM_LBUTTONUP).\n");
            Put_Mouse_Message(VK_LBUTTON, point.x, point.y, true);
            handled = true;
            break;

        case WM_LBUTTONDBLCLK:
            DEBUG_LOG("KeyboardClass::Message_Handler(WM_LBUTTONDBLCLK).\n");

            // register double click?
            Put_Mouse_Message(VK_LBUTTON, point.x, point.y, false);
            Put_Mouse_Message(VK_LBUTTON, point.x, point.y, true);
            Put_Mouse_Message(VK_LBUTTON, point.x, point.y, false);
            Put_Mouse_Message(VK_LBUTTON, point.x, point.y, true);
            handled = true;
            break;

        case WM_MBUTTONDOWN:
            DEBUG_LOG("KeyboardClass::Message_Handler(WM_MBUTTONDOWN).\n");
            Put_Mouse_Message(VK_MBUTTON, point.x, point.y, false);
            handled = true;
            break;

        case WM_MBUTTONUP:
            DEBUG_LOG("KeyboardClass::Message_Handler(WM_MBUTTONUP).\n");
            Put_Mouse_Message(VK_MBUTTON, point.x, point.y, true);
            handled = true;
            break;

        case WM_MBUTTONDBLCLK:
            DEBUG_LOG("KeyboardClass::Message_Handler(WM_MBUTTONDBLCLK).\n");
            Put_Mouse_Message(VK_MBUTTON, point.x, point.y, false);
            Put_Mouse_Message(VK_MBUTTON, point.x, point.y, true);
            Put_Mouse_Message(VK_MBUTTON, point.x, point.y, false);
            Put_Mouse_Message(VK_MBUTTON, point.x, point.y, true);
            handled = true;
            break;

        case WM_RBUTTONDOWN:
            DEBUG_LOG("KeyboardClass::Message_Handler(WM_RBUTTONDOWN).\n");
            Put_Mouse_Message(VK_RBUTTON, point.x, point.y, false);
            handled = true;
            break;

        case WM_RBUTTONUP:
            DEBUG_LOG("KeyboardClass::Message_Handler(WM_RBUTTONUP).\n");
            Put_Mouse_Message(VK_RBUTTON, point.x, point.y, true);
            handled = true;
            break;

        case WM_RBUTTONDBLCLK:
            DEBUG_LOG("KeyboardClass::Message_Handler(WM_RBUTTONDBLCLK).\n");
            Put_Mouse_Message(VK_RBUTTON, point.x, point.y, false);
            Put_Mouse_Message(VK_RBUTTON, point.x, point.y, true);
            Put_Mouse_Message(VK_RBUTTON, point.x, point.y, false);
            Put_Mouse_Message(VK_RBUTTON, point.x, point.y, true);
            handled = true;
            break;

        case WM_SYSKEYUP:
            DEBUG_LOG("KeyboardClass::Message_Handler(WM_SYSKEYUP).\n");
            Put_Key_Message(wParam, true);
            handled = true;
            break;

        case WM_KEYUP:
            DEBUG_LOG("KeyboardClass::Message_Handler(WM_KEYUP).\n");
            if (wParam != VK_SCROLL) {
                Put_Key_Message(wParam, true);
            }

            handled = true;
            break;

        case WM_SYSKEYDOWN:
            DEBUG_LOG("KeyboardClass::Message_Handler(WM_SYSKEYDOWN).\n");
            Put_Key_Message(wParam, false);
            handled = true;
            break;

        // WM_KEYDOWN and WM_KEYFIRST have the same value so only KEYDOWN
        case WM_KEYDOWN:
            DEBUG_LOG("KeyboardClass::Message_Handler(WM_KEYDOWN).\n");
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
    // Busy wait for input?
    while (Check() == 0) {
        // empty
    }

    uint16_t key_val = Fetch_Element();

    if (Is_Mouse_Key(key_val)) {
        m_mouseQX = Fetch_Element();
        m_mouseQY = Fetch_Element();
    }

    return key_val;
}

BOOL KeyboardClass::Is_Buffer_Full() const
{
    return (m_elementPutPos + 1) % KBD_BUFFER_SIZE == m_elementGetPos;
}

BOOL KeyboardClass::Is_Buffer_Empty() const
{
    return m_elementGetPos == m_elementPutPos;
}

void KeyboardClass::Clear()
{
    Fill_Buffer_From_System();
    m_elementGetPos = m_elementPutPos;

    Fill_Buffer_From_System();
    m_elementGetPos = m_elementPutPos;
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
    if (m_elementGetPos == m_elementPutPos) {
        return KBD_BUFFER_SIZE; // ARRAY_SIZE(m_elements)
    }

    if (m_elementGetPos < m_elementPutPos) {
        return m_elementPutPos - m_elementGetPos;
    }

    if (m_elementGetPos > m_elementPutPos) {
        return KBD_BUFFER_SIZE - (m_elementPutPos - m_elementGetPos); // ARRAY_SIZE(m_elements)
    }

    return 0;
}

#ifndef RAPP_STANDALONE
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
