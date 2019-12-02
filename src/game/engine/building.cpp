/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
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
#include "building.h"
#include "drawshape.h"
#include "gameevent.h"
#include "globals.h"
#include "house.h"
#include "iomap.h"
#include "queue.h"
#include "rules.h"
#include <algorithm>

#ifndef GAME_DLL
TFixedIHeapClass<BuildingClass> g_Buildings;
#endif

BuildingClass::BuildingClass(BuildingType type, HousesType house) :
    TechnoClass(RTTI_BUILDING, g_Buildings.ID(this), house)
{
}

BuildingClass::BuildingClass(const BuildingClass &that) :
    TechnoClass(that)
{
}

BuildingClass::BuildingClass(const NoInitClass &noinit) :
    TechnoClass(noinit)
{
}

BuildingClass::~BuildingClass()
{}

/**
 *
 *
 * @address 0x00460AD0
 */
const BuildingTypeClass &BuildingClass::Class_Of() const
{
    return *m_Class;
}

/**
 *
 *
 * @address 0x0045DE68
 */
void BuildingClass::Death_Announcement(TechnoClass *killer) const
{
    if (killer != nullptr && m_OwnerHouse->Player_Has_Control()) {
        Speak(VOX_STRUCTURE_DESTROYED);
    }
}

/**
 *
 *
 * @address 0x0045EAC0
 */
BOOL BuildingClass::Can_Player_Move() const
{
    return What_Type() == BUILDING_FACT;
}

/**
 *
 *
 * @address 0x0045E3E4
 */
void BuildingClass::Detach(target_t target, int a2)
{
    TechnoClass::Detach(target, a2);
    if (m_SabotagedBy == target) {
        m_SabotagedBy = 0;
    }
    if (m_AttachedAnim == target) {
        m_AttachedAnim = 0;
    }
}

/**
 *
 *
 * @address 0x0046072C
 */
int BuildingClass::Value() const
{
    if (Class_Of().Is_Fake()) {
        BuildingTypeClass *btptr = nullptr;
        switch (What_Type()) {
            case BUILDING_WEAF:
                btptr = &BuildingTypeClass::As_Reference(BUILDING_WEAP);
                return btptr->Get_ThreatPosed() + btptr->Get_ThreatPoints();
            case BUILDING_FACF:
                btptr = &BuildingTypeClass::As_Reference(BUILDING_FACT);
                return btptr->Get_ThreatPosed() + btptr->Get_ThreatPoints();
            case BUILDING_SYRF:
                btptr = &BuildingTypeClass::As_Reference(BUILDING_SYRD);
                return btptr->Get_ThreatPosed() + btptr->Get_ThreatPoints();
            case BUILDING_SPEF:
                btptr = &BuildingTypeClass::As_Reference(BUILDING_SPEN);
                return btptr->Get_ThreatPosed() + btptr->Get_ThreatPoints();
            case BUILDING_DOMF:
                btptr = &BuildingTypeClass::As_Reference(BUILDING_DOME);
                return btptr->Get_ThreatPosed() + btptr->Get_ThreatPoints();
            default:
                DEBUG_LOG("BuildingClass::Value - Unhandled fake!\n");
                break;
        }
    }
    return TechnoClass::Value();
}

/**
 *
 *
 * @address 0x004606D0
 */
void *BuildingClass::Get_Image_Data() const
{
    if (m_CurrentState == BSTATE_0) {
        return Class_Of().Get_Buildup_Data();
    }
    return TechnoClass::Get_Image_Data();
}

/**
 *
 *
 */
void BuildingClass::Active_Click_With(ActionType action, ObjectClass *object)
{
    switch (action) {
        case ACTION_ATTACK:
            if (object != nullptr) {
                Player_Assign_Mission(MISSION_ATTACK, object->As_Target());
            }
            break;

        case ACTION_SELF:
            if (Class_Of().Factory_Type() != RTTI_NONE) {
                GameEventClass event(GameEventClass::EVENT_PRIMARY, object);
                OutgoingEvents.Add(event);
            }
            break;

        default:
            break;
    }
}

/**
 *
 *
 */
