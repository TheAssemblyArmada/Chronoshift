/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Enumerations and other constant defintions that don't have a better home.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef GAMETYPES_H
#define GAMETYPES_H

#include "always.h"
#include "rtti.h"

#define DEF_TICKS_PER_SECOND 15
#define DEF_TICKS_PER_MINUTE (DEF_TICKS_PER_SECOND * 60)
#define TICKS_HALF_MINUTE (DEF_TICKS_PER_MINUTE / 2)

typedef uint32_t coord_t;
typedef int16_t cell_t;
typedef int32_t target_t;
typedef int16_t lepton_t;

inline int Cells_To_Pixels(coord_t cells)
{
    return (cells * 24);
}

inline coord_t Pixels_To_Cells(int pixels)
{
    return (pixels / 24);
}

enum ControlID
{
    CONTROL_0 = 0, // Special case to ignore action?

    // Standard Button ID's
    BUTTON_OK = 1,
    BUTTON_CANCEL = 2,
    BUTTON_YES = 3,
    BUTTON_NO = 4,

    // Ingame UI Gadget ID's
    BUTTON_REPAIR = 101,
    // BUTTON_
    BUTTON_SELL = 103,
    // BUTTON_
    BUTTON_ZOOM = 105,

    // Sidebar UI ID's
    GADGET_STRIP_COLUMN_LEFT = 200,
    GADGET_STRIP_COLUMN_RIGHT = 210,
    GADGET_STRIP_SELECT_BUTTON = 220,

    // Sound Controls ID's
    BUTTON_SOUND_STOP = 605,
    BUTTON_SOUND_PLAY = 606,
    BUTTON_SOUND_SUFFLE = 607,
    BUTTON_SOUND_REPEAT = 608,
    BUTTON_SOUND_OK = 609,
    SLIDER_SCORE_VOL = 610,
    SLIDER_SOUND_VOL = 611,

    // Menu Button ID's
    BUTTON_CS = 1000,
    BUTTON_AM = 1001,
    BUTTON_NEW_GAME = 1002,
    BUTTON_LOAD_GAME = 1003,
    BUTTON_MPLAYER_GAME = 1004,
    BUTTON_SNEAK_PEAK = 1005,
    BUTTON_MOVIES = 1005,
    BUTTON_MAP_EDITOR = 1006,
    BUTTON_OPTIONS = 1007,
    BUTTON_EXIT = 1008,

    // Unknown
    BUTTON__ = 1010,

    // Mutliplayer Button ID's
    BUTTON_MODEM_SERIAL = 1020,
    BUTTON_SKIRMISH = 1021,
    BUTTON_NETWORK = 1022,
    BUTTON_INTERNET = 1023,

    // Fetch Difficulty IDs
    SLIDER_DIFFICULTY = 1030,

    CONTROL_SB_SELECT_BUTTON = 0, // TODO: Work out of this is special case or not.

    // Right Click Context Menu ID
    CONTROL_CONTEXT_MENU = 8000,

    BUTTON_NONE = -1
};

DEFINE_ENUMERATION_BITWISE_OPERATORS(ControlID);

// needs to stay here for now.
// inf death anims
enum InfDeathType
{
    INF_DEATH_INVALID = -1,
    INF_DEATH_INSTANT = 0,
    INF_DEATH_TWIRL = 1,
    INF_DEATH_EXPLODES = 2,
    INF_DEATH_FLYING = 3,
    INF_DEATH_BURN = 4,
    INF_DEATH_ELECTRO = 5,
};

// might move back to warhead.h
// warhead explosions
enum WarheadExplosionType
{
    WH_EXPLOSION_INVALID = -1,
    WH_EXPLOSION_NONE = 0,
    WH_EXPLOSION_PIFF = 1,
    WH_EXPLOSION_PIFFS = 2,
    WH_EXPLOSION_FIRE = 3,
    WH_EXPLOSION_FRAGS = 4,
    WH_EXPLOSION_POPS = 5,
    WH_EXPLOSION_NUKE = 6,
};

enum TeamGroupType
{
    GROUP_0,
    GROUP_1,
    GROUP_2,
    GROUP_3,
    GROUP_4,
    GROUP_5,
    GROUP_6,
    GROUP_7,
    GROUP_8,
    GROUP_9,

    GROUP_COUNT,

    GROUP_FIRST = GROUP_0,
    GROUP_NONE = -1,
};

