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
#include "damage.h"
#include "techno.h"

void Explosion_Damage(coord_t at, int a2, TechnoClass *object, WarheadType warhead)
{
#ifdef GAME_DLL
    void (*func)(coord_t, int, TechnoClass *, WarheadType) = reinterpret_cast<void (*)(coord_t, int, TechnoClass *, WarheadType)>(0x004A32D8);
    func(at, a2, object, warhead);
#endif
}

int Modify_Damage(int a1, WarheadType warhead, ArmorType armor, int a4)
{
#ifdef GAME_DLL
    int (*func)(int, WarheadType, ArmorType, int) = reinterpret_cast<int (*)(int, WarheadType, ArmorType, int)>(0x004A31F0);
    return func(a1, warhead, armor, warhead);
#else
    return 0;
#endif
}

AnimType Combat_Anim(int a1, WarheadType warhead, LandType land)
{
#ifdef GAME_DLL
    AnimType (*func)(int, WarheadType, LandType) = reinterpret_cast<AnimType (*)(int, WarheadType, LandType)>(0x004A35D4);
    return func(a1, warhead, land);
#else
    return ANIM_NONE;
#endif
}

void Wide_Area_Damage(coord_t at, unsigned short a2, int a3, TechnoClass* object, WarheadType warhead)
{
#ifdef GAME_DLL
    void (*func)(coord_t, int, TechnoClass *, WarheadType) = reinterpret_cast<void (*)(coord_t, int, TechnoClass *, WarheadType)>(0x004A37D4);
    func(at, a2, object, warhead);
#endif
}
