/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Enumerations and other constant defintions that don't have a better home.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef CCTYPE_H
#define CCTYPE_H

#include "always.h"

#define DEF_TICKS_PER_SECOND 15
#define DEF_TICKS_PER_MINUTE (DEF_TICKS_PER_SECOND * 60)
#define TICKS_HALF_MINUTE (DEF_TICKS_PER_MINUTE / 2)

typedef uint32_t coord_t;
typedef int16_t cell_t;
typedef int32_t target_t;
typedef uint16_t lepton_t;

inline int Cells_To_Pixels(uint32_t cells)
{
    return (cells * 24);
}

inline uint32_t Pixels_To_Cells(int pixels)
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

// this is temp (made up name) until we work out what it really is
enum TeamNumberType
{
    TEAM_NUMBER_NONE = -1,

    TEAM_NUMBER_FIRST = 0,

    TEAM_NUMBER_0 = 0,
    TEAM_NUMBER_1 = 1,
    TEAM_NUMBER_2 = 2,
    TEAM_NUMBER_3 = 3,
    TEAM_NUMBER_4 = 4,
    TEAM_NUMBER_5 = 5,
    TEAM_NUMBER_6 = 6,
    TEAM_NUMBER_7 = 7,
    TEAM_NUMBER_8 = 8,
    TEAM_NUMBER_9 = 9,

    TEAM_NUMBER_LAST = 9,

    TEAM_NUMBER_COUNT = 10
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

// threat type is in fact linked to Quarry, these all do not seems right...
enum ThreatType
{
    THREAT_NONE = -1,

    THREAT_ANY = 0x0,
    THREAT_1 = 0x1,
    THREAT_2 = 0x2,
    THREAT_AIRCRAFT = 0x4, // AIR / AIRCRAFT?
    THREAT_INFANTRY = 0x8,
    THREAT_VEHICLE = 0x10,
    THREAT_BUILDING = 0x20,
    THREAT_HARVESTERS = 0x40, // Economy?
    THREAT_VESSEL = 0x80,
    THREAT_NEUTRAL = 0x100, // from RA1
    THREAT_CAPTURE = 0x200,
    THREAT_FAKES = 0x400, // from RA1
    THREAT_POWER_FACILTIES = 0x800,
    THREAT_FACTORIES = 0x1000,
    THREAT_BASE_DEFENSES = 0x2000,

    // combined
    THREAT_3D60 = (THREAT_BUILDING | THREAT_HARVESTERS | THREAT_NEUTRAL | THREAT_FAKES | THREAT_POWER_FACILTIES
        | THREAT_FACTORIES | THREAT_BASE_DEFENSES) // 0x3D60            //
};

DEFINE_ENUMERATION_BITWISE_OPERATORS(ThreatType);

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

    RADIO_STATIC = 0,               // "static (no message)"
    RADIO_ROGER = 1,                // "Roger."
    RADIO_HELLO = 2,                // "Come in."
    RADIO_OVER_AND_OUT = 3,         // "Over and out."
    RADIO_REQ_TRANSPORT = 4,        // "Requesting transport."
    RADIO_ATTACH_TO_TRANSPORT = 5,  // "Attach to transport."
    RADIO_GOT_DELIVERY = 6,         // "I've got a delivery for you."
    RADIO_7 = 7,                    // "I'm performing load/unload maneuver. Be careful.""
    RADIO_CLEAR = 8,                // "I'm clear."
    RADIO_9 = 9,                    // "You are clear to unload. Driving away now.
    RADIO_UNABLE_TO_COMPLY = 10,    // "Am unable to comply."
    RADIO_11 = 11,                  // "I'm starting construction now... act busy."
    RADIO_12 = 12,                  // "I've finished construction. You are free."
    RADIO_13 = 13,                  // "We bumped, redraw yourself please."
    RADIO_14 = 14,                  // "I'm trying to load up now."
    RADIO_REQ_PASSENGER = 15,       // "May I become a passenger?"
    RADIO_16 = 16,                  // "Are you ready to receive shipment?"
    RADIO_17 = 17,                  // "Are you trying to become a passenger?""
    RADIO_MOVE_TO_LOCATION = 18,    // "Move to location X."
    RADIO_NEED_TO_MOVE = 19,        // "Do you need to move?"
    RADIO_20 = 20,                  // "All right already. Now what?"
    RADIO_21 = 21,                  // "I'm a passenger now."
    RADIO_22 = 22,                  // "Backup into refinery now."
    RADIO_RUN_AWAY = 23,            // "Run away!"
    RADIO_TETHER_ESTABLISHED = 24,  // "Tether established."
    RADIO_TETHER_BROKEN = 25,       // "Tether broken."
    RADIO_REPAIR_ONE_STEP = 26,     // "Repair one step."
    RADIO_27 = 27,                  // "Are you prepared to fight?"
    RADIO_28 = 28,                  // "Attack this target please."
    RADIO_29 = 29,                  // "Reload one step."
    RADIO_30 = 30,                  // "Circumstances prevent success."
    RADIO_31 = 31,                  // "All done with the request."
    RADIO_32 = 32,                  // "Do you need service depot work?"
    RADIO_33 = 33,                  // "Are you sitting on service depot?

