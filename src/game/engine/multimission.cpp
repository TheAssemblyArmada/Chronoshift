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

void MultiMission::Draw_It(int unknown, int xpos, int ypos, int xmax, int ymax, int somestyle, TextPrintType style)
{
    //maybe TODO, unused function
}

void MultiMission::Set_Description(const char *desc)
{
    if (desc != nullptr){
        strlcpy(Description, desc, sizeof(Description));
    }
}

void MultiMission::Set_Filename(const char *filename)
{
    if (filename != nullptr){
        strlcpy(Filename, filename, sizeof(Filename));
    }
}

void MultiMission::Set_Digest(const char *digest)
{
  if (digest != nullptr)
  {
    strlcpy(Digest, digest, sizeof(Digest));
  } else {
    strcpy(Digest, "NODIGEST");
  }
}


void MultiMission::Set_Official(int official)
{
    IsOfficial = official;
}

void MultiMission::Set_Expansion(int expansion)
{
    IsExpansion = expansion;
}
