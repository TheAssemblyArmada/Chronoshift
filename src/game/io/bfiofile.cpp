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
#include "bfiofile.h"
#include "gamedebug.h"
#include "minmax.h"

BufferIOFileClass::BufferIOFileClass() :
    m_bufferAllocated(false),
    m_bufferedFileAvailable(false),
    m_bufferedFileOpen(false),
    m_bufferFull(false),
    m_uncommited(false),
    m_buffered(false),
    m_bufferedRights(FM_CLOSED),
    m_buffer(nullptr),
    m_bufferedSize(0),
    m_bufferPosition(0),
    m_bufferStart(0),
    m_uncommitedStart(-1),
    m_uncommitedEnd(-1),
    m_bufferedFileSize(0),
    m_bufferedBiasStart(0),
    m_bufferedFileStart(0)
{
}

BufferIOFileClass::BufferIOFileClass(const char *filename) :
    m_bufferAllocated(false),
    m_bufferedFileAvailable(false),
    m_bufferedFileOpen(false),
    m_bufferFull(false),
    m_uncommited(false),
    m_buffered(false),
    m_bufferedRights(FM_CLOSED),
    m_buffer(nullptr),
    m_bufferedSize(0),
    m_bufferPosition(0),
    m_bufferStart(0),
    m_uncommitedStart(-1),
    m_uncommitedEnd(-1),
    m_bufferedFileSize(0),
    m_bufferedBiasStart(0),
    m_bufferedFileStart(0)
{
    BufferIOFileClass::Set_Name(filename);
}

BOOL BufferIOFileClass::Cache(int size, void *buffer)
{
    if (m_buffer) {
        return size == 0 && buffer == nullptr;
    }

    if (Is_Available()) {
        m_bufferedFileSize = Size();
    } else {
        m_bufferedFileSize = 0;
    }

    if (size > 0) {
        if (size < MIN_CACHE_SIZE) {
            size = MIN_CACHE_SIZE;
            if (buffer) {
                // Passed a buffer that is smaller than our min size
                Error(9);
            }
        }
        m_bufferedSize = size;
    } else {
        m_bufferedSize = m_bufferedFileSize;
    }

    if ((size || buffer == nullptr) && m_bufferedSize > 0) {
        if (buffer) {
            m_buffer = buffer;
        } else {
            m_buffer = new char[m_bufferedSize];
        }

        if (m_buffer == nullptr) {
            Error(5);
            return false;
        }

        m_bufferAllocated = true;
        m_bufferedFileOpen = false;
        m_bufferPosition = 0;
        m_bufferStart = 0;
        m_uncommitedStart = -1;
        m_uncommitedEnd = -1;
        m_bufferedBiasStart = 0;
        m_bufferedFileStart = 0;

        if (m_bufferedFileSize > 0) {
            bool opened = false;
            int startpos = 0;

            int sizetocache = Min(m_bufferedFileSize, m_bufferedSize);

            if (Is_Open()) {
                startpos = Tell();

                if (Is_Open()) {
                    m_bufferedFileStart = RawFileClass::Tell();
                } else {
                    m_bufferedFileStart = startpos;
                }

                if (m_bufferedFileSize > m_bufferedSize) {
                    m_bufferStart = startpos;
                } else {
                    if (startpos > 0)
                        Seek(0, FS_SEEK_START);

                    m_bufferPosition = startpos;
                }

                m_bufferedBiasStart = startpos;

            } else {
                if (Open()) {
                    m_bufferedFileStart = RawFileClass::Tell();
                    opened = true;
                }
            }

            if (Read(m_buffer, sizetocache) != sizetocache) {
                Error(21);
            }

            if (opened) {
                Close();
            } else {
                RawFileClass::Tell();
            }

            m_bufferFull = true;
        }

        m_buffered = true;

        return true;
    }

    return false;
}

void BufferIOFileClass::Free()
{
    if (m_buffer) {
        if (m_bufferAllocated) {
            delete[] static_cast<char *>(m_buffer);
            m_bufferAllocated = false;
        }

        m_buffer = nullptr;
    }

    m_bufferedFileAvailable = false;
    m_bufferedFileOpen = false;
    m_uncommited = false;
    m_bufferedSize = 0;
    m_buffered = false;
}

