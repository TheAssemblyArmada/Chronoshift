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

#ifndef COMBAT_H
#define COMBAT_H

#include "always.h"
#include "armor.h"
#include "gametypes.h"
#include "land.h"
#include "warheadtype.h"

class TechnoClass;

int Modify_Damage(int damage, WarheadType warhead, ArmorType armor, int intval);
void Explosion_Damage(coord_t coord, int intval, TechnoClass *object, WarheadType warhead);
AnimType Combat_Anim(int damage, WarheadType warhead, LandType land);
void Wide_Area_Damage(coord_t coord, uint16_t shortval, int damage, TechnoClass *object, WarheadType warhead);

void Shake_The_Screen(int shakes);

#endif //COMBAT_H
