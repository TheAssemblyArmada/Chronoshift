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
#include "techno.h"
#include "coord.h"
#include "display.h"
#include "drawshape.h"
#include "globals.h"

TechnoClass::TechnoClass(RTTIType type, int id, HousesType house) :
    RadioClass(type, house),
    m_Flasher(),
    m_AnimStage(),
    m_Cargo(),
    m_Door(),
    m_KillCount(0),
    m_IsUseless(false),
    m_IsTickedOff(false),
    m_Cloakable(false),
    m_IsPrimary(false),
    m_IsALoaner(false),
    m_LockedOnMap(false),
    m_IsRecoiling(false),
    m_Tethered(false),
    m_PlayerOwned(false),
    m_PlayerAware(false),
    m_AIAware(false),
    m_Lemon(false),
    m_TechnoBit2_16(true),
    m_ArmorMult(),
    m_FirepowerMult(),
    m_IdleActionTimer(),
    m_InvulnerabilityTimer(),
    m_SpiedBits(0),
    m_Archive(0),
    m_OwnerHouse(/*HouseClass::As_Pointer(house)*/),
    m_CloakState(CLOAK_UNCLOAKED),
    m_CloakingStage(),
    m_CloakDelayTimer(),
    m_TarCom(0),
    m_SuspendedTarCom(0),
    m_Facing(),
    m_RearmTimer(),
    m_Ammo(-1),
    m_Price(0)
{
}

TechnoClass::TechnoClass(NoInitClass &init) : 
    RadioClass(init),
    m_Flasher(init),
    m_AnimStage(init),
    m_Cargo(init),
    m_Door(init),
    m_IdleActionTimer(init),
    m_InvulnerabilityTimer(init),
    m_CloakingStage(init),
    m_CloakDelayTimer(init),
    m_Facing(init),
    m_RearmTimer(init)
{
    
}

TechnoClass::~TechnoClass() {}

HousesType TechnoClass::Owner() const
{
    return HousesType();
}

void TechnoClass::AI() {}

BOOL TechnoClass::Is_Player_Army() const
{
    return 0;
}

ActionType TechnoClass::What_Action(ObjectClass *object) const
{
    return ActionType();
}

ActionType TechnoClass::What_Action(int16_t cellnum) const
{
    return ActionType();
}

int TechnoClass::Get_Ownable() const
{
    return 0;
}

BOOL TechnoClass::Can_Repair() const
{
    return 0;
}

BOOL TechnoClass::Can_Player_Fire() const
{
    return 0;
}

BOOL TechnoClass::Can_Player_Move() const
{
    return 0;
}

uint32_t TechnoClass::Fire_Coord(int weapon) const
{
    return uint32_t();
}

BOOL TechnoClass::Unlimbo(uint32_t coord, DirType dir)
{
    return 0;
}

void TechnoClass::Detach(int32_t target, int a2) {}

void TechnoClass::Record_The_Kill(TechnoClass *object) {}

void TechnoClass::Do_Shimmer() {}

int TechnoClass::Exit_Object(TechnoClass *object)
{
    return 0;
}

void TechnoClass::Draw_It(int x, int y, WindowNumberType window) const {}

void TechnoClass::Hidden() {}

void TechnoClass::Look(BOOL a1) {}

BOOL TechnoClass::Mark(MarkType mark)
{
    return 0;
}

void TechnoClass::Clicked_As_Target(int a1) {}

BOOL TechnoClass::Select()
{
    return 0;
}

BOOL TechnoClass::In_Range(uint32_t a1, int weapon) const
{
    return 0;
}

int TechnoClass::Weapon_Range(int weapon) const
{
    return 0;
}

DamageResultType TechnoClass::Take_Damage(int &damage, int a2, WarheadType warhead, TechnoClass *object, BOOL a5)
{
    return DamageResultType();
}

int TechnoClass::Value() const
{
    return 0;
}

void TechnoClass::Per_Cell_Process(PCPType pcp) {}

RadioMessageType TechnoClass::Receive_Message(RadioClass *radio, RadioMessageType message, target_t &target)
{
    return RadioMessageType();
}

BOOL TechnoClass::Revealed(HouseClass *house)
{
    return 0;
}

void TechnoClass::Code_Pointers() {}

void TechnoClass::Decode_Pointers() {}

void TechnoClass::Override_Mission(MissionType mission, int target1, int target2) {}

BOOL TechnoClass::Restore_Mission()
{
    return 0;
}

int TechnoClass::How_Many_Survivors() const
{
    return 0;
}

DirType TechnoClass::Turret_Facing() const
{
    return DirType();
}

