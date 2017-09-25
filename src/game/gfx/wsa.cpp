/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Functions for loading and rendering a WSA file.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "wsa.h"
#include "abs.h"
#include "alloc.h"
#include "blitters.h"
#include "ccfileclass.h"
#include "endiantype.h"
#include "gamedebug.h"
#include "gbuffer.h"
#include "lcw.h"
#include "stringex.h"
#include "xordelta.h"
#include <inttypes.h>

SysAnimHeaderType *__cdecl Open_Animation(char const *filename, void *buffer, int size, char flags, void *pal)
{
    WSAHeaderType fileheader;
    int filehandle;
    int pal_size;
    uint32_t offset_array_size;
    int frame0_size;
    int frame_size;
    int delta_size;
    int file_and_decode_size;
    int decode_size;
    void *frame_ptr;
    void *delta_ptr;
    void *first_frame_ptr;
    int buffer_size;
    size_t header_size;
    int mem_flags = 0;
    bool is_hd = false;

    filehandle = Open_File(filename, FM_READ);

    if (filehandle == -1) {
        DEBUG_LOG("Open_Animation() - Couldn't open file\n");

        return nullptr;
    }

    if (Read_File(filehandle, &fileheader, sizeof(WSAFileHeaderType)) != sizeof(WSAFileHeaderType)) {
        DEBUG_LOG("Open_Animation() - Failed to read header correctly for size %d\n", sizeof(WSAFileHeaderType));

        return nullptr;
    }

    // Byteswap header if needed, not in original code
    fileheader.sd.total_frames = le16toh(fileheader.sd.total_frames);
    fileheader.sd.x_pos = le16toh(fileheader.sd.x_pos);
    fileheader.sd.y_pos = le16toh(fileheader.sd.y_pos);
    fileheader.sd.width = le16toh(fileheader.sd.width);
    fileheader.sd.height = le16toh(fileheader.sd.height);

    // Detect if we have a "HD" version of WSA.
    if (fileheader.sd.width > 320 || fileheader.sd.height > 200) {
        DEBUG_LOG("Opening High Res WSA File '%s'.\n", filename);
        is_hd = true;
        header_size = sizeof(WSAHDFileHeader);
        // Read the missing 2 bytes for the HD header.
        Read_File(filehandle,
            reinterpret_cast<char *>(&fileheader.sd.first_frame_end) + sizeof(fileheader.sd.first_frame_end),
            sizeof(WSAHDFileHeaderType) - sizeof(WSAFileHeaderType));

        fileheader.hd.largest_frame_size = le32toh(fileheader.hd.largest_frame_size);
        fileheader.hd.flags = le16toh(fileheader.hd.flags);
        fileheader.hd.first_frame_start = le32toh(fileheader.hd.first_frame_start);
        fileheader.hd.first_frame_end = le32toh(fileheader.hd.first_frame_end);
        DEBUG_LOG("Total frames: %" PRIu16 "\n"
            "X pos : %" PRIu16 "\n"
            "Y pos : %" PRIu16 "\n"
            "Width : %" PRIu16 "\n"
            "Height : %" PRIu16 "\n"
            "Largest frame : %" PRIu32"\n"
            "Flags : %04" PRIx16 "\n"
            "First frame start : %" PRIu32 "\n"
            "First frame end : %" PRIu32 "\n",
            fileheader.hd.total_frames,
            fileheader.hd.x_pos,
            fileheader.hd.y_pos,
            fileheader.hd.width,
            fileheader.hd.height,
            fileheader.hd.largest_frame_size,
            fileheader.hd.flags,
            fileheader.hd.first_frame_start,
            fileheader.hd.first_frame_end
        );
    } else {
        DEBUG_LOG("Opening Low Res WSA File '%s'.\n", filename);
        header_size = sizeof(WSAFileHeader);
        fileheader.sd.largest_frame_size = le16toh(fileheader.sd.largest_frame_size);
        fileheader.sd.flags = le16toh(fileheader.sd.flags);
        fileheader.sd.first_frame_start = le32toh(fileheader.sd.first_frame_start);
        fileheader.sd.first_frame_end = le32toh(fileheader.sd.first_frame_end);
        DEBUG_LOG("Total frames: %" PRIu16 "\n"
            "X pos : %" PRIu16 "\n"
            "Y pos : %" PRIu16 "\n"
            "Width : %" PRIu16 "\n"
            "Height : %" PRIu16 "\n"
            "Largest frame : %" PRIu16 "\n"
            "Flags : %04" PRIx16 "\n"
            "First frame start : %" PRIu32 "\n"
            "First frame end : %" PRIu32 "\n",
            fileheader.sd.total_frames,
            fileheader.sd.x_pos,
            fileheader.sd.y_pos,
            fileheader.sd.width,
            fileheader.sd.height,
            fileheader.sd.largest_frame_size,
            fileheader.sd.flags,
            fileheader.sd.first_frame_start,
            fileheader.sd.first_frame_end
        );
    }

    // Do we have a palette? If so account for it and read it into provided
    // palette pointer.
    if (is_hd ? (fileheader.hd.flags & 1) != 0 : (fileheader.sd.flags & 1) != 0) {
        mem_flags |= 0x100;
        pal_size = 768;

        if (pal) {
            Seek_File(filehandle, 4 * fileheader.sd.total_frames, FS_SEEK_CURRENT);
            Read_File(filehandle, pal, 0x300);
        }

    } else {
        pal_size = 0;
    }

    // Record flag 2 if we have it into our in memory flags.
    if (is_hd ? (fileheader.hd.flags & 2) != 0 : (fileheader.sd.flags & 2) != 0) {
        mem_flags |= 0x200;
    }

    uint32_t file_size = Seek_File(filehandle, 0, FS_SEEK_END);

    if (is_hd && fileheader.hd.first_frame_start != 0) {
        frame0_size = fileheader.hd.first_frame_end - fileheader.hd.first_frame_start;
    } else if (fileheader.sd.first_frame_start != 0) {
        frame0_size = fileheader.sd.first_frame_end - fileheader.sd.first_frame_start;
    } else {
        mem_flags |= WSA_NO_INIT;
        frame0_size = 0;
    }

    file_size -= frame0_size + pal_size + header_size;

    // something to do with handling the frame size? Decode first frame?
    if (flags & WSA_OPEN_DIRECT) {
        frame_size = 0;
    } else {
        mem_flags |= WSA_DECODE_BUFF;
        frame_size = fileheader.sd.height * fileheader.sd.width;
    }

    // Why plus 6? Looks like WSA header DeltaBufferSize is largest delta +
    // 43 (size of SysAnimHeaderType on 32bit machines) - 6?
    //
    // Added adjustment based on size of SysAnimHeaderType incase of pointer sizes
    // greater than 4 bytes which is what original was hardcoded to expect
    delta_size = is_hd ? fileheader.hd.largest_frame_size : fileheader.sd.largest_frame_size;
    delta_size += 6 + (sizeof(SysAnimHeaderType) - 43);

    decode_size = delta_size + frame_size;
    file_and_decode_size = file_size + delta_size + frame_size;

    // We got passed a buffer that was too small, close and abort.
    if (buffer != nullptr && size < decode_size) {
        Close_File(filehandle);

        return nullptr;
    }

    // parameter flag bit 1 seems to indicate we are only buffering a frame at a
    // time, not the whole file as well.
    if (buffer != nullptr) {
        if (flags & WSA_OPEN_FROM_DISK || size < file_and_decode_size) {
            buffer_size = decode_size;
        } else {
            buffer_size = file_and_decode_size;
        }

        // In memory flag bit one seems to flag that we didn't allocate the buffer
        mem_flags |= WSA_NO_ALLOC;
    } else {
        if (flags & WSA_OPEN_FROM_DISK || (size && size < file_and_decode_size)) {
            // DEBUG_LOG("Preparing for WSA from file\n");
            buffer_size = decode_size;
        } else {
            buffer_size = file_and_decode_size;
        }

        if (Ram_Free() < buffer_size) {
            DEBUG_LOG("Not enough free memory\n");
            if (Ram_Free() < decode_size) {
                Close_File(filehandle);
                return 0;
            }
            buffer_size = decode_size;
        }

        buffer = malloc(buffer_size);
        memset(buffer, 0, buffer_size);

        mem_flags |= WSA_ALLOC;
    }

    SysAnimHeaderType *mem_header = static_cast<SysAnimHeaderType *>(buffer);
    frame_ptr = static_cast<char *>(buffer) + sizeof(SysAnimHeaderType);
    delta_ptr = static_cast<char *>(frame_ptr) + frame_size;

    // Zero out the memory area we are using for the decoded frames.
    // If we allocated this is probably already done so could check and skip?
    if (frame_size != 0) {
        memset(frame_ptr, 0, frame_size);
    }

    mem_header->current_frame = fileheader.sd.total_frames;
    mem_header->total_frames = fileheader.sd.total_frames;
    mem_header->x_pos = fileheader.sd.x_pos;
    mem_header->y_pos = fileheader.sd.y_pos;
    mem_header->width = fileheader.sd.width;
    mem_header->height = fileheader.sd.height;
    mem_header->largest_frame_size = delta_size - sizeof(SysAnimHeaderType);
    mem_header->anim_mem_size = buffer_size;
    mem_header->delta_buffer = delta_ptr; // Pointer to end of frame buffer

    // copy the filename into the buffer header
    strlcpy(mem_header->file_name, filename, sizeof(mem_header->file_name));

    offset_array_size = 4 * (fileheader.sd.total_frames + 2);

    // If we have enough space, read the whole file
    if (buffer_size == file_and_decode_size) {
        DEBUG_LOG("Open_Animation() - Caching file to memory\n");
        mem_header->file_buffer = static_cast<char *>(delta_ptr) + mem_header->largest_frame_size;

        // This gets the entire set of offsets so needs to use sizeof the file header.
        Seek_File(filehandle, sizeof(WSAFileHeader), FS_SEEK_START);
        Read_File(filehandle, mem_header->file_buffer, offset_array_size);
        Seek_File(filehandle, pal_size + frame0_size, FS_SEEK_CURRENT);
        Read_File(
            filehandle, offset_array_size + static_cast<char *>(mem_header->file_buffer), file_size - offset_array_size);

        // Check if we have a looping animation
        if (Get_Resident_Frame_Offset(mem_header->file_buffer, mem_header->total_frames + 1) != 0) {
            mem_flags |= WSA_CACHED;
        } else {
            mem_flags |= WSA_CACHED | WSA_NOLOOP;
        }
    } else {
        DEBUG_LOG("Open_Animation() - Handling from file\n");
        if (Get_File_Frame_Offset(filehandle, mem_header->total_frames + 1, pal_size)) {
            mem_flags |= WSA_FROMFILE;
        } else {
            mem_flags |= WSA_FROMFILE | WSA_NOLOOP;
        }

        mem_header->file_buffer = nullptr;
    }

    // This sticks the first LCW data at the end of the delta buffer we decompress into?
    first_frame_ptr = static_cast<char *>(delta_ptr) + mem_header->largest_frame_size - frame0_size;

    Seek_File(filehandle, pal_size + offset_array_size + sizeof(WSAFileHeader), FS_SEEK_START);

    Read_File(filehandle, first_frame_ptr, frame0_size);

    if (mem_flags & WSA_CACHED) {
        mem_header->file_handle = -1;
        Close_File(filehandle);
    } else {
        mem_header->file_handle = filehandle;
    }

    LCW_Uncomp(first_frame_ptr, delta_ptr, mem_header->largest_frame_size);
    mem_header->flags = mem_flags;

    return static_cast<SysAnimHeaderType *>(buffer);
}

