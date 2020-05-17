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
#include "fuse.h"
#include "basefile.h"
#include "coord.h"

/**
 * @brief Arms the fuse, sets its target location and distance to it.
 */
void FuseClass::Arm_Fuse(coord_t pos, coord_t target, int duration, int arm_delay)
{
    m_Duration = std::min(std::max(duration, arm_delay), 255);
    m_ArmDelay = std::min(arm_delay, 255);
    m_Target = target;
    m_Proximity = Coord_Distance(pos, target);
}

/**
 * @brief Checks if the fuse should ignite.
 */
FuseResultType FuseClass::Fuse_Checkup(coord_t pos)
{
    if (m_Duration > 0) {
        --m_Duration;
    }

    if (m_ArmDelay > 0) {
        --m_ArmDelay;
        // Has not been armed yet.
        return FUSE_WAIT;
    }

    if (m_Duration == 0) {
        return FUSE_EXPLODE;
    }

    int dist = Coord_Distance(pos, m_Target);

    if (dist < 16) {
        // Is close enough to the ignition position.
        return FUSE_EXPLODE;
    }

    if (dist < 256 && dist > m_Proximity) {
        // This returns FUSE_2 in later games.
        return FUSE_EXPLODE;
    }

    // Still too far from target position so set the new distance.
    m_Proximity = dist;

    return FUSE_WAIT;
}

/**
 * @brief Writes the fuse state to a FileClass instance.
 */
void FuseClass::Fuse_Write(FileClass &file)
{
    file.Write(&m_Duration, sizeof(m_Duration));
    file.Write(&m_ArmDelay, sizeof(m_ArmDelay));
    file.Write(&m_Target, sizeof(m_Target));
    file.Write(&m_Proximity, sizeof(m_Proximity));
}

/**
 * @brief Reads the fuse state from a FileClass instance.
 */
void FuseClass::Fuse_Read(FileClass &file)
{
    file.Read(&m_Duration, sizeof(m_Duration));
    file.Read(&m_ArmDelay, sizeof(m_ArmDelay));
    file.Read(&m_Target, sizeof(m_Target));
    file.Read(&m_Proximity, sizeof(m_Proximity));
}
