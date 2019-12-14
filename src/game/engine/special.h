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
        m_ShroudRegrows = false;
        m_BuildTimeAccelerated = false;
        m_FirstRun = false;
        m_CaptureTheFlag = false;
        m_NoDamage = false;
        m_ThreePointTurnLogic = false;
        m_OreGrows = true;
        m_OreSpreads = true;
        m_Spawned = false;
        m_Remixes = false;
        m_ShowHealth = false;
    }

    BOOL Allow_Remixes() const { return m_Remixes; }
    BOOL Is_Spawned() const { return m_Spawned; }
    BOOL Is_First_Run() const { return m_FirstRun; }
    void Set_First_Run(BOOL first_run) { m_FirstRun = first_run; }
    BOOL Always_Show_Health() { return m_ShowHealth; }

    uint32_t Pack() const { return m_SpecialFlags; }

private:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required so we fetch the data easily with SpecialFlags.
    union
    {
        struct
        {
            BOOL m_ShroudRegrows : 1; // & 1
            BOOL m_BuildTimeAccelerated : 1; // & 2
            BOOL m_FirstRun : 1; // & 4
            //BOOL m_DemoMode : 1; // & 4
            BOOL m_CaptureTheFlag : 1; // & 8
            BOOL m_NoDamage : 1; // & 16
            BOOL m_ThreePointTurnLogic : 1; // & 32
            BOOL m_OreGrows : 1; // & 64
            BOOL m_OreSpreads : 1; // & 128

            BOOL m_Spawned : 1; // & 1
            BOOL m_Remixes : 1; // & 2
            BOOL m_ShowHealth : 1; // & 4

            //m_Bit3_1 is debug path finding is C&C Dos.
        };
        int m_SpecialFlags;
    };
#else
    bool m_ShroudRegrows; // Used in skirmish menu
    bool m_BuildTimeAccelerated; //  Used in special dialog
    bool m_FirstRun; // Play intro followed by allied 1 or soviet 1.
    // bool m_DemoMode; // activated by 0xD95C68A2 command shows the standby screen, the intro, then to allied 1 or soviet 1.
    bool m_CaptureTheFlag; // Used in lan dialog, also checked in bridge destruction related code.
    bool m_NoDamage; // Used by Explosion_Damage code.
    bool m_ThreePointTurnLogic; // Used in hidden special dialog.
    bool m_OreGrows; // Set in skirmish menu
    bool m_OreSpreads; // Set in skirmish menu

    // Chronoshift additions
    bool m_Spawned; // "About to initialise Winsock" string RA demo, now only skips intro and fades in the menu. For Wchat?
    bool m_Remixes; // Enable remix versions of audio tracks that have them. From C&C/Sole, absent from RA.
    bool m_ShowHealth; // Always show health bars
#endif
};

#ifdef GAME_DLL
extern SpecialClass &s_Special;
#else
extern SpecialClass s_Special;
#endif

#endif // SPECIAL_H
