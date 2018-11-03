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
#include "dialog.h"
#include "abs.h"
#include "gamefile.h"
#include "drawshape.h"
#include "gadget.h"
#include "gamedebug.h"
#include "gbuffer.h"
#include "globals.h"
#include "language.h"
#include "mixfile.h"
#include "mouse.h"
#include "textprint.h"
#include <cstdarg>
#include <cstdio>

#ifndef CHRONOSHIFT_STANDALONE
WindowType *WindowList = reinterpret_cast<WindowType*>(0x006016CC);
unsigned &Window = Make_Global<unsigned>(0x006B1A08);
int &WindowColumns = Make_Global<int>(0x0060CE6C);
int &WindowLines = Make_Global<int>(0x0060CE70);
int &WindowWidth = Make_Global<int>(0x0060CE74);
int &WPos = Make_Global<int>(0x006B19DC);
int &WinX = Make_Global<int>(0x006B19F8);
int &WinY = Make_Global<int>(0x006B19FC);
int &WinW = Make_Global<int>(0x0060CE80);
int &WinH = Make_Global<int>(0x0060CE7C);
int &WinC = Make_Global<int>(0x0060CE78);
int &WinB = Make_Global<int>(0x006B19F4);
int &WinCx = Make_Global<int>(0x006B1A00);
int &WinCy = Make_Global<int>(0x006B1A04);
int &ScrollCounter = Make_Global<int>(0x006B197C);
#else
WindowType WindowList[WINDOW_COUNT] = {
    { 0, 0, 640, 400, 15, 12, 0, 0 },
    { 8, 75, 304, 100, 15, 12, 0, 0 },
    { 0, 0, 640, 400, 15, 14, 0, 0 },
    { 96, 157, 128, 42, 14, 13, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 40, 30, 240, 140, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 15, 12, 0, 0 }
};
unsigned Window;
int WindowColumns = 40;
int WindowLines = 25;
int WindowWidth = 40;
int WPos = 0;
int WinX = 0;
int WinY = 0;
int WinW = 40;
int WinH = 25;
int WinC = 1;
int WinB = 0;
int WinCx = 0;
int WinCy = 0;
int ScrollCounter = 0;
#endif

/**
 * @brief Prints a string to the current logic page using font and settings based on TextPrintType passed in.
 */
