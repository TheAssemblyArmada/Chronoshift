/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 * @author tomsons26
 *
 * @brief Class providing a fuse function for weapons that need some time to arm.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef FUSE_H
#define FUSE_H

#include "always.h"
#include "gametypes.h"

// forward decs
class NoInitClass;
class FileClass;

enum FuseResultType
{
    FUSE_WAIT = 0,
    FUSE_EXPLODE = 1,
    FUSE_2 = 2,
};

class FuseClass
{
public:
    FuseClass() : m_Duration(0), m_ArmDelay(0), m_Target(0), m_Proximity(0) {}
    FuseClass(const NoInitClass &noinit) {}

    void Arm_Fuse(coord_t pos, coord_t target, int duration, int arm_delay);
    FuseResultType Fuse_Checkup(coord_t pos);

    coord_t Get_Target() const { return m_Target; }

    void Code_Pointers() {}
    void Decode_Pointers() {}

    void Fuse_Write(FileClass &file);
    void Fuse_Read(FileClass &file);

private:
    uint8_t m_Duration; // Total life span of the fuse.
    uint8_t m_ArmDelay; // Delay until fuse is armed.
    coord_t m_Target; // Position where the fuse should ignite.
    int16_t m_Proximity; // Last registered distance to target position.
};

#endif // FUSE_H
