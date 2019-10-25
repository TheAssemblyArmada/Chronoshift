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
#include "bullettype.h"
#include "coord.h"
#include "display.h"
#include "drawshape.h"
#include "gbuffer.h"
#include "globals.h"
#include "house.h"
#include "remap.h"
#include "rules.h"
#include "weapontype.h"
#include "special.h"
#include <algorithm>

using std::min;

int const TechnoClass::BodyShape32[32] = {
    0, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1
};

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
    m_IsALoner(false),
    m_LockedOnMap(false),
    m_IsRecoiling(false),
    m_Tethered(false),
    m_PlayerOwned(false),
    m_PlayerAware(false),
    m_AIAware(false),
    m_Lemon(false),
    m_Bit2_16(true),
    m_ArmorMult(),
    m_FirepowerMult(),
    m_IdleActionTimer(),
    m_InvulnerabilityTimer(),
    m_Spied(0),
    m_Archive(0),
    m_OwnerHouse(HouseClass::As_Pointer(house)),
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

TechnoClass::TechnoClass(const TechnoClass &that) :
    RadioClass(that),
    m_Flasher(that.m_Flasher),
    m_AnimStage(that.m_AnimStage),
    m_Cargo(that.m_Cargo),
    m_Door(that.m_Door),
    m_KillCount(that.m_KillCount),
    m_IsUseless(that.m_IsUseless),
    m_IsTickedOff(that.m_IsTickedOff),
    m_Cloakable(that.m_Cloakable),
    m_IsPrimary(that.m_IsPrimary),
    m_IsALoner(that.m_IsALoner),
    m_LockedOnMap(that.m_LockedOnMap),
    m_IsRecoiling(that.m_IsRecoiling),
    m_Tethered(that.m_Tethered),
    m_PlayerOwned(that.m_PlayerOwned),
    m_PlayerAware(that.m_PlayerAware),
    m_AIAware(that.m_AIAware),
    m_Lemon(that.m_Lemon),
    m_Bit2_16(that.m_Bit2_16),
    m_ArmorMult(that.m_ArmorMult),
    m_FirepowerMult(that.m_FirepowerMult),
    m_IdleActionTimer(that.m_IdleActionTimer),
    m_InvulnerabilityTimer(that.m_InvulnerabilityTimer),
    m_Spied(that.m_Spied),
    m_Archive(that.m_Archive),
    m_OwnerHouse(that.m_OwnerHouse),
    m_CloakState(that.m_CloakState),
    m_CloakingStage(that.m_CloakingStage),
    m_CloakDelayTimer(that.m_CloakDelayTimer),
    m_TarCom(that.m_TarCom),
    m_SuspendedTarCom(that.m_SuspendedTarCom),
    m_Facing(that.m_Facing),
    m_RearmTimer(that.m_RearmTimer),
    m_Ammo(that.m_Ammo),
    m_Price(that.m_Price)
{
}

TechnoClass::TechnoClass(const NoInitClass &noinit) : 
    RadioClass(noinit),
    m_Flasher(noinit),
    m_AnimStage(noinit),
    m_Cargo(noinit),
    m_Door(noinit),
    m_IdleActionTimer(noinit),
    m_InvulnerabilityTimer(noinit),
    m_CloakingStage(noinit),
    m_CloakDelayTimer(noinit),
    m_Facing(noinit),
    m_RearmTimer(noinit)
{
}

#ifdef CHRONOSHIFT_DEBUG
void TechnoClass::Debug_Dump(MonoClass *mono) const
{
    RadioClass::Debug_Dump(mono);
}
#endif

HousesType TechnoClass::Owner() const
{
    return HousesType();
}

void TechnoClass::AI()
{
}

BOOL TechnoClass::Is_Player_Army() const
{
    return 0;
}

ActionType TechnoClass::What_Action(ObjectClass *object) const
{
    return ActionType();
}

ActionType TechnoClass::What_Action(cell_t cellnum) const
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

