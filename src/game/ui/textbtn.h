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
#pragma once

#ifndef TEXTBTN_H
#define TEXTBTN_H

#include "always.h"
#include "dialog.h"
#include "language.h"
#include "toggle.h"

class TextButtonClass : public ToggleClass
{
public:
    TextButtonClass();
    TextButtonClass(unsigned id, const char *string, TextPrintType style, int x, int y, int w = -1, int h = -1, BOOL outline = false);
    TextButtonClass(unsigned id, int str_id, TextPrintType style, int x, int y, int w = -1, int h = -1, BOOL outline = false);
    TextButtonClass(TextButtonClass &that);
    virtual ~TextButtonClass() {}

    TextButtonClass &operator=(TextButtonClass &that);

    virtual BOOL Draw_Me(BOOL redraw) override;
    virtual void Set_Text(const char *string, BOOL adjust = true);
    virtual void Set_Text(int str_id, BOOL adjust = true);
    virtual void Set_Style(TextPrintType style) { TextStyle = style; }
    virtual void Draw_Background();
    virtual void Draw_Text(const char *string);

#ifndef CHRONOSHIFT_STANDALONE
    static void Hook_Me();
#endif

private:
    void Calculate_Button_Size(int w, int h);

protected:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL HasOutline : 1; // & 1
    BOOL FilledBackground : 1; // & 2
    BOOL HasShadow : 1; // & 4
#else
    bool HasOutline;
    bool FilledBackground;
    bool HasShadow;
#endif
    const char *ButtonText;
    TextPrintType TextStyle;
};


inline TextButtonClass &TextButtonClass::operator=(TextButtonClass &that)
{
    if (this != &that) {
        ToggleClass::operator=(that);
        FilledBackground = that.FilledBackground;
        HasShadow = that.HasShadow;
        HasOutline = that.HasOutline;
        ButtonText = that.ButtonText;
        TextStyle = that.TextStyle;
    }

    return *this;
}

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"

inline void TextButtonClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x0056BC2C, *TextButtonClass::Draw_Me);
    Hook_Function(0x0056BD74, *TextButtonClass::Draw_Background);
    Hook_Function(0x0056BDE4, *TextButtonClass::Draw_Text);
#endif
}
#endif

#endif // TEXTBTN_H
