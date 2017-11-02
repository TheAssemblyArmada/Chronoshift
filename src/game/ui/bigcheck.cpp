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
#include "bigcheck.h"
#include "ccfileclass.h"
#include "drawshape.h"
#include "mixfile.h"
#include "mouse.h"
#include "stringex.h"

BigCheckBoxClass::BigCheckBoxClass(unsigned id, int x, int y, int w, int h, const char *text, TextPrintType style) :
    ToggleClass(id, x, y, w, h),
    TextStyle(style),
    TextString(nstrdup(text))
{
    ToggleDisabled = true;
}

BigCheckBoxClass::BigCheckBoxClass(BigCheckBoxClass &that) :
    ToggleClass(that),
    TextStyle(that.TextStyle),
    TextString(nstrdup(that.TextString))
{
}

BigCheckBoxClass::~BigCheckBoxClass()
{
    delete[] TextString;
}

BOOL BigCheckBoxClass::Draw_Me(BOOL redraw)
{
    if (ControlClass::Draw_Me(redraw)) {
        g_mouse->Hide_Mouse();

        int shape_frame = 0;

        if (!IsDisabled) {
            if (!ToggleState) {
                shape_frame = 0; // unchecked
            } else {
                shape_frame = 1; // checked
            }
        } else {
            if (!ToggleState) {
                shape_frame = 2; // disabled unchecked
            } else {
                shape_frame = 3; // disabled checked
            }
        }

        void *bigcheckboxshape = MixFileClass<CCFileClass>::Retrieve("bigcheck.shp");
        CC_Draw_Shape(bigcheckboxshape, shape_frame, XPos, YPos);
        Conquer_Clip_Text_Print(
            TextString, XPos + 20, YPos, GadgetClass::Get_Color_Scheme(), 0, TextStyle, Width, nullptr);

        g_mouse->Show_Mouse();

        return true;
    }

    return false;
}

int BigCheckBoxClass::Action(unsigned flags, KeyNumType &key)
{
    return ToggleClass::Action(flags, key);
}
