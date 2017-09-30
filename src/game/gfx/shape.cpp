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
#include "shape.h"
#include "alloc.h"
#include "endiantype.h"
#include "gamedebug.h"
#include "lcw.h"
#include "minmax.h"
#include "xordelta.h"

#define SHP_HAS_PAL 0x0001
#define SHP_LCW_FRAME 0x80
#define SHP_XOR_FAR_FRAME 0x40
#define SHP_XOR_PREV_FRAME 0x20
#define BIGSHP_BUFFER_MIN_FREE 128000
#define BIGSHP_BUFFER_GROW 2000000

#ifndef RAPP_STANDALONE
BOOL &UseBigShapeBuffer = Make_Global<BOOL>(0x006A1784);
BOOL &IsTheaterShape = Make_Global<BOOL>(0x006A1788);
BOOL &OriginalUseBigShapeBuffer = Make_Global<BOOL>(0x006A178C);
char *&BigShapeBufferStart = Make_Global<char *>(0x006A177C);
char *&BigShapeBufferPtr = Make_Global<char *>(0x006A1790);
// int &TotalBigShapes;
BOOL &ReallocShapeBufferFlag = Make_Global<BOOL>(0x006A1798);
char *&TheaterShapeBufferStart = Make_Global<char *>(0x006A1780);
char *&TheaterShapeBufferPtr = Make_Global<char *>(0x006A179C);
int &TotalTheaterShapes = Make_Global<int>(0x006A17A0);
uint32_t **KeyFrameSlots = reinterpret_cast<uint32_t **>(0x006A17A4);
int &TotalSlotsUsed = Make_Global<int>(0x006A2F14);
// int &BuildFrameLength;
BOOL &UseOldShapeDraw = Make_Global<BOOL>(0x00655C08);
uint8_t *&ShapeBuffer = Make_Global<uint8_t *>(0x0060BE58);
int &ShapeBufferSize = Make_Global<int>(0x0060BE5C);
unsigned int &BigShapeBufferLength = Make_Global<unsigned int>(0x00609D68);
unsigned int &TheaterShapeBufferLength = Make_Global<unsigned int>(0x00609D6C);
int &TheaterSlotsUsed = Make_Global<int>(0x00609D70);
int &Length = Make_Global<int>(0x006A2F18);
#else
BOOL UseBigShapeBuffer;
BOOL IsTheaterShape = false;
BOOL OriginalUseBigShapeBuffer;
char *BigShapeBufferStart = nullptr;
char *BigShapeBufferPtr = nullptr;
// int TotalBigShapes = 0;
BOOL ReallocShapeBufferFlag;
char *TheaterShapeBufferStart = nullptr;
char *TheaterShapeBufferPtr = nullptr;
int TotalTheaterShapes = 0;
uint32_t *KeyFrameSlots[1500];
int TotalSlotsUsed = 0;
// int BuildFrameLength = 0;
BOOL UseOldShapeDraw = false;
uint8_t *ShapeBuffer = nullptr;
int ShapeBufferSize = 0;
unsigned int BigShapeBufferLength = 8000000;
unsigned int TheaterShapeBufferLength = 4000000;
int TheaterSlotsUsed = 1000;
int Length;
#endif

// The predator effect basically just takes a destination pixel and replaces it
// with the value g_predatorShiftTab[g_predatorFrame] pixels away if g_predatorAccumulated
// is greater than or equal to 256. After every pixel, it is increased by
// g_predatorIncrement and reset to % 256 after reaching 256 or greater.
static const int16_t g_predatorShiftTab[8] = { 1, 3, 2, 5, 4, 3, 2, 1 };
static uint32_t g_predatorFrame;
static uint32_t g_predatorAccumulated;
static uint32_t g_predatorIncrement;

// Buffer Frame to Page function pointer type defs

typedef void (*BF_Function)(int, int, uint8_t *, uint8_t *, int, int, uint8_t *, uint8_t *, uint8_t *, int);
typedef void (*Single_Line_Function)(int, uint8_t *, uint8_t *, uint8_t *, uint8_t *, uint8_t *, int);

// Just copy source to dest as is.
void BF_Copy(int width, int height, uint8_t *dst, uint8_t *src, int dst_pitch, int src_pitch, uint8_t *ghost_lookup,
    uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    while (height--) {
        memcpy(dst, src, width);
        dst = dst + dst_pitch + width;
        src = src + src_pitch + width;
    }
}

// Index 0 transparency
void BF_Trans(int width, int height, uint8_t *dst, uint8_t *src, int dst_pitch, int src_pitch, uint8_t *ghost_lookup,
    uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    while (height--) {
        for (int i = width; i > 0; --i) {
            uint8_t sbyte = *src++;

            if (sbyte) {
                *dst = sbyte;
            }

            ++dst;
        }

        src += src_pitch;
        dst += dst_pitch;
    }
}

// Fading table based shadow and transparency
void BF_Ghost(int width, int height, uint8_t *dst, uint8_t *src, int dst_pitch, int src_pitch, uint8_t *ghost_lookup,
    uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    while (height--) {
        for (int i = width; i > 0; --i) {
            uint8_t sbyte = *src++;
            uint8_t fbyte = ghost_lookup[sbyte];

            if (fbyte != 0xFF) {
                sbyte = ghost_tab[*dst + fbyte * 256];
            }

            *dst++ = sbyte;
        }

        src += src_pitch;
        dst += dst_pitch;
    }
}

// Fading table based shadow and transparency with index 0 ignored
void BF_Ghost_Trans(int width, int height, uint8_t *dst, uint8_t *src, int dst_pitch, int src_pitch, uint8_t *ghost_lookup,
    uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    while (height--) {
        for (int i = width; i > 0; --i) {
            uint8_t sbyte = *src++;

            if (sbyte) {
                uint8_t fbyte = ghost_lookup[sbyte];

                if (fbyte != 0xFF) {
                    sbyte = ghost_tab[*dst + fbyte * 256];
                }

                *dst = sbyte;
            }

            ++dst;
        }

        src += src_pitch;
        dst += dst_pitch;
    }
}