void Simple_Text_Print(
    const char *string, unsigned x, unsigned y, RemapControlType *remapper, uint8_t bgcolor, TextPrintType style)
{
    // Are these static for some reason?
    static int yspace;
    static int xspace;

    char fgcolor;
    void *fontptr;
    uint8_t xlat[16];
    RemapControlType *mapper;

    int xpos = x;
    int ypos = y;

    // Set default remapper
    // Sole/C&C gets passed an enum which indexes into various static color tables
    // rather than there being remap objects that contain all the various color
    // mapping tables
    if (!remapper) {
        mapper = &ColorRemaps[REMAP_2]; // Red remap
    } else {
        mapper = remapper;
    }

    // DEBUG_SAY("Using RemapControlType:\n");
    // for ( int i = 0; i < sizeof(RemapControlType); ++i ) {
    //	DEBUG_SAY("%03u ", mapper[i]);
    //}

    // DEBUG_SAY("\n");

    memset(xlat, bgcolor, 16);
    int font = style & TPF_FONTS;
    fgcolor = mapper->MediumColor;

    // Does the font need gradient color range?
    switch (font) {
        case TPF_VCR:
        case TPF_6PT_GRAD:
        case TPF_12PT_METAL:
        case TPF_EDITOR:
        case TPF_TYPEWRITER:
            if (style & TPF_USE_GRAD_PAL) {
                memcpy(xlat, mapper->FontPalette, sizeof(xlat));
                fgcolor = mapper->MediumColor;

                if (font == TPF_TYPEWRITER) {
                    fgcolor = xlat[1];
                }

            } else {
                memset(&xlat[4], mapper->MediumColor, 12);
                fgcolor = mapper->MediumColor;
            }

            if (style & TPF_USE_MEDIUM) {
                fgcolor = mapper->MediumColor;
                memset(&xlat[4], mapper->MediumColor, 12);
            }

            if (style & TPF_USE_BRIGHT) {
                fgcolor = mapper->WindowPalette[5];
                memset(&xlat[4], mapper->BrightColor, 12);
            }

            break;

        default:
            break;
    }

    xspace = 1;
    yspace = 0;

    switch (font) {
        case TPF_SCORE:
            fontptr = ScoreFontPtr;
            break;

        case TPF_12PT_METAL:
            // xspace = 2; //temp adjust
            fontptr = Metal12FontPtr;
            break;

        case TPF_MAP:
            fontptr = MapFontPtr;
            xspace = 0;
            break;

        case TPF_VCR:
            fontptr = VCRFontPtr;
            break;

        case TPF_6PT_GRAD:
            fontptr = GradFont6Ptr;
            xspace = 0;
            break;

        case TPF_3PT:
            style = style & (TPF_GRAD_ALL | TPF_ALIGN_MAX | TPF_RED_SHAD | TPF_FONTS);
            fontptr = Font3Ptr;
            xspace = 2;
            break;

        case TPF_6PT:
            fontptr = Font6Ptr;
            xspace = 0;
            break;

        case TPF_EDITOR:
            fontptr = EditorFont;
            yspace = 1;
            xspace = -1;
            break;

        case TPF_8PT:
            fontptr = Font8Ptr;
            xspace = -1;
            yspace = -4;
            break;

        case TPF_LED:
            fontptr = FontLEDPtr;
            xspace = -3;
            break;

        case TPF_TYPEWRITER:
            fontptr = TypeFontPtr;
            xspace = -2;
            yspace = 2;
            break;

        default:
            fontptr = g_fontPtr;
            break;
    }

    int colors = style & TPF_OUTLINE_ALL;

    switch (colors) {
        case TPF_NOSHADOW:
            xlat[2] = bgcolor;
            xlat[3] = bgcolor;
            --xspace;
            yspace -= 2;
            break;

        case TPF_SHADOW:
            --xspace;
            xlat[2] = 12;
            xlat[3] = bgcolor;
            break;

        case TPF_OUTLINE:
            --xspace;
            xlat[2] = 12;
            xlat[3] = 12;
            break;

        case TPF_RED_SHAD:
            xlat[2] = 232; // signed -24
            --xspace;
            xlat[3] = bgcolor;
            break;

        default:
            break;
    }

    if (font != TPF_TYPEWRITER) {
        xlat[0] = bgcolor;
        xlat[1] = mapper->MediumColor;
    }

    g_fontXSpacing = xspace;
    g_fontYSpacing = yspace;

    Set_Font(fontptr);

    // Copy our local modified font palette to the global one used by Buffer_Print.
    Set_Font_Palette_Range(xlat, 0, 15);

    if (string && string[0] != '\0') {
        int alignment = style & TPF_ALIGNMENT;

        switch (alignment) {
            case TPF_CENTER:
                xpos -= String_Pixel_Width(string) / 2;
                break;

            case TPF_RIGHT:
                xpos -= String_Pixel_Width(string);
                break;

            case TPF_LEFT:
            default:
                break;
        }

        if (xpos < g_logicPage->Get_Width() && ypos < g_logicPage->Get_Height()) {
            g_logicPage->Print(string, xpos, ypos, fgcolor, bgcolor);
        }
    }
}

/**
 * @brief Static function available only in this translation unit (cpp file) as low level handler for variadic functions.
 */
static void Fancy_Text_Print_VL(const char *string, unsigned x, unsigned y, RemapControlType *remapper, uint8_t bgcolor,
    TextPrintType style, va_list args)
{
    char str_buff[512];

    if (string != nullptr) {
        vsnprintf(str_buff, sizeof(str_buff), string, args);
        Simple_Text_Print(str_buff, x, y, remapper, bgcolor, style);
    } else {
        Simple_Text_Print(nullptr, x, y, remapper, bgcolor, style);
    }
}

/**
 * @brief Prints a string to the current logic page using font and settings based on TextPrintType passed in.
 */
