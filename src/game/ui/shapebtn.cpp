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
#include "shapebtn.h"
#include "drawshape.h"
#include "gamedebug.h"
#include "gbuffer.h"
#include "mouse.h"

ShapeButtonClass::ShapeButtonClass(unsigned id, void *button_shape, int x, int y) :
    ToggleClass(id, x, y, INPUT_NONE, false),
    m_BooleanOne(false)
{
    Set_Shape(button_shape);
}

ShapeButtonClass::ShapeButtonClass(ShapeButtonClass &that) :
    ToggleClass(that),
    m_BooleanOne(that.m_BooleanOne),
    m_ButtonShape(that.m_ButtonShape)
{
}

ShapeButtonClass &ShapeButtonClass::operator=(ShapeButtonClass &that)
{
    if (this != &that) {
        ToggleClass::operator=(that);
        m_BooleanOne = that.m_BooleanOne;
        m_ButtonShape = that.m_ButtonShape;
    }

    return *this;
}

BOOL ShapeButtonClass::Draw_Me(BOOL redraw)
{
    if (!ToggleClass::Draw_Me(redraw) || m_ButtonShape == nullptr) {
        return false;
    }

    if (g_LogicPage == &g_SeenBuff) {
        g_Mouse->Conditional_Hide_Mouse(m_XPos, m_YPos, m_Width + m_XPos - 1, m_Height + m_YPos - 1);
    }

    int shape_frame = SHAPE_DEFAULT;

    if (m_IsDisabled) {
        shape_frame = SHAPE_DISABLED;
    } else if (m_BooleanOne) {
        shape_frame = m_ToggleState ? SHAPE_ENABLED : SHAPE_DEFAULT;
    } else {
        shape_frame = m_Toggle_Boolean1 ? SHAPE_ENABLED : SHAPE_DEFAULT;
    }

    CC_Draw_Shape(m_ButtonShape, shape_frame, m_XPos, m_YPos);

    if (g_LogicPage == &g_SeenBuff) {
        g_Mouse->Conditional_Show_Mouse();
    }

    return true;
}

void ShapeButtonClass::Set_Shape(void *button_shape)
{
    DEBUG_ASSERT(Get_Build_Frame_Count(button_shape) > 0);

    m_ButtonShape = button_shape;

    if (m_ButtonShape != nullptr) {
        m_Width = Get_Build_Frame_Width(m_ButtonShape);
        m_Height = Get_Build_Frame_Height(m_ButtonShape);
    }
}
