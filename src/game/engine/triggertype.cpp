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

#ifndef GAME_DLL
TFixedIHeapClass<TriggerTypeClass> g_TriggerTypes;
#endif

TriggerTypeClass::TriggerTypeClass() :
    AbstractTypeClass(RTTI_TRIGGERTYPE, g_TriggerTypes.ID(this), 0, "x")
{
}

/**
 * Encodes pointers for serialisation.
 *
 * @address 0x004F9448
 */
void TriggerTypeClass::Code_Pointers()
{
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
    m_ActionOne.Decode_Pointers();
    m_ActionTwo.Decode_Pointers();
}

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

const char *TriggerTypeClass::Name_From(TriggerType trigger)
{
    return (trigger != TRIGGER_NONE) && (trigger < g_TriggerTypes.Count()) ? As_Reference(trigger).Get_Name() : "<none>";
}

const char *TriggerTypeClass::Name_From(TriggerTypeClass *trigger)
{
    if (trigger != nullptr) {
        return g_TriggerTypes[g_TriggerTypes.ID(trigger)].Get_Name();
    }
    return nullptr;
}

TriggerTypeClass &TriggerTypeClass::As_Reference(TriggerType type)
{
    DEBUG_ASSERT(type != TRIGGER_NONE);
    DEBUG_ASSERT(type < g_TriggerTypes.Count());

    return g_TriggerTypes[type];
}

TriggerTypeClass *TriggerTypeClass::As_Pointer(TriggerType type)
{
    return type != TRIGGER_NONE && type < g_TriggerTypes.Count() ? &g_TriggerTypes[type] : nullptr;
}
