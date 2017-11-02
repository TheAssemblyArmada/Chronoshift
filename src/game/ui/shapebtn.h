/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Classes for representing button UI elements with sprites.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef SHAPEBTN_H
#define SHAPEBTN_H

#include "always.h"
#include "dialog.h"
#include "language.h"
#include "toggle.h"

class ShapeButtonClass : public ToggleClass
{
    enum
    {
        SHAPE_DEFAULT,
        SHAPE_ENABLED,
        SHAPE_DISABLED,
    };
public:
    ShapeButtonClass() : ToggleClass(0, 0, 0, INPUT_NONE, false), BooleanOne(false) {}
    ShapeButtonClass(unsigned id, void *button_shape, int x, int y);
    ShapeButtonClass(ShapeButtonClass &that);
    virtual ~ShapeButtonClass() {}

    ShapeButtonClass &operator=(ShapeButtonClass &that);

    virtual BOOL Draw_Me(BOOL redraw) override;
    virtual void Set_Shape(void *button_shape);

    void *Get_Shape() { return ButtonShape; }

#ifndef RAPP_STANDALONE
static void Hook_Me();
#endif

protected:
#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool BooleanOne : 1; // & 1
        };
        int m_shapebtnFlags;
    };
#else
    bool BooleanOne;
#endif
    void *ButtonShape;
};


#ifndef RAPP_STANDALONE
#include "hooker.h"

inline void ShapeButtonClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x0054CD7C, *ShapeButtonClass::Set_Shape);
    Hook_Function(0x0054CDB0, *ShapeButtonClass::Draw_Me);
#endif
}
#endif

#endif // SHAPEBTN_H