void __cdecl Close_Animation(SysAnimHeaderType *header)
{
    if (header != nullptr) {
        if (header->flags & WSA_FROMFILE) {
            // File handle must be stored in wsa buffer if Flag bit 3 is set.
            Close_File(header->file_handle);
        }

        if (header->flags & WSA_ALLOC) {
            free(header);
        }
    }
}

BOOL __cdecl Animate_Frame(SysAnimHeaderType *header, GraphicViewPortClass &viewport, int frame, int x_pos, int y_pos)
{
    DEBUG_ASSERT(header != nullptr);
    DEBUG_ASSERT(frame < header->total_frames);

    if (header != nullptr && frame < header->total_frames) {
        if (viewport.Lock()) {
            int pitch = viewport.Get_Pitch() + viewport.Get_Width() + viewport.Get_XAdd();
            int x_posa = header->x_pos + x_pos;
            int y_posa = header->y_pos + y_pos;

            bool xor_to_vp;
            void *frame_buff;

            if (header->flags & WSA_DECODE_BUFF) {
                // void *header = static_cast<void *>(header);
                frame_buff = reinterpret_cast<char *>(header) + sizeof(SysAnimHeaderType);
                xor_to_vp = false;
            } else {
                frame_buff = x_posa + pitch * y_posa + static_cast<char *>(viewport.Get_Offset());
                xor_to_vp = true;
            }

            // We are handling the first frame if current and total frames are equal
            if (header->current_frame == header->total_frames) {
                //DEBUG_LOG("Animate_Frame() - Handing initial frame\n");
                if (!(header->flags & WSA_NO_INIT)) {
                    //DEBUG_LOG("Animate_Frame() - Anim_Open staged initial frame, applying now\n");
                    if (xor_to_vp) {
                        Apply_XOR_Delta_To_Page_Or_Viewport(
                            frame_buff, header->delta_buffer, header->width, pitch, (header->flags & WSA_ALLOC) == 0);
                    } else {
                        Apply_XOR_Delta(frame_buff, header->delta_buffer);
                    }
                }
                header->current_frame = 0;
            }

            int cur_frame = header->current_frame;
            int frame_diff = Abs(cur_frame - frame);
            int direction = 1;

            // This replaces the goto stuff, I think the logic is correct
            int to_apply = header->total_frames - cur_frame + frame;

            if (to_apply >= frame_diff || header->flags & WSA_NOLOOP) {
                to_apply = frame_diff;
                if (frame <= cur_frame) {
                    direction = -1;
                }
            } else if (frame > cur_frame) {
                direction = -1;
            }

            DEBUG_LOG(
                "Animate_Frame() - Frame step is %d, to apply is %d, current frame is %d\n", direction, to_apply, cur_frame);

            if (direction <= 0) {
                for (int i = 0; i < to_apply; ++i) {
                    if (!cur_frame) {
                        cur_frame = header->total_frames;
                    }

                    DEBUG_LOG("Animate_Frame() - Framestep is <=0, applying frame %d\n", cur_frame);
                    Apply_Delta(header, cur_frame, frame_buff, pitch);
                    cur_frame += direction;
                }
            } else {
                for (int i = 0; i < to_apply; ++i) {
                    DEBUG_LOG("Animate_Frame() - Framestep is > 0, applying frame %d\n", cur_frame + direction);
                    Apply_Delta(header, direction + cur_frame, frame_buff, pitch);

                    if (cur_frame == header->total_frames) {
                        cur_frame = 0;
                    }
                }
            }

            header->current_frame = frame;

            if (header->flags & WSA_DECODE_BUFF) {
                Buffer_To_Page(x_posa, y_posa, header->width, header->height, frame_buff, viewport);
            }

            viewport.Unlock();

            return true;
        }
    }

    return false;
}

