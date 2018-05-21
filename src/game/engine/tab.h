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
private:
    class CreditClass
    {
    public:
        CreditClass(void);

        void Graphic_Logic(bool a1 = false);
        void AI(bool a1 = false);
        void Set_Available(int avail) { Available = avail; }

    protected:
        int Available;
        int Credits;
#ifndef RAPP_NO_BITFIELDS
        // Union/Struct required to get correct packing when compiler packing set to 1.
        union
        {
            struct
            {
                bool CreditToRedraw : 1;
                bool CreditHasIncreased : 1;
                bool CreditHasChanged : 1;
            };
            int Bitfield;
        };
#else
        bool CreditToRedraw;
        bool CreditHasIncreased;
        bool CreditHasChanged;
#endif
        int field_C;
    };

public:
    TabClass();
    TabClass(NoInitClass &noinit) : SidebarClass(noinit) {}

    virtual void One_Time() override;
    virtual void AI(KeyNumType &key, int mouse_x, int mouse_y) override;
    virtual void Draw_It(BOOL force_redraw) override;

    void Flash_Money(void);
    void Draw_Passable_Tab(BOOL state);
    void Draw_Map_Size_Tab();
    static void Draw_Credits_Tab(void);
    static void Hilite_Tab(TabEnum tab = TAB_NONE);
    void Set_Active(TabEnum tab = TAB_NONE);

#ifndef RAPP_STANDALONE
    static void Hook_Me();
#endif

protected:
    CreditClass CreditDisplay;
    TCountDownTimerClass<FrameTimerClass> TimerFlashTimer;
#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool TabToRedraw : 1; // 1
        };
        int Bitfield;
    };
#else
    bool TabToRedraw;
#endif
    TCountDownTimerClass<FrameTimerClass> CreditsFlashTimer; // 0x1617
private:
#ifndef RAPP_STANDALONE
    static void *&TabShape;
#else
    static void *TabShape;
#endif
    static void *PassableShape; // Edwin tab for showing passablity of cells
};

#ifndef RAPP_STANALONE
#include "hooker.h"
inline void TabClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    // Hook_Function(0x, *TabClass::Init_Clear);
#endif
}
#endif

#endif // TAB_H
