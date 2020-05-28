/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class containing information about warhead behaviour.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "warheadtype.h"
#include "gameini.h"
#include "warheaddata.h"


#ifndef GAME_DLL
TFixedIHeapClass<WarheadTypeClass> g_WarheadTypes;
#endif

/**
 * Global template ctor differs intentionally from the binary, don't change unless
 * you are sure you know why they are different and that your changes won't break
 * anything.
 *
 * 0x0058F9D8 Prototypes don't match but it fulfills same purpose, do not hook.
 */
WarheadTypeClass::WarheadTypeClass(WarheadType warhead, const char *name) :
    m_Type(warhead),
    m_Name(name),
    m_Spread(1),
    m_Wall(false),
    m_Wood(false),
    m_Ore(false),
    m_UnkBool(false),
    m_Explosion(0),
    m_Death(0)
{
    for (ArmorType armor = ARMOR_FIRST; armor < ARMOR_COUNT; ++armor) {
        m_Verses[armor] = fixed_t(1, 1);
    }
}

WarheadTypeClass::WarheadTypeClass(WarheadTypeClass const &that) :
    m_Type(that.m_Type),
    m_Name(that.m_Name),
    m_Spread(that.m_Spread),
    m_Wall(that.m_Wall),
    m_Wood(that.m_Wood),
    m_Ore(that.m_Ore),
    m_UnkBool(that.m_UnkBool),
    m_Explosion(that.m_Explosion),
    m_Death(that.m_Death)
{
    memcpy(m_Verses, that.m_Verses, sizeof(m_Verses));
}

/**
 * 0x0058FA64
 */
void *WarheadTypeClass::operator new(size_t size)
{
    return g_WarheadTypes.Alloc();
}

/**
 * 0x0058FA78
 */
void WarheadTypeClass::operator delete(void *ptr)
{
    g_WarheadTypes.Free(ptr);
}

/**
 * @brief Initialises object heap. Original did this in RulesClass::Heap_Maximums.
 */
void WarheadTypeClass::Init_Heap()
{
    // in the binary, all warhead types are initliased in RulesClass::Heap_Maximums("") and
    // do not have global class instance, so i have moved them here for continuity.
    new WarheadTypeClass(WarheadSmallArms);
    new WarheadTypeClass(WarheadHighExplosive);
    new WarheadTypeClass(WarheadArmorPiercing);
    new WarheadTypeClass(WarheadFire);
    new WarheadTypeClass(WarheadHollowPoint);
    new WarheadTypeClass(WarheadSuper);
    new WarheadTypeClass(WarheadOrganic);
    new WarheadTypeClass(WarheadNuke);
    new WarheadTypeClass(WarheadMechanical);
}

/**
 * @brief Get type enum value from string.
 */
WarheadType WarheadTypeClass::From_Name(const char *name)
{
    captainslog_assert(name != nullptr);

    if (strcasecmp(name, "<none>") == 0 || strcasecmp(name, "none") == 0) {
        return WARHEAD_NONE;
    }

    if (name != nullptr) {
        for (WarheadType warhead = WARHEAD_FIRST; warhead < WARHEAD_COUNT; ++warhead) {
            if (strcasecmp(name, Name_From(warhead)) == 0) {
                return warhead;
            }
        }
    }

    return WARHEAD_NONE;
}

/**
 * @brief Get string name from type enum value.
 */
const char *WarheadTypeClass::Name_From(WarheadType type)
{
    return type != WARHEAD_NONE && type < WARHEAD_COUNT ? As_Reference(type).m_Name : "<none>";
}

/**
 * @brief Get reference to object from type enum value.
 */
WarheadTypeClass &WarheadTypeClass::As_Reference(WarheadType type)
{
    captainslog_assert(type != WARHEAD_NONE);
    captainslog_assert(type < WARHEAD_COUNT);

    return g_WarheadTypes[type];
}

/**
 * @brief Get pointer to object from enum value.
 *
 * 0x0058FA90
 */
WarheadTypeClass *WarheadTypeClass::As_Pointer(WarheadType warhead)
{
    if (warhead != WARHEAD_NONE && warhead < WARHEAD_COUNT) {
        return &g_WarheadTypes[warhead];
    }

    return nullptr;
}

/**
 * @brief Read object data from an ini file.
 *
 * 0x0058FAB0
 */
BOOL WarheadTypeClass::Read_INI(GameINIClass &ini)
{
    char verses_buffer[128];
    static char _verses_format_buffer[128];

    if (ini.Find_Section(m_Name) != nullptr) {
        m_Spread = ini.Get_Int(m_Name, "Spread", m_Spread);
        m_Wall = ini.Get_Bool(m_Name, "Wall", m_Wall);
        m_Wood = ini.Get_Bool(m_Name, "Wood", m_Wood);
        m_Ore = ini.Get_Bool(m_Name, "Ore", m_Ore);
        m_Explosion = ini.Get_Int(m_Name, "Explosion", m_Explosion);
        m_Death = ini.Get_Int(m_Name, "InfDeath", m_Death);

        static bool _one_time = false;

        if (_one_time) {
            _verses_format_buffer[0] = '\0';

            // Build the verses format based on the armor count.
            for (ArmorType armor = ARMOR_FIRST; armor < ARMOR_COUNT; ++armor) {
                size_t check = strlcat(_verses_format_buffer, "100%%", sizeof(_verses_format_buffer));
                captainslog_assert(check <= sizeof(_verses_format_buffer));

                if (armor != (ARMOR_COUNT - 1)) {
                    check = strlcat(_verses_format_buffer, ",", sizeof(_verses_format_buffer));
                    captainslog_assert(check <= sizeof(_verses_format_buffer));
                }
            }

            _one_time = true;
        }

        if (ini.Get_String(m_Name, "Verses", _verses_format_buffer, verses_buffer, sizeof(verses_buffer)) > 0) {
            char *value = strtok(verses_buffer, ",");

            for (ArmorType armor = ARMOR_FIRST; (armor < ARMOR_COUNT) && (value != nullptr); ++armor) {
                captainslog_assert(value != nullptr);
                m_Verses[armor] = fixed_t(value);
                value = strtok(nullptr, ",");
            }
        }

        m_UnkBool = m_Verses[ARMOR_HEAVY] == fixed_t(0, 1);

        return true;
    }

    return false;
}