void Fancy_Text_Print(
    int str_id, unsigned x, unsigned y, RemapControlType *remapper, uint8_t bgcolor, TextPrintType style, ...)
{
    if (str_id != TXT_NONE) {
        va_list args;
        va_start(args, style);
        Fancy_Text_Print_VL(Fetch_String(str_id), x, y, remapper, bgcolor, style, args);
        va_end(args);
    } else {
        Simple_Text_Print(nullptr, x, y, remapper, bgcolor, style);
    }
}

/**
 * @brief Prints a string to the current logic page using font and settings based on TextPrintType passed in.
 */
void Fancy_Text_Print(
    const char *string, unsigned x, unsigned y, RemapControlType *remapper, uint8_t bgcolor, TextPrintType style, ...)
{
    if (string) {
        va_list args;
        va_start(args, style);
        Fancy_Text_Print_VL(string, x, y, remapper, bgcolor, style, args);
        va_end(args);
    } else {
        Simple_Text_Print(nullptr, x, y, remapper, bgcolor, style);
    }
}

void Conquer_Clip_Text_Print(const char *string, unsigned x, unsigned y, RemapControlType *remapper, uint8_t bgcolor,
    TextPrintType style, int x_max, int *tabs)
{
    char fmt_buff[512];

    if (string) {
        strncpy(fmt_buff, string, 512);
        fmt_buff[511] = '\0';

        // These calls to Simple_Text_Print with a NULL string cause the global
        // font and style settings to be set so the rest of the formatting is
        // correct for the font the style refers to.
        Simple_Text_Print(nullptr, 0, 0, 0, 0, style);

        int current_tab = 0;
        char *fmt_ptr = fmt_buff;
        bool chars_to_process = true;

        while (chars_to_process && current_tab < x_max) {
            char *tab_finder = fmt_ptr;

            // Scan the string for any tab chars to process
            while (*tab_finder != '\t') {
                if (*tab_finder) {
                    ++tab_finder;
                    char current_byte = *tab_finder;
                    if (current_byte == '\t') {
                        break;
                    }

                    ++tab_finder;
                    if (current_byte) {
                        continue;
                    }
                }
                tab_finder = nullptr;
                break;
            }

            // Replace the tab char with '\0' to terminate string at the tab but
            // only if we found a tab in the first place
            if (tab_finder) {
                *tab_finder = '\0';
            }

            if (*fmt_ptr) {
                char *str_ptr = fmt_ptr;
                int x_add = 0;

                // Work out how long our string fragment is in pixels, stopping
                // if we exceed the max xpos.
                while (*str_ptr && current_tab + x_add < x_max) {
                    char current_byte = *str_ptr++;
                    x_add += Char_Pixel_Width(current_byte);
                }

                // If we exceeded max, back up and flag to end the loop as well
                // as nulling the prev char in the buffer
                if (current_tab + x_add >= x_max) {
                    int last_char_x = Char_Pixel_Width(*(str_ptr - 1));
                    *(str_ptr - 1) = '\0';
                    x_add -= last_char_x;
                    chars_to_process = false;
                }

                int print_xpos = current_tab + x;
                current_tab += x_add;
                Simple_Text_Print(fmt_ptr, print_xpos, y, remapper, bgcolor, style);
            }

            // At this point, if we didn't find a tab we will have printed the
            // remainder of the string so we can exit the loop.
            if (!tab_finder) {
                break;
            }

            // If we have an array of tab offsets, scan through for the next one
            // and set it to but if not, set it to standard of 50 * last + 1
            if (tabs) {
                while (current_tab > *tabs) {
                    ++tabs;
                }

                current_tab = *tabs;
            } else {
                current_tab = 50 * (current_tab + 1);
            }

            fmt_ptr = tab_finder + 1;
        }
    }
}

