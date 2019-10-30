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
#pragma once

#ifndef STATICBTN_H
#define STATICBTN_H

#include "always.h"
#include "buffer.h"
#include "dialog.h"
#include "gadget.h"
#include "language.h"

class StaticButtonClass : public GadgetClass
{
public:
    StaticButtonClass(unsigned id, const char *text, TextPrintType style, int x, int y, int w = -1, int h = -1);
    StaticButtonClass(StaticButtonClass &that);
    virtual ~StaticButtonClass() {}

    virtual BOOL Draw_Me(BOOL redraw) override;
    virtual void Set_Text(const char *string, BOOL adjust);
    virtual void Draw_Background();
    virtual void Draw_Text(const char *string);

    StaticButtonClass &operator=(StaticButtonClass &that);
    
    void Set_Style(TextPrintType style) { m_TextStyle = style; }

protected:
    BufferClass m_DrawBuffer;
    char *m_ButtonText;
    TextPrintType m_TextStyle;
};

inline StaticButtonClass &StaticButtonClass::operator=(StaticButtonClass &that)
{
    if (this != &that) {
        m_DrawBuffer = that.m_DrawBuffer;
        m_ButtonText = that.m_ButtonText;
        m_TextStyle = that.m_TextStyle;
    }

    return *this;
}

#endif // STATICBTN_H
