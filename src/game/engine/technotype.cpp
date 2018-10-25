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
#include "ccfileclass.h"
#include "cell.h"
#include "iomap.h"
#include "lists.h"
#include "mixfile.h"
#include "rules.h"
#include "session.h"

#ifndef CHRONOSHIFT_STANDALONE
void *&TechnoTypeClass::WakeShapes = Make_Global<void *>(0x0068D2DC);
void *&TechnoTypeClass::TurretShapes = Make_Global<void *>(0x0068D2E0);
void *&TechnoTypeClass::SamShapes = Make_Global<void *>(0x0068D2E4);
void *&TechnoTypeClass::MGunShapes = Make_Global<void *>(0x0068D2E8);
#else
void *TechnoTypeClass::WakeShapes = nullptr;
void *TechnoTypeClass::TurretShapes = nullptr;
void *TechnoTypeClass::SamShapes = nullptr;
void *TechnoTypeClass::MGunShapes = nullptr;
#endif
void *TechnoTypeClass::LightningShapes = nullptr;
void *TechnoTypeClass::MissingCameoShape = nullptr;

/**
 * 0x00569564
 */
TechnoTypeClass::TechnoTypeClass(RTTIType type, int id, int uiname, const char *name, RemapType altremap, int def_fire_coord,
    int pri_fire_coord_a, int pri_fire_coord_b, int sec_fire_coord_a, int sec_fire_coord_b, BOOL nominal,
    BOOL radar_invisible, BOOL selectable, BOOL legal_target, BOOL insignificant, BOOL is_immune, BOOL theater, BOOL turret,
    BOOL remapable, BOOL logical, int rot_count, SpeedType speed) :
    ObjectTypeClass(
        type, id, nominal, radar_invisible, selectable, legal_target, insignificant, is_immune, logical, uiname, name),
    Remap(altremap),
    DoubleOwned(false),
    IsInvisible(false),
    IsLeader(false),
    IsScanner(false),
    IsNominal(nominal),
    IsTheater(theater),
    IsTurretEquipped(turret),
    IsRepairable(true),
    IsCrewed(false),
    IsRemapable(remapable),
    Cloakable(false),
    IsSelfHealing(false),
    Explodes(false),
    MovementZone(MZONE_NORMAL),
    Passengers(0),
    Sight(0),
    Cost(0),
    TechLevel(0),
    Prerequisite(0),
    ThreatPosed(0),
    ThreatPoints(0),
    MPH((MPHType)0),
    Speed(speed),
    Ammo(-1),
    Owner(OWNER_NONE),
    CameoData(nullptr),
    ROTCount(rot_count),
    ROT(0),
    Primary(nullptr),
    Secondary(nullptr),
    DefualtFireCoord(def_fire_coord),
    PrimaryFireCoordA(pri_fire_coord_a),
    PrimaryFireCoordB(pri_fire_coord_b),
    SecondaryFireCoordA(sec_fire_coord_a),
    SecondaryFireCoordB(sec_fire_coord_b),
    Points(0)
{
}

TechnoTypeClass::TechnoTypeClass(const TechnoTypeClass &that) :
    ObjectTypeClass(that),
    Remap(that.Remap),
    DoubleOwned(that.DoubleOwned),
    IsInvisible(that.IsInvisible),
    IsLeader(that.IsLeader),
    IsScanner(that.IsScanner),
    IsNominal(that.IsNominal),
    IsTheater(that.IsTheater),
    IsTurretEquipped(that.IsTurretEquipped),
    IsRepairable(that.IsRepairable),
    IsCrewed(that.IsCrewed),
    IsRemapable(that.IsRemapable),
    Cloakable(that.Cloakable),
    IsSelfHealing(that.IsSelfHealing),
    Explodes(that.Explodes),
    MovementZone(that.MovementZone),
    GuardRange(that.GuardRange),
    Passengers(that.Passengers),
    Sight(that.Sight),
    Cost(that.Cost),
    TechLevel(that.TechLevel),
    Prerequisite(that.Prerequisite),
    ThreatPosed(that.ThreatPosed),
    ThreatPoints(that.ThreatPoints),
    MPH(that.MPH),
    Speed(that.Speed),
    Owner(that.Owner),
    CameoData(that.CameoData),
    ROTCount(that.ROTCount),
    ROT(that.ROT),
    Primary(that.Primary),
    Secondary(that.Secondary),
    DefualtFireCoord(that.DefualtFireCoord),
    PrimaryFireCoordA(that.PrimaryFireCoordA),
    PrimaryFireCoordB(that.PrimaryFireCoordB),
    SecondaryFireCoordA(that.SecondaryFireCoordA),
    SecondaryFireCoordB(that.SecondaryFireCoordB),
    Points(that.Points)
{
}

