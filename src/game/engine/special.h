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
    union
    {
        struct
        {
            BOOL m_ShroudRegrows : 1; // & 1 // Used in skirmish menu.
            BOOL m_BuildTimeAccelerated : 1; // & 2 // Used in special dialog
            BOOL m_FirstRun : 1; // & 4 // Play intro followed by allied 1 or soviet 1.
                                        // activated by 0xD95C68A2 command shows the standby screen, the intro, then to allied 1 or soviet 1.
            BOOL m_CaptureTheFlag : 1; // & 8 // is this a CTF game?, Used in lan dialog
            BOOL m_NoDamage : 1; // & 16 // Used by Explosion_Damage code.
            BOOL m_ThreePointTurnLogic : 1; // & 32 // Used in hidden special dialog.
            BOOL m_OreGrows : 1; // & 64 // Set in skirmish menu
            BOOL m_OreSpreads : 1; // & 128 // Set in skirmish menu

            // Chronoshift additions
            BOOL m_Spawned : 1; // & 1 // "About to initialise Winsock" string RA demo, now only skips intro and fades in the menu. For Wchat?
            BOOL m_Remixes : 1; // & 2 // Enable remix versions of audio tracks that have them. From C&C/Sole, absent from RA.
            BOOL m_ShowHealth : 1; // & 4 // Always show health bars

            //m_Bit3_1 is debug path finding is C&C Dos.
        };
        int m_SpecialFlags;
    };
};

#ifdef GAME_DLL
extern SpecialClass &s_Special;
#else
extern SpecialClass s_Special;
#endif

#endif // SPECIAL_H