void BuildingClass::Active_Click_With(ActionType action, cell_t cellnum)
{
    switch (action) {
        case ACTION_ATTACK:
            Player_Assign_Mission(MISSION_ATTACK, ::As_Target(cellnum));
            break;

        case ACTION_MOVE:
            if (What_Type() == BUILDING_FACT) {
                GameEventClass archev(GameEventClass::EVENT_ARCHIVE, this, ::As_Target(cellnum));
                OutgoingEvents.Add(archev);

                GameEventClass sellev(GameEventClass::EVENT_SELL, this);
                OutgoingEvents.Add(sellev);
            }
            break;
        default:
            break;
    }
}
/**
 *
 *
 */
void BuildingClass::Draw_It(int x, int y, WindowNumberType window)
{
    void *image = Get_Image_Data();
    if (image != nullptr) {
        g_isTheaterShape = Class_Of().Is_Theater();
        Techno_Draw_Object(image, Shape_Number(), x, y, window);
        g_isTheaterShape = false;
        if (m_CurrentState != BSTATE_0) {
            if (m_Tethered && m_Radio != nullptr && !m_Radio->In_Limbo() && m_Radio->What_Am_I() != RTTI_BUILDING) {
                int radio_x = Lepton_To_Pixel(Coord_Lepton_X(m_Radio->Render_Coord()));
                int offset_x = radio_x - Lepton_To_Pixel(Coord_Lepton_X(Render_Coord()));
                int radio_y = Lepton_To_Pixel(Coord_Lepton_Y(m_Radio->Render_Coord()));
                int offset_y = radio_y - Lepton_To_Pixel(Coord_Lepton_Y(Render_Coord()));

                m_Radio->Draw_It(x + offset_x, y + offset_y, window);
                m_Radio->Set_ToDisplay(false);
            }

            // draw war factory doors
            if (What_Type() == BUILDING_WEAP || What_Type() == BUILDING_WEAF) {
                int frame = m_Door.Door_Stage();
                if (Health_Ratio() <= Rule.Condition_Yellow()) {
                    frame += 4;
                }
                Techno_Draw_Object(BuildingTypeClass::g_WarFactoryOverlay, frame, x, y, window);
            }

            // draw repair shape on building
            if (m_Bit16 && m_Bit32) {
                CC_Draw_Shape(ObjectTypeClass::SelectShapes, 2, x, y, window, SHAPE_CENTER | SHAPE_VIEWPORT_REL);
            }
        }
        TechnoClass::Draw_It(x, y, window);

        // draw what's currently in production has spied this
        if ((1 << g_PlayerPtr->What_Type()) & m_Spied && m_Selected) {
            FactoryClass *fptr = nullptr;
            if (m_OwnerHouse->Is_Human()) {
                fptr = m_OwnerHouse->Fetch_Factory(Class_Of().Factory_Type());
            } else {
                fptr = m_Factory;
            }
            if (fptr != nullptr) {
                TechnoClass *tptr = fptr->Get_Object();
                if (tptr != nullptr) {
                    CC_Draw_Shape(tptr->Class_Of().Get_Cameo_Data(), 0, x, y, window, SHAPE_CENTER | SHAPE_VIEWPORT_REL);
                }
            }
        }
    }
}

int BuildingClass::Shape_Number()
{
#ifdef GAME_DLL
    DEFINE_CALL(func, 0x00455618, int, BuildingClass *);
    return func(this);
#else
    return 0;
#endif
}

/**
 *
 *
 */
int BuildingClass::Power_Output()
{
    if (Class_Of().Power_Output() > 0) {
        fixed_t power(m_field_EA, Class_Of().Get_Strength());
        return power * Class_Of().Power_Output();
    }
    return 0;
}

/**
 *
 *
 */
BOOL BuildingClass::Limbo()
{
    if (!m_InLimbo) {
        m_OwnerHouse->Active_Remove(this);
        m_OwnerHouse->Flag_To_Recalc();
        m_OwnerHouse->Recalc_Center();
        m_OwnerHouse->Adjust_Power(-Power_Output());
        m_OwnerHouse->Adjust_Drain(-Class_Of().Power_Drain());
        m_OwnerHouse->Adjust_Capacity(-Class_Of().Storage_Capacity(), true);

        if (m_OwnerHouse == g_PlayerPtr) {
            Map.Flag_Power_To_Redraw();
            Map.Flag_To_Redraw();
        }
    }
    return TechnoClass::Limbo();
}
