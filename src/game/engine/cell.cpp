/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class containing information pertaining to a single map cell.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "cell.h"
#include "iomap.h"

CellClass::CellClass() :
    CellNumber(Map.Cell_Number(this)),
    Bit1(false),
    Bit2(false),
    Bit4(false),
    Bit8(false),
    Bit16(false),
    Bit32(false),
    HasFlag(false),
    Bit128(false),
    field_A(HOUSES_COUNT),
    CellTag(),
    Template(TEMPLATE_NONE),
    Icon(0),
    Overlay(OVERLAY_NONE),
    OverlayFrame(-1),
    Smudge(SMUDGE_NONE),
    SmudgeFrame(-1),
    OwnerHouse(HOUSES_NONE),
    field_18(HOUSES_NONE),
    OccupierPtr(nullptr),
    OccupantBit(0),
    Land(LAND_CLEAR)
{
    for (int i = 0; i < MZONE_COUNT; ++i) {
        field_6[i] = 0;
    }

    for (int i = 0; i < ARRAY_SIZE(Overlapper); ++i) {
        Overlapper[i] = nullptr;
    }
}

void CellClass::Recalc_Attributes()
{
    if (g_lastTheater != THEATER_INTERIOR || (Template != TEMPLATE_NONE && Template >= TEMPLATE_FIRST)) {
        if (Overlay == OVERLAY_NONE || (Land = OverlayTypeClass::As_Reference(Overlay).Get_Land(), Land == LAND_CLEAR)) {
            // TODO Possible bug here where TEMPLATE_ARRO0003 can only be land type LAND_CLEAR because it is 0xFF, the value
            // that was TEMPLATE_NONE in the TD and SS map formats due to lower number of templates andusing uint8 to hold
            // the value.
            if (Template == TEMPLATE_NONE || Template == 0xFF) {
                Land = LAND_CLEAR;
            } else {
                Land = TemplateTypeClass::As_Reference(Template).Land_Type(Icon);
            }
        }

    } else {
        Land = LAND_ROCK;
    }
}