void BF_Fading(int width, int height, uint8_t *dst, uint8_t *src, int dst_pitch, int src_pitch, uint8_t *ghost_lookup,
    uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    while (height--) {
        for (int i = width; i > 0; --i) {
            uint8_t sbyte = *src;

            for (int i = 0; i < count; ++i) {
                sbyte = fade_tab[sbyte];
            }

            *dst++ = sbyte;
        }
    }
}

void BF_Fading_Trans(int width, int height, uint8_t *dst, uint8_t *src, int dst_pitch, int src_pitch, uint8_t *ghost_lookup,
    uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    while (height--) {
        for (int i = width; i > 0; --i) {
            uint8_t sbyte = *src++;

            if (sbyte) {
                for (int i = 0; i < count; ++i) {
                    sbyte = fade_tab[sbyte];
                }

                *dst = sbyte;
            }

            ++dst;
        }

        src += src_pitch;
        dst += dst_pitch;
    }
}

void BF_Ghost_Fading(int width, int height, uint8_t *dst, uint8_t *src, int dst_pitch, int src_pitch, uint8_t *ghost_lookup,
    uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    while (height--) {
        for (int i = width; i > 0; --i) {
            uint8_t sbyte = *src++;
            uint8_t fbyte = ghost_lookup[sbyte];

            if (fbyte != 0xFF) {
                sbyte = ghost_tab[*dst + fbyte * 256];
            }

            for (int i = 0; i < count; ++i) {
                sbyte = fade_tab[sbyte];
            }

            *dst++ = sbyte;
        }

        src += src_pitch;
        dst += dst_pitch;
    }
}

void BF_Ghost_Fading_Trans(int width, int height, uint8_t *dst, uint8_t *src, int dst_pitch, int src_pitch,
    uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    while (height--) {
        for (int i = width; i > 0; --i) {
            uint8_t sbyte = *src++;

            if (sbyte) {
                uint8_t fbyte = ghost_lookup[sbyte];

                if (fbyte != 0xFF) {
                    sbyte = ghost_tab[*dst + fbyte * 256];
                }

                for (int i = 0; i < count; ++i) {
                    sbyte = fade_tab[sbyte];
                }

                *dst = sbyte;
            }

            ++dst;
        }

        src += src_pitch;
        dst += dst_pitch;
    }
}

void BF_Predator(int width, int height, uint8_t *dst, uint8_t *src, int dst_pitch, int src_pitch, uint8_t *ghost_lookup,
    uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    while (height--) {
        for (int i = width; i > 0; --i) {
            g_predatorAccumulated += g_predatorIncrement;

            // if ( g_predatorAccumulated & 0xFF00 ) {
            //	g_predatorAccumulated &= 0xFFFF00FF;
            if (g_predatorAccumulated >= 256) {
                g_predatorAccumulated %= 256;
                *dst = dst[g_predatorShiftTab[g_predatorFrame]];
                g_predatorFrame = (g_predatorFrame + 1) % 8;
            }

            ++dst;
        }

        src += src_pitch + width;
        dst += dst_pitch;
    }
}

void BF_Predator_Trans(int width, int height, uint8_t *dst, uint8_t *src, int dst_pitch, int src_pitch,
    uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    while (height--) {
        for (int i = width; i > 0; --i) {
            uint8_t sbyte = *src++;
            if (sbyte) {
                g_predatorAccumulated += g_predatorIncrement;

                if (g_predatorAccumulated >= 256) {
                    g_predatorAccumulated %= 256;
                    sbyte = dst[g_predatorShiftTab[g_predatorFrame]];
                    g_predatorFrame = (g_predatorFrame + 1) % 8;
                }

                *dst = sbyte;
            }

            ++dst;
        }

        src += src_pitch;
        dst += dst_pitch;
    }
}

void BF_Predator_Ghost(int width, int height, uint8_t *dst, uint8_t *src, int dst_pitch, int src_pitch,
    uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    while (height--) {
        for (int i = width; i > 0; --i) {
            uint8_t sbyte = *src++;
            g_predatorAccumulated += g_predatorIncrement;

            if (g_predatorAccumulated >= 256) {
                g_predatorAccumulated %= 256;
                sbyte = dst[g_predatorShiftTab[g_predatorFrame]];
                g_predatorFrame = (g_predatorFrame + 1) % 8;
            }

            uint8_t fbyte = ghost_lookup[sbyte];

            if (fbyte != 0xFF) {
                sbyte = ghost_tab[*dst + fbyte * 256];
            }

            *dst++ = sbyte;
        }

        src += src_pitch;
        dst += dst_pitch;
    }
}

void BF_Predator_Ghost_Trans(int width, int height, uint8_t *dst, uint8_t *src, int dst_pitch, int src_pitch,
    uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    while (height--) {
        for (int i = width; i > 0; --i) {
            uint8_t sbyte = *src++;
            if (sbyte) {
                g_predatorAccumulated += g_predatorIncrement;

                if (g_predatorAccumulated >= 256) {
                    g_predatorAccumulated %= 256;
                    sbyte = dst[g_predatorShiftTab[g_predatorFrame]];
                    g_predatorFrame = (g_predatorFrame + 1) % 8;
                }

                uint8_t fbyte = ghost_lookup[sbyte];

                if (fbyte != 0xFF) {
                    sbyte = ghost_tab[*dst + fbyte * 256];
                }

                *dst = sbyte;
            }

            ++dst;
        }

        src += src_pitch;
        dst += dst_pitch;
    }
}

void BF_Predator_Fading(int width, int height, uint8_t *dst, uint8_t *src, int dst_pitch, int src_pitch,
    uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    while (height--) {
        for (int i = width; i > 0; --i) {
            uint8_t sbyte = *src++;
            g_predatorAccumulated += g_predatorIncrement;

            if (g_predatorAccumulated >= 256) {
                g_predatorAccumulated %= 256;
                sbyte = dst[g_predatorShiftTab[g_predatorFrame]];
                g_predatorFrame = (g_predatorFrame + 1) % 8;
            }

            for (int i = 0; i < count; ++i) {
                sbyte = fade_tab[sbyte];
            }

            *dst++ = sbyte;
        }
    }
}

