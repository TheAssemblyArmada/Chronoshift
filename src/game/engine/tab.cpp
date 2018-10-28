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
#include "audio.h"
#include "gamefile.h"
#include "dialog.h"
#include "drawshape.h"
#include "gameoptions.h"
#include "gbuffer.h"
#include "globals.h"
#include "iomap.h"
#include "mixfile.h"
#include "queue.h"
#include "rules.h"
#include "scenario.h"

#ifndef CHRONOSHIFT_STANDALONE
void *&TabClass::TabShape = Make_Global<void *>(0x0068A4C0);
#else
void *TabClass::TabShape = nullptr;
#endif
void *TabClass::PassableShape = nullptr;
void *TabClass::TabBackgroundShape = nullptr;

/**
 * 0x004ACB90
 */
TabClass::CreditClass::CreditClass() :
    Available(0),
    Credits(0),
    CreditToRedraw(false),
    CreditHasIncreased(false),
    CreditHasChanged(false),
    field_C(0)
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
    if (force_redraw || CreditToRedraw) {
        if (CreditHasChanged) {
            Sound_Effect(CreditHasIncreased ? VOC_MONEY_UP : VOC_MONEY_DOWN, fixed::_1_2, 1, 0, HOUSES_NONE);
        }

        TabClass::Draw_Credits_Tab();

        // Print credit amount.
        if (Options.Sidebar_Toggle_Allowed()) {
            Fancy_Text_Print("%d",
                g_seenBuff.Get_Width() - (TAB_WIDTH / 2) - TAB_WIDTH,
                0,
                &MetalScheme,
                0,
                TPF_12PT_METAL | TPF_CENTER | TPF_USE_GRAD_PAL,
                Credits);
        } else {
            Fancy_Text_Print("%d",
                g_seenBuff.Get_Width() - (TAB_WIDTH / 2),
                0,
                &MetalScheme,
                0,
                TPF_12PT_METAL | TPF_CENTER | TPF_USE_GRAD_PAL,
                Credits);
        }

        if (Scen.Global_Timer_Running()) {
            int frame_time = Scen.Get_Global_Time();
            int total_seconds = frame_time / 15; // Game ticks at 15 frames per logical second (not actual seconds unless at 15 fps).
            VoxType voctospeak = VOX_NONE;
            int hours = (total_seconds / 60) / 60;
            int minutes = (total_seconds / 60) % 60;
            int seconds = total_seconds % 60;

            // Speak timer warnings.
            if (frame_time == 900) {
                voctospeak = VOX_TIME_1;
            }

            if (frame_time == 1800) {
                voctospeak = VOX_TIME_2;
            }

            if (frame_time == 2700) {
                voctospeak = VOX_TIME_3;
            }

            if (frame_time == 3600) {
                voctospeak = VOX_TIME_4;
            }

            if (frame_time == 4500) {
                voctospeak = VOX_TIME_5;
            }

            if (frame_time == 9000) {
                voctospeak = VOX_TIME_10;
            }
            
            if (frame_time == 18000) {
                voctospeak = VOX_TIME_20;
            }
            
            if (frame_time == 27000) {
                voctospeak = VOX_TIME_30;
            }
            
            if (frame_time == 36000) {
                voctospeak = VOX_TIME_40;
            }

            if (voctospeak != VOX_NONE) {
                Speak(voctospeak);
                Map.TimerFlashTimer = 7;
            }

            // Print the time.
            if (Options.Sidebar_Toggle_Allowed()) {
                if (hours != 0) {
                    Fancy_Text_Print(TXT_TIME_HMS_FORMAT,
                        g_logicPage->Get_Width() - (2 * TAB_WIDTH) - (TAB_WIDTH / 2),
                        0,
                        &MetalScheme,
                        0,
                        TPF_12PT_METAL | TPF_CENTER | TPF_USE_GRAD_PAL,
                        hours,
                        minutes,
                        seconds);
                } else {
                    Fancy_Text_Print(TXT_TIME_MS_FORMAT,
                        g_logicPage->Get_Width() - (2 * TAB_WIDTH) - (TAB_WIDTH / 2),
                        0,
                        &MetalScheme,
                        0,
                        TPF_12PT_METAL | TPF_CENTER | TPF_USE_GRAD_PAL,
                        minutes,
                        seconds);
                }
            } else {
                if (hours != 0) {
                    Fancy_Text_Print(TXT_TIME_HMS_FORMAT,
                        g_logicPage->Get_Width() - TAB_WIDTH - (TAB_WIDTH / 2),
                        0,
                        &MetalScheme,
                        0,
                        TPF_12PT_METAL | TPF_CENTER | TPF_USE_GRAD_PAL,
                        hours,
                        minutes,
                        seconds);
                } else {
                    Fancy_Text_Print(TXT_TIME_MS_FORMAT,
                        g_logicPage->Get_Width() - TAB_WIDTH - (TAB_WIDTH / 2),
                        0,
                        &MetalScheme,
                        0,
                        TPF_12PT_METAL | TPF_CENTER | TPF_USE_GRAD_PAL,
                        minutes,
                        seconds);
                }
            }
        }

        CreditToRedraw = false;
        CreditHasChanged = false;
    }
}

