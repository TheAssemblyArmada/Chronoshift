#include	"triggertype.h"

#ifndef RAPP_STANDALONE
TFixedIHeapClass<TriggerTypeClass> &TriggerTypes = *reinterpret_cast<TFixedIHeapClass<TriggerTypeClass> *>(0x0065DCD8);
#else
TFixedIHeapClass<TriggerTypeClass> TriggerTypes;
#endif

void TriggerTypeClass::Code_Pointers()
{
#ifndef RAPP_STANDALONE
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
#ifndef RAPP_STANDALONE
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

TriggerTypeClass *TriggerTypeClass::From_Name(char const *name)
{
    if (strcasecmp(name, "<none>") == 0 || strcasecmp(name, "none") == 0) {
        return nullptr;
    }

    if (name != nullptr) {
        for (int index = 0; index < TriggerTypes.Count(); ++index) {
            if (strcasecmp(name, Name_From((TriggerType)index)) == 0) {
                return &TriggerTypes[index];
            }
        }
    }

    return nullptr;
}

char const *TriggerTypeClass::Name_From(TriggerType trigger)
{
    return (trigger != TRIGGER_NONE) && (trigger < TriggerTypes.Count()) ? As_Reference(trigger).Get_Name() : "<none>";
}

char const *TriggerTypeClass::Name_From(TriggerTypeClass *trigger)
{
    if (trigger != nullptr) {
        return TriggerTypes[TriggerTypes.ID(trigger)].Get_Name();
    }
    return nullptr;
}

TriggerTypeClass &TriggerTypeClass::As_Reference(TriggerType trigger)
{
    return TriggerTypes[trigger];
}

TriggerTypeClass *TriggerTypeClass::As_Pointer(TriggerType trigger)
{
    return trigger != TRIGGER_NONE && trigger < TriggerTypes.Count() ? &TriggerTypes[trigger] : nullptr;
}