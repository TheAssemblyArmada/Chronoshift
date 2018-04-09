#include	"source.h"

char const *SourceName[SOURCE_COUNT] = { "North", "East", "South", "West", "Air" };

SourceType Source_From_Name(char const *name)
{
    if (strcasecmp(name, "<none>") == 0 || strcasecmp(name, "none") == 0) {
        return SOURCE_NONE;
    }

    if (name != nullptr) {
        for (SourceType source = SOURCE_FIRST; source < SOURCE_COUNT; ++source) {
            if (strcasecmp(name, SourceName[source]) == 0) {
                return source;
            }
        }
    }

    return SOURCE_NONE;
}

char const *Name_From_Source(SourceType source)
{
    if (source < SOURCE_COUNT && source != SOURCE_NONE) {
        return SourceName[source];
    }
    return "<none>";
}
