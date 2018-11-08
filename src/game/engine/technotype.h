/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Techno layer of type classes, basis for all player unit type classes.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef TECHNOTYPE_H
#define TECHNOTYPE_H

#include "always.h"
#include "gametypes.h"
#include "land.h"
#include "mzone.h"
#include "objecttype.h"
#include "remap.h"

class TechnoClass;
class GameINIClass;

class TechnoTypeClass : public ObjectTypeClass
{
public:
    TechnoTypeClass(RTTIType type, int id, int uiname, const char *name, RemapType remap, int def_fire_coord,
        int pri_fire_coord_a, int pri_fire_coord_b, int sec_fire_coord_a, int sec_fire_coord_b, BOOL crushable,
        BOOL radar_invisible, BOOL selectable, BOOL legal_target, BOOL insignificant, BOOL is_immune, BOOL theater,
        BOOL turret, BOOL remapable, BOOL logical, int facings, SpeedType speed);

    TechnoTypeClass(TechnoTypeClass const &that);
    TechnoTypeClass(NoInitClass const &noinit) : ObjectTypeClass(noinit) {}
    ~TechnoTypeClass() {} // TODO not virtual in original binary, consider making it so after standalone?

    TechnoTypeClass &operator=(TechnoTypeClass &that);

    virtual int Get_Ownable() const;
    virtual int Cost_Of() const { return Cost; }
    virtual int Time_To_Build() const;
    virtual void *Get_Cameo_Data() const;
    virtual int Raw_Cost() const { return Cost; }
    virtual int Max_Passengers() const { return Passengers; }
    virtual int Repair_Cost() const;
    virtual int Repair_Step() const;
    virtual BOOL Read_INI(GameINIClass &ini);

    BOOL Write_INI(GameINIClass &ini) const;
    BOOL Is_Two_Shooter() const;
    BOOL Legal_Placement(int16_t cellnum) const;
    BOOL Is_Nominal() const { return IsNominal; } 
    int Get_ThreatPosed() const { return ThreatPosed; }
    BOOL Is_Invisible() const { return IsInvisible; }
    int Get_Cost() const { return Cost; }

    TechnoClass *Techno_Create_One_Of(HouseClass *house) const { return reinterpret_cast<TechnoClass *>(Create_One_Of(house)); }

    static void One_Time();

protected:
    RemapType Remap;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool DoubleOwned : 1; // 1 Can be built/owned by all countries in a multiplayer game (def = false)?
            bool IsInvisible : 1; // 2 confirmed or RadarVisible, depending on how its used.
            bool IsLeader : 1; // 4 99.9% sure, its set when a object has a valid Primary with Damage greater than 0.
            bool IsScanner : 1; // 8 In TS this is Sensors?
            bool IsNominal : 1; // 16 Always use the given name rather than generic "enemy object" (def = false)?
            bool IsTheater : 1; // 32 Does it have theater specific imagery (def = false)?
            bool IsTurretEquipped : 1; // 64
            bool IsRepairable : 1; // 128
            bool IsCrewed : 1; // 1 Does it contain a crew that can escape [never infantry] (def = false)?
            bool IsRemapable : 1; // 2
            bool Cloakable : 1; // 4 or NoMovingFire?
            bool IsSelfHealing : 1; // 8 Does the object heal automatically up to half strength (def = false)?
            bool Explodes : 1; // 16 Does it explode violently when destroyed(def = false)?
        };
        int Bitfield;
    };
#else
    bool DoubleOwned; // Can be built/owned by all countries in a multiplayer game (def = false)?
    bool IsInvisible; // confirmed or RadarVisible, depending on how its used.
    bool IsLeader; // 99.9% sure, its set when a object has a valid Primary with Damage greater than 0. see 00569CCA
    bool IsScanner; // In TS this is Sensors?
    bool IsNominal; // Always use the given name rather than generic "enemy object" (def = false)?
    bool IsTheater; // Does it have theater specific imagery (def = false)?
    bool IsTurretEquipped;
    bool IsRepairable;
    bool IsCrewed; // Does it contain a crew that can escape [never infantry] (def = false)?
    bool IsRemapable;
    bool Cloakable; // or NoMovingFire?
    bool IsSelfHealing; // Does the object heal automatically up to half strength (def = false)?
    bool Explodes; // Does it explode violently when destroyed [i.e., does it do collateral damage] (def = false)?
#endif
    MZoneType MovementZone;
    int16_t GuardRange; // Distance to scan for enemies to attack in leptons (def = use weapon range)
    int Passengers; // Number of passengers it may carry (def = 0)
    int Sight; // Sight range, in cells (def=1)
    int Cost; // Cost to build object (in credits) (def = 0)
    int TechLevel; // Tech level required to build this [-1 means can't build] (def = -1)
    uint32_t Prerequisite; // List of buildings needed before this can be manufactured (def = no requirement)
    int ThreatPosed;
    int ThreatPoints;
    MPHType MPH; // Speed of the unit, value relates to rules.ini value but is a transformed value.
    SpeedType Speed; // Movement speed type for purposes of adjustments.
    int Ammo; // Number of rounds carried between reloads [-1 means unlimited] (def = -1)
    uint32_t Owner; // bitfield of who can build objects of this type.
    void *CameoData; // Shape data for the side bar icon.
    int ROTCount; // Number of facing stages (def = 32)?
    int ROT; // Rate Of Turn for body (if present) and turret (if present) (def = 0)
    void *Primary; // TODO Should be WeaponTypeClass *// Primary weapon equipped with (def = none)
    void *Secondary; // TODO Should be WeaponTypeClass *// Secondary weapon equipped with (def = none)
    uint32_t DefualtFireCoord;
    uint32_t PrimaryFireCoordA;
    uint32_t PrimaryFireCoordB;
    uint32_t SecondaryFireCoordA;
    uint32_t SecondaryFireCoordB;
    int Points;

#ifndef CHRONOSHIFT_STANDALONE
    static void *&WakeShapes;
    static void *&TurretShapes;
    static void *&SamShapes;
    static void *&MGunShapes;
#else
    static void *WakeShapes;
    static void *TurretShapes;
    static void *SamShapes;
    static void *MGunShapes;
#endif
    static void *LightningShapes; // New with RA++
    static void *MissingCameoShape; // New with RA++
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
#endif

#endif // TECHNOTYPE_H