    RADIO_LAST = 34,

    RADIO_COUNT = 34
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

enum NeedType
{
    NEED_NONE = -1,

    NEED_FIRST = 0,

    NEED_0 = 0,
    NEED_1 = 1,
    NEED_2 = 2,
    NEED_3 = 3,
    NEED_4 = 4,
    NEED_5 = 5,
    NEED_6 = 6,
    NEED_7 = 7,
    NEED_8 = 8,
    NEED_9 = 9,
    NEED_10 = 10,
    NEED_11 = 11,
    NEED_12 = 12, // team?
    NEED_13 = 13,
    NEED_14 = 14,
    NEED_15 = 15,
    NEED_16 = 16,
    NEED_17 = 17,
    NEED_18 = 18,
    NEED_19 = 19,
    NEED_20 = 20,

    NEED_LAST = 999,

    NEED_COUNT = 999
};

enum ZoneType
{
    ZONE_NONE = -1,
    ZONE_CORE,
    ZONE_NORTH,
    ZONE_EAST,
    ZONE_SOUTH,
    ZONE_WEST,
    ZONE_COUNT,
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
    MISSION_ATTACK = 1,
    MISSION_MOVE = 2,
    MISSION_QMOVE = 3,
    MISSION_RETREAT = 4,
    MISSION_GUARD = 5,
    MISSION_STICKY = 6,
    MISSION_ENTER = 7,
    MISSION_CAPTURE = 8,
    MISSION_HARVEST = 9,
    MISSION_AREA_GUARD = 10,
    MISSION_RETURN = 11,
    MISSION_STOP = 12,
    MISSION_AMBUSH = 13,
    MISSION_HUNT = 14,
    MISSION_UNLOAD = 15,
    MISSION_SABOTAGE = 16,
    MISSION_CONSTRUCTION = 17,
    MISSION_DECONSTRUCTION = 18,
    MISSION_REPAIR = 19,
    MISSION_RESCUE = 20,
    MISSION_MISSILE = 21,
    MISSION_HARMLESS = 22,
    MISSION_FIND_CRATE = 23,
    MISSION_TIMED_HUNT = 24,

    MISSION_LAST = 24,

    MISSION_COUNT = 25
};

//TODO: To be moved to aircrafttype.h when class is implimented.
enum AircraftType {
    AIRCRAFT_NONE = -1,

    AIRCRAFT_FIRST = 0,

    AIRCRAFT_TRANSPORT = 0,
    AIRCRAFT_BADGER = 1,
    AIRCRAFT_U2 = 2,
    AIRCRAFT_MIG = 3,
    AIRCRAFT_YAK = 4,
    AIRCRAFT_HELI = 5,
    AIRCRAFT_HIND = 6,

    AIRCRAFT_LAST = 6,

