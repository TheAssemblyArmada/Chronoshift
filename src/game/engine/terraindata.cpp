/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Static instances of terrain type objects.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "terraindata.h"
#include "coord.h"
#include "lists.h"

/**
 * For reference, the constructor argument order is;
 *
 * Type (TerrainType)
 * A3
 * Immune
 * Water Bound
 * Name
 * UI Name
 * Occupy List
 * Overlap List
 * 
 */

// NOTE: Fixed occupy bit coord (changed from Coord_From_Pixel_XY(12, 24) to Coord_From_Pixel_XY(12, 12)).
const TerrainTypeClass TerrainMine(TERRAIN_MINE, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW, Coord_From_Pixel_XY(12, 12), true,
    false, "MINE", TXT_ORE_MINE, List10, nullptr);

const TerrainTypeClass TerrainBoxes01(
    TERRAIN_BOXES01, THEATER_BIT_INTERIOR, Coord_From_Pixel_XY(12, 24), true, false, "BOXES01", TXT_CRATES, List10, nullptr);

const TerrainTypeClass TerrainBoxes02(
    TERRAIN_BOXES02, THEATER_BIT_INTERIOR, Coord_From_Pixel_XY(12, 24), true, false, "BOXES02", TXT_CRATES, List10, nullptr);

const TerrainTypeClass TerrainBoxes03(
    TERRAIN_BOXES03, THEATER_BIT_INTERIOR, Coord_From_Pixel_XY(12, 24), true, false, "BOXES03", TXT_CRATES, List10, nullptr);

const TerrainTypeClass TerrainBoxes04(
    TERRAIN_BOXES04, THEATER_BIT_INTERIOR, Coord_From_Pixel_XY(12, 24), true, false, "BOXES04", TXT_CRATES, List10, nullptr);

const TerrainTypeClass TerrainBoxes05(
    TERRAIN_BOXES05, THEATER_BIT_INTERIOR, Coord_From_Pixel_XY(12, 24), true, false, "BOXES05", TXT_CRATES, List10, nullptr);

const TerrainTypeClass TerrainBoxes06(
    TERRAIN_BOXES06, THEATER_BIT_INTERIOR, Coord_From_Pixel_XY(12, 24), true, false, "BOXES06", TXT_CRATES, List10, nullptr);

const TerrainTypeClass TerrainBoxes07(
    TERRAIN_BOXES07, THEATER_BIT_INTERIOR, Coord_From_Pixel_XY(12, 24), true, false, "BOXES07", TXT_CRATES, List10, nullptr);

const TerrainTypeClass TerrainBoxes08(
    TERRAIN_BOXES08, THEATER_BIT_INTERIOR, Coord_From_Pixel_XY(12, 24), true, false, "BOXES08", TXT_CRATES, List10, nullptr);

const TerrainTypeClass TerrainBoxes09(
    TERRAIN_BOXES09, THEATER_BIT_INTERIOR, Coord_From_Pixel_XY(12, 24), true, false, "BOXES09", TXT_CRATES, List10, nullptr);

const TerrainTypeClass TerrainIce01(
    TERRAIN_ICE01, THEATER_BIT_SNOW, Coord_From_Pixel_XY(24, 24), true, true, "ICE01", TXT_ICE, List1111, nullptr);

const TerrainTypeClass TerrainIce02(
    TERRAIN_ICE02, THEATER_BIT_SNOW, Coord_From_Pixel_XY(12, 24), true, true, "ICE02", TXT_ICE, List1010, nullptr);

const TerrainTypeClass TerrainIce03(
    TERRAIN_ICE03, THEATER_BIT_SNOW, Coord_From_Pixel_XY(24, 12), true, true, "ICE03", TXT_ICE, List11, nullptr);

const TerrainTypeClass TerrainIce04(
    TERRAIN_ICE04, THEATER_BIT_SNOW, Coord_From_Pixel_XY(12, 12), true, true, "ICE04", TXT_ICE, List10, nullptr);

const TerrainTypeClass TerrainIce05(
    TERRAIN_ICE05, THEATER_BIT_SNOW, Coord_From_Pixel_XY(12, 12), true, true, "ICE05", TXT_ICE, List10, nullptr);

