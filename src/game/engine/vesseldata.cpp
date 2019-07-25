/**
* @file
*
* @author CCHyper
* @author OmniBlade
*
* @brief Static instances of vessel type objects.
*
* @copyright Chronoshift is free software: you can redistribute it and/or
*            modify it under the terms of the GNU General Public License
*            as published by the Free Software Foundation, either version
*            2 of the License, or (at your option) any later version.
*            A full copy of the GNU General Public License can be found in
*            LICENSE
*/
#include "vesseldata.h"
#include "facing.h"

/**
* For reference, the constructor argument order is;
*
* Type (VesselType)
* UI Name
* Name
* Death Anim (AnimType)
* A5
* A6
* A7
* A8
* A9
* A10
* Nominal
* Has Turret
* ROT Count
* MissionType
* 
*/

const VesselTypeClass VesselSubmarine(
    VESSEL_SUBMARINE, TXT_SS, "SS", ANIM_FBALL1, 0, 0, 0, 0, 0, false, true, false, FACING_COUNT_8, MISSION_HUNT);

const VesselTypeClass VesselDestroyer(
    VESSEL_DESTROYER, TXT_DD, "DD", ANIM_FBALL1, 0, 0, 0, 0, 0, false, true, true, FACING_COUNT_8, MISSION_HUNT);

const VesselTypeClass VesselCruiser(
    VESSEL_CRUISER, TXT_CA, "CA", ANIM_FBALL1, 0, 0, 0, 0, 0, false, true, true, FACING_COUNT_8, MISSION_HUNT);

const VesselTypeClass VesselTransport(
    VESSEL_TRANSPORT, TXT_LST, "LST", ANIM_FBALL1, 0, 0, 0, 0, 0, false, true, false, FACING_COUNT_NONE, MISSION_SLEEP);

const VesselTypeClass VesselPTBoat(
    VESSEL_PT_BOAT, TXT_PT, "PT", ANIM_FBALL1, 0, 0, 0, 0, 0, false, true, true, FACING_COUNT_8, MISSION_HUNT);

const VesselTypeClass VesselMissileSubmarine(
    VESSEL_MISSILE_SUB, TXT_MS, "MSUB", ANIM_FBALL1, 0, 0, 0, 0, 0, false, true, false, FACING_COUNT_8, MISSION_HUNT);

const VesselTypeClass VesselHeliCarrier(
    VESSEL_CARRIER, TXT_CARR, "CARR", ANIM_FBALL1, 0, 0, 0, 0, 0, false, true, false, FACING_COUNT_NONE, MISSION_SLEEP);
