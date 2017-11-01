/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class for representing editable UI gadgets.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef EDIT_H
#define EDIT_H

#include "always.h"
#include "controlc.h"
#include "dialog.h"
#include "keyboard.h"

enum EditStyleType
{
    // Based on my attemps to dicipher the watcom IsTable
    // https://github.com/open-watcom/open-watcom-v2/blob/master/bld/clib/char/c/istable.c

    EDIT_NONE = 0,
    EDIT_TEXT = 1, // Text entry
    EDIT_NUMS = 2, // Number entry
    EDIT_SYMS = 4, // None alpha numeric characters
    EDIT_UPPER = 8 // Convert to uppercase
};

DEFINE_ENUMERATION_BITWISE_OPERATORS(EditStyleType);

class EditClass : public ControlClass
{
public:
    EditClass(int id, char *textbuffer, int bufferlen, TextPrintType text_style, int x, int y, int w, int h,
        EditStyleType edit_style);
    EditClass(EditClass &that);
    virtual ~EditClass();

    virtual void Set_Focus() override;
    virtual BOOL Draw_Me(BOOL redraw) override;
    virtual BOOL Action(unsigned flags, KeyNumType &key) override;
    virtual void Set_Text(char *text, int max_len);
    virtual char *Get_Text() { return Text; }
    virtual void Draw_Background();
    virtual void Draw_Text(const char *text);
    virtual int Handle_Key(KeyASCIIType keycode);

    EditClass &operator=(EditClass &that);

protected:
    TextPrintType TextStyle;
    EditStyleType EditStyle;
    char *Text;
    int MaxTextLength;
    int TextLength;
    RemapControlType *ColorScheme;
    int IgnoreInput;
};

inline EditClass &EditClass::operator=(EditClass &that)
{
    if (this != &that) {
        ControlClass::operator=(that);
        TextStyle = that.TextStyle;
        EditStyle = that.EditStyle;
        Text = that.Text;
        MaxTextLength = that.MaxTextLength;
        TextLength = that.TextLength;
        ColorScheme = that.ColorScheme;
        IgnoreInput = that.IgnoreInput;
    }

    return *this;
}

#endif // EDIT_H
