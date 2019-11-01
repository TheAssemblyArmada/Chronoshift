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
#include "session.h"
#include "house.h"
#include "multimission.h"
#include "pipe.h"
#include "straw.h"

#ifndef GAME_DLL
SessionClass Session;
#endif

SessionClass::SessionClass()
{
}

SessionClass::~SessionClass()
{
}

void SessionClass::One_Time()
{
    Read_MultiPlayer_Settings();
    Read_Scenario_Descriptions();

    m_UniqueID = Compute_Unique_ID();
}

void SessionClass::Init()
{
    //empty
}

BOOL SessionClass::Save(Pipe &pipe)
{
    pipe.Put(&m_PacketProtocol, sizeof(m_PacketProtocol));
    pipe.Put(&m_MaxAhead, sizeof(m_MaxAhead));
    pipe.Put(&m_FrameSendRate, sizeof(m_FrameSendRate));
    pipe.Put(&m_DesiredFrameRate, sizeof(m_DesiredFrameRate));
    pipe.Put(&m_MPlayerPrefColor, sizeof(m_MPlayerPrefColor));
    pipe.Put(&m_MPlayerColorIdx, sizeof(m_MPlayerColorIdx));
    pipe.Put(&m_MPlayerHouse, sizeof(m_MPlayerHouse));
    pipe.Put(&m_MPlayerCount, sizeof(m_MPlayerCount));
    pipe.Put(&m_Options.m_Bases, sizeof(m_Options.m_Bases));
    pipe.Put(&m_Options.m_Credits, sizeof(m_Options.m_Credits));
    pipe.Put(&m_Options.m_Ore, sizeof(m_Options.m_Ore));
    pipe.Put(&m_Options.m_Goodies, sizeof(m_Options.m_Goodies));
    pipe.Put(&m_Options.m_Ghosts, sizeof(m_Options.m_Ghosts));
    pipe.Put(&m_Options.m_UnitCount, sizeof(m_Options.m_UnitCount));
    pipe.Put(&m_Options.m_AIPlayers, sizeof(m_Options.m_AIPlayers));
    pipe.Put(&m_MPlayerObiWan, sizeof(m_MPlayerObiWan));
    pipe.Put(&m_SaveGame, sizeof(m_SaveGame));
    return true;
}

BOOL SessionClass::Load(Straw &straw)
{
    straw.Get(&m_PacketProtocol, sizeof(m_PacketProtocol));
    straw.Get(&m_MaxAhead, sizeof(m_MaxAhead));
    straw.Get(&m_FrameSendRate, sizeof(m_FrameSendRate));
    straw.Get(&m_DesiredFrameRate, sizeof(m_DesiredFrameRate));
    straw.Get(&m_MPlayerPrefColor, sizeof(m_MPlayerPrefColor));
    straw.Get(&m_MPlayerColorIdx, sizeof(m_MPlayerColorIdx));
    straw.Get(&m_MPlayerHouse, sizeof(m_MPlayerHouse));
    straw.Get(&m_MPlayerCount, sizeof(m_MPlayerCount));
    straw.Get(&m_Options.m_Bases, sizeof(m_Options.m_Bases));
    straw.Get(&m_Options.m_Credits, sizeof(m_Options.m_Credits));
    straw.Get(&m_Options.m_Ore, sizeof(m_Options.m_Ore));
    straw.Get(&m_Options.m_Goodies, sizeof(m_Options.m_Goodies));
    straw.Get(&m_Options.m_Ghosts, sizeof(m_Options.m_Ghosts));
    straw.Get(&m_Options.m_UnitCount, sizeof(m_Options.m_UnitCount));
    straw.Get(&m_Options.m_AIPlayers, sizeof(m_Options.m_AIPlayers));
    straw.Get(&m_MPlayerObiWan, sizeof(m_MPlayerObiWan));
    straw.Get(&m_SaveGame, sizeof(m_SaveGame));
    return true;
}

BOOL SessionClass::Save(GameFileClass &file)
{
    file.Write(&m_PacketProtocol, sizeof(m_GameToPlay));
    file.Write(&m_PacketProtocol, sizeof(m_PacketProtocol));
    file.Write(&m_MaxAhead, sizeof(m_MaxAhead));
    file.Write(&m_FrameSendRate, sizeof(m_FrameSendRate));
    file.Write(&m_DesiredFrameRate, sizeof(m_DesiredFrameRate));
    file.Write(&m_MPlayerPrefColor, sizeof(m_MPlayerPrefColor));
    file.Write(&m_MPlayerColorIdx, sizeof(m_MPlayerColorIdx));
    file.Write(&m_MPlayerHouse, sizeof(m_MPlayerHouse));
    file.Write(&m_MPlayerCount, sizeof(m_MPlayerCount));
    file.Write(&m_Options.m_Bases, sizeof(m_Options.m_Bases));
    file.Write(&m_Options.m_Credits, sizeof(m_Options.m_Credits));
    file.Write(&m_Options.m_Ore, sizeof(m_Options.m_Ore));
    file.Write(&m_Options.m_Goodies, sizeof(m_Options.m_Goodies));
    file.Write(&m_Options.m_Ghosts, sizeof(m_Options.m_Ghosts));
    file.Write(&m_Options.m_UnitCount, sizeof(m_Options.m_UnitCount));
    file.Write(&m_Options.m_AIPlayers, sizeof(m_Options.m_AIPlayers));
    file.Write(&m_MPlayerObiWan, sizeof(m_MPlayerObiWan));
    file.Write(&m_SaveGame, sizeof(m_SaveGame));
    
    int playercount = m_Players.Count();
    file.Write(&playercount, sizeof(playercount));
    for (int i = 0; i < m_Players.Count(); ++i) {
        file.Write(&m_Players[i], sizeof(NodeNameTag));
    }
    
    return true;
}

