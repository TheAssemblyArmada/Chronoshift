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
#include "textprint.h"
#include "coord.h"
#include "lists.h"
#include "minmax.h"
#include <stdio.h>

int16_t HelpClass::OverlapList[60];
char *HelpClass::HelpText = NULL;

HelpClass::HelpClass() :
    field_1620(0),
    field_1624(0),
    field_1628(0),
    HelpBit1(false),
    HelpCost(0),
    HelpMouseXPos(0),
    HelpMouseYPos(0),
    HelpXPos(0),
    HelpYPos(0),
    HelpWidth(0),
    HelpTextID(0),
    HelpTextColor(14),
    CountDownTimer()
{
}

void HelpClass::Init_Clear()
{
    TabClass::Init_Clear();
    Set_Text(TXT_NULL);
}

void HelpClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
    if (!CountDownTimer.Time() && !HelpBit1 && (mouse_x != HelpMouseXPos || mouse_y != HelpMouseYPos)) {
        HelpClass::Help_Text(TXT_NULL);
    }
    if (CountDownTimer.Time()) {
        if (HelpText == nullptr && HelpTextID != TXT_NULL) {
            if (HelpBit1 || HelpMouseXPos == mouse_x && HelpMouseYPos == mouse_y) {
                HelpClass::Set_Text((TextEnum)HelpTextID);
            } else {
                HelpMouseXPos = mouse_x;
                HelpMouseYPos = mouse_y;
                CountDownTimer = 60;
                HelpClass::Set_Text(TXT_NULL);
            }
        }
    }
    TabClass::AI(key, mouse_x, mouse_y);
}

void HelpClass::Draw_It(BOOL force_redraw)
{
    char str[4];
    TabClass::Draw_It(force_redraw);
    if (HelpText && (force_redraw || !CountDownTimer.Time()) && g_logicPage->Lock()) {
        Plain_Text_Print(HelpText, HelpXPos, HelpYPos, HelpTextColor, 12, TPF_NOSHADOW | TPF_MAP);
        g_logicPage->Draw_Rect(HelpXPos - 1, HelpYPos - 1, HelpWidth + HelpXPos + 1, g_fontHeight + HelpYPos, HelpTextColor);
        if (HelpCost) {
            sprintf(str, "$%d", HelpCost);
            int stringwidth = String_Pixel_Width(str);
            Plain_Text_Print(str, HelpXPos, HelpYPos + g_fontHeight, HelpTextColor, 12, TPF_NOSHADOW | TPF_MAP);
            g_logicPage->Draw_Rect(HelpXPos - 1,
                HelpYPos + g_fontHeight,
                stringwidth + HelpXPos + 1,
                g_fontHeight + HelpYPos + g_fontHeight - 1,
                HelpTextColor);
            g_logicPage->Draw_Line(HelpXPos,
                HelpYPos + g_fontHeight,
                HelpXPos + Min(stringwidth + 1, HelpWidth) - 1,
                HelpYPos + g_fontHeight,
                12);
        }
        g_logicPage->Unlock();
    }
}

void HelpClass::Help_Text(int str_id, int x, int y, int color, BOOL no_wait)
{
    if (HelpTextID != str_id) {
        if (HelpTextID != TXT_NULL) {
            Refresh_Cells(Coord_To_Cell(DisplayPos), HelpClass::OverlapList);
        }
        HelpMouseXPos = (x == -1 ? g_mouse->Get_Mouse_X() : x);
        HelpMouseYPos = (y == -1 ? g_mouse->Get_Mouse_Y() : y);

        HelpBit1 = (x != -1 || y != -1);

        if (no_wait) {
            CountDownTimer = 1;
        } else {
            CountDownTimer = 60;
        }
        HelpTextColor = 80;
        HelpTextID = str_id;
        HelpCost = 0;
    }
}

BOOL HelpClass::Scroll_Map(DirType dir, int &distance, BOOL redraw)
{
    if (redraw) {
        Help_Text(TXT_NULL);
    }
    return TabClass::Scroll_Map(dir, distance, redraw);
}

void HelpClass::Set_Tactical_Position(uint32_t location)
{
    if (DisplayPos != location) {
        Help_Text(TXT_NULL);
    }
    return TabClass::Set_Tactical_Position(location);
}

int16_t *const HelpClass::Overlap_List(void) const
{
    if (HelpTextID == TXT_NULL /*|| CountDownTimer.Time() > 0*/) { // Error! E419: col(40) cannot call non-const function for
                                                                   // a constant object
        OverlapList[0] = LIST_END;
    }
    return OverlapList;
}

void HelpClass::Set_Text(TextEnum string_id)
{
    if (string_id) {
        HelpTextID = string_id;
        Plain_Text_Print(0, 0, 0, 0, 0, TPF_NOSHADOW | TPF_MAP);
        const char *string = Fetch_String(HelpTextID);
        HelpWidth = String_Pixel_Width(string);
        if (HelpBit1) {
            HelpXPos = HelpMouseXPos - HelpWidth;
            HelpYPos = HelpMouseYPos;
        } else {
            int v10 = TacOffsetX + Lepton_To_Pixel(DisplayHeight) - 3;
            int v9 = TacOffsetY + Lepton_To_Pixel(DisplayWidth) - 1;
            HelpXPos = HelpMouseXPos + 10;
            HelpYPos = HelpMouseYPos;
            if (HelpWidth + HelpXPos > v10) {
                HelpXPos -= HelpWidth + HelpXPos - v10;
            }
            if (HelpYPos + 10 > v9) {
                HelpYPos -= HelpYPos + 10 - v9;
            }
            if (TacOffsetX + 1 > HelpXPos) {
                HelpXPos = TacOffsetX + 1;
            }
            if (TacOffsetY + 1 > HelpYPos) {
                HelpYPos = TacOffsetY + 1;
            }
        }
        short const *overlaplist = Text_Overlap_List(string, HelpXPos - 1, HelpYPos);
        memcpy(HelpClass::OverlapList, overlaplist, sizeof(HelpClass::OverlapList));
    }
}

void HelpClass::Set_Cost(int cost)
{
    HelpCost = cost;
}

void HelpClass::Set_Text_Color(unsigned char color)
{
    HelpTextColor = color;
}