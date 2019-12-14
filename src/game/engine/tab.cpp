/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling tabs.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */

#include "tab.h"
#include "bench.h"
#include "dialog.h"
#include "drawshape.h"
#include "gamefile.h"
#include "gameoptions.h"
#include "gbuffer.h"
#include "globals.h"
#include "house.h"
#include "iomap.h"
#include "mixfile.h"
#include "queue.h"
#include "rules.h"
#include "scenario.h"
#include "vox.h"
#include <algorithm>
#include <cstdlib>

#ifndef GAME_DLL
void *TabClass::s_TabShape = nullptr;
#endif
void *TabClass::s_PassableShape = nullptr;
void *TabClass::s_TabBackgroundShape = nullptr;

/**
 * 0x004ACB90
 */
TabClass::CreditClass::CreditClass() :
    m_Available(0),
    m_Credits(0),
    m_CreditToRedraw(false),
    m_CreditHasIncreased(false),
    m_CreditHasChanged(false),
    m_TicksToNextRecalc(0)
{
    return;
}

/**
 * @brief Drawing logic for credit and timer tabs.
 *
 * 0x004ACBB8
 */
void TabClass::CreditClass::Graphic_Logic(BOOL force_redraw)
{
    if (force_redraw || m_CreditToRedraw) {

        BENCHMARK_START(BENCH_TABS);

        if (m_CreditHasChanged) {
            Sound_Effect(m_CreditHasIncreased ? VOC_MONEY_UP : VOC_MONEY_DOWN, fixed_t::_1_2);
        }

        TabClass::Draw_Credits_Tab();

        // Print credit amount.
        if (g_Options.Sidebar_Toggle_Allowed()) {
            Fancy_Text_Print("%d",
                g_SeenBuff.Get_Width() - (TAB_WIDTH / 2) - TAB_WIDTH,
                0,
                &g_MetalScheme,
                0,
                TPF_12PT_METAL | TPF_CENTER | TPF_USE_GRAD_PAL,
                m_Credits);
        } else {
            Fancy_Text_Print("%d",
                g_SeenBuff.Get_Width() - (TAB_WIDTH / 2),
                0,
                &g_MetalScheme,
                0,
                TPF_12PT_METAL | TPF_CENTER | TPF_USE_GRAD_PAL,
                m_Credits);
        }

        if (g_Scen.Global_Timer_Running()) {
            int frame_time = g_Scen.Get_Global_Time();
            int total_seconds = frame_time / 15; // Game ticks at 15 frames per logical second (not actual seconds unless at 15 fps).
            int hours = (total_seconds / 60) / 60;
            int minutes = (total_seconds / 60) % 60;
            int seconds = total_seconds % 60;

            VoxType voctospeak = VOX_NONE;

            // Speak timer warnings.
            if (frame_time == 900) { // 1 Minute.
                voctospeak = VOX_TIME_1;
            }

            if (frame_time == 1800) { // 2 Minutes.
                voctospeak = VOX_TIME_2;
            }

            if (frame_time == 2700) { // 3 Minutes.
                voctospeak = VOX_TIME_3;
            }

            if (frame_time == 3600) { // 4 Minutes.
                voctospeak = VOX_TIME_4;
            }

            if (frame_time == 4500) { // 5 Minutes.
                voctospeak = VOX_TIME_5;
            }

            if (frame_time == 9000) { // 10 Minutes.
                voctospeak = VOX_TIME_10;
            }
            
            if (frame_time == 18000) { // 20 Minutes.
                voctospeak = VOX_TIME_20;
            }
            
            if (frame_time == 27000) { // 30 Minutes.
                voctospeak = VOX_TIME_30;
            }
            
            if (frame_time == 36000) { // 40 Minutes.
                voctospeak = VOX_TIME_40;
            }

            if (voctospeak != VOX_NONE) {
                Speak(voctospeak);
                g_Map.m_TimerFlashTimer = 7;
            }

            // Print the time.
            if (g_Options.Sidebar_Toggle_Allowed()) {
                if (hours != 0) {
                    Fancy_Text_Print(TXT_TIME_HMS_FORMAT,
                        g_LogicPage->Get_Width() - (2 * TAB_WIDTH) - (TAB_WIDTH / 2),
                        0,
                        &g_MetalScheme,
                        0,
                        TPF_12PT_METAL | TPF_CENTER | TPF_USE_GRAD_PAL,
                        hours,
                        minutes,
                        seconds);
                } else {
                    Fancy_Text_Print(TXT_TIME_MS_FORMAT,
                        g_LogicPage->Get_Width() - (2 * TAB_WIDTH) - (TAB_WIDTH / 2),
                        0,
                        &g_MetalScheme,
                        0,
                        TPF_12PT_METAL | TPF_CENTER | TPF_USE_GRAD_PAL,
                        minutes,
                        seconds);
                }
            } else {
                if (hours != 0) {
                    Fancy_Text_Print(TXT_TIME_HMS_FORMAT,
                        g_LogicPage->Get_Width() - TAB_WIDTH - (TAB_WIDTH / 2),
                        0,
                        &g_MetalScheme,
                        0,
                        TPF_12PT_METAL | TPF_CENTER | TPF_USE_GRAD_PAL,
                        hours,
                        minutes,
                        seconds);
                } else {
                    Fancy_Text_Print(TXT_TIME_MS_FORMAT,
                        g_LogicPage->Get_Width() - TAB_WIDTH - (TAB_WIDTH / 2),
                        0,
                        &g_MetalScheme,
                        0,
                        TPF_12PT_METAL | TPF_CENTER | TPF_USE_GRAD_PAL,
                        minutes,
                        seconds);
                }
            }
        }

        m_CreditToRedraw = false;
        m_CreditHasChanged = false;

        BENCHMARK_END(BENCH_TABS);
    }
}

