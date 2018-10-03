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
    Delay(0),
    ID(0),
    TextStyle(style),
    LabelText(text),
    Remap(remap),
    MaxWidth(-1)
{
}

TextLabelClass::TextLabelClass(TextLabelClass &that) :
    GadgetClass(that),
    Delay(that.Delay),
    ID(that.ID),
    TextStyle(that.TextStyle),
    LabelText(that.LabelText),
    Remap(that.Remap),
    MaxWidth(that.MaxWidth)
{
}

BOOL TextLabelClass::Draw_Me(BOOL redraw)
{
    if (GadgetClass::Draw_Me(redraw)) {
        if (MaxWidth == -1) {
            Simple_Text_Print(LabelText, XPos, YPos, Remap, 0, TextStyle);
        } else {
            Conquer_Clip_Text_Print(LabelText, XPos, YPos, Remap, 0, TextStyle, MaxWidth, nullptr);
        }

        return true;
    }

    return false;
}

void TextLabelClass::Set_Style(TextPrintType style)
{
    TextStyle = style;
}
