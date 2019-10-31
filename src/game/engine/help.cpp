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
    m_HelpUnkInt1(0),
    m_HelpUnkInt2(0),
    m_HelpUnkInt3(0),
    m_HelpForceDraw(false),
    m_HelpCost(0),
    m_HelpMouseXPos(0),
    m_HelpMouseYPos(0),
    m_HelpXPos(0),
    m_HelpYPos(0),
    m_HelpWidth(0),
    m_HelpTextID(0),
    m_HelpTextColor(14),
    m_CountDownTimer()
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
    if (m_CountDownTimer == 0 && !m_HelpForceDraw && (mouse_x != m_HelpMouseXPos || mouse_y != m_HelpMouseYPos)) {
        HelpClass::Help_Text(TXT_NULL);
    }

    if (m_CountDownTimer != 0 && HelpText == nullptr && m_HelpTextID != TXT_NULL) {
        if (m_HelpForceDraw || (m_HelpMouseXPos == mouse_x && m_HelpMouseYPos == mouse_y)) {
            HelpClass::Set_Text(m_HelpTextID);
        } else {
            m_HelpMouseXPos = mouse_x;
            m_HelpMouseYPos = mouse_y;
            m_CountDownTimer = 60;
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

    if (m_HelpTextID == TXT_NULL) {
        return;
    }

    if (m_CountDownTimer == 0 && g_logicPage->Lock()) {
        Plain_Text_Print(m_HelpTextID, m_HelpXPos, m_HelpYPos, m_HelpTextColor, COLOR_BLACK, TPF_NOSHADOW | TPF_MAP);
        g_logicPage->Draw_Rect(m_HelpXPos - 1, m_HelpYPos - 1, m_HelpWidth + m_HelpXPos + 1, g_fontHeight + m_HelpYPos, m_HelpTextColor);
        
        if (m_HelpCost != 0) {
            snprintf(buffer, sizeof(buffer), "$%d", m_HelpCost);
            int stringwidth = String_Pixel_Width(buffer);
            Plain_Text_Print(buffer, m_HelpXPos, m_HelpYPos + g_fontHeight, m_HelpTextColor, COLOR_BLACK, TPF_NOSHADOW | TPF_MAP);
            
            g_logicPage->Draw_Rect(m_HelpXPos - 1,
                m_HelpYPos + g_fontHeight,
                stringwidth + m_HelpXPos + 1,
                g_fontHeight + m_HelpYPos + g_fontHeight - 1,
                m_HelpTextColor);

            g_logicPage->Draw_Line(m_HelpXPos,
                m_HelpYPos + g_fontHeight,
                m_HelpXPos + std::min(stringwidth + 1, m_HelpWidth) - 1,
                m_HelpYPos + g_fontHeight,
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
    if (m_HelpTextID != str_id) {
        // If we are changing a displayed string, clear the cells it overlapped.
        if (m_HelpTextID != TXT_NULL) {
            Refresh_Cells(Coord_To_Cell(m_DisplayPos), HelpClass::OverlapList);
        }

        m_HelpMouseXPos = (x == -1 ? g_mouse->Get_Mouse_X() : x);
        m_HelpMouseYPos = (y == -1 ? g_mouse->Get_Mouse_Y() : y);
        m_HelpForceDraw = (x != -1 || y != -1);

        if (no_wait) {
            m_CountDownTimer = 1;
        } else {
            m_CountDownTimer = 60;
        }

        m_HelpTextColor = 80;
        m_HelpTextID = str_id;
        m_HelpCost = 0;
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
    if (m_DisplayPos != location) {
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
    if (m_HelpTextID == TXT_NULL || m_CountDownTimer > 0) {
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
        m_HelpTextID = string_id;
        Plain_Text_Print(0, 0, 0, 0, 0, TPF_NOSHADOW | TPF_MAP); // Clears the formatting from previous calls.
        m_HelpWidth = String_Pixel_Width(Text_String(m_HelpTextID));

        if (m_HelpForceDraw) {
            m_HelpXPos = m_HelpMouseXPos - m_HelpWidth;
            m_HelpYPos = m_HelpMouseYPos;
        } else {
            int x_limit = m_TacOffsetX + Lepton_To_Pixel(m_DisplayWidth) - 6;
            int y_limit = m_TacOffsetY + Lepton_To_Pixel(m_DisplayHeight) - 2;
            m_HelpXPos = m_HelpMouseXPos + 12;
            m_HelpYPos = m_HelpMouseYPos;

            int x_right = m_HelpWidth + m_HelpXPos;

            if (x_right > x_limit) {
                m_HelpXPos -= x_right - x_limit;
            }
            
            int y_bottom = m_HelpYPos + 20;

            if (y_bottom > y_limit) {
                m_HelpYPos -= y_bottom - y_limit;
            }
            
            if (m_TacOffsetX + 1 > m_HelpXPos) {
                m_HelpXPos = m_TacOffsetX + 1;
            }
            
            if (m_TacOffsetY + 1 > m_HelpYPos) {
                m_HelpYPos = m_TacOffsetY + 1;
            }
        }

        // Copy list and mark final buffer position as end in case source list is longer than buffer.
        memcpy(OverlapList, Text_Overlap_List(Text_String(m_HelpTextID), m_HelpXPos - 1, m_HelpYPos), sizeof(OverlapList));
        OverlapList[HELP_OVERLAP_BUFFER - 1] = LIST_END;
    }
}
