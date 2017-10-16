/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Base class for toggleable UI elements.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
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
    Toggle_Boolean1(false),
    ToggleState(false),
    Toggle_Boolean3(false)
{
}

ToggleClass::ToggleClass(ToggleClass &that) :
    ControlClass(that),
    Toggle_Boolean1(that.Toggle_Boolean1),
    ToggleState(that.ToggleState),
    Toggle_Boolean3(that.Toggle_Boolean3)
{
}

BOOL ToggleClass::Action(unsigned flags, KeyNumType &key)
{
    if (!flags) {
        if (((unsigned)g_mouse->Get_Mouse_X() - XPos) < (unsigned)Width
            && ((unsigned)g_mouse->Get_Mouse_Y() - YPos) < (unsigned)Height) {
            if (!Toggle_Boolean1) {
                Toggle_Boolean1 = true;
                Flag_To_Redraw();
            }
        } else {
            if (Toggle_Boolean1) {
                Toggle_Boolean1 = false;
                Flag_To_Redraw();
            }
        }
    }

    Sticky_Process(flags);

    if (flags & MOUSE_LEFT_PRESS) {
        Toggle_Boolean1 = true;
        Flag_To_Redraw();
        ControlClass::Action(flags & ~(MOUSE_LEFT_PRESS | MOUSE_RIGHT_PRESS), key);
        key = KN_NONE;
        return true;
    }

    if (flags & MOUSE_LEFT_RLSE) {
        if (Toggle_Boolean1) {
            if (Toggle_Boolean3) {
                if (((unsigned)g_mouse->Get_Mouse_X() - XPos) < (unsigned)Width
                    && ((unsigned)g_mouse->Get_Mouse_Y() - YPos) < (unsigned)Height) {
                    ToggleState = (ToggleState == false);
                }
            }

            Toggle_Boolean1 = false;
            Flag_To_Redraw();
        } else {
            flags &= ~MOUSE_LEFT_RLSE;
        }
    }

    return ControlClass::Action(flags, key);
}

void ToggleClass::Turn_On()
{
    ToggleState = true;
    Flag_To_Redraw();
}

void ToggleClass::Turn_Off()
{
    ToggleState = false;
    Flag_To_Redraw();
}
