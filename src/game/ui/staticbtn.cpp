/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class for static text UI gadgets.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "staticbtn.h"
#include "gbuffer.h"
#include "language.h"
#include "mouse.h"
#include "stringex.h"
#include "textprint.h"

StaticButtonClass::StaticButtonClass(unsigned id, const char *text, TextPrintType style, int x, int y, int w, int h) :
    GadgetClass(x, y, w, h, INPUT_NONE, false),
    ButtonText(nullptr),
    TextStyle(style)
{
    Set_Text(text, false);

    // If we got invalid width and height values, calc them from string.
    if (w == -1 || h == -1) {
        Fancy_Text_Print(nullptr, 0, 0, nullptr, COLOR_TBLACK, TextStyle);

        if (w == -1) {
            Width = String_Pixel_Width(ButtonText);
        }

        if (h == -1) {
            Height = g_fontHeight;
        }
    }
}

StaticButtonClass::StaticButtonClass(StaticButtonClass &that) :
    GadgetClass(that),
    DrawBuffer(that.DrawBuffer),
    ButtonText(that.ButtonText),
    TextStyle(that.TextStyle)
{
}

BOOL StaticButtonClass::Draw_Me(BOOL redraw)
{
    if (GadgetClass::Draw_Me(redraw)) {
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

void StaticButtonClass::Set_Text(const char *string, BOOL adjust)
{
    if (ButtonText) {
        delete[] ButtonText;
        ButtonText = nullptr;
    }

    ButtonText = nstrdup(string);

    Flag_To_Redraw();

    // If adjust is set then we also adjust the width and height for the new
    // text string.
    if (adjust && ButtonText) {
        Draw_Background();

        Fancy_Text_Print(nullptr, 0, 0, nullptr, COLOR_TBLACK, TextStyle);
        Width = String_Pixel_Width(ButtonText) + 8;
        Height = g_fontYSpacing + g_fontHeight + 2;

        BufferClass buff;
        DrawBuffer = buff;
    }
}

void StaticButtonClass::Draw_Background()
{
    // If we don't have the buffer allocated already, allocate it and grab whatever is in the viewport to it.
    if (DrawBuffer.Get_Buffer() == nullptr && Width > 0 && Height > 0 ) {
        DrawBuffer.Resize(nullptr, Width * Height);
        
        if (DrawBuffer.Get_Buffer() != nullptr) {
            g_logicPage->To_Buffer(XPos, YPos, Width, Height, DrawBuffer.Get_Buffer(), DrawBuffer.Get_Size());
        }
    }

    // Draw what we have in the buffer back to the viewport, sampled once, the replaced each draw.
    if (DrawBuffer.Get_Buffer()) {
        g_logicPage->From_Buffer(XPos, YPos, Width, Height, DrawBuffer.Get_Buffer());
    }
}

void StaticButtonClass::Draw_Text(const char *string)
{
    if (ButtonText) {
        int xpos = XPos;

        if (TextStyle & TPF_CENTER) {
            xpos += Width / 2;
        }

        if (TextStyle & TPF_RIGHT) {
            xpos += Width - 1;
        }

        // Print the button text!
        Fancy_Text_Print(string, xpos, YPos, GadgetClass::ColorScheme, COLOR_TBLACK, TextStyle);
    }
}
