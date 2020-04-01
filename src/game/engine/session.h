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
    GAME_1, //These two are modem, unknown which.
    GAME_2,
    GAME_NETWORK,
    GAME_INTERNET,
    GAME_SKIRMISH,
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
    char m_Name[12];
    int m_field_C; //incrimented in HouseClass::Tally_Score when house !ToDie
    int m_Score[4];//scores per 4 game sessions
    PlayerColorType m_Scheme;
};

struct MPlayerOptionsStruct
{
    int m_LocalID;
    BOOL m_Bases;
    int m_Credits;
    BOOL m_Ore;
    BOOL m_Goodies;
    BOOL m_Ghosts;
    int m_UnitCount;
    int m_AIPlayers;
    char m_ScenarioName[44];
};

struct SerialSettingsType
{
    int m_Port;
    int m_IRQ;
    int m_Baud;
    char m_DialMethod;
    int m_InitStringIndex;
    int m_CallWaitStringIndex;
    char m_CallWaitString[16];
    BOOL m_Compression;
    BOOL m_ErrorCorrection;
    BOOL m_HardwareFlowControl;
    char m_ModemName[63];
};

struct NodeNameTag
{
    char m_Name[12];
    IPXAddressClass m_Address;
    HousesType m_House;
    PlayerColorType m_Color;
    HousesType m_AltHouse;
};

class SessionClass
{
public:
    SessionClass();
    ~SessionClass();

    void One_Time();
    void Init();

    GameEnum Game_To_Play() const { return m_GameToPlay; }
    void Set_Game_To_Play(GameEnum game) { m_GameToPlay = game; }
    CommProtocolEnum Packet_Protocol() const { return m_PacketProtocol; }
    int Frame_Send_Rate() { return m_FrameSendRate; }

    MPlayerOptionsStruct &MPlayer_Options() { return m_Options; }
    BOOL MPlayer_Goodies_Allowed() const { return m_Options.m_Goodies; }
    BOOL MPlayer_Ore_Growth() const { return m_Options.m_Ore; }
    BOOL MPlayer_Obi_Wan() const { return m_MPlayerObiWan; }
    void Set_MPlayer_Obi_Wan(BOOL onoff) { m_MPlayerObiWan = onoff; }
    BOOL MPlayer_Blitz() const { return m_MPlayerBlitz; }
    void Set_MPlayer_Blitz(BOOL onoff) { m_MPlayerBlitz = onoff; }
    int MPlayer_Max() const { return m_MPlayerMax; }

    int Desired_Frame_Rate() const { return m_DesiredFrameRate; }
    void Set_Desired_Frame_Rate(int value) { m_DesiredFrameRate = value; }
    int Processing_Start_Tick() const { return m_ProcessingStartTick; }
    void Set_Processing_Start_Tick(int value) { m_ProcessingStartTick = value; }
    void Reset_Processing_Values() { m_ProcessingStartTick = 0; m_ProcessingTicks = 0; m_ProcessingFrames = 0; }
    void Update_Processing_Tick_Value(int value) { m_ProcessingTicks += value; }
    void Tick_Processing_Frame() { ++m_ProcessingFrames; }

    BOOL Loading_Game() const { return m_LoadGame; }
    BOOL Saving_Game() const { return m_SaveGame; }

    int Save(Pipe &pipe);
    int Load(Straw &straw);
    int Save(GameFileClass &file);
    int Load(GameFileClass &file);

    void Read_MultiPlayer_Settings();
    void Write_MultiPlayer_Settings();
    void Read_Scenario_Descriptions();
    void Free_Scenario_Descriptions();

    MessageListClass &Get_Messages() { return m_Messages; }

    MPlayerScoreStruct &MPlayer_Score_Info(HousesType house) { return m_MPlayerScores[house]; }
    int MPlayer_Current_Game() const { return m_MPlayerCurrentGame; }
    void MPlayer_Clear_Game_Name() { m_MPlayerGameName[0] = '\0'; }

    GameFileClass &Recording_File() { return m_RecordFile; }
    BOOL Attraction_Allowed() const { return m_AllowAttract; }
    void Set_Allow_Attraction(BOOL onoff) { m_AllowAttract = onoff; }
    BOOL Record_Game() const { return m_RecordGame; }
    void Set_Record_Game(BOOL onoff) { m_RecordGame = onoff; }
    BOOL Playback_Game() const { return m_PlaybackGame; }
    void Set_Playback_Game(BOOL onoff) { m_PlaybackGame = onoff; }
    BOOL Super_Record_Game() const { return m_SuperRecord; }
    void Set_Super_Record_Game(BOOL onoff) { m_SuperRecord = onoff; }

