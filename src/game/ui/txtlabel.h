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
    void Set_Lifetime(unsigned lifetime) { m_Lifetime = lifetime; }
    void Set_Label_ID(int id) { ID = id; }
    void Set_Remap(RemapControlType *remap) { m_Remap = remap; }
    void Set_Max_Width(int max) { m_MaxWidth = max; }
    char *Get_Text() { return LabelText; }
    TextPrintType Get_Style() { return m_TextStyle; }
    unsigned Get_Lifetime() { return m_Lifetime; }
    int Get_Label_ID() { return ID; }
    RemapControlType *Get_Remap() { return m_Remap; }

protected:
    unsigned m_Lifetime;
    int ID;
    TextPrintType m_TextStyle;
    char *LabelText;
    RemapControlType *m_Remap;
    int m_MaxWidth;
};

inline TextLabelClass &TextLabelClass::operator=(TextLabelClass &that)
{
    if (this != &that) {
        GadgetClass::operator=(that);
        m_Lifetime = that.m_Lifetime;
        ID = that.ID;
        m_TextStyle = that.m_TextStyle;
        LabelText = that.LabelText;
        m_Remap = that.m_Remap;
        m_MaxWidth = that.m_MaxWidth;
    }

    return *this;
}

#endif // TXTLABEL_H
