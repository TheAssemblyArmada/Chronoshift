/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief FileClass for reading files with raw OS API calls.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef RAWFILECLASS_H
#define RAWFILECLASS_H

#include "always.h"
#include "basefile.h"

class RawFileClass : public FileClass
{
    friend void Setup_Hooks();

public:
    RawFileClass();
    RawFileClass(const char *filename);

    virtual ~RawFileClass() { Reset(); };
    virtual const char *File_Name() const { return m_Filename; };
    virtual const char *Set_Name(const char *filename);
    virtual BOOL Create();
    virtual BOOL Delete();
    virtual BOOL Is_Available(BOOL forced = false);
#ifdef PLATFORM_WINDOWS
    virtual BOOL Is_Open() const { return m_Handle != (HANDLE)(LONG_PTR)-1; };
#else
    virtual BOOL Is_Open() const { return m_Handle != -1; };
#endif
    virtual BOOL Open(const char *filename, int rights = FM_READ);
    virtual BOOL Open(int rights = FM_READ);
    virtual int Read(void *buffer, int length);
    virtual off_t Seek(off_t offset, int whence = FS_SEEK_CURRENT);
    virtual off_t Size();
    virtual int Write(const void *buffer, int length);
    virtual void Close();
    virtual time_t Get_Date_Time();
    virtual BOOL Set_Date_Time(time_t date_time);
    virtual void Error(int error, BOOL can_retry = false, const char *filename = nullptr);

    void Bias(int start, int length = -1);
    int Get_Bias_Start() const { return m_BiasStart; }
#ifdef PLATFORM_WINDOWS
    HANDLE Get_File_Handle() { return m_Handle; }
#else
    int Get_File_Handle() { return m_Handle; }
#endif

private:
    void Reset();
    off_t Raw_Seek(off_t offset, int whence = FS_SEEK_CURRENT);

#ifdef GAME_DLL
    BOOL Hook_Open(int rights = FM_READ) { return RawFileClass::Open(rights); }
#endif

protected:
    int m_Rights; // This is the current rights to this file.
    int m_BiasStart; // Offset from true start to treat as start of file
    int m_BiasLength; // Logical length of file.
#ifdef PLATFORM_WINDOWS
    HANDLE m_Handle;
#else
    int m_Handle;
#endif
    char *m_Filename;
    time_t m_DateTime;
    BOOL m_IsAllocated;
};

#endif // _RAWFILECLASS_H
