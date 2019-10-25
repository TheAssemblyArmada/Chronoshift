/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 * @author tomsons26
 *
 * @brief Class holding various special options.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef SPECIAL_H
#define SPECIAL_H

#include "always.h"

class SpecialClass
{
public:
    void Init()
    {
        ShroudRegrows = false;
        BuildTimeAccelerated = false;
        FirstRun = false;
        CaptureTheFlag = false;
        NoDamage = false;
        ThreePointTurnLogic = false;
        OreGrows = true;
        OreSpreads = true;
        Spawned = false;
        Remixes = false;
        ShowHealth = false;
    }

    BOOL Allow_Remixes() const { return Remixes; }
    BOOL Is_Spawned() const { return Spawned; }
    BOOL Is_First_Run() const { return FirstRun; }
    void Set_First_Run(BOOL first_run) { FirstRun = first_run; }
    BOOL Always_Show_Health() { return ShowHealth; }

    uint32_t Pack() const { return SpecialFlags; }

private:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required so we fetch the data easily with SpecialFlags.
    union
    {
        struct
        {
            BOOL ShroudRegrows : 1; // & 1
            BOOL BuildTimeAccelerated : 1; // & 2
            BOOL FirstRun : 1; // & 4
            //BOOL DemoMode : 1; // & 4
            BOOL CaptureTheFlag : 1; // & 8
            BOOL NoDamage : 1; // & 16
            BOOL ThreePointTurnLogic : 1; // & 32
            BOOL OreGrows : 1; // & 64
            BOOL OreSpreads : 1; // & 128

            BOOL Spawned : 1; // & 1
            BOOL Remixes : 1; // & 2
            BOOL ShowHealth : 1; // & 4

            //Bit3_1 is debug path finding is C&C Dos.
        };
        int SpecialFlags;
    };
#else
    bool ShroudRegrows; // Used in skirmish menu
    bool BuildTimeAccelerated; //  Used in special dialog
    bool FirstRun; // Play intro followed by allied 1 or soviet 1.
    // bool DemoMode; // activated by 0xD95C68A2 command shows the standby screen, the intro, then to allied 1 or soviet 1.
    bool CaptureTheFlag; // Used in lan dialog, also checked in bridge destruction related code.
    bool NoDamage; // Used by Explosion_Damage code.
    bool ThreePointTurnLogic; // Used in hidden special dialog.
    bool OreGrows; // Set in skirmish menu
    bool OreSpreads; // Set in skirmish menu

    // Chronoshift additions
    bool Spawned; // "About to initialise Winsock" string RA demo, now only skips intro and fades in the menu. For Wchat?
    bool Remixes; // Enable remix versions of audio tracks that have them. From C&C/Sole, absent from RA.
    bool ShowHealth; // Always show health bars
#endif
};

#ifdef GAME_DLL
extern SpecialClass &Special;
#else
extern SpecialClass Special;
#endif

#endif // SPECIAL_H
