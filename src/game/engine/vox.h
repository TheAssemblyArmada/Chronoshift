/**
 * @file
 *
 * @author CCHyper
 * @author tomsons26
 *
 * @brief Speech handling functions, Vox enum and conversion to and from string.
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
    VOX_BASE_UNDER_ATTACK,
    VOX_UNABLE_TO_BUILD,
    VOX_PRIMARY_BUILDING,
    VOX_MAD_TANK_DEPLOYED,
    VOX_SPEECH_NULL1, // NULL
    VOX_UNIT_LOST,
    VOX_SELECT_TARGET,
    VOX_ENEMY_APPROACHING,
    VOX_SILOS_NEEDED,
    VOX_ON_HOLD,
    VOX_REPAIRING,
    VOX_SPEECH_NULL2, // NULL
    VOX_SPEECH_NULL3, // NULL
    VOX_AIRBORNE_LOST,
    VOX_SPEECH_NULL4, // NULL
    VOX_ALLIED_FORCES_APPROACHING,
    VOX_ALLIED_APPROACHING,
    VOX_SPEECH_NULL5, // NULL
    VOX_SPEECH_NULL6, // NULL
    VOX_BUILDING_INFILTRATED,
    VOX_CHRONO_CHARGING,
    VOX_CHRONO_READY,
    VOX_CHROYES1,
    VOX_CMDCNTR1,
    VOX_CNTLDED1,
    VOX_CONVOY_APPROACHING,
    VOX_CONVOY_UNIT_LOST,
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
    VOX_ALLIED_REINF_N,
    VOX_ALLIED_REINF_S,
    VOX_ALLIED_REINF_E,
    VOX_ALLIED_REINF_W,
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
    VOX_OP_CONTROL_TERMINATED,
    VOX_OBJECTIVE_REACHED,
    VOX_OBJECTIVE_NOT_REACHED,
    VOX_OBJECTIVE_MET,
    VOX_MERCENARY_RESCUED,
    VOX_MERCENARY_FREED,
    VOX_KOSYGIN_FREED,
    VOX_FLARE_DETECTED,
    VOX_COMMANDO_RESCUED,
    VOX_COMMANDO_FREED,
    VOX_BUILDING_IN_PROGRESS,
    VOC_ATOMBOMB_PREPPING,
    VOX_ALLIED_SELECTED,
    VOX_ABOMB_PREPPING,
    VOX_ATOMBOMB_LAUNCH,
    VOX_ALLIED_FALLEN,
    VOX_ABOMB_AVAILABLE,
    VOX_ALLIED_APPROACHING_DUP, // duplicate of 32
    VOX_MISSION_SAVED,
    VOX_MISSION_LOADED,

    VOX_COUNT,

    VOX_FIRST = VOX_MISSION_WON,
    VOX_NONE = -1
};

DEFINE_ENUMERATION_OPERATORS(VoxType);

#define VOX_BUFFERS 2 // Number of buffers for audio files.

// Hook the original binary's globals until standalone or have implemented and hooked all references.
#ifdef GAME_DLL
#define VOX_BUFFER_SIZE 50000 // ~48 kb
extern void **g_SpeechBuffer;
extern VoxType *g_SpeechRecord;
extern VoxType &g_SpeakQueue;
extern VoxType &g_CurrentVoice;
#else
#define VOX_BUFFER_SIZE 126000 // ~123 kb - Size of the buffer where the audio file of the speech is loaded into.
extern void *g_SpeechBuffer[VOX_BUFFERS]; // Loaded speech audio file buffer.
extern VoxType g_SpeechRecord[VOX_BUFFERS]; // Loaded speech types.
extern VoxType g_SpeakQueue; // Queued speech to be played.
extern VoxType g_CurrentVoice; // Currently playing speech.
#endif

VoxType Vox_From_Name(const char *name);
const char *Name_From_Vox(VoxType vox);


void Init_Speech_Buffers();
void Speak(VoxType vox);
void Speak_AI();
void Stop_Speaking();
BOOL Is_Speaking();

#endif // VOX_H
