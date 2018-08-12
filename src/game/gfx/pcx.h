/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief PCX file readers and writers.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef PCX_H
#define PCX_H

#include "always.h"
#include "buffer.h"
#include "fileclass.h"
#include "gbuffer.h"
#include "palette.h"

enum PCX_MANF_ID
{
    PCX_ZSOFT = 10
};

enum PCX_VERSION
{
    PCX_VER25 = 0,
    PCX_VER28_PAL = 2,
    PCX_VER28_NPAL = 3,
    PCX_FOR_WIN = 4,
    PCX_VER30 = 5
};

enum PCX_ENCODING
{
    PCX_ENC_NONE = 0,
    PCX_ENC_RLE = 1
};

enum PCX_BITSPERPIXEL
{
    PCX_BPP_MONO = 1,
    PCX_BPP_16COLOR = 4,
    PCX_BPP_256COLOR = 8,
    PCX_BPP_16_7MILLION = 24
};

enum PCX_PAL_INFO
{
    PCX_PAL_INFO_COLOR = 1,
    PCX_PAL_INFO_GREYSCALE = 2
};

#pragma pack(push, 1)
struct PCX_HEADER
{
    uint8_t Identifier;
    uint8_t Version;
    uint8_t Encoding;
    uint8_t BitsPixelPlane;
    uint16_t XStart;
    uint16_t YStart;
    uint16_t XEnd;
    uint16_t YEnd;
    uint16_t HRes;
    uint16_t VRes;
    uint8_t EgaPalette[48];
    uint8_t ReservedByte;
    uint8_t NumberOfPlanes;
    uint16_t BytesLinePlane;
    uint16_t PaletteInfo;
    uint16_t HScreenSize;
    uint16_t VScreenSize;
    uint8_t ReservedBlock[54];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct PCX_IMAGE
{
    uint32_t Width;
    uint32_t Height;
    uint8_t *Data;
    uint8_t *Pal;
};
#pragma pack(pop)

GraphicBufferClass *Read_PCX_File(const char *filename, PaletteClass *pal = nullptr, void *buffer = nullptr, int size = 0);
int Write_PCX_File(FileClass &file, GraphicBufferClass &gbuff, PaletteClass &pal);

#endif // PCX_H
