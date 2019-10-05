/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief 
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef TEVENT_H
#define TEVENT_H

#include "always.h"
#include "gameptr.h"
#include "teamtype.h"

enum TEventType
{
    TEVENT_NO_EVENT, // "-No Event-"
    TEVENT_ENTERED_BY, // "Entered by..."
    TEVENT_SPIED_BY, // "Spied by..."
    TEVENT_THIEVED_BY, // "Thieved by..."
    TEVENT_DISCOVERED_BY_PLAYER, // "Discovered by player"
    TEVENT_HOUSE_DISCOVERED, // "House Discovered..."
    TEVENT_ATTACKED, // "Attacked by anybody"
    TEVENT_DESTROYED, // "Destroyed by anybody"
    TEVENT_ANY, // "Any Event"
    TEVENT_DESTORYED_ALL_UNITS, // "Destroyed, Units, All..."
    TEVENT_DESTORYED_ALL_BUILDINGS, // "Destroyed, Buildings, All..."
    TEVENT_DESTROYED_ALL, // "Destroyed, All..."
    TEVENT_CREDIT_EXCEED, // "Credits exceed (x100)..."
    TEVENT_ELAPSED_TIME, // "Elapsed Time (1/10th min)..."
    TEVENT_TIMER_EXPIRED, // "Mission Timer Expired"
    TEVENT_15, // "Destroyed, Buildings, #..."
    TEVENT_16, // "Destroyed, Units, #..."
    TEVENT_NO_FACTORIES, // "No Factories left"
    TEVENT_18, // "Civilians Evacuated"
    TEVENT_BUILD_BUILDING, // "Build Building Type..."
    TEVENT_BUILD_UNIT, // "Build Unit Type..."
    TEVENT_BUILD_INFANTRY, // "Build Infantry Type..."
    TEVENT_BUILD_AIRCRAFT, // "Build Aircraft Type..."
    TEVENT_23, // "Leaves map (team)..."
    TEVENT_24, // "Zone Entry by..."
    TEVENT_25, // "Crosses Horizontal Line..."
    TEVENT_26, // "Crosses Vertical Line..."
    TEVENT_GLOBAL_SET, // "Global is set..."
    TEVENT_GLOBAL_CLEAR, // "Global is clear..."
    TEVENT_29, // "Destroyed, Fakes, All..."
    TEVENT_LOW_POWER, // "Low Power..."
    TEVENT_ATTACHED_BRIDGE_DESTROYED, // "All bridges destroyed"
    TEVENT_BUILDING_EXISTS, // "Building exists..."

    TEVENT_FIRST = TEVENT_NO_EVENT,
    TEVENT_COUNT
};

DEFINE_ENUMERATION_OPERATORS(TEventType);

class TEventClass
{
public:
    TEventClass() : m_Type(TEVENT_NO_EVENT), m_TeamType(nullptr), m_Value2(0) {}
    ~TEventClass() { m_TeamType = nullptr; }

    void Code_Pointers() {}
    void Decode_Pointers() {}

    TEventType What_Type() const { return m_Type; }

protected:
    TEventType m_Type;
    GamePtr<TeamTypeClass> m_TeamType;
    int m_Value2; // 
};

#endif // TEVENT_H
