/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class for representing slider elements such as scroll bars.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "slider.h"
#include "dialog.h"
#include "fixed.h"
#include "gamefile.h"
#include "gbuffer.h"
#include "mixfile.h"
#include "mouse.h"
#include "shapebtn.h"
#include <algorithm>

SliderClass::SliderClass(unsigned id, int x, int y, int w, int h, BOOL no_buttons) :
    GaugeClass(id, x, y, w, h),
    m_ButtonPlus(nullptr),
    m_ButtonMinus(nullptr),
    m_NoButtons(no_buttons)
{
    if (!no_buttons) {
        m_ButtonPlus = new ShapeButtonClass(id, GameFileClass::Retrieve("btn-plus.shp"), m_XPos + m_Width + 2, m_YPos);

        if (m_ButtonPlus) {
            m_ButtonPlus->Make_Peer(*this);
            m_ButtonPlus->Add(*this);
            m_ButtonPlus->Flag_To_Redraw();
        }

        m_ButtonMinus = new ShapeButtonClass(id, GameFileClass::Retrieve("btn-mins.shp"), m_XPos - 6, m_YPos);

        if (m_ButtonMinus) {
            m_ButtonMinus->Make_Peer(*this);
            m_ButtonMinus->Add(*this);
            m_ButtonMinus->Flag_To_Redraw();
        }
    }

    Set_Thumb_Size(1);
    Recalc_Thumb();

    m_FillGauge = false;
}

SliderClass::SliderClass(SliderClass &that) :
    GaugeClass(that),
    m_ButtonPlus(that.m_ButtonPlus),
    m_ButtonMinus(that.m_ButtonMinus),
    m_NoButtons(that.m_NoButtons),
    m_BumpSize(that.m_BumpSize),
    m_ThumbnailPixels(that.m_ThumbnailPixels),
    m_ThumbnailSize(that.m_ThumbnailSize)
{
}

SliderClass::~SliderClass()
{
    if (m_ButtonPlus != nullptr) {
        delete m_ButtonPlus;
        m_ButtonPlus = nullptr;
    }

    if (m_ButtonMinus != nullptr) {
        delete m_ButtonMinus;
        m_ButtonMinus = nullptr;
    }
}

void SliderClass::Peer_To_Peer(unsigned flags, KeyNumType &key, ControlClass &peer)
{
    if (flags & MOUSE_LEFT_RLSE) {
        if (&peer == m_ButtonPlus) {
            Step(false);
        }

        if (&peer == m_ButtonMinus) {
            Step(true);
        }
    }
}

BOOL SliderClass::Draw_Me(BOOL redraw)
{
    if (m_NoButtons && ControlClass::Draw_Me(redraw)) {
        if (&g_seenBuff == g_logicPage) {
            g_mouse->Conditional_Hide_Mouse(m_XPos, m_YPos, m_Width + m_XPos, m_Height + m_YPos);
        }

        Draw_Box(m_XPos, m_YPos, m_Width, m_Height, BOX_STYLE_0, true);
        Draw_Thumb();

        if (&g_seenBuff == g_logicPage) {
            g_mouse->Conditional_Show_Mouse();
        }

        return true;
    }

    return GaugeClass::Draw_Me(redraw);
}

BOOL SliderClass::Action(unsigned flags, KeyNumType &key)
{
    if (flags & MOUSE_LEFT_PRESS) {
        int mouse_pos;
        int gadget_pos;

        if (m_IsHorizontal) {
            mouse_pos = g_mouse->Get_Mouse_X();
            gadget_pos = m_XPos;
        } else {
            mouse_pos = g_mouse->Get_Mouse_Y();
            gadget_pos = m_YPos;
        }

        gadget_pos += m_ThumbnailSize + 1;

        if (mouse_pos >= gadget_pos) {
            if (mouse_pos <= m_ThumbnailPixels + gadget_pos) {
                GaugeClass::Action(flags, key);
                key = KN_NONE;

                return true;
            } else {
                Bump(false); // bump down
                GaugeClass::Action(0, key);
                key = KN_NONE;

                return true;
            }
        } else {
            Bump(true); // bump up
            GaugeClass::Action(0, key);
            key = KN_NONE;

            return true;
        }
    }

    return GaugeClass::Action(flags, key);
}

BOOL SliderClass::Set_Maximum(int max)
{
    if (GaugeClass::Set_Maximum(max)) {
        Recalc_Thumb();

        return true;
    }

    return false;
}

BOOL SliderClass::Set_Value(int value)
{
    if (GaugeClass::Set_Value(std::min(value, m_Maximum - m_BumpSize))) {
        Recalc_Thumb();

        return true;
    }

    return false;
}

void SliderClass::Draw_Thumb()
{
    if (m_IsHorizontal) {
        Draw_Box(m_ThumbnailSize + m_XPos, m_YPos, m_ThumbnailPixels, m_Height, BOX_STYLE_1, true);
    } else {
        Draw_Box(m_XPos, m_ThumbnailSize + m_YPos, m_Width, m_ThumbnailPixels, BOX_STYLE_1, true);
    }
}

void SliderClass::Set_Thumb_Size(int size)
{
    m_BumpSize = std::clamp(size, 1, m_Maximum);
    Recalc_Thumb();
}

BOOL SliderClass::Bump(BOOL bump_up)
{
    if (bump_up) {
        return Set_Value(m_Value - m_BumpSize);
    }

    return Set_Value(m_Value + m_BumpSize);
}

BOOL SliderClass::Step(BOOL step_up)
{
    if (step_up) {
        return Set_Value(m_Value - 1);
    }

    return Set_Value(m_Value + 1);
}

void SliderClass::Recalc_Thumb()
{
    int length = m_IsHorizontal ? m_Width : m_Height;
    m_ThumbnailPixels = std::max(4, length * fixed_t(m_BumpSize, m_Maximum));
    m_ThumbnailSize = std::min(length - m_ThumbnailPixels, length * fixed_t(m_Value, m_Maximum));
}
