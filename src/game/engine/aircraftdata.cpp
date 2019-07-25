/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Static instances of aircraft type objects.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "aircraftdata.h"

/**
 * For reference, the constructor argument order is;
 *
 * Type (AircraftType)
 * UI Name
 * Name
 * A4
 * A5
 * A6
 * Airplane
 * Rotors
 * Transport
 * Custom Rotors
 * A11
 * A12
 * A13
 * A14
 * A15
 * Dock (BuildingType)
 * Landing Distance
 * A18
 * MissionType
 * 
 */

const AircraftTypeClass AircraftBadgerPlane(AIRCRAFT_BADGER, TXT_BADGER, "BADR", 0, 0, 0, true, false, false, false, true,
    false, true, false, false, BUILDING_NONE, 255, FACING_COUNT_16, MISSION_UNLOAD);

const AircraftTypeClass AircraftU2Plane(AIRCRAFT_U2, TXT_U2, "U2", 0, 0, 0, true, false, false, false, true, false, true,
    false, false, BUILDING_NONE, 255, FACING_COUNT_16, MISSION_UNLOAD);

const AircraftTypeClass AircraftMigPlane(AIRCRAFT_MIG, TXT_MIG, "MIG", 0, 32, 32, true, false, false, false, true, true,
    true, false, false, BUILDING_AIRFIELD, 192, FACING_COUNT_16, MISSION_UNLOAD);

const AircraftTypeClass AircraftYakPlane(AIRCRAFT_YAK, TXT_YAK, "YAK", 0, 32, 32, true, false, false, false, true, true,
    true, false, false, BUILDING_AIRFIELD, 255, FACING_COUNT_16, MISSION_UNLOAD);

const AircraftTypeClass AircraftTransportHeli(AIRCRAFT_TRANSPORT, TXT_TRANS, "TRAN", 0, 0, 0, false, true, true, true, true,
    true, true, false, false, BUILDING_NONE, 255, FACING_COUNT_32, MISSION_UNLOAD);

const AircraftTypeClass AircraftAttackHeli(AIRCRAFT_HELI, TXT_HELI, "HELI", 0, 64, 0, false, true, false, false, true, true,
    true, false, false, BUILDING_HELIPAD, 255, FACING_COUNT_32, MISSION_UNLOAD);

const AircraftTypeClass AircraftHindHeli(AIRCRAFT_HIND, TXT_HIND, "HIND", 0, 64, 0, false, true, false, false, true, true,
    true, false, false, BUILDING_HELIPAD, 255, FACING_COUNT_32, MISSION_UNLOAD);
