/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Handles the processing for a flashing object.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef FLASHER_H
#define FLASHER_H

#include "always.h"

class NoInitClass;

class FlasherClass
{
public:
    FlasherClass() : Frames(0), Flashed(false) {}
    FlasherClass(const FlasherClass &that) : Frames(that.Frames), Flashed(that.Flashed) {}
    FlasherClass(const NoInitClass &noinit) {}

    void Flash(int flash_frames, bool flash);
    BOOL Get_Flashed() { return Flashed; }
    BOOL Process();
    void Code_Pointers() {}
    void Decode_Pointers() {}

#ifndef CHRONOSHIFT_STANDALONE
    static void Hook_Me();
#endif

private:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    unsigned int Frames : 7;
    BOOL Flashed : 1;
#else
    unsigned int Frames; // Number of frames to flash for.
    bool Flashed; // Is current frame a flash frame?
#endif
};

inline void FlasherClass::Flash(int flash_frames, bool flash)
{
    Frames = flash_frames;
    Flashed = flash;
}

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"

inline void FlasherClass::Hook_Me()
{
#ifdef COMPILER_WATCOM
    Hook_Function(0x004C0690, *FlasherClass::Process);
#endif
}
#endif

#endif // FLASHER_H
