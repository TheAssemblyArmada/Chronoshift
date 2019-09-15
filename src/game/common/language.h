/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Functions for handling string tables and name overrides.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "always.h"

#define NAME_OVERRIDE_MAX 25

#ifdef GAME_DLL
extern char **NameOverride;
extern int *NameIDOverride;
#else
extern char *NameOverride[NAME_OVERRIDE_MAX];
extern int NameIDOverride[NAME_OVERRIDE_MAX];
#endif

// clang-format off

// the following variables defines the text used throughout the game.
enum TextEnum
{
    TXT_NULL = 0, // Empty string.
    TXT_CREDIT_FORMAT = 1, // %3d.%02d
    TXT_TIME_HMS_FORMAT = 2, // Time:%02d:%02d:%02d
    TXT_TIME_MS_FORMAT = 3, // Time:%02d:%02d
    TXT_BUTTON_SELL = 4, // Sell
    TXT_SELL = 5, // Sell Structure
    TXT_REPAIR_BUTTON = 6, // Repair
    TXT_YOU_COLON = 7, // You:
    TXT_ENEMY_COLON = 8, // Enemy:
    TXT_BUILD_DEST = 9, // Buildings Destroyed By
    TXT_UNIT_DEST = 10, // Units Destroyed By
    TXT_ORE_HARV = 11, // Ore Harvested By
    TXT_SCORE_1 = 12, // Score: %d
    TXT_YES = 13, // Yes
    TXT_NO = 14, // No
    TXT_SCENARIO_WON = 15, // Mission Accomplished
    TXT_SCENARIO_LOST = 16, // Mission Failed
    TXT_START_NEW_GAME = 17, // Start New Game
    TXT_INTRO = 18, // Intro & Sneak Peek
    TXT_CANCEL = 19, // Cancel
    TXT_ROCK = 20, // Rock
    TXT_CIVILIAN = 21, // Civilian
    TXT_JP = 22, // Containment Team
    TXT_OK = 23, // OK
    TXT_TREE = 24, // Tree
    TXT_LEFT = 25, // º
    TXT_RIGHT = 26, // º
    TXT_UP = 27, // º
    TXT_DOWN = 28, // º
    TXT_CLEAR = 29, // Clear
    TXT_WATER = 30, // Water
    TXT_ROAD = 31, // Road
    TXT_SLOPE = 32, // Slope
    TXT_PATCH = 33, // Patch
    TXT_RIVER = 34, // River
    TXT_LOAD_MISSION = 35, // Load Mission
    TXT_SAVE_MISSION = 36, // Save Mission
    TXT_DELETE_MISSION = 37, // Delete Mission
    TXT_LOAD_BUTTON = 38, // Load
    TXT_SAVE_BUTTON = 39, // Save
    TXT_DELETE_BUTTON = 40, // Delete
    TXT_GAME_CONTROLS = 41, // Game Controls
    TXT_SOUND_CONTROLS = 42, // Sound Controls
    TXT_RESUME_MISSION = 43, // Resume Mission
    TXT_VISUAL_CONTROLS = 44, // Visual Controls
    TXT_ABORT_MISSION = 45, // Abort Mission
    TXT_EXIT_GAME = 46, // Exit Game
    TXT_OPTIONS = 47, // Options
    TXT_SQUISH = 48, // Squish mark
    TXT_CRATER = 49, // Crater
    TXT_SCORCH = 50, // Scorch Mark
    TXT_BRIGHTNESS = 51, // BRIGHTNESS:
    TXT_MUSIC = 52, // Music Volume
    TXT_VOLUME = 53, // Sound Volume
    TXT_TINT = 54, // Tint:
    TXT_CONTRAST = 55, // Contrast:
    TXT_SPEED = 56, // Game Speed:
    TXT_SCROLLRATE = 57, // Scroll Rate:
    TXT_COLOR = 58, // Color:
    TXT_RETURN_TO_GAME = 59, // Return to game
    TXT_ENEMY_SOLDIER = 60, // Enemy Soldier
    TXT_ENEMY_VEHICLE = 61, // Enemy Vehicle
    TXT_ENEMY_STRUCTURE = 62, // Enemy Structure
    TXT_LTANK = 63, // Light Tank
    TXT_HTANK = 64, // Heavy Tank
    TXT_MTANK = 65, // Medium Tank
    TXT_4TANK = 66, // Mammoth Tank
    TXT_SAM = 67, // SAM Site
    TXT_JEEP = 68, // Ranger
    TXT_TRANS = 69, // Chinook Helicopter
    TXT_HARVESTER = 70, // Ore Truck
    TXT_ARTY = 71, // Artillery
    TXT_E1 = 72, // Rifle Infantry
    TXT_E2 = 73, // Grenadier
    TXT_E3 = 74, // Rocket Soldier
    TXT_E4 = 75, // Flamethrower
    TXT_HELI = 76, // Longbow Helicopter
    TXT_HIND = 77, // Hind
    TXT_APC = 78, // APC
    TXT_GUARD_TOWER = 79, // Guard Tower
    TXT_DOME = 80, // Radar Dome
    TXT_HELIPAD = 81, // Helipad
    TXT_AIRSTRIP = 82, // Airfield
    TXT_STORAGE = 83, // Ore Silo
    TXT_CONST_YARD = 84, // Construction Yard
    TXT_REFINERY = 85, // Ore Refinery
    TXT_CIV1 = 86, // Church
    TXT_CIV2 = 87, // Han's and Gretel's
    TXT_CIV3 = 88, // Hewitt's Manor
    TXT_CIV4 = 89, // Ricktor's House
    TXT_CIV5 = 90, // Gretchin's House
    TXT_CIV6 = 91, // The Barn
    TXT_CIV7 = 92, // Damon's pub
    TXT_CIV8 = 93, // Fran's House
    TXT_CIV9 = 94, // Music Factory
    TXT_CIV10 = 95, // Toymaker's
    TXT_CIV11 = 96, // Ludwig's House
    TXT_CIV12 = 97, // Haystacks
    TXT_CIV13 = 98, // Haystack
    TXT_CIV14 = 99, // Wheat Field
    TXT_CIV15 = 100, // Fallow Field
    TXT_CIV16 = 101, // Corn Field
    TXT_CIV17 = 102, // Celery Field
    TXT_CIV18 = 103, // Potato Field
    TXT_CIV20 = 104, // Sala's House
    TXT_CIV21 = 105, // Abdul's House
    TXT_CIV22 = 106, // Pablo's Wicked Pub
    TXT_CIV23 = 107, // Village Well
    TXT_CIV24 = 108, // Camel Trader
    TXT_CIV25 = 109, // Church
    TXT_CIV26 = 110, // Ali's House
    TXT_CIV27 = 111, // Trader Ted's
    TXT_CIV28 = 112, // Menelik's House
    TXT_CIV29 = 113, // Prestor John's House
    TXT_CIV30 = 114, // Village Well
    TXT_CIV31 = 115, // Witch Doctor's Hut
    TXT_CIV32 = 116, // Rikitikitembo's Hut
    TXT_CIV33 = 117, // Roarke's Hut
    TXT_CIV34 = 118, // Mubasa's Hut
    TXT_CIV35 = 119, // Aksum's Hut
    TXT_CIV36 = 120, // Mambo's Hut
    TXT_CIV37 = 121, // The Studio
    TXT_CIVMISS = 122, // Technology Center
    TXT_TURRET = 123, // Turret
    TXT_GUNBOAT = 124, // Gunboat
    TXT_MCV = 125, // Mobile Construction Vehicle
    TXT_POWER = 126, // Power Plant
    TXT_ADVANCED_POWER = 127, // Advanced Power Plant
    TXT_HOSPITAL = 128, // Hospital
    TXT_BARRACKS = 129, // Barracks
    TXT_PUMP = 130, // Oil Pump
    TXT_TANKER = 131, // Oil Tanker
    TXT_SANDBAG_WALL = 132, // Sandbags
    TXT_CYCLONE_WALL = 133, // Chain Link Fence
    TXT_CONCRETE_WALL = 134, // Concrete Wall
    TXT_BARBWIRE_WALL = 135, // Barbwire Fence
    TXT_WOOD_WALL = 136, // Wood Fence
    TXT_WEAPON_FACTORY = 137, // War Factory
    TXT_ADV_GUARD_TOWER = 138, // Advanced Guard Tower
    TXT_BIO_LAB = 139, // Bio-Research Laboratory
    TXT_FIX_IT = 140, // Service Depot
    TXT_TAB_SIDEBAR = 141, // Sidebar
    TXT_TAB_BUTTON_CONTROLS = 142, // Options
    TXT_TAB_BUTTON_DATABASE = 143, // Database
    TXT_SHADOW = 144, // Unrevealed Terrain
    TXT_OPTIONS_MENU = 145, // Options Menu
    TXT_STOP = 146, // Stop
    TXT_PLAY = 147, // Play
    TXT_SUFFLE = 148, // Shuffle
    TXT_REPEAT = 149, // Repeat
    TXT_MUSIC_VOLUME_COLON = 150, // Music volume:
    TXT_SOUND_VOLUME = 151, // Sound volume:
    TXT_ON = 152, // On
    TXT_OFF = 153, // Off
    TXT_MULTIPLAYER_GAME = 154, // Multiplayer Game
    TXT_NO_FILES = 155, // No files available
    TXT_DELETE_SINGLE_FILE = 156, // Do you want to delete this file?
    TXT_DELETE_MULTIPLE_FILES = 157, // Do you want to delete %d files?
    TXT_RESET_VALUES = 158, // Reset Values
    TXT_ASK_ABORT_MISSION = 159, // Do you want to abort the mission?
    TXT_MISSION_DESCRIPTION = 160, // Mission Description
    TXT_C1 = 161, // Joe
    TXT_C2 = 162, // Barry
    TXT_C3 = 163, // Shelly
    TXT_C4 = 164, // Maria
    TXT_C5 = 165, // Karen
    TXT_C6 = 166, // Steve
    TXT_C7 = 167, // Phil
    TXT_C8 = 168, // Dwight
    TXT_C9 = 169, // Erik
    TXT_EINSTEIN = 170, // Prof. Einstein
    TXT_BIB = 171, // Road Bib
    TXT_FASTER = 172, // Faster
    TXT_SLOWER = 173, // Slower
    TXT_AIR_STRIKE = 174, // Air Strike
    TXT_STEEL_CRATE = 175, // Steel Crate
    TXT_WOOD_CRATE = 176, // Wood Crate
    TXT_WATER_CRATE = 177, // Water Crate
    TXT_FLAG_SPOT = 178, // Flag Location
    TXT_UNABLE_READ_SCENARIO = 179, // Unable to read scenario!
    TXT_ERROR_LOADING_GAME = 180, // Error loading game!
    TXT_OBSOLETE_SAVEGAME = 181, // Obsolete saved game.
    TXT_MUST_ENTER_DESCRIPTION = 182, // You must enter a description!
    TXT_ERROR_SAVING_GAME = 183, // Error saving game!
    TXT_DELETE_FILE_QUERY = 184, // Delete this file?
    TXT_EMPTY_SLOT = 185, // [EMPTY SLOT]
    TXT_SELECT_MPLAYER_GAME = 186, // Select Multiplayer Game
    TXT_MODEM_SERIAL = 187, // Modem/Serial
    TXT_NETWORK = 188, // Network
    TXT_NETWORK_ERROR = 189, // Unable to initialize network!
    TXT_JOIN_NETWORK_GAME = 190, // Join Network Game
    TXT_NEW = 191, // New
    TXT_JOIN = 192, // Join
    TXT_SEND_MESSAGE = 193, // Send Message
    TXT_YOUR_NAME = 194, // Your Name:
    TXT_SIDE_COLON = 195, // Your Side:
    TXT_COLOR_COLON = 196, // Your Color:
    TXT_GAMES = 197, // Games
    TXT_PLAYERS = 198, // Players
    TXT_SCENARIO_COLON = 199, // Scenario:
    TXT_NOT_FOUND = 200, // >> NOT FOUND <<
    TXT_START_CREDITS_COLON = 201, // Starting Credits:
    TXT_BASES_COLON = 202, // Bases:
    TXT_ORE_COLON = 203, // Ore:
    TXT_CRATES_COLON = 204, // Crates:
    TXT_AI_PLAYERS_COLON = 205, // AI Players:
    TXT_REQUEST_DENIED = 206, // Request denied.
    TXT_UNABLE_PLAY_WAAUGH = 207, // Unable to play; scenario not found.
    TXT_NOTHING_TO_JOIN = 208, // Nothing to join!
    TXT_NAME_ERROR = 209, // You must enter a name!
    TXT_DUPENAMES_NOTALLOWED = 210, // Duplicate names are not allowed.
    TXT_YOURGAME_OUTDATED = 211, // Your game version is outdated.
    TXT_DESTGAME_OUTDATED = 212, // Destination game version is outdated.
    TXT_THATGUYS_GAME = 213, // %s's Game
    TXT_THATGUYS_GAME_BRACKET = 214, // [%s's Game]
    TXT_NETGAME_SETUP = 215, // Network Game Setup
    TXT_REJECT = 216, // Reject
    TXT_CANT_REJECT_SELF = 217, // You can't reject yourself!
    TXT_SELECT_PLAYER_REJECT = 218, // You must select a player to reject.
    TXT_BASES = 219, // Bases
    TXT_CRATES = 220, // Crates
    TXT_AI_PLAYERS = 221, // AI Players
    TXT_SCENARIOS = 222, // Scenarios
    TXT_CREDITS = 223, // Credits:
    TXT_ONLY_ONE = 224, // Only one player?
    TXT_OOPS = 225, // Oops!
    TXT_TO = 226, // To %s:
    TXT_TO_ALL = 227, // To All:
    TXT_MESSAGE = 228, // Message:
    TXT_CONNECTION_LOST = 229, // Connection to %s lost!
    TXT_PLAYER_LEFT_GAME = 230, // %s has left the game.
    TXT_PLAYER_DEFEATED = 231, // %s has been defeated!
    TXT_WAITING_CONNECT = 232, // Waiting to Connect...
    TXT_NULL_CONNERR_CHECK_CABLES = 233, // Connection error! Check your cables. Attempting to Reconnect...
    TXT_MODEM_CONNERR_REDIALING = 234, // Connection error! Redialing...
    TXT_MODEM_CONNERR_WAITING = 235, // Connection error! Waiting for Call...
    TXT_SELECT_SERIAL_GAME = 236, // Select Serial Game
    TXT_DIAL_MODEM = 237, // Dial Modem
    TXT_ANSWER_MODEM = 238, // Answer Modem
    TXT_NULL_MODEM = 239, // Null Modem
    TXT_SETTINGS = 240, // Settings
    TXT_PORT_COLON = 241, // Port:
    TXT_IRQ_COLON = 242, // IRQ:
    TXT_BAUD_COLON = 243, // Baud:
    TXT_INIT_STRING = 244, // Init String:
    TXT_CWAIT_STRING = 245, // Call Waiting String:
    TXT_TONE_BUTTON = 246, // Tone Dialing
    TXT_PULSE_BUTTON = 247, // Pulse Dialing
    TXT_HOST_SERIAL_GAME = 248, // Host Serial Game
    TXT_OPPONENT_COLON = 249, // Opponent:
    TXT_USER_SIGNED_OFF = 250, // User signed off!
    TXT_JOIN_SERIAL_GAME = 251, // Join Serial Game
    TXT_PHONE_LIST = 252, // Phone List
    TXT_ADD = 253, // Add
    TXT_EDIT = 254, // Edit
    TXT_DIAL = 255, // Dial
    TXT_DEFAULT = 256, // Default
    TXT_DEFAULT_SETTINGS = 257, // Default Settings
    TXT_CUSTOM_SETTINGS = 258, // Custom Settings
    TXT_PHONE_LISTING = 259, // Phone Listing
    TXT_NAME_COLON = 260, // Name:
    TXT_NUMBER_COLON = 261, // Number:
    TXT_UNABLE_FIND_MODEM = 262, // Unable to find modem. Check power and cables.
    TXT_NO_CARRIER = 263, // No carrier.
    TXT_LINE_BUSY = 264, // Line busy.
    TXT_NUMBER_INVALID = 265, // Number invalid.
    TXT_SYSTEM_NOT_RESPONDING = 266, // Other system not responding!
    TXT_OUT_OF_SYNC = 267, // Games are out of sync!
    TXT_PACKET_TOO_LATE = 268, // Packet received too late!
    TXT_PLAYER_LEFT_GAME_2 = 269, // Other player has left the game.                   //TODO, check enum name for this one
    TXT_FROM = 270, // From %s:%s
    TXT_SCORE_TIME = 271, // TIME:
    TXT_SCORE_LEAD = 272, // LEADERSHIP:
    TXT_SCORE_EFFI = 273, // ECONOMY:
    TXT_SCORE_TOTAL = 274, // TOTAL SCORE:
    TXT_SCORE_CASU = 275, // CASUALTIES:
    TXT_SCORE_NEUT = 276, // NEUTRAL:
    TXT_SCORE_BUIL = 277, // BUILDINGS LOST
    TXT_SCORE_BUIL1 = 278, // BUILDINGS
    TXT_SCORE_BUIL2 = 279, // LOST:
    TXT_SCORE_TOP = 280, // TOP SCORES
    TXT_SCORE_ENDCRED = 281, // ENDING CREDITS:
    TXT_SCORE_TIMEFORMAT1 = 282, // %dh %dm
    TXT_SCORE_TIMEFORMAT2 = 283, // %dm
    TXT_DIALING = 284, // Dialing...
    TXT_DIALING_CANCELED = 285, // Dialing Canceled
    TXT_WAITING_FOR_CALL = 286, // Waiting for Call...
    TXT_ANSWERING_CANCELED = 287, // Answering Canceled
    TXT_E6 = 288, // Engineer
    TXT_SPY = 289, // Spy
    TXT_MODEM_OR_LOOPBACK = 290, // Not a Null Modem Cable Attached! It is a modem or loopback cable.
    TXT_MAP = 291, // Map
    TXT_BLOSSOM_TREE = 292, // Blossom Tree
    TXT_RESTATE_MISSION = 293, // Briefing
    TXT_COMPUTER = 294, // Computer
    TXT_UNIT_COUNT = 295, // Unit Count:
    TXT_TECH_LEVEL = 296, // Tech Level:
    TXT_OPPONENT = 297, // Opponent
    TXT_KILLS_COLON = 298, // Kills:
    TXT_VIDEO = 299, // Video
    TXT_SCIENTIST = 300, // Scientist
    TXT_CAPTURE_THE_FLAG = 301, // Capture The Flag
    TXT_OBJECTIVE = 302, // Mission Objective
    TXT_MISSION = 303, // Mission
    TXT_NO_SAVES = 304, // No saved games available.
    TXT_CIVILIAN_BUILDING = 305, // Civilian Building
    TXT_TECHNICIAN = 306, // Technician
    TXT_NO_SAVELOAD = 307, // Save game options are not allowed during a multiplayer session.
    TXT_DELPHI = 308, // Special 1 agent delphi
    TXT_TO_REPLAY = 309, // Would you like to replay this mission?
    TXT_RECONN_TO = 310, // Reconnecting to %s.
    TXT_PLEASE_WAIT = 311, // Please wait %02d seconds.
    TXT_SURRENDER = 312, // Do you wish to surrender?
    TXT_SEL_TRANS = 313, // SELECT TRANSMISSION
    TXT_GAMENAME_MUSTBE_UNIQUE = 314, // Your game name must be unique.
    TXT_GAME_IS_CLOSED = 315, // Game is closed.
    TXT_NAME_MUSTBE_UNIQUE = 316, // Your name must be unique.
    TXT_RECONNECTING_TO = 317, // Reconnecting to %s
    TXT_WAITING_FOR_CONNECTIONS = 318, // Waiting for connections...
    TXT_TIME_ALLOWED = 319, // Time allowed: %02d seconds
    TXT_PRESS_ESC = 320, // Press ESC to cancel.
    TXT_JUST_YOU_AND_ME = 321, // From Computer: It's just you and me now!
    TXT_CAPTURE_THE_FLAG_COLON = 322, // Capture the Flag:
    TXT_CHAN = 323, // Dr Chan from TD.
    TXT_HAS_ALLIED = 324, // %s has allied with %s
    TXT_AT_WAR = 325, // %s declares war on %s
    TXT_SEL_TARGET = 326, // Select a target
    TXT_RESIGN = 327, // Resign Game
    TXT_ORE_FAST = 328, // Ore grows quickly.
    TXT_ANSWERING = 329, // Answering...
    TXT_INITIALIZING_MODEM = 330, // Initializing Modem...
    TXT_SCENARIOS_DO_NOT_MATCH = 331, // Scenarios don't match.
    TXT_POWER_OUTPUT = 332, // Power Output
    TXT_POWER_OUTPUT_LOW = 333, // Power Output (low)
    TXT_CONTINUE = 334, // Continue
    TXT_QUEUE_FULL = 335, // Data Queue Overflow
    TXT_SPECIAL_WARNING = 336, // %s changed game options!
    TXT_CD_DIALOG_1 = 337, // Please insert a Red Alert CD into the CD-ROM drive.
    TXT_CD_DIALOG_2 = 338, // Please insert CD %d (%s) into the CD-ROM drive.
    TXT_CD_DIALOG_ERROR1 = 339, // Red Alert is unable to detect your CD ROM drive.
    TXT_NO_SOUND_CARD = 340, // No Sound Card Detected
    TXT_UNKNOWN = 341, // UNKNOWN
    TXT_OLD_GAME = 342, // (old)
    TXT_NO_SPACE = 343, // Insufficient Disk Space to run Red Alert.
    TXT_HAVE_SPACE = 344, // You must have %d megabytes of free disk space.
    TXT_RUN_SETUP = 345, // Run SETUP program first.
    TXT_WAITING_FOR_OPPONENT = 346, // Waiting for Opponent
    TXT_SELECT_SETTINGS = 347, // Please select 'Settings' to setup default configuration
    TXT_PRISON = 348, // Prison
    TXT_GAME_WAS_SAVED = 349, // Mission Saved
    TXT_INSUFFICIENT_SAVE = 350, // Insufficient disk space to save a game.  Please delete a previous save to free up some disk space and try again.
    TXT_PORT_ADDRESS = 351, // Invalid Port/Address. COM 1-4 OR ADDRESS
    TXT_PORT_INVALID_SETTINGS = 352, // Invalid Port and/or IRQ settings
    TXT_IRQ_ALREADY_IN_USE = 353, // IRQ already in use
    TXT_ABORT = 354, // Abort
    TXT_RESTART = 355, // Restart
    TXT_RESTARTING = 356, // Mission is restarting. Please wait...
    TXT_LOADING = 357, // Mission is loading. Please wait...
    TXT_ERROR_IN_INITSTRING = 358, // Error in the InitString
    TXT_SHROUD_COLON = 359, // Shroud:
    TXT_AVM = 360, // Anti-Vehicle Mine
    TXT_APM = 361, // Anti-Personnel Mine
    TXT_NEW_MISSIONS = 362, // New Missions
    TXT_THIEF = 363, // Thief
    TXT_MRJ = 364, // Radar Jammer
    TXT_MGG = 365, // Gap Generator
    TXT_PILLBOX = 366, // Pillbox
    TXT_CAMO_PILLBOX = 367, // Camo. Pillbox
    TXT_CHRONOSPHERE = 368, // Chronosphere
    TXT_ENGLAND = 369, // England
    TXT_GERMANY = 370, // Germany
    TXT_SPAIN = 371, // Spain
    TXT_RUSSIA = 372, // Russia
    TXT_UKRAINE = 373, // Ukraine
    TXT_GREECE = 374, // Greece
    TXT_FRANCE = 375, // France
    TXT_TURKEY = 376, // Turkey
    TXT_SHORE = 377, // Shore
    TXT_SELECT_OBJECT = 378, // Select Object
    TXT_SS = 379, // Submarine
    TXT_DD = 380, // Destroyer
    TXT_CA = 381, // Cruiser
    TXT_LST = 382, // Transport
    TXT_PT = 383, // Gun Boat
    TXT_LOBBY = 384, // Lobby
    TXT_LOBBY_GAMES = 385, // Games
    TXT_SAVING_GAME = 386, // Save Game...
    TXT_GAME_FULL = 387, // Game is full.
    TXT_MUST_SELECT_GAME = 388, // You must select a game!
    TXT_S_PLAYING_S = 389, // %s playing %s
    TXT_ONLY_HOST_CAN_MODIFY = 390, // Only the host can modify this option.
    TXT_GAME_CANCELLED = 391, // Game was cancelled.
    TXT_S_FORMED_NEW_GAME = 392, // %s has formed a new game.
    TXT_GAME_NOW_IN_PROGRESS = 393, // %s's game is now in progress.
    TXT_TESLA_COIL = 394, // Tesla Coil
    TXT_MOBILE_GAP_GEN = 395, // Mobile Gap Generator
    TXT_FLAME_TOWER = 396, // Flame Tower
    TXT_AA_GUN = 397, // AA Gun
    TXT_KENNEL = 398, // Kennel
    TXT_TECH_CENTER = 399, // Soviet Tech Center
    TXT_BADGER = 400, // Badger Bomber
    TXT_MIG = 401, // Mig Attack Plane
    TXT_YAK = 402, // Yak Attack Plane
    TXT_FENC = 403, // Barbed Wire
    TXT_MEDIC = 404, // Field Medic
    TXT_SABOTEUR = 405, // Saboteur
    TXT_GENERAL = 406, // General
    TXT_E7 = 407, // Tanya
    TXT_PARABOMBS = 408, // Parabombs
    TXT_PARATROOPERS = 409, // Paratroopers
    TXT_PARA_SABOTEUR = 410, // Parachute Saboteur
    TXT_NAVAL_YARD = 411, // Naval Yard
    TXT_SUB_PEN = 412, // Sub Pen
    TXT_SCENARIO_OPTS = 413, // Scenario Options
    TXT_SPY_PLANE = 414, // Spy Plane
    TXT_U2 = 415, // Spy Plane
    TXT_DOG = 416, // Attack Dog
    TXT_SPY_INFO = 417, // Spy Info
    TXT_INFO_BUILDINGS = 418, // Buildings
    TXT_INFO_UNITS = 419, // Units
    TXT_INFO_INFANTRY = 420, // Infantry
    TXT_INFO_AIRCRAFT = 421, // Aircraft
    TXT_TRUCK = 422, // Supply Truck
    TXT_INVUL_DEVICE = 423, // Invulnerability Device
    TXT_IRON_CURTAIN = 424, // Iron Curtain
    TXT_ALLIED_TECH_CENTER = 425, // Allied Tech Center
    TXT_V2RL = 426, // V2 Rocket
    TXT_FCOM = 427, // Forward Command Post
    TXT_DEMOLITIONER = 428, // Demolitioner
    TXT_MNLY = 429, // Mine Layer
    TXT_FAKE_CONST_YARD = 430, // Fake Construction Yard
    TXT_FAKE_WEAP_FACT = 431, // Fake War Factory
    TXT_FAKE_NAVAL_YARD = 432, // Fake Naval Yard
    TXT_FAKE_SUB_PEN = 433, // Fake Sub Pen
    TXT_FAKE_RADAR_DOME = 434, // Fake Radar Dome
    TXT_THEME_BIGFOOT = 435, // Bigfoot
    TXT_THEME_CRUSH = 436, // Crush
    TXT_THEME_FACE_ENEMY_1 = 437, // Face the Enemy 1
    TXT_THEME_FACE_ENEMY_2 = 438, // Face the Enemy 2
    TXT_THEME_HELL_MARCH = 439, // Hell March
    TXT_THEME_RUN_LIFE = 440, // Run for Your Life
    TXT_THEME_SMASH = 441, // Smash
    TXT_THEME_TRENCHES = 442, // Trenches
    TXT_THEME_WORKMEN = 443, // Workmen
    TXT_THEME_AWAIT = 444, // Await
    TXT_THEME_DENSE = 445, // Dense
    TXT_THEME_MAP_SEL = 446, // Map Selection
    TXT_THEME_FOGGER = 447, // Fogger
    TXT_THEME_MUD = 448, // Mud
    TXT_THEME_RADIO2 = 449, // Radio 2
    TXT_THEME_ROLL_OUT = 450, // Roll Out
    TXT_THEME_SNAKE = 451, // Snake
    TXT_THEME_TERMINATE = 452, // Terminate
    TXT_THEME_TWIN = 453, // Twin
    TXT_THEME_VECTOR = 454, // Vector
    TXT_TEAM_MEMBERS = 455, // Team Members
    TXT_BRIDGE = 456, // Bridge
    TXT_BARREL = 457, // Barrel
    TXT_FRIENDLY = 458, // Friendly
    TXT_ENEMY = 459, // Enemy
    TXT_GOLD = 460, // Gold
    TXT_GEMS = 461, // Gems
    TXT_TITLE_MOVIE = 462, // Title Movie
    TXT_MOVIES = 463, // Movies
    TXT_INTERIOR = 464, // Interior
    TXT_SONAR_PULSE = 465, // Sonar Pulse
    TXT_MISSILE_SILO = 466, // Missile Silo
    TXT_GPS_SATELLITE = 467, // GPS Satellite
    TXT_NUCLEAR_BOMB = 468, // Atom Bomb
    TXT_EASY = 469, // Easy
    TXT_HARD = 470, // Hard
    TXT_NORMAL = 471, // Normal
    TXT_SET_DIFFICULTY = 472, // Please set the difficulty level. It will be used throughout this campaign.
    TXT_ALLIES = 473, // Allies
    TXT_SOVIET = 474, // Soviet
    TXT_THEME_MENU = 475, // Intro Theme
    TXT_SHROUD_REGROWS = 476, // Shroud Regrows
    TXT_ORE_REGENERATES = 477, // Ore Regenerates
    TXT_THEME_SCORE = 478, // Score Theme
    TXT_INTERNET_GAME = 479, // Internet Game
    TXT_ICE = 480, // Ice
    TXT_CRATES_2 = 481, // Crates                    //TODO, check enum name for this one
    TXT_SKIRMISH = 482, // Skirmish
    TXT_CHOOSE_SIDE = 483, // Choose your side.
    TXT_VALUABLE_MINERALS = 484, // Valuable Minerals
    TXT_IGNORE = 485, // Ignore
    TXT_ERROR_NOT_RESPONDING = 486, // Error - modem is not responding.
    TXT_ERROR_NOT_ENABLED = 487, // Error - modem did not respond to result code enable command.
    TXT_ERROR_NO_INIT = 488, // Error - modem did not respond to initialisation string.
    TXT_ERROR_NO_VERBOSE = 489, // Error - modem did not respond to 'verbose' command.
    TXT_ERROR_NO_ECHO = 490, // Error - modem did not respond to 'echo' command.
    TXT_ERROR_NO_DISABLE = 491, // Error - unable to disable modem auto answer.
    TXT_ERROR_TOO_MANY = 492, // Error - Too many errors initialising modem - Aborting.
    TXT_ERROR_ERROR = 493, // Error - Modem returned error status.
    TXT_ERROR_TIMEOUT = 494, // Error - Time out waiting for connect.
    TXT_ACCOMPLISHED = 495, // Accomplished
    TXT_CLICK_CONTINUE = 496, // Click to Continue
    TXT_RECEIVING_SCENARIO = 497, // Receiving scenario from host.
    TXT_SENDING_SCENARIO = 498, // Sending scenario to remote players.
    TXT_NO_FLOW_CONTROL_RESPONSE = 499, // Error - Modem failed to respond to flow control command. Your Windows configuration may be incorrect.
    TXT_NO_COMP_DISABLE_RESPONSE = 500, // Error - Modem failed to respond to compression command. Your Windows configuration may be incorrect.
    TXT_NO_EC_DISABLE_RESPONSE = 501, // Error - Modem failed to respond to error correction command. Your Windows configuration may be incorrect.
    TXT_REGISTER_PLANETWW = 502, // To play Red Alert via the internet you must be connected to an internet services provider and be registered with Planet Westwood
    TXT_NO_WCHAT = 503, // Wchat not installed. Please install it from either CD.
    TXT_REGISTER = 504, // Register
    TXT_ORE_MINE = 505, // Ore Mine
    TXT_NO_REGISTERED_MODEM = 506, // No registered modem
    TXT_CHRONOSHIFT = 507, // Chronoshift
    TXT_UNABLE_TO_OPEN_PORT = 508, // Invalid Port or Port is in use
    TXT_NO_DIAL_TONE = 509, // No dial tone. Ensure your modem is connected to the phone line and try again.
    TXT_NO_EXPANSION_SCENARIO = 510, // Error - other player does not have this expansion scenario.
    TXT_PLEASE_STANDBY = 511, // Please Stand By...
    TXT_THEME_CREDITS = 512, // End Credits Theme
    TXT_AA_GUNS_OFFLINE = 513, // Low Power: AA-Guns offline
    TXT_TESLA_COILS_OFFLINE = 514, // Low Power: Tesla Coils offline
    TXT_LOWER_POWER = 515, // Low Power
    TXT_COMMANDER_COLON = 516, // Commander:
    TXT_BATTLES_WON_COLON = 517, // Battles Won:
    TXT_MISMATCH = 518, // Game versions incompatible. To make sure you have the latest version, visit www.westwood.com
    TXT_SCENARIO_ERROR = 519, // Incompatible scenario file detected. The scenario may be corrupt.
    TXT_CONNECTING = 520, // Connecting...
    TXT_MODEM_INIT = 521, // Modem Initialization
    TXT_DATA_COMPRESSION = 522, // Data Compression
    TXT_ERROR_CORRECTION = 523, // Error Correction
    TXT_HARDWARE_FLOW_CONTROL = 524, // Hardware Flow Control
    TXT_ADVANCED = 525, // Advanced
    TXT_THEME_2NDHAND = 526, // 2nd_Hand
    TXT_THEME_ARAZOID = 527, // Arazoid
    TXT_THEME_BACKSTAB = 528, // BackStab
    TXT_THEME_CHAOS2 = 529, // Chaos2
    TXT_THEME_SHUT_IT = 530, // Shut_It
    TXT_THEME_TWINMIX1 = 531, // TwinMix1
    TXT_THEME_UNDER3 = 532, // Under3
    TXT_THEME_VR2 = 533, // VR2
    TXT_PLAYER_NOT_RESPONDING = 534, // The other system is not responding. Do you wish to attempt an emergency game save? Both players must save for this to work.
    TXT_PLAYER_HUNG_UP = 535, // The other system hung up. Do you wish to attempt an emergency game save? Both players must save for this to work.
    TXT_ERROR_REGISTRATION_FAILED = 536, // Red Alert was unable to run the registration software. You need to install Westwood Chat from the Red Alert CD to register.
    TXT_PLAYER_NO_EXPANSION_SCENARIO = 537, // A player in the game does not have this expansion scenario.
    TXT_MS = 538, // Missile Sub
    TXT_SHOK = 539, // Shock Trooper
    TXT_MECH = 540, // Mechanic
    TXT_CTNK = 541, // Chrono Tank
    TXT_TTNK = 542, // Tesla Tank
    TXT_QTNK = 543, // M.A.D. Tank
    TXT_DTRK = 544, // Demolition Truck
    TXT_STNK = 545, // Phase Transport
    TXT_THEME_BOG = 546, // Bog
    TXT_THEME_FLOATING = 547, // Floating
    TXT_THEME_GLOOM = 548, // Gloom
    TXT_THEME_GROUNDWIRE = 549, // Ground Wire
    TXT_THEME_MECHMAN2 = 550, // Mech Man 2
    TXT_THEME_SEARCH = 551, // Search
    TXT_THEME_TRACTION = 552, // Traction
    TXT_THEME_WASTELAND = 553, // Wasteland
    TXT_CARR = 554, // Helicarrier

