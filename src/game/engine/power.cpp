/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling power bar.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "power.h"
#include "gamefile.h"
#include "house.h"
#include "coord.h"
#include "drawshape.h"
#include "iomap.h"
#include "lists.h"
#include "mixfile.h"
#include <algorithm>

#ifndef GAME_DLL
PowerClass::PowerButtonClass PowerClass::s_PowerButton;
void *PowerClass::s_PowerShape = nullptr;
void *PowerClass::s_PowerBarShape = nullptr;
#endif

PowerClass::PowerButtonClass::PowerButtonClass() :
    GadgetClass(0, 0, 0, 0, MOUSE_LEFT_PRESS | MOUSE_LEFT_HELD | MOUSE_LEFT_RLSE | MOUSE_LEFT_UP | MOUSE_RIGHT_PRESS, true)
{
}

/**
 *
 *
 */
BOOL PowerClass::PowerButtonClass::Action(unsigned flags, KeyNumType &key)
{
    if (!g_Map.Is_Sidebar_Shown()) {
        return false;
    }

    g_Map.Override_Mouse_Shape(MOUSE_POINTER);

    int str_id = (g_PlayerPtr->Power_Fraction() < fixed_t(1, 1) || g_PlayerPtr->Get_Power() > 0) ? TXT_POWER_OUTPUT : TXT_POWER_OUTPUT_LOW;
    g_Map.Help_Text(str_id, -1, -1, GadgetClass::Get_Color_Scheme()->MediumColor);

    GadgetClass::Action(flags, key);
    return true;
}

PowerClass::PowerClass() :
    RadarClass(),
    m_PowerToRedraw(false),
    m_PowerBit2(false),
    m_FlashTimer(),
    m_Drain(-1),
    m_Output(-1),
    m_NewDrainHeight(0),
    m_NewOutputHeight(0),
    m_DrainHeight(0),
    m_OutputHeight(0),
    m_DrainMod(0),
    m_OutputMod(0),
    m_OutputInc(0),
    m_DrainInc(0)
{
    return;
}

void PowerClass::One_Time()
{
    RadarClass::One_Time();
    s_PowerShape = GameFileClass::Retrieve("power.shp");
    captainslog_assert(s_PowerShape != nullptr);

    s_PowerBarShape = GameFileClass::Retrieve("powerbar.shp");
    captainslog_assert(s_PowerBarShape != nullptr);

    s_PowerButton.Set_Position(480, 180);
    s_PowerButton.Set_Size(15, 220);
}

void PowerClass::Init_Clear()
{
    RadarClass::Init_Clear();

    m_Drain = -1;
    m_Output = -1;
    m_NewDrainHeight = 0;
    m_NewOutputHeight = 0;
    m_DrainHeight = 0;
    m_OutputHeight = 0;
    m_OutputMod = 0;
    m_DrainMod = 0;
    m_DrainInc = 0;

    m_FlashTimer.Reset();
}

void PowerClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
    //Needs HouseClass
#ifdef GAME_DLL
    void (*func)(const PowerClass *, KeyNumType &, int, int) =
        reinterpret_cast<void (*)(const PowerClass *, KeyNumType &, int, int)>(0x00527C60);
    func(this, key, mouse_x, mouse_y);
#endif
    /*
    if (g_Map.Is_Sidebar_Drawn()) {
        int prev_drain = m_DrainHeight;
        int prev_output = m_OutputHeight;

        // Work out if output has changed and needs updating.
        if (PlayerPtr->Power != m_Output) {
            m_NewOutputHeight = Power_Height(PlayerPtr->Power);
            m_Output = PlayerPtr->Power;

            if (m_NewOutputHeight == m_OutputHeight) {
                m_OutputMod = 0;
            } else {
                m_OutputMod = 12;
                m_OutputInc = m_NewOutputHeight < m_OutputHeight ? -1 : 1;
            }
        }

        // Work out if drain has changed and needs updating.
        if (PlayerPtr->m_Drain != m_Drain) {
            m_NewDrainHeight = Power_Height(PlayerPtr->m_Drain);
            m_Drain = PlayerPtr->m_Drain;

            if (m_NewDrainHeight == m_DrainHeight) {
                m_DrainMod = 0;
            } else {
                m_DrainMod = 12;
                m_DrainInc = m_NewDrainHeight < m_DrainHeight ? -1 : 1;
            }
        }

        // Wobble when settling at new drain level after gaining/loosing some power draining structure.
        if (m_DrainMod > 0 && m_DrainHeight == m_NewDrainHeight) {
            m_PowerToRedraw = true;
            Flag_To_Redraw();
            --m_DrainMod;
        } else if (m_DrainHeight != m_NewDrainHeight) {
            m_DrainHeight += m_DrainInc;
        }

        // Wobble when settling at new output level after gaining/loosing some power generating structure.
        if (m_OutputMod > 0 && m_OutputHeight == m_NewOutputHeight) {
            m_PowerToRedraw = true;
            Flag_To_Redraw();
            --m_OutputMod;
        } else if (m_OutputHeight != m_NewOutputHeight) {
            m_OutputHeight += m_OutputInc;
        }

        if (prev_drain != m_DrainHeight || prev_output != m_OutputHeight) {
            m_PowerToRedraw = true;
            Flag_To_Redraw();
        }

        if (!m_FlashTimer.Has_Expired()) {
            m_PowerToRedraw = true;
            Flag_To_Redraw();
        }
    }

    RadarClass::AI(key, mouse_x, mouse_y);
    */
}

