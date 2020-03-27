/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Techno layer of the object class hierachy.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef TECHNO_H
#define TECHNO_H

#include "always.h"
#include "buildingtype.h"
#include "cargo.h"
#include "door.h"
#include "flasher.h"
#include "gameptr.h"
#include "gametypes.h"
#include "house.h"
#include "infantrytype.h"
#include "radio.h"
#include "stage.h"
#include "ttimer.h"

class BulletClass;

enum CloakState
{
    CLOAK_UNCLOAKED = 0,
    CLOAK_CLOAKING = 1,
    CLOAK_CLOAKED = 2,
    CLOAK_UNCLOAKING = 3
};

enum VisualType
{
    // VISUAL_NONE = -1,
    VISUAL_NORMAL,
    VISUAL_INDISTINCT,
    VISUAL_DARKEN,
    VISUAL_SHADOWY,
    VISUAL_RIPPLE,
    VISUAL_HIDDEN,
};

class TechnoClass : public RadioClass
{
public:
    TechnoClass(RTTIType type, int id, HousesType house);
    TechnoClass(const TechnoClass &that);
    TechnoClass(const NoInitClass &init);
    virtual ~TechnoClass();

#ifdef CHRONOSHIFT_DEBUG
#ifdef CHRONOSHIFT_STANDALONE
    virtual void Debug_Dump(MonoClass *mono) const override;
#else
    void Debug_Dump(MonoClass *mono) const;
#endif
#endif

    // AbstractClass
    virtual HousesType Owner() const override;
    virtual void AI() override;

    // ObjectClass
    virtual BOOL Is_Players_Army() const override;
    virtual ActionType What_Action(ObjectClass *object) const override;
    virtual ActionType What_Action(cell_t cellnum) const override;
    virtual int Get_Ownable() const override;
    virtual BOOL Can_Repair() const override;
    virtual BOOL Can_Player_Fire() const override;
    virtual BOOL Can_Player_Move() const override;
    virtual coord_t Fire_Coord(WeaponSlotType weapon = WEAPON_SLOT_PRIMARY) const override;
    virtual BOOL Unlimbo(coord_t coord, DirType dir = DIR_NORTH) override;
    virtual void Detach(target_t target, int a2) override;
    virtual void Record_The_Kill(TechnoClass *object = nullptr) override;
    virtual void Do_Shimmer() override;
    virtual int Exit_Object(TechnoClass *object) override;
    virtual void Draw_It(int x, int y, WindowNumberType window) const override;
    virtual void Hidden() override;
    virtual void Look(BOOL a1 = false) override;
    virtual BOOL Mark(MarkType mark) override;
    virtual void Clicked_As_Target(int flash_frames) override;
    virtual BOOL Select() override;
    virtual BOOL In_Range(coord_t coord, WeaponSlotType weapon = WEAPON_SLOT_PRIMARY) const override;
    virtual int Weapon_Range(WeaponSlotType weapon = WEAPON_SLOT_PRIMARY) const override;
    virtual DamageResultType Take_Damage(
        int &damage, int a2, WarheadType warhead, TechnoClass *object = nullptr, BOOL a5 = false) override;
    virtual int Value() const override;
    virtual void Per_Cell_Process(PCPType pcp) override;
    virtual RadioMessageType Receive_Message(RadioClass *radio, RadioMessageType message, target_t &target) override;
    virtual BOOL Revealed(HouseClass *house) override;
    virtual void Code_Pointers() override;
    virtual void Decode_Pointers() override;

    // MissionClass.
    virtual void Override_Mission(MissionType mission, int target1 = -1, int target2 = -1) override;
    virtual BOOL Restore_Mission() override;

