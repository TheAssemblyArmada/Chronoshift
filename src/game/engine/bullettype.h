/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class containing information about projectile behaviour.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef BULLETTYPE_H
#define BULLETTYPE_H

#include "always.h"
#include "heap.h"
#include "objecttype.h"

class CCINIClass;

// If you add an entry here, add matching entries to bullettype.cpp globals and Init_Heap.
enum BulletType
{
    BULLET_NONE = -1,
    BULLET_FIRST = 0,
    BULLET_INVISIBLE = 0,
    BULLET_CANNON = 1,
    BULLET_ACK = 2,
    BULLET_TORPEDO = 3,
    BULLET_FROG = 4,
    BULLET_HEATSEEKER = 5,
    BULLET_LASERGUIDED = 6,
    BULLET_LOBBED = 7,
    BULLET_BOMBLET = 8,
    BULLET_BALLISTIC = 9,
    BULLET_PARACHUTE = 10,
    BULLET_FIREBALL = 11,
    BULLET_LEAPDOG = 12,
    BULLET_CATAPULT = 13,
    BULLET_AAMISSILE = 14,
    BULLET_GPSSATELLITE = 15,
    BULLET_NUKEUP = 16,
    BULLET_NUKEDOWN = 17,
    BULLET_COUNT,
};

DEFINE_ENUMERATION_OPERATORS(BulletType);

class BulletTypeClass : public ObjectTypeClass
{
public:
    BulletTypeClass(BulletType bullet, const char *name);
    BulletTypeClass(BulletTypeClass const &that);
    BulletTypeClass(NoInitClass const &noinit) : ObjectTypeClass(noinit) {}
    ~BulletTypeClass() {}

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    virtual BOOL Create_And_Place(int16_t cellnum, HousesType house = HOUSES_NONE) const override { return false; };
    virtual ObjectClass *Create_One_Of(HouseClass *house) const override { return nullptr; }
    virtual BOOL Read_INI(CCINIClass &ini);

    int Get_Type() const { return Type; }
    BOOL Is_Anti_Air() const { return AntiAir; }
    BOOL Is_Anti_Ground() const { return AntiGround; }
    void Code_Pointers() {}
    void Decode_Pointers() {}

    static void Init_Heap();
    static void One_Time();
    static BulletType From_Name(const char *name);
    static const char *Name_From(BulletType bullet);
    static BulletTypeClass &As_Reference(BulletType bullet);
    static BulletTypeClass *As_Pointer(BulletType bullet);

private:
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool High : 1; // Can it fly over walls?
            bool Shadow : 1; // If High, does this bullet need to have a shadow drawn?
            bool Arcing : 1; // Does it have a ballistic trajectory?
            bool Dropping : 1; // Does it fall from a starting height?
            bool Inviso : 1; // Is the projectile invisible as it travels?
            bool Proximity : 1; // Does it blow up when near its target?
            bool Animates : 1; // Does it animate [this means smoke puffs]?
            bool Ranged : 1; // Can it run out of fuel?
            bool Rotates : 1; // Does the projectile have rotation specific imagery?
            bool Inaccurate : 1; // Is it inherently inaccurate?
            bool Translucent : 1; // Are translucent colors used in artwork?
            bool AntiAir : 1; // Can this weapon fire upon flying aircraft?
            bool AntiGround : 1; // Can this weapon fire upon ground objects?
            bool AntiSubWarfare : 1; // Is this an Anti-Submarine-Warfare projectile?
            bool Degenerates : 1; // Does the bullet strength weaken as it travels?
            bool UnderWater : 1; // Does the projectile travel under water?
            bool Parachuted : 1; // Equipped with a parachute for dropping from plane (def = false)?
            bool Gigundo : 1; // Is the projectile larger than normal?
        };
        int Bitfield;
    };
#else
    bool High; // Can it fly over walls?
    bool Shadow; // If High, does this bullet need to have a shadow drawn?
    bool Arcing; // Does it have a ballistic trajectory?
    bool Dropping; // Does it fall from a starting height?
    bool Inviso; // Is the projectile invisible as it travels?
    bool Proximity; // Does it blow up when near its target?
    bool Animates; // Does it animate [this means smoke puffs]?
    bool Ranged; // Can it run out of fuel?
    bool Rotates; // Does the projectile have rotation specific imagery?
    bool Inaccurate; // Is it inherently inaccurate?
    bool Translucent; // Are translucent colors used in artwork?
    bool AntiAir; // Can this weapon fire upon flying aircraft?
    bool AntiGround; // Can this weapon fire upon ground objects?
    bool AntiSubWarfare; // Is this an Anti-Submarine-Warfare projectile?
    bool Degenerates; // Does the bullet strength weaken as it travels?
    bool UnderWater; // Does the projectile travel under water?
    bool Parachuted; // Equipped with a parachute for dropping from plane (def = false)?
    bool Gigundo; // Is the projectile larger than normal?
#endif 
    BulletType Type;
    uint8_t ROT; // Rate Of Turn [non zero implies homing].
    int Arm; // Arming delay.
    int Frames; // Number of image frames for animation purposes.
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
extern TFixedIHeapClass<BulletTypeClass> &BulletTypes;
#else
extern TFixedIHeapClass<BulletTypeClass> BulletTypes;
#endif

#endif // BULLETTYPE_H
