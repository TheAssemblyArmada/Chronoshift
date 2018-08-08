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
#include "language.h"
#include "abs.h"
#include "ccfileclass.h"
#include "dipthong.h"
#include "gamedebug.h"
#include "minmax.h"
#include "mixfile.h"
#include <cstdio>

using std::snprintf;

#ifndef CHRONOSHIFT_STANDALONE
char *&GameStrings = Make_Global<char *>(0x0066991C);
char *&DebugStrings = Make_Global<char *>(0x00669920);
char **NameOverride = reinterpret_cast<char **>(0x0066616C);
int *NameIDOverride = reinterpret_cast<int *>(0x006661D0);
#else
char *GameStrings = nullptr;
char *DebugStrings = nullptr;
char *NameOverride[25];
int NameIDOverride[25];
#endif

char *EditorStrings = nullptr;
LanguageType Language = LANGUAGE_ENGLISH;

const char *MissionStr[] = { "Coastal Influence (Med)",
    "Middle Mayhem (Sm)",
    "Equal Opportunity (Sm)",
    "Marooned II (Med)",
    "Keep off the Grass (Sm)",
    "Isle of Fury (Lg)",
    "Ivory Wastelands (Sm)",
    "Shallow Grave (Med)",
    "North By Northwest (Lg)",
    "First Come, First Serve (Sm)",
    "Island Hoppers (Sm)",
    "Raraku (Lg)",
    "Central Conflict (Lg)",
    "Combat Alley (Med)",
    "Island Wars (Lg)",
    "Desolation (Lg)",
    "No Escape (Med)",
    "No Man's Land (Med)",
    "Normandy (Med)",
    "Pond Skirmish (Med)",
    "Ridge War (Med)",
    "A Path Beyond (Lg)",
    "Dugout Isle (Med)",
    "Treasure Isle (Med)",
    "Africa (Lg)",
    "Alaska Anarchy (Lg)",
    "All that Glitters... (Lg)",
    "Apre's Peace (Lg)",
    "Antartica (Lg)",
    "Armourgarden (Lg)",
    "Austraila (Med)",
    "Barrier to Entry (Lg)",
    "Bavarian Blast (Med)",
    "Be Shore (Med)",
    "Bearing Straits (Med)",
    "Blow Holes (Lg)",
    "Bonsai (Sm)",
    "Brother Stalin (Lg)",
    "Bullseye (Lg)",
    "C&C (Med)",
    "Camos Canyon (Med)",
    "Camos Coves (Lg)",
    "Camos Cross (Lg)",
    "Camos Crossing (Sm)",
    "Central Arena (Lg)",
    "Canyon River (Med)",
    "Crossroads (Sm)",
    "Czech Mate (Lg)",
    "Dday (Med)",
    "Disaster Central (Lg)",
    "Docklands (Med)",
    "East Coast (Med)",
    "Eastern Seaboard (Lg)",
    "Finger Lake (Lg)",
    "Fjords (Med)",
    "Floodlands (Lg)",
    "Forest under fire (Lg)",
    "Four Corners (Lg)",
    "Frostbit Fjords (Lg)",
    "Glenboig (Sm)",
    "Hell Frozen Over (Lg)",
    "India (Lg)",
    "Indirect Fire (Lg)",
    "Island Wars II (Lg)",
    "Italy (Lg)",
    "Kabalo (Lg)",
    "King of the Hills (Lg)",
    "Lake Divide (Med)",
    "Lakelands (Lg)",
    "Land Ladder (Lg)",
    "Lotsa Lakes (Lg)",
    "Lunar Battlefield (Lg Special)",
    "Malibu Fields (Med)",
    "Marshland (Med)",
    "MyLai Delta (Med)",
    "Natural Harbor (Med)",
    "No Way Out (Lg)",
    "Normandy Landing (Lg)",
    "Ore Wars (Med)",
    "Oz (Lg)",
    "Pilgrim Fathers II (Lg)",
    "Pip's Ice Tea (Med)",
    "Polar Panic (Lg)",
    "Ponds (Med)",
    "Putney (Lg)",
    "Return to Zion (Lg)",
    "Ring of Land (Lg)",
    "River Basin (Lg)",
    "River Delta (Med)",
    "River Islands (Med)",
    "River Maze (Sm)",
    "Rivers (Sm)",
    "Run the Gauntlet (Med)",
    "Scappa Flow (Lg)",
    "Siberian Slaughter (Lg)",
    "Sleepy Valley (Sm)",
    "Snake River (Lg)",
    "Snow Wars (Lg)",
    "Snowball fight (Lg)",
    "Snowy Island (Lg)",
    "So Near So Far (Sm)",
    "South America (Lg)",
    "Spring Line (Lg)",
    "Star (Lg)",
    "Straighter & Narrower (Sm)",
    "TerrainSpotting (Sm)",
    "The Bay (Lg)",
    "The Garden (Lg)",
    "The Great Lakes (Med)",
    "The Ice Arena (Lg)",
    "The Lake District (Lg)",
    "The Linked lands (Lg)",
    "The Mississippi (Med)",
    "The Sticky Bit (Lg)",
    "The Valley (Med)",
    "The Woods Today (Lg)",
    "Things to Come (Lg)",
    "Tiger Core (Sm)",
    "To the Core (Sm)",
    "Tournament Hills (Lg)",
    "Tropical Storm (Med)",
    "Tundra Trouble (Lg)",
    "Uk (Med)",
    "Undiscovered Country (Sm)",
    "United States (Med)",
    "Volcano (Sm)",
    "Wastelands (Lg)",
    "Water Works (Sm)",
    "World Map (Med)",
    "Zambezi (Lg)",
    "A Pattern of Islands (Lg 8 players)",
    "Arena Valley Extreme (Mega 8 players)",
    "Around the Rim (Sm 4 players)",
    "Ashes to Ashes (Lg 6 players)",
    "Artic Wasteland (Mega 8 players)",
    "Badajoz (Med 4 players)",
    "Baptism of Fire (Lg 6 players)",
    "Big Fish, Small Pond (Lg 6 players)",
    "Blue Lakes (Lg 8 players)",
    "Booby Traps (Mega 8 players)",
    "Bridgehead (Lg 6 players)",
    "Butterfly Bay (Lg 6 players)",
    "Central Conflict Extreme (Mega 8 players)",
    "Circles of Death (Mega 8 players)",
    "Cold Front (Med 6 players)",
    "Cold Pass (Med 4 players)",
    "Combat Zones (Mega 8 players)",
    "Conflict Cove (Sm 4 players)",
    "Culloden Moor (Med 8 players)",
    "Damnation Alley (Mega 8 players)",
    "Death Valley (Mega 8 players)",
    "Deep Six (Mega 8 players)",
    "Destruction Derby (Mega 8 players)",
    "Diamonds Aren't Forever (Mega 8 players)",
    "Elysium (Sm 4 players)",
    "Equal Shares (Lg 4 players)",
    "Frost Bitten (Mega 8 players)",
    "Frozen Valley (Med 6 players)",
    "Gettysburg (Sm 4 players)",
    "Glacial Valley (Sm 4 players)",
    "Gold Coast (Med 6 players)",
    "Gold Rush (Lg 4 players)",
    "Habitat (Lg 4 players)",
    "Hades Frozen Over (Sm 4 players)",
    "Hamburger Hill (Mega 8 players)",
    "Hastings (Sm 4 players)",
    "Hell's Pass (Med 6 players)",
    "Holy Grounds (Mega 8 players)",
    "Ice Bergs (Med 6 players)",
    "Ice Station (Lg 6 players)",
    "Ice Queen (Lg 4 players)",
    "In the Sun (Med 6 players)",
    "Innocents? (Mega 8 players)",
    "Islands (Med 8 players)",
    "Island Plateau (Lg 4 players)",
    "Island Wars Extreme (Mega 8 players)",
    "Kananga (Med 6 players)",
    "King of the Hills Extreme (Mega 8 players)",
    "Lake Land (Lg 8 players)",
    "Land Locked (Lg 8 players)",
    "Lanes (Med 8 players)",
    "Leipzip (Sm 4 players)",
    "Meander (Lg 8 players)",
    "Mekong (Med 8 players)",
    "Middle Ground (Med 8 players)",
    "Naval Conquests (Mega 8 players)",
    "On your Marks (Med 4 players)",
    "Open Warfare (Mega 8 players)",
    "Ore Gardens (Lg 8 players)",
    "Potholes (Mega 8 players)",
    "Puddles (Med 4 players)",
    "Random Violence (Mega 8 players)",
    "Revenge (Med 8 players)",
    "Rias (Med 8 players)",
    "River Crossing (Sm 4 players)",
    "River Rampage (Mega 8 players)",
    "River Rapids (Lg 6 players)",
    "Rivers Wild (Mega 8 players)",
    "Rorkes Drift (Lg 4 players)",
    "Seaside (Med 4 players)",
    "Shades (Med 8 players)",
    "Smuggler's Cove (Lg 6 players)",
    "Snow Garden (Sm 2 players)",
    "Stalingrad (Sm 4 players)",
    "Sticks & Stones (Med 4 players)",
    "Strathearn Valley (Lg 6 players)",
    "Super Bridgehead (Mega 8 players)",
    "Super Mekong (Mega 8 players)",
    "Super Ore Gardens (Mega 8 players)",
    "Switch (Med 4 players)",
    "The Berg (Mega 8 players)",
    "The Boyne (Med 4 players)",
    "The Bulge (Sm 4 players)",
    "The Cauldron (Lg 6 players)",
    "The Finger (Lg 6 players)",
    "The Hills Have Eyes (Mega 8 players)",
    "The Keyes (Med 6 players)",
    "The Lakes (Med 8 players)",
    "The Neck (Med 6 players)",
    "The Web (Lg 6 players)",
    "To the Core (Lg 4 players)",
    "Trafalgar (Lg 4 players)",
    "Twin Rivers (Sm 4 players)",
    "Umtumbo Gorge (Lg 4 players)",
    "Watch Your Step Extreme (Mega 8 players)",
    "Waterfalls (Lg 8 players)",
    "Waterloo Revisited (Lg 6 players)",
    "Water Werks (Mega 8 players)",
    "Warlord's Lake (Sm 4 players)",
    "Zama (Sm 4 players)" };

