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
    High(false),
    Shadow(true),
    Arcing(false),
    Dropping(false),
    Inviso(false),
    Proximity(false),
    Animates(false),
    Ranged(false),
    NoRotate(true),
    Inaccurate(false),
    Translucent(false),
    AntiAir(false),
    AntiGround(true),
    AntiSubWarfare(false),
    Degenerates(false),
    UnderWater(false),
    Parachuted(false),
    Gigundo(false),
    Type(bullet), // This was originally HeapID and the ctor only took name.
    ROT(0),
    Arm(0),
    Frames(0)
{
    // Bit64 = true;    //TS shows what classes set this to true, needs debuging once logics are up and running.
    // Bit128 = false;
}

BulletTypeClass::BulletTypeClass(BulletTypeClass const &that) :
    ObjectTypeClass(that),
    High(that.High),
    Shadow(that.Shadow),
    Arcing(that.Arcing),
    Dropping(that.Dropping),
    Inviso(that.Inviso),
    Proximity(that.Proximity),
    Animates(that.Animates),
    Ranged(that.Ranged),
    NoRotate(that.NoRotate),
    Inaccurate(that.Inaccurate),
    Translucent(that.Translucent),
    AntiAir(that.AntiAir),
    AntiGround(that.AntiGround),
    AntiSubWarfare(that.AntiSubWarfare),
    Degenerates(that.Degenerates),
    UnderWater(that.UnderWater),
    Parachuted(that.Parachuted),
    Gigundo(that.Gigundo),
    Type(that.Type),
    ROT(that.ROT),
    Arm(that.Arm),
    Frames(that.Frames)
{
}

/**
 * @brief Initialises the object heap for this class.
 *
 * 0x00426ADC
 */
void BulletTypeClass::Init_Heap()
{
    // In the original binary, these where created with just names. this has been changed so all Bullets are created globaly
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

        if (!bptr->Inviso) {
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
BulletTypeClass &BulletTypeClass::As_Reference(BulletType bullet)
{
    return g_BulletTypes[bullet];
}

/**
 * @brief Get pointer to object from enum value.
 */
BulletTypeClass *BulletTypeClass::As_Pointer(BulletType bullet)
{
    return bullet < BULLET_COUNT && bullet != BULLET_NONE ? &g_BulletTypes[bullet] : nullptr;
}

/**
 * @brief Reads object data from an INI file.
 *
 * 0x00426D1C
 */
BOOL BulletTypeClass::Read_INI(GameINIClass &ini)
{
    if (ObjectTypeClass::Read_INI(ini)) {
        Arm = ini.Get_Int(Get_Name(), "Arm", Arm);
        ROT = ini.Get_Int(Get_Name(), "ROT", ROT);
        Frames = ini.Get_Int(Get_Name(), "Frames", Frames);
        High = ini.Get_Bool(Get_Name(), "High", High);
        Shadow = ini.Get_Bool(Get_Name(), "Shadow", Shadow);
        Arcing = ini.Get_Bool(Get_Name(), "Arcing", Arcing);
        Dropping = ini.Get_Bool(Get_Name(), "Dropping", Dropping);
        Inviso = ini.Get_Bool(Get_Name(), "Inviso", Inviso);
        Proximity = ini.Get_Bool(Get_Name(), "Proximity", Proximity);
        Animates = ini.Get_Bool(Get_Name(), "Animates", Animates);
        Ranged = ini.Get_Bool(Get_Name(), "Ranged", Ranged);
        Inaccurate = ini.Get_Bool(Get_Name(), "Inaccuate", Inaccurate);
        AntiAir = ini.Get_Bool(Get_Name(), "AA", AntiAir);
        AntiGround = ini.Get_Bool(Get_Name(), "AG", AntiGround);
        AntiSubWarfare = ini.Get_Bool(Get_Name(), "ASW", AntiSubWarfare);
        Degenerates = ini.Get_Bool(Get_Name(), "Degenerates", Degenerates);
        UnderWater = ini.Get_Bool(Get_Name(), "UnderWater", UnderWater);
        Parachuted = ini.Get_Bool(Get_Name(), "Parachuted", Parachuted);
        NoRotate = !ini.Get_Bool(Get_Name(), "Rotates", !NoRotate);
        Translucent = ini.Get_Bool(Get_Name(), "Translucent", Translucent);
        Gigundo = ini.Get_Bool(Get_Name(), "Gigundo", Gigundo);

        return true;
    }

    return false;
}
