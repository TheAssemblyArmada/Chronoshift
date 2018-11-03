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
#include "gamedebug.h"
#include "minmax.h"
#include <cstring>

using std::memmove;

RAMFileClass::RAMFileClass() :
    Buffer(nullptr),
    BufferSize(0),
    BufferAvailable(0),
    BufferOffset(0),
    BufferRights(FM_READ),
    IsOpen(false),
    IsAllocated(false)
{
}

RAMFileClass::RAMFileClass(void *data, int size) :
    Buffer((char *)data),
    BufferSize(size),
    BufferAvailable(0),
    BufferOffset(0),
    BufferRights(FM_READ),
    IsOpen(false),
    IsAllocated(false)
{
    if (!data && size > 0) {
        Buffer = new char[size];
        IsAllocated = true;
    }
}

RAMFileClass::~RAMFileClass()
{
    Close();

    IsOpen = false;

    if (IsAllocated) {
        delete[] Buffer;
        Buffer = nullptr;
        IsAllocated = false;
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
        BufferAvailable = 0;
        return true;
    }

    return false;
}

BOOL RAMFileClass::Delete()
{
    if (!Is_Open()) {
        BufferAvailable = 0;
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
    if (Buffer != nullptr || !Is_Open()) {
        BufferOffset = 0;
        BufferRights = rights;
        IsOpen = true;

        // Treat as newly created file?
        if (rights == FM_WRITE) {
            BufferAvailable = 0;
        } else {
            BufferRights = FM_READ | FM_WRITE;
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
    DEBUG_ASSERT(buffer != nullptr);

    if (Buffer && buffer && size) {
        BOOL opened = false;

        if (Is_Open()) {
            if (!(BufferRights & FM_READ)) {
                return 0;
            }
        } else {
            Open(FM_READ);
            opened = true;
        }

        size = Min((int)(BufferSize - BufferOffset), size);
        memmove(buffer, Buffer + BufferOffset, size);
        BufferOffset += size;
        BufferAvailable = Max(BufferOffset, BufferAvailable);

        if (opened) {
            Close();
        }

        return size;
    }
    return 0;
}

off_t RAMFileClass::Seek(off_t offset, int whence)
{
    if (Buffer && Is_Open()) {
        int buffer_end = BufferAvailable;

        if (BufferRights & FM_WRITE) {
            buffer_end = BufferSize;
        }

        switch (whence) {
            case FS_SEEK_START:
                BufferOffset = offset;
                break;

            case FS_SEEK_CURRENT:
                BufferOffset += offset;
                break;

            case FS_SEEK_END:
                BufferOffset = buffer_end + offset;
                break;

            default:
                // for debugging purposes, we want this to hit an assertion.
                DEBUG_ASSERT_PRINT(false, "Unhandled Seek mode in RAMFileClass::Seek()!");
                break;
        }

        BufferOffset = Min(Max(BufferOffset, 0), buffer_end);
        BufferAvailable = Max(BufferAvailable, BufferOffset);

        return BufferOffset;
    }
    return BufferOffset;
}

off_t RAMFileClass::Size()
{
    DEBUG_ASSERT_PRINT(Buffer != nullptr, "Buffer is NULL while retrieving the size!");

    if (Buffer != nullptr) {
        return BufferAvailable;
    }

    return 0;
}

int RAMFileClass::Write(const void *buffer, int size)
{
    DEBUG_ASSERT(buffer != nullptr);
    DEBUG_ASSERT(size > 0);

    if (Buffer && buffer && size) {
        BOOL opened = false;

        if (Is_Open()) {
            // If we don't have the write bit set, return 0 as can't write any
            if (!(BufferRights & FM_WRITE)) {
                return 0;
            }
        } else {
            Open(FM_WRITE);
            opened = true;
        }

        // Adjust size if we are trying to write out of bounds
        size = Min(size, BufferSize - BufferOffset);
        memmove(Buffer + BufferOffset, buffer, size);
        BufferOffset += size;
        BufferAvailable = Max(BufferAvailable, BufferOffset);

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
