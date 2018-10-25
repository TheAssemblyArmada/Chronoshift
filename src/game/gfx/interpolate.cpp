/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Interpolation tables and functions for scaling up 320 x 200 image content.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "interpolate.h"
#include "gamefile.h"
#include "endiantype.h"
#include "gamedebug.h"
#include "gbuffer.h"
#include "mouse.h"
#include "palette.h"
#include "rotr.h"
#include "swap.h"
#include <malloc.h>

// PaletteClass *g_interpolationPalette = &PaletteClass::CurrentPalette;
#ifndef CHRONOSHIFT_STANDALONE
uint8_t *g_paletteInterpolationTable = reinterpret_cast<uint8_t *>(0x00691734);
BOOL &g_palettesRead = Make_Global<BOOL>(0x00655DA0);
int &g_paletteCounter = Make_Global<int>(0x00655DA4);
int &g_interpolationMode = Make_Global<int>(0x006A1738);
uint8_t **g_interpolatedPalettes = reinterpret_cast<uint8_t **>(0x00655C10);
#else
uint8_t g_paletteInterpolationTable[INTERPOL_PAL_SIZE]; // rewrite this in to true 256 * 256 grid?
BOOL g_palettesRead = false;
int g_paletteCounter = 0;
int g_interpolationMode = COPY_LINE_INTERPOLATE;
uint8_t *g_interpolatedPalettes[100];
#endif

static BOOL g_interpolationPaletteChanged = false;
static uint8_t g_interpolationBuffer[7680];

/**
 * @brief Creates an interpolation table based on the current palette.
 */
void Create_Palette_Interpolation_Table()
{
    // PaletteClass &_InterpolationPalette = *g_interpolationPalette;
    PaletteClass &_InterpolationPalette = PaletteClass::CurrentPalette;

    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            if (j == i) {
                g_paletteInterpolationTable[(i << 8) + j] = i;
            } else {
                RGBClass avg = _InterpolationPalette[j].Average(_InterpolationPalette[i]);
                g_paletteInterpolationTable[(i << 8) + j] = g_paletteInterpolationTable[(j << 8) + i] =
                    _InterpolationPalette.Closest_Color(avg);
            }
        }
    }

    g_interpolationPaletteChanged = false;
}

/**
 * @brief Reads an interpolation table from a file.
 */
void Read_Interpolation_Palette(const char *filename)
{
    GameFileClass fc(filename);

    if (fc.Is_Available()) {
        fc.Open(FM_READ);
        fc.Read(g_paletteInterpolationTable, sizeof(g_paletteInterpolationTable));
        fc.Close();
        g_interpolationPaletteChanged = false;
    }
}

/**
 * @brief Writes an interpolation table to a file.
 */
void Write_Interpolation_Palette(const char *filename)
{
    GameFileClass fc(filename);
    if (!fc.Is_Available()) {
        fc.Open(FM_WRITE);
        fc.Write(g_paletteInterpolationTable, sizeof(g_paletteInterpolationTable));
        fc.Close();
    }
}

/**
 * @brief Rebuilds an interpolation table that was stored in a VQP packed format that removed replicated entries.
 */
void Rebuild_Interpolated_Palette(void *ipalette)
{
    uint8_t *ipal = static_cast<uint8_t *>(ipalette);

    int k = 0;
    for (int i = 0; i < INTERPOL_PAL_SIZE; i += 256) {
        int j = k + 1;

        if (j < 256) {
            uint8_t *dst = &ipal[j] + i;
            uint8_t *src = &ipal[j * 256] + k;

            for (; j < 256; ++j) {
                *dst++ = *src;
                src += 256;
            }
        }

        ++k;
    }
}

/**
 * @brief Reads a series of interpolation tables from a file that was packed to VQP format.
 */
