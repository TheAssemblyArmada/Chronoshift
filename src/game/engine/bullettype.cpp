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
#include "bullettype.h"
#include "bulletdata.h"
#include "gameini.h"
#include "gamefile.h"
#include "mixfile.h"
#include <cstdio>

using std::snprintf;

#ifndef GAME_DLL
TFixedIHeapClass<BulletTypeClass> g_BulletTypes;
#endif

/**
 * @brief
 *
 * 0x00426A20 Prototypes don't match but it fulfills same purpose, do not hook.
 */
BulletTypeClass::BulletTypeClass(BulletType bullet, const char *name) :
    ObjectTypeClass(RTTI_BULLETTYPE, bullet, true, true, false, false, true, true, false, TXT_NULL, name),
    m_High(false),
    m_Shadow(true),
    m_Arcing(false),
    m_Dropping(false),
    m_Inviso(false),
    m_Proximity(false),
    m_Animates(false),
    m_Ranged(false),
    m_NoRotate(true),
    m_Inaccurate(false),
    m_Translucent(false),
    m_AntiAir(false),
    m_AntiGround(true),
    m_AntiSubWarfare(false),
    m_Degenerates(false),
    m_UnderWater(false),
    m_Parachuted(false),
    m_Gigundo(false),
    m_Type(bullet), // This was originally HeapID and the ctor only took name.
    m_ROT(0),
    m_Arm(0),
    m_Frames(0)
{
    // Bit64 = true;    //TS shows what classes set this to true, needs debuging once logics are up and running.
    // Bit128 = false;
}

BulletTypeClass::BulletTypeClass(BulletTypeClass const &that) :
    ObjectTypeClass(that),
    m_High(that.m_High),
    m_Shadow(that.m_Shadow),
    m_Arcing(that.m_Arcing),
    m_Dropping(that.m_Dropping),
    m_Inviso(that.m_Inviso),
    m_Proximity(that.m_Proximity),
    m_Animates(that.m_Animates),
    m_Ranged(that.m_Ranged),
    m_NoRotate(that.m_NoRotate),
    m_Inaccurate(that.m_Inaccurate),
    m_Translucent(that.m_Translucent),
    m_AntiAir(that.m_AntiAir),
    m_AntiGround(that.m_AntiGround),
    m_AntiSubWarfare(that.m_AntiSubWarfare),
    m_Degenerates(that.m_Degenerates),
    m_UnderWater(that.m_UnderWater),
    m_Parachuted(that.m_Parachuted),
    m_Gigundo(that.m_Gigundo),
    m_Type(that.m_Type),
    m_ROT(that.m_ROT),
    m_Arm(that.m_Arm),
    m_Frames(that.m_Frames)
{
}

/**
 * @brief Initialises the object heap for this class.
 *
 * 0x00426ADC
 */
void BulletTypeClass::Init_Heap()
{
    // In the original binary, these where created with just names.
    // This has been changed so all Bullets are created globaly
    // and initialised like all other game objects.
    // Order of construction must match enum in bullettypes.h
    new BulletTypeClass(BulletInvisible);
    new BulletTypeClass(BulletCannon);
    new BulletTypeClass(BulletAck);
    new BulletTypeClass(BulletTorpedo);
    new BulletTypeClass(BulletFROG);
    new BulletTypeClass(BulletHeatSeeker);
    new BulletTypeClass(BulletLaserGuided);
    new BulletTypeClass(BulletLobbed);
    new BulletTypeClass(BulletBomblet);
    new BulletTypeClass(BulletBallistic);
    new BulletTypeClass(BulletParachute);
    new BulletTypeClass(BulletFireball);
    new BulletTypeClass(BulletLeapDog);
    new BulletTypeClass(BulletCatapult);
    new BulletTypeClass(BulletAAMissile);
    new BulletTypeClass(BulletGPSSatellite);
    new BulletTypeClass(BulletNukeUp);
    new BulletTypeClass(BulletNukeDown);
}

/**
 * @brief Carries out one time initialisation of persistant data.
 *
 * 0x00426C98
 */
void BulletTypeClass::One_Time()
{
    char filename[512];

    for (BulletType bullet = BULLET_FIRST; bullet < BULLET_COUNT; ++bullet) {
        BulletTypeClass *bptr = As_Pointer(bullet);

        if (!bptr->m_Inviso) {
            const char *name = bptr->ImageName[0] != '\0' ? bptr->ImageName : bptr->m_Name;
            snprintf(filename, sizeof(filename), "%s.shp", name);
            bptr->ImageData = GameFileClass::Retrieve(filename);
        }
    }
}

