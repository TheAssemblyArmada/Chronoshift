/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief FileClass pure virtual base class providing the file io interface.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef FILECLASS_H
#define FILECLASS_H

#include "always.h"
#include <time.h>

enum FileOpenType
{
    FM_CLOSED = 0,
    FM_READ = 1,
    FM_WRITE = 2,
    FM_READ_WRITE = FM_READ | FM_WRITE,
};

enum FileSeekType
{
    FS_SEEK_START = 0, // Seek to position relative to the start of file.
    FS_SEEK_CURRENT = 1, // Seek to position relative to the current offset.
    FS_SEEK_END = 2, // Seek to position relative to the end of file.
};

class FileClass
{
public:
    FileClass() {}
    virtual ~FileClass() {}

    virtual const char *File_Name() const = 0;
    virtual const char *Set_Name(const char *filename) = 0;
    virtual BOOL Create() = 0;
    virtual BOOL Delete() = 0;
    virtual BOOL Is_Available(BOOL forced = false) = 0;
    virtual BOOL Is_Open() const = 0;
    virtual BOOL Open(const char *filename, int rights = FM_READ) = 0;
    virtual BOOL Open(int rights = FM_READ) = 0;
    virtual int Read(void *buffer, int length) = 0;
    virtual off_t Seek(off_t offset, int whence = FS_SEEK_CURRENT) = 0;
    virtual off_t Size() = 0;
    virtual int Write(const void *buffer, int size) = 0;
    virtual void Close() = 0;
    virtual time_t Get_Date_Time();
    virtual BOOL Set_Date_Time(time_t date_time);
    virtual void Error(int error, BOOL can_retry = false, const char *filename = nullptr) = 0;

    int Tell() { return Seek(0, FS_SEEK_CURRENT); }
};

#endif // _FILECLASS_H