    BOOL Net_Protect_Enabled() const { return m_NetProtect; }
    void Set_Net_Protect(BOOL onoff) { m_NetProtect = onoff; }
    BOOL Net_Stealth_Enabled() const { return m_NetStealth; }
    void Set_Net_Stealth(BOOL onoff) { m_NetStealth = onoff; }

    DynamicVectorClass<NodeNameTag *> &Games_List() { return m_Games; }
    DynamicVectorClass<NodeNameTag *> &Players_List() { return m_Players; }
    DynamicVectorClass<NodeNameTag *> &Network_Players_List() { return m_NetworkPlayers; }

    BOOL Modem_Service() const { return m_ModemService; }

    BOOL Am_I_Master();
    BOOL Create_Connections();

    void Trap_Object();

    int Trap_Frame() const { return m_TrapFrame; }
    BOOL Trap_Check_Heap() const { return m_TrapCheckHeap; }
    int Trap_Print_CRC() const { return m_TrapPrintCRC; }

    uint32_t Compute_Unique_ID();

private:
    GameEnum m_GameToPlay;
    CommProtocolEnum m_PacketProtocol;
    MPlayerOptionsStruct m_Options;
    int m_UniqueID;
    char m_MPlayerName[12];
    PlayerColorType m_MPlayerPrefColor;
    PlayerColorType m_MPlayerColorIdx;
    HousesType m_MPlayerHouse;
    BOOL m_MPlayerObiWan;
    BOOL m_MPlayerBlitz;
    int m_MPlayerMax;
    int m_MPlayerCount;
    int m_MaxAhead;
    int m_FrameSendRate;
    int m_DesiredFrameRate;
    int m_ProcessingStartTick;
    int m_ProcessingTicks;
    int m_ProcessingFrames;
    BOOL m_LoadGame;
    BOOL m_SaveGame;
    DynamicVectorClass<MultiMission *> m_MPlayerScenarios;
    char m_SendScenarioName[513];
    char m_SendScenarioDigest[32];
    int m_SendScenarioSize;
    BOOL m_SendScenarioIsOfficial;
    char m_UnkArray[20];
    int m_UnkArrayPos;
    IPXAddressClass m_HostAddress;
    MessageListClass m_Messages;
    IPXAddressClass m_MessageAddress;
    char m_LastMessage[120];
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_SessionBit1 : 1; // & 1
#else
    bool m_SessionBit1;
#endif
    MPlayerScoreStruct m_MPlayerScores[HOUSES_MULTI_COUNT];
    int m_MPlayerGamesPlayed;
    int m_MPlayerNumScores;
    int m_MPlayerWinner;
    int m_MPlayerCurrentGame;
    GameFileClass m_RecordFile;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_RecordGame : 1; // & 1
    BOOL m_PlaybackGame : 1; // & 2
    BOOL m_AllowAttract : 1; // & 4
    BOOL m_SuperRecord : 1; // & 8 // Unknown if in RA, but seems to be used as if there is one huge recording file (ie, multipule games)?
#else
    bool m_RecordGame;
    bool m_PlaybackGame;
    bool m_AllowAttract;
    bool m_SuperRecord;
#endif
    BOOL m_IsBridge;
    IPXAddressClass m_BridgeNetwork;
    BOOL m_NetStealth;
    BOOL m_NetProtect;
    BOOL m_NetOpen;
    char m_MPlayerGameName[12];
    GlobalPacket m_GPacket;
    int m_GPacketLen;
    IPXAddressClass m_GAddress;
    int16_t m_GProductID;
    char m_MetaPacket[520];
    int m_MetaSize;
    DynamicVectorClass<NodeNameTag *> m_Games;
    DynamicVectorClass<NodeNameTag *> m_Players;
    DynamicVectorClass<NodeNameTag *> m_NetworkPlayers;
    BOOL m_ModemService;
    int m_PhoneIndex;
    SerialSettingsType m_SerialDefaults;
    char m_ModemGameToPlay;
    DynamicVectorClass<char *> m_PhoneBookEntries;
    DynamicVectorClass<char *> m_InitStrings;
    int m_TrapFrame;
    RTTIType m_TrapType; // Suspect this is RTTIType but needs confirming.
    int m_TrappedObject;
    coord_t m_TrapCoord;
    target_t m_TrapTarget;
    CellClass *m_TrapCell;
    BOOL m_TrapCheckHeap;
    int m_TrapPrintCRC;
};

#ifdef GAME_DLL
extern SessionClass &g_Session;
#else
extern SessionClass g_Session;
#endif

#endif // SESSION_H
