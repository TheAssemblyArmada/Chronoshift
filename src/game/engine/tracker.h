/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Just a function to detach a target from everything else in game.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef TRACKER_H
#define TRACKER_H

#include "always.h"
#include "gametypes.h"

void Detach_This_From_All(target_t target, BOOL a2 = true);

#endif // TRACKER_H
