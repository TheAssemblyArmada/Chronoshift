/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class holding information regarding the current game session.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef SESSION_H
#define SESSION_H

#include "always.h"
#include "abstract.h"
#include "ccfileclass.h"
#include "ipxaddress.h"
#include "msglist.h"
#include "remap.h"
#include "rtti.h"
#include "vector.h"
#include "version.h" // Current place for CommProtocolEnum but will likely move.

enum GameEnum
{
    GAME_NONE = -1,
    GAME_CAMPAIGN,
    GAME_1,
    GAME_2,
    GAME_3, // think this is LAN/NETWORK
    GAME_INTERNET,
    GAME_SKIRMISH,
    GAME_6,
    GAME_7,
    GAME_SOLE_SURVIVOR, // TODO, add SS code to allow RA to have a Sole Survivor mode
    GAME_COUNT
};

struct MultiMission;

// Temp, this contains a union of a number of different structs.
struct GlobalPacket
{
    char pad[0x8B];
};

struct MPlayerScoreType
{
    char Name[12];
    int field_0D;
    int Lost;
    int Kills;
    int Economy;
    int Score;
    PlayerColorType Scheme;
};

struct NodeNameTag
{
    char Name[12];
    IPXAddressClass Address;
    HousesType House; // HousesType?
    PlayerColorType Color; // looks like a PlayerColorType?
    HousesType AltHouse; // HousesType?
};

class SessionClass
{
public:
    SessionClass();

    MessageListClass &Get_Messages() { return Messages; }

private:
    GameEnum GameToPlay;
    CommProtocolEnum PacketProtocol;
    int MPlayerLocalID;
    BOOL MPlayerBases;
    int MPlayerCredits;
    BOOL MPlayerOre;
    BOOL MPlayerGoodies;
    BOOL MPlayerGhosts;
    int MPlayerUnitCount;
    int MPlayerAIPlayers;
    char MPlayerScenarioName[44];
    int UniqueID;
    char MPlayerName[12];
    PlayerColorType MPlayerPrefColor;
    PlayerColorType MPlayerColorIdx;
    HousesType MPlayerHouse; // Should be HouseType?
    BOOL MPlayerObiWan;
    BOOL MPlayerBlitz;
    int MPlayerMax;
    int MPlayerCount;
    int MaxAhead;
    int FrameSendRate;
    int DesiredFrameRate;
    int ProcessingStartTick;
    int ProcessingTicks;
    int ProcessingFrames;
    BOOL LoadGame;
    BOOL SaveGame;
    DynamicVectorClass<MultiMission *> MPlayerScenarios;
    char SendScenarioName[513];
    char SendScenarioDigest[32];
    int SendScenarioSize;
    BOOL SendScenarioIsOfficial;
    char UnkArray[20];
    int UnkArrayPos;
    IPXAddressClass UnkIPXAddress;
    MessageListClass Messages;
    IPXAddressClass MessageAddress;
    char LastMessage[124];
    MPlayerScoreType MPlayerScores[8];
    int MPlayerGamesPlayed;
    int MPlayerNumScores;
    int MPlayerWinner;
    int MPlayerCurrentGame;
    CCFileClass RecordFile;
#ifndef RAPP_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool RecordGame : 1; // & 1
            bool PlaybackGame : 1; // & 2
            bool AllowAttract : 1; // & 4
        };
        int m_sessionFlags;
    };
#else
    bool RecordGame;
    bool PlaybackGame;
    bool AllowAttract;

#endif
    BOOL IsBridge;
    IPXAddressClass BridgeNetwork;
    BOOL NetStealth;
    BOOL NetProtect;
    BOOL NetOpen;
    char MPlayerGameName[12];
    GlobalPacket GPacket;
    int GPacketLen;
    IPXAddressClass GAddress;
    int16_t GProductID;
    char MetaPacket[520];
    int MetaSize;
    DynamicVectorClass<NodeNameTag *> Games;
    DynamicVectorClass<NodeNameTag *> Players;
    DynamicVectorClass<NodeNameTag *> NetworkPlayers;
    int UnkInt;
    int PhoneIndex;
    int Port;
    int IRQ;
    int Baud;
    char DialMethod;
    int InitStringIndex;
    int CallWaitStringIndex;
    char CallWaitString[16];
    BOOL Compression;
    BOOL ErrorCorrection;
    BOOL HardwareFlowControl;
    char ModemName[63];
    char ModemGameToPlay;
    DynamicVectorClass<char *> PhoneBookEntries;
    DynamicVectorClass<char *> InitStrings;
    int TrapFrame;
    RTTIType TrapType; // Suspect this is RTTIType but needs confirming.
    int TrapTrappedObject;
    uint32_t TrapCoord;
    uint32_t TrapTarget;
    uint32_t TrapCell;
    BOOL TrapCheckHeap;
    BOOL TrapPrintCRC;
};

#ifndef RAPP_STANDALONE
#include "hooker.h"
extern SessionClass &Session;
#else
extern SessionClass Session;
#endif

#endif // SESSION_H
