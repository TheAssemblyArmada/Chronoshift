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

#ifndef FLY_H
#define FLY_H

#include "always.h"
#include "facing.h"
#include "gametypes.h"


// maybe?
// https://github.com/Ares-Developers/YRpp/blob/d387fbb9d36e4645d93caf87be502d6d87d042c3/GeneralDefinitions.h#L568
enum ImpactType
{
    IMPACT_NONE = -1,

    IMPACT_0 = 0, // hit?
    IMPACT_1 = 1, // missed?
    IMPACT_2 = 2, // nullified?

    IMPACT_COUNT = 3
};

class FlyClass
{
public:
    FlyClass() : field_0(0), Speed(0) {}

    inline uint8_t Get_Speed() { return Speed; }
    ImpactType Physics(unsigned int &somevalue, DirType dir);
    void Fly_Speed(int speed, MPHType mph);
    void Code_Pointers(){}
    void Decode_Pointers(){}

private:
    int field_0;
    uint8_t Speed;
};

#endif // FLY_H
