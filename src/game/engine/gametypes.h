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

// Controls ID's
#define BUTTON_NONE -1

// Standard Button ID's
#define BUTTON_OK 1
#define BUTTON_CANCEL 2
#define BUTTON_YES 3
#define BUTTON_NO 4

// Ingame GUI Gadget ID's
#define BUTTON_REPAIR 101
//#define BUTTON_
#define BUTTON_SELL 103
//#define BUTTON_
#define BUTTON_ZOOM 105
//#define BUTTON_
//#define BUTTON_
//#define BUTTON_
#define BUTTON_ED_CLEAR 109
#define BUTTON_ED_ORE 110
#define BUTTON_ED_GEMS 111
#define BUTTON_ED_WATER 112
#define BUTTON_ED_SHORE 103
#define BUTTON_ED_RIVER 104
#define BUTTON_ED_ROAD 105
#define BUTTON_ED_RIDGES 106
#define BUTTON_ED_TREES 107
#define BUTTON_ED_DEBRIS 108
#define BUTTON_ED_PREV_ITEM 113
#define BUTTON_ED_NEXT_ITEM 114
#define BUTTON_ED_MX 115
#define BUTTON_ED_START_FLAG 116
#define CONTROL_ED_PREVIEW 117
//#define BUTTON_
//#define BUTTON_
//#define BUTTON_
//#define BUTTON_
//#define BUTTON_
//#define BUTTON_
//#define BUTTON_
//#define BUTTON_

#define CONTROL_SB_SELECT_BUTTON 0
#define CONTROL_TILE_BROWSER 150

// Sidebar ID's
#define GADGET_STRIP_COLUMN_LEFT 200
#define GADGET_STRIP_COLUMN_RIGHT 210

//#define BUTTON_
//#define BUTTON_
//#define BUTTON_
//#define BUTTON_
//#define BUTTON_
//#define BUTTON_

// Editor Viewport Control ID
#define CONTROL_EDITOR_VIEWPORT_INPUT 515

// Menu Button IDs
#define BUTTON_CS 1000
#define BUTTON_AM 1001
#define BUTTON_NEW_GAME 1002
#define BUTTON_LOAD_GAME 1003
#define BUTTON_MP_GAME 1004
#define BUTTON_SNEAK_PEAK 1005
#define BUTTON_MOVIES 1006
#define BUTTON_OPTIONS 1007
#define BUTTON_EXIT 1008

#define BUTTON__ 1010

// Mutliplayer Button IDs
#define BUTTON_MODEM_SERIAL 1020
#define BUTTON_SKIRMISH 1021
#define BUTTON_NETWORK 1022
#define BUTTON_INTERNET 1023

// Fetch Difficulty IDs
#define SLIDER_DIFFICULTY 1030

// Right click context menu id
#define CONTROL_CONTEXT_MENU 8000


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

enum QuarryType
{
    QUARRY_NONE = -1,
    QUARRY_FIRST = 0,
    QUARRY_NA = 0,
    QUARRY_ANYTHING,
    QUARRY_ANY_BUILDING,
    QUARRY_HARVESTERS,
    QUARRY_INFANTRY,
    QUARRY_ANY_VEHICLES,
    QUARRY_ANY_VESSEL,
    QUARRY_FACTORIES,
    QUARRY_BASE_DEFENSES,
    QUARRY_BASE_THREATS,
    QUARRY_POWER_FACILITIES,
    QUARRY_FAKE_BUILDINGS,
    QUARRY_COUNT
};

DEFINE_ENUMERATION_BITWISE_OPERATORS(QuarryType);

// threat type is in fact linked to Quarry, these all do not seems right...
enum ThreatType
{
    THREAT_NONE = -1,
    THREAT_ANY = 0x0,
    THREAT_1 = 0x1,
    THREAT_2 = 0x2,
    THREAT_AIRCRAFT = 0x4,          // AIR / AIRCRAFT?
    THREAT_INFANTRY = 0x8,
    THREAT_VEHICLE = 0x10,
    THREAT_BUILDING = 0x20,
    THREAT_HARVESTERS = 0x40,       // Economy?
    THREAT_VESSEL = 0x80,
    THREAT_NEUTRAL = 0x100,
    THREAT_CAPTURE = 0x200,
    THREAT_FAKES = 0x400,
    THREAT_POWER_FACILTIES = 0x800,
    THREAT_FACTORIES = 0x1000,
    THREAT_BASE_DEFENSES = 0x2000,
    THREAT_3D60 = (THREAT_BUILDING | THREAT_HARVESTERS | THREAT_NEUTRAL | THREAT_FAKES | THREAT_POWER_FACILTIES
        | THREAT_FACTORIES | THREAT_BASE_DEFENSES)  // 0x3D60
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
    ATTACH_1 = 1, // 1 ??
    ATTACH_2 = 2, // 2 ??
    ATTACH_4 = 4, // 4 seems to be attatch to MapTriggers
    ATTACH_8 = 8, // 8 seems to be attatch to HouseTriggers
    ATTACH_16 = 16 // 16 seems to be attatch to LogicTriggers
};

DEFINE_ENUMERATION_BITWISE_OPERATORS(AttachType);

//TODO: Will need looking into to really confirm, but credit to tomsons for the list.
enum NeedType
{
    NEED_NONE = 0,
    NEED_FIRST = 0,
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
    MARK_3,         // TODO: choose a name; this one adds content?
    MARK_4,         // TODO: choose a name; MARK_OVERLAP_PUT?
    MARK_5,         // TODO: choose a name; MARK_OVERLAP_REMOVE?
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

//TODO: consider renaming enum.
// these definately needs renaming.
enum DamageResultType
{
    DAMAGE_NONE = -1,
    DAMAGE_UNAFFECTED = 0,
    DAMAGE_UNDAMAGED = 1,
    DAMAGE_YELLOW = 2,
    DAMAGE_RED = 3,
    DAMAGE_DEAD = 4,
    DAMAGE_POST_MORTEM = 5,
    DAMAGE_COUNT = 6
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
    HOUSES_MULTI_LAST = 19,
    HOUSES_MULTI_COUNT = 8,

    HOUSES_ALL_LAST = 19,

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
