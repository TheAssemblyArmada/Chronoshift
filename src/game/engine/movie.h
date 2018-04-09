/**
* @file
*
* @author CCHyper
*
* @brief <todo>
*
* @copyright RedAlert++ is free software: you can redistribute it and/or
*            modify it under the terms of the GNU General Public License
*            as published by the Free Software Foundation, either version
*            2 of the License, or (at your option) any later version.
*            A full copy of the GNU General Public License can be found in
*            LICENSE
*/
#pragma once

#ifndef MOVIE_H
#define MOVIE_H

#include	"always.h"

enum MovieType
{
    MOVIE_NONE = -1,
    MOVIE_FIRST = 0,
    MOVIE_PROLOG = 75,
    MOVIE_ENGLISH = 999,
    MOVIE_REDINTRO = 999,
    MOVIE_SIZZLE = 97,
    MOVIE_SIZZLE2 = 98,
    MOVIE_INTRO = 999,
    MOVIE_COUNT = 104
};

DEFINE_ENUMERATION_OPERATORS(MovieType);

struct MovieInfoStruct
{
    char const *Name;
    bool IsMovie640;
};

MovieType Movie_From_Name(char const *name);
char const *Name_From_Movie(MovieType movie);

MovieInfoStruct *MovieInfo_From_Movie(MovieType movie);

extern MovieInfoStruct MovieTypes[MOVIE_COUNT];

#endif // _PLAYMOVIE_H_
