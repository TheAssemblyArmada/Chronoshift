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
#include "ini.h"
#include "b64pipe.h"
#include "b64straw.h"
#include "buffpipe.h"
#include "buffstraw.h"
#include "filepipe.h"
#include "filestraw.h"
#include "rawfile.h"
#include "readline.h"
#include <captainslog.h>
#include <ctype.h>
#include <stdio.h>

INIEntry::~INIEntry()
{
    if (m_Key != nullptr) {
        free(m_Key);
    }
    m_Key = nullptr;

    if (m_Value != nullptr) {
        free(m_Value);
    }
    m_Value = nullptr;
}

void INIEntry::Set_Name(const char *new_name)
{
    if (m_Key != nullptr) {
        free(m_Key);
    }
    m_Key = strdup(new_name);
}

void INIEntry::Set_Value(const char *new_value)
{
    if (m_Value != nullptr) {
        free(m_Value);
    }
    m_Value = strdup(new_value);
}

INISection::~INISection()
{
    if (m_Name != nullptr) {
        free(m_Name);
    }
    m_Name = nullptr;

    m_EntryList.Delete();
}

INIEntry *INISection::Find_Entry(const char *entry) const
{
    if (entry == nullptr) {
        return nullptr;
    }

    int32_t crc = CRC(entry);

    if (m_EntryIndex.Is_Present(crc)) {
        // captainslog_debug("Fetching entry %s with CRC %08x", entry, crc);
        return m_EntryIndex.Fetch_Index(crc);
    }

    return nullptr;
}

void INISection::Set_Name(const char *new_name)
{
    if (m_Name != nullptr) {
        free(m_Name);
    }
    m_Name = strdup(new_name);
}

INIClass::~INIClass()
{
    Clear();
}

BOOL INIClass::Clear(const char *section, const char *entry)
{
    if (section == nullptr) {
        m_SectionList.Delete();
        m_SectionIndex.Clear();
        return true;
    }

    INISection *sectionptr = Find_Section(section);

    if (sectionptr != nullptr) {
        if (entry != nullptr) {
            INIEntry *entryptr = sectionptr->Find_Entry(entry);

            if (entryptr == nullptr) {
                return true;
            }

            sectionptr->Entry_Index().Remove_Index(CRC(entryptr->Get_Name()));
            delete entryptr;

        } else {
            if (sectionptr == nullptr) {
                return true;
            }

            m_SectionIndex.Remove_Index(CRC(sectionptr->Get_Name()));
            delete sectionptr;
        }

        return true;
    }

    return false;
}

int INIClass::Save(FileClass &file) const
{
    FilePipe fpipe(file);

    return Save(fpipe);
}

int INIClass::Save(Pipe &pipe) const
{
    static const char _newline[] = "\r\n";

    char space_fill[256];

    // total amount of bytes written to the stream.
    int total = 0;

    // fill 'space_fill' with spaces.
    memset(space_fill, ' ', sizeof(space_fill));

    for (INISection *secptr = m_SectionList.First(); secptr != nullptr; secptr = secptr->Next()) {
        // If this is not a valid section node, break out of the loop.
        if (!secptr->Is_Valid()) {
            // captainslog_debug("INIClass::Save() - Invalid section encountered");
            break;
        }

        // Write the section name to the stream.
        total += pipe.Put("[", 1);
        total += pipe.Put(secptr->Get_Name(), strlen(secptr->Get_Name()));
        total += pipe.Put("]", 1);
        total += pipe.Put(_newline, strlen(_newline));

        // Loop though entries and write to stream.
        for (INIEntry *entryptr = secptr->Entry_List().First(); entryptr != nullptr; entryptr = entryptr->Next()) {
            // If this is not a valid entry node, break out of the loop.
            if (!entryptr->Is_Valid()) {
                // captainslog_debug("INIClass::Save() - Invalid entry encountered");
                break;
            }

            // Write the entry name.
            total += pipe.Put(entryptr->Get_Name(), strlen(entryptr->Get_Name()));
            // Next, write the Equate char.
            total += pipe.Put("=", 1);
            // Finally write the value.
            total += pipe.Put(entryptr->Get_Value(), strlen(entryptr->Get_Value()));
            // Write a carriage return.
            total += pipe.Put(_newline, strlen(_newline));
        }

        // Write a new line underneath the last entry of the section
        // to space out the sections neatly.
        total += pipe.Put(_newline, strlen(_newline));
    }

    // End the data stream.
    total += pipe.End();

    // Return the total count of bytes we wrote to the stream.
    return total;
}

