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
    void Set_Lifetime(unsigned lifetime) { Lifetime = lifetime; }
    void Set_ID(int id) { ID = id; }
    void Set_Remap(RemapControlType *remap) { Remap = remap; }
    void Set_Max_Width(int max) { MaxWidth = max; }
    char *Get_Text() { return LabelText; }
    TextPrintType Get_Style() { return TextStyle; }
    unsigned Get_Lifetime() { return Lifetime; }
    int Get_ID() { return ID; }
    RemapControlType *Get_Remap() { return Remap; }

protected:
    unsigned Lifetime;
    int ID;
    TextPrintType TextStyle;
    char *LabelText;
    RemapControlType *Remap;
    int MaxWidth;
};

inline TextLabelClass &TextLabelClass::operator=(TextLabelClass &that)
{
    if (this != &that) {
        GadgetClass::operator=(that);
        Lifetime = that.Lifetime;
        ID = that.ID;
        TextStyle = that.TextStyle;
        LabelText = that.LabelText;
        Remap = that.Remap;
        MaxWidth = that.MaxWidth;
    }

    return *this;
}

#endif // TXTLABEL_H
