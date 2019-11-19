/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "triggertype.h"
#include "gameini.h"
#include <cstring>
#include <cstdio>

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

using std::strtok;
using std::sprintf;
using std::strcat;

#ifndef GAME_DLL
TFixedIHeapClass<TriggerTypeClass> g_TriggerTypes;
#endif

TriggerTypeClass::TriggerTypeClass() : 
    AbstractTypeClass(RTTI_TRIGGERTYPE, g_TriggerTypes.ID(this), 0, "x"),
    m_State(STATE_VOLATILE),
    m_House(HOUSES_NONE),
    m_EventLinkage(EVLINK_SINGLE),
    m_ActionLinkage(ACTLINK_SINGLE)
{
    // TODO verify the default member values.
}

/**
 * @address 0x0056D3F0
 */
void *TriggerTypeClass::operator new(size_t size)
{
    TriggerTypeClass *this_ptr = g_TriggerTypes.Alloc();
    DEBUG_ASSERT(this_ptr != nullptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = true;
    }
    return this_ptr;
}

/**
 * @address 0x0056D410
 */
void TriggerTypeClass::operator delete(void *ptr)
{
    TriggerTypeClass *this_ptr = static_cast<TriggerTypeClass *>(ptr);
    DEBUG_ASSERT(this_ptr != nullptr);
    if (this_ptr != nullptr) {
        this_ptr->m_IsActive = false;
    }
    g_TriggerTypes.Free(this_ptr);
}

/**
 * Encodes pointers for serialisation.
 *
 * @address 0x004F9448
 */
void TriggerTypeClass::Code_Pointers()
{
    m_EventOne.Code_Pointers();
    m_EventTwo.Code_Pointers();
    m_ActionOne.Code_Pointers();
    m_ActionTwo.Code_Pointers();
}

/**
 * Decodes pointers for deserialisation.
 *
 * @address 0x004F9464
 */
void TriggerTypeClass::Decode_Pointers()
{
    m_EventOne.Decode_Pointers();
    m_EventTwo.Decode_Pointers();
    m_ActionOne.Decode_Pointers();
    m_ActionTwo.Decode_Pointers();
}

/**
 * Finds and existed Trigger instance or makes a new one from the TriggerTypeClass.
 */
TriggerClass *TriggerTypeClass::Find_Or_Make()
{
    // Find or make is a free function in original, but takes TriggerTypeClass so makes sense as a member.
#ifdef GAME_DLL
    TriggerClass *(*func)(TriggerTypeClass *) = reinterpret_cast<TriggerClass *(*)(TriggerTypeClass *)>(0x0056D248);
    return func(this);
#else
    return nullptr;
#endif
}

/**
 * Detaches this trigger type from the target.
 */
void TriggerTypeClass::Detach(target_t target, int unk)
{
    m_ActionOne.Detach(target);
    m_ActionTwo.Detach(target);
}

/**
 * Generates a bitfield that indicates what this TriggerType can attach to.
 */
AttachType TriggerTypeClass::Attaches_To()
{
    AttachType ret = m_EventOne.Attaches_To();

    if (m_EventLinkage != EVLINK_SINGLE) {
        ret |= m_EventTwo.Attaches_To();
    }

    return ret;
}

/**
 * Initialises the TriggerTypes heap.
 */
void TriggerTypeClass::Init()
{
    g_TriggerTypes.Free_All();
}

/**
 * Reads the [Trigs] section from an ini file and generates TriggerTypes from it.
 */
void TriggerTypeClass::Read_INI(GameINIClass &ini)
{
    int entries = ini.Entry_Count("Trigs");

    // Iterate all entries in Trigs and create trigger types from them.
    for (int i = 0; i < entries; ++i) {
        char buff[128];
        const char *entry = ini.Get_Entry("Trigs", i);
        TriggerTypeClass *tt = new TriggerTypeClass;
        ini.Get_String("Trigs", entry, nullptr, buff, sizeof(buff));
        tt->Fill_In(entry, buff);
    }

    // Original code sweeps through if NewINIFormat < 2 and calls From_Name on all the strdup entries,
    // then frees them. Call to From_Name moved to TActionClass Read_INI and strdup/free requirement removed.
}

