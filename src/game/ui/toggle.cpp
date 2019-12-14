/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Base class for toggleable UI elements.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "toggle.h"
#include "mouse.h"

ToggleClass::ToggleClass(unsigned id, int x, int y, int w, int h) :
    ControlClass(id, x, y, w, h, MOUSE_LEFT_PRESS | MOUSE_LEFT_RLSE, true),
    m_Toggle_Boolean1(false),
    m_ToggleState(false),
    m_ToggleDisabled(false)
{
}

ToggleClass::ToggleClass(ToggleClass &that) :
    ControlClass(that),
    m_Toggle_Boolean1(that.m_Toggle_Boolean1),
    m_ToggleState(that.m_ToggleState),
    m_ToggleDisabled(that.m_ToggleDisabled)
{
}

BOOL ToggleClass::Action(unsigned flags, KeyNumType &key)
{
    if (!flags) {
        if (((unsigned)g_Mouse->Get_Mouse_X() - m_XPos) < (unsigned)m_Width
            && ((unsigned)g_Mouse->Get_Mouse_Y() - m_YPos) < (unsigned)m_Height) {
            if (!m_Toggle_Boolean1) {
                m_Toggle_Boolean1 = true;
                Flag_To_Redraw();
            }
        } else {
            if (m_Toggle_Boolean1) {
                m_Toggle_Boolean1 = false;
                Flag_To_Redraw();
            }
        }
    }

    Sticky_Process(flags);

    if (flags & MOUSE_LEFT_PRESS) {
        m_Toggle_Boolean1 = true;
        Flag_To_Redraw();
        ControlClass::Action(flags & ~(MOUSE_LEFT_PRESS | MOUSE_RIGHT_PRESS), key);
        key = KN_NONE;
        return true;
    }

    if (flags & MOUSE_LEFT_RLSE) {
        if (m_Toggle_Boolean1) {
            if (m_ToggleDisabled) {
                if (((unsigned)g_Mouse->Get_Mouse_X() - m_XPos) < (unsigned)m_Width
                    && ((unsigned)g_Mouse->Get_Mouse_Y() - m_YPos) < (unsigned)m_Height) {
                    m_ToggleState = (m_ToggleState == false);
                }
            }

            m_Toggle_Boolean1 = false;
            Flag_To_Redraw();
        } else {
            flags &= ~MOUSE_LEFT_RLSE;
        }
    }

    return ControlClass::Action(flags, key);
}

void ToggleClass::Turn_On()
{
    m_ToggleState = true;
    Flag_To_Redraw();
}

void ToggleClass::Turn_Off()
{
    m_ToggleState = false;
    Flag_To_Redraw();
}
