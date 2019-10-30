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
            g_mouse->Conditional_Hide_Mouse(m_XPos, m_YPos, (m_XPos + m_Width), (m_YPos + m_Height));
        }

        Draw_Box(m_XPos, m_YPos, m_Width, m_Height, m_IsDisabled ? BOX_STYLE_1 : BOX_STYLE_0, true);

        int vpixel = Value_To_Pixel(m_Value);
        int ypixel = Value_To_Pixel(m_YellowLimit);
        int rpixel = Value_To_Pixel(m_RedLimit);

        if (m_Value > m_RedLimit) {
            if (m_Value > m_RedLimit && m_Value <= m_YellowLimit) {
                if (m_IsHorizontal) {
                    g_logicPage->Fill_Rect(rpixel, m_YPos + 1, vpixel, m_Height + m_YPos - 2, COLOR_YELLOW);
                    g_logicPage->Fill_Rect(m_XPos + 1, m_YPos + 1, rpixel, m_Height + m_YPos - 2, COLOR_PINK);
                } else {
                    g_logicPage->Fill_Rect(m_XPos + 1, rpixel, m_Width + m_XPos - 2, vpixel, COLOR_YELLOW);
                    g_logicPage->Fill_Rect(m_XPos + 1, m_YPos + 1, m_Width + m_XPos - 2, rpixel, COLOR_PINK);
                }

            } else if (m_Value > m_YellowLimit && m_Value <= m_Maximum) {
                if (m_IsHorizontal) {
                    g_logicPage->Fill_Rect(ypixel, m_YPos + 1, vpixel, m_Height + m_YPos - 2, COLOR_GREEN);
                    g_logicPage->Fill_Rect(rpixel, m_YPos + 1, ypixel, m_Height + m_YPos - 2, COLOR_YELLOW);
                    g_logicPage->Fill_Rect(m_XPos + 1, m_YPos + 1, rpixel, m_Height + m_YPos - 2, COLOR_PINK);
                } else {
                    g_logicPage->Fill_Rect(m_XPos + 1, ypixel, m_Width + m_XPos - 2, vpixel, COLOR_GREEN);
                    g_logicPage->Fill_Rect(m_XPos + 1, rpixel, m_Width + m_XPos - 2, ypixel, COLOR_YELLOW);
                    g_logicPage->Fill_Rect(m_XPos + 1, m_YPos + 1, m_Width + m_XPos - 2, rpixel, COLOR_PINK);
                }
            }

        } else if (m_IsHorizontal) {
            g_logicPage->Fill_Rect(m_XPos + 1, m_YPos + 1, vpixel, m_Height + m_YPos - 2, COLOR_PINK);
        } else {
            g_logicPage->Fill_Rect(m_XPos + 1, m_YPos + 1, m_Width + m_XPos - 2, vpixel, COLOR_PINK);
        }

        if (m_UseThumb) {
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
    if (value >= 0 && value < m_Maximum) {
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
    if (value >= 0 && value < m_Maximum) {
        m_YellowLimit = value;
        Flag_To_Redraw();
        return true;
    }

    return false;
}