// this is temp (made up name) until we work out what it really is
enum TeamEventType
{
    TEAM_EVENT_NONE = -1,
    TEAM_EVENT_0 = 0, // defualt
    TEAM_EVENT_1 = 1, // Shift
    TEAM_EVENT_2 = 2, // Ctrl
    TEAM_EVENT_3 = 3, // Alt
    TEAM_EVENT_COUNT = 4
};

// threat type is linked to Quarry
// Quarry is resolved to the threats to target
// These seem to serve two purposes, flagging what threat something poses,
// and what threats something is vulnerable to
enum ThreatType
{
    THREAT_NONE = -1,

    // absolutely anything?
    THREAT_ANY = 0,

    // Threat scanning is limited to the range of the object.
    // Usually set when a unit is on a Guard mission.
    THREAT_RANGE = 1 << 0, // 0x1

    // Threat scan is bound to a defined Area.
    // Usually set when a unit is on a Area Guard or Patrol mission.
    THREAT_AREA = 1 << 1, // 0x2

    // any aircraft
    THREAT_AIRCRAFT = 1 << 2, // 0x4

    // any infantry
    THREAT_INFANTRY = 1 << 3, // 0x8

    // any unit
    THREAT_VEHICLES = 1 << 4, // 0x10

    // any building
    THREAT_BUILDINGS = 1 << 5, // 0x20

    // anything that will threaten a house's economy
    // this includes buildings with storage not 0 and harvesters
    THREAT_ECONOMY = 1 << 6, // 0x40

    // any vessels
    THREAT_VESSELS = 1 << 7, // 0x80

    // usually harmless? civilian? easy targets?
    THREAT_CIVILIAN = 1 << 8, // 0x100

    // threats that can infiltrate, in RA this seems to only be Thieves
    // in YR this includes Thieves, Spies, Engineers, units with C4
    THREAT_INFILTRATORS = 1 << 9, // 0x200

    // any fake buildings
    THREAT_FAKES = 1 << 10, // 0x400

    // any power facilities
    THREAT_POWER_FACILTIES = 1 << 11, // 0x800

    // any factories
    THREAT_FACTORIES = 1 << 12, // 0x1000

    // any base defenses
    THREAT_BASE_DEFENSES = 1 << 13, // 0x2000

    // any ground
    THREAT_GROUND = THREAT_INFANTRY | THREAT_VEHICLES | THREAT_BUILDINGS, // 0x38

    // set in InfantryClass::Greatest_Threat if Thief
    THREAT_THEFT = THREAT_INFILTRATORS | THREAT_ECONOMY, // 0x240

    // checked in TechnoClass::Greatest_Threat
    THREAT_2108 = THREAT_INFANTRY | THREAT_CIVILIAN | THREAT_BASE_DEFENSES, // 0x2108

    // checked in TechnoClass::Greatest_Threat
    // 
    THREAT_3D40 = THREAT_ECONOMY | THREAT_CIVILIAN | THREAT_FAKES | THREAT_POWER_FACILTIES
        | THREAT_FACTORIES | THREAT_BASE_DEFENSES, // 0x3D40
};

// This is used within TechnoClass::Greatest_Threat
enum ThreatScanType
{
    THREAT_SCAN_AIRCRAFT = 1 << RTTI_AIRCRAFT,
    THREAT_SCAN_BUILDING = 1 << RTTI_BUILDING,
    THREAT_SCAN_INFANTRY = 1 << RTTI_INFANTRY,
    THREAT_SCAN_UNIT = 1 << RTTI_UNIT,
    THREAT_SCAN_VESSEL = 1 << RTTI_VESSEL,
};

DEFINE_ENUMERATION_BITWISE_OPERATORS(ThreatType);

enum UrgencyType
{
    URGENCY_FIRST = 0,
    URGENCY_NOTHING = 0,
    URGENCY_LOW_MONEY, // Low on money.
    URGENCY_LOW_POWER, // Low on power.
    URGENCY_3,
    URGENCY_FIRE_SALE, // TODO, used for Fire_Sale and setting the whole house to Hunt (last resort attack?)
    URGENCY_COUNT
};

DEFINE_ENUMERATION_BITWISE_OPERATORS(UrgencyType);

enum SpeedType
{
    SPEED_NONE = -1,
    SPEED_FIRST = 0,
    SPEED_FOOT = 0,
    SPEED_TRACK,
    SPEED_WHEEL,
    SPEED_WINGED,
    SPEED_FLOAT,
    SPEED_COUNT
};

DEFINE_ENUMERATION_OPERATORS(SpeedType);

