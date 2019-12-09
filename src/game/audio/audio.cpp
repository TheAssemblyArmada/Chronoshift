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
#include "audio.h"
#include "alloc.h"
#include "eventhandler.h"
#include "gbuffer.h"
#include "globals.h"
#include "ostimer.h"
#include <algorithm>
#include <cmath>

using std::log;
using std::exp;

#ifdef BUILD_WITH_DSOUND
#define DIRECTSOUND_VERSION 0x0600

// These includes must not be resorted.
// clang-format off
#include <mmsystem.h>
#include <dsound.h>
// clang-format on
#endif
enum
{
    AUD_CHUNK_MAGIC_ID = 0x0000DEAF,
    VOLUME_MIN = 0,
    VOLUME_MAX = 255,
    PRIORITY_MIN = 0,
    PRIORITY_MAX = 255,
#ifdef CHRONOSHIFT_STANDALONE
    MAX_SAMPLE_TRACKERS = 30, // RA has a issue where lots of sounds get cut
                              // off because of only a small number of trackers...
#else
    MAX_SAMPLE_TRACKERS = 5,
#endif
    STREAM_BUFFER_COUNT = 16,
    BUFFER_CHUNK_SIZE = 8192, // 256 * 32,
    UNCOMP_BUFFER_SIZE = 2098,
    BUFFER_TOTAL_BYTES = BUFFER_CHUNK_SIZE * 4, // 32 kb
    TIMER_DELAY = 25,
    TIMER_RESOLUTION = 1,
    TIMER_TARGET_RESOLUTION = 10 // 10-millisecond target resolution
};

struct SampleTrackerType
{
    BOOL m_Active; // Is this sample currently active?
    BOOL m_Loading; // Is this to be preloaded?
    BOOL m_DontTouch; // You heard the man. Seems to be used to skip callbacks.
    BOOL m_IsScore; // Is this sample score(music)?
    void *m_Original; // Original sample data pointer.
    int m_OriginalSize; // Original sample size, including header.
#ifdef BUILD_WITH_DSOUND
    LPDIRECTSOUNDBUFFER m_PlayBuffer;
#endif
    int m_PlaybackRate; // Samples per second.
    int m_BitSize; // Bit size represented as 0 on 8 bits, 2 on 16 bits.
    BOOL m_Stereo; // Is this sample stereo?
    int m_DataLength; // Unused.
    unsigned m_DestPtr; // Used as both a pointer and cursor location.
    BOOL m_MoreSource; // Should load more data?
    BOOL m_OneShot; // Not sure, should play only once maybe?
    void *m_Source; // Sample data pointer, not including header.
    int m_Remainder; // Sample size not including header.
#ifdef BUILD_WITH_DSOUND
    CRITICAL_SECTION m_CriticalSection;
#endif
    int m_Priority; // Priority of this sample tracker.
    int16_t m_Handle; // Sample ID.
    unsigned m_Volume; // Volume of this sample tracker.
    unsigned m_Reducer; // By how much to reduce the volume.
    SoundCompressType m_Compression; // Sample compression type.
    int16_t m_TrailerLen; // Unused.
    char m_Trailer[32]; // Unused.
    int m_Pitch; // Unused.
    int16_t m_Flags; // Unused.
    int16_t m_Service; // Requires servicing, checked in callbacks.
    int m_Restart; // Unused.
    AudioStreamCallback m_Callback; // Function to call in callback loop when handling this tracker.
    void *m_QueueBuffer; // Queued file buffer.
    int m_QueueSize; // Queued file size.
    int16_t m_Odd;
    int m_FilePending; // Number of pending files.
    int m_FilePendingSize; // Total pending size of all files.
    int m_FileHandle; // File handle of the current sample.
    void *m_FileBuffer; // Current file buffer.
    ADPCMStreamType m_StreamInfo; // ADPCM stream info, only used on COMP_ADPCM.
};

struct LockedDataType
{
    int m_DigiHandle;
    int m_ServiceSomething;
    unsigned m_MagicNumber;
    void *m_UncompBuffer;
    int m_StreamBufferSize;
    int16_t m_StreamBufferCount;
    SampleTrackerType m_SampleTracker[MAX_SAMPLE_TRACKERS];
    unsigned m_SoundVolume;
    unsigned m_ScoreVolume;
    int m_LockCount;
};

#ifdef GAME_DLL
extern BOOL &g_ReverseChannels;
extern LockedDataType &g_LockedData;
#ifdef BUILD_WITH_DSOUND
extern LPDIRECTSOUND &g_SoundObject;
extern LPDIRECTSOUNDBUFFER &g_DumpBuffer;
extern LPDIRECTSOUNDBUFFER &g_PrimaryBufferPtr;
extern WAVEFORMATEX &g_DsBuffFormat;
extern WAVEFORMATEX &g_PrimaryBuffFormat;
extern DSBUFFERDESC &g_BufferDesc;
extern DSBUFFERDESC &g_PrimaryBufferDesc;
extern CRITICAL_SECTION &g_GlobalAudioCriticalSection;
#endif
extern void *&g_SoundThreadHandle;
extern BOOL &g_SoundThreadActive;
extern BOOL &g_StartingFileStream;
extern MemoryFlagType &g_StreamBufferFlag;
extern int &g_Misc;
#ifdef BUILD_WITH_DSOUND
extern UINT &g_SoundTimerHandle;
#endif
extern void *&g_FileStreamBuffer;
extern BOOL volatile &g_AudioDone;
extern int16_t &g_SoundType;
extern int16_t &g_SampleType;
#else
BOOL g_ReverseChannels;
LockedDataType g_LockedData;
#ifdef BUILD_WITH_DSOUND
LPDIRECTSOUND g_SoundObject;
LPDIRECTSOUNDBUFFER g_DumpBuffer;
LPDIRECTSOUNDBUFFER g_PrimaryBufferPtr;
WAVEFORMATEX g_DsBuffFormat;
WAVEFORMATEX g_PrimaryBuffFormat;
DSBUFFERDESC g_BufferDesc;
DSBUFFERDESC g_PrimaryBufferDesc;
CRITICAL_SECTION g_GlobalAudioCriticalSection;
#endif
void *g_SoundThreadHandle;
BOOL g_SoundThreadActive;
BOOL g_StartingFileStream;
MemoryFlagType g_StreamBufferFlag;
int g_Misc;
#ifdef BUILD_WITH_DSOUND
UINT g_SoundTimerHandle;
#endif
void *g_FileStreamBuffer;
BOOL volatile g_AudioDone;
int16_t g_SoundType;
int16_t g_SampleType;
#endif

// Forward declare some internal functions.
#ifdef BUILD_WITH_DSOUND
BOOL Attempt_Audio_Restore(LPDIRECTSOUNDBUFFER sound_buffer);
void CALLBACK Sound_Timer_Callback(UINT uID = 0, UINT uMsg = 0, DWORD_PTR dwUser = 0, DWORD_PTR dw1 = 0, DWORD_PTR dw2 = 0);
#endif
int Simple_Copy(void **source, int *ssize, void **alternate, int *altsize, void **dest, int size);
int Sample_Copy(SampleTrackerType *st, void **source, int *ssize, void **alternate, int *altsize, void *dest, int size,
    SoundCompressType sound_comp, void *trailer, int16_t *trailersize);

/**
 * Callback registered to be called periodically to keep buffers topped up with data from each playing sample.
 */