    TXT_COUNT,
    TXT_NONE = -1,
    TXT_FIRST = TXT_NULL,

    //
    // Debug Strings.
    //
    TXT_DEBUG_MAGIC_NUM = 1000,

    TXT_DEBUG_CLEAR_MAP = 1000, // Clear the map
    TXT_DEBUG_INHERIT_PREV_MAP, // Inherit previous map
    TXT_DEBUG_SPECIAL_OPTIONS, // Select Special Options
    TXT_DEBUG_FLASH_TO_ALL, // Targeting flash visible to all.
    TXT_DEBUG_TREE_TARGETING, // Allow targeting of trees.
    TXT_DEBUG_CONST_UNDEPLOY, // Allow undeploy of construction yard.
    TXT_DEBUG_SMARTER_SELF_DEFENSE, // Employ smarter self defense logic.
    TXT_DEBUG_MODERATE_PRODUCTION, // Moderate production speed.
    TXT_DEBUG_3POINT_TURN, // Use three point turn logic.
    TXT_DEBUG_ORE_GROW, // Ore will grow.
    TXT_DEBUG_ORE_SPREAD, // Ore will spread.
    TXT_DEBUG_DISABLE_BIBS, // Disable building "bib" pieces.
    TXT_DEBUG_RUN_FROM_THREATS, // Allow running from immediate threats.
    TXT_DEBUG_TRUE_NAMES, // Show true object names.
    TXT_DEBUG_DEFENDER_ADVANTAGE, // Defender has the advantage.
    TXT_DEBUG_SEPERATE_HELIPAD, // Allow separate helipad purchase
    TXT_DEBUG_PASSWORD_REQUEST, // Password Request
    TXT_DEBUG_ENTER_CODE, // Enter Red Alert access code to gain access.
    TXT_DEBUG_ACCESS_ERROR, // Access code error detected.
    TXT_DEBUG_TRY_AGAIN, // Try Again
    TXT_DEBUG_AVOID_MINES, // Friendly units avoid friendly mines
    TXT_DEBUG_TRIGGER_EDITOR, // Trigger Editor
    TXT_DEBUG_JUST_EVENT, // Just This Event
    TXT_DEBUG_TRIGGER_AND, // ... and ...
    TXT_DEBUG_TRIGGER_OR, // ... or ...
    TXT_DEBUG_TRIGGER_LINKED, // ... linked ...
    TXT_DEBUG_JUST_ACTION, // Just This Action
    TXT_DEBUG_TEAM_EDITOR, // Team Editor
    TXT_DEBUG_SELLABLE, // Sellable
    TXT_DEBUG_REBUILD, // Rebuild
    TXT_DEBUG_CONSTRUCTION_ACCELERATED, // Building constructin time accelerated?
    TXT_DEBUG_SCEN_AUTH_MISSING, // Scenario authentication code missing or invalid. Re-save scenario to correct it.
    TXT_DEBUG_MONO_1, // 
    TXT_DEBUG_MONO_2, // 
    TXT_DEBUG_MONO_3, // 
    TXT_DEBUG_MONO_4, // 
    TXT_DEBUG_MONO_5, // 
    TXT_DEBUG_MONO_6, // 
    TXT_DEBUG_MONO_7, // 
    TXT_DEBUG_MONO_8, // 
    TXT_DEBUG_PENTIUM_PAD_TOP, // *******************************
    TXT_DEBUG_PENTIUM_NOT_DETECTED, // ** Pentium CPU not detected. **
    TXT_DEBUG_PENTIUM_PAD_BOTTOM, // *******************************
    TXT_DEBUG_SIZE_MAP, // Size Map
    TXT_DEBUG_TRUCKS_DROP_CRATE, // Trucks drop crate when destroyed?

