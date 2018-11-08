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
#pragma once

#ifndef DOOR_H
#define DOOR_H

#include "always.h"
#include "noinit.h"
#include "stage.h"

class DoorClass
{
    enum DoorState
    {
        DOOR_CLOSED,
        DOOR_OPENING,
        DOOR_OPEN,
        DOOR_CLOSING,
    };
public:
    DoorClass() : DoorTimer(), Stage(0), State(DOOR_CLOSED), StageComplete(false) {}
    DoorClass(const NoInitClass &noinit) : DoorTimer(noinit) {}

    void AI();
    BOOL Open_Door(int delay, int stages);
    BOOL Close_Door(int delay, int stages);
    int Door_Stage();
    BOOL Get_Stage_Complete() { return StageComplete; }
    void Set_Stage_Compelte(BOOL complete) { StageComplete = complete; }
private:
    StageClass DoorTimer;
    int8_t Stage; // Number of stages between end states (open or closed).
    DoorState State; // Current processing state.
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool StageComplete : 1;
        };
        int Bitfield; // This shouldn't be used in the code.
    };
#else
    bool StageComplete;
#endif
};

#endif // DOOR_H
