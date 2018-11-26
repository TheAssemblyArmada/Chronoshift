/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief 
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef TACTION_H
#define TACTION_H

#include "always.h"
#include "gameptr.h"

class TeamTypeClass;
class TriggerTypeClass;

 ////////////////////////////////////////////////////////////////////////////////
 //  Enumerations
 ////////////////////////////////////////////////////////////////////////////////
enum TActionType
{
    TACTION_FIRST = 0,
    TACTION_NO_ACTION = 0, // "-No Action-"
    TACTION_WINNER_IS, // "Winner is..."
    TACTION_LOSER_IS, // "Loser is..."
    TACTION_3, // "Production Begins"
    TACTION_CREATE_TEAM, // "Create Team..."
    TACTION_5, // "Destroy All Teams"
    TACTION_ALL_TO_HUNT, // "All to Hunt..."
    TACTION_7, // "Reinforcement (team)..."
    TACTION_8, // "Drop Zone Flare (waypoint)..."
    TACTION_FIRE_SALE, // "Fire Sale..."
    TACTION_PLAY_MOVIE, // "Play Movie..."
    TACTION_11, // "Text Trigger (ID num)..."
    TACTION_12, // "Destroy Trigger..."
    TACTION_13, // "Autocreate Begins..."
    TACTION_CHANGE_HOUSE, // "Change House..."
    TACTION_ALLOW_WIN, // "Allow Win"
    TACTION_REVEAL_MAP, // "Reveal all map"
    TACTION_REVEAL_WAYPOINT, // "Reveal around waypoint..."
    TACTION_REVEAL_WAYPOINT_ZONE, // "Reveal zone of waypoint..."
    TACTION_PLAY_SOUND_EFFECT, // "Play sound effect..."
    TACTION_PLAY_MUSIC_THEME, // "Play music theme..."
    TACTION_PLAY_SPEECH, // "Play speech..."
    TACTION_22, // "Force Trigger..."
    TACTION_TIMER_START, // "Timer Start"
    TACTION_TIMER_STOP, // "Timer Stop"
    TACTION_25, // "Timer Extend (1/10th min)..."
    TACTION_26, // "Timer Shorten (1/10th min)..."
    TACTION_27, // "Timer Set (1/10th min)..."
    TACTION_GLOBAL_SET, // "Global Set..."
    TACTION_GLOBAL_CLEAR, // "Global Clear..."
    TACTION_30, // "Auto Base Building..."
    TACTION_GROW_SHROUD, // "Grow shroud one 'step'"
    TACTION_32, // "Destroy attached building"
    TACTION_33, // "Add 1-time special weapon..."
    TACTION_34, // "Add repeating special weapon..."
    TACTION_35, // "Preferred target..."
    TACTION_LAUNCH_NUKES, // "Launch Nukes"
    TACTION_COUNT
};

DEFINE_ENUMERATION_OPERATORS(TActionType);

class TActionClass
{
public:
    TActionClass() : m_Type(TACTION_NO_ACTION), m_TeamType(nullptr), m_TriggerType(nullptr), m_Value3(0) {}
    ~TActionClass() { m_TeamType = nullptr; m_TriggerType = nullptr; }

    void Code_Pointers() {}
    void Decode_Pointers() {}

    TActionType What_Type() const { return m_Type; }

protected:
    TActionType m_Type;
    GamePtr<TeamTypeClass> m_TeamType;
    GamePtr<TriggerTypeClass> m_TriggerType;
    int m_Value3; // 
};

#endif // TACTION_H
