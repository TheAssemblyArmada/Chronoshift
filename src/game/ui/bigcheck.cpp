/**
 * @file
 *
 * @author tomsons26
 *
 * @brief Class representing a big checkbox element.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "bigcheck.h"
#include "gamefile.h"
#include "drawshape.h"
#include "mixfile.h"
#include "mouse.h"
#include "nstrdup.h"
#include "stringex.h"

BigCheckBoxClass::BigCheckBoxClass(unsigned id, int x, int y, int w, int h, const char *text, TextPrintType style) :
    ToggleClass(id, x, y, w, h),
    m_TextStyle(style),
    m_TextString(nstrdup(text))
{
    m_ToggleDisabled = true;
}

BigCheckBoxClass::BigCheckBoxClass(BigCheckBoxClass &that) :
    ToggleClass(that),
    m_TextStyle(that.m_TextStyle),
    m_TextString(nstrdup(that.m_TextString))
{
}

BigCheckBoxClass::~BigCheckBoxClass()
{
    delete[] m_TextString;
}

BOOL BigCheckBoxClass::Draw_Me(BOOL redraw)
{
    if (ControlClass::Draw_Me(redraw)) {
        g_Mouse->Hide_Mouse();

        int shape_frame = 0;

        if (!m_IsDisabled) {
            if (!m_ToggleState) {
                shape_frame = 0; // unchecked
            } else {
                shape_frame = 1; // checked
            }
        } else {
            if (!m_ToggleState) {
                shape_frame = 2; // disabled unchecked
            } else {
                shape_frame = 3; // disabled checked
            }
        }

        void *bigcheckboxshape = GameFileClass::Retrieve("bigcheck.shp");
        CC_Draw_Shape(bigcheckboxshape, shape_frame, m_XPos, m_YPos);
        Conquer_Clip_Text_Print(
            m_TextString, m_XPos + 20, m_YPos, GadgetClass::Get_Color_Scheme(), 0, m_TextStyle, m_Width, nullptr);

        g_Mouse->Show_Mouse();

        return true;
    }

    return false;
}

int BigCheckBoxClass::Action(unsigned flags, KeyNumType &key)
{
    return ToggleClass::Action(flags, key);
}