int Apply_Delta(SysAnimHeaderType *header, int curr_frame, void *buffer, int pitch)
{
    int pal_offset = 0;

    if (header->flags & 0x100) {
        pal_offset = 768;
    }

    void *lcw_buff = header->delta_buffer;
    uint32_t frame_data_size;
    int32_t frame_offset;

    // Read from Ram if cached, else get a frame from memory
    if (header->flags & WSA_CACHED) {
        // DEBUG_LOG("Applying delta for WSA_CACHED\n");

        frame_offset = Get_Resident_Frame_Offset(header->file_buffer, curr_frame);

        frame_data_size = Get_Resident_Frame_Offset(header->file_buffer, curr_frame + 1) - frame_offset;

        void *frame_data = static_cast<char *>(header->file_buffer) + frame_offset;
        lcw_buff = static_cast<char *>(lcw_buff) + header->largest_frame_size - frame_data_size;

        memcpy(lcw_buff, frame_data, frame_data_size);
    } else if (header->flags & WSA_FROMFILE) {
        // DEBUG_LOG("Applying delta for WSA_FROMFILE\n");
        int file_handle = header->file_handle;

        Seek_File(file_handle, 0, FS_SEEK_START);
        frame_offset = Get_File_Frame_Offset(file_handle, curr_frame, pal_offset);
        frame_data_size = Get_File_Frame_Offset(file_handle, curr_frame + 1, pal_offset) - frame_offset;

        if (!frame_offset || !frame_data_size) {
            return 0;
        }

        Seek_File(file_handle, frame_offset, FS_SEEK_START);
        lcw_buff = static_cast<char *>(lcw_buff) + header->largest_frame_size - frame_data_size;

        if (Read_File(file_handle, lcw_buff, frame_data_size) != frame_data_size) {
            return 0;
        }
    }

    LCW_Uncomp(lcw_buff, header->delta_buffer, header->largest_frame_size);

    if (header->flags & WSA_DECODE_BUFF) {
        Apply_XOR_Delta(buffer, header->delta_buffer);
    } else {
        Apply_XOR_Delta_To_Page_Or_Viewport(buffer, header->delta_buffer, header->width, pitch, 0);
    }

    return 1;
}

