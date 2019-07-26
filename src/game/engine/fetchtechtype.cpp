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
            return (TechnoTypeClass *)&AircraftTypeClass::As_Reference((AircraftType)id);

        case RTTI_INFANTRYTYPE:
            return (TechnoTypeClass *)&InfantryTypeClass::As_Reference((InfantryType)id);

        case RTTI_UNITTYPE:
            return (TechnoTypeClass *)&UnitTypeClass::As_Reference((UnitType)id);

        case RTTI_VESSELTYPE:
            return (TechnoTypeClass *)&VesselTypeClass::As_Reference((VesselType)id);

        case RTTI_BUILDINGTYPE:
            return (TechnoTypeClass *)&BuildingTypeClass::As_Reference((BuildingType)id);

        default:
            return nullptr;
    }
}