/**
 * @brief Tick logic for credit and timer tabs.
 *
 * 0x004ACF04
 */
void TabClass::CreditClass::AI(BOOL force_update)
{
    static int _last = 0;

    if (g_InMapEditor) {
        m_Available = std::max(0, g_Map.m_TotalValue);

        if (m_Credits != m_Available) {
            m_CreditHasChanged = false;
            m_Credits = m_Available;
            m_CreditToRedraw = true;
            g_Map.Flag_To_Redraw();
        }
    } else if (force_update || g_GameFrame != _last) {
        _last = g_GameFrame;
        m_Available = std::max(0, g_PlayerPtr->Available_Money());

        if (!g_Scen.Global_Timer().Expired()) {
            m_CreditToRedraw = true;
            g_Map.Flag_To_Redraw();
        }

        if (m_Credits != m_Available) {
            if (force_update) {
                m_CreditHasChanged = false;
                m_Credits = m_Available;
            } else {
                if (m_TicksToNextRecalc) {
                    --m_TicksToNextRecalc;
                }

                if (m_TicksToNextRecalc == 0) {
                    m_TicksToNextRecalc = m_Available - m_Credits <= 0 ? 3 : 1;
                    int unk = std::clamp((unsigned)std::abs(m_Available - m_Credits) / 8, 1u, 143u);

                    if (m_Credits > m_Available) {
                        unk = -unk;
                    }

                    m_Credits += unk;

                    if (m_Credits - unk != m_Credits) {
                        m_CreditHasChanged = true;
                        m_CreditHasIncreased = unk > 0;
                    }
                }
            }

            m_CreditToRedraw = true;
            g_Map.Flag_To_Redraw();
        }
    }
}

TabClass::TabClass() :
    m_CreditDisplay(),
    m_TimerFlashTimer(),
    m_TabToRedraw(false),
    m_CreditsFlashTimer()
{
}