void BF_Predator_Fading_Trans(int width, int height, uint8_t *dst, uint8_t *src, int dst_pitch, int src_pitch,
    uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    while (height--) {
        for (int i = width; i > 0; --i) {
            uint8_t sbyte = *src++;
            if (sbyte) {
                g_predatorAccumulated += g_predatorIncrement;

                if (g_predatorAccumulated >= 256) {
                    g_predatorAccumulated %= 256;
                    sbyte = dst[g_predatorShiftTab[g_predatorFrame]];
                    g_predatorFrame = (g_predatorFrame + 1) % 8;
                }

                for (int i = 0; i < count; ++i) {
                    sbyte = fade_tab[sbyte];
                }

                *dst = sbyte;
            }

            ++dst;
        }

        src += src_pitch;
        dst += dst_pitch;
    }
}

void BF_Predator_Ghost_Fading(int width, int height, uint8_t *dst, uint8_t *src, int dst_pitch, int src_pitch,
    uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    while (height--) {
        for (int i = width; i > 0; --i) {
            uint8_t sbyte = *src++;

            g_predatorAccumulated += g_predatorIncrement;

            if (g_predatorAccumulated >= 256) {
                g_predatorAccumulated %= 256;
                sbyte = dst[g_predatorShiftTab[g_predatorFrame]];
                g_predatorFrame = (g_predatorFrame + 1) % 8;
            }

            uint8_t fbyte = ghost_lookup[sbyte];

            if (fbyte != 0xFF) {
                sbyte = ghost_tab[*dst + fbyte * 256];
            }

            for (int i = 0; i < count; ++i) {
                sbyte = fade_tab[sbyte];
            }

            *dst++ = sbyte;
        }

        src += src_pitch;
        dst += dst_pitch;
    }
}

void BF_Predator_Ghost_Fading_Trans(int width, int height, uint8_t *dst, uint8_t *src, int dst_pitch, int src_pitch,
    uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    while (height--) {
        for (int i = width; i > 0; --i) {
            uint8_t sbyte = *src++;

            if (sbyte) {
                g_predatorAccumulated += g_predatorIncrement;

                if (g_predatorAccumulated >= 256) {
                    g_predatorAccumulated %= 256;
                    sbyte = dst[g_predatorShiftTab[g_predatorFrame]];
                    g_predatorFrame = (g_predatorFrame + 1) % 8;
                }

                uint8_t fbyte = ghost_lookup[sbyte];

                if (fbyte != 0xFF) {
                    sbyte = ghost_tab[*dst + fbyte * 256];
                }

                for (int i = 0; i < count; ++i) {
                    sbyte = fade_tab[sbyte];
                }

                *dst = sbyte;
            }

            ++dst;
        }

        src += src_pitch;
        dst += dst_pitch;
    }
}

// Jump table for BF_* functions
static const BF_Function OldShapeJumpTable[16] = { BF_Copy,
    BF_Trans,
    BF_Ghost,
    BF_Ghost_Trans,
    BF_Fading,
    BF_Fading_Trans,
    BF_Ghost_Fading,
    BF_Ghost_Fading_Trans,
    BF_Predator,
    BF_Predator_Trans,
    BF_Predator_Ghost,
    BF_Predator_Ghost_Trans,
    BF_Predator_Fading,
    BF_Predator_Fading_Trans,
    BF_Predator_Ghost_Fading,
    BF_Predator_Ghost_Fading_Trans };

// Single line versions
void Single_Line_Skip(
    int width, uint8_t *dst, uint8_t *src, uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
}

// This was Short_Single_Line_Copy, renamed for consistency
void Single_Line_Copy(
    int width, uint8_t *dst, uint8_t *src, uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    memcpy(dst, src, width);
}

void Single_Line_Trans(
    int width, uint8_t *dst, uint8_t *src, uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    for (int i = width; i > 0; --i) {
        uint8_t sbyte = *src++;
        if (sbyte) {
            *dst = sbyte;
        }

        ++dst;
    }
}

void Single_Line_Ghost(
    int width, uint8_t *dst, uint8_t *src, uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    for (int i = width; i > 0; --i) {
        uint8_t sbyte = *src++;
        uint8_t fbyte = ghost_lookup[sbyte];

        if (fbyte != 0xFF) {
            sbyte = ghost_tab[*dst + fbyte * 256];
        }

        *dst++ = sbyte;
    }
}

void Single_Line_Ghost_Trans(
    int width, uint8_t *dst, uint8_t *src, uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    for (int i = width; i > 0; --i) {
        uint8_t sbyte = *src++;
        if (sbyte) {
            uint8_t fbyte = ghost_lookup[sbyte];

            if (fbyte != 0xFF) {
                sbyte = ghost_tab[*dst + fbyte * 256];
            }

            *dst = sbyte;
        }

        ++dst;
    }
}

void Single_Line_Fading(
    int width, uint8_t *dst, uint8_t *src, uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    for (int i = width; i > 0; --i) {
        uint8_t sbyte = *src++;
        for (int i = 0; i < count; ++i) {
            sbyte = fade_tab[sbyte];
        }

        *dst++ = sbyte;
    }
}

void Single_Line_Fading_Trans(
    int width, uint8_t *dst, uint8_t *src, uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    for (int i = width; i > 0; --i) {
        uint8_t sbyte = *src++;

        if (sbyte) {
            for (int i = 0; i < count; ++i) {
                sbyte = fade_tab[sbyte];
            }

            *dst = sbyte;
        }

        ++dst;
    }
}

void Single_Line_Single_Fade(
    int width, uint8_t *dst, uint8_t *src, uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    for (int i = width; i > 0; --i) {
        *dst++ = fade_tab[*src++];
    }
}

void Single_Line_Single_Fade_Trans(
    int width, uint8_t *dst, uint8_t *src, uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    for (int i = width; i > 0; --i) {
        uint8_t sbyte = *src++;

        if (sbyte) {
            *dst = fade_tab[sbyte];
        }

        ++dst;
    }
}

void Single_Line_Ghost_Fading(
    int width, uint8_t *dst, uint8_t *src, uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    for (int i = width; i > 0; --i) {
        uint8_t sbyte = *src++;
        uint8_t fbyte = ghost_lookup[sbyte];

        if (fbyte != 0xFF) {
            sbyte = ghost_tab[*dst + fbyte * 256];
        }

        for (int i = 0; i < count; ++i) {
            sbyte = fade_tab[sbyte];
        }

        *dst++ = sbyte;
    }
}

