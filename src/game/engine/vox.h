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

// Update this enum if you add additional EVA lines, make sure you
// also update the VOX_COUNT value at the end of the list too!

// VOX_ALLIES_S
// 5_SOVIET_REINFORCEMENTS

enum VoxType
{
    VOX_NONE = -1,

    VOX_FIRST = 0,

    VOX_MISSION_WON = 0,
    VOX_MISSION_LOST = 1,
    VOX_NO_COMPLY_BUILDING_IN_PROGRESS = 2, // rename?
    VOX_CONSTRUCTION_COMPLETE = 3,
    VOX_UNIT_READY = 4,
    VOX_NEW_OPTIONS = 5,
    VOX_NO_DEPLOY = 6,
    VOX_STRUCTURE_DESTROYED = 7,
    VOX_NO_POWER = 8,
    VOX_NO_FUNDS = 9,
    VOX_BATTLE_CONTROL_TERMINATED = 10,
    VOX_REINFORCEMENTS_ARRIVED = 11,
    VOX_CANCELLED = 12,
    VOX_BUILDING = 13,
    VOX_LOW_POWER = 14,
    VOX_NOFUNDS1_DUP = 15, // duplicate of 9
    VOX_BASER_UNDER_ATTACK = 16,
    VOX_UNABLE_TO_BUILD = 17,
    VOX_PRIMARY_BUILDING = 18,
    VOX_MAD_TANK_DEPLOYED = 19,
    VOX_SPEECH_NULL1 = 20, // NULL
    VOX_UNIT_LOST = 21,
    VOX_SELECT_TARGET = 22,
    VOX_ENMYAPP1 = 23,
    VOX_SILOS_NEEDED = 24,
    VOX_ON_HOLD = 25,
    VOX_REPAIRING = 26,
    VOX_SPEECH_NULL2 = 27, // NULL
    VOX_SPEECH_NULL3 = 28, // NULL
    VOX_AUNITL1 = 29,
    VOX_SPEECH_NULL4 = 30, // NULL
    VOX_AAPPRO1 = 31,
    VOX_AARRIVE1 = 32,
    VOX_SPEECH_NULL5 = 33, // NULL
    VOX_SPEECH_NULL6 = 34, // NULL
    VOX_BUILDING_INFILTRATED = 35,
    VOX_CHROCHR1 = 36,
    VOX_CHRORDY1 = 37,
    VOX_CHROYES1 = 38,
    VOX_CMDCNTR1 = 39,
    VOX_CNTLDED1 = 40,
    VOX_CONVYAP1 = 41,
    VOX_CONVLST1 = 42,
    VOX_EXPLOSIVE_PLACED = 43,
    VOX_CREDITS_STOLEN = 44,
    VOX_NAVAL_LOST = 45,
    VOX_SAT_LAUNCHED = 46,
    VOX_SONAR_PULSE_AVAILABLE = 47,
    VOX_SPEECH_NULL7 = 48, // NULL
    VOX_SOVIET_APPROACHING = 49,
    VOX_SOVIET_REINFORCEMENTS = 50,
    VOX_TRAINING = 51,
    VOX_ABOMB_READY = 52,
    VOX_ABOMB_LAUNCH = 53,
    VOX_AARRIVN1 = 54,
    VOX_AARRIVS1 = 55,
    VOX_AARIVE1 = 56,
    VOX_AARRIVW1 = 57,
    VOX_OBJECTIVE1 = 58,
    VOX_OBJECTIVE2 = 59,
    VOX_OBJECTIVE3 = 60,
    VOX_IRON_CHARGING = 61,
    VOX_IRON_READY = 62,
    VOX_KOSYGIN_RESCUED = 63,
    VOX_OBJECTIVE_NOT = 64,
    VOX_SIGNAL_FLARE_N = 65,
    VOX_SIGNAL_FLARE_S = 66,
    VOX_SIGNAL_FLARE_E = 67,
    VOX_SIGNAL_FLARE_W = 68,
    VOX_SPY_PLANE = 69,
    VOX_TANYA_FREED = 70,
    VOX_UPGRADE_ARMOR = 71,
    VOX_UPGRADE_FIREPOWER = 72,
    VOX_UPGRADE_SPEED = 73,
    VOX_MISSION_TIMER_INIT = 74,
    VOX_UNIT_FULL = 75,
    VOX_UNIT_REPAIRED = 76,
    VOX_TIME_40 = 77,
    VOX_TIME_30 = 78,
    VOX_TIME_20 = 79,
    VOX_TIME_10 = 80,
    VOX_TIME_5 = 81,
    VOX_TIME_4 = 82,
    VOX_TIME_3 = 83,
    VOX_TIME_2 = 84,
    VOX_TIME_1 = 85,
    VOX_TIME_STOP = 86,
    VOX_UNIT_SOLD = 87,
    VOX_TIMER_STARTED = 88,
    VOX_TARGET_RESCUED = 89,
    VOX_TARGET_FREED = 90,
    VOX_TANYA_RESCUED = 91,
    VOX_STRUCTURE_SOLD = 92,
    VOX_SOVIET_FORCES_FALLEN = 93,
    VOX_SOVIET_SELECTED = 94,
    VOX_SOVIET_FALLEN = 95,
    VOX_OPTERM1 = 96,
    VOX_OBJRCH1 = 97,
    VOX_OBJNRCH1 = 98,
    VOX_OBJMET1 = 99,
    VOX_MERCR1 = 100,
    VOX_MERCF1 = 101,
    VOX_KOSYGIN_FREED = 102,
    VOX_FLARE_DETECTED = 103,
    VOX_COMNDOR1 = 104,
    VOX_COMNDOF1 = 105,
    VOX_BUILDING_IN_PROGRESS = 106,
    VOC_ATOMBOMB_PREPING = 107,
    VOX_ALLIED_SELECTED = 108,
    VOX_ABOMB_PREPING = 109,
    VOX_ATOMBOMB_LAUNCH = 110,
    VOX_AFALLEN1 = 111,
    VOX_ABOMB_AVAILABLE = 112,
    VOX_AARRIVE1_DUP = 113, // duplicate of 32
    VOX_MISSION_SAVED = 114,
    VOX_MISSION_LOADED = 115,

    VOX_LAST = 115,

    VOX_COUNT = 116
};

DEFINE_ENUMERATION_OPERATORS(VoxType);

#define VOX_BUFFERS 2

// Hook the original binary's globals until standalone or have implemented and hooked all references.
#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
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
#ifndef CHRONOSHIFT_STANDALONE
    void (*call_Speak)(VoxType) = reinterpret_cast<void (*)(VoxType)>(0x00426158);
    call_Speak(vox);
#endif
}

inline void Speak_AI()
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*call_Speak_AI)() = reinterpret_cast<void (*)()>(0x004261B4);
    call_Speak_AI();
#endif
}

inline void Stop_Speaking()
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*call_Stop_Speaking)() = reinterpret_cast<void (*)()>(0x0042632C);
    call_Stop_Speaking();
#endif
}

inline BOOL Is_Speaking()
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*call_Is_Speaking)() = reinterpret_cast<BOOL (*)()>(0x00426344);
    return call_Is_Speaking();
#else
    return 0;
#endif
}

#endif // VOX_H