    TXT_DEBUG_COUNT,
    TXT_DEBUG_FIRST = TXT_DEBUG_CLEAR_MAP,

    //
    // Editor Strings.
    //
    TXT_EDITOR_MAGIC_NUM = 2000,

    TXT_EDITOR_NEW_MAP = 2000, // New Map
    TXT_EDITOR_LOAD_MAP, // Load Map
    TXT_EDITOR_SAVE_MAP, // Save Map
    TXT_EDITOR_MODIFY_MAP, // Modify Map Info
    TXT_EDITOR_RESUME_EDITING, // Resume Editing
    TXT_EDITOR_QUIT_EDITOR, // Quit Editor
    TXT_EDITOR_UNSAVED_LOAD_FILE, // You have not saved your changes!  Load Another file?
    TXT_EDITOR_UNSAVED_EXIT_EDITOR, // You have not saved your changes!  Exit Scenario Editor?
    TXT_EDITOR_EXIT_EDITOR, // Exit Scenario Editor?
    TXT_EDITOR_64_BY_64, // 64 x 64
    TXT_EDITOR_96_BY_96, // 96 x 96
    TXT_EDITOR_126_BY_64, // 126 x 64
    TXT_EDITOR_64_BY_126, // 64 x 126
    TXT_EDITOR_TEMPERATE, // Temperate
    TXT_EDITOR_SNOW, // Snow
    TXT_EDITOR_RIDGES, // Ridges
    TXT_EDITOR_TREES, // Trees
    TXT_EDITOR_DEBRIS, // Debris
    TXT_EDITOR_UNABLE_TO_READ, // Unable to read scenario!
    TXT_EDITOR_SCENARIO_LOADED, // Scenario Loaded
    TXT_EDITOR_SCENARIO_SAVED, // Scenario Saved.
    TXT_EDITOR_OBSOLETE_SCENARIO, // Obsolete Scenario.
    TXT_EDITOR_ASK_REPLACE, // Do you wish to replace the existing scenario named %s?
    TXT_EDITOR_LOW_DISK_SPACE, // Insufficient disk space to save scenario.  Please delete a previous scenario to free up some disk space and try again.
    TXT_EDITOR_SELECT_TILE, // Select Tile
    TXT_EDITOR_ASK_UNSAVED_NEW, // You have not saved your changes!  Create a new map?
    TXT_EDITOR_DELETE_MAP, // Delete Map
    TXT_EDITOR_ASK_DELETE, // Are you sure you wish to delete this scenario?
    TXT_EDITOR_MAP_DESCRIPTION, // Map Description
    TXT_EDITOR_PASSABLE, // Passable
    TXT_EDITOR_CONFIRMATION, // Confirmation
    TXT_EDITOR_ASK_RESIZE, // Portions of the map that don't fit on the new size will be deleted!  Resize the map?
    TXT_EDITOR_SCROLL_RATE, // Change Scroll Rate
    TXT_EDITOR_LOADING_MAP, // Loading Map
    TXT_EDITOR_NO_MORE_AVAILABLE, // No more objects of this type are available.
    TXT_EDITOR_FAILED_INIT_TIMER, // Unable to to initialize system timer.  Please restart windows.
    TXT_EDITOR_STARTUP_ERROR, // Startup Error
    TXT_EDITOR_TILESET_PAGE, // Page %d of %d
    TXT_EDITOR_CURRENT_MAP, // Current Map: %s
    TXT_EDITOR_CURRENT_MAP_UNTITLED, // Current Map: Untitled

