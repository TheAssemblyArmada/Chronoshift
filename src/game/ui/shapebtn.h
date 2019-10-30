/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Classes for representing button UI elements with sprites.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
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
    ShapeButtonClass() : ToggleClass(0, 0, 0, INPUT_NONE, false), m_BooleanOne(false) {}
    ShapeButtonClass(unsigned id, void *button_shape, int x, int y);
    ShapeButtonClass(ShapeButtonClass &that);
    virtual ~ShapeButtonClass() {}

    ShapeButtonClass &operator=(ShapeButtonClass &that);

    virtual BOOL Draw_Me(BOOL redraw) override;
    virtual void Set_Shape(void *button_shape);

    void *Get_Shape() const { return m_ButtonShape; }
    void Set_Shape_Bool_One(BOOL unk) { m_BooleanOne = unk; }

protected:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_BooleanOne : 1; // & 1
#else
    bool m_BooleanOne;
#endif
    void *m_ButtonShape;
};

#endif // SHAPEBTN_H
