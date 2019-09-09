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
#include "fly.h"

ImpactType FlyClass::Physics(unsigned int &somevalue, DirType dir)
{
#ifdef GAME_DLL
    ImpactType (*func)(FlyClass *, unsigned int &, DirType) =
        reinterpret_cast<ImpactType (*)(FlyClass *, unsigned int &, DirType)>(0x004C06E0);
    return func(this, somevalue, dir);
#else
    return IMPACT_NONE;
#endif
}

void FlyClass::Fly_Speed(int speed, MPHType mph)
{
#ifdef GAME_DLL
    void (*func)(FlyClass *, int, MPHType) = reinterpret_cast<void (*)(FlyClass *, int, MPHType)>(0x004C0764);
    func(this, speed, mph);
#endif
}
