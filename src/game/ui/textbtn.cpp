/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class representing a clickable button with text.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "textbtn.h"
#include "dialog.h"
#include "gamedebug.h"
#include "gbuffer.h"
#include "language.h"
#include "mouse.h"
#include "textprint.h"

TextButtonClass::TextButtonClass() :
    ToggleClass(0, 0, 0, 0, 0),
    HasOutline(false),
    HasShadow(false),
    HasFilledBackground(true),
    ButtonText(nullptr)
{
}

TextButtonClass::TextButtonClass(unsigned id, const char *string, TextPrintType style, int x, int y, int w, int h, BOOL background) :
    ToggleClass(id, x, y, w, h),
    HasOutline(false),
    HasShadow(false),
    HasFilledBackground(background),
    ButtonText(string)
{
    Set_Style(style);

    // If we got invalid width and height values, calc them from string.
    if (w == -1 || h == -1) {
        Fancy_Text_Print(nullptr, 0, 0, nullptr, 0, TextStyle);

        if (w == -1) {
            Width = String_Pixel_Width(ButtonText) + 8;
        }

        if (h == -1) {
            Height = g_fontYSpacing + g_fontHeight + 2;
        }
    }
}

TextButtonClass::TextButtonClass(unsigned id, int str_id, TextPrintType style, int x, int y, int w, int h, BOOL background) :
    ToggleClass(id, x, y, w, h),
    HasOutline(false),
    HasShadow(false),
    HasFilledBackground(background),
    ButtonText(nullptr)
{
    Set_Style(style);
    Set_Text(str_id, false);

    // If we got invalid width and height values, calc them from string.
    if (w == -1 || h == -1) {
        Fancy_Text_Print(nullptr, 0, 0, nullptr, 0, TextStyle);

        if (w == -1) {
            Width = String_Pixel_Width(ButtonText) + 8;
        }

        if (h == -1) {
            Height = g_fontYSpacing + g_fontHeight + 2;
        }
    }
}

TextButtonClass::TextButtonClass(TextButtonClass &that) :
    ToggleClass(that),
    HasOutline(that.HasOutline),
    HasFilledBackground(that.HasFilledBackground),
    ButtonText(that.ButtonText),
    TextStyle(that.TextStyle)
{
}

BOOL TextButtonClass::Draw_Me(BOOL redraw)
{
    if (ControlClass::Draw_Me(redraw)) {
        if (g_logicPage == &g_seenBuff) {
            g_mouse->Conditional_Hide_Mouse(XPos, YPos, Width + XPos - 1, Height + YPos - 1);
        }

        Draw_Background();
        Draw_Text(ButtonText);

        if (g_logicPage == &g_seenBuff) {
            g_mouse->Conditional_Show_Mouse();
        }

        return true;
    }

    return false;
}

void TextButtonClass::Set_Text(const char *string, BOOL adjust)
{
    ButtonText = string;
    Flag_To_Redraw();

    // If adjust is set then we also adjust the width and height for the new
    // text string.
    if (ButtonText && adjust) {
        Fancy_Text_Print(nullptr, 0, 0, nullptr, 0, TextStyle);
        Width = String_Pixel_Width(ButtonText) + 8;
        Height = g_fontYSpacing + g_fontHeight + 2;
    }
}

void TextButtonClass::Set_Text(int str_id, BOOL adjust)
{
    if (str_id) {
        Set_Text(Fetch_String(str_id), adjust);
    }
}

void TextButtonClass::Set_Style(TextPrintType style)
{
    TextStyle = style;
}

void TextButtonClass::Draw_Background()
{
    BoxStyleEnum style;

    if (HasFilledBackground) {
        // Draw an drop shadow underneath the button if required.
        //if (HasShadow) {
        //    LogicPage->Draw_Line(Width + XPos + 2, YPos, Width + XPos + 2, Height + YPos + 2, ShadowColor);
        //    LogicPage->Draw_Line(XPos, Height + YPos + 2, Width + XPos + 2, Height + YPos + 2, ShadowColor);
        //}

        // Draw an outline around the button if required.
        //if (HasOutline) {
        //    LogicPage->Fill_Rect(XPos - 1, YPos - 1, Width + XPos + 2, Height + YPos + 2, OutlineColor);
        //}

        // Select required box style based on button state.
        if (IsDisabled) {
            style = BOX_STYLE_4;
        } else {
            if (Toggle_Boolean1) {
                style = BOX_STYLE_0;
            } else {
                style = BOX_STYLE_1;
            }
        }

        Draw_Box(XPos, YPos, Width, Height, style, true);
    }
}

void TextButtonClass::Draw_Text(const char *string)
{
    TextPrintType style = TextStyle;

    if (ButtonText != nullptr) {
        if (!IsDisabled) {
            if (Toggle_Boolean1 || ToggleState) {
                style = style | TPF_USE_BRIGHT | TPF_USE_GRAD_PAL;
            } else {
                style = style | TPF_USE_MEDIUM | TPF_USE_GRAD_PAL;
            }
        }

        // Set the text alignment to the center
        style = style | TPF_CENTER;

        // Print the button text!
        Fancy_Text_Print(string, (Width / 2) + XPos - 1, YPos + 1, GadgetClass::ColorScheme, COLOR_TBLACK, style);
    }
}
