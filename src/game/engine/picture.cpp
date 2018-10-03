/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Implements a cps format picture loader.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
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

/**
 * Load cps formatted image data from a FileClass object.
 *
 * 0x004FB60C
 */
int32_t Load_Uncompress(FileClass &file, BufferClass &uncomp_buff, BufferClass &dest_buff, void *palette)
{
    int16_t bytesremaining;
    PictureHeader header;
    //uint8_t tmpbuff[(320 * 240) + sizeof(PictureHeader)];
    bool opened = false; // File opened by this function?
    uint8_t *inbuffer = uncomp_buff.Get_Buffer();
    uint8_t *outbuffer = dest_buff.Get_Buffer();

    if (!file.Is_Open()) {
        if (!file.Open()) {
            return 0;
        }
        opened = true;
    }

    file.Read(&bytesremaining, sizeof(bytesremaining));
    file.Read(&header, sizeof(header));
    bytesremaining = le16toh(bytesremaining);
    bytesremaining -= sizeof(header);
    header.PaletteSize = le16toh(header.PaletteSize);

    // test for palette?
    if (header.PaletteSize > 0) {
        bytesremaining -= header.PaletteSize;

        if (palette != nullptr) {
            // DEBUG_LOG("Pal size %d, remaining %d\n", palettesize, bytesremaining);
            file.Read(palette, header.PaletteSize);
        } else {
            file.Seek(header.PaletteSize, FS_SEEK_CURRENT);
        }

        header.PaletteSize = 0;
    }

    if (uncomp_buff.Get_Buffer() == dest_buff.Get_Buffer()) {
        inbuffer = &inbuffer[uncomp_buff.Get_Size() - sizeof(PictureHeader)] - bytesremaining;
    }

    memmove(inbuffer, &header, sizeof(PictureHeader));
    file.Read(inbuffer + sizeof(PictureHeader), bytesremaining);

    bytesremaining = Uncompress_Data(inbuffer, outbuffer);

    if (opened) {
        file.Close();
    }

    return bytesremaining;
}

/**
 * Decompress cps formatted data after any palette has been removed.
 *
 * 0x005CF6B0
 */
uint32_t Uncompress_Data(const void *src, void *dst)
{
    if (src != nullptr && dst != nullptr) {
        const PictureHeader *head = static_cast<const PictureHeader *>(src);
        uint32_t uncomp_size = le16toh(head->UncompressedSize);
        CompressionType method = (CompressionType)le16toh((int16_t)head->CompMode);
        uint8_t const *datap = static_cast<uint8_t const *>(src) + sizeof(PictureHeader) + head->PaletteSize;

        switch (method) {
            default:
            case COMP_NOCOMPRESS:
            case COMP_LZW12: // These 3 are possible formats for other games but unused in C&C games.
            case COMP_LZW14:
            case COMP_HORIZONTAL:
                memcpy(dst, datap, uncomp_size);
                break;

            case COMP_LCW:
                LCW_Uncomp(datap, dst, uncomp_size);
                break;
        }

        return uncomp_size;
    }

    return 0;
}

/**
 * Load a cps file from a FileClass instance.
 */
int Load_Picture(FileClass &file, BufferClass &buff1, BufferClass &buff2, void *palette, PicturePlaneType plane)
{
    return Load_Uncompress(file, buff1, buff2, palette) / 8000;
}

/**
 * Load a cps file from a file name.
 *
 * 0x004FB724
 */
int __cdecl Load_Picture(const char *filename, BufferClass &buff1, BufferClass &buff2, void *palette, PicturePlaneType plane)
{
    CCFileClass file(filename);

    return Load_Uncompress(file, buff1, buff2, palette) / 8000;
}
