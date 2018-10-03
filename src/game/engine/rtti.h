/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief ID Values for the various types of objects derived from AbstractClass or AbstractTypeClass.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef RTTI_H
#define RTTI_H

#include "always.h"

enum RTTIType
{
    RTTI_INVALID = -1,
    RTTI_NONE,
    RTTI_AIRCRAFT,
    RTTI_AIRCRAFTTYPE,
    RTTI_ANIM,
    RTTI_ANIMTYPE,
    RTTI_BUILDING,
    RTTI_BUILDINGTYPE,
    RTTI_BULLET,
    RTTI_BULLETTYPE,
    RTTI_CELL,
    RTTI_FACTORY,
    RTTI_HOUSE,
    RTTI_HOUSETYPE,
    RTTI_INFANTRY,
    RTTI_INFANTRYTYPE,
    RTTI_OVERLAY,
    RTTI_OVERLAYTYPE,
    RTTI_SMUDGE,
    RTTI_SMUDGETYPE,
    RTTI_SPECIAL, // super weapon cameos
    RTTI_TEAM,
    RTTI_TEAMTYPE,
    RTTI_TEMPLATE,
    RTTI_TEMPLATETYPE,
    RTTI_TERRAIN,
    RTTI_TERRAINTYPE,
    RTTI_TRIGGER,
    RTTI_TRIGGERTYPE,
    RTTI_UNIT,
    RTTI_UNITTYPE,
    RTTI_VESSEL,
    RTTI_VESSELTYPE,
    // RTTI_WEAPONTYPE = 32, //Added these two, as this allows us to have WeaponTypeClass and WarheadTypeClass derive from
    // Abstract classes.
    // RTTI_WARHEADTYPE = 33,
    // RTTI_SUPERWEAPON = 34, //Added this for SuperWeaponClass
    // RTTI_SUPERWEAPONTYPE = 35, //Added this for the new SuperWeaponTypeClass
    RTTI_COUNT,
};

DEFINE_ENUMERATION_OPERATORS(RTTIType)

struct RTTITypeStruct
{
    const char *Name;
    RTTIType ID;
    const char *Description;
};

extern RTTITypeStruct RTTIName[RTTI_COUNT];

RTTIType RTTI_From_Name(const char *name);
const char *Name_From_RTTI(RTTIType rtti);
const char *Description_From_RTTI(RTTIType rtti);

#endif // RTTI_H
