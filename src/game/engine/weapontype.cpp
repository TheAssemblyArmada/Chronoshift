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
    m_Type(weapon),
    m_Name(name),
    m_TurboBoost(false),
    m_Supress(false),
    m_Camera(false),
    m_Charges(false),
    m_Burst(1),
    m_Projectile(nullptr),
    m_Damage(0),
    m_Speed(MPH_MIN),
    m_Warhead(nullptr),
    m_ROF(0),
    m_Range(0),
    m_Report(VOC_NONE),
    m_Anim(ANIM_NONE)
{
}

WeaponTypeClass::~WeaponTypeClass()
{
    m_Name = nullptr;
    m_Projectile = nullptr;
    m_Warhead = nullptr;
}

WeaponTypeClass::WeaponTypeClass(WeaponTypeClass const &that) :
    m_Type(that.m_Type),
    m_Name(that.m_Name),
    m_TurboBoost(that.m_TurboBoost),
    m_Supress(that.m_Supress),
    m_Camera(that.m_Camera),
    m_Charges(that.m_Charges),
    m_Burst(that.m_Burst),
    m_Projectile(that.m_Projectile),
    m_Damage(that.m_Damage),
    m_Speed(that.m_Speed),
    m_Warhead(that.m_Warhead),
    m_ROF(that.m_ROF),
    m_Range(that.m_Range),
    m_Report(that.m_Report),
    m_Anim(that.m_Anim)
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
    captainslog_assert(name != nullptr);

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
    return weapon != WEAPON_NONE && weapon < WEAPON_COUNT ? As_Reference(weapon).m_Name : "<none>";
}

/**
 * @brief Get reference to object from type enum value.
 */
WeaponTypeClass &WeaponTypeClass::As_Reference(WeaponType type)
{
    captainslog_assert(type !=  WEAPON_NONE);
    captainslog_assert(type < WEAPON_COUNT);

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
    if (ini.Find_Section(m_Name) != nullptr) {
        m_Supress = ini.Get_Bool(m_Name, "Supress", m_Supress);
        m_Burst = ini.Get_Int(m_Name, "Burst", m_Burst);
        m_Damage = ini.Get_Int(m_Name, "Damage", m_Damage);
        m_Speed = ini.Get_MPHType(m_Name, "Speed", m_Speed);
        m_ROF = ini.Get_Int(m_Name, "ROF", m_ROF);
        m_Range = ini.Get_Lepton(m_Name, "Range", m_Range);
        m_Report = ini.Get_VocType(m_Name, "Report", m_Report);
        m_Anim = ini.Get_AnimType(m_Name, "Anim", m_Anim);
        m_Camera = ini.Get_Bool(m_Name, "Camera", m_Camera);
        m_Charges = ini.Get_Bool(m_Name, "Charges", m_Charges);
        m_TurboBoost = ini.Get_Bool(m_Name, "TurboBoost", m_TurboBoost);
        m_Warhead = WarheadTypeClass::As_Pointer(ini.Get_WarheadType(
            m_Name, "Warhead", (m_Warhead != nullptr ? m_Warhead->What_Type() : WARHEAD_NONE)));
        m_Projectile = BulletTypeClass::As_Pointer(ini.Get_BulletType(
            m_Name, "Projectile", (m_Projectile != nullptr ? m_Projectile->What_Type() : BULLET_NONE)));

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

    if (m_Projectile->Is_Anti_Air()) {
        threat = THREAT_AIRCRAFT;
    }

    if (m_Projectile->Is_Anti_Ground()) {
        threat |= (THREAT_INFANTRY | THREAT_VEHICLES | THREAT_BUILDINGS | THREAT_VESSELS);
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
    return m_Warhead != nullptr && m_Warhead->Is_Wall_Destroyer();
}
