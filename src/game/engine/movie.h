/**
 * @file
 *
 * @author CCHyper
 *
 * @brief Movies enum and code for parsing the name to type and vice versa.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef MOVIE_H
#define MOVIE_H

#include "always.h"
#include "theme.h"

enum MovieType
{
    MOVIE_AAGUN,
    MOVIE_MIG,
    MOVIE_SFROZEN,
    MOVIE_AIRFIELD,
    MOVIE_BATTLE,
    MOVIE_BMAP,
    MOVIE_BOMBRUN,
    MOVIE_DPTHCHRG,
    MOVIE_GRVESTNE,
    MOVIE_MONTPASS,
    MOVIE_MTNKFACT,
    MOVIE_CRONTEST,
    MOVIE_OILDRUM,
    MOVIE_ALLYEND,
    MOVIE_RADRRAID,
    MOVIE_SHIPYARD,
    MOVIE_SHORBOMB,
    MOVIE_SITDUCK,
    MOVIE_SLNTSRVC,
    MOVIE_SNOWBASE,
    MOVIE_EXECUTE,
    MOVIE_REDINTRO,
    MOVIE_NUKESTOK,
    MOVIE_V2ROCKET,
    MOVIE_SEARCH,
    MOVIE_BINOC,
    MOVIE_ELEVATOR,
    MOVIE_FROZEN,
    MOVIE_MCV,
    MOVIE_SHIPSINK,
    MOVIE_SOVMCV,
    MOVIE_TRINITY,
    MOVIE_ALLYMORF,
    MOVIE_APCESCPE,
    MOVIE_BRDGTILT,
    MOVIE_CRONFAIL,
    MOVIE_STRAFE,
    MOVIE_DESTROYR,
    MOVIE_DOUBLE,
    MOVIE_FLARE,
    MOVIE_SNSTRAFE,
    MOVIE_LANDING,
    MOVIE_ONTHPRWL,
    MOVIE_OVERRUN,
    MOVIE_SNOWBOMB,
    MOVIE_SOVCEMET,
    MOVIE_TAKE_OFF,
    MOVIE_TESLA,
    MOVIE_SOVIET8,
    MOVIE_SPOTTER,
    MOVIE_ALLY1,
    MOVIE_ALLY2,
    MOVIE_ALLY4,
    MOVIE_SOVFINAL,
    MOVIE_ASSESS,
    MOVIE_SOVIET10,
    MOVIE_DUD,
    MOVIE_MCV_LAND,
    MOVIE_MCVBRDGE,
    MOVIE_PERISCOP,
    MOVIE_SHORBOM1,
    MOVIE_SHORBOM2,
    MOVIE_SOVBATL,
    MOVIE_SOVTSTAR,
    MOVIE_AFTRMATH,
    MOVIE_SOVIET11,
    MOVIE_MASASSLT,
    MOVIE_ENGLISH,
    MOVIE_SOVIET1,
    MOVIE_SOVIET2,
    MOVIE_SOVIET3,
    MOVIE_SOVIET4,
    MOVIE_SOVIET5,
    MOVIE_SOVIET6,
    MOVIE_SOVIET7,
    MOVIE_PROLOG,
    MOVIE_AVERTED,
    MOVIE_COUNTDWN,
    MOVIE_MOVINGIN,
    MOVIE_ALLY10,
    MOVIE_ALLY12,
    MOVIE_ALLY5,
    MOVIE_ALLY6,
    MOVIE_ALLY8,
    MOVIE_TANYA1,
    MOVIE_TANYA2,
    MOVIE_ALLY10B,
    MOVIE_ALLY11,
    MOVIE_ALLY14,
    MOVIE_ALLY9,
    MOVIE_SPY,
    MOVIE_TOOFAR,
    MOVIE_SOVIET12,
    MOVIE_SOVIET13,
    MOVIE_SOVIET9,
    MOVIE_BEACHEAD,
    MOVIE_SOVIET14,
    MOVIE_SIZZLE,
    MOVIE_SIZZLE2,
    MOVIE_ANTEND,
    MOVIE_ANTINTRO,

    MOVIE_COUNT,

    MOVIE_NONE = -1,
    MOVIE_FIRST = MOVIE_AAGUN,
};

DEFINE_ENUMERATION_OPERATORS(MovieType);

struct MovieInfoStruct
{
    const char *Name;
    bool IsMovie640;
};

MovieType Movie_From_Name(const char *name);
const char *Name_From_Movie(MovieType movie);

MovieInfoStruct *MovieInfo_From_Movie(MovieType movie);

void Play_Movie(MovieType movie, ThemeType theme = THEME_NONE, BOOL a3 = true);
void Play_Intro(BOOL flag = false);
void Play_SneakPeak();

extern MovieInfoStruct g_MovieTypes[MOVIE_COUNT];

#endif // MOVIE_H
