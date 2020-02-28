/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class holding static info on infantry objects.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "infantrytype.h"
#include "facing.h"
#include "gamefile.h"
#include "gameini.h"
#include "globals.h"
#include "infantry.h"
#include "infantrydata.h"
#include "iomap.h"
#include "lists.h"
#include "rules.h"

#ifndef GAME_DLL
TFixedIHeapClass<InfantryTypeClass> g_InfantryTypes;
#endif

/**
 * 0x004DF5E0
 */
InfantryTypeClass::InfantryTypeClass(InfantryType type, int uiname, const char *name, int def_fire_coord, int rot_count,
    BOOL female, BOOL crawls, BOOL civilian, BOOL has_alt_remap, BOOL nominal, BOOL theater, PipEnum pip, const DoInfoStruct *do_info,
    int fire_up, int fire_prone, const uint8_t *alt_remap) :
    TechnoTypeClass(RTTI_INFANTRYTYPE, type, uiname, name, REMAP_1, def_fire_coord, rot_count, 0, 0, 0, nominal, true, true,
        true, false, false, theater, false, true, true, FACING_COUNT_8, SPEED_FOOT),

    m_FemaleVoice(female),
    m_IsCrawler(crawls),
    m_IsInfiltrator(false),
    m_IsFraidycat(false),
    m_IsCivilian(civilian),
    m_HasC4(false),
    m_IsCanine(false),
    m_HasAltRemap(has_alt_remap),
    m_Type(type),
    m_Pip(pip),
    m_DoControl(do_info),
    m_FireUp(fire_up),
    m_FireProne(fire_prone),
    m_AltRemap(alt_remap)
{
    // Set techno flags that don't get set by the TechnoTypeClass ctor.
    m_Crushable = true;
    m_IsScanner = true;
    m_IsRepairable = false;
}

/**
 * 0x004EB2F0
 */
InfantryTypeClass::InfantryTypeClass(const InfantryTypeClass &that) :
    TechnoTypeClass(that),
    m_FemaleVoice(that.m_FemaleVoice),
    m_IsCrawler(that.m_IsCrawler),
    m_IsInfiltrator(that.m_IsInfiltrator),
    m_IsFraidycat(that.m_IsFraidycat),
    m_IsCivilian(that.m_IsCivilian),
    m_HasC4(that.m_HasC4),
    m_IsCanine(that.m_IsCanine),
    m_HasAltRemap(that.m_HasAltRemap),
    m_Type(that.m_Type),
    m_Pip(that.m_Pip),
    m_DoControl(that.m_DoControl),
    m_FireUp(that.m_FireUp),
    m_FireProne(that.m_FireProne),
    m_AltRemap(that.m_AltRemap)
{
}

/**
 * 0x004DF728
 */
void *InfantryTypeClass::operator new(size_t size)
{
    captainslog_assert(size == sizeof(InfantryTypeClass) && size == g_InfantryTypes.Heap_Size());
    return g_InfantryTypes.Allocate();
}

/**
 * 0x004DF73C
 */
void InfantryTypeClass::operator delete(void *ptr)
{
    captainslog_assert(ptr != nullptr);
    g_InfantryTypes.Free(ptr);
}

/**
 * Fetches the name of the object for display purposes.
 *
 * 0x004EB16C
 */
int InfantryTypeClass::Full_Name() const
{
    if (g_InMapEditor || !m_IsNominal || g_Rule.Named_Civilians() || m_Type == INFANTRY_C10 || m_Type == INFANTRY_EINSTEIN
        || m_Type == INFANTRY_DELPHI) {
        return AbstractTypeClass::Full_Name();
    }

    return TXT_CIVILIAN;
}

/**
 * Fetches the width and height of the object.
 *
 * 0x004EB2DC
 */
void InfantryTypeClass::Dimensions(int &w, int &h) const
{
    w = 14;
    h = 20;
}

/**
 * Creates an InfantyClass instance for the specified house and places it at the specified cell.
 *
 * 0x004EAF74
 */
BOOL InfantryTypeClass::Create_And_Place(cell_t cellnum, HousesType house) const
{
    InfantryClass *iptr = new InfantryClass(m_Type, house);

    if (iptr != nullptr) {
        coord_t coord = g_Map[cellnum].Closest_Free_Spot(Cell_To_Coord(cellnum));
        if (coord != 0) {
            return iptr->Unlimbo(coord, DIR_EAST);
        }
    }

    return false;
}

/**
 * Creates an InfantyClass for the specified house.
 *
 * 0x004EAF20
 */
ObjectClass *InfantryTypeClass::Create_One_Of(HouseClass *house) const
{
    captainslog_assert(house != nullptr);

    return new InfantryClass(m_Type, house->What_Type());
}

/**
 * Fetches the occupy list for this type.
 *
 * 0x004EB01C
 */