/**
 * 0x00426AB0
 */
void *BulletTypeClass::operator new(size_t size)
{
    return g_BulletTypes.Allocate();
}

/**
 * 0x00426AC4
 */
void BulletTypeClass::operator delete(void *ptr)
{
    g_BulletTypes.Free(ptr);
}

/**
 * @brief Get type enum value from a string.
 */
BulletType BulletTypeClass::From_Name(const char *name)
{
    DEBUG_ASSERT(name != nullptr);

    if (strcasecmp(name, "<none>") == 0 || strcasecmp(name, "none") == 0) {
        return BULLET_NONE;
    }

    if (name != nullptr) {
        for (BulletType bullet = BULLET_FIRST; bullet < BULLET_COUNT; ++bullet) {
            if (strcasecmp(name, Name_From(bullet)) == 0) {
                return bullet;
            }
        }
    }
    return BULLET_NONE;
}

/**
 * @brief Get string from enum value.
 */
const char *BulletTypeClass::Name_From(BulletType bullet)
{
    return bullet != BULLET_NONE && bullet < BULLET_COUNT ? As_Reference(bullet).m_Name : "<none>";
}

/**
 * @brief Get reference to object from enum value.
 *
 * 0x00426D04
 */
BulletTypeClass &BulletTypeClass::As_Reference(BulletType type)
{
    DEBUG_ASSERT(type != BULLET_NONE);
    DEBUG_ASSERT(type < BULLET_COUNT);

    return g_BulletTypes[type];
}

/**
 * @brief Get pointer to object from enum value.
 */
BulletTypeClass *BulletTypeClass::As_Pointer(BulletType type)
{
    return type < BULLET_COUNT && type != BULLET_NONE ? &g_BulletTypes[type] : nullptr;
}

/**
 * @brief Reads object data from an INI file.
 *
 * 0x00426D1C
 */
BOOL BulletTypeClass::Read_INI(GameINIClass &ini)
{
    if (ObjectTypeClass::Read_INI(ini)) {
        m_Arm = ini.Get_Int(Get_Name(), "Arm", m_Arm);
        m_ROT = ini.Get_Int(Get_Name(), "ROT", m_ROT);
        m_Frames = ini.Get_Int(Get_Name(), "Frames", m_Frames);
        m_High = ini.Get_Bool(Get_Name(), "High", m_High);
        m_Shadow = ini.Get_Bool(Get_Name(), "Shadow", m_Shadow);
        m_Arcing = ini.Get_Bool(Get_Name(), "Arcing", m_Arcing);
        m_Dropping = ini.Get_Bool(Get_Name(), "Dropping", m_Dropping);
        m_Inviso = ini.Get_Bool(Get_Name(), "Inviso", m_Inviso);
        m_Proximity = ini.Get_Bool(Get_Name(), "Proximity", m_Proximity);
        m_Animates = ini.Get_Bool(Get_Name(), "Animates", m_Animates);
        m_Ranged = ini.Get_Bool(Get_Name(), "Ranged", m_Ranged);
        m_Inaccurate = ini.Get_Bool(Get_Name(), "Inaccuate", m_Inaccurate);
        m_AntiAir = ini.Get_Bool(Get_Name(), "AA", m_AntiAir);
        m_AntiGround = ini.Get_Bool(Get_Name(), "AG", m_AntiGround);
        m_AntiSubWarfare = ini.Get_Bool(Get_Name(), "ASW", m_AntiSubWarfare);
        m_Degenerates = ini.Get_Bool(Get_Name(), "Degenerates", m_Degenerates);
        m_UnderWater = ini.Get_Bool(Get_Name(), "UnderWater", m_UnderWater);
        m_Parachuted = ini.Get_Bool(Get_Name(), "Parachuted", m_Parachuted);
        m_NoRotate = !ini.Get_Bool(Get_Name(), "Rotates", !m_NoRotate);
        m_Translucent = ini.Get_Bool(Get_Name(), "Translucent", m_Translucent);
        m_Gigundo = ini.Get_Bool(Get_Name(), "Gigundo", m_Gigundo);

        return true;
    }

    return false;
}
