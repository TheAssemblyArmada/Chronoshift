/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Functions for handling the "attract" demo mode.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef ATTRACT_H
#define ATTRACT_H

#include "always.h"

class GameFileClass;

void Do_Record_Playback();
BOOL Save_Recording_Values(GameFileClass &file);
BOOL Load_Recording_Values(GameFileClass &file);

#endif // ATTRACT_H
