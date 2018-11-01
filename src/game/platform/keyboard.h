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

// Keycode info
// http://codebreak.com/git/azuki/Odilon/commit/d2119a90d807d09f90b3364199e672dc678a4e80/raw/?path=Odilon%2FGKeyFook.cs
// https://opensource.apple.com/source/WebCore/WebCore-1298/platform/WindowsKeyboardCodes.h
// http://www.opensource.apple.com/source/WebCore/WebCore-4A93/platform/gdk/KeyboardCodes.h
// https://code.google.com/p/chromium/codesearch#chromium/src/ui/events/keycodes/keyboard_codes_posix.h
// http://thecodeforyou.blogspot.com/2013/01/vb-keyascii-values.html

enum KeyType
{
    KEY_SHIFT_BIT = 0x100,
    KEY_CTRL_BIT = 0x200,
    KEY_ALT_BIT = 0x400,
    KEY_RLS_BIT = 0x800,
    KEY_VK_BIT = 0x1000,
    KEY_DBL_BIT = 0x2000,
    KEY_BTN_BIT = 0x8000
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
    KA_NONE = 0x0,

    KA_MORE = 0x1,
    KA_SETBKGDCOL = 0x2,
    KA_SETFORECOL = 0x6,
    KA_FORMFEED = 0xC,
    KA_SPCTAB = 0x14,
    KA_SETX = 0x19,
    KA_SETY = 0x1A,
    KA_SPACE = 0x20,
    KA_EXCLAMATION = 0x21,
    KA_DQUOTE = 0x22,
    KA_POUND = 0x23,
    KA_DOLLAR = 0x24,
    KA_PERCENT = 0x25,
    KA_AMPER = 0x26,
    KA_SQUOTE = 0x27,
    KA_LPAREN = 0x28,
    KA_RPAREN = 0x29,
    KA_ASTERISK = 0x2A,
    KA_PLUS = 0x2B,
    KA_COMMA = 0x2C,
    KA_MINUS = 0x2D,
    KA_PERIOD = 0x2E,
    KA_SLASH = 0x2F,
    KA_0 = 0x30,
    KA_1 = 0x31,
    KA_2 = 0x32,
    KA_3 = 0x33,
    KA_4 = 0x34,
    KA_5 = 0x35,
    KA_6 = 0x36,
    KA_7 = 0x37,
    KA_8 = 0x38,
    KA_9 = 0x39,
    KA_COLON = 0x3A,
    KA_SEMICOLON = 0x3B,
    KA_LESS_THAN = 0x3C,
    KA_EQUAL = 0x3D,
    KA_GREATER_THAN = 0x3E,
    KA_QUESTION = 0x3F,
    KA_AT = 0x40,
    KA_A = 0x41,
    KA_B = 0x42,
    KA_C = 0x43,
    KA_D = 0x44,
    KA_E = 0x45,
    KA_F = 0x46,
    KA_G = 0x47,
    KA_H = 0x48,
    KA_I = 0x49,
    KA_J = 0x4A,
    KA_K = 0x4B,
    KA_L = 0x4C,
    KA_M = 0x4D,
    KA_N = 0x4E,
    KA_O = 0x4F,
    KA_P = 0x50,
    KA_Q = 0x51,
    KA_R = 0x52,
    KA_S = 0x53,
    KA_T = 0x54,
    KA_U = 0x55,
    KA_V = 0x56,
    KA_W = 0x57,
    KA_X = 0x58,
    KA_Y = 0x59,
    KA_Z = 0x5A,
    KA_LBRACKET = 0x5B,
    KA_BACKSLASH = 0x5C,
    KA_RBRACKET = 0x5D,
    KA_CARROT = 0x5E,
    KA_UNDERLINE = 0x5F,
    KA_GRAVE = 0x60,
    KA_a = 0x61,
    KA_b = 0x62,
    KA_c = 0x63,
    KA_d = 0x64,
    KA_e = 0x65,
    KA_f = 0x66,
    KA_g = 0x67,
    KA_h = 0x68,
    KA_i = 0x69,
    KA_j = 0x6A,
    KA_k = 0x6B,
    KA_l = 0x6C,
    KA_m = 0x6D,
    KA_n = 0x6E,
    KA_o = 0x6F,
    KA_p = 0x70,
    KA_q = 0x71,
    KA_r = 0x72,
    KA_s = 0x73,
    KA_t = 0x74,
    KA_u = 0x75,
    KA_v = 0x76,
    KA_w = 0x77,
    KA_x = 0x78,
    KA_y = 0x79,
    KA_z = 0x7A,
    KA_LBRACE = 0x7B,
    KA_BAR = 0x7C,
    KA_RBRACE = 0x7D,
    KA_TILDA = 0x7E,
    KA_ESC = 0x1B,
    KA_EXTEND = 0x1B,
    KA_RETURN = 0xD,
    KA_BACKSPACE = 0x8,
    KA_TAB = 0x9,
    KA_DELETE = 0x2E,
    KA_INSERT = 0x2D,
    KA_PGDN = 0x22,
    KA_DOWNRIGHT = 0x22,
    KA_DOWN = 0x28,
    KA_END = 0x23,
    KA_DOWNLEFT = 0x23,
    KA_RIGHT = 0x27,
    KA_KEYPAD5 = 0x29,
    KA_LEFT = 0x25,
    KA_PGUP = 0x21,
    KA_UPRIGHT = 0x21,
    KA_UP = 0x26,
    KA_HOME = 0x24,
    KA_UPLEFT = 0x24,
    KA_F12 = 0x7B,
    KA_F11 = 0x7A,
    KA_F10 = 0x79,
    KA_F9 = 0x78,
    KA_F8 = 0x77,
    KA_F7 = 0x76,
    KA_F6 = 0x75,
    KA_F5 = 0x74,
    KA_F4 = 0x73,
    KA_F3 = 0x72,
    KA_F2 = 0x71,
    KA_F1 = 0x70,
    KA_LMOUSE = 0x1,
    KA_RMOUSE = 0x2,
    KA_SHIFT_BIT = 0x100,
    KA_CTRL_BIT = 0x200,
    KA_ALT_BIT = 0x400,
    KA_RLSE_BIT = 0x800,