BOOL BufferIOFileClass::Commit()
{
    if (m_buffered && m_uncommited) {
        int writesize = m_uncommitedEnd - m_uncommitedStart;

        if (m_bufferedFileOpen) {
            RawFileClass::Seek(m_uncommitedStart + m_bufferedFileStart + m_bufferStart, FS_SEEK_START);
            RawFileClass::Write(m_buffer, writesize);
            RawFileClass::Seek(m_bufferedFileStart + m_bufferedBiasStart, FS_SEEK_START);
        } else {
            RawFileClass::Open(FM_WRITE);
            RawFileClass::Seek(m_uncommitedStart + m_bufferStart + m_bufferedFileStart, FS_SEEK_START);
            RawFileClass::Write(m_buffer, writesize);
            RawFileClass::Close();
        }

        m_uncommited = false;

        return true;
    }

    return false;
}

const char *BufferIOFileClass::Set_Name(const char *filename)
{
    if (File_Name() != nullptr && m_buffered) {
        if (strcmp(filename, File_Name()) == 0) {
            return File_Name();
        }

        Commit();
        m_bufferFull = false;
    }

    RawFileClass::Set_Name(filename);

    return File_Name();
}

BOOL BufferIOFileClass::Is_Available(BOOL forced)
{
    if (m_buffered) {
        return true;
    }

    return RawFileClass::Is_Available(forced);
}

BOOL BufferIOFileClass::Is_Open() const
{
    if (m_bufferedFileAvailable && m_buffered) {
        return true;
    }

    return RawFileClass::Is_Open();
}

BOOL BufferIOFileClass::Open(const char *filename, int rights)
{
    Set_Name(filename);

    return BufferIOFileClass::Open(rights);
}

BOOL BufferIOFileClass::Open(int rights)
{
    BufferIOFileClass::Close();

    if (m_buffered) {
        m_bufferedRights = rights;

        if (rights == FM_READ && m_bufferedFileSize <= m_bufferedSize) {
            m_bufferedFileOpen = false;

        } else {
            if (rights == FM_WRITE) {
                RawFileClass::Open(rights);
                RawFileClass::Close();
                m_bufferedFileStart = 0;
            }

            if (m_bufferedFileStart) {
                m_buffered = false;
                Open(rights);
                m_buffered = true;
            } else {
                RawFileClass::Open(rights);
            }

            m_bufferedFileOpen = true;

            if (m_bufferedRights == FM_WRITE) {
                m_bufferedFileSize = 0;
            }
        }

        m_bufferPosition = 0;
        m_bufferStart = 0;
        m_uncommitedStart = -1;
        m_uncommitedEnd = -1;
        m_bufferedBiasStart = 0;
        m_bufferedFileAvailable = true;

        return true;
    }

    return RawFileClass::Open(rights);
}

int BufferIOFileClass::Write(const void *buffer, int length)
{
    int readsize = 0;
    int movedsize = 0;
    int retval = 0;
    BOOL opened = false;

    if (!Is_Open()) {
        if (!Open(FM_WRITE)) {
            return 0;
        } else {
            m_bufferedFileStart = Tell();
            opened = true;
        }
    }

    if (m_buffered) {
        int totalwrite = 0;

        if (m_bufferedRights == FM_READ) {
            Error(13);

        } else {
            while (length) {
                movedsize = Min(length, (m_bufferedSize - m_bufferPosition));

                if (movedsize != m_bufferedSize && !m_bufferFull) {
                    if (m_bufferedFileSize >= m_bufferedSize) {
                        readsize = m_bufferedSize;
                        m_bufferStart = m_bufferedBiasStart;
                    } else {
                        readsize = m_bufferedFileSize;
                        m_bufferStart = 0;
                    }

                    if (m_bufferedFileStart) {
                        m_buffered = false;
                        Seek(m_bufferedBiasStart, FS_SEEK_START);
                        Read(m_buffer, m_bufferedSize);
                        Seek(m_bufferedBiasStart, FS_SEEK_START);
                        m_buffered = true;
                    } else {
                        RawFileClass::Seek(m_bufferStart, FS_SEEK_START);
                        RawFileClass::Read(m_buffer, readsize);
                    }

                    m_bufferPosition = 0;
                    m_uncommitedStart = -1;
                    m_uncommitedEnd = -1;
                    m_bufferFull = true;
                }

                memmove(static_cast<char *>(m_buffer) + m_bufferPosition,
                    static_cast<const char *>(buffer) + totalwrite,
                    movedsize);

                m_uncommited = true;
                totalwrite += movedsize;
                length -= movedsize;

                if (m_uncommitedStart == -1) {
                    m_uncommitedStart = m_bufferPosition;
                    m_uncommitedEnd = m_bufferPosition;
                } else {
                    if (m_uncommitedStart > m_bufferPosition) {
                        m_uncommitedStart = m_bufferPosition;
                    }
                }
            }

            m_bufferPosition += movedsize;
            m_uncommitedEnd = Max(m_uncommitedEnd, m_bufferPosition);
            m_bufferedBiasStart = m_bufferPosition + m_bufferStart;
            m_bufferedFileSize = Max(m_bufferedFileSize, m_bufferedBiasStart);

            if (m_bufferPosition == m_bufferedSize) {
                Commit();
                m_bufferPosition = 0;
                m_bufferStart = m_bufferedBiasStart;
                m_uncommitedStart = -1;
                m_uncommitedEnd = -1;

                if (length && m_bufferedFileSize > m_bufferedBiasStart) {
                    if (m_bufferedFileStart) {
                        m_buffered = false;
                        Seek(m_bufferedBiasStart, FS_SEEK_START);
                        Read(m_buffer, m_bufferedSize);
                        Seek(m_bufferedBiasStart, FS_SEEK_START);
                        m_buffered = true;
                    } else {
                        RawFileClass::Seek(m_bufferedBiasStart, FS_SEEK_START);
                        RawFileClass::Read(m_buffer, m_bufferedSize);
                    }
                } else {
                    m_bufferFull = false;
                }
            }
        }

        retval = totalwrite;

    } else {
        retval = RawFileClass::Write(buffer, length);
    }

    if (opened) {
        Close();
    }

    return retval;
}

