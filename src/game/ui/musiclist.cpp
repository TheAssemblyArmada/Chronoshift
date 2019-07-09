/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class specialised for the list of music tracks.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "musiclist.h"
#include "gamefile.h"
#include "gbuffer.h"
#include "mixfile.h"
#include "remap.h"

MusicListClass::MusicListClass(int id, int x, int y, int w, int h, TextPrintType style) :
    ListClass(id, x, y, w, h, style | TPF_6PT_GRAD | TPF_NOSHADOW, GameFileClass::Retrieve("BTN-DN.SHP"),
        GameFileClass::Retrieve("BTN-UP.SHP"))
{
    // empty
}

void MusicListClass::Draw_Entry(int index, int x, int y, int x_max, BOOL selected)
{
    TextPrintType style = TextStyle;
    RemapControlType *remapper = Get_Color_Scheme();
    const char *entry = Entries[index] + 1;

    if (style & TPF_6PT_GRAD) {
        if (selected) {
            style |= TPF_USE_BRIGHT;
            g_logicPage->Fill_Rect(x, y, ((x + x_max) - 1), ((y + YSpacing) - 1), remapper->WindowPalette[0]);
        } else if (!(style & TPF_USE_GRAD_PAL)) {
            style |= TPF_USE_MEDIUM;
        }
    } else {
        remapper = (selected ? &ColorRemaps[REMAP_10] : &ColorRemaps[REMAP_5]);
    }

    Conquer_Clip_Text_Print(entry, x, y, remapper, COLOR_TBLACK, style, x_max, Tabs);
}
