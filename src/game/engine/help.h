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
    friend void Setup_Hooks();

public:
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
    virtual void Set_Tactical_Position(coord_t location) override;

    const int16_t *Overlap_List() const;
    void Set_Cost(int cost) { HelpCost = cost; }
    void Set_Text_Color(uint8_t color) { HelpTextColor = color; }

#ifdef GAME_DLL
    const int16_t *Hook_Text_Overlap_List(const char *string, int x, int y)
    {
        return HelpClass::Text_Overlap_List(string, x, y);
    }
#endif

private:
    void Set_Text(int string_id);   

protected:
    int HelpUnkInt1;
    int HelpUnkInt2;
    int HelpUnkInt3;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL HelpForceDraw : 1; // 1
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
#ifdef GAME_DLL
    static ARRAY_DEC(int16_t, OverlapList, HELP_OVERLAP_BUFFER);
    static char *&HelpText;
#else
    static int16_t OverlapList[HELP_OVERLAP_BUFFER];
    static char *HelpText;
#endif
};

#endif // HELP_H
