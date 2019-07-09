/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief PCX file readers and writers.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "pcx.h"
#include "endiantype.h"
#include "filepipe.h"
#include "filestraw.h"
#include "gamedebug.h"
#include "gamefile.h"
#include "gbuffer.h"
#include "palette.h"
#include "pcxpipe.h"
#include "pcxrle.h"
#include "pcxstraw.h"
#include <algorithm>

/**
 * Read a PCX file directly into a graphic buffer.
 *
 * 0x005B3E30
 */
GraphicBufferClass *Read_PCX_File(const char *filename, PaletteClass *pal, void *buffer, int size)
{
    PCX_HEADER header;
    GraphicBufferClass *gbuff;

    GameFileClass file(filename);

    if (file.Is_Available()) {
        file.Open(FM_READ);
        file.Read(&header, sizeof(PCX_HEADER));
        if (header.Identifier != PCX_ZSOFT && header.Version != PCX_VER30 && header.BitsPixelPlane != 8) {
            DEBUG_LOG("PCX does not have expected header.\n");
            file.Close();
            return nullptr;
        } else {
            int width = le16toh(header.XEnd) - le16toh(header.XStart) + 1;
            int height = le16toh(header.YEnd) - le16toh(header.YStart) + 1;

            if (buffer) {
                height = std::min(size / width - 1, height);
                gbuff = new GraphicBufferClass(width, height, buffer, size);
                if (!gbuff || !gbuff->Get_GBuffer()->Get_Buffer()) {
                    DEBUG_LOG("GraphicBuffer for PCX is not valid.\n");
                    file.Close();
                    return nullptr;
                }
            } else {
                gbuff = new GraphicBufferClass(width, height, 0, width * (height + 4));
                if (!gbuff || !gbuff->Get_GBuffer()->Get_Buffer()) {
                    DEBUG_LOG("Allocated GraphicBuffer for PCX is not valid.\n");
                    file.Close();
                    return nullptr;
                }
            }
            uint8_t *gbufferp = static_cast<uint8_t *>(gbuff->Get_GBuffer()->Get_Buffer());
            FileStraw fstraw(&file);
            PCXStraw rstraw(STRAW_DECODE, header.BytesLinePlane, width);
            rstraw.Get_From(&fstraw);
            rstraw.Get(gbufferp, width * height);

            // Do we have a palette buffer provided?
            if (pal) {
                file.Seek(-((long)sizeof(PaletteClass)), FS_SEEK_END);
                file.Read(pal, sizeof(PaletteClass));
                uint8_t *entry = reinterpret_cast<uint8_t *>(pal);

                // Shifts palette to 6bit format used by 1st gen C&C games.
                for (int i = 0; i < PaletteClass::PALETTE_ENTRIES; ++i) {
                    entry[i * 3] = entry[i * 3] >> 2;
                    entry[i * 3 + 1] = entry[i * 3 + 1] >> 2;
                    entry[i * 3 + 2] = entry[i * 3 + 2] >> 2;
                }
            }

            file.Close();

            return gbuff;
        }
    } else {
        DEBUG_LOG("File '%s' wasn't available\n", filename);

        return nullptr;
    }
}

/**
 * Writes the contents of a graphic buffer to a file.
 *
 * 0x005B4500
 */
int Write_PCX_File(FileClass &file, GraphicBufferClass &gbuff, PaletteClass &pal)
{
    PCX_HEADER header; // the PCX file header
    PaletteClass tmppal; // temp palette for writing the input palette to the PCX file.
    bool file_opened = false;

    // Create a null header for our PCX file.
    memset(&header, 0, sizeof(PCX_HEADER));
    header.Identifier = (uint8_t)PCX_ZSOFT;
    header.Version = (uint8_t)PCX_VER30;
    header.Encoding = (uint8_t)PCX_ENC_RLE;
    header.BitsPixelPlane = (uint8_t)PCX_BPP_256COLOR;
    header.XEnd = htole16(gbuff.Get_Width() - 1);
    header.YEnd = htole16(gbuff.Get_Height() - 1);
    header.HRes = htole16(gbuff.Get_Width());
    header.VRes = htole16(gbuff.Get_Height());
    header.NumberOfPlanes = 1;
    header.BytesLinePlane = htole16(gbuff.Get_Width());
    header.PaletteInfo = htole16((uint16_t)PCX_PAL_INFO_COLOR);
    
    // If the file is not open already, open with write access.
    if (!file.Is_Open()) {
        file_opened = true;
        file.Open(FM_WRITE);
    }

    // Set up the pipe system to write the RLE data.
    FilePipe fpipe(&file);
    PCXPipe rpipe(PIPE_ENCODE, gbuff.Get_Width());
    rpipe.Put_To(&fpipe);

    // Write a temp header to the file.

    DEBUG_LOG("Write_PCX_File() writing empty header.\n");
    int data_put = file.Write(&header, sizeof(PCX_HEADER));

    // is this loop checking if the Xmax can be (*&header.Xmax >> 16) divided by 16?
    int pitch = gbuff.Get_Full_Pitch();
    // Not sure we need this Get_VideoBuffer->Get_Buffer stuff, can't we just lock and get offset for the same end?
    char *bitmap_data = (char *)gbuff.Get_GBuffer()->Get_Buffer() + pitch * gbuff.Get_YPos() + gbuff.Get_XPos();

    DEBUG_LOG("Write_PCX_File() writing scan lines.\n");
    for (int i = 0; i < gbuff.Get_Height(); ++i) {
        DEBUG_LOG("\tscan line %d of %d", i, gbuff.Get_Height());
        data_put += rpipe.Put(bitmap_data, gbuff.Get_Width());
        bitmap_data += pitch;
    }

    // Mark end of RLE data.
    uint8_t pal_marker = 0x0C;
    data_put += file.Write(&pal_marker, 1);

    // Fill tmppal with the input palette.
    DEBUG_LOG("Write_PCX_File() preparing to write image palette.\n");
    for (int i = 0; i < 256; ++i) {
        DEBUG_LOG("\tpreparing entry %d of %d\n", i, 256);
        tmppal[i].Set_Red(pal[i].Get_Red() << 2);
        tmppal[i].Set_Green(pal[i].Get_Green() << 2);
        tmppal[i].Set_Blue(pal[i].Get_Blue() << 2);
    }

    // Write the tmppal palette to the file.
    DEBUG_LOG("Write_PCX_File() writing image palette.\n");
    file.Write(&tmppal, sizeof(PaletteClass));

    // If we opened the file for writing, lets be safe and close it.
    if (file_opened) {
        file.Close();
    }

    return 0;
}