BOOL SessionClass::Load(GameFileClass &file)
{
    file.Read(&m_PacketProtocol, sizeof(m_GameToPlay));
    file.Read(&m_PacketProtocol, sizeof(m_PacketProtocol));
    file.Read(&m_MaxAhead, sizeof(m_MaxAhead));
    file.Read(&m_FrameSendRate, sizeof(m_FrameSendRate));
    file.Read(&m_DesiredFrameRate, sizeof(m_DesiredFrameRate));
    file.Read(&m_MPlayerPrefColor, sizeof(m_MPlayerPrefColor));
    file.Read(&m_MPlayerColorIdx, sizeof(m_MPlayerColorIdx));
    file.Read(&m_MPlayerHouse, sizeof(m_MPlayerHouse));
    file.Read(&m_MPlayerCount, sizeof(m_MPlayerCount));
    file.Read(&m_Options.m_Bases, sizeof(m_Options.m_Bases));
    file.Read(&m_Options.m_Credits, sizeof(m_Options.m_Credits));
    file.Read(&m_Options.m_Ore, sizeof(m_Options.m_Ore));
    file.Read(&m_Options.m_Goodies, sizeof(m_Options.m_Goodies));
    file.Read(&m_Options.m_Ghosts, sizeof(m_Options.m_Ghosts));
    file.Read(&m_Options.m_UnitCount, sizeof(m_Options.m_UnitCount));
    file.Read(&m_Options.m_AIPlayers, sizeof(m_Options.m_AIPlayers));
    file.Read(&m_MPlayerObiWan, sizeof(m_MPlayerObiWan));
    file.Read(&m_SaveGame, sizeof(m_SaveGame));
    
    int playercount;
    file.Read(&playercount, sizeof(playercount));
    if (playercount > 0) {
        for (int i = 0; i < playercount; ++i) {
            NodeNameTag *nodename = new NodeNameTag();
            file.Read(nodename, sizeof(NodeNameTag));
            m_Players.Add(nodename);
        }
    }

    return true;
}

void SessionClass::Read_MultiPlayer_Settings()
{
#ifdef GAME_DLL
    void (*func)(SessionClass*) = reinterpret_cast<void (*)(SessionClass*)>(0x0054AB08);
    return func(this);
#endif
}

void SessionClass::Write_MultiPlayer_Settings()
{
#ifdef GAME_DLL
    void (*func)(SessionClass*) = reinterpret_cast<void (*)(SessionClass*)>(0x0054B510);
    return func(this);
#endif
}

void SessionClass::Read_Scenario_Descriptions()
{
#ifdef GAME_DLL
    void (*func)(SessionClass*) = reinterpret_cast<void (*)(SessionClass*)>(0x0054BA1C);
    return func(this);
#endif
}

void SessionClass::Free_Scenario_Descriptions()
{
    for (int i = 0; i < m_MPlayerScenarios.Count(); ++i) {
        delete m_MPlayerScenarios[i];
    }
    m_MPlayerScenarios.Clear();

    for (int i = 0; i < m_InitStrings.Count(); ++i) {
        delete m_InitStrings[i];
    }
    m_InitStrings.Clear();

    for (int i = 0; i < m_PhoneBookEntries.Count(); ++i) {
        delete m_PhoneBookEntries[i];
    }
    m_PhoneBookEntries.Clear();
}

/**
* @brief 
*
* 0x0054C298
*/
void SessionClass::Trap_Object()
{
#ifdef GAME_DLL
    void (*func)(SessionClass *) = reinterpret_cast<void (*)(SessionClass *)>(0x0054C298);
    return func(this);
#endif
}

uint32_t SessionClass::Compute_Unique_ID()
{
#ifdef GAME_DLL
    uint32_t (*func)(SessionClass *) = reinterpret_cast<uint32_t (*)(SessionClass *)>(0x0054C8F4);
    return func(this);
#else
    return 0;
#endif
}

BOOL SessionClass::Am_I_Master()
{
    for (int i = 0; i < Session.m_MPlayerMax; ++i) {
        HouseClass *house = HouseClass::As_Pointer(HousesType(i) + HOUSES_MULTI_FIRST);
        if (house->Is_Human()) {
            if (house == g_PlayerPtr) {
                return true;
            }
            return false;
        }
    }
    return false;
}

BOOL SessionClass::Create_Connections()
{
#ifdef GAME_DLL
    BOOL (*func)(SessionClass *) = reinterpret_cast<BOOL (*)(SessionClass *)>(0x0054A4F8);
    return func(this);
#else
    return false;
#endif
}
