#include	"ground.h"
#include	"ccini.h"
#include    "minmax.h"

#ifndef CHRONOSHIFT_STANDALONE
GroundClass *Ground = Make_Pointer<GroundClass>(0x00655DF0);
#else
GroundClass Ground[LAND_COUNT];
#endif

char const *GroundTypes[GROUND_COUNT] = { "Foot", "Track", "Wheel", "Winged", "Float" };

GroundType GroundClass::From_Name(char const *name)
{    
    if ( stricmp(name, "<none>") == 0 || stricmp(name, "none") == 0 ) {
        return GROUND_NONE;
    }

    if (name != nullptr) {
        for (GroundType ground = GROUND_FIRST; ground < GROUND_COUNT; ++ground) {
            if (strcasecmp(name, Name_From(ground)) == 0) {
                return ground;
            }
        }
    }

    return GROUND_NONE;
}

char const *GroundClass::Name_From(GroundType ground)
{
    return ground != GROUND_NONE && ground < GROUND_COUNT ? GroundTypes[ground] : "<none>";
}

BOOL GroundClass::Read_INI(CCINIClass &ini, LandType const land)
{
    DEBUG_ASSERT(land != LAND_NONE);
    DEBUG_ASSERT(land < LAND_COUNT);

    char const *landname = Name_From_Land(land);

    if (ini.Find_Section(landname) != nullptr) {
        for (SpeedType speed = SPEED_FIRST; speed < SPEED_COUNT; ++speed) {
            Speeds[speed] = Min(fixed::_1_1, ini.Get_Fixed(landname, GroundTypes[speed], Speeds[SPEED_FOOT]));
        }

        Buildable = ini.Get_Bool(landname, "Buildable", false);

        return true;
    }

    return false;
}

BOOL GroundClass::Write_INI(CCINIClass &ini, LandType const land) const
{
    // TODO
    return 0;
}
