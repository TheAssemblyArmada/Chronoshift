/**
 * @file
 *
 * @author tomsons26
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

#ifndef CARRYOVER_H
#define CARRYOVER_H

#include "always.h"
#include "link.h"
#include "house.h"

class TechnoClass;

class CarryoverClass : public LinkClass
{
public:
    CarryoverClass(TechnoClass *techno);
    ~CarryoverClass() {}

    BOOL Create();

#ifdef GAME_DLL
public:
    CarryoverClass *Hook_Ctor(TechnoClass *techno) { return new(this)CarryoverClass(techno); }
#endif

private:
    RTTIType m_RTTI;
    uint8_t m_Type;
    cell_t m_Cell;
    int16_t m_Health;
    HousesType m_Owner;
};

#ifdef GAME_DLL
extern CarryoverClass *&g_Carryover;
#else
extern CarryoverClass *g_Carryover;
#endif

#endif // CARRYOVER_H