void Single_Line_Ghost_Fading_Trans(
    int width, uint8_t *dst, uint8_t *src, uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    for (int i = width; i > 0; --i) {
        uint8_t sbyte = *src++;

        if (sbyte) {
            uint8_t fbyte = ghost_lookup[sbyte];

            if (fbyte != 0xFF) {
                sbyte = ghost_tab[*dst + fbyte * 256];
            }

            for (int i = 0; i < count; ++i) {
                sbyte = fade_tab[sbyte];
            }

            *dst = sbyte;
        }

        ++dst;
    }
}

void Single_Line_Predator(
    int width, uint8_t *dst, uint8_t *src, uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    for (int i = width; i > 0; --i) {
        g_predatorAccumulated += g_predatorIncrement;

        if (g_predatorAccumulated >= 256) {
            g_predatorAccumulated %= 256;
            *dst = dst[g_predatorShiftTab[g_predatorFrame]];
            g_predatorFrame = (g_predatorFrame + 1) % 8;
        }

        ++dst;
    }
}

void Single_Line_Predator_Trans(
    int width, uint8_t *dst, uint8_t *src, uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    for (int i = width; i > 0; --i) {
        uint8_t sbyte = *src++;
        if (sbyte) {
            g_predatorAccumulated += g_predatorIncrement;

            if (g_predatorAccumulated >= 256) {
                g_predatorAccumulated %= 256;
                sbyte = dst[g_predatorShiftTab[g_predatorFrame]];
                g_predatorFrame = (g_predatorFrame + 1) % 8;
            }

            *dst = sbyte;
        }

        ++dst;
    }
}

void Single_Line_Predator_Ghost(
    int width, uint8_t *dst, uint8_t *src, uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    for (int i = width; i > 0; --i) {
        uint8_t sbyte = *src++;
        g_predatorAccumulated += g_predatorIncrement;

        if (g_predatorAccumulated >= 256) {
            g_predatorAccumulated %= 256;
            sbyte = dst[g_predatorShiftTab[g_predatorFrame]];
            g_predatorFrame = (g_predatorFrame + 1) % 8;
        }

        uint8_t fbyte = ghost_lookup[sbyte];

        if (fbyte != 0xFF) {
            sbyte = ghost_tab[*dst + fbyte * 256];
        }

        *dst++ = sbyte;
    }
}

void Single_Line_Predator_Ghost_Trans(
    int width, uint8_t *dst, uint8_t *src, uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    for (int i = width; i > 0; --i) {
        uint8_t sbyte = *src++;
        if (sbyte) {
            g_predatorAccumulated += g_predatorIncrement;

            if (g_predatorAccumulated >= 256) {
                g_predatorAccumulated %= 256;
                sbyte = dst[g_predatorShiftTab[g_predatorFrame]];
                g_predatorFrame = (g_predatorFrame + 1) % 8;
            }

            uint8_t fbyte = ghost_lookup[sbyte];

            if (fbyte != 0xFF) {
                sbyte = ghost_tab[*dst + fbyte * 256];
            }

            *dst = sbyte;
        }

        ++dst;
    }
}

void Single_Line_Predator_Fading(
    int width, uint8_t *dst, uint8_t *src, uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    for (int i = width; i > 0; --i) {
        uint8_t sbyte = *src++;
        g_predatorAccumulated += g_predatorIncrement;

        if (g_predatorAccumulated >= 256) {
            g_predatorAccumulated %= 256;
            sbyte = dst[g_predatorShiftTab[g_predatorFrame]];
            g_predatorFrame = (g_predatorFrame + 1) % 8;
        }

        for (int i = 0; i < count; ++i) {
            sbyte = fade_tab[sbyte];
        }

        *dst++ = sbyte;
    }
}

void Single_Line_Predator_Fading_Trans(
    int width, uint8_t *dst, uint8_t *src, uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    for (int i = width; i > 0; --i) {
        uint8_t sbyte = *src++;
        if (sbyte) {
            g_predatorAccumulated += g_predatorIncrement;

            if (g_predatorAccumulated >= 256) {
                g_predatorAccumulated %= 256;
                sbyte = dst[g_predatorShiftTab[g_predatorFrame]];
                g_predatorFrame = (g_predatorFrame + 1) % 8;
            }

            for (int i = 0; i < count; ++i) {
                sbyte = fade_tab[sbyte];
            }

            *dst = sbyte;
        }

        ++dst;
    }
}

void Single_Line_Predator_Ghost_Fading(
    int width, uint8_t *dst, uint8_t *src, uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    for (int i = width; i > 0; --i) {
        uint8_t sbyte = *src++;

        g_predatorAccumulated += g_predatorIncrement;

        if (g_predatorAccumulated >= 256) {
            g_predatorAccumulated %= 256;
            sbyte = dst[g_predatorShiftTab[g_predatorFrame]];
            g_predatorFrame = (g_predatorFrame + 1) % 8;
        }

        uint8_t fbyte = ghost_lookup[sbyte];

        if (fbyte != 0xFF) {
            sbyte = ghost_tab[*dst + fbyte * 256];
        }

        for (int i = 0; i < count; ++i) {
            sbyte = fade_tab[sbyte];
        }

        *dst++ = sbyte;
    }
}

void Single_Line_Predator_Ghost_Fading_Trans(
    int width, uint8_t *dst, uint8_t *src, uint8_t *ghost_lookup, uint8_t *ghost_tab, uint8_t *fade_tab, int count)
{
    for (int i = width; i > 0; --i) {
        uint8_t sbyte = *src++;

        if (sbyte) {
            g_predatorAccumulated += g_predatorIncrement;

            if (g_predatorAccumulated >= 256) {
                g_predatorAccumulated %= 256;
                sbyte = dst[g_predatorShiftTab[g_predatorFrame]];
                g_predatorFrame = (g_predatorFrame + 1) % 8;
            }

            uint8_t fbyte = ghost_lookup[sbyte];

            if (fbyte != 0xFF) {
                sbyte = ghost_tab[*dst + fbyte * 256];
            }

            for (int i = 0; i < count; ++i) {
                sbyte = fade_tab[sbyte];
            }

            *dst = sbyte;
        }

        ++dst;
    }
}

