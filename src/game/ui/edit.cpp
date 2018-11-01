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
#include "mouse.h"
#include "textprint.h"

EditClass::EditClass(int id, char *textbuffer, int bufferlength, TextPrintType text_style, int x, int y, int w, int h,
    EditStyleType edit_style) :
    ControlClass(id, x, y, w, h, MOUSE_LEFT_PRESS, false),
    TextStyle(text_style & (~TPF_CENTER)),
    EditStyle(edit_style),
    Text(textbuffer),
    MaxTextLength(bufferlength - 1),
    TextLength(strlen(textbuffer)),
    ColorScheme(GadgetClass::ColorScheme)
{
    Flag_To_Redraw();
    Set_Text(textbuffer, bufferlength);

    if (w == -1 || h == -1) {
        Fancy_Text_Print(nullptr, 0, 0, nullptr, COLOR_TBLACK, TextStyle);
        if (h == -1) {
            Height = g_fontHeight + 1;
        }

        if (w == -1) {
            if (strlen(Text) == 0) {
                Width = (MaxTextLength + 1) * (g_fontXSpacing + Char_Pixel_Width('X')) + 2;
            } else {
                Width = String_Pixel_Width(Text) + 6;
            }
        }
    }

    IgnoreInput = 0;
}

EditClass::~EditClass()
{
    if (GadgetClass::Has_Focus()) {
        GadgetClass::Clear_Focus();
    }
}

EditClass::EditClass(EditClass &that) :
    ControlClass(that),
    TextStyle(that.TextStyle),
    EditStyle(that.EditStyle),
    Text(that.Text),
    MaxTextLength(that.MaxTextLength),
    TextLength(that.TextLength),
    ColorScheme(that.ColorScheme),
    IgnoreInput(that.IgnoreInput)
{
}

void EditClass::Set_Text(char *text, int max_len)
{
    MaxTextLength = max_len - 1;
    Text = text;
    TextLength = strlen(text);
    Flag_To_Redraw();
}

BOOL EditClass::Draw_Me(BOOL redraw)
{
    if (ControlClass::Draw_Me(redraw)) {
        if (g_logicPage == &g_seenBuff) {
            g_mouse->Conditional_Hide_Mouse(XPos, YPos, Width + XPos, Height + YPos);
        }

        Draw_Background();
        Draw_Text(Text);

        if (g_logicPage == &g_seenBuff) {
            g_mouse->Conditional_Show_Mouse();
        }

        return true;
    }

    return false;
}

BOOL EditClass::Action(unsigned flags, KeyNumType &key)
{
    char character;

    if (IgnoreInput) {
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
            character = g_keyboard->To_ASCII(key);
            if (key & 0x1000 && character >= '0' && character <= '9') {
                key = (KeyNumType)(key & (~0x1000));

                if (!(flags & MOUSE_LEFT_RLSE) && !(flags & MOUSE_RIGHT_RLSE) && Handle_Key((KeyASCIIType)character)) {
                    key = KN_NONE;
                    flags &= ~KEYBOARD_INPUT;

                    return ControlClass::Action(flags, key);
                }
            } else if ((key & 0x1000 || character < ' ' || character > '\xFF') && key != KN_RETURN && key != KN_BACKSPACE) {
                key = KN_NONE;
                flags &= ~KEYBOARD_INPUT;
            } else if (!(flags & MOUSE_LEFT_RLSE) && !(flags & MOUSE_RIGHT_RLSE)) {
                if (Handle_Key((KeyASCIIType)g_keyboard->To_ASCII(key))) {
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
    Draw_Box(XPos, YPos, Width, Height, BOX_STYLE_4, true);
}

void EditClass::Draw_Text(const char *text)
{
    TextPrintType style = Has_Focus() ? TPF_USE_BRIGHT : TPF_NONE;

    // Draw the current text
    Conquer_Clip_Text_Print(text, XPos + 1, YPos + 1, ColorScheme, 0, TextStyle | style, Width - 2, nullptr);

    // If the control has focus and it will fit in the space, draw an underscore
    // to represent the text cursor
    if (Has_Focus() && (int)strlen(text) < MaxTextLength) {
        int text_width = String_Pixel_Width(text);
        int cursor_width = String_Pixel_Width("_");
        if (cursor_width + text_width < Width - 2) {
            Conquer_Clip_Text_Print(
                "_", XPos + 1 + text_width, YPos + 1, ColorScheme, 0, TextStyle | style, Width - 2, nullptr);
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
            if (TextLength) {
                --TextLength;
                Text[TextLength] = '\0';
                Flag_To_Redraw();
            }

            return true;
        }

        if (character == KA_RETURN) {
            Clear_Focus();

            return false;
        }
    }

    if (String_Pixel_Width(Text) + Char_Pixel_Width(character) < (Width - 2) && TextLength < MaxTextLength
        && (isprint(character) || character == KA_SPACE) && (character != KA_SPACE || TextLength)) {
        if (EditStyle & EDIT_UPPER && isalpha(character)) {
            character = (KeyASCIIType)toupper(character);
        }

        if (EditStyle & EDIT_NUMS && isdigit(character) || EditStyle & EDIT_TEXT && isalpha(character)
            || EditStyle & EDIT_SYMS && !(isalnum(character)) || character == KA_SPACE) {
            Text[TextLength++] = character;
            Text[TextLength] = '\0';
            Flag_To_Redraw();

            return true;
        }
    }

    return true;
}

void EditClass::Set_Focus()
{
    TextLength = 0;

    if (Text != nullptr) {
        TextLength = (int)strlen(Text);
    }

    GadgetClass::Set_Focus();
}
