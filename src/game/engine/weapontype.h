/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class containing information about weapon behaviour.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
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
#include "gametypes.h"
#include "heap.h"
#include "voc.h"

enum WeaponType
{
    WEAPON_COLT45,
    WEAPON_ZSU23,
    WEAPON_VULCAN,
    WEAPON_MAVERICK,
    WEAPON_CAMERA,
    WEAPON_FIREBALL_LAUNCHER,
    WEAPON_SNIPER,
    WEAPON_CHAINGUN,
    WEAPON_PISTOL,
    WEAPON_M1CARBINE,
    WEAPON_DRAGON,
    WEAPON_HELLFIRE,
    WEAPON_GRENADE,
    WEAPON_75MM,
    WEAPON_90MM,
    WEAPON_105MM,
    WEAPON_120MM,
    WEAPON_TURRET_GUN,
    WEAPON_MAMMOTH_TUSK,
    WEAPON_155MM,
    WEAPON_M60MG,
    WEAPON_NAPALM,
    WEAPON_TESLAZAP,
    WEAPON_NIKE,
    WEAPON_8INCH,
    WEAPON_STINGER,
    WEAPON_TORPTUBE,
    WEAPON_2INCH,
    WEAPON_DEPTH_CHARGE,
    WEAPON_PARABOMB,
    WEAPON_DOGJAW,
    WEAPON_HEAL,
    WEAPON_SCUD,
    WEAPON_FLAMER,
    WEAPON_REDEYE,
    WEAPON_MANDIBLE,
    WEAPON_PORTATESLA,
    WEAPON_GOOD_WRENCH,
    WEAPON_SUBSCUD,
    WEAPON_TTANKZAP,
    WEAPON_APTUSK,
    WEAPON_DEMO_CHARGE,
    WEAPON_AIR_ASSAULT,

    WEAPON_COUNT,

    WEAPON_FIRST = WEAPON_COLT45,
    WEAPON_NONE = -1,
};

DEFINE_ENUMERATION_OPERATORS(WeaponType);

class GameINIClass;
class WarheadTypeClass;
class BulletTypeClass;

class WeaponTypeClass
{
public:
    WeaponTypeClass(const char *name);
    WeaponTypeClass(WeaponType weapon, const char *name);
    WeaponTypeClass(const WeaponTypeClass &that);
    WeaponTypeClass(const NoInitClass &noinit) {}
    ~WeaponTypeClass();

    void *operator new(size_t size);
    void *operator new(size_t size, void *ptr) { return ptr; }
    void operator delete(void *ptr);
#ifndef COMPILER_WATCOM // Watcom doesn't like this, MSVC/GCC does.
    void operator delete(void *ptr, void *place) {}
#endif

    void Code_Pointers() {}
    void Decode_Pointers() {}

    BOOL Read_INI(GameINIClass &ini);
    ThreatType Allowed_Threats() const;
    BOOL Is_Wall_Destroyer() const;
    WarheadTypeClass *Get_Warhead() const { return m_Warhead; }
    BulletTypeClass *Get_Projectile() const { return m_Projectile; }
    const char *Get_Name() const { return m_Name; }
    WeaponType What_Type() const { return (WeaponType)m_Type; }
    int Get_Damage() const { return m_Damage; }
    lepton_t Get_Range() const { return m_Range; }
    int Get_ROF() const { return m_ROF; }

    static void Init_Heap();
    static WeaponType From_Name(const char *name);
    static const char *Name_From(WeaponType weapon);
    static WeaponTypeClass &As_Reference(WeaponType weapon);
    static WeaponTypeClass *As_Pointer(WeaponType weapon);

private:
    int m_Type; // WeaponType
    const char *m_Name;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_TurboBoost : 1;
    BOOL m_Supress : 1;
    BOOL m_Camera : 1;
    BOOL m_Charges : 1;
#else
    bool m_TurboBoost; // Should the weapon get a boosted speed bonus when firing upon aircraft?
    bool m_Supress; // Should nearby friendly buildings be scanned for and if found, discourage firing on target?
    bool m_Camera; // Reveals area around firer ?
    bool m_Charges; // Does it have charge-up-before-firing logic?
#endif
    int m_Burst; // Number of rapid succession shots from this weapon (def = 1)
    BulletTypeClass *m_Projectile; // Projectile characteristic to use
    int m_Damage; // The amount of damage (unattenuated) dealt with every bullet
    MPHType m_Speed; // Speed of projectile to target (100 is maximum)
    WarheadTypeClass *m_Warhead; // Warhead to attach to projectile
    int m_ROF; // Delay between shots in ticks [15 = 1 second at middle speed setting]
    lepton_t m_Range; // Maximum cell range in leptons
    VocType m_Report; // Sound to play when firing
    AnimType m_Anim; // Animation to display as a firing effect
};

#ifdef GAME_DLL
extern TFixedIHeapClass<WeaponTypeClass> &g_WeaponTypes;
#else
extern TFixedIHeapClass<WeaponTypeClass> g_WeaponTypes;
#endif

#endif // WEAPONTYPE_H
