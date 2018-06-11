/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling scrolling the viewport.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */

#include "scroll.h"
#include "gameoptions.h"
#include "gametypes.h"
#include "globals.h"
#include "minmax.h"

#ifndef RAPP_STANDALONE
TCountDownTimerClass<SystemTimerClass> &ScrollClass::ScrollingCounter =
    *reinterpret_cast<TCountDownTimerClass<SystemTimerClass> *>(0x00687C08);
#else
TCountDownTimerClass<SystemTimerClass> ScrollClass::ScrollingCounter;
#endif

/**
 * 0x00547018
 */
ScrollClass::ScrollClass() :
    Autoscroll(true),
    ScrollUnkInt(0)
{
    ScrollingCounter = 0;
}

/**
 * @brief Initialise the class for IO.
 *
 * 0x004F92E0
 */
void ScrollClass::Init_IO()
{
    ScrollingCounter = 0;
    HelpClass::Init_IO();
}

/**
 * @brief Initialise the class for IO.
 *
 * 0x00547088
 */
void ScrollClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
    static const int _rate[] = { 448, 384, 320, 256, 192, 128, 64, 32, 16 };

    static const MouseType _scroll_mouse[FACING_COUNT] = { MOUSE_SCROLL_N,
        MOUSE_SCROLL_NE,
        MOUSE_SCROLL_E,
        MOUSE_SCROLL_SE,
        MOUSE_SCROLL_S,
        MOUSE_SCROLL_SW,
        MOUSE_SCROLL_W,
        MOUSE_SCROLL_NW };

    static const MouseType _noscroll_mouse[FACING_COUNT] = { MOUSE_CANT_SCROLL_N,
        MOUSE_CANT_SCROLL_NE,
        MOUSE_CANT_SCROLL_E,
        MOUSE_CANT_SCROLL_SE,
        MOUSE_CANT_SCROLL_S,
        MOUSE_CANT_SCROLL_SW,
        MOUSE_CANT_SCROLL_W,
        MOUSE_CANT_SCROLL_NW };

    static DirType _direction = DIR_NONE;

    if (!DisplayBit8) {
        int vp_w = g_seenBuff.Get_Width();
        int vp_h = g_seenBuff.Get_Height();

        bool at_edge = (mouse_x <= 0 || mouse_y <= 0 || vp_w - 1 <= mouse_x || vp_h - 1 <= mouse_y);
        bool edge_scrolling = false;

        if (ScrollUnkInt != 0 || at_edge) {
            if (at_edge) {
                edge_scrolling = true;

                int x_pos = mouse_x;
                if (x_pos < 100) {
                    x_pos -= x_pos - 100;
                }

                x_pos = Max(0, x_pos);

                if (x_pos > /*540*/ (vp_w - 100)) {
                    x_pos += x_pos - /*540*/ (vp_w - 100);
                }

                x_pos = Min(x_pos, vp_w);
                
                if (x_pos > 100 && x_pos < /*540*/ (vp_w - 100)) {
                    x_pos += (320 - x_pos) / 2;
                }

                int y_pos = mouse_y;

                if (y_pos < 100) {
                    y_pos -= 100 - y_pos;
                }

                y_pos = Max(0, y_pos);

                if (y_pos > /*300*/ (vp_h - 100)) {
                    y_pos += y_pos - /*300*/ (vp_h - 100);
                }

                y_pos = Min(y_pos, vp_h);

                _direction = Desired_Facing256(320, 200, x_pos, y_pos);
            }

            FacingType scroll_facing = Direction_To_Facing(_direction);

            int rate_index = g_inMapEditor ? Options.Get_Scroll_Rate() + 1 : (8 - ScrollUnkInt);

            if (rate_index < (Options.Get_Scroll_Rate() + 1)) {
                rate_index = (Options.Get_Scroll_Rate() + 1);
                ScrollUnkInt = 8 - (Options.Get_Scroll_Rate() + 1);
            }

            // if the right mouse button is down, the scroll speed halfs?
            if (g_keyboard->Down(KN_RMOUSE)) {
                rate_index = Clamp((rate_index + 1), 4, 8);
            }

            if (!Options.Get_Auto_Scroll()) {
                _direction = Facing_To_Direction(Direction_To_Facing(_direction));
            }

            int distance = _rate[rate_index] / 2;

            if (Scroll_Map(_direction, distance, false)) {
                Override_Mouse_Shape(_scroll_mouse[scroll_facing]);

                if (g_keyboard->Down(KN_LMOUSE) || Autoscroll) {
                    distance = _rate[rate_index];
                    Scroll_Map(_direction, distance, true);
                        
                    if (g_inMapEditor) {
                        ScrollingCounter = 1;
                    } else {
                        if (edge_scrolling && ScrollingCounter == 0) {
                            ScrollingCounter = 1;
                            ++ScrollUnkInt;
                        }
                    }
                }
            } else {
                Override_Mouse_Shape(_noscroll_mouse[scroll_facing]);
            }
        }

        if (!g_inMapEditor && !edge_scrolling && ScrollingCounter == 0) {
            --ScrollUnkInt;

            if (ScrollUnkInt < 0) {
                ++ScrollUnkInt;
            }

            ScrollingCounter = 1;
        }
    }

    HelpClass::AI(key, mouse_x, mouse_y);
}

/**
 * @brief Set auto scrolling on or off.
 *
 * 0x00547464
 */
BOOL ScrollClass::Set_Autoscroll(int mode)
{
    BOOL old = Autoscroll;

    if (mode == SCROLLMODE_TOGGLE) {
        Autoscroll = !Autoscroll;
    } else {
        Autoscroll = mode == SCROLLMODE_AUTO;
    }

    return old;
}
