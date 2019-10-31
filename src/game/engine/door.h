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
#include "stage.h"

class NoInitClass;

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
    DoorClass() : m_DoorTimer(), m_Stage(0), m_State(DOOR_CLOSED), m_ToRedraw(false) {}
    DoorClass(const NoInitClass &noinit) : m_DoorTimer(noinit) {}

    void AI();
    BOOL Open_Door(int delay, int stages);
    BOOL Close_Door(int delay, int stages);
    int Door_Stage() const;
    void Mark_To_Redraw() const { m_ToRedraw = true; }
    BOOL To_Redraw() const { return m_ToRedraw; }
    void Clear_To_Redraw() const { m_ToRedraw = false; }
    BOOL Is_Closed() const { return m_State == DOOR_CLOSED; }
    BOOL Is_Open() const { return m_State == DOOR_OPEN; }
    BOOL Is_Opening() const { return m_State == DOOR_OPENING; }
    BOOL Is_Closing() const { return m_State == DOOR_CLOSING; }

#ifdef GAME_DLL
    int Hook_Door_Stage() { return DoorClass::Door_Stage(); }
#endif

private:
    StageClass m_DoorTimer;
    int8_t m_Stage; // Number of stages between end states (open or closed).
    DoorState m_State; // Current processing state.
#ifndef CHRONOSHIFT_NO_BITFIELDS
    mutable BOOL m_ToRedraw : 1;
#else
    mutable bool m_ToRedraw;
#endif
};

#endif // DOOR_H
