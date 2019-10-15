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
#include "weapontype.h"
#include "weapondata.h"
#include "bullettype.h"
#include "gameini.h"
#include "warheadtype.h"

#ifndef GAME_DLL
TFixedIHeapClass<WeaponTypeClass> g_WeaponTypes;
#endif

/**
 * Global template ctor differs intentionally from the binary, don't change unless
 * you are sure you know why they are different and that your changes won't break
 * anything.
 *
 * 0x0058FD98 Prototypes don't match but it fulfills same purpose, do not hook.
 */
WeaponTypeClass::WeaponTypeClass(WeaponType weapon, const char *name) :
    Type(weapon),
    Name(name),
    TurboBoost(false),
    Supress(false),
    Camera(false),
    Charges(false),
    Burst(1),
    Projectile(nullptr),
    Damage(0),
    Speed(MPH_MIN),
    Warhead(nullptr),
    ROF(0),
    Range(0),
    Report(VOC_NONE),
    Anim(ANIM_NONE)
{
}

WeaponTypeClass::~WeaponTypeClass()
{
    Name = nullptr;
    Projectile = nullptr;
    Warhead = nullptr;
}

WeaponTypeClass::WeaponTypeClass(WeaponTypeClass const &that) :
    Type(that.Type),
    Name(that.Name),
    TurboBoost(that.TurboBoost),
    Supress(that.Supress),
    Camera(that.Camera),
    Charges(that.Charges),
    Burst(that.Burst),
    Projectile(that.Projectile),
    Damage(that.Damage),
    Speed(that.Speed),
    Warhead(that.Warhead),
    ROF(that.ROF),
    Range(that.Range),
    Report(that.Report),
    Anim(that.Anim)
{
}

/**
 * 0x0058FE18
 */
void *WeaponTypeClass::operator new(size_t size)
{
    return g_WeaponTypes.Alloc();
}

/**
 * 0x0058FE2C
 */
void WeaponTypeClass::operator delete(void *ptr)
{
    g_WeaponTypes.Free(ptr);
}

/**
 * @brief This initialisation happend in RuleClass::Heap_Maximums in the original game.
 */
void WeaponTypeClass::Init_Heap()
{
    // in the binary, all weapon types are initliased in RulesClass::Heap_Maximums("") and
    // do not have global class instance, so i have moved them here for continuity.
    new WeaponTypeClass(WeaponColt45);
    new WeaponTypeClass(WeaponZSU23);
    new WeaponTypeClass(WeaponVulcan);
    new WeaponTypeClass(WeaponMaverick);
    new WeaponTypeClass(WeaponCamera);
    new WeaponTypeClass(WeaponFireballLauncher);
    new WeaponTypeClass(WeaponSniper);
    new WeaponTypeClass(WeaponChainGun);
    new WeaponTypeClass(WeaponPistol);
    new WeaponTypeClass(WeaponM1Carbine);
    new WeaponTypeClass(WeaponDragon);
    new WeaponTypeClass(WeaponHellfire);
    new WeaponTypeClass(WeaponGrenade);
    new WeaponTypeClass(Weapon75mm);
    new WeaponTypeClass(Weapon90mm);
    new WeaponTypeClass(Weapon105mm);
    new WeaponTypeClass(Weapon120mm);
    new WeaponTypeClass(WeaponTurretGun);
    new WeaponTypeClass(WeaponMammothTusk);
    new WeaponTypeClass(Weapon155mm);
    new WeaponTypeClass(WeaponM60mg);
    new WeaponTypeClass(WeaponNapalm);
    new WeaponTypeClass(WeaponTeslaZap);
    new WeaponTypeClass(WeaponNike);
    new WeaponTypeClass(Weapon8Inch);
    new WeaponTypeClass(WeaponStinger);
    new WeaponTypeClass(WeaponTorpTube);
    new WeaponTypeClass(Weapon2Inch);
    new WeaponTypeClass(WeaponDepthCharge);
    new WeaponTypeClass(WeaponParaBomb);
    new WeaponTypeClass(WeaponDogJaw);
    new WeaponTypeClass(WeaponHeal);
    new WeaponTypeClass(WeaponSCUD);
    new WeaponTypeClass(WeaponFlamer);
    new WeaponTypeClass(WeaponRedEye);
    new WeaponTypeClass(WeaponMandible);
    new WeaponTypeClass(WeaponPortaTesla);
    new WeaponTypeClass(WeaponGoodWrench);
    new WeaponTypeClass(WeaponSubSCUD);
    new WeaponTypeClass(WeaponTTankZap);
    new WeaponTypeClass(WeaponAPTusk);
    new WeaponTypeClass(WeaponDemocharge);
    new WeaponTypeClass(WeaponAirAssault);
}

