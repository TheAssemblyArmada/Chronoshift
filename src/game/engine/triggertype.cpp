#include "triggertype.h"

#ifndef CHRONOSHIFT_STANDALONE
TFixedIHeapClass<TriggerTypeClass> &g_TriggerTypes = *reinterpret_cast<TFixedIHeapClass<TriggerTypeClass> *>(0x0065DCD8);
#else
TFixedIHeapClass<TriggerTypeClass> g_TriggerTypes;
#endif

void TriggerTypeClass::Code_Pointers()
{
#ifndef CHRONOSHIFT_STANDALONE
    void(*func)(TriggerTypeClass *) =
        reinterpret_cast<void(*)(TriggerTypeClass *)>(0x004F9448);
    return func(this);
#else
    //TODO: Requires TActionClass and TEventClass
    //EventOne.Code_Pointers();
    //EventTwo.Code_Pointers();
    //ActionOne.Code_Pointers();
    //ActionTwo.Code_Pointers();
#endif
}

void TriggerTypeClass::Decode_Pointers()
{
#ifndef CHRONOSHIFT_STANDALONE
    void(*func)(TriggerTypeClass *) =
        reinterpret_cast<void(*)(TriggerTypeClass *)>(0x004F9464);
    return func(this);
#else
    //TODO: Requires TActionClass and TEventClass
    //EventOne.Decode_Pointers();
    //EventTwo.Decode_Pointers();
    //ActionOne.Decode_Pointers();
    //ActionTwo.Decode_Pointers();
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

TriggerTypeClass &TriggerTypeClass::As_Reference(TriggerType trigger)
{
    return g_TriggerTypes[trigger];
}

TriggerTypeClass *TriggerTypeClass::As_Pointer(TriggerType trigger)
{
    return trigger != TRIGGER_NONE && trigger < g_TriggerTypes.Count() ? &g_TriggerTypes[trigger] : nullptr;
}