TechnoTypeClass &TechnoTypeClass::operator=(TechnoTypeClass &that)
{
    if (this != &that) {
        ObjectTypeClass::operator=(that);
        Remap = that.Remap;
        DoubleOwned = that.DoubleOwned;
        IsInvisible = that.IsInvisible;
        IsLeader = that.IsLeader;
        IsScanner = that.IsScanner;
        IsNominal = that.IsNominal;
        IsTheater = that.IsTheater;
        IsTurretEquipped = that.IsTurretEquipped;
        IsRepairable = that.IsRepairable;
        IsCrewed = that.IsCrewed;
        IsRemapable = that.IsRemapable;
        Cloakable = that.Cloakable;
        IsSelfHealing = that.IsSelfHealing;
        Explodes = that.Explodes;
        MovementZone = that.MovementZone;
        GuardRange = that.GuardRange;
        Passengers = that.Passengers;
        Sight = that.Sight;
        Cost = that.Cost;
        TechLevel = that.TechLevel;
        Prerequisite = that.Prerequisite;
        ThreatPosed = that.ThreatPosed;
        ThreatPoints = that.ThreatPoints;
        MPH = that.MPH;
        Speed = that.Speed;
        Owner = that.Owner;
        CameoData = that.CameoData;
        ROTCount = that.ROTCount;
        ROT = that.ROT;
        Primary = that.Primary;
        Secondary = that.Secondary;
        DefualtFireCoord = that.DefualtFireCoord;
        PrimaryFireCoordA = that.PrimaryFireCoordA;
        PrimaryFireCoordB = that.PrimaryFireCoordB;
        SecondaryFireCoordA = that.SecondaryFireCoordA;
        SecondaryFireCoordB = that.SecondaryFireCoordB;
        Points = that.Points;
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
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)(const TechnoTypeClass *) = reinterpret_cast<BOOL (*)(const TechnoTypeClass *)>(0x005698E8);
    return func(this);
#elif 0
    return Primary != nullptr && (Primary == Secondary || Primary->Burst > 1);
#else
    return false;
#endif
}

/**
 * @brief Would this object be legally places at given cellnum?
 *
 * 0x00569E30
 */