coord_t TechnoClass::Fire_Coord(WeaponSlotType weapon) const
{
    return coord_t();
}

BOOL TechnoClass::Unlimbo(coord_t coord, DirType dir)
{
    return 0;
}

void TechnoClass::Detach(target_t target, int a2)
{
}

void TechnoClass::Record_The_Kill(TechnoClass *object)
{
}

void TechnoClass::Do_Shimmer()
{
}

int TechnoClass::Exit_Object(TechnoClass *object)
{
    return 0;
}

void TechnoClass::Draw_It(int x, int y, WindowNumberType window) const
{
    m_Door.Clear_To_Redraw();
    if (m_Selected || Special.Always_Show_Health()) {
        GraphicViewPortClass rect(g_logicPage->Get_Graphic_Buffer(),
            g_logicPage->Get_XPos() + WindowList[WINDOW_TACTICAL].X,
            g_logicPage->Get_YPos() + WindowList[WINDOW_TACTICAL].Y,
            WindowList[WINDOW_TACTICAL].W,
            WindowList[WINDOW_TACTICAL].H);

        if (m_RTTI == RTTI_INFANTRY) {
            y -= 6;
        }
        if (m_RTTI == RTTI_BUILDING
            && reinterpret_cast<const BuildingTypeClass &>(Class_Of()).What_Type() == BUILDING_BARR) {
            y -= 5;
        }

        int dim_w = 0;
        int dim_h = 0;
        Class_Of().Dimensions(dim_w, dim_h);

        // draw health bar
        if (m_Health > 0) {
            if (Get_Owner_House()->Is_Ally(g_PlayerPtr) || Rule.Show_Enemy_Health()) {
                fixed_t health = Health_Ratio();
                int bar_left = x - dim_w / 2;
                int bar_top = y - dim_h / 2;
                // fade background of health bar
                rect.Remap(bar_left + 1, bar_top + 1, dim_w - 1, 2, (unsigned char *)DisplayClass::FadingShade);
                // draw health bar bounding box
                rect.Draw_Rect(bar_left, bar_top, bar_left + dim_w - 1, bar_top + 3, 12);
                int health_w = std::clamp((dim_w - 2) * health, 1, dim_w - 2);
                ColorType healthcolor = COLOR_LTGREEN;
                if (health <= Rule.Condition_Yellow()) {
                    healthcolor = COLOR_YELLOW;
                }
                if (health <= Rule.Condition_Red()) {
                    healthcolor = COLOR_RED;
                }
                // draw health bar itself
                rect.Fill_Rect(bar_left + 1, bar_top + 1, bar_left + health_w, bar_top + 2, healthcolor);
            }
        }
        // draw selection box
        if (m_Selected) {
            int center_x = dim_w / 2;
            int center_y = dim_h / 2;
            int line_w = dim_w / 5;
            int line_h = dim_h / 5;

            int adj_y = 0;
            // if we are showing the health bar nudge the selection box a bit
            if (Get_Owner_House()->Is_Ally(g_PlayerPtr) || Rule.Show_Enemy_Health()) {
                adj_y = 4;
            }
            //this seems useless cause we did that already....
            if (m_RTTI == RTTI_VESSEL) {
                center_x = dim_w / 2;
            }
            // left center
            int left = x - center_x;
            // right center
            int right = x + center_x;
            // top center
            int top = y + adj_y - center_y;
            // bottom center
            int bottom = y + center_y;

            // left horizonal line
            int h_l = left + line_w;
            // right horizonal line
            int h_r = right - line_w;
            // top vertical line
            int v_t = top + line_h;
            // bottom vertical line
            int v_b = bottom - line_h;

            // draw top left
            rect.Draw_Line(left, top, h_l, top, COLOR_WHITE);
            rect.Draw_Line(left, top, left, v_t, COLOR_WHITE);

            // draw top right
            rect.Draw_Line(right, top, h_r, top, COLOR_WHITE);
            rect.Draw_Line(right, top, right, v_t, COLOR_WHITE);

            // draw bottom right
            rect.Draw_Line(right, bottom, h_r, bottom, COLOR_WHITE);
            rect.Draw_Line(right, bottom, right, v_b, COLOR_WHITE);

            // draw bottom left
            rect.Draw_Line(left, bottom, h_l, bottom, COLOR_WHITE);
            rect.Draw_Line(left, bottom, left, v_b, COLOR_WHITE);

            if (Get_Owner_House()->Is_Ally(g_PlayerPtr) || Get_Owner_House()->Spied_My_Radar(g_PlayerPtr)) {
                Draw_Pips((x - center_x) + 5, (y + center_y) - 3, window);
            }
        }
    }
}

