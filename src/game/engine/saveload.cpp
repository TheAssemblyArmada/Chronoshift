/**
 * @file
 *
 * @author tomsons26
 *
 * @brief Functions used for Saving and Loading savegames.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "saveload.h"
#include "gamedebug.h"
#include "gameoptions.h"
#include "globals.h"
#include "house.h"
#include "iomap.h"
#include "pipe.h"
#include "scenario.h"
#include "session.h"
#include "special.h"
#include "straw.h"
#include "vortex.h"

void Put_All(Pipe &pipe, int skip_callback)
{
#ifdef GAME_DLL
    void (*func)(Pipe &, int) = reinterpret_cast<void (*)(Pipe &, int)>(0x005375D0);
    func(pipe, skip_callback);
#endif
}

int Save_Game(int number, char *savename, int unknown)
{
#ifdef GAME_DLL
    int (*func)(int, char *, int) = reinterpret_cast<int (*)(int, char *, int)>(0x005379FC);
    return func(number, savename, unknown);
#else
    return 0;
#endif
}

void Post_Load_Game(int skip_overpass)
{
    if (skip_overpass == false) {
        Map.Overpass();
    }
    Scen.Set_Bridge_Count(Map.Intact_Bridge_Count());
    Map.Zone_Reset(15);
}

int Load_Game(int unknown)
{
#ifdef GAME_DLL
    int (*func)() = reinterpret_cast<int (*)()>(0x00537D10);
    return func();
#else
    return 0;
#endif
}

int Save_Misc_Values(Pipe &pipe)
{
    int house = g_PlayerPtr->What_Type();
    pipe.Put(&house, sizeof(house));
    pipe.Put(&g_GameFrame, sizeof(g_GameFrame));
    int objcount = CurrentObjects.Count();
    for (int i = 0; i < objcount; ++i) {
        ObjectClass *obj = CurrentObjects[i];
        pipe.Put(&obj, sizeof(obj));
    }

    g_ChronalVortex.Save(pipe);
    pipe.Put(&g_IsTanyaDead, sizeof(g_IsTanyaDead));
    pipe.Put(&g_SaveTanya, sizeof(g_SaveTanya));
    return 1;
}

int Load_Misc_Values(Straw &straw)
{
    int house;
    int objcount;
    straw.Get(&house, sizeof(house));
    g_PlayerPtr = HouseClass::As_Pointer((HousesType)house);
    straw.Get(&g_GameFrame, sizeof(g_GameFrame));
    straw.Get(&objcount, sizeof(objcount));
    for (int i = 0; i < objcount; ++i) {
        ObjectClass *obj = nullptr;
        straw.Get(&obj, sizeof(obj));
        CurrentObjects.Add(obj);
    }

    g_ChronalVortex.Load(straw);
    straw.Get(&g_IsTanyaDead, sizeof(g_IsTanyaDead));
    straw.Get(&g_SaveTanya, sizeof(g_SaveTanya));
    return 1;
}

int Save_MPlayer_Values(Pipe &pipe)
{
    Session.Save(pipe);
    pipe.Put(&g_BuildLevel, sizeof(g_BuildLevel));
    pipe.Put(&g_Debug_Unshroud, sizeof(g_Debug_Unshroud));
    pipe.Put(&g_seed, sizeof(g_seed));
    pipe.Put(&Whom, sizeof(Whom));
    pipe.Put(&Special, sizeof(Special));
    pipe.Put(&Options, sizeof(Options));
    return 1;
}

int Load_MPlayer_Values(Straw &straw)
{
    Session.Load(straw);
    straw.Get(&g_BuildLevel, sizeof(g_BuildLevel));
    straw.Get(&g_Debug_Unshroud, sizeof(g_Debug_Unshroud));
    straw.Get(&g_seed, sizeof(g_seed));
    straw.Get(&Whom, sizeof(Whom));
    straw.Get(&Special, sizeof(Special));
    straw.Get(&Options, sizeof(Options));
    return 1;
}

void Code_All_Pointers()
{
#ifdef GAME_DLL
    void (*func)() = reinterpret_cast<void (*)()>(0x005392A4);
    func();
#endif
}
void Decode_All_Pointers()
{
#ifdef GAME_DLL
    void (*func)() = reinterpret_cast<void (*)()>(0x005393D4);
    func();
#endif
}

int Get_Savefile_Info(int number, char *savename, unsigned *scenarioidx, HousesType *house)
{
    char filename[512];
    char name[44];
    sprintf(filename, "savegame.%03d", number);
    BufferIOFileClass file(filename);
    FileStraw straw(file);
    if (straw.Get(name, sizeof(name)) != sizeof(name)) {
        return false;
    }
    name[strlen(name) - 1] = '\0';
    strcpy(savename, name);
    if (straw.Get(scenarioidx, sizeof(*scenarioidx)) != sizeof(*scenarioidx)) {
        return false;
    }
    // BUGFIX: Original code used sizeof(house) instead of sizeof(*house).
    if (straw.Get(house, sizeof(*house)) != sizeof(*house)) {
        return false;
    }
    int magic;
    if (straw.Get(&magic, sizeof(magic)) != sizeof(magic)) {
        return false;
    }
    // original code
    // if (magic != RA95_2XX_MAGIC_ID && magic != RA95_30X_MAGIC_ID) {
    //    return false;
    //}
    switch (magic) {
        default:
            DEBUG_LOG("Get_Savefile_Info - Uknown Magic Number! Got %x\n", magic);
            return false;
        case RADOS_10X_MAGIC_ID:
        case RADOS_20X_MAGIC_ID:
            DEBUG_LOG("Get_Savefile_Info - DOS saves are not supported!\n");
            return false;
        case RA95_10X_MAGIC_ID:
        case RA95_20X_MAGIC_ID:
        case RA95_105_MAGIC_ID:
            DEBUG_LOG("Get_Savefile_Info - Old saves currently are not supported!\n");
            return false;
        case RA95_2XX_MAGIC_ID:
        case RA95_30X_MAGIC_ID:
            break;
    }

    return true;
}

int Reconcile_Players()
{
#ifdef GAME_DLL
    int (*func)() = reinterpret_cast<int (*)()>(0x00539708);
    return func();
#else
    return 0;
#endif
}

void MPlayer_Save_Message()
{
}