    AIRCRAFT_COUNT = 7
};

//TODO: To be moved to vesseltype.h when class is implimented.
enum VesselType {
    VESSEL_NONE = -1,

    VESSEL_FIRST = 0,

    VESSEL_SUBMARINE = 0,
    VESSEL_DESTROYER = 1,
    VESSEL_CRUISER = 2,
    VESSEL_TRANSPORT = 3,
    VESSEL_PT_BOAT = 4,
    VESSEL_MISSILE_SUB = 5,
    VESSEL_CARRIER = 6,

    VESSEL_LAST = 6,

    VESSEL_COUNT = 7
};

//TODO: To be moved to buildingtype.h when class is implimented.
enum BuildingType {
    BUILDING_NONE = -1,

    BUILDING_FIRST = 0,

    // The follow right hand comments are the bit index (uint32) for storing
    // which buildings are set, ie, HouseClas storing the buildings the house
    // has constructed, or CCINIClass writing an entry.
    BUILDING_ATEK = 0,				// 1 - 1
    BUILDING_IRON_CURTAIN = 1,		// 1 - 2
    BUILDING_WEAP = 2,				// 1 - 4
    BUILDING_PDOX = 3,				// 1 - 8
    BUILDING_PBOX = 4,				// 1 - 16
    BUILDING_HBOX = 5,				// 1 - 32
    BUILDING_DOME = 6,				// 1 - 64
    BUILDING_GAP_GENERATOR = 7,		// 1 - 128
    BUILDING_GUN = 8,				// 2 - 1
    BUILDING_AGUN = 9,				// 2 - 2
    BUILDING_FTUR = 10,				// 2 - 4
    BUILDING_FACT = 11,				// 2 - 8
    BUILDING_PROC = 12,				// 2 - 16
    BUILDING_SILO = 13,				// 2 - 32
    BUILDING_HELIPAD = 14,		    // 2 - 64
    BUILDING_SAM = 15,				// 2 - 128
    BUILDING_AIRFIELD = 16,			// 3 - 1
    BUILDING_POWR = 17,				// 3 - 2
    BUILDING_APWR = 18,				// 3 - 4
    BUILDING_STEK = 19,				// 3 - 8
    BUILDING_HOSPITAL = 20,			// 3 - 16
    BUILDING_BARR = 21,				// 3 - 32
    BUILDING_TENT = 22,				// 3 - 64
    BUILDING_KENN = 23,				// 3 - 128
    BUILDING_FIX = 24,				// 4 - 1
    BUILDING_BIO = 25,				// 4 - 2
    BUILDING_MISS = 26,				// 4 - 4
    BUILDING_SYRD = 27,				// 4 - 8
    BUILDING_SPEN = 28,				// 4 - 16
    BUILDING_MSLO = 29,				// 4 - 32
    BUILDING_FCOM = 30,				// 4 - 64
    BUILDING_TESLA_COIL = 31,		// 4 - 128

    // Fakes
    BUILDING_WEAF = 32,
    BUILDING_FACF = 33,
    BUILDING_SYRF = 34,
    BUILDING_SPEF = 35,
    BUILDING_DOMF = 36,

    // Walls
    BUILDING_SBAG = 37,
    BUILDING_CYCL = 38,
    BUILDING_BRIK = 39,
    BUILDING_BARB = 40,
    BUILDING_WOOD = 41,
    BUILDING_FENC = 42,

    // Mines
    BUILDING_MINV = 43,
    BUILDING_MINP = 44,

