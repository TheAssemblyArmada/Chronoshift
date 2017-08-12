////////////////////////////////////////////////////////////////////////////////
//                        --  C H R O N O  S T L --                           //
////////////////////////////////////////////////////////////////////////////////
//
//  Project Name:: ChronoSTL
//
//			File:: ROTR.H
//
//        Author:: CCHyper
//
//  Contributors:: 
//
//   Description:: <TODO>
//
//       License:: Distributed under the GNU License, Version 2.0.
//			       (See accompanying file LICENSE.TXT)
//
////////////////////////////////////////////////////////////////////////////////
#if defined(COMPILER_MSVC) && (COMPILER_VERSION >= 1000) // MSVC++ 4.0
#pragma once
#endif // COMPILER_MSVC && COMPILER_VERSION >= 1000

#if !defined(_ROTR_H_)
#define _ROTR_H_

#include	"always.h"


////////////////////////////////////////////////////////////////////////////////
///
/// <!-- RotateRight() -->
///
/// \brief
///     this function rotates the value specified, as determined
///     by 'x', to the right by the number of bits specified in 'n'.
///
/// \param
///     x           The value to rotate.
///
/// \param
///     n           Rotate by this number of bits.
///
/// \return
///     value       TODO
///
/// \warning
///     None
///
////////////////////////////////////////////////////////////////////////////////
template<class T>
T RotateRight(T x, int n)
{
    return ( x << ((sizeof(T) * 8) - n) ) | ( x >> n );
}

#endif // _ROTR_H_