int BufferIOFileClass::Read(void *buffer, int size)
{
    int readsize;
    int movedsize;
    int retval;
    BOOL opened = false;

    if (!Is_Open() && Open(FM_READ)) {
        m_bufferedFileStart = RawFileClass::Tell();
        opened = true;
    }

    if (m_buffered) {
        int totalread = 0;

        if (m_bufferedRights == FM_WRITE) {
            Error(13);

        } else {
            while (size > 0) {
                movedsize = Min(size, (m_bufferedSize - m_bufferPosition));

                if (!m_bufferFull) {
                    if (m_bufferedFileSize >= m_bufferedSize) {
                        readsize = m_bufferedSize;
                        m_bufferStart = m_bufferedBiasStart;
                    } else {
                        readsize = m_bufferedFileSize;
                        m_bufferStart = 0;
                    }

                    if (m_bufferedFileStart) {
                        m_buffered = false;
                        Seek(m_bufferedBiasStart, FS_SEEK_START);
                        Read(m_buffer, m_bufferedSize);
                        Seek(m_bufferedBiasStart, FS_SEEK_START);
                        m_buffered = true;

                    } else {
                        RawFileClass::Seek(m_bufferStart, FS_SEEK_START);
                        RawFileClass::Read(m_buffer, readsize);
                    }

                    m_bufferPosition = 0;
                    m_uncommitedStart = -1;
                    m_uncommitedEnd = -1;
                    m_bufferFull = true;
                }

                memmove(
                    static_cast<char *>(buffer) + totalread, static_cast<char *>(m_buffer) + m_bufferPosition, movedsize);

                totalread += movedsize;
                size -= movedsize;

                m_bufferPosition += movedsize;
                m_bufferedBiasStart = m_bufferPosition + m_bufferStart;

                if (m_bufferPosition == m_bufferedSize) {
                    Commit();
                    m_bufferPosition = 0;
                    m_bufferStart = m_bufferedBiasStart;
                    m_uncommitedStart = -1;
                    m_uncommitedEnd = -1;

                    if (size && m_bufferedFileSize > m_bufferedBiasStart) {
                        if (m_bufferedFileStart) {
                            m_buffered = false;
                            Seek(m_bufferedBiasStart, FS_SEEK_START);
                            Read(m_buffer, m_bufferedSize);
                            Seek(m_bufferedBiasStart, FS_SEEK_START);
                            m_buffered = true;

                        } else {
                            // DEBUG_SAY("BufferIOFileClass::Read(%s) - Calling RawFileClass::Seek()\n", File_Name());
                            RawFileClass::Seek(m_bufferedBiasStart, FS_SEEK_START);

                            // DEBUG_SAY("BufferIOFileClass::Read(%s) - Calling RawFileClass::Read()\n", File_Name());
                            RawFileClass::Read(m_buffer, m_bufferedSize);
                        }

                    } else {
                        m_bufferFull = false;
                    }
                }
            }
        }

        retval = totalread;
    } else {
        retval = RawFileClass::Read(buffer, size);
    }

    if (opened) {
        Close();
    }

    return retval;
}

