/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Lists of coord adjustments for overlap and occupier calculations.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "lists.h"

void List_Copy(int16_t *dst, const int16_t *src, int size)
{
    // Iterate the list and stop when we run out of size for the destination
    // or we encounter LIST_END which terminates the overlap and occupy lists.
    if (dst != nullptr) {
        int i = size;
        while (i-- > 0) {
            int16_t chk = *src;

            *dst++ = *src++;

            if (chk == LIST_END) {
                return;
            }
        }

        // Insert a list terminator at the end.
        dst[size - 1] = LIST_END;
    }
}

int List_Size(const int16_t *list)
{
    int size = 0;
    while (*list++ != LIST_END) {
        ++size;
    }
    return size;
}

////////////////////////////////////////////////////////////////////////////////
//  Occupy Lists
//    The foundation of the object.
////////////////////////////////////////////////////////////////////////////////

//-X-
//XXX
//-X-
const int16_t ListFix[] = {
    1,              //-X-
    128, 129, 130,  //XXX
    257,            //-X-
    LIST_END
};

//XXX
//XXX
const int16_t ListWeap[] = {
    0, 1, 2,        //XXX
    128, 129, 130,  //XXX
    LIST_END
};

const int16_t ListWestwood[] = {
    1, 2, 3, 129, 130, 131,
    LIST_END
};

//XXX
//XXX
//XXX
const int16_t ListSPen[] = {
    0, 1, 2,        //XXX
    128, 129, 130,  //XXX
    256, 257, 258,  //XXX
    LIST_END
};

//XXX
//XXX
//XXX
const int16_t ListFactory[] = {
    0, 1, 2,        //XXX
    128, 129, 130,  //XXX
    256, 257, 258,  //XXX
    LIST_END
};

//X
const int16_t ListSilo[] = {
    0,
    LIST_END
};

//
const int16_t ListCom[] = {
    0, 1, 128, 129,
    LIST_END
};

//X
const int16_t List1[] = {
    0,          //X
    LIST_END
};

//X-
const int16_t List10[] = {
    0,            //X-
    LIST_END
};

//XX
const int16_t List11[] = {
    0, 1,        //XX
    LIST_END
};

//XX
//XX
const int16_t List1111[] = {
    0, 1,        //XX
    128, 129,    //XX
    LIST_END
};

//X-
//X-
const int16_t List1010[] = {
    0,            //X-
    128,        //X-
    LIST_END
};

//--
//X-
const int16_t List0010[] = {
    //--
    128,        //X-
    LIST_END
};

//--
//XX
const int16_t List0011[] = {
    //--
    128, 129,    //XX
    LIST_END
};

//-X
//XX
const int16_t List0111[] = {
    1,   //-X
    128, 129,   //XX
    LIST_END
};

//
const int16_t List1101[] = {
    0, 1, 129,
    LIST_END
};

//---
//X--
const int16_t List000110[] = {
    //---
    128, 129,    //X--
    LIST_END
};

const int16_t List111111[] = {
    0, 1, 2, 128, 129, 130,
    LIST_END
};

//-X-
//XX-
const int16_t List010110[] = {
    1,            //-X-
    128, 129,    //XX-
    LIST_END
};

//XX-
//XX-
const int16_t List110110[] = {
    0, 1,        //XX-
    128, 129,    //XX-
    LIST_END
};

//
const int16_t List000111111[] = {
    128, 129, 130, 256, 257, 258,
    LIST_END
};

//-X-
//XXX
//X--
const int16_t List010111100[] = {
    1,              //-X-
    128, 129, 130,  //XXX
    256,            //X--
    LIST_END
};

//--X-
//XXX-
//-XX-
const int16_t List001011100110[] = {
    2,                //--X-
    128, 129, 130,    //XXX-
    257, 258,        //-XX-
    LIST_END
};

//----
//XXX-
//X---
const int16_t List000011101000[] = {
    //----
    128, 129, 130,    //XXX-
    256,            //X---
    LIST_END
};


////////////////////////////////////////////////////////////////////////////////
//  Overlap Lists
//    Area of the object that is not really occupied by the object, but overlaps
//    surrounding cells. This is required so these cells can be refreshed for
//    drawing purposes.
////////////////////////////////////////////////////////////////////////////////

//
const int16_t OListCom[] = {
    1,
    LIST_END
};

//
const int16_t OListTmpl[] = {
    0, 1, 2,
    LIST_END
};

//X-X
//---
//X-X
const int16_t OListFix[] = {
    0, 2,           //X-X
                    //---
    256, 258,       //X-X
    LIST_END
};

//
const int16_t OListWeap[] = {
    LIST_END
};

//
const int16_t OListSPen[] = {
    LIST_END
};

//X
//X
const int16_t OListWestwood[] = {
    0,      //X
    128,    //X
    LIST_END
};

const int16_t OListSAM[] = {
    -128, -127,
    LIST_END
};

//
const int16_t OList12[] = {
    0,
    LIST_END
};

//-X
const int16_t OList01[] = {
    1,            //-X
    LIST_END
};

//--X
//---
const int16_t OList001[] = {
    2,            //--X
                //---
    LIST_END
};

//X-
//-X
const int16_t OList1001[] = {
    0,            //X-
    129,        //-X
    LIST_END
};

//XX
//--
const int16_t OList1100[] = {
    0, 1,        //XX
                //--
    LIST_END
};

//XX
//-X
const int16_t OList1101[] = {
    0, 1,        //XX
    129,        //-X
    LIST_END
};

//--
//X-
const int16_t OList0010[] = {
    //--
    128,        //X-
    LIST_END
};

//X-
//--
const int16_t OList1000[] = {
    0,             //X-
                   //--
                   LIST_END
};

//X-X
//--X
const int16_t OList101001[] = {
    0, 2,        //X-X
    130,        //--X
    LIST_END
};

//XX-
//--X
const int16_t OList110001[] = {
    0, 1,        //XX-
    130,        //--X
    LIST_END
};

//X-X
//---
//-XX
const int16_t OList101000011[] = {
    0, 2,       //X-X
                //---
    257, 258,   //-XX
    LIST_END
};

//XX--
//---X
//X--X
const int16_t OList110000011001[] = {
    0, 1,        //XX--
    131,        //---X
    256, 259,    //X--X
    LIST_END
};

//XXX-
//---X
//-XX-
const int16_t OList111000010110[] = {
    0, 1, 2,    //XXX-
    131,        //---X
    257, 258,    //-XX-
    LIST_END
};


////////////////////////////////////////////////////////////////////////////////
//  Exit Lists
//    A 'ring' around the object that defines the available area for a
//    object to exit this object.
////////////////////////////////////////////////////////////////////////////////

//
const int16_t ExitPyle[] = { 
    257, 258, 256, 255, -129, -128, -127, -126, -126, -1, 2, 130, 127,
    LIST_END
};

//
const int16_t ExitSub[] = {
    256, 258, 255, 257, 259
};

//
const int16_t ExitWeap[] = {

    257,
    383, 384, 385, 382, 386,
    LIST_END
};
