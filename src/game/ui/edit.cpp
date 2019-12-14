/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class for representing editable UI gadgets.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "edit.h"
#include "gbuffer.h"
#include "mouse.h"
#include "textprint.h"
#include <cctype>
#include <cstring>

using std::isalpha;
using std::isalnum;
using std::isprint;
using std::toupper;
using std::strlen;

EditClass::EditClass(int id, char *textbuffer, int bufferlength, TextPrintType text_style, int x, int y, int w, int h,
    EditStyleType edit_style) :
    ControlClass(id, x, y, w, h, MOUSE_LEFT_PRESS, false),
    m_TextStyle(text_style & (~TPF_CENTER)),
    m_EditStyle(edit_style),
    m_Text(textbuffer),
    m_MaxTextLength(bufferlength - 1),
    m_TextLength(strlen(textbuffer)),
    m_ColorScheme(GadgetClass::s_ColorScheme)
{
    Flag_To_Redraw();
    Set_Text(textbuffer, bufferlength);

    if (w == -1 || h == -1) {
        Fancy_Text_Print(nullptr, 0, 0, nullptr, COLOR_TBLACK, m_TextStyle);
        if (h == -1) {
            m_Height = g_FontHeight + 1;
        }

        if (w == -1) {
            if (strlen(m_Text) == 0) {
                m_Width = (m_MaxTextLength + 1) * (g_FontXSpacing + Char_Pixel_Width('X')) + 2;
            } else {
                m_Width = String_Pixel_Width(m_Text) + 6;
            }
        }
    }

    m_IgnoreInput = 0;
}

EditClass::~EditClass()
{
    if (GadgetClass::Has_Focus()) {
        GadgetClass::Clear_Focus();
    }
}

EditClass::EditClass(EditClass &that) :
    ControlClass(that),
    m_TextStyle(that.m_TextStyle),
    m_EditStyle(that.m_EditStyle),
    m_Text(that.m_Text),
    m_MaxTextLength(that.m_MaxTextLength),
    m_TextLength(that.m_TextLength),
    m_ColorScheme(that.m_ColorScheme),
    m_IgnoreInput(that.m_IgnoreInput)
{
}

void EditClass::Set_Text(char *text, int max_len)
{
    m_MaxTextLength = max_len - 1;
    m_Text = text;
    m_TextLength = strlen(text);
    Flag_To_Redraw();
}

BOOL EditClass::Draw_Me(BOOL redraw)
{
    if (ControlClass::Draw_Me(redraw)) {
        if (g_LogicPage == &g_SeenBuff) {
            g_Mouse->Conditional_Hide_Mouse(m_XPos, m_YPos, m_Width + m_XPos, m_Height + m_YPos);
        }

        Draw_Background();
        Draw_Text(m_Text);

        if (g_LogicPage == &g_SeenBuff) {
            g_Mouse->Conditional_Show_Mouse();
        }

        return true;
    }

    return false;
}

BOOL EditClass::Action(unsigned flags, KeyNumType &key)
{
    KeyASCIIType character;

    if (m_IgnoreInput) {
        return false;
    }

    if (flags & MOUSE_LEFT_PRESS) {
        flags &= ~MOUSE_LEFT_PRESS;
        Set_Focus();
        Flag_To_Redraw();
    }

    if (flags & KEYBOARD_INPUT && Has_Focus()) {
        if (key == KN_ESC) {
            Clear_Focus();
            flags = INPUT_NONE;
        } else {
            character = g_Keyboard->To_ASCII(key);
            if (key & KEY_VK_BIT && character >= KA_0 && character <= KA_9) {
                key = (KeyNumType)(key & (~KEY_VK_BIT));

                if (!(flags & MOUSE_LEFT_RLSE) && !(flags & MOUSE_RIGHT_RLSE) && Handle_Key(character)) {
                    key = KN_NONE;
                    flags &= ~KEYBOARD_INPUT;

                    return ControlClass::Action(flags, key);
                }
            } else if ((key & KEY_VK_BIT || character < KA_SPACE || character > '\xFF') && key != KN_RETURN && key != KN_BACKSPACE) {
                key = KN_NONE;
                flags &= ~KEYBOARD_INPUT;
            } else if (!(flags & MOUSE_LEFT_RLSE) && !(flags & MOUSE_RIGHT_RLSE)) {
                if (Handle_Key(g_Keyboard->To_ASCII(key))) {
                    key = KN_NONE;
                    flags &= ~KEYBOARD_INPUT;

                    return ControlClass::Action(flags, key);
                }
            }
        }
    }

    return ControlClass::Action(flags, key);
}

void EditClass::Draw_Background()
{
    Draw_Box(m_XPos, m_YPos, m_Width, m_Height, BOX_STYLE_4, true);
}

void EditClass::Draw_Text(const char *text)
{
    TextPrintType style = Has_Focus() ? TPF_USE_BRIGHT : TPF_NONE;

    // Draw the current text
    Conquer_Clip_Text_Print(text, m_XPos + 1, m_YPos + 1, m_ColorScheme, 0, m_TextStyle | style, m_Width - 2, nullptr);

    // If the control has focus and it will fit in the space, draw an underscore
    // to represent the text cursor
    if (Has_Focus() && (int)strlen(text) < m_MaxTextLength) {
        int text_width = String_Pixel_Width(text);
        int cursor_width = String_Pixel_Width("_");
        if (cursor_width + text_width < m_Width - 2) {
            Conquer_Clip_Text_Print(
                "_", m_XPos + 1 + text_width, m_YPos + 1, m_ColorScheme, 0, m_TextStyle | style, m_Width - 2, nullptr);
        }
    }
}

int EditClass::Handle_Key(KeyASCIIType character)
{
    if (character < KA_BACKSPACE) {
        if (character == KA_NONE) {
            return true;
        }
    } else {
        if (character == KA_BACKSPACE) {
            if (m_TextLength) {
                --m_TextLength;
                m_Text[m_TextLength] = '\0';
                Flag_To_Redraw();
            }

            return true;
        }

        if (character == KA_RETURN) {
            Clear_Focus();

            return false;
        }
    }

    if (String_Pixel_Width(m_Text) + Char_Pixel_Width(character) < (m_Width - 2) && m_TextLength < m_MaxTextLength
        && (isprint(character) || character == KA_SPACE) && (character != KA_SPACE || m_TextLength)) {
        if (m_EditStyle & EDIT_UPPER && isalpha(character)) {
            character = (KeyASCIIType)toupper(character);
        }

        if (m_EditStyle & EDIT_NUMS && isdigit(character) || m_EditStyle & EDIT_TEXT && isalpha(character)
            || m_EditStyle & EDIT_SYMS && !(isalnum(character)) || character == KA_SPACE) {
            m_Text[m_TextLength++] = character;
            m_Text[m_TextLength] = '\0';
            Flag_To_Redraw();

            return true;
        }
    }

    return true;
}

void EditClass::Set_Focus()
{
    m_TextLength = 0;

    if (m_Text != nullptr) {
        m_TextLength = (int)strlen(m_Text);
    }

    GadgetClass::Set_Focus();
}