/**
 * @brief One time load of tab related graphical assets.
 *
 * 0x005539D8
 */
void TabClass::One_Time()
{
    SidebarClass::One_Time();
    s_TabShape = GameFileClass::Retrieve("tabs.shp");
    s_PassableShape = GameFileClass::Retrieve("passable.shp");
    s_TabBackgroundShape = GameFileClass::Retrieve("tabback.shp");
}

/**
 * @brief Per tick logic for the tabs in the UI.
 *
 * 0x005538D0
 */
void TabClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
    if (mouse_y > 0 && mouse_y < 16 && mouse_x < g_SeenBuff.Get_Width() && mouse_x > 0) {
        // Handle a left click and set key to 0 to indicate handled to lower layers.
        if (key == KN_LMOUSE) {
            int tab_index = -1;

            // We are in the "g_Options" tab here.
            if (mouse_x < TAB_WIDTH) {
                tab_index = 0;
            }

            // If Sidebar toggle allowed and we are in the Sidebar tab here.
            if (g_Options.Sidebar_Toggle_Allowed() && mouse_x > g_SeenBuff.Get_Width() - TAB_WIDTH) {
                tab_index = 1;
            }

            // If we selected a tab mark it active and mark key as handled.
            if (tab_index >= 0) {
                TabClass::Set_Active(tab_index);
                key = KN_NONE;
            }
        }

        Override_Mouse_Shape(MOUSE_POINTER, 0);
    }

    if (m_CreditsFlashTimer == 1) {
        m_TabToRedraw = true;
        Flag_To_Redraw(0);
    }

    m_CreditDisplay.AI(0);
    SidebarClass::AI(key, mouse_x, mouse_y);
}

/**
 * @brief Draws the tabs.
 *
 * 0x005533A0
 */
void TabClass::Draw_It(BOOL force_redraw)
{
    SidebarClass::Draw_It(force_redraw);

    if ((force_redraw || m_TabToRedraw) && g_LogicPage->Lock()) {
        g_LogicPage->Fill_Rect(0, 0, g_SeenBuff.Get_Width() - 1, 15, COLOR_BLACK);

        // Draw the new Chronoshift tab background if available.
        if (s_TabBackgroundShape != nullptr) {
            int distance = g_LogicPage->Get_Width() / TAB_WIDTH;
            int framecount = Get_Build_Frame_Count(s_TabBackgroundShape);
            int xpos = 0;

            if (framecount == 1) {
                for (int i = 0; i <= distance; ++i) {
                    CC_Draw_Shape(s_TabBackgroundShape, 0, xpos, g_LogicPage->Get_YPos());
                    xpos += TAB_WIDTH;
                }
            } else if (framecount >= 1) {
                int frameindex = 0;

                for (int i = 0; i <= distance && frameindex <= framecount; ++i) {
                    CC_Draw_Shape(s_TabBackgroundShape, frameindex, xpos, g_LogicPage->Get_YPos());
                    xpos += TAB_WIDTH;
                    ++frameindex;
                }
            }
        }

        // Draw options tab.
        CC_Draw_Shape(s_TabShape, 0, 0, 0, WINDOW_0, SHAPE_NORMAL);
        Fancy_Text_Print(
            TXT_TAB_BUTTON_CONTROLS, 80, 0, &g_MetalScheme, COLOR_TBLACK, TPF_USE_GRAD_PAL | TPF_CENTER | TPF_12PT_METAL);

        // Credits and timer tab.
        TabClass::Draw_Credits_Tab();

        // Line underneath tab area to separate from tactical display.
        g_LogicPage->Draw_Line(0, 16 - 2, g_SeenBuff.Get_Width() - 1, 16 - 2, COLOR_BLACK);

        // For when sidebar is allowed to be hidden ala C&C95 or the DOS versions.
        if (g_Options.Sidebar_Toggle_Allowed()) {
            CC_Draw_Shape(s_TabShape, 6, g_SeenBuff.Get_Width() - TAB_WIDTH, 0, WINDOW_0, SHAPE_NORMAL);
            Fancy_Text_Print(TXT_TAB_SIDEBAR,
                g_SeenBuff.Get_Width() - 80,
                0,
                &g_MetalScheme,
                COLOR_TBLACK,
                TPF_USE_GRAD_PAL | TPF_CENTER | TPF_12PT_METAL);
        }

        g_LogicPage->Unlock();
    }

    m_CreditDisplay.Graphic_Logic(force_redraw || m_TabToRedraw);

    m_TabToRedraw = false;
}

