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
#include "bullettype.h"
#include "gameini.h"
#include "warheadtype.h"

#ifndef CHRONOSHIFT_STANDALONE
TFixedIHeapClass<WeaponTypeClass> &g_WeaponTypes = Make_Global<TFixedIHeapClass<WeaponTypeClass> >(0x0069164C);
#else
TFixedIHeapClass<WeaponTypeClass> g_WeaponTypes;
#endif

const WeaponTypeClass WeaponColt45(WEAPON_COLT45, "Colt45");
const WeaponTypeClass WeaponZSU23(WEAPON_ZSU23, "ZSU-23");
const WeaponTypeClass WeaponVulcan(WEAPON_VULCAN, "Vulcan");
const WeaponTypeClass WeaponMaverick(WEAPON_MAVERICK, "Maverick");
const WeaponTypeClass WeaponCamera(WEAPON_CAMERA, "Camera");
const WeaponTypeClass WeaponFireballLauncher(WEAPON_FIREBALL_LAUNCHER, "FireballLauncher");
const WeaponTypeClass WeaponSniper(WEAPON_SNIPER, "Sniper");
const WeaponTypeClass WeaponChainGun(WEAPON_CHAINGUN, "ChainGun");
const WeaponTypeClass WeaponPistol(WEAPON_PISTOL, "Pistol");
const WeaponTypeClass WeaponM1Carbine(WEAPON_M1CARBINE, "M1Carbine");
const WeaponTypeClass WeaponDragon(WEAPON_DRAGON, "Dragon");
const WeaponTypeClass WeaponHellfire(WEAPON_HELLFIRE, "Hellfire");
const WeaponTypeClass WeaponGrenade(WEAPON_GRENADE, "Grenade");
const WeaponTypeClass Weapon75mm(WEAPON_75MM, "75mm");
const WeaponTypeClass Weapon90mm(WEAPON_90MM, "90mm");
const WeaponTypeClass Weapon105mm(WEAPON_105MM, "105mm");
const WeaponTypeClass Weapon120mm(WEAPON_120MM, "120mm");
const WeaponTypeClass WeaponTurretGun(WEAPON_TURRET_GUN, "TurretGun");
const WeaponTypeClass WeaponMammothTusk(WEAPON_MAMMOTH_TUSK, "MammothTusk");
const WeaponTypeClass Weapon155mm(WEAPON_155MM, "155mm");
const WeaponTypeClass WeaponM60mg(WEAPON_M60MG, "M60mg");
const WeaponTypeClass WeaponNapalm(WEAPON_NAPALM, "Napalm");
const WeaponTypeClass WeaponTeslaZap(WEAPON_TESLAZAP, "TeslaZap");
const WeaponTypeClass WeaponNike(WEAPON_NIKE, "Nike");
const WeaponTypeClass Weapon8Inch(WEAPON_8INCH, "8Inch");
const WeaponTypeClass WeaponStinger(WEAPON_STINGER, "Stinger");
const WeaponTypeClass WeaponTorpTube(WEAPON_TORPTUBE, "TorpTube");
const WeaponTypeClass Weapon2Inch(WEAPON_2INCH, "2Inch");
const WeaponTypeClass WeaponDepthCharge(WEAPON_DEPTH_CHARGE, "DepthCharge");
const WeaponTypeClass WeaponParaBomb(WEAPON_PARABOMB, "ParaBomb");
const WeaponTypeClass WeaponDogJaw(WEAPON_DOGJAW, "DogJaw");
const WeaponTypeClass WeaponHeal(WEAPON_HEAL, "Heal");
const WeaponTypeClass WeaponSCUD(WEAPON_SCUD, "SCUD");
const WeaponTypeClass WeaponFlamer(WEAPON_FLAMER, "Flamer");
const WeaponTypeClass WeaponRedEye(WEAPON_REDEYE, "RedEye");
const WeaponTypeClass WeaponMandible(WEAPON_MANDIBLE, "Mandible");
const WeaponTypeClass WeaponPortaTesla(WEAPON_PORTATESLA, "PortaTesla");
const WeaponTypeClass WeaponGoodWrench(WEAPON_GOOD_WRENCH, "GoodWrench");
const WeaponTypeClass WeaponSubSCUD(WEAPON_SUBSCUD, "SubSCUD");
const WeaponTypeClass WeaponTTankZap(WEAPON_TTANKZAP, "TTankZap");
const WeaponTypeClass WeaponAPTusk(WEAPON_APTUSK, "APTusk");
const WeaponTypeClass WeaponDemocharge(WEAPON_DEMO_CHARGE, "Democharge");
const WeaponTypeClass WeaponAirAssault(WEAPON_AIR_ASSAULT, "AirAssault");

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
    return weapon != WEAPON_NONE && weapon < WEAPON_COUNT ? As_Reference(weapon).Get_Name() : "<none>";
}

/**
 * @brief Get reference to object from type enum value.
 */
WeaponTypeClass &WeaponTypeClass::As_Reference(WeaponType weapon)
{
    return g_WeaponTypes[weapon];
}

/**
 * @brief Get pointer to object from type enum value.
 *
 * 0x0058FE44
 */
WeaponTypeClass *WeaponTypeClass::As_Pointer(WeaponType weapon)
{
    if (weapon != WEAPON_NONE && weapon < WEAPON_COUNT) {
        return &g_WeaponTypes[weapon];
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
    if (ini.Find_Section(Get_Name()) != nullptr) {
        Supress = ini.Get_Bool(Get_Name(), "Supress", Supress);
        Burst = ini.Get_Int(Get_Name(), "Burst", Burst);
        Damage = ini.Get_Int(Get_Name(), "Damage", Damage);
        Speed = ini.Get_MPHType(Get_Name(), "Speed", Speed);
        ROF = ini.Get_Int(Get_Name(), "ROF", ROF);
        Range = ini.Get_Lepton(Get_Name(), "Range", Range);
        Report = ini.Get_VocType(Get_Name(), "Report", Report);
        Anim = ini.Get_AnimType(Get_Name(), "Anim", Anim);
        Camera = ini.Get_Bool(Get_Name(), "Camera", Camera);
        Charges = ini.Get_Bool(Get_Name(), "Charges", Charges);
        TurboBoost = ini.Get_Bool(Get_Name(), "TurboBoost", TurboBoost);
        Warhead = WarheadTypeClass::As_Pointer(ini.Get_WarheadType(
            Get_Name(), "Warhead", (Warhead != nullptr ? WarheadType(Warhead->What_Type()) : WARHEAD_NONE)));
        Projectile = BulletTypeClass::As_Pointer(ini.Get_BulletType(
            Get_Name(), "Projectile", (Projectile != nullptr ? BulletType(Projectile->What_Type()) : BULLET_NONE)));

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