int Load_Interpolated_Palettes(const char *filename, BOOL append)
{
    int palette_count = 0;
    int next_free = 0;
    g_palettesRead = false;
    GameFileClass fc(filename);

    // Do we want to append additional tables or just start fresh?
    if (append) {
        for (next_free = 0; next_free < 100; ++next_free) {
            if (g_interpolatedPalettes[next_free] == nullptr) {
                break;
            }
        }
    } else {
        for (int i = 0; i < 100; ++i) {
            g_interpolatedPalettes[i] = nullptr;
        }
    }

    if (!fc.Is_Available()) {
        DEBUG_LOG("'%s' not available, loading default.\n", fc.File_Name());
        fc.Set_Name("AAGUN.VQP"); // gets defualt vqp pal?
    }

    if (fc.Is_Available()) {
        fc.Open(FM_READ);
        fc.Read(&palette_count, 4); // sizeof(palette_count)
        palette_count = le32toh(palette_count);

        for (int i = 0; i < palette_count; ++i) {
            g_interpolatedPalettes[next_free] = (uint8_t *)malloc(INTERPOL_PAL_SIZE);
            memset(g_interpolatedPalettes[next_free], 0, INTERPOL_PAL_SIZE);

            int position = 0;
            for (int j = 0; j < 256; ++j) {
                fc.Read(&g_interpolatedPalettes[next_free][position], j + 1);
                position += 256;
            }

            Rebuild_Interpolated_Palette(g_interpolatedPalettes[next_free]);
            ++next_free;
        }

        g_palettesRead = true;
        fc.Close();
    }

    g_paletteCounter = 0;

    return palette_count;
}

/**
 * @brief Frees any allocated interpolation tables.
 */
void Free_Interpolated_Palettes()
{
    for (int index = 0; index < ARRAY_SIZE(g_interpolatedPalettes); ++index) {
        free(g_interpolatedPalettes[index]);

        g_interpolatedPalettes[index] = nullptr;
    }
}

/**
 * @brief Performs the interpolation using different methods based on the value of g_interpolationMode.
 */
void Interpolate_2X_Scale(GraphicBufferClass &src, GraphicViewPortClass &dst, const char *filename)
{
    BOOL dst_locked = false;
    BOOL src_locked = false;

    if (g_interpolationPaletteChanged) {
        if (filename) {
            Read_Interpolation_Palette(filename);
        }

        // If the read failed, create a new interpolation pal
        if (g_interpolationPaletteChanged) {
            DEBUG_LOG("Table '%s' not loaded, generating.\n", filename);
            Create_Palette_Interpolation_Table();
        }
    }

    if (filename) {
        // Try and write out the palette if it didn't exist
        Write_Interpolation_Palette(filename);
    }

    if (&dst == &g_seenBuff) {
        g_mouse->Hide_Mouse();
    }

    Wait_Blit();

    if ((src_locked = src.Lock()) == false) {
        if (&dst == &g_seenBuff) {
            g_mouse->Show_Mouse();
        }

        return;
    }

    if ((dst_locked = dst.Lock()) == false) {
        if (src_locked) {
            src.Unlock();
        }

        if (&dst == &g_seenBuff) {
            g_mouse->Show_Mouse();
        }

        return;
    }

    switch (g_interpolationMode) {
        case COPY_INTERLEAVE:
            /* Temp fall through to force best method.
            Interpolate_Interleave(src.Get_Offset(),
                dst.Get_Offset(),
                src.Get_Height(),
                src.Get_Width(),
                2 * (dst.Get_Pitch() + dst.Get_XAdd() + dst.Get_Width()));
            break;
            */
        case COPY_LINE_DOUBLE:
            /*
            Interpolate_Line_Double(src.Get_Offset(),
                dst.Get_Offset(),
                src.Get_Height(),
                src.Get_Width(),
                2 * (dst.Get_Pitch() + dst.Get_XAdd() + dst.Get_Width()));
            break;
            */
        case COPY_LINE_INTERPOLATE:
            Interpolate_Line_Interpolate(src.Get_Offset(),
                dst.Get_Offset(),
                src.Get_Height(),
                src.Get_Width(),
                2 * (dst.Get_Pitch() + dst.Get_XAdd() + dst.Get_Width()));
            break;

        case COPY_NONE:
        default:
            break;
    }

    if (src_locked) {
        src.Unlock();
    }

    if (dst_locked) {
        dst.Unlock();
    }

    if (&dst == &g_seenBuff) {
        g_mouse->Show_Mouse();
    }
}

/**
 * @brief Interpolates in X axis, leaves additional lines blank in the Y axis.
 */
