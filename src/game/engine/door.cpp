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
    if (DoorTimer.Stage_Changed()) {
        if (DoorTimer.Get_Stage() >= Stage) {
            DoorTimer.Set_Delay(0);

            switch (State) {
                case DOOR_OPENING:
                    State = DOOR_OPEN;
                    break;
                case DOOR_CLOSING:
                    State = DOOR_CLOSED;
                    break;
                default:
                    break;
            }
        }

        ToRedraw = true;
    }
}

/**
 * @brief Sets the door opening process to take the given stages with the given frame delay between each stage.
 */
BOOL DoorClass::Open_Door(int delay, int stages)
{
    if (State != DOOR_CLOSED && State != DOOR_CLOSING) {
        return false;
    }

    State = DOOR_OPENING;
    Stage = stages - 1;
    DoorTimer.Set_Stage(0);
    DoorTimer.Set_Delay(delay);

    return true;
}

/**
 * @brief Sets the door closing process to take the given stages with the given frame delay between each stage.
 */
BOOL DoorClass::Close_Door(int delay, int stages)
{
    if (State != DOOR_OPEN && State != DOOR_OPENING) {
        return false;
    }

    State = DOOR_CLOSING;
    Stage = stages - 1;
    DoorTimer.Set_Stage(0);
    DoorTimer.Set_Delay(delay);

    return true;
}

/**
 * @brief get the current state of the door.
 */
int DoorClass::Door_Stage() const
{
    switch (State) {
        case DOOR_CLOSING:
            return Stage - 1 - DoorTimer.Get_Stage();
        case DOOR_OPENING:
            return DoorTimer.Get_Stage();
        case DOOR_OPEN:
            return Stage - 1;
        case DOOR_CLOSED: // fallthrough, returns 0.
        default:
            break;
    }

    return 0;
}
