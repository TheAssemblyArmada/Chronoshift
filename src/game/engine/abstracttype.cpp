#include "abstracttype.h"

AbstractTypeClass::AbstractTypeClass(RTTIType type, int id, int uiname, const char *name) :
    RTTI(type),
    HeapID(id),
    UIName(uiname)
{
    strlcpy(Name, name, sizeof(Name));
}

AbstractTypeClass::AbstractTypeClass(AbstractTypeClass const &that) :
    RTTI(that.RTTI),
    HeapID(that.HeapID),
    UIName(that.UIName)
{
    strlcpy(Name, that.Name, sizeof(Name));
}

int AbstractTypeClass::Full_Name() const
{
    for (int i = 0; i < 25; ++i) {
        if (100 * (RTTI + 1) + HeapID == NameIDOverride[i]) {
            return -(i + 1);
        }
    }

    return UIName;
}