/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
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

// forward decs
class FileClass;

enum FuseResultType
{
    FUSE_0 = 0,
    FUSE_1 = 1,
    FUSE_2 = 2,
};

class FuseClass
{
public:
    FuseClass() : Duration(0), ArmTimer(0), Position(0), ArmDistance(0) {}

    void Arm_Fuse(uint32_t pos, uint32_t arm_pos, int duration, int arm_time);
    int Fuse_Checkup(uint32_t a1);

    void Code_Pointers() {}
    void Decode_Pointers() {}

    void Fuse_Write(FileClass &file);
    void Fuse_Read(FileClass &file);

private:
    uint8_t Duration;
    uint8_t ArmTimer;
    uint32_t Position;
    int16_t ArmDistance;
};

#endif // FUSE_H
