/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class for representing gauge like UI elements.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "gauge.h"
#include "dialog.h"
#include "fixed.h"
#include "gbuffer.h"
#include "mouse.h"
#include "remap.h"
#include <algorithm>

const int GaugeClass::DefaultThumbSize = 4;

GaugeClass::GaugeClass(unsigned id, int x, int y, int w, int h) :
    ControlClass(id, x, y, w, h, MOUSE_LEFT_PRESS | MOUSE_LEFT_HELD | MOUSE_LEFT_RLSE, true)
{
    Set_Maximum(255);
    Set_Value(0);

    m_UseThumb = true;
    m_IsHorizontal = w > h;
    m_FillGauge = true;
    m_LastPos = 0;
}

GaugeClass::GaugeClass(GaugeClass &that) :
    ControlClass(that),
    m_FillGauge(that.m_FillGauge),
    m_UseThumb(that.m_UseThumb),
    m_IsHorizontal(that.m_IsHorizontal),
    m_Maximum(that.m_Maximum),
    m_Value(that.m_Value),
    m_LastPos(that.m_LastPos)
{
}

BOOL GaugeClass::Draw_Me(BOOL redraw)
{
    if (ControlClass::Draw_Me(redraw)) {
        if (g_LogicPage == &g_SeenBuff) {
            g_Mouse->Conditional_Hide_Mouse(m_XPos, m_YPos, m_XPos + m_Width, m_YPos + m_Height);
        }

        Draw_Box(m_XPos, m_YPos, m_Width, m_Height, BOX_STYLE_0, true);

        if (m_FillGauge) {
            int pval = Value_To_Pixel(m_Value);

            if (m_IsHorizontal) {
                if (pval >= m_XPos + 1) {
                    g_LogicPage->Fill_Rect(
                        m_XPos + 1, m_YPos + 1, pval, m_YPos + m_Height - 2, GadgetClass::s_ColorScheme->WindowPalette[5]);
                }
            } else {
                if (pval >= m_YPos + 1) {
                    g_LogicPage->Fill_Rect(
                        m_XPos + 1, m_YPos + 1, m_XPos + m_Width - 2, pval, GadgetClass::s_ColorScheme->WindowPalette[5]);
                }
            }
        }

        if (m_UseThumb) {
            Draw_Thumb();
        }

        if (g_LogicPage == &g_SeenBuff) {
            g_Mouse->Conditional_Show_Mouse();
        }

        return true;
    }

    return false;
}

BOOL GaugeClass::Action(unsigned flags, KeyNumType &key)
{
    if (!m_UseThumb) {
        return false;
    }

    Sticky_Process(flags);

    if ((flags & MOUSE_LEFT_PRESS) || ((flags & MOUSE_LEFT_HELD) && this == GadgetClass::s_StuckOn)) {
        int valpixel;
        int mousepos;

        if (flags & MOUSE_LEFT_PRESS) {
            int pixelval = Value_To_Pixel(m_Value);
            mousepos = m_IsHorizontal ? g_Mouse->Get_Mouse_X() : g_Mouse->Get_Mouse_Y();

            if (mousepos <= pixelval) {
                m_LastPos = 0;
            } else {
                mousepos -= pixelval;
                if (mousepos >= Thumb_Pixels()) {
                    m_LastPos = mousepos;
                } else {
                    m_LastPos = 0;
                }
            }

            mousepos = m_IsHorizontal ? g_Mouse->Get_Mouse_X() : g_Mouse->Get_Mouse_Y();
            valpixel = Pixel_To_Value(mousepos - m_LastPos);

            for (int i = valpixel; i < m_Value;) {
                if (m_LastPos <= 0) {
                    break;
                }

                --m_LastPos;
                mousepos = m_IsHorizontal ? g_Mouse->Get_Mouse_X() : g_Mouse->Get_Mouse_Y();
                i = Pixel_To_Value(mousepos - m_LastPos);
            }
        }

        mousepos = m_IsHorizontal ? g_Mouse->Get_Mouse_X() : g_Mouse->Get_Mouse_Y();
        valpixel = Pixel_To_Value(mousepos - m_LastPos);

        if (!Set_Value(valpixel)) {
            ControlClass::Action(INPUT_NONE, key);
            return true;
        }
    } else {
        flags &= ~MOUSE_LEFT_HELD;
    }

    return ControlClass::Action(flags, key);
}

BOOL GaugeClass::Set_Maximum(int max)
{
    if (max != m_Maximum) {
        m_Maximum = max;
        Flag_To_Redraw();

        return true;
    }

    return false;
}

BOOL GaugeClass::Set_Value(int value)
{
    value = std::clamp(value, 0, m_Maximum);

    if (value != m_Value) {
        m_Value = value;
        Flag_To_Redraw();

        return true;
    }

    return false;
}

void GaugeClass::Use_Thumb(BOOL use_thumb)
{
    m_UseThumb = use_thumb;
}

void GaugeClass::Draw_Thumb()
{
    int start_pixel = Value_To_Pixel(m_Value);

    if (start_pixel + 4 > Value_To_Pixel(m_Maximum)) {
        start_pixel = Value_To_Pixel(m_Maximum) - 2;
    }

    start_pixel = std::max(m_XPos, start_pixel);

    if (m_IsHorizontal) {
        Draw_Box(start_pixel, m_YPos, Thumb_Pixels(), m_Height, BOX_STYLE_1, true);
    } else {
        Draw_Box(m_XPos, start_pixel, m_Width, Thumb_Pixels(), BOX_STYLE_1, true);
    }
}

int GaugeClass::Pixel_To_Value(int pixel)
{
    int offset;
    int max;

    if (m_IsHorizontal) {
        offset = pixel - (m_XPos + 1);
        max = m_Width - 2;
    } else {
        offset = pixel - (m_YPos + 1);
        max = m_Height - 2;
    }

    int value = std::clamp(offset, 0, max);
    fixed_t fval(value, max);

    return m_Maximum * fval;
}

int GaugeClass::Value_To_Pixel(int value)
{
    int offset;
    int max;

    if (m_IsHorizontal) {
        max = m_Width - 2;
        offset = m_XPos;
    } else {
        max = m_Height - 2;
        offset = m_YPos;
    }

    fixed_t result(value, m_Maximum);

    return offset + (max * result);
}