void TechnoClass::Hidden()
{
}

void TechnoClass::Look(BOOL a1)
{
}

BOOL TechnoClass::Mark(MarkType mark)
{
    return 0;
}

void TechnoClass::Clicked_As_Target(int a1)
{
}

BOOL TechnoClass::Select()
{
    return 0;
}

BOOL TechnoClass::In_Range(coord_t a1, WeaponSlotType weapon) const
{
    return 0;
}

int TechnoClass::Weapon_Range(WeaponSlotType weapon) const
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

void TechnoClass::Per_Cell_Process(PCPType pcp)
{
}

RadioMessageType TechnoClass::Receive_Message(RadioClass *radio, RadioMessageType message, target_t &target)
{
    return RadioMessageType();
}

BOOL TechnoClass::Revealed(HouseClass *house)
{
    return 0;
}

/**
 * @brief
 *
 * @address 0x004F984C
 */
void TechnoClass::Code_Pointers()
{
    m_Cargo.Code_Pointers();
    RadioClass::Code_Pointers();
}

/**
 * @brief
 *
 * @address 0x004F9868
 */
void TechnoClass::Decode_Pointers()
{
    m_Cargo.Decode_Pointers();
    RadioClass::Decode_Pointers();
}

void TechnoClass::Override_Mission(MissionType mission, int target1, int target2)
{
}

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
    return m_Facing.Get_Current();
}

BuildingClass *TechnoClass::Find_Docking_Bay(BuildingType building, int a2) const
{
    return nullptr;
}

cell_t TechnoClass::Find_Exit_Cell(TechnoClass *object) const
{
    return cell_t();
}

/**
 * Get the desired direction to face to load up.
 *
 * 0x005685C4
 */
DirType TechnoClass::Desired_Load_Dir(ObjectClass *object, cell_t &cellnum) const
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

void TechnoClass::Response_Select()
{
}

void TechnoClass::Response_Move()
{
}

void TechnoClass::Response_Attack()
{
}

void TechnoClass::Player_Assign_Mission(MissionType mission, target_t target, target_t dest)
{
}

int TechnoClass::Made_A_Kill()
{
    return 0;
}

BOOL TechnoClass::Target_Something_Nearby(ThreatType threat)
{
    return 0;
}

void TechnoClass::Stun()
{
}

BOOL TechnoClass::In_Range(target_t target, WeaponSlotType weapon) const
{
    return 0;
}

BOOL TechnoClass::In_Range(ObjectClass *object, WeaponSlotType weapon) const
{
    return 0;
}

FireErrorType TechnoClass::Can_Fire(target_t target, WeaponSlotType weapon) const
{
    return FireErrorType();
}

target_t TechnoClass::Greatest_Threat(ThreatType threat)
{
    return target_t();
}

void TechnoClass::Assign_Target(target_t target)
{
}

BulletClass *TechnoClass::Fire_At(target_t target, WeaponSlotType weapon)
{
    return nullptr;
}

BOOL TechnoClass::Captured(HouseClass *house)
{
    return 0;
}

BOOL TechnoClass::Electric_Zap(target_t target, BOOL a2, coord_t a3, uint8_t *a4)
{
    return 0;
}

void TechnoClass::Renovate()
{
}

uint8_t *TechnoClass::Remap_Table() const
{
    return nullptr;
}

