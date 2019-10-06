/**
 * @file
 *
 * @author CCHyper
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

#ifndef DAMAGE_H
#define DAMAGE_H

#include "always.h"
#include "gametypes.h"

void Explosion_Damage(coord_t at, int, TechnoClass *object, WarheadType warhead);
int Modify_Damage(int, WarheadType warhead, ArmorType armor, int);
AnimType Combat_Anim(int, WarheadType warhead, LandType land);
void Wide_Area_Damage(coord_t at, unsigned short, int, TechnoClass *object, WarheadType warhead);

#endif // DAMAGE_H