    TXT_EDITOR_COUNT = 40,
    TXT_EDITOR_FIRST = TXT_EDITOR_NEW_MAP,

    //
    // Chronoshift Strings.
    //
    TXT_ADDITIONAL_MAGIC_NUM = 3000, //

    TXT_ADDITIONAL_CS_MISSIONS = 3000, // Counterstrike Missions
    TXT_ADDITIONAL_AM_MISSIONS, // = 3001,     // Aftermath Missions

    TXT_ADDITIONAL_MORE_1, // = 3002,     // --More--
    TXT_ADDITIONAL_MORE_2, // = 3003,     // <MORE>
    TXT_ADDITIONAL_MORE_3, // = 3004,     // More

    TXT_ADDITIONAL_CONFIRM_QUIT, // = 3005,     // Are you sure you want to quit?

    TXT_ADDITIONAL_COMP_MSG1, // = 3006,     // Prepare to die!
    TXT_ADDITIONAL_COMP_MSG2, // = 3007,     // How about a bullet sandwich?!
    TXT_ADDITIONAL_COMP_MSG3, // = 3008,     // Incoming!
    TXT_ADDITIONAL_COMP_MSG4, // = 3009,     // I see you!
    TXT_ADDITIONAL_COMP_MSG5, // = 3010,     // Hey, I'm over here!
    TXT_ADDITIONAL_COMP_MSG6, // = 3011,     // Come get some!
    TXT_ADDITIONAL_COMP_MSG7, // = 3012,     // I got you!
    TXT_ADDITIONAL_COMP_MSG8, // = 3013,     // You humans are never a challenge!
    TXT_ADDITIONAL_COMP_MSG9, // = 3014,     // Abort, Retry, Ignore? (Ha ha!)
    TXT_ADDITIONAL_COMP_MSG10, // = 3015,     // Format another? (Just kidding!)
    TXT_ADDITIONAL_COMP_MSG11, // = 3016,     // Beat me and I'll reboot!
    TXT_ADDITIONAL_COMP_MSG12, // = 3017,     // You're artificial intelligence!
    TXT_ADDITIONAL_COMP_MSG13, // = 3018,     // My AI is better than your AI.

