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
#include "rtti.h"
#include "gamedebug.h"

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

RTTITypeStruct RTTIName[RTTI_COUNT] = {
    { "<none>", RTTI_NONE, "Invalid object" },
    { "Aircraft", RTTI_AIRCRAFT, "Aircraft object" },
    { "AircraftType", RTTI_AIRCRAFTTYPE, "AircraftType object" },
    { "Anim", RTTI_ANIM, "Animation object" },
    { "AnimType", RTTI_ANIMTYPE, "AnimationType object" },
    { "Building", RTTI_BUILDING, "Building object" },
    { "BuildingType", RTTI_BUILDINGTYPE, "BuildingType object" },
    { "Bullet", RTTI_BULLET, "Bullet object" },
    { "BulletType", RTTI_BULLETTYPE, "BulletType object" },
    { "Cell", RTTI_CELL, "Map Cell object" },
    { "Factory", RTTI_FACTORY, "Factory object" },
    { "House", RTTI_HOUSE, "House object" },
    { "HouseType", RTTI_HOUSETYPE, "HouseType object" },
    { "Infantry", RTTI_INFANTRY, "Infantry object" },
    { "InfantryType", RTTI_INFANTRYTYPE, "InfantryType object" },
    { "Overlay", RTTI_OVERLAY, "Overlay object" },
    { "OverlayType", RTTI_OVERLAYTYPE, "OverlayType object" },
    { "Smudge", RTTI_SMUDGE, "Smudge object" },
    { "SmudgeType", RTTI_SMUDGETYPE, "SmudgeType object" },
    { "Special", RTTI_SPECIAL, "Special object" },
    { "Team", RTTI_TEAM, "Team object" },
    { "TeamType", RTTI_TEAMTYPE, "TeamType object" },
    { "Template", RTTI_TEMPLATE, "Template object" },
    { "TemplateType", RTTI_TEMPLATETYPE, "TemplateType object" },
    { "Terrain", RTTI_TERRAIN, "Terrain object" },
    { "TerrainType", RTTI_TERRAINTYPE, "TerrainType object" },
    { "Trigger", RTTI_TRIGGER, "Trigger object" },
    { "TriggerType", RTTI_TRIGGERTYPE, "TriggerType object" },
    { "Unit", RTTI_UNIT, "Unit object" },
    { "UnitType", RTTI_UNITTYPE, "UnitType object" },
    { "Vessel", RTTI_VESSEL, "Vessel object" },
    { "VesselType", RTTI_VESSELTYPE, "VesselType object" },
};

RTTIType RTTI_From_Name(const char *name)
{
    DEBUG_ASSERT(name != nullptr);

    if (strcasecmp(name, "<none>") == 0 || strcasecmp(name, "none") == 0) {
        return RTTI_NONE;
    }

    if (name != nullptr) {
        for (RTTIType rtti = RTTI_NONE; rtti < RTTI_COUNT; ++rtti) {
            if (strcasecmp(name, RTTIName[rtti].Name) == 0) {
                return rtti;
            }
        }
    }

    return RTTI_NONE;
}

const char *Name_From_RTTI(RTTIType rtti)
{
    if (rtti < RTTI_COUNT) {
        return RTTIName[rtti].Name;
    }

    return "<none>";
}

const char *Description_From_RTTI(RTTIType rtti)
{
    if (rtti < RTTI_COUNT) {
        return RTTIName[rtti].Description;
    }

    return "<none>";
}