enum FireErrorType
{
    FIRE_NONE = -1,     // no valid value
    FIRE_OK,            // no problem, can fire
    FIRE_AMMO,          // no ammo
    FIRE_FACING,        // bad facing
    FIRE_REARM,         // still reloading
    FIRE_ROTATING,      // busy rotating
    FIRE_ILLEGAL,       // can't fire
    FIRE_CANT,          // sorry, I can't do that
    FIRE_MOVING,        // moving, can't fire
    FIRE_RANGE,         // out of range
    FIRE_CLOAKED,       // need to decloak
    FIRE_BUSY,          // busy, please hold
    FIRE_COUNT,
};

DEFINE_ENUMERATION_OPERATORS(FireErrorType);

//TODO: This enum must stay here becuase of a ciruclar includ issue with
//      object.h using RadioMessageType.
//TODO2: one of these is RADIO_CONTACT and one is RADIO_ENTER
enum RadioMessageType
{
    RADIO_NONE = -1,
    RADIO_FIRST = 0,
    RADIO_STATIC = 0,              // "static (no message)"
    RADIO_ROGER,                   // "Roger."
    RADIO_HELLO,                   // "Come in."
    RADIO_OVER_AND_OUT,            // "Over and out."
    RADIO_NEED_TRANSPORT,          // "Requesting transport."
    RADIO_ATTACH_TO_TRANSPORT,     // "Attach to transport."
    RADIO_GOT_DELIVERY,            // "I've got a delivery for you."
    RADIO_PERFORMING_MANEUVER,     // "I'm performing load/unload maneuver. Be careful."
    RADIO_IM_CLEAR,                // "I'm clear."
    RADIO_CLEAR_TO_UNLOAD,         // "You are clear to unload. Driving away now.
    RADIO_UNABLE_TO_COMPLY,        // "Am unable to comply."
    RADIO_STARTING_CONSTRUCTION,   // "I'm starting construction now... act busy."
    RADIO_FINISHED_CONSTRUCTION,   // "I've finished construction. You are free."
    RADIO_WE_BUMPED,               // "We bumped, redraw yourself please."
    RADIO_TRYING_TO_LOAD,          // "I'm trying to load up now."
    RADIO_BECOME_PASSENGER,        // "May I become a passenger?"
    RADIO_READY_TO_RECEIVE,        // "Are you ready to receive shipment?"
    RADIO_NEED_A_LIFT,             // "Are you trying to become a passenger?"
    RADIO_MOVE_TO_LOCATION,        // "Move to location X."
    RADIO_NEED_TO_MOVE,            // "Do you need to move?"
    RADIO_NOW_WHAT,                // "All right already. Now what?"
    RADIO_IM_IN,                   // "I'm a passenger now."
    RADIO_BACK_UP,                 // "Backup into refinery now."
    RADIO_RUN_AWAY,                // "Run away!"
    RADIO_TETHER_ESTABLISHED,      // "Tether established."
    RADIO_TETHER_BROKEN,           // "Tether broken."
    RADIO_REPAIR_ONE_STEP,         // "Repair one step."
    RADIO_FIGHTING_WORDS,          // "Are you prepared to fight?"
    RADIO_ATTACK_THIS,             // "Attack this target please."
    RADIO_RELOAD,                  // "Reload one step."
    RADIO_SCREWED,                 // "Circumstances prevent success."
    RADIO_ALL_DONE,                // "All done with the request."
    RADIO_NEED_SERVICE,            // "Do you need service depot work?"
    RADIO_ON_SERVICE,              // "Are you sitting on service depot?
    RADIO_COUNT
};

DEFINE_ENUMERATION_OPERATORS(RadioMessageType);

// in Fill_In_Data, TriggerTypeClass::Attaches_To() is used to check what a trigger
// attaches to, and it checks 4, 8 and 16.
enum AttachType
{
    ATTACH_NONE = 0,
    ATTACH_CELL = 1 << 0,
    ATTACH_OBJECT = 1 << 1,
    ATTACH_MAP = 1 << 2,
    ATTACH_HOUSE = 1 << 3,
    ATTACH_LOGIC = 1 << 4,
};

DEFINE_ENUMERATION_BITWISE_OPERATORS(AttachType);