void TechnoClass::Draw_Pips(int x, int y, WindowNumberType window) const
{
}

void TechnoClass::Do_Uncloak()
{
}

void TechnoClass::Do_Cloak()
{
}

bool TechnoClass::Is_Ready_To_Random_Animate() const
{
    return false;
}

bool TechnoClass::Random_Animate()
{
    return false;
}

void TechnoClass::Assign_Destination(target_t dest)
{
}

void TechnoClass::Enter_Idle_Mode(BOOL a1)
{
}

void TechnoClass::Set_Player_Owned()
{
    m_PlayerOwned = (m_OwnerHouse == *g_PlayerPtr);
}

void TechnoClass::Techno_Draw_It(
    const void *shape, int frame, int x, int y, WindowNumberType window, DirType dir, int scale) const
{
    DEBUG_ASSERT(m_IsActive);

    if (shape != nullptr) {

        VisualType visual = Visual_Character();

        void *remap_table = Remap_Table();
        void *shadow_table = DisplayClass::UnitShadow;

        const ObjectTypeClass &type = Class_Of();

        int frame_count = Get_Build_Frame_Count((void *)shape);

        if (type.Get_Image_Data() == shape && frame_count - 1 > frame) {
            type.Init_Frame_Dimensions(frame_count);
            type.Set_Frame_Dimensions((void *)shape, frame);
        }

        if (m_Height > 0) {
            shadow_table = DisplayClass::UnitShadowAir;
        }

        y -= Lepton_To_Pixel(Get_Height());

        if (m_RTTI == RTTI_INFANTRY) {
            if (!m_PlayerOwned) {
                if (reinterpret_cast<const InfantryTypeClass &>(Class_Of()).What_Type() == INFANTRY_SPY) {
                    remap_table = (void *)g_PlayerPtr->Remap_Table(false, REMAP_1);
                }
            }

            if (reinterpret_cast<const InfantryTypeClass &>(Class_Of()).Has_Alt_Remap()) {
                remap_table = (void *)reinterpret_cast<const InfantryTypeClass &>(Class_Of()).Alt_Remap_Table();
            }
        }

        if (!m_InvulnerabilityTimer.Expired()) {
            remap_table = DisplayClass::FadingRed;
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
                            flags | SHAPE_GHOST | SHAPE_FADING,
                            remap_table,
                            shadow_table,
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
                            remap_table,
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

                break;
            default:
                if (visual != VISUAL_HIDDEN && visual != VISUAL_RIPPLE) {
                    if (visual == VISUAL_SHADOWY) {
                        CC_Draw_Shape((void *)shape,
                            frame,
                            x,
                            y,
                            window,
                            flags | SHAPE_FADING | SHAPE_PREDATOR,
                            nullptr,
                            DisplayClass::FadingShade,
                            dir);

                    } else {
                        CC_Draw_Shape((void *)shape,
                            frame,
                            x,
                            y,
                            window,
                            flags | SHAPE_GHOST | SHAPE_FADING,
                            remap_table,
                            shadow_table,
                            dir);
                    }

                    if (visual == VISUAL_DARKEN) {
                        CC_Draw_Shape((void *)shape,
                            frame,
                            x,
                            y,
                            window,
                            flags | SHAPE_FADING | SHAPE_PREDATOR,
                            remap_table,
                            DisplayClass::FadingShade,
                            dir);
                    }
                }

                // This draws for all except VISUAL_NORMAL and VISUAL_HIDDEN, provides predator effect over whatever is drawn
                // above. This is the major difference between C&C and RA in addition to the stages being more nuanced.
                if (visual != VISUAL_NORMAL && visual != VISUAL_HIDDEN) {
                    CC_Draw_Shape((void *)shape, frame, x, y, window, flags | SHAPE_PREDATOR, nullptr, nullptr, dir);
                }

                break;
        }
    }
}