void Maintenance_Callback()
{
#ifdef BUILD_WITH_DSOUND
    SampleTrackerType *st = g_LockedData.m_SampleTracker;
    HRESULT ret;
    DWORD play_cursor;
    DWORD write_cursor;

    for (int i = 0; i < MAX_SAMPLE_TRACKERS; ++i) {
        if (st->m_Active) { // If this tracker needs processing and isn't already marked as being processed, then process it.
            if (st->m_Service && !st->m_DontTouch) {
                st->m_DontTouch = true;

                ret = st->m_PlayBuffer->GetCurrentPosition(&play_cursor, &write_cursor);

                if (ret != DS_OK) {
                    captainslog_debug("%s - Failed to get Buffer play position!", __CURRENT_FUNCTION__);
                    if (ret == DSERR_BUFFERLOST && g_AudioFocusLoss != nullptr) {
                        g_AudioFocusLoss();
                    }

                    return;
                }

                // Do we have more data in this tracker to play?
                if (st->m_MoreSource) {
                    BOOL write_more = false;

                    // Work out where we are relative to where we copied up to? Code suggests m_DestPtr was a pointer in
                    // original and was abused to hold an integer value for the DSound implementation.
                    if (play_cursor >= st->m_DestPtr) {
                        if (play_cursor > BUFFER_TOTAL_BYTES - BUFFER_CHUNK_SIZE && st->m_DestPtr == 0) {
                            write_more = true;
                        }
                    } else if (st->m_DestPtr - play_cursor <= BUFFER_CHUNK_SIZE) {
                        write_more = true;
                    }

                    void *play_buffer_ptr;
                    void *wrapped_buffer_ptr;
                    DWORD play_lock_len;
                    DWORD wrapped_lock_len;

                    // Lock the buffer object and copy uncompressed PCM samples to it.
                    if (write_more
                        && st->m_PlayBuffer->Lock(st->m_DestPtr,
                               2 * BUFFER_CHUNK_SIZE,
                               &play_buffer_ptr,
                               &play_lock_len,
                               &wrapped_buffer_ptr,
                               &wrapped_lock_len,
                               0)
                            == DS_OK) {
                        int bytes_copied = Sample_Copy(st,
                            &st->m_Source,
                            &st->m_Remainder,
                            &st->m_QueueBuffer,
                            &st->m_QueueSize,
                            play_buffer_ptr,
                            BUFFER_CHUNK_SIZE,
                            st->m_Compression,
                            st->m_Trailer,
                            &st->m_TrailerLen);

                        // We didn't have enough data left to finish filling the chunk so fill with silence.
                        if (bytes_copied != BUFFER_CHUNK_SIZE) {
                            st->m_MoreSource = false;

                            if (st->m_BitSize != 0) {
                                memset(static_cast<char *>(play_buffer_ptr) + bytes_copied,
                                    0,
                                    BUFFER_CHUNK_SIZE - bytes_copied);
                            } else {
                                // Sole has these memsets for when bitsize is 0, possibly for 8bit unsigned audio?
                                memset(static_cast<char *>(play_buffer_ptr) + bytes_copied,
                                    0x80,
                                    BUFFER_CHUNK_SIZE - bytes_copied);
                            }
                        }

                        // This block silences the next block, ensures we don't play random garbage with buffer underruns.
                        if (st->m_DestPtr == BUFFER_TOTAL_BYTES - BUFFER_CHUNK_SIZE) {
                            if (wrapped_buffer_ptr != nullptr && wrapped_lock_len != 0) {
                                if (st->m_BitSize != 0) {
                                    memset(wrapped_buffer_ptr, 0, wrapped_lock_len);
                                } else {
                                    // Sole has these memsets for when bitsize is 0, possibly for 8bit unsigned audio?
                                    memset(wrapped_buffer_ptr, 0x80, wrapped_lock_len);
                                }
                            }
                        } else {
                            if (st->m_BitSize != 0) {
                                memset(static_cast<char *>(play_buffer_ptr) + BUFFER_CHUNK_SIZE, 0, BUFFER_CHUNK_SIZE);
                            } else {
                                // Sole has these memsets for when bitsize is 0, possibly for 8bit unsigned audio?
                                memset(static_cast<char *>(play_buffer_ptr) + BUFFER_CHUNK_SIZE, 0x80, BUFFER_CHUNK_SIZE);
                            }
                        }

                        st->m_DestPtr += bytes_copied;

                        // If we reached the end of the buffer, loop back around.
                        if (st->m_DestPtr >= BUFFER_TOTAL_BYTES) {
                            st->m_DestPtr -= BUFFER_TOTAL_BYTES;
                        }

                        st->m_PlayBuffer->Unlock(play_buffer_ptr, play_lock_len, wrapped_buffer_ptr, wrapped_lock_len);
                    }
                } else if (play_cursor >= st->m_DestPtr && play_cursor - st->m_DestPtr < BUFFER_CHUNK_SIZE
                    || !st->m_OneShot && play_cursor < st->m_DestPtr
                        && st->m_DestPtr - play_cursor > BUFFER_TOTAL_BYTES - BUFFER_CHUNK_SIZE) {
                    st->m_PlayBuffer->Stop();
                    st->m_Service = 0;
                    Stop_Sample(i);
                }

                st->m_DontTouch = false;
            }

            if (!st->m_DontTouch && !st->m_QueueBuffer && st->m_FilePending != 0) {
                st->m_QueueBuffer = static_cast<char *>(st->m_FileBuffer)
                    + g_LockedData.m_StreamBufferSize * (st->m_Odd % g_LockedData.m_StreamBufferCount);
                --st->m_FilePending;
                ++st->m_Odd;

                if (st->m_FilePending != 0) {
                    st->m_QueueSize = g_LockedData.m_StreamBufferSize;
                } else {
                    st->m_QueueSize = st->m_FilePendingSize;
                }
            }
        }

        ++st;
    }

    // Perform any volume modifications that need to be made.
    if (g_LockedData.m_LockCount == 0) {
        ++g_LockedData.m_LockCount;
        st = g_LockedData.m_SampleTracker;

        for (int i = 0; i < MAX_SAMPLE_TRACKERS; ++i) {
            if (st->m_Active && st->m_Reducer > 0 && st->m_Volume > 0) {
                if (st->m_Reducer >= st->m_Volume) {
                    st->m_Volume = VOLUME_MIN;
                } else {
                    st->m_Volume -= st->m_Reducer;
                }

                if (!st->m_IsScore) {
                    st->m_PlayBuffer->SetVolume(
                        Convert_HMI_To_Direct_Sound_Volume(g_LockedData.m_SoundVolume * (st->m_Volume / 128)) / 256);
                } else {
                    st->m_PlayBuffer->SetVolume(
                        Convert_HMI_To_Direct_Sound_Volume(g_LockedData.m_ScoreVolume * (st->m_Volume / 128)) / 256);
                }
            }

            ++st;
        }

        --g_LockedData.m_LockCount;
    }
#endif
}

/**
 * Initialise a LockedData struct.
 */
void Init_Locked_Data()
{
    g_LockedData.m_DigiHandle = INVALID_AUDIO_HANDLE;
    g_LockedData.m_ServiceSomething = 0;
    g_LockedData.m_MagicNumber = AUD_CHUNK_MAGIC_ID;
    g_LockedData.m_UncompBuffer = 0;
    g_LockedData.m_StreamBufferSize = BUFFER_CHUNK_SIZE + 128;
    g_LockedData.m_StreamBufferCount = STREAM_BUFFER_COUNT;
    g_LockedData.m_SoundVolume = VOLUME_MAX;
    g_LockedData.m_ScoreVolume = VOLUME_MAX;
    g_LockedData.m_LockCount = 0;
}

/**
 * Callback executed in sound callback on score(music) samples.
 */