    TXT_ADDITIONAL_FROM_COMPUTER, // = 3019,     // From Computer: %s
    TXT_ADDITIONAL_FROM_FORMAT, // = 3020,     // From %s: %s

    TXT_ADDITIONAL_ORE_GEM_SPECIAL, // = 3021,     // Ore and Gems are now people!!

    TXT_ADDITIONAL_BASE_DEFENSES_OFFLINE, // = 3022,     // Low Power: Base defenses offline

    TXT_ADDITIONAL_CHRONOSHIFT_TODO, // = 3023,     // << !! Chronoshift TODO !! >>

    // Keyboard Command Categories
    TXT_ADDITIONAL_CONTROL, // = ,     // Control
    TXT_ADDITIONAL_INFORMATION, // = ,     // Information
    TXT_ADDITIONAL_INTERFACE, // = ,     // Interface
    TXT_ADDITIONAL_SELECTION, // = ,     // Selection
    TXT_ADDITIONAL_TEAM, // = ,     // Team
    TXT_ADDITIONAL_MISC, // = ,     // Misc
    TXT_ADDITIONAL_THEME, // = ,     // Theme
    TXT_ADDITIONAL_EDITOR, // = ,     // Editor
    TXT_ADDITIONAL_DEBUG, // = ,     // Debug

