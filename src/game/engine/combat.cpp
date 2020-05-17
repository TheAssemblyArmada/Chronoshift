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
#include "combat.h"
#ifdef GAME_DLL
#include "hooker.h"
#endif

int Modify_Damage(int damage, WarheadType warhead, ArmorType armor, int distance)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004A31F0, int, int, WarheadType, ArmorType, int);
    return func(damage, warhead, armor, distance);
#else
    return 0;
#endif
}

void Explosion_Damage(coord_t coord, int intval, TechnoClass *object, WarheadType warhead)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004A32D8, void, coord_t, int, TechnoClass *, WarheadType);
    func(coord, intval, object, warhead);
#endif
}

AnimType Combat_Anim(int damage, WarheadType warhead, LandType land)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004A35D4, AnimType, int, WarheadType, LandType);
    return func(damage, warhead, land);
#else
    return AnimType();
#endif
}

void Wide_Area_Damage(coord_t coord, uint16_t radius, int damage, TechnoClass *object, WarheadType warhead)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004A37D4, void, coord_t, uint16_t, int, TechnoClass *, WarheadType);
    func(coord, radius, damage, object, warhead);
#endif
}

void Shake_The_Screen(int shakes)
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x004AB678, void, int);
    func(shakes);
#endif
}