enum NeedType
{
    NEED_NOTHING,
    NEED_THEME,
    NEED_MOVIE,             //see MovieType
    NEED_SOUND,
    NEED_SPEECH,
    NEED_INFANTRY,
    NEED_UNIT,
    NEED_AIRCRAFT,
    NEED_BUILDING,
    NEED_WAYPOINT,
    NEED_NUMBER,
    NEED_TRIGGER,
    NEED_TEAM,
    NEED_HOUSE,
    NEED_14,               // not used, only odd ones out seem Source, Crate, Armor, Animation
    NEED_QUARRY,           //see QuarryType
    NEED_FORMATION,        //see FormationName
    NEED_BOOL,
    NEED_SPECIAL,          //see SpecialWeaponFile
    NEED_MISSION,          //see MissionType
    NEED_CELL,
    NEED_COUNT,
    NEED_FIRST = 0,
};

DEFINE_ENUMERATION_OPERATORS(NeedType);

enum FormationType
{
    FORMATION_FIRST = 0,
    FORMATION_NONE = 0,
    FORMATION_TIGHT,
    FORMATION_LOOSE,
    FORMATION_WEDGE_NORTH,
    FORMATION_WEDGE_EAST,
    FORMATION_WEDGE_SOUTH,
    FORMATION_WEDGE_WEST,
    FORMATION_LINE_NS,
    FORMATION_LINE_EW,
    FORMATION_COUNT
};

DEFINE_ENUMERATION_OPERATORS(FormationType);

enum ZoneType
{
    ZONE_NONE = -1,
    ZONE_CORE,
    ZONE_NORTH,
    ZONE_EAST,
    ZONE_SOUTH,
    ZONE_WEST,
    ZONE_COUNT,
    ZONE_FIRST = 0,
};

DEFINE_ENUMERATION_OPERATORS(ZoneType);

enum MarkType
{
    MARK_NONE = -1,
    MARK_REMOVE,
    MARK_PUT,
    MARK_REDRAW,
    MARK_3,         // TODO: choose a name; this one adds content? could be refresh/update, see FootClass::Mark
    MARK_4,         // TODO: choose a name; MARK_OVERLAP_PUT?, ObjectClass::Mark calls Overlap_Down
    MARK_5,         // TODO: choose a name; MARK_OVERLAP_REMOVE?, ObjectClass::Mark calls Overlap_Up
    MARK_COUNT,
};

DEFINE_ENUMERATION_OPERATORS(MarkType);

enum MoveType
{
    MOVE_NONE = -1,
    MOVE_OK,
    MOVE_CLOAK,
    MOVE_MOVING_BLOCK,
    MOVE_DESTROYABLE,
    MOVE_TEMP,
    MOVE_NO,
    MOVE_COUNT,
};

DEFINE_ENUMERATION_OPERATORS(MoveType);

enum DamageResultType
{
    DAMAGE_NONE = 0, // No damage dealt.
    DAMAGE_LIGHT = 1, // Light damage dealt.
    DAMAGE_MEDIUM = 2, // Object left with half of health.
    DAMAGE_HEAVY = 3, // Object left with quarter of health.
    DAMAGE_DEAD = 4,
    DAMAGE_POST_MORTEM = 5, // Some kind of error/zombie state? Unused in RA
};

// "per cell process" types.
enum PCPType
{
    PCP_NONE = -1,
    PCP_0 = 0,
    PCP_1 = 1,
    PCP_2 = 2,
    PCP_COUNT = 3
};

DEFINE_ENUMERATION_OPERATORS(PCPType);

/*
enum _PipScaleType {
    PIP_AMMO         = 1
    PIP_TIBERIUM     = 2
    PIP_PASSENGERS   = 3
    PIP_POWER        = 4
    PIP_CHARGE       = 5
};
*/

enum PipEnum
{
    PIP_NONE = -1,
    PIP_EMPTY = 0,
    PIP_GREEN = 1,
    PIP_PRIMARY = 2,
    PIP_READY = 3,
    PIP_HOLD = 4,
    PIP_YELLOW = 5,
    PIP_WHITE = 6,
    PIP_RED = 7,
    PIP_NUM_0 = 8,
    PIP_NUM_1 = 9,
    PIP_NUM_2 = 10,
    PIP_NUM_3 = 11,
    PIP_NUM_4 = 12,
    PIP_NUM_5 = 13,
    PIP_NUM_6 = 14,
    PIP_NUM_7 = 15,
    PIP_NUM_8 = 16,
    PIP_NUM_9 = 17,
    PIP_FAKE = 18,
    PIP_SM_FAKE = 19,
    PIP_SMALL_F = 19,   // just for readablity
    PIP_MEDIC = 20,
    PIP_SM_PRIMARY = 21,
    PIP_COUNT = 22
};