void __cdecl Interpolate_Interleave(void *src, void *dst, int src_height, int src_width, int dst_pitch)
{
    uint8_t *dptr = static_cast<uint8_t *>(dst);
    uint8_t *sptr = static_cast<uint8_t *>(src);

    while (src_height--) {
        uint8_t *wptr = dptr;
        for (int i = 0; i < src_width - 1; ++i) {
            *wptr++ = *sptr;
            *wptr++ = g_paletteInterpolationTable[*(uint16_t *)sptr];
            ++sptr;
        }

        *wptr++ = *sptr++;
        *wptr = 0;
        dptr += dst_pitch;
    }
}

/**
 * @brief Interpolates in X axis, duplicates lines in the Y axis.
 */
void __cdecl Interpolate_Line_Double(void *src, void *dst, int src_height, int src_width, int dst_pitch)
{
    uint8_t *dptr = static_cast<uint8_t *>(dst);
    uint8_t *sptr = static_cast<uint8_t *>(src);

    while (src_height--) {
        uint8_t *wptr = dptr;
        uint8_t *bptr = g_interpolationBuffer;

        for (int i = 0; i < src_width - 1; ++i) {
            *wptr++ = *sptr;
            *bptr++ = *sptr;
            *wptr++ = g_paletteInterpolationTable[*(uint16_t *)sptr];
            *bptr++ = g_paletteInterpolationTable[*(uint16_t *)sptr];
            ++sptr;
        }

        *wptr++ = *sptr;
        *bptr++ = *sptr++;
        *wptr = 0;
        *bptr = 0;
        dptr += dst_pitch / 2;
        memcpy(dptr, g_interpolationBuffer, src_width * 2);
        dptr += dst_pitch / 2;
    }
}

static void Interpolate_X_Axis(void *src, void *dst, int src_width)
{
    uint8_t *dptr = static_cast<uint8_t *>(dst);
    uint8_t *sptr = static_cast<uint8_t *>(src);

    uint8_t *wptr = dptr;

    for (int i = 0; i < src_width - 1; ++i) {
        *wptr++ = *sptr;
        *wptr++ = g_paletteInterpolationTable[*(uint16_t *)sptr];
        ++sptr;
    }

    *wptr++ = *sptr;
    *wptr = 0;
}

static void Interpolate_Y_Axis(void *top_line, void *bottom_line, void *middle_line, int src_width)
{
    uint8_t *tlp = static_cast<uint8_t *>(top_line);
    uint8_t *mlp = static_cast<uint8_t *>(middle_line);
    uint8_t *blp = static_cast<uint8_t *>(bottom_line);
    int dst_width = 2 * src_width;

    for (int i = 0; i < dst_width; ++i) {
        uint16_t tmp = *tlp++;
        tmp |= (*blp++) << 8;
        *mlp++ = g_paletteInterpolationTable[tmp];
    }
}

/**
 * @brief Interpolates in both the X and Y axis.
 */
void __cdecl Interpolate_Line_Interpolate(void *src, void *dst, int src_height, int src_width, int dst_pitch)
{
    uint8_t *dptr = static_cast<uint8_t *>(dst);
    uint8_t *buff_offset1 = g_interpolationBuffer + 2560;
    uint8_t *buff_offset2 = g_interpolationBuffer + 5120;

    int pitch = dst_pitch / 2;
    Interpolate_X_Axis(src, buff_offset1, src_width);

    Swap(buff_offset1, buff_offset2);

    uint8_t *current_line = static_cast<uint8_t *>(src) + src_width;
    int lines = src_height - 1;

    while (lines--) {
        Interpolate_X_Axis(current_line, buff_offset1, src_width);
        Interpolate_Y_Axis(buff_offset2, buff_offset1, g_interpolationBuffer, src_width);
        memcpy(dptr, buff_offset2, 2 * src_width);
        dptr += pitch;
        memcpy(dptr, g_interpolationBuffer, 2 * src_width);
        current_line += src_width;
        dptr += pitch;
        Swap(buff_offset1, buff_offset2);
    }

    memcpy(dptr, buff_offset2, 2 * src_width);
    dptr += pitch;
    // memcpy(dptr, buff_offset1, 2 * src_width);
}
