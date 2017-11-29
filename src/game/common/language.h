/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Functions for handling string tables and name overrides.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
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

#ifndef RAPP_STANDALONE
#include "hooker.h"

extern char *&GameStrings;
extern char *&DebugStrings;
extern char **NameOverride;
extern int *NameIDOverride;
#else
extern char *GameStrings;
extern char *DebugStrings;
extern char *NameOverride[25];
extern int NameIDOverride[25];
#endif

extern char *EditorStrings;

// the following variables defines the text used throughout the game.
enum TextEnum
{
    TXT_NONE = -1,

    TXT_FIRST = 0,

    TXT_NULL = 0, //"bollocks"	//good ol' Retaliation tells me this, even though its blank in the PC verison.
    // TXT_ 	= 1,		// %3d.%02d
    TXT_TIME_HMS = 2, // Time:%02d:%02d:%02d
    TXT_TIME_HM = 3, // Time:%02d:%02d
    // TXT_ 	= 4,		// Sell
    // TXT_ 	= 5,		// Sell Structure
    // TXT_ 	= 6,		// Repair
    // TXT_ 	= 7,		// You:
    // TXT_ 	= 8,		// Enemy:
    // TXT_ 	= 9,		// Buildings Destroyed By
    // TXT_ 	= 10,		// Units Destroyed By
    // TXT_ 	= 11,		// Ore Harvested By
    // TXT_ 	= 12,		// Score: %d
    // TXT_YES 	= 13,		// Yes
    // TXT_NO 	= 14,		// No
    TXT_MISSION_ACCOMPLISHED = 15, // Mission Accomplished
    TXT_MISSION_FAILED = 16, // Mission Failed
    TXT_NEW_GAME = 17, // Start New Game
    TXT_SNEAK_PEEK = 18, // Intro & Sneak Peek
    TXT_CANCEL = 19, // Cancel
    // TXT_ 	= 20,		// Rock
    // TXT_ 	= 21,		// Civilian
    // TXT_ 	= 22,		// Containment Team
    TXT_OK = 23, // OK
    // TXT_ 	= 24,		// Tree
    // TXT_ 	= 25,		// º
    // TXT_ 	= 26,		// º
    // TXT_ 	= 27,		// º
    // TXT_ 	= 28,		// º
    // TXT_ 	= 29,		// Clear
    // TXT_ 	= 30,		// Water
    // TXT_ 	= 31,		// Road
    // TXT_ 	= 32,		// Slope
    // TXT_ 	= 33,		// Patch
    // TXT_ 	= 34,		// River
    TXT_LOAD_MISSION = 35, // Load Mission
    TXT_SAVE_MISSION = 36, // Save Mission
    TXT_DELETE_MISSION = 37, // Delete Mission
    TXT_LOAD = 38, // Load
    TXT_SAVE = 39, // Save
    TXT_DELETE = 40, // Delete
    TXT_GAME_CONTROLS = 41, // Game Controls
    TXT_SOUND_CONTROLS = 42, // Sound Controls
    TXT_RESUME_MISSION = 43, // Resume Mission
    TXT_VISUAL_CONTROLS = 44, // Visual Controls
    TXT_ABORT_MISSION = 45, // Abort Mission
    TXT_EXIT_GAME = 46, // Exit Game
    // TXT_OPTIONS 	= 47,		// Options
    // TXT_ 	= 48,		// Squish mark
    // TXT_ 	= 49,		// Crater
    // TXT_ 	= 50,		// Scorch Mark
    // TXT_ 	= 51,		// BRIGHTNESS:
    // TXT_ 	= 52,		// Music Volume
    // TXT_ 	= 53,		// Sound Volume
    // TXT_ 	= 54,		// Tint:
    // TXT_ 	= 55,		// Contrast:
    // TXT_ 	= 56,		// Game Speed:
    // TXT_ 	= 57,		// Scroll Rate:
    // TXT_ 	= 58,		// Color:
    // TXT_ 	= 59,		// Return to game
    TXT_TTIP_ENEMY_SOLDIER = 60, // Enemy Soldier
    TXT_TTIP_ENEMY_VEHICLE = 61, // Enemy Vehicle
    TXT_TTIP_ENEMY_STRUCTURE = 62, // Enemy Structure
    // TXT_ 	= 63,		// Light Tank
    // TXT_ 	= 64,		// Heavy Tank
    // TXT_ 	= 65,		// Medium Tank
    // TXT_ 	= 66,		// Mammoth Tank
    // TXT_ 	= 67,		// SAM Site
    // TXT_ 	= 68,		// Ranger
    // TXT_ 	= 69,		// Chinook Helicopter
    // TXT_ 	= 70,		// Ore Truck
    // TXT_ 	= 71,		// Artillery
    // TXT_ 	= 72,		// Rifle Infantry
    // TXT_ 	= 73,		// Grenadier
    // TXT_ 	= 74,		// Rocket Soldier
    // TXT_ 	= 75,		// Flamethrower
    // TXT_ 	= 76,		// Longbow Helicopter
    // TXT_ 	= 77,		// Hind
    // TXT_ 	= 78,		// APC
    // TXT_ 	= 79,		// Guard Tower
    // TXT_ 	= 80,		// Radar Dome
    // TXT_ 	= 81,		// Helipad
    // TXT_ 	= 82,		// Airfield
    // TXT_ 	= 83,		// Ore Silo
    // TXT_ 	= 84,		// Construction Yard
    // TXT_ 	= 85,		// Ore Refinery
    // TXT_ 	= 86,		// Church
    // TXT_ 	= 87,		// Han's and Gretel's
    // TXT_ 	= 88,		// Hewitt's Manor
    // TXT_ 	= 89,		// Ricktor's House
    // TXT_ 	= 90,		// Gretchin's House
    // TXT_ 	= 91,		// The Barn
    // TXT_ 	= 92,		// Damon's pub
    // TXT_ 	= 93,		// Fran's House
    // TXT_ 	= 94,		// Music Factory
    // TXT_ 	= 95,		// Toymaker's
    // TXT_ 	= 96,		// Ludwig's House
    // TXT_ 	= 97,		// Haystacks
    // TXT_ 	= 98,		// Haystack
    // TXT_ 	= 99,		// Wheat Field
    // TXT_ 	= 100,		// Fallow Field
    // TXT_ 	= ,		// Corn Field
    // TXT_ 	= ,		// Celery Field
    // TXT_ 	= ,		// Potato Field
    // TXT_ 	= ,		// Sala's House
    // TXT_ 	= ,		// Abdul's House
    // TXT_ 	= ,		// Pablo's Wicked Pub
    // TXT_ 	= ,		// Village Well
    // TXT_ 	= ,		// Camel Trader
    // TXT_ 	= ,		// Church
    // TXT_ 	= ,		// Ali's House
    // TXT_ 	= ,		// Trader Ted's
    // TXT_ 	= ,		// Menelik's House
    // TXT_ 	= ,		// Prestor John's House
    // TXT_ 	= ,		// Village Well
    // TXT_ 	= ,		// Witch Doctor's Hut
    // TXT_ 	= ,		// Rikitikitembo's Hut
    // TXT_ 	= ,		// Roarke's Hut
    // TXT_ 	= ,		// Mubasa's Hut
    // TXT_ 	= ,		// Aksum's Hut
    // TXT_ 	= ,		// Mambo's Hut
    // TXT_ 	= ,		// The Studio
    // TXT_ 	= ,		// Technology Center
    // TXT_ 	= ,		// Turret
    // TXT_ 	= ,		// Gunboat
    // TXT_ 	= ,		// Mobile Construction Vehicle
    // TXT_ 	= ,		// Power Plant
    // TXT_ 	= ,		// Advanced Power Plant
    // TXT_ 	= ,		// Hospital
    // TXT_ 	= ,		// Barracks
    // TXT_ 	= ,		// Oil Pump
    // TXT_ 	= ,		// Oil Tanker
    // TXT_ 	= ,		// Sandbags
    // TXT_ 	= ,		// Chain Link Fence
    // TXT_ 	= ,		// Concrete Wall
    // TXT_ 	= ,		// Barbwire Fence
    // TXT_ 	= ,		// Wood Fence
    // TXT_ 	= ,		// War Factory
    // TXT_ 	= ,		// Advanced Guard Tower
    // TXT_ 	= ,		// Bio-Research Laboratory
    // TXT_ 	= ,		// Service Depot
    TXT_SIDEBAR = 141, // Sidebar
    TXT_OPTIONS = 142, // Options
    TXT_DATABASE = 143, // Database
    TXT_TTIP_UNREVEALED_TERRAIN = 144, // Unrevealed Terrain
    // TXT_ 	= 145,		// Options Menu
    TXT_STOP = 146, // Stop
    TXT_PLAY = 147, // Play
    TXT_SUFFLE = 148, // Shuffle
    TXT_REPEAT = 149, // Repeat
    // TXT_ 	= 150,		// Music volume:
    // TXT_ 	= 151,		// Sound volume:
    TXT_ON = 152, // On
    TXT_OFF = 153, // Off
    TXT_MULTIPLAYER_GAME = 154, // Multiplayer Game
    TXT_NO_FILES_AVAILABLE = 155, // No files available
    // TXT_ 	= 156,		// Do you want to delete this file?
    // TXT_ 	= 157,		// Do you want to delete %d files?
    // TXT_ 	= 158,		// Reset Values
    TXT_ASK_ABORT_MISSION = 159, // Do you want to abort the mission?
    TXT_MISSION_DESCRIPTION = 160, // Mission Description
    // TXT_ 	= ,		// Joe
    // TXT_ 	= ,		// Barry
    // TXT_ 	= ,		// Shelly
    // TXT_ 	= ,		// Maria
    // TXT_ 	= ,		// Karen
    // TXT_ 	= ,		// Steve
    // TXT_ 	= ,		// Phil
    // TXT_ 	= ,		// Dwight
    // TXT_ 	= ,		// Erik
    // TXT_ 	= 170,		// Prof. Einstein
    // TXT_ 	= ,		// Road Bib
    // TXT_ 	= ,		// Faster
    // TXT_ 	= ,		// Slower
    // TXT_ 	= ,		// Air Strike
    // TXT_ 	= ,		// Steel Crate
    // TXT_ 	= ,		// Wood Crate
    // TXT_ 	= ,		// Water Crate
    // TXT_ 	= ,		// Flag Location
    TXT_UNABLE_TO_READ_SCENARIO = 179, // Unable to read scenario!
    TXT_ERROR_LOADING_GAME = 180, // Error loading game!
    TXT_OBSOLETE_SAVED_GAME = 181, // Obsolete saved game.
    // TXT_ 	= 182,		// You must enter a description!
    TXT_ERROR_SAVING_GAME = 183, // Error saving game!
    TXT_DELETE_THIS_FILE = 184, // Delete this file?
    TXT_EMPTY_SLOT = 185, // [EMPTY SLOT]
    TXT_SELECT_MP_GAME = 186, // Select Multiplayer Game
    // TXT_ 	= ,		// Modem/Serial
    // TXT_ 	= ,		// Network
    // TXT_ 	= ,		// Unable to initialize network!
    // TXT_ 	= ,		// Join Network Game
    // TXT_ 	= ,		// New
    // TXT_ 	= ,		// Join
    // TXT_ 	= ,		// Send Message
    // TXT_ 	= ,		// Your Name:
    // TXT_ 	= ,		// Your Side:
    // TXT_ 	= ,		// Your Color:
    // TXT_ 	= ,		// Games
    // TXT_ 	= ,		// Players
    // TXT_ 	= ,		// Scenario:
    TXT_NOT_FOUND = 999, // >> NOT FOUND <<
    // TXT_ 	= ,		// Starting Credits:
    // TXT_ 	= ,		// Bases:
    // TXT_ 	= ,		// Ore:
    // TXT_ 	= ,		// Crates:
    // TXT_ 	= ,		// AI Players:
    TXT_REQ_DENIED = 999, // Request denied.
    // TXT_ 	= ,		// Unable to play; scenario not found.
    // TXT_ 	= ,		// Nothing to join!
    // TXT_ 	= ,		// You must enter a name!
    // TXT_ 	= ,		// Duplicate names are not allowed.
    // TXT_ 	= ,		// Your game version is outdated.
    // TXT_ 	= ,		// Destination game version is outdated.
    // TXT_ 	= ,		// %s's Game
    // TXT_ 	= ,		// [%s's Game]
    // TXT_ 	= ,		// Network Game Setup
    TXT_REJECT = 999, // Reject
    // TXT_ 	= ,		// You can't reject yourself!
    // TXT_ 	= ,		// You must select a player to reject.
    // TXT_ 	= ,		// Bases
    // TXT_ 	= ,		// Crates
    // TXT_ 	= ,		// AI Players
    // TXT_ 	= ,		// Scenarios
    // TXT_ 	= ,		// Credits:
    // TXT_ 	= ,		// Only one player?
    // TXT_ 	= ,		// Oops!
    TXT_MESSAGE_TO = 999, // To %s:
    TXT_MESSAGE_TO_ALL = 999, // To All:
    TXT_MESSAGE_TYPE = 999, // Message:
    // TXT_ 	= ,		// Connection to %s lost!
    // TXT_ 	= ,		// %s has left the game.
    // TXT_ 	= ,		// %s has been defeated!
    // TXT_ 	= ,		// Waiting to Connect...
    // TXT_ 	= ,		// Connection error! Check your cables. Attempting to Reconnect...
    // TXT_ 	= ,		// Connection error! Redialing...
    // TXT_ 	= ,		// Connection error! Waiting for Call...
    // TXT_ 	= ,		// Select Serial Game
    // TXT_ 	= ,		// Dial Modem
    // TXT_ 	= ,		// Answer Modem
    // TXT_ 	= ,		// Null Modem
    // TXT_ 	= ,		// Settings
    // TXT_ 	= ,		// Port:
    // TXT_ 	= ,		// IRQ:
    // TXT_ 	= ,		// Baud:
    // TXT_ 	= ,		// Init String:
    // TXT_ 	= ,		// Call Waiting String:
    // TXT_ 	= ,		// Tone Dialing
    // TXT_ 	= ,		// Pulse Dialing
    // TXT_ 	= ,		// Host Serial Game
    // TXT_ 	= ,		// Opponent:
    // TXT_ 	= ,		// User signed off!
    // TXT_ 	= ,		// Join Serial Game
    // TXT_ 	= ,		// Phone List
    // TXT_ 	= ,		// Add
    // TXT_ 	= ,		// Edit
    // TXT_ 	= ,		// Dial
    // TXT_ 	= ,		// Default
    // TXT_ 	= ,		// Default Settings
    // TXT_ 	= ,		// Custom Settings
    // TXT_ 	= ,		// Phone Listing
    TXT_NAME_COLON = 260, // Name:
    // TXT_ 	= ,		// Number:
    // TXT_ 	= ,		// Unable to find modem. Check power and cables.
    // TXT_ 	= ,		// No carrier.
    // TXT_ 	= ,		// Line busy.
    // TXT_ 	= ,		// Number invalid.
    // TXT_ 	= ,		// Other system not responding!
    // TXT_ 	= ,		// Games are out of sync!
    // TXT_ 	= ,		// Packet received too late!
    // TXT_ 	= ,		// Other player has left the game.
    // TXT_ 	= ,		// From %s:%s
    // TXT_ 	= ,		// TIME:
    // TXT_ 	= ,		// LEADERSHIP:
    // TXT_ 	= ,		// ECONOMY:
    // TXT_ 	= ,		// TOTAL SCORE:
    // TXT_ 	= ,		// CASUALTIES:
    // TXT_ 	= ,		// NEUTRAL:
    // TXT_ 	= ,		// BUILDINGS LOST
    // TXT_ 	= ,		// BUILDINGS
    // TXT_ 	= ,		// LOST:
    // TXT_ 	= ,		// TOP SCORES
    // TXT_ 	= ,		// ENDING CREDITS:
    // TXT_ 	= ,		// %dh %dm
    // TXT_ 	= ,		// %dm
    // TXT_ 	= ,		// Dialing...
    // TXT_ 	= ,		// Dialing Canceled
    // TXT_ 	= ,		// Waiting for Call...
    // TXT_ 	= ,		// Answering Canceled
    // TXT_ 	= ,		// Engineer
    // TXT_ 	= ,		// Spy
    // TXT_ 	= ,		// Not a Null Modem Cable Attached! It is a modem or loopback cable.
    // TXT_ 	= ,		// Map
    // TXT_ 	= ,		// Blossom Tree
    // TXT_ 	= ,		// Briefing
    TXT_COMPUTER = 294, // Computer
    // TXT_ 	= ,		// Unit Count:
    // TXT_ 	= ,		// Tech Level:
    // TXT_ 	= ,		// Opponent
    TXT_KILLS_COLON = 298, // Kills:
    // TXT_ 	= ,		// Video
    // TXT_ 	= ,		// Scientist
    // TXT_CAPTURE_THE_FLAG 	= ,		// Capture The Flag
    TXT_MISSION_OBJ = 302, // Mission Objective
    TXT_MISSION = 303, // Mission
    TXT_NO_SAVED_GAMES_AVAILABLE = 304, // No saved games available.
    TXT_CIVILIAN_BUILDING = 305, // Civilian Building
    // TXT_TECHNICIAN 	= 306,		// Technician
    // TXT_ 	= 307,		// Save game options are not allowed during a multiplayer session.
    // TXT_ 	= 308,		// Special 1
    // TXT_ 	= 309,		// Would you like to replay this mission?
    // TXT_ 	= 310,		// Reconnecting to %s.
    // TXT_ 	= 311,		// Please wait %02d seconds.
    TXT_WISH_TO_SURRENDER = 312, // Do you wish to surrender?
    // TXT_SELECT_TRANSMISSION 	= ,		// SELECT TRANSMISSION
    // TXT_ 	= ,		// Your game name must be unique.
    // TXT_ 	= ,		// Game is closed.
    // TXT_ 	= ,		// Your name must be unique.
    // TXT_ 	= ,		// Reconnecting to %s
    // TXT_ 	= ,		// Waiting for connections...
    // TXT_ 	= ,		// Time allowed: %02d seconds
    // TXT_ 	= ,		// Press ESC to cancel.
    // TXT_ 	= ,		// From Computer: It's just you and me now!
    // TXT_ 	= ,		// Capture the Flag:
    // TXT_ 	= ,		// Special 2
    TXT_HAS_ALLIED = 324, // %s has allied with %s
    TXT_AT_WAR = 325, // %s declares war on %s
    // TXT_ 	= ,		// Select a target
    TXT_RESIGN_GAME = 327, // Resign Game
    // TXT_ 	= ,		// Ore grows quickly.
    // TXT_ 	= ,		// Answering...
    // TXT_ 	= ,		// Initializing Modem...
    // TXT_ 	= ,		// Scenarios don't match.
    TXT_TTIP_POWER = 332, // Power Output
    TXT_TTIP_LOW_POWER = 333, // Power Output (low)
    TXT_CONTINUE = 999, // Continue
    // TXT_ 	= ,		// Data Queue Overflow
    // TXT_ 	= ,		// %s changed game options!
    // TXT_ 	= ,		// Please insert a Red Alert CD into the CD-ROM drive.
    // TXT_ 	= ,		// Please insert CD %d (%s) into the CD-ROM drive.
    // TXT_ 	= ,		// Red Alert is unable to detect your CD ROM drive.
    // TXT_ 	= ,		// No Sound Card Detected
    TXT_UNKNOWN = 999, // UNKNOWN
    // TXT_ 	= ,		// (old)
    // TXT_ 	= ,		// Insufficient Disk Space to run Red Alert.
    // TXT_ 	= ,		// You must have %d megabytes of free disk space.
    // TXT_ 	= ,		// Run SETUP program first.
    // TXT_ 	= ,		// Waiting for Opponent
    // TXT_ 	= ,		// Please select 'Settings' to setup default configuration
    // TXT_ 	= ,		// Prison
    TXT_MISSION_SAVED = 999, // Mission Saved
    // TXT_ 	= ,		// Insufficient disk space to save a game.  Please delete a previous save to free up some disk space and
    // try again.  TXT_ 	= ,		// Invalid Port/Address. COM 1-4 OR ADDRESS  TXT_ 	= ,		// Invalid Port and/or IRQ
    // settings  TXT_ 	= ,		// IRQ already in use
    TXT_ABORT = 999, // Abort
    TXT_RESTART = 999, // Restart
    // TXT_ 	= ,		// Mission is restarting. Please wait...
    // TXT_ 	= ,		// Mission is loading. Please wait...
    // TXT_ 	= ,		// Error in the InitString
    // TXT_ 	= ,		// Shroud:
    // TXT_ 	= ,		// Anti-Vehicle Mine
    // TXT_ 	= ,		// Anti-Personnel Mine
    // TXT_ 	= ,		// New Missions
    // TXT_ 	= ,		// Thief
    // TXT_ 	= ,		// Radar Jammer
    // TXT_ 	= ,		// Gap Generator
    // TXT_ 	= ,		// Pillbox
    // TXT_ 	= ,		// Camo. Pillbox
    // TXT_ 	= ,		// Chronosphere
    // TXT_ 	= ,		// England
    // TXT_ 	= ,		// Germany
    // TXT_ 	= ,		// Spain
    // TXT_ 	= ,		// Russia
    // TXT_ 	= ,		// Ukraine
    // TXT_ 	= ,		// Greece
    // TXT_ 	= ,		// France
    // TXT_ 	= ,		// Turkey
    // TXT_ 	= ,		// Shore
    // TXT_ 	= ,		// Select Object
    // TXT_ 	= ,		// Submarine
    // TXT_ 	= ,		// Destroyer
    // TXT_ 	= ,		// Cruiser
    // TXT_ 	= ,		// Transport
    // TXT_ 	= ,		// Gun Boat
    // TXT_ 	= ,		// Lobby
    // TXT_ 	= ,		// Games
    // TXT_ 	= ,		// Save Game...
    // TXT_ 	= ,		// Game is full.
    // TXT_ 	= ,		// You must select a game!
    // TXT_ 	= ,		// %s playing %s
    // TXT_ 	= ,		// Only the host can modify this option.
    // TXT_ 	= ,		// Game was cancelled.
    // TXT_ 	= ,		// %s has formed a new game.
    // TXT_ 	= ,		// %s's game is now in progress.
    // TXT_ 	= ,		// Tesla Coil
    // TXT_ 	= ,		// Mobile Gap Generator
    // TXT_ 	= ,		// Flame Tower
    // TXT_ 	= ,		// AA Gun
    // TXT_ 	= ,		// Kennel
    // TXT_ 	= ,		// Soviet Tech Center
    // TXT_ 	= ,		// Badger Bomber
    // TXT_ 	= ,		// Mig Attack Plane
    // TXT_ 	= ,		// Yak Attack Plane
    // TXT_ 	= ,		// Barbed Wire
    // TXT_ 	= ,		// Field Medic
    // TXT_ 	= ,		// Saboteur
    // TXT_ 	= ,		// General
    // TXT_ 	= ,		// Tanya
    // TXT_ 	= ,		// Parabombs
    // TXT_ 	= ,		// Paratroopers
    // TXT_ 	= ,		// Parachute Saboteur
    // TXT_ 	= ,		// Naval Yard
    // TXT_ 	= ,		// Sub Pen
    // TXT_ 	= ,		// Scenario Options
    // TXT_ 	= ,		// Spy Plane
    // TXT_ 	= ,		// Spy Plane
    // TXT_ 	= ,		// Attack Dog
    TXT_SPY_INFO = 417, // Spy Info
    TXT_INFO_BUILDINGS = 418, // Buildings
    TXT_INFO_UNITS = 419, // Units
    TXT_INFO_INFANTRY = 420, // Infantry
    TXT_INFO_AIRCRAFT = 421, // Aircraft
    // TXT_ 	= ,		// Supply Truck
    // TXT_ 	= ,		// Invulnerability Device
    // TXT_ 	= ,		// Iron Curtain
    // TXT_ 	= ,		// Allied Tech Center
    // TXT_ 	= ,		// V2 Rocket
    // TXT_ 	= ,		// Forward Command Post
    // TXT_ 	= ,		// Demolitioner
    // TXT_ 	= ,		// Mine Layer
    // TXT_ 	= ,		// Fake Construction Yard
    // TXT_ 	= ,		// Fake War Factory
    // TXT_ 	= ,		// Fake Naval Yard
    // TXT_ 	= ,		// Fake Sub Pen
    // TXT_ 	= ,		// Fake Radar Dome
    // TXT_TRACK_ 	= ,		// Bigfoot
    // TXT_TRACK_ 	= ,		// Crush
    // TXT_TRACK_ 	= ,		// Face the Enemy 1
    // TXT_TRACK_ 	= ,		// Face the Enemy 2
    // TXT_TRACK_ 	= ,		// Hell March
    // TXT_TRACK_ 	= ,		// Run for Your Life
    // TXT_TRACK_ 	= ,		// Smash
    // TXT_TRACK_ 	= ,		// Trenches
    // TXT_TRACK_ 	= ,		// Workmen
    // TXT_TRACK_ 	= ,		// Await
    // TXT_TRACK_ 	= ,		// Dense
    // TXT_TRACK_ 	= ,		// Map Selection
    // TXT_TRACK_ 	= ,		// Fogger
    // TXT_TRACK_ 	= ,		// Mud
    // TXT_TRACK_ 	= ,		// Radio 2
    // TXT_TRACK_ 	= ,		// Roll Out
    // TXT_TRACK_ 	= ,		// Snake
    // TXT_TRACK_ 	= ,		// Terminate
    // TXT_TRACK_ 	= ,		// Twin
    // TXT_TRACK_ 	= ,		// Vector
    // TXT_ 	= ,		// Team Members
    // TXT_ 	= ,		// Bridge
    // TXT_ 	= ,		// Barrel
    // TXT_ 	= ,		// Friendly
    // TXT_ 	= ,		// Enemy
    // TXT_ 	= ,		// Gold
    // TXT_ 	= ,		// Gems
    // TXT_ 	= ,		// Title Movie
    TXT_MOVIES = 463, // Movies
    // TXT_ 	= 464,		// Interior
    // TXT_ 	= 465,		// Sonar Pulse
    // TXT_ 	= 466,		// Missile Silo
    // TXT_ 	= 467,		// GPS Satellite
    // TXT_ 	= 468,		// Atom Bomb
    TXT_EASY = 469, // Easy
    TXT_HARD = 470, // Hard
    TXT_NORMAL = 471, // Normal
    TXT_SET_DIFFICULTY = 472,		// Please set the difficulty level. It will be used throughout this campaign.
    TXT_ALLIES = 473, // Allies
    TXT_SOVIET = 474, // Soviet
    // TXT_ 	= ,		// Intro Theme
    // TXT_ 	= ,		// Shroud Regrows
    // TXT_ 	= ,		// Ore Regenerates
    // TXT_ 	= ,		// Score Theme
    // TXT_ 	= ,		// Internet Game
    // TXT_ 	= ,		// Ice
    // TXT_ 	= ,		// Crates
    // TXT_ 	= ,		// Skirmish
    // TXT_ 	= ,		// Choose your side.
    TXT_VALUABLE_MINERALS = 484, // Valuable Minerals
    // TXT_ 	= ,		// Ignore
    // TXT_ 	= ,		// Error - modem is not responding.
    // TXT_ 	= ,		// Error - modem did not respond to result code enable command.
    // TXT_ 	= ,		// Error - modem did not respond to initialisation string.
    // TXT_ 	= ,		// Error - modem did not respond to 'verbose' command.
    // TXT_ 	= ,		// Error - modem did not respond to 'echo' command.
    // TXT_ 	= ,		// Error - unable to disable modem auto answer.
    // TXT_ 	= ,		// Error - Too many errors initialising modem - Aborting.
    // TXT_ 	= ,		// Error - Modem returned error status.
    // TXT_ 	= ,		// Error - TIme out waiting for connect.
    // TXT_ 	= ,		// Accomplished
    // TXT_ 	= ,		// Click to Continue
    // TXT_ 	= ,		// Receiving scenario from host.
    // TXT_ 	= ,		// Sending scenario to remote players.
    // TXT_ 	= ,		// Error - Modem failed to respond to flow control command. Your Windows configuration may be
    // incorrect.  TXT_ 	= ,		// Error - Modem failed to respond to compression command. Your Windows configuration may
    // be incorrect.  TXT_ 	= ,		// Error - Modem failed to respond to error correction command. Your Windows
    // configuration may be incorrect.  TXT_ 	= ,		// To play Red Alert via the internet you must be connected to an
    // internet services provider and be registered with Planet Westwood  TXT_ 	= ,		// Wchat not installed. Please
    // install it from either CD.  TXT_ 	= ,		// Register  TXT_ 	= ,		// Ore Mine  TXT_ 	= ,		// No registered
    // modem  TXT_ 	= ,		// Chronoshift  TXT_ 	= ,		// Invalid Port or Port is in use  TXT_ 	= ,		// No dial tone.
    // Ensure your modem is connected to the phone line and try again.  TXT_ 	= ,		// Error - other player does not have
    // this expansion scenario.
    TXT_PLEASESTANDBY = 511, // Please Stand By...
    // TXT_ 	= 512,		// End Credits Theme
    TXT_AA_GUNS_OFFLINE = 513, // Low Power: AA-Guns offline
    TXT_TESLA_COILS_OFFLINE = 514, // Low Power: Tesla Coils offline
    TXT_LOWER_POWER = 515, // Low Power
    // TXT_ 	= 516,		// Commander:
    // TXT_ 	= 517,		// Battles Won:
    // TXT_ 	= 518,		// Game versions incompatible. To make sure you have the latest version, visit www.westwood.com
    // TXT_ 	= 519,		// Incompatible scenario file detected. The scenario may be corrupt.
    // TXT_ 	= 520,		// Connecting...
    // TXT_ 	= 521,		// Modem Initialization
    // TXT_ 	= 522,		// Data Compression
    // TXT_ 	= 523,		// Error Correction
    // TXT_ 	= 524,		// Hardware Flow Control
    // TXT_ 	= 525,		// Advanced
    // TXT_SONG_ 	= 526,		// 2nd_Hand
    // TXT_SONG_ 	= 527,		// Arazoid
    // TXT_SONG_ 	= 528,		// BackStab
    // TXT_SONG_ 	= 529,		// Chaos2
    // TXT_SONG_ 	= 530,		// Shut_It
    // TXT_SONG_ 	= 531,		// TwinMix1
    // TXT_SONG_ 	= 532,		// Under3
    TXT_SONG_VR2 = 533, // VR2
    // TXT_ 	= ,		// The other system is not responding. Do you wish to attempt an emergency game save? Both players must
    // save for this to work.  TXT_ 	= ,		// The other system hung up. Do you wish to attempt an emergency game save?
    // Both players must save for this to work.
    // TXT_ 	= ,		// Red Alert was unable to run the registration software. You need to install Westwood Chat from the Red
    // Alert CD to register.  TXT_ 	= ,		// A player in the game does not have this expansion scenario.
    TXT_NAME_MISSILE_SUB = 538, // Missile Sub
    TXT_NAME_SHOCK_TROOPER = 539, // Shock Trooper
    TXT_NAME_MECHANIC = 540, // Mechanic
    TXT_NAME_CHRONO_TANK = 541, // Chrono Tank
    // TXT_ 	= ,		// Tesla Tank
    // TXT_ 	= ,		// M.A.D. Tank
    // TXT_ 	= ,		// Demolition Truck
    // TXT_ 	= ,		// Phase Transport
    // TXT_SONG_ 	= ,		// Bog
    // TXT_SONG_ 	= ,		// Floating
    // TXT_SONG_ 	= ,		// Gloom
    // TXT_SONG_ 	= ,		// Ground Wire
    // TXT_SONG_ 	= ,		// Mech Man 2
    TXT_SONG_SEARCH = 551, // Search
    // TXT_SONG_ 	= ,		// Traction
    // TXT_SONG_ 	= ,		// Wasteland
    // TXT_ 	= ,		// Helicarrier

