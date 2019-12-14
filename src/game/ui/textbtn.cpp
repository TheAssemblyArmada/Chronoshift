/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class representing a clickable button with text.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "textbtn.h"
#include "gbuffer.h"
#include "mouse.h"
#include "textprint.h"

TextButtonClass::TextButtonClass() :
    ToggleClass(0, 0, 0, 0, 0),
    m_HasOutline(true),
    m_FilledBackground(false),
    m_HasShadow(false),
    m_ButtonText(nullptr)
{
}

TextButtonClass::TextButtonClass(unsigned id, const char *string, TextPrintType style, int x, int y, int w, int h, BOOL outline) :
    ToggleClass(id, x, y, w, h),
    m_HasOutline(outline),
    m_FilledBackground(false),
    m_HasShadow(false),
    m_ButtonText(string)
{
    Set_Style(style);
    Calculate_Button_Size(w, h);
}

TextButtonClass::TextButtonClass(unsigned id, int str_id, TextPrintType style, int x, int y, int w, int h, BOOL outline) :
    ToggleClass(id, x, y, w, h),
    m_HasOutline(outline),
    m_FilledBackground(false),
    m_HasShadow(false),
    m_ButtonText(nullptr)
{
    Set_Style(style);
    Set_Text(str_id, false);
    Calculate_Button_Size(w, h);
}

TextButtonClass::TextButtonClass(TextButtonClass &that) :
    ToggleClass(that),
    m_HasOutline(that.m_HasOutline),
    m_FilledBackground(that.m_FilledBackground),
    m_ButtonText(that.m_ButtonText),
    m_TextStyle(that.m_TextStyle)
{
}

BOOL TextButtonClass::Draw_Me(BOOL redraw)
{
    if (ControlClass::Draw_Me(redraw)) {
        if (g_LogicPage == &g_SeenBuff) {
            g_Mouse->Conditional_Hide_Mouse(m_XPos, m_YPos, m_Width + m_XPos - 1, m_Height + m_YPos - 1);
        }

        Draw_Background();
        Draw_Text(m_ButtonText);

        if (g_LogicPage == &g_SeenBuff) {
            g_Mouse->Conditional_Show_Mouse();
        }

        return true;
    }

    return false;
}

void TextButtonClass::Set_Text(const char *string, BOOL adjust)
{
    m_ButtonText = string;
    Flag_To_Redraw();

    // If adjust is set then we also adjust the width and height for the new
    // text string.
    if (m_ButtonText && adjust) {
        Fancy_Text_Print(nullptr, 0, 0, nullptr, 0, m_TextStyle);
        m_Width = String_Pixel_Width(m_ButtonText) + 8;
        m_Height = g_FontYSpacing + g_FontHeight + 2;
    }
}

void TextButtonClass::Set_Text(int str_id, BOOL adjust)
{
    if (str_id) {
        Set_Text(Text_String(str_id), adjust);
    }
}

void TextButtonClass::Draw_Background()
{
    BoxStyleEnum style;

    if (m_HasOutline) {
        g_LogicPage->Draw_Rect(m_XPos - 1, m_YPos - 1, m_Width + m_XPos + 2, m_Height + m_YPos + 2, COLOR_BLACK);
    }

    // Select required box style based on button state.
    if (m_IsDisabled) {
        style = BOX_STYLE_4;
    } else {
        if (m_Toggle_Boolean1) {
            style = BOX_STYLE_0;
        } else {
            style = BOX_STYLE_1;
        }
    }

    Draw_Box(m_XPos, m_YPos, m_Width, m_Height, style, true);
}

void TextButtonClass::Draw_Text(const char *string)
{
    TextPrintType style = m_TextStyle;

    if (m_ButtonText != nullptr) {
        if (!m_IsDisabled) {
            if (m_Toggle_Boolean1 || m_ToggleState) {
                style |= TPF_USE_BRIGHT | TPF_USE_GRAD_PAL;
            } else {
                style |= TPF_USE_MEDIUM | TPF_USE_GRAD_PAL;
            }
        }

        // Set the text alignment to the center
        style |= TPF_CENTER;

        // Print the button text!
        Fancy_Text_Print(string, (m_Width / 2) + m_XPos - 1, m_YPos + 1, GadgetClass::s_ColorScheme, COLOR_TBLACK, style);
    }
}

void TextButtonClass::Calculate_Button_Size(int w, int h)
{
    // If we got invalid width and height values, calc them from string.
    if (w == -1 || h == -1) {
        Fancy_Text_Print(nullptr, 0, 0, nullptr, 0, m_TextStyle);

        if (w == -1) {
            m_Width = String_Pixel_Width(m_ButtonText) + 8;
        }

        if (h == -1) {
            m_Height = g_FontYSpacing + g_FontHeight + 2;
        }
    }
}