BOOL File_Callback(int16_t id, int16_t *odd, void **buffer, int *size)
{
#ifdef BUILD_WITH_DSOUND
    if (id == INVALID_AUDIO_HANDLE) {
        return false;
    }

    SampleTrackerType *st = &g_LockedData.m_SampleTracker[id];

    if (st->m_FileBuffer == nullptr) {
        return false;
    }

    if (!g_AudioDone) {
        EnterCriticalSection(&g_GlobalAudioCriticalSection);
    }

    st->m_DontTouch = true;

    if (!g_AudioDone) {
        LeaveCriticalSection(&g_GlobalAudioCriticalSection);
    }

    if (*buffer == nullptr && st->m_FilePending) {
        *buffer = static_cast<char *>(st->m_FileBuffer)
            + g_LockedData.m_StreamBufferSize * (*odd % g_LockedData.m_StreamBufferCount);
        --st->m_FilePending;
        ++*odd;
        *size = st->m_FilePending == 0 ? st->m_FilePendingSize : g_LockedData.m_StreamBufferSize;
    }

    if (!g_AudioDone) {
        EnterCriticalSection(&g_GlobalAudioCriticalSection);
    }

    st->m_DontTouch = false;

    if (!g_AudioDone) {
        LeaveCriticalSection(&g_GlobalAudioCriticalSection);
    }

    Sound_Timer_Callback();

    int count = g_StreamLowImpact ? g_LockedData.m_StreamBufferCount / 2 : g_LockedData.m_StreamBufferCount - 3;

    if (count > st->m_FilePending && st->m_FileHandle != INVALID_FILE_HANDLE) {
        if (g_LockedData.m_StreamBufferCount - 2 != st->m_FilePending) {
            // Fill empty buffers.
            for (int num_empty_buffers = g_LockedData.m_StreamBufferCount - 2 - st->m_FilePending;
                 num_empty_buffers && st->m_FileHandle != INVALID_FILE_HANDLE;
                 --num_empty_buffers) {
                // Buffer to fill with data.
                void *tofill = static_cast<char *>(st->m_FileBuffer)
                    + g_LockedData.m_StreamBufferSize * ((st->m_FilePending + *odd) % g_LockedData.m_StreamBufferCount);

                int psize = Read_File(st->m_FileHandle, tofill, g_LockedData.m_StreamBufferSize);

                if (psize != g_LockedData.m_StreamBufferSize) {
                    Close_File(st->m_FileHandle);
                    st->m_FileHandle = INVALID_FILE_HANDLE;
                }

                if (psize > 0) {
                    if (!g_AudioDone) {
                        EnterCriticalSection(&g_GlobalAudioCriticalSection);
                    }

                    st->m_DontTouch = true;
                    st->m_FilePendingSize = psize;
                    ++st->m_FilePending;
                    st->m_DontTouch = false;

                    if (!g_AudioDone) {
                        LeaveCriticalSection(&g_GlobalAudioCriticalSection);
                    }

                    Sound_Timer_Callback();
                }
            }
        }

        if (!g_AudioDone) {
            EnterCriticalSection(&g_GlobalAudioCriticalSection);
        }

        st->m_DontTouch = true;

        if (st->m_QueueBuffer == nullptr && st->m_FilePending) {
            st->m_QueueBuffer = static_cast<char *>(st->m_FileBuffer)
                + g_LockedData.m_StreamBufferSize * (st->m_Odd % g_LockedData.m_StreamBufferCount);
            --st->m_FilePending;
            ++st->m_Odd;
            st->m_QueueSize = st->m_FilePending > 0 ? g_LockedData.m_StreamBufferSize : st->m_FilePendingSize;
        }

        st->m_DontTouch = false;

        if (!g_AudioDone) {
            LeaveCriticalSection(&g_GlobalAudioCriticalSection);
        }

        Sound_Timer_Callback();
    }

    if (st->m_FilePending) {
        return true;
    }

    return false;
#endif
}

/**
 * Starts playback of a score(music) sample in a specific tracker, at a specific volume.
 */
int __cdecl Stream_Sample_Vol(void *buffer, int size, AudioStreamCallback callback, int volume, int handle)
{
    if (g_AudioDone || buffer == nullptr || size == 0 || g_LockedData.m_DigiHandle == INVALID_AUDIO_HANDLE) {
        return INVALID_AUDIO_HANDLE;
    }

    AUDHeaderStruct header;
    memcpy(&header, buffer, sizeof(header));
    int oldsize = header.m_Size;
    header.m_Size = size - sizeof(header);
    memcpy(buffer, &header, sizeof(header));
    int playid = Play_Sample_Handle(buffer, PRIORITY_MAX, volume, 0, handle);
    header.m_Size = oldsize;
    memcpy(buffer, &header, sizeof(header));

    if (playid == INVALID_AUDIO_HANDLE) {
        return INVALID_AUDIO_HANDLE;
    }

    SampleTrackerType *st = &g_LockedData.m_SampleTracker[playid];
    st->m_Callback = callback;
    st->m_Odd = 0;

    return playid;
}

/**
 * Loads a sample and puts it into the sample tracker as score(music).
 */
int File_Stream_Sample(const char *filename, BOOL real_time_start)
{
    return File_Stream_Sample_Vol(filename, VOLUME_MAX, real_time_start);
}

/**
 * Preloads a sample for later playback.
 */
void File_Stream_Preload(int index)
{
#ifdef BUILD_WITH_DSOUND
    SampleTrackerType *st = &g_LockedData.m_SampleTracker[index];
    int maxnum = (g_LockedData.m_StreamBufferCount / 2) + 4;
    int num = st->m_Loading ? std::min(st->m_FilePending + 2, maxnum) : maxnum;

    int i = 0;

    for (i = st->m_FilePending; i < num; ++i) {
        int size = Read_File(st->m_FileHandle,
            static_cast<char *>(st->m_FileBuffer) + i * g_LockedData.m_StreamBufferSize,
            g_LockedData.m_StreamBufferSize);

        if (size > 0) {
            st->m_FilePendingSize = size;
            ++st->m_FilePending;
        }

        if (size < g_LockedData.m_StreamBufferSize) {
            break;
        }
    }

    Sound_Timer_Callback();

    if (g_LockedData.m_StreamBufferSize > st->m_FilePendingSize || i == maxnum) {
        int old_vol = g_LockedData.m_SoundVolume;

        int stream_size = st->m_FilePending == 1 ? st->m_FilePendingSize : g_LockedData.m_StreamBufferSize;

        g_LockedData.m_SoundVolume = g_LockedData.m_ScoreVolume;
        g_StartingFileStream = true;
        Stream_Sample_Vol(st->m_FileBuffer, stream_size, File_Callback, st->m_Volume, index);
        g_StartingFileStream = false;

        g_LockedData.m_SoundVolume = old_vol;

        st->m_Loading = false;
        --st->m_FilePending;

        if (st->m_FilePending == 0) {
            st->m_Odd = 0;
            st->m_QueueBuffer = 0;
            st->m_QueueSize = 0;
            st->m_FilePendingSize = 0;
            st->m_Callback = nullptr;
            Close_File(st->m_FileHandle);
        } else {
            st->m_Odd = 2;
            --st->m_FilePending;

            if (st->m_FilePendingSize != g_LockedData.m_StreamBufferSize) {
                Close_File(st->m_FileHandle);
                st->m_FileHandle = INVALID_FILE_HANDLE;
            }

            st->m_QueueBuffer = static_cast<char *>(st->m_FileBuffer) + g_LockedData.m_StreamBufferSize;
            st->m_QueueSize = st->m_FilePending == 0 ? st->m_FilePendingSize : g_LockedData.m_StreamBufferSize;
        }
    }
#endif
}

/**
 * Loads a sample and puts it into the sample tracker as score(music), sets a specific volume.
 */
int File_Stream_Sample_Vol(const char *filename, int volume, BOOL real_time_start)
{
    if (g_LockedData.m_DigiHandle == INVALID_AUDIO_HANDLE || filename == nullptr || !Find_File(filename)) {
        return INVALID_AUDIO_HANDLE;
    }

    if (g_FileStreamBuffer == nullptr) {
        g_FileStreamBuffer = Alloc(
            g_LockedData.m_StreamBufferSize * g_LockedData.m_StreamBufferCount, g_StreamBufferFlag | MEM_LOCK | MEM_TEMP);

        for (int i = 0; i < MAX_SAMPLE_TRACKERS; ++i) {
            g_LockedData.m_SampleTracker[i].m_FileBuffer = g_FileStreamBuffer;
        }
    }

    if (g_FileStreamBuffer == nullptr) {
        return INVALID_AUDIO_HANDLE;
    }

    int fh = Open_File(filename, 1);

    if (fh == INVALID_FILE_HANDLE) {
        return INVALID_AUDIO_HANDLE;
    }

    int handle = Get_Free_Sample_Handle(PRIORITY_MAX);

    if (handle < MAX_SAMPLE_TRACKERS) {
        SampleTrackerType *st = &g_LockedData.m_SampleTracker[handle];
        st->m_IsScore = true;
        st->m_FilePending = 0;
        st->m_FilePendingSize = 0;
        st->m_Loading = real_time_start;
        st->m_Volume = volume;
        st->m_FileHandle = fh;
        File_Stream_Preload(handle);
        return handle;
    }

    return INVALID_AUDIO_HANDLE;
}