    TXT_LAST = 554,

    TXT_COUNT = 555,

    //
    //
    //
    TXT_DEBUG_MAGIC_NUM = 1000, //

    TXT_DEBUG_FIRST = 1000, //

    // TXT_DEBUG_	= ,	//

    TXT_DEBUG_COUNT = 9999,

    TXT_EDITOR_MAGIC_NUM = 2000,

    TXT_EDITOR_FIRST = 2000,

    TXT_EDITOR_NEW_MAP = 2000, // New Map
    TXT_EDITOR_LOAD_MAP = 2001, // Load Map
    TXT_EDITOR_SAVE_MAP = 2002, // Save Map
    TXT_EDITOR_MODIFY_MAP = 2003, // Modify Map Info
    TXT_EDITOR_RESUME_EDITING = 2004, // Resume Editing
    TXT_EDITOR_QUIT_EDITOR = 2005, // Quit Editor
    // TXT_EDITOR_	= 2006,		// You have not saved your changes!  Load Another file?
    // TXT_EDITOR_	= 2007,		// You have not saved your changes!  Exit Scenario Editor?
    TXT_EDITOR_EXIT_EDITOR = 2008, // Exit Scenario Editor?
    TXT_EDITOR_64_BY_64 = 2009, // 64 x 64
    TXT_EDITOR_96_BY_96 = 2010, // 96 x 96
    TXT_EDITOR_126_BY_64 = 2011, // 126 x 64
    TXT_EDITOR_64_BY_126 = 2012, // 64 x 126
    TXT_EDITOR_TEMPERATE = 2013, // Temperate
    TXT_EDITOR_SNOW = 2014, // Snow
    TXT_EDITOR_RIDGES = 2015, // Ridges
    TXT_EDITOR_TREES = 2016, // Trees
    TXT_EDITOR_DEBRIS = 2017, // Debris
    TXT_EDITOR_UNABLE_TO_READ = 2018, // Unable to read scenario!
    TXT_EDITOR_SCENARIO_LOADED = 2019, // Scenario Loaded
    TXT_EDITOR_SCENARIO_SAVED = 2020, // Scenario Saved.
    TXT_EDITOR_OBSOLETE_SCENARIO = 2021, // Obsolete Scenario.
    TXT_EDITOR_ASK_REPLACE = 2022, // Do you wish to replace the existing scenario named %s?
    TXT_EDITOR_LOW_DISK_SPACE = 2023, // Insufficient disk space to save scenario.  Please delete a previous scenario to free
                                      // up some disk space and try again.
    TXT_EDITOR_SELECT_TILE = 2024, // Select Tile
    TXT_EDITOR_ASK_UNSAVED_NEW = 2025, // You have not saved your changes!  Create a new map?
    TXT_EDITOR_DELETE_MAP = 2026, // Delete Map
    TXT_EDITOR_ASK_DELETE = 2027, // Are you sure you wish to delete this scenario?
    TXT_EDITOR_MAP_DESCRIPTION = 2025, // Map Description
    TXT_EDITOR_PASSABLE = 2029, // Passable
    TXT_EDITOR_CONFIRMATION = 2030, // Confirmation
    TXT_EDITOR_ASK_RESIZE = 2031, // Portions of the map that don't fit on the new size will be deleted!  Resize the map?
    TXT_EDITOR_SCROLL_RATE = 2032, // Change Scroll Rate
    TXT_EDITOR_LOADING_MAP = 2033, // Loading Map
    TXT_EDITOR_NO_MORE_AVAILABLE = 2034, // No more objects of this type are available.
    TXT_EDITOR_FAILED_INIT_TIMER = 2035, // Unable to to initialize system timer.  Please restart windows.
    TXT_EDITOR_STARTUP_ERROR = 2036, // Startup Error
    TXT_EDITOR_TILESET_PAGE = 2037, // Page %d of %d
    TXT_EDITOR_CURRENT_MAP = 2038, // Current Map: %s
    TXT_EDITOR_CURRENT_MAP_UNTITLED = 2039, // Current Map: Untitled

    TXT_EDITOR_COUNT = 40,

    //TXT_FORCE_INT = 0x7FFFFFFF,
};

enum LanguageType
{
    LANGUAGE_NONE = -1,
    LANGUAGE_ENGLISH,
    LANGUAGE_FRENCH,
    LANGUAGE_GERMAN,
    LANGUAGE_ITALIAN,
    LANGUAGE_SPANISH,
    LANGUAGE_COUNT,
};

extern LanguageType Language;
extern const char *MissionStr[];
extern const char TXT_CS_MISSIONS[];
extern const char TXT_AM_MISSIONS[];
extern const char TXT_MORE_1[];
extern const char TXT_MORE_2[];
extern const char TXT_CONFIRM_QUIT[];

bool Init_Language();
const char *Language_Name(const char *filename);
const char *Get_Language_Char();
const char *Fetch_String(int str_id);
const char *Fetch_Mission_String(int str_id);

#endif // LANGUAGE_H