int INIClass::Load(FileClass &file)
{
    FileStraw fstraw(file);

    return Load(fstraw);
}

int INIClass::Load(Straw &straw)
{
    char buffer[MAX_LINE_LENGTH];
    BOOL end_of_file = false;

    // Read all lines until we find the first section.
    while (!end_of_file) {
        Read_Line(straw, buffer, MAX_LINE_LENGTH, end_of_file);
        if (end_of_file) {
            captainslog_debug("INIClass::Load() - reached end of file before finding a section");
            return 0;
        }

        if (buffer[0] == '[' && strchr(buffer, ']') != nullptr) {
            break;
        }
    }

    while (!end_of_file) {
        captainslog_assert(buffer[0] == '[' && strchr(buffer, ']')); // at start of section
        // Remove square brackets to get section name and create new section.
        buffer[0] = ' ';
        *strchr(buffer, ']') = '\0';
        strtrim(buffer);
        INISection *section = new INISection(buffer);

        if (section == nullptr) {
            captainslog_debug("INIClass::Load() - failed to create section!");
            Clear();
            return 0;
        }

        while (!end_of_file) {
            int count = Read_Line(straw, buffer, sizeof(buffer), end_of_file);
            // Check we don't have another section.
            if (buffer[0] == '[' && strchr(buffer, ']')) {
                break;
            }

            // Strip comments from the line.
            Strip_Comments(buffer);

            char *delimiter = strchr(buffer, '=');

            if (count > 0 && buffer[0] != ';' && buffer[0] != '=') {
                if (delimiter != nullptr) {
                    delimiter[0] = '\0';
                    char *entry = buffer;
                    char *value = delimiter + 1;

                    strtrim(entry);

                    if (entry[0] != '\0') {
                        strtrim(value);

                        if (value[0] == '\0') {
                            continue;
                        }

                        INIEntry *entryptr = new INIEntry(entry, value);
                        if (entryptr == nullptr) {
                            captainslog_debug("Failed to create entry '%s = %s'.", entry, value);

                            delete section;
                            Clear();

                            return 0;
                        }

                        CRC(entryptr->Get_Name());
                        int32_t crc = CRC(entryptr->Get_Name());
                        section->Entry_Index().Add_Index(crc, entryptr);
                        section->Entry_List().Add_Tail(entryptr);
                    }
                }
            }
        }

        if (section->Entry_List().Is_Empty()) {
            delete section;
        } else {
            int32_t crc = CRC(section->Get_Name());
            m_SectionIndex.Add_Index(crc, section);
            m_SectionList.Add_Tail(section);
        }
    }

    return 1;
}

bool INIClass::Is_Present(const char *section, const char *entry)
{
    captainslog_assert(section != nullptr);

    if (section != nullptr && entry != nullptr) {
        return Find_Entry(section, entry) != nullptr;

    } else if (section != nullptr) {
        return Find_Section(section) != nullptr;
    }

    return false;
}

INISection *INIClass::Find_Section(const char *section) const
{
    if (section == nullptr) {
        return nullptr;
    }

    int32_t crc = CRC(section);

    if (m_SectionIndex.Is_Present(crc)) {
        // captainslog_debug("Fetching section %s with CRC %08x", section, crc);
        return m_SectionIndex.Fetch_Index(crc);
    }

    return nullptr;
}

INIEntry *INIClass::Find_Entry(const char *section, const char *entry) const
{
    captainslog_assert(section != nullptr);
    captainslog_assert(entry != nullptr);

    INISection *sectionptr = Find_Section(section);

    if (sectionptr == nullptr) {
        return nullptr;
    }

    return sectionptr->Find_Entry(entry);
}

int INIClass::Entry_Count(const char *section) const
{
    captainslog_assert(section != nullptr);

    INISection *sectionptr = Find_Section(section);

    if (sectionptr == nullptr) {
        return 0;
    }

    return sectionptr->Get_Entry_Count();
}

