////////////////////////////////////////////////////////////////////////////////
//                            --  REDALERT++ --                               //
////////////////////////////////////////////////////////////////////////////////
//
//  Project Name:: Redalert++
//
//          File:: SHASTRAW.CPP
//
//        Author:: CCHyper
//
//  Contributors:: OmniBlade
//
//   Description:: Stream implementation that hashes data with SHA1 as it
//                 passes through.
//
//       License:: Redalert++ is free software: you can redistribute it and/or
//                 modify it under the terms of the GNU General Public License
//                 as published by the Free Software Foundation, either version
//                 2 of the License, or (at your option) any later version.
//
//                 A full copy of the GNU General Public License can be found in
//                 LICENSE
//
////////////////////////////////////////////////////////////////////////////////
#include "shastraw.h"
#include "gamedebug.h"

/**
 * @brief Read data from the straw into specified memory buffer.
 *
 * @return Returns the number of bytes read into the buffer.
 */
int SHAStraw::Get(void *buffer, int length)
{
    int retval = 0;

    if (buffer && length > 0) {
        retval = Straw::Get(buffer, length);
        m_sha1.Hash(buffer, retval);
    }

    return retval;
}

/**
* @brief Read the SHA1 hash result into the specified memory buffer.
*
* @return Returns the number of bytes read into the buffer.
*/
int SHAStraw::Result(void *data)
{
    return m_sha1.Result(data);
}

#ifndef CHRONOSHIFT_STANDALONE
int SHAStraw::Hook_Result(SHAStraw *ptr, void *data)
{
    char hash[41];
    ptr->m_sha1.Print_Result(hash);
    DEBUG_LOG("SHAStraw returns hash of '%s'.\n", hash);
    return ptr->SHAStraw::Result(data);
}

int SHAStraw::Hook_Get(SHAStraw *ptr, void *source, int length)
{
    return ptr->SHAStraw::Get(source, length);
}
#endif
