/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Low level functions for loading and rendering C&C terrain tilesets.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "tileset.h"
#include "endiantype.h"

static int IconEntry;
static void *IconData;
static IconControlType *LastIconset;
static uint8_t *StampPtr;
static uint8_t *TransFlagPtr;
static uint8_t *MapPtr;
static int IconWidth;
static int IconHeight;
static int IconSize;
static int IconCount;

void __cdecl Init_Stamps(IconControlType *iconset)
{
    if (iconset && LastIconset != iconset) {
        IconCount = le16toh(iconset->count);
        IconWidth = le16toh(iconset->width);
        IconHeight = le16toh(iconset->height);
        LastIconset = iconset;
        IconSize = IconWidth * IconHeight;

        // TD and RA tileset headers are slightly different, so check a constant that only exists in one type.
        if (le32toh(iconset->td.icons) == TD_TILESET_CHECK) {
            MapPtr = reinterpret_cast<uint8_t*>(iconset) + le32toh(iconset->td.map);
            StampPtr = reinterpret_cast<uint8_t*>(iconset) + le32toh(iconset->td.icons);
            TransFlagPtr = reinterpret_cast<uint8_t*>(iconset) + le32toh(iconset->td.trans_flag);
        } else {
            MapPtr = reinterpret_cast<uint8_t*>(iconset) + le32toh(iconset->ra.map);
            StampPtr = reinterpret_cast<uint8_t*>(iconset) + le32toh(iconset->ra.icons);
            TransFlagPtr = reinterpret_cast<uint8_t*>(iconset) + le32toh(iconset->ra.trans_flag);
        }
    }
}

void __cdecl Buffer_Draw_Stamp(
    GraphicViewPortClass &viewport, IconControlType *tileset, int icon, int x, int y, const void *remapper)
{
    if (!tileset) {
        return;
    }

    if (LastIconset != tileset) {
        Init_Stamps(tileset);
    }

    int32_t icon_index = MapPtr != nullptr ? MapPtr[icon] : icon;

    if (icon_index < IconCount) {

        int32_t fullpitch = viewport.Get_Full_Pitch(); //(viewport.Get_Pitch() + viewport.Get_XAdd() + viewport.Get_Width());
        uint8_t *dst = x + y * fullpitch + static_cast<uint8_t *>(viewport.Get_Offset());
        int32_t blitpitch = fullpitch - IconWidth;
        uint8_t *src = &StampPtr[IconSize * icon_index];

        if (remapper) {
            const uint8_t *remap = static_cast<const uint8_t *>(remapper);
            for (int i = 0; i < IconHeight; ++i) {
                for (int j = 0; j < IconWidth; ++j) {
                    uint8_t cur_byte = remap[*src++];

                    if (cur_byte) {
                        *dst = cur_byte;
                    }

                    ++dst;
                }

                dst += blitpitch;
            }

        } else if (TransFlagPtr[icon_index]) {
            for (int i = 0; i < IconHeight; ++i) {
                for (int j = 0; j < IconWidth; ++j) {
                    uint8_t cur_byte = *src++;

                    if (cur_byte) {
                        *dst = cur_byte;
                    }

                    ++dst;
                }

                dst += blitpitch;
            }
        } else {
            for (int32_t i = 0; i < IconHeight; ++i) {
                memcpy(dst, src, IconWidth);
                dst += fullpitch;
                src += IconWidth;
            }
        }
    }
}
void __cdecl Buffer_Draw_Stamp_Clip(GraphicViewPortClass &viewport, IconControlType *tileset, int icon, int x, int y,
    const void *remapper, int left, int top, int right, int bottom)
{
    if (!tileset) {
        return;
    }

    if (LastIconset != tileset) {
        Init_Stamps(tileset);
    }

    int icon_index = MapPtr != nullptr ? MapPtr[icon] : icon;

    if (icon_index < IconCount) {
        int blit_height = IconHeight;
        int blit_width = IconWidth;
        uint8_t *src = &StampPtr[IconSize * icon_index];
        int width = left + right;
        int xstart = left + x;
        int height = top + bottom;
        int ystart = top + y;

        if (xstart < width && ystart < height && IconHeight + ystart > top && IconWidth + xstart > left) {
            if (xstart < left) {
                src += left - xstart;
                blit_width -= left - xstart;
                xstart = left;
            }

            int src_pitch = IconWidth - blit_width;

            if (blit_width + xstart > width) {
                src_pitch += blit_width - (width - xstart);
                blit_width = width - xstart;
            }

            if (top > ystart) {
                blit_height = IconHeight - (top - ystart);
                src += IconWidth * (top - ystart);
                ystart = top;
            }

            if (blit_height + ystart > height) {
                blit_height = height - ystart;
            }

            int full_pitch = viewport.Get_Full_Pitch(); //(viewport.Get_Pitch() + viewport.Get_XAdd() + viewport.Get_Width());
            uint8_t *dst = xstart + ystart * full_pitch + static_cast<uint8_t *>(viewport.Get_Offset());
            int dst_pitch = full_pitch - blit_width;

            if (remapper) {
                const uint8_t *remap = static_cast<const uint8_t *>(remapper);
                for (int i = 0; i < blit_height; ++i) {
                    for (int j = 0; j < blit_width; ++j) {
                        uint8_t cur_byte = remap[*src++];
                        if (cur_byte) {
                            *dst = cur_byte;
                        }

                        ++dst;
                    }
                    dst += dst_pitch;
                }

            } else if (TransFlagPtr[icon_index]) {
                for (int i = 0; i < blit_height; ++i) {
                    for (int j = 0; j < blit_width; ++j) {
                        uint8_t cur_byte = *src++;
                        if (cur_byte) {
                            *dst = cur_byte;
                        }

                        ++dst;
                    }
                    src += src_pitch;
                    dst += dst_pitch;
                }

            } else {
                for (int i = 0; i < blit_height; ++i) {
                    memcpy(dst, src, blit_width);
                    dst += full_pitch;
                    src += IconWidth;
                }
            }
        }
    }
}

uint8_t *Get_Icon_Set_Map(void *temp)
{
    if (temp != nullptr) {
        if (le32toh(static_cast<IconControlType*>(temp)->td.icons) == TD_TILESET_CHECK) {
            return static_cast<uint8_t *>(temp) + le32toh(static_cast<IconControlType*>(temp)->td.icons);
        } else {
            return static_cast<uint8_t *>(temp) + le32toh(static_cast<IconControlType*>(temp)->ra.icons);
        }
    }

    return nullptr;
}