    // TechnoClass
    virtual int How_Many_Survivors() const;
    virtual DirType Turret_Facing() const;
    virtual BuildingClass *Find_Docking_Bay(BuildingType building, int a2 = 0) const;
    virtual cell_t Find_Exit_Cell(TechnoClass *object) const;
    virtual DirType Desired_Load_Dir(ObjectClass *object, cell_t &cellnum) const;
    virtual DirType Fire_Direction() const;
    virtual InfantryType Crew_Type() const;
    virtual BOOL Is_Allowed_To_Recloak() const;
    virtual BOOL Is_Weapon_Equipped() const;
    virtual fixed_t Ore_Load() const { return fixed_t::_0_1; }
    virtual int Pip_Count() const { return 0; }
    virtual int Rearm_Delay(BOOL a1, WeaponSlotType weapon = WEAPON_SLOT_PRIMARY) const;
    virtual int Refund_Amount() const;
    virtual int Risk() const;
    virtual int Threat_Range(int a1) const;
    virtual void Response_Select();
    virtual void Response_Move();
    virtual void Response_Attack();
    virtual void Player_Assign_Mission(MissionType mission, target_t target = 0, target_t dest = 0);
    virtual int Made_A_Kill();
    virtual BOOL Target_Something_Nearby(ThreatType threat);
    virtual void Stun();
    virtual BOOL In_Range(target_t target, WeaponSlotType weapon = WEAPON_SLOT_PRIMARY) const;
    virtual BOOL In_Range(ObjectClass *object, WeaponSlotType weapon = WEAPON_SLOT_PRIMARY) const;
    virtual void Death_Announcement(TechnoClass *killer) const = 0;
    virtual FireErrorType Can_Fire(target_t target, WeaponSlotType weapon = WEAPON_SLOT_PRIMARY) const; // Queries if any specific state prevents the object from firing.
    virtual target_t Greatest_Threat(ThreatType threat);
    virtual void Assign_Target(target_t target);
    virtual BulletClass *Fire_At(target_t target, WeaponSlotType weapon = WEAPON_SLOT_PRIMARY);
    virtual BOOL Captured(HouseClass *house);
    virtual BOOL Electric_Zap(target_t target, BOOL a2, coord_t a3 = 0, uint8_t *a4 = nullptr);
    virtual void Renovate();
    virtual uint8_t *Remap_Table() const;
    virtual void Draw_Pips(int x, int y, WindowNumberType window) const;
    virtual void Do_Uncloak();
    virtual void Do_Cloak();
    virtual BOOL Is_Ready_To_Random_Animate() const;
    virtual BOOL Random_Animate();
    virtual void Assign_Destination(target_t dest);
    virtual void Enter_Idle_Mode(BOOL a1 = false);

    BOOL Visually_Unclear() const
    {
        VisualType visual = Visual_Character();
        return visual != VISUAL_NORMAL && visual != VISUAL_HIDDEN;
    }

    int Time_To_Build() const;
    int Is_Visible_On_Radar() const;
    fixed_t Area_Modify(cell_t cellnum);
    int Evaluate_Object(ThreatType threat, int scan, int intval2, TechnoClass *techno, int &distance, int zoneval);//TODO: for scan see ThreatScanType
    int Evaluate_Cell(ThreatType threat, int scan, cell_t cellnum, int intval2, TechnoClass **techno, int &distance, int intval3);//TODO: for scan see ThreatScanType
    void Cloaking_AI();
    int Is_Ready_To_Cloak() const;
    BOOL Is_In_Same_Zone(cell_t cellnum);
    void Base_Is_Attacked(TechnoClass *techno);
    int Is_Allowed_To_Retaliate(TechnoClass *techno);

    int Anti_Air();
    int Anti_Armor();
    int Anti_Infantry();

    const GamePtr<HouseClass> &Get_Owner_House() const { return m_OwnerHouse; }
    FacingClass &Get_Facing() { return m_Facing; }
    int Get_Price() const { return m_Price; }
    void Set_Price(int price) { m_Price = price; }
    void Set_Archive(target_t archive) { m_Archive = archive; }
    void Set_Suspended_TarCom(target_t target) { m_SuspendedTarCom = target; }

    target_t Get_TarCom() const { return m_TarCom; }
    BOOL Can_Teleport_Here(cell_t cell) const;
    BOOL Is_Useless() const { return m_IsUseless; }
    BOOL Is_Ticked_Off() const { return m_IsTickedOff; }
    BOOL Is_Cloakable() const { return m_Cloakable; }
    void Set_Cloakable(BOOL value) { m_Cloakable = value; }
    BOOL Is_Primary() const { return m_IsPrimary; }
    BOOL Is_A_Loner() const { return m_IsALoner; }
    BOOL Is_Locked_On_Map() const { return m_LockedOnMap; }
    BOOL Is_Recoiling() const { return m_IsRecoiling; }
    BOOL Is_Tethered() const { return m_Tethered; }
    BOOL Is_Player_Owned() const { return m_PlayerOwned; }
    void Set_Player_Owned();
    BOOL Is_Player_Aware() const { return m_PlayerAware; }
    BOOL Is_AI_Aware() const { return m_AIAware; }
    BOOL Is_Lemon() const { return m_Lemon; }
    BOOL Is_Bit2_16() const { return m_Bit2_16; }
    CloakState Cloak_State() const { return m_CloakState; }

#ifdef GAME_DLL
    friend void Setup_Hooks(void);

