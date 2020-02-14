/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief FileClass for reading files with raw OS API calls.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "rawfile.h"
#include <captainslog.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <malloc.h>
#include <string.h> // for strdup
#include <sys/stat.h>
#include <sys/types.h>

using std::remove;

// Headers needed for posix open, close, read... etc.
#ifdef PLATFORM_WINDOWS
#include <io.h>
#include <winbase.h>

// Make lseek 64bit on windows to match other platforms behaviour?
//#ifdef lseek
//    #undef lseek
//#endif

//#ifdef off_t
//    #undef off_t
//#endif

//#define lseek _lseeki64
// typedef __int64 off_t;
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_UTIME_H
#include <utime.h>
#elif defined HAVE_SYS_UTIME_H
#include <sys/utime.h>
#endif

RawFileClass::RawFileClass() :
    m_Rights(FM_CLOSED),
    m_BiasStart(0),
    m_BiasLength(-1),
#ifdef PLATFORM_WINDOWS
    m_Handle(INVALID_HANDLE_VALUE),
#else
    m_Handle(-1),
#endif
    m_Filename(nullptr),
    m_IsAllocated(false),
    m_DateTime(0)
{
    Set_Name("");
};

RawFileClass::RawFileClass(const char *filename) :
    m_Rights(FM_CLOSED),
    m_BiasStart(0),
    m_BiasLength(-1),
#ifdef PLATFORM_WINDOWS
    m_Handle(INVALID_HANDLE_VALUE),
#else
    m_Handle(-1),
#endif
    m_Filename(nullptr),
    m_IsAllocated(false),
    m_DateTime(0)
{
    Set_Name(filename);
};

void RawFileClass::Reset()
{
    // Close an open file handle.
    Close();

    // free the existing filename if it exists.
    if (m_Filename != nullptr && m_IsAllocated) {
        free(m_Filename);

        m_Filename = nullptr;
        m_IsAllocated = false;
    }

    // clear the date/time.
    m_DateTime = 0;
}

BOOL RawFileClass::Create()
{
    // if file open, close it file before we can create it.
    Close();

    // open the file with write access.
    if (Open(FM_WRITE)) {
        if (m_BiasLength != -1) {
            Seek(0, FS_SEEK_START);
        }

        Close();

        return true;
    }

    return false;
}

BOOL RawFileClass::Delete()
{
    // if file open, close it file before we delete it.
    Close();

    // is the filename valid and the file available?
    if (m_Filename == nullptr) {
        Error(2);
    } else if (Is_Available()) {
        // delete the file.
        if (remove(m_Filename)) {
            return true;
        } else {
            Error(errno, 0, m_Filename);
            return false;
        }
    }

    return false;
}

BOOL RawFileClass::Open(const char *filename, int rights)
{
    Set_Name(filename);

    return Open(rights);
}

