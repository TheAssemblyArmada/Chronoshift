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
#include "technotype.h"
#include "gamefile.h"
#include "cell.h"
#include "iomap.h"
#include "lists.h"
#include "mixfile.h"
#include "rules.h"
#include "session.h"

#ifndef GAME_DLL
void *TechnoTypeClass::s_WakeShapes = nullptr;
void *TechnoTypeClass::s_TurretShapes = nullptr;
void *TechnoTypeClass::s_SamShapes = nullptr;
void *TechnoTypeClass::s_MGunShapes = nullptr;
#endif
void *TechnoTypeClass::s_LightningShapes = nullptr;
void *TechnoTypeClass::s_MissingCameoShape = nullptr;

/**
 * 0x00569564
 */
TechnoTypeClass::TechnoTypeClass(RTTIType type, int id, int ui_name, const char *name, RemapType altremap, int fire_offset_z,
    int pri_fire_off_x, int pri_fire_off_y, int sec_fire_off_x, int sec_fire_off_y, BOOL nominal,
    BOOL radar_invisible, BOOL selectable, BOOL legal_target, BOOL insignificant, BOOL is_immune, BOOL theater, BOOL turret,
    BOOL remapable, BOOL unk1, int rot_count, SpeedType speed) :
    ObjectTypeClass(
        type, id, true, radar_invisible, selectable, legal_target, insignificant, is_immune, unk1, ui_name, name),
    m_Remap(altremap),
    m_DoubleOwned(false),
    m_IsInvisible(false),
    m_IsLeader(false),
    m_IsScanner(false),
    m_IsNominal(nominal),
    m_IsTheater(theater),
    m_IsTurretEquipped(turret),
    m_IsRepairable(true),
    m_IsCrewed(false),
    m_IsRemapable(remapable),
    m_Cloakable(false),
    m_IsSelfHealing(false),
    m_Explodes(false),
    m_MovementZone(MZONE_NORMAL),
    m_GuardRange(0),
    m_Passengers(0),
    m_Sight(0),
    m_Cost(0),
    m_TechLevel(-1),
    m_Prerequisite(0),
    m_ThreatPosed(0),
    m_ThreatPoints(0),
    m_MPH((MPHType)0),
    m_Speed(speed),
    m_Ammo(-1),
    m_Owner(OWNER_NONE),
    m_CameoData(nullptr),
    m_ROTCount(rot_count),
    m_ROT(0),
    m_Primary(nullptr),
    m_Secondary(nullptr),
    m_FireOffsetZ(fire_offset_z),
    m_PrimaryFireOffsetX(pri_fire_off_x),
    m_PrimaryFireOffsetY(pri_fire_off_y),
    m_SecondaryFireOffsetX(sec_fire_off_x),
    m_SecondaryFireOffsetY(sec_fire_off_y),
    m_Points(0)
{
}

TechnoTypeClass::TechnoTypeClass(const TechnoTypeClass &that) :
    ObjectTypeClass(that),
    m_Remap(that.m_Remap),
    m_DoubleOwned(that.m_DoubleOwned),
    m_IsInvisible(that.m_IsInvisible),
    m_IsLeader(that.m_IsLeader),
    m_IsScanner(that.m_IsScanner),
    m_IsNominal(that.m_IsNominal),
    m_IsTheater(that.m_IsTheater),
    m_IsTurretEquipped(that.m_IsTurretEquipped),
    m_IsRepairable(that.m_IsRepairable),
    m_IsCrewed(that.m_IsCrewed),
    m_IsRemapable(that.m_IsRemapable),
    m_Cloakable(that.m_Cloakable),
    m_IsSelfHealing(that.m_IsSelfHealing),
    m_Explodes(that.m_Explodes),
    m_MovementZone(that.m_MovementZone),
    m_GuardRange(that.m_GuardRange),
    m_Passengers(that.m_Passengers),
    m_Sight(that.m_Sight),
    m_Cost(that.m_Cost),
    m_TechLevel(that.m_TechLevel),
    m_Prerequisite(that.m_Prerequisite),
    m_ThreatPosed(that.m_ThreatPosed),
    m_ThreatPoints(that.m_ThreatPoints),
    m_MPH(that.m_MPH),
    m_Speed(that.m_Speed),
    m_Owner(that.m_Owner),
    m_CameoData(that.m_CameoData),
    m_ROTCount(that.m_ROTCount),
    m_ROT(that.m_ROT),
    m_Primary(that.m_Primary),
    m_Secondary(that.m_Secondary),
    m_FireOffsetZ(that.m_FireOffsetZ),
    m_PrimaryFireOffsetX(that.m_PrimaryFireOffsetX),
    m_PrimaryFireOffsetY(that.m_PrimaryFireOffsetY),
    m_SecondaryFireOffsetX(that.m_SecondaryFireOffsetX),
    m_SecondaryFireOffsetY(that.m_SecondaryFireOffsetY),
    m_Points(that.m_Points)
{
}

