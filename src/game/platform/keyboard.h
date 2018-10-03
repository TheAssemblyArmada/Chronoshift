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
#pragma once

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "always.h"

#ifdef PLATFORM_WINDOWS
#include <winuser.h>
#endif

#define KBD_BUFFER_SIZE 256

// Keycode info
// http://codebreak.com/git/azuki/Odilon/commit/d2119a90d807d09f90b3364199e672dc678a4e80/raw/?path=Odilon%2FGKeyFook.cs
// https://opensource.apple.com/source/WebCore/WebCore-1298/platform/WindowsKeyboardCodes.h
// http://www.opensource.apple.com/source/WebCore/WebCore-4A93/platform/gdk/KeyboardCodes.h
// https://code.google.com/p/chromium/codesearch#chromium/src/ui/events/keycodes/keyboard_codes_posix.h

#define SHIFT_KEY_PRESSED 0x100
#define CTRL_KEY_PRESSED 0x200
#define ALT_KEY_PRESSED 0x400
#define WIN_KEY_PRESSED 0x800 // can't be 0x800, that is used to flag a key released
#define NUMLOCK_KEY_PRESSED 0x200 // needs checking!
#define SCROLL_KEY_PRESSED 0x200 // needs checking!

typedef uint16_t KeyType;

enum WWKeyType
{
    WWKEY_SHIFT_BIT = 0x1000,
    WWKEY_CTRL_BIT = 0x1000,
    WWKEY_ALT_BIT = 0x1000,
    WWKEY_RLS_BIT = 0x1000,
    WWKEY_VK_BIT = 0x1000,
    WWKEY_DBL_BIT = 0x1000,
    WWKEY_BTN_BIT = 0x00401000
};

enum KeyMWheelType
{
    WHEEL_UP = 0x0,
    WHEEL_DOWN = 0x1
};

enum KeyModifierType
{
    KEY_SHIFT = 0x1,
    KEY_CTRL = 0x2,
    KEY_ALT = 0x4,
    KEY_WINKEY = 0x8
};

