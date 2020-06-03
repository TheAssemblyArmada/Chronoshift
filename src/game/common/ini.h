/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief INI file parsing class.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef INI_H
#define INI_H

#include "always.h"
#include "crc.h"
#include "fixed.h"
#include "index.h"
#include "listnode.h"
#include "pipe.h"
#include "straw.h"
#include <string.h>

class FileClass;
class Straw;
class Pipe;

// find out what this really is.
enum INIIntegerType
{
    INI_INTEGER_AS_DECIMAL = 0,
    INI_INTEGER_AS_HEX = 1,
    INI_INTEGER_AS_MOTOROLA_HEX = 2
};

class INIEntry : public Node<INIEntry *>
{
public:
    INIEntry(const char *name, const char *value) : m_Key(strdup(name)), m_Value(strdup(value)) {}
    ~INIEntry();

    int32_t Index_ID() const { return CRC(m_Key); }

    const char *Get_Name() { return m_Key; }
    void Set_Name(const char *new_name);

    const char *Get_Value() { return m_Value; }
    void Set_Value(const char *new_value);

    int32_t CRC(const char *string) const { return Calculate_CRC(string, strlen(string)); }

private:
    char *m_Key;
    char *m_Value;
};

class INISection : public Node<INISection *>
{
public:
    INISection(const char *name) : m_Name(strdup(name)){}
    ~INISection();

    int32_t Index_ID() const { return CRC(m_Name); }

    INIEntry *Find_Entry(const char *entry) const;

    const char *Get_Name() const { return m_Name; }
    void Set_Name(const char *new_name);

    List<INIEntry *> &Entry_List() { return m_EntryList; }
    IndexClass<int, INIEntry *> &Entry_Index() { return m_EntryIndex; }

    int Get_Entry_Count() const { return m_EntryIndex.Count(); }

    int32_t CRC(const char *string) const { return Calculate_CRC(string, strlen(string)); }

private:
    char *m_Name;
    List<INIEntry *> m_EntryList;
    IndexClass<int, INIEntry *> m_EntryIndex;
};

class INIClass
{
public:
    enum
    {
        MAX_LINE_LENGTH = 128,
        MAX_BUF_SIZE = 4096,
        MAX_UUBLOCK_LINE_LENGTH = 70,
        MAX_TEXTBLOCK_LINE_LENGTH = 75
    };

public:
    INIClass() {}
    ~INIClass();

    BOOL Clear(const char *section = nullptr, const char *entry = nullptr);
    BOOL Is_Loaded() const { return m_SectionList.First()->Is_Valid(); }

    int Save(FileClass &file) const;
    int Save(Pipe &pipe) const;
    int Load(FileClass &file);
    int Load(Straw &straw);

    List<INISection *> &Get_Section_List() { return m_SectionList; }
    IndexClass<int, INISection *> &Get_Section_Index() { return m_SectionIndex; }

    bool Is_Present(const char *section, const char *entry = nullptr);

    // Returns the section object if it exists.
    INISection *Find_Section(const char *section) const;
    bool Section_Present(const char *section) const { return Find_Section(section) != nullptr; }
    int Section_Count() { return m_SectionIndex.Count(); }

    // Returns the entry object if it exists.
    INIEntry *Find_Entry(const char *section, const char *entry) const;
    int Entry_Count(const char *section) const;
    const char *Get_Entry(const char *section, int index) const;

    int Enumerate_Entries(const char *section, const char *entry_prefix, uint32_t start_number, uint32_t end_number);

    BOOL Put_UUBlock(const char *section, void *block, int length);
    int Get_UUBlock(const char *section, void *block, int length = 0) const;
    BOOL Put_TextBlock(const char *section, const char *text);
    int Get_TextBlock(const char *section, char *block, int length = 0) const;
    BOOL Put_Int(const char *section, const char *entry, int value, int format = INI_INTEGER_AS_DECIMAL);
    int Get_Int(const char *section, const char *entry, int defvalue = 0) const;
    BOOL Put_Bool(const char *section, const char *entry, BOOL value);
    BOOL Get_Bool(const char *section, const char *entry, BOOL defvalue = false) const;
    BOOL Put_Hex(const char *section, const char *entry, unsigned value);
    unsigned Get_Hex(const char *section, const char *entry, unsigned defvalue = 0) const;
    BOOL Put_Float(const char *section, const char *entry, double value);
    float Get_Float(const char *section, const char *entry, float defvalue = 0) const;
    BOOL Put_Double(const char *section, const char *entry, double value);
    double Get_Double(const char *section, const char *entry, double defvalue = 0) const;
    BOOL Put_String(const char *section, const char *entry, const char *string);
    int Get_String(const char *section, const char *entry, const char *defvalue = "", char *buffer = nullptr, int length = 0) const;
    int Get_String(const char *section, const char *entry, char *buffer = nullptr, int length = 0) const;
    BOOL Put_Fixed(const char *section, const char *entry, fixed_t value);
    const fixed_t Get_Fixed(const char *section, const char *entry, const fixed_t defvalue = 0) const;

#ifdef GAME_DLL
    int Hook_Load(Straw &straw) { return INIClass::Load(straw); }
#endif
private:
    static void Strip_Comments(char *line);
    static int32_t CRC(const char *string);

protected:
    List<INISection *> m_SectionList;
    IndexClass<int, INISection *> m_SectionIndex;
};

#endif // INI_H