/**
 * Callback executed each loop.
 */
void Sound_Callback()
{
#ifdef BUILD_WITH_DSOUND
    if (!g_AudioDone && g_LockedData.m_DigiHandle != INVALID_AUDIO_HANDLE) {
        Sound_Timer_Callback();

        for (int i = 0; i < MAX_SAMPLE_TRACKERS; ++i) {
            SampleTrackerType *st = &g_LockedData.m_SampleTracker[i];

            // Is a load pending?
            if (st->m_Loading) {
                File_Stream_Preload(i);
                // We are done with this sample.
                continue;
            }

            // Is this sample inactive?
            if (!st->m_Active) {
                // If so, we close the handle.
                if (st->m_FileHandle != INVALID_FILE_HANDLE) {
                    Close_File(st->m_FileHandle);
                    st->m_FileHandle = INVALID_FILE_HANDLE;
                }
                // We are done with this sample.
                continue;
            }

            // Has it been faded Is the volume 0?
            if (st->m_Reducer && !st->m_Volume) {
                // If so stop it.
                Stop_Sample(i);

                // We are done with this sample.
                continue;
            }

            // Process pending files.
            if (st->m_QueueBuffer == nullptr
                || st->m_FileHandle != INVALID_FILE_HANDLE && g_LockedData.m_StreamBufferCount - 3 > st->m_FilePending) {
                if (st->m_Callback != nullptr) {
                    if (!st->m_Callback(i, &st->m_Odd, &st->m_QueueBuffer, &st->m_QueueSize)) {
                        // No files are pending so pending file callback not needed anymore.
                        st->m_Callback = nullptr;
                    }
                }

                // We are done with this sample.
                continue;
            }
        }
    }
#endif
}

/**
 * Loads a sample from a file into memory.
 */
void *Load_Sample(char *filename)
{
    if (g_LockedData.m_DigiHandle == INVALID_AUDIO_HANDLE || filename == nullptr || !Find_File(filename)) {
        return nullptr;
    }

    void *data = nullptr;
    int handle = Open_File(filename, 1);

    if (handle != INVALID_FILE_HANDLE) {
        int data_size = File_Size(handle) + sizeof(AUDHeaderStruct);
        data = Alloc(data_size, MEM_NORMAL);

        if (data != nullptr) {
            Sample_Read(handle, data, data_size);
        }

        Close_File(handle);
        g_Misc = data_size;
    }

    return data;
}

/**
 * Opens and reads a sample into a fixed size buffer.
 */
int Load_Sample_Into_Buffer(char *filename, void *buffer, int size)
{
    if (buffer == nullptr || size == 0 || g_LockedData.m_DigiHandle == INVALID_AUDIO_HANDLE || !filename
        || !Find_File(filename)) {
        return 0;
    }

    int handle = Open_File(filename, 1);

    if (handle == INVALID_FILE_HANDLE) {
        return 0;
    }

    int sample_size = Sample_Read(handle, buffer, size);
    Close_File(handle);
    return sample_size;
}

/**
 * Reads a already open sample into a fixed size buffer.
 */
int Sample_Read(int handle, void *buffer, int size)
{
    if (buffer == nullptr || handle == INVALID_AUDIO_HANDLE || size <= sizeof(AUDHeaderStruct)) {
        return 0;
    }

    AUDHeaderStruct header;
    int actual_bytes_read = Read_File(handle, &header, sizeof(AUDHeaderStruct));
    int to_read = std::min<unsigned>(size - sizeof(AUDHeaderStruct), header.m_Size);

    actual_bytes_read += Read_File(handle, static_cast<char *>(buffer) + sizeof(AUDHeaderStruct), to_read);

    memcpy(buffer, &header, sizeof(AUDHeaderStruct));

    return actual_bytes_read;
}

/**
 * Free's a allocated sample. Allocation must be made with Alloc.
 */
void Free_Sample(void *sample)
{
    if (sample != nullptr) {
        Free(sample);
    }
}

/**
 * Callback executed on every timer event.
 */
#ifdef BUILD_WITH_DSOUND
void CALLBACK Sound_Timer_Callback(UINT uID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    if (!g_AudioDone) {
        EnterCriticalSection(&g_GlobalAudioCriticalSection);
        Maintenance_Callback();
        LeaveCriticalSection(&g_GlobalAudioCriticalSection);
    }
}
#endif

/**
 * Dedicated thread for sound.
 */
void Sound_Thread(void *a1)
{
#ifdef BUILD_WITH_DSOUND
    // TODO : Find a alternative solution, this is the original code, and likely causes lockups on modern systems.
    DuplicateHandle(
        GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &g_SoundThreadHandle, THREAD_ALL_ACCESS, true, 0);
    SetThreadPriority(g_SoundThreadHandle, 15);
    g_SoundThreadActive = true;

    while (!g_AudioDone) {
        EnterCriticalSection(&g_GlobalAudioCriticalSection);
        Maintenance_Callback();
        LeaveCriticalSection(&g_GlobalAudioCriticalSection);
        PlatformTimerClass::Sleep(TIMER_DELAY);
    }

    g_SoundThreadActive = false;
#endif
}

/**
 * Attempts to set primary buffer format.
 */
BOOL Set_Primary_Buffer_Format()
{
#ifdef BUILD_WITH_DSOUND
    if (g_SoundObject != nullptr && g_PrimaryBufferPtr != nullptr) {
        return g_PrimaryBufferPtr->SetFormat(&g_PrimaryBuffFormat) == DS_OK;
    }

    return false;
#endif
}

/**
 * Shows error that occured as a message box.
 */
int Print_Sound_Error(char *sound_error, void *window)
{
#ifdef BUILD_WITH_DSOUND
    return MessageBoxA((HWND)window, sound_error, "DirectSound Audio Error", MB_OK | MB_ICONWARNING);
#else
    return 0;
#endif
}

/**
 * Initializes audio engine.
 */