BuildingClass *TechnoClass::Find_Docking_Bay(BuildingType building, int a2) const
{
    return nullptr;
}

int16_t TechnoClass::Find_Exit_Cell(TechnoClass *object) const
{
    return int16_t();
}

/**
 * Get the desired direction to face to load up.
 *
 * 0x005685C4
 */
DirType TechnoClass::Desired_Load_Dir(ObjectClass *object, int16_t &cellnum) const
{
    cellnum = 0;

    return DIR_NORTH;
}

/**
 * Get the direction we are currently able to fire in.
 *
 * 0x005685E0
 */
DirType TechnoClass::Fire_Direction() const
{
    return Turret_Facing();
}

InfantryType TechnoClass::Crew_Type() const
{
    return InfantryType();
}

BOOL TechnoClass::Is_Allowed_To_Recloak() const
{
    return 0;
}

BOOL TechnoClass::Is_Weapon_Equipped() const
{
    return 0;
}

int TechnoClass::Rearm_Delay(int a1, int a2) const
{
    return 0;
}

int TechnoClass::Refund_Amount() const
{
    return 0;
}

int TechnoClass::Threat_Range(int a1) const
{
    return 0;
}

void TechnoClass::Response_Select() {}

void TechnoClass::Response_Move() {}

void TechnoClass::Response_Attack() {}

void TechnoClass::Player_Assign_Mission(MissionType mission, int32_t target, int32_t dest) {}

int TechnoClass::Made_A_Kill()
{
    return 0;
}

BOOL TechnoClass::Target_Something_Nearby(ThreatType threat)
{
    return 0;
}

void TechnoClass::Stun() {}

BOOL TechnoClass::In_Range(int32_t target, int weapon) const
{
    return 0;
}

BOOL TechnoClass::In_Range(ObjectClass *object, int weapon) const
{
    return 0;
}

FireErrorType TechnoClass::Can_Fire(int32_t target, int weapon) const
{
    return FireErrorType();
}

int32_t TechnoClass::Greatest_Threat(ThreatType threat)
{
    return int32_t();
}

void TechnoClass::Assign_Target(int32_t target) {}

BulletClass *TechnoClass::Fire_At(int32_t target, int weapon)
{
    return nullptr;
}

BOOL TechnoClass::Captured(HouseClass *house)
{
    return 0;
}

BOOL TechnoClass::Electric_Zap(int32_t target, BOOL a2, uint32_t a3, uint8_t *a4)
{
    return 0;
}

void TechnoClass::Renovate() {}

uint8_t *TechnoClass::Remap_Table() const
{
    return nullptr;
}

void TechnoClass::Draw_Pips(int x, int y, WindowNumberType window) const {}

void TechnoClass::Do_Uncloak() {}

void TechnoClass::Do_Cloak() {}

bool TechnoClass::Is_Ready_To_Random_Animate() const
{
    return false;
}

bool TechnoClass::Random_Animate()
{
    return false;
}

void TechnoClass::Assign_Destination(int32_t target) {}

void TechnoClass::Enter_Idle_Mode(BOOL a1) {}

