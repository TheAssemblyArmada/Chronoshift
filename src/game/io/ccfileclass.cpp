#include "ccfileclass.h"
#include "cd.h"
#include "minmax.h"
#include "mixfile.h"
#include "startup.h"
#include <new>
#include <stdio.h>
#include <string.h>

#define CCFILE_HANDLE_COUNT 10

#ifndef RAPP_STANDALONE
CCFileClass *g_handles = Make_Pointer<CCFileClass>(0x00635BE4);
#else
CCFileClass g_handles[CCFILE_HANDLE_COUNT];
#endif

typedef MixFileClass<CCFileClass> CCMixFileClass;

CCFileClass::CCFileClass(const char *filename) : m_fileBuffer()
{
    CDFileClass::Set_Name(filename);
}

void CCFileClass::Error(int error, BOOL can_retry, const char *filename)
{
    RawFileClass::Error(error, can_retry, filename);

    if (!Force_CD_Available(g_requiredCD)) {
        Emergency_Exit(0xFF);
    }
}

int CCFileClass::Write(void const *buffer, int length)
{
    // if the buffer pointer is null, raise an error.
    if (Is_Cached()) {
        // raise an error.
        Error(13, 0, File_Name());
    }

    return BufferIOFileClass::Write(buffer, length);
}

int CCFileClass::Read(void *buffer, int length)
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

off_t CCFileClass::Seek(off_t offset, int whence)
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

off_t CCFileClass::Size()
{
    // if Buffer is valid, return buffer Size.
    if (Is_Cached()) {
        return m_fileBuffer.Get_Size();
    }

    // if BufferIO is available, return Size().
    if (BufferIOFileClass::Is_Available()) {
        return BufferIOFileClass::Size();
    } else {
        int filesize = 0;
        CCMixFileClass::Offset(File_Name(), nullptr, nullptr, nullptr, &filesize);

        return filesize;
    }

    return 0;
}

BOOL CCFileClass::Is_Available(BOOL forced)
{
    if (Is_Open()) {
        return true;
    }

    if (CCMixFileClass::Offset(File_Name())) {
        return true;
    }

    return BufferIOFileClass::Is_Available(forced);
}

BOOL CCFileClass::Is_Open() const
{
    // if the buffer pointer is set, return true.
    if (Is_Cached()) {
        return true;
    }

    return BufferIOFileClass::Is_Open();
}

void CCFileClass::Close()
{
    m_fileBuffer.Reset();
    m_cachePosition = 0;

    return BufferIOFileClass::Close();
}

BOOL CCFileClass::Open(const char *filename, int rights)
{
    Set_Name(filename);
    return Open(rights);
}

BOOL CCFileClass::Open(int mode)
{
    Close();

    int filesize = 0;
    int fileoffset = 0;
    CCMixFileClass *mixfile = nullptr;
    void *cachedfile = nullptr;

    if ((mode & FM_WRITE) || BufferIOFileClass::Is_Available()) {
        return CDFileClass::Open(mode);
    }

    if (CCMixFileClass::Offset(File_Name(), &cachedfile, &mixfile, &fileoffset, &filesize)) {
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

            return true;
        } else {
            if (m_fileBuffer.Get_Buffer() == nullptr) {
                new(&m_fileBuffer) BufferClass(cachedfile, filesize);
            }

            m_cachePosition = 0;

            return true;
        }

        return false;
    }

    return CDFileClass::Open(mode);
}

time_t CCFileClass::Get_Date_Time()
{
    CCMixFileClass *mixfile = nullptr;

    if (!CCMixFileClass::Offset(File_Name(), nullptr, &mixfile)) {
        return RawFileClass::Get_Date_Time();
    }

    CCFileClass tmpfile(mixfile->Get_Filename());

    return tmpfile.Get_Date_Time();
}

BOOL CCFileClass::Set_Date_Time(time_t date_time)
{
    BOOL retval;
    CCMixFileClass *mixfile = nullptr;

    if ((retval = (RawFileClass::Set_Date_Time(date_time) || !CCMixFileClass::Offset(File_Name(), nullptr, &mixfile)))) {
        return retval;
    }

    CCFileClass tmpfile(mixfile->Get_Filename());

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
    CCFileClass fc(filename);

    return fc.Is_Available();
}

void *Hires_Load(const char *filename)
{
    static char _hires_filename[32];
    snprintf(_hires_filename, sizeof(_hires_filename), "H%s", filename);

    CCFileClass file(_hires_filename);

    return Load_Alloc_Data(file);
}

void *Load_Alloc_Data(const char *filename, int mode)
{
    CCFileClass file(filename);

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

#ifndef RAPP_STANDALONE

void CCFileClass::Hook_Me()
{
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
}

BOOL CCFileClass::Hook_Is_Available(CCFileClass *ptr, BOOL forced)
{
    return ptr->CCFileClass::Is_Available(forced);
}

BOOL CCFileClass::Hook_Is_Open(CCFileClass *ptr)
{
    return ptr->CCFileClass::Is_Open();
}

BOOL CCFileClass::Hook_Open_Name(CCFileClass *ptr, const char *filename, int rights)
{
    return ptr->CCFileClass::Open(filename, rights);
}

BOOL CCFileClass::Hook_Open(CCFileClass *ptr, int rights)
{
    return ptr->CCFileClass::Open(rights);
}

int CCFileClass::Hook_Read(CCFileClass *ptr, void *buffer, int length)
{
    return ptr->CCFileClass::Read(buffer, length);
}

off_t CCFileClass::Hook_Seek(CCFileClass *ptr, off_t offset, int whence)
{
    return ptr->CCFileClass::Seek(offset, whence);
}

off_t CCFileClass::Hook_Size(CCFileClass *ptr)
{
    return ptr->CCFileClass::Size();
}

int CCFileClass::Hook_Write(CCFileClass *ptr, void const *buffer, int length)
{
    return ptr->CCFileClass::Write(buffer, length);
}

void CCFileClass::Hook_Close(CCFileClass *ptr)
{
    ptr->CCFileClass::Close();
}

time_t CCFileClass::Hook_Get_Date_Time(CCFileClass *ptr)
{
    return ptr->CCFileClass::Get_Date_Time();
}

BOOL CCFileClass::Hook_Set_Date_Time(CCFileClass *ptr, time_t date_time)
{
    return ptr->CCFileClass::Set_Date_Time(date_time);
}

void CCFileClass::Hook_Error(CCFileClass *ptr, int error, BOOL can_retry, const char *filename)
{
    ptr->CCFileClass::Error(error, can_retry, filename);
}
#endif
