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

    UniqueID = Compute_Unique_ID();
}

void SessionClass::Init()
{
    //empty
}

BOOL SessionClass::Save(Pipe &pipe)
{
    pipe.Put(&PacketProtocol, sizeof(PacketProtocol));
    pipe.Put(&MaxAhead, sizeof(MaxAhead));
    pipe.Put(&FrameSendRate, sizeof(FrameSendRate));
    pipe.Put(&DesiredFrameRate, sizeof(DesiredFrameRate));
    pipe.Put(&MPlayerPrefColor, sizeof(MPlayerPrefColor));
    pipe.Put(&MPlayerColorIdx, sizeof(MPlayerColorIdx));
    pipe.Put(&MPlayerHouse, sizeof(MPlayerHouse));
    pipe.Put(&MPlayerCount, sizeof(MPlayerCount));
    pipe.Put(&Options.Bases, sizeof(Options.Bases));
    pipe.Put(&Options.Credits, sizeof(Options.Credits));
    pipe.Put(&Options.Ore, sizeof(Options.Ore));
    pipe.Put(&Options.Goodies, sizeof(Options.Goodies));
    pipe.Put(&Options.Ghosts, sizeof(Options.Ghosts));
    pipe.Put(&Options.UnitCount, sizeof(Options.UnitCount));
    pipe.Put(&Options.AIPlayers, sizeof(Options.AIPlayers));
    pipe.Put(&MPlayerObiWan, sizeof(MPlayerObiWan));
    pipe.Put(&SaveGame, sizeof(SaveGame));
    return true;
}

BOOL SessionClass::Load(Straw &straw)
{
    straw.Get(&PacketProtocol, sizeof(PacketProtocol));
    straw.Get(&MaxAhead, sizeof(MaxAhead));
    straw.Get(&FrameSendRate, sizeof(FrameSendRate));
    straw.Get(&DesiredFrameRate, sizeof(DesiredFrameRate));
    straw.Get(&MPlayerPrefColor, sizeof(MPlayerPrefColor));
    straw.Get(&MPlayerColorIdx, sizeof(MPlayerColorIdx));
    straw.Get(&MPlayerHouse, sizeof(MPlayerHouse));
    straw.Get(&MPlayerCount, sizeof(MPlayerCount));
    straw.Get(&Options.Bases, sizeof(Options.Bases));
    straw.Get(&Options.Credits, sizeof(Options.Credits));
    straw.Get(&Options.Ore, sizeof(Options.Ore));
    straw.Get(&Options.Goodies, sizeof(Options.Goodies));
    straw.Get(&Options.Ghosts, sizeof(Options.Ghosts));
    straw.Get(&Options.UnitCount, sizeof(Options.UnitCount));
    straw.Get(&Options.AIPlayers, sizeof(Options.AIPlayers));
    straw.Get(&MPlayerObiWan, sizeof(MPlayerObiWan));
    straw.Get(&SaveGame, sizeof(SaveGame));
    return true;
}

BOOL SessionClass::Save(GameFileClass &file)
{
    file.Write(&PacketProtocol, sizeof(GameToPlay));
    file.Write(&PacketProtocol, sizeof(PacketProtocol));
    file.Write(&MaxAhead, sizeof(MaxAhead));
    file.Write(&FrameSendRate, sizeof(FrameSendRate));
    file.Write(&DesiredFrameRate, sizeof(DesiredFrameRate));
    file.Write(&MPlayerPrefColor, sizeof(MPlayerPrefColor));
    file.Write(&MPlayerColorIdx, sizeof(MPlayerColorIdx));
    file.Write(&MPlayerHouse, sizeof(MPlayerHouse));
    file.Write(&MPlayerCount, sizeof(MPlayerCount));
    file.Write(&Options.Bases, sizeof(Options.Bases));
    file.Write(&Options.Credits, sizeof(Options.Credits));
    file.Write(&Options.Ore, sizeof(Options.Ore));
    file.Write(&Options.Goodies, sizeof(Options.Goodies));
    file.Write(&Options.Ghosts, sizeof(Options.Ghosts));
    file.Write(&Options.UnitCount, sizeof(Options.UnitCount));
    file.Write(&Options.AIPlayers, sizeof(Options.AIPlayers));
    file.Write(&MPlayerObiWan, sizeof(MPlayerObiWan));
    file.Write(&SaveGame, sizeof(SaveGame));
    
    int playercount = Players.Count();
    file.Write(&playercount, sizeof(playercount));
    for (int i = 0; i < Players.Count(); ++i) {
        file.Write(&Players[i], sizeof(NodeNameTag));
    }
    
    return true;
}

BOOL SessionClass::Load(GameFileClass &file)
{
    file.Read(&PacketProtocol, sizeof(GameToPlay));
    file.Read(&PacketProtocol, sizeof(PacketProtocol));
    file.Read(&MaxAhead, sizeof(MaxAhead));
    file.Read(&FrameSendRate, sizeof(FrameSendRate));
    file.Read(&DesiredFrameRate, sizeof(DesiredFrameRate));
    file.Read(&MPlayerPrefColor, sizeof(MPlayerPrefColor));
    file.Read(&MPlayerColorIdx, sizeof(MPlayerColorIdx));
    file.Read(&MPlayerHouse, sizeof(MPlayerHouse));
    file.Read(&MPlayerCount, sizeof(MPlayerCount));
    file.Read(&Options.Bases, sizeof(Options.Bases));
    file.Read(&Options.Credits, sizeof(Options.Credits));
    file.Read(&Options.Ore, sizeof(Options.Ore));
    file.Read(&Options.Goodies, sizeof(Options.Goodies));
    file.Read(&Options.Ghosts, sizeof(Options.Ghosts));
    file.Read(&Options.UnitCount, sizeof(Options.UnitCount));
    file.Read(&Options.AIPlayers, sizeof(Options.AIPlayers));
    file.Read(&MPlayerObiWan, sizeof(MPlayerObiWan));
    file.Read(&SaveGame, sizeof(SaveGame));
    
    int playercount;
    file.Read(&playercount, sizeof(playercount));
    if (playercount > 0) {
        for (int i = 0; i < playercount; ++i) {
            NodeNameTag *nodename = new NodeNameTag();
            file.Read(nodename, sizeof(NodeNameTag));
            Players.Add(nodename);
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
    for (int i = 0; i < MPlayerScenarios.Count(); ++i) {
        delete MPlayerScenarios[i];
    }
    MPlayerScenarios.Clear();

    for (int i = 0; i < InitStrings.Count(); ++i) {
        delete InitStrings[i];
    }
    InitStrings.Clear();

    for (int i = 0; i < PhoneBookEntries.Count(); ++i) {
        delete PhoneBookEntries[i];
    }
    PhoneBookEntries.Clear();
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
    for (int i = 0; i < Session.MPlayerMax; ++i) {
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
