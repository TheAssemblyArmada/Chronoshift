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
#include "ccfileclass.h"
#include "cd.h"
#include "minmax.h"
#include "mixfile.h"
#include "startup.h"
#include <new>
#include <stdio.h>
#include <string.h>

#define CCFILE_HANDLE_COUNT 10

#ifndef CHRONOSHIFT_STANDALONE
GameFileClass *g_handles = Make_Pointer<GameFileClass>(0x00635BE4);
#else
GameFileClass g_handles[CCFILE_HANDLE_COUNT];
#endif

typedef MixFileClass<GameFileClass> CCMixFileClass;

GameFileClass::GameFileClass(const char *filename) : m_fileBuffer()
{
    CDFileClass::Set_Name(filename);
}

void GameFileClass::Error(int error, BOOL can_retry, const char *filename)
{
    RawFileClass::Error(error, can_retry, filename);

    if (!Force_CD_Available(g_requiredCD)) {
        Emergency_Exit(0xFF);
    }
}

int GameFileClass::Write(const void *buffer, int length)
{
    // if the buffer pointer is null, raise an error.
    if (Is_Cached()) {
        // raise an error.
        Error(13, 0, File_Name());
    }

    return BufferIOFileClass::Write(buffer, length);
}

int GameFileClass::Read(void *buffer, int length)
{
    BOOL opened = false;

    if (!Is_Open() && Open(FM_READ)) {
        opened = true;
    }

    if (Is_Cached()) {
        length = Min((int)m_fileBuffer.Get_Size() - m_cachePosition, length);

        if (length > 0) {
            memmove(buffer, (m_fileBuffer.Get_Buffer() + m_cachePosition), length);
            m_cachePosition += length;
        }

        if (opened) {
            Close();
        }

        return length;
    }

    length = BufferIOFileClass::Read(buffer, length);

    if (opened) {
        Close();
    }

    return length;
}

off_t GameFileClass::Seek(off_t offset, int whence)
{
    if (Is_Cached()) {
        switch (whence) {
            case FS_SEEK_START:
                m_cachePosition = 0;
                break;

            case FS_SEEK_CURRENT:
                break;

            case FS_SEEK_END:
                m_cachePosition = m_fileBuffer.Get_Size();
                break;
        }

        m_cachePosition = Min(Max(int(offset + m_cachePosition), 0), m_fileBuffer.Get_Size());

        return m_cachePosition;
    }

    return BufferIOFileClass::Seek(offset, whence);
}

off_t GameFileClass::Size()
{
    // if Buffer is valid, return buffer Size.
    if (Is_Cached()) {
        return m_fileBuffer.Get_Size();
    }

    // if BufferIO is available, return Size().
    if (BufferIOFileClass::Is_Available()) {
        return BufferIOFileClass::Size();
    }

    int filesize = 0;
    CCMixFileClass::Offset(File_Name(), nullptr, nullptr, nullptr, &filesize);

    return filesize;
}

BOOL GameFileClass::Is_Available(BOOL forced)
{
    if (Is_Open()) {
        return true;
    }

    if (CCMixFileClass::Offset(File_Name())) {
        return true;
    }

    return BufferIOFileClass::Is_Available(forced);
}

BOOL GameFileClass::Is_Open() const
{
    // if the buffer pointer is set, return true.
    if (Is_Cached()) {
        return true;
    }

    return BufferIOFileClass::Is_Open();
}

void GameFileClass::Close()
{
    m_fileBuffer.Reset();
    m_cachePosition = 0;

    return BufferIOFileClass::Close();
}

BOOL GameFileClass::Open(const char *filename, int rights)
{
    Set_Name(filename);
    return Open(rights);
}

BOOL GameFileClass::Open(int mode)
{
    Close();

    int filesize = 0;
    int fileoffset = 0;
    CCMixFileClass *mixfile = nullptr;
    void *cachedfile = nullptr;

    if ((mode & FM_WRITE) || BufferIOFileClass::Is_Available()
        || !CCMixFileClass::Offset(File_Name(), &cachedfile, &mixfile, &fileoffset, &filesize)) {
        return CDFileClass::Open(mode);
    }

    DEBUG_ASSERT(mixfile != nullptr);

    if (cachedfile == nullptr && mixfile != nullptr) {
        char *tmpfilename = strdup(File_Name());

        Open(mixfile->Get_Filename(), FM_READ);

        Disable_Search_Drives(false);
        Set_Name(tmpfilename);
        Disable_Search_Drives(true);
        free((void *)tmpfilename);

        Bias(0);
        Bias(fileoffset, filesize);

        Seek(0, FS_SEEK_START);
    } else {
        if (m_fileBuffer.Get_Buffer() == nullptr) {
            new(&m_fileBuffer) BufferClass(cachedfile, filesize);
        }

        m_cachePosition = 0;
    }

    return true;
}

time_t GameFileClass::Get_Date_Time()
{
    CCMixFileClass *mixfile = nullptr;

    if (!CCMixFileClass::Offset(File_Name(), nullptr, &mixfile)) {
        return RawFileClass::Get_Date_Time();
    }

    GameFileClass tmpfile(mixfile->Get_Filename());

    return tmpfile.Get_Date_Time();
}

