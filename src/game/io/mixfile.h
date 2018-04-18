/**
 * @file
 *
 * @Author CCHyper, OmniBlade
 *
 * @brief  Handles the "Mix" file archives used by Westwood games.
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

#ifndef MIXFILE_H
#define MIXFILE_H

#include "always.h"
#include "cd.h"
#include "crc.h"
#include "fileclass.h"
#include "filestraw.h"
#include "gamedebug.h"
#include "listnode.h"
//#include "obscure.h"
#include "buffer.h"
#include "endiantype.h"
#include "pkstraw.h"
#include "rndstraw.h"
#include "sha.h"
#include "shastraw.h"
#include "startup.h"
#include "stringex.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef PLATFORM_WINDOWS
// Needed for basename
#include <dirent.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif // !PLATFORM_WINDOWS

#define MIX_CACHED_CHUNK_SIZE 4096

enum MixFlags
{
    HAS_CHECKSUM = 0x00010000,
    IS_ENCRYPTED = 0x00020000
};

#pragma pack(push, 1)
struct MIXHeaderStruct
{
    int16_t file_count; // Number Of Files In the MIX
    int32_t file_size; // Size Of The Body
};

struct FileInfoStruct
{
    int32_t m_crc; // The hash ID of The File, derived from the name of the file
    int32_t m_offset; // Offset Of The FIle From The Start Of The Body
    int32_t m_size; // Size Of The File
};
#pragma pack(pop)

inline int FileInfo_Compare(const void *info1, const void *info2)
{
    if (static_cast<const FileInfoStruct *>(info1)->m_crc >= static_cast<const FileInfoStruct *>(info2)->m_crc) {
        return static_cast<const FileInfoStruct *>(info1)->m_crc > static_cast<const FileInfoStruct *>(info2)->m_crc ? 1 : 0;
    }

    return -1;
}

template<class FC>
class MixFileClass : public Node<MixFileClass<FC> *>
{
#ifndef COMPILER_WATCOM
    // Looks like watcom doesn't like these declarations, newer compilers need them for standards compliance related to
    // template lookup.
    using Node<MixFileClass<FC> *>::Unlink;
#endif
public:
    MixFileClass(const char *filename, PKey *key);
    MixFileClass(FC &file, PKey *key);
    ~MixFileClass();

    bool Cache(BufferClass *buffer = nullptr);
    void Invalidate_Cache();
    char *Get_Filename() const { return m_fileName; }
    int Get_File_Count() const { return m_fileCount; }
    uint32_t Get_Size() const { return m_fileSize; }
    const FileInfoStruct *Get_Index() const { return m_fileIndex; }

    static void Discard_Cache(const char *filename);
    static void *Retrieve(const char *filename);
    static MixFileClass<FC> *Finder(const char *filename);
    static BOOL Cache(const char *filename, BufferClass *buffer = nullptr);
    static BOOL Offset(const char *filename, void **cachedfile = nullptr, MixFileClass<FC> **mixfile = nullptr,
        int *file_offset = 0, int *file_size = 0);
    static void Hook_Load_File(MixFileClass *ptr, FC &file, PKey *key) { ptr->Load_File(file, key); }

private:
    // Private load function shared by the ctors.
    void Load_File(FC &file, PKey *key);

    // Union to handle two possible uses of first DWORD in mix file
    union MixFlags
    {
        int16_t file_count;
        uint32_t flags;
    };

private:
    char *m_fileName;
#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool m_hasChecksum : 1;
            bool m_isEncrypted : 1;
            bool m_isAllocated : 1;
        };
        int m_flags;
    };
#else
    bool m_hasChecksum;
    bool m_isEncrypted;
    bool m_isAllocated;
    bool m_useObscureHash;
#endif
    int m_fileCount;
    int m_fileSize; // size of the body, not including this header and the index.
    int m_fileStart;
    FileInfoStruct *m_fileIndex;
    uint8_t *m_fileCache;
// A linked list of all currently indexed mix files for the program
#ifndef RAPP_STANDALONE
    static List<MixFileClass<FC> *> &s_mixList;
#else
    static List<MixFileClass<FC> *> s_mixList;
#endif
};

// Initialised for CCFileClass in mixfile.cpp
// template<class FC>
// List<MixFileClass<FC> *> MixFileClass<FC>::s_mixList;

template<class FC>
MixFileClass<FC>::MixFileClass(const char *filename, PKey *key) :
    m_fileName(nullptr),
    m_hasChecksum(false),
    m_isEncrypted(false),
    m_isAllocated(false),
    m_fileCount(0),
    m_fileSize(0),
    m_fileStart(0),
    m_fileIndex(nullptr),
    m_fileCache(nullptr)
{
    DEBUG_ASSERT(filename != nullptr);
    DEBUG_ASSERT(key != nullptr);

    if (!Force_CD_Available(g_requiredCD)) {
        Emergency_Exit(0xFF);
    }

    // Create a working file class to handle our mix file.
    FC file(filename);

    Load_File(file, key);
}

template<class FC>
MixFileClass<FC>::MixFileClass(FC &file, PKey *key) :
    m_fileName(nullptr),
    m_hasChecksum(false),
    m_isEncrypted(false),
    m_isAllocated(false),
    m_fileCount(0),
    m_fileSize(0),
    m_fileStart(0),
    m_fileIndex(nullptr),
    m_fileCache(nullptr)
{
    DEBUG_ASSERT(key != nullptr);

    Load_File(file, key);
}

template<class FC>
void MixFileClass<FC>::Load_File(FC &file, PKey *key)
{
    // Store the filename for the mix file.
    m_fileName = strdup(file.File_Name());

    if (file.Is_Available()) {
        // CryptRandom is a global instance of RandomStraw, see extern above.
        PKStraw pkstrw(STRAW_DECRYPT, g_cryptRandom);
        FileStraw filestrw(&file);

        // This Straw pointer will point to which straw we need to read the header
        // and index depending on the initial flags we get with a FileStraw
        Straw *straw_to_use = &filestrw;

        // This fetches the first 4 bytes, either a flag or part of the header
        MixFlags file_flags;
        straw_to_use->Get(&file_flags, sizeof(MixFlags));

        // If the first short of file flags has a value, it is the file count and
        // we don't actually have flags in this mix.
        MIXHeaderStruct tmp_header;

        if (file_flags.file_count) {
            // Getting the flags when we don't have any gets us the file count
            // and part of the file size. This gets us the rest of it into our
            // temporary header struct
            memmove(&tmp_header, &file_flags, sizeof(MixFlags));
            straw_to_use->Get(reinterpret_cast<uint8_t *>(&tmp_header.file_size) + 2, 2);
        } else {
            unsigned int flags = le32toh(file_flags.flags);
            m_hasChecksum = (flags & HAS_CHECKSUM) ? true : false;
            m_isEncrypted = (flags & IS_ENCRYPTED) ? true : false;

            // If encrypted, set the PKStraw key and set it as the Straw we are
            // using.
            if (m_isEncrypted) {
                pkstrw.Key(key);
                pkstrw.Get_From(&filestrw);
                straw_to_use = &pkstrw;
            }

            // Load the header from the file.
            straw_to_use->Get(&tmp_header, sizeof(MIXHeaderStruct));
        }

        m_fileCount = le16toh(tmp_header.file_count);
        m_fileSize = le32toh(tmp_header.file_size);
        m_fileIndex = new FileInfoStruct[m_fileCount];

        if (m_fileIndex) {
            straw_to_use->Get(m_fileIndex, sizeof(FileInfoStruct) * m_fileCount);
            m_fileStart = file.Seek(0, FS_SEEK_CURRENT) + file.Get_Bias_Start();

#if defined(SYSTEM_BIG_ENDIAN)
            // Mix files are a little endian file format, so when we load on a
            // big endian system, we need to byte swap everything in the index
            for (int index = 0; index < m_fileCount; ++index) {
                m_fileIndex[index].m_crc = le32toh(m_fileIndex[index].m_crc);
                m_fileIndex[index].m_offset = le32toh(m_fileIndex[index].m_offset);
                m_fileIndex[index].m_size = le32toh(m_fileIndex[index].m_size);
            }
#endif // !SYSTEM_BIG_ENDIAN

            // Add the mix file to the global list.
            s_mixList.Add_Tail(this);
        }
    }
}

template<class FC>
MixFileClass<FC>::~MixFileClass()
{
    if (m_fileName) {
        free((void *)m_fileName);
    }

    m_fileName = nullptr;

    if (m_fileIndex) {
        delete[] m_fileIndex;
    }

    m_fileIndex = nullptr;

    Invalidate_Cache();
    Unlink();
}

/**
 * @brief Retrieve a pointer to a file's data if the mix containing it is cached in memory already.
 */
