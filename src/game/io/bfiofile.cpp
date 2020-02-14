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
#include <cstring>
#include <algorithm>
#include <captainslog.h>

using std::strcmp;

BufferIOFileClass::BufferIOFileClass() :
    m_BufferAllocated(false),
    m_BufferedFileAvailable(false),
    m_BufferedFileOpen(false),
    m_BufferFull(false),
    m_Uncommited(false),
    m_Buffered(false),
    m_BufferedRights(FM_CLOSED),
    m_Buffer(nullptr),
    m_BufferedSize(0),
    m_BufferPosition(0),
    m_BufferStart(0),
    m_UncommitedStart(-1),
    m_UncommitedEnd(-1),
    m_BufferedFileSize(0),
    m_BufferedBiasStart(0),
    m_BufferedFileStart(0)
{
}

BufferIOFileClass::BufferIOFileClass(const char *filename) :
    m_BufferAllocated(false),
    m_BufferedFileAvailable(false),
    m_BufferedFileOpen(false),
    m_BufferFull(false),
    m_Uncommited(false),
    m_Buffered(false),
    m_BufferedRights(FM_CLOSED),
    m_Buffer(nullptr),
    m_BufferedSize(0),
    m_BufferPosition(0),
    m_BufferStart(0),
    m_UncommitedStart(-1),
    m_UncommitedEnd(-1),
    m_BufferedFileSize(0),
    m_BufferedBiasStart(0),
    m_BufferedFileStart(0)
{
    BufferIOFileClass::Set_Name(filename);
}

BOOL BufferIOFileClass::Cache(int size, void *buffer)
{
    if (m_Buffer) {
        return size == 0 && buffer == nullptr;
    }

    if (Is_Available()) {
        m_BufferedFileSize = Size();
    } else {
        m_BufferedFileSize = 0;
    }

    if (size > 0) {
        if (size < MIN_CACHE_SIZE) {
            size = MIN_CACHE_SIZE;
            if (buffer) {
                // Passed a buffer that is smaller than our min size
                Error(9);
            }
        }
        m_BufferedSize = size;
    } else {
        m_BufferedSize = m_BufferedFileSize;
    }

    if ((size || buffer == nullptr) && m_BufferedSize > 0) {
        if (buffer) {
            m_Buffer = buffer;
        } else {
            m_Buffer = new char[m_BufferedSize];
        }

        if (m_Buffer == nullptr) {
            Error(5);
            return false;
        }

        m_BufferAllocated = true;
        m_BufferedFileOpen = false;
        m_BufferPosition = 0;
        m_BufferStart = 0;
        m_UncommitedStart = -1;
        m_UncommitedEnd = -1;
        m_BufferedBiasStart = 0;
        m_BufferedFileStart = 0;

        if (m_BufferedFileSize > 0) {
            bool opened = false;
            int startpos = 0;

            int sizetocache = std::min(m_BufferedFileSize, m_BufferedSize);

            if (Is_Open()) {
                startpos = Tell();

                if (Is_Open()) {
                    m_BufferedFileStart = RawFileClass::Tell();
                } else {
                    m_BufferedFileStart = startpos;
                }

                if (m_BufferedFileSize > m_BufferedSize) {
                    m_BufferStart = startpos;
                } else {
                    if (startpos > 0)
                        Seek(0, FS_SEEK_START);

                    m_BufferPosition = startpos;
                }

                m_BufferedBiasStart = startpos;

            } else {
                if (Open()) {
                    m_BufferedFileStart = RawFileClass::Tell();
                    opened = true;
                }
            }

            if (Read(m_Buffer, sizetocache) != sizetocache) {
                Error(21);
            }

            if (opened) {
                Close();
            } else {
                RawFileClass::Tell();
            }

            m_BufferFull = true;
        }

        m_Buffered = true;

        return true;
    }

    return false;
}

void BufferIOFileClass::Free()
{
    if (m_Buffer) {
        if (m_BufferAllocated) {
            delete[] static_cast<char *>(m_Buffer);
            m_BufferAllocated = false;
        }

        m_Buffer = nullptr;
    }

    m_BufferedFileAvailable = false;
    m_BufferedFileOpen = false;
    m_Uncommited = false;
    m_BufferedSize = 0;
    m_Buffered = false;
}

BOOL BufferIOFileClass::Commit()
{
    if (m_Buffered && m_Uncommited) {
        int writesize = m_UncommitedEnd - m_UncommitedStart;

        if (m_BufferedFileOpen) {
            RawFileClass::Seek(m_UncommitedStart + m_BufferedFileStart + m_BufferStart, FS_SEEK_START);
            RawFileClass::Write(m_Buffer, writesize);
            RawFileClass::Seek(m_BufferedFileStart + m_BufferedBiasStart, FS_SEEK_START);
        } else {
            RawFileClass::Open(FM_WRITE);
            RawFileClass::Seek(m_UncommitedStart + m_BufferStart + m_BufferedFileStart, FS_SEEK_START);
            RawFileClass::Write(m_Buffer, writesize);
            RawFileClass::Close();
        }

        m_Uncommited = false;

        return true;
    }

    return false;
}

