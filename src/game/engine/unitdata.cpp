/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Static instances of unit type objects.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "unitdata.h"
#include "facing.h"

/**
 * For reference, the constructor argument order is;
 *
 * Type (UnitType)
 * UI Name
 * Name
 * Death Anim (AnimType)
 * Remap (RemapType)
 * A6
 * A7
 * A8
 * A9
 * A10
 * Crate Goodie
 * Nominal
 * Crusher
 * Harvester
 * Radar Invisible
 * Insignificant
 * Turret
 * Turret Spins
 * A19
 * A20
 * Large Image
 * Viceroid
 * Radar Jammer
 * Mobile Gap Gen
 * ROT Count
 * MissionType
 * MissionType
 * 
 */

const UnitTypeClass UnitV2Launcher(UNIT_V2RL, TXT_V2RL, "V2RL", ANIM_FRAG1, REMAP_1, 0, 0, 0, 0, 0, true, false, true, false,
    false, false, false, false, false, false, true, false, false, false, FACING_COUNT_32, MISSION_SLEEP, MISSION_HUNT);

const UnitTypeClass UnitLTank(UNIT_LTANK, TXT_LTANK, "1TNK", ANIM_FRAG1, REMAP_1, 32, 192, 0, 0, 0, true, false, true, false,
    false, false, true, false, false, false, false, false, false, false, FACING_COUNT_32, MISSION_SLEEP, MISSION_HUNT);

const UnitTypeClass UnitMTank(UNIT_MTANK, TXT_HTANK, "3TNK", ANIM_FRAG1, REMAP_1, 64, 128, 24, 128, 24, true, false, true,
    false, false, false, true, false, false, false, true, false, false, false, FACING_COUNT_32, MISSION_SLEEP, MISSION_HUNT);

const UnitTypeClass UnitMTank2(UNIT_MTANK2, TXT_MTANK, "2TNK", ANIM_FRAG1, REMAP_1, 48, 192, 0, 192, 0, true, false, true,
    false, false, false, true, false, false, false, true, false, false, false, FACING_COUNT_32, MISSION_SLEEP, MISSION_HUNT);

const UnitTypeClass UnitHTank(UNIT_HTANK, TXT_4TANK, "4TNK", ANIM_ART_EXP1, REMAP_1, 32, 192, 40, 8, 64, true, false, true,
    false, false, false, true, false, false, false, true, false, false, false, FACING_COUNT_32, MISSION_SLEEP, MISSION_HUNT);

const UnitTypeClass UnitMRJammer(UNIT_MRJAMMER, TXT_MRJ, "MRJ", ANIM_FRAG1, REMAP_1, 0, 0, 0, 0, 0, false, false, true,
    false, true, false, false, true, false, false, false, false, true, false, FACING_COUNT_32, MISSION_SLEEP, MISSION_HUNT);

const UnitTypeClass UnitMGG(UNIT_MGAPGEN, TXT_MGG, "MGG", ANIM_FRAG1, REMAP_1, 0, 0, 0, 0, 0, false, false, true, false,
    false, false, false, true, false, false, true, false, false, true, FACING_COUNT_32, MISSION_SLEEP, MISSION_HUNT);

const UnitTypeClass UnitArty(UNIT_ARTY, TXT_ARTY, "ARTY", ANIM_ART_EXP1, REMAP_1, 64, 96, 0, 0, 0, true, false, false, false,
    false, false, false, false, false, false, false, false, false, false, FACING_COUNT_32, MISSION_SLEEP, MISSION_HUNT);

const UnitTypeClass UnitHarvester(UNIT_HARVESTER, TXT_HARVESTER, "HARV", ANIM_FBALL1, REMAP_2, 0, 0, 0, 0, 0, true, true,
    true, true, false, false, false, false, false, false, true, false, false, false, FACING_COUNT_32, MISSION_SLEEP,
    MISSION_HARVEST);

const UnitTypeClass UnitMCV(UNIT_MCV, TXT_MCV, "MCV", ANIM_FBALL1, REMAP_2, 0, 0, 0, 0, 0, true, false, true, false, false,
    false, false, false, false, false, true, false, false, false, FACING_COUNT_32, MISSION_SLEEP, MISSION_HUNT);

