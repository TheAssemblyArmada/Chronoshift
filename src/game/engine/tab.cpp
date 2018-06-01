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

#include "ccfileclass.h"
#include "drawshape.h"
#include "gameoptions.h"
#include "gbuffer.h"
#include "globals.h"
#include "iomap.h"
#include "mixfile.h"
#include "rules.h"
#include "scenario.h"
#include "tab.h"

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

void TabClass::CreditClass::Graphic_Logic(BOOL a1)
{
#ifndef RAPP_STANDALONE
    void (*func)(const CreditClass *, BOOL) = reinterpret_cast<void (*)(const CreditClass *, BOOL)>(0x004ACBB8);
    func(this, a1);
#else
    _WORD *v5; // edx@5
    VocType se_voc; // eax@5
    unsigned int v7; // eax@9
    signed int v8; // eax@13
    signed int v9; // ebx@13
    signed int v10; // ecx@13
    unsigned __int64 v11; // rax@13
    signed __int64 v12; // rtt@13
    int v13; // ebx@13
    int v15; // ecx@13
    char se_houses; // [sp-4h] [bp-30h]@5
    _BYTE v26[9]; // [sp+0h] [bp-2Ch]@0
    unsigned int v27; // [sp+Ch] [bp-20h]@13
    __int16 v29; // [sp+14h] [bp-18h]@6
    __int16 resulta; // [sp+18h] [bp-14h]@5

    if (a1 || CreditToRedraw) {
        if (CreditHasChanged) {
            if (CreditHasIncreased) {
                se_houses = -1;
                v5 = fixed::fixed(&resulta, 1, 2);
                se_voc = VOC_MONEY_UP;
            } else {
                se_houses = -1;
                v5 = fixed::fixed(&v29, 1, 2);
                se_voc = VOC_MONEY_DOWN;
            }
            Sound_Effect(se_voc, v5, 1, 0, se_houses);
        }
        TabClass::Draw_Credits_Tab();
        Fancy_Text_Print("%ld", g_seenBuff.Get_Width() - 80, 0, &MetalScheme, 0, 8, Credits);
        if (Scen_GlobalScenarioTimer.Started != -1) {
            v7 = Scen_GlobalScenarioTimer.Accumulated;
            if (Scen_GlobalScenarioTimer.Started != -1) {
                if (Frame - Scen_GlobalScenarioTimer.Started >= Scen_GlobalScenarioTimer.Accumulated) {
                    v7 = 0;
                } else {
                    v7 = Scen_GlobalScenarioTimer.Accumulated - (Frame - Scen_GlobalScenarioTimer.Started);
                }
            }
            v8 = v7 / 0xF;
            v9 = v8;
            v8 /= 60;
            v10 = v8;
            v11 = __PAIR__(v9, v8 / 60);
            v27 = v11;
            LODWORD(v12) = v9;
            HIDWORD(v12) = SHIDWORD(v11) >> 31;
            v13 = v12 % 60;
            VoxType voctospeak = VOX_NONE;
            v15 = v10 % 60;
            if (Scen.Get_Global_Time() == 900) {
                voctospeak = VOX_TIME_1;
            }
            if (Scen.Get_Global_Time() == 1800) {
                voctospeak = VOX_TIME_2;
            }
            if (Scen.Get_Global_Time() == 2700) {
                voctospeak = VOX_TIME_3;
            }
            if (Scen.Get_Global_Time() == 3600) {
                voctospeak = VOX_TIME_4;
            }
            if (Scen.Get_Global_Time() == 4500) {
                voctospeak = VOX_TIME_5;
            }
            if (Scen.Get_Global_Time() == 9000) {
                voctospeak = VOX_TIME_10;
            }
            if (Scen.Get_Global_Time() == 18000) {
                voctospeak = VOX_TIME_20;
            }
            if (Scen.Get_Global_Time() == 27000) {
                voctospeak = VOX_TIME_30;
            }
            if (Scen.Get_Global_Time() == 36000) {
                vvoctospeak14 = VOX_TIME_40;
            }
            if (voctospeak != VOX_NONE) {
                Speak(voctospeak);
                Map.TimerFlashTimer = 7;
            }
            if (v27) {
                Fancy_Text_Print(2, 400, 0, &MetalScheme, 0, 0x4108, v27, v15, v13);
            } else {
                Fancy_Text_Print(3, 400, 0, &MetalScheme, 0, 0x4108, v15, v13);
            }
        }
        CreditToRedraw = true;
        CreditHasChanged = true;
    }
#endif
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
#ifndef RAPP_STANDALONE
    void (*func)(const TabClass *, KeyNumType &, int, int) =
        reinterpret_cast<void (*)(const TabClass *, KeyNumType &, int, int)>(0x005538D0);
    func(this, key, mouse_x, mouse_y);
#else
    signed int v5; // eax@5
    int v6; // edx@9
    unsigned int v7; // eax@14
    unsigned int v8; // edx@15
    unsigned int v9; // edx@17

    if (mouse_y >= 0 && mouse_y < 16 && g_seenBuff.Get_Width() - 1 > mouse_x && mouse_x > 0) {
        v5 = 0;
        if (mouse_y > 0) {
            v5 = 1;
        }
        if (v5) {
            if (*key == 1) {
                v6 = -1;
                if (mouse_x < 160) {
                    v6 = 0;
                }
                if (v6 >= 0) {
                    TabClass::Set_Active(v6);
                    *key = 0;
                }
            }
            Override_Mouse_Shape(0, 0);
        }
    }
    v7 = this->CreditsFlashTimer.Accumulated;
    if (this->CreditsFlashTimer.Started != -1) {
        v8 = Frame - this->CreditsFlashTimer.Started;
        if (v8 >= v7) {
            v9 = 0;
            goto LABEL_19;
        }
        v7 -= v8;
    }
    v9 = v7;
LABEL_19:
    if (v9 == 1) {
        TabToRedraw = true;
        Flag_To_Redraw(0);
    }
    CreditDisplay.AI(0);
    SidebarClass::AI(key, mouse_x, mouse_y);
#endif
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
        TabClass::Draw_Credits_Tab();
        g_logicPage->Draw_Line(0, 16 - 2, g_seenBuff.Get_Width() - 1, 16 - 2, COLOR_BLACK);

        // For when sidebar is allowed to be hidden ala C&C95 or the DOS versions.
        if (Options.Sidebar_Toggle_Allowed()) {
            CC_Draw_Shape(TabShape, 0, g_seenBuff.Get_Width() - TAB_WIDTH, 0, WINDOW_0, SHAPE_NORMAL);
            Fancy_Text_Print(TXT_TAB_SIDEBAR,
                g_seenBuff.Get_Width() - 80,
                0,
                &ColorRemaps[REMAP_5],
                0,
                TPF_USE_BRIGHT | TPF_CENTER | TPF_NOSHADOW | TPF_12PT_METAL);
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
            tabframe = 2;
        } else {
            tabframe = 4;
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

    switch (tab)
    {
        case 0:
            pos = 0;
            break;
        default:
            pos = g_seenBuff.Get_Width() - TAB_WIDTH;
            break;
    }

    CC_Draw_Shape(TabShape, 1, pos, 0, WINDOW_0, SHAPE_NORMAL);
    MetalScheme.MediumColor = 134;
    Fancy_Text_Print(
        TXT_TAB_BUTTON_CONTROLS, 80, 0, &MetalScheme, COLOR_TBLACK, TPF_USE_GRAD_PAL | TPF_CENTER | TPF_12PT_METAL);
    MetalScheme.MediumColor = 128;
}

void TabClass::Set_Active(int tab)
{
    // It seems it might be feed minus values too....
    DEBUG_LOG("TabClass::Set_Active got %d\n", tab);
    // rewrote into a switch, as its cleaner. see binary
    switch (tab) {
        case TAB_OPTIONS:
            // Queue_Options();
            break;

        case TAB_SIDEBAR:
            // Map.Activate(); // Toggles the sidebar.
            break;

        default:
            break;
    }
}