/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling power bar.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "power.h"
#include "ccfileclass.h"
#include "coord.h"
#include "drawshape.h"
#include "iomap.h"
#include "lists.h"
#include "mixfile.h"

#define POWER_MAX_HEIGHT 108

PowerClass::PowerButtonClass &PowerClass::PowerButton = Make_Global<PowerClass::PowerButtonClass>(0x006877C0);
void *&PowerClass::PowerShape = Make_Global<void *>(0x006877B8);
void *&PowerClass::PowerBarShape = Make_Global<void *>(0x006877BC);

PowerClass::PowerButtonClass::PowerButtonClass() :
    GadgetClass(0, 0, 0, 0, MOUSE_LEFT_PRESS | MOUSE_LEFT_HELD | MOUSE_LEFT_RLSE | MOUSE_LEFT_UP | MOUSE_RIGHT_PRESS, true)
{
}

BOOL PowerClass::PowerButtonClass::Action(unsigned flags, KeyNumType &key)
{
    // TODO, needs HouseClass, TabClass, HelpClass.
#ifndef RAPP_STANDALONE
    BOOL(*func)
    (const PowerButtonClass *, unsigned, KeyNumType &) =
        reinterpret_cast<BOOL (*)(const PowerButtonClass *, unsigned, KeyNumType &)>(0x00527F54);
    return func(this, flags, key);
#else
    return false;
#endif
}

PowerClass::PowerClass(void) :
    RadarClass(),
    FlashTimer(),
    Drain(-1),
    Output(-1),
    NewDrainHeight(0),
    NewOutputHeight(0),
    DrainHeight(0),
    OutputHeight(0),
    DrainMod(0),
    OutputMod(0),
    OutputInc(0),
    DrainInc(0)
{
    return;
}

void PowerClass::One_Time(void)
{
    RadarClass::One_Time();
    PowerShape = MixFileClass<CCFileClass>::Retrieve("power.shp");
    DEBUG_ASSERT(PowerShape != nullptr);

    PowerBarShape = MixFileClass<CCFileClass>::Retrieve("powerbar.shp");
    DEBUG_ASSERT(PowerBarShape != nullptr);

    PowerButton.Set_Position(480, 180);
    PowerButton.Set_Size(15, 220);
}

void PowerClass::Init_Clear(void)
{
    RadarClass::Init_Clear();

    Drain = -1;
    Output = -1;
    NewDrainHeight = 0;
    NewOutputHeight = 0;
    DrainHeight = 0;
    OutputHeight = 0;
    OutputMod = 0;
    DrainMod = 0;
    DrainInc = 0;

    FlashTimer.Reset();
}

void PowerClass::AI(KeyNumType &key, int mouse_x, int mouse_y)
{
    //Needs HouseClass
#ifndef RAPP_STANDALONE
    void (*func)(const PowerClass *, KeyNumType &, int, int) =
        reinterpret_cast<void (*)(const PowerClass *, KeyNumType &, int, int)>(0x00527C60);
    func(this, key, mouse_x, mouse_y);
#endif
    /*
    if (Map.Is_Sidebar_Drawn()) {
        int prev_drain = DrainHeight;
        int prev_output = OutputHeight;

        // Work out if output has changed and needs updating.
        if (PlayerPtr->Power != Output) {
            NewOutputHeight = Power_Height(PlayerPtr->Power);
            Output = PlayerPtr->Power;

            if (NewOutputHeight == OutputHeight) {
                OutputMod = 0;
            } else {
                OutputMod = 12;
                OutputInc = NewOutputHeight < OutputHeight ? -1 : 1;
            }
        }

        // Work out if drain has changed and needs updating.
        if (PlayerPtr->Drain != Drain) {
            NewDrainHeight = Power_Height(PlayerPtr->Drain);
            Drain = PlayerPtr->Drain;

            if (NewDrainHeight == DrainHeight) {
                DrainMod = 0;
            } else {
                DrainMod = 12;
                DrainInc = NewDrainHeight < DrainHeight ? -1 : 1;
            }
        }

        // Wobble when settling at new drain level after gaining/loosing some power draining structure.
        if (DrainMod > 0 && DrainHeight == NewDrainHeight) {
            PowerToRedraw = true;
            Flag_To_Redraw();
            --DrainMod;
        } else if (DrainHeight != NewDrainHeight) {
            DrainHeight += DrainInc;
        }

        // Wobble when settling at new output level after gaining/loosing some power generating structure.
        if (OutputMod > 0 && OutputHeight == NewOutputHeight) {
            PowerToRedraw = true;
            Flag_To_Redraw();
            --OutputMod;
        } else if (OutputHeight != NewOutputHeight) {
            OutputHeight += OutputInc;
        }

        if (prev_drain != DrainHeight || prev_output != OutputHeight) {
            PowerToRedraw = true;
            Flag_To_Redraw();
        }

        if (!FlashTimer.Has_Expired()) {
            PowerToRedraw = true;
            Flag_To_Redraw();
        }
    }

    RadarClass::AI(key, mouse_x, mouse_y);
    */
}

