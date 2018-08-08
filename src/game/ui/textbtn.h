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
    TextButtonClass(unsigned id, const char *string, TextPrintType style, int x, int y, int w = -1, int h = -1, BOOL outline = true);
    TextButtonClass(unsigned id, int str_id, TextPrintType style, int x, int y, int w = -1, int h = -1, BOOL outline = true);
    TextButtonClass(TextButtonClass &that);
    virtual ~TextButtonClass() {}

    TextButtonClass &operator=(TextButtonClass &that);

    virtual BOOL Draw_Me(BOOL redraw) override;
    virtual void Set_Text(const char *string, BOOL adjust = true);
    virtual void Set_Text(int str_id, BOOL adjust = true);
    virtual void Set_Style(TextPrintType style);
    virtual void Draw_Background();
    virtual void Draw_Text(const char *string);

#ifndef CHRONOSHIFT_STANDALONE
    static void Hook_Me();
#endif

protected:
#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool HasOutline : 1; // & 1
            bool FilledBackground : 1; // & 2
            bool HasShadow : 1; // & 4
        };
        int m_toggleFlags;
    };
#else
    bool HasOutline;
    bool FilledBackground;
    bool HasShadow;
#endif
    const char *ButtonText;
    TextPrintType TextStyle;

    // uint8_t OutlineColor;
    // uint8_t ShadowColor;
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
        //OutlineColor = that.OutlineColor;
        //ShadowColor = that.ShadowColor;
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