/**
 * @brief Sets the countdown for the credit tab flash.
 *
 * 0x00553A4C
 */
void TabClass::Flash_Money()
{
    m_TabToRedraw = true;
    Flag_To_Redraw();
    m_CreditsFlashTimer = 7;
}

/**
 * @brief Draws the credit and timer tabs.
 *
 * 0x005533A0
 */
void TabClass::Draw_Credits_Tab()
{
    int tabframe = 0;
    int width = g_LogicPage->Get_Width();

    if (g_Options.Sidebar_Toggle_Allowed()) {
        if (g_Map.m_CreditsFlashTimer > 1) {
            tabframe = 4;
        } else {
            tabframe = 2;
        }

        CC_Draw_Shape(s_TabShape, tabframe, width - 2 * TAB_WIDTH, 0, WINDOW_0, SHAPE_NORMAL);
    } else {
        if (g_Map.m_CreditsFlashTimer > 1) {
            tabframe = 8;
        } else {
            tabframe = 6;
        }

        CC_Draw_Shape(s_TabShape, tabframe, width - TAB_WIDTH, 0, WINDOW_0, SHAPE_NORMAL);
    }

    if (g_Scen.Global_Timer_Running()) {
        if (g_Scen.Get_Global_Time() >= 900 * g_Rule.Get_Timer_Warning()) {
            tabframe = g_Map.m_TimerFlashTimer > 0 ? 4 : 2;
        } else {
            tabframe = 4;
        }

        if (g_Options.Sidebar_Toggle_Allowed()) {
            CC_Draw_Shape(s_TabShape, tabframe, width - 3 * TAB_WIDTH, 0, WINDOW_0, SHAPE_NORMAL);
        } else {
            CC_Draw_Shape(s_TabShape, tabframe, width - 2 * TAB_WIDTH, 0, WINDOW_0, SHAPE_NORMAL);
        }
    }
}

/**
 * @brief Draw tab as though pressed, g_Options or Sidebar tab only.
 *
 * 0x00553868
 */
void TabClass::Hilite_Tab(int tab)
{
    int pos = 0;
    int frame = 0;

    switch (tab) {
        case TAB_OPTIONS:
            pos = 0;
            frame = 1;
            break;

        case TAB_SIDEBAR:
            pos = g_SeenBuff.Get_Width() - TAB_WIDTH;
            frame = 7;
            break;

        default:
            break;
    }

    CC_Draw_Shape(s_TabShape, frame, pos, 0, WINDOW_0, SHAPE_NORMAL);
    g_MetalScheme.MediumColor = 134;
    Fancy_Text_Print(
        TXT_TAB_BUTTON_CONTROLS, 80, 0, &g_MetalScheme, COLOR_TBLACK, TPF_USE_GRAD_PAL | TPF_CENTER | TPF_12PT_METAL);
    g_MetalScheme.MediumColor = 128;
}

/**
 * @brief Activates logic behind either g_Options or Sidebar tab.
 *
 * 0x005539AC
 */
void TabClass::Set_Active(int tab)
{
    switch (tab) {
        case TAB_OPTIONS:
            Queue_Options();
            break;

        case TAB_SIDEBAR:
            Activate(SIDEBAR_TOGGLE); // Toggles the sidebar.
            break;

        default:
            break;
    }
}