    // 
    BUILDING_V01 = 45,
    BUILDING_V02 = 46,
    BUILDING_V03 = 47,
    BUILDING_V04 = 48,
    BUILDING_V05 = 49,
    BUILDING_V06 = 50,
    BUILDING_V07 = 51,
    BUILDING_V08 = 52,
    BUILDING_V09 = 53,
    BUILDING_V10 = 54,
    BUILDING_V11 = 55,
    BUILDING_V12 = 56,
    BUILDING_V13 = 57,
    BUILDING_V14 = 58,
    BUILDING_V15 = 59,
    BUILDING_V16 = 60,
    BUILDING_V17 = 61,
    BUILDING_V18 = 62,
    BUILDING_V19 = 63,
    BUILDING_V20 = 64,
    BUILDING_V21 = 65,
    BUILDING_V22 = 66,
    BUILDING_V23 = 67,
    BUILDING_V24 = 68,
    BUILDING_V25 = 69,
    BUILDING_V26 = 70,
    BUILDING_V27 = 71,
    BUILDING_V28 = 72,
    BUILDING_V29 = 73,
    BUILDING_V30 = 74,
    BUILDING_V31 = 75,
    BUILDING_V32 = 76,
    BUILDING_V33 = 77,
    BUILDING_V34 = 78,
    BUILDING_V35 = 79,
    BUILDING_V36 = 80,
    BUILDING_V37 = 81,

    // Barrels
    BUILDING_BARREL = 82,
    BUILDING_BRL3 = 83,

    // Ants
    BUILDING_QUEE = 84,
    BUILDING_LAR1 = 85,
    BUILDING_LAR2 = 86,

    BUILDING_LAST = 86,

    BUILDING_COUNT = 87
};

//TODO: To be moved to infantrytype.h when class is implimented.
enum InfantryType {
    INFANTRY_NONE				= -1,		// Invalid InfantryType

    INFANTRY_FIRST				= 0,

    INFANTRY_E1					= 0,		// Rifle Infantry
    INFANTRY_E2					= 1,		// Grenadier
    INFANTRY_E3					= 2,		// Rocket Soldier
    INFANTRY_FLAME_TROOPER		= 3,		// Flamethrower
    INFANTRY_ENGINEER			= 4,		// Engineer
    INFANTRY_TANYA			    = 5,		// Tanya
    INFANTRY_SPY				= 6,		// Spy
    INFANTRY_THIEF				= 7,		// Theif
    INFANTRY_MEDIC				= 8,		// Medic
    INFANTRY_GENERAL			= 9,		// General
    INFANTRY_DOG				= 10,		// Attack Dog
    INFANTRY_C1					= 11,		//
    INFANTRY_C2					= 12,		//
    INFANTRY_C3					= 13,		//
    INFANTRY_C4					= 14,		//
    INFANTRY_C5					= 15,		//
    INFANTRY_C6					= 16,		//
    INFANTRY_C7					= 17,		//
    INFANTRY_C8					= 18,		//
    INFANTRY_C9					= 19,		//
    INFANTRY_C10				= 20,		//
    INFANTRY_EINSTEIN			= 21,		// Einstein
    INFANTRY_DELPHI				= 22,		// Delphi
    INFANTRY_CHAN				= 23,		//
    INFANTRY_SHOCK_TROOPER		= 24,		// Shock Trooper
    INFANTRY_MECHANIC			= 25,		// Mechanic

    INFANTRY_LAST				= 25,		//

    INFANTRY_COUNT				= 26		//
};

//TODO: To be moved to infantry.h when class is implimented.
enum DoType {
    DO_NONE = -1,

    DO_FIRST = 0,

    DO_READY = 0,
    DO_GUARD = 1,
    DO_PRONE = 2,
    DO_WALK = 3,
    DO_FIRE_UP = 4,
    DO_DOWN = 5,
    DO_CRAWL = 6,
    DO_UP = 7,
    DO_FIRE_PRONE = 8,

    DO_IDLE1 = 9,
    DO_IDLE2 = 10,

    DO_DIE1 = 11,
    DO_DIE2 = 12,
    DO_DIE3 = 13,
    DO_DIE4 = 14,
    DO_DIE5 = 15,

    DO_16 = 16,
    DO_17 = 17,
    DO_18 = 18,
    DO_19 = 19,
    DO_20 = 20,

    DO_LAST = 20,

    DO_COUNT = 21
};


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

    // MISSION_FIND_CRATE

    // MISSION_TIMED_HUNT

};

DEFINE_ENUMERATION_OPERATORS(MissionType);
DEFINE_ENUMERATION_OPERATORS(MissionStatusType);