// Jump table for Single_Line_* functions
static Single_Line_Function NewShapeJumpTable[32] = { Single_Line_Copy,
    Single_Line_Trans,
    Single_Line_Ghost,
    Single_Line_Ghost_Trans,
    Single_Line_Fading,
    Single_Line_Fading_Trans,
    Single_Line_Ghost_Fading,
    Single_Line_Ghost_Fading_Trans,
    Single_Line_Predator,
    Single_Line_Predator_Trans,
    Single_Line_Predator_Ghost,
    Single_Line_Predator_Ghost_Trans,
    Single_Line_Predator_Fading,
    Single_Line_Predator_Fading_Trans,
    Single_Line_Predator_Ghost_Fading,
    Single_Line_Predator_Ghost_Fading_Trans,
    Single_Line_Skip,
    Single_Line_Skip,
    Single_Line_Skip,
    Single_Line_Skip,
    Single_Line_Skip,
    Single_Line_Skip,
    Single_Line_Skip,
    Single_Line_Skip,
    Single_Line_Skip,
    Single_Line_Skip,
    Single_Line_Skip,
    Single_Line_Skip,
    Single_Line_Skip,
    Single_Line_Skip,
    Single_Line_Skip,
    Single_Line_Skip };

TRect<int> Shape_Dimensions(void *shape, int frame)
{
    TRect<int> rect;

    if (shape != nullptr && frame >= 0 && frame <= Get_Build_Frame_Count(shape)) {
        void *bframe = Build_Frame(shape, frame, ShapeBuffer);

        if (bframe != nullptr) {
            void *data = Get_Shape_Header_Data(bframe);
            int w = Get_Build_Frame_Width(shape);
            int h = Get_Build_Frame_Height(shape);

            // top, left, bottom, right
            int t = 0;
            int l = 0;
            int b = h - 1;
            int r = w - 1;

            for (int i = 0; i <= b; ++i) {
                if (r >= 0) {
                    uint8_t *sptr = static_cast<uint8_t *>(data) + i * h;
                    int tmp_l = 0;

                    while (*sptr++ == 0) {
                        ++tmp_l;

                        if (tmp_l > r) {
                            break;
                        }
                    }

                    if (tmp_l <= r) {
                        t = i;
                        l = tmp_l;
                        i = b + 1;
                    }
                }
            }

            for (int i = b; i >= t; --i) {
                if (r >= 0) {
                    uint8_t *sptr = static_cast<uint8_t *>(data) + r + i * w;
                    int tmp_r = r;

                    while (*sptr-- == 0) {
                        --tmp_r;

                        if (tmp_r < 0) {
                            break;
                        }
                    }

                    if (tmp_r >= 0) {
                        rect.Bottom = i - t + 1;
                        r = tmp_r;
                        i = t - 1;
                    }
                }
            }

            for (int i = 0; i < l; ++i) {
                if (rect.Bottom + t > t) {
                    uint8_t *sptr = static_cast<uint8_t *>(data) + w * t + i;
                    int tmp_t = t;

                    while (*sptr == 0) {
                        sptr += w;
                        ++tmp_t;

                        if (tmp_t >= rect.Bottom + t) {
                            break;
                        }
                    }

                    if (tmp_t < rect.Bottom + t) {
                        l = i;
                    }
                }
            }

            for (int i = w - 1; i > r; --i) {
                if (rect.Bottom + t > t) {
                    uint8_t *sptr = static_cast<uint8_t *>(data) + w * t + i;
                    int tmp_t = t;

                    while (*sptr == 0) {
                        sptr += w;
                        ++tmp_t;

                        if (tmp_t >= rect.Bottom + t) {
                            break;
                        }
                    }

                    if (tmp_t < rect.Bottom + t) {
                        rect.Right = i - l + 1;
                        i = r - 1;
                    }
                }
            }

            rect.Top = t - h / 2;
            rect.Left = l - w / 2;
        }
    }

    return rect;
}

// This one appears to deal with the buffered shape data
void *Get_Shape_Header_Data(void *shape)
{
    ShapeBufferHeader *header = static_cast<ShapeBufferHeader *>(shape);
    if (UseBigShapeBuffer) {
        if (header->is_theater_shape) {
            return header->frame_offset + TheaterShapeBufferStart;
        } else {
            return header->frame_offset + BigShapeBufferStart;
        }
    }

    return shape;
}

int Get_Last_Frame_Length()
{
    return Length;
}

void Reset_Theater_Shapes()
{
    // I think this loops through and deletes any slot that is > 1000
    if (TheaterSlotsUsed > 1000) {
        for (int i = 1000; i < TheaterSlotsUsed; ++i) {
            delete[] KeyFrameSlots[i];
        }
    }

    TheaterShapeBufferPtr = TheaterShapeBufferStart;
    TotalTheaterShapes = 0;
    TheaterSlotsUsed = 1000;
}

void Reallocate_Big_Shape_Buffer()
{
    if (ReallocShapeBufferFlag) {
        g_memoryError = nullptr;
        BigShapeBufferLength += BIGSHP_BUFFER_GROW;
        BigShapeBufferPtr -= (intptr_t)BigShapeBufferStart;
        BigShapeBufferStart = (char *)Resize_Alloc(BigShapeBufferStart, BigShapeBufferLength);
        DEBUG_LOG("Reallocating Big Shape Buffer, size is now %d.\n", BigShapeBufferLength);
        // TODO
        // g_memoryError = Memory_Error_Handler;
        if (BigShapeBufferStart) {
            ReallocShapeBufferFlag = false;
            BigShapeBufferPtr += (intptr_t)BigShapeBufferStart;
        } else {
            UseBigShapeBuffer = false;
        }
    }
}

void Disable_Uncompressed_Shapes()
{
    UseBigShapeBuffer = false;
}

void Enable_Uncompressed_Shapes()
{
    UseBigShapeBuffer = OriginalUseBigShapeBuffer;
}

