#pragma once

#ifndef AUDIO_H
#define AUDIO_H

#include "fixed.h"
#include "housetype.h"

enum VoxType
{
    VOX_NONE = -1,
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
    VOX_COUNT
};

enum VocType
{
    VOC_NONE = -1,
    VOC_GIRL_OKAY = 0, // Civilian Girl
    VOC_GIRL_YEAH = 1,
    VOC_GUY_OKAY = 2, // Civilian Guy
    VOC_GUY_YEAH = 3,
    VOC_MINELAY = 4,
    VOC_ACKNO = 5,
    VOC_AFFIRM1 = 6,
    VOC_AWAIT1 = 7,
    VOC_EAFFIRM1 = 8, // Engineer
    VOC_EENGIN1 = 9,
    VOC_NOPROB = 10,
    VOC_READY = 11,
    VOC_REPORT1 = 12,
    VOC_RITAWAY = 13,
    VOC_ROGER = 14,
    VOC_UGOTIT = 15,
    VOC_VEHIC1 = 16,
    VOC_YESSIR1 = 17,
    VOC_DEDMAN1 = 18, // Death Screams
    VOC_DEDMAN2 = 19,
    VOC_DEDMAN3 = 20,
    VOC_DEDMAN4 = 21,
    VOC_DEDMAN5 = 22,
    VOC_DEDMAN6 = 23,
    VOC_DEDMAN7 = 24,
    VOC_DEDMAN8 = 25,
    VOC_DEDMAN10 = 26,
    VOC_CHRONO2 = 27,
    VOC_CANNON1 = 28,
    VOC_CANNON2 = 29,
    VOC_IRONCUR9 = 30,
    VOC_EMOVOUT1 = 31,
    VOC_SONPULSE = 32,
    VOC_SANDBAG2 = 33,
    VOC_MINEBLO1 = 34,
    VOC_CHUTE1 = 35,
    VOC_DOGY1 = 36,
    VOC_DOGW5 = 37,
    VOC_DOGG5P = 38,
    VOC_FIREBL3 = 39,
    VOC_FIRETRT1 = 40,
    VOC_GRENADE1 = 41,
    VOC_GUN11 = 42,
    VOC_GUN13 = 43,
    VOC_EYESSIR1 = 44,
    VOC_GUN27 = 45,
    VOC_HEAL2 = 46,
    VOC_HYDROD1 = 47,
    VOC_INVUL2 = 48,
    VOC_KABOOM1 = 49,
    VOC_KABOOM12 = 50,
    VOC_KABOOM15 = 51,
    VOC_SPLASH9 = 52,
    VOC_KABOOM22 = 53,
    VOC_AACANON3 = 54,
    VOC_TANDETH1 = 55,
    VOC_MGUNINF1 = 56,
    VOC_MISSILE1 = 57,
    VOC_MISSILE6 = 58,
    VOC_MISSILE7 = 59,
    VOC_ORETRUK1 = 60,
    VOC_PILLBOX1 = 61,
    VOC_RABEEP1 = 62,
    VOC_RAMENU1 = 63,
    VOC_SILENCER = 64,
    VOC_TANK5 = 65,
    VOC_TANK6 = 66,
    VOC_TORPEDO1 = 67,
    VOC_TURRET1 = 68,
    VOC_TSLACHG2 = 69,
    VOC_TESLA1 = 70,
    VOC_SQUISHY2 = 71,
    VOC_SCOLDY1 = 72,
    VOC_RADAR_ON = 73, // Radar
    VOC_RADAR_OFF = 74,
    VOC_PLACE_BUILDING = 75,
    VOC_KABOOM30 = 76,
    VOC_KABOOM25 = 77,
    VOC_KABOOM21 = 78,
    VOC_DOGW7 = 79,
    VOC_DOGW3PX = 80,
    VOC_CRMBLE2 = 81,
    VOC_MONEY_UP = 82, // Credits Tick Up/Down
    VOC_MONEY_DOWN = 83,
    VOC_CONSTRUCTION = 84,
    VOC_GAME_CLOSED = 85,
    VOC_INCOMING_MESSAGE = 86,
    VOC_SYS_ERROR = 87,
    VOC_OPTIONS_CHANGED = 88,
    VOC_GAME_FORMING = 89,
    VOC_PLAYER_LEFT = 90,
    VOC_PLAYER_JOINED = 91,
    VOC_DEPTH_CHARGE = 92,
    VOC_CASHTURN = 93, // Selling
    VOC_CHEW_ON_THIS = 94, // Tanya
    VOC_LETS_ROCK = 95,
    VOC_LAUGH = 96,
    VOC_SHAKE_IT_BABY = 97,
    VOC_CHA_CHING = 98,
    VOC_ALL_YA_GOT = 99,
    VOC_KISS_BYE_BYE = 100,
    VOC_IM_THERE = 101,
    VOC_GIVE_IT_TO_ME = 102,
    VOC_YEAH = 103,
    VOC_YES_SIR = 104,
    VOC_WHATS_UP = 105,
    VOC_WALLKIL2 = 106,
    VOC_UNKNOWN22 = 107,
    VOC_GUN5 = 108,
    VOC_SUB_SHOW = 109,
    VOC_EINSTEIN_YEAH = 110, // Einstein
    VOC_EINSTEIN_OK = 111,
    VOC_EINSTEIN_YES = 112,
    VOC_MINE1 = 113,
    VOC_SCOMND1 = 114, // Spy
    VOC_SYESSIR1 = 115,
    VOC_SPY_INDEED = 116,
    VOC_SONWAY1 = 117,
    VOC_SPY_KING_AND_COUNTRY = 118,
    VOC_MRESPON1 = 119,
    VOC_MYESSIR1 = 120,
    VOC_MAFFIRM1 = 121,
    VOC_MMOVOUT1 = 122,
    VOC_BEEPSLCT = 123,
    VOC_SYEAH1 = 124,
    VOC_ANT_DIE = 125, // Giant Ant
    VOC_ANT_BITE = 126,
    VOC_SMOUT1 = 127,
    VOC_SOKAY1 = 128,
    VOC_UNKNOWN23 = 129,
    VOC_SWHAT1 = 130,
    VOC_SAFFIRM1 = 131,
    VOC_STAVCMDR = 132,
    VOC_STAVCRSE = 133,
    VOC_STAVYES = 134,
    VOC_STAVMOV = 135,
    VOC_BUZZY1 = 136,
    VOC_RAMBO1 = 137,
    VOC_RAMBO2 = 138,
    VOC_RAMBO3 = 139,
    VOC_MYES1 = 140,
    VOC_MHOWDY1 = 141,
    VOC_MRISE1 = 142,
    VOC_MHUH1 = 143,
    VOC_MHEAR1 = 144,
    VOC_MLAFF1 = 145,
    VOC_MBOSS1 = 146,
    VOC_MYEEHAW1 = 147,
    VOC_MHOTDIG1 = 148,
    VOC_MWRENCH1 = 149,
    VOC_JBURN1 = 150,
    VOC_JCHRGE1 = 151,
    VOC_JCRISP1 = 152,
    VOC_JDANCE1 = 153,
    VOC_JJUICE1 = 154,
    VOC_JJUMP1 = 155,
    VOC_JLIGHT1 = 156,
    VOC_JPOWER1 = 157,
    VOC_JSHOCK1 = 158,
    VOC_JYES1 = 159,
    VOC_CHROTNK1 = 160,
    VOC_FIXIT1 = 161,
    VOC_MADCHRG2 = 162,
    VOC_MADEXPLO = 163,
    VOC_SHKTROP1 = 164,
    VOC_COUNT
};