const char *BufferIOFileClass::Set_Name(const char *filename)
{
    if (File_Name() != nullptr && m_Buffered) {
        if (strcmp(filename, File_Name()) == 0) {
            return File_Name();
        }

        Commit();
        m_BufferFull = false;
    }

    RawFileClass::Set_Name(filename);

    return File_Name();
}

BOOL BufferIOFileClass::Is_Available(BOOL forced)
{
    if (m_Buffered) {
        return true;
    }

    return RawFileClass::Is_Available(forced);
}

BOOL BufferIOFileClass::Is_Open() const
{
    if (m_BufferedFileAvailable && m_Buffered) {
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

    if (m_Buffered) {
        m_BufferedRights = rights;

        if (rights == FM_READ && m_BufferedFileSize <= m_BufferedSize) {
            m_BufferedFileOpen = false;

        } else {
            if (rights == FM_WRITE) {
                RawFileClass::Open(rights);
                RawFileClass::Close();
                m_BufferedFileStart = 0;
            }

            if (m_BufferedFileStart) {
                m_Buffered = false;
                Open(rights);
                m_Buffered = true;
            } else {
                RawFileClass::Open(rights);
            }

            m_BufferedFileOpen = true;

            if (m_BufferedRights == FM_WRITE) {
                m_BufferedFileSize = 0;
            }
        }

        m_BufferPosition = 0;
        m_BufferStart = 0;
        m_UncommitedStart = -1;
        m_UncommitedEnd = -1;
        m_BufferedBiasStart = 0;
        m_BufferedFileAvailable = true;

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
            m_BufferedFileStart = Tell();
            opened = true;
        }
    }

    if (m_Buffered) {
        int totalwrite = 0;

        if (m_BufferedRights == FM_READ) {
            Error(13);

        } else {
            while (length) {
                movedsize = std::min(length, (m_BufferedSize - m_BufferPosition));

                if (movedsize != m_BufferedSize && !m_BufferFull) {
                    if (m_BufferedFileSize >= m_BufferedSize) {
                        readsize = m_BufferedSize;
                        m_BufferStart = m_BufferedBiasStart;
                    } else {
                        readsize = m_BufferedFileSize;
                        m_BufferStart = 0;
                    }

                    if (m_BufferedFileStart) {
                        m_Buffered = false;
                        Seek(m_BufferedBiasStart, FS_SEEK_START);
                        Read(m_Buffer, m_BufferedSize);
                        Seek(m_BufferedBiasStart, FS_SEEK_START);
                        m_Buffered = true;
                    } else {
                        RawFileClass::Seek(m_BufferStart, FS_SEEK_START);
                        RawFileClass::Read(m_Buffer, readsize);
                    }

                    m_BufferPosition = 0;
                    m_UncommitedStart = -1;
                    m_UncommitedEnd = -1;
                    m_BufferFull = true;
                }

                memmove(static_cast<char *>(m_Buffer) + m_BufferPosition,
                    static_cast<const char *>(buffer) + totalwrite,
                    movedsize);

                m_Uncommited = true;
                totalwrite += movedsize;
                length -= movedsize;

                if (m_UncommitedStart == -1) {
                    m_UncommitedStart = m_BufferPosition;
                    m_UncommitedEnd = m_BufferPosition;
                } else {
                    if (m_UncommitedStart > m_BufferPosition) {
                        m_UncommitedStart = m_BufferPosition;
                    }
                }
            }

            m_BufferPosition += movedsize;
            m_UncommitedEnd = std::max(m_UncommitedEnd, m_BufferPosition);
            m_BufferedBiasStart = m_BufferPosition + m_BufferStart;
            m_BufferedFileSize = std::max(m_BufferedFileSize, m_BufferedBiasStart);

            if (m_BufferPosition == m_BufferedSize) {
                Commit();
                m_BufferPosition = 0;
                m_BufferStart = m_BufferedBiasStart;
                m_UncommitedStart = -1;
                m_UncommitedEnd = -1;

                if (length && m_BufferedFileSize > m_BufferedBiasStart) {
                    if (m_BufferedFileStart) {
                        m_Buffered = false;
                        Seek(m_BufferedBiasStart, FS_SEEK_START);
                        Read(m_Buffer, m_BufferedSize);
                        Seek(m_BufferedBiasStart, FS_SEEK_START);
                        m_Buffered = true;
                    } else {
                        RawFileClass::Seek(m_BufferedBiasStart, FS_SEEK_START);
                        RawFileClass::Read(m_Buffer, m_BufferedSize);
                    }
                } else {
                    m_BufferFull = false;
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
        m_BufferedFileStart = RawFileClass::Tell();
        opened = true;
    }

    if (m_Buffered) {
        int totalread = 0;

        if (m_BufferedRights == FM_WRITE) {
            Error(13);

        } else {
            while (size > 0) {
                movedsize = std::min(size, (m_BufferedSize - m_BufferPosition));

                if (!m_BufferFull) {
                    if (m_BufferedFileSize >= m_BufferedSize) {
                        readsize = m_BufferedSize;
                        m_BufferStart = m_BufferedBiasStart;
                    } else {
                        readsize = m_BufferedFileSize;
                        m_BufferStart = 0;
                    }

                    if (m_BufferedFileStart) {
                        m_Buffered = false;
                        Seek(m_BufferedBiasStart, FS_SEEK_START);
                        Read(m_Buffer, m_BufferedSize);
                        Seek(m_BufferedBiasStart, FS_SEEK_START);
                        m_Buffered = true;

                    } else {
                        RawFileClass::Seek(m_BufferStart, FS_SEEK_START);
                        RawFileClass::Read(m_Buffer, readsize);
                    }

                    m_BufferPosition = 0;
                    m_UncommitedStart = -1;
                    m_UncommitedEnd = -1;
                    m_BufferFull = true;
                }

                memmove(
                    static_cast<char *>(buffer) + totalread, static_cast<char *>(m_Buffer) + m_BufferPosition, movedsize);

                totalread += movedsize;
                size -= movedsize;

                m_BufferPosition += movedsize;
                m_BufferedBiasStart = m_BufferPosition + m_BufferStart;

                if (m_BufferPosition == m_BufferedSize) {
                    Commit();
                    m_BufferPosition = 0;
                    m_BufferStart = m_BufferedBiasStart;
                    m_UncommitedStart = -1;
                    m_UncommitedEnd = -1;

                    if (size && m_BufferedFileSize > m_BufferedBiasStart) {
                        if (m_BufferedFileStart) {
                            m_Buffered = false;
                            Seek(m_BufferedBiasStart, FS_SEEK_START);
                            Read(m_Buffer, m_BufferedSize);
                            Seek(m_BufferedBiasStart, FS_SEEK_START);
                            m_Buffered = true;

                        } else {
                            // DEBUG_SAY("BufferIOFileClass::Read(%s) - Calling RawFileClass::Seek()\n", File_Name());
                            RawFileClass::Seek(m_BufferedBiasStart, FS_SEEK_START);

                            // DEBUG_SAY("BufferIOFileClass::Read(%s) - Calling RawFileClass::Read()\n", File_Name());
                            RawFileClass::Read(m_Buffer, m_BufferedSize);
                        }

                    } else {
                        m_BufferFull = false;
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
    if (m_Buffered) {
        BOOL offset_adjusted = false;

        if (whence == FS_SEEK_START) {
            m_BufferedBiasStart = 0;
        } else {
            if (whence == FS_SEEK_END) {
                m_BufferedBiasStart = m_BufferedFileSize;
            }
        }

        if (m_BufferedFileStart && offset >= m_BufferedFileStart) {
            offset = offset - m_BufferedFileStart;
            offset_adjusted = true;
        }

        m_BufferedBiasStart += offset;

        if (m_BiasStart < 0) {
            m_BufferedBiasStart = 0;
        }

        if (m_BufferedBiasStart > m_BufferedFileSize) {
            m_BufferedBiasStart = m_BufferedFileSize;
        }

        if (m_BufferedFileSize > m_BufferedSize) {
            if (m_BufferedBiasStart >= m_BufferStart && m_BufferedSize + m_BufferStart > m_BufferedBiasStart) {
                m_BufferPosition = m_BufferedBiasStart - m_BufferStart;

            } else {
                Commit();

                if (m_BufferedFileStart) {
                    m_Buffered = false;
                    Seek(m_BufferedBiasStart, FS_SEEK_START);
                    m_Buffered = true;

                } else {
                    RawFileClass::Seek(m_BufferedBiasStart, FS_SEEK_START);
                }
                m_BufferFull = false;
            }
        } else {
            m_BufferPosition = m_BufferedBiasStart;
        }

        if (m_BufferedFileStart && offset_adjusted) {
            return m_BufferedFileStart + m_BufferedBiasStart;
        }

        return m_BufferedBiasStart;
    }

    return RawFileClass::Seek(offset, whence);
}

off_t BufferIOFileClass::Size()
{
    if (m_BufferedFileAvailable && m_Buffered) {
        return m_BufferedFileSize;
    }

    return RawFileClass::Size();
}

void BufferIOFileClass::Close()
{
    if (m_Buffered) {
        BufferIOFileClass::Commit();

        if (m_BufferedFileOpen) {
            if (m_BufferedFileStart) {
                m_Buffered = false;

                Close();

                m_Buffered = true;
                m_BufferedFileOpen = false;
                m_BufferedFileAvailable = false;
            }

            RawFileClass::Close();
            m_BufferedFileOpen = false;
        }

        m_BufferedFileAvailable = false;

    } else {
        RawFileClass::Close();
    }
}
