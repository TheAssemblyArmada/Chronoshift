/**
* @file
*
* @author CCHyper
* @author OmniBlade
*
* @brief Static instances of building type objects.
*
* @copyright Chronoshift is free software: you can redistribute it and/or
*            modify it under the terms of the GNU General Public License
*            as published by the Free Software Foundation, either version
*            2 of the License, or (at your option) any later version.
*            A full copy of the GNU General Public License can be found in
*            LICENSE
*/
#include "buildingdata.h"
#include "lists.h"

/**
 * For reference, the constructor argument order is;
 *
 * Type (BuildingType)
 * UI Name
 * Name
 * Facing (FacingType)
 * ExitCoord (coord_t)
 * Remap (RemapType)
 * Primary F
 * Primary L
 * Primary H
 * Fake
 * Normalized
 * Nominal
 * Wall
 * SimpleDamage
 * RadarInvisible
 * StartFacing (DirType)
 * Size (BSizeType)
 * ExitList
 * OccupyList
 * OverlapList 
 * 
 */

const BuildingTypeClass BuildingBarrel(BUILDING_BARREL, TXT_BARREL, "BARL", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, false,
    true, false, true, true, true, true, true, false, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingBarrel3(BUILDING_BRL3, TXT_BARREL, "BRL3", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, false, true, false,
    true, true, false, true, true, false, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingAVMine(BUILDING_MINV, TXT_AVM, "MINV", FACING_NONE, 0, REMAP_0, 0, 0, 0, false, false, false, false, true,
    true, false, false, true, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingAPMine(BUILDING_MINP, TXT_APM, "MINP", FACING_NONE, 0, REMAP_0, 0, 0, 0, false, false, false, false, true,
    true, false, false, true, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingIronCurtain(BUILDING_IRON_CURTAIN, TXT_IRON_CURTAIN, "IRON", FACING_SOUTH, 0, REMAP_2, 0, 0, 0,
    false, true, false, false, true, false, true, true, false, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_2BY2, nullptr, List0011, OList1100);

const BuildingTypeClass BuildingForwardCom(BUILDING_FCOM, TXT_FCOM, "FCOM", FACING_SOUTH, 0, REMAP_2, 0, 0, 0, false, true, false,
    false, true, false, true, true, false, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_2BY2, nullptr, List0011, OList1100);

const BuildingTypeClass BuildingAdvancedTech(BUILDING_ATEK, TXT_ALLIED_TECH_CENTER, "ATEK", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, false,
    false, true, false, true, true, false, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_2BY2, nullptr, List1111, nullptr);

const BuildingTypeClass BuildingChronosphere(BUILDING_PDOX, TXT_CHRONOSPHERE, "PDOX", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, false,
    false, true, false, true, true, false, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_2BY2, nullptr, List1111, nullptr);

const BuildingTypeClass BuildingWeapon(BUILDING_WEAP, TXT_WEAPON_FACTORY, "WEAP", FACING_NONE, 16777600, REMAP_2, 0, 0, 0, false, false,
    false, false, false, false, true, true, false, false, false, true, RTTI_UNITTYPE, DIR_NORTH, BSIZE_3BY2, ExitWeap, ListWeap, OListWeap);

const BuildingTypeClass BuildingShipYard(BUILDING_SYRD, TXT_NAVAL_YARD, "SYRD", FACING_NONE, 25166187, REMAP_2, 0, 0, 0, false, false,
    false, false, false, false, true, true, false, false, false, true, RTTI_VESSELTYPE, DIR_NORTH, BSIZE_3BY3, nullptr, ListSPen, OListSPen);

const BuildingTypeClass BuildingSubPen(BUILDING_SPEN, TXT_SUB_PEN, "SPEN", FACING_NONE, 25166187, REMAP_2, 0, 0, 0, false, false,
    false, false, false, false, true, true, false, false, false, true, RTTI_VESSELTYPE, DIR_NORTH, BSIZE_3BY3, ExitSub, ListSPen, OListSPen);

const BuildingTypeClass BuildingPillbox(BUILDING_PBOX, TXT_PILLBOX, "PBOX", FACING_NONE, 0, REMAP_2, 16, 64, 0, false, false, false, false,
    true, false, true, true, false, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingCamoPillbox(BUILDING_HBOX, TXT_CAMO_PILLBOX, "HBOX", FACING_NONE, 0, REMAP_2, 16, 64, 0, false,
    false, false, false, true, false, true, true, false, true, false, true, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingTesla(BUILDING_TESLA_COIL, TXT_TESLA_COIL, "TSLA", FACING_SOUTH, 0, REMAP_2, 200, 0, 0, false, false,
    false, false, false, false, true, true, false, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_1BY2, nullptr, List0010, OList12);

const BuildingTypeClass BuildingTurret(BUILDING_GUN, TXT_TURRET, "GUN", FACING_NONE, 0, REMAP_2, 48, 128, 0, false, false, false, false,
    false, false, true, true, false, false, true, true, RTTI_NONE, (DirType)48, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingAAGun(BUILDING_AGUN, TXT_AA_GUN, "AGUN", FACING_SOUTH, 0, REMAP_2, 0, 0, 0, false, false, false, false, false,
    false, true,true, false, false, true, true, RTTI_NONE, DIR_NORTH_EAST, BSIZE_1BY2, nullptr, List0010, OList12);

const BuildingTypeClass BuildingFlameTurret(BUILDING_FTUR, TXT_FLAME_TOWER, "FTUR", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, false, false,
    false, true, false, true, true, false, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingConst(BUILDING_FACT, TXT_CONST_YARD, "FACT", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, false, false, false, false,
    false, true, true, false, false, false, true, RTTI_BUILDINGTYPE, DIR_NORTH, BSIZE_3BY3, nullptr, ListFactory, nullptr);

const BuildingTypeClass BuildingFakeConst(BUILDING_FACF, TXT_FAKE_CONST_YARD, "FACF", FACING_NONE, 0, REMAP_2, 0, 0, 0, true, 0, 0, 0,
    false, false, true, true, false, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_3BY3, nullptr, ListFactory, nullptr);

const BuildingTypeClass BuildingFakeWeapon(BUILDING_WEAF, TXT_FAKE_WEAP_FACT, "WEAF", FACING_NONE, 27263211, REMAP_2, 0, 0, 0, true,
    false, false, false, false, false, true, true, false, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_3BY2, ExitWeap, ListWeap, OListWeap);

const BuildingTypeClass BuildingRefinery(BUILDING_PROC, TXT_REFINERY, "PROC", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, false, false, false,
    false, false, true, true, false, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_3BY3, nullptr, List010111100, OList101000011);

const BuildingTypeClass BuildingStorage(BUILDING_SILO, TXT_STORAGE, "SILO", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, false, false, false, true,
    false, true, true, false, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, ListSilo, nullptr);

const BuildingTypeClass BuildingHelipad(BUILDING_HELIPAD, TXT_HELIPAD, "HPAD", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, false, false, false,
    false, false, true, true, false, false, false, true, RTTI_AIRCRAFTTYPE, DIR_NORTH, BSIZE_2BY2, nullptr, List11, nullptr);

const BuildingTypeClass BuildingCommand(BUILDING_DOME, TXT_DOME, "DOME", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, false, false, false,
    false, true, true, false, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_2BY2, nullptr, ListCom, OListCom);

const BuildingTypeClass BuildingGapGenerator(BUILDING_GAP_GENERATOR, TXT_MGG, "GAP", FACING_SOUTH, 0, REMAP_2, 0, 0, 0,
    false, true, false, false, false, false, true, true, false, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_1BY2, nullptr, List0010, List1);

const BuildingTypeClass BuildingSAM(BUILDING_SAM, TXT_SAM, "SAM", FACING_NONE, 0, REMAP_2, 48, 128, 0, false, false, false, false, false, false,
    true, true, false, false, true, true, RTTI_NONE, DIR_NORTH, BSIZE_2BY1, nullptr, List11, OListSAM);

const BuildingTypeClass BuildingMissileSilo(BUILDING_MSLO, TXT_MISSILE_SILO, "MSLO", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, 0,
    false, false, false, true, true, false, true, false, true, RTTI_NONE, DIR_NORTH, BSIZE_2BY1, nullptr, List11, OListSAM);

const BuildingTypeClass BuildingAirStrip(BUILDING_AIRFIELD, TXT_AIRSTRIP, "AFLD", FACING_SOUTH, 0, REMAP_2, 0, 0, 0, false, true, 0,
    false, false, false, true, true, false, false, false, true, RTTI_AIRCRAFTTYPE, DIR_NORTH, BSIZE_3BY2, nullptr, List111111, nullptr);

const BuildingTypeClass BuildingPower(BUILDING_POWR, TXT_POWER, "POWR", FACING_SOUTH, 0, REMAP_2, 0, 0, 0, 0, true, false, false, true,
    false, true, true, false, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_2BY2, nullptr, List1111, OList1100);

const BuildingTypeClass BuildingAdvancedPower(BUILDING_APWR, TXT_ADVANCED_POWER, "APWR", FACING_SOUTH, 0, REMAP_2, 0, 0, 0, false, true,
    false, false, true, false, true, true, false, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_3BY3, nullptr, List000111111, OListTmpl);

const BuildingTypeClass BuildingSovietTech(BUILDING_STEK, TXT_TECH_CENTER, "STEK", FACING_SOUTH, 0, REMAP_2, 0, 0, 0, false, true, 0,
    false, true, false, true, true, false, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_3BY3, nullptr, List000111111, OListTmpl);

const BuildingTypeClass BuildingHospital(BUILDING_HOSPITAL, TXT_HOSPITAL, "HOSP", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, 0,
    false, false, false, true, true, false, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_2BY2, nullptr, List11, nullptr);

const BuildingTypeClass BuildingBioLab(BUILDING_BIO, TXT_BIO_LAB, "BIO", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, 0,
    false, true, true, false, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_2BY2, nullptr, List11, nullptr);

const BuildingTypeClass BuildingBarracks(BUILDING_BARR, TXT_BARRACKS, "BARR", FACING_NONE, 32833728, REMAP_2, 0, 0, 0, false, true,
    false, false, false, false, true, true, false, false, false, true, RTTI_INFANTRYTYPE, DIR_NORTH, BSIZE_2BY2, ExitPyle, List1111, nullptr);

const BuildingTypeClass BuildingTent(BUILDING_TENT, TXT_BARRACKS, "TENT", FACING_NONE, 32833792, REMAP_2, 0, 0, 0, false, true, 0,
    false, false, false, true, true, false, false, false, true, RTTI_INFANTRYTYPE, DIR_NORTH, BSIZE_2BY2, ExitPyle, List1111, nullptr);

const BuildingTypeClass BuildingKennel(BUILDING_KENN, TXT_KENNEL, "KENN", FACING_NONE, 11206741, REMAP_2, 0, 0, 0, false, true,
    false, false, false, false, true, true, false, false, false, true, RTTI_INFANTRYTYPE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingFakeShipYard(BUILDING_SYRF, TXT_FAKE_NAVAL_YARD, "SYRF", FACING_NONE, 25166187, REMAP_2, 0, 0, 0,
    true, false, false, false, false, false, true, true, false, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_3BY3, ExitWeap, ListSPen, OListSPen);

const BuildingTypeClass BuildingFakeSubPen(BUILDING_SPEF, TXT_FAKE_SUB_PEN, "SPEF", FACING_NONE, 25166187, REMAP_2, 0,
    0, 0, true, false, false, false, false, false, true, true, false, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_3BY3, ExitSub, ListSPen, OListSPen);

const BuildingTypeClass BuildingFakeCommand(BUILDING_DOMF, TXT_FAKE_RADAR_DOME, "DOMF", FACING_NONE, 0, REMAP_2, 0, 0, 0, true, true, false,
    false, false, false, true, true, false, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_2BY2, nullptr, ListCom, OListCom);

const BuildingTypeClass BuildingRepair(BUILDING_FIX, TXT_FIX_IT, "FIX", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, 0, 0, 0,
    false, true, true, false, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_3BY3, nullptr, ListFix, OListFix);

const BuildingTypeClass BuildingV01(BUILDING_V01, TXT_CIV1, "V01", FACING_SOUTH, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, true,
    true, true,true, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_2BY2, nullptr, List0011, OList1100);

const BuildingTypeClass BuildingV02(BUILDING_V02, TXT_CIV2, "V02", FACING_SOUTH, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, true,
    true, true,true, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_2BY2, nullptr, List0011, OList1100);

const BuildingTypeClass BuildingV03(BUILDING_V03, TXT_CIV3, "V03", FACING_SOUTH, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, true,
    true, true,true, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_2BY2, nullptr, List0111, OList1000);

const BuildingTypeClass BuildingV04(BUILDING_V04, TXT_CIV4, "V04", FACING_SOUTH, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, true,
    true, true,true, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_2BY2, nullptr, List0011, OList1100);

const BuildingTypeClass BuildingV05(BUILDING_V05, TXT_CIV5, "V05", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, true, true,
    true, true, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_2BY1, nullptr, List11, nullptr);

const BuildingTypeClass BuildingV06(BUILDING_V06, TXT_CIV6, "V06", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, true, true,
    true, true, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_2BY1, nullptr, List11, nullptr);

const BuildingTypeClass BuildingV07(BUILDING_V07, TXT_CIV7, "V07", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, true, true,
    true, true, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_2BY1, nullptr, List11, nullptr);

const BuildingTypeClass BuildingV08(BUILDING_V08, TXT_CIV8, "V08", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, true, true,
    true, true, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingV09(BUILDING_V09, TXT_CIV9, "V09", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, true, true,
    true, true, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingV10(BUILDING_V10, TXT_CIV10, "V10", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, true, true,
    true, true, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingV11(BUILDING_V11, TXT_CIV11, "V11", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, true, true,
    true, true, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingV12(BUILDING_V12, TXT_CIV12, "V12", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, true, true,
    true, true, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingV13(BUILDING_V13, TXT_CIV13, "V13", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, true, true,
    true, true, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingV14(BUILDING_V14, TXT_CIV14, "V14", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, true, true,
    true, true, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingV15(BUILDING_V15, TXT_CIV15, "V15", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, true,
    true, true,true, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingV16(BUILDING_V16, TXT_CIV16, "V16", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, true,
    true, true,true, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingV17(BUILDING_V17, TXT_CIV17, "V17", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, true,
    true, true,true, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingV18(BUILDING_V18, TXT_CIV18, "V18", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, true,
    true, true,true, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingV19(BUILDING_V19, TXT_PUMP, "V19", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, false, false,
    true, true, false, false, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingV20(BUILDING_V20, TXT_CIV20, "V20", FACING_SOUTH, 0, REMAP_2, 0, 0, 0, false, true, true, false, false, false,
    true, true, false, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_2BY2, nullptr, List0011, OList1100);

const BuildingTypeClass BuildingV21(BUILDING_V21, TXT_CIV21, "V21", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, false, false,
    true, true, false, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_2BY2, nullptr, List1101, OList0010);

const BuildingTypeClass BuildingV22(BUILDING_V22, TXT_CIV22, "V22", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, false, false,
    true, true, false, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_2BY1, nullptr, List11, nullptr);

const BuildingTypeClass BuildingV23(BUILDING_V23, TXT_CIV23, "V23", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, false, false,
    true, true, false, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingV24(BUILDING_V24, TXT_CIV24, "V24", FACING_SOUTH, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, false,
    true, true, false, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_2BY2, nullptr, List0011, OList1100);

const BuildingTypeClass BuildingV25(BUILDING_V25, TXT_CIV25, "V25", FACING_SOUTH, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, false,
    true, true, false, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_2BY2, nullptr, List0111, OList1000);

const BuildingTypeClass BuildingV26(BUILDING_V26, TXT_CIV26, "V26", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, false,
    true, true, false, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_2BY1, nullptr, List11, nullptr);

const BuildingTypeClass BuildingV27(BUILDING_V27, TXT_CIV27, "V27", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, false,
    true, true, false, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingV28(BUILDING_V28, TXT_CIV28, "V28", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, false,
    true, true, false, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingV29(BUILDING_V29, TXT_CIV29, "V29", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, false,
    true, true, false, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingV30(BUILDING_V30, TXT_CIV30, "V30", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, false,
    true, true, false, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_2BY1, nullptr, List11, nullptr);

const BuildingTypeClass BuildingV31(BUILDING_V31, TXT_CIV31, "V31", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, false,
    true, true, false, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_2BY1, nullptr, List11, nullptr);

const BuildingTypeClass BuildingV32(BUILDING_V32, TXT_CIV32, "V32", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, false,
    true, true, false, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_2BY1, nullptr, List11, nullptr);

const BuildingTypeClass BuildingV33(BUILDING_V33, TXT_CIV33, "V33", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, false,
    true, true, false, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_2BY1, nullptr, List11, nullptr);

const BuildingTypeClass BuildingV34(BUILDING_V34, TXT_CIV34, "V34", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, false,
    true, true, false, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingV35(BUILDING_V35, TXT_CIV35, "V35", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, false,
    true, true, false, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingV36(BUILDING_V36, TXT_CIV36, "V36", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, false,
    true, true, false, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingV37(BUILDING_V37, TXT_CIV37, "V37", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false, true, false,
    true, true, false, true, false, false, RTTI_NONE, DIR_NORTH, BSIZE_4BY2, nullptr, ListWestwood, OListWestwood);

const BuildingTypeClass BuildingMission(BUILDING_MISS, TXT_CIVMISS, "MISS", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, true, true, false,
    true, false, true, true, false, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_3BY2, nullptr, List111111, nullptr);

const BuildingTypeClass BuildingSandbag(BUILDING_SBAG, TXT_SANDBAG_WALL, "SBAG", FACING_NONE, 0, REMAP_0, 0, 0, 0, false, false, true, true,
    0, false, false, true, true, false, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingCyclone(BUILDING_CYCL, TXT_CYCLONE_WALL, "CYCL", FACING_NONE, 0, REMAP_0, 0, 0, 0, false, false, true, true,
    0, false, false, true, true, false, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingBrick(BUILDING_BRIK, TXT_CONCRETE_WALL, "BRIK", FACING_NONE, 0, REMAP_0, 0, 0, 0, false, false, true, true, false,
    false, false, true, true, false, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingBarbwire(BUILDING_BARB, TXT_BARBWIRE_WALL, "BARB", FACING_NONE, 0, REMAP_0, 0, 0, 0, false, false, true, true,
    0, false, false, true, true, false, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingWood(BUILDING_WOOD, TXT_WOOD_WALL, "WOOD", FACING_NONE, 0, REMAP_0, 0, 0, 0, false, false, true, true, false,
    false, false, true, true, false, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingFence(BUILDING_FENC, TXT_FENCE, "FENC", FACING_NONE, 0, REMAP_0, 0, 0, 0, false, false, true, true, false,
    false, false, true, true, false, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingQueen(BUILDING_QUEE, TXT_NULL, "QUEE", FACING_NONE, 32833792, REMAP_2, 0, 0, 0, false, true, true, false,
    false, false, true, true, false, false, false, true, RTTI_NONE, DIR_NORTH, BSIZE_2BY1, nullptr, List11, nullptr);

const BuildingTypeClass BuildingLarva1(BUILDING_LAR1, TXT_NULL, "LAR1", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, false, true, false, true, true,
    true, true, true, false, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);

const BuildingTypeClass BuildingLarva2(BUILDING_LAR2, TXT_NULL, "LAR2", FACING_NONE, 0, REMAP_2, 0, 0, 0, false, false, true, false, true, true,
    true, true, true, false, false, false, RTTI_NONE, DIR_NORTH, BSIZE_1BY1, nullptr, List1, nullptr);
