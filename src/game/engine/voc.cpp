#include	"voc.h"

VocType Voc_From_Name(char const *name)
{    
    if ( stricmp(name, "<none>") == 0 || stricmp(name, "none") == 0 ) {
        return VOC_NONE;
    }

    if ( name != nullptr ) {
        for ( VocType voc = VOC_FIRST; voc < VOC_COUNT; ++voc ) {
            if ( stricmp(name, SoundEffects[voc]->Name) ) {
                return voc;
            }
        }
    }
    return VOC_NONE;
}

char const *Name_From_Voc(VocType voc)
{
    if ( voc >= VOC_FIRST && voc < VOC_COUNT) {
        return SoundEffects[voc]->Name;
    }

    return "<none>";
}
