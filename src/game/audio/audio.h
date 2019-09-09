#pragma once

#ifndef AUDIO_H
#define AUDIO_H

#include "always.h"

inline void Sound_Callback()
{
#ifdef GAME_DLL
    void(*call_Sound_Callback)() = reinterpret_cast<void(*)()>(0x005BE560);
    call_Sound_Callback();
#endif
}

inline void Set_Score_Vol(int vol)
{
#ifdef GAME_DLL
    void(*call_set_Score)(int) = reinterpret_cast<void(*)(int)>(0x005BF940);
    call_set_Score(vol);
#endif
}

inline void Stop_Sample(int sample)
{
#ifdef GAME_DLL
    void(*call_Stop_Sample)(int) = reinterpret_cast<void(*)(int)>(0x005BEEA0);
    call_Stop_Sample(sample);
#endif
}

inline void Fade_Sample(int sample, int fade)
{
#ifdef GAME_DLL
    void(*call_Fade_Sample)(int, int) = reinterpret_cast<void(*)(int, int)>(0x005BF9B0);
    call_Fade_Sample(sample, fade);
#endif
}

inline int Sample_Status(int sample)
{
#ifdef GAME_DLL
    int(*call_Sample_Status)(int) = reinterpret_cast<int(*)(int)>(0x005BEFB0);
    return call_Sample_Status(sample);
#else
    return 0;
#endif
}

inline int File_Stream_Sample_Vol(const char *name, int vol, BOOL unk)
{
#ifdef GAME_DLL
    int(*call_File_Stream_Sample_Vol)(const char *,int,BOOL) = reinterpret_cast<int(*)(const char *, int, BOOL)>(0x005BE430);
    return call_File_Stream_Sample_Vol(name, vol, unk);
#else
    return 0;
#endif
}

inline void Stop_Primary_Sound_Buffer()
{
#ifdef GAME_DLL
    void (*func)() = reinterpret_cast<void (*)()>(0x005BFAE0);
    func();
#endif
}

inline void Start_Primary_Sound_Buffer(BOOL unk)
{
#ifdef GAME_DLL
    void (*func)(BOOL) = reinterpret_cast<void (*)(BOOL)>(0x005BFA70);
    func(unk);
#endif
}

inline BOOL Audio_Init(void *unk1, int unk2, int unk3, int unk4, int unk5)
{
#ifdef GAME_DLL
    BOOL (*func)(void *, int, int, int, int) = reinterpret_cast<BOOL (*)(void *, int, int, int, int)>(0x005BE930);
    return func(unk1, unk2, unk3, unk4, unk5);
#else
    return false;
#endif
}

#endif // AUDIO_H
