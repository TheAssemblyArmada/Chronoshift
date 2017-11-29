/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class providing a fuse function for weapons that need some time to arm.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "fuse.h"
#include "coord.h"
#include "fileclass.h"

/**
 * @brief Sets the fuse countdown running.
 */
void FuseClass::Arm_Fuse(uint32_t pos, uint32_t arm_pos, int duration, int arm_time)
{
    if (duration <= arm_time) {
        duration = arm_time;
    }

    if (duration >= 255) {
        duration = 255;
    }

    Duration = duration;

    if (arm_time >= 255) {
        arm_time = 255;
    }

    ArmTimer = arm_time;
    Position = arm_pos;
    ArmDistance = Distance(pos, arm_pos);
}

/**
 * @brief Checks the state of the fuse.
 */
int FuseClass::Fuse_Checkup(uint32_t pos)
{
    if (Duration > 0) {
        --Duration;
    }

    if (ArmTimer > 0) {
        --ArmTimer;
    } else {
        if (Duration == 0) {
            return FUSE_1;
        }

        int dist = Distance(pos, Position);

        if (dist < 16) {
            return FUSE_1;
        }

        if (dist < 256 && dist > ArmDistance) {
            return FUSE_2;
        }

        ArmDistance = dist;
    }

    return FUSE_0;
}

/**
 * @brief Writes the fuse state to a FileClass instance.
 */
void FuseClass::Fuse_Write(FileClass &file)
{
    file.Write(&Duration, sizeof(Duration));
    file.Write(&ArmTimer, sizeof(ArmTimer));
    file.Write(&Position, sizeof(Position));
    file.Write(&ArmDistance, sizeof(ArmDistance));
}

/**
 * @brief Reads the fuse state from a FileClass instance.
 */
void FuseClass::Fuse_Read(FileClass &file)
{
    file.Read(&Duration, sizeof(Duration));
    file.Read(&ArmTimer, sizeof(ArmTimer));
    file.Read(&Position, sizeof(Position));
    file.Read(&ArmDistance, sizeof(ArmDistance));
}
