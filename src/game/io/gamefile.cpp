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
#include "gamefile.h"
#include "cd.h"
#include "mixfile.h"
#include "startup.h"
#include <new>
#include <stdio.h>
#include <string.h>
#include <algorithm>

#define FILE_HANDLE_COUNT 10

#ifdef GAME_DLL
#include "hooker.h"
extern ARRAY_DEC(GameFileClass, g_handles, 10);
#else
GameFileClass g_handles[FILE_HANDLE_COUNT];
#endif

GameFileClass::GameFileClass(const char *filename) : m_FileBuffer()
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
        length = std::min((int)m_FileBuffer.Get_Size() - m_CachePosition, length);

        if (length > 0) {
            memmove(buffer, (m_FileBuffer.Get_Buffer() + m_CachePosition), length);
            m_CachePosition += length;
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
                m_CachePosition = 0;
                break;

            case FS_SEEK_CURRENT:
                break;

            case FS_SEEK_END:
                m_CachePosition = m_FileBuffer.Get_Size();
                break;
        }

        m_CachePosition = std::min(std::max(int(offset + m_CachePosition), 0), m_FileBuffer.Get_Size());

        return m_CachePosition;
    }

    return BufferIOFileClass::Seek(offset, whence);
}

off_t GameFileClass::Size()
{
    // if Buffer is valid, return buffer Size.
    if (Is_Cached()) {
        return m_FileBuffer.Get_Size();
    }

    // if BufferIO is available, return Size().
    if (BufferIOFileClass::Is_Available()) {
        return BufferIOFileClass::Size();
    }

    int filesize = 0;
    GameMixFile::Offset(File_Name(), nullptr, nullptr, nullptr, &filesize);

    return filesize;
}

BOOL GameFileClass::Is_Available(BOOL forced)
{
    if (Is_Open()) {
        return true;
    }

    if (GameMixFile::Offset(File_Name())) {
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
    m_FileBuffer.Reset();
    m_CachePosition = 0;

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
    GameMixFile *mixfile = nullptr;
    void *cachedfile = nullptr;

    if ((mode & FM_WRITE) || BufferIOFileClass::Is_Available()
        || !GameMixFile::Offset(File_Name(), &cachedfile, &mixfile, &fileoffset, &filesize)) {
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
        if (m_FileBuffer.Get_Buffer() == nullptr) {
            new(&m_FileBuffer) BufferClass(cachedfile, filesize);
        }

        m_CachePosition = 0;
    }

    return true;
}

time_t GameFileClass::Get_Date_Time()
{
    GameMixFile *mixfile = nullptr;

    if (!GameMixFile::Offset(File_Name(), nullptr, &mixfile)) {
        return RawFileClass::Get_Date_Time();
    }

    GameFileClass tmpfile(mixfile->Get_Filename());

    return tmpfile.Get_Date_Time();
}

BOOL GameFileClass::Set_Date_Time(time_t date_time)
{
    BOOL retval;
    GameMixFile *mixfile = nullptr;

    if (((retval = RawFileClass::Set_Date_Time(date_time)) != false || !GameMixFile::Offset(File_Name(), nullptr, &mixfile))) {
        return retval;
    }

    GameFileClass tmpfile(mixfile->Get_Filename());

    return tmpfile.Set_Date_Time(date_time);
}

void *GameFileClass::Retrieve(char const *filename)
{
    DEBUG_ASSERT(filename != nullptr);

    void *fileptr = GameMixFile::Retrieve(filename);

    // DEBUG_ERROR_CONDITIONAL(fileptr == nullptr, "Unable to find file %s!", strupr(filename));
    // DEBUG_ASSERT(fileptr != nullptr);

    return fileptr;
}

int GameFileClass::Retrieve_Size(char const *filename)
{
    DEBUG_ASSERT(filename != nullptr);

    // Size of the file in bytes.
    int filesize = 0;

    GameMixFile::Offset(filename, nullptr, nullptr, nullptr, &filesize);

    // DEBUG_ERROR_CONDITIONAL(mixfile == nullptr, "Unable to find file %s!", std::strupr(filename));
    // DEBUG_ASSERT(mixfile != nullptr);

    return filesize;
}

GameMixFile *GameFileClass::Retrieve_Mix(char const *mix_filename)
{
    DEBUG_ASSERT(mix_filename != nullptr);

    // Pointer to the mix file that contains the file we want.
    GameMixFile *mixfile = nullptr;

    mixfile = GameMixFile::Finder(mix_filename);

    // DEBUG_ERROR_CONDITIONAL(mixfile == nullptr, "Unable to find file %s!", std::strupr(filename));
    // DEBUG_ASSERT(mixfile != nullptr);

    return mixfile;
}

bool GameFileClass::File_Available(char const *filename)
{
    GameFileClass file(filename);
    return file.Is_Available();
}

GameMixFile *GameFileClass::Allocate_Mix(char const *filename)
{
    GameMixFile *mixfile = new GameMixFile(filename);
    DEBUG_ASSERT(mixfile != nullptr);
    return mixfile;
}

bool GameFileClass::Cache_Mix(char const *filename)
{
    bool cached = GameMixFile::Cache(filename);
    DEBUG_ASSERT(cached);
    return cached;
}

int Open_File(const char *filename, int mode)
{
    int handle = 0;

    while (g_handles[handle].Is_Open()) {
        ++handle;

        if (handle >= FILE_HANDLE_COUNT) {
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
