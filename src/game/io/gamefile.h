/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief  FileClass layer for reading files in mix archives.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef GAMEFILECLASS_H
#define GAMEFILECLASS_H

#include "buffer.h"
#include "cdfile.h"
#include "mixfile.h"

// Forward declaration for typedef below.
class GameFileClass;

typedef MixFileClass<GameFileClass> GameMixFile;

class GameFileClass : public CDFileClass
{
public:
    GameFileClass() : m_fileBuffer() {}
    GameFileClass(const char *filename);
    virtual ~GameFileClass() {};

    virtual BOOL Is_Available(BOOL forced = false) override;
    virtual BOOL Is_Open() const override;
    virtual BOOL Open(const char *filename, int rights = FM_READ) override;
    virtual BOOL Open(int rights = FM_READ) override;
    virtual int Read(void *buffer, int length) override;
    virtual off_t Seek(off_t offset, int whence) override;
    virtual off_t Size() override;
    virtual int Write(const void *buffer, int length) override;
    virtual void Close() override;
    virtual time_t Get_Date_Time() override;
    virtual BOOL Set_Date_Time(time_t date_time) override;
    virtual void Error(int error, BOOL can_retry = false, const char *filename = nullptr) override;

    BOOL Is_Cached() const { return m_fileBuffer.Get_Buffer() != nullptr; }
    void Reset_Cache_Buffer() { m_fileBuffer.Reset(); m_cachePosition = 0; }

    static void *Retrieve(char const *filename);
    static int Retrieve_Size(char const *filename);
    static GameMixFile *Retrieve_Mix(char const *mix_filename);
    static bool File_Available(char const *filename);
    static GameMixFile *Allocate_Mix(char const *filename);
    static bool Cache_Mix(char const *filename);

#ifndef CHRONOSHIFT_STANDALONE
    static void Hook_Me();
    static BOOL Hook_Is_Available(GameFileClass *ptr, BOOL forced);
    static BOOL Hook_Is_Open(GameFileClass *ptr);
    static BOOL Hook_Open_Name(GameFileClass *ptr, const char *filename, int rights);
    static BOOL Hook_Open(GameFileClass *ptr, int rights);
    static int Hook_Read(GameFileClass *ptr, void *buffer, int length);
    static off_t Hook_Seek(GameFileClass *ptr, off_t offset, int whence);
    static off_t Hook_Size(GameFileClass *ptr);
    static int Hook_Write(GameFileClass *ptr, const void *buffer, int length);
    static void Hook_Close(GameFileClass *ptr);
    static time_t Hook_Get_Date_Time(GameFileClass *ptr);
    static BOOL Hook_Set_Date_Time(GameFileClass *ptr, time_t date_time);
    static void Hook_Error(GameFileClass *ptr, int error, BOOL can_retry, const char *filename);
#endif
private:
    BufferClass m_fileBuffer;
    int m_cachePosition;
};

// Wrappers for old stype IO.
int Open_File(const char *filename, int mode);
void Close_File(int handle);
int Read_File(int handle, void *buffer, unsigned length);
int Write_File(int handle, const void *buffer, unsigned length);
unsigned File_Size(int handle);
unsigned Seek_File(int handle, off_t offset, int whence);
int Find_File(const char *filename);
void *Hires_Load(const char *filename);
void *Load_Alloc_Data(FileClass &file);
void *Load_Alloc_Data(const char *filename, int mode);

#endif // GAMEFILECLASS_H
