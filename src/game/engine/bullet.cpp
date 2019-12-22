/**
 * @file
 *
 * @author CCHyper
 *
 * @brief
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "bullet.h"
#include "aircraft.h"
#include "anim.h"
#include "combat.h"
#include "drawshape.h"
#include "globals.h"
#include "infantry.h"
#include "iomap.h"
#include "lists.h"
#include "rules.h"
#include "scenario.h"
#include "techno.h"
#include "vessel.h"

#ifndef GAME_DLL
TFixedIHeapClass<BulletClass> g_Bullets;
#endif

/**
 * Class constructor.
 */
BulletClass::BulletClass(
    BulletType bullet, target_t target, TechnoClass *payback, int health, WarheadType warhead, int speed) :
    ObjectClass(RTTI_BULLET, g_Bullets.ID(this)),
    m_Class(BulletTypeClass::As_Pointer(bullet)),
    m_Flyer(),
    m_Fuse(),
    m_Payback(payback),
    m_Facing(),
    m_BBit1(false),
    m_ToSpawnTrailer(false),
    m_BBit4(true),
    m_Target(target),
    m_Speed(MPHType(speed)),
    m_Warhead(warhead)
{
    m_Height = 256;
    m_Health = health;
}

/**
 * Copy constructor.
 */
BulletClass::BulletClass(const BulletClass &that) :
    ObjectClass(that),
    m_Flyer(that.m_Flyer),
    m_Fuse(that.m_Fuse),
    m_Class(that.m_Class),
    m_Payback(that.m_Payback),
    m_Facing(that.m_Facing),
    m_BBit1(that.m_BBit1),
    m_ToSpawnTrailer(that.m_ToSpawnTrailer),
    m_BBit4(that.m_BBit4),
    m_Target(that.m_Target),
    m_Speed(that.m_Speed),
    m_Warhead(that.m_Warhead)
{
}

/**
 * Class deconstructor.
 */
BulletClass::~BulletClass()
{
    if (g_GameActive) {
        // payback for the doggie limbo launcher logic.
        if (m_Payback != nullptr && m_Payback->What_Am_I() == RTTI_INFANTRY) {
            // The payback logic is used here just for the limbo launch of
            // the doggies weapon, and dogs are infantry by design.
            InfantryClass *pbptr = static_cast<InfantryClass *>(m_Payback);
            if (pbptr != nullptr && pbptr->Class_Of().Is_Canine()) {
                bool paid_back = false;
                coord_t coord = m_Coord;
                cell_t cell = Coord_To_Cell(coord);

                // if we can't be in the cell, find a nearby location where we can.
                if (Can_Enter_Cell(cell) != MOVE_OK) {
                    cell = g_Map.Nearby_Location(Coord_To_Cell(coord), pbptr->Class_Of().Get_Speed());
                    coord = Cell_To_Coord(cell);
                }

                // loop though all possible facings and unlimbo at the first available adjacent coord.
                // intial facing is invalid, which allows the loop to first try to unlimbo the payback
                // object at the current or nearby coord.
                for (FacingType i = FACING_NONE; i == FACING_NONE || i < FACING_COUNT; ++i) {
                    if (i != FACING_NONE) {
                        coord = Coord_Get_Adjacent(m_Coord, i);
                    }

                    ++g_ScenarioInit;

                    if (pbptr->Unlimbo(coord, pbptr->Get_Facing().Get_Current())) {
                        break;
                    }

                    --g_ScenarioInit;
                }

                pbptr->Mark(MARK_PUT);
                pbptr->Do_Action(DO_20, true);

                // and now re-select the launcher.
                if (pbptr->Get_Bit16()) {
                    pbptr->Select();
                }

                paid_back = true;

                --g_ScenarioInit;

                // if we wasnt able to hand ourself back to the
                // payback object, delete them from the world.
                if (!paid_back) {
                    delete pbptr;
                }
            }
        }

        Limbo();
    }

    m_Class = nullptr;
    m_Payback = nullptr;
}

/**
 *
 */
void *BulletClass::operator new(size_t size)
{
    BulletClass *this_ptr = g_Bullets.Alloc();
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = true;
    }
    return this_ptr;
}

/**
 *
 */
void BulletClass::operator delete(void *ptr)
{
    BulletClass *this_ptr = static_cast<BulletClass *>(ptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = false;
    }
    g_Bullets.Free(this_ptr);
}

/**
 *
 */
coord_t BulletClass::Target_Coord() const
{
    DEBUG_ASSERT(m_IsActive);

    return Coord_From_Lepton_XY(Coord_Lepton_X(m_Coord), Coord_Lepton_Y(m_Coord) - m_Height);
}

