////////////////////////////////////////////////////////////////////////////////
//                            --  REDALERT++ --                               //
////////////////////////////////////////////////////////////////////////////////
//
//  Project Name:: Redalert++
//
//          File:: ROTL.H
//
//        Author:: CCHyper
//
//  Contributors:: OmniBlade
//
//   Description:: Base fetching stream class.
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
#pragma once

#ifndef ROTL_H
#define ROTL_H

#include	"always.h"


////////////////////////////////////////////////////////////////////////////////
///
/// <!-- RotateLeft() -->
///
/// \brief
///     this function rotates the value specified, as determined
///     by 'x', to the left by the number of bits specified in 'n'.
///
/// \param
///     x           The value to rotate.
///
/// \param
///     n           Rotate by this number of bits.
///
/// \return
///     value       Rotated value.
///
/// \warning
///     None
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
T RotateLeft(T x, int n)
{
    return ( x >> ((sizeof(T) * 8) - n) ) | ( x << n );
}

#endif // ROTL_H