    // Keyboard Commands
    TXT_ADDITIONAL_ALLIANCE, // = ,     // Alliance
    TXT_ADDITIONAL_ALLIANCE_DESC, // = ,     // Toggle alliance with owner of selected object.
    TXT_ADDITIONAL_DEPLOY_OBJECT, // = ,     // Deploy Object
    TXT_ADDITIONAL_DEPLOY_OBJECT_DESC, // = ,     // Deploy selected object(s).

    TXT_ADDITIONAL_COUNT = 3999,
    TXT_ADDITIONAL_FIRST = TXT_ADDITIONAL_CS_MISSIONS,

    //
    // 
    //
    TXT_MPMAP_MAGIC_NUM = 5000, //

    TXT_MAX = 9999

};

// clang-format on

enum LanguageType
{
    LANGUAGE_ENGLISH,
    LANGUAGE_FRENCH,
    LANGUAGE_GERMAN,
    LANGUAGE_ITALIAN,
    LANGUAGE_SPANISH,
    LANGUAGE_JAPANESE,

    LANGUAGE_COUNT,

    LANGUAGE_NONE = -1,
};

extern LanguageType Language;
extern const char *MissionStr[];
extern const char TXT_CS_MISSIONS[];
extern const char TXT_AM_MISSIONS[];
extern const char TXT_ABOUT[];
extern const char TXT_MORE_1[];
extern const char TXT_MORE_2[];
extern const char TXT_CONFIRM_QUIT[];
extern const char TXT_CHRONOSHIFT_TITLE[];
extern const char TXT_CHRONOSHIFT_NOTE[];
extern const char TXT_CHRONOSHIFT_WEBSITE[];

bool Init_Language();
const char *Language_Name(const char *filename);
const char *Get_Language_Char();
const char *Fetch_String(int str_id);
const char *Fetch_Mission_String(int str_id);

#endif // LANGUAGE_H
