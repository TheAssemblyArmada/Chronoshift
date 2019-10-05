/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Part of IOMap stack handling power bar.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef POWER_H
#define POWER_H

#include "always.h"
#include "radar.h"

class PowerClass : public RadarClass
{
    // Internal gadget class for handling input to the powerbar area of the sidebar.
    class PowerButtonClass : public GadgetClass
    {
    public:
        PowerButtonClass();
        PowerButtonClass(PowerButtonClass &that) : GadgetClass(that) {}
        virtual ~PowerButtonClass() {}

        virtual BOOL Action(unsigned flags, KeyNumType &key) override;
    };

public:
    enum {
        POWER_MAX_HEIGHT = 108
    };

public:
    PowerClass();
    PowerClass(const NoInitClass &noinit) : RadarClass(noinit), FlashTimer(noinit) {}

    virtual void One_Time() override;
    virtual void Init_Clear() override;
    virtual void AI(KeyNumType &key, int mouse_x, int mouse_y) override;
    virtual void Draw_It(BOOL force_redraw) override;
    virtual void Refresh_Cells(cell_t cellnum, const int16_t *list) override;

    int Power_Height(int power);
    void Flash_Power();

protected:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL PowerToRedraw : 1; // 1
    BOOL PowerBit2 : 1; // 2
#else
    bool PowerToRedraw;
    bool PowerBit2;
#endif
    TCountDownTimerClass<FrameTimerClass> FlashTimer;
    int Drain;
    int Output;
    int NewDrainHeight;
    int NewOutputHeight;
    int DrainHeight;
    int OutputHeight;
    int DrainMod;
    int OutputMod;
    int16_t OutputInc;
    int16_t DrainInc;

#ifdef GAME_DLL
    static PowerButtonClass &PowerButton;
    static void *&PowerShape;
    static void *&PowerBarShape;
#else
    static PowerButtonClass PowerButton;
    static void *PowerShape;
    static void *PowerBarShape;
#endif
};

#endif // POWER_H
