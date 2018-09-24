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
#include "gametypes.h"
#include "infantrytype.h"
#include "noinit.h"
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
    //VISUAL_NONE = -1,
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
    TechnoClass(NoInitClass &init);
    virtual ~TechnoClass();

    virtual HousesType Owner() const override;
    virtual void AI() override;
    virtual BOOL Is_Player_Army() const override;
    virtual ActionType What_Action(ObjectClass *object) const override;
    virtual ActionType What_Action(int16_t cellnum) const override;
    virtual int Get_Ownable() const override;
    virtual BOOL Can_Repair() const override;
    virtual BOOL Can_Player_Fire() const override;
    virtual BOOL Can_Player_Move() const override;
    virtual uint32_t Fire_Coord(int weapon = WEAPON_SLOT_PRIMARY) const override;
    virtual BOOL Unlimbo(uint32_t coord, DirType dir = DIR_NORTH) override;
    virtual void Detach(int32_t target, int a2) override;
    virtual void Record_The_Kill(TechnoClass *object = nullptr) override;
    virtual void Do_Shimmer() override;
    virtual int Exit_Object(TechnoClass *object) override;
    virtual void Draw_It(int x, int y, WindowNumberType window) const override;
    virtual void Hidden() override;
    virtual void Look(BOOL a1 = false) override;
    virtual BOOL Mark(MarkType mark) override;
    virtual void Clicked_As_Target(int a1) override;
    virtual BOOL Select() override;
    virtual BOOL In_Range(uint32_t a1, int weapon = 0) const override;
    virtual int Weapon_Range(int weapon = WEAPON_SLOT_PRIMARY) const override;
    virtual DamageResultType Take_Damage(
        int &damage, int a2, WarheadType warhead, TechnoClass *object = nullptr, BOOL a5 = false) override;
    virtual int Value() const override;
    virtual void Per_Cell_Process(PCPType pcp) override;
    virtual RadioMessageType Receive_Message(RadioClass *radio, RadioMessageType message, target_t &target) override;
    virtual BOOL Revealed(HouseClass *house) override;
    virtual void Code_Pointers() override;
    virtual void Decode_Pointers() override;
    // MissionClass virtuals.
    virtual void Override_Mission(MissionType mission, int target1 = -1, int target2 = -1) override;
    virtual BOOL Restore_Mission() override; 
    // TechnoClass virtuals
    virtual int How_Many_Survivors() const;
    virtual DirType Turret_Facing() const;
    virtual BuildingClass *Find_Docking_Bay(BuildingType building, int a2 = 0) const;
    virtual int16_t Find_Exit_Cell(TechnoClass *object) const;
    virtual DirType Desired_Load_Dir(ObjectClass *object, int16_t &cellnum) const;
    virtual DirType Fire_Direction() const;
    virtual InfantryType Crew_Type() const;
    virtual BOOL Is_Allowed_To_Recloak() const;
    virtual BOOL Is_Weapon_Equipped() const;
    virtual fixed Ore_Load() const { return fixed::_0_1; }
    virtual int Pip_Count() const { return 0; }
    virtual int Rearm_Delay(int a1, int a2) const;
    virtual int Refund_Amount() const;
    virtual int Risk() const { return reinterpret_cast<TechnoTypeClass &>(Class_Of()).Get_Risk(); }
    virtual int Threat_Range(int a1) const;
    virtual void Response_Select();
    virtual void Response_Move();
    virtual void Response_Attack();
    virtual void Player_Assign_Mission(MissionType mission, int32_t target = 0, int32_t dest = 0);
    virtual int Made_A_Kill();
    virtual BOOL Target_Something_Nearby(ThreatType threat);
    virtual void Stun();
    virtual BOOL In_Range(int32_t target, int weapon = WEAPON_SLOT_PRIMARY) const;
    virtual BOOL In_Range(ObjectClass *object, int weapon = WEAPON_SLOT_PRIMARY) const;
    virtual void Death_Announcement(TechnoClass *object) const = 0;
    virtual FireErrorType Can_Fire(int32_t target, int weapon = WEAPON_SLOT_PRIMARY) const;
    virtual int32_t Greatest_Threat(ThreatType threat);
    virtual void Assign_Target(int32_t target);
    virtual BulletClass *Fire_At(int32_t target, int weapon = WEAPON_SLOT_PRIMARY);
    virtual BOOL Captured(HouseClass *house);
    virtual BOOL Electric_Zap(int32_t target, BOOL a2, uint32_t a3 = 0, uint8_t *a4 = nullptr);
    virtual void Renovate();
    virtual uint8_t *Remap_Table() const;
    virtual void Draw_Pips(int x, int y, WindowNumberType window) const;
    virtual void Do_Uncloak();
    virtual void Do_Cloak();
    virtual bool Is_Ready_To_Random_Animate() const;
    virtual bool Random_Animate();
    virtual void Assign_Destination(int32_t target); // TODO A second arg exists in SS and TS
    virtual void Enter_Idle_Mode(BOOL a1 = false);

    void Techno_Draw_It(const void *shape, int frame, int x, int y, WindowNumberType window, DirType dir, int scale) const;

protected:
    FlasherClass m_Flasher;
    StageClass m_AnimStage;
    CargoClass m_Cargo;
    DoorClass m_Door;
    uint16_t m_KillCount;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool m_IsUseless; // & 1
            bool m_IsTickedOff; // & 2 Has this object been attacked and pissed off?
            bool m_Cloakable; // & 4
            bool m_IsPrimary; // & 8
            bool m_IsALoaner; // & 16
            bool m_LockedOnMap; // & 32
            bool m_IsRecoiling; // & 64
            bool m_Tethered; // & 128 Is this in radio chit chat with something (on repair bay, helipad etc)
            bool m_PlayerOwned; // & 1
            bool m_PlayerAware; // & 2 C&CDOS has this as "Discovered"
            bool m_AIAware; // & 4 maybe this is Discovered in C&C95?
            bool m_Lemon; // & 8"degrades" in OpenDUNE, seems to imply some decaying logic
            bool m_TechnoBit2_16; // & 16
        };
        int m_Bitfield;
    };
#else
    bool m_IsUseless;
    bool m_IsTickedOff; // Has this object been attacked and pissed off?
    bool m_Cloakable; 
    bool m_IsPrimary; 
    bool m_IsALoaner; 
    bool m_LockedOnMap; 
    bool m_IsRecoiling; 
    bool m_Tethered; // Is this in radio chit chat with something (on repair bay, helipad etc)
    bool m_PlayerOwned;
    bool m_PlayerAware; // C&CDOS has this as "Discovered"
    bool m_AIAware; // maybe this is Discovered in C&C95?
    bool m_Lemon; //"degrades" in OpenDUNE, seems to imply some decaying logic
    bool m_TechnoBit2_16;
#endif
    fixed m_ArmorMult;
    fixed m_FirepowerMult;
    TCountDownTimerClass<FrameTimerClass> m_IdleActionTimer;
    TCountDownTimerClass<FrameTimerClass> m_InvulnerabilityTimer;
    int m_SpiedBits;
    int m_Archive;
    int m_OwnerHouse; // Should be CCPtr<HouseClass> m_OwnerHouse; when HouseClass is done.
    CloakState m_CloakState;
    StageClass m_CloakingStage;
    TCountDownTimerClass<FrameTimerClass> m_CloakDelayTimer;
    int m_TarCom;
    int m_SuspendedTarCom;
    FacingClass m_Facing;
    TCountDownTimerClass<FrameTimerClass> m_RearmTimer;
    int m_Ammo;
    int m_Price;
};

#endif // TECHNO_H
