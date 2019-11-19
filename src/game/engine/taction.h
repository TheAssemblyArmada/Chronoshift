/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Classes holding action information for the trigger system.
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
#include "gametypes.h"

class ObjectClass;
class TeamTypeClass;
class TriggerTypeClass;

enum TActionType
{
    TACTION_FIRST = 0,
    TACTION_NO_ACTION = 0, // "-No Action-"
    TACTION_WINNER_IS, // "Winner is..."
    TACTION_LOSER_IS, // "Loser is..."
    TACTION_PROD_BEGIN, // "Production Begins"
    TACTION_CREATE_TEAM, // "Create Team..."
    TACTION_DESTROY_ALL_TEAMS, // "Destroy All Teams"
    TACTION_ALL_TO_HUNT, // "All to Hunt..."
    TACTION_REINFORCE_TEAM, // "Reinforcement (team)..."
    TACTION_DROP_FLARE, // "Drop Zone Flare (waypoint)..."
    TACTION_FIRE_SALE, // "Fire Sale..."
    TACTION_PLAY_MOVIE, // "Play Movie..."
    TACTION_DISPLAY_TEXT, // "Text Trigger (ID num)..."
    TACTION_DESTROY_TRIGGER, // "Destroy Trigger..."
    TACTION_ENABLE_AUTOCREATE, // "Autocreate Begins..."
    TACTION_CHANGE_HOUSE, // "Change House..."
    TACTION_ALLOW_WIN, // "Allow Win"
    TACTION_REVEAL_MAP, // "Reveal all map"
    TACTION_REVEAL_WAYPOINT, // "Reveal around waypoint..."
    TACTION_REVEAL_WAYPOINT_ZONE, // "Reveal zone of waypoint..."
    TACTION_PLAY_SOUND_EFFECT, // "Play sound effect..."
    TACTION_PLAY_MUSIC_THEME, // "Play music theme..."
    TACTION_PLAY_SPEECH, // "Play speech..."
    TACTION_FORCE_TRIGGER, // "Force Trigger..."
    TACTION_TIMER_START, // "Timer Start"
    TACTION_TIMER_STOP, // "Timer Stop"
    TACTION_TIMER_EXTEND, // "Timer Extend (1/10th min)..."
    TACTION_TIMER_REDUCE, // "Timer Shorten (1/10th min)..."
    TACTION_TIMER_SET, // "Timer Set (1/10th min)..."
    TACTION_GLOBAL_SET, // "Global Set..."
    TACTION_GLOBAL_CLEAR, // "Global Clear..."
    TACTION_AUTO_BASE_AI, // "Auto Base Building..."
    TACTION_GROW_SHROUD, // "Grow shroud one 'step'"
    TACTION_DESTROY_ATTACHED, // "Destroy attached building"
    TACTION_ONE_TIME_SUPER, // "Add 1-time special weapon..."
    TACTION_REPEATING_SUPER, // "Add repeating special weapon..."
    TACTION_PREFERRED_TARGET, // "Preferred target..."
    TACTION_LAUNCH_NUKES, // "Launch Nukes"
    TACTION_COUNT
};

DEFINE_ENUMERATION_OPERATORS(TActionType);

class TActionClass
{
    friend class TriggerTypeClass;

public:
    TActionClass() : m_Type(TACTION_NO_ACTION), m_TeamType(nullptr), m_TriggerType(nullptr), m_IntegerValue(0) {}
    ~TActionClass()
    {
        m_TeamType = nullptr;
        m_TriggerType = nullptr;
    }

    BOOL operator()(HousesType house, ObjectClass *object, int trigger_id, cell_t cell_num);

    void Detach(target_t target);
    void Build_INI_Entry(char *entry_buffer) const;
    void Read_INI();

    void Code_Pointers() {}
    void Decode_Pointers() {}

    TActionType What_Type() const { return m_Type; }

    static TActionType Action_From_Name(const char *name);
    static const char *Name_From_Action(TActionType taction);
    static NeedType Action_Needs(TActionType taction);

protected:
    TActionType m_Type;
    GamePtr<TeamTypeClass> m_TeamType;
    GamePtr<TriggerTypeClass> m_TriggerType;
    int32_t m_IntegerValue;

private:
    struct ActionTextStruct
    {
        char const *m_Name;
        char const *Description;
    };

    static ActionTextStruct s_ActionText[TACTION_COUNT];
};

#endif // TACTION_H