// ftp://ftp.westwood.com/pub/redalert/info/remap.txt
// this is the VK codes?
// looking at ra3.00 dissembly, the code at 0051D4A2 shows KeyNumType checking for 0x1B (ESC) and then for 0x8064, and aslo
// looking at the  To_ASCII code, what makes me think that this is like a strage VK num and bitfield mix? could it be
// including some modifer?
#if 0 //DOS Version
enum KeyNumType
{
    KN_NONE = 0x0,
    KN_GRAVE = 0x01,
    KN_1 = 0x02,
    KN_2 = 0x03,
    KN_3 = 0x04,
    KN_4 = 0x05,
    KN_5 = 0x06,
    KN_6 = 0x07,
    KN_7 = 0x08,
    KN_8 = 0x09,
    KN_9 = 0x0A,
    KN_0 = 0x0B,
    KN_MINUS = 0x0C,
    KN_EQUAL = 0x0D,
    KN_RESERVED1 = 0x0E,
    KN_BACKSPACE = 0x0F,
    KN_TAB = 0x10,
    KN_Q = 0x11,
    KN_W = 0x12,
    KN_E = 0x13,
    KN_R = 0x14,
    KN_T = 0x15,
    KN_Y = 0x16,
    KN_U = 0x17,
    KN_I = 0x18,
    KN_O = 0x19,
    KN_P = 0x1A,
    KN_LBRACKET = 0x1B,
    KN_RBRACKET = 0x1C,
    KN_BACKSLASH = 0x1D,
    KN_CAPSLOCK = 0x1E,
    KN_A = 0x1F,
    KN_S = 0x20,
    KN_D = 0x21,
    KN_F = 0x22,
    KN_G = 0x23,
    KN_H = 0x24,
    KN_J = 0x25,
    KN_K = 0x26,
    KN_L = 0x27,
    KN_SEMICOLON = 0x28,
    KN_SQUOTE = 0x29,
    KN_BACKSLASH2 = 0x2A,
    KN_RETURN = 0x2B,
    KN_LSHIFT = 0x2C,
    KN_MOUSE_MOVE = 0x2D,
    KN_Z = 0x2E,
    KN_X = 0x2F,
    KN_C = 0x30,
    KN_V = 0x31,
    KN_B = 0x32,
    KN_N = 0x33,
    KN_M = 0x34,
    KN_COMMA = 0x35,
    KN_PERIOD = 0x36,
    KN_SLASH = 0x37,
    KN_RESERVED3 = 0x38,
    KN_RSHIFT = 0x39,
    KN_LCTRL = 0x3A,
    KN_LCOMM = 0x3B, // todo KN_WINKEY
    KN_LALT = 0x3C,
    KN_SPACE = 0x3D,
    KN_RALT = 0x3E,
    KN_RCOMM = 0x3F, // todo KN_WINKEY
    KN_RCTRL = 0x40,
    KN_LMOUSE = 0x41,
    KN_RMOUSE = 0x42,
    KN_JBUTTON1 = 0x43,
    KN_JBUTTON2 = 0x44,
    KN_J_UP = 0x45,
    KN_J_RIGHT = 0x46,
    KN_J_DOWN = 0x47,
    KN_J_LEFT = 0x48,
    KN_SPECIAL9 = 0x49,
    KN_SPECIAL10 = 0x4A,
    KN_E_INSERT = 0x4B,
    KN_E_DELETE = 0x4C,
    KN_RESERVED4 = 0x4D,
    KN_RESERVED5 = 0x4E,
    KN_E_LEFT = 0x4F,
    KN_E_HOME = 0x50,
    KN_E_END = 0x51,
    KN_RESERVED6 = 0x52,
    KN_E_UP = 0x53,
    KN_E_DOWN = 0x54,
    KN_E_PGUP = 0x55,
    KN_E_PGDN = 0x56,
    KN_K_LPAREN = 0x57,
    KN_K_RPAREN = 0x58,
    KN_E_RIGHT = 0x59,
    KN_NUMLOCK = 0x5A,
    KN_HOME = 0x5B,
    KN_UPLEFT = 0x5B,
    KN_LEFT = 0x5C,
    KN_END = 0x5D,
    KN_DOWNLEFT = 0x5D,
    KN_RESERVED7 = 0x5E,
    KN_KEYPAD_SLASH = 0x5F,
    KN_UP = 0x60,
    KN_CENTER = 0x61,
    KN_DOWN = 0x62,
    KN_INSERT = 0x63,
    KN_KEYPAD_ASTERISK = 0x64,
    KN_PGUP = 0x65,
    KN_UPRIGHT = 0x65,
    KN_RIGHT = 0x66,
    KN_PGDN = 0x67,
    KN_DOWNRIGHT = 0x67,
    KN_DELETE = 0x68,
    KN_KEYPAD_MINUS = 0x69,
    KN_KEYPAD_PLUS = 0x6A,
    KN_RESERVED8 = 0x6B,
    KN_KEYPAD_RETURN = 0x6C,
    KN_RESERVED9 = 0x6D,
    KN_ESC = 0x6E,
    KN_HELP = 0x6F,
    KN_F1 = 0x70,
    KN_F2 = 0x71,
    KN_F3 = 0x72,
    KN_F4 = 0x73,
    KN_F5 = 0x74,
    KN_F6 = 0x75,
    KN_F7 = 0x76,
    KN_F8 = 0x77,
    KN_F9 = 0x78,
    KN_F10 = 0x79,
    KN_F11 = 0x7A,
    KN_F12 = 0x7B,
    KN_PRNTSCRN = 0x7C,
    KN_SCROLLLOCK = 0x7D,
    KN_PAUSE = 0x7E,
    KN_SHIFT_BIT = 0x1000,
    KN_CTRL_BIT = 0x1000,
    KN_ALT_BIT = 0x1000,
    KN_RLSE_BIT = 0x1000,
    KN_LCOMM_BIT = 0x1000,
    KN_RCOMM_BIT = 0x1000,
    KN_BUTTON = 0x00401000
};
#endif
// Windows verson (virtual keys)
enum KeyNumType
{
    KN_NONE = 0x0,
    KN_0 = 0x30,
    KN_1 = 0x31,
    KN_2 = 0x32,
    KN_3 = 0x33,
    KN_4 = 0x34,
    KN_5 = 0x35,
    KN_6 = 0x36,
    KN_7 = 0x37,
    KN_8 = 0x38,
    KN_9 = 0x39,
    KN_A = 0x41,
    KN_B = 0x42,
    KN_BACKSLASH = 0xDC,
    KN_BACKSPACE = 0x8,
    KN_C = 0x43,
    KN_CAPSLOCK = 0x14,
    KN_CENTER = 0xC,
    KN_COMMA = 0xBC,
    KN_D = 0x44,
    KN_DELETE = 0x2E,
    KN_DOWN = 0x28,
    KN_DOWNLEFT = 0x23,
    KN_DOWNRIGHT = 0x22,
    KN_E = 0x45,
    KN_END = 0x23,
    KN_EQUAL = 0xBB,
    KN_ESC = 0x1B,
    KN_E_DELETE = 0x2E,
    KN_E_DOWN = 0x62,
    KN_E_END = 0x61,
    KN_E_HOME = 0x67,
    KN_E_INSERT = 0x2D,
    KN_E_LEFT = 0x64,
    KN_E_PGDN = 0x63,
    KN_E_PGUP = 0x69,
    KN_E_RIGHT = 0x66,
    KN_E_UP = 0x68,
    KN_F = 0x46,
    KN_F1 = 0x70,
    KN_F10 = 0x79,
    KN_F11 = 0x7A,
    KN_F12 = 0x7B,
    KN_F2 = 0x71,
    KN_F3 = 0x72,
    KN_F4 = 0x73,
    KN_F5 = 0x74,
    KN_F6 = 0x75,
    KN_F7 = 0x76,
    KN_F8 = 0x77,
    KN_F9 = 0x78,
    KN_G = 0x47,
    KN_GRAVE = 0xC0,
    KN_H = 0x48,
    KN_HOME = 0x24,
    KN_I = 0x49,
    KN_INSERT = 0x2D,
    KN_J = 0x4A,
    KN_K = 0x4B,
    KN_KEYPAD_ASTERISK = 0x6A,
    KN_KEYPAD_MINUS = 0x6D,
    KN_KEYPAD_PLUS = 0x6B,
    KN_KEYPAD_RETURN = 0xD,
    KN_KEYPAD_SLASH = 0x6F,
    KN_L = 0x4C,
    KN_LALT = 0x12,
    KN_LBRACKET = 0xDB,
    KN_LCTRL = 0x11,
    KN_LEFT = 0x25,
    KN_LMOUSE = 0x1,
    KN_LSHIFT = 0x10,
    KN_M = 0x4D,
    KN_MINUS = 0xBD,
    KN_N = 0x4E,
    KN_NUMLOCK = 0x90,
    KN_O = 0x4F,
    KN_P = 0x50,
    KN_PAUSE = 0x13,
    KN_PERIOD = 0xBE,
    KN_PGDN = 0x22,
    KN_PGUP = 0x21,
    KN_PRNTSCRN = 0x2A,
    KN_Q = 0x51,
    KN_R = 0x52,
    KN_RALT = 0x12,
    KN_RBRACKET = 0xDD,
    KN_RCTRL = 0x11,
    KN_RETURN = 0xD,
    KN_RIGHT = 0x27,
    KN_RMOUSE = 0x2,
    KN_RSHIFT = 0x10,
    KN_S = 0x53,
    KN_SCROLLLOCK = 0x91,
    KN_SEMICOLON = 0xBA,
    KN_SLASH = 0xBF,
    KN_SPACE = 0x20,
    KN_SQUOTE = 0xDE,
    KN_T = 0x54,
    KN_TAB = 0x9,
    KN_U = 0x55,
    KN_UP = 0x26,
    KN_UPLEFT = 0x24,
    KN_UPRIGHT = 0x21,
    KN_V = 0x56,
    KN_W = 0x57,
    KN_X = 0x58,
    KN_Y = 0x59,
    KN_Z = 0x5A,
    KN_SHIFT_BIT = 0x100,
    KN_CTRL_BIT = 0x200,
    KN_ALT_BIT = 0x400,
    KN_RLSE_BIT = 0x800,
    KN_BUTTON = 0x8000,
};

