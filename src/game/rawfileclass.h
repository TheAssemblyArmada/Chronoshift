////////////////////////////////////////////////////////////////////////////////
//                               --  THYME  --                                //
////////////////////////////////////////////////////////////////////////////////
//
//  Project Name:: Thyme
//
//          File:: RAWFILECLASS.H
//
//        Author:: CCHyper, OmniBlade
//
//  Contributors::
//
//   Description:: FileClass for reading files with raw OS API calls.
//
//       License:: Thyme is free software: you can redistribute it and/or
//                 modify it under the terms of the GNU General Public License
//                 as published by the Free Software Foundation, either version
//                 2 of the License, or (at your option) any later version.
//
//                 A full copy of the GNU General Public License can be found in
//                 LICENSE
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef RAWFILECLASS_H
#define RAWFILECLASS_H

#include "always.h"
#include "fileclass.h"

#ifndef RAPP_STANDALONE
#include "hooker.h"
#endif

class RawFileClass : public FileClass
{
public:
    RawFileClass();
    RawFileClass(const char *filename);

    virtual ~RawFileClass() { Reset(); };
    virtual const char *File_Name() const { return m_filename; };
    virtual const char *Set_Name(const char *filename);
    virtual BOOL Create();
    virtual BOOL Delete();
    virtual BOOL Is_Available(BOOL forced = false);
#ifdef PLATFORM_WINDOWS
    virtual BOOL Is_Open() const { return m_handle != INVALID_HANDLE_VALUE; };
#else
    virtual BOOL Is_Open() const { return m_handle != -1; };
#endif
    virtual BOOL Open(const char *filename, int rights = FM_READ);
    virtual BOOL Open(int rights = FM_READ);
    virtual int Read(void *buffer, int length);
    virtual int Seek(int offset, int whence = FS_SEEK_CURRENT);
    virtual int Size();
    virtual int Write(void const *buffer, int length);
    virtual void Close();
    virtual time_t Get_Date_Time();
    virtual BOOL Set_Date_Time(time_t date_time);
    virtual void Error(int error, BOOL can_retry = false, const char *filename = nullptr);
#ifdef PLATFORM_WINDOWS
    HANDLE Get_File_Handle() { return m_handle; }
#else
    int Get_File_Handle() { return m_handle; }
#endif
    void Bias(int start, int length = -1);

#ifndef RAPP_STANDALONE
    static void Hook_Me();
#endif

private:
    void Reset();
    int Raw_Seek(int offset, int whence = FS_SEEK_CURRENT);

#ifndef RAPP_STANDALONE
    static const char *Hook_File_Name(RawFileClass *ptr);
    static const char *Hook_Set_Name(RawFileClass *ptr, const char *filename);
    static BOOL Hook_Create(RawFileClass *ptr);
    static BOOL Hook_Delete(RawFileClass *ptr);
    static BOOL Hook_Open(RawFileClass *ptr, int rights = FM_READ);
    static BOOL Hook_Is_Available(RawFileClass *ptr, BOOL forced = false);
    static int Hook_Read(RawFileClass *ptr, void *buffer, int length);
    static int Hook_Seek(RawFileClass *ptr, int offset, int whence = FS_SEEK_CURRENT);
    static int Hook_Size(RawFileClass *ptr);
    static int Hook_Write(RawFileClass *ptr, void const *buffer, int length);
    static void Hook_Close(RawFileClass *ptr);
    static int Hook_Raw_Seek(RawFileClass *ptr, int offset, int whence = FS_SEEK_CURRENT);
#endif

protected:
    int m_rights; // This is the current rights to this file.
    int m_biasStart; // Offset from true start to treat as start of file
    int m_biasLength; // Logical length of file.
#ifdef PLATFORM_WINDOWS
    HANDLE m_handle;
#else
    int m_handle;
#endif
    char *m_filename;
    time_t m_dateTime;
    BOOL m_isAllocated;
};

#ifndef RAPP_STANDALONE
inline void RawFileClass::Hook_Me()
{
    Hook_Function((void*)0x005C063C, (void*)&Hook_File_Name);
    Hook_Function((void*)0x005C063C, (void*)&Hook_Set_Name);
    Hook_Function((void*)0x005C063C, (void*)&Hook_Create);
    Hook_Function((void*)0x005C063C, (void*)&Hook_Delete);
    Hook_Function((void*)0x005C0100, (void*)&Hook_Open);
    Hook_Function((void*)0x005C0210, (void*)&Hook_Is_Available);
    Hook_Function((void*)0x005C0314, (void*)&Hook_Read);
    Hook_Function((void*)0x005C04E8, (void*)&Hook_Seek);
    Hook_Function((void*)0x005C056C, (void*)&Hook_Size);
    Hook_Function((void*)0x005C0430, (void*)&Hook_Write);
    Hook_Function((void*)0x005C02C8, (void*)&Hook_Close);
    Hook_Function((void*)0x005C07CC, (void*)&Hook_Raw_Seek);
}
#endif

#endif // _RAWFILECLASS_H
