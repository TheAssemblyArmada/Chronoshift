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

public:
    MissionControlClass();
    ~MissionControlClass() {}

    const char *Name() const;
    BOOL Read_INI(GameINIClass &ini);
    void Write_INI(GameINIClass &ini);

private:
    MissionType Mission; // The mission we control (def = MISSION_NONE).
#ifndef CHRONOSHIFT_NO_BITFIELDS
                         // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool NoThreat : 1; // & 1 Is its weapons disabled and thus ignored as a potential target until fired upon (def = false)?
            bool Zombie : 1; // & 2 Is forced to sit there like a zombie and never recovers (def = false)?
            bool Recruitable : 1; // & 4 Can it be recruited into a team or base defense (def = true)?
            bool Paralyzed : 1;    // & 8 Is the object frozen in place but can still fire and function (def = false)?
            bool Retaliate : 1; // & 16 Is allowed to retaliate while on this mission (def = true)?
            bool Scatter : 1; // & 32 Is allowed to scatter from threats (def = true)?
        };
        int Bitfield;
    };
#else
    bool NoThreat; // Is its weapons disabled and thus ignored as a potential target until fired upon (def = false)?
    bool Zombie; // Is forced to sit there like a zombie and never recovers (def = false)?
    bool Recruitable; // Can it be recruited into a team or base defense (def = true)?
    bool Paralyzed;    // Is the object frozen in place but can still fire and function (def = false)?
    bool Retaliate; // Is allowed to retaliate while on this mission (def = true)?
    bool Scatter; // Is allowed to scatter from threats (def = true)?
#endif
    fixed_t Rate; // Delay between normal processing (larger = faster game, less responsiveness) (def = 0.016).
    fixed_t AARate; // Anti-aircraft delay rate (if not specifed it uses regular rate) (def = 0.016).

#ifndef CHRONOSHIFT_STANDALONE
    static MissionControlClass *const MissionControl;
#else
    static MissionControlClass MissionControl[MISSION_COUNT];
#endif
};

#endif // MISSIONCONTROL_H
