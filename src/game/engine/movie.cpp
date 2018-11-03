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
#include "movie.h"

#ifndef PLATFORM_WINDOWS
#include <strings.h>
#endif

MovieInfoStruct MovieTypes[MOVIE_COUNT] = {
    { "AAGUN", false },
    { "MIG", false },
    { "SFROZEN", false },
    { "AIRFIELD", false },
    { "BATTLE", false },
    { "BMAP", false },
    { "BOMBRUN", false },
    { "DPTHCHRG", false },
    { "GRVESTNE", false },
    { "MONTPASS", false },
    { "MTNKFACT", false },
    { "CRONTEST", false },
    { "OILDRUM", false },
    { "ALLYEND", false },
    { "RADRRAID", false },
    { "SHIPYARD", false },
    { "SHORBOMB", false },
    { "SITDUCK", false },
    { "SLNTSRVC", false },
    { "SNOWBASE", false },
    { "EXECUTE", false },
    { "REDINTRO", false },
    { "NUKESTOK", false },
    { "V2ROCKET", false },
    { "SEARCH", false },
    { "BINOC", false },
    { "ELEVATOR", false },
    { "FROZEN", false },
    { "MCV", false },
    { "SHIPSINK", false },
    { "SOVMCV", false },
    { "TRINITY", false },
    { "ALLYMORF", false },
    { "APCESCPE", false },
    { "BRDGTILT", false },
    { "CRONFAIL", false },
    { "STRAFE", false },
    { "DESTROYR", false },
    { "DOUBLE", false },
    { "FLARE", false },
    { "SNSTRAFE", false },
    { "LANDING", false },
    { "ONTHPRWL", false },
    { "OVERRUN", false },
    { "SNOWBOMB", false },
    { "SOVCEMET", false },
    { "TAKE_OFF", false },
    { "TESLA", false },
    { "SOVIET8", false },
    { "SPOTTER", false },
    { "ALLY1", false },
    { "ALLY2", false },
    { "ALLY4", false },
    { "SOVFINAL", false },
    { "ASSESS", false },
    { "SOVIET10", false },
    { "DUD", false },
    { "MCV_LAND", false },
    { "MCVBRDGE", false },
    { "PERISCOP", false },
    { "SHORBOM1", false },
    { "SHORBOM2", false },
    { "SOVBATL", false },
    { "SOVTSTAR", false },
    { "AFTRMATH", false },
    { "SOVIET11", false },
    { "MASASSLT", false },
    { "ENGLISH", true },
    { "SOVIET1", false },
    { "SOVIET2", false },
    { "SOVIET3", false },
    { "SOVIET4", false },
    { "SOVIET5", false },
    { "SOVIET6", false },
    { "SOVIET7", false },
    { "PROLOG", false },
    { "AVERTED", false },
    { "COUNTDWN", false },
    { "MOVINGIN", false },
    { "ALLY10", false },
    { "ALLY12", false },
    { "ALLY5", false },
    { "ALLY6", false },
    { "ALLY8", false },
    { "TANYA1", false },
    { "TANYA2", false },
    { "ALLY10B", false },
    { "ALLY11", false },
    { "ALLY14", false },
    { "ALLY9", false },
    { "SPY", false },
    { "TOOFAR", false },
    { "SOVIET12", false },
    { "SOVIET13", false },
    { "SOVIET9", false },
    { "BEACHEAD", false },
    { "SOVIET14", false },

    // Sizzle/Trailers
    { "SIZZLE", false },
    { "SIZZLE2", false },

    // Aftermath Ant Missions
    { "ANTEND", false },
    { "ANTINTRO", false } /*,

     //Retaliation
     { "R_ANTS", false },
     { "R_INTRO", false },

     //Retaliation Allies Campaign
     { "R_ALLIED1", false },
     { "R_ALLIED2", false },
     { "R_ALLIED3", false },
     { "R_ALLIED4", false },
     { "R_ALLIED5", false },
     { "R_ALLIED6", false },
     { "R_ALLIED7", false },
     { "R_ALLIED8", false },
     { "R_ALLIED9", false },
     { "R_WINA", false },

     { "R_CHRONTNK", false },
     { "R_GPSLNCH", false },
     { "R_NUKETRUK", false },
     { "R_SNOWFILD", false },

     //Retaliation Soviet Campaign
     { "R_SOVIET1", false },
     { "R_SOVIET2", false },
     { "R_SOVIET3", false },
     { "R_SOVIET4", false },
     { "R_SOVIET5", false },
     { "R_SOVIET6", false },
     { "R_SOVIET7", false },
     { "R_SOVIET8", false },
     { "R_SOVIET9", false },
     { "R_WINS", false },

     { "R_TANESCP", false },
     { "R_TESLATNK", false },

     //Sizzle/Trailers
     { "SIZZLE3", false },
     { "SIZZLE4", false }
     */
};

MovieType Movie_From_Name(const char *name)
{
    if (strcasecmp(name, "<none>") == 0 || strcasecmp(name, "none") == 0) {
        return MOVIE_NONE;
    }

    if (name != nullptr) {
        for (MovieType movie = MOVIE_FIRST; movie < MOVIE_COUNT; ++movie) {
            if (strcasecmp(name, MovieTypes[movie].Name) == 0) {
                return movie;
            }
        }
    }

    return MOVIE_NONE;
}

const char *Name_From_Movie(MovieType movie)
{
    if (movie != MOVIE_NONE && movie < MOVIE_COUNT) {
        return MovieTypes[movie].Name;
    }
    return "<none>";
}

MovieInfoStruct *MovieInfo_From_Movie(MovieType movie)
{
    if (movie != MOVIE_NONE && movie < MOVIE_COUNT) {
        return &MovieTypes[movie];
    }
    return nullptr;
}
