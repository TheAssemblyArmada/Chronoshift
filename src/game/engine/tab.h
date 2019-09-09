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
#pragma once

#ifndef TAB_H
#define TAB_H

#include "always.h"
#include "sidebar.h"

enum TabEnum
{
    TAB_NONE = -1,
    TAB_OPTIONS = 0,
    TAB_SIDEBAR = 1,
    TAB_PASSABLE = 2,
    TAB_CREDITS = 3
};

// this basicly represents the shape frames.
enum TabShapeEnum
{
    TAB_SHAPE_0 = 0, // left up
    TAB_SHAPE_1 = 1, // left down
    TAB_SHAPE_2 = 2, // middle up
    TAB_SHAPE_3 = 3, // middle down
    TAB_SHAPE_4 = 4, // middle red up
    TAB_SHAPE_5 = 5, // middle red down
    TAB_SHAPE_6 = 6, // right up
    TAB_SHAPE_7 = 7, // right down
    TAB_SHAPE_8 = 8, // right red down
};

class TabClass : public SidebarClass
{
    enum
    {
        TAB_WIDTH = 160,
    };

    class CreditClass
    {
    public:
        CreditClass();
        CreditClass(const NoInitClass &noinit) {}

        void Graphic_Logic(BOOL force_redraw = false);
        void AI(BOOL a1 = false);
        void Set_Available(int avail) { Available = avail; }

    protected:
        int Available;
        int Credits;
#ifndef CHRONOSHIFT_NO_BITFIELDS
        BOOL CreditToRedraw : 1; // & 1
        BOOL CreditHasIncreased : 1; // & 2
        BOOL CreditHasChanged : 1; // & 4
#else
        bool CreditToRedraw;
        bool CreditHasIncreased;
        bool CreditHasChanged;
#endif
        int field_C;
    };

public:
    TabClass();
    TabClass(const NoInitClass &noinit) : SidebarClass(noinit), CreditDisplay(noinit), TimerFlashTimer(noinit), CreditsFlashTimer(noinit) {}

    virtual void One_Time() override;
    virtual void AI(KeyNumType &key, int mouse_x, int mouse_y) override;
    virtual void Draw_It(BOOL force_redraw) override;

    void Flash_Money();

    static void Hilite_Tab(int tab);

#ifdef GAME_DLL
    static void Hook_Me();
#endif

private:
    void Set_Active(int tab);
    static void Draw_Credits_Tab();

protected:
    CreditClass CreditDisplay;
    TCountDownTimerClass<FrameTimerClass> TimerFlashTimer;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL TabToRedraw : 1; // 1
#else
    bool TabToRedraw;
#endif
    TCountDownTimerClass<FrameTimerClass> CreditsFlashTimer;

private:
#ifdef GAME_DLL
    static void *&TabShape;
#else
    static void *TabShape;
#endif
    static void *PassableShape; // Edwin tab for showing passablity of cells
    static void *TabBackgroundShape; // name subject to change
};

#ifdef GAME_DLL
#include "hooker.h"
inline void TabClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x005533A0, *TabClass::Draw_It);
    Hook_Function(0x005539D8, *TabClass::One_Time);
    Hook_Function(0x005538D0, *TabClass::AI);
    Hook_Function(0x00553744, *TabClass::Draw_Credits_Tab);
#endif
}
#endif

#endif // TAB_H
