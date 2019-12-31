/**
 * @file
 *
 * @author tomsons26
 *
 * @brief
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */

#pragma once

#ifndef FETCHTECHTYPE_H
#define FETCHTECHTYPE_H

#include "always.h"
#include "rtti.h"

class TechnoTypeClass;

TechnoTypeClass *Fetch_Techno_Type(RTTIType type, int id);

#endif // FETCHTECHTYPE_H