void Check_Use_Compressed_Shapes()
{
#if defined PLATFORM_WINDOWS
    // Use EX to account for > 4Gb memory, won't work if compiled with cygwin
    // Supported on XP and later
    MEMORYSTATUSEX Buffer;

    Buffer.dwLength = sizeof(Buffer);
    GlobalMemoryStatusEx(&Buffer);
    UseBigShapeBuffer = Buffer.ullTotalPhys > 0x1000000;
    OriginalUseBigShapeBuffer = Buffer.ullTotalPhys > 0x1000000;
    DEBUG_LOG("Using Big Shape Buffer and Original Buffer is %s.\n", Buffer.ullTotalPhys > 0x1000000 ? "true" : "false");
#elif defined PLATFORM_OSX
    size_t totalmem;
    size_t len = sizeof(totalmem);

    if (sysctlbyname("hw.memsize", &totalmem, &len, nullptr, 0) != -1) {
        UseBigShapeBuffer = totalmem > 0x1000000;
        OriginalUseBigShapeBuffer = totalmem > 0x1000000;
    } else {
        UseBigShapeBuffer = false;
        OriginalUseBigShapeBuffer = false;
    }
#else // Posix version?
    // This should be sufficient on both Linux and OS X
    size_t totalmem = (size_t)sysconf(_SC_PHYS_PAGES) * (size_t)sysconf(_SC_PAGESIZE);

    UseBigShapeBuffer = totalmem > 0x1000000;
    OriginalUseBigShapeBuffer = totalmem > 0x1000000;
#endif
}

void *Build_Frame(void *shape, uint16_t frame, void *buffer)
{
    uint8_t *shape_data = static_cast<uint8_t *>(shape);
    // frame = frame;
    Length = 0;

    if (shape == nullptr || buffer == nullptr) {
        return nullptr;
    }

    ShapeHeaderStruct *header = static_cast<ShapeHeaderStruct *>(shape);
    if (frame >= le16toh(header->frame_count)) {
        DEBUG_LOG(
            "Requested frame %d is greater than total frames %d in this shape file.\n", frame, le16toh(header->frame_count));

        return nullptr;
    }

    // If we are using a cache
    if (UseBigShapeBuffer) {
        if (!BigShapeBufferStart) {
            DEBUG_LOG("Allocating buffers for UseBigShapeBuffer.\n");
            BigShapeBufferStart = static_cast<char *>(Alloc(BigShapeBufferLength, MEM_NORMAL));
            BigShapeBufferPtr = BigShapeBufferStart;
            TheaterShapeBufferStart = static_cast<char *>(Alloc(TheaterShapeBufferLength, MEM_NORMAL));
            TheaterShapeBufferPtr = TheaterShapeBufferStart;
        }

        if (BigShapeBufferLength + (uintptr_t)BigShapeBufferStart - (uintptr_t)BigShapeBufferPtr < BIGSHP_BUFFER_MIN_FREE) {
            ReallocShapeBufferFlag = true;
        }

        // Do we have a keyframe slot allocated already?
        if (header->x_pos != 0xDDD5) {
            header->x_pos = 0xDDD5;
            if (IsTheaterShape) {
                header->y_pos = TheaterSlotsUsed++;
            } else {
                header->y_pos = TotalSlotsUsed++;
            }

            KeyFrameSlots[header->y_pos] = new uint32_t[header->frame_count];
            memset(KeyFrameSlots[header->y_pos], 0, sizeof(uint32_t) * header->frame_count);
        }

        // Do we have anything in our keyframe slot yet? If so, return it.
        uint32_t shp_buff_offset = KeyFrameSlots[header->y_pos][frame];
        if (shp_buff_offset != 0) {
            DEBUG_LOG("Using Cached frame.\n");

            if (IsTheaterShape) {
                return shp_buff_offset + TheaterShapeBufferStart;
            } else {
                return shp_buff_offset + BigShapeBufferStart;
            }
        }
    }

    // If we don't have a cache or failed to find a cached image for this frame, we need to decode the frame we want.
    uint32_t offset_buff[7];
    int frame_size = le16toh(header->height) * le16toh(header->width);
    memcpy(offset_buff, &shape_data[8 * frame + sizeof(ShapeHeaderStruct)], 12);
    uint8_t frame_type = (le32toh(offset_buff[0]) & 0xFF000000) >> 24;

    if (frame_type & SHP_LCW_FRAME) {
        DEBUG_LOG("Decoding key frame.\n");
        uint8_t *frame_data = &shape_data[le32toh(offset_buff[0]) & 0xFFFFFF];

        // Amazingly it seems that shp files actually do support having a pal, just none do.
        if (header->flags & 1) {
            frame_data = &shape_data[(le32toh(offset_buff[0]) & 0xFFFFFF) + 768];
        }

        frame_size = LCW_Uncomp(frame_data, buffer, frame_size);
    } else {
        DEBUG_LOG("Decoding XOR frame.\n");
        int ref_frame = 0;
        // If we have an Xor chain, load first delta address into buffer
        if (frame_type & SHP_XOR_PREV_FRAME) {
            ref_frame = le32toh(offset_buff[1]) & 0xFFFF;
            memcpy(offset_buff, &shape_data[8 * ref_frame + sizeof(ShapeHeaderStruct)], 28);
        }

        // Get the base LCW data and the offset from it to the Xor data
        int base_frame_offset = offset_buff[1] & 0xFFFFFF;
        int xor_data_offset = (le32toh(offset_buff[0]) & 0xFFFFFF) - (le32toh(offset_buff[1]) & 0xFFFFFF);
        uint8_t *lcw_data = &shape_data[le32toh(offset_buff[1]) & 0xFFFFFF];

        if (header->flags & 1) {
            lcw_data = &shape_data[(le32toh(offset_buff[1]) & 0xFFFFFF) + 768];
        }

        if (LCW_Uncomp(lcw_data, buffer, frame_size) > frame_size) {
            DEBUG_LOG("LCW decompressed more data than expected.\n");
            return nullptr;
        }

        Apply_XOR_Delta(buffer, lcw_data + xor_data_offset);

        if (frame_type & SHP_XOR_PREV_FRAME) {
            DEBUG_LOG("Decoding delta sequence.\n");
            ++ref_frame;
            int offset_index = 2;

            while (ref_frame <= frame) {
                Apply_XOR_Delta(buffer, &lcw_data[(le32toh(offset_buff[offset_index]) & 0xFFFFFF) - base_frame_offset]);
                ++ref_frame;
                offset_index += 2;

                if (offset_index >= 6 && ref_frame <= frame) {
                    offset_index = 0;
                    memcpy(offset_buff, &shape_data[8 * ref_frame + sizeof(ShapeHeaderStruct)], 28);
                }
            }
        }
    }

    // This bit handles if we have a shape buffer to cache the decompressed frames
    ShapeBufferHeader *buff_header = nullptr;

    if (UseBigShapeBuffer) {
        if (IsTheaterShape) {
            char *saved_tsbp = TheaterShapeBufferPtr;
            // Why height? I don't get it? Anyhow, this bit is aligning the memory
            // Ahh, Buffer_Frame_To_Page writes flags into the extra area
            // for how each line is to be processed.
            char *aligned_tsbp = TheaterShapeBufferPtr + header->height + sizeof(ShapeBufferHeader);

            // Align memory pointer
            uintptr_t align = (uintptr_t)aligned_tsbp;

            if ((align % sizeof(void *)) != 0) {
                align += sizeof(void *) - (align % sizeof(void *));
            }

            aligned_tsbp = (char *)align;

            memcpy(aligned_tsbp, buffer, frame_size);
            buff_header = reinterpret_cast<ShapeBufferHeader *>(TheaterShapeBufferPtr);
            buff_header->draw_flags = -1;
            buff_header->is_theater_shape = true;
            buff_header->frame_offset = aligned_tsbp - TheaterShapeBufferStart;
            KeyFrameSlots[header->y_pos][frame] = TheaterShapeBufferPtr - TheaterShapeBufferStart;
            TheaterShapeBufferPtr = aligned_tsbp + frame_size;

            // Align memory pointer
            align = (uintptr_t)TheaterShapeBufferPtr;
            if ((align % sizeof(void *)) != 0) {
                align += sizeof(void *) - (align % sizeof(void *));
            }
            TheaterShapeBufferPtr = (char *)align;

            Length = frame_size;

            return saved_tsbp;
        } else {
            char *saved_bsbp = BigShapeBufferPtr;
            char *aligned_bsbp = BigShapeBufferPtr + header->height + sizeof(ShapeBufferHeader);

            // Align memory pointer
            uintptr_t align = (uintptr_t)aligned_bsbp;

            if ((align % sizeof(void *)) != 0) {
                align += sizeof(void *) - (align % sizeof(void *));
            }

            aligned_bsbp = (char *)align;

            memcpy(aligned_bsbp, buffer, frame_size);
            buff_header = reinterpret_cast<ShapeBufferHeader *>(BigShapeBufferPtr);
            buff_header->draw_flags = -1;
            buff_header->is_theater_shape = false;
            buff_header->frame_offset = aligned_bsbp - BigShapeBufferStart;
            KeyFrameSlots[header->y_pos][frame] = BigShapeBufferPtr - BigShapeBufferStart;
            BigShapeBufferPtr = aligned_bsbp + frame_size;

            // Align memory pointer
            align = (uintptr_t)BigShapeBufferPtr;

            if ((align % sizeof(void *)) != 0) {
                align += sizeof(void *) - (align % sizeof(void *));
            }

            BigShapeBufferPtr = (char *)align;

            Length = frame_size;
            return saved_bsbp;
        }
    }

    return buffer;
}