enum WeaponSlotType
{
    WEAPON_SLOT_NONE = -1,
    WEAPON_SLOT_PRIMARY,
    WEAPON_SLOT_SECONDARY,
    WEAPON_SLOT_COUNT,
};

//TODO: To be moved to housetype.h when class is implimented.
enum HousesType
{
    HOUSES_NONE = -1,

    HOUSES_FIRST = 0,

    HOUSES_SPN = 0,         //"Spain"
    HOUSES_GRE = 1,         //"Greece"
    HOUSES_RED = 2,         //"USSR"
    HOUSES_ENG = 3,         //"England"
    HOUSES_UKA = 4,         //"Ukraine"		    //EDWIN shows entry 4 as "Italy"       // HOUSES_ITA = 4,
    HOUSES_GER = 5,         //"Germany"
    HOUSES_FRA = 6,         //"France"
    HOUSES_TRK = 7,         //"Turkey"

    HOUSES_GDI = 8,         //"GoodGuy"
    HOUSES_NOD = 9,         //"BadGuy"

    HOUSES_CIV = 10,        //"Neutral"
    HOUSES_JP = 11,         //"Special"

    HOUSES_LAST = 11,

    HOUSES_MULTI_FIRST = 12,

    HOUSES_MP1 = 12,        //"Multi1"
    HOUSES_MP2 = 13,        //"Multi2"
    HOUSES_MP3 = 14,        //"Multi3"
    HOUSES_MP4 = 15,        //"Multi4"
    HOUSES_MP5 = 16,        //"Multi5"
    HOUSES_MP6 = 17,        //"Multi6"
    HOUSES_MP7 = 18,        //"Multi7"
    HOUSES_MP8 = 19,        //"Multi8"

    HOUSES_MPLAST = 19,

    HOUSES_ALL_LAST = 19,

    HOUSES_MPCOUNT = 8,

    HOUSES_COUNT = 20
};

DEFINE_ENUMERATION_OPERATORS(HousesType);

//TODO: To be moved to house.h when class is implimented.
enum OwnerType
{
    OWNER_NONE = 0, // No owner

    OWNER_SPAIN = 1 << HOUSES_SPN,      //"Spain"         //Allies
    OWNER_GREECE = 1 << HOUSES_GRE,     //"Greece"        //Allies
    OWNER_USSR = 1 << HOUSES_RED,       //"USSR"          //Soviet            //  OWNER_ITALY    = 4,        //"Italy"        //Allies
    OWNER_ENGLAND = 1 << HOUSES_ENG,    //"England"       //Allies
    OWNER_UKRAINE = 1 << HOUSES_UKA,    //"Ukraine"       //Soviet
    OWNER_GERMANY = 1 << HOUSES_GER,    //"Germany"       //Allies
    OWNER_FRANCE = 1 << HOUSES_FRA,     //"France"        //Allies
    OWNER_TURKEY = 1 << HOUSES_TRK,     //"Turkey"        //Allies

    OWNER_GOODGUY = 1 << HOUSES_GDI,    //"GoodGuy"       //Allies
    OWNER_BADGUY = 1 << HOUSES_NOD,     //"BadGuy"        //Soviet

    OWNER_NEUTRAL = 1 << HOUSES_CIV,    //"Neutral"
    OWNER_SPECIAL = 1 << HOUSES_JP,     //"Special"

    OWNER_MP1 = 1 << HOUSES_MP1,    //"Multi1"
    OWNER_MP2 = 1 << HOUSES_MP2,    //"Multi2"
    OWNER_MP3 = 1 << HOUSES_MP3,    //"Multi3"
    OWNER_MP4 = 1 << HOUSES_MP4,    //"Multi4"
    OWNER_MP5 = 1 << HOUSES_MP5,    //"Multi5"
    OWNER_MP6 = 1 << HOUSES_MP6,    //"Multi6"
    OWNER_MP7 = 1 << HOUSES_MP7,    //"Multi7"
    OWNER_MP8 = 1 << HOUSES_MP8,    //"Multi8"