BOOL TechnoTypeClass::Legal_Placement(int16_t cellnum) const
{
    if (cellnum != -1) {
        for (const int16_t *list_pos = Occupy_List(true); list_pos[0] == LIST_END; ++list_pos) {
            int16_t offset_cellnum = (list_pos[0] + cellnum);

            if (!Map.In_Radar(offset_cellnum)) {
                return false;
            }

            CellClass &cell = Map[offset_cellnum];

            if (What_Am_I() == RTTI_BUILDINGTYPE) {
                if (!cell.Is_Clear_To_Build(Speed)) {
                    return false;
                }

            } else {
                if (!cell.Is_Clear_To_Move(Speed, false, false, -1, MZONE_NORMAL)) {
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
    if (DoubleOwned && Session.Game_To_Play() != GAME_CAMPAIGN) {
        return Owner | (SIDE_ALLIES | SIDE_SOVIET);
    }

    return Owner;
}

/**
 * @brief Returns build time (in ticks or seconds?).
 *
 * 0x00569784
 */
int TechnoTypeClass::Time_To_Build() const
{
    return fixed(900, 1000) * Cost * Rule.Get_Build_Speed();
}

/**
 * @brief Gets the sidebar icon shape data for this object.
 *
 * 0x005697E8
 */
void *TechnoTypeClass::Get_Cameo_Data() const
{
    if (CameoData != nullptr) {
        return CameoData;
    }

    // Enhancement backported from TS.
    if (MissingCameoShape != nullptr) {
        return MissingCameoShape;
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
        return (Raw_Cost() / (Strength / Rule.Get_Unit_Repair_Step()))
            * Rule.Get_Unit_Repair_Percent(); // this code check if the result is greater than 1(.0), if so,
                                              // forces 1, see for TS/RA2 version
                                              // http://dc.strategy-x.com/src2/TechnoTypeClass/GetRepairStepCost.cpp needs
                                              // recoding for use with fixed *see (U)RepairPercent type.
    }

    return (Raw_Cost() / (Strength / Rule.Get_Repair_Step()))
        * Rule.Get_Repair_Percent(); // same as above, but this is normal repair percent for buildings.
}

/**
 * @brief Gets how much is repaired per repair "tick".
 *
 * 0x005698AC
 */
int TechnoTypeClass::Repair_Step() const
{
    if (Is_FootType()) {
        return Rule.Get_Unit_Repair_Step();
    }

    return Rule.Get_Repair_Step();
}

/**
 * @brief Reads the information from the objects ini entry that pertains to the TechnoType layer.
 *
 * 0x00569914
 */
BOOL TechnoTypeClass::Read_INI(GameINIClass &ini)
{
    // TODO Requires WeaponTypeClass and more functions in GameINIClass
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)
    (const TechnoTypeClass *, GameINIClass &) = reinterpret_cast<BOOL (*)(const TechnoTypeClass *, GameINIClass &)>(0x00569914);
    return func(this, ini);
#elif 0
    if (ObjectTypeClass::Read_INI(ini)) {
        DoubleOwned = ini.Get_Bool(Get_Name(), "DoubleOwned", DoubleOwned);
        GuardRange = ini.Get_Lepton(Get_Name(), "GuardRange", GuardRange);
        Explodes = ini.Get_Bool(Get_Name(), "Explodes", Explodes);
        Primary =
            WeaponTypeClass::As_Pointer(ini.Get_WeaponType(Get_Name(), "Primary", (Primary ? Primary->Type : WEAPON_NONE)));
        Secondary = WeaponTypeClass::As_Pointer(
            ini.Get_WeaponType(Get_Name(), "Secondary", (Secondary ? Secondary->Type : WEAPON_NONE)));
        Cloakable = ini.Get_Bool(Get_Name(), "Cloakable", Cloakable);
        IsScanner = ini.Get_Bool(Get_Name(), "Sensors", IsScanner);
        Prerequisite = ini.Get_Buildings(Get_Name(), "Prerequisite", Prerequisite);
        Sight = ini.Get_Int(Get_Name(), "Sight", Sight);
        TechLevel = ini.Get_Int(Get_Name(), "TechLevel", TechLevel);

        // MPH
        // TODO, two versions of code, maybe we need to fix Get_MPH?, see CCINI
        fixed def(MPH, 256);
        int value = ini.Get_MPHType(Get_Name(), "Speed", MPH);

        if (value != -1) {
            value = Bound(value, 0, 100);

            int retval = (value * 255) / 100;

            if (retval >= 255) {
                retval = -1;
            }

            MPH = (MPHType)retval;
        }

        Cost = ini.Get_Int(Get_Name(), "Cost", Cost);
        Ammo = ini.Get_Int(Get_Name(), "Ammo", Ammo);
        Points = ini.Get_Int(Get_Name(), "Points", Points);
        Owner = ini.Get_Owners(Get_Name(), "Owner", Owner);
        IsCrewed = ini.Get_Bool(Get_Name(), "Crewed", IsCrewed);
        IsRepairable = ini.Get_Bool(Get_Name(), "Repairable", IsRepairable);
        IsInvisible = ini.Get_Bool(Get_Name(), "Invisible", IsInvisible);
        IsSelfHealing = ini.Get_Bool(Get_Name(), "SelfHealing", IsSelfHealing);
        ROT = ini.Get_Int(Get_Name(), "ROT", ROT);
        Passengers = ini.Get_Int(Get_Name(), "Passengers", Passengers);
        
        ThreatPoints = Points;

        if (Primary != nullptr && Primary->Damage > 0) {
            IsLeader = true;
        } else {
            IsLeader = false;
        }
        if (Primary && Primary->Warhead && Primary->Warhead->Wall) { // need confirming
            MovementZone = MZONE_DESTROYER;
        }

        if (Speed == SPEED_FLOAT) {
            MovementZone = MZONE_AMPHIBIOUS_DESTROYER;
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
    // NOTE, Moved from UnitTypeClass as it was actually setting TechnoType statics. Call from UnitTypeClass One_Time().
    if (!WakeShapes) {
        WakeShapes = MixFileClass<GameFileClass>::Retrieve("wake.shp");
        DEBUG_ASSERT(WakeShapes != nullptr);
    }
    if (!TurretShapes) {
        TurretShapes = MixFileClass<GameFileClass>::Retrieve("turr.shp");
        DEBUG_ASSERT(TurretShapes != nullptr);
    }
    if (!SamShapes) {
        SamShapes = MixFileClass<GameFileClass>::Retrieve("ssam.shp");
        DEBUG_ASSERT(SamShapes != nullptr);
    }
    if (!MGunShapes) {
        MGunShapes = MixFileClass<GameFileClass>::Retrieve("mgun.shp");
        DEBUG_ASSERT(MGunShapes != nullptr);
    }
    if (!LightningShapes) {
        LightningShapes = MixFileClass<GameFileClass>::Retrieve("litning.shp");
        DEBUG_ASSERT(LightningShapes != nullptr);
    }
    if (!MissingCameoShape) {
        MissingCameoShape = MixFileClass<GameFileClass>::Retrieve("xxicon.shp");
        DEBUG_ASSERT(MissingCameoShape != nullptr);
    }
}
