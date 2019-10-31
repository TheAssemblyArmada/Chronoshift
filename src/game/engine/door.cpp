/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Handles door states for things like transport units.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "door.h"

/**
 * @brief Carries out per game tick processing on this object.
 */
void DoorClass::AI()
{
    if (m_DoorTimer.Stage_Changed()) {
        if (m_DoorTimer.Get_Stage() >= m_Stage) {
            m_DoorTimer.Set_Delay(0);

            switch (m_State) {
                case DOOR_OPENING:
                    m_State = DOOR_OPEN;
                    break;
                case DOOR_CLOSING:
                    m_State = DOOR_CLOSED;
                    break;
                default:
                    break;
            }
        }

        m_ToRedraw = true;
    }
}

/**
 * @brief Sets the door opening process to take the given stages with the given frame delay between each stage.
 */
BOOL DoorClass::Open_Door(int delay, int stages)
{
    if (m_State != DOOR_CLOSED && m_State != DOOR_CLOSING) {
        return false;
    }

    m_State = DOOR_OPENING;
    m_Stage = stages - 1;
    m_DoorTimer.Set_Stage(0);
    m_DoorTimer.Set_Delay(delay);

    return true;
}

/**
 * @brief Sets the door closing process to take the given stages with the given frame delay between each stage.
 */
BOOL DoorClass::Close_Door(int delay, int stages)
{
    if (m_State != DOOR_OPEN && m_State != DOOR_OPENING) {
        return false;
    }

    m_State = DOOR_CLOSING;
    m_Stage = stages - 1;
    m_DoorTimer.Set_Stage(0);
    m_DoorTimer.Set_Delay(delay);

    return true;
}

/**
 * @brief get the current state of the door.
 */
int DoorClass::Door_Stage() const
{
    switch (m_State) {
        case DOOR_CLOSING:
            return m_Stage - 1 - m_DoorTimer.Get_Stage();
        case DOOR_OPENING:
            return m_DoorTimer.Get_Stage();
        case DOOR_OPEN:
            return m_Stage - 1;
        case DOOR_CLOSED: // fallthrough, returns 0.
        default:
            break;
    }

    return 0;
}
