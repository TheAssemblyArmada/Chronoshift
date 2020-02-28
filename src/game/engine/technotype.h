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
#include "weapontype.h"

class TechnoClass;
class GameINIClass;

class TechnoTypeClass : public ObjectTypeClass
{
public:
    TechnoTypeClass(RTTIType type, int id, int ui_name, const char *name, RemapType remap, int fire_offset_z,
        int pri_fire_off_x, int pri_fire_off_y, int sec_fire_off_x, int sec_fire_off_y, BOOL nominal,
        BOOL radar_invisible, BOOL selectable, BOOL legal_target, BOOL insignificant, BOOL is_immune, BOOL theater,
        BOOL turret, BOOL remapable, BOOL unk1, int rot_count, SpeedType speed);

    TechnoTypeClass(const TechnoTypeClass &that);
    TechnoTypeClass(const NoInitClass &noinit) : ObjectTypeClass(noinit) {}
    ~TechnoTypeClass() {}

    TechnoTypeClass &operator=(TechnoTypeClass &that);

    virtual int Get_Ownable() const;
    virtual int Cost_Of() const { return m_Cost; }
    virtual int Time_To_Build() const;
    virtual void *Get_Cameo_Data() const;
    virtual int Raw_Cost() const { return m_Cost; }
    virtual int Max_Passengers() const { return m_Passengers; }
    virtual int Repair_Cost() const;
    virtual int Repair_Step() const;
    virtual BOOL Read_INI(GameINIClass &ini);

    BOOL Write_INI(GameINIClass &ini) const;
    BOOL Is_Two_Shooter() const;
    BOOL Legal_Placement(cell_t cellnum) const;
    BOOL Is_Nominal() const { return m_IsNominal; }
    int Get_ThreatPosed() const { return m_ThreatPosed; }
    int Get_ThreatPoints() const { return m_ThreatPoints; }
    BOOL Is_Invisible() const { return m_IsInvisible; }
    BOOL Is_Leader() const { return m_IsLeader; }
    BOOL Is_Cloakable() const { return m_Cloakable; }
    BOOL Is_Theater() const { return m_IsTheater; }
    BOOL Is_Turret_Equipped() const { return m_IsTurretEquipped; }
    BOOL Is_Crewed() const { return m_IsCrewed; }
    BOOL Is_Repairable() const { return m_IsRepairable; }
    BOOL Is_Remapable() const { return m_IsRemapable; }
    int Get_Cost() const { return m_Cost; }
    SpeedType Get_Speed() const { return m_Speed; }
    int Get_Ammo() const { return m_Ammo; }
    RemapType Get_Remap() const { return m_Remap; }
    int Get_Sight() const { return m_Sight; }
    int Get_Guard_Range() const { return m_GuardRange; }
    int Get_TechLevel() const { return m_TechLevel; }
    uint32_t Get_Prerequisites() const { return m_Prerequisite; }
    MZoneType Get_Movement_Zone() const { return m_MovementZone; }
    int Get_ROT_Count() const { return m_ROTCount; }
    int Get_Rate_Of_Turn() const { return m_ROT; }
    MPHType Get_MPH() const { return m_MPH; }

    WeaponTypeClass *Get_Weapon(WeaponSlotType weapon) const
    {
        WeaponTypeClass *wptr = nullptr;
        switch (weapon) {
            default:
            case WEAPON_SLOT_PRIMARY:
                wptr = m_Primary;
                break;
            case WEAPON_SLOT_SECONDARY:
                wptr = m_Secondary;
                break;
        };
        return wptr;
    }

    inline void Get_Fire_Offsets(WeaponSlotType weapon, int &x, int &y, int &z) const
    {
        switch (weapon) {
            default:
            case WEAPON_SLOT_PRIMARY:
                x = m_PrimaryFireOffsetX;
                y = m_PrimaryFireOffsetY;
                break;
            case WEAPON_SLOT_SECONDARY:
                x = m_SecondaryFireOffsetX;
                y = m_SecondaryFireOffsetY;
                break;
        };
        z = m_FireOffsetZ;
    }

    TechnoClass *Techno_Create_One_Of(HouseClass *house) const
    {
        return reinterpret_cast<TechnoClass *>(Create_One_Of(house));
    }

    static void One_Time();

#ifdef GAME_DLL
    friend void Setup_Hooks();

    /*TechnoTypeClass *Hook_Ctor(RTTIType type, int id, int ui_name, const char *name, RemapType remap, int fire_offset_z,
        int pri_fire_off_x, int pri_fire_off_y, int sec_fire_off_x, int sec_fire_off_y, BOOL nominal, BOOL radar_invisible,
        BOOL selectable, BOOL legal_target, BOOL insignificant, BOOL is_immune, BOOL theater, BOOL turret, BOOL remapable,
        BOOL unk1, int rot_count, SpeedType speed)
    {
        return new (this) TechnoTypeClass(type,
            id,
            ui_name,
            name,
            remap,
            fire_offset_z,
            pri_fire_off_x,
            pri_fire_off_y,
            sec_fire_off_x,
            sec_fire_off_y,
            nominal,
            radar_invisible,
            selectable,
            legal_target,
            insignificant,
            is_immune,
            theater,
            turret,
            remapable,
            unk1,
            rot_count,
            speed);
    }*/

