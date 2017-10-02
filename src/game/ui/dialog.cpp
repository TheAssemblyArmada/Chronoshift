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
#include "dialog.h"
#include "abs.h"
#include "gadget.h"
#include "gbuffer.h"
#include "globals.h"
#include "language.h"
#include "textprint.h"
#include "gamedebug.h"
#include <stdio.h>

/**
 * @brief Prints a string to the current logic page using font and settings based on TextPrintType passed in.
 */
void Simple_Text_Print(
    char const *string, unsigned x, unsigned y, RemapControlType *remapper, uint8_t bgcolor, TextPrintType style)
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
static void Fancy_Text_Print_VL(char const *string, unsigned x, unsigned y, RemapControlType *remapper, uint8_t bgcolor,
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
    char const *string, unsigned x, unsigned y, RemapControlType *remapper, uint8_t bgcolor, TextPrintType style, ...)
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

void Conquer_Clip_Text_Print(char const *string, unsigned x, unsigned y, RemapControlType *remapper, uint8_t bgcolor,
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

    va_list args;
    va_start(args, style);
    Fancy_Text_Print_VL(Fetch_String(str_id), x, y, &remapper, bgcolor, style, args);
    va_end(args);
}

void Plain_Text_Print(char const *string, unsigned x, unsigned y, uint8_t fgcolor, uint8_t bgcolor, TextPrintType style, ...)
{
    RemapControlType remapper;

    memset(&remapper, 0, sizeof(RemapControlType));
    memset(&remapper.FontPalette[4], fgcolor, 12);

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

int Format_Window_String_New(char const *string, int max_w, int &w, int &h, char *new_string, int offset)
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
                char const *string_pos;
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

void Draw_Caption(char const *string, int x, int y, int w)
{
    if (string && *string) {
        int middle = w / 2 + x;

        if (g_inMapEditor) {
            Fancy_Text_Print(string,
                middle,
                y + 4,
                GadgetClass::Get_Color_Scheme(),
                0,
                TPF_EDITOR | TPF_NOSHADOW | TPF_CENTER | TPF_USE_GRAD_PAL);
        } else {
            Fancy_Text_Print(string, middle, y + 16, GadgetClass::Get_Color_Scheme(), 0, TPF_6PT_GRAD | TPF_NOSHADOW | TPF_CENTER);

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
