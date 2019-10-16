/**
 * @file
 *
 * @author CCHyper
 *
 * @brief
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "trigauge.h"
#include "dialog.h"
#include "gbuffer.h"
#include "mouse.h"

/**
 * @brief
 *
 */
TriColorGaugeClass &TriColorGaugeClass::operator=(TriColorGaugeClass &that)
{
    if (this != &that) {
        GaugeClass::operator=(that);
        m_RedLimit = that.m_RedLimit;
        m_YellowLimit = that.m_YellowLimit;
    }
    return *this;
}

/**
 * @brief
 *
 * @address 0x004C553C
 */
BOOL TriColorGaugeClass::Draw_Me(BOOL redraw)
{
    if (ControlClass::Draw_Me(redraw)) {
        if (g_logicPage == &g_seenBuff) {
            g_mouse->Conditional_Hide_Mouse(XPos, YPos, (XPos + Width), (YPos + Height));
        }

        Draw_Box(XPos, YPos, Width, Height, IsDisabled ? BOX_STYLE_1 : BOX_STYLE_0, true);

        int vpixel = Value_To_Pixel(Value);
        int ypixel = Value_To_Pixel(m_YellowLimit);
        int rpixel = Value_To_Pixel(m_RedLimit);

        if (Value > m_RedLimit) {
            if (Value > m_RedLimit && Value <= m_YellowLimit) {
                if (IsHorizontal) {
                    g_logicPage->Fill_Rect(rpixel, YPos + 1, vpixel, Height + YPos - 2, COLOR_YELLOW);
                    g_logicPage->Fill_Rect(XPos + 1, YPos + 1, rpixel, Height + YPos - 2, COLOR_PINK);
                } else {
                    g_logicPage->Fill_Rect(XPos + 1, rpixel, Width + XPos - 2, vpixel, COLOR_YELLOW);
                    g_logicPage->Fill_Rect(XPos + 1, YPos + 1, Width + XPos - 2, rpixel, COLOR_PINK);
                }

            } else if (Value > m_YellowLimit && Value <= Maximum) {
                if (IsHorizontal) {
                    g_logicPage->Fill_Rect(ypixel, YPos + 1, vpixel, Height + YPos - 2, COLOR_GREEN);
                    g_logicPage->Fill_Rect(rpixel, YPos + 1, ypixel, Height + YPos - 2, COLOR_YELLOW);
                    g_logicPage->Fill_Rect(XPos + 1, YPos + 1, rpixel, Height + YPos - 2, COLOR_PINK);
                } else {
                    g_logicPage->Fill_Rect(XPos + 1, ypixel, Width + XPos - 2, vpixel, COLOR_GREEN);
                    g_logicPage->Fill_Rect(XPos + 1, rpixel, Width + XPos - 2, ypixel, COLOR_YELLOW);
                    g_logicPage->Fill_Rect(XPos + 1, YPos + 1, Width + XPos - 2, rpixel, COLOR_PINK);
                }
            }

        } else if (IsHorizontal) {
            g_logicPage->Fill_Rect(XPos + 1, YPos + 1, vpixel, Height + YPos - 2, COLOR_PINK);
        } else {
            g_logicPage->Fill_Rect(XPos + 1, YPos + 1, Width + XPos - 2, vpixel, COLOR_PINK);
        }

        if (UseThumb) {
            Draw_Thumb();
        }

        if (g_logicPage == &g_seenBuff) {
            g_mouse->Conditional_Show_Mouse();
        }

        return true;
    }

    return false;
}

/**
 * @brief
 *
 * @address 0x004C54F4
 */
bool TriColorGaugeClass::Set_Red_Limit(int value)
{
    if (value >= 0 && value < Maximum) {
        m_RedLimit = value;
        Flag_To_Redraw();
        return true;
    }

    return false;
}

/**
 * @brief
 *
 * @address 0x004C5518
 */
bool TriColorGaugeClass::Set_Yellow_Limit(int value)
{
    if (value >= 0 && value < Maximum) {
        m_YellowLimit = value;
        Flag_To_Redraw();
        return true;
    }

    return false;
}