BOOL Audio_Init(void *window, int bits_per_sample, BOOL stereo, int rate, BOOL reverse_channels)
{
#ifdef BUILD_WITH_DSOUND
    Init_Locked_Data();
    g_FileStreamBuffer = nullptr;
    memset(g_LockedData.m_SampleTracker, 0, sizeof(g_LockedData.m_SampleTracker));

    // Audio already init'ed.
    if (g_SoundObject != nullptr) {
        return true;
    }

    HRESULT return_code = DirectSoundCreate(NULL, &g_SoundObject, NULL);

    if (return_code != DS_OK) {
        captainslog_debug("Audio_Init - Failed to create Direct Sound Object. Error code %d.", return_code);
        return false;
    }

    return_code = g_SoundObject->SetCooperativeLevel((HWND)window, DSSCL_PRIORITY);

    if (return_code != DS_OK) {
        captainslog_debug("Audio_Init - Unable to set Direct Sound cooperative level. Error code %d.", return_code);
        g_SoundObject->Release();
        g_SoundObject = nullptr;

        return false;
    }

    // Set up DSBUFFERDESC structure.
    memset(&g_BufferDesc, 0, sizeof(g_BufferDesc));
    g_BufferDesc.dwSize = sizeof(DSBUFFERDESC);
    g_BufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;

    // Set up wave format structure.
    memset(&g_DsBuffFormat, 0, sizeof(g_DsBuffFormat));
    g_DsBuffFormat.wFormatTag = WAVE_FORMAT_PCM;
    g_DsBuffFormat.nChannels = stereo + 1;
    g_DsBuffFormat.nSamplesPerSec = rate;
    g_DsBuffFormat.wBitsPerSample = bits_per_sample;
    g_DsBuffFormat.nBlockAlign = g_DsBuffFormat.nChannels * g_DsBuffFormat.wBitsPerSample / 8; // todo confirm the math
    g_DsBuffFormat.nAvgBytesPerSec = g_DsBuffFormat.nBlockAlign * g_DsBuffFormat.nSamplesPerSec;
    g_DsBuffFormat.cbSize = 0;

    memcpy(&g_PrimaryBufferDesc, &g_BufferDesc, sizeof(g_PrimaryBufferDesc));
    memcpy(&g_PrimaryBuffFormat, &g_DsBuffFormat, sizeof(g_PrimaryBuffFormat));

    return_code = g_SoundObject->CreateSoundBuffer(&g_PrimaryBufferDesc, &g_PrimaryBufferPtr, NULL);

    if (return_code != DS_OK) {
        captainslog_debug("Audio_Init - Failed to create the primary sound buffer. Error code %d.");
        g_SoundObject->Release();
        g_SoundObject = nullptr;

        return false;
    }

    // Attempt to allocate buffer.
    if (!Set_Primary_Buffer_Format()) {
        captainslog_debug("Audio_Init - Failed to set primary buffer format.");

        int old_bits_per_sample = g_DsBuffFormat.wBitsPerSample;
        int old_block_align = g_DsBuffFormat.nBlockAlign;
        int old_bytes_per_sec = g_DsBuffFormat.nAvgBytesPerSec;

        if (g_DsBuffFormat.wBitsPerSample == 16) {
            captainslog_debug("Audio_Init - Trying a 8-bit primary buffer format.");
            g_DsBuffFormat.wBitsPerSample = 8;
            g_DsBuffFormat.nBlockAlign = g_DsBuffFormat.nChannels;
            g_DsBuffFormat.nAvgBytesPerSec = g_DsBuffFormat.nChannels * g_DsBuffFormat.nSamplesPerSec;

            memcpy(&g_PrimaryBufferDesc, &g_BufferDesc, sizeof(g_PrimaryBufferDesc));
            memcpy(&g_PrimaryBuffFormat, &g_DsBuffFormat, sizeof(g_PrimaryBuffFormat));
        }

        // Attempt to allocate 8 bit buffer.
        if (!Set_Primary_Buffer_Format()) {
            g_PrimaryBufferPtr->Release();
            g_PrimaryBufferPtr = nullptr;

            g_SoundObject->Release();
            g_SoundObject = nullptr;

            captainslog_debug("Audio_Init - Failed to set any primary buffer format. Disabling audio.");

            return false;
        }

        // Restore original format settings.
        g_DsBuffFormat.wBitsPerSample = old_bits_per_sample;
        g_DsBuffFormat.nBlockAlign = old_block_align;
        g_DsBuffFormat.nAvgBytesPerSec = old_bytes_per_sec;
    }

    // Attempt to start playback.
    return_code = g_PrimaryBufferPtr->Play(0, 0, DSBPLAY_LOOPING);
    if (return_code != DS_OK) {
        captainslog_debug("Audio_Init - Failed to start primary sound buffer. Disabling audio. Error code %d.");

        g_PrimaryBufferPtr->Release();
        g_PrimaryBufferPtr = nullptr;

        g_SoundObject->Release();
        g_SoundObject = nullptr;

        return false;
    }

    g_LockedData.m_DigiHandle = 1;

    InitializeCriticalSection(&g_GlobalAudioCriticalSection);

    g_SoundTimerHandle = timeSetEvent(TIMER_DELAY, TIMER_RESOLUTION, Sound_Timer_Callback, 0, 1);

    g_BufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
    g_BufferDesc.dwBufferBytes = BUFFER_TOTAL_BYTES;

    g_BufferDesc.lpwfxFormat = &g_DsBuffFormat;

    g_LockedData.m_UncompBuffer = Alloc(UNCOMP_BUFFER_SIZE, MEM_LOCK | MEM_CLEAR);

    if (g_LockedData.m_UncompBuffer == nullptr) {
        captainslog_debug("Audio_Init - Failed to allocate UncompBuffer.");
        return false;
    }

    // Create placback buffers for all trackers.
    for (int i = 0; i < MAX_SAMPLE_TRACKERS; ++i) {
        SampleTrackerType *st = &g_LockedData.m_SampleTracker[i];

        return_code = g_SoundObject->CreateSoundBuffer(&g_BufferDesc, &st->m_PlayBuffer, NULL);

        if (return_code != DS_OK) {
            captainslog_debug("Audio_Init - Failed to allocate Play Buffer for tracker %d. Error code %d.", i, return_code);
        }

        st->m_PlaybackRate = rate;
        st->m_Stereo = stereo;
        st->m_BitSize = (bits_per_sample == 16 ? 2 : 0);
        st->m_FileHandle = INVALID_FILE_HANDLE;
        st->m_QueueBuffer = nullptr;
        InitializeCriticalSection(&st->m_CriticalSection);
    }

    g_SoundType = 1;
    g_SampleType = 1;
    g_ReverseChannels = reverse_channels;
    g_AudioDone = false;

    return true;
#else
    return false;
#endif
}

/**
 * Cleanup function when exiting the game.
 */
void Sound_End()
{
#ifdef BUILD_WITH_DSOUND
    if (g_SoundObject != nullptr && g_PrimaryBufferPtr != nullptr) {
        for (int i = 0; i < MAX_SAMPLE_TRACKERS; ++i) {
            if (g_LockedData.m_SampleTracker[i].m_PlayBuffer != nullptr) {
                Stop_Sample(i);
                g_LockedData.m_SampleTracker[i].m_PlayBuffer->Stop();
                g_LockedData.m_SampleTracker[i].m_PlayBuffer->Release();
                g_LockedData.m_SampleTracker[i].m_PlayBuffer = nullptr;
                DeleteCriticalSection(&g_LockedData.m_SampleTracker[i].m_CriticalSection);
            }
        }
    }

    if (g_FileStreamBuffer != nullptr) {
        Free(g_FileStreamBuffer);
        g_FileStreamBuffer = nullptr;
    }

    if (g_PrimaryBufferPtr != nullptr) {
        g_PrimaryBufferPtr->Stop();
        g_PrimaryBufferPtr->Release();
        g_PrimaryBufferPtr = nullptr;
    }

    if (g_SoundObject != nullptr) {
        g_SoundObject->Release();
        g_SoundObject = nullptr;
    }

    if (g_LockedData.m_UncompBuffer != nullptr) {
        Free(g_LockedData.m_UncompBuffer);
        g_LockedData.m_UncompBuffer = nullptr;
    }

    if (g_SoundTimerHandle != 0) {
        timeKillEvent(g_SoundTimerHandle);
        g_SoundTimerHandle = 0;
    }

    DeleteCriticalSection(&g_GlobalAudioCriticalSection);

    g_AudioDone = true;
#endif
}

/**
 * Stops a specific sample tracker.
 */
void Stop_Sample(int index)
{
#ifdef BUILD_WITH_DSOUND
    if (g_LockedData.m_DigiHandle != INVALID_AUDIO_HANDLE && index < MAX_SAMPLE_TRACKERS && !g_AudioDone) {
        EnterCriticalSection(&g_GlobalAudioCriticalSection);
        SampleTrackerType *st = &g_LockedData.m_SampleTracker[index];

        if (st->m_Active || st->m_Loading) {
            st->m_Active = false;

            if (!st->m_IsScore) {
                st->m_Original = nullptr;
            }

            st->m_Priority = 0;

            if (!st->m_Loading) {
                st->m_PlayBuffer->Stop();
            }

            st->m_Loading = false;

            if (st->m_FileHandle != INVALID_FILE_HANDLE) {
                Close_File(st->m_FileHandle);
                st->m_FileHandle = INVALID_FILE_HANDLE;
            }

            st->m_QueueBuffer = nullptr;
        }

        LeaveCriticalSection(&g_GlobalAudioCriticalSection);
    }
#endif
}

/**
 * Returns status of a specific sample.
 */
BOOL Sample_Status(int index)
{
#ifdef BUILD_WITH_DSOUND
    if (g_AudioDone) {
        return false;
    }

    if (g_LockedData.m_DigiHandle == INVALID_AUDIO_HANDLE || index >= MAX_SAMPLE_TRACKERS) {
        return false;
    }

    SampleTrackerType *st = &g_LockedData.m_SampleTracker[index];

    if (st->m_Loading) {
        return true;
    }

    if (!st->m_Active) {
        return false;
    }

    g_DumpBuffer = st->m_PlayBuffer;
    DWORD status;

    if (g_DumpBuffer->GetStatus(&status) != DS_OK) {
        captainslog_debug("Sample_Status - GetStatus failed");
        return true;
    }

    // original check, possible typo.
    // return (status & DSBSTATUS_PLAYING) || (status & DSBSTATUS_LOOPING);

    // Buffer has to be set as looping to qualify as playing.
    return (status & (DSBSTATUS_PLAYING | DSBSTATUS_LOOPING)) != 0;
#else
    return false;
#endif
}

