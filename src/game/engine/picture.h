/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Implements a cps format picture loader.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#include "always.h"
#include "buffer.h"
#include "fileclass.h"

enum PicturePlaneType
{
    BM_AMIGA = 0,
    BM_MCGA = 1,
};

enum IFFForm_Type
{
    FORM_ILBM = 0,
    FORM_PBM = 1
};

// needs to expand to int16(word) not int32(dword)
#ifdef COMPILER_WATCOM
enum CompressionType
#else
enum CompressionType : int16_t
#endif
{
    // more information on these at http://eob.wikispaces.com/eob.cps
    COMP_NOCOMPRESS = 0,
    COMP_LZW12 = 1, // crunch1
    COMP_LZW14 = 2, // format2
    COMP_HORIZONTAL = 3, // rle?      //format3
    COMP_LCW = 4, // format80
    COMP_FORCESIZE = 0x7FFF,
};

#pragma pack(push, 1)
struct PictureHeader
{
    uint16_t Filesize;
    CompressionType CompMode; // use CompressionType cast on this
    uint32_t UncompressedSize;
    uint16_t PaletteSize;
};
#pragma pack(pop)

//------------------------------------------------------------------------------

int Load_Picture(
    FileClass &file, BufferClass &buff1, BufferClass &buff2, void *palette = nullptr, PicturePlaneType plane = BM_MCGA);
int __cdecl Load_Picture(
    const char *filename, BufferClass &buff1, BufferClass &buff2, void *palette = nullptr, PicturePlaneType plane = BM_MCGA);

int32_t Load_Uncompress(FileClass &file, BufferClass &buff1, BufferClass &buff2, void *palette);
uint32_t Uncompress_Data(const void *a1, void *a2);