BOOL RawFileClass::Open(int rights)
{
    // close the file if it is already open.
    RawFileClass::Close();

    // make sure we have a valid filename set.
    if (m_Filename == nullptr) {
        Error(2);
    }

    // set the file rights.
    m_Rights = rights;

#ifdef PLATFORM_WINDOWS
    switch (rights) {
        case FM_READ:
            m_Handle = CreateFileA(m_Filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
            break;

        case FM_WRITE:
            m_Handle = CreateFileA(m_Filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
            break;

        // Based on the RA code, this is actually READ/WRITE
        case FM_READ | FM_WRITE:
            m_Handle = CreateFileA(m_Filename, GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
            break;

        default:
            break;
    }
#else // _WIN32

    switch (rights) {
        case FM_READ:
            m_Handle = open(m_Filename, O_RDONLY, S_IREAD);
            break;
        case FM_WRITE:
            m_Handle = open(m_Filename, O_WRONLY | O_CREAT | O_TRUNC, S_IWRITE);
            break;
        case FM_READ_WRITE:
            m_Handle = open(m_Filename, O_RDWR | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE);
            break;
        default:
            break;
    }
#endif

    if (m_BiasStart || m_BiasLength != -1) {
        RawFileClass::Seek(0, FS_SEEK_START);
    }

#ifdef PLATFORM_WINDOWS
    if (m_Handle == INVALID_HANDLE_VALUE) {
#else
    if (m_Handle == -1) {
#endif
        Error(errno, 0, m_Filename);
        return false;
    }

    return true;
}

BOOL RawFileClass::Is_Available(BOOL forced)
{
    // if the filename is invalid, the file is not available.
    if (m_Filename == nullptr) {
        return false;
    }

    // if the file is already open, it is available.
    if (Is_Open()) {
        return true;
    }

    // Mode appears to indicated if we want to keep the current value of m_handle or not
    if (forced) {
        RawFileClass::Open(FM_READ);
        RawFileClass::Close();

        return true;
    }

#ifdef PLATFORM_WINDOWS
    m_Handle = CreateFileA(m_Filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (m_Handle != INVALID_HANDLE_VALUE) {
        // close the file handle/stream.
        if (!CloseHandle(m_Handle)) {
            Error(GetLastError(), 0, m_Filename);
        }

        m_Handle = INVALID_HANDLE_VALUE;

        return true;
    }
#else
    m_Handle = open(m_Filename, O_RDONLY, S_IREAD);

    if (m_Handle != -1) {
        // close the file handle/stream.
        if (close(m_Handle) == -1) {
            Error(errno, 0, m_Filename);
        }

        m_Handle = -1;

        return true;
    }
#endif

    return false;
}

void RawFileClass::Close()
{
    // if the file is NOT open, we can return.
    if (Is_Open()) {
#ifdef PLATFORM_WINDOWS
        if (!CloseHandle(m_Handle)) {
            Error(GetLastError(), 0, m_Filename);
        }

        m_Handle = INVALID_HANDLE_VALUE;
#else

        // close the file handle/stream.
        if (close(m_Handle) == -1) {
            Error(errno, 0, m_Filename);
        }

        m_Handle = -1;
#endif
    }
}

int RawFileClass::Read(void *buffer, int length)
{
    bool opened = false; // have we opened the file to allow us to read?

    if (!Is_Open()) {
        if (!Open(FM_READ)) {
            return 0;
        }

        opened = true;
    }

    if (m_BiasLength != -1) {
        int tmplen = m_BiasLength - Seek(0, FS_SEEK_CURRENT);

        if (tmplen > length) {
            tmplen = length;
        }

        length = tmplen;
    }

    int totalread = 0;

    while (length > 0) {
#ifdef PLATFORM_WINDOWS
        int readlen;
        SetErrorMode(SEM_FAILCRITICALERRORS);

        if (ReadFile(m_Handle, buffer, length, reinterpret_cast<LPDWORD>(&readlen), 0)) {
            SetErrorMode(0);
            length -= readlen;
            totalread += readlen;

            if (!readlen) {
                break;
            }
        } else {
            length -= readlen;
            totalread += readlen;

            Error(GetLastError(), 0, m_Filename);
            SetErrorMode(0);
            break;
        }
#else
        int readlen = read(m_Handle, buffer, length);
        captainslog_debug("Read %d out of attempted %d bytes.", readlen, length);

        if (readlen == 0) {
            readlen = read(m_Handle, buffer, length);
            captainslog_debug("Attempting to get more data after read of 0, got %d.", readlen);
        }

        if (readlen >= 0) {
            length -= readlen;
            totalread += readlen;

            if (!readlen) {
                break;
            }
        } else {
            Error(errno, 0, m_Filename);
            break;
        }
#endif
    }

    if (opened) {
        Close();
    }

    return totalread;
}

off_t RawFileClass::Seek(off_t offset, int whence)
{
    if (m_BiasLength == -1) {
        return Raw_Seek(offset, whence);
    }

    switch (whence) {
        case FS_SEEK_START:
            if (offset > m_BiasLength) {
                offset = m_BiasLength;
            }

            offset += m_BiasStart;

            break;

        case FS_SEEK_CURRENT:
            break;

        case FS_SEEK_END:
            offset += m_BiasLength + m_BiasStart;
            break;

        default:
            break;
    }

    off_t seekval = Raw_Seek(offset, whence) - m_BiasStart;

    if (seekval < 0) {
        return Raw_Seek(m_BiasStart, FS_SEEK_START) - m_BiasStart;
    }

    if (seekval > m_BiasLength) {
        return Raw_Seek(m_BiasLength + m_BiasStart, FS_SEEK_START) - m_BiasStart;
    }

    return seekval;
}

off_t RawFileClass::Size()
{
    /*
    stat_t attrib;

    if (m_biasLength == -1) {
        if (stat(m_filename, &attrib) == 0) {
            m_biasLength = attrib.st_size - m_biasStart;
        }
    }

    return m_biasLength;
    */

    int size = 0;

    if (m_BiasLength == -1) {
        if (Is_Open()) {
#ifdef PLATFORM_WINDOWS
            size = GetFileSize(m_Handle, 0);
            if (size == -1) {
                Error(GetLastError(), 0, m_Filename);
            }
#else

            off_t cur = lseek(m_Handle, 0, FS_SEEK_CURRENT);

            size = lseek(m_Handle, 0, FS_SEEK_END);

            if (size == -1) {
                Error(errno, 0, m_Filename);
            }

            // reset our pos in the file
            lseek(m_Handle, cur, FS_SEEK_START);
#endif
        } else if (Open(FM_READ)) {
            size = Size();
            Close();
        }

        m_BiasLength = size - m_BiasStart;
    }

    return m_BiasLength;
}

int RawFileClass::Write(const void *buffer, int length)
{
    int writelen = 0; // total bytes written to file.
    bool opened = false; // have we opened the file to allow us to write?

    if (!Is_Open()) {
        if (!Open(FM_WRITE)) {
            return 0;
        }

        opened = true;
    }

// write the data in the buffer to the file, and
// store the total bytes written.
#if defined(_WIN32)
    if (!WriteFile(m_Handle, buffer, length, reinterpret_cast<LPDWORD>(&writelen), 0)) {
        Error(GetLastError(), 0, m_Filename);
    }
#else

    writelen = write(m_Handle, buffer, length);

    if (writelen < 0) {
        Error(errno, 0, m_Filename);
    }
#endif
    if (m_BiasLength != -1 && Raw_Seek(0, FS_SEEK_CURRENT) > (int)(m_BiasLength + m_BiasStart)) {
        m_BiasLength = Raw_Seek(0, FS_SEEK_CURRENT) - m_BiasStart;
    }

    // close the file if we opened it.
    if (opened) {
        Close();
    }

    return writelen;
}

const char *RawFileClass::Set_Name(const char *filename)
{
    // free the existing filename if it exists.
    if (m_Filename && m_IsAllocated) {
        free(m_Filename);
        m_Filename = nullptr;
        m_IsAllocated = false;
    }

    // if the argument 'filename' is valid, set this as the filename.
    if (filename != nullptr) {
        // reset the file bias.
        Bias(0, -1);

        m_Filename = strdup(filename);

        // if we could not copy the filename, return a error.
        if (m_Filename == nullptr) {
            Error(12, 0, m_Filename);

            return 0;
        }

        // the filename has been set succesfully.
        m_IsAllocated = true;

        return m_Filename;
    }

    return nullptr;
}

time_t RawFileClass::Get_Date_Time()
{
    struct stat attrib;

    // get stats
    if (stat(m_Filename, &attrib) == 0) {
        return attrib.st_mtime;
    }

    return 0;
}

BOOL RawFileClass::Set_Date_Time(time_t datetime)
{
    struct utimbuf tstamp;

    // assumes datetime is time_t format
    tstamp.modtime = datetime;
    tstamp.actime = datetime;

    if (Is_Open()) {
        // set the file time and return if it succeeded
        return utime(m_Filename, &tstamp) != 0;
    }

    return false;
}

void RawFileClass::Error(int error, BOOL can_retry, const char *filename)
{
    // Nothing in RA
    captainslog_debug("Triggered error %d for file '%s'.", error, filename);
}

void RawFileClass::Bias(int offset, int length)
{
    if (offset) {
        int fsize = RawFileClass::Size();

        m_BiasLength = fsize;
        m_BiasStart += offset;

        if (length != -1) {
            if (fsize >= length) {
                fsize = length;
            }

            m_BiasLength = fsize;
        }

        m_BiasLength &= (m_BiasLength >> 31) - 1;

        // if the file is open, reset seek position.
        if (Is_Open()) {
            RawFileClass::Seek(0, FS_SEEK_START);
        }
    } else {
        m_BiasStart = 0;
        m_BiasLength = -1;
    }
}

off_t RawFileClass::Raw_Seek(off_t offset, int whence)
{
    // if the file is not open, raise an error.
    if (!Is_Open()) {
        Error(9, 0, m_Filename);
        return 0;
    }
#ifdef PLATFORM_WINDOWS
    int retval = 0;

    // seek to the offset specified, return the position.
    // and if fseek returned with a error, raise an error too.
    retval = SetFilePointer(m_Handle, offset, 0, whence);
    if (retval == -1) {
        Error(GetLastError(), 0, m_Filename);
        return 0;
    }
#else
    int retval = lseek(m_Handle, offset, whence);

    // seek to the offset specified, return the position.
    // and if lseek returned with a error, raise an error too.
    if (retval == -1) {
        Error(errno, 0, m_Filename);
        return 0;
    }
#endif
    return retval;
}
