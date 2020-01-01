/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Just a function to detach a target from everything else in game.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "tracker.h"
#include "aircraft.h"
#include "anim.h"
#include "building.h"
#include "bullet.h"
#include "house.h"
#include "infantry.h"
#include "iomap.h"
#include "logic.h"
#include "team.h"
#include "teamtype.h"
#include "trigger.h"
#include "triggertype.h"
#include "unit.h"
#include "vessel.h"
#include "vortex.h"

void Detach_This_From_All(target_t target, BOOL a2)
{
    if (a2) {
        for (int i = 0; i < g_Houses.Count(); ++i) {
            g_Houses[i].Detach(target, a2);
        }
        for (int i = 0; i < g_Teams.Count(); ++i) {
            g_Teams[i].Detach(target, a2);
        }
        for (int i = 0; i < g_TeamTypes.Count(); ++i) {
            g_TeamTypes[i].Detach(target, a2);
        }
        for (int i = 0; i < g_Units.Count(); ++i) {
            g_Units[i].Detach(target, a2);
        }
        for (int i = 0; i < g_Vessels.Count(); ++i) {
            g_Vessels[i].Detach(target, a2);
        }
        for (int i = 0; i < g_Aircraft.Count(); ++i) {
            g_Aircraft[i].Detach(target, a2);
        }
        for (int i = 0; i < g_Buildings.Count(); ++i) {
            g_Buildings[i].Detach(target, a2);
        }
        for (int i = 0; i < g_Bullets.Count(); ++i) {
            g_Bullets[i].Detach(target, a2);
        }
        for (int i = 0; i < g_Infantry.Count(); ++i) {
            g_Infantry[i].Detach(target, a2);
        }
        for (int i = 0; i < g_Anims.Count(); ++i) {
            g_Anims[i].Detach(target, a2);
        }
        g_Map.Detach(target, a2);
        g_Logic.Detach(target, a2);
        g_ChronalVortex.Detach(target);
        if (As_TriggerType(target) != nullptr) {
            for (int i = 0; i < g_Triggers.Count(); ++i) {
                TriggerClass *trig = &g_Triggers[i];
                if (trig->As_Target() == target) {
                    Detach_This_From_All(trig->As_Target(), a2);
                    delete trig;
                }
                g_Triggers[i].Detach(target, a2);
            }
        }
        for (int i = 0; i < g_Triggers.Count(); ++i) {
            g_Triggers[i].Detach(target, a2);
        }
        for (int i = 0; i < g_TriggerTypes.Count(); ++i) {
            g_TriggerTypes[i].Detach(target, a2);
        }
    }
}
