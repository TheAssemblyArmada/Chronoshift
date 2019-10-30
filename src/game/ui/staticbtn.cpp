/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class for static text UI gadgets.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "staticbtn.h"
#include "gbuffer.h"
#include "mouse.h"
#include "nstrdup.h"
#include "stringex.h"
#include "textprint.h"

StaticButtonClass::StaticButtonClass(unsigned id, const char *text, TextPrintType style, int x, int y, int w, int h) :
    GadgetClass(x, y, w, h, INPUT_NONE, false),
    m_ButtonText(nullptr),
    m_TextStyle(style)
{
    Set_Text(text, false);

    // If we got invalid width and height values, calc them from string.
    if (w == -1 || h == -1) {
        Fancy_Text_Print(nullptr, 0, 0, nullptr, COLOR_TBLACK, m_TextStyle);

        if (w == -1) {
            m_Width = String_Pixel_Width(m_ButtonText);
        }

        if (h == -1) {
            m_Height = g_fontHeight;
        }
    }
}

StaticButtonClass::StaticButtonClass(StaticButtonClass &that) :
    GadgetClass(that),
    m_DrawBuffer(that.m_DrawBuffer),
    m_ButtonText(that.m_ButtonText),
    m_TextStyle(that.m_TextStyle)
{
}

BOOL StaticButtonClass::Draw_Me(BOOL redraw)
{
    if (GadgetClass::Draw_Me(redraw)) {
        if (g_logicPage == &g_seenBuff) {
            g_mouse->Conditional_Hide_Mouse(m_XPos, m_YPos, m_Width + m_XPos - 1, m_Height + m_YPos - 1);
        }

        Draw_Background();
        Draw_Text(m_ButtonText);

        if (g_logicPage == &g_seenBuff) {
            g_mouse->Conditional_Show_Mouse();
        }

        return true;
    }

    return false;
}

void StaticButtonClass::Set_Text(const char *string, BOOL adjust)
{
    if (m_ButtonText) {
        delete[] m_ButtonText;
        m_ButtonText = nullptr;
    }

    m_ButtonText = nstrdup(string);

    Flag_To_Redraw();

    // If adjust is set then we also adjust the width and height for the new
    // text string.
    if (adjust && m_ButtonText) {
        Draw_Background();

        Fancy_Text_Print(nullptr, 0, 0, nullptr, COLOR_TBLACK, m_TextStyle);
        m_Width = String_Pixel_Width(m_ButtonText) + 8;
        m_Height = g_fontYSpacing + g_fontHeight + 2;

        BufferClass buff;
        m_DrawBuffer = buff;
    }
}

void StaticButtonClass::Draw_Background()
{
    // If we don't have the buffer allocated already, allocate it and grab whatever is in the viewport to it.
    if (m_DrawBuffer.Get_Buffer() == nullptr && m_Width > 0 && m_Height > 0) {
        m_DrawBuffer.Resize(nullptr, m_Width * m_Height);

        if (m_DrawBuffer.Get_Buffer() != nullptr) {
            g_logicPage->To_Buffer(m_XPos, m_YPos, m_Width, m_Height, m_DrawBuffer.Get_Buffer(), m_DrawBuffer.Get_Size());
        }
    }

    // Draw what we have in the buffer back to the viewport, sampled once, the replaced each draw.
    if (m_DrawBuffer.Get_Buffer()) {
        g_logicPage->From_Buffer(m_XPos, m_YPos, m_Width, m_Height, m_DrawBuffer.Get_Buffer());
    }
}

void StaticButtonClass::Draw_Text(const char *string)
{
    if (m_ButtonText) {
        int xpos = m_XPos;

        if (m_TextStyle & TPF_CENTER) {
            xpos += m_Width / 2;
        }

        if (m_TextStyle & TPF_RIGHT) {
            xpos += m_Width - 1;
        }

        // Print the button text!
        Fancy_Text_Print(string, xpos, m_YPos, GadgetClass::ColorScheme, COLOR_TBLACK, m_TextStyle);
    }
}