// this will need moving soon
enum MenuNumberType
{
    MENU_0 = 0,
    MENU_1 = 1
};

enum SpecialDialogType
{
    SPECIAL_DLG_NONE = 0,
    SPECIAL_DLG_OPTIONS = 1,
    SPECIAL_DLG_SURRENDER = 2,
    SPECIAL_DLG_SPECIAL = 3,
};

enum INIFormatEnum
{
    INIFORMAT_0 = 0,
    INIFORMAT_1 = 1,
    INIFORMAT_2 = 2,
    INIFORMAT_3 = 3,
};

//TODO: To be moved to mission.h when class is implimented
enum MissionType
{
    MISSION_NONE = -1,
    MISSION_FIRST = 0,
    MISSION_SLEEP = 0,
    MISSION_ATTACK,
    MISSION_MOVE,
    MISSION_QMOVE,
    MISSION_RETREAT,
    MISSION_GUARD,
    MISSION_STICKY,
    MISSION_ENTER,
    MISSION_CAPTURE,
    MISSION_HARVEST,
    MISSION_AREA_GUARD,
    MISSION_RETURN,
    MISSION_STOP,
    MISSION_AMBUSH,
    MISSION_HUNT,
    MISSION_UNLOAD,
    MISSION_SABOTAGE,
    MISSION_CONSTRUCTION,
    MISSION_DECONSTRUCTION,
    MISSION_REPAIR,
    MISSION_RESCUE,
    MISSION_MISSILE,
    MISSION_HARMLESS,
    MISSION_COUNT,
};

DEFINE_ENUMERATION_OPERATORS(MissionType);

//TODO: To be moved to mission.h when class is implimented.
// this might be might worst nightmare come true, status per mission type... D:
// EDIT: im sure that each mission would share the same status type regardless of class, but tomsons is sure...
enum MissionStatusType
{
    STATUS_NONE = -1,

    STATUS_0 = 0, // defualt mission state?

    // MISSION_SLEEP

    // MISSION_ATTACK

    // MISSION_MOVE - Aircraft
    STATUS_MISSION_MOVE_VALIDATE_LZ = 0,
    STATUS_MISSION_MOVE_TAKE_OFF = 1,
    STATUS_MISSION_MOVE_FLY_TO_LZ = 2,
    STATUS_MISSION_MOVE_LAND = 3,

    // MISSION_QMOVE

    // MISSION_RETREAT
    STATUS_MISSION_RETREAT_0 = 0,
    STATUS_MISSION_RETREAT_1 = 1,

    // MISSION_GUARD

    // MISSION_STICKY

    // MISSION_ENTER - Aircraft
    STATUS_MISSION_ENTER_0 = 0,
    STATUS_MISSION_ENTER_1 = 1,
    STATUS_MISSION_ENTER_2 = 2, // Set when hind ordered to enter Helicarrier
    STATUS_MISSION_ENTER_3 = 3, // Set when yak/mig starting to land
    STATUS_MISSION_ENTER_4 = 4, // Set when yak/mig making landing pass going horizontally
    STATUS_MISSION_ENTER_5 = 5, // Set when yak/mig making landing pass going vertically
    STATUS_MISSION_ENTER_6 = 6, // About to touch down
    STATUS_MISSION_ENTER_7 = 7, // Touching down

    // MISSION_ENTER
    // STATUS_MISSION_ENTER_0 = 0,
    // STATUS_MISSION_ENTER_1 = 1,

    // MISSION_CAPTURE

    // MISSION_HARVEST - Unit
    STATUS_MISSION_HARVEST_0 = 0, // from tomsons testing, Moving to resources
    STATUS_MISSION_HARVEST_1 = 1, // from tomsons testing, Harvesting
    STATUS_MISSION_HARVEST_2 = 2, // from tomsons testing, Returing to Refinery
    STATUS_MISSION_HARVEST_3 = 3, // from tomsons testing, Radio'ing Refinery, then it switches to 2 and then to Enter Mission
    STATUS_MISSION_HARVEST_4 = 4, // from tomsons testing, ?????????

    // MISSION_AREA_GUARD

    // MISSION_RETURN

    // MISSION_STOP

    // MISSION_AMBUSH

    // MISSION_HUNT - Unit
    STATUS_MISSION_HUNT_0 = 0, // from tomsons testing, find clear spot and try to deploy?
    STATUS_MISSION_HUNT_1 = 1, // from tomsons testing, deployed sucessfully (code resets to 0 if Deploying is false)

