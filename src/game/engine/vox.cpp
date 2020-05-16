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
#include "vox.h"
#include "audio.h"
#include "gameoptions.h"
#include "globals.h"
#include <captainslog.h>

#ifdef GAME_DLL
extern int16_t &g_SoundType;
extern int16_t &g_SampleType;
#else
extern int16_t g_SoundType;
extern int16_t g_SampleType;
#endif

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#ifndef GAME_DLL
void *g_SpeechBuffer[VOX_BUFFERS];
VoxType g_SpeechRecord[VOX_BUFFERS];
VoxType g_SpeakQueue = VOX_NONE;
VoxType g_CurrentVoice = VOX_NONE;
#endif

// clang-format off
// Update the VoxType enum in vox.h if you add additional EVA lines
const char *VoxName[VOX_COUNT] = {
    "misnwon1", // 0
    "misnlst1", // 1
    "progres1", // 2
    "conscmp1", // 3
    "unitrdy1", // 4
    "newopt1", // 5
    "nodeply1", // 6
    "strckil1", // 7
    "nopowr1", // 8
    "nofunds1", // 9
    "bct1", // 10
    "reinfor1", // 11
    "cancld1", // 12
    "abldgin1", // 13
    "lopower1", // 14
    "nofunds1", // 15    //duplicate?!
    "baseatk1", // 16
    "nobuild1", // 17
    "pribldg1", // 18
    "tank01", // 19
    "<none>", // 20
    "unitlst1", // 21
    "slcttgt1", // 22
    "enmyapp1", // 23
    "silond1", // 24
    "onhold1", // 25
    "repair1", // 26
    "<none>", // 27
    "<none>", // 28
    "aunitl1", // 29
    "<none>", // 30
    "aappro1", // 31
    "aarrive1", // 32
    "<none>", // 33
    "<none>", // 34
    "bldginf1", // 35
    "chrochr1", // 36
    "chrordy1", // 37
    "chroyes1", // 38
    "cmdcntr1", // 39
    "cntlded1", // 40
    "convyap1", // 41
    "convlst1", // 42
    "xploplc1", // 43
    "credit1", // 44
    "navylst1", // 45
    "satlnch1", // 46
    "pulse1", // 47
    "<none>", // 48
    "sovfapp1", // 49
    "sovrein1", // 50
    "train1", // 51
    "aready1", // 52
    "alaunch1", // 53
    "aarrivn1", // 54
    "aarrivs1", // 55
    "aarive1", // 56
    "aarrivw1", // 57
    "1objmet1", // 58
    "2objmet1", // 59
    "3objmet1", // 60
    "ironchg1", // 61
    "ironrdy1", // 62
    "kosyres1", // 63
    "objnmet1", // 64
    "flaren1", // 65
    "flares1", // 66
    "flaree1", // 67
    "flarew1", // 68
    "spypln1", // 69
    "tanyaf1", // 70
    "armorup1", // 71
    "firepo1", // 72
    "unitspd1", // 73
    "mtimein1", // 74
    "unitful1", // 75
    "unitrep1", // 76
    "40minr", // 77
    "30minr", // 78
    "20minr", // 79
    "10minr", // 80
    "5minr", // 81
    "4minr", // 82
    "3minr", // 83
    "2minr", // 84
    "1minr", // 85
    "timerno1", // 86
    "unitsld1", // 87
    "timergo1", // 88
    "targres1", // 89
    "targfre1", // 90
    "tanyar1", // 91
    "strusld1", // 92
    "sovforc1", // 93
    "sovemp1", // 94
    "sovefal1", // 95
    "opterm1", // 96
    "objrch1", // 97
    "objnrch1", // 98
    "objmet1", // 99
    "mercr1", // 100
    "mercf1", // 101
    "kosyfre1", // 102
    "flare1", // 103
    "comndor1", // 104
    "comndof1", // 105
    "bldgprg1", // 106
    "atprep1", // 107
    "aselect1", // 108
    "aprep1", // 109
    "atlnch1", // 110
    "afallen1", // 111
    "aavail1", // 112
    "aarrive1", // 113    //duplicate with 32?
    "save1", // 114
    "load1" // 115
};
// clang-format on

VoxType Vox_From_Name(const char *name)
{
    captainslog_assert(name != nullptr);

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

/**
 * Allocates buffers where to load speech files into.
 */
void Init_Speech_Buffers()
{
    for (int i = 0; i < VOX_BUFFERS; ++i) {
        g_SpeechBuffer[i] = new char[VOX_BUFFER_SIZE];
        g_SpeechRecord[i] = VOX_NONE;
    }
}

/**
 * Queue's a speech to play and plays it if possible.
 */
void Speak(VoxType vox)
{
    if (!g_Debug_Quiet && g_Options.Get_Sound_Volume() > 0 && g_SampleType && vox != VOX_NONE) {
        if (vox != g_SpeakQueue && vox != g_CurrentVoice && g_SpeakQueue == VOX_NONE) {
            g_SpeakQueue = vox;
            Speak_AI();
        }
    }
}

/**
 * speech playback handling loop.
 */
void Speak_AI()
{
    static int _index = 0;

    char string[NAME_MAX];

    if (!g_Debug_Quiet && g_SampleType && !Is_Sample_Playing(g_SpeechBuffer[_index])) {
        g_CurrentVoice = VOX_NONE;

        if (VOX_NONE != g_SpeakQueue) {
            void *sample = nullptr;

            _index = (_index + 1) % VOX_BUFFERS;

            snprintf(string, sizeof(string), "%s.aud", VoxName[g_SpeakQueue]);
            GameFileClass file(string);

            if (file.Is_Available() && file.Read(g_SpeechBuffer[_index], VOX_BUFFER_SIZE)) {
                sample = g_SpeechBuffer[_index];
                g_SpeechRecord[_index] = g_SpeakQueue;
            }

            if (sample != nullptr) {
                Play_Sample(sample, 254, g_Options.Get_Sound_Volume() * 256, 0);
                g_CurrentVoice = g_SpeakQueue;
            }

            g_SpeakQueue = VOX_NONE;
        }
    }
}

/**
 * Stops speech playback.
 */
void Stop_Speaking()
{
    g_SpeakQueue = VOX_NONE;
    // TODO: To look into, g_SpeechBuffer[0] will fail when Speak_AI's _index is not 0
    Stop_Sample_Playing(g_SpeechBuffer[0]);
}

/**
 * Is there any speech playing?
 */
BOOL Is_Speaking()
{
    Speak_AI();

    if (g_Debug_Quiet || !g_SampleType) {
        return false;
    }

    // TODO: To look into, g_SpeechBuffer[0] will fail when Speak_AI's _index is not 0
    if (g_SpeakQueue == VOX_NONE && !Is_Sample_Playing(g_SpeechBuffer[0])) {
        return false;
    }

    return true;
}