void Plain_Text_Print(int str_id, unsigned x, unsigned y, uint8_t fgcolor, uint8_t bgcolor, TextPrintType style, ...)
{
    RemapControlType remapper;

    memset(&remapper, 0, sizeof(RemapControlType));
    memset(&remapper.FontPalette[4], fgcolor, 12);
    remapper.BrightColor = fgcolor;
    remapper.MediumColor = fgcolor;
    remapper.WindowPalette[0] = fgcolor;
    remapper.WindowPalette[1] = fgcolor;
    remapper.WindowPalette[2] = fgcolor;
    remapper.WindowPalette[3] = fgcolor;
    remapper.WindowPalette[4] = fgcolor;
    remapper.WindowPalette[5] = fgcolor;
    remapper.WindowPalette[6] = fgcolor;
    remapper.WindowPalette[7] = fgcolor;

    va_list args;
    va_start(args, style);
    Fancy_Text_Print_VL(Fetch_String(str_id), x, y, &remapper, bgcolor, style, args);
    va_end(args);
}

void Plain_Text_Print(const char *string, unsigned x, unsigned y, uint8_t fgcolor, uint8_t bgcolor, TextPrintType style, ...)
{
    RemapControlType remapper;

    memset(&remapper, 0, sizeof(RemapControlType));
    memset(&remapper.FontPalette[4], fgcolor, 12);
    remapper.BrightColor = fgcolor;
    remapper.MediumColor = fgcolor;
    remapper.WindowPalette[0] = fgcolor;
    remapper.WindowPalette[1] = fgcolor;
    remapper.WindowPalette[2] = fgcolor;
    remapper.WindowPalette[3] = fgcolor;
    remapper.WindowPalette[4] = fgcolor;
    remapper.WindowPalette[5] = fgcolor;
    remapper.WindowPalette[6] = fgcolor;
    remapper.WindowPalette[7] = fgcolor;

    va_list args;
    va_start(args, style);
    Fancy_Text_Print_VL(string, x, y, &remapper, bgcolor, style, args);
    va_end(args);
}

int Format_Window_String(char *string, int max_w, int &w, int &h)
{
    w = 0;
    h = 0;

    if (string) {
        int lines = 0;
        while (*string) {
            // We just started a new line, so increment the height by font height
            h += g_fontYSpacing + g_fontHeight;
            int width = 0;
            ++lines;

            // Replace @ with a carriage return
            if (*string == '@') {
                *string = '\r';
            }

            // Scan through the string for another char indicating a carriage
            // return or the end of the string.
            while (width < max_w) {
                char current_char = *string;

                if (current_char == '\r' || !current_char || current_char == '@') {
                    break;
                }

                ++string;
                width += Char_Pixel_Width(current_char);
            }

            // If scanning through string exceeded max width in pixels, back up
            // to a suitable line break point.
            if (width >= max_w) {
                while (true) {
                    char current_char = *string;

                    if (*string == ' ' || current_char == '\r' || !current_char || current_char == '@') {
                        break;
                    }

                    --string;
                    width -= Char_Pixel_Width(current_char);
                }
            }

            // If our width exceeded the last we put in the provided pointer,
            // put new value
            if (width > w) {
                w = width;
            }

            // Set current point to be a new carriage return for the print func
            if (*string) {
                *string++ = '\r';
            }
        }

        return lines;
    }

    return 0;
}

int Format_Window_String_New(const char *string, int max_w, int &w, int &h, char *new_string, int offset)
{
    w = 0;
    h = 0;

    if (string) {
        int lines = 0;

        while (*string) {
            // We just started a new line, so increment the height by font height
            h += g_fontYSpacing + g_fontHeight;
            ++lines;

            int width;
            char current_char = *string;
            *new_string = current_char;

            // Scan through the string for a char indicating a carriage return
            // or the end of the string.
            for (width = Char_Pixel_Width(current_char); width < max_w; width += Char_Pixel_Width(current_char)) {
                if (current_char == '\r' || !current_char) {
                    break;
                }

                current_char = *(++string);
                *(++new_string) = current_char;
            }

            // If scanning through string exceeded max width in pixels, back up
            // to a suitable line break point.
            if (width >= max_w) {
                const char *string_pos;
                char past_char;

                for (string_pos = string; width > 0; width -= Char_Pixel_Width(past_char)) {
                    past_char = *string;

                    if (past_char == ' ' || past_char == '\r' || !past_char) {
                        break;
                    }

                    --string;
                }

                if (width > 0) {
                    new_string -= string_pos - string;
                } else {
                    string = string_pos - 1;

                    if (offset <= 0) {
                        --new_string;
                    } else {
                        --offset;
                    }
                }
            }

            // If our width exceeded the last we put in the provided pointer,
            // put new value
            if (width > w) {
                w = width;
            }

            // Set current point to be a new carriage return for the print func.
            if (*string) {
                ++string;
                *new_string++ = '\r';
            }
        }

        return lines;
    }

    return 0;
}