    // MISSION_UNLOAD
    STATUS_MISSION_UNLOAD_0 = 0,
    STATUS_MISSION_UNLOAD_1 = 1,
    STATUS_MISSION_UNLOAD_2 = 2,
    STATUS_MISSION_UNLOAD_3 = 3,
    STATUS_MISSION_UNLOAD_4 = 4,

    // MISSION_SABOTAGE

    // MISSION_CONSTRUCTION - Building
    STATUS_MISSION_CONSTRUCTION_0 = 0,
    STATUS_MISSION_CONSTRUCTION_1 = 1,
    STATUS_MISSION_CONSTRUCTION_2 = 2,

    // MISSION_DECONSTRUCTION

    // MISSION_REPAIR
    STATUS_MISSION_REPAIR_0 = 0,
    STATUS_MISSION_REPAIR_1 = 1,

    // MISSION_RESCUE

    // MISSION_MISSILE

    // MISSION_HARMLESS

};

DEFINE_ENUMERATION_OPERATORS(MissionStatusType);

enum WeaponSlotType
{
    WEAPON_SLOT_NONE = -1,
    WEAPON_SLOT_PRIMARY,
    WEAPON_SLOT_SECONDARY,
    WEAPON_SLOT_COUNT,
};

//TODO: To be moved to housetype.h when class is implimented?
enum HousesType
{
    HOUSES_NONE = -1,
    HOUSES_FIRST = 0,
    HOUSES_SPAIN = 0,
    HOUSES_GREECE = 1,
    HOUSES_USSR = 2,
    HOUSES_ENGLAND = 3,
    HOUSES_UKRAINE = 4,        //HOUSES_ITALY = 4,        //from EDWIN
    HOUSES_GERMANY = 5,
    HOUSES_FRANCE = 6,
    HOUSES_TURKEY = 7,
    HOUSES_GOODGUY = 8,
    HOUSES_BADGUY = 9,
    HOUSES_NEUTRAL = 10,
    HOUSES_SPECIAL = 11,
    HOUSES_LAST = 11,

    HOUSES_MULTI_FIRST = 12,
    HOUSES_MULTI_1 = 12,
    HOUSES_MULTI_2 = 13,
    HOUSES_MULTI_3 = 14,
    HOUSES_MULTI_4 = 15,
    HOUSES_MULTI_5 = 16,
    HOUSES_MULTI_6 = 17,
    HOUSES_MULTI_7 = 18,
    HOUSES_MULTI_8 = 19,
    HOUSES_MULTI_LAST,
    HOUSES_MULTI_COUNT = 8,

    HOUSES_SP_LAST = 8,
    HOUSES_ALL_LAST = 19,
    HOUSES_PLAYABLE_COUNT = 10,
    HOUSES_COUNT = 20
};

DEFINE_ENUMERATION_OPERATORS(HousesType);

//TODO: To be moved to house.h when class is implimented.
enum OwnerType
{
    OWNER_NONE = 0,                             // No owner(s)
    OWNER_SPAIN = 1 << HOUSES_SPAIN,            //Allies
    OWNER_GREECE = 1 << HOUSES_GREECE,          //Allies
    OWNER_USSR = 1 << HOUSES_USSR,              //Soviet            //  OWNER_ITALY = 1 << HOUSES_ITALY,        //Allies
    OWNER_ENGLAND = 1 << HOUSES_ENGLAND,        //Allies
    OWNER_UKRAINE = 1 << HOUSES_UKRAINE,        //Soviet
    OWNER_GERMANY = 1 << HOUSES_GERMANY,        //Allies
    OWNER_FRANCE = 1 << HOUSES_FRANCE,          //Allies
    OWNER_TURKEY = 1 << HOUSES_TURKEY,          //Allies
    OWNER_GOODGUY = 1 << HOUSES_GOODGUY,        //Allies
    OWNER_BADGUY = 1 << HOUSES_BADGUY,          //Soviet
    OWNER_NEUTRAL = 1 << HOUSES_NEUTRAL,
    OWNER_SPECIAL = 1 << HOUSES_SPECIAL,
    OWNER_MULTI_1 = 1 << HOUSES_MULTI_1,
    OWNER_MULTI_2 = 1 << HOUSES_MULTI_2,
    OWNER_MULTI_3 = 1 << HOUSES_MULTI_3,
    OWNER_MULTI_4 = 1 << HOUSES_MULTI_4,
    OWNER_MULTI_5 = 1 << HOUSES_MULTI_5,
    OWNER_MULTI_6 = 1 << HOUSES_MULTI_6,
    OWNER_MULTI_7 = 1 << HOUSES_MULTI_7,
    OWNER_MULTI_8 = 1 << HOUSES_MULTI_8,
    OWNER_ALL = OWNER_SPAIN | OWNER_GREECE | OWNER_USSR | OWNER_ENGLAND | OWNER_UKRAINE | OWNER_GERMANY | OWNER_FRANCE | OWNER_TURKEY
              | OWNER_GOODGUY | OWNER_BADGUY | OWNER_NEUTRAL | OWNER_SPECIAL
              | OWNER_MULTI_1 | OWNER_MULTI_2 | OWNER_MULTI_3 | OWNER_MULTI_4 | OWNER_MULTI_5 | OWNER_MULTI_6 | OWNER_MULTI_7 | OWNER_MULTI_8
};