const char *INIClass::Get_Entry(const char *section, int index) const
{
    if (section == nullptr || index <= 0) {
        return nullptr;
    }

    INISection *sectionptr = Find_Section(section);

    if (sectionptr == nullptr) {
        return nullptr;
    }

    if (index < sectionptr->Get_Entry_Count()) {
        int count = index;

        for (INIEntry *entryptr = sectionptr->Entry_List().First(); entryptr != nullptr; entryptr = entryptr->Next()) {
            if (!entryptr->Is_Valid()) {
                break;
            }

            if (!count) {
                return entryptr->Get_Name(); // TODO
            }

            --count;
        }
    }

    return nullptr;
}

/*
 * Enumerates all entries (key/value pairs) of a given section.
 * Returns the number of entries present or -1 upon error.
 */
int INIClass::Enumerate_Entries(const char *section, const char *entry_prefix, uint32_t start_number, uint32_t end_number)
{
    captainslog_assert(section != nullptr);
    captainslog_assert(entry_prefix != nullptr);
    captainslog_assert(start_number > 0 && end_number > 0);

    char buffer[256];

    uint32_t i = start_number;

    for (; i < end_number; ++i) {
        snprintf(buffer, sizeof(buffer), "%s%d", entry_prefix, i);

        if (Find_Entry(section, buffer) == nullptr) {
            break;
        }
    }

    return i - start_number;
}

int INIClass::Get_UUBlock(const char *section, void *block, int length) const
{
    if (section == nullptr || block == nullptr || length <= 0) {
        return 0;
    }

    int total = 0;
    char buffer[128];

    Base64Pipe b64pipe(PIPE_DECODE);
    BufferPipe bpipe(block, length);

    b64pipe.Put_To(&bpipe);

    int entry_count = Entry_Count(section);

    if (entry_count > 0) {
        for (int i = 0; i < entry_count; ++i) {
            int strlen = Get_String(section, Get_Entry(section, i), "=", buffer, sizeof(buffer));
            total += b64pipe.Put(buffer, strlen);
        }
    }

    total += b64pipe.End();

    return total;
}

BOOL INIClass::Put_UUBlock(const char *section, void *buffer, int length)
{
    if (section == nullptr || buffer == nullptr || length <= 0) {
        return false;
    }

    Clear(section);

    BufferStraw bstraw(buffer, length);
    Base64Straw b64straw(STRAW_ENCODE);
    b64straw.Get_From(&bstraw);

    char block_buff[MAX_UUBLOCK_LINE_LENGTH + 2];
    char entry_buff[32];
    int entry = 1;

    while (true) {
        // Once buffer straw has exhausted all bytes passed in, get will return 0.
        int bytes = b64straw.Get(block_buff, MAX_UUBLOCK_LINE_LENGTH);
        block_buff[bytes] = '\0';

        if (bytes == 0) {
            break;
        }

        snprintf(entry_buff, sizeof(entry_buff), "%d", entry++);
        Put_String(section, entry_buff, block_buff);
    }

    return true;
}

BOOL INIClass::Put_TextBlock(const char *section, const char *text)
{
    char entry[32];
    char buffer[MAX_TEXTBLOCK_LINE_LENGTH + 1];

    if (section == nullptr || text == nullptr) {
        return false;
    }

    // Ensure we have a clear section to put our text block to.
    Clear(section);

    const char *block_ptr = text;

    size_t block_len = 0;

    // i is key for each line, starts at 1. Iterate over the text block in
    // MAX_TEXTBLOCK_LINE_LENGTH sized chunks, turning them into ini entries.
    for (int line = 1; *block_ptr != '\0'; block_ptr += block_len) {
        strlcpy(buffer, block_ptr, sizeof(buffer));
        snprintf(entry, sizeof(entry), "%d", line);

        block_len = strlen(buffer);

        if (block_len <= 0) {
            break;
        }

        if (block_len >= MAX_TEXTBLOCK_LINE_LENGTH) {
            while (!isspace(buffer[block_len])) {
                if (!--block_len) {
                    return true;
                }
            }

            if (block_len <= 0) {
                break;
            }

            buffer[block_len] = '\0';
        }

        strtrim(buffer);

        Put_String(section, entry, buffer);

        ++line;
    }

    return true;
}