/**
 * Is this sample currently playing?
 */
int Is_Sample_Playing(void *sample)
{
    if (g_AudioDone || sample == nullptr) {
        return false;
    }

    for (int i = 0; i < MAX_SAMPLE_TRACKERS; ++i) {
        if (sample == g_LockedData.m_SampleTracker[i].m_Original && Sample_Status(i)) {
            return true;
        }
    }

    return false;
}

/**
 * Stops playing a specific sample.
 */
void Stop_Sample_Playing(void *sample)
{
    if (sample != nullptr) {
        for (int i = 0; i < MAX_SAMPLE_TRACKERS; ++i) {
            if (g_LockedData.m_SampleTracker[i].m_Original == sample) {
                Stop_Sample(i);
                break;
            }
        }
    }
}

/**
 * Gets a usable sample tracker slot based on priority.
 */
int Get_Free_Sample_Handle(int priority)
{
    int index = 0;

    for (index = MAX_SAMPLE_TRACKERS - 1; index >= 0; --index) {
        if (!g_LockedData.m_SampleTracker[index].m_Active && !g_LockedData.m_SampleTracker[index].m_Loading) {
            if (g_StartingFileStream || !g_LockedData.m_SampleTracker[index].m_IsScore) {
                break;
            }

            g_StartingFileStream = true;
        }
    }

    if (index < 0) {
        for (index = 0; index < MAX_SAMPLE_TRACKERS && g_LockedData.m_SampleTracker[index].m_Priority > priority; ++index) {
            ;
        }

        if (index == MAX_SAMPLE_TRACKERS) {
            return INVALID_AUDIO_HANDLE;
        }

        Stop_Sample(index);
    }

    if (index == INVALID_AUDIO_HANDLE) {
        return INVALID_AUDIO_HANDLE;
    }

    if (g_LockedData.m_SampleTracker[index].m_FileHandle != INVALID_FILE_HANDLE) {
        Close_File(g_LockedData.m_SampleTracker[index].m_FileHandle);
        g_LockedData.m_SampleTracker[index].m_FileHandle = INVALID_FILE_HANDLE;
    }

    if (g_LockedData.m_SampleTracker[index].m_Original) {
        if (!g_LockedData.m_SampleTracker[index].m_IsScore) {
            g_LockedData.m_SampleTracker[index].m_Original = 0;
        }
    }

    g_LockedData.m_SampleTracker[index].m_IsScore = 0;
    return index;
}

/**
 * Queue's a sample to be played.
 */
int Play_Sample(void *sample, int priority, int volume, int16_t panloc)
{
    return Play_Sample_Handle(sample, priority, volume, panloc, Get_Free_Sample_Handle(priority));
}

/**
 * Attempts to restore sound buffer that was suspended on a focus loss.
 */
#ifdef BUILD_WITH_DSOUND
BOOL Attempt_Audio_Restore(LPDIRECTSOUNDBUFFER sound_buffer)
{
    HRESULT return_code = 0;
    DWORD play_status = 0;

    if (g_AudioDone || sound_buffer == nullptr) {
        return false;
    }

    if (g_AudioFocusLoss != nullptr) {
        g_AudioFocusLoss();
    }

    for (int restore_attempts = 0; restore_attempts < 2 && return_code == DSERR_BUFFERLOST; ++restore_attempts) {
        Restore_Sound_Buffers();
        HRESULT return_code = sound_buffer->GetStatus(&play_status);
    }

    return return_code != DSERR_BUFFERLOST;
}
#endif

/**
 * Attempts to play the buffer, returns id of the playing sample.
 */
int Attempt_To_Play_Buffer(int id)
{
#ifdef BUILD_WITH_DSOUND
    HRESULT return_code;
    SampleTrackerType *st = &g_LockedData.m_SampleTracker[id];

    // Attempts to play the current sample's buffer.
    do {
        return_code = st->m_PlayBuffer->Play(0, 0, 1);

        // Playback was started so we set some needed sample tracker values.
        if (return_code == DS_OK) {
            if (!g_AudioDone) {
                EnterCriticalSection(&g_GlobalAudioCriticalSection);
            }

            st->m_Active = true;
            st->m_DontTouch = false;
            st->m_Handle = id;

            if (!g_AudioDone) {
                LeaveCriticalSection(&g_GlobalAudioCriticalSection);
            }

            return st->m_Handle;
        }

        // A error we can't handle here occured.
        if (return_code != DSERR_BUFFERLOST) {
            // Flag this sample as not active.
            st->m_Active = false;
            break;
        }

        // We got a DSERR_BUFFERLOST so attempting to restore and if able to trying again.
        if (!Attempt_Audio_Restore(st->m_PlayBuffer)) {
            break;
        }
    } while (return_code == DSERR_BUFFERLOST);
#endif
    return INVALID_AUDIO_HANDLE;
}

/**
 * Converts volume this audio engine uses to DirectSound decibels.
 */
int Convert_HMI_To_Direct_Sound_Volume(int vol)
{
#ifdef BUILD_WITH_DSOUND
    // Complete silence.
    if (vol <= 0) {
        return DSBVOLUME_MIN;
    }

    // Max volume.
    if (vol >= 255) {
        return DSBVOLUME_MAX;
    }

    // Dark magic.
    double v = exp((255.0 - vol) * log(double(10000 + 1)) / 255.0);

    // Simple clamping. 10000.99 would clamp to 9999.99.
    // Flip the value as we need a inverted value for DirectSound.
    return -(v + -1.0);
#endif
}

/**
 * Plays a loaded sample using a specific sample tracker.
 */
