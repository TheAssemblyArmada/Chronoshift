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
    uint16_t m_FrameCount;
    uint16_t m_XPos; // In memory 0xDDD5 here indicates we have a keyframe slot allocated
    uint16_t m_YPos; // Used in memory to hold keyframe slot
    uint16_t m_Width;
    uint16_t m_Height;
    uint16_t m_LargestFrameSize;
    uint16_t m_Flags;
};

struct ShapeBufferHeader
{
    uint32_t m_DrawFlags;
    uint32_t m_FrameOffset;
    BOOL m_IsTheaterShape;
};
#pragma pack(pop)

#ifdef GAME_DLL
extern BOOL &g_UseBigShapeBuffer;
extern BOOL &g_IsTheaterShape;
extern BOOL &g_OriginalUseBigShapeBuffer;
extern char *&g_BigShapeBufferStart;
extern char *&g_BigShapeBufferPtr;
//extern int &TotalBigShapes;
extern BOOL &g_ReallocShapeBufferFlag;
extern char *&g_TheaterShapeBufferStart;
extern char *&g_TheaterShapeBufferPtr;
extern int &g_TotalTheaterShapes;
extern uint32_t **g_KeyFrameSlots;
extern int &g_TotalSlotsUsed;
//extern int &BuildFrameLength;
extern BOOL &g_UseOldShapeDraw;
extern uint8_t *&g_ShapeBuffer;
extern int &g_ShapeBufferSize;
extern unsigned int &g_BigShapeBufferLength;
extern unsigned int &g_TheaterShapeBufferLength;
extern int &g_TheaterSlotsUsed;
extern int &g_ShapeLength;
#else
extern BOOL g_UseBigShapeBuffer;
extern BOOL g_IsTheaterShape;
extern BOOL g_OriginalUseBigShapeBuffer;
extern char *g_BigShapeBufferStart;
extern char *g_BigShapeBufferPtr;
//extern int TotalBigShapes;
extern BOOL g_ReallocShapeBufferFlag;
extern char *g_TheaterShapeBufferStart;
extern char *g_TheaterShapeBufferPtr;
extern int g_TotalTheaterShapes;
extern uint32_t *g_KeyFrameSlots[1500];
extern int g_TotalSlotsUsed;
//extern int BuildFrameLength;
extern BOOL g_UseOldShapeDraw;
extern uint8_t *g_ShapeBuffer;
extern int g_ShapeBufferSize;
extern unsigned int g_BigShapeBufferLength;
extern unsigned int g_TheaterShapeBufferLength;
extern int g_TheaterSlotsUsed;
extern int g_ShapeLength;
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
