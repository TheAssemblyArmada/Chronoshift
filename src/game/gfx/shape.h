/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Low level functions for loading and rendering C&C sprite files.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef SHAPE_H
#define SHAPE_H

#include "always.h"
#include "gbuffer.h"
#include "trect.h"

#define SHAPE_BUFFER_SIZE 65000

enum ShapeFlags
{
    SHAPE_NORMAL = 0,
    SHAPE_HORZ_REV = 1,
    SHAPE_VERT_REV = 2,
    SHAPE_SCALING = 4,
    SHAPE_VIEWPORT_REL = 0x10,
    SHAPE_WIN_REL = 0x10,
    SHAPE_CENTER = 0x20,
    SHAPE_FADING = 0x100,
    SHAPE_PREDATOR = 0x200,
    SHAPE_COMPACT = 0x1000,
    SHAPE_PRIORITY = 0x1000,
    SHAPE_GHOST = 0x1000,
    SHAPE_SHADOW = 0x1000,
    SHAPE_PARTIAL = 0x1000,
    SHAPE_COLOR = 0x00401000,
};

DEFINE_ENUMERATION_BITWISE_OPERATORS(ShapeFlags);

#pragma pack(push,1)
struct ShapeHeaderStruct
{
    uint16_t frame_count;
    uint16_t x_pos; // In memory 0xDDD5 here indicates we have a keyframe slot allocated
    uint16_t y_pos; // Used in memory to hold keyframe slot
    uint16_t width;
    uint16_t height;
    uint16_t largest_frame_size;
    uint16_t flags;
};

struct ShapeBufferHeader
{
    uint32_t draw_flags;
    uint32_t frame_offset;
    BOOL is_theater_shape;
};
#pragma pack(pop)

#ifndef RAPP_STANDALONE
extern BOOL &UseBigShapeBuffer;
extern BOOL &IsTheaterShape;
extern BOOL &OriginalUseBigShapeBuffer;
extern char *&BigShapeBufferStart;
extern char *&BigShapeBufferPtr;
//extern int &TotalBigShapes;
extern BOOL &ReallocShapeBufferFlag;
extern char *&TheaterShapeBufferStart;
extern char *&TheaterShapeBufferPtr;
extern int &TotalTheaterShapes;
extern uint32_t **KeyFrameSlots;
extern int &TotalSlotsUsed;
//extern int &BuildFrameLength;
extern BOOL &UseOldShapeDraw;
extern uint8_t *&ShapeBuffer;
extern int &ShapeBufferSize;
extern unsigned int &BigShapeBufferLength;
extern unsigned int &TheaterShapeBufferLength;
extern int &TheaterSlotsUsed;
extern int &Length;
#else
extern BOOL UseBigShapeBuffer;
extern BOOL IsTheaterShape;
extern BOOL OriginalUseBigShapeBuffer;
extern char *BigShapeBufferStart;
extern char *BigShapeBufferPtr;
//extern int TotalBigShapes;
extern BOOL ReallocShapeBufferFlag;
extern char *TheaterShapeBufferStart;
extern char *TheaterShapeBufferPtr;
extern int TotalTheaterShapes;
extern uint32_t *KeyFrameSlots[1500];
extern int TotalSlotsUsed;
//extern int BuildFrameLength;
extern BOOL UseOldShapeDraw;
extern uint8_t *ShapeBuffer;
extern int ShapeBufferSize;
extern unsigned int BigShapeBufferLength;
extern unsigned int TheaterShapeBufferLength;
extern int TheaterSlotsUsed;
extern int Length;
#endif

TRect<int> Shape_Dimensions(void *shape, int frame);
void *Get_Shape_Header_Data(void *shape);
int Get_Last_Frame_Length();
void Reset_Theater_Shapes();
void Reallocate_Big_Shape_Buffer();
void Disable_Uncompressed_Shapes();
void Enable_Uncompressed_Shapes();
void Check_Use_Compressed_Shapes();
void *Build_Frame(void *shape, uint16_t frame, void *buffer);
unsigned short Get_Build_Frame_Count(void *shape);
unsigned short Get_Build_Frame_X(void *shape);
unsigned short Get_Build_Frame_Y(void *shape);
unsigned short Get_Build_Frame_Width(void *shape);
unsigned short Get_Build_Frame_Height(void *shape);
BOOL Get_Build_Frame_Palette(void *shape, void *pal);
void Set_Shape_Buffer(void *buffer, int size);
int Set_Shape_Height(void *shape, unsigned short new_height);
void Buffer_Frame_To_Page(int x, int y, int width, int height, void *shape, GraphicViewPortClass &viewport, int flags, ...);

#ifndef RAPP_STANDALONE
#include "hooker.h"

namespace Shape
{

inline void Hook_Me()
{
    Hook_Function(0x005B48EC, Build_Frame);
    Hook_Function(0x005AB354, Buffer_Frame_To_Page);
    Hook_Function(0x004A9AB8, Shape_Dimensions);
}

}
#endif

#endif // SHAPE_H