unsigned short Get_Build_Frame_Count(void *shape)
{
    if (shape != nullptr) {
        return static_cast<ShapeHeaderStruct *>(shape)->frame_count;
    }

    return 0;
}

unsigned short Get_Build_Frame_X(void *shape)
{
    if (shape != nullptr) {
        return static_cast<ShapeHeaderStruct *>(shape)->x_pos;
    }

    return 0;
}

unsigned short Get_Build_Frame_Y(void *shape)
{
    if (shape != nullptr) {
        return static_cast<ShapeHeaderStruct *>(shape)->y_pos;
    }

    return 0;
}

unsigned short Get_Build_Frame_Width(void *shape)
{
    if (shape != nullptr) {
        return static_cast<ShapeHeaderStruct *>(shape)->width;
    }

    return 0;
}

unsigned short Get_Build_Frame_Height(void *shape)
{
    if (shape != nullptr) {
        return static_cast<ShapeHeaderStruct *>(shape)->height;
    }

    return 0;
}

// This would handle a shp file with a palette, RA doesn't actually have any that
// have this though.
BOOL Get_Build_Frame_Palette(void *shape, void *pal)
{
    ShapeHeaderStruct *header = static_cast<ShapeHeaderStruct *>(shape);

    if (shape && header->flags & SHP_HAS_PAL) {
        // calc offset to palette
        memcpy(pal, static_cast<char *>(shape) + 8 * header->frame_count + sizeof(ShapeHeaderStruct), 768);

        return true;
    }

    return false;
}

void Set_Shape_Buffer(void *buffer, int size)
{
    ShapeBuffer = static_cast<uint8_t *>(buffer);
    ShapeBufferSize = size;
}

int Set_Shape_Height(void *shape, unsigned short new_height)
{
    int oldheight = static_cast<ShapeHeaderStruct *>(shape)->height;
    static_cast<ShapeHeaderStruct *>(shape)->height = new_height;
    return oldheight;
}

// Used by Buffer_Frame_To_Page to flag which blit function to use for each line
// Results are cached for subsequent draw calls.
static void Single_Line_Flagger(
    int width, int height, void *frame, void *draw_header, int flags, void *ghost_tab, void *ghost_lookup)
{
    uint8_t *shape_src;
    uint8_t *flag_dst;
    uint8_t tmp_flags;
    int current_byte;
    uint8_t has_skipped;
    uint8_t row_flags;
    int skipped;

    shape_src = static_cast<uint8_t *>(frame);
    ShapeBufferHeader *header = static_cast<ShapeBufferHeader *>(draw_header);
    header->draw_flags = flags & 0x1340;
    flag_dst = static_cast<uint8_t *>(draw_header) + sizeof(ShapeBufferHeader);
    uint8_t *ghost = static_cast<uint8_t *>(ghost_lookup);

    for (int i = height; i > 0; --i) {
        tmp_flags = 0;
        skipped = 0;
        for (int j = width; j > 0; --j) {
            current_byte = *shape_src++;

            if (current_byte || !(flags & 0x40)) {
                if (flags & SHAPE_PREDATOR) {
                    tmp_flags |= 8;
                }

                if (flags & SHAPE_GHOST && ghost[current_byte] != 0xFF) {
                    tmp_flags |= 2;
                }

                if (flags & SHAPE_FADING) {
                    tmp_flags |= 4;
                }
            } else {
                tmp_flags |= 1;
                ++skipped;
            }
        }

        has_skipped = 0;

        if (tmp_flags & 1 && (has_skipped = 1, width == skipped)) {
            row_flags = 0x10;
        } else {
            row_flags = tmp_flags & 4 | tmp_flags & 2 | tmp_flags & 8 | has_skipped;
        }

        *flag_dst++ = row_flags;
    }
}