unsigned __cdecl Get_Animation_Size(SysAnimHeaderType *header)
{
    if (header != nullptr) {
        return header->anim_mem_size;
    }

    return 0;
}

unsigned Get_Resident_Frame_Offset(void *anim, int frame)
{
    DEBUG_LOG("Getting resident frame offsets.\n");
    uint32_t *offsets = static_cast<uint32_t *>(anim);
    int first_frame_size = 0;

    if (offsets[0]) {
        first_frame_size = offsets[1] - offsets[0];
    }

    if (offsets[frame]) {
        return offsets[frame] - (first_frame_size + 14);
    }

    return 0;
}

unsigned Get_File_Frame_Offset(int handle, int frame, int palette_adjust)
{
    DEBUG_LOG("Getting file frame offsets.\n");
    uint32_t offset;
    Seek_File(handle, 4 * frame + sizeof(SysAnimHeaderType), FS_SEEK_START);

    if (Read_File(handle, &offset, sizeof(uint32_t)) != sizeof(uint32_t)) {
        offset = 0;
    }

    offset += palette_adjust;

    return offset;
}

int __cdecl Get_Animation_Frame_Count(SysAnimHeaderType *header)
{
    if (header != nullptr) {
        return header->total_frames;
    }

    return 0;
}

int Get_Animation_X(SysAnimHeaderType *header)
{
    if (header != nullptr) {
        return header->x_pos;
    }

    return 0;
}

int Get_Animation_Y(SysAnimHeaderType *header)
{
    if (header != nullptr) {
        return header->y_pos;
    }

    return 0;
}

int Get_Animation_Width(SysAnimHeaderType *header)
{
    if (header != nullptr) {
        return header->width;
    }

    return 0;
}

int Get_Animation_Height(SysAnimHeaderType *header)
{
    if (header != nullptr) {
        return header->height;
    }

    return 0;
}

int Get_Animation_Palette(SysAnimHeaderType *header)
{
    if (header != nullptr) {
        return header->flags & 0x100;
    }

    return 0;
}