template<class FC>
void *MixFileClass<FC>::Retrieve(const char *filename)
{
    DEBUG_ASSERT(filename != nullptr);
    // This is a pointer to the retrieved file data.
    void *data = nullptr;
    Offset(filename, &data);

    return data;
}

/**
 * @brief Find the first MixFileClass instance that matches the provided name.
 */
template<class FC>
MixFileClass<FC> *MixFileClass<FC>::Finder(const char *filename)
{
    DEBUG_ASSERT(filename != nullptr);

    MixFileClass<FC> *file = nullptr;

#if defined(PLATFORM_WINDOWS)

    char ext[NAME_MAX];
    char fname[NAME_MAX];
    char dest[NAME_MAX];

    for (file = s_mixList.First(); file != nullptr && file->Is_Valid();
         file = reinterpret_cast<MixFileClass *>(file->Next())) {
        _splitpath(file->m_fileName, nullptr, nullptr, fname, ext);
        _makepath(dest, nullptr, nullptr, fname, ext);
        if (strcasecmp(dest, filename) == 0) {
            return file;
        }
    }
#else // !PLATFORM_WINDOWS

    char path[PATH_MAX];
    char *dest = nullptr;

    for (file = s_mixList.First(); file != nullptr && file->Is_Valid();
         file = reinterpret_cast<MixFileClass *>(file->Next())) {
        strlcpy(path, file->m_fileName, PATH_MAX);
        dest = basename(path);
        if (dest && strcasecmp(dest, filename) == 0) {
            return file;
        }
    }
#endif // PLATFORM_WINDOWS

    // We where unable to find the find in the loaded list, return NULL.
    return nullptr;
}