    BOOL Hook_Is_Two_Shooter() { return TechnoTypeClass::Is_Two_Shooter(); }
    BOOL Hook_Legal_Placement(cell_t cellnum) { return TechnoTypeClass::Legal_Placement(cellnum); }
    int Hook_Get_Ownable() { return TechnoTypeClass::Get_Ownable(); }
    int Hook_Time_To_Build() { return TechnoTypeClass::Time_To_Build(); }
    void *Hook_Get_Cameo_Data() { return TechnoTypeClass::Get_Cameo_Data(); }
    int Hook_Repair_Cost() { return TechnoTypeClass::Repair_Cost(); }
    int Hook_Repair_Step() { return TechnoTypeClass::Repair_Step(); }
    BOOL Hook_Read_INI(GameINIClass &ini) { return TechnoTypeClass::Read_INI(ini); }
#endif

protected:
    RemapType m_Remap;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_DoubleOwned : 1; // 1
    BOOL m_IsInvisible : 1; // 2
    BOOL m_IsLeader : 1; // 4
    BOOL m_IsScanner : 1; // 8
    BOOL m_IsNominal : 1; // 16
    BOOL m_IsTheater : 1; // 32
    BOOL m_IsTurretEquipped : 1; // 64
    BOOL m_IsRepairable : 1; // 128
    BOOL m_IsCrewed : 1; // 1
    BOOL m_IsRemapable : 1; // 2
    BOOL m_Cloakable : 1; // 4
    BOOL m_IsSelfHealing : 1; // 8
    BOOL m_Explodes : 1; // 16
#else
    bool m_DoubleOwned; // Can be built/owned by all countries in a multiplayer game (def = false)?
    bool m_IsInvisible; // confirmed or RadarVisible, depending on how its used.
    bool m_IsLeader; // 99.9% sure, its set when a object has a valid Primary with Damage greater than 0. see 00569CCA
    bool m_IsScanner; // In TS this is Sensors?
    bool m_IsNominal; // Always use the given name rather than generic "enemy object" (def = false)?
    bool m_IsTheater; // Does it have theater specific imagery (def = false)?
    bool m_IsTurretEquipped;
    bool m_IsRepairable;
    bool m_IsCrewed; // Does it contain a crew that can escape [never infantry] (def = false)?
    bool m_IsRemapable;
    bool m_Cloakable; // or NoMovingFire?
    bool m_IsSelfHealing; // Does the object heal automatically up to half strength (def = false)?
    bool m_Explodes; // Does it explode violently when destroyed [i.e., does it do collateral damage] (def = false)?
#endif
    MZoneType m_MovementZone;
    lepton_t m_GuardRange; // Distance to scan for enemies to attack in leptons (def = use weapon range)
    int m_Passengers; // Number of passengers it may carry (def = 0)
    int m_Sight; // Sight range, in cells (def=1)
    int m_Cost; // Cost to build object (in credits) (def = 0)
    int m_TechLevel; // Tech level required to build this [-1 means can't build] (def = -1)
    uint32_t m_Prerequisite; // List of buildings needed before this can be manufactured (def = no requirement)
    int m_ThreatPosed;
    int m_ThreatPoints;
    MPHType m_MPH; // Speed of the unit, value relates to rules.ini value but is a transformed value.
    SpeedType m_Speed; // Movement speed type for purposes of adjustments.
    int m_Ammo; // Number of rounds carried between reloads [-1 means unlimited] (def = -1)
    uint32_t m_Owner; // bitfield of who can build objects of this type.
    void *m_CameoData; // Shape data for the side bar icon.
    int m_ROTCount; // Number of facing stages (def = 32)?
    int m_ROT; // Rate Of Turn for body (if present) and turret (if present) (def = 0)
    WeaponTypeClass *m_Primary; // Primary weapon equipped with (def = none)
    WeaponTypeClass *m_Secondary; // Secondary weapon equipped with (def = none)
    uint32_t m_FireOffsetZ;
    uint32_t m_PrimaryFireOffsetX;
    uint32_t m_PrimaryFireOffsetY;
    uint32_t m_SecondaryFireOffsetX;
    uint32_t m_SecondaryFireOffsetY;
    int m_Points;

#ifdef GAME_DLL
    static void *&s_WakeShapes;
    static void *&s_TurretShapes;
    static void *&s_SamShapes;
    static void *&s_MGunShapes;
#else
    static void *s_WakeShapes;
    static void *s_TurretShapes;
    static void *s_SamShapes;
    static void *s_MGunShapes;
#endif
    static void *s_LightningShapes; // NOTE: New addition for Chronoshift.
    static void *s_MissingCameoShape; // NOTE: New addition for Chronoshift.
};

#endif // TECHNOTYPE_H
