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
    PowerClass(const NoInitClass &noinit) : RadarClass(noinit), m_FlashTimer(noinit) {}

    virtual void One_Time() override;
    virtual void Init_Clear() override;
    virtual void AI(KeyNumType &key, int mouse_x, int mouse_y) override;
    virtual void Draw_It(BOOL force_redraw) override;
    virtual void Refresh_Cells(cell_t cellnum, const int16_t *list) override;

    int Power_Height(int power);
    void Flash_Power();

    void Flag_Power_To_Redraw() { m_PowerToRedraw = true; }

protected:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_PowerToRedraw : 1; // 1
    BOOL m_PowerBit2 : 1; // 2
#else
    bool m_PowerToRedraw;
    bool m_PowerBit2;
#endif
    TCountDownTimerClass<FrameTimerClass> m_FlashTimer;
    int m_Drain;
    int m_Output;
    int m_NewDrainHeight;
    int m_NewOutputHeight;
    int m_DrainHeight;
    int m_OutputHeight;
    int m_DrainMod;
    int m_OutputMod;
    int16_t m_OutputInc;
    int16_t m_DrainInc;

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