int Play_Sample_Handle(void *sample, int priority, int volume, int16_t panloc, int id)
{
#ifdef BUILD_WITH_DSOUND
    HRESULT return_code;
    DWORD status;

    if (g_SeenBuff.Get_LockCount() != 0 || g_HidPage.Get_LockCount() != 0) {
        // BUGFIX: Original returned 0 which im pretty sure could be a valid handle.
        return INVALID_AUDIO_HANDLE;
    }

    if (!g_AudioDone) {
        if (sample == nullptr || g_LockedData.m_DigiHandle == INVALID_AUDIO_HANDLE) {
            return INVALID_AUDIO_HANDLE;
        }

        if (id == INVALID_AUDIO_HANDLE) {
            return INVALID_AUDIO_HANDLE;
        }

        SampleTrackerType *st = &g_LockedData.m_SampleTracker[id];

        // Read in the sample's header.
        AUDHeaderStruct raw_header;
        memcpy(&raw_header, sample, sizeof(raw_header));

        // We don't support anything lower than 20000 hz.
        if (raw_header.m_Rate < 24000 && raw_header.m_Rate > 20000) {
            raw_header.m_Rate = 22050;
        }

        if (!g_AudioDone) {
            EnterCriticalSection(&g_GlobalAudioCriticalSection);
        }

        // Set up basic sample tracker info.
        st->m_Compression = SoundCompressType(raw_header.m_Compression);
        st->m_Original = sample;
        st->m_DontTouch = true;
        st->m_Odd = 0;
        st->m_Reducer = 0;
        st->m_Restart = 0;
        st->m_QueueBuffer = 0;
        st->m_QueueSize = 0;
        st->m_TrailerLen = 0;
        st->m_OriginalSize = raw_header.m_Size + sizeof(AUDHeaderStruct);
        st->m_Priority = priority;
        st->m_Service = 0;
        st->m_Remainder = raw_header.m_Size;
        st->m_Source = static_cast<char *>(sample) + sizeof(AUDHeaderStruct);

        if (!g_AudioDone) {
            LeaveCriticalSection(&g_GlobalAudioCriticalSection);
        }

        // Compression is ADPCM so we need to init it's stream info.
        if (st->m_Compression == COMP_ADPCM) {
            st->m_StreamInfo.m_Channels = (raw_header.m_Flags & 1) + 1;
            st->m_StreamInfo.m_BitsPerSample = raw_header.m_Flags & 2 ? 16 : 8;
            st->m_StreamInfo.m_CompSize = raw_header.m_Size;
            st->m_StreamInfo.m_UnCompSize = raw_header.m_Size * (st->m_StreamInfo.m_BitsPerSample / 4);
            ADPCM_Stream_Init(&st->m_StreamInfo);
        }

        // If the loaded sample doesn't match the sample tracker we need to adjust the tracker.
        if (raw_header.m_Rate != st->m_PlaybackRate || (raw_header.m_Flags & 2) != (st->m_BitSize & 2)
            || (raw_header.m_Flags & 1) != (st->m_Stereo & 1)) {
            captainslog_debug("Play_Sample_Handle - Changing sample format.");
            st->m_Active = false;
            st->m_Service = 0;
            st->m_MoreSource = false;
            g_DumpBuffer = st->m_PlayBuffer;

            // Querry the playback status.
            do {
                return_code = st->m_PlayBuffer->GetStatus(&status);
                if (return_code == DSERR_BUFFERLOST && !Attempt_Audio_Restore(st->m_PlayBuffer)) {
                    captainslog_debug("Play_Sample_Handle - Unable to get PlaybBuffer status!");
                    return INVALID_AUDIO_HANDLE;
                }
            } while (return_code == DSERR_BUFFERLOST);

            // Stop the sample if its already playing.
            // TODO: Investigate this, logics here are possibly wrong.
            // - What happens when we call Restore when we have stopped the the buffer?
            // - Stop return isn't checked, in TS it checks for DSERR_BUFFERLOST, but thats not a valid Stop return.
            if (status & (DSBSTATUS_PLAYING | DSBSTATUS_LOOPING)
                && (st->m_PlayBuffer->Stop(), return_code == DSERR_BUFFERLOST) && !Attempt_Audio_Restore(st->m_PlayBuffer)) {
                captainslog_debug("Play_Sample_Handle - Unable to stop buffer!");
                return INVALID_AUDIO_HANDLE;
            }

            st->m_PlayBuffer->Release();
            st->m_PlayBuffer = nullptr;
            g_DsBuffFormat.nSamplesPerSec = raw_header.m_Rate;
            g_DsBuffFormat.nChannels = (raw_header.m_Flags & 1) + 1;
            g_DsBuffFormat.wBitsPerSample = raw_header.m_Flags & 2 ? 16 : 8;
            g_DsBuffFormat.nBlockAlign = g_DsBuffFormat.nChannels * g_DsBuffFormat.wBitsPerSample / 8;
            g_DsBuffFormat.nAvgBytesPerSec = g_DsBuffFormat.nBlockAlign * g_DsBuffFormat.nSamplesPerSec;

            // Attempt to create a new buffer for this new sample.
            return_code = g_SoundObject->CreateSoundBuffer(&g_BufferDesc, &st->m_PlayBuffer, 0);

            if (return_code == DSERR_BUFFERLOST && !Attempt_Audio_Restore(st->m_PlayBuffer)) {
                return INVALID_AUDIO_HANDLE;
            }

            // We failed to create the buffer, bail!
            if (return_code != DS_OK && return_code != DSERR_BUFFERLOST) {
                st->m_PlaybackRate = 0;
                st->m_Stereo = false;
                st->m_BitSize = 0;
                captainslog_debug("Play_Sample_Handle - Bad sample format!");
                return INVALID_AUDIO_HANDLE;
            }

            // Set the new sample info.
            st->m_PlaybackRate = raw_header.m_Rate;
            st->m_Stereo = raw_header.m_Flags & 1;
            st->m_BitSize = raw_header.m_Flags & 2;
        }

        // If the sample tracker matches the loaded file we load the samples.
        do {
            g_DumpBuffer = st->m_PlayBuffer;
            return_code = g_DumpBuffer->GetStatus(&status);

            if (return_code == DSERR_BUFFERLOST && !Attempt_Audio_Restore(st->m_PlayBuffer)) {
                captainslog_debug("Play_Sample_Handle - Can't get DumpBuffer status.");
                return INVALID_AUDIO_HANDLE;
            }
        } while (return_code == DSERR_BUFFERLOST);

        // If the sample is already playing stop it.
        if (status & (DSBSTATUS_PLAYING | DSBSTATUS_LOOPING)) {
            st->m_Active = false;
            st->m_Service = 0;
            st->m_MoreSource = false;
            st->m_PlayBuffer->Stop();
        }

        LPVOID play_buffer_ptr;
        DWORD lock_length1;
        LPVOID dummy_buffer_ptr;
        DWORD lock_length2;

        do {
            return_code = st->m_PlayBuffer->Lock(
                0, BUFFER_TOTAL_BYTES, &play_buffer_ptr, &lock_length1, &dummy_buffer_ptr, &lock_length2, 0);

            if (return_code == DSERR_BUFFERLOST && !Attempt_Audio_Restore(st->m_PlayBuffer)) {
                captainslog_debug("Play_Sample_Handle - Unable to lock buffer! Buffer lost.");
                return INVALID_AUDIO_HANDLE;
            }
        } while (return_code == DSERR_BUFFERLOST);

        if (return_code != DS_OK) {
            captainslog_debug("Play_Sample_Handle - Unable to lock buffer! Unknown error.");
            return INVALID_AUDIO_HANDLE;
        }

        st->m_DestPtr = Sample_Copy(st,
            &st->m_Source,
            &st->m_Remainder,
            &st->m_QueueBuffer,
            &st->m_QueueSize,
            play_buffer_ptr,
            BUFFER_CHUNK_SIZE,
            st->m_Compression,
            st->m_Trailer,
            &st->m_TrailerLen);

        if (st->m_DestPtr == BUFFER_CHUNK_SIZE) {
            st->m_MoreSource = true;
            st->m_Service = 1;
            st->m_OneShot = false;
        } else {
            st->m_MoreSource = false;
            st->m_OneShot = true;
            st->m_Service = 1;
            memset(static_cast<char *>(play_buffer_ptr) + st->m_DestPtr, 0, BUFFER_CHUNK_SIZE);
        }

        st->m_PlayBuffer->Unlock(play_buffer_ptr, lock_length1, dummy_buffer_ptr, lock_length2);
        st->m_Volume = volume * 128;

        do {
            st->m_PlayBuffer->SetVolume(Convert_HMI_To_Direct_Sound_Volume((volume * g_LockedData.m_SoundVolume) / 256));

            if (return_code == DSERR_BUFFERLOST && !Attempt_Audio_Restore(st->m_PlayBuffer)) {
                captainslog_debug("Play_Sample_Handle - Can't set volume!");
                return INVALID_AUDIO_HANDLE;
            }
        } while (return_code == DSERR_BUFFERLOST);

        if (!Start_Primary_Sound_Buffer(false)) {
            captainslog_debug("Play_Sample_Handle - Can't start primary buffer!");
            return INVALID_AUDIO_HANDLE;
        }

        // Reset buffer playback position.
        do {
            return_code = st->m_PlayBuffer->SetCurrentPosition(0);

            if (return_code == DSERR_BUFFERLOST && !Attempt_Audio_Restore(st->m_PlayBuffer)) {
                captainslog_debug("Play_Sample_Handle - Can't set current position!");
                return INVALID_AUDIO_HANDLE;
            }
        } while (return_code == DSERR_BUFFERLOST);

        return Attempt_To_Play_Buffer(id);
    }
#endif
    return INVALID_AUDIO_HANDLE;
}

/**
 * Restores primary and all sample tracker sound buffers lost on a focus loss.
 */
void Restore_Sound_Buffers()
{
#ifdef BUILD_WITH_DSOUND
    if (g_PrimaryBufferPtr != nullptr) {
        g_PrimaryBufferPtr->Restore();
    }

    for (int i = 0; i < MAX_SAMPLE_TRACKERS; ++i) {
        if (g_LockedData.m_SampleTracker[i].m_PlayBuffer != nullptr) {
            g_LockedData.m_SampleTracker[i].m_PlayBuffer->Restore();
        }
    }
#endif
}