void PowerClass::Draw_It(BOOL force_redraw)
{
    //Needs HouseClass
#ifdef GAME_DLL
    void (*func)(const PowerClass *, BOOL) = reinterpret_cast<void (*)(const PowerClass *, BOOL)>(0x0052762C);
    func(this, force_redraw);
#endif
    /*
    static int _modtable[] = { 0, -1, 0, 1, 0, -1, -2, -1, 0, 1, 2, 1, 0 };

    if ((m_PowerToRedraw || force_redraw) && g_Map.Is_Sidebar_Drawn()) {
        m_PowerToRedraw = false;

        ShapeFlags flags = SHAPE_NORMAL;

        void *fadingtable = nullptr;

        int output_height = 0;
        int drain_height = 0;

        // If timer is set, flash the bar red?
        if (!m_FlashTimer.Has_Expired() && m_FlashTimer.Time() > 1) {
            // If remaining time mod 3 is odd?
            if ((m_FlashTimer.Time() % 3) & 1) {
                flags |= SHAPE_FADING;
                fadingtable = DisplayClass::s_FadingRed;
            }
        }

        // Draw the power bar background shapes
        CC_Draw_Shape(PowerBarShape, 0, PowerButton.Get_XPos(), 176, WINDOW_0, flags | SHAPE_WIN_REL, fadingtable);
        CC_Draw_Shape(PowerBarShape, 1, PowerButton.Get_XPos(), 288, WINDOW_0, flags | SHAPE_WIN_REL, fadingtable);

        if (m_OutputHeight == m_NewOutputHeight) {
            output_height = m_OutputHeight + _modtable[m_OutputMod] * m_OutputInc;
        } else {
            output_height = m_OutputHeight;
        }

        if (m_DrainHeight == m_NewDrainHeight) {
            drain_height = m_DrainHeight + _modtable[m_DrainMod] * m_DrainInc;
        } else {
            drain_height = m_DrainHeight;
        }

        output_height = Clamp(output_height, 0, POWER_MAX_HEIGHT);
        drain_height = Clamp(drain_height, 0, POWER_MAX_HEIGHT);

        // If we actually have some power
        if (output_height > 0) {
            uint8_t lcolor = COLOR_LTGREEN;
            uint8_t dcolor = COLOR_GREEN;

            // Slightly low power
            if (PlayerPtr->Power <= PlayerPtr->Drain) {
                lcolor = 211; // lightish orange
                dcolor = 214; // darkish orange
            }

            // Very low power
            if ((PlayerPtr->Power * 2) < PlayerPtr->Drain) {
                lcolor = 230; // lightish red
                dcolor = 235; // darkish red
            }

            output_height = (output_height / 107) * 153;
            drain_height = (drain_height / 107) * 153;

            // Draw the power bar, two similar color lines next to each other
            g_LogicPage->Fill_Rect(PowerButton.Get_XPos() + 10, output_height - 351, PowerButton.Get_XPos() + 11, 351, lcolor);
            g_LogicPage->Fill_Rect(PowerButton.Get_XPos() + 12, output_height - 351, PowerButton.Get_XPos() + 13, 351, dcolor);
        }

        // Draw the power level indicator.
        CC_Draw_Shape(
            PowerShape, 0, PowerButton.Get_XPos() + 2, (drain_height + 4) - 351, WINDOW_0, flags | SHAPE_WIN_REL);
        m_PowerToRedraw = false;
    }

    RadarClass::Draw_It(force_redraw);
    */
}

void PowerClass::Refresh_Cells(cell_t cellnum, const int16_t *list)
{
    captainslog_assert(cellnum < MAP_MAX_AREA);

    if (list != nullptr) {
        if (*list == LIST_START) {
            m_PowerToRedraw = true;
            Flag_To_Redraw();
        }
    }

    RadarClass::Refresh_Cells(cellnum, list);
}

int PowerClass::Power_Height(int power)
{
    int power_left = power;
    int height_inc = 0;
    int height = 0;

    for (int i = 0; i <= power / 100; ++i) {
        height_inc = (POWER_MAX_HEIGHT - height) / 5;
        power_left -= 100;
        height += height_inc;
    }

    if (power_left != 0) {
        height += power_left * height_inc / 100;
    }

    return std::clamp(height, 0, int(POWER_MAX_HEIGHT));
}

void PowerClass::Flash_Power()
{
    m_FlashTimer.Reset(15);
    m_PowerToRedraw = true;
    Flag_To_Redraw();
}