/**
 * @brief Cache the named mix if it exists in the list of loaded mix files already.
 */
template<class FC>
BOOL MixFileClass<FC>::Cache(const char *filename, BufferClass *buffer)
{
    DEBUG_ASSERT(filename != nullptr);

    MixFileClass<FC> *file = Finder(filename);

    if (file != nullptr) {
        return file->Cache(buffer);
    }

    return false;
}

/**
 * @brief Cache the mix file into memory.
 */
template<class FC>
bool MixFileClass<FC>::Cache(BufferClass *buffer)
{
    int buff_size;
    SHAEngine::SHADigest computed_checksum;
    SHAEngine::SHADigest file_checksum;

    // Check if the Mix file has already been cached.
    if (m_fileCache != nullptr) {
        return true;
    }

    // Does the passed buffer have enough space to use? If not allocate our own.
    if (buffer == nullptr || ((buff_size = buffer->Get_Size()) != 0 && buff_size < m_fileSize)) {
        uint8_t *cache_buffer = new uint8_t[m_fileSize];
        m_fileCache = cache_buffer;
        m_isAllocated = true;
    } else {
        m_fileCache = buffer->Get_Buffer();
    }

    // Check we now have a valid m_fileCache pointer and we are good to go.
    if (m_fileCache != nullptr) {
        FC mixfile(m_fileName);
        FileStraw filestrw(&mixfile);
        SHAStraw shastrw;
        Straw *straw_to_use;

        // If the file has a checksum, then the last 20 bytes are a SHA1 digest.
        // Set our Straw to be a SHAStraw that will calculate the digest on the
        // file as we load it so we can compare to the stored value.
        if (m_hasChecksum) {
            shastrw.Get_From(&filestrw);
            straw_to_use = &shastrw;
        } else {
            straw_to_use = &filestrw;
        }

        mixfile.Open(FM_READ);
        mixfile.Bias(0, -1);
        mixfile.Seek(m_fileStart, FS_SEEK_START);

        // Load the file using our chosen Straw, if we don't read enough bytes
        // then something went wrong, so cleanup and abort.
        if (straw_to_use->Get(m_fileCache, m_fileSize) == m_fileSize) {
            // If we have a checksum, read the remaining bytes for comparison
            // If the two digests don't match, something is wrong so cleanup and
            // abort.
            if (m_hasChecksum) {
                filestrw.Get(&file_checksum, sizeof(SHAEngine::SHADigest));
                shastrw.Result(&computed_checksum);

                if (memcmp(&computed_checksum, &file_checksum, sizeof(SHAEngine::SHADigest))) {
                    DEBUG_LOG("Mixfile checksum doesn't match calculated for %s\n", m_fileName);

                    delete m_fileCache;
                    m_fileCache = nullptr;

                    return false;
                }
            }

            return true;

        } else {
            delete m_fileCache;
            m_fileCache = nullptr;
            mixfile.Error(21);

            return false;
        }
    }

    m_isAllocated = false;

    return false;
}