/**
 * Sets regular sound volume.
 */
int Set_Sound_Vol(int vol)
{
    int oldvol = g_LockedData.m_SoundVolume;
    g_LockedData.m_SoundVolume = vol;
    return oldvol;
}

/**
 * Sets score(music) volume.
 */
int Set_Score_Vol(int volume)
{
    int old = g_LockedData.m_ScoreVolume;
    g_LockedData.m_ScoreVolume = volume;
#ifdef BUILD_WITH_DSOUND
    for (int i = 0; i < MAX_SAMPLE_TRACKERS; ++i) {
        SampleTrackerType *st = &g_LockedData.m_SampleTracker[i];

        if (st->m_IsScore & st->m_Active) {
            st->m_PlayBuffer->SetVolume(
                Convert_HMI_To_Direct_Sound_Volume((g_LockedData.m_ScoreVolume * (st->m_Volume / 128)) / 256));
        }
    }
#endif
    return old;
}

/**
 * Gradual fade a sample.
 */
void Fade_Sample(int index, int ticks)
{
    if (Sample_Status(index)) {
        SampleTrackerType *st = &g_LockedData.m_SampleTracker[index];

        if (ticks > 0 && !st->m_Loading) {
            st->m_Reducer = ((st->m_Volume / ticks) + 1);
        } else {
            Stop_Sample(index);
        }
    }
}

/**
 * Gradual unfade a faded sample.
 */
void Unfade_Sample(int index, int ticks)
{
    if (Sample_Status(index)) {
        SampleTrackerType *st = &g_LockedData.m_SampleTracker[index];

        if (ticks > 0 && !st->m_Loading) {
            st->m_Reducer -= ((st->m_Volume / ticks) + 1);
        } else {
            st->m_Reducer = 0;
        }
    }
}

/**
 *
 */
int Get_Digi_Handle()
{
    return g_LockedData.m_DigiHandle;
}

/**
 * Determines sample length.
 */
unsigned Sample_Length(void *sample)
{
    if (sample == nullptr) {
        return 0;
    }

    AUDHeaderStruct header;
    memcpy(&header, sample, sizeof(header));
    unsigned time = header.m_UncompSize;

    if (header.m_Flags & 2) {
        time /= 2;
    }

    if (header.m_Flags & 1) {
        time /= 2;
    }

    if (header.m_Rate / 60 > 0) {
        time /= header.m_Rate / 60;
    }

    return time;
}

/**
 * Starts playback of the primary buffer.
 */
int Start_Primary_Sound_Buffer(BOOL forced)
{
#ifdef BUILD_WITH_DSOUND
    if (g_PrimaryBufferPtr == nullptr || !g_GameInFocus) {
        return false;
    }

    if (forced) {
        g_PrimaryBufferPtr->Play(0, 0, DSBPLAY_LOOPING);
        return true;
    }

    DWORD status = 0;

    if (g_PrimaryBufferPtr->GetStatus(&status) != DS_OK) {
        return false;
    }

    // Primary buffer has to be set as looping to qualify as playing.
    if (status & DSBSTATUS_PLAYING | DSBSTATUS_LOOPING) {
        return true;
    }

    g_PrimaryBufferPtr->Play(0, 0, DSBPLAY_LOOPING);
    return true;
#else
    return false;
#endif
}

/**
 * Stops playback of the primary buffer and associated samples.
 */
void Stop_Primary_Sound_Buffer()
{
    /*
    if (g_PrimaryBufferPtr != nullptr) {
        // why
        g_PrimaryBufferPtr->Stop();
        g_PrimaryBufferPtr->Stop();
        g_PrimaryBufferPtr->Stop();
        g_PrimaryBufferPtr->Stop();
    }

    for (int i = 0; i < MAX_SAMPLE_TRACKERS; ++i) {
        Stop_Sample(i);
    }
    */

    // BUGFIX: Original code is above, with redundant stop calls and before samples are stopped?
    for (int i = 0; i < MAX_SAMPLE_TRACKERS; ++i) {
        Stop_Sample(i);
    }
#ifdef BUILD_WITH_DSOUND
    if (g_PrimaryBufferPtr != nullptr) {
        g_PrimaryBufferPtr->Stop();
    }
#endif
}

/**
 * Suspends a active audio thread.
 */
void Suspend_Audio_Thread()
{
#ifdef BUILD_WITH_DSOUND
    if (g_SoundThreadActive) {
        timeKillEvent(g_SoundTimerHandle);
        g_SoundTimerHandle = 0;
        g_SoundThreadActive = false;
    }
#endif
}

/**
 * Resumes a suspended audio thread.
 */
void Resume_Audio_Thread()
{
#ifdef BUILD_WITH_DSOUND
    if (!g_SoundThreadActive) {
        g_SoundTimerHandle = timeSetEvent(TIMER_DELAY, TIMER_RESOLUTION, Sound_Timer_Callback, g_SoundThreadActive, 1);
        g_SoundThreadActive = true;
    }
#endif
}

/**
 * Simple copies one buffer to another.
 */
int Simple_Copy(void **source, int *ssize, void **alternate, int *altsize, void **dest, int size)
{
    int out = 0;

    if (*ssize == 0) {
        *source = *alternate;
        *ssize = *altsize;
        *alternate = nullptr;
        *altsize = 0;
    }

    if (*source == nullptr || *ssize == 0) {
        return out;
    }

    int s = size;

    if (*ssize < size) {
        s = *ssize;
    }

    memcpy(*dest, *source, s);
    *source = static_cast<char *>(*source) + s;
    *ssize -= s;
    *dest = static_cast<char *>(*dest) + s;
    out = s;

    if ((size - s) == 0) {
        return out;
    }

    *source = *alternate;
    *ssize = *altsize;
    *alternate = nullptr;
    *altsize = 0;

    out = Simple_Copy(source, ssize, alternate, altsize, dest, (size - s)) + s;

    return out;
}

/**
 * Copies one buffer to another, decompressing if needed.
 */
int Sample_Copy(SampleTrackerType *st, void **source, int *ssize, void **alternate, int *altsize, void *dest, int size,
    SoundCompressType scomp, void *trailer, int16_t *trailersize)
{
    int datasize = 0;

    // There is no compression or it doesn't match any of the supported compressions so we just copy the data over.
    if (scomp == COMP_NONE || (scomp != COMP_ZAP && scomp != COMP_ADPCM)) {
        return Simple_Copy(source, ssize, alternate, altsize, &dest, size);
    }

    ADPCMStreamType *s = &st->m_StreamInfo;

    while (size > 0) {
        uint16_t fsize;
        uint16_t dsize;
        unsigned magic;

        void *fptr = &fsize;
        void *dptr = &dsize;
        void *mptr = &magic;

        // Verify and seek over the chunk header.
        if (Simple_Copy(source, ssize, alternate, altsize, &fptr, sizeof(fsize)) < sizeof(fsize)) {
            break;
        }

        if (Simple_Copy(source, ssize, alternate, altsize, &dptr, sizeof(dsize)) < sizeof(dsize) || dsize > size) {
            break;
        }

        if (Simple_Copy(source, ssize, alternate, altsize, &mptr, sizeof(magic)) < sizeof(magic)
            || magic != g_LockedData.m_MagicNumber) {
            break;
        }

        if (fsize == dsize) {
            // File size matches size to decompress, so there's nothing to do other than copy the buffer over.
            if (Simple_Copy(source, ssize, alternate, altsize, &dest, fsize) < dsize) {
                return datasize;
            }
        } else {
            // Else we need to decompress it.
            void *uptr = g_LockedData.m_UncompBuffer;

            if (Simple_Copy(source, ssize, alternate, altsize, &uptr, fsize) < fsize) {
                return datasize;
            }

            if (scomp == COMP_ZAP) {
                Audio_Unzap(g_LockedData.m_UncompBuffer, dest, dsize);
            } else {
                s->m_Source = g_LockedData.m_UncompBuffer;
                s->m_Dest = dest;

                ADPCM_Decompress(s, dsize);
            }

            dest = reinterpret_cast<char *>(dest) + dsize;
        }

        datasize += dsize;
        size -= dsize;
    }

    return datasize;
}
