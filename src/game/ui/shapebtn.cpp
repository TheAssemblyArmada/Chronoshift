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
#include "shapebtn.h"
#include "drawshape.h"
#include "gamedebug.h"
#include "mouse.h"

ShapeButtonClass::ShapeButtonClass(unsigned id, void *button_shape, int x, int y) :
    ToggleClass(id, x, y, INPUT_NONE, false),
    BooleanOne(false)
{
    Set_Shape(button_shape);
}

ShapeButtonClass::ShapeButtonClass(ShapeButtonClass &that) :
    ToggleClass(that),
    BooleanOne(that.BooleanOne),
    ButtonShape(that.ButtonShape)
{
}

ShapeButtonClass &ShapeButtonClass::operator=(ShapeButtonClass &that)
{
    if (this != &that) {
        ToggleClass::operator=(that);
        BooleanOne = that.BooleanOne;
        ButtonShape = that.ButtonShape;
    }

    return *this;
}

BOOL ShapeButtonClass::Draw_Me(BOOL redraw)
{
    if (ToggleClass::Draw_Me(redraw) && ButtonShape != nullptr) {
        if (g_logicPage == &g_seenBuff) {
            g_mouse->Conditional_Hide_Mouse(XPos, YPos, Width + XPos - 1, Height + YPos - 1);
        }

        int frame_count = Get_Build_Frame_Count(ButtonShape);
        DEBUG_ASSERT(frame_count > 0);

        int shape_frame = SHAPE_DEFAULT;

        if (frame_count > 1) {
            if (ToggleDisabled) {
                shape_frame = SHAPE_DISABLED;
            } else {
                if (BooleanOne) {
                    shape_frame = ToggleState ? SHAPE_ENABLED : SHAPE_DEFAULT;
                } else {
                    shape_frame = Toggle_Boolean1 ? SHAPE_ENABLED : SHAPE_DEFAULT;
                }
            }
        }

        CC_Draw_Shape(ButtonShape, shape_frame, XPos, YPos);

        if (g_logicPage == &g_seenBuff) {
            g_mouse->Conditional_Show_Mouse();
        }

        return true;
    }

    return false;
}

void ShapeButtonClass::Set_Shape(void *button_shape)
{
    DEBUG_ASSERT(Get_Build_Frame_Count(button_shape) > 0);

    ButtonShape = button_shape;

    if (ButtonShape != nullptr) {
        Width = Get_Build_Frame_Width(ButtonShape);
        Height = Get_Build_Frame_Height(ButtonShape);
    }
}