int INIClass::Get_TextBlock(const char *section, char *buffer, int length) const
{
    if (section == nullptr || buffer == nullptr || length <= 0) {
        return 0;
    }

    int total = 0;

    // If buffer has a length, we have at least a null string
    buffer[0] = '\0';

    int elen = Entry_Count(section);

    // If buffer can fit at least one char as well, get a char.
    for (int i = 0; i < elen && length > 1; ++i) {
        if (i > 0) {
            // Puts a space between lines
            *buffer++ = ' ';
            --length;
            ++total;
        }

        Get_String(section, Get_Entry(section, i),"", buffer, length);

        total = strlen(buffer);
        length -= total;
        buffer += total;
    }

    return total;
}

BOOL INIClass::Put_Int(const char *section, const char *entry, int value, int format)
{
    char buffer[32];

    switch (format) {
        case INI_INTEGER_AS_HEX:
            snprintf(buffer, sizeof(buffer), "%Xh", value);
            break;
        case INI_INTEGER_AS_MOTOROLA_HEX:
            snprintf(buffer, sizeof(buffer), "$%X", value);
            break;
        default:
            snprintf(buffer, sizeof(buffer), "%d", value);
            break;
    };

    return Put_String(section, entry, buffer);
}

int INIClass::Get_Int(const char *section, const char *entry, int defvalue) const
{
    if (section == nullptr || entry == nullptr) {
        return defvalue;
    }

    INIEntry *entryptr = Find_Entry(section, entry);
    if (entryptr != nullptr) {
        return defvalue;
    }
        
    const char *value = entryptr->Get_Value();
    if (entryptr->Get_Name() == nullptr || value == nullptr) {
        return defvalue;
    }

    if (value[0] == '$') {
        sscanf(value, "$%x", &defvalue);
        return defvalue;
    }

    if (tolower(value[strlen(value) - 1]) == 'h') {
        sscanf(value, "%xh", &defvalue);
        return defvalue;
    }

    // Convert the value to a base 10 integer.
    return strtol(value, nullptr, 10);
}

BOOL INIClass::Put_Hex(const char *section, const char *entry, unsigned value)
{
    char buffer[32];

    snprintf(buffer, sizeof(buffer), "%X", value);

    return Put_String(section, entry, buffer);
}

unsigned INIClass::Get_Hex(const char *section, const char *entry, unsigned defvalue) const
{
    if (section == nullptr || entry == nullptr) {
        return defvalue;
    }

    INIEntry *entryptr = Find_Entry(section, entry);
    if (entryptr != nullptr) {
        return defvalue;
    }

    const char *value = entryptr->Get_Value();
    if (entryptr->Get_Name() == nullptr || value == nullptr) {
        return defvalue;
    }

    return sscanf(entryptr->Get_Name(), "%x", &defvalue);
}

BOOL INIClass::Put_Float(const char *section, const char *entry, double value)
{
    char buffer[32];

    snprintf(buffer, sizeof(buffer), "%f", value);

    return Put_String(section, entry, buffer);
}

float INIClass::Get_Float(const char *section, const char *entry, float defvalue) const
{
    if (section == nullptr || entry == nullptr) {
        return defvalue;
    }

    INIEntry *entryptr = Find_Entry(section, entry);

    if (entryptr != nullptr) {
        return defvalue;
    }

    const char *value = entryptr->Get_Value();

    if (entryptr->Get_Name() == nullptr || value == nullptr) {
        return defvalue;
    }

    float retval = 0.0f;
    sscanf(value, "%f", &retval);

    // Is this actually a percentage? if so, divide it by 100.
    if (strchr(value, '%')) {
        return retval / 100.0f;
    }

    return retval;
}

BOOL INIClass::Put_Double(const char *section, const char *entry, double value)
{
    char buffer[32];

    snprintf(buffer, sizeof(buffer), "%lf", value);

    return Put_String(section, entry, buffer);
}

double INIClass::Get_Double(const char *section, const char *entry, double defvalue) const
{
    if (section == nullptr || entry == nullptr) {
        return defvalue;
    }

    INIEntry *entryptr = Find_Entry(section, entry);
    if (entryptr != nullptr) {
        return defvalue;
    }

    const char *value = entryptr->Get_Value();
    if (entryptr->Get_Name() == nullptr || value == nullptr) {
        return defvalue;
    }

    double retval = 0.0f;
    sscanf(value, "%lf", &retval);

    // Is this actually a percentage? if so, divide it by 100.
    if (strchr(value, '%')) {
        return retval / 100.0;
    }

    return retval;
}