DEFINE_ENUMERATION_OPERATORS(OwnerType);

// Extention of OwnerType.
enum SideType
{
    SIDE_NONE = 0,
    SIDE_ALLIES = OWNER_SPAIN | OWNER_GREECE | OWNER_ENGLAND | OWNER_GERMANY | OWNER_FRANCE | OWNER_TURKEY | OWNER_GOODGUY,
    SIDE_SOVIET = OWNER_USSR | OWNER_UKRAINE | OWNER_BADGUY,
    SIDE_ALL = OWNER_SPAIN | OWNER_GREECE | OWNER_ENGLAND | OWNER_GERMANY | OWNER_FRANCE | OWNER_TURKEY | OWNER_GOODGUY
             | OWNER_USSR | OWNER_UKRAINE | OWNER_BADGUY
};

DEFINE_ENUMERATION_OPERATORS(SideType);

#ifdef COMPILER_WATCOM
enum MPHType
#else
enum MPHType : uint8_t
#endif
{
    MPH_MIN = 0,
    MPH_MAX = 255,
};

enum SpecialWeaponType {
    SPECIAL_SONAR_PULSE,
    SPECIAL_ATOM_BOMB,
    SPECIAL_WARP_SPHERE,
    SPECIAL_PARA_BOMB,
    SPECIAL_PARA_INFANTRY,
    SPECIAL_SPY_PLANE,
    SPECIAL_IRON_CURTAIN,
    SPECIAL_GPS,
    SPECIAL_COUNT,
    SPECIAL_WARP_TELEPORT = SPECIAL_COUNT, // Teleport isn't a real super, its a special case for DisplayClass PendingSuper.
    SPECIAL_NONE = -1,
    SPECIAL_FIRST = 0,
};

DEFINE_ENUMERATION_OPERATORS(SpecialWeaponType);

