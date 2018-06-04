/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling tabs.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */

#include "tab.h"
#include "audio.h"
#include "ccfileclass.h"
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

#ifndef RAPP_STANDALONE
void *&TabClass::TabShape = Make_Global<void *>(0x0068A4C0);
#else
void *TabClass::TabShape = nullptr;
#endif
void *TabClass::PassableShape = nullptr;
void *TabClass::TabBackgroundShape = nullptr;

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

        CreditToRedraw = true;
        CreditHasChanged = true;
    }
}

void TabClass::CreditClass::AI(BOOL a1)
{
#ifndef RAPP_STANDALONE
    void (*func)(const CreditClass *, BOOL) = reinterpret_cast<void (*)(const CreditClass *, BOOL)>(0x004ACF04);
    func(this, a1);
#else
    int v3; // eax@3
    CreditClass *v4; // ecx@3
    int v5; // eax@6
    char v6; // bh@14
    int v7; // eax@15
    int v8; // eax@18
    int v9; // eax@21
    int v10; // eax@21
    int v11; // edx@23
    char *v12; // edx@24

    if (a2 || Frame != AI::.0 ::_last) {
        AI::.0 ::_last = Frame;
        v3 = HouseClass::Available_Money(PlayerPtr);
        v4->Available = v3;
        if (v3 <= 0) {
            v3 = 0;
        }
        v4->Available = v3;
        if (Scen.Has_Started() != -1) {
            goto LABEL_28;
        }
        if (Scen.Get_Global_Time() != 0) {
        LABEL_28:
            v4->Boolean |= 1u;
            Flag_To_Redraw(0);
        }
        if (v4->Credits != v4->Available) {
            if (a2) {
                v6 = v4->Boolean & 0xFB;
                v4->Credits = v4->Available;
                v4->Boolean = v6;
            LABEL_25:
                v4->Boolean |= 1u;
                Flag_To_Redraw(0);
                return;
            }
            v7 = v4->field_C;
            if (v7) {
                v4->field_C = v7 - 1;
            }
            if (!v4->field_C) {
                v8 = v4->Available - v4->Credits;
                if (v8 <= 0) {
                    v4->field_C = 3;
                } else {
                    v4->field_C = 1;
                }
                v9 = ABS(v8);
                v10 = Bound(v9 >> 3, 1, 143);
                if (v4->Credits > v4->Available) {
                    v10 = -v10;
                }
                v11 = v10 + v4->Credits;
                v4->Credits = v11;
                if (v11 - v10 != v4->Credits) {
                    v4->Boolean |= 4u;
                    v12 = &v4->Boolean;
                    *v12 = v4->Boolean & 0xFD;
                    *v12 |= 2 * (v10 > 0);
                }
                goto LABEL_25;
            }
        }
    }
#endif
}

TabClass::TabClass() : CreditDisplay(), TimerFlashTimer(), TabToRedraw(false), CreditsFlashTimer() {}

void TabClass::One_Time()
{
    SidebarClass::One_Time();
    TabShape = MixFileClass<CCFileClass>::Retrieve("tabs.shp");
    PassableShape = MixFileClass<CCFileClass>::Retrieve("passable.shp");
    TabBackgroundShape = MixFileClass<CCFileClass>::Retrieve("tabback.shp");
}

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

void TabClass::Draw_It(BOOL force_redraw)
{
    SidebarClass::Draw_It(force_redraw);

    if ((force_redraw || TabToRedraw) && g_logicPage->Lock()) {
        // Draw the RA++ background if available, otherwise default to original behaviour.
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
        } else {
            g_logicPage->Fill_Rect(0, 0, g_seenBuff.Get_Width() - 1, 15, COLOR_BLACK);
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

void TabClass::Flash_Money()
{
    TabToRedraw = true;
    Flag_To_Redraw();
    CreditsFlashTimer = 7;
}

void TabClass::Draw_Passable_Tab(BOOL state)
{
    CC_Draw_Shape(PassableShape, state != 0, 160, 0, WINDOW_0, SHAPE_NORMAL);
    Fancy_Text_Print(
        TXT_EDITOR_PASSABLE, 240, 0, &MetalScheme, COLOR_TBLACK, TPF_12PT_METAL | TPF_CENTER | TPF_USE_GRAD_PAL);
}

void TabClass::Draw_Map_Size_Tab()
{
    CC_Draw_Shape(TabShape, 1, 320, 0, WINDOW_0, SHAPE_NORMAL);
    Fancy_Text_Print("%d x %d",
        400,
        0,
        &MetalScheme,
        COLOR_TBLACK,
        TPF_12PT_METAL | TPF_CENTER | TPF_USE_GRAD_PAL,
        Map.Get_Map_Cell_Width(),
        Map.Get_Map_Cell_Height());
}

void TabClass::Draw_Credits_Tab()
{
    int tabframe = 0;
    int width = g_logicPage->Get_Width();

    if (Options.Sidebar_Toggle_Allowed()) {
        if (Map.CreditsFlashTimer.Time() > 1) {
            tabframe = 4;
        } else {
            tabframe = 2;
        }

        CC_Draw_Shape(TabShape, tabframe, width - 2 * TAB_WIDTH, 0, WINDOW_0, SHAPE_NORMAL);
    } else {
        if (Map.CreditsFlashTimer.Time() > 1) {
            tabframe = 8;
        } else {
            tabframe = 6;
        }

        CC_Draw_Shape(TabShape, tabframe, width - TAB_WIDTH, 0, WINDOW_0, SHAPE_NORMAL);
    }

    if (Scen.Global_Timer_Running()) {
        if (Scen.Get_Global_Time() >= 900 * Rule.Get_Timer_Warning()) {
            tabframe = Map.TimerFlashTimer.Time() > 0 ? 4 : 2;
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

void TabClass::Hilite_Tab(int tab)
{
    int pos;
    int frame;

    switch (tab) {
        case 0:
            pos = 0;
            frame = 1;
            break;

        case 1:
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