void TechnoClass::Techno_Draw_It(
    const void *shape, int frame, int x, int y, WindowNumberType window, DirType dir, int scale) const
{
    // TODO needs InfantryTypeClass, HouseClass
#ifndef CHRONOSHIFT_STANDALONE
    void (*func)(const void *, int, int, int, WindowNumberType, DirType, int) =
        reinterpret_cast<void (*)(const void *, int, int, int, WindowNumberType, DirType, int)>(0x0056706C);
    func(shape, frame, x, y, window, dir, scale);
#else
    /*DEBUG_ASSERT(IsActive);

    if (shape != nullptr) {
        VisualType visual = Visual_Character();
        void *fading_table = Remap_Table();
        void *ghost_table = DisplayClass::UnitShadow;

        const ObjectTypeClass &type = Class_Of();

        int frame_count = Get_Build_Frame_Count((void *)shape);

        if (type.Get_Image_Data() == shape && frame_count - 1 > frame) {
            type.Init_Frame_Dimensions(frame_count);
            type.Set_Frame_Dimensions((void *)shape, frame);
        }

        if (Get_Height() > 0) {
            ghost_table = DisplayClass::UnitShadowAir;
        }

        y -= Lepton_To_Pixel(Get_Height());

        if (What_Am_I() == RTTI_INFANTRY) {
            if (!m_PlayerOwned) {
                if (reinterpret_cast<InfantryTypeClass &>(Class_Of()).Type == INFANTRY_SPY) {
                    fading_table = (void *)PlayerPtr->Remap_Table(false, REMAP_1);
                }
            }

            if (reinterpret_cast<InfantryTypeClass &>(Class_Of()).HasAltRemap) {
                fading_table = (void *)reinterpret_cast<TechnoTypeClass &>(Class_Of()).Remap;
            }
        }

        if (!m_InvulnerabilityTimer == 0) {
            fading_table = DisplayClass::FadingRed;
        }

        ShapeFlags flags = SHAPE_WIN_REL | SHAPE_CENTER;

        // *Enhancement* In the original RA, all units were treated as vessels are here. Here they are treated separately and
        // all other units are treated as they were in C&C instead. This should give land units the C&C stealth tank effect
        // when cloaked.
        switch (What_Am_I()) {
            case RTTI_VESSEL:
                switch (visual) {
                    case VISUAL_NORMAL:
                        CC_Draw_Shape((void *)shape,
                            frame,
                            x,
                            y,
                            window,
                            flags | SHAPE_SHADOW | SHAPE_FADING,
                            fading_table,
                            ghost_table,
                            dir);
                        break;

                    case VISUAL_INDISTINCT:
                    case VISUAL_DARKEN:
                        CC_Draw_Shape((void *)shape,
                            frame,
                            x,
                            y,
                            window,
                            flags | SHAPE_FADING,
                            fading_table,
                            DisplayClass::FadingShade,
                            dir);
                        break;

                    case VISUAL_SHADOWY:
                    case VISUAL_RIPPLE:
                        CC_Draw_Shape((void *)shape,
                            frame,
                            x,
                            y,
                            window,
                            flags | SHAPE_PREDATOR | SHAPE_FADING,
                            nullptr,
                            DisplayClass::FadingShade,
                            dir);
                        break;

                    case VISUAL_HIDDEN:
                        break;

                    default:
                        break;
                }
            default:
                if (visual != VISUAL_HIDDEN && visual != VISUAL_RIPPLE) {
                    if (visual == VISUAL_SHADOWY) {
                        CC_Draw_Shape((void *)shape,
                            frame,
                            x,
                            y,
                            window,
                            flags | SHAPE_FADING,
                            nullptr,
                            DisplayClass::FadingShade,
                            dir);

                    } else {
                        CC_Draw_Shape((void *)shape,
                            frame,
                            x,
                            y,
                            window,
                            flags | SHAPE_SHADOW | SHAPE_FADING,
                            fading_table,
                            ghost_table,
                            dir);
                    }

                    if (visual == VISUAL_DARKEN) {
                        CC_Draw_Shape((void *)shape,
                            frame,
                            x,
                            y,
                            window,
                            flags | SHAPE_FADING | SHAPE_PREDATOR,
                            fading_table,
                            DisplayClass::FadingShade,
                            dir);
                    }
                }

                // This draws for all except VISUAL_NORMAL and VISUAL_HIDDEN, provides predator effect over whatever is drawn above.
                // This is the major difference between C&C and RA in addition to the stages being more nuanced.
                if (visual != VISUAL_NORMAL && visual != VISUAL_HIDDEN) {
                    CC_Draw_Shape((void *)shape, frame, x, y, window, flags | SHAPE_PREDATOR, nullptr, nullptr, dir);
                }

                break;
        }
    }*/
#endif
}

VisualType TechnoClass::Visual_Character(BOOL flag) const
{
    DEBUG_ASSERT(IsActive);

    if (Techno_Class_Of().Is_Invisible() && m_PlayerOwned) {
        return VISUAL_NORMAL;
    }

    if (Techno_Class_Of().Is_Invisible() || m_PlayerOwned || g_inMapEditor) {
        if (m_CloakState == CLOAK_UNCLOAKED || g_inMapEditor) {
            return VISUAL_NORMAL;
        }

        if (m_CloakState == CLOAK_CLOAKED) {
            if (!flag && m_PlayerOwned) {
                return VISUAL_SHADOWY;
            }
        }

        int32_t value = m_CloakingStage.Get_Stage();

        if (m_CloakState == CLOAK_UNCLOAKING) {
            value = (m_CloakingStage.Get_Stage() - 38);
        }

        if (m_CloakingStage.Get_Stage() <= 0) {
            return VISUAL_NORMAL;
        }

        int stage = fixed(value, 38) * 256;

        if (stage < 64) {
            return VISUAL_INDISTINCT;
        }

        if (stage < 128) {
            return VISUAL_DARKEN;
        }

        if (stage < 192) {
            return VISUAL_SHADOWY;
        }

        if (!flag && m_PlayerOwned) {
            return VISUAL_SHADOWY;
        }

        if (stage < 255) {
            return VISUAL_RIPPLE;
        }

        return VISUAL_HIDDEN;
    }

    return VISUAL_NORMAL;
}
