/**
 * @file
 *
 * @Author CCHyper, OmniBlade
 *
 * @brief INI file parsing class.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "ini.h"
#include "b64pipe.h"
#include "b64straw.h"
#include "buffpipe.h"
#include "buffstraw.h"
//#include "cstraw.h"
#include "filepipe.h"
#include "filestraw.h"
#include "rawfileclass.h"
#include "readline.h"
#include <ctype.h>
#include <stdio.h>

#ifndef RAPP_STANDALONE
#include "hooker.h"
#endif

INIEntry::~INIEntry()
{
    if (m_key != nullptr) {
        free((void*)m_key);
    }

    m_key = nullptr;

    if (m_value != nullptr) {
        free((void*)m_value);
    }

    m_value = nullptr;
}

void INIEntry::Set_Name(const char *new_name)
{
    if (m_key != nullptr) {
        free((void*)m_key);
    }

    m_key = strdup(new_name);
}

void INIEntry::Set_Value(const char *str)
{
    if (m_value != nullptr) {
        free((void*)m_value);
    }

    m_value = strdup(str);
}

INISection::~INISection()
{
    if (m_sectionName != nullptr) {
        free((void*)m_sectionName);
    }
    m_sectionName = nullptr;

    m_entryList.Delete();
}

INIEntry *INISection::Find_Entry(const char *entry) const
{
    int crc;

    DEBUG_ASSERT(entry != nullptr);

    // this function setup needs switching around. errors first etc.
    if (entry != nullptr && (crc = CRC(entry), m_entryIndex.Is_Present(crc))) {
        // DEBUG_LOG("Fetching entry %s with CRC %08x\n", entry, crc);
        return m_entryIndex.Fetch_Index(crc);
    }
    return nullptr;
}

void INISection::Set_Name(const char *str)
{
    if (m_sectionName != nullptr) {
        free((void*)m_sectionName);
    }

    m_sectionName = strdup(str);
}

INIClass::~INIClass()
{
    Clear();
}

BOOL INIClass::Clear(const char *section, const char *entry)
{
    INISection *sectionptr;
    INIEntry *entryptr;

    if (section) {
        if ((sectionptr = Find_Section(section)) != nullptr) {
            if (entry) {
                if (!(entryptr = sectionptr->Find_Entry(entry))) {
                    return true;
                }

                sectionptr->m_entryIndex.Remove_Index(CRC(entryptr->Get_Name()));
                delete entryptr;

            } else {
                if (!sectionptr) {
                    return true;
                }

                m_sectionIndex.Remove_Index(CRC(sectionptr->Get_Name()));
                delete sectionptr;
            }

            return true;
        }

        return false;
    }
    
    m_sectionList.Delete();
    m_sectionIndex.Clear();

    return true;
}

int INIClass::Save(FileClass &file) const
{
    FilePipe fpipe(file);

    return Save(fpipe);
}

int INIClass::Save(Pipe &pipe) const
{
    char space_fill[256];

    // total amount of bytes written to the stream.
    int total = 0;

    // fill 'space_fill' with spaces.
    memset(space_fill, ' ', sizeof(space_fill));

    for (INISection *secptr = m_sectionList.First(); secptr != nullptr; secptr = secptr->Next()) {
        // If this is not a valid section node, break out of the loop.
        if (!secptr->Is_Valid()) {
            // DEBUG_LOG("INIClass::Save() - Invalid section encountered\n");
            break;
        }

        // Write the section name to the stream.
        total += pipe.Put("[", 1);
        total += pipe.Put(secptr->Get_Name(), strlen(secptr->Get_Name()));
        total += pipe.Put("]", 1);
        total += pipe.Put(SYS_NEW_LINE, strlen(SYS_NEW_LINE));

        // Loop though entries and write to stream.
        for (INIEntry *entryptr = secptr->m_entryList.First(); entryptr != nullptr; entryptr = entryptr->Next()) {
            // If this is not a valid entry node, break out of the loop.
            if (!entryptr->Is_Valid()) {
                // DEBUG_LOG("INIClass::Save() - Invalid entry encountered\n");
                break;
            }

            // Write the entry name.
            total += pipe.Put(entryptr->Get_Name(), (int)strlen(entryptr->Get_Name()));
            // Next, write the Equate char.
            total += pipe.Put("=", 1);
            // Finally write the value.
            total += pipe.Put(entryptr->Get_Value(), (int)strlen(entryptr->Get_Value()));
            // Write a carriage return.
            total += pipe.Put(SYS_NEW_LINE, (int)strlen(SYS_NEW_LINE));
        }

        // Write a new line underneath the last entry of the section
        // to space out the sections neatly.
        total += pipe.Put(SYS_NEW_LINE, (int)strlen(SYS_NEW_LINE));
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
    //char section[64];
    BOOL merge = false;
    BOOL end_of_file = false;

    // Read all lines until we find the first section.
    while (!end_of_file) {
        Read_Line(straw, buffer, MAX_LINE_LENGTH, end_of_file);
        if (end_of_file) {
            DEBUG_LOG("INIClass::Load() - reached end of file before finding a section\n");
            return INI_LOAD_INVALID;
        }

        if (buffer[0] == '[' && strchr(buffer, ']') != nullptr) {
            break;
        }
    }

    while (!end_of_file) {
        DEBUG_ASSERT(buffer[0] == '[' && strchr(buffer, ']')); // at start of section
        // Remove square brackets to get section name and create new section.
        buffer[0] = ' ';
        *strchr(buffer, ']') = '\0';
        strtrim(buffer);
        INISection *section = new INISection(buffer);

        if (section == nullptr) {
            DEBUG_LOG("INIClass::Load() - failed to create section!\n");

            Clear();

            return INI_LOAD_INVALID;
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
                        if (!entryptr) {
                            DEBUG_LOG("Failed to create entry '%s = %s'.\n", entry, value);

                            delete section;
                            Clear();

                            return INI_LOAD_INVALID;
                        }
                        
                        // Is this Name, Value or something?
                        CRC(entryptr->Get_Name());
                        int32_t crc = CRC(entryptr->Get_Name());

                        if (section->m_entryIndex.Is_Present(crc)) {
                            // Duplicate_CRC_Error(__FUNCTION__, section->Get_Name(), entryptr->Get_Name());
                        }

                        section->m_entryIndex.Add_Index(crc, entryptr);
                        section->m_entryList.Add_Tail(entryptr);
                    }
                }
            }
        }

        if (section->m_entryList.Is_Empty()) {
            delete section;
        } else {
            int32_t crc = CRC(section->Get_Name());
            m_sectionIndex.Add_Index(crc, section);
            m_sectionList.Add_Tail(section);
        }
    }

    return INI_LOAD_OVERWRITE;
}

INISection *INIClass::Find_Section(const char *section) const
{
    DEBUG_ASSERT(section != nullptr);

    int crc;

    if (section != nullptr && (crc = CRC(section)) && m_sectionIndex.Is_Present(crc)) {
        return m_sectionIndex.Fetch_Index(crc);
    }

    return nullptr;
}

INIEntry *INIClass::Find_Entry(const char *section, const char *entry) const
{
    DEBUG_ASSERT(section != nullptr);
    DEBUG_ASSERT(entry != nullptr);

    INISection *sectionptr = Find_Section(section);

    if (sectionptr != nullptr) {
        return sectionptr->Find_Entry(entry);
    }

    return nullptr;
}

int INIClass::Entry_Count(const char *section) const
{
    DEBUG_ASSERT(section != nullptr);

    INISection *sectionptr = Find_Section(section);

    if (sectionptr != nullptr) {
        return sectionptr->Get_Entry_Count();
    }

    return 0;
}

const char *INIClass::Get_Entry(const char *section, int index) const
{
    DEBUG_ASSERT(section != nullptr);

    INISection *sectionptr = Find_Section(section);

    if (sectionptr != nullptr) {
        int count = index;

        if (index < sectionptr->Get_Entry_Count()) {
            for (INIEntry *entryptr = sectionptr->m_entryList.First(); entryptr != nullptr; entryptr = entryptr->Next()) {
                if (!entryptr->Is_Valid()) {
                    break;
                }

                if (!count) {
                    return entryptr->Get_Name(); // TODO
                }

                count--;
            }
        }
    }

    return nullptr;
}

int INIClass::Enumerate_Entries(const char *section, const char *entry_prefix, uint32_t start_number, uint32_t end_number)
{
    char buffer[256];
    uint32_t i = start_number;

    DEBUG_ASSERT(section != nullptr);
    DEBUG_ASSERT(!start_number && !end_number);

    for (; i < end_number; ++i) {
        snprintf(buffer, sizeof(buffer), "%s%d", entry_prefix, i);

        if (!Find_Entry(section, buffer)) {
            break;
        }
    }

    return i - start_number;
}

int INIClass::Get_UUBlock(const char *section, void *block, int length) const
{
    DEBUG_ASSERT(length > 0);
    DEBUG_ASSERT(block != nullptr);

    int total = 0;
    char buffer[128];

    if (section != nullptr) {
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
    }

    return total;
}

BOOL INIClass::Put_UUBlock(const char *section, void *buffer, int length)
{
    DEBUG_ASSERT(length > 0);
    DEBUG_ASSERT(buffer != nullptr);

    if (section != nullptr && buffer != nullptr && length > 0) {
        Clear(section);

        BufferStraw bstraw(buffer, length);
        Base64Straw b64straw(STRAW_ENCODE);
        b64straw.Get_From(&bstraw);

        char block_buff[MAX_UUBLOCK_LINE_LENGTH];
        char entry_buff[32];
        int entry = 1;

        while (true) {
            // Once buffer straw has exhausted all bytes passed in, get will return 0.
            int bytes = b64straw.Get(block_buff, sizeof(block_buff) - 2);
            block_buff[bytes] = '\0';

            if (bytes == 0) {
                break;
            }

            snprintf(entry_buff, sizeof(entry_buff), "%d", entry++);
            Put_String(section, entry_buff, block_buff);
        }

        return true;
    }

    return false;
}

BOOL INIClass::Put_TextBlock(const char *section, const char *text)
{
    DEBUG_ASSERT(text != nullptr);

    char entry[32];
    char buffer[MAX_TEXTBLOCK_LINE_LENGTH];

    if (section != nullptr && text != nullptr) {
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

            if (block_len >= sizeof(buffer)) {
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

    return false;
}

int INIClass::Get_TextBlock(const char *section, char *buffer, int length) const
{
    DEBUG_ASSERT(buffer != nullptr);
    DEBUG_ASSERT(length != 0);

    int total = 0;

    if (section != nullptr && buffer != nullptr && length > 0) {
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

            total = (int)strlen(buffer);
            length -= total;
            buffer += total;
        }
    }

    return total;
}

BOOL INIClass::Put_Int(const char *section, const char *entry, int value, int format)
{
    char buffer[512];

    if (format == INIINTEGER_AS_HEX) {
        sprintf(buffer, "%Xh", value);
    } else if (format == INIINTEGER_AS_MOTOROLA_HEX) {
        sprintf(buffer, "$%X", value);
    } else {
        sprintf(buffer, "%d", value);
    }

    return Put_String(section, entry, buffer);
}

int INIClass::Get_Int(const char *section, const char *entry, int defvalue) const
{
    INIEntry *entryptr;
    const char *value;

    if (section != nullptr && entry && (entryptr = Find_Entry(section, entry)) && entryptr->Get_Name()
        && (value = entryptr->Get_Value())) {
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

    return defvalue;
}

BOOL INIClass::Put_Hex(const char *section, const char *entry, int value)
{
    char buffer[32];

    sprintf(buffer, "%X", (unsigned)value);

    return Put_String(section, entry, buffer);
}

int INIClass::Get_Hex(const char *section, const char *entry, int defvalue) const
{
    INIEntry *entryptr;

    if (section && entry && (entryptr = Find_Entry(section, entry)) != nullptr && *(entryptr->Get_Value())) {
        return sscanf(entryptr->Get_Name(), "%x", (unsigned *)&defvalue);
    }

    return defvalue;
}

BOOL INIClass::Put_Float(const char *section, const char *entry, double value)
{
    char buffer[32];

    sprintf(buffer, "%f", value);

    return Put_String(section, entry, buffer);
}

float INIClass::Get_Float(const char *section, const char *entry, float defvalue) const
{
    INIEntry *entryptr;

    if (section != nullptr && entry && (entryptr = Find_Entry(section, entry)) && *(entryptr->Get_Value())) {
        sscanf(entryptr->Get_Value(), "%f", &defvalue);

        // Is this actually a percentage? if so, divide it by 100
        if (strchr(entryptr->Get_Value(), '%')) {
            return defvalue / (float)100.0;
        }
    }

    return defvalue;
}

BOOL INIClass::Put_Double(const char *section, const char *entry, double value)
{
    char buffer[32];

    sprintf(buffer, "%lf", value);

    return Put_String(section, entry, buffer);
}

double INIClass::Get_Double(const char *section, const char *entry, double defvalue) const
{
    INIEntry *entryptr;

    if (section && entry && (entryptr = Find_Entry(section, entry)) && *(entryptr->Get_Value())) {
        sscanf(entryptr->Get_Value(), "%lf", &defvalue);

        //
        // Is this value really a percentage? if so, divide it by 100.
        //
        if (strchr(entryptr->Get_Value(), '%')) {
            return defvalue / 100.0;
        }
    }

    return defvalue;
}

BOOL INIClass::Put_String(const char *section, const char *entry, const char *string)
{
    DEBUG_ASSERT(section != nullptr);
    DEBUG_ASSERT(entry != nullptr);

    INISection *sectionptr;
    INIEntry *entryptr;

    if (section != nullptr && entry != nullptr) {
        if (!(sectionptr = Find_Section(section))) {
            DEBUG_LOG("INIClass::Put_String() Creating new section [%s]\n", section);
            sectionptr = new INISection(section);
            m_sectionList.Add_Tail(sectionptr);
            m_sectionIndex.Add_Index(CRC(sectionptr->Get_Name()), sectionptr);
        }

        if ((entryptr = sectionptr->Find_Entry(entry))) {
            // TODO needs rewriting, see BMFE or Ren
            if (strcmp(entryptr->Get_Name(), entry) == 0) {
                // Duplicate_CRC_Error(__CURRENT_FUNCTION__, section, entry);
            } else {
                // Duplicate_CRC(__CURRENT_FUNCTION__, section, entry);
            }

            // If we already have the entry, we are replacing it so delete
            sectionptr->m_entryIndex.Remove_Index(CRC(entryptr->Get_Name()));

            if (entryptr != nullptr) {
                delete entryptr;
            }
        }

        if (string != nullptr && strlen(string) > 0) {
            if (string[0] != '\0' /*|| KeepBlankEntries*/) {
                DEBUG_ASSERT(strlen(string) < MAX_LINE_LENGTH);

                entryptr = new INIEntry(entry, string);
                sectionptr->m_entryList.Add_Tail(entryptr);
                sectionptr->m_entryIndex.Add_Index(CRC(entryptr->Get_Name()), entryptr);
            }
        }

        return true;
    }

    return false;
}