const char TXT_CS_MISSIONS[] = { "Counterstrike Missions" };
const char TXT_AM_MISSIONS[] = { "Aftermath Missions" };
const char TXT_MORE_1[] = { "--More--" };
const char TXT_MORE_2[] = { "<MORE>" };
const char TXT_CONFIRM_QUIT[] = { "Are you sure you want to quit RedAlert++?" };

bool Init_Language(void)
{
    // Load System string table
    DEBUG_LOG("Bootstrap() - Loading Game Strings...\n");
    CCFileClass gamestr_file;
    gamestr_file.Set_Name(Language_Name("CONQUER"));

    if (gamestr_file.Is_Available()) {
        GameStrings = (char *)MixFileClass<CCFileClass>::Retrieve(Language_Name("CONQUER"));
    } else {
        DEBUG_LOG("Failed to find string file %s.\n", Language_Name("CONQUER"));
        GameStrings = nullptr;
    }

    gamestr_file.Close();

    // Load Editor string table
    DEBUG_LOG("Bootstrap() - Loading Editor Strings...\n");
    CCFileClass edstr_file;
    edstr_file.Set_Name(Language_Name("EDITOR"));

    if (edstr_file.Is_Available()) {
        EditorStrings = (char *)MixFileClass<CCFileClass>::Retrieve(Language_Name("EDITOR"));
    } else {
        DEBUG_LOG("Failed to find string file %s.\n", Language_Name("EDITOR"));
        EditorStrings = nullptr;
    }
    edstr_file.Close();

    // Load Debug string table
#if defined(RAPP_DEBUG)
    DEBUG_LOG("Bootstrap() - Loading Debug Strings...\n");
    CCFileClass dbgstr_file;
    dbgstr_file.Set_Name(Language_Name("DEBUG"));
    if (dbgstr_file.Is_Available()) {
        DebugStrings = (char *)MixFileClass<CCFileClass>::Retrieve(Language_Name("DEBUG"));
    } else {
        DEBUG_LOG("Failed to find string file %s.\n", Language_Name("DEBUG"));
        DebugStrings = nullptr;
    }
    dbgstr_file.Close();
#endif // RAPP_DEBUG

    return GameStrings != nullptr;
}

