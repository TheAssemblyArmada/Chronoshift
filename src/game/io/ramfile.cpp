/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class for reading data from a memory buffer using FileClass IO interface.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "ramfile.h"
#include <cstring>
#include <algorithm>
#include <captainslog.h>

using std::memmove;

RAMFileClass::RAMFileClass() :
    m_Buffer(nullptr),
    m_BufferSize(0),
    m_BufferAvailable(0),
    m_BufferOffset(0),
    m_BufferRights(FM_READ),
    m_IsOpen(false),
    m_IsAllocated(false)
{
}

RAMFileClass::RAMFileClass(void *data, int size) :
    m_Buffer((char *)data),
    m_BufferSize(size),
    m_BufferAvailable(0),
    m_BufferOffset(0),
    m_BufferRights(FM_READ),
    m_IsOpen(false),
    m_IsAllocated(false)
{
    if (!data && size > 0) {
        m_Buffer = new char[size];
        m_IsAllocated = true;
    }
}

RAMFileClass::~RAMFileClass()
{
    Close();

    m_IsOpen = false;

    if (m_IsAllocated) {
        delete[] m_Buffer;
        m_Buffer = nullptr;
        m_IsAllocated = false;
    }
}

const char *RAMFileClass::File_Name() const
{
    return "UNKNOWN";
}

const char *RAMFileClass::Set_Name(const char *filename)
{
    return File_Name();
}

BOOL RAMFileClass::Create()
{
    if (!Is_Open()) {
        m_BufferAvailable = 0;
        return true;
    }

    return false;
}

BOOL RAMFileClass::Delete()
{
    if (!Is_Open()) {
        m_BufferAvailable = 0;
        return true;
    }

    return false;
}

BOOL RAMFileClass::Is_Available(BOOL forced)
{
    return true;
}

BOOL RAMFileClass::Open(int rights)
{
    if (m_Buffer != nullptr || !Is_Open()) {
        m_BufferOffset = 0;
        m_BufferRights = rights;
        m_IsOpen = true;

        // Treat as newly created file?
        if (rights == FM_WRITE) {
            m_BufferAvailable = 0;
        } else {
            m_BufferRights = FM_READ | FM_WRITE;
        }

        return Is_Open();
    }

    return false;
}

BOOL RAMFileClass::Open(const char *filename, int rights)
{
    return Open(rights);
}

int RAMFileClass::Read(void *buffer, int size)
{
    captainslog_assert(buffer != nullptr);

    if (m_Buffer && buffer && size) {
        BOOL opened = false;

        if (Is_Open()) {
            if (!(m_BufferRights & FM_READ)) {
                return 0;
            }
        } else {
            Open(FM_READ);
            opened = true;
        }

        size = std::min((int)(m_BufferSize - m_BufferOffset), size);
        memmove(buffer, m_Buffer + m_BufferOffset, size);
        m_BufferOffset += size;
        m_BufferAvailable = std::max(m_BufferOffset, m_BufferAvailable);

        if (opened) {
            Close();
        }

        return size;
    }
    return 0;
}

off_t RAMFileClass::Seek(off_t offset, int whence)
{
    if (m_Buffer && Is_Open()) {
        int buffer_end = m_BufferAvailable;

        if (m_BufferRights & FM_WRITE) {
            buffer_end = m_BufferSize;
        }

        switch (whence) {
            case FS_SEEK_START:
                m_BufferOffset = offset;
                break;

            case FS_SEEK_CURRENT:
                m_BufferOffset += offset;
                break;

            case FS_SEEK_END:
                m_BufferOffset = buffer_end + offset;
                break;

            default:
                break;
        }

        m_BufferOffset = std::min(std::max(m_BufferOffset, 0), buffer_end);
        m_BufferAvailable = std::max(m_BufferAvailable, m_BufferOffset);

        return m_BufferOffset;
    }
    return m_BufferOffset;
}

off_t RAMFileClass::Size()
{
    captainslog_dbgassert(m_Buffer != nullptr, "m_Buffer is NULL while retrieving the size!");

    if (m_Buffer != nullptr) {
        return m_BufferAvailable;
    }

    return 0;
}

int RAMFileClass::Write(const void *buffer, int size)
{
    captainslog_assert(buffer != nullptr);
    captainslog_assert(size > 0);

    if (m_Buffer && buffer && size) {
        BOOL opened = false;

        if (Is_Open()) {
            // If we don't have the write bit set, return 0 as can't write any
            if (!(m_BufferRights & FM_WRITE)) {
                return 0;
            }
        } else {
            Open(FM_WRITE);
            opened = true;
        }

        // Adjust size if we are trying to write out of bounds
        size = std::min(size, m_BufferSize - m_BufferOffset);
        memmove(m_Buffer + m_BufferOffset, buffer, size);
        m_BufferOffset += size;
        m_BufferAvailable = std::max(m_BufferAvailable, m_BufferOffset);

        if (opened) {
            Close();
        }

        return size;
    }

    return 0;
}

time_t RAMFileClass::Get_Date_Time()
{
    return 0;
}

BOOL RAMFileClass::Set_Date_Time(time_t date_time)
{
    return true;
}

void RAMFileClass::Error(int error, BOOL can_retry, const char *filename)
{
    return;
}
