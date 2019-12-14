/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class for holding customisable settings for each type of object mission.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef MISSIONCONTROL_H
#define MISSIONCONTROL_H

#include "always.h"
#include "gameini.h"
#include "gametypes.h"

class MissionControlClass
{
    friend class MissionClass;
    friend class FootClass;

public:
    MissionControlClass();
    ~MissionControlClass() {}

    const char *Name() const;
    BOOL Read_INI(GameINIClass &ini);
    void Write_INI(GameINIClass &ini);

private:
    MissionType m_Mission; // The mission we control.
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_NoThreat : 1;
    BOOL m_Zombie : 1;
    BOOL m_Recruitable : 1;
    BOOL m_Paralyzed : 1;
    BOOL m_Retaliate : 1;
    BOOL m_Scatter : 1;
#else
    bool m_NoThreat; // Is its weapons disabled and thus ignored as a potential target until fired upon?
    bool m_Zombie; // Is forced to sit there like a zombie and never recovers?
    bool m_Recruitable; // Can it be recruited into a team or base defense?
    bool m_Paralyzed;    // Is the object frozen in place but can still fire and function?
    bool m_Retaliate; // Is allowed to retaliate while on this mission?
    bool m_Scatter; // Is allowed to scatter from threats?
#endif
    fixed_t m_Rate; // Delay between normal processing (larger = faster game, less responsiveness).
    fixed_t m_AARate; // Anti-aircraft delay rate (if not specifed it uses regular rate).

#ifdef GAME_DLL
    static MissionControlClass *const s_MissionControl;
#else
    static MissionControlClass s_MissionControl[MISSION_COUNT];
#endif
};

#endif // MISSIONCONTROL_H
