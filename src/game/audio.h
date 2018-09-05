#pragma once

#ifndef AUDIO_H
#define AUDIO_H

#include "fixed.h"
#include "housetype.h"
#include "voc.h"
#include "vox.h"

inline void Sound_Effect(VocType voc, unsigned long int1, int int2, HousesType house)
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*call_Sound_Effect)(VocType, unsigned long, int, HousesType) =
        reinterpret_cast<void (*)(VocType, unsigned long, int, HousesType)>(0x00425D1C);
    call_Sound_Effect(voc, int1, int2, house);
#else
    return 0;
#endif
}

inline int Sound_Effect(VocType voc, fixed fix, int int1, short short1, HousesType house)
{
#ifndef CHRONOSHIFT_STANDALONE
    int (*call_Sound_Effect)(VocType, fixed, int, short, HousesType) =
        reinterpret_cast<int (*)(VocType, fixed, int, short, HousesType)>(0x00425F24);
    return call_Sound_Effect(voc, fix, int1, short1, house);
#else
    return 0;
#endif
}

inline void Speak(VoxType vox)
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*call_Speak)(VoxType) = reinterpret_cast<void (*)(VoxType)>(0x00426158);
    call_Speak(vox);
#else
    return 0;
#endif
}

inline void Speak_AI()
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*call_Speak_AI)() = reinterpret_cast<void (*)()>(0x004261B4);
    call_Speak_AI();
#else
    return 0;
#endif
}

inline void Stop_Speaking()
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*call_Stop_Speaking)() = reinterpret_cast<void (*)()>(0x0042632C);
    call_Stop_Speaking();
#else
    return 0;
#endif
}

inline void Sound_Callback()
{
#ifndef CHRONOSHIFT_STANDALONE
    void(*call_Sound_Callback)() = reinterpret_cast<void(*)()>(0x005BE560);
    call_Sound_Callback();
#else
    return 0;
#endif
}

inline int Is_Speaking()
{
#ifndef CHRONOSHIFT_STANDALONE
    int (*call_Is_Speaking)() = reinterpret_cast<int (*)()>(0x00426344);
    return call_Is_Speaking();
#else
    return 0;
#endif
}

inline void Set_Score_Vol(int vol)
{
#ifndef CHRONOSHIFT_STANDALONE
    void(*call_set_Score)(int) = reinterpret_cast<void(*)(int)>(0x005BF940);
    call_set_Score(vol);
#else
    return 0;
#endif
}

inline void Stop_Sample(int sample)
{
#ifndef CHRONOSHIFT_STANDALONE
    void(*call_Stop_Sample)(int) = reinterpret_cast<void(*)(int)>(0x005BEEA0);
    call_Stop_Sample(sample);
#else
    return 0;
#endif
}

inline void Fade_Sample(int sample, int fade)
{
#ifndef CHRONOSHIFT_STANDALONE
    void(*call_Fade_Sample)(int, int) = reinterpret_cast<void(*)(int, int)>(0x005BF9B0);
    call_Fade_Sample(sample, fade);
#else
    return 0;
#endif
}

inline int Sample_Status(int sample)
{
#ifndef CHRONOSHIFT_STANDALONE
    int(*call_Sample_Status)(int) = reinterpret_cast<int(*)(int)>(0x005BEFB0);
    return call_Sample_Status(sample);
#else
    return 0;
#endif
}

inline int File_Stream_Sample_Vol(const char *name, int vol, BOOL unk)
{
#ifndef CHRONOSHIFT_STANDALONE
    int(*call_File_Stream_Sample_Vol)(const char *,int,BOOL) = reinterpret_cast<int(*)(const char *, int, BOOL)>(0x005BE430);
    return call_File_Stream_Sample_Vol(name, vol, unk);
#else
    return 0;
#endif
}

inline void Stop_Primary_Sound_Buffer()
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)() = reinterpret_cast<void (*)()>(0x005BFAE0);
    func();
#endif
}

inline void Start_Primary_Sound_Buffer(BOOL unk)
{
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(BOOL) = reinterpret_cast<void (*)(BOOL)>(0x005BFA70);
    func(unk);
#endif
}

inline BOOL Audio_Init(void *unk1, int unk2, int unk3, int unk4, int unk5)
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(void *, int, int, int, int) = reinterpret_cast<BOOL (*)(void *, int, int, int, int)>(0x005BE930);
    return func(unk1, unk2, unk3, unk4, unk5);
#else
    return false;
#endif
}

#endif // AUDIO_H
