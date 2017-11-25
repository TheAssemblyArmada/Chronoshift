/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class for reading data from a memory buffer using FileClass IO interface.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef RAMFILE_H
#define RAMFILE_H

#include "always.h"
#include "fileclass.h"

class RAMFileClass : public FileClass
{
public:
    RAMFileClass();
    RAMFileClass(void *data, int size);
    virtual ~RAMFileClass();

    virtual const char *File_Name() const override;
    virtual const char *Set_Name(const char *filename) override;
    virtual BOOL Create() override;
    virtual BOOL Delete() override;
    virtual BOOL Is_Available(BOOL forced = false) override;
    virtual BOOL Is_Open() const override { return IsOpen; }
    virtual BOOL Open(const char *filename, int rights = FM_READ) override;
    virtual BOOL Open(int rights = FM_READ) override;
    virtual int Read(void *buffer, int length) override;
    virtual off_t Seek(off_t offset, int whence = FS_SEEK_CURRENT) override;
    virtual off_t Size() override;
    virtual int Write(void const *buffer, int size) override;
    virtual void Close() override { IsOpen = false; };
    virtual time_t Get_Date_Time() override;
    virtual BOOL Set_Date_Time(time_t date_time) override;
    virtual void Error(int error, BOOL can_retry = false, const char *filename = nullptr) override;

private:
    char *Buffer;
    int BufferSize;
    int BufferAvailable;
    int BufferOffset;
    int BufferRights;
    BOOL IsOpen;
    BOOL IsAllocated;
};

#endif // RAMFILE_H
