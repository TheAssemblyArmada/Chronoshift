/**
* @file
*
* @Author CCHyper, OmniBlade
*
* @brief  FileClass layer for providing file buffering services.
*
* @copyright Redalert++ is free software: you can redistribute it and/or
*            modify it under the terms of the GNU General Public License
*            as published by the Free Software Foundation, either version
*            2 of the License, or (at your option) any later version.
*
*            A full copy of the GNU General Public License can be found in
*            LICENSE
*/
#pragma once

#ifndef BFIOFILECLASS_H
#define BFIOFILECLASS_H

#include "always.h"
#include "rawfileclass.h"

#define MIN_CACHE_SIZE 1024

class BufferIOFileClass : public RawFileClass
{
public:
    BufferIOFileClass();
    BufferIOFileClass(char const *filename);

    virtual ~BufferIOFileClass() { Free(); }

    virtual char const *Set_Name(char const *filename) override;
    virtual BOOL Is_Available(BOOL forced = false) override;
    virtual BOOL Is_Open() const override;
    virtual BOOL Open(char const *filename, int rights = FM_READ) override;
    virtual BOOL Open(int rights = FM_READ) override;
    virtual int Read(void *buffer, int size) override;
    virtual off_t Seek(off_t offset = 0, int whence = FS_SEEK_CURRENT) override;
    virtual off_t Size() override;
    virtual int Write(void const *buffer, int length) override;
    virtual void Close() override;

    BOOL Cache(int size = 0, void *buffer = nullptr);
    void Free();
    BOOL Commit();

#ifndef RAPP_STANDALONE
    static void Hook_Me();
    static BOOL Hook_Is_Available(BufferIOFileClass *ptr, BOOL forced);
    static BOOL Hook_Is_Open(BufferIOFileClass *ptr);
    static BOOL Hook_Open_Name(BufferIOFileClass *ptr, const char *filename, int rights);
    static BOOL Hook_Open(BufferIOFileClass *ptr, int rights);
    static int Hook_Read(BufferIOFileClass *ptr, void *buffer, int length);
    static off_t Hook_Seek(BufferIOFileClass *ptr, off_t offset, int whence);
    static off_t Hook_Size(BufferIOFileClass *ptr);
    static int Hook_Write(BufferIOFileClass *ptr, void const *buffer, int length);
    static void Hook_Close(BufferIOFileClass *ptr);
#endif
protected:
#ifndef RAPP_STANDALONE
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool m_bufferAllocated : 1; // & 1
            bool m_bufferedFileAvailable : 1; // & 2
            bool m_bufferedFileOpen : 1; // & 4
            bool m_bufferFull : 1; // & 8
            bool m_uncommited : 1; // & 0x10
            bool m_buffered : 1; // & 0x20
        };
        int m_bufferFlags;
    };
#else
    bool m_bufferAllocated;
    bool m_bufferedFileAvailable;
    bool m_bufferedFileOpen;
    bool m_bufferFull;
    bool m_uncommited;
    bool m_buffered;
#endif
    int m_bufferedRights; //Duplicate of Rights member inherited from RawFile???
    void *m_buffer; // 0x2C
    int m_bufferedSize; //Looks like this is the current size of the buffer??
    int m_bufferPosition; //fairly sure this keeps track of the pos in buffer
    int m_bufferStart; //Position in file that we have cached up to?
    int m_uncommitedStart;
    int m_uncommitedEnd;
    int m_bufferedFileSize; //This appears to actually be the backing file size
    int m_bufferedBiasStart; //Again, isn't this already inherited from RawFile?
    int m_bufferedFileStart; //position in the file when we opened it.
};

#endif // BFIOFILECLASS_H