TechnoTypeClass &TechnoTypeClass::operator=(TechnoTypeClass &that)
{
    if (this != &that) {
        ObjectTypeClass::operator=(that);
        m_Remap = that.m_Remap;
        m_DoubleOwned = that.m_DoubleOwned;
        m_IsInvisible = that.m_IsInvisible;
        m_IsLeader = that.m_IsLeader;
        m_IsScanner = that.m_IsScanner;
        m_IsNominal = that.m_IsNominal;
        m_IsTheater = that.m_IsTheater;
        m_IsTurretEquipped = that.m_IsTurretEquipped;
        m_IsRepairable = that.m_IsRepairable;
        m_IsCrewed = that.m_IsCrewed;
        m_IsRemapable = that.m_IsRemapable;
        m_Cloakable = that.m_Cloakable;
        m_IsSelfHealing = that.m_IsSelfHealing;
        m_Explodes = that.m_Explodes;
        m_MovementZone = that.m_MovementZone;
        m_GuardRange = that.m_GuardRange;
        m_Passengers = that.m_Passengers;
        m_Sight = that.m_Sight;
        m_Cost = that.m_Cost;
        m_TechLevel = that.m_TechLevel;
        m_Prerequisite = that.m_Prerequisite;
        m_ThreatPosed = that.m_ThreatPosed;
        m_ThreatPoints = that.m_ThreatPoints;
        m_MPH = that.m_MPH;
        m_Speed = that.m_Speed;
        m_Owner = that.m_Owner;
        m_CameoData = that.m_CameoData;
        m_ROTCount = that.m_ROTCount;
        m_ROT = that.m_ROT;
        m_Primary = that.m_Primary;
        m_Secondary = that.m_Secondary;
        m_FireOffsetZ = that.m_FireOffsetZ;
        m_PrimaryFireOffsetX = that.m_PrimaryFireOffsetX;
        m_PrimaryFireOffsetY = that.m_PrimaryFireOffsetY;
        m_SecondaryFireOffsetX = that.m_SecondaryFireOffsetX;
        m_SecondaryFireOffsetY = that.m_SecondaryFireOffsetY;
        m_Points = that.m_Points;
    }

    return *this;
}

/**
 * @brief Does this object fire two shots per attack?
 *
 * 0x005698E8
 */
BOOL TechnoTypeClass::Is_Two_Shooter() const
{
    // TODO Requires WeaponTypeClass
#ifdef GAME_DLL
    BOOL (*func)(const TechnoTypeClass *) = reinterpret_cast<BOOL (*)(const TechnoTypeClass *)>(0x005698E8);
    return func(this);
#elif 0
    return m_Primary != nullptr && (m_Primary == m_Secondary || m_Primary->Burst > 1);
#else
    return false;
#endif
}

/**
 * @brief Would this object be legally places at given cellnum?
 *
 * 0x00569E30
 */