/**
 *
 */
void BulletClass::AI()
{
    DEBUG_ASSERT(m_IsActive);

    bool to_explode = false;

    ObjectClass::AI();

    // check the bullet objects active state has not changed before continuing.
    if (!m_IsActive) {
        return;
    }

    if (Class_Of().Get_Arcing() || Class_Of().Is_Dropping()) {
        if (!m_IsFalling) {
            to_explode = true;
        }
    }

    // home in by updating the facing to point to the target.
    if (g_GameFrame & 1) {
        if (Target_Legal(m_Target) && Homes_In()) {
            coord_t target_coord = As_Coord(m_Target);
            DirType dir = Desired_Facing256(Coord_Lepton_X(m_Coord),
                Coord_Lepton_Y(m_Coord),
                Coord_Lepton_X(target_coord),
                Coord_Lepton_Y(target_coord)); // TODO: Replace with call to "Direction(coord, coord) when implemented.
            m_Facing.Set_Desired(dir);
        }
    }

    // create the bullets trailing animation. this logic flips its flag so it spawns every other frame.
    if (Class_Of().Animates()) {
        if (m_ToSpawnTrailer) {
            // fireball uses its own trailer animation, otherwise all other bullets use standard smoke puff.
            AnimClass *anim = new AnimClass((What_Type() == BULLET_FIREBALL ? ANIM_FIREBALLFADE : ANIM_SMOKE_PUFF), m_Coord, 1);
            DEBUG_ASSERT(anim != nullptr);
        }
        m_ToSpawnTrailer = !m_ToSpawnTrailer;
    }

    if (m_Facing.Has_Changed()) {
        m_Facing.Rotation_Adjust(Class_Of().Rate_Of_Turn());
    }

    coord_t new_coord = m_Coord;

    switch (m_Flyer.Physics(new_coord, m_Facing.Get_Current())) {
        case IMPACT_0: // fall-though.
        case IMPACT_1:

            Mark(MARK_REDRAW);

            // if we are the nuke-up bullet, and we have escaped the viewport, we can be deleted.
            if (What_Type() == BULLET_NUKEUP
                && (m_Payback != nullptr
                    && Distance_To_Target(m_Payback->As_Target())
                        > (CELL_LEPTONS * 12))) { // TODO: will need updating to be calculated based on display height.

                // we are out of view, now we can be deleted.
                delete this;

                break;
            }

            m_Coord = new_coord; // update our coord with adjusted fly coord.

            if (!to_explode) {
                to_explode = Is_Forced_To_Explode(m_Coord);
            }

            if (to_explode || (!Class_Of().Is_Dropping() && m_Fuse.Fuse_Checkup(m_Coord))) {
                Bullet_Explodes(to_explode);

                // bullet exploded, we can be deleted now.
                delete this;

                break;
            }

            if (Class_Of().Degenerates() && m_Health > 5) {
                --m_Health;
            }

            break;

        case IMPACT_2:
            if (m_Payback != nullptr && What_Type() == BULLET_GPSSATELLITE) {
                Do_Map_Reveal();
            }
            break;
    };
}

/**
 *
 */
LayerType BulletClass::In_Which_Layer() const
{
    DEBUG_ASSERT(m_IsActive);

    if (Class_Of().Is_UnderWater()) {
        return LAYER_SURFACE;
    }

    return ObjectClass::In_Which_Layer();
}

/**
 *
 */
coord_t BulletClass::Sort_Y() const
{
    DEBUG_ASSERT(m_IsActive);

    return Coord_Move(m_Coord, DIR_SOUTH, 128);
}

/**
 *
 */