    KEY_MAX = 0x7FFF, // Force enum size.
};

class KeyboardClass
{
public:
    KeyboardClass();
    ~KeyboardClass() {}
    uint16_t Check();
    uint16_t Get();
    KeyASCIIType To_ASCII(uint16_t keycode);
    BOOL Down(uint16_t keycode);
    BOOL Up(uint16_t keycode);
    void Clear();
    void Set_Initialised(BOOL initialised) { m_Initialised = initialised; }
    int Get_MouseQX() { return m_MouseQX; }
    int Get_MouseQY() { return m_MouseQY; }

#if defined PLATFORM_WINDOWS
    BOOL Message_Handler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif

private:
    BOOL Is_Mouse_Key(uint16_t keycode);
    BOOL Mouse_Buttons_Swapped();
    BOOL Put(uint16_t keycode);
    BOOL Put_Key_Message(uint16_t keycode, BOOL release = false);
    BOOL Put_Mouse_Message(uint16_t keycode, int mouse_x, int mouse_y, BOOL release = false);
    uint16_t Fetch_Element();
    uint16_t Peek_Element() const;
    uint16_t Peek_Element_Ahead(int steps);
    BOOL Put_Element(uint16_t keycode);
    BOOL Is_Buffer_Full() const;
    BOOL Is_Buffer_Empty() const;
    void Fill_Buffer_From_System();
    int Available_Buffer_Room() const;
    uint16_t Buff_Get();

private:
    enum {
        KEYBOARD_BUFFER_SIZE = 256
    };

    int m_MouseQX;
    int m_MouseQY;

    // 256-byte array that has the status data for each key using the same
    // format as the windows virtual key state array.
    // Values 0 to 31 are non - printable and values 128 - 255 are
    // considered "Extended". These "extended" codes may vary in appearance
    // from computer to computer(and from font to font), so use with caution.
    uint8_t m_KeyboardState[256];

    uint16_t m_Elements[KEYBOARD_BUFFER_SIZE]; // Buffer area for circular buffer.
    int m_ElementGetPos; // Position of the next retrievable entry.
    int m_ElementPutPos; // Position of the next free space in the buffer.
    BOOL m_Initialised;

#ifndef CHRONOSHIFT_STANDALONE
public:
    static void Hook_Me();

private:
    static BOOL Hook_Handler(KeyboardClass *ptr, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static uint16_t Hook_Check(KeyboardClass *ptr);
    static uint16_t Hook_Get(KeyboardClass *ptr);
    static void Hook_Clear(KeyboardClass *ptr);
    static void Hook_Fill(KeyboardClass *ptr);
#endif
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
