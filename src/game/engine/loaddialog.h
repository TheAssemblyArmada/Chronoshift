/**
 * @file
 *
 * @author tomsons26
 *
 * @brief Save/Load dialog class.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */

#pragma once

#ifndef LOADDIALOG_H
#define LOADDIALOG_H

#include "always.h"
#include "gametypes.h"
#include "vector.h"

class ListClass;

class LoadOptionsClass
{
public:
    enum OperationModeEnum
    {
        MODE_NONE = 0,
        MODE_LOAD = 1,
        MODE_SAVE = 2,
        MODE_DELETE = 3
    };
    class FileEntryClass
    {
    public:
        FileEntryClass() {}
        FileEntryClass(FileEntryClass const &that) {}
        ~FileEntryClass() {}

        char Name[80];
        int Index; // scenario index
        HousesType House;
        int Number; // number that's the extension of the savegame
        int Timestamp; // packed timestamp
        int State; // is this save old or new
    };

    LoadOptionsClass(OperationModeEnum mode);
    ~LoadOptionsClass();

    int Process();

    void Clear_List(ListClass *list);
    void Fill_List(ListClass *list);

    static int Num_From_Ext(const char *name);
    static int Compare(const void *ptr1, const void *ptr2);

#ifdef GAME_DLL
    static void Hook_Me();
#endif

protected:
    OperationModeEnum OperationMode;
    DynamicVectorClass<FileEntryClass *> Files;
};

#ifdef GAME_DLL
#include "hooker.h"

inline void LoadOptionsClass::Hook_Me()
{
#ifdef COMPLIER_WATCOM
    // Hook_Function(0x004FCEB4, *LoadOptionsClass::Process); // Not implemented.
    Hook_Function(0x004FDA04, *LoadOptionsClass::Clear_List);
    Hook_Function(0x004FDA80, *LoadOptionsClass::Fill_List);
#endif
}
#endif

#endif // LOADDIALOG_H
