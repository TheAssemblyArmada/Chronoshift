#pragma once

#ifndef TRIGGERTYPE_H
#define TRIGGERTYPE_H

#include "always.h"
#include "abstracttype.h"
#include "housetype.h"

// This enum is just for casting the TriggerType heap indexes.
enum TriggerType
{
    TRIGGER_NONE = -1,
};

DEFINE_ENUMERATION_OPERATORS(TriggerType);

//needs rename?
/*
//0 = Volatile
//1 = Semi-persistant
//2 = Persistent

The first number tells RA whether this is a repeating trigger or not.
If it has the value 0, then the trigger will only ever be activated once
(not strictly true as the trigger can be fired again if associated with
a teamtype).  If it has a value greater than 0, then the trigger is a
repeating trigger (ie it will be fired more than once).

For repeating triggers, there are two types.  When the repeating
trigger has a value of 1, the trigger will only occur once the trigger
event has happened to all items (units and buildings) to which this
trigger has been attached.  This is useful if you want, for example,
some action to occur after a specific set of buildings have been
destroy.

The second type of repeating trigger is the free repeater.  When this
item has the value of 2, it will continue to repeat itself whenever its
trigger event is true.  This is of use if you want, for example, to have
the trigger activate every 20 time units.
*/
enum TriggerStateType
{
    STATE_NONE = -1,

    STATE_FIRST = 0,

    STATE_VOLATILE = 0,
    STATE_SEMI_PERSISTANT = 1,
    STATE_PERSISTANT = 2,

    STATE_LAST = 3,

    STATE_COUNT = 3
};

class TriggerTypeClass : public AbstractTypeClass
{
public:
    TriggerTypeClass(const NoInitClass &noinit) : AbstractTypeClass(noinit) {}

    void Code_Pointers();
    void Decode_Pointers();

    static TriggerTypeClass *From_Name(const char *name);
    static const char *Name_From(TriggerType trigger);
    static const char *Name_From(TriggerTypeClass *trigger);

    static TriggerTypeClass &As_Reference(TriggerType trigger);
    static TriggerTypeClass *As_Pointer(TriggerType trigger);

protected:

    //bitfield 0x25
#ifndef CHRONOSHIFT_NO_BITFIELDS
    // Union/Struct required to get correct packing when compiler packing set to 1.
    union
    {
        struct
        {
            bool IsActive : 1; // & 1
            //bool TrigTypeBit2;    // 2
            //bool TrigTypeBit4;    // 4
            //bool TrigTypeBit8;    // 8
            //bool TrigTypeBit16;    // 16
            //bool TrigTypeBit32;    // 32
            //bool TrigTypeBit64;    // 64
            //bool TrigTypeBit128;    // 128
        };
        int Bitfield;
    };
#else
    bool IsActive;
#endif

    TriggerStateType RepetitionState;            //0x29
    HousesType House;            //0x2A

    //TODO: Requires TEventClass
    char EventOne[9]; //TEventClass EventOne;        //0x2B
    char EventTwo[9]; //TEventClass EventTwo;        //0x34
    char TrigEventWhen;        //0x3D        //could be a enum.
            /*
            When 0, only the first trigger event (part 5) must be true for the trigger to be activated.
            When 1, the first (part 5) and second (part 8) must both be true for the trigger to be activated.
            When 2, either the first trigger event or the second trigger event must be true, whichever comes first.
            When 3, either the first trigger event or the second trigger event must be true. See the upcoming summary (next) for further details on how this operates.
            */

    //TODO: Requires TActionClass
    char ActionOne[13]; //TActionClass ActionOne;    //0x3E
    char ActionTwo[13]; //TActionClass ActionTwo;    //0x4B
    char TrigActionWhen;    //0x58        //could be a enum.
            /*
            When 0, only one trigger action is activated when the event is triggered. See the summary (next) for which trigger action is activated, and when.
            When 1, both trigger actions are activated when the event is triggered.
            */
};

#ifndef CHRONOSHIFT_STANDALONE
#include "hooker.h"
extern TFixedIHeapClass<TriggerTypeClass> &TriggerTypes;
#else
extern TFixedIHeapClass<TriggerTypeClass> TriggerTypes;
#endif

#endif // TRIGGERTYPE_H