BOOL TechnoTypeClass::Legal_Placement(cell_t cellnum) const
{
    if (cellnum != -1) {
        for (const int16_t *list_pos = Occupy_List(true); list_pos[0] == LIST_END; ++list_pos) {
            cell_t offset_cellnum = (list_pos[0] + cellnum);

            if (!g_Map.In_Radar(offset_cellnum)) {
                return false;
            }

            CellClass &cell = g_Map[offset_cellnum];

            if (What_Am_I() == RTTI_BUILDINGTYPE) {
                if (!cell.Is_Clear_To_Build(m_Speed)) {
                    return false;
                }

            } else {
                if (!cell.Is_Clear_To_Move(m_Speed, false, false, -1, MZONE_NORMAL)) {
                    return false;
                }
            }
        }

        return true;
    }

    return false;
}

/**
 * @brief Returns bitfield of allowed owners for this object.
 *
 * 0x00569754
 */
int TechnoTypeClass::Get_Ownable() const
{
    if (m_DoubleOwned && g_Session.Game_To_Play() != GAME_CAMPAIGN) {
        return m_Owner | (SIDE_ALLIES | SIDE_SOVIET);
    }

    return m_Owner;
}

/**
 * @brief Returns build time (in ticks or seconds?).
 *
 * 0x00569784
 */
int TechnoTypeClass::Time_To_Build() const
{
    return fixed_t(900, 1000) * m_Cost * g_Rule.Get_Build_Speed();
}

/**
 * @brief Gets the sidebar icon shape data for this object.
 *
 * 0x005697E8
 */
void *TechnoTypeClass::Get_Cameo_Data() const
{
    if (m_CameoData != nullptr) {
        return m_CameoData;
    }

    // Enhancement backported from TS.
    if (s_MissingCameoShape != nullptr) {
        return s_MissingCameoShape;
    }

    return nullptr;
}

/**
 * @brief Gets cost of repairing this object in credits.
 *
 * 0x005697F8
 */
int TechnoTypeClass::Repair_Cost() const
{
    if (Is_FootType()) {
        return (Raw_Cost() / (m_Strength / g_Rule.Get_Unit_Repair_Step()))
            * g_Rule.Get_Unit_Repair_Percent(); // this code check if the result is greater than 1(.0), if so,
                                              // forces 1, see for TS/RA2 version
                                              // http://dc.strategy-x.com/src2/TechnoTypeClass/GetRepairStepCost.cpp needs
                                              // recoding for use with fixed *see (U)RepairPercent type.
    }

    return (Raw_Cost() / (m_Strength / g_Rule.Get_Repair_Step()))
        * g_Rule.Get_Repair_Percent(); // same as above, but this is normal repair percent for buildings.
}

/**
 * @brief Gets how much is repaired per repair "tick".
 *
 * 0x005698AC
 */
int TechnoTypeClass::Repair_Step() const
{
    if (Is_FootType()) {
        return g_Rule.Get_Unit_Repair_Step();
    }

    return g_Rule.Get_Repair_Step();
}

/**
 * @brief Reads the information from the objects ini entry that pertains to the TechnoType layer.
 *
 * 0x00569914
 */
