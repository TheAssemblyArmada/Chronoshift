/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class containing information about weapon behaviour.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef WEAPONTYPE_H
#define WEAPONTYPE_H

#include "always.h"
#include "animtype.h"
#include "audio.h"
#include "gametypes.h"
#include "heap.h"

enum WeaponType {
    WEAPON_NONE = -1,
    WEAPON_FIRST = 0,
    WEAPON_COLT45 = 0,
    WEAPON_ZSU23 = 1,
    WEAPON_VULCAN = 2,
    WEAPON_MAVERICK = 3,
    WEAPON_CAMERA = 4,
    WEAPON_FIREBALL_LAUNCHER = 5,
    WEAPON_SNIPER = 6,
    WEAPON_CHAINGUN = 7,
    WEAPON_M1CARBINE = 8,
    WEAPON_DRAGON = 9,
    WEAPON_HELLFIRE = 10,
    WEAPON_GRENADE = 11,
    WEAPON_75MM = 12,
    WEAPON_90MM = 13,
    WEAPON_105MM = 14,
    WEAPON_120MM = 15,
    WEAPON_TURRET_GUN = 16,
    WEAPON_MAMMOTH_TUSK = 17,
    WEAPON_155MM = 18,
    WEAPON_M60MG = 19,
    WEAPON_NAPALM = 20,
    WEAPON_TESLAZAP = 21,
    WEAPON_NIKE = 22,
    WEAPON_8INCH = 23,
    WEAPON_STINGER = 24,
    WEAPON_TORPTUBE = 25,
    WEAPON_2INCH = 26,
    WEAPON_DEPTH_CHARGE = 27,
    WEAPON_PARABOMB = 28,
    WEAPON_DOGJAW = 29,
    WEAPON_HEAL = 30,
    WEAPON_SCUD = 31,
    WEAPON_FLAMER = 32,
    WEAPON_REDEYE = 33,
    WEAPON_MANDIBLE = 34,
    WEAPON_PORTATESLA = 35,
    WEAPON_GOOD_WRENCH = 36,
    WEAPON_SUBSCUD = 37,
    WEAPON_TTANKZAP = 38,
    WEAPON_APTUSK = 39,
    WEAPON_DEMO_CHARGE = 40,
    WEAPON_AIR_ASSAULT = 41,
    WEAPON_COUNT,
};

DEFINE_ENUMERATION_OPERATORS(WeaponType);

class CCINIClass;
class WarheadTypeClass;
class BulletTypeClass;

class WeaponTypeClass
{
public:
    WeaponTypeClass(const char *name);
    WeaponTypeClass(WeaponType weapon, const char *name);
    WeaponTypeClass(WeaponTypeClass const &that);
    WeaponTypeClass(NoInitClass const &noinit) {}
    ~WeaponTypeClass();

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    void Code_Pointers() {}
    void Decode_Pointers() {}

    BOOL Read_INI(CCINIClass &ini);
    ThreatType Allowed_Threats() const;
    BOOL Is_Wall_Destroyer() const;
    WarheadTypeClass *Get_Warhead() const;
    BulletTypeClass *Get_Projectile() const;
    const char *Get_Name() const { return Name; }

    static void Init_Heap();
    static WeaponType From_Name(const char *name);
    static const char *Name_From(WeaponType weapon);
    static WeaponTypeClass &As_Reference(WeaponType weapon);
    static WeaponTypeClass *As_Pointer(WeaponType weapon);

private:
    int Type;
    const char *Name;
#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool TurboBoost : 1; // Should the weapon get a boosted speed bonus when firing upon aircraft?
            bool Supress : 1;// Should nearby friendly buildings be scanned for and if found, discourage firing on target?
            bool Camera : 1; // Reveals area around firer ?
            bool Charges : 1; // Does it have charge-up-before-firing logic?
            bool ElectricZap : 1; // RA++ addition, use ElectricZap logic?
            bool LaserZap : 1; // RA++ addition, use C&C Laser logic?
        };
        int Bitfield;
    };
#else
    bool TurboBoost; // Should the weapon get a boosted speed bonus when firing upon aircraft?
    bool Supress;// Should nearby friendly buildings be scanned for and if found, discourage firing on target?
    bool Camera; // Reveals area around firer ?
    bool Charges; // Does it have charge-up-before-firing logic?
    bool ElectricZap; // RA++ addition, use ElectricZap logic?
    bool LaserZap; // RA++ addition, use C&C Laser logic?
#endif
    int Burst; // Number of rapid succession shots from this weapon (def = 1)
    BulletTypeClass *Projectile; // Projectile characteristic to use
    int Damage; // The amount of damage (unattenuated) dealt with every bullet
    MPHType Speed; // Speed of projectile to target (100 is maximum)
    WarheadTypeClass *Warhead;	// Warhead to attach to projectile
    int ROF; // Delay between shots in ticks [15 = 1 second at middle speed setting]
    int16_t Range;	// Maximum cell range in leptons
    VocType Report; // Sound to play when firing
    AnimType Anim; // Animation to display as a firing effect
};

#ifndef RAPP_STANDALONE
#include "hooker.h"
extern TFixedIHeapClass<WeaponTypeClass> &WeaponTypes;
#else
extern TFixedIHeapClass<WeaponTypeClass> WeaponTypes;
#endif

#endif // WEAPONTYPE_H
