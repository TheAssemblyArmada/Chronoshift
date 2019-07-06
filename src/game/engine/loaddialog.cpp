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
#include "loaddialog.h"
#include "list.h" 
#include "saveload.h"
#include <cstdio>

#if defined PLATFORM_WINDOWS && !defined COMPILER_WATCOM
#include <fileapi.h>
#include <winbase.h>
#endif

using std::sprintf;

LoadOptionsClass::LoadOptionsClass(LoadOptionsClass::OperationModeEnum mode) : OperationMode(mode)
{
    Files.Clear();
}

LoadOptionsClass::~LoadOptionsClass()
{
    for (int i = 0; i < Files.Count(); ++i) {
        delete Files[i];
    }
    Files.Clear();
}

int LoadOptionsClass::Process()
{
#ifndef CHRONOSHIFT_STANDALONE
    int (*func)(const LoadOptionsClass *) = reinterpret_cast<int (*)(const LoadOptionsClass *)>(0x004FCEB4);
    return func(this);
#else
    return 0;
#endif
}

void LoadOptionsClass::Clear_List(ListClass *list)
{
    for (int i = 0; i < list->Count(); ++i) {
        FileEntryClass *item = (FileEntryClass *)list->Get_Item(0);
        list->Remove_Item(item->Name);
    }

    for (int j = 0; j < Files.Count(); ++j) {
        delete Files[j];
    }
    Files.Clear();
}

void LoadOptionsClass::Fill_List(ListClass *list)
{
#ifdef PLATFORM_WINDOWS
    WIN32_FIND_DATA find;
    FileEntryClass *file_entry = nullptr;
    Clear_List(list);

    if (OperationMode == MODE_SAVE) {
        file_entry = new FileEntryClass();
        strcpy(file_entry->Name, Fetch_String(TXT_EMPTY_SLOT));
        file_entry->Timestamp = -1;
        Files.Add(file_entry);
    }

    HANDLE hndl = FindFirstFileA("savegame.*", &find);
    if (hndl != INVALID_HANDLE_VALUE) {
        do {
            if (strcasecmp(find.cFileName, "savegame.net")) {
                char savename[76];
                unsigned scenarioidx;
                HousesType house;
                int number = Num_From_Ext(find.cFileName);
                int state = Get_Savefile_Info(number, savename, &scenarioidx, &house);
                file_entry = new FileEntryClass();
                file_entry->Name[0] = 0;
                if (state == FALSE) {
                    const char *old = Fetch_String(TXT_OLD_GAME);
                    strcpy(file_entry->Name, old); // according to DOS its strcpy here
                    strcat(file_entry->Name, " "); // add space

                } else if (house == HOUSES_USSR || house == HOUSES_UKRAINE) {
                    sprintf(file_entry->Name, "(%s) ", Fetch_String(TXT_SOVIET));
                } else {
                    sprintf(file_entry->Name, "(%s) ", Fetch_String(TXT_ALLIES));
                }
                strlcat(file_entry->Name, savename, sizeof(file_entry->Name));
                file_entry->State = state;
                file_entry->Index = scenarioidx;
                file_entry->House = house;
                file_entry->Number = number;
                uint16_t time, date;
                FileTimeToDosDateTime(&find.ftCreationTime, &date, &time);
                file_entry->Timestamp = time | (date << 16);
                Files.Add(file_entry);
            }
        } while (FindNextFileA(hndl, &find));
    }

    if (OperationMode == MODE_SAVE) {
        int i;
        for (i = 0; i < Files.Count(); ++i) {
            int last = -1;
            for (int j = 0; j < Files.Count(); ++j) {
                if (i == Files[j]->Number) {
                    last = j;
                    break;
                }
            }
            if (last == -1) {
                break;
            }
        }
        Files[0]->Number = i;
    }
    qsort(&Files[0], Files.Count(), sizeof(Files[0]), Compare);
    for (int k = 0; k < Files.Count(); ++k) {
        list->Add_Item(Files[k]->Name);
    }
#endif
}

int LoadOptionsClass::Num_From_Ext(const char *name)
{
#ifdef PLATFORM_WINDOWS
    char ext[256];
    _splitpath(name, nullptr, nullptr, nullptr, ext);
    return atoi(&ext[1]);
#else
    // TODO cross platform versions.
    return 0;
#endif
}

int LoadOptionsClass::Compare(const void *ptr1, const void *ptr2)
{
    FileEntryClass *entry1 = *(FileEntryClass **)ptr1;
    FileEntryClass *entry2 = *(FileEntryClass **)ptr2;

    if (entry1->Timestamp > entry2->Timestamp) {
        return -1;
    }

    return entry1->Timestamp < entry2->Timestamp;
}
