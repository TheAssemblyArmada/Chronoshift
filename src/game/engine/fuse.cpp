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
#include "fuse.h"
#include "basefile.h"
#include "coord.h"

/**
 * @brief Sets the fuse countdown running.
 */
void FuseClass::Arm_Fuse(coord_t pos, coord_t arm_pos, int duration, int arm_time)
{
    if (duration <= arm_time) {
        duration = arm_time;
    }

    if (duration >= 255) {
        duration = 255;
    }

    m_Duration = duration;

    if (arm_time >= 255) {
        arm_time = 255;
    }

    m_ArmTimer = arm_time;
    m_Position = arm_pos;
    m_ArmDistance = Distance(pos, arm_pos);
}

/**
 * @brief Checks the state of the fuse.
 */
int FuseClass::Fuse_Checkup(coord_t pos)
{
    if (m_Duration > 0) {
        --m_Duration;
    }

    if (m_ArmTimer > 0) {
        --m_ArmTimer;
    } else {
        if (m_Duration == 0) {
            return FUSE_1;
        }

        int dist = Distance(pos, m_Position);

        if (dist < 16) {
            return FUSE_1;
        }

        if (dist < 256 && dist > m_ArmDistance) {
            return FUSE_1;
        }

        m_ArmDistance = dist;
    }

    return FUSE_0;
}

/**
 * @brief Writes the fuse state to a FileClass instance.
 */
void FuseClass::Fuse_Write(FileClass &file)
{
    file.Write(&m_Duration, sizeof(m_Duration));
    file.Write(&m_ArmTimer, sizeof(m_ArmTimer));
    file.Write(&m_Position, sizeof(m_Position));
    file.Write(&m_ArmDistance, sizeof(m_ArmDistance));
}

/**
 * @brief Reads the fuse state from a FileClass instance.
 */
void FuseClass::Fuse_Read(FileClass &file)
{
    file.Read(&m_Duration, sizeof(m_Duration));
    file.Read(&m_ArmTimer, sizeof(m_ArmTimer));
    file.Read(&m_Position, sizeof(m_Position));
    file.Read(&m_ArmDistance, sizeof(m_ArmDistance));
}