    OWNER_ALL = OWNER_SPAIN | OWNER_GREECE | OWNER_USSR | OWNER_ENGLAND | OWNER_UKRAINE | OWNER_GERMANY | OWNER_FRANCE | OWNER_TURKEY
            | OWNER_GOODGUY | OWNER_BADGUY | OWNER_NEUTRAL | OWNER_SPECIAL
            | OWNER_MP1 | OWNER_MP2 | OWNER_MP3 | OWNER_MP4 | OWNER_MP5 | OWNER_MP6 | OWNER_MP7 | OWNER_MP8
};

DEFINE_ENUMERATION_OPERATORS(OwnerType);

enum SideType
{
=======
    OWNER_NONE = 0,         // No house

    OWNER_SPAIN = 1 << HOUSES_SPN,          //"Spain"		//Allies
    OWNER_GREECE = 1 << HOUSES_GRE,         //"Greece"		//Allies
    OWNER_USSR = 1 << HOUSES_RED,           //"USSR"		//Soviet            //  OWNER_ITALY	= 4,		//"Italy"		//Allies
    OWNER_ENGLAND = 1 << HOUSES_ENG,        //"England"		//Allies
    OWNER_UKRAINE = 1 << HOUSES_UKA,        //"Ukraine"		//Soviet
    OWNER_GERMANY = 1 << HOUSES_GER,        //"Germany"		//Allies
    OWNER_FRANCE = 1 << HOUSES_FRA,         //"France"		//Allies
    OWNER_TURKEY = 1 << HOUSES_TRK,         //"Turkey"		//Allies
    OWNER_GOODGUY = 1 << HOUSES_GDI,        //"GoodGuy"		//Allies
    OWNER_BADGUY = 1 << HOUSES_NOD,         //"BadGuy"		//Soviet
    OWNER_NEUTRAL = 1 << HOUSES_CIV,        //"Neutral"
    OWNER_SPECIAL = 1 << HOUSES_JP,         //"Special"

    OWNER_MP1 = 1 << HOUSES_MP1,        //"Multi1"
    OWNER_MP2 = 1 << HOUSES_MP2,        //"Multi2"
    OWNER_MP3 = 1 << HOUSES_MP3,        //"Multi3"
    OWNER_MP4 = 1 << HOUSES_MP4,        //"Multi4"
    OWNER_MP5 = 1 << HOUSES_MP5,        //"Multi5"
    OWNER_MP6 = 1 << HOUSES_MP6,        //"Multi6"
    OWNER_MP7 = 1 << HOUSES_MP7,        //"Multi7"
    OWNER_MP8 = 1 << HOUSES_MP8,        //"Multi8"

>>>>>>> Added required enums (AircrafType, VesselType etc) for additional CCINIClass functions.
    SIDE_ALLIES = OWNER_SPAIN | OWNER_GREECE | OWNER_ENGLAND | OWNER_GERMANY | OWNER_FRANCE | OWNER_TURKEY | OWNER_GOODGUY,
    SIDE_SOVIET = OWNER_USSR | OWNER_UKRAINE | OWNER_BADGUY,

    SIDE_ALL = OWNER_SPAIN | OWNER_GREECE | OWNER_ENGLAND | OWNER_GERMANY | OWNER_FRANCE | OWNER_TURKEY | OWNER_GOODGUY
            | OWNER_USSR | OWNER_UKRAINE | OWNER_BADGUY
};

DEFINE_ENUMERATION_OPERATORS(SideType);

//TODO: To be moved to movie.h when class is implimented.
enum MovieType
{
    MOVIE_NONE = -1,
    MOVIE_FIRST = 0,
    MOVIE_PROLOG = 75,
    MOVIE_SIZZLE = 97,
    MOVIE_SIZZLE2 = 98,
    MOVIE_COUNT = 104
};

DEFINE_ENUMERATION_OPERATORS(MovieType);

#ifdef COMPILER_WATCOM
enum MPHType
#else
enum MPHType : uint8_t
#endif
{
    MPH_MIN = 0,
    MPH_MAX = 255,
};

#endif // CCTYPE_H
