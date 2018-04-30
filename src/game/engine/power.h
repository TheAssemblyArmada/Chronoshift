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
#pragma once

#ifndef POWER_H
#define POWER_H

#include "always.h"
#include "radar.h"

class PowerClass : public RadarClass
{
    // Internal gadget class for handling input to the radar area of the game screen.
    class PowerButtonClass : public GadgetClass
    {
    public:
        PowerButtonClass();
        PowerButtonClass(PowerButtonClass &that) : GadgetClass(that) {}
        virtual ~PowerButtonClass() {}

        virtual BOOL Action(unsigned flags, KeyNumType &key) override;
    };

public:
    PowerClass();

    virtual void One_Time() override;
    virtual void Init_Clear() override;
    virtual void AI(KeyNumType &key, int mouse_x, int mouse_y) override;
    virtual void Draw_It(BOOL force_redraw) override;
    virtual void Refresh_Cells(int16_t cellnum, int16_t *overlap_list) override;

    int Power_Height(int power);
    void Flash_Power();


#ifndef RAPP_STANDALONE
    static void Hook_Me();
#endif

protected:
#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            // bitfield 0x100D
            bool PowerToRedraw : 1; // 1
        };
        int Bitfield;
    };
#else
    // bitfield 0x100D
    bool PowerToRedraw; // 1
#endif
    //might not be BTC, but TCountDownTimerClass<FrameTimerClass>
    BasicTimerClass<FrameTimerClass> FlashTimer;		// 0x1011
    int Drain;		// 0x101A
    int Output;		// 0x101E
    int NewDrainHeight;		// 0x1022
    int NewOutputHeight;		// 0x1026
    int DrainHeight;		// 0x102A
    int OutputHeight;		// 0x102E
    int DrainMod;		// 0x1032
    int OutputMod;		// 0x1036
    int16_t OutputInc;	// 0x103A
    int16_t DrainInc;	// 0x103C

#ifndef RAPP_STANDALONE
    static PowerButtonClass &PowerButton;
    static void *&PowerShape;
    static void *&PowerBarShape;
#else
    static PowerButtonClass PowerButton;
    static void *PowerShape;
    static void *PowerBarShape;
#endif
};

#ifndef RAPP_STANALONE
#include "hooker.h"
inline void PowerClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x005275CC, *PowerClass::One_Time); // seems to work
    Hook_Function(0x00527534, *PowerClass::Init_Clear); // seems to work
    Hook_Function(0x00527EA4, *PowerClass::Refresh_Cells); // seems to work
    Hook_Function(0x00527EE4, *PowerClass::Power_Height); // seems to work
    Hook_Function(0x00528010, *PowerClass::Flash_Power); // seems to work
#endif
}
#endif

#endif // POWER_H
