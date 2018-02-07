/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Handles bonus crates.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "crate.h"
#include "gamedebug.h"
#include "scenario.h"

BOOL CrateClass::Remove_It()
{
    if (Cell == -1) {
        return false;
    }

    Get_Crate(Cell);
    Cell = -1;
    CrateTimer.Stop();

    return true;
}

BOOL CrateClass::Create_Crate(int16_t cell)
{
    // TODO Requires ScenarioClass and RuleClass to actuall implement the functions.
#ifndef RAPP_STANDALONE
    BOOL(*func)(const CrateClass*, int16_t) = reinterpret_cast<BOOL(*)(const CrateClass*, int16_t)>(0x004AC91C);
    return func(this, cell);
#elif 0
    Remove_It();

    if (Put_Crate(cell)) {

    }

    return false;
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called.\n");
    return false;
#endif
}

BOOL CrateClass::Put_Crate(int16_t &cell)
{
    // TODO Requires MapClass, CellClass and ScenarioClass to actuall implement the functions.
#ifndef RAPP_STANDALONE
    BOOL(*func)(int16_t&) = reinterpret_cast<BOOL(*)(int16_t&)>(0x004AC9B4);
    return func(cell);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called.\n");
    return false;
#endif
}

BOOL CrateClass::Get_Crate(int16_t cell)
{
    // TODO Requires MapClass, CellClass and ScenarioClass to actuall implement the functions.
#ifndef RAPP_STANDALONE
    BOOL(*func)(int16_t) = reinterpret_cast<BOOL(*)(int16_t)>(0x004ACB1C);
    return func(cell);
#else
    DEBUG_ASSERT_PRINT(false, "Unimplemented function called.\n");
    return false;
#endif
}
