/**
 * @file
 *
 * @author tomsons26
 *
 * @brief Class representing a big checkbox element.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef BIGCHECK_H
#define BIGCHECK_H

#include "toggle.h"
#include "dialog.h"

class BigCheckBoxClass : public ToggleClass
{
public:
    BigCheckBoxClass(unsigned id, int x, int y, int w, int h, const char *text, TextPrintType style);
    BigCheckBoxClass(BigCheckBoxClass &that);
    virtual ~BigCheckBoxClass();

    virtual BOOL Draw_Me(BOOL redraw) override;
    virtual BOOL Action(unsigned flags, KeyNumType &key) override;

    BigCheckBoxClass & operator=(BigCheckBoxClass &that);

#ifndef RAPP_STANDALONE
    static void Hook_Me();
#endif

protected:
    TextPrintType TextStyle;
    const char *TextString;
};

inline BigCheckBoxClass &BigCheckBoxClass::operator=(BigCheckBoxClass &that)
{
    if (this != &that) {
        ToggleClass::operator=(that);
        TextStyle = that.TextStyle;
        TextString = that.TextString;
    }

    return *this;
}

#ifndef RAPP_STANDALONE
#include "hooker.h"

inline void BigCheckBoxClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x005AAC70, *BigCheckBoxClass::Draw_Me);
    Hook_Function(0x005AAD4C, *BigCheckBoxClass::Action);
#endif
}
#endif

#endif // BIGCHECK_H
