/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Functions for building the color lookup tables to simulate transparency.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "fading.h"
#include "gamedebug.h"
#include "rgb.h"

void *Make_Fading_Table(const PaletteClass &palette, void *fade_table, int color, int frac)
{
    if (fade_table) {
        RGBClass const *target_col = &palette[color];
        uint8_t *dst = static_cast<uint8_t *>(fade_table);

        for (int i = 0; i < 256; ++i) {
            RGBClass tmp = palette[i];
            tmp.Adjust(frac, *target_col);
            dst[i] = palette.Closest_Color(tmp);
        }
    }

    return fade_table;
}

void *Conquer_Build_Fading_Table(const PaletteClass &palette, void *fade_table, int color, int frac)
{
    if (fade_table) {
        uint8_t *dst = static_cast<uint8_t *>(fade_table);
        RGBClass const *target_col = &palette[color];

        for (int i = 0; i < 256; ++i) {
            if (i <= 240 && i) {
                RGBClass tmp = palette[i];
                tmp.Adjust(frac, *target_col);

                int index = 0;
                int prevdiff = -1;

                for (int j = 240; j < 255; ++j) {
                    int difference = palette[j].Difference(tmp);

                    if (prevdiff == -1 || difference < prevdiff) {
                        index = j;
                        prevdiff = difference;
                    }
                }

                dst[i] = index;
            } else {
                dst[i] = i;
            }
        }
    }

    return fade_table;
}

void *__cdecl Build_Fading_Table(const PaletteClass &palette, void *fade_table, int color, int frac)
{
    if (fade_table == nullptr) {
        return nullptr;
    }

    uint8_t *table = static_cast<uint8_t *>(fade_table);
    uint8_t const *pal = reinterpret_cast<const uint8_t *>(&palette);

    // First entry always maps to itself for transparency
    *table++ = 0;

    for (int i = 1; i < 256; i++) {
        // Create adjusted versions of the palette entries based on reference
        // colour and fraction to adjust by.
        uint8_t red = pal[3 * i + 0] - (((pal[3 * i + 0] - pal[3 * color + 0]) * (frac / 2)) >> 7);
        uint8_t grn = pal[3 * i + 1] - (((pal[3 * i + 1] - pal[3 * color + 1]) * (frac / 2)) >> 7);
        uint8_t blu = pal[3 * i + 2] - (((pal[3 * i + 2] - pal[3 * color + 2]) * (frac / 2)) >> 7);
        uint8_t closest = color;
        unsigned min_diff = 0xFFFFFFFF;

        // For each palette entry calculate difference from adjusted version of
        // target color and find closest colour to use from palette.
        for (int j = 1; j < 256; ++j) {
            if (j == i) {
                continue;
            }

            unsigned diff = 0;

            diff += (pal[3 * j + 0] - red) * (pal[3 * j + 0] - red);
            diff += (pal[3 * j + 1] - grn) * (pal[3 * j + 1] - grn);
            diff += (pal[3 * j + 2] - blu) * (pal[3 * j + 2] - blu);

            if (!diff) {
                closest = j;
                break;
            }

            if (diff <= min_diff) {
                min_diff = diff;
                closest = j;
            }
        }

        *table++ = closest;
    }

    return fade_table;
}

void *Build_Translucent_Table(PaletteClass &palette, const TLucentType *type, int tab_count, void *table)
{
    uint8_t *dst = static_cast<uint8_t *>(table);

    if (tab_count && type /*&& palette*/) {
        if (!table) {
            dst = new uint8_t[Translucent_Table_Size(tab_count)];
        }

        if (tab_count > 0) {
            memset(dst, 0xFF, 256);
            uint8_t *ftable = dst + 256;

            for (int i = 0; i < tab_count; ++i) {
                *(dst + type->PaletteIndex) = i;
                Build_Fading_Table(palette, ftable, type->FadeColor, type->FadeFraction);
                ++type;
                ftable += 256;
            }
        }
    }

    return dst;
}

void *Conquer_Build_Translucent_Table(PaletteClass &palette, const TLucentType *type, int tab_count, void *table)
{
    uint8_t *dst = static_cast<uint8_t *>(table);

    if (tab_count && type /*&& palette*/) {
        if (!table) {
            dst = new uint8_t[Translucent_Table_Size(tab_count)];
        }

        if (tab_count > 0) {
            memset(dst, 0xFF, 256);
            uint8_t *ftable = dst + 256;
            for (int i = 0; i < tab_count; ++i) {
                *(dst + type->PaletteIndex) = i;
                Conquer_Build_Fading_Table(palette, ftable, type->FadeColor, type->FadeFraction);
                ++type;
                ftable += 256;
            }
        }
    }

    return dst;
}