int INIClass::Get_String(const char *section, const char *entry, const char *defvalue, char *buffer, int length) const
{
    INIEntry *entryptr;
    const char *value = defvalue;

    DEBUG_ASSERT(section != nullptr);
    DEBUG_ASSERT(entry != nullptr);
    DEBUG_ASSERT(buffer != nullptr);
    DEBUG_ASSERT(length > 0);

    if (buffer != nullptr && length > 0 && section != nullptr && entry != nullptr) {
        if ((entryptr = Find_Entry(section, entry)) == nullptr || (value = entryptr->Get_Value()) == nullptr) {
            if (defvalue == nullptr) {
                buffer[0] = '\0'; // nullify the first byte of char
                return 0;
            }

            value = defvalue;
        }

        // copy string to return result buffer
        strlcpy(buffer, value, length);
        strtrim(buffer);

        return (int)strlen(buffer);
    }

    return 0;
}

BOOL INIClass::Put_Bool(const char *section, const char *entry, BOOL value)
{

    if (value) {
        return Put_String(section, entry, "true");
    }

    return Put_String(section, entry, "false");
}

BOOL const INIClass::Get_Bool(const char *section, const char *entry, BOOL defvalue) const
{
    INIEntry *entryptr;
    const char *value;

    DEBUG_ASSERT(section != nullptr);
    DEBUG_ASSERT(entry != nullptr);

    if (section != nullptr && entry != nullptr) {
        if ((entryptr = Find_Entry(section, entry)) && entryptr->Get_Name() && (value = entryptr->Get_Value())) {
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
                    DEBUG_LOG("Invalid boolean entry in INIClass::Get_Bool()!");
                    return false;
            }
        }
    }

    return defvalue;
}

