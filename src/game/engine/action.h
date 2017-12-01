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
#pragma once

#ifndef ACTION_H
#define ACTION_H

#include "always.h"

enum ActionType
{
    ACTION_NONE,
    ACTION_MOVE,
    ACTION_NO_MOVE,
    ACTION_ENTER,
    ACTION_SELF,
    ACTION_ATTACK,
    ACTION_HARVEST,
    ACTION_SELECT,
    ACTION_TOGGLE_SELECT,
    ACTION_CAPTURE,
    ACTION_REPAIR,
    ACTION_SELL,
    ACTION_SELL_UNIT,
    ACTION_NO_SELL,
    ACTION_NO_REPAIR,
    ACTION_SABOTAGE,
    ACTION_PARA_BOMB,
    ACTION_PARA_SABOTEUR,
    ACTION_ION_CANNON,
    ACTION_NUKE,
    ACTION_AIR_STRIKE,
    ACTION_CHRONOSPHERE, //WarpIn 
    ACTION_CHRONOWARP, //WarpOut
    ACTION_IRON_CURTAIN,
    ACTION_SPY_MISSION,
    ACTION_GUARD_AREA5,
    ACTION_HEAL,
    ACTION_DAMAGE,
    ACTION_GREPAIR8,
    ACTION_NO_DEPLOY,
    ACTION_NO_ENTER,
    ACTION_NO_GREPAIR,
    ACTION_COUNT,
};

DEFINE_ENUMERATION_OPERATORS(ActionType);

ActionType Action_From_Name(const char *name);
const char *Name_From_Action(ActionType action);

#endif // _TEMPLATE_HH_
