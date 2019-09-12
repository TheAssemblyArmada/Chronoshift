/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Low level functions for loading and rendering C&C sprite files.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
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
#include "trect.h"

class GraphicViewPortClass;

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
    SHAPE_TRANSPARENT = 0x40,
    SHAPE_FADING = 0x100,
    SHAPE_PREDATOR = 0x200,
    SHAPE_COMPACT = 0x400,
    SHAPE_PRIORITY = 0x800,
    SHAPE_GHOST = 0x1000,
    SHAPE_SHADOW = 0x2000,
    SHAPE_PARTIAL = 0x4000,
    SHAPE_COLOR = 0x8000,
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

#ifdef GAME_DLL
extern BOOL &g_useBigShapeBuffer;
extern BOOL &g_isTheaterShape;
extern BOOL &g_originalUseBigShapeBuffer;
extern char *&g_bigShapeBufferStart;
extern char *&g_bigShapeBufferPtr;
//extern int &TotalBigShapes;
extern BOOL &g_reallocShapeBufferFlag;
extern char *&g_theaterShapeBufferStart;
extern char *&g_theaterShapeBufferPtr;
extern int &g_totalTheaterShapes;
extern uint32_t **g_keyFrameSlots;
extern int &g_totalSlotsUsed;
//extern int &BuildFrameLength;
extern BOOL &g_useOldShapeDraw;
extern uint8_t *&g_shapeBuffer;
extern int &g_shapeBufferSize;
extern unsigned int &g_bigShapeBufferLength;
extern unsigned int &g_theaterShapeBufferLength;
extern int &g_theaterSlotsUsed;
extern int &g_shapeLength;
#else
extern BOOL g_useBigShapeBuffer;
extern BOOL g_isTheaterShape;
extern BOOL g_originalUseBigShapeBuffer;
extern char *g_bigShapeBufferStart;
extern char *g_bigShapeBufferPtr;
//extern int TotalBigShapes;
extern BOOL g_reallocShapeBufferFlag;
extern char *g_theaterShapeBufferStart;
extern char *g_theaterShapeBufferPtr;
extern int g_totalTheaterShapes;
extern uint32_t *g_keyFrameSlots[1500];
extern int g_totalSlotsUsed;
//extern int BuildFrameLength;
extern BOOL g_useOldShapeDraw;
extern uint8_t *g_shapeBuffer;
extern int g_shapeBufferSize;
extern unsigned int g_bigShapeBufferLength;
extern unsigned int g_theaterShapeBufferLength;
extern int g_theaterSlotsUsed;
extern int g_shapeLength;
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

#endif // SHAPE_H
