/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class representing UI text labels.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef TXTLABEL_H
#define TXTLABEL_H

#include "always.h"
#include "dialog.h"
#include "gadget.h"
#include "language.h"

class TextLabelClass : public GadgetClass
{
public:
    TextLabelClass(char *text, int x, int y, RemapControlType *remap, TextPrintType style);
    TextLabelClass(TextLabelClass &that);
    virtual ~TextLabelClass() {}

    virtual BOOL Draw_Me(BOOL redraw = false) override;
    virtual void Set_Text(char *text) { LabelText = text; }

    TextLabelClass &operator=(TextLabelClass &that);
    
    void Set_Style(TextPrintType style);
    
protected:
    int UnkInt1;
    int UnkInt2;
    TextPrintType TextStyle;
    char *LabelText;
    RemapControlType *Remap;
    int XPosMax;
};

inline TextLabelClass &TextLabelClass::operator=(TextLabelClass &that)
{
    if (this != &that) {
        GadgetClass::operator=(that);
        UnkInt1 = that.UnkInt1;
        UnkInt2 = that.UnkInt2;
        TextStyle = that.TextStyle;
        LabelText = that.LabelText;
        Remap = that.Remap;
        XPosMax = that.XPosMax;
    }

    return *this;
}

#endif // TXTLABEL_H