inline VocType Voc_From_Name(char *name)
{
#ifndef RAPP_STANDALONE
    VocType (*call_Voc_From_Name)(char *) = reinterpret_cast<VocType (*)(char *)>(0x00425CA0);
    return call_Voc_From_Name(name);
#else
    return 0;
#endif
}

inline char *Voc_Name(VocType voc)
{
#ifndef RAPP_STANDALONE
    char *(*call_Voc_Name)(VocType) = reinterpret_cast<char *(*)(VocType)>(0x00425CF8);
    return call_Voc_Name(voc);
#else
    return 0;
#endif
}

inline void Sound_Effect(VocType voc, unsigned long int1, int int2, HousesType house)
{
#ifndef RAPP_STANDALONE
    void (*call_Sound_Effect)(VocType, unsigned long, int, HousesType) =
        reinterpret_cast<void (*)(VocType, unsigned long, int, HousesType)>(0x00425D1C);
    call_Sound_Effect(voc, int1, int2, house);
#else
    return 0;
#endif
}

inline int Sound_Effect(VocType voc, fixed fix, int int1, short short1, HousesType house)
{
#ifndef RAPP_STANDALONE
    int (*call_Sound_Effect)(VocType, fixed, int, short, HousesType) =
        reinterpret_cast<int (*)(VocType, fixed, int, short, HousesType)>(0x00425F24);
    return call_Sound_Effect(voc, fix, int1, short1, house);
#else
    return 0;
#endif
}

