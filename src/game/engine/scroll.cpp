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

#ifndef RAPP_STANDALONE
TCountDownTimerClass<SystemTimerClass> &ScrollClass::ScrollingCounter =
    *reinterpret_cast<TCountDownTimerClass<SystemTimerClass> *>(0x00687C08);
#else
TCountDownTimerClass<SystemTimerClass> ScrollClass::ScrollingCounter;
#endif

ScrollClass::ScrollClass() :
    Autoscroll(false),
    field_165D(0)
{
    ScrollingCounter = 0;
}

void ScrollClass::Init_IO(void)
{
    ScrollingCounter = 0;
    HelpClass::Init_IO();
}

void ScrollClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
#ifndef RAPP_STANDALONE
    DEFINE_CALL(func, 0x00547088, void, (const ScrollClass *, KeyNumType &, int, int));
    func(this, key, mouse_x, mouse_y);
#else
    int a1a; // [sp+34h] [bp-34h]@16
    int v16; // [sp+38h] [bp-30h]@24
    int ratetouse; // [sp+40h] [bp-28h]@38
    int hires; // [sp+48h] [bp-20h]@2
    int v21; // [sp+4Ch] [bp-1Ch]@2
    int v22; // [sp+50h] [bp-18h]@1
    int scrollrate; // [sp+54h] [bp-14h]@30

    v22 = 0;
    if (!DisplayBit8) {
        hires = Is_HiRes();
        v21 = 0;
        if (field_165D || !mouse_y || !mouse_x || g_seenBuff->Get_Width() - 1 == mouse_x
            || g_seenBuff->Get_Height() - 1 == mouse_y) {
            if (!mouse_y || !mouse_x || g_seenBuff->Get_Width(&SeenBuff) - 1 == mouse_x
                || g_seenBuff->Get_Height() - 1 == mouse_y) {
                v22 = 1;
                a1a = mouse_x;
                if (50 << hires > mouse_x) {
                    a1a -= (50 << hires) - a1a;
                }
                a1a = MAX(a1a, 0);
                if (270 << hires < a1a) {
                    a1a += a1a - (270 << hires);
                }
                a1a = MIN(a1a, 320 << hires);
                if (50 << hires < a1a && 270 << hires > a1a) {
                    a1a += ((160 << hires) - a1a) / 2;
                }
                v16 = mouse_y;
                if (50 << hires > mouse_y) {
                    v16 -= (50 << hires) - v16;
                }
                v16 = MAX(v16, 0);
                if (150 << hires < v16) {
                    v16 += v16 - (150 << hires);
                }
                v16 = MIN(v16, 200 << hires);
                AI::.0 ::direction[0] = Desired_Facing256(160 << hires, 100 << hires, a1a, v16);
            }
            v17 = Direction_To_Facing(AI::.0 ::direction[0]);
            if (Debug_Map) {
                scrollrate = Options.options.ScrollRate + 1;
            } else {
                scrollrate = 8 - field_165D;
            }
            if (Options.options.ScrollRate + 1 > scrollrate) {
                scrollrate = Options.options.ScrollRate + 1;
                field_165D = 8 - (Options.options.ScrollRate + 1);
            }
            if (KeyboardClass::Down(Keyboard, WWKN_RMOUSE)) {
                scrollrate = Bound(scrollrate + 1, 4, 8);
            }
            if (!(Options.options.Bitfield & 0x10)) {
                v4 = Direction_To_Facing(AI::.0 ::direction[0]);
                AI::.0 ::direction[0] = Facing_To_Dir(v4);
            }
            ratetouse = AI::.6 ::_rate[scrollrate] / 2;
            if (Scroll_Map(AI::.0 ::direction[0], &ratetouse, 0)) {
                Override_Mouse_Shape((v17 + 1), 0);
                if (KeyboardClass::Down(Keyboard, WWKN_LMOUSE) || Autoscroll) {
                    ratetouse = AI::.6 ::_rate[scrollrate];
                    if (Debug_Map) {
                        Scroll_Map(AI::.0 ::direction[0], &ratetouse, 1);
                        Counter = 1;
                    } else {
                        ratetouse = AI::.6 ::_rate[scrollrate];
                        Scroll_Map(AI::.0 ::direction[0], &ratetouse, 1);
                        if (!Counter.Time() && v22) {
                            Counter = 1;
                            ++field_165D;
                        }
                    }
                }
            } else {
                Override_Mouse_Shape((v17 + 9), 0);
            }
        }
        if (!Debug_Map && !v22 && !Counter.Time()) {
            if (--field_165D < 0) {
                ++field_165D;
            }
            Counter = 1;
        }
    }
    HelpClass::AI(key, mouse_x, mouse_y);
#endif
}

BOOL ScrollClass::Set_Autoscroll(int a1)
{
    BOOL old = Autoscroll;

    if (a1 == -1) {
        Autoscroll = !Autoscroll;
    } else {
        Autoscroll = a1;
    }
    return old;
}