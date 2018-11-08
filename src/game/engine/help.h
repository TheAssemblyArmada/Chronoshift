/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling help tooltips.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef HELP_H
#define HELP_H

#include "always.h"
#include "tab.h"

class HelpClass : public TabClass
{
    enum
    {
        HELP_OVERLAP_BUFFER = 60,
    };

public:
    HelpClass();
    HelpClass(const NoInitClass &noinit) : TabClass(noinit), CountDownTimer(noinit) {}

    virtual void Init_Clear() override;
    virtual void AI(KeyNumType &key, int mouse_x, int mouse_y) override;
    virtual void Draw_It(BOOL force_redraw) override;
    virtual void Help_Text(int str_id, int x = -1, int y = -1, int color = 14, BOOL no_wait = false) override;
    virtual BOOL Scroll_Map(DirType dir, int &distance, BOOL redraw = true) override;
    virtual void Set_Tactical_Position(uint32_t location) override;

    const int16_t *Overlap_List() const;
    void Set_Cost(int cost) { HelpCost = cost; }
    void Set_Text_Color(uint8_t color) { HelpTextColor = color; }

#ifndef CHRONOSHIFT_STANDALONE
    static void Hook_Me();
    const int16_t *Hook_Text_Overlap_List(const char *string, int x, int y);
#endif

private:
    void Set_Text(int string_id);   

protected:
    int HelpUnkInt1;
    int HelpUnkInt2;
    int HelpUnkInt3;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool HelpForceDraw : 1; // 1
        };
        int Bitfield;
    };
#else
    bool HelpForceDraw;
#endif
    int HelpCost;
    int HelpMouseXPos;
    int HelpMouseYPos;
    int32_t HelpXPos;
    int32_t HelpYPos;
    int HelpWidth;
    int HelpTextID; // TextEnum is but setting it offsets alignment
    int HelpTextColor; // should just be unsigned chat but it offsets alignment
    TCountDownTimerClass<SystemTimerClass> CountDownTimer;

private:
#ifndef CHRONOSHIFT_STANDALONE
    static int16_t *const OverlapList;
    static char *&HelpText;
#else
    static int16_t OverlapList[HELP_OVERLAP_BUFFER];
    static char *HelpText;
#endif
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
inline const int16_t *HelpClass::Hook_Text_Overlap_List(const char *string, int x, int y)
{
    return HelpClass::Text_Overlap_List(string, x, y);
}

inline void HelpClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x004D2338, *HelpClass::Init_Clear);
    Hook_Function(0x004D23C8, *HelpClass::AI);
    Hook_Function(0x004D26B0, *HelpClass::Draw_It);
    Hook_Function(0x004D2574, *HelpClass::Help_Text);
    Hook_Function(0x004D2B6C, *HelpClass::Scroll_Map);
    Hook_Function(0x004D2BC0, *HelpClass::Set_Tactical_Position);
    Hook_Function(0x004D2354, *HelpClass::Hook_Text_Overlap_List);
    Hook_Function(0x004D293C, *HelpClass::Set_Text);
#endif
}
#endif

#endif // HELP_H