BOOL TechnoTypeClass::Read_INI(GameINIClass &ini)
{
    // TODO Requires WeaponTypeClass and more functions in GameINIClass
#ifdef GAME_DLL
    BOOL (*func)
    (const TechnoTypeClass *, GameINIClass &) = reinterpret_cast<BOOL (*)(const TechnoTypeClass *, GameINIClass &)>(0x00569914);
    return func(this, ini);
#elif 0
    if (ObjectTypeClass::Read_INI(ini)) {
        m_DoubleOwned = ini.Get_Bool(m_Name, "DoubleOwned", m_DoubleOwned);
        m_GuardRange = ini.Get_Lepton(m_Name, "GuardRange", m_GuardRange);
        m_Explodes = ini.Get_Bool(m_Name, "Explodes", m_Explodes);
        m_Primary =
            WeaponTypeClass::As_Pointer(ini.Get_WeaponType(m_Name, "Primary", (m_Primary ? m_Primary->m_Type : WEAPON_NONE)));
        m_Secondary = WeaponTypeClass::As_Pointer(
            ini.Get_WeaponType(m_Name, "Secondary", (m_Secondary ? m_Secondary->m_Type : WEAPON_NONE)));
        m_Cloakable = ini.Get_Bool(m_Name, "Cloakable", m_Cloakable);
        m_IsScanner = ini.Get_Bool(m_Name, "Sensors", m_IsScanner);
        m_Prerequisite = ini.Get_Buildings(m_Name, "Prerequisite", m_Prerequisite);
        m_Sight = ini.Get_Int(m_Name, "Sight", m_Sight);
        m_TechLevel = ini.Get_Int(m_Name, "TechLevel", m_TechLevel);

        // MPH
        // TODO, two versions of code, maybe we need to fix Get_MPH?, see GameINIClass
        fixed_t def(m_MPH, 256);
        int value = ini.Get_MPHType(m_Name, "Speed", m_MPH);

        if (value != -1) {
            value = Bound(value, 0, 100);

            int retval = (value * 255) / 100;

            if (retval >= 255) {
                retval = -1;
            }

            m_MPH = (MPHType)retval;
        }

        m_Cost = ini.Get_Int(m_Name, "Cost", m_Cost);
        m_Ammo = ini.Get_Int(m_Name, "Ammo", m_Ammo);
        m_Points = ini.Get_Int(m_Name, "Points", m_Points);
        Owner = ini.Get_Owners(m_Name, "Owner", Owner);
        m_IsCrewed = ini.Get_Bool(m_Name, "Crewed", m_IsCrewed);
        m_IsRepairable = ini.Get_Bool(m_Name, "Repairable", m_IsRepairable);
        m_IsInvisible = ini.Get_Bool(m_Name, "Invisible", m_IsInvisible);
        m_IsSelfHealing = ini.Get_Bool(m_Name, "SelfHealing", m_IsSelfHealing);
        m_ROT = ini.Get_Int(m_Name, "ROT", m_ROT);
        m_Passengers = ini.Get_Int(m_Name, "Passengers", m_Passengers);
        
        m_ThreatPoints = m_Points;

        if (m_Primary != nullptr && m_Primary->Damage > 0) {
            m_IsLeader = true;
        } else {
            m_IsLeader = false;
        }
        if (m_Primary && m_Primary->Warhead && m_Primary->Warhead->m_Wall) { // need confirming
            m_MovementZone = MZONE_DESTROYER;
        }

        if (m_Speed == SPEED_FLOAT) {
            m_MovementZone = MZONE_AMPHIBIOUS_DESTROYER;
        }

        return true;
    }

    return false;
#else
    return false;
#endif
}

/**
 * @brief Writes the information to the objects ini entry that pertains to the TechnoType layer.
 */
BOOL TechnoTypeClass::Write_INI(GameINIClass &ini) const
{
    // TODO
    return false;
}

/**
 * @brief Performs one time initialisation of TechnoType related data.
 */
void TechnoTypeClass::One_Time()
{
    // NOTE: Moved from UnitTypeClass as it was actually setting
    // TechnoType statics. Called from UnitTypeClass One_Time().
    if (s_WakeShapes == nullptr) {
        s_WakeShapes = GameFileClass::Retrieve("wake.shp");
        captainslog_assert(s_WakeShapes != nullptr);
    }
    if (s_TurretShapes == nullptr) {
        s_TurretShapes = GameFileClass::Retrieve("turr.shp");
        captainslog_assert(s_TurretShapes != nullptr);
    }
    if (s_SamShapes == nullptr) {
        s_SamShapes = GameFileClass::Retrieve("ssam.shp");
        captainslog_assert(s_SamShapes != nullptr);
    }
    if (s_MGunShapes == nullptr) {
        s_MGunShapes = GameFileClass::Retrieve("mgun.shp");
        captainslog_assert(s_MGunShapes != nullptr);
    }
    if (s_LightningShapes == nullptr) {
        s_LightningShapes = GameFileClass::Retrieve("litning.shp");
        captainslog_assert(s_LightningShapes != nullptr);
    }
    if (s_MissingCameoShape == nullptr) {
        s_MissingCameoShape = GameFileClass::Retrieve("xxicon.shp");
    }
}
