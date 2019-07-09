/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Low level bitmap font string blitter.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#ifndef TEXTPRINT_H
#define TEXTPRINT_H

#include "always.h"
class GraphicViewPortClass;

// Defines for TextPrintType fonts
#define FONT_6 1
#define FONT_8 2
#define FONT_3 3
#define FONT_LED 4
#define FONT_VCR 5
#define FONT_GRAD_6 6
#define FONT_MAP 7
#define FONT_METAL_12 8
#define FONT_EDITOR 9
#define FONT_TYPE 10
#define FONT_SCORE 11
#define FONT_COUNT 15

#pragma pack(push, 1)
struct FontHeader
{
    uint16_t file_size; // Size of the file
    uint8_t file_version; // file version? 0==EOB? 1==RA? 2==TS? what about RA2?
    uint8_t unknown; // 5==TD/RA 0==TS? unused in RA
    uint16_t start; // offset to start of font description Pad, always 0x0010
    uint16_t data_offset_list; // offset to bitmap data offset list, always 0x0014 as follows header
    uint16_t width_list; // points to list of widths
    uint16_t data; // points to graphics data, unused by RA code
    uint16_t line_list; // points to list of heights (or lines)
    uint16_t unknown_const; // Unknown entry (always 0x1012 or 0x1011) unused by RA
    uint8_t pad; // padding?
    uint8_t number_of_chars; // Number of chars in font - 1
    uint8_t max_height; // Max char height
    uint8_t max_width; // Max char width
};
#pragma pack(pop)

// Global attributes for the current font.
#ifndef CHRONOSHIFT_STANDALONE
extern char *&g_fontPtr;
extern int &g_fontYSpacing;
extern int &g_fontXSpacing;
extern uint8_t &g_fontHeight;
extern uint8_t &g_fontWidth;
extern uint8_t *g_colorXlat;
extern uint8_t *byte_608988;
#else
extern char *g_fontPtr;
extern int g_fontYSpacing;
extern int g_fontXSpacing;
extern uint8_t g_fontHeight;
extern uint8_t g_fontWidth;
extern uint8_t g_colorXlat[16];
extern uint8_t byte_608988[];
#endif

void __cdecl Set_Font_Palette_Range(uint8_t *colors, uint8_t first, uint8_t last);
void *__cdecl Set_Font(void *font);
uint16_t __cdecl String_Pixel_Width(const char *string);
int __cdecl Char_Pixel_Width(char letter);
int __cdecl Buffer_Print(GraphicViewPortClass &vp, const char *string, int x, int y, uint8_t fground, uint8_t bground);

#endif // TEXTPRINT_H