void Draw_Caption(const char *string, int x, int y, int w)
{
    if (string != nullptr && *string != '\0') {
        int middle = w / 2 + x;

        if (g_inMapEditor) {
            Fancy_Text_Print(string,
                middle,
                y + 4,
                GadgetClass::Get_Color_Scheme(),
                0,
                TPF_EDITOR | TPF_NOSHADOW | TPF_CENTER | TPF_USE_GRAD_PAL);
        } else {
            Fancy_Text_Print(
                string, middle, y + 16, GadgetClass::Get_Color_Scheme(), 0, TPF_6PT_GRAD | TPF_NOSHADOW | TPF_CENTER);

            int width = String_Pixel_Width(string) / 2;

            g_logicPage->Draw_Line(middle - width,
                g_fontYSpacing + g_fontHeight + y + 16,
                middle + width,
                g_fontYSpacing + g_fontHeight + y + 16,
                GadgetClass::Get_Color_Scheme()->WindowPalette[4]);
        }
    }
}

void Draw_Caption(int str_id, int x, int y, int w)
{
    Draw_Caption(Fetch_String(str_id), x, y, w);
}

void Dialog_Box(int x_pos, int y_pos, int width, int height)
{
    //int new_x_pos = Max(x_pos - 30, 0);
    //int new_y_pos = Max(y_pos - 8, 0);

    //int new_width = Min(width + 60, GraphicViewPortClass::ScreenWidth - new_x_pos);
    //int new_height = Min(height + 16, GraphicViewPortClass::ScreenHeight - new_y_pos);

    WindowList[WINDOW_6].X = x_pos;
    WindowList[WINDOW_6].Y = y_pos;
    WindowList[WINDOW_6].W = width;
    WindowList[WINDOW_6].H = height;

    GraphicViewPortClass *saved = Set_Logic_Page(g_hidPage);

    // Draw dialog background.
    void *bkgndshape = GameFileClass::Retrieve("DD-BKGND.SHP");
    CC_Draw_Shape(bkgndshape,
        0,
        width / 2 - Get_Build_Frame_Width(bkgndshape),
        height / 2 - Get_Build_Frame_Height(bkgndshape),
        WINDOW_6,
        SHAPE_VIEWPORT_REL);
    CC_Draw_Shape(bkgndshape, 1, width / 2, height / 2 - Get_Build_Frame_Height(bkgndshape), WINDOW_6, SHAPE_VIEWPORT_REL);
    CC_Draw_Shape(bkgndshape, 2, width / 2 - Get_Build_Frame_Width(bkgndshape), height / 2, WINDOW_6, SHAPE_VIEWPORT_REL);
    CC_Draw_Shape(bkgndshape, 3, width / 2, height / 2, WINDOW_6, SHAPE_VIEWPORT_REL);

    // Draw dialog inner edges.
    void *edgeshape = GameFileClass::Retrieve("DD-EDGE.SHP");
    for (int i = 0; i < height; i += Get_Build_Frame_Height(edgeshape)) {
        CC_Draw_Shape(edgeshape, 0, 14, i, WINDOW_6, SHAPE_VIEWPORT_REL);
        CC_Draw_Shape(edgeshape, 1, width - 30, i, WINDOW_6, SHAPE_VIEWPORT_REL);
    }

    // Draw dialog left border.
    void *leftshape = GameFileClass::Retrieve("DD-LEFT.SHP");
    CC_Draw_Shape(leftshape, 0, 0, height / 2 - Get_Build_Frame_Height(leftshape), WINDOW_6, SHAPE_VIEWPORT_REL);
    CC_Draw_Shape(leftshape, 0, 0, height / 2, WINDOW_6, SHAPE_VIEWPORT_REL);

    // Draw dialog right border.
    void *rightshape = GameFileClass::Retrieve("DD-RIGHT.SHP");
    CC_Draw_Shape(rightshape,
        0,
        width - Get_Build_Frame_Width(rightshape),
        height / 2 - Get_Build_Frame_Height(rightshape),
        WINDOW_6,
        SHAPE_VIEWPORT_REL);
    CC_Draw_Shape(rightshape, 0, width - Get_Build_Frame_Width(rightshape), height / 2, WINDOW_6, SHAPE_VIEWPORT_REL);

    // Draw dialog bottom border.
    void *botmshape = GameFileClass::Retrieve("DD-BOTM.SHP");
    CC_Draw_Shape(botmshape, 0, width / 2 - 320, height - 16, WINDOW_6, SHAPE_VIEWPORT_REL);
    CC_Draw_Shape(botmshape, 0, width / 2, height - 16, WINDOW_6, SHAPE_VIEWPORT_REL);

    // Draw dialog top border.
    void *topshape = GameFileClass::Retrieve("DD-TOP.SHP");
    CC_Draw_Shape(topshape, 0, width / 2 - 320, 0, WINDOW_6, SHAPE_VIEWPORT_REL);
    CC_Draw_Shape(topshape, 0, width / 2, 0, WINDOW_6, SHAPE_VIEWPORT_REL);

    // Draw dialog corners.
    void *cornershape = GameFileClass::Retrieve("DD-CRNR.SHP");
    CC_Draw_Shape(cornershape, 0, 0, 0, WINDOW_6, SHAPE_VIEWPORT_REL);
    CC_Draw_Shape(cornershape, 1, width - Get_Build_Frame_Width(cornershape), 0, WINDOW_6, SHAPE_VIEWPORT_REL);
    CC_Draw_Shape(cornershape, 2, 0, height - Get_Build_Frame_Height(cornershape), WINDOW_6, SHAPE_VIEWPORT_REL);
    CC_Draw_Shape(cornershape,
        3,
        width - Get_Build_Frame_Width(cornershape),
        height - Get_Build_Frame_Height(cornershape),
        WINDOW_6,
        SHAPE_VIEWPORT_REL);

    g_mouse->Draw_Mouse(g_hidPage);

    g_hidPage.Blit(g_seenBuff, x_pos, y_pos, x_pos, y_pos, width, height, false);

    g_mouse->Erase_Mouse(g_hidPage, false);

    Set_Logic_Page(saved);
}

