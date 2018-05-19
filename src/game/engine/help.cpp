/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling help tooltips.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */

#include "help.h"
#include "tab.h"
#include "globals.h"

HelpClass::HelpClass()
{
#ifndef RAPP_STANDALONE
    void (*func)(const HelpClass *) = reinterpret_cast<void (*)(const HelpClass *)>(0x004D2270);
    func(this);
#endif
}

void HelpClass::Init_Clear()
{
    TabClass::Init_Clear();
    Set_Text(TXT_NULL);
}

void HelpClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
#ifndef RAPP_STANDALONE
    void (*func)(const HelpClass *, KeyNumType &, int, int) = reinterpret_cast<void (*)(const HelpClass *, KeyNumType &, int, int)>(0x004D23C8);
    func(this, key, mouse_x, mouse_y);
#endif
}

void HelpClass::Draw_It(BOOL force_redraw)
{
#ifndef RAPP_STANDALONE
    void (*func)(const HelpClass *, BOOL) = reinterpret_cast<void (*)(const HelpClass *, BOOL)>(0x004D26B0);
    func(this, force_redraw);
#endif
}

void HelpClass::Help_Text(int str_id, int x, int y, int color, BOOL on_wait)
{
#ifndef RAPP_STANDALONE
    void (*func)(const HelpClass *, int, int, int, int, BOOL) = reinterpret_cast<void (*)(const HelpClass *, int, int, int, int, BOOL)>(0x004D2574);
    func(this, str_id, x, y, color, on_wait);
#endif
}

BOOL HelpClass::Scroll_Map(DirType dir, int &distance, BOOL redraw)
{
    if ( redraw ) {
        Help_Text(TXT_NULL);
    }
    return TabClass::Scroll_Map(dir, distance, redraw);
}

void HelpClass::Set_Tactical_Position(uint32_t location)
{
    if ( DisplayPos != location ) {
        Help_Text(TXT_NULL);
    }
    return TabClass::Set_Tactical_Position(location);
}

int16_t *const HelpClass::Overlap_List(void) const
{
#ifndef RAPP_STANDALONE
    int16_t* (*func)(const HelpClass *) = reinterpret_cast<int16_t* (*)(const HelpClass *)>(0x004D2354);
    return func(this);
#endif
}

void HelpClass::Set_Text(TextEnum string_id)
{
#ifndef RAPP_STANDALONE
    void (*func)(const HelpClass *, TextEnum) = reinterpret_cast<void (*)(const HelpClass *, TextEnum)>(0x004D293C);
    func(this, string_id);
#endif
}

void HelpClass::Set_Cost(int cost)
{
    HelpCost = cost;
}

void HelpClass::Set_Text_Color(unsigned char color)
{
    HelpTextColor = color;
}