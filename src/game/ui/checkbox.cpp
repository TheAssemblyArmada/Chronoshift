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
#include "checkbox.h"
#include "dialog.h"
#include "gbuffer.h"
#include "mouse.h"

BOOL CheckBoxClass::Draw_Me(BOOL redraw)
{
    if (ControlClass::Draw_Me(redraw)) {
        g_mouse->Hide_Mouse();
        Draw_Box(m_XPos, m_YPos, m_Width, m_Height, BOX_STYLE_0, false);
        // This appears to be how it was originally
        // LogicPage->Fill_Rect(XPos + 1, YPos + 1,
        //                     XPos + Width - 2, YPos + Height - 2,
        //                     13);
        // if ( ToggleState ) {
        //    LogicPage->Fill_Rect(XPos + 1, YPos + 1,
        //                         XPos + Width - 2, YPos + Height - 2,
        //                         4);
        //}

        // This way only calls Fill_Rect once.
        int fill_color = m_ToggleState ? 4 : 13;
        g_logicPage->Fill_Rect(m_XPos + 1, m_YPos + 1, m_XPos + m_Width - 2, m_YPos + m_Height - 2, fill_color);
        g_mouse->Show_Mouse();

        return true;
    }

    return false;
}

BOOL CheckBoxClass::Action(unsigned flags, KeyNumType &key)
{
    if (flags & MOUSE_LEFT_RLSE) {
        if (m_ToggleState) {
            Turn_Off();
        } else {
            Turn_On();
        }
    }

    return ToggleClass::Action(flags, key);
}
