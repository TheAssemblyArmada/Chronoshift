/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Implements global instance of the final IOMap object.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef IOMAP_H
#define IOMAP_H

#include "always.h"
#include "gmouse.h"

// TODO Update this as additional layers of the hierachy are implemented.
#ifndef RAPP_STANDALONE
#include "hooker.h"
extern GameMouseClass &Map;
#else
extern GameMouseClass Map;
#endif

#endif // IOMAP_H