/**
 * @brief Tick logic for credit and timer tabs.
 *
 * 0x004ACF04
 */
void TabClass::CreditClass::AI(BOOL a1)
{
    // TODO Needs HouseClass.
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(const CreditClass *, BOOL) = reinterpret_cast<void (*)(const CreditClass *, BOOL)>(0x004ACF04);
    func(this, a1);
#endif
}

TabClass::TabClass() : CreditDisplay(), TimerFlashTimer(), TabToRedraw(false), CreditsFlashTimer() {}

/**
 * @brief One time load of tab related graphical assets.
 *
 * 0x005539D8
 */
void TabClass::One_Time()
{
    SidebarClass::One_Time();
    TabShape = GameFileClass::Retrieve_File("tabs.shp");
    PassableShape = GameFileClass::Retrieve_File("passable.shp");
    TabBackgroundShape = GameFileClass::Retrieve_File("tabback.shp");
}

/**
 * @brief Per tick logic for the tabs in the UI.
 *
 * 0x005538D0
 */
void TabClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
    if (mouse_y > 0 && mouse_y < 16 && mouse_x < g_seenBuff.Get_Width() && mouse_x > 0) {
        // Handle a left click and set key to 0 to indicate handled to lower layers.
        if (key == KN_LMOUSE) {
            int tab_index = -1;

            // We are in the "Options" tab here.
            if (mouse_x < TAB_WIDTH) {
                tab_index = 0;
            }

            // If Sidebar toggle allowed and we are in the Sidebar tab here.
            if (Options.Sidebar_Toggle_Allowed() && mouse_x > g_seenBuff.Get_Width() - TAB_WIDTH) {
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

    if (CreditsFlashTimer == 1) {
        TabToRedraw = true;
        Flag_To_Redraw(0);
    }

    CreditDisplay.AI(0);
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

    if ((force_redraw || TabToRedraw) && g_logicPage->Lock()) {
        g_logicPage->Fill_Rect(0, 0, g_seenBuff.Get_Width() - 1, 15, COLOR_BLACK);

        // Draw the RA++ background if available.
        if (TabBackgroundShape != nullptr) {
            int distance = g_logicPage->Get_Width() / TAB_WIDTH;
            int framecount = Get_Build_Frame_Count(TabBackgroundShape);
            int xpos = 0;

            if (framecount == 1) {
                for (int i = 0; i <= distance; ++i) {
                    CC_Draw_Shape(TabBackgroundShape, 0, xpos, g_logicPage->Get_YPos());
                    xpos += TAB_WIDTH;
                }
            } else if (framecount >= 1) {
                int frameindex = 0;

                for (int i = 0; i <= distance && frameindex <= framecount; ++i) {
                    CC_Draw_Shape(TabBackgroundShape, frameindex, xpos, g_logicPage->Get_YPos());
                    xpos += TAB_WIDTH;
                    ++frameindex;
                }
            }
        }

        // Draw options tab.
        CC_Draw_Shape(TabShape, 0, 0, 0, WINDOW_0, SHAPE_NORMAL);
        Fancy_Text_Print(
            TXT_TAB_BUTTON_CONTROLS, 80, 0, &MetalScheme, COLOR_TBLACK, TPF_USE_GRAD_PAL | TPF_CENTER | TPF_12PT_METAL);

        // Credits and timer tab.
        TabClass::Draw_Credits_Tab();

        // Line underneath tab area to separate from tactical display.
        g_logicPage->Draw_Line(0, 16 - 2, g_seenBuff.Get_Width() - 1, 16 - 2, COLOR_BLACK);

        // For when sidebar is allowed to be hidden ala C&C95 or the DOS versions.
        if (Options.Sidebar_Toggle_Allowed()) {
            CC_Draw_Shape(TabShape, 6, g_seenBuff.Get_Width() - TAB_WIDTH, 0, WINDOW_0, SHAPE_NORMAL);
            Fancy_Text_Print(TXT_TAB_SIDEBAR,
                g_seenBuff.Get_Width() - 80,
                0,
                &MetalScheme,
                COLOR_TBLACK,
                TPF_USE_GRAD_PAL | TPF_CENTER | TPF_12PT_METAL);
        }

        g_logicPage->Unlock();
    }

    CreditDisplay.Graphic_Logic(force_redraw || TabToRedraw);

    TabToRedraw = false;
}

/**
 * @brief Sets the countdown for the credit tab flash.
 *
 * 0x00553A4C
 */
void TabClass::Flash_Money()
{
    TabToRedraw = true;
    Flag_To_Redraw();
    CreditsFlashTimer = 7;
}

/**
 * @brief Draws the credit and timer tabs.
 *
 * 0x005533A0
 */
void TabClass::Draw_Credits_Tab()
{
    int tabframe = 0;
    int width = g_logicPage->Get_Width();

    if (Options.Sidebar_Toggle_Allowed()) {
        if (Map.CreditsFlashTimer > 1) {
            tabframe = 4;
        } else {
            tabframe = 2;
        }

        CC_Draw_Shape(TabShape, tabframe, width - 2 * TAB_WIDTH, 0, WINDOW_0, SHAPE_NORMAL);
    } else {
        if (Map.CreditsFlashTimer > 1) {
            tabframe = 8;
        } else {
            tabframe = 6;
        }

        CC_Draw_Shape(TabShape, tabframe, width - TAB_WIDTH, 0, WINDOW_0, SHAPE_NORMAL);
    }

    if (Scen.Global_Timer_Running()) {
        if (Scen.Get_Global_Time() >= 900 * Rule.Get_Timer_Warning()) {
            tabframe = Map.TimerFlashTimer > 0 ? 4 : 2;
        } else {
            tabframe = 4;
        }

        if (Options.Sidebar_Toggle_Allowed()) {
            CC_Draw_Shape(TabShape, tabframe, width - 3 * TAB_WIDTH, 0, WINDOW_0, SHAPE_NORMAL);
        } else {
            CC_Draw_Shape(TabShape, tabframe, width - 2 * TAB_WIDTH, 0, WINDOW_0, SHAPE_NORMAL);
        }
    }
}

/**
 * @brief Draw tab as though pressed, Options or Sidebar tab only.
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
            pos = g_seenBuff.Get_Width() - TAB_WIDTH;
            frame = 7;
            break;

        default:
            break;
    }

    CC_Draw_Shape(TabShape, frame, pos, 0, WINDOW_0, SHAPE_NORMAL);
    MetalScheme.MediumColor = 134;
    Fancy_Text_Print(
        TXT_TAB_BUTTON_CONTROLS, 80, 0, &MetalScheme, COLOR_TBLACK, TPF_USE_GRAD_PAL | TPF_CENTER | TPF_12PT_METAL);
    MetalScheme.MediumColor = 128;
}

/**
 * @brief Activates logic behind either Options or Sidebar tab.
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