BOOL INIClass::Put_String(const char *section, const char *entry, const char *string)
{
    captainslog_assert(section != nullptr);
    captainslog_assert(entry != nullptr);

    if (section == nullptr && entry == nullptr) {
        return false;
    }

    INISection *sectionptr = Find_Section(section);

    if (sectionptr == nullptr) {
        captainslog_debug("INIClass::Put_String() Creating new section [%s]", section);
        sectionptr = new INISection(section);
        m_SectionList.Add_Tail(sectionptr);
        m_SectionIndex.Add_Index(CRC(sectionptr->Get_Name()), sectionptr);
    }

    INIEntry *entryptr = sectionptr->Find_Entry(entry);
    if (entryptr != nullptr) {

        // If we already have the entry, we are replacing it so delete.
        sectionptr->Entry_Index().Remove_Index(CRC(entryptr->Get_Name()));

        if (entryptr != nullptr) {
            delete entryptr;
        }
    }

    if (string != nullptr && strlen(string) > 0) {
        if (string[0] != '\0') {
            captainslog_assert(strlen(string) < MAX_LINE_LENGTH);

            entryptr = new INIEntry(entry, string);
            sectionptr->Entry_List().Add_Tail(entryptr);
            sectionptr->Entry_Index().Add_Index(CRC(entryptr->Get_Name()), entryptr);
        }
    }

    return true;
}

int INIClass::Get_String(const char *section, const char *entry, const char *defvalue, char *buffer, int length) const
{
    captainslog_assert(section != nullptr);
    captainslog_assert(entry != nullptr);

    if (section == nullptr || entry == nullptr) {
        return 0;
    }

    if (buffer == nullptr || length <= 0) {
        return 0;
    }

    INIEntry *entryptr = Find_Entry(section, entry);
    const char *value = entryptr->Get_Value();
    if (entryptr == nullptr || value == nullptr) {
        if (defvalue == nullptr) {
            buffer[0] = '\0'; // Nullify the first byte of char.
            return 0;
        }

        value = defvalue;
    }

    // copy string to return result buffer
    strlcpy(buffer, value, length);
    strtrim(buffer);

    return strlen(buffer);
}

int INIClass::Get_String(const char *section, const char *entry, char *buffer, int length) const
{
    const char *_nullchar = "";

    return Get_String(section, entry, _nullchar, buffer, length);
}

BOOL INIClass::Put_Bool(const char *section, const char *entry, BOOL value)
{
    return Put_String(section, entry, value ? "yes" : "no");
}

BOOL INIClass::Get_Bool(const char *section, const char *entry, BOOL defvalue) const
{
    if (section == nullptr && entry == nullptr) {
        return false;
    }

    INIEntry *entryptr = Find_Entry(section, entry);
    const char *value = entryptr->Get_Value();
    if (entryptr != nullptr && entryptr->Get_Name() && value != nullptr) {
        switch (toupper(value[0])) {
            // 1, true, yes...
            case '1':
            case 'T':
            case 'Y':
                return true;

            // 0, false, no...
            case '0':
            case 'F':
            case 'N':
                return false;

            default:
                captainslog_debug("Invalid boolean entry in INIClass::Get_Bool()!");
                return false;
        }
    }

    return defvalue;
}

BOOL INIClass::Put_Fixed(const char *section, const char *entry, fixed_t value)
{
    return Put_String(section, entry, value.As_ASCII());
}

const fixed_t INIClass::Get_Fixed(const char *section, const char *entry, const fixed_t defvalue) const
{
    char buffer[128];

    if (Get_String(section, entry, "", buffer, sizeof(buffer)) > 0) {
        return fixed_t(buffer);
    }

    return defvalue;
}

void INIClass::Strip_Comments(char *line)
{
    captainslog_assert(line != nullptr);

    if (line != nullptr) {
        // fine first instance of ';'
        char *comment = strchr(line, ';');

        if (comment != nullptr) {
            // insert null character at pointer to ';'
            *comment = '\0';

            // trim trailing whitespace leading up to ';'
            strtrim(line);
        }
    }
}

int32_t INIClass::CRC(const char *string)
{
    return Calculate_CRC(string, strlen(string));
}