void PowerClass::Draw_It(BOOL force_redraw)
{
    //Needs HouseClass
#ifndef RAPP_STANDALONE
    void (*func)(const PowerClass *, BOOL) = reinterpret_cast<void (*)(const PowerClass *, BOOL)>(0x0052762C);
    func(this, force_redraw);
#endif
    /*
    static int _modtable[] = { 0, -1, 0, 1, 0, -1, -2, -1, 0, 1, 2, 1, 0 };

    if ((PowerToRedraw || force_redraw) && Map.Is_Sidebar_Drawn()) {
        PowerToRedraw = false;

        ShapeFlags flags = SHAPE_NORMAL;

        void *fadingtable = nullptr;

        int output_height = 0;
        int drain_height = 0;

        // If timer is set, flash the bar red?
        if (!FlashTimer.Has_Expired() && FlashTimer.Time() > 1) {
            // If remaining time mod 3 is odd?
            if ((FlashTimer.Time() % 3) & 1) {
                flags |= SHAPE_FADING;
                fadingtable = DisplayClass::FadingRed;
            }
        }

        // Draw the power bar background shapes
        CC_Draw_Shape(PowerBarShape, 0, PowerButton.Get_XPos(), 176, WINDOW_0, flags | SHAPE_WIN_REL, fadingtable);
        CC_Draw_Shape(PowerBarShape, 1, PowerButton.Get_XPos(), 288, WINDOW_0, flags | SHAPE_WIN_REL, fadingtable);

        if (OutputHeight == NewOutputHeight) {
            output_height = OutputHeight + _modtable[OutputMod] * OutputInc;
        } else {
            output_height = OutputHeight;
        }

        if (DrainHeight == NewDrainHeight) {
            drain_height = DrainHeight + _modtable[DrainMod] * DrainInc;
        } else {
            drain_height = DrainHeight;
        }

        output_height = Clamp(output_height, 0, POWER_MAX_HEIGHT);
        drain_height = Clamp(drain_height, 0, POWER_MAX_HEIGHT);

        // If we actually have some power
        if (output_height > 0) {
            uint8_t lcolor = COLOR_LTGREEN;
            uint8_t dcolor = COLOR_GREEN;

            // Slightly low power
            if (PlayerPtr->Power <= PlayerPtr->Drain) {
                lcolor = 211; // lightish orange
                dcolor = 214; // darkish orange
            }

            // Very low power
            if ((PlayerPtr->Power * 2) < PlayerPtr->Drain) {
                lcolor = 230; // lightish red
                dcolor = 235; // darkish red
            }

            output_height = (output_height / 107) * 153;
            drain_height = (drain_height / 107) * 153;

            // Draw the power bar, two similar color lines next to each other
            g_logicPage->Fill_Rect(PowerButton.Get_XPos() + 10, output_height - 351, PowerButton.Get_XPos() + 11, 351, lcolor);
            g_logicPage->Fill_Rect(PowerButton.Get_XPos() + 12, output_height - 351, PowerButton.Get_XPos() + 13, 351, dcolor);
        }

        // Draw the power level indicator.
        CC_Draw_Shape(
            PowerShape, 0, PowerButton.Get_XPos() + 2, (drain_height + 4) - 351, WINDOW_0, flags | SHAPE_WIN_REL);
        PowerToRedraw = false;
    }

    RadarClass::Draw_It(force_redraw);
    */
}

void PowerClass::Refresh_Cells(int16_t cellnum, int16_t *overlap_list)
{
    DEBUG_ASSERT(cellnum < MAP_MAX_AREA);

    if (overlap_list != nullptr) {
        if (*overlap_list == LIST_START) {
            PowerToRedraw = true;
            Flag_To_Redraw();
        }
    }

    RadarClass::Refresh_Cells(cellnum, overlap_list);
}

int PowerClass::Power_Height(int power)
{
    int power_left = power;
    int height_inc = 0;
    int height = 0;

    for (int i = 0; i <= power / 100; ++i) {
        height_inc = (POWER_MAX_HEIGHT - height) / 5;
        power_left -= 100;
        height += height_inc;
    }

    if (power_left != 0) {
        height += power_left * height_inc / 100;
    }

    return Clamp(height, 0, POWER_MAX_HEIGHT);
}

void PowerClass::Flash_Power(void)
{
    FlashTimer.Reset(15);
    PowerToRedraw = true;
    Flag_To_Redraw();
}