off_t BufferIOFileClass::Seek(off_t offset, int whence)
{
    if (m_buffered) {
        BOOL offset_adjusted = false;

        if (whence == FS_SEEK_START) {
            m_bufferedBiasStart = 0;
        } else {
            if (whence == FS_SEEK_END) {
                m_bufferedBiasStart = m_bufferedFileSize;
            }
        }

        if (m_bufferedFileStart && offset >= m_bufferedFileStart) {
            offset = offset - m_bufferedFileStart;
            offset_adjusted = true;
        }

        m_bufferedBiasStart += offset;

        if (m_biasStart < 0) {
            m_bufferedBiasStart = 0;
        }

        if (m_bufferedBiasStart > m_bufferedFileSize) {
            m_bufferedBiasStart = m_bufferedFileSize;
        }

        if (m_bufferedFileSize > m_bufferedSize) {
            if (m_bufferedBiasStart >= m_bufferStart && m_bufferedSize + m_bufferStart > m_bufferedBiasStart) {
                m_bufferPosition = m_bufferedBiasStart - m_bufferStart;

            } else {
                Commit();

                if (m_bufferedFileStart) {
                    m_buffered = false;
                    Seek(m_bufferedBiasStart, FS_SEEK_START);
                    m_buffered = true;

                } else {
                    RawFileClass::Seek(m_bufferedBiasStart, FS_SEEK_START);
                }
                m_bufferFull = false;
            }
        } else {
            m_bufferPosition = m_bufferedBiasStart;
        }

        if (m_bufferedFileStart && offset_adjusted) {
            return m_bufferedFileStart + m_bufferedBiasStart;
        }

        return m_bufferedBiasStart;
    }

    return RawFileClass::Seek(offset, whence);
}

off_t BufferIOFileClass::Size()
{
    if (m_bufferedFileAvailable && m_buffered) {
        return m_bufferedFileSize;
    }

    return RawFileClass::Size();
}

void BufferIOFileClass::Close()
{
    if (m_buffered) {
        BufferIOFileClass::Commit();

        if (m_bufferedFileOpen) {
            if (m_bufferedFileStart) {
                m_buffered = false;

                Close();

                m_buffered = true;
                m_bufferedFileOpen = false;
                m_bufferedFileAvailable = false;
            }

            RawFileClass::Close();
            m_bufferedFileOpen = false;
        }

        m_bufferedFileAvailable = false;

    } else {
        RawFileClass::Close();
    }
}

#ifndef CHRONOSHIFT_STANDALONE

void BufferIOFileClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x005BD734, &Hook_Is_Available);
    Hook_Function(0x005BD754, &Hook_Is_Open);
    Hook_Function(0x005BD780, &Hook_Open_Name);
    Hook_Function(0x005BD79C, &Hook_Open);
    Hook_Function(0x005BDAD4, &Hook_Read);
    Hook_Function(0x005BDCF4, &Hook_Seek);
    Hook_Function(0x005BDDF0, &Hook_Size);
    Hook_Function(0x005BD870, &Hook_Write);
    Hook_Function(0x005BDE14, &Hook_Close);
#endif
}

BOOL BufferIOFileClass::Hook_Is_Available(BufferIOFileClass *ptr, BOOL forced)
{
    return ptr->BufferIOFileClass::Is_Available(forced);
}

BOOL BufferIOFileClass::Hook_Is_Open(BufferIOFileClass *ptr)
{
    return ptr->BufferIOFileClass::Is_Open();
}

BOOL BufferIOFileClass::Hook_Open_Name(BufferIOFileClass *ptr, const char *filename, int rights)
{
    return ptr->BufferIOFileClass::Open(filename, rights);
}

BOOL BufferIOFileClass::Hook_Open(BufferIOFileClass *ptr, int rights)
{
    return ptr->BufferIOFileClass::Open(rights);
}

int BufferIOFileClass::Hook_Read(BufferIOFileClass *ptr, void *buffer, int length)
{
    return ptr->BufferIOFileClass::Read(buffer, length);
}

off_t BufferIOFileClass::Hook_Seek(BufferIOFileClass *ptr, off_t offset, int whence)
{
    return ptr->BufferIOFileClass::Seek(offset, whence);
}

off_t BufferIOFileClass::Hook_Size(BufferIOFileClass *ptr)
{
    return ptr->BufferIOFileClass::Size();
}

int BufferIOFileClass::Hook_Write(BufferIOFileClass *ptr, const void *buffer, int length)
{
    return ptr->BufferIOFileClass::Write(buffer, length);
}

void BufferIOFileClass::Hook_Close(BufferIOFileClass *ptr)
{
    ptr->BufferIOFileClass::Close();
}
#endif
