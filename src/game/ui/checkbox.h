/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class representing a ehckbox element.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "always.h"
#include "toggle.h"

class CheckBoxClass : public ToggleClass
{
public:
    CheckBoxClass(unsigned id, int x, int y, int w, int h) : ToggleClass(id, x, y, w, h) {}
    CheckBoxClass(CheckBoxClass &that) : ToggleClass(that) {}
    virtual ~CheckBoxClass() {}

    virtual BOOL Draw_Me(BOOL redraw) override;
    virtual BOOL Action(unsigned flags, KeyNumType &key) override;

    CheckBoxClass &operator=(CheckBoxClass &that);
};

inline CheckBoxClass &CheckBoxClass::operator=(CheckBoxClass &that)
{
    if (this != &that) {
        ToggleClass::operator=(that);
    }

    return *this;
}

#endif // CHECKBOX_H