BOOL BulletClass::Unlimbo(coord_t coord, DirType dir)
{
    DEBUG_ASSERT(m_IsActive);

    // clamp the height if we don't cruise high.
    if (!Class_Of().Get_High()) {
        m_Height = 0;
    }

    if (!ObjectClass::Unlimbo(coord)) {
        return false;
    }

    // remove this bullet from the display layer.
    g_Map.Remove(this, In_Which_Layer());

    DirType direction = dir;
    coord_t target_coord = As_Coord(m_Target);

    if (!Homes_In() && !Class_Of().Is_Dropping()) {
        direction = Desired_Facing256(Coord_Lepton_X(Center_Coord()),
            Coord_Lepton_Y(Center_Coord()),
            Coord_Lepton_X(target_coord),
            Coord_Lepton_Y(target_coord)); // TODO: Replace with call to "Direction(coord, coord) when implemented.
    }

    if (m_BBit1 || Class_Of().Is_Inaccurate() || (Target_Is_Cell(m_Target) || Target_Is_Infantry(m_Target))
        && (m_Warhead == WARHEAD_AP || Class_Of().Ranged())) {
        if (Class_Of().Get_Arcing()) {
            int scatter_dist = std::max<lepton_t>(std::min<lepton_t>((Distance(coord, target_coord) / 16) - 64, g_Rule.Homing_Scatter_Distance()), 0);
            direction += g_Scen.Get_Random_Value(0, 10) - 5;
            int coord_dist = g_Scen.Get_Random_Value(0, scatter_dist);
            target_coord = Coord_Scatter(target_coord, coord_dist);
        } else {
            int scatter_dist = std::max<lepton_t>(std::min<lepton_t>((Distance(coord, target_coord) / 16) - 64, g_Rule.Ballistic_Scatter_Distance()), 0);
            int coord_dist = g_Scen.Get_Random_Value(0, scatter_dist);
            target_coord = Coord_Move(target_coord, direction, coord_dist);
        }
    }

    // invisible bullets with max speed get instantly spawned at the target.
    if (Class_Of().Invisible()) {
        if (m_Speed == MPH_MAX) {
            m_Coord = target_coord;
        }
    }

    int dist = 255;
    if (!Class_Of().Is_Dropping()) {
        dist = Distance(m_Coord, target_coord) / m_Speed + 4;
    }

    MPHType speed = MPHType(m_Speed);
    if (speed == 255) {
        speed = MPH_MIN;
    }

    if (Class_Of().Get_Arcing()) {
        int arc_dist = Distance(Center_Coord(), target_coord);
        speed = MPHType(std::max((arc_dist / 32) + m_Speed, 25));
    }

    if (!Class_Of().Is_Dropping()) {
        m_Flyer.Fly_Speed(255, speed);
    }

    bool targ_aircraft = As_Aircraft(m_Target) != nullptr;

    m_Fuse.Arm_Fuse(m_Coord, target_coord, dist, targ_aircraft ? 0 : Class_Of().Arming_Delay());

    m_FallingHeight = 0;

    if (Class_Of().Get_Arcing()) {
        m_Height = 1;
        m_IsFalling = true;
        m_FallingHeight = g_Rule.Get_Gravity() * (Distance(Center_Coord(), target_coord) / 2 / (speed + 1));
        m_FallingHeight = std::max(m_FallingHeight, 10);
    }

    if (Class_Of().Is_Dropping()) {
        m_Height = 256;
        m_IsFalling = true;
        m_FallingHeight = 0;
        if (Class_Of().Get_Parachuted()) {
            AnimClass *anim = new AnimClass(ANIM_PARABOMB, Target_Coord());
            if (anim != nullptr) {
                anim->Attach_To(this);
            }
        }
    }

    m_Facing = direction;

    // place bullet back on the display layer.
    g_Map.Submit(this, In_Which_Layer());

    return true;
}

/**
 *
 */
void BulletClass::Detach(target_t target, BOOL a2)
{
    DEBUG_ASSERT(m_IsActive);

    if (m_Payback != nullptr) {
        if (m_Payback == As_Object(target)) {
            if (m_Payback->What_Am_I() != RTTI_INFANTRY
                || !static_cast<InfantryClass *>(m_Payback)->Class_Of().Is_Canine()) {
                m_Payback = nullptr;
            }
        }
    }

    if (a2) {
        if (target == m_Target) {
            m_Target = 0;
        }
    }
}

/**
 *
 */
const int16_t *BulletClass::Occupy_List(BOOL a1) const
{
    DEBUG_ASSERT(m_IsActive);

    // clang-format off
    // 3x6 list for the gps and atom bomb anims.
    // # is the origin cell.
    // 1: XXX
    // 2: XXX
    // 3: XXX
    // 4: X#X
    // 5: XXX
    // 6: XXX
    static int16_t const _gigundo[] = {
        -1,    0,    1,     // row 4
         127,  128,  129,   // row 3
        -129, -128, -127,   // row 5
         255,  256,  257,   // row 6
        -257, -256, -255,   // row 2
        -385, -384, -383,   // row 1
        LIST_END
    };
    // clang-format on

    // static for returning the calculated occupy list.
    static int16_t _list[32];

    if (Class_Of().Get_Gigundo()) {
        // now requires a larger list becuase of the gps and atom bomb anims.
        // Coord_Spillage_List() gigundo is too small for these.
        // return Coord_Spillage_List(m_Coord, 64);
        return _gigundo;
    }

    // bullet is grounded, calculate a spill list of 10 pixels around our coord.
    if (m_Height <= 0) {
        return Coord_Spillage_List(m_Coord, 10);
    }

    int16_t *dst_list = _list;

    // generate the list for our coord with a radius of 5 pixels.
    const int16_t *spill_list = Coord_Spillage_List(m_Coord, 5);
    while (spill_list != nullptr && *spill_list != LIST_END) {
        *dst_list++ = *spill_list++;
    }

    // generate and append to the above list another list for the bullets
    // height with a radius of 5 pixels.
    coord_t height_coord = Coord_Move(m_Coord, DIR_NORTH, m_Height);
    const int16_t *h_spill_list = Coord_Spillage_List(height_coord, 5);
    cell_t diff = Coord_To_Cell(height_coord) - Get_Cell();
    while (h_spill_list != nullptr && *h_spill_list != LIST_END) {
        *dst_list++ = diff + *h_spill_list++;
    }

    // terminate the list.
    *dst_list = LIST_END;

    return _list;
}