void Draw_Box(int x_pos, int y_pos, int width, int height, BoxStyleEnum style, BOOL fill)
{
    DEBUG_ASSERT(style != BOX_STYLE_NONE);
    DEBUG_ASSERT(style < BOX_STYLE_COUNT);
    
    // this is a list of colour indices for rendering the bevel effect of a box i think.
    // Looks like each style has 4 colours, one for the body of the button,
    // two for a beveled look if its a button and 1 for the bevel corner transitions
    static BoxStyleType base_styles[BOX_STYLE_COUNT] = { { COLOR_TBLACK, COLOR_TBLACK, COLOR_TBLACK, COLOR_TBLACK },
        { COLOR_TBLACK, COLOR_TBLACK, COLOR_TBLACK, COLOR_TBLACK },
        { COLOR_GREY, COLOR_WHITE, COLOR_BLACK, COLOR_GREY },
        { COLOR_GREY, COLOR_BLACK, COLOR_LTGREY, COLOR_GREY },
        { COLOR_BLACK, COLOR_TBLACK, COLOR_TBLACK, COLOR_BLACK },
        { COLOR_BLACK, COLOR_TBLACK, COLOR_TBLACK, COLOR_BLACK } };

    BoxStyleType styles[BOX_STYLE_COUNT];
    memcpy(styles, base_styles, sizeof(base_styles));
    styles[BOX_STYLE_0].FillColor = GadgetClass::Get_Color_Scheme()->WindowPalette[1];
    styles[BOX_STYLE_0].ButtonLowColor = GadgetClass::Get_Color_Scheme()->WindowPalette[3];
    styles[BOX_STYLE_0].ButtonHighColor = GadgetClass::Get_Color_Scheme()->WindowPalette[0];
    styles[BOX_STYLE_0].TransitionColor = GadgetClass::Get_Color_Scheme()->WindowPalette[2];
    styles[BOX_STYLE_1].FillColor = GadgetClass::Get_Color_Scheme()->WindowPalette[1];
    styles[BOX_STYLE_1].ButtonLowColor = GadgetClass::Get_Color_Scheme()->WindowPalette[0];
    styles[BOX_STYLE_1].ButtonHighColor = GadgetClass::Get_Color_Scheme()->WindowPalette[3];
    styles[BOX_STYLE_1].TransitionColor = GadgetClass::Get_Color_Scheme()->WindowPalette[2];
    styles[BOX_STYLE_4].ButtonLowColor = GadgetClass::Get_Color_Scheme()->WindowPalette[4];
    styles[BOX_STYLE_4].ButtonHighColor = GadgetClass::Get_Color_Scheme()->WindowPalette[4];
    styles[BOX_STYLE_5].ButtonLowColor = GadgetClass::Get_Color_Scheme()->WindowPalette[4];
    styles[BOX_STYLE_5].ButtonHighColor = GadgetClass::Get_Color_Scheme()->WindowPalette[4];

    BoxStyleType *box_style = &styles[style];

    if (fill) {
        g_logicPage->Fill_Rect(x_pos, y_pos, width + x_pos - 1, height + y_pos - 1, box_style->FillColor);
    }

    switch (style) {
        case BOX_STYLE_4:
            g_logicPage->Draw_Rect(x_pos, y_pos, width + x_pos - 1, height + y_pos - 1, box_style->ButtonHighColor);
            break;

        case BOX_STYLE_5:
            g_logicPage->Draw_Rect(x_pos + 1, y_pos + 1, width + x_pos - 2, height + y_pos - 2, box_style->ButtonHighColor);
            break;

        default:
            g_logicPage->Draw_Line(
                x_pos, height + y_pos - 1, width + x_pos - 1, height + y_pos - 1, box_style->ButtonLowColor); // bottom line
            g_logicPage->Draw_Line(
                width + x_pos - 1, y_pos, width + x_pos - 1, height + y_pos - 1, box_style->ButtonLowColor); // left line
            g_logicPage->Draw_Line(x_pos, y_pos, width + x_pos - 1, y_pos, box_style->ButtonHighColor); // top line
            g_logicPage->Draw_Line(x_pos, y_pos, x_pos, height + y_pos - 1, box_style->ButtonHighColor); // right line
            g_logicPage->Put_Pixel(x_pos, height + y_pos - 1, box_style->TransitionColor); // transition bottom left
            g_logicPage->Put_Pixel(width + x_pos - 1, y_pos, box_style->TransitionColor); // transition top right
            break;
    }
}

