/**
 * @file
 *
 * @author CCHyper
 *
 * @brief Vox enum and conversion to and from string.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef VOX_H
#define VOX_H

#include "always.h"

// VOX_ALLIES_S
// 5_SOVIET_REINFORCEMENTS

enum VoxType
{
    VOX_MISSION_WON,
    VOX_MISSION_LOST,
    VOX_NO_COMPLY_BUILDING_IN_PROGRESS, // rename?
    VOX_CONSTRUCTION_COMPLETE,
    VOX_UNIT_READY,
    VOX_NEW_OPTIONS,
    VOX_NO_DEPLOY,
    VOX_STRUCTURE_DESTROYED,
    VOX_NO_POWER,
    VOX_NO_FUNDS,
    VOX_BATTLE_CONTROL_TERMINATED,
    VOX_REINFORCEMENTS_ARRIVED,
    VOX_CANCELLED,
    VOX_BUILDING,
    VOX_LOW_POWER,
    VOX_NOFUNDS1_DUP, // duplicate of 9
    VOX_BASER_UNDER_ATTACK,
    VOX_UNABLE_TO_BUILD,
    VOX_PRIMARY_BUILDING,
    VOX_MAD_TANK_DEPLOYED,
    VOX_SPEECH_NULL1, // NULL
    VOX_UNIT_LOST,
    VOX_SELECT_TARGET,
    VOX_ENMYAPP1,
    VOX_SILOS_NEEDED,
    VOX_ON_HOLD,
    VOX_REPAIRING,
    VOX_SPEECH_NULL2, // NULL
    VOX_SPEECH_NULL3, // NULL
    VOX_AUNITL1,
    VOX_SPEECH_NULL4, // NULL
    VOX_AAPPRO1,
    VOX_AARRIVE1,
    VOX_SPEECH_NULL5, // NULL
    VOX_SPEECH_NULL6, // NULL
    VOX_BUILDING_INFILTRATED,
    VOX_CHROCHR1,
    VOX_CHRORDY1,
    VOX_CHROYES1,
    VOX_CMDCNTR1,
    VOX_CNTLDED1,
    VOX_CONVYAP1,
    VOX_CONVLST1,
    VOX_EXPLOSIVE_PLACED,
    VOX_CREDITS_STOLEN,
    VOX_NAVAL_LOST,
    VOX_SAT_LAUNCHED,
    VOX_SONAR_PULSE_AVAILABLE,
    VOX_SPEECH_NULL7, // NULL
    VOX_SOVIET_APPROACHING,
    VOX_SOVIET_REINFORCEMENTS,
    VOX_TRAINING,
    VOX_ABOMB_READY,
    VOX_ABOMB_LAUNCH,
    VOX_AARRIVN1,
    VOX_AARRIVS1,
    VOX_AARIVE1,
    VOX_AARRIVW1,
    VOX_OBJECTIVE1,
    VOX_OBJECTIVE2,
    VOX_OBJECTIVE3,
    VOX_IRON_CHARGING,
    VOX_IRON_READY,
    VOX_KOSYGIN_RESCUED,
    VOX_OBJECTIVE_NOT,
    VOX_SIGNAL_FLARE_N,
    VOX_SIGNAL_FLARE_S,
    VOX_SIGNAL_FLARE_E,
    VOX_SIGNAL_FLARE_W,
    VOX_SPY_PLANE,
    VOX_TANYA_FREED,
    VOX_UPGRADE_ARMOR,
    VOX_UPGRADE_FIREPOWER,
    VOX_UPGRADE_SPEED,
    VOX_MISSION_TIMER_INIT,
    VOX_UNIT_FULL,
    VOX_UNIT_REPAIRED,
    VOX_TIME_40,
    VOX_TIME_30,
    VOX_TIME_20,
    VOX_TIME_10,
    VOX_TIME_5,
    VOX_TIME_4,
    VOX_TIME_3,
    VOX_TIME_2,
    VOX_TIME_1,
    VOX_TIME_STOP,
    VOX_UNIT_SOLD,
    VOX_TIMER_STARTED,
    VOX_TARGET_RESCUED,
    VOX_TARGET_FREED,
    VOX_TANYA_RESCUED,
    VOX_STRUCTURE_SOLD,
    VOX_SOVIET_FORCES_FALLEN,
    VOX_SOVIET_SELECTED,
    VOX_SOVIET_FALLEN,
    VOX_OPTERM1,
    VOX_OBJRCH1,
    VOX_OBJNRCH1,
    VOX_OBJMET1,
    VOX_MERCR1,
    VOX_MERCF1,
    VOX_KOSYGIN_FREED,
    VOX_FLARE_DETECTED,
    VOX_COMNDOR1,
    VOX_COMNDOF1,
    VOX_BUILDING_IN_PROGRESS,
    VOC_ATOMBOMB_PREPING,
    VOX_ALLIED_SELECTED,
    VOX_ABOMB_PREPING,
    VOX_ATOMBOMB_LAUNCH,
    VOX_AFALLEN1,
    VOX_ABOMB_AVAILABLE,
    VOX_AARRIVE1_DUP, // duplicate of 32
    VOX_MISSION_SAVED,
    VOX_MISSION_LOADED,

    VOX_COUNT,

    VOX_FIRST = VOX_MISSION_WON,
    VOX_NONE = -1
};

DEFINE_ENUMERATION_OPERATORS(VoxType);

#define VOX_BUFFERS 2

// Hook the original binary's globals until standalone or have implemented and hooked all references.
#ifdef GAME_DLL
extern void **SpeechBuffer;
extern VoxType *SpeechRecord;
#else
extern void *SpeechBuffer[VOX_BUFFERS];
extern VoxType SpeechRecord[VOX_BUFFERS];
#endif

VoxType Vox_From_Name(const char *name);
const char *Name_From_Vox(VoxType vox);

inline void Speak(VoxType vox)
{
#ifdef GAME_DLL
    void (*call_Speak)(VoxType) = reinterpret_cast<void (*)(VoxType)>(0x00426158);
    call_Speak(vox);
#endif
}

inline void Speak_AI()
{
#ifdef GAME_DLL
    void (*call_Speak_AI)() = reinterpret_cast<void (*)()>(0x004261B4);
    call_Speak_AI();
#endif
}

inline void Stop_Speaking()
{
#ifdef GAME_DLL
    void (*call_Stop_Speaking)() = reinterpret_cast<void (*)()>(0x0042632C);
    call_Stop_Speaking();
#endif
}

inline BOOL Is_Speaking()
{
#ifdef GAME_DLL
    BOOL (*call_Is_Speaking)() = reinterpret_cast<BOOL (*)()>(0x00426344);
    return call_Is_Speaking();
#else
    return 0;
#endif
}

#endif // VOX_H