/**
 *
 */
void BulletClass::Draw_It(int x, int y, WindowNumberType window) const
{
    DEBUG_ASSERT(m_IsActive);

    // no need to draw invisible bullets...
    if (Class_Of().Invisible()) {
        return;
    }

    void *image = Get_Image_Data();
    if (image == nullptr) {
        return;
    }

    ShapeFlags draw_flags = SHAPE_NORMAL | SHAPE_WIN_REL | SHAPE_CENTER;

    void *table = DisplayClass::s_UnitShadow;
    int frame = Shape_Number();

    if (m_Height > 0) {
        if (Class_Of().Draw_Shadow()) {
            int shad_x = x;
            int shad_y = y;
            void *shad_image = image;
            int shad_frame = frame;

            if (Class_Of().Get_Parachuted()) {
                shad_x = x + Lepton_To_Pixel(m_Height / 2);
                shad_y += 10;
                shad_frame = 1;
                shad_image = AnimTypeClass::As_Reference(ANIM_PARABOMB).Get_Image_Data();
            }

            CC_Draw_Shape(shad_image, shad_frame, shad_x, shad_y, window, draw_flags | SHAPE_PREDATOR | SHAPE_FADING, nullptr, table);

            y -= Lepton_To_Pixel(m_Height);
        }
    }

    if (Class_Of().Is_Translucent()) {
        draw_flags |= SHAPE_GHOST;
    }

    // if the projectile is underwater, fake this effect by drawing it transparent.
    if (Class_Of().Is_UnderWater()) {
        table = DisplayClass::s_FadingShade;
        draw_flags |= SHAPE_PREDATOR | SHAPE_FADING;
    }

    CC_Draw_Shape(image, frame, x, y, window, draw_flags, nullptr, table);
}

/**
 *
 */
BOOL BulletClass::Mark(MarkType mark)
{
    DEBUG_ASSERT(m_IsActive);

    if (!ObjectClass::Mark(mark)) {
        return false;
    }

    // invisible bullets don't visually mark, so we don't need to do anything.
    if (Class_Of().Invisible()) {
        return true;
    }

    // refresh the cells we occupy.
    g_Map.Refresh_Cells(Get_Cell(), Occupy_List());

    return true;
}

/**
 *
 */
void BulletClass::Code_Pointers()
{
    DEBUG_ASSERT(m_IsActive);

    if (m_Payback != nullptr) {
        m_Payback = reinterpret_cast<TechnoClass *>(m_Payback->As_Target());
    }

    ObjectClass::Code_Pointers();
}

/**
 *
 */
void BulletClass::Decode_Pointers()
{
    DEBUG_ASSERT(m_IsActive);

    if (m_Payback != nullptr) {
        m_Payback = reinterpret_cast<TechnoClass *>(As_Techno(uintptr_t(m_Payback)));
        DEBUG_ASSERT(m_Payback != nullptr);
    }

    ObjectClass::Decode_Pointers();
}

/**
 *
 */
int BulletClass::Shape_Number() const
{
    DEBUG_ASSERT(m_IsActive);

    int number = 0;

    if (Class_Of().Rotates()) {
        number = TechnoClass::s_BodyShape32[FacingClass::s_Facing32[m_Facing.Get_Current()]];
    }

    if (Class_Of().Get_Frames() <= 0) {
        return number;
    }

    number += g_GameFrame % Class_Of().Get_Frames();

    return number;
}

/**
 *
 */