int TechnoClass::Anti_Air()
{
    if (!Is_Weapon_Equipped()) {
        return 0;
    }

    const TechnoTypeClass &tech_type = reinterpret_cast<const TechnoTypeClass &>(Class_Of());
    WeaponTypeClass *weapon = tech_type.Get_Weapon(WEAPON_SLOT_PRIMARY);

    if (!weapon->Get_Projectile()->Is_Anti_Air()) {
        return 0;
    }

    int effectiveness = weapon->Get_Damage() * weapon->Get_Warhead()->Get_Verses(ARMOR_LIGHT) * weapon->Get_Range();

    if (tech_type.Is_Two_Shooter()) {
        effectiveness *= 2;
    }

    return effectiveness / 50;
}

int TechnoClass::Anti_Armor()
{
    if (!Is_Weapon_Equipped()) {
        return 0;
    }

    const TechnoTypeClass &tech_type = reinterpret_cast<const TechnoTypeClass &>(Class_Of());
    WeaponTypeClass *weapon = tech_type.Get_Weapon(WEAPON_SLOT_PRIMARY);

    if (!weapon->Get_Projectile()->Is_Anti_Ground()) {
        return 0;
    }

    lepton_t range = min<lepton_t>(weapon->Get_Range(), 1024);
    int effectiveness = weapon->Get_Damage() * weapon->Get_Warhead()->Get_Verses(ARMOR_HEAVY) * range
        * weapon->Get_Warhead()->Get_Spread() / weapon->Get_ROF();

    if (tech_type.Is_Two_Shooter()) {
        effectiveness *= 2;
    }

    if (weapon->Get_Projectile()->Is_Inaccurate()) {
        effectiveness /= 2;
    }

    return effectiveness / 50;
}

int TechnoClass::Anti_Infantry()
{
    if (!Is_Weapon_Equipped()) {
        return 0;
    }

    const TechnoTypeClass &tech_type = reinterpret_cast<const TechnoTypeClass &>(Class_Of());
    WeaponTypeClass *weapon = tech_type.Get_Weapon(WEAPON_SLOT_PRIMARY);

    if (!weapon->Get_Projectile()->Is_Anti_Ground()) {
        return 0;
    }

    lepton_t range = min<lepton_t>(weapon->Get_Range(), 1024);
    int effectiveness = weapon->Get_Damage() * weapon->Get_Warhead()->Get_Verses(ARMOR_NONE) * range
        * weapon->Get_Warhead()->Get_Spread() / weapon->Get_ROF();

    if (tech_type.Is_Two_Shooter()) {
        effectiveness *= 2;
    }

    if (weapon->Get_Projectile()->Is_Inaccurate()) {
        effectiveness /= 2;
    }

    return effectiveness / 50;
}

BOOL TechnoClass::Can_Teleport_Here(cell_t cell) const
{
#ifdef GAME_DLL
    BOOL (*func)(const TechnoClass *, cell_t) = reinterpret_cast<BOOL (*)(const TechnoClass *, cell_t)>(0x00560A58);
    return func(this, cell);
#else
    return 0;
#endif
}

VisualType TechnoClass::Visual_Character(BOOL flag) const
{
    DEBUG_ASSERT(m_IsActive);

    if (Techno_Class_Of().Is_Invisible() && m_PlayerOwned) {
        return VISUAL_NORMAL;
    }

    if (!Techno_Class_Of().Is_Invisible() || m_PlayerOwned || g_InMapEditor) {
        if (m_CloakState == CLOAK_UNCLOAKED || g_InMapEditor) {
            return VISUAL_NORMAL;
        }

        if (m_CloakState == CLOAK_CLOAKED) {
            if (!flag && m_PlayerOwned) {
                return VISUAL_SHADOWY;
            }
        } else {
            int32_t value = m_CloakingStage.Get_Stage();

            if (m_CloakState == CLOAK_UNCLOAKING) {
                value = 38 - m_CloakingStage.Get_Stage();
            }

            if (value <= 0) {
                return VISUAL_NORMAL;
            }

            int stage = fixed_t(value, 38) * 256;

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
        }
    }

    return VISUAL_HIDDEN;
}
