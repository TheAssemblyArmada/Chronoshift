/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Functions for printing nicely formatted text.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
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

enum ColorType
{
    COLOR_NONE = -1,
    COLOR_TBLACK = 0,
    COLOR_PURPLE = 1,
    COLOR_CYAN = 2,
    COLOR_GREEN = 3,
    COLOR_LTGREEN = 4,
    COLOR_YELLOW = 5,
    COLOR_PINK = 6,
    COLOR_BROWN = 7,
    COLOR_RED = 8,
    COLOR_LTCYAN = 9,
    COLOR_LTBLUE = 10,
    COLOR_BLUE = 11,
    COLOR_BLACK = 12,
    COLOR_GREY = 13,
    COLOR_LTGREY = 14,
    COLOR_WHITE = 15,
};

 //0 - 3 look like they draw button styles
 //0 and 1 look like up and down styles for a remapped button
 //2 and 3 look like styles for fixed color scheme buttons
 //4 draws a box
 //5 draws an inset box, probably to look like it has a border?
enum BoxStyleEnum
{
    BOX_STYLE_NONE = -1,

    BOX_STYLE_0 = 0,
    BOX_STYLE_1 = 1,
    BOX_STYLE_2 = 2,
    BOX_STYLE_3 = 3,
    BOX_STYLE_4 = 4,
    BOX_STYLE_5 = 5,

    BOX_STYLE_COUNT = 6
};

struct BoxStyleType
{
    uint8_t FillColor;
    uint8_t ButtonLowColor;
    uint8_t ButtonHighColor;
    uint8_t TransitionColor;

};

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

struct WindowType
{
    int X;		//window x
    int Y;		//window y
    int W;		//window width
    int H;		//window height
    int C;	//window foreground color
    int B;	//window background color
    int Cx;		//window cursor x
    int Cy;		//window cursor y
};

enum WindowIndexType
{
    WINDOWX = 0,
    WINDOWY = 1,
    WINDOWWIDTH = 2,
    WINDOWHEIGHT = 3,
    WINDOWFCOL = 4,
    WINDOWBCOL = 5,
    WINDOWCURSORX = 6,
    WINDOWCURSORY = 7,
};

enum WindowNumberType
{
//    WINDOW_NONE = -1,
    WINDOW_0 = 0,
    WINDOW_1 = 1,
    WINDOW_TACTICAL = 2,
    WINDOW_3 = 3,
    WINDOW_SIDEBAR = 4,
    WINDOW_5 = 5,
    WINDOW_6 = 6,
    WINDOW_COUNT = 7
};

// TODO Refactor global naming after all locations implemented.
#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
extern WindowType *WindowList;
extern unsigned &Window;
extern int &WindowColumns;
extern int &WindowLines;
extern int &WindowWidth;
extern int &WPos;
extern int &WinX;
extern int &WinY;
extern int &WinW;
extern int &WinH;
extern int &WinC;
extern int &WinB;
extern int &WinCx;
extern int &WinCy;
extern int &ScrollCounter;
#else
extern WindowType WindowList[WINDOW_COUNT];
extern unsigned Window;
extern int WindowColumns;
extern int WindowLines;
extern int WindowWidth;
extern int WPos;
extern int WinX;
extern int WinY;
extern int WinW;
extern int WinH;
extern int WinC;
extern int WinB;
extern int WinCx;
extern int WinCy;
extern int ScrollCounter;
#endif

void Simple_Text_Print(
    const char *string, unsigned x, unsigned y, RemapControlType *remapper, uint8_t bgcolor, TextPrintType style);
void Fancy_Text_Print(
    int str_id, unsigned x, unsigned y, RemapControlType *remapper, uint8_t bgcolor, TextPrintType style, ...);
void Fancy_Text_Print(
    const char *string, unsigned x, unsigned y, RemapControlType *remapper, uint8_t bgcolor, TextPrintType style, ...);
void Conquer_Clip_Text_Print(const char *string, unsigned x, unsigned y, RemapControlType *remapper, uint8_t bgcolor,
    TextPrintType style, int x_max, int *tabs);
void Plain_Text_Print(int str_id, unsigned x, unsigned y, uint8_t fgcolor, uint8_t bgcolor, TextPrintType style, ...);
void Plain_Text_Print(
    const char *string, unsigned x, unsigned y, uint8_t fgcolor, uint8_t bgcolor, TextPrintType style, ...);
int Format_Window_String(char *string, int max_w, int &w, int &h);
int Format_Window_String_New(const char *string, int max_w, int &w, int &h, char *new_string, int offset);
void Draw_Caption(const char *string, int x, int y, int w);
void Draw_Caption(int str_id, int x, int y, int w);
void Dialog_Box(int x_pos, int y_pos, int width, int height);
void Draw_Box(int x_pos, int y_pos, int width, int height, BoxStyleEnum style, BOOL unk);
void Window_Box(WindowNumberType type, BoxStyleEnum style);
unsigned Change_Window(unsigned window);

#ifndef CHRONOSHIFT_STANDALONE
namespace Dialog
{

inline void Hook_Me()
{
    Hook_Function(0x004AE3FC, &Simple_Text_Print);
    Hook_Function(0x004AE7FC, static_cast<void(*)(int, unsigned, unsigned, RemapControlType *, uint8_t, TextPrintType, ...)>(&Fancy_Text_Print));//int
    Hook_Function(0x004AE8C0, static_cast<void(*)(const char *, unsigned, unsigned, RemapControlType *, uint8_t, TextPrintType, ...)>(&Fancy_Text_Print));//char
    Hook_Function(0x004AE930, &Conquer_Clip_Text_Print);
    Hook_Function(0x004AEA74, static_cast<void(*)(int, unsigned, unsigned, uint8_t, uint8_t, TextPrintType, ...)>(&Plain_Text_Print));//int
    Hook_Function(0x004AEAF8, static_cast<void(*)(const char *, unsigned, unsigned, uint8_t, uint8_t, TextPrintType, ...)>(&Plain_Text_Print));//char
    Hook_Function(0x004AE284, &Format_Window_String);
    Hook_Function(0x00591F3C, &Format_Window_String_New);
    Hook_Function(0x004AEBB4, static_cast<void(*)(int, int, int, int)>(&Draw_Caption));//int
    Hook_Function(0x004AEC14, static_cast<void(*)(const char *, int, int, int)>(&Draw_Caption));//char
    Hook_Function(0x004AD670, &Dialog_Box);
    Hook_Function(0x004ADB5C, &Draw_Box);
    Hook_Function(0x004AE350, &Window_Box);
}
}

#endif

#endif // DIALOG_H