// If you want to add extra animations, you will also need to add a anim type
// class instance in animtype.cpp and add it to the Init_Heap function.
enum AnimType
{
    ANIM_NONE = -1,
    ANIM_FIRST = 0,
    ANIM_FBALL1 = 0, // "FBALL1"
    ANIM_FIREBALLFADE = 1, // "FB2
    ANIM_FRAG1 = 2, // "FRAG1"
    ANIM_VEH_HIT1 = 3, // "VEH-HIT1"
    ANIM_VEH_HIT2 = 4, // "VEH-HIT2"
    ANIM_VEH_HIT3 = 5, // "VEH-HIT3"
    ANIM_ART_EXP1 = 6, // "ART-EXP1"
    ANIM_NAPALM1 = 7, // "NAPALM1"
    ANIM_NAPALM2 = 8, // "NAPALM2"
    ANIM_NAPALM3 = 9, // "NAPALM3"
    ANIM_SMOKE_PUFF = 10, // "SMOKEY"
    ANIM_PIFF = 11, // "PIFF"            //small arms fire piff (single shot)
    ANIM_PIFFPIFF = 12, // "PIFFPIFF"        //small arms fire piff (multiple shots)
    ANIM_FIRE3 = 13, // "FIRE3"            //small flames
    ANIM_FIRE2 = 14, // "FIRE2"
    ANIM_FIRE1 = 15, // "FIRE1"
    ANIM_FIRE4 = 16, // "FIRE4"
    ANIM_MUZZLE_FLASH = 17, // "GUNFIRE"
    ANIM_SMOKE_M = 18, // "SMOKE_M"
    ANIM_BURN_SMALL = 19, // "BURN-S"
    ANIM_BURN_MED = 20, // "BURN-M"
    ANIM_BURN_BIG = 21, // "BURN-L"
    ANIM_ON_FIRE_SMALL = 22, // "BURN-S"
    ANIM_ON_FIRE_MED = 23, // "BURN-M"
    ANIM_ON_FIRE_BIG = 24, // "BURN-L"
    ANIM_SAM_N = 25, // "SAMFIRE"
    ANIM_SAM_NE = 26, // "SAMFIRE"
    ANIM_SAM_E = 27, // "SAMFIRE"
    ANIM_SAM_SE = 28, // "SAMFIRE"
    ANIM_SAM_S = 29, // "SAMFIRE"
    ANIM_SAM_SW = 30, // "SAMFIRE"
    ANIM_SAM_W = 31, // "SAMFIRE"
    ANIM_SAM_NW = 32, // "SAMFIRE"
    ANIM_GUN_N = 33, // "MINIGUN"
    ANIM_GUN_NE = 34, // "MINIGUN"
    ANIM_GUN_E = 35, // "MINIGUN"
    ANIM_GUN_SE = 36, // "MINIGUN"
    ANIM_GUN_S = 37, // "MINIGUN"
    ANIM_GUN_SW = 38, // "MINIGUN"
    ANIM_GUN_W = 39, // "MINIGUN"
    ANIM_GUN_NW = 40, // "MINIGUN"
    ANIM_LZ_SMOKE = 41, // "SMOKLAND"
    ANIM_CRATE_DEVIATOR = 42, // "DEVIATOR"
    ANIM_CRATE_DOLLAR = 43, // "DOLLAR"            //Money bonus
    ANIM_CRATE_EARTH = 44, // "EARTH"
    ANIM_CRATE_EMPULSE = 45, // "EMPULSE"
    ANIM_CRATE_INVUN = 46, // "INVUN"
    ANIM_CRATE_MINE = 47, // "MINE"
    ANIM_CRATE_RAPID = 48, // "RAPID"
    ANIM_CRATE_STEALTH = 49, // "STEALTH2"
    ANIM_CRATE_MISSILE = 50, // "MISSILE2"
    ANIM_MOVEFLASH = 51, // "MOVEFLSH"
    ANIM_OILFIELD_BURN = 52, // "FLMSPT"
    ANIM_ELECTRO = 53, // "ELECTRO"
    ANIM_PARACHUTE = 54, // "PARACH"
    ANIM_ELECTDOG = 55, // "ELECTDOG"
    ANIM_CORPSE1 = 56, // "CORPSE1"
    ANIM_CORPSE2 = 57, // "CORPSE2"
    ANIM_CORPSE3 = 58, // "CORPSE3"
    ANIM_SPUTDOOR = 59, // "SPUTDOOR"
    ANIM_ATOMSFX = 60, // "ATOMSFX"
    ANIM_CHRONOBOX = 61, // "CHRONBOX"
    ANIM_GPSBOX = 62, // "GPSBOX"
    ANIM_INVULBOX = 63, // "INVULBOX"
    ANIM_PARABOX = 64, // "PARABOX"
    ANIM_SONARBOX = 65, // "SONARBOX"
    ANIM_TWINKLE1 = 66, // "TWINKLE1"            //ore twinkle animation
    ANIM_TWINKLE2 = 67, // "TWINKLE2"            //ore twinkle animation
    ANIM_TWINKLE3 = 68, // "TWINKLE3"            //ore twinkle animation
    ANIM_FLAK = 69, // "FLAK"
    ANIM_WATER_EXP1 = 70, // "H2O_EXP1"
    ANIM_WATER_EXP2 = 71, // "H2O_EXP2"
    ANIM_WATER_EXP3 = 72, // "H2O_EXP3"
    ANIM_CRATE_ARMOR = 73, // "ARMOR"            //Armor bonus
    ANIM_CRATE_SPEED = 74, // "SPEED"
    ANIM_CRATE_FPOWER = 75, // "FPOWER"            //Firepower bonus crate animation
    ANIM_CRATE_TQUAKE = 76, // "TQUAKE"
    ANIM_PARABOMB = 77, // "PARABOMB"
    ANIM_MINEEXP1 = 78, // "VEH-HIT2"
    ANIM_ANTDEATH = 79, // "ANTDIE"
    // ANIM_WWCRATE = 80, // "WWCRATE" - Chronoshift
    ANIM_COUNT,
};

DEFINE_ENUMERATION_OPERATORS(AnimType);

#endif // GAMETYPES_H
