/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief FileClass layer for providing file buffering services.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef BFIOFILECLASS_H
#define BFIOFILECLASS_H

#include "always.h"
#include "rawfile.h"

#define MIN_CACHE_SIZE 1024

class BufferIOFileClass : public RawFileClass
{
public:
    BufferIOFileClass();
    BufferIOFileClass(const char *filename);

    virtual ~BufferIOFileClass() { Free(); }

    virtual const char *Set_Name(const char *filename) override;
    virtual BOOL Is_Available(BOOL forced = false) override;
    virtual BOOL Is_Open() const override;
    virtual BOOL Open(const char *filename, int rights = FM_READ) override;
    virtual BOOL Open(int rights = FM_READ) override;
    virtual int Read(void *buffer, int size) override;
    virtual off_t Seek(off_t offset = 0, int whence = FS_SEEK_CURRENT) override;
    virtual off_t Size() override;
    virtual int Write(const void *buffer, int length) override;
    virtual void Close() override;

    BOOL Cache(int size = 0, void *buffer = nullptr);
    void Free();
    BOOL Commit();

#ifdef GAME_DLL
    BOOL Hook_Is_Open() { return BufferIOFileClass::Is_Open(); }
    BOOL Hook_Open_Name(const char *filename, int rights) { return BufferIOFileClass::Open(filename, rights); }
    BOOL Hook_Open(int rights) { return BufferIOFileClass::Open(rights); }
#endif
protected:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_BufferAllocated : 1; // & 1
    BOOL m_BufferedFileAvailable : 1; // & 2
    BOOL m_BufferedFileOpen : 1; // & 4
    BOOL m_BufferFull : 1; // & 8
    BOOL m_Uncommited : 1; // & 0x10
    BOOL m_Buffered : 1; // & 0x20
#else
    bool m_BufferAllocated;
    bool m_BufferedFileAvailable;
    bool m_BufferedFileOpen;
    bool m_BufferFull;
    bool m_Uncommited;
    bool m_Buffered;
#endif
    int m_BufferedRights; // Duplicate of Rights member inherited from RawFile???
    void *m_Buffer; // 0x2C
    int m_BufferedSize; // Looks like this is the current size of the buffer??
    int m_BufferPosition; // fairly sure this keeps track of the pos in buffer
    int m_BufferStart; // Position in file that we have cached up to?
    int m_UncommitedStart;
    int m_UncommitedEnd;
    int m_BufferedFileSize; // This appears to actually be the backing file size
    int m_BufferedBiasStart; // Again, isn't this already inherited from RawFile?
    int m_BufferedFileStart; // position in the file when we opened it.
};

#endif // BFIOFILECLASS_H
