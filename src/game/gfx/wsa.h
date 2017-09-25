/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Functions for loading and rendering a WSA file.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef WSA_H
#define WSA_H

#include "always.h"

class GraphicViewPortClass;

// WSA Header
//
// This is the header as it is read from the file and includes the first two
// offsets which are used to load the first frame.
#pragma pack(push, 1)
struct WSAFileHeaderType
{
    uint16_t total_frames;
    uint16_t x_pos;
    uint16_t y_pos;
    uint16_t width;
    uint16_t height;
    uint16_t largest_frame_size;
    uint16_t flags;
    uint32_t first_frame_start;
    uint32_t first_frame_end;
};

// Header version with a larger Delta as used in Poly for images > 320 x 200.
struct WSAHDFileHeaderType
{
    uint16_t total_frames;
    uint16_t x_pos;
    uint16_t y_pos;
    uint16_t width;
    uint16_t height;
    uint32_t largest_frame_size;
    uint16_t flags;
    uint32_t first_frame_start;
    uint32_t first_frame_end;
};

//
// WSA Header
//
// This is the actual header as it occurs in the file and is followed by a
// uint32_t array of offsets to the frame data
struct WSAFileHeader
{
    uint16_t frame_count;
    uint16_t x_pos;
    uint16_t y_pos;
    uint16_t width;
    uint16_t height;
    uint16_t delta_buffer_size;
    uint16_t flags;
};

struct WSAHDFileHeader
{
    uint16_t frame_count;
    uint16_t x_pos;
    uint16_t y_pos;
    uint16_t width;
    uint16_t height;
    uint32_t delta_buffer_size;
    uint16_t flags;
};

// Loaded WSA header
struct SysAnimHeaderType
{
    uint16_t current_frame;
    uint16_t total_frames;
    uint16_t x_pos;
    uint16_t y_pos;
    uint16_t width;
    uint16_t height;
    uint32_t largest_frame_size;
    void *delta_buffer;
    void *file_buffer;
    char file_name[13];
    uint16_t flags;
    uint16_t file_handle;
    uint32_t anim_mem_size;
};

#pragma pack(pop)

enum WSAOpenType
{
    WSA_OPEN_FROM_MEM = 0x0,
    WSA_OPEN_INDIRECT = 0x0,
    WSA_OPEN_FROM_DISK = 0x1,
    WSA_OPEN_DIRECT = 0x2,
    WSA_OPEN_TO_PAGE = 0x2,
    WSA_OPEN_TO_BUFFER = 0x0,
};

// Flags set for the anim in memory when it was opened
enum WSAMode
{
    WSA_NO_ALLOC = 0x01,
    WSA_ALLOC = 0x02,
    WSA_FROMFILE = 0x04,
    WSA_CACHED = 0x08,
    WSA_DECODE_BUFF = 0x10,
    WSA_NOLOOP = 0x20,
    WSA_NO_INIT = 0x40,
};

enum WSAType
{
    WSA_NORMAL = 0x0,
    WSA_GHOST = 0x1000,
    WSA_PRIORITY2 = 0x2000,
    WSA_TRANS = 0x4000,
    WSA_PRIORITY = 0x8000
};

union WSAHeaderType
{
    WSAFileHeaderType sd;
    WSAHDFileHeaderType hd;
};

SysAnimHeaderType *__cdecl Open_Animation(const char *filename, void *buffer, int size, char flags, void *pal);
void __cdecl Close_Animation(SysAnimHeaderType *header);
BOOL __cdecl Animate_Frame(SysAnimHeaderType *header, GraphicViewPortClass &viewport, int frame, int x_pos, int y_pos);
int Apply_Delta(SysAnimHeaderType *header, int curr_frame, void *buffer, int pitch);
unsigned __cdecl Get_Animation_Size(SysAnimHeaderType *header);
unsigned Get_Resident_Frame_Offset(void *anim, int frame);
unsigned Get_File_Frame_Offset(int handle, int frame, int palette_adjust = 0);
int __cdecl Get_Animation_Frame_Count(SysAnimHeaderType *header);
int __cdecl Get_Animation_X(SysAnimHeaderType *header);
int __cdecl Get_Animation_Y(SysAnimHeaderType *header);
int __cdecl Get_Animation_Width(SysAnimHeaderType *header);
int __cdecl Get_Animation_Height(SysAnimHeaderType *header);
int __cdecl Get_Animation_Palette(SysAnimHeaderType *header);

#ifndef RAPP_STANDALONE
#include "hooker.h"

namespace WSAFile {

inline void Hook_Me()
{
    Hook_Function(0x005D03C0, Open_Animation);
    Hook_Function(0x005D0820, Close_Animation);
    Hook_Function(0x005D0860, Animate_Frame);
    Hook_Function(0x005D0CF0, Apply_Delta);
    Hook_Function(0x005D0C00, Get_Animation_Size);
    Hook_Function(0x005D0C30, Get_Resident_Frame_Offset);
    Hook_Function(0x005D0C90, Get_File_Frame_Offset);
    Hook_Function(0x005D0AE0, Get_Animation_Frame_Count);
    Hook_Function(0x005D0B10, Get_Animation_X);
    Hook_Function(0x005D0B40, Get_Animation_Y);
    Hook_Function(0x005D0B70, Get_Animation_Width);
    Hook_Function(0x005D0BA0, Get_Animation_Height);
    Hook_Function(0x005D0BD0, Get_Animation_Palette);
}

}
#endif

#endif // WSA_H
