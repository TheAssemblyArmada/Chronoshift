/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief <todo>
 *
 * @copyright RedAlert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef VESSELTYPE_H
#define VESSELTYPE_H

#include "always.h"
#include "technotype.h"

enum VesselType
{
    VESSEL_NONE = -1,
    VESSEL_FIRST = 0,
    VESSEL_SUBMARINE = 0,
    VESSEL_DESTROYER = 1,
    VESSEL_CRUISER = 2,
    VESSEL_TRANSPORT = 3,
    VESSEL_PT_BOAT = 4,
    VESSEL_MISSILE_SUB = 5,
    VESSEL_CARRIER = 6,
    VESSEL_LAST = 6,
    VESSEL_COUNT
};

DEFINE_ENUMERATION_OPERATORS(VesselType);
DEFINE_ENUMERATION_BITWISE_OPERATORS(VesselType);

class VesselTypeClass : public TechnoTypeClass
{
};

#ifndef RAPP_STANDALONE
#include "hooker.h"
//extern TFixedIHeapClass<VesselTypeClass> &VesselTypes;
#else
//extern TFixedIHeapClass<VesselTypeClass> VesselTypes;
#endif

#endif // VESSELTYPE_H
