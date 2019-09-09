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
#include "vox.h"

#ifndef PLATFORM_WINDOWS
#include <strings.h>
#endif

#ifdef GAME_DLL
void **SpeechBuffer = reinterpret_cast<void **>(0x006680D8);
VoxType *SpeechRecord = Make_Pointer<VoxType>(0x006680E0);
#else
void *SpeechBuffer[VOX_BUFFERS];
VoxType SpeechRecord[VOX_BUFFERS];
#endif

// Update the VoxType enum in vox.h if you add additional EVA lines
const char *VoxName[VOX_COUNT] = {
    "MISNWON1", // 0
    "MISNLST1", // 1
    "PROGRES1", // 2
    "CONSCMP1", // 3
    "UNITRDY1", // 4
    "NEWOPT1", // 5
    "NODEPLY1", // 6
    "STRCKIL1", // 7
    "NOPOWR1", // 8
    "NOFUNDS1", // 9
    "BCT1", // 10
    "REINFOR1", // 11
    "CANCLD1", // 12
    "ABLDGIN1", // 13
    "LOPOWER1", // 14
    "NOFUNDS1", // 15    //duplicate?!
    "BASEATK1", // 16
    "NOBUILD1", // 17
    "PRIBLDG1", // 18
    "TANK01", // 19
    "<none>", // 20
    "UNITLST1", // 21
    "SLCTTGT1", // 22
    "ENMYAPP1", // 23
    "SILOND1", // 24
    "ONHOLD1", // 25
    "REPAIR1", // 26
    "<none>", // 27
    "<none>", // 28
    "AUNITL1", // 29
    "<none>", // 30
    "AAPPRO1", // 31
    "AARRIVE1", // 32
    "<none>", // 33
    "<none>", // 34
    "BLDGINF1", // 35
    "CHROCHR1", // 36
    "CHRORDY1", // 37
    "CHROYES1", // 38
    "CMDCNTR1", // 39
    "CNTLDED1", // 40
    "CONVYAP1", // 41
    "CONVLST1", // 42
    "XPLOPLC1", // 43
    "CREDIT1", // 44
    "NAVYLST1", // 45
    "SATLNCH1", // 46
    "PULSE1", // 47
    "<none>", // 48
    "SOVFAPP1", // 49
    "SOVREIN1", // 50
    "TRAIN1", // 51
    "AREADY1", // 52
    "ALAUNCH1", // 53
    "AARRIVN1", // 54
    "AARRIVS1", // 55
    "AARIVE1", // 56
    "AARRIVW1", // 57
    "1OBJMET1", // 58
    "2OBJMET1", // 59
    "3OBJMET1", // 60
    "IRONCHG1", // 61
    "IRONRDY1", // 62
    "KOSYRES1", // 63
    "OBJNMET1", // 64
    "FLAREN1", // 65
    "FLARES1", // 66
    "FLAREE1", // 67
    "FLAREW1", // 68
    "SPYPLN1", // 69
    "TANYAF1", // 70
    "ARMORUP1", // 71
    "FIREPO1", // 72
    "UNITSPD1", // 73
    "MTIMEIN1", // 74
    "UNITFUL1", // 75
    "UNITREP1", // 76
    "40MINR", // 77
    "30MINR", // 78
    "20MINR", // 79
    "10MINR", // 80
    "5MINR", // 81
    "4MINR", // 82
    "3MINR", // 83
    "2MINR", // 84
    "1MINR", // 85
    "TIMERNO1", // 86
    "UNITSLD1", // 87
    "TIMERGO1", // 88
    "TARGRES1", // 89
    "TARGFRE1", // 90
    "TANYAR1", // 91
    "STRUSLD1", // 92
    "SOVFORC1", // 93
    "SOVEMP1", // 94
    "SOVEFAL1", // 95
    "OPTERM1", // 96
    "OBJRCH1", // 97
    "OBJNRCH1", // 98
    "OBJMET1", // 99
    "MERCR1", // 100
    "MERCF1", // 101
    "KOSYFRE1", // 102
    "FLARE1", // 103
    "COMNDOR1", // 104
    "COMNDOF1", // 105
    "BLDGPRG1", // 106
    "ATPREP1", // 107
    "ASELECT1", // 108
    "APREP1", // 109
    "ATLNCH1", // 110
    "AFALLEN1", // 111
    "AAVAIL1", // 112
    "AARRIVE1", // 113    //duplicate with 32?
    "SAVE1", // 114
    "LOAD1" // 115
};

VoxType Vox_From_Name(const char *name)
{
    if (strcasecmp(name, "<none>") == 0 || strcasecmp(name, "none") == 0) {
        return VOX_NONE;
    }

    if (name != nullptr) {
        for (VoxType vox = VOX_FIRST; vox < VOX_COUNT; ++vox) {
            if (strcasecmp(name, VoxName[vox]) == 0) {
                return vox;
            }
        }
    }
    return VOX_NONE;
}

const char *Name_From_Vox(VoxType vox)
{
    if (vox != VOX_NONE && vox < VOX_COUNT) {
        return VoxName[vox];
    }
    return "<none>";
}