/**
 * @brief Delete the cache for the named mix file.
 */
template<class FC>
void MixFileClass<FC>::Discard_Cache(const char *filename)
{
    MixFileClass<FC> *file;

    DEBUG_ASSERT(filename != nullptr);

    if ((file = Finder(filename)) != nullptr) {
        file->Invalidate_Cache();
    }
}

/**
 * @brief Delete the cache for this mix file.
 */
template<class FC>
void MixFileClass<FC>::Invalidate_Cache()
{
    if (m_fileCache && m_isAllocated) {
        delete[] m_fileCache;
    }

    m_fileCache = nullptr;
    m_isAllocated = false;
}

/**
 * @brief Returns various data about a file located in a loaded mix file.
 *
 * @param filename Name of the file to retrieve information regarding.
 * @param cachedfile Pointer to pointer to set to the file data if file is in a cached mix.
 * @param mixfile Pointer to a MixFileClass pointer to set to the MixFileClass instance containing requested file.
 * @param file_offset Pointer to a 32bit int to set to the offset within first mix file that requested file is found in.
 * @param file_size Pointer to a 32bit int to set to the size of the file within the first mix that requested file is found
 *        in.
 */
template<class FC>
BOOL MixFileClass<FC>::Offset(
    const char *filename, void **cachedfile, MixFileClass<FC> **mixfile, int *file_offset, int *file_size)
{
    if (filename == nullptr) {
        return false;
    }

    char fname[NAME_MAX];
    strlcpy(fname, filename, NAME_MAX);
    FileInfoStruct entry;

    // If the filename is valid, generate a hash based on the filename so we
    entry.m_crc = Calculate_CRC(strupr(fname), strlen(fname));

    // Iterate over the list of loaded MixFileClass' and search them until
    // we find one that has the file or we reach the end of the list.
    for (MixFileClass<FC> *tmp_mix = s_mixList.First(); tmp_mix != nullptr && tmp_mix->Is_Valid();
         tmp_mix = tmp_mix->Next()) {
        // compare the current entry with the entry we need.
        FileInfoStruct *entry_pointer =
            (FileInfoStruct *)bsearch(&entry, tmp_mix->m_fileIndex, tmp_mix->m_fileCount, sizeof(entry), FileInfo_Compare);
        if (entry_pointer != nullptr) {
            // If we found the mix containing the file, now we fill in any pointers
            // we were passed as arguments.
            //
            // If we have specified a destination for found MixFile instance, set it.
            if (mixfile != nullptr) {
                *mixfile = tmp_mix;
            }

            // If we have specified a destination for the size, set it.
            if (file_size != nullptr) {
                *file_size = entry_pointer->m_size;
            }

            // Make sure the output pointer is null? I guess it just makes sure if
            // it isn't set below that it will be NULL.
            if (cachedfile != nullptr) {
                *cachedfile = nullptr;
            }

            // If we have specified a destination for the offset, set it.
            if (file_offset != nullptr) {
                *file_offset = entry_pointer->m_offset;
            }

            // If the Mix is cached and we were provided a pointer for the files
            // position in memory, we set it here.
            if (cachedfile != nullptr && tmp_mix->m_fileCache) {
                *cachedfile = entry_pointer->m_offset + tmp_mix->m_fileCache;
            }

            // If the Mix isn't cached and we have a pointer for the offset in the
            // file, add the offset to the start of the Mix file in the actual file
            // stored on disk (handles nested Mix files).
            if (file_offset != nullptr && tmp_mix->m_fileCache == nullptr) {
                *file_offset += tmp_mix->m_fileStart;
            }

            return true;
        }
    }

    return false;
}

#endif // MIXFILE_H
