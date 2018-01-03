/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class information relating to the current scenario.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "scenario.h"

#ifndef RAPP_STANDALONE
ScenarioClass &Scen = Make_Global<ScenarioClass>(0x006678E8);
#else
ScenarioClass Scen;
#endif

ScenarioClass::ScenarioClass()
{
}
