/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Functions for using the action enum.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "action.h"
#include "gamedebug.h"

const char *ActionName[ACTION_COUNT] = { 
    "None",
    "Move",
    "NoMove",
    "Enter",
    "Self",
    "Attack",
    "Harvest",
    "Select",
    "ToggleSelect",
    "Capture",
    "Repair",
    "Sell",
    "SellUnit",
    "NoSell",
    "NoRepair",
    "Sabotage",
    "ParaBomb",
    "ParaSaboteur",
    "IonCannon",
    "Nuke",
    "Airstrike",
    "Chronosphere",
    "ChronoWarp",
    "IronCurtain",
    "SpyMission",
    "GuardArea",
    "Heal",
    "Damage",
    "GRepair",
    "NoDeploy",
    "NoEnter",
    "NoGRepair" };

ActionType Action_From_Name(const char *name)
{
    if (strcasecmp(name, "<none>") == 0 || strcasecmp(name, "none") == 0) {
        return ACTION_NONE;
    }

    if (name != nullptr) {
        // Try to find a action with this name in the list.
        for (ActionType action = ACTION_NONE; action < ACTION_COUNT; ++action) {
            if (strcasecmp(name, ActionName[action]) == 0) {
                return action;
            }
        }
    }

    // The input name was NULL, or we failed to find it within the list.
    // Return ACTION_NONE to represent this.
    return ACTION_NONE;
}

const char *Name_From_Action(ActionType action)
{
    DEBUG_ASSERT(action < ACTION_COUNT);

    if (action >= ACTION_NONE && action < ACTION_COUNT) {
        return ActionName[action];
    }

    return ActionName[ACTION_NONE];
}