const UnitTypeClass UnitJeep(UNIT_JEEP, TXT_JEEP, "JEEP", ANIM_FRAG1, REMAP_1, 48, 48, 0, 48, 0, true, false, false, false,
    false, false, true, false, false, false, false, false, false, false, FACING_COUNT_32, MISSION_SLEEP, MISSION_HUNT);

const UnitTypeClass UnitAPC(UNIT_APC, TXT_APC, "APC", ANIM_FRAG1, REMAP_1, 48, 48, 0, 48, 0, true, false, true, false, false,
    false, false, false, false, false, false, false, false, false, FACING_COUNT_32, MISSION_SLEEP, MISSION_HUNT);

const UnitTypeClass UnitMineLayer(UNIT_MINELAYER, TXT_MNLY, "MNLY", ANIM_FRAG1, REMAP_1, 0, 0, 0, 0, 0, true, false, true,
    false, false, false, false, false, false, false, false, false, false, false, FACING_COUNT_32, MISSION_SLEEP,
    MISSION_HUNT);

const UnitTypeClass UnitConvoyTruck(UNIT_CARGO_TRUCK, TXT_TRUCK, "TRUK", ANIM_FRAG1, REMAP_1, 0, 0, 0, 0, 0, false, false,
    false, false, false, false, false, false, false, false, false, false, false, false, FACING_COUNT_32, MISSION_SLEEP,
    MISSION_GUARD);

const UnitTypeClass UnitAnt1(UNIT_ANT1, TXT_NULL, "ANT1", ANIM_ANTDEATH, REMAP_1, 0, 0, 0, 0, 0, false, true, false, false,
    false, true, false, false, false, false, true, false, false, false, FACING_COUNT_8, MISSION_SLEEP, MISSION_HUNT);

const UnitTypeClass UnitAnt2(UNIT_ANT2, TXT_NULL, "ANT2", ANIM_ANTDEATH, REMAP_1, 0, 0, 0, 0, 0, false, true, false, false,
    false, true, false, false, false, false, true, false, false, false, FACING_COUNT_8, MISSION_SLEEP, MISSION_HUNT);

const UnitTypeClass UnitAnt3(UNIT_ANT3, TXT_NULL, "ANT3", ANIM_ANTDEATH, REMAP_1, 0, 0, 0, 0, 0, false, true, false, false,
    false, true, false, false, false, false, true, false, false, false, FACING_COUNT_8, MISSION_SLEEP, MISSION_HUNT);

const UnitTypeClass UnitChrono(UNIT_CHRONO, TXT_CTNK, "CTNK", ANIM_FRAG1, REMAP_1, 0, 0, 0, 0, 0, false, false, true, false,
    false, false, false, false, false, false, true, false, false, false, FACING_COUNT_32, MISSION_SLEEP, MISSION_HUNT);

const UnitTypeClass UnitTesla(UNIT_TESLA, TXT_TTNK, "TTNK", ANIM_FRAG1, REMAP_1, 0, 0, 0, 0, 0, false, false, true, false,
    true, false, false, true, false, false, true, false, true, false, FACING_COUNT_32, MISSION_SLEEP, MISSION_HUNT);

const UnitTypeClass UnitMAD(UNIT_MAD_TANK, TXT_QTNK, "QTNK", ANIM_FRAG1, REMAP_1, 0, 0, 0, 0, 0, false, false, true, false,
    false, false, false, false, false, false, true, false, false, false, FACING_COUNT_32, MISSION_SLEEP, MISSION_HUNT);

const UnitTypeClass UnitDemoTruck(UNIT_DEMO_TRUCK, TXT_DTRK, "DTRK", ANIM_FRAG1, REMAP_1, 0, 0, 0, 0, 0, false, false, false,
    false, false, false, false, false, false, false, false, false, false, false, FACING_COUNT_32, MISSION_SLEEP,
    MISSION_GUARD);

const UnitTypeClass UnitPhase(UNIT_PHASE, TXT_STNK, "STNK", ANIM_FRAG1, REMAP_1, 48, 48, 0, 48, 0, false, false, true, false,
    false, false, true, false, false, false, true, false, false, false, FACING_COUNT_32, MISSION_SLEEP, MISSION_HUNT);
