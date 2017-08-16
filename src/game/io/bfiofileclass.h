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
    virtual void Close();

    BOOL Cache(int size = 0, void *buffer = nullptr);
    void Free();
    BOOL Commit();

protected:
    bool m_bufferAllocated:1; // & 1
    bool m_bufferedFileAvailable:1; // & 2
    bool m_bufferedFileOpen:1; // & 4 //This or BoolOne could be IsFileOpen, Two could also be IsWriteable
    bool m_bufferFull:1; // & 8	//leaning toward this being a buffer full flag
    bool m_uncommited:1; // & 0x10 //Looks like it keeps track of if we have unwritten data
    bool m_buffered:1; // & 0x20 //Is there cached data
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
