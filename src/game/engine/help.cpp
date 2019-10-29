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
#include "help.h"
#include "lists.h"
#include "mouse.h"
#include "textprint.h"
#include "coord.h"
#include <algorithm>
#include <cstdio>

using std::snprintf;

#ifndef GAME_DLL
int16_t HelpClass::OverlapList[60];
char *HelpClass::HelpText = nullptr;
#endif

/**
 * 0x004D2270
 */
HelpClass::HelpClass() :
    HelpUnkInt1(0),
    HelpUnkInt2(0),
    HelpUnkInt3(0),
    HelpForceDraw(false),
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

/**
 * @brief Clears the current state.
 *
 * 0x004D2338
 */
void HelpClass::Init_Clear()
{
    TabClass::Init_Clear();
    Set_Text(TXT_NULL);
}

/**
 * @brief Performs per tick logic for the object.
 *
 * 0x004D23C8
 */
void HelpClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
    // Clear the text if the mouse has moved.
    if (CountDownTimer == 0 && !HelpForceDraw && (mouse_x != HelpMouseXPos || mouse_y != HelpMouseYPos)) {
        HelpClass::Help_Text(TXT_NULL);
    }

    if (CountDownTimer != 0 && HelpText == nullptr && HelpTextID != TXT_NULL) {
        if (HelpForceDraw || (HelpMouseXPos == mouse_x && HelpMouseYPos == mouse_y)) {
            HelpClass::Set_Text(HelpTextID);
        } else {
            HelpMouseXPos = mouse_x;
            HelpMouseYPos = mouse_y;
            CountDownTimer = 60;
            HelpClass::Set_Text(TXT_NULL);
        }
    }

    TabClass::AI(key, mouse_x, mouse_y);
}

/**
 * @brief Draws tooltip if conditions allow.
 *
 * 0x004D26B0
 */
void HelpClass::Draw_It(BOOL force_redraw)
{
    char buffer[16];
    TabClass::Draw_It(force_redraw);

    if (HelpTextID == TXT_NULL) {
        return;
    }

    if (CountDownTimer == 0 && g_logicPage->Lock()) {
        Plain_Text_Print(HelpTextID, HelpXPos, HelpYPos, HelpTextColor, COLOR_BLACK, TPF_NOSHADOW | TPF_MAP);
        g_logicPage->Draw_Rect(HelpXPos - 1, HelpYPos - 1, HelpWidth + HelpXPos + 1, g_fontHeight + HelpYPos, HelpTextColor);
        
        if (HelpCost != 0) {
            snprintf(buffer, sizeof(buffer), "$%d", HelpCost);
            int stringwidth = String_Pixel_Width(buffer);
            Plain_Text_Print(buffer, HelpXPos, HelpYPos + g_fontHeight, HelpTextColor, COLOR_BLACK, TPF_NOSHADOW | TPF_MAP);
            
            g_logicPage->Draw_Rect(HelpXPos - 1,
                HelpYPos + g_fontHeight,
                stringwidth + HelpXPos + 1,
                g_fontHeight + HelpYPos + g_fontHeight - 1,
                HelpTextColor);

            g_logicPage->Draw_Line(HelpXPos,
                HelpYPos + g_fontHeight,
                HelpXPos + std::min(stringwidth + 1, HelpWidth) - 1,
                HelpYPos + g_fontHeight,
                COLOR_BLACK);
        }

        g_logicPage->Unlock();
    }
}

/**
 * @brief Sets the help text string from the string table.
 *
 * 0x004D2574
 */
void HelpClass::Help_Text(int str_id, int x, int y, int color, BOOL no_wait)
{
    if (HelpTextID != str_id) {
        // If we are changing a displayed string, clear the cells it overlapped.
        if (HelpTextID != TXT_NULL) {
            Refresh_Cells(Coord_To_Cell(DisplayPos), HelpClass::OverlapList);
        }

        HelpMouseXPos = (x == -1 ? g_mouse->Get_Mouse_X() : x);
        HelpMouseYPos = (y == -1 ? g_mouse->Get_Mouse_Y() : y);
        HelpForceDraw = (x != -1 || y != -1);

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

/**
 * @brief Scrolls the map, clearing any tooltip currently displayed.
 *
 * 0x004D2B6C
 */
BOOL HelpClass::Scroll_Map(DirType dir, int &distance, BOOL redraw)
{
    if (redraw) {
        Help_Text(TXT_NULL);
    }

    return TabClass::Scroll_Map(dir, distance, redraw);
}

/**
 * @brief Set the tactical display position, clearing any tooltip currently displayed.
 *
 * 0x004D2BC0
 */
void HelpClass::Set_Tactical_Position(coord_t location)
{
    if (DisplayPos != location) {
        Help_Text(TXT_NULL);
    }

    return TabClass::Set_Tactical_Position(location);
}

/**
 * @brief Gets the overlap list for the current tooltip, nulling it if the text is to be removed.
 *
 * 0x004D2354
 */
const int16_t *HelpClass::Overlap_List() const
{
    if (HelpTextID == TXT_NULL || CountDownTimer > 0) {
        OverlapList[0] = LIST_END;
    }

    return OverlapList;
}

/**
 * @brief Sets the tooltip string.
 *
 * 0x004D293C
 */
void HelpClass::Set_Text(int string_id)
{
    if (string_id) {
        HelpTextID = string_id;
        Plain_Text_Print(0, 0, 0, 0, 0, TPF_NOSHADOW | TPF_MAP); // Clears the formatting from previous calls.
        HelpWidth = String_Pixel_Width(Text_String(HelpTextID));

        if (HelpForceDraw) {
            HelpXPos = HelpMouseXPos - HelpWidth;
            HelpYPos = HelpMouseYPos;
        } else {
            int x_limit = TacOffsetX + Lepton_To_Pixel(DisplayWidth) - 6;
            int y_limit = TacOffsetY + Lepton_To_Pixel(DisplayHeight) - 2;
            HelpXPos = HelpMouseXPos + 12;
            HelpYPos = HelpMouseYPos;

            int x_right = HelpWidth + HelpXPos;

            if (x_right > x_limit) {
                HelpXPos -= x_right - x_limit;
            }
            
            int y_bottom = HelpYPos + 20;

            if (y_bottom > y_limit) {
                HelpYPos -= y_bottom - y_limit;
            }
            
            if (TacOffsetX + 1 > HelpXPos) {
                HelpXPos = TacOffsetX + 1;
            }
            
            if (TacOffsetY + 1 > HelpYPos) {
                HelpYPos = TacOffsetY + 1;
            }
        }

        // Copy list and mark final buffer position as end in case source list is longer than buffer.
        memcpy(OverlapList, Text_Overlap_List(Text_String(HelpTextID), HelpXPos - 1, HelpYPos), sizeof(OverlapList));
        OverlapList[HELP_OVERLAP_BUFFER - 1] = LIST_END;
    }
}