inline char *Speech_Name(VoxType vox)
{
#ifndef RAPP_STANDALONE
    char *(*call_Speech_Name)(VoxType) = reinterpret_cast<char *(*)(VoxType)>(0x00426138);
    return call_Speech_Name(vox);
#else
    return 0;
#endif
}

inline void Speak(VoxType vox)
{
#ifndef RAPP_STANDALONE
    void (*call_Speak)(VoxType) = reinterpret_cast<void (*)(VoxType)>(0x00426158);
    call_Speak(vox);
#else
    return 0;
#endif
}

inline void Speak_AI()
{
#ifndef RAPP_STANDALONE
    void (*call_Speak_AI)() = reinterpret_cast<void (*)()>(0x004261B4);
    call_Speak_AI();
#else
    return 0;
#endif
}

inline void Stop_Speaking()
{
#ifndef RAPP_STANDALONE
    void (*call_Stop_Speaking)() = reinterpret_cast<void (*)()>(0x0042632C);
    call_Stop_Speaking();
#else
    return 0;
#endif
}

inline void Sound_Callback()
{
#ifndef RAPP_STANDALONE
    void(*call_Sound_Callback)() = reinterpret_cast<void(*)()>(0x005BE560);
    call_Sound_Callback();
#else
    return 0;
#endif
}

inline int Is_Speaking()
{
#ifndef RAPP_STANDALONE
    int (*call_Is_Speaking)() = reinterpret_cast<int (*)()>(0x00426344);
    return call_Is_Speaking();
#else
    return 0;
#endif
}

inline void Set_Score_Vol(int vol)
{
#ifndef RAPP_STANDALONE
    void(*call_set_Score)(int) = reinterpret_cast<void(*)(int)>(0x005BF940);
    call_set_Score(vol);
#else
    return 0;
#endif
}

inline void Stop_Sample(int sample)
{
#ifndef RAPP_STANDALONE
    void(*call_Stop_Sample)(int) = reinterpret_cast<void(*)(int)>(0x005BEEA0);
    call_Stop_Sample(sample);
#else
    return 0;
#endif
}

inline void Fade_Sample(int sample, int fade)
{
#ifndef RAPP_STANDALONE
    void(*call_Fade_Sample)(int, int) = reinterpret_cast<void(*)(int, int)>(0x005BF9B0);
    call_Fade_Sample(sample, fade);
#else
    return 0;
#endif
}

inline int Sample_Status(int sample)
{
#ifndef RAPP_STANDALONE
    int(*call_Sample_Status)(int) = reinterpret_cast<int(*)(int)>(0x005BEFB0);
    return call_Sample_Status(sample);
#else
    return 0;
#endif
}

inline int File_Stream_Sample_Vol(const char *name, int vol, BOOL unk)
{
#ifndef RAPP_STANDALONE
    int(*call_File_Stream_Sample_Vol)(const char *,int,BOOL) = reinterpret_cast<int(*)(const char *, int, BOOL)>(0x005BE430);
    return call_File_Stream_Sample_Vol(name, vol, unk);
#else
    return 0;
#endif
}

#endif // AUDIO_H
