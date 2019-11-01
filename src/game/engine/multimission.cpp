/**
 * @file
 *
 * @author tomsons26
 *
 * @brief Class for storing basic mission map information.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "multimission.h"
#include <cstring>
    
using std::strlen;
using std::strncpy;

MultiMission::MultiMission(const char *filename, const char *description, const char *digest, int isofficial, int isexpansion)
{
    Set_Filename(filename);
    Set_Description(description);
    Set_Digest(digest);
    Set_Official(isofficial);
    Set_Expansion(isexpansion);
}

void MultiMission::Draw_It(int index, int xpos, int ypos, int xmax, int ymax, BOOL redraw_entry, TextPrintType style)
{
    // empty
}

void MultiMission::Set_Description(const char *desc)
{
    if (desc != nullptr){
        strlcpy(m_Description, desc, sizeof(m_Description));
    }
}

void MultiMission::Set_Filename(const char *filename)
{
    if (filename != nullptr){
        strlcpy(m_Filename, filename, sizeof(m_Filename));
    }
}

void MultiMission::Set_Digest(const char *digest)
{
    if (digest != nullptr) {
        strlcpy(m_Digest, digest, sizeof(m_Digest));
    } else {
        strcpy(m_Digest, "NODIGEST");
    }
}


void MultiMission::Set_Official(int official)
{
    m_IsOfficial = official;
}

void MultiMission::Set_Expansion(int expansion)
{
    m_IsExpansion = expansion;
}
