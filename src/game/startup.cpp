/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Startup code.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "startup.h"
#include <captainslog.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_SYS_STATVFS_H
#include <sys/statvfs.h>
#endif

#ifdef PLATFORM_WINDOWS
#ifdef __WATCOMC__
#include <windows.h>
#else
#include <fileapi.h>
#endif
#endif

void Emergency_Exit(int exit_code)
{
#ifdef GAME_DLL
    captainslog_debug("Calling emergency exit.");
    // TODO Requires GraphicBufferClass, PaletteClass, KeyboardClass
    void (*hooked_exit)(int) = reinterpret_cast<void (*)(int)>(0x00552714);
    hooked_exit(exit_code);
#endif
}

void Read_Startup_Options(FileClass *fc)
{
#ifdef GAME_DLL
    // TODO Requires GraphicBufferClass, PaletteClass, KeyboardClass
    void (*func)(FileClass *) = reinterpret_cast<void (*)(FileClass *)>(0x005528A8);
    func(fc);
#endif
}

/**
 * Gets the amount of disk space available to the current user in the current working directory.
 */
uint64_t Disk_Space_Available()
{
#ifdef PLATFORM_WINDOWS
    ULARGE_INTEGER free_space;

    if (!GetDiskFreeSpaceExA(nullptr, &free_space, nullptr, nullptr)) {
        return 0;
    }

    // Original abi used 32bit values for this.
    return free_space.QuadPart;
#else
    char path[PATH_MAX];
    struct statvfs free_space;

    if (getcwd(path, sizeof(path)) == nullptr) {
        captainslog_dbgassert(false, "Couldn't get current working directory.\n");
        return 0;
    }

    if (statvfs(path, &free_space) != 0) {
        return 0;
    }

    return free_space.f_bsize * free_space.f_bavail;
#endif
}
