/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 * @author tomsons26
 *
 * @brief Class holding information regarding the current game session.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
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
#include "gamefile.h"
#include "ipxaddress.h"
#include "msglist.h"
#include "remap.h"
#include "rtti.h"
#include "vector.h"
#include "version.h" // Current place for CommProtocolEnum but will likely move.

#define MAX_PROCESSING_TICKS 1000

class CellClass;
class GameFileClass;
class Pipe;
class Straw;

enum GameEnum
{
    GAME_NONE = -1,
    GAME_CAMPAIGN,
    GAME_1,
    GAME_2,
    GAME_IPX,
    GAME_INTERNET,
    GAME_SKIRMISH,
    GAME_6,
    GAME_7,
    // GAME_SOLE_SURVIVOR, // TODO, add SS code to allow RA to have a Sole Survivor mode
    GAME_COUNT
};

class MultiMission;

// Temp, this contains a union of a number of different structs.
struct GlobalPacket
{
    char pad[0x8B];
};

struct MPlayerScoreStruct
{
    char Name[12];
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL Bit1 : 1; // & 1
#else
    bool Bit1;
#endif
    int Winner;
    int field_10[4];
    PlayerColorType Scheme;
};

struct MPlayerOptionsStruct
{
    int LocalID;
    BOOL Bases;
    int Credits;
    BOOL Ore;
    BOOL Goodies;
    BOOL Ghosts;
    int UnitCount;
    int AIPlayers;
    char ScenarioName[44];
};

struct SerialSettingsType
{
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
};

struct NodeNameTag
{
    char Name[12];
    IPXAddressClass Address;
    HousesType House; // HousesType?
    PlayerColorType Color; // looks like a PlayerColorType?
    HousesType AltHouse; // HousesType?
    int field_19;
};

class SessionClass
{
public:
    SessionClass();
    ~SessionClass();

    void One_Time();
    void Init();

    GameEnum Game_To_Play() const { return GameToPlay; }
    void Set_Game_To_Play(GameEnum game) { GameToPlay = game; }
    void Set_MaxAhead(int maxahead) { MaxAhead = maxahead; }
    int Get_MaxAhead() const { return MaxAhead; }
    CommProtocolEnum Packet_Protocol() const { return PacketProtocol; }

    MPlayerOptionsStruct &MPlayer_Options() { return Options; }
    BOOL MPlayer_Goodies_Allowed() const { return Options.Goodies; }
    BOOL MPlayer_Ore_Growth() const { return Options.Ore; }
    BOOL MPlayer_Obi_Wan() const { return MPlayerObiWan; }
    void Set_MPlayer_Obi_Wan(BOOL onoff) { MPlayerObiWan = onoff; }
    BOOL MPlayer_Blitz() const { return MPlayerBlitz; }
    void Set_MPlayer_Blitz(BOOL onoff) { MPlayerBlitz = onoff; }

    int Desired_Frame_Rate() const { return DesiredFrameRate; }
    void Set_Desired_Frame_Rate(int value) { DesiredFrameRate = value; }
    int Processing_Start_Tick() const { return ProcessingStartTick; }
    void Set_Processing_Start_Tick(int value) { ProcessingStartTick = value; }
    void Reset_Processing_Values()
    {
        ProcessingStartTick = 0;
        ProcessingTicks = 0;
        ProcessingFrames = 0;
    }
    void Update_Processing_Tick_Value(int value) { ProcessingTicks += value; }
    void Tick_Processing_Frame() { ++ProcessingFrames; }

    BOOL Loading_Game() const { return LoadGame; }
    BOOL Saving_Game() const { return SaveGame; }

    int Save(Pipe &pipe);
    int Load(Straw &straw);
    int Save(GameFileClass &file);
    int Load(GameFileClass &file);

    void Read_MultiPlayer_Settings();
    void Write_MultiPlayer_Settings();
    void Read_Scenario_Descriptions();
    void Free_Scenario_Descriptions();

    MessageListClass &Get_Messages() { return Messages; }

    MPlayerScoreStruct &MPlayer_Score_Info(HousesType house) { return MPlayerScores[house]; }
    int MPlayer_Current_Game() const { return MPlayerCurrentGame; }

    GameFileClass &Recording_File() { return RecordFile; }
    BOOL Record_Game() const { return RecordGame; }
    void Set_Record_Game(BOOL onoff) { RecordGame = onoff; }
    BOOL Playback_Game() const { return PlaybackGame; }
    void Set_Playback_Game(BOOL onoff) { PlaybackGame = onoff; }
    BOOL Attraction_Allowed() const { return AllowAttract; }
    void Set_Allow_Attraction(BOOL onoff) { AllowAttract = onoff; }
    BOOL Super_Record_Game() const { return SuperRecord; }

    DynamicVectorClass<NodeNameTag *> &Games_List() { return Games; }
    DynamicVectorClass<NodeNameTag *> &Players_List() { return Players; }
    DynamicVectorClass<NodeNameTag *> &Network_Players_List() { return NetworkPlayers; }
    int Player_Count() const { return Players.Count(); }
    NodeNameTag *Player(int num) { return Players[num]; }

    BOOL Modem_Service() const { return ModemService; }

    BOOL Am_I_Master();
    BOOL Create_Connections();

    void Trap_Object();

    int Trap_Frame() const { return TrapFrame; }
    BOOL Trap_Check_Heap() const { return TrapCheckHeap; }

    uint32_t Compute_Unique_ID();

private:
    GameEnum GameToPlay;
    CommProtocolEnum PacketProtocol;
    MPlayerOptionsStruct Options;
    int UniqueID;
    char MPlayerName[12];
    PlayerColorType MPlayerPrefColor;
    PlayerColorType MPlayerColorIdx;
    HousesType MPlayerHouse;
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
    IPXAddressClass HostAddress;
    MessageListClass Messages;
    IPXAddressClass MessageAddress;
    char LastMessage[118];
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL SessionBit1 : 1; // & 1
#else
    bool SessionBit1;
#endif
    MPlayerScoreStruct MPlayerScores[HOUSES_MULTI_COUNT];
    int MPlayerGamesPlayed;
    int MPlayerNumScores;
    int MPlayerWinner;
    int MPlayerCurrentGame;
    GameFileClass RecordFile;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL RecordGame : 1; // & 1
    BOOL PlaybackGame : 1; // & 2
    BOOL AllowAttract : 1; // & 4
    BOOL SuperRecord : 1; // & 8 // Unknown if in RA, but seems to be used as if there is one huge recording file (ie,
                          // multipule games)?
#else
    bool RecordGame;
    bool PlaybackGame;
    bool AllowAttract;
    bool SuperRecord;
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
    BOOL ModemService;
    int PhoneIndex;
    SerialSettingsType SerialDefaults;
    char ModemGameToPlay;
    DynamicVectorClass<char *> PhoneBookEntries;
    DynamicVectorClass<char *> InitStrings;
    int TrapFrame;
    RTTIType TrapType; // Suspect this is RTTIType but needs confirming.
    int TrappedObject;
    coord_t TrapCoord;
    target_t TrapTarget;
    CellClass *TrapCell;
    BOOL TrapCheckHeap;
    BOOL TrapPrintCRC;
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
extern SessionClass &Session;
#else
extern SessionClass Session;
#endif

#endif // SESSION_H
