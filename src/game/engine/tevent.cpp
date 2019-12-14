/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Classes holding event information for the trigger system.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "tevent.h"
#include "gadget.h"
#include "gbuffer.h"
#include "globals.h"
#include "house.h"
#include "object.h"
#include "scenario.h"
#include "team.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

using std::atoi;
using std::sprintf;
using std::strcmp;
using std::strtok;

EventChoiceClass EventChoiceClass::s_EventChoices[TEVENT_COUNT] = { EventChoiceClass(TEVENT_NO_EVENT),
    EventChoiceClass(TEVENT_ENTERED_BY),
    EventChoiceClass(TEVENT_SPIED_BY),
    EventChoiceClass(TEVENT_THIEVED_BY),
    EventChoiceClass(TEVENT_DISCOVERED_BY_PLAYER),
    EventChoiceClass(TEVENT_HOUSE_DISCOVERED),
    EventChoiceClass(TEVENT_ATTACKED),
    EventChoiceClass(TEVENT_DESTROYED),
    EventChoiceClass(TEVENT_ANY),
    EventChoiceClass(TEVENT_DESTROYED_ALL_UNITS),
    EventChoiceClass(TEVENT_DESTROYED_ALL_BUILDINGS),
    EventChoiceClass(TEVENT_DESTROYED_ALL),
    EventChoiceClass(TEVENT_CREDIT_EXCEED),
    EventChoiceClass(TEVENT_ELAPSED_TIME),
    EventChoiceClass(TEVENT_TIMER_EXPIRED),
    EventChoiceClass(TEVENT_DESTROYED_BUILDINGS),
    EventChoiceClass(TEVENT_DESTROYED_UNITS),
    EventChoiceClass(TEVENT_NO_FACTORIES),
    EventChoiceClass(TEVENT_CIVS_EVACUATED),
    EventChoiceClass(TEVENT_BUILD_BUILDING),
    EventChoiceClass(TEVENT_BUILD_UNIT),
    EventChoiceClass(TEVENT_BUILD_INFANTRY),
    EventChoiceClass(TEVENT_BUILD_AIRCRAFT),
    EventChoiceClass(TEVENT_LEAVES_MAP),
    EventChoiceClass(TEVENT_ZONE_ENTERED),
    EventChoiceClass(TEVENT_CROSSED_HORIZ_LINE),
    EventChoiceClass(TEVENT_CROSSED_VERT_LINE),
    EventChoiceClass(TEVENT_GLOBAL_SET),
    EventChoiceClass(TEVENT_GLOBAL_CLEAR),
    EventChoiceClass(TEVENT_DESTROYED_ALL_FAKES),
    EventChoiceClass(TEVENT_LOW_POWER),
    EventChoiceClass(TEVENT_ATTACHED_BRIDGE_DESTROYED),
    EventChoiceClass(TEVENT_BUILDING_EXISTS) };