BOOL GameFileClass::Set_Date_Time(time_t date_time)
{
    BOOL retval;
    CCMixFileClass *mixfile = nullptr;

    if (((retval = RawFileClass::Set_Date_Time(date_time)) != false || !CCMixFileClass::Offset(File_Name(), nullptr, &mixfile))) {
        return retval;
    }

    GameFileClass tmpfile(mixfile->Get_Filename());

    return tmpfile.Set_Date_Time(date_time);
}

int Open_File(const char *filename, int mode)
{
    int handle = 0;

    while (g_handles[handle].Is_Open()) {
        ++handle;

        if (handle >= CCFILE_HANDLE_COUNT) {
            return -1;
        }
    }

    if (!g_handles[handle].Open(filename)) {
        return -1;
    }

    return handle;
}

void Close_File(int handle)
{
    if (handle != -1) {
        g_handles[handle].Close();
    }
}

int Read_File(int handle, void *buffer, unsigned length)
{
    if (handle != -1 && g_handles[handle].Is_Open()) {
        return g_handles[handle].Read(buffer, length);
    }

    return 0;
}

int Write_File(int handle, const void *buffer, unsigned length)
{
    if (handle != -1 && g_handles[handle].Is_Open()) {
        return g_handles[handle].Write(buffer, length);
    }

    return 0;
}

unsigned File_Size(int handle)
{
    if (handle != -1 && g_handles[handle].Is_Open()) {
        return g_handles[handle].Size();
    }

    return 0;
}

unsigned Seek_File(int handle, off_t offset, int whence)
{
    if (handle != -1 && g_handles[handle].Is_Open()) {
        return g_handles[handle].Seek(offset, whence);
    }

    return 0;
}

int Find_File(const char *filename)
{
    GameFileClass fc(filename);

    return fc.Is_Available();
}

void *Hires_Load(const char *filename)
{
    static char _hires_filename[32];
    snprintf(_hires_filename, sizeof(_hires_filename), "H%s", filename);

    GameFileClass file(_hires_filename);

    return Load_Alloc_Data(file);
}

void *Load_Alloc_Data(const char *filename, int mode)
{
    GameFileClass file(filename);

    return Load_Alloc_Data(file);
}

void *Load_Alloc_Data(FileClass &file)
{
    if (file.Is_Available()) {
        void *data = new char[file.Size()];
        file.Read(data, file.Size());

        return data;
    }

    return nullptr;
}

#ifndef CHRONOSHIFT_STANDALONE

void GameFileClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x00462A30, &Hook_Is_Available);
    Hook_Function(0x00462A80, &Hook_Is_Open);
    Hook_Function(0x00426400, &Hook_Open_Name);
    Hook_Function(0x00462AD4, &Hook_Open);
    Hook_Function(0x004628B0, &Hook_Read);
    Hook_Function(0x00462958, &Hook_Seek);
    Hook_Function(0x004629CC, &Hook_Size);
    Hook_Function(0x00462860, &Hook_Write);
    Hook_Function(0x00462AA8, &Hook_Close);
    Hook_Function(0x00462BD8, &Hook_Get_Date_Time);
    Hook_Function(0x00462C50, &Hook_Set_Date_Time);
    Hook_Function(0x00462840, &Hook_Error);
#endif
}

BOOL GameFileClass::Hook_Is_Available(GameFileClass *ptr, BOOL forced)
{
    return ptr->GameFileClass::Is_Available(forced);
}

BOOL GameFileClass::Hook_Is_Open(GameFileClass *ptr)
{
    return ptr->GameFileClass::Is_Open();
}

BOOL GameFileClass::Hook_Open_Name(GameFileClass *ptr, const char *filename, int rights)
{
    return ptr->GameFileClass::Open(filename, rights);
}

BOOL GameFileClass::Hook_Open(GameFileClass *ptr, int rights)
{
    return ptr->GameFileClass::Open(rights);
}

int GameFileClass::Hook_Read(GameFileClass *ptr, void *buffer, int length)
{
    return ptr->GameFileClass::Read(buffer, length);
}

off_t GameFileClass::Hook_Seek(GameFileClass *ptr, off_t offset, int whence)
{
    return ptr->GameFileClass::Seek(offset, whence);
}

off_t GameFileClass::Hook_Size(GameFileClass *ptr)
{
    return ptr->GameFileClass::Size();
}

int GameFileClass::Hook_Write(GameFileClass *ptr, const void *buffer, int length)
{
    return ptr->GameFileClass::Write(buffer, length);
}

void GameFileClass::Hook_Close(GameFileClass *ptr)
{
    ptr->GameFileClass::Close();
}

time_t GameFileClass::Hook_Get_Date_Time(GameFileClass *ptr)
{
    return ptr->GameFileClass::Get_Date_Time();
}

BOOL GameFileClass::Hook_Set_Date_Time(GameFileClass *ptr, time_t date_time)
{
    return ptr->GameFileClass::Set_Date_Time(date_time);
}

void GameFileClass::Hook_Error(GameFileClass *ptr, int error, BOOL can_retry, const char *filename)
{
    ptr->GameFileClass::Error(error, can_retry, filename);
}
#endif