/**
 * Writes the [Trigs] section to an ini file from the TriggerTypes heap.
 */
void TriggerTypeClass::Write_INI(GameINIClass &ini)
{
    // Clear older section name in case we imported an old map format?
    ini.Clear("Triggers");
    ini.Clear("Trigs");

    for (int i = 0; i < g_TriggerTypes.Count(); ++i) {
        char buff[256];
        g_TriggerTypes[i].Build_INI_Entry(buff);
        ini.Put_String("Trigs", g_TriggerTypes[i].Get_Name(), buff);
    }
}

/**
 * Gets a TriggerTypeClass instance from its name.
 */
TriggerTypeClass *TriggerTypeClass::From_Name(const char *name)
{
    if (strcasecmp(name, "<none>") == 0 || strcasecmp(name, "none") == 0) {
        return nullptr;
    }

    if (name != nullptr) {
        for (int index = 0; index < g_TriggerTypes.Count(); ++index) {
            if (strcasecmp(name, Name_From((TriggerType)index)) == 0) {
                return &g_TriggerTypes[index];
            }
        }
    }

    return nullptr;
}

/**
 * Gets a trigger name from its heap ID.
 */
const char *TriggerTypeClass::Name_From(TriggerType trigger)
{
    return (trigger != TRIGGER_NONE) && (trigger < g_TriggerTypes.Count()) ? As_Reference(trigger).Get_Name() : "<none>";
}

/**
 * Gets a TriggerTypeClass name from an instance.
 */
const char *TriggerTypeClass::Name_From(TriggerTypeClass *trigger)
{
    if (trigger != nullptr) {
        return trigger->Get_Name();
    }
    return nullptr;
}

/**
 * Gets a TriggerTypeClass reference from its heap ID.
 */
TriggerTypeClass &TriggerTypeClass::As_Reference(TriggerType type)
{
    DEBUG_ASSERT(type != TRIGGER_NONE);
    DEBUG_ASSERT(type < g_TriggerTypes.Count());

    return g_TriggerTypes[type];
}

/**
 * Gets a TriggerTypeClass pointer from its heap ID.
 */
TriggerTypeClass *TriggerTypeClass::As_Pointer(TriggerType type)
{
    return type != TRIGGER_NONE && type < g_TriggerTypes.Count() ? &g_TriggerTypes[type] : nullptr;
}

/**
 * Fills in a TriggerTypeClass from the ini entry string.
 */
void TriggerTypeClass::Fill_In(const char *name, char *options)
{
    strlcpy(m_Name, name, sizeof(m_Name));
    m_State = PersistanceType(atoi(strtok(options, ",")));
    m_House = HousesType(atoi(strtok(nullptr, ",")));
    m_EventLinkage = EventLinkType(atoi(strtok(nullptr, ",")));
    m_ActionLinkage = ActionLinkType(atoi(strtok(nullptr, ",")));
    m_EventOne.Read_INI();
    m_EventTwo.Read_INI();
    m_ActionOne.Read_INI();
    m_ActionTwo.Read_INI();
}

/**
 * Generates a TriggerTypeClass ini entry string.
 */
void TriggerTypeClass::Build_INI_Entry(char *buffer)
{
    sprintf(buffer, "%d,%d,%d,%d,", m_State, m_House, m_EventLinkage, m_ActionLinkage);
    m_EventOne.Build_INI_Entry(&buffer[strlen(buffer)]);
    strcat(buffer, ",");
    m_EventTwo.Build_INI_Entry(&buffer[strlen(buffer)]);
    strcat(buffer, ",");
    m_ActionOne.Build_INI_Entry(&buffer[strlen(buffer)]);
    strcat(buffer, ",");
    m_ActionTwo.Build_INI_Entry(&buffer[strlen(buffer)]);
}
