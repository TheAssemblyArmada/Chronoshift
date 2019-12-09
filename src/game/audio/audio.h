/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 * @author tomsons26
 *
 * @brief Game audio engine.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef AUDIO_H
#define AUDIO_H

#include "always.h"
#include "adpcm.h"
#include "gamefile.h"

#define PAN_LEFT 32767
#define PAN_CENTER 0
#define PAN_RIGHT -32767

enum
{
    INVALID_AUDIO_HANDLE = -1,
};

#pragma pack(push, 1)
struct AUDHeaderStruct
{
    uint16_t m_Rate; // Frequency rate of the sound data
    uint32_t m_Size; // Size of the packed data in the file, filesize - header
    uint32_t m_UncompSize; // Size of the uncompressed stream
    uint8_t m_Flags; // see SChannelFlag.
    uint8_t m_Compression; // see SCompressType.
};

struct AUDChunkHeaderStruct
{
    uint16_t m_Size; // Size of the chunk
    uint16_t m_OutSize; // Decompressed size of the chunk data
    uint32_t m_MagicID; // In little endian format.
};
#pragma pack(pop)

enum SoundCompressType
{
    COMP_NONE = 0,
    COMP_ZAP = 1,
    COMP_ADPCM = 99
};

typedef void (*AudioFocusCallback)();
typedef BOOL (*AudioStreamCallback)(int16_t, int16_t *, void **, int *);

void Init_Locked_Data();
BOOL File_Callback(int16_t id, int16_t *odd, void **buffer, int *size);
int __cdecl Stream_Sample_Vol(void *buffer, int size, AudioStreamCallback callback, int volume, int handle);
int File_Stream_Sample(char const *filename, BOOL real_time_start);
void File_Stream_Preload(int handle);
int File_Stream_Sample_Vol(const char *filename, int volume, BOOL real_time_start);
void Sound_Callback();
void *Load_Sample(char *filename);
int Load_Sample_Into_Buffer(char *filename, void *buffer, int size);
int Sample_Read(int handle, void *buffer, int size);
void Free_Sample(void *sample);
void Sound_Thread(void *a1);
BOOL Set_Primary_Buffer_Format();
int Print_Sound_Error(char *sound_error, void *window);
BOOL Audio_Init(void *window, int bits_per_sample, BOOL stereo, int rate, BOOL reverse_channels);
void Sound_End();
void Stop_Sample(int index);
BOOL Sample_Status(int index);
int Is_Sample_Playing(void *sample);
void Stop_Sample_Playing(void *sample);
int Get_Free_Sample_Handle(int priority);
int Play_Sample(void *sample, int priority, int volume, int16_t panloc);
int Convert_HMI_To_Direct_Sound_Volume(int vol);
int Play_Sample_Handle(void *a1, int a2, int a3, int16_t a4, int a5);
void Restore_Sound_Buffers();
int Set_Sound_Vol(int vol);
int Set_Score_Vol(int vol);
void Fade_Sample(int index, int ticks);
void Unfade_Sample(int index, int ticks);
int Get_Digi_Handle();
unsigned Sample_Length(void *sample);
int Start_Primary_Sound_Buffer(BOOL forced);
void Stop_Primary_Sound_Buffer();
void Suspend_Audio_Thread();
void Resume_Audio_Thread();

#endif // AUDIO_H
