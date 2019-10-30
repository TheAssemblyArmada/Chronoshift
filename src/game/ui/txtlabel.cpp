/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class representing UI text labels.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "txtlabel.h"
#include "language.h"

TextLabelClass::TextLabelClass(char *text, int x, int y, RemapControlType *remap, TextPrintType style) :
    GadgetClass(x, y, 1, 1, INPUT_NONE, false),
    m_Lifetime(0),
    ID(0),
    m_TextStyle(style),
    LabelText(text),
    m_Remap(remap),
    m_MaxWidth(-1)
{
}

TextLabelClass::TextLabelClass(TextLabelClass &that) :
    GadgetClass(that),
    m_Lifetime(that.m_Lifetime),
    ID(that.ID),
    m_TextStyle(that.m_TextStyle),
    LabelText(that.LabelText),
    m_Remap(that.m_Remap),
    m_MaxWidth(that.m_MaxWidth)
{
}

BOOL TextLabelClass::Draw_Me(BOOL redraw)
{
    if (GadgetClass::Draw_Me(redraw)) {
        if (m_MaxWidth == -1) {
            Simple_Text_Print(LabelText, m_XPos, m_YPos, m_Remap, 0, m_TextStyle);
        } else {
            Conquer_Clip_Text_Print(LabelText, m_XPos, m_YPos, m_Remap, 0, m_TextStyle, m_MaxWidth, nullptr);
        }

        return true;
    }

    return false;
}

void TextLabelClass::Set_Style(TextPrintType style)
{
    m_TextStyle = style;
}
