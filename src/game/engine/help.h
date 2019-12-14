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
    HelpClass(const NoInitClass &noinit) : TabClass(noinit), m_CountDownTimer(noinit) {}

    virtual void Init_Clear() override;
    virtual void AI(KeyNumType &key, int mouse_x, int mouse_y) override;
    virtual void Draw_It(BOOL force_redraw) override;
    virtual void Help_Text(int str_id, int x = -1, int y = -1, int color = 14, BOOL no_wait = false) override;
    virtual BOOL Scroll_Map(DirType dir, int &distance, BOOL redraw = true) override;
    virtual void Set_Tactical_Position(coord_t location) override;

    const int16_t *Overlap_List() const;
    void Set_Cost(int cost) { m_HelpCost = cost; }
    void Set_Text_Color(uint8_t color) { m_HelpTextColor = color; }

#ifdef GAME_DLL
    const int16_t *Hook_Overlap_List()
    {
        return HelpClass::Overlap_List();
    }
#endif

private:
    void Set_Text(int string_id);   

protected:
    int m_HelpUnkInt1;
    int m_HelpUnkInt2;
    int m_HelpUnkInt3;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_HelpForceDraw : 1; // 1
#else
    bool m_HelpForceDraw;
#endif
    int m_HelpCost;
    int m_HelpMouseXPos;
    int m_HelpMouseYPos;
    int32_t m_HelpXPos;
    int32_t m_HelpYPos;
    int m_HelpWidth;
    int m_HelpTextID; // TextEnum is but setting it offsets alignment
    int m_HelpTextColor; // should just be unsigned chat but it offsets alignment
    TCountDownTimerClass<SystemTimerClass> m_CountDownTimer;

private:
#ifdef GAME_DLL
    static ARRAY_DEC(int16_t, s_OverlapList, HELP_OVERLAP_BUFFER);
    static char *&s_HelpText;
#else
    static int16_t s_OverlapList[HELP_OVERLAP_BUFFER];
    static char *s_HelpText;
#endif
};

#endif // HELP_H
