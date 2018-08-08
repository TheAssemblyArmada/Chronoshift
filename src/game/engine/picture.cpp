/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Implements a cps format picture loader.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */ 
#include "picture.h"
#include "ccfileclass.h"
#include "endiantype.h"
#include "lcw.h"
#include "palette.h"

int32_t Load_Uncompress(FileClass &file, BufferClass &uncomp_buff, BufferClass &dest_buff, void *palette)
{
    int16_t bytesremaining;
    PictureHeader header;
    int16_t palettesize;
    uint8_t tmpbuff[(320 * 240) + sizeof(PictureHeader)];
    bool opened = false; // File opened by this function?
    uint8_t *inbuffer = uncomp_buff.Get_Buffer();
    uint8_t *outbuffer = uncomp_buff.Get_Buffer();

    if (!file.Is_Open()) {
        if (!file.Open()) {
            return 0;
        }
        opened = true;
    }

    file.Read(&header, sizeof(header));
    bytesremaining = le16toh(header.Filesize);
    bytesremaining -= sizeof(header) - sizeof(header.Filesize);
    palettesize = le16toh(header.PaletteSize);

    // test for palette?
    if (palettesize > 0) {
        bytesremaining -= palettesize;

        if (palette != nullptr) {
            // DEBUG_LOG("Pal size %d, remaining %d\n", palettesize, bytesremaining);
            file.Read(palette, palettesize);
        } else {
            file.Seek(palettesize, FS_SEEK_CURRENT);
        }

        palettesize = 0;
    }

    if (uncomp_buff.Get_Buffer() == dest_buff.Get_Buffer()) {
        inbuffer = tmpbuff;
    }

    memmove(inbuffer, &header, sizeof(PictureHeader));
    file.Read(inbuffer + sizeof(PictureHeader), bytesremaining);

    bytesremaining = Uncompress_Data(inbuffer, outbuffer);

    if (opened) {
        file.Close();
    }

    return bytesremaining;
}

uint32_t Uncompress_Data(void const *src, void *dst)
{
    if (src != nullptr && dst != nullptr) {
        uint32_t uncomp_size = le16toh(static_cast<const PictureHeader *>(src)->UncompressedSize);
        CompressionType method = (CompressionType)le16toh((int16_t)static_cast<const PictureHeader *>(src)->CompMode);
        uint8_t const *datap = static_cast<uint8_t const *>(src) + sizeof(PictureHeader);

        switch (method) {
            default:
            case COMP_NOCOMPRESS:
                memcpy(dst, datap, uncomp_size);
                break;

            case COMP_LZW12: // These 3 are possible formats for other games but unused in C&C games.
            case COMP_LZW14:
            case COMP_HORIZONTAL:
                break;

            case COMP_LCW:
                LCW_Uncomp(datap, dst, uncomp_size);
                break;
        }

        return uncomp_size;
    }

    return 0;
}

int Load_Picture(FileClass &file, BufferClass &buff1, BufferClass &buff2, void *palette, PicturePlaneType a5)
{
    return Load_Uncompress(file, buff1, buff2, palette) / 8000;
}

int __cdecl Load_Picture(char const *filename, BufferClass &buff1, BufferClass &buff2, void *palette, PicturePlaneType a5)
{
    CCFileClass file(filename);

    return Load_Uncompress(file, buff1, buff2, palette) / 8000;
}