enum KeyASCIIType
{
    KA_NULL = 0,
    KA_BACKSPACE = 8,
    KA_RETURN = 13,
    KA_ESC = 27,
    KA_SPACE = 32,
    KEY_MAX = 0x7FFF,
};

class KeyboardClass
{
public:
    KeyboardClass();
    ~KeyboardClass() {}

    int Get_MouseQX() { return m_mouseQX; }
    int Get_MouseQY() { return m_mouseQY; }
    uint16_t Check();
    uint16_t Get();
    char To_ASCII(KeyType keycode);
    BOOL Down(KeyType keycode);
    BOOL Up(KeyType keycode);
    void Clear();
    void Set_Initialised(BOOL initialised) { m_initialised = initialised; }
#if defined PLATFORM_WINDOWS
    BOOL Message_Handler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif

#ifndef CHRONOSHIFT_STANDALONE
    static void Hook_Me();
    static BOOL Hook_Handler(KeyboardClass *ptr, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static uint16_t Hook_Check(KeyboardClass *ptr);
    static uint16_t Hook_Get(KeyboardClass *ptr);
    static void Hook_Clear(KeyboardClass *ptr);
    static void Hook_Fill(KeyboardClass *ptr);
#endif

private:
    BOOL Is_Mouse_Key(KeyType keycode);
    BOOL Mouse_Buttons_Swapped();
    BOOL Put(KeyType keycode);
    BOOL Put_Key_Message(KeyType keycode, BOOL release);
    BOOL Put_Mouse_Message(KeyType keycode, int x, int y, BOOL release);
    uint16_t Fetch_Element();
    uint16_t Peek_Element() const;
    uint16_t Peek_Element_Ahead(int steps);
    BOOL Put_Element(KeyType keycode);
    BOOL Is_Buffer_Full() const;
    BOOL Is_Buffer_Empty() const;
    void Fill_Buffer_From_System();
    int Available_Buffer_Room() const;
    uint16_t Buff_Get();

private:
    int m_mouseQX;
    int m_mouseQY;
    uint8_t m_keyboardState[KBD_BUFFER_SIZE];
    KeyType m_elements[KBD_BUFFER_SIZE]; // Buffer area for circular buffer.
    int m_elementGetPos; // Position of the next retrievable entry.
    int m_elementPutPos; // Position of the next free space in the buffer.
    BOOL m_initialised;
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"

inline void KeyboardClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x005B82FC, Hook_Handler);
    //Hook_Function(0x005B7408, Hook_Fill);
    Hook_Function(0x005B7F30, Hook_Check);
    Hook_Function(0x005B7F5C, Hook_Get);
    Hook_Function(0x005B82CC, Hook_Clear);
#endif
}

extern KeyboardClass *&g_keyboard;
#else
extern KeyboardClass *g_keyboard;
#endif

#endif // KEYBOARD_H
