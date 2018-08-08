/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 * @author tomsons26
 *
 * @brief Class holding various special options.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
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
        DemoMode = false;
        CaptureTheFlag = false;
        NoDamage = false;
        ThreePointTurnLogic = false;
        OreGrows = true;
        OreSpreads = true;
        Spawned = false;
        Remixes = false;
    }

    BOOL Allow_Remixes() { return Remixes; }

#ifndef CHRONOSHIFT_STANDALONE
    static void Hook_Me();
#endif

private:
#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool ShroudRegrows : 1; // & 1 Used in skirmish menu
            bool BuildTimeAccelerated : 1; // & 2 Used in special dialog
            bool DemoMode : 1; // & 4 activated by 0xD95C68A2 command shows the standby screen, the intro, then to allied 1.
            bool CaptureTheFlag : 1; // & 8 Used in lan dialog, also checked in bridge destruction related code.
            bool NoDamage : 1; // & 16 Used by Explosion_Damage code.
            bool ThreePointTurnLogic : 1; // & 32 Used in hidden special dialog.
            bool OreGrows : 1; // & 64 Set in skirmish menu
            bool OreSpreads : 1; // & 128 Set in skirmish menu
            bool Spawned : 1; // & 1 "About to initialise Winsock" string RA demo, ow only skips intro and fades in the menu.
            bool Remixes : 1; // & 2 Enable remix versions of audio tracks that have them. From C&C/Sole, absent from RA.
        };
        int SpecialFlags;
    };
#else
    bool ShroudRegrows;
    bool BuildTimeAccelerated;
    bool FirstRun;
    bool CaptureTheFlag;
    bool NoDamage;
    bool ThreePointTurnLogic;
    bool OreGrows;
    bool OreSpreads;
    bool Spawned;
    bool Remixes;
#endif
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
extern SpecialClass &Special;

inline void SpecialClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x00550DE0, *SpecialClass::Init);
#endif
}
#else
extern SpecialClass Special;
#endif

#endif // SPECIAL_H