BOOL INIClass::Put_Fixed(const char *section, const char *entry, fixed value)
{
    return Put_String(section, entry, value.As_ASCII());
}

fixed const INIClass::Get_Fixed(const char *section, const char *entry, fixed defvalue) const
{
    char buffer[128];

    if (Get_String(section, entry, "", buffer, sizeof(buffer)) > 0) {
        return fixed(buffer);
    }

    return defvalue;
}

BOOL INIClass::Put_PKey(PKey &key)
{
    char buffer[sizeof(PKey)];

    // Put public key
    Put_UUBlock("PublicKey", &buffer, key.Encode_Modulus(&buffer));

    // Put private key
    Put_UUBlock("PrivateKey", &buffer, key.Encode_Exponent(&buffer));

    return true;
}

PKey INIClass::Get_PKey(BOOL fast) const
{
    PKey key;
    uint8_t buffer[512];

    if (fast) {
        DEBUG_LOG("INIClass::Get_PKey() - Preparing PublicKey...\n");
        Int<MAX_UNIT_PRECISION> exp(0x10001);
        MPMath::DER_Encode(exp, buffer, MAX_UNIT_PRECISION);
    } else {
        DEBUG_LOG("INIClass::Get_PKey() - Loading PrivateKey\n");
        Get_UUBlock("PrivateKey", buffer, sizeof(buffer));
    }

    DEBUG_LOG("INIClass::Get_PKey() - Decoding Exponent\n");
    key.Decode_Exponent(buffer);

    DEBUG_LOG("INIClass::Get_PKey() - Loading PublicKey\n");

    Get_UUBlock("PublicKey", buffer, sizeof(buffer));

    DEBUG_LOG("INIClass::Get_PKey() - Decoding Modulus\n");
    key.Decode_Modulus(buffer);

    return key;
}

void INIClass::Strip_Comments(char *line)
{
    DEBUG_ASSERT(line != nullptr);

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

int32_t const INIClass::CRC(const char *string)
{
    return Calculate_CRC(string, (int)strlen(string));
}

#ifndef RAPP_STANDALONE
void INIClass::Hook_Me()
{
    //Hook_Function((void*)0x004F2900, (void*)&Hook_Load);
}

int INIClass::Hook_Load(INIClass *ptr, Straw &straw)
{
    return ptr->Load(straw);
}
#endif