BOOL BulletClass::Is_Forced_To_Explode(coord_t &at) const
{
    DEBUG_ASSERT(m_IsActive);

    at = m_Coord;

    CellClass *cptr = &g_Map[Get_Cell()];
    DEBUG_ASSERT(cptr != nullptr);

    if (!Class_Of().Get_High()) {
        if (cptr->Has_High_Overlay()) {
            at = Cell_To_Coord(Coord_To_Cell(at));
            return true;
        }
    }

    if (Class_Of().Is_UnderWater()) {
        // get the distance between the sub-cell position of requested coord from the center coord of a cell.
        int dist = Distance(Coord_Sub_Cell(at), Coord_From_Lepton_XY(128, 128));

        TechnoClass *ctptr = cptr->Cell_Techno();
        if (cptr->Get_Land() != LAND_WATER || (dist < 85 && ctptr != nullptr && ctptr != m_Payback)) {
            if (ctptr != nullptr) {
                at = ctptr->Target_Coord();
            }

            if (cptr->Is_Bridge_Here()) {
                at = Coord_Centered(at);
            }

            return true;
        }
    }

    if (Class_Of().Is_Anti_Air()) {
        if (As_Aircraft(m_Target) != nullptr && Distance_To_Target(m_Target) < 128) {
            return true;
        }
    }

    return false;
}

/**
 *
 */
void BulletClass::Bullet_Explodes(BOOL a1)
{
    DEBUG_ASSERT(m_IsActive);

    if ((m_Payback != nullptr && m_Payback->What_Am_I() == RTTI_INFANTRY && static_cast<InfantryClass *>(m_Payback)->Class_Of().Is_Canine()
        || !a1) && (!Class_Of().Get_Arcing() && !Homes_In() && m_Fuse.Get_Position())) {
        m_Coord = m_Fuse.Get_Position();
    }

    if (!Target_Is_Aircraft(m_Target) || (As_Aircraft(m_Target)->In_Which_Layer() == LAYER_GROUND)) {
        // deal damage to ground units by proxy.
        Explosion_Damage(m_Coord, m_Health, m_Payback, m_Warhead);
        if (!m_IsActive) {
            return;
        }

    } else if (Distance_To_Target(m_Target) < 128) {
        // deal damage to air unit directly.
        AircraftClass *aptr = As_Aircraft(m_Target);
        if (aptr != nullptr) {
            int damage = m_Health;
            aptr->Take_Damage(damage, 0, m_Warhead, m_Payback);
        }
    }

    if (Class_Of().Invisible()) {
        m_Coord = Coord_Scatter(m_Coord, 32);
    }

    LandType land = g_Map[Get_Cell()].Get_Land();

    if (Target_Is_Aircraft(m_Target)) {
        AircraftClass *aptr = As_Aircraft(m_Target);
        if (aptr->In_Which_Layer() == LAYER_TOP) {
            land = LAND_NONE;
        }
    }

    AnimType anim = Combat_Anim(m_Health, m_Warhead, land);

    if (Target_Is_Vessel(m_Target)) {
        VesselClass *vptr = As_Vessel(m_Target);
        // DEBUG_ASSERT(vptr != nullptr);
        if (Get_Cell() == vptr->Center_Cell()) {
            switch (anim) {
                case ANIM_WATER_EXP1:
                    anim = ANIM_VEH_HIT1;
                    break;
                case ANIM_WATER_EXP2:
                    anim = ANIM_VEH_HIT2;
                    break;
                case ANIM_WATER_EXP3:
                    anim = ANIM_VEH_HIT3;
                    break;
                default:
                    break;
            };
        }
    }

    if (anim != ANIM_NONE) {
        AnimClass *aptr = new AnimClass(anim, m_Coord);
        if (aptr == nullptr && anim == ANIM_ATOMSFX) {
            HousesType house = HOUSES_NONE;
            if (m_Payback != nullptr) {
                house = m_Payback->Owner();
            }
            AnimClass::Do_Atom_Damage(house, Get_Cell());
        }
    }

    if (m_Payback != nullptr && What_Type() == BULLET_GPSSATELLITE) {
        Do_Map_Reveal();
    }
}

/**
 *
 */
void BulletClass::Do_Map_Reveal()
{
    HouseClass *hptr = m_Payback->Get_Owner_House();

    if (hptr->Is_Player()) {
        if (!g_Map.Is_Radar_Active()) {
            g_Map.Radar_Activate(1); // TODO: Argument type.
        }

        // iterate all the map cells and assign ownership to
        // the player, effectively revealing the whole map.
        for (cell_t cell = 0; cell < MAP_MAX_AREA; ++cell) {
            g_Map.Map_Cell(cell, hptr);
        }

        g_Map.Flag_To_Redraw(true);
    }

    hptr->Set_Map_Clear(true);
    hptr->Set_Visionary(true);
}

/**
 *
 */
void BulletClass::Init()
{
    g_Bullets.Free_All();
}
