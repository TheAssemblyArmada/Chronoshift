#include "aircrafttype.h"
#include "buildingtype.h"
#include "infantrytype.h"
#include "technotype.h"
#include "unittype.h"
#include "vesseltype.h"

TechnoTypeClass *Fetch_Techno_Type(RTTIType type, int id)
{
    switch (type) {
        case RTTI_AIRCRAFTTYPE:
            return AircraftTypeClass::As_Pointer(AircraftType(id));
        case RTTI_INFANTRYTYPE:
            return InfantryTypeClass::As_Pointer(InfantryType(id));
        case RTTI_UNITTYPE:
            return UnitTypeClass::As_Pointer(UnitType(id));
        case RTTI_VESSELTYPE:
            return VesselTypeClass::As_Pointer(VesselType(id));
        case RTTI_BUILDINGTYPE:
            return BuildingTypeClass::As_Pointer(BuildingType(id));
        default:
            return nullptr;
    }
}
