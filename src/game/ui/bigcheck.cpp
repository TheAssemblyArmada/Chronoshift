#include "bigcheck.h"
#include "drawshape.h"
#include "mouse.h"
#include "mixfile.h"
#include "ccfileclass.h"

BigCheckBoxClass::~BigCheckBoxClass(void)
{
    delete[] m_textString;
}

BOOL BigCheckBoxClass::Draw_Me(BOOL redraw)
{
    if (ControlClass::Draw_Me(redraw)) {
        g_mouse->Hide_Mouse();

        // default frame
        int shape_frame = 0;

        // Original ifs
        // if (ToggleState) {
        //    if (IsDisabled) {
        //        shape_frame = 3; // disabled checked
        //    } else {
        //        shape_frame = 1; // checked
        //    }
        //} else {
        //    if (!IsDisabled) {
        //        shape_frame = 0; // unchecked
        //    } else {
        //        shape_frame = 2; // disabled unchecked
        //    }
        //}

        // Rewritten to seem more logical
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
            m_textString, XPos + 20, YPos, GadgetClass::Get_Color_Scheme(), 0, m_textStyle, Width, nullptr);

        g_mouse->Show_Mouse();

        return true;
    }

    return false;
}

int BigCheckBoxClass::Action(unsigned flags, KeyNumType &key)
{
    return ToggleClass::Action(flags, key);
}