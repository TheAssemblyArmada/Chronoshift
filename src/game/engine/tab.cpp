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
#include "ccfileclass.h"
#include "mixfile.h"
#include "drawshape.h"

#ifndef RAPP_STANDALONE
void *&TabClass::TabShape = Make_Global<void *>(0x0068A4C0);
#else
void *TabClass::TabShape = nullptr;
#endif

TabClass::CreditClass::CreditClass(void) :
    Available(0),
    Credits(0),
    CreditToRedraw(false),
    CreditHasIncreased(false),
    CreditHasChanged(false),
    field_C(0)
{
    return;
}

TabClass::CreditClass::~CreditClass(void)
{
    return;
}

void TabClass::CreditClass::Graphic_Logic(bool a1)
{
#ifndef RAPP_STANDALONE
    void (*func)(const CreditClass *, bool) = reinterpret_cast<void (*)(const CreditClass *, bool)>(0x004ACBB8);
    func(this, a1);
#endif
}

void TabClass::CreditClass::AI(bool a1)
{
#ifndef RAPP_STANDALONE
    void (*func)(const CreditClass *, bool) = reinterpret_cast<void (*)(const CreditClass *, bool)>(0x004ACF04);
    func(this, a1);
#endif
}

TabClass::TabClass():
    CreditDisplay(),
    TimerFlashTimer(),
    TabToRedraw(false),
    CreditsFlashTimer()
{}

void TabClass::One_Time()
{
    SidebarClass::One_Time();
    TabShape = MixFileClass<CCFileClass>::Retrieve("tabs.shp");
}

void TabClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
#ifndef RAPP_STANDALONE
    void (*func)(const TabClass *, KeyNumType &, int, int) =
        reinterpret_cast<void (*)(const TabClass *, KeyNumType &, int, int)>(0x005538D0);
    func(this, key, mouse_x, mouse_y);
#endif
}

void TabClass::Draw_It(BOOL force_redraw)
{
#ifndef RAPP_STANDALONE
    void (*func)(const TabClass *, BOOL) = reinterpret_cast<void (*)(const TabClass *, BOOL)>(0x005533A0);
    func(this, force_redraw);
#else
    SidebarClass::Draw_It(force_redraw);
    if (Debug_Map) {
        if ((force_redraw || TabToRedraw) && g_logicPage->Lock()) {
            v13 = Is_HiRes();
            x2 = g_seenBuff->Get_Width() - 1;
            g_logicPage->Fill_Rect(0, 0, x2, (8 << v13) - (2 << v13), 12);
            if (v13) {
                CC_Draw_Shape(TabShape, 1, 0, 0, 0, 0);
            } else {
                CC_Draw_Shape(TabShape, 2, 0, 0, 0, 0);
            }
            TabClass::Draw_Credits_Tab();
            TabClass::Draw_Passable_Tab(Map.ShowPassable);
            v10 = TabClass_Draw_Map_Size_Tab_defarg();
            TabClass::Draw_Map_Size_Tab(1, v10);
            g_logicPage->Draw_Line(0, (8 << v13) - (1 << v13), x2, (8 << v13) - (1 << v13), 0xCu);
            v11 = operator_OR(8, 256);
            v12 = operator_OR(v11, 0x4000);
            Fancy_Text_Print(TXT_OPTIONS, 40 << v13, 0, &MetalScheme, 0, v12);
            g_logicPage->Unlock();
        }
        if (force_redraw || TabToRedraw) {
            CreditDisplay.Graphic_Logic(1);
        } else {
            CreditDisplay.Graphic_Logic(0);
        }
        TabToRedraw = false;
    } else { //from RA 2.00 dos so values could be wrong, but its the cleanest pseudo
        if (force_redraw || TabToRedraw) {
            g_logicPage->Fill_Rect(0, 0, SeenBuff.vp.Width - 1, 7, 12);
            CC_Draw_Shape(TabShape, 0, 0, 0, 0, 0, 0, 0, 0, 256);
            TabClass::Draw_Credits_Tab();
            g_logicPage->(0, 7, SeenBuff.vp.Width - 1, 7, 12);
            Fancy_Text_Print(142, 0x50u, 0, &MetalScheme, 0, 16648);
            if (SidebarIsDrawn) {
                TabClass::Hilite_Tab(1);
            } else {
                CC_Draw_Shape(TabClass::TabShape, 0, SeenBuff.vp.Width - 80, 0, 0, 0, 0, 0, 0, 256);
                Fancy_Text_Print(141, SeenBuff.vp.Width - 40, 0, &RemapType_5, 0, 8472);
            }
            g_logicPage->Unlock();
        }
        CreditDisplay.Graphic_Logic(force_redraw || TabToRedraw);
        TabToRedraw = false;
    }
#endif
}

void TabClass::Flash_Money(void)
{
    TabToRedraw = true;
    Flag_To_Redraw();
    CreditsFlashTimer = 7;
}

void TabClass::Draw_Credits_Tab(void)
{
#ifndef RAPP_STANDALONE
    void (*func)(const TabClass *) = reinterpret_cast<void (*)(const TabClass *)>(0x00553744);
    func(this);
#endif
}

void TabClass::Hilite_Tab(TabEnum tab)
{
    int v1; // ebx@1
    char v2; // dl@3

    v1 = 0;
    if (tab) {
        v1 = 480;
    }
    CC_Draw_Shape(TabShape, 1, v1, 0, WINDOW_0, SHAPE_NORMAL);
    MetalScheme.MediumColor = 134;
    Fancy_Text_Print(
        TXT_TAB_BUTTON_CONTROLS, 80, 0, &MetalScheme, COLOR_TBLACK, TPF_USE_GRAD_PAL | TPF_CENTER | TPF_12PT_METAL);
    MetalScheme.MediumColor = v2;
}

void TabClass::Set_Active(TabEnum tab)
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