const TEventClass::EventTextStruct TEventClass::s_EventText[TEVENT_COUNT] = {
    { "-No Event-", "This is a null event. There is no need to ever use this in a real trigger." },
    { "Entered by...",
        "Triggers when an infantry or vehicle enters the attached object. Typically this trigger is attached to a building "
        "or a cell." },
    { "Spied by...", "Detects when a spy has entered the attached building." },
    { "Thieved by...", "Triggers when a thief steals money from the specified house." },
    { "Discovered by player",
        "Detects when the attached object has been discovered by the player. Discovered means reavealed from under the "
        "shroud." },
    { "House Discovered...",
        "Triggers when the specified house has any of its units or buildings discovered by the player." },
    { "Attacked by anybody",
        "Triggers when the attached unit is attacked in some manner. Incidental damage or friendly fire does not count." },
    { "Destroyed by anybody",
        "Triggers when the attached object has been destroyed. Destroyed by incidental damage or friendly fire doesn't "
        "count." },
    { "Any Event", "When used alone, it will force the trigger to spring immediately." },
    { "Destroyed, Units, All...",
        "Triggers when all units of the specified house have been destroyed. Typically used for end of game conditions." },
    { "Destroyed, Buildings, All...",
        "Triggers when all buildings of the specified side have been destroyed. Typically used for end of game "
        "conditions." },
    { "Destroyed, All...",
        "Triggers when all objects owned by the specified house have been destroyed. This is the normal (destroy everyone) "
        "trigger condition for end of game." },
    { "Credits exceed (x100)...", "Triggers when the house (for this trigger) credit total exceeds this specified amount." },
    { "Elapsed Time (1/10th min)...",
        "Triggers when the elapsed time has expired. This time is initialized when the trigger is created. Timer is reset "
        "whenever trigger is sprung when trigger is 'persistant'." },
    { "Mission Timer Expired", "Triggers when the global mission timer (as displayed on the screen) has reached zero." },
    { "Destroyed, Buildings, #...",
        "Triggers when the number of buildings, owned by the trigger's specified house, have been destroyed." },
    { "Destroyed, Units, #...",
        "Triggers when the number of units, owned by the trigger's specified house, have been destroyed." },
    { "No Factories left", "Triggers when there are no factories left for the house specified in the trigger." },
    { "Civilians Evacuated", "Triggers when civilians have been evacuated (left the map)." },
    { "Build Building Type...",
        "When the trigger's house builds the building type specified, then this event will spring." },
    { "Build Unit Type...", "When the trigger's house builds the unit type specified, then this event will spring." },
    { "Build Infantry Type...",
        "When the trigger's house builds the infantry type specified, then this event will spring." },
    { "Build Aircraft Type...",
        "When the trigger's house builds the aircraft type specified, then this event will spring." },
    { "Leaves map (team)...",
        "Triggers when the specified team leaves the map. If the team is destroyed, it won't trigger. If all but one member "
        "is destroyed and that last member leaves the map, it WILL spring." },
    { "Zone Entry by...",
        "Triggers when a unit of the dpecified house enters the same zone that this trigger is located in. This trigger "
        "must be located in a cell and only a cell." },
    { "Crosses Horizontal Line...",
        "Triggers when a unit of the specified house crosses the horizontal line as indicated by the location of this "
        "trigger. This trigger must be placed in a cell." },
    { "Crosses Vertical Line...",
        "Triggers when a unit of the specified house crosses the vertical line as indicated by the location of this "
        "trigger. This trigger must be placed in a cell." },
    { "Global is set...", "Triggers when the specifed global (named in Globals.INI) is turned on." },
    { "Global is clear...", "Triggers when the specified global (named in Globals.INI) is turned off." },
    { "Destroyed, Fakes, All...", "Triggers when attached object is destroyed, but not if it infiltrates a building/unit." },
    { "Low Power...", "Triggers when the specified house's power falls below 100% level." },
    { "All bridges destroyed",
        "Triggers when the attached bridge is destroyed. A bridge is considered destroyed when an impassable gap is created "
        "in the bridge." },
    { "Building exists...",
        "Triggers when the building (owned by the house of this trigger) specified exists on the map. This works for "
        "buildings that are preexisting or constructed by deploying." }
};

BOOL TEventClass::operator()(TDelayEventClass &tdevent, TEventType tevent, HousesType house, ObjectClass *object, BOOL a5)
{
    if (a5) {
        tdevent.Set_Bit1(true);
    }

    if (tdevent.Get_Bit1()) {
        return true;
    }

    HouseClass *hc = nullptr;

    switch (m_Type) {
        case TEVENT_NO_EVENT:
            return false;
        case TEVENT_ENTERED_BY:
        case TEVENT_ZONE_ENTERED:
        case TEVENT_CROSSED_HORIZ_LINE:
        case TEVENT_CROSSED_VERT_LINE:
            if (tevent != m_Type && tevent != TEVENT_ANY) {
                return false;
            }

            if (object == nullptr || object->Owner() != (HousesType)m_Value) {
                return false;
            }

            tdevent.Set_Bit1(true);
            return true;
        case TEVENT_SPIED_BY:
            if (tevent != m_Type && tevent != TEVENT_ANY) {
                return false;
            }

            return true;
        case TEVENT_THIEVED_BY:
            hc = HouseClass::As_Pointer((HousesType)m_Value);

            if (hc == nullptr) {
                return true;
            }

            return hc->Is_Infiltrated();
        case TEVENT_DISCOVERED_BY_PLAYER:
            if (tevent != m_Type && tevent != TEVENT_ANY) {
                return false;
            }

            return true;
        case TEVENT_HOUSE_DISCOVERED:
            hc = HouseClass::As_Pointer((HousesType)m_Value);

            if (hc == nullptr) {
                return true;
            }

            return hc->Is_Discovered();
        case TEVENT_ATTACKED:
            if (tevent != m_Type && tevent != TEVENT_ANY) {
                return false;
            }

            return true;
        case TEVENT_DESTROYED:
            if (tevent != m_Type && tevent != TEVENT_ANY) {
                return false;
            }

            return true;
        case TEVENT_ANY:
            return true;
        case TEVENT_DESTROYED_ALL_UNITS: // Only seems to count infantry and units.
            hc = HouseClass::As_Pointer((HousesType)m_Value);

            if (hc == nullptr) {
                return true;
            }

            return hc->Get_IScan_Human() == 0 && hc->Get_UScan_Human() == 0;
        case TEVENT_DESTROYED_ALL_BUILDINGS:
            hc = HouseClass::As_Pointer((HousesType)m_Value);

            if (hc == nullptr) {
                return true;
            }

            return hc->Get_BScan_Human() == 0;
        case TEVENT_DESTROYED_ALL:
            hc = HouseClass::As_Pointer((HousesType)m_Value);

            if (hc == nullptr) {
                return true;
            }

            return hc->Get_VScan_Human() == 0 && hc->Get_IScan_Human() == 0 && hc->Get_UScan_Human() == 0
                && hc->Get_BScan_Human() == 0;
        case TEVENT_CREDIT_EXCEED:
            hc = HouseClass::As_Pointer(house);

            if (hc->Available_Money() < m_Value) {
                return false;
            }

            return true;
        case TEVENT_ELAPSED_TIME:
            return tdevent.Timer_Expired();
        case TEVENT_TIMER_EXPIRED:
            return g_Scen.Global_Timer().Expired();
        case TEVENT_DESTROYED_BUILDINGS:
            hc = HouseClass::As_Pointer(house);

            if (hc == nullptr) {
                return true;
            }

            return hc->Buildings_Lost() >= m_Value;
        case TEVENT_DESTROYED_UNITS:
            hc = HouseClass::As_Pointer(house);

            if (hc == nullptr) {
                return true;
            }

            return hc->Units_Lost() >= m_Value;
        case TEVENT_NO_FACTORIES:
            hc = HouseClass::As_Pointer(house);

            if (hc == nullptr) {
                return true;
            }

            // TODO, this magic number is constructed from a series of 1 << BuildingType values OR'd together.
            return (hc->Get_BScan_Built() & 0x00610804) == 0;
        case TEVENT_CIVS_EVACUATED:
            hc = HouseClass::As_Pointer(house);

            if (hc == nullptr) {
                return true;
            }

            return hc->Civilians_Evacuated();

        case TEVENT_BUILD_BUILDING:
            hc = HouseClass::As_Pointer(house);

            if (hc == nullptr) {
                return true;
            }

            if (hc->Just_Building() == (BuildingType)m_Value) {
                tdevent.Set_Bit1(true);
                return true;
            }

            return false;
        case TEVENT_BUILD_UNIT:
            hc = HouseClass::As_Pointer(house);

            if (hc == nullptr) {
                return true;
            }

            if (hc->Just_Unit() == (UnitType)m_Value) {
                tdevent.Set_Bit1(true);
                return true;
            }

            return false;
        case TEVENT_BUILD_INFANTRY:
            hc = HouseClass::As_Pointer(house);

            if (hc == nullptr) {
                return true;
            }

            if (hc->Just_Infantry() == (InfantryType)m_Value) {
                tdevent.Set_Bit1(true);
                return true;
            }

            return false;
        case TEVENT_BUILD_AIRCRAFT:
            hc = HouseClass::As_Pointer(house);

            if (hc == nullptr) {
                return true;
            }

            if (hc->Just_Aircraft() == (AircraftType)m_Value) {
                tdevent.Set_Bit1(true);
                return true;
            }

            return false;
        case TEVENT_LEAVES_MAP:
            hc = HouseClass::As_Pointer(house);

            if (hc == nullptr) {
                return true;
            }

            for (int i = 0; i < g_Teams.Count(); ++i) {
                if (g_Teams[i].Get_Class() == m_TeamType && g_Teams[i].Get_field_54() == nullptr
                    && g_Teams[i].Get_Bit2_4()) {
                    tdevent.Set_Bit1(true);
                    return true;
                }
            }

            return false;
        case TEVENT_GLOBAL_SET:
            return g_Scen.Get_Global(m_Value);
        case TEVENT_GLOBAL_CLEAR:
            return !g_Scen.Get_Global(m_Value);
        case TEVENT_DESTROYED_ALL_FAKES:
            return true;
        case TEVENT_LOW_POWER:
            hc = HouseClass::As_Pointer(house);

            if (hc == nullptr) {
                return true;
            }

            hc = HouseClass::As_Pointer((HousesType)m_Value);

            if (hc == nullptr) {
                return true;
            }

            return hc->Power_Fraction() < fixed_t(1, 1);
        case TEVENT_ATTACHED_BRIDGE_DESTROYED:
            if (g_Scen.Get_Bridge_Count() == 0) {
                tdevent.Set_Bit1(true);
                return true;
            }

            return false;
        case TEVENT_BUILDING_EXISTS:
            hc = HouseClass::As_Pointer(house);

            if (hc == nullptr) {
                return true;
            }

            if ((hc->Get_BScan_Human() & (1 << (m_Value & 0xFF))) != 0) {
                return true;
            }

            return false;
        default:
            break;
    }

    return false;
}

void TEventClass::Reset(TDelayEventClass &tdevent) const
{
    tdevent.Set_Bit1(false);

    if (m_Type == TEVENT_ELAPSED_TIME) {
        tdevent.Set_Delay_Timer(90 * m_Value);
    }
}

void TEventClass::Build_INI_Entry(char *entry_buffer) const
{
    sprintf(entry_buffer, "%d,%d,%d", m_Type, g_TeamTypes.Logical_ID(m_TeamType), m_Value);
}

void TEventClass::Read_INI()
{
    switch (g_INIFormat) {
        case 0: // Similar to TD trigger format? Skips none event related entries.
            m_Type = (TEventType)atoi(strtok(nullptr, ","));
            // skip next 2 entries.
            strtok(nullptr, ",");
            strtok(nullptr, ",");
            m_TeamType = TeamTypeClass::From_Name(strtok(nullptr, ","));
            m_Value = atoi(strtok(nullptr, ","));
            // skip another entry.
            strtok(nullptr, ",");
            break;
        case 1: { // Looks like this format used a more concise event string.
            m_Type = TEVENT_NO_EVENT;
            m_TeamType = nullptr;
            m_Value = -1;
            const char *tok = strtok(nullptr, ",");

            if (tok != nullptr) {
                m_Type = (TEventType)atoi(tok);
            }

            tok = strtok(nullptr, ",");

            if (tok != nullptr) {
                if (Event_Needs() == NEED_TEAM) {
                    m_TeamType = &g_TeamTypes[atoi(tok)];
                } else {
                    m_Value = atoi(tok);
                }
            }

            break;
        }
        default: // 2 and 3, default behaviour.
            m_Type = (TEventType)atoi(strtok(nullptr, ","));
            m_TeamType.Set_ID(atoi(strtok(nullptr, ","))); // Takes the maps word for it.
            m_Value = atoi(strtok(nullptr, ","));
            break;
    }
}

/**
 * Previously a global function taking TEventType, but makes more sense as member.
 * @return A bitfield describing what this event can attach to.
 */
AttachType TEventClass::Attaches_To()
{
    AttachType ret = ATTACH_NONE;

    switch (m_Type) {
        case TEVENT_NO_EVENT:
        case TEVENT_ENTERED_BY:
        case TEVENT_DISCOVERED_BY_PLAYER:
        case TEVENT_ANY:
        case TEVENT_ZONE_ENTERED:
        case TEVENT_CROSSED_HORIZ_LINE:
        case TEVENT_CROSSED_VERT_LINE:
            ret |= ATTACH_CELL;
        default:
            break;
    }

    switch (m_Type) {
        case TEVENT_NO_EVENT:
        case TEVENT_ENTERED_BY:
        case TEVENT_SPIED_BY:
        case TEVENT_DISCOVERED_BY_PLAYER:
        case TEVENT_ATTACKED:
        case TEVENT_DESTROYED:
        case TEVENT_ANY:
            ret |= ATTACH_OBJECT;
        default:
            break;
    }

    switch (m_Type) {
        case TEVENT_ANY:
        case TEVENT_ZONE_ENTERED:
            ret |= ATTACH_MAP;
        default:
            break;
    }

    switch (m_Type) {
        case TEVENT_THIEVED_BY:
        case TEVENT_HOUSE_DISCOVERED:
        case TEVENT_ANY:
        case TEVENT_DESTROYED_ALL_UNITS:
        case TEVENT_DESTROYED_ALL_BUILDINGS:
        case TEVENT_DESTROYED_ALL:
        case TEVENT_CREDIT_EXCEED:
        case TEVENT_DESTROYED_BUILDINGS:
        case TEVENT_DESTROYED_UNITS:
        case TEVENT_NO_FACTORIES:
        case TEVENT_CIVS_EVACUATED:
        case TEVENT_BUILD_BUILDING:
        case TEVENT_BUILD_UNIT:
        case TEVENT_BUILD_INFANTRY:
        case TEVENT_BUILD_AIRCRAFT:
        case TEVENT_DESTROYED_ALL_FAKES:
        case TEVENT_LOW_POWER:
        case TEVENT_BUILDING_EXISTS:
            ret |= ATTACH_HOUSE;
        default:
            break;
    }

    switch (m_Type) {
        case TEVENT_ANY:
        case TEVENT_ELAPSED_TIME:
        case TEVENT_TIMER_EXPIRED:
        case TEVENT_LEAVES_MAP:
        case TEVENT_GLOBAL_SET:
        case TEVENT_GLOBAL_CLEAR:
        case TEVENT_ATTACHED_BRIDGE_DESTROYED:
            ret |= ATTACH_LOGIC;
        default:
            break;
    }

    return ret;
}

TEventType TEventClass::Event_From_Name(const char *name)
{
    for (TEventType i = TEVENT_FIRST; i < TEVENT_COUNT; ++i) {
        if (strcmp(name, s_EventText[i].m_Name) == 0) {
            return i;
        }
    }

    return TEVENT_NO_EVENT;
}

const char *TEventClass::Name_From_Event(TEventType tevent)
{
    return s_EventText[tevent].m_Name;
}

NeedType TEventClass::Event_Needs(TEventType tevent)
{
    switch (tevent) {
        case TEVENT_NO_EVENT:
        case TEVENT_SPIED_BY:
        case TEVENT_DISCOVERED_BY_PLAYER:
        case TEVENT_DESTROYED:
        case TEVENT_ATTACKED:
        case TEVENT_ANY:
        case TEVENT_TIMER_EXPIRED:
        case TEVENT_NO_FACTORIES:
        case TEVENT_CIVS_EVACUATED:
        case TEVENT_DESTROYED_ALL_FAKES:
        case TEVENT_ATTACHED_BRIDGE_DESTROYED:
            return NEED_NOTHING;
        case TEVENT_ENTERED_BY:
        case TEVENT_THIEVED_BY:
        case TEVENT_HOUSE_DISCOVERED:
        case TEVENT_DESTROYED_ALL_UNITS:
        case TEVENT_DESTROYED_ALL_BUILDINGS:
        case TEVENT_DESTROYED_ALL:
        case TEVENT_ZONE_ENTERED:
        case TEVENT_CROSSED_HORIZ_LINE:
        case TEVENT_CROSSED_VERT_LINE:
        case TEVENT_LOW_POWER:
            return NEED_HOUSE;
        case TEVENT_CREDIT_EXCEED:
        case TEVENT_ELAPSED_TIME:
        case TEVENT_DESTROYED_BUILDINGS:
        case TEVENT_DESTROYED_UNITS:
        case TEVENT_GLOBAL_SET:
        case TEVENT_GLOBAL_CLEAR:
            return NEED_NUMBER;
        case TEVENT_BUILD_BUILDING:
        case TEVENT_BUILDING_EXISTS:
            return NEED_BUILDING;
        case TEVENT_BUILD_UNIT:
            return NEED_UNIT;
        case TEVENT_BUILD_INFANTRY:
            return NEED_INFANTRY;
        case TEVENT_BUILD_AIRCRAFT:
            return NEED_AIRCRAFT;
        case TEVENT_LEAVES_MAP:
            return NEED_TEAM;
        default:
            break;
    }

    return NEED_NOTHING;
}

void EventChoiceClass::Draw_It(int index, int x, int y, int x_max, int y_max, BOOL selected, TextPrintType style) const
{
    static int _tabs[] = { 13, 40 };
    RemapControlType *remapper = GadgetClass::Get_Color_Scheme();

    if ((style & TPF_FONTS) == TPF_6PT_GRAD || (style & TPF_FONTS) == TPF_EDITOR) {
        if (selected) {
            style |= TPF_USE_BRIGHT;
            g_LogicPage->Fill_Rect(x, y, ((x + x_max) - 1), ((y + y_max) - 1), remapper->WindowPalette[0]);
        } else if (!(style & TPF_USE_GRAD_PAL)) {
            style |= TPF_USE_MEDIUM;
        }
    } else {
        remapper = (selected ? &g_ColorRemaps[REMAP_10] : &g_ColorRemaps[REMAP_5]);
    }

    Conquer_Clip_Text_Print(TEventClass::Name_From_Event(m_Event), x, y, remapper, COLOR_TBLACK, style, x_max, _tabs);
}

int EventChoiceClass::Comp(const void *a, const void *b)
{
    return strcasecmp(TEventClass::Name_From_Event((*(const EventChoiceClass **)a)->m_Event),
        TEventClass::Name_From_Event((*(const EventChoiceClass **)b)->m_Event));
}
