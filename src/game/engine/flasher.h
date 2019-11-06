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
    FlasherClass() : m_Frames(0), m_Flashed(false) {}
    FlasherClass(const FlasherClass &that) : m_Frames(that.m_Frames), m_Flashed(that.m_Flashed) {}
    FlasherClass(const NoInitClass &noinit) {}

    void Flash(int flash_frames, bool flash);
    BOOL Get_Flashed() const { return m_Flashed; }
    BOOL Process();
    void Code_Pointers() {}
    void Decode_Pointers() {}

private:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    unsigned int m_Frames : 7;
    BOOL m_Flashed : 1;
#else
    unsigned int m_Frames; // Number of frames to flash for.
    bool m_Flashed; // Is current frame a flash frame?
#endif
};

inline void FlasherClass::Flash(int flash_frames, bool flash)
{
    m_Frames = flash_frames;
    m_Flashed = flash;
}

#endif // FLASHER_H
