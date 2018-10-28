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
#include "gameini.h"
#include "gamefile.h"
#include "mixfile.h"
#include <cstdio>

using std::snprintf;

#ifndef CHRONOSHIFT_STANDALONE
TFixedIHeapClass<BulletTypeClass> &BulletTypes = *reinterpret_cast<TFixedIHeapClass<BulletTypeClass> *>(0x0065DE54);
#else
TFixedIHeapClass<BulletTypeClass> BulletTypes;
#endif

// If you want to add a bullet type, add it hear, to the enum in bullettype.h and Init_Heap further down. Order must match.
const BulletTypeClass BulletInvisible(BULLET_INVISIBLE, "Invisible");
const BulletTypeClass BulletCannon(BULLET_CANNON, "Cannon");
const BulletTypeClass BulletAck(BULLET_ACK, "Ack");
const BulletTypeClass BulletTorpedo(BULLET_TORPEDO, "Torpedo");
const BulletTypeClass BulletFROG(BULLET_FROG, "FROG");
const BulletTypeClass BulletHeatSeeker(BULLET_HEATSEEKER, "HeatSeeker");
const BulletTypeClass BulletLaserGuided(BULLET_LASERGUIDED, "LaserGuided");
const BulletTypeClass BulletLobbed(BULLET_LOBBED, "Lobbed");
const BulletTypeClass BulletBomblet(BULLET_BOMBLET, "Bomblet");
const BulletTypeClass BulletBallistic(BULLET_BALLISTIC, "Ballistic");
const BulletTypeClass BulletParachute(BULLET_PARACHUTE, "Parachute");
const BulletTypeClass BulletFireball(BULLET_FIREBALL, "Fireball");
const BulletTypeClass BulletLeapDog(BULLET_LEAPDOG, "LeapDog");
const BulletTypeClass BulletCatapult(BULLET_CATAPULT, "Catapult");
const BulletTypeClass BulletAAMissile(BULLET_AAMISSILE, "AAMissile");
const BulletTypeClass BulletGPSSatellite(BULLET_GPSSATELLITE, "GPSSatellite");
const BulletTypeClass BulletNukeUp(BULLET_NUKEUP, "NukeUp");
const BulletTypeClass BulletNukeDown(BULLET_NUKEDOWN, "NukeDown");

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
    Rotates(true),
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
    // Bit64 = true;	//TS shows what classes set this to true, needs debuging once logics are up and running.
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
    Rotates(that.Rotates),
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
            const char *name = bptr->ImageName[0] != '\0' ? bptr->ImageName : bptr->Get_Name();
            snprintf(filename, sizeof(filename), "%s.shp", name);
            bptr->ImageData = GameFileClass::Retrieve_File(filename);
        }
    }
}

/**
 * 0x00426AB0
 */
void *BulletTypeClass::operator new(size_t size)
{
    return BulletTypes.Allocate();
}

/**
 * 0x00426AC4
 */
void BulletTypeClass::operator delete(void *ptr)
{
    BulletTypes.Free(ptr);
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
    return bullet != BULLET_NONE && bullet < BULLET_COUNT ? As_Reference(bullet).Get_Name() : "<none>";
}

/**
 * @brief Get reference to object from enum value.
 *
 * 0x00426D04
 */
BulletTypeClass &BulletTypeClass::As_Reference(BulletType bullet)
{
    return BulletTypes[bullet];
}

/**
 * @brief Get pointer to object from enum value.
 */
BulletTypeClass *BulletTypeClass::As_Pointer(BulletType bullet)
{
    return bullet < BULLET_COUNT && bullet != BULLET_NONE ? &BulletTypes[bullet] : nullptr;
}

/**
 * @brief Reads object data from an INI file.
 *
 * 0x00426D1C
 */
BOOL BulletTypeClass::Read_INI(GameINIClass &ini)
{
    if (ObjectTypeClass::Read_INI(ini)) {
        Arm = ini.Get_Int(Get_Name(), "Arm", 0);
        ROT = ini.Get_Int(Get_Name(), "ROT", 0);
        Frames = ini.Get_Int(Get_Name(), "Frames", 1);
        High = ini.Get_Bool(Get_Name(), "High", false);
        Shadow = ini.Get_Bool(Get_Name(), "Shadow", true);
        Arcing = ini.Get_Bool(Get_Name(), "Arcing", false);
        Dropping = ini.Get_Bool(Get_Name(), "Dropping", false);
        Inviso = ini.Get_Bool(Get_Name(), "Inviso", false);
        Proximity = ini.Get_Bool(Get_Name(), "Proximity", false);
        Animates = ini.Get_Bool(Get_Name(), "Animates", false);
        Ranged = ini.Get_Bool(Get_Name(), "Ranged", false);
        Inaccurate = ini.Get_Bool(Get_Name(), "Inaccuate", false);
        AntiAir = ini.Get_Bool(Get_Name(), "AA", false);
        AntiGround = ini.Get_Bool(Get_Name(), "AG", true);
        AntiSubWarfare = ini.Get_Bool(Get_Name(), "ASW", false);
        Degenerates = ini.Get_Bool(Get_Name(), "Degenerates", false);
        UnderWater = ini.Get_Bool(Get_Name(), "UnderWater", false);
        Parachuted = ini.Get_Bool(Get_Name(), "Parachuted", false);
        Rotates = ini.Get_Bool(Get_Name(), "Rotates", false);
        Translucent = ini.Get_Bool(Get_Name(), "Translucent", false);
        Gigundo = ini.Get_Bool(Get_Name(), "Gigundo", false);

        return true;
    }

    return false;
}