void Buffer_Frame_To_Page(int x, int y, int width, int height, void *shape, GraphicViewPortClass &viewport, int flags, ...)
{
    uint8_t *shape_buff;
    int current_frame;
    int pitch;
    uint8_t *dst;
    int use_old_drawer;
    ShapeBufferHeader *draw_header;
    int src_pitch;
    int dst_pitch;
    int blit_style;
    int fade_count = 0;
    uint8_t *fade_table = nullptr;
    uint8_t *ghost_table = nullptr;
    uint8_t *ghost_lookup = nullptr;

    if (!shape) {
        return;
    }

    uint8_t *frame_data = static_cast<uint8_t *>(shape);

    if (UseOldShapeDraw) {
        use_old_drawer = 1;
    } else if (UseBigShapeBuffer) {
        draw_header = static_cast<ShapeBufferHeader *>(shape);
        shape_buff = reinterpret_cast<uint8_t *>(BigShapeBufferStart);

        if (draw_header->is_theater_shape) {
            shape_buff = reinterpret_cast<uint8_t *>(TheaterShapeBufferStart);
        }

        frame_data = shape_buff + draw_header->frame_offset;
        use_old_drawer = 0;
    }

    va_list ap;
    va_start(ap, flags);

    blit_style = 0;
    if (flags & SHAPE_CENTER) {
        x -= width >> 1;
        y -= height >> 1;
    }

    // Sets for BF_Trans functions
    if (flags & 0x40) {
        blit_style |= 1;
    }

    // Sets for BF_Ghost functions
    if (flags & SHAPE_GHOST) {
        blit_style |= 2;
        ghost_lookup = va_arg(ap, uint8_t *);
        ghost_table = ghost_lookup + 256;
    }

    if (!UseBigShapeBuffer || UseOldShapeDraw) {
        use_old_drawer = 1;
    }

    if (use_old_drawer != 1 && (draw_header->draw_flags == 0xFFFFFFFF || draw_header->draw_flags != (flags & 0x1340))) {
        Single_Line_Flagger(width, height, frame_data, draw_header, flags, ghost_table, ghost_lookup);
    }

    // Sets for BF_Fading functions
    if (flags & SHAPE_FADING) {
        fade_table = va_arg(ap, uint8_t *);
        fade_count = va_arg(ap, int) & 0x3F;
        blit_style |= 4;

        if (!fade_count) {
            flags &= ~SHAPE_FADING;
        }

        // Special blitters for if fade step count is only 1
        NewShapeJumpTable[4] = Single_Line_Single_Fade;
        NewShapeJumpTable[5] = Single_Line_Single_Fade_Trans;

        if (fade_count != 1) {
            NewShapeJumpTable[4] = Single_Line_Fading;
            NewShapeJumpTable[5] = Single_Line_Fading_Trans;
        }
    }

    // Sets for BF_Predator functions
    if (flags & SHAPE_PREDATOR) {
        current_frame = va_arg(ap, uint32_t);
        blit_style |= 8;

        g_predatorFrame = current_frame % 8;
        g_predatorAccumulated = 0;
        g_predatorIncrement = 256;

        // TODO Fix C&C stealth bug, C&C stealth effect can read outside the buffer at the lower right corner of the screen.
        // Unused by default in RA, but would be nice on the phase tank in Aftermath.
    }

    if (flags & 0x4000) {
        g_predatorIncrement = va_arg(ap, int) & 0xFF;
    }

    va_end(ap);

    int xstart = x;
    int ystart = y;
    int yend = y + height - 1;
    int xend = x + width - 1;
    int ms_img_offset = 0;

    // If we aren't drawing within the viewport, return.
    if (xstart >= viewport.Get_Width() || ystart >= viewport.Get_Height() || xend <= 0 || yend <= 0) {
        return;
    }

    // Do any needed clipping.
    if (xstart < 0) {
        ms_img_offset = -xstart;
        xstart = 0;
        use_old_drawer = 1;
    }

    if (ystart < 0) {
        frame_data += width * (-ystart);
        ystart = 0;
        use_old_drawer = 1;
    }

    if (xend >= viewport.Get_Width() - 1) {
        xend = viewport.Get_Width() - 1;
        use_old_drawer = 1;
    }

    if (yend >= viewport.Get_Height() - 1) {
        yend = viewport.Get_Height() - 1;
        use_old_drawer = 1;
    }

    int blit_width = xend - xstart + 1;
    int blit_height = yend - ystart + 1;

    pitch = viewport.Get_Full_Pitch();
    dst = ystart * pitch + xstart + static_cast<uint8_t *>(viewport.Get_Offset());
    uint8_t *src = frame_data + ms_img_offset;
    dst_pitch = pitch - blit_width;
    src_pitch = width - blit_width;

    // Use "new" line drawing routines that appear to have been added during the windows port.
    if (use_old_drawer != 1) {
        // DEBUG_SAY("Drawing with Single_Line draw functions\n");

        // Here we can use the individual line drawing routines
        // Means we can skip drawing some lines all together or avoid using
        // more expensive routines on some lines.
        uint8_t *line_flags = reinterpret_cast<uint8_t *>(draw_header + 1);

        for (int i = 0; i < blit_height; ++i) {
            NewShapeJumpTable[line_flags[i] & 0x1F](blit_width, dst, src, ghost_lookup, ghost_table, fade_table, fade_count);
            src += width;
            dst += pitch;
        }

        return;
    }

    // Here we just use the function that will blit the entire frame
    // using the appropriate effects.
    if (blit_height > 0 && blit_width > 0) {
        // DEBUG_SAY("Drawing with BF draw functions\n");
        OldShapeJumpTable[blit_style & 0xF](
            blit_width, blit_height, dst, src, dst_pitch, src_pitch, ghost_lookup, ghost_table, fade_table, fade_count);
    }
}
