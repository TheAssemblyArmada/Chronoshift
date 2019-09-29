/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Static instances of overlay type objects.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "overlaydata.h"

/**
 * For reference, the constructor argument order is;
 *
 * Type (OverlayType)
 * Name
 * UI Name
 * Land
 * Damage Levels
 * Strength
 * A7
 * A8
 * Legal Target
 * Crushable
 * A11
 * A12
 * A13
 * Wall
 * Crate
 *
 */

const OverlayTypeClass OverlaySandbag(
    OVERLAY_SANDBAG, "SBAG", TXT_SANDBAG_WALL, LAND_WALL, 1, 20, true, false, true, true, false, false, false, true, false);

const OverlayTypeClass OverlayCyclone(OVERLAY_CYCLONE_FENCE, "CYCL", TXT_CYCLONE_WALL, LAND_WALL, 2, 10, true, false, true,
    true, false, false, false, true, false);

const OverlayTypeClass OverlayBrick(OVERLAY_BRICK_WALL, "BRIK", TXT_CONCRETE_WALL, LAND_WALL, 3, 70, true, false, true,
    false, false, true, false, true, false);

const OverlayTypeClass OverlayBarbwire(OVERLAY_BARB_WIRE, "BARB", TXT_BARBWIRE_WALL, LAND_WALL, 1, 2, true, false, true,
    true, false, false, false, true, false);

const OverlayTypeClass OverlayWood(
    OVERLAY_WOOD_FENCE, "WOOD", TXT_WOOD_WALL, LAND_WALL, 1, 2, true, true, true, true, false, false, false, true, false);

const OverlayTypeClass OverlayFence(
    OVERLAY_FENCE, "FENC", TXT_FENCE, LAND_WALL, 2, 10, true, false, true, true, false, false, false, true, false);

const OverlayTypeClass OverlayGold1(
    OVERLAY_GOLD_01, "GOLD01", TXT_GOLD, LAND_ORE, 0, 0, true, false, false, false, true, false, true, false, false);

const OverlayTypeClass OverlayGold2(
    OVERLAY_GOLD_02, "GOLD02", TXT_GOLD, LAND_ORE, 0, 0, true, false, false, false, true, false, true, false, false);

const OverlayTypeClass OverlayGold3(
    OVERLAY_GOLD_03, "GOLD03", TXT_GOLD, LAND_ORE, 0, 0, true, false, false, false, true, false, true, false, false);

const OverlayTypeClass OverlayGold4(
    OVERLAY_GOLD_04, "GOLD04", TXT_GOLD, LAND_ORE, 0, 0, true, false, false, false, true, false, true, false, false);

const OverlayTypeClass OverlayGems1(
    OVERLAY_GEM_01, "GEM01", TXT_GEMS, LAND_ORE, 0, 0, true, false, false, false, true, false, true, false, false);

const OverlayTypeClass OverlayGems2(
    OVERLAY_GEM_02, "GEM02", TXT_GEMS, LAND_ORE, 0, 0, true, false, false, false, true, false, true, false, false);

const OverlayTypeClass OverlayGems3(
    OVERLAY_GEM_03, "GEM03", TXT_GEMS, LAND_ORE, 0, 0, true, false, false, false, true, false, true, false, false);

const OverlayTypeClass OverlayGems4(
    OVERLAY_GEM_04, "GEM04", TXT_GEMS, LAND_ORE, 0, 0, true, false, false, false, true, false, true, false, false);

const OverlayTypeClass OverlayV12(
    OVERLAY_V12, "V12", TXT_CIV12, LAND_ROCK, 0, 0, false, false, false, true, false, false, true, false, false);

const OverlayTypeClass OverlayV13(
    OVERLAY_V13, "V13", TXT_CIV13, LAND_ROCK, 0, 0, false, false, false, true, false, false, true, false, false);

const OverlayTypeClass OverlayV14(
    OVERLAY_V14, "V14", TXT_CIV14, LAND_ROCK, 0, 0, false, false, false, true, false, false, true, false, false);

const OverlayTypeClass OverlayV15(
    OVERLAY_V15, "V15", TXT_CIV15, LAND_ROCK, 0, 0, false, false, false, true, false, false, true, false, false);

const OverlayTypeClass OverlayV16(
    OVERLAY_V16, "V16", TXT_CIV16, LAND_ROCK, 0, 0, false, false, false, true, false, false, true, false, false);

const OverlayTypeClass OverlayV17(
    OVERLAY_V17, "V17", TXT_CIV17, LAND_ROCK, 0, 0, false, false, false, true, false, false, true, false, false);

const OverlayTypeClass OverlayV18(
    OVERLAY_V18, "V18", TXT_CIV18, LAND_ROCK, 0, 0, false, false, false, true, false, false, true, false, false);

const OverlayTypeClass OverlayFlagSpot(
    OVERLAY_FPLS, "FPLS", TXT_FLAG_SPOT, LAND_CLEAR, 0, 0, true, false, false, false, false, false, false, false, false);

const OverlayTypeClass OverlayWoodCrate(OVERLAY_WOOD_FENCE_CRATE, "WCRATE", TXT_WOOD_CRATE, LAND_CLEAR, 0, 0, false, false,
    false, false, false, false, false, false, true);

const OverlayTypeClass OverlayWaterCrate(OVERLAY_WATER_WOOD_CRATE, "WWCRATE", TXT_WATER_CRATE, LAND_WATER, 0, 0, false,
    false, false, false, false, false, false, false, true);

const OverlayTypeClass OverlaySteelCrate(OVERLAY_STEEL_CRATE, "SCRATE", TXT_STEEL_CRATE, LAND_CLEAR, 0, 0, false, false,
    false, false, false, false, false, false, true);