void Window_Box(WindowNumberType type, BoxStyleEnum style)
{
    if (g_logicPage == &g_seenBuff) {
        g_mouse->Conditional_Hide_Mouse(WindowList[style].X,
            WindowList[style].Y,
            WindowList[style].X + WindowList[style].W,
            WindowList[style].Y + WindowList[style].H);
    }

    Draw_Box(WindowList[style].X, WindowList[style].Y, WindowList[style].W, WindowList[style].H, style, true);

    if (g_logicPage == &g_seenBuff) {
        g_mouse->Conditional_Show_Mouse();
    }
}

unsigned Change_Window(unsigned window)
{
    unsigned old_window;
    old_window = Window;
    Window = window;
    WinX = WindowList[window].X;
    WinY = WindowList[window].Y;
    WinW = WindowList[window].W;
    WinH = WindowList[window].H;
    WinC = WindowList[window].C;
    WinB = WindowList[window].B;
    WinCx = WindowList[window].Cx;
    WinCy = WindowList[window].Cy;

    int full_width = g_fontXSpacing + g_fontWidth;
    WPos = WinCx / full_width;
    WindowLines = (WinH - g_fontYSpacing) / (g_fontHeight + g_fontYSpacing);
    WindowWidth = 8 * WinW;
    ScrollCounter = 0;
    WindowColumns = (8 * WinW) / full_width;

    return old_window;
}
