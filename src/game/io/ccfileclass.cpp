#include "ccfileclass.h"
#include "cd.h"
#include "minmax.h"
#include "mixfile.h"
#include "startup.h"
#include <new>
#include <stdio.h>
#include <string.h>

typedef MixFileClass<CCFileClass> CCMixFileClass;

CCFileClass::CCFileClass() : m_fileBuffer()
{

}

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
    if (!Buffer_Allocated()) {
        // raise an error.
        Error(13, 0, File_Name());
    }

    return BufferIOFileClass::Write(buffer, length);
}

int CCFileClass::Read(void *buffer, int length)
{
    //DEBUG_LOG("CCFileClass::Read(%s) - Attempting to read %d bytes\n", File_Name(), length);
    BOOL opened = false;

    if (!Is_Open() && Open(FM_READ)) {
        //DEBUG_LOG("CCFileClass::Read(%s) - Opened file OK\n", File_Name());
        opened = true;
    }

    if (Buffer_Allocated()) {
        //DEBUG_LOG("CCFileClass::Read(%s) - File buffer is %d bytes\n", File_Name(), m_fileBuffer.Get_Size());

        // if ( m_fileBuffer.Get_Size() - m_cachePosition < length ) {
        //	length = m_fileBuffer.Get_Size() - m_cachePosition;
        //}

        //DEBUG_LOG("CCFileClass::Read(%s) - Reading from cached file %s with start pos of %d\n", File_Name(), File_Name(), m_cachePosition);

        length = Min((int)m_fileBuffer.Get_Size() - m_cachePosition, length);

        if (length > 0) {
            //DEBUG_LOG("CCFileClass::Read(%s) - Read size is %d\n", File_Name(), length);
            memmove(buffer, (m_fileBuffer.Get_Buffer() + m_cachePosition), length);
            m_cachePosition += length;
            //DEBUG_LOG("CCFileClass::Read(%s) - m_cachePosition is %d\n", File_Name(), m_cachePosition);
        }

        if (opened) {
            //DEBUG_LOG("CCFileClass::Read(%s) - We opened the file, calling Close()\n", File_Name());
            Close();
        }

        //DEBUG_LOG("CCFileClass::Read(%s) - Read %d bytes\n", File_Name(), length);

        return length;
    }

    //DEBUG_LOG("CCFileClass::Read(%s) - About to call BufferIOFileClass::Read()\n", File_Name());
    length = BufferIOFileClass::Read(buffer, length);

    if (opened) {
        //DEBUG_LOG("CCFileClass::Read(%s) - We opened the file, calling Close()\n", File_Name());
        Close();
    }

    //DEBUG_LOG("CCFileClass::Read(%s) - Read %d bytes\n", File_Name(), length);

    return length;
}

off_t CCFileClass::Seek(off_t offset, int whence)
{
    //DEBUG_LOG("CCFileClass::Seek() Seeking in file to %d with mode %d.\n", offset, whence);
    if (Buffer_Allocated()) {
        //DEBUG_LOG("CCFileClass::Seek() Seeking in cached file.\n");
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

    //DEBUG_LOG("CCFileClass::Seek() Seeking in physical file.\n");
    return BufferIOFileClass::Seek(offset, whence);
}

off_t CCFileClass::Size()
{
    // if Buffer is valid, return buffer Size.
    if (Buffer_Allocated()) {
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
    if (Buffer_Allocated()) {
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
        DEBUG_LOG("CCFileClass::Open(%s) - Opening raw file from disk.\n", File_Name());
        return CDFileClass::Open(mode);
    }


    DEBUG_LOG("CCFileClass::Open(%s) - About to call MixFileClass::Offset()\n", File_Name());

    if (CCMixFileClass::Offset(File_Name(), &cachedfile, &mixfile, &fileoffset, &filesize)) {
        DEBUG_ASSERT(mixfile != nullptr);

        if (cachedfile == nullptr && mixfile != nullptr) {
            char *tmpfilename = strdup(File_Name());

            //DEBUG_LOG("CCFileClass::Open(%s) - Opening file from disk\n", tmpfilename);
            Open(mixfile->Get_Filename(), FM_READ);

            //DEBUG_LOG("CCFileClass::Open(%s) - about to disable search drives to set the filename\n", tmpfilename);
            Disable_Search_Drives(false);
            Set_Name(tmpfilename);
            Disable_Search_Drives(true);
            free((void *)tmpfilename);

            //DEBUG_LOG("CCFileClass::Open(%s) - setting file bias %d, size %d\n", m_filename, fileoffset, filesize);
            Bias(0);
            Bias(fileoffset, filesize);

            //DEBUG_LOG("CCFileClass::Open(%s) - seeking file\n", m_filename);
            Seek(0, FS_SEEK_START);

            DEBUG_LOG("CCFileClass::Open(%s) - opened from disk OK\n", m_filename);

            return true;

        } else {
            DEBUG_LOG("CCFileClass::Open(%s) - Opening file from cache\n", m_filename);
            if (m_fileBuffer.Get_Buffer() == nullptr) {
                //m_fileBuffer = BufferClass(cachedfile, filesize);
                DEBUG_LOG("CCFileClass::Open(%s) - Setting up buffer with size %d\n", m_filename, filesize);
                new(&m_fileBuffer) BufferClass(cachedfile, filesize);
            }

            m_cachePosition = 0;

            //DEBUG_LOG("CCFileClass::Open(%s) - opened from cache OK\n", m_filename);
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

void *Hires_Load(const char *filename)
{
    static char _hires_filename[32];
    snprintf(_hires_filename, sizeof(_hires_filename), "H%s", filename);

    CCFileClass file(_hires_filename);

    return Load_Alloc_Data(file);
}

void *Load_Alloc_Data(const char *filename)
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
    Hook_Function((void*)0x00462A30, (void*)&Hook_Is_Available);
    Hook_Function((void*)0x00462A80, (void*)&Hook_Is_Open);
    Hook_Function((void*)0x00426400, (void*)&Hook_Open_Name);
    Hook_Function((void*)0x00462AD4, (void*)&Hook_Open);
    Hook_Function((void*)0x004628B0, (void*)&Hook_Read);
    Hook_Function((void*)0x00462958, (void*)&Hook_Seek);
    Hook_Function((void*)0x004629CC, (void*)&Hook_Size);
    Hook_Function((void*)0x00462860, (void*)&Hook_Write);
    Hook_Function((void*)0x00462AA8, (void*)&Hook_Close);
    Hook_Function((void*)0x00462BD8, (void*)&Hook_Get_Date_Time);
    Hook_Function((void*)0x00462C50, (void*)&Hook_Set_Date_Time);
    Hook_Function((void*)0x00462840, (void*)&Hook_Error);
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