/**
 * @brief Get weapon type enum from string.
 */
WeaponType WeaponTypeClass::From_Name(const char *name)
{
    DEBUG_ASSERT(name != nullptr);

    if (strcasecmp(name, "<none>") == 0 || strcasecmp(name, "none") == 0) {
        return WEAPON_NONE;
    }

    if (name != nullptr) {
        for (WeaponType weapon = WEAPON_FIRST; weapon < WEAPON_COUNT; ++weapon) {
            if (strcasecmp(name, Name_From(weapon)) == 0) {
                return weapon;
            }
        }
    }

    return WEAPON_NONE;
}

/**
 * @brief Get weapon name string from type enum.
 */
const char *WeaponTypeClass::Name_From(WeaponType weapon)
{
    return weapon != WEAPON_NONE && weapon < WEAPON_COUNT ? As_Reference(weapon).Name : "<none>";
}

/**
 * @brief Get reference to object from type enum value.
 */
WeaponTypeClass &WeaponTypeClass::As_Reference(WeaponType type)
{
    DEBUG_ASSERT(type !=  WEAPON_NONE);
    DEBUG_ASSERT(type < WEAPON_COUNT);

    return g_WeaponTypes[type];
}

/**
 * @brief Get pointer to object from type enum value.
 *
 * 0x0058FE44
 */
WeaponTypeClass *WeaponTypeClass::As_Pointer(WeaponType type)
{
    if (type != WEAPON_NONE && type < WEAPON_COUNT) {
        return &g_WeaponTypes[type];
    }

    return nullptr;
}

/**
 * @brief Read object information from ini file.
 *
 * 0x0058FE64
 */
BOOL WeaponTypeClass::Read_INI(GameINIClass &ini)
{
    if (ini.Find_Section(Name) != nullptr) {
        Supress = ini.Get_Bool(Name, "Supress", Supress);
        Burst = ini.Get_Int(Name, "Burst", Burst);
        Damage = ini.Get_Int(Name, "Damage", Damage);
        Speed = ini.Get_MPHType(Name, "Speed", Speed);
        ROF = ini.Get_Int(Name, "ROF", ROF);
        Range = ini.Get_Lepton(Name, "Range", Range);
        Report = ini.Get_VocType(Name, "Report", Report);
        Anim = ini.Get_AnimType(Name, "Anim", Anim);
        Camera = ini.Get_Bool(Name, "Camera", Camera);
        Charges = ini.Get_Bool(Name, "Charges", Charges);
        TurboBoost = ini.Get_Bool(Name, "TurboBoost", TurboBoost);
        Warhead = WarheadTypeClass::As_Pointer(ini.Get_WarheadType(
            Name, "Warhead", (Warhead != nullptr ? Warhead->What_Type() : WARHEAD_NONE)));
        Projectile = BulletTypeClass::As_Pointer(ini.Get_BulletType(
            Name, "Projectile", (Projectile != nullptr ? Projectile->What_Type() : BULLET_NONE)));

        return true;
    }

    return false;
}

/**
 * @brief Calculate which threats this weapon can target based on projectile characteristics.
 *
 * 0x005900F8
 */
ThreatType WeaponTypeClass::Allowed_Threats() const
{
    ThreatType threat = THREAT_ANY;

    if (Projectile->Is_Anti_Air()) {
        threat = THREAT_AIRCRAFT;
    }

    if (Projectile->Is_Anti_Ground()) {
        threat |= (THREAT_INFANTRY | THREAT_VEHICLE | THREAT_BUILDING | THREAT_VESSEL);
    }

    return threat;
}

/**
 * @brief Determine if this weapon can destroy walls based on the warhead characteristics.
 *
 * 0x0059012C
 */
BOOL WeaponTypeClass::Is_Wall_Destroyer() const
{
    return Warhead != nullptr && Warhead->Is_Wall_Destroyer();
}
