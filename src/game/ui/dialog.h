/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Functions for printing nicely formatted text.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef DIALOG_H
#define DIALOG_H

#include "always.h"
#include "remap.h"

enum TextPrintType
{
    TPF_NONE = 0,
    TPF_6PT = 1,
    TPF_8PT = 2,
    TPF_3PT = 3,
    TPF_LED = 4,
    TPF_VCR = 5,
    TPF_6PT_GRAD = 6,
    TPF_MAP = 7, // 8FAT.FNT in Sole --xspace in Simple_Text_Print
    TPF_12PT_METAL = 8, // 12GREEN.FNT in Sole
    TPF_EDITOR = 9, // 12GRNGRD.FNT in Sole
    TPF_TYPEWRITER = 0xA,
    TPF_SCORE = 0xB,
    TPF_FONTS = 0xF,

    TPF_NOSHADOW = 0x10, // bit 5 //Forces no shadow irrespective of remapper font palette.
    TPF_SHADOW = 0x20, // bit 6 //Forces black shadow irrespective of remapper
    TPF_OUTLINE = 0x40, // bit 7 //Forces black outline irrespective of remapper
    TPF_RED_SHAD = 0x80, // bit 8 //index 232 shadow for fntpal 2, red shadow?
    TPF_OUTLINE_ALL = 0xF0,

    TPF_LEFT = 0,
    TPF_CENTER = 0x100, // bit 9
    TPF_RIGHT = 0x200, // bit 10
    TPF_ALIGN_UNK1 = 0x400,
    TPF_ALIGN_UNK2 = 0x800,
    TPF_ALIGNMENT = 0x300,
    TPF_ALIGN_MAX = 0xF00,

    TPF_USE_MEDIUM = 0x1000, // bit 13 //medium table in Sole/C&C
    TPF_USE_BRIGHT = 0x2000, // bit 14 white //bright table in Sole/C&C
    TPF_USE_GRAD_PAL = 0x4000, // bit 15 transparent on most, gradient on MetalScheme fontpal in Sole/C&C
    TPF_UNK_COLOR = 0x8000, // bit 16
    TPF_GRAD_ALL = 0xF000,
};

DEFINE_ENUMERATION_BITWISE_OPERATORS(TextPrintType);

void Simple_Text_Print(
    char const *string, unsigned x, unsigned y, RemapControlType *remapper, uint8_t bgcolor, TextPrintType style);
void Fancy_Text_Print(
    int str_id, unsigned x, unsigned y, RemapControlType *remapper, uint8_t bgcolor, TextPrintType style, ...);
void Fancy_Text_Print(
    char const *string, unsigned x, unsigned y, RemapControlType *remapper, uint8_t bgcolor, TextPrintType style, ...);
void Conquer_Clip_Text_Print(char const *string, unsigned x, unsigned y, RemapControlType *remapper, uint8_t bgcolor,
    TextPrintType style, int x_max, int *tabs);
void Plain_Text_Print(int str_id, unsigned x, unsigned y, uint8_t fgcolor, uint8_t bgcolor, TextPrintType style, ...);
void Plain_Text_Print(
    char const *string, unsigned x, unsigned y, uint8_t fgcolor, uint8_t bgcolor, TextPrintType style, ...);
int Format_Window_String(char *string, int max_w, int &w, int &h);
int Format_Window_String_New(char const *string, int max_w, int &w, int &h, char *new_string, int offset);
void Draw_Caption(char const *string, int x, int y, int w);
void Draw_Caption(int str_id, int x, int y, int w);

#endif // DIALOG_H