const TerrainTypeClass TerrainTree1(TERRAIN_TREE1, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW, Coord_From_Pixel_XY(11, 41),
    false, false, "T01", TXT_TREE, List0010, OList1001);

const TerrainTypeClass TerrainTree2(TERRAIN_TREE2, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW, Coord_From_Pixel_XY(11, 44),
    false, false, "T02", TXT_TREE, List0010, OList1001);

const TerrainTypeClass TerrainTree3(TERRAIN_TREE3, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW, Coord_From_Pixel_XY(12, 45),
    false, false, "T03", TXT_TREE, List0010, OList1001);

const TerrainTypeClass TerrainTree5(TERRAIN_TREE5, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW, Coord_From_Pixel_XY(15, 41),
    false, false, "T05", TXT_TREE, List0010, OList1001);

const TerrainTypeClass TerrainTree6(TERRAIN_TREE6, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW, Coord_From_Pixel_XY(16, 37),
    false, false, "T06", TXT_TREE, List0010, OList1001);

const TerrainTypeClass TerrainTree7(TERRAIN_TREE7, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW, Coord_From_Pixel_XY(15, 41),
    false, false, "T07", TXT_TREE, List0010, OList1001);

const TerrainTypeClass TerrainTree8(TERRAIN_TREE8, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW, Coord_From_Pixel_XY(14, 22),
    false, false, "T08", TXT_TREE, List10, OList01);

const TerrainTypeClass TerrainTree10(TERRAIN_TREE10, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW, Coord_From_Pixel_XY(25, 43),
    false, false, "T10", TXT_TREE, List0011, OList1100);

const TerrainTypeClass TerrainTree11(TERRAIN_TREE11, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW, Coord_From_Pixel_XY(23, 44),
    false, false, "T11", TXT_TREE, List0011, OList1100);

const TerrainTypeClass TerrainTree12(TERRAIN_TREE12, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW, Coord_From_Pixel_XY(14, 36),
    false, false, "T12", TXT_TREE, List0010, OList1001);

const TerrainTypeClass TerrainTree13(TERRAIN_TREE13, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW, Coord_From_Pixel_XY(19, 40),
    false, false, "T13", TXT_TREE, List0010, OList1101);

const TerrainTypeClass TerrainTree14(TERRAIN_TREE14, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW, Coord_From_Pixel_XY(19, 40),
    false, false, "T14", TXT_TREE, List0011, OList1100);

const TerrainTypeClass TerrainTree15(TERRAIN_TREE15, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW, Coord_From_Pixel_XY(19, 40),
    false, false, "T15", TXT_TREE, List0011, OList1100);

const TerrainTypeClass TerrainTree16(TERRAIN_TREE16, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW, Coord_From_Pixel_XY(13, 36),
    false, false, "T16", TXT_TREE, List0010, OList1001);

const TerrainTypeClass TerrainTree17(TERRAIN_TREE17, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW, Coord_From_Pixel_XY(18, 44),
    false, false, "T17", TXT_TREE, List0010, OList1001);

const TerrainTypeClass TerrainTreeClump1(TERRAIN_TREECLUMP1, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(28, 41), true, false, "TC01", TXT_TREE, List000110, OList110001);

const TerrainTypeClass TerrainTreeClump2(TERRAIN_TREECLUMP2, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(38, 41), true, false, "TC02", TXT_TREE, List010110, OList101001);

const TerrainTypeClass TerrainTreeClump3(TERRAIN_TREECLUMP3, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(33, 35), true, false, "TC03", TXT_TREE, List110110, OList001);

const TerrainTypeClass TerrainTreeClump4(TERRAIN_TREECLUMP4, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(44, 49), true, false, "TC04", TXT_TREE, List000011101000, OList111000010110);

const TerrainTypeClass TerrainTreeClump5(TERRAIN_TREECLUMP5, THEATER_BIT_TEMPERATE | THEATER_BIT_SNOW,
    Coord_From_Pixel_XY(49, 58), true, false, "TC05", TXT_TREE, List001011100110, OList110000011001);

/*const TerrainTypeClass TerrainFlagFly(
    TERRAIN_FLAGFLY, THEATER_BIT_NONE, Coord_From_Pixel_XY(15, 41), true, false, "FLAGFLY", TXT_NULL, List10, OList01);*/