const char *Language_Name(const char *filename)
{
    static char _fullname[32];

    DEBUG_ASSERT(filename != nullptr);
    DEBUG_ASSERT(Language != LANGUAGE_NONE);

    if (filename != nullptr) {
        memset(_fullname, 0, sizeof(_fullname));

        switch (Language) {
            default: // Fallthrough.
            case LANGUAGE_ENGLISH:
                snprintf(_fullname, sizeof(_fullname), "%s.ENG", filename);
                break;

            case LANGUAGE_FRENCH:
                snprintf(_fullname, sizeof(_fullname), "%s.FRE", filename);
                break;

            case LANGUAGE_GERMAN:
                snprintf(_fullname, sizeof(_fullname), "%s.GER", filename);
                break;

            case LANGUAGE_SPANISH:
                snprintf(_fullname, sizeof(_fullname), "%s.SPN", filename);
                break;
        }

        return _fullname;
    }

    return nullptr;
}

const char *Get_Language_Char(void)
{
    static char _char[2];

    DEBUG_ASSERT(Language != LANGUAGE_NONE);

    _char[0] = '\0';

    switch (Language) {
        default: // Fallthrough.
        case LANGUAGE_ENGLISH:
            strcpy(_char, "E");
            break;

        case LANGUAGE_FRENCH:
            strcpy(_char, "F");
            break;

        case LANGUAGE_GERMAN:
            strcpy(_char, "G");
            break;

        case LANGUAGE_SPANISH:
            strcpy(_char, "S");
            break;
    }

    return _char;
}

const char *Fetch_String(int str_id)
{
    if (str_id >= TXT_EDITOR_FIRST) {
        str_id -= TXT_EDITOR_MAGIC_NUM; // ground the value to zero based
        DEBUG_ASSERT(str_id < (TXT_EDITOR_FIRST + TXT_EDITOR_COUNT));
        DEBUG_ASSERT(EditorStrings != nullptr);
        return Extract_String(EditorStrings, str_id);
    }

    if (str_id >= TXT_DEBUG_FIRST) {
        str_id -= TXT_DEBUG_MAGIC_NUM; // ground the value to zero based
        DEBUG_ASSERT(str_id < (TXT_DEBUG_FIRST + TXT_DEBUG_COUNT));
        DEBUG_ASSERT(DebugStrings != nullptr);
        return Extract_String(DebugStrings, str_id);
    }

    if (str_id < TXT_FIRST && Abs(str_id) < 25) {
        return NameOverride[-(str_id + 1)];
    }

    DEBUG_ASSERT(str_id < TXT_COUNT);
    DEBUG_ASSERT(GameStrings != nullptr);
    return Extract_String(GameStrings, str_id);
}

const char *Fetch_Mission_String(int str_id)
{
    return nullptr;
}