    void Hook_Techno_Draw_Object(const void *shape, int frame, int x, int y, WindowNumberType window, DirType dir, int scale)
    {
        TechnoClass::Techno_Draw_Object(shape, frame, x, y, window, dir, scale);
    }
    void Hook_Draw_It(int x, int y, WindowNumberType window)
    {
        TechnoClass::Draw_It(x, y, window);
    }
    VisualType Hook_Visual_Character(BOOL flag)
    {
        return TechnoClass::Visual_Character(flag);
    }
    BOOL Hook_Can_Repair()
    {
        return TechnoClass::Can_Repair();
    }
    BOOL Hook_Can_Player_Fire()
    {
        return TechnoClass::Can_Player_Fire();
    }
    int Hook_Weapon_Range(WeaponSlotType weapon)
    {
        return TechnoClass::Weapon_Range(weapon);
    }
    int Hook_How_Many_Survivors()
    {
        return TechnoClass::How_Many_Survivors();
    }
    BOOL Hook_Is_Weapon_Equipped()
    {
        return TechnoClass::Is_Weapon_Equipped();
    }
    cell_t Hook_Find_Exit_Cell(TechnoClass *object)
    {
        return TechnoClass::Find_Exit_Cell(object);
    }
    uint8_t *Hook_Remap_Table()
    {
        return TechnoClass::Remap_Table();
    }
    BOOL Hook_Is_Ready_To_Random_Animate()
    {
        return TechnoClass::Is_Ready_To_Random_Animate();
    }
    BOOL Hook_Is_Players_Army()
    {
        return TechnoClass::Is_Players_Army();
    }
    BOOL Hook_In_Range_Coord(coord_t coord, WeaponSlotType weapon)
    {
        return TechnoClass::In_Range(coord, weapon);
    }
    int Hook_Value()
    {
        return TechnoClass::Value();
    }
    int Hook_Refund_Amount()
    {
        return TechnoClass::Refund_Amount();
    }
    int Hook_Threat_Range(int a1)
    {
        return TechnoClass::Threat_Range(a1);
    }
    int Hook_Evaluate_Just_Cell(cell_t cellnum)
    {
        return TechnoClass::Evaluate_Just_Cell(cellnum);
    }
#endif

protected:
    void Techno_Draw_Object(const void *shape, int frame, int x, int y, WindowNumberType window, DirType dir = DIR_NORTH, int scale = 256) const;
    VisualType Visual_Character(BOOL flag = false) const; // Visual character for drawing.
    void Kill_Cargo(TechnoClass * object);
    void Destroy_Cargo();
    WeaponSlotType What_Weapon_Should_I_Use(target_t target) const;
    int Combat_Damage(WeaponSlotType weapon = WEAPON_SLOT_NONE) const;
    cell_t Nearby_Location(TechnoClass *techno) const;
    int Evaluate_Just_Cell(cell_t cellnum) const;

    const TechnoTypeClass &Techno_Class_Of() const { return reinterpret_cast<const TechnoTypeClass &>(Class_Of()); }

protected:
    FlasherClass m_Flasher;
    StageClass m_AnimStage;
    CargoClass m_Cargo;
    DoorClass m_Door;
    uint16_t m_KillCount;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_IsUseless : 1; // & 1
    BOOL m_IsTickedOff : 1; // & 2
    BOOL m_Cloakable : 1; // & 4
    BOOL m_IsPrimary : 1; // & 8
    BOOL m_IsALoner : 1; // & 16
    BOOL m_LockedOnMap : 1; // & 32
    BOOL m_IsRecoiling : 1; // & 64
    BOOL m_Tethered : 1; // & 128

    BOOL m_PlayerOwned : 1; // & 1
    BOOL m_PlayerAware : 1; // & 2
    BOOL m_AIAware : 1; // & 4
    BOOL m_Lemon : 1; // & 8
    BOOL m_Bit2_16 : 1; // & 16
#else
    bool m_IsUseless;
    bool m_IsTickedOff; // Has this object been attacked and pissed off?
    bool m_Cloakable;
    bool m_IsPrimary;
    bool m_IsALoner;
    bool m_LockedOnMap;
    bool m_IsRecoiling;
    bool m_Tethered; // Is this in radio chit chat with something (on repair bay, helipad etc)
    bool m_PlayerOwned;
    bool m_PlayerAware; // C&CDOS has this as "Discovered"
    bool m_AIAware; // maybe this is Discovered in C&C95?
    bool m_Lemon; //"degrades" in OpenDUNE, seems to imply some decaying logic
    bool m_Bit2_16;
#endif
    fixed_t m_ArmorMult;
    fixed_t m_FirepowerMult;
    TCountDownTimerClass<FrameTimerClass> m_IdleActionTimer;
    TCountDownTimerClass<FrameTimerClass> m_InvulnerabilityTimer;
    int m_Spied;
    target_t m_Archive;
    GamePtr<HouseClass> m_OwnerHouse;
    CloakState m_CloakState;
    StageClass m_CloakingStage;
    TCountDownTimerClass<FrameTimerClass> m_CloakDelayTimer;
    target_t m_TarCom;
    target_t m_SuspendedTarCom;
    FacingClass m_Facing;
    TCountDownTimerClass<FrameTimerClass> m_RearmTimer;
    int m_Ammo;
    int m_Price;

public:
    static int const s_BodyShape32[32];
};

#endif // TECHNO_H