const int16_t *InfantryTypeClass::Occupy_List(BOOL a1) const
{
    // Infantry always occupy only the one cell
    static int16_t _list[] = { 0, LIST_END };

    return _list;
}

/**
 * Reads an object of this type from an ini file.
 *
 * 0x004EB1D0
 */
BOOL InfantryTypeClass::Read_INI(GameINIClass &ini)
{
    if (!TechnoTypeClass::Read_INI(ini)) {
        return false;
    }

    m_IsFraidycat = ini.Get_Bool(m_Name, "Fraidycat", m_IsFraidycat);
    m_IsInfiltrator = ini.Get_Bool(m_Name, "Infiltrate", m_IsInfiltrator);
    m_HasC4 = ini.Get_Bool(m_Name, "C4", m_HasC4);
    m_IsCanine = ini.Get_Bool(m_Name, "IsCanine", m_IsCanine);

    // C4 presumes infiltrate.
    if (m_HasC4) {
        m_IsInfiltrator = true;
    }

    // Dogs can't be leaders.
    if (m_IsCanine) {
        m_IsLeader = false;
    }

    return true;
}

/**
 * Fetches a reference to the actual object from a type enum value.
 *
 * @warning Heap allocation order MUST match the enum order in Init_Heap for this to work.
 */
InfantryTypeClass &InfantryTypeClass::As_Reference(InfantryType type)
{
    captainslog_assert(type != INFANTRY_NONE);
    captainslog_assert(type < INFANTRY_COUNT);

    return g_InfantryTypes[type];
}

InfantryTypeClass *InfantryTypeClass::As_Pointer(InfantryType type)
{
    return (type < INFANTRY_COUNT) && (type != INFANTRY_NONE) ? &g_InfantryTypes[type] : nullptr;
}

/**
 * Fetches the type enum value from a name string.
 */
InfantryType InfantryTypeClass::From_Name(const char *name)
{
    if (name != nullptr) {
        for (InfantryType type = INFANTRY_FIRST; type < INFANTRY_COUNT; ++type) {
            if (strcasecmp(name, As_Reference(type).m_Name) == 0) {
                return type;
            }
        }
    }

    return INFANTRY_NONE;
}

/**
 * @brief 
 *
 * @address 0x004FF9A0 (beta)
 */
void InfantryTypeClass::Prep_For_Add()
{
    for (InfantryType i = INFANTRY_FIRST; i < INFANTRY_COUNT; ++i) {
        InfantryTypeClass *itptr = As_Pointer(i);
        if (itptr != nullptr) {
            g_Map.Add_To_List(itptr);
        }
    }
}

/**
 * @brief Initialises the memory heap for InfantryTypeClass objects.
 * @warning Order of initialisation is important so enum matches position in the heap.
 *
 * @address 0x004DF754
 */
void InfantryTypeClass::Init_Heap()
{
    // The order of heap initialisation MUST match the InfantryType enum in infantrytype.h
    new InfantryTypeClass(InfantryE1);
    new InfantryTypeClass(InfantryGrenadier);
    new InfantryTypeClass(InfantryE3);
    new InfantryTypeClass(InfantryFlameTrooper);
    new InfantryTypeClass(InfantryEngineer);
    new InfantryTypeClass(InfantryE7);
    new InfantryTypeClass(InfantrySpy);
    new InfantryTypeClass(InfantryThief);
    new InfantryTypeClass(InfantryMedic);
    new InfantryTypeClass(InfantryGeneral);
    new InfantryTypeClass(InfantryDog);
    new InfantryTypeClass(InfantryC1);
    new InfantryTypeClass(InfantryC2);
    new InfantryTypeClass(InfantryC3);
    new InfantryTypeClass(InfantryC4);
    new InfantryTypeClass(InfantryC5);
    new InfantryTypeClass(InfantryC6);
    new InfantryTypeClass(InfantryC7);
    new InfantryTypeClass(InfantryC8);
    new InfantryTypeClass(InfantryC9);
    new InfantryTypeClass(InfantryC10);
    new InfantryTypeClass(InfantryEinstein);
    new InfantryTypeClass(InfantryDelphi);
    new InfantryTypeClass(InfantryDrChan);
    new InfantryTypeClass(InfantryShockTrooper);
    new InfantryTypeClass(InfantryMechanic);
}

/**
 *
 *
 */
void InfantryTypeClass::One_Time()
{
    char filename[512];

    for (InfantryType i = INFANTRY_FIRST; i < INFANTRY_COUNT; ++i) {
        InfantryTypeClass &infantry = As_Reference(i);
        const char *name = infantry.m_ImageName[0] != '\0' ? infantry.m_ImageName : infantry.m_Name;

        snprintf(filename, sizeof(filename), "%s.shp", name);
        infantry.m_ImageData = GameFileClass::Retrieve(filename);

        snprintf(filename, sizeof(filename), "%.4sicon.shp", name);
        infantry.m_CameoData = GameFileClass::Retrieve(filename);
    }
}
