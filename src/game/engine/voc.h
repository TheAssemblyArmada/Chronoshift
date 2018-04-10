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

#ifndef VOC_H
#define VOC_H

#include	"always.h"

enum VocType {
    VOC_NONE = -1,

    VOC_FIRST = 0,

    //Civilian Girl
    VOC_GIRL_OKAY = 0,
    VOC_GIRL_YEAH = 1,

    //Civilian Guy
    VOC_GUY_OKAY = 2,
    VOC_GUY_YEAH = 3,

    VOC_MINELAY = 4,

    //
    VOC_ACKNO = 5,		//rename this.
    VOC_AFFIRM1 = 6,		//rename this.
    VOC_AWAIT1 = 7,		//rename this.

    //Engineer
    VOC_EAFFIRM1 = 8,		//rename this.
    VOC_EENGIN1 = 9,		//rename this.

    //
    VOC_NOPROB = 10,		//rename this.
    VOC_READY = 11,		//rename this.
    VOC_REPORT1 = 12,		//rename this.
    VOC_RITAWAY = 13,		//rename this.
    VOC_ROGER = 14,		//rename this.
    VOC_UGOTIT = 15,		//rename this.
    VOC_VEHIC1 = 16,		//rename this.
    VOC_YESSIR1 = 17,		//rename this.

    //Death Screams
    VOC_DEDMAN1 = 18,
    VOC_DEDMAN2 = 19,
    VOC_DEDMAN3 = 20,
    VOC_DEDMAN4 = 21,
    VOC_DEDMAN5 = 22,
    VOC_DEDMAN6 = 23,
    VOC_DEDMAN7 = 24,
    VOC_DEDMAN8 = 25,

    VOC_DEDMAN10 = 26,

    VOC_CHRONO2 = 27,		//rename this.
    VOC_CANNON1 = 28,		//rename this.
    VOC_CANNON2 = 29,		//rename this.
    VOC_IRONCUR9 = 30,		//rename this.
    VOC_EMOVOUT1 = 31,		//rename this.
    VOC_SONPULSE = 32,		//rename this.
    VOC_SANDBAG2 = 33,		//rename this.
    VOC_MINEBLO1 = 34,		//rename this.
    VOC_CHUTE1 = 35,		//rename this.

    VOC_DOGY1 = 36,		//rename this.
    VOC_DOGW5 = 37,		//rename this.
    VOC_DOGG5P = 38,		//rename this.

    VOC_FIREBL3 = 39,		//rename this.
    VOC_FIRETRT1 = 40,		//rename this.
    VOC_GRENADE1 = 41,		//rename this.
    VOC_GUN11 = 42,		//rename this.
    VOC_GUN13 = 43,		//rename this.
    VOC_EYESSIR1 = 44,		//rename this.
    VOC_GUN27 = 45,		//rename this.
    VOC_HEAL2 = 46,		//rename this.
    VOC_HYDROD1 = 47,		//rename this.
    VOC_INVUL2 = 48,		//rename this.
    VOC_KABOOM1 = 49,		//rename this.
    VOC_KABOOM12 = 50,		//rename this.
    VOC_KABOOM15 = 51,		//rename this.
    VOC_SPLASH9 = 52,		//rename this.
    VOC_KABOOM22 = 53,		//rename this.
    VOC_AACANON3 = 54,		//rename this.
    VOC_TANDETH1 = 55,		//rename this.
    VOC_MGUNINF1 = 56,		//rename this.
    VOC_MISSILE1 = 57,		//rename this.
    VOC_MISSILE6 = 58,		//rename this.
    VOC_MISSILE7 = 59,		//rename this.
    VOC_ORETRUK1 = 60,		//rename this.
    VOC_PILLBOX1 = 61,		//rename this.
    VOC_RABEEP1 = 62,		//rename this.
    VOC_RAMENU1 = 63,		//rename this.
    VOC_SILENCER = 64,		//rename this.
    VOC_TANK5 = 65,		//rename this.
    VOC_TANK6 = 66,		//rename this.
    VOC_TORPEDO1 = 67,		//rename this.
    VOC_TURRET1 = 68,		//rename this.
    VOC_TSLACHG2 = 69,		//rename this.
    VOC_TESLA1 = 70,
    VOC_SQUISHY2 = 71,		//rename this.
    VOC_SCOLDY1 = 72,		//rename this.

    //Radar
    VOC_RADAR_ON = 73,
    VOC_RADAR_OFF = 74,

    VOC_PLACE_BUILDING = 75,

    VOC_KABOOM30 = 76,		//rename this.
    VOC_KABOOM25 = 77,		//rename this.
    VOC_KABOOM21 = 78,		//rename this.

    VOC_DOGW7 = 79,		//rename this.
    VOC_DOGW3PX = 80,		//rename this.

    VOC_CRMBLE2 = 81,		//rename this.

    //Credits Tick Up/Down
    VOC_MONEY_UP = 82,
    VOC_MONEY_DOWN = 83,

    VOC_CONSTRUCTION = 84,

    VOC_GAME_CLOSED = 85,
    VOC_INCOMING_MESSAGE = 86,
    VOC_SYS_ERROR = 87,
    VOC_OPTIONS_CHANGED = 88,
    VOC_GAME_FORMING = 89,
    VOC_PLAYER_LEFT = 90,
    VOC_PLAYER_JOINED = 91,

    VOC_DEPTH_CHARGE = 92,

    VOC_CASHTURN = 93,			//Selling

    //Tanya
    VOC_CHEW_ON_THIS = 94,
    VOC_LETS_ROCK = 95,
    VOC_LAUGH = 96,
    VOC_SHAKE_IT_BABY = 97,
    VOC_CHA_CHING = 98,
    VOC_ALL_YA_GOT = 99,
    VOC_KISS_BYE_BYE = 100,
    VOC_IM_THERE = 101,
    VOC_GIVE_IT_TO_ME = 102,
    VOC_YEAH = 103,
    VOC_YES_SIR = 104,
    VOC_WHATS_UP = 105,

    VOC_WALLKIL2 = 106,		//rename this.
    VOC_UNKNOWN22 = 107,		//rename this.
    VOC_GUN5 = 108,		//rename this.
    VOC_SUB_SHOW = 109,

    //Einstein
    VOC_EINSTEIN_YEAH = 110,
    VOC_EINSTEIN_OK = 111,
    VOC_EINSTEIN_YES = 112,

    VOC_MINE1 = 113,

    //Spy
    VOC_SCOMND1 = 114,	    //rename this.
    VOC_SYESSIR1 = 115, 	//rename this.
    VOC_SPY_INDEED = 116,
    VOC_SONWAY1 = 117,	//rename this.
    VOC_SPY_KING_AND_COUNTRY = 118,		//rename this.
    VOC_MRESPON1 = 119,	    //rename this.
    VOC_MYESSIR1 = 120,	    //rename this.
    VOC_MAFFIRM1 = 121,	//rename this.  

    VOC_MMOVOUT1 = 122,   	//rename this.
    VOC_BEEPSLCT = 123,   	//rename this.
    VOC_SYEAH1 = 124,   	//rename this.

    VOC_ANT_DIE = 125,
    VOC_ANT_BITE = 126,

    VOC_SMOUT1 = 127,   	//rename this.
    VOC_SOKAY1 = 128,   	//rename this.
    VOC_UNKNOWN23 = 129,   	//rename this.
    VOC_SWHAT1 = 130,   	//rename this.
    VOC_SAFFIRM1 = 131,   	//rename this.
    VOC_STAVCMDR = 132,   	//rename this.
    VOC_STAVCRSE = 133,   	//rename this.
    VOC_STAVYES = 134,   	//rename this.
    VOC_STAVMOV = 135,   	//rename this.
    VOC_BUZZY1 = 136,   	//rename this.
    VOC_RAMBO1 = 137,   	//rename this.
    VOC_RAMBO2 = 138,   	//rename this.
    VOC_RAMBO3 = 139,   	//rename this.

    VOC_MYES1 = 140,   	//rename this.
    VOC_MHOWDY1 = 141,   	//rename this.
    VOC_MRISE1 = 142,   	//rename this.
    VOC_MHUH1 = 143,   	//rename this.
    VOC_MHEAR1 = 144,   	//rename this.
    VOC_MLAFF1 = 145,   	//rename this.
    VOC_MBOSS1  = 146,   	//rename this.
    VOC_MYEEHAW1 = 147,   	//rename this.
    VOC_MHOTDIG1 = 148,   	//rename this.
    VOC_MWRENCH1 = 149,   	//rename this.

    VOC_JBURN1 = 150,   	//rename this.
    VOC_JCHRGE1 = 151,   	//rename this.
    VOC_JCRISP1 = 152,   	//rename this.
    VOC_JDANCE1 = 153,   	//rename this.
    VOC_JJUICE1 = 154,   	//rename this.
    VOC_JJUMP1 = 155,   	//rename this.
    VOC_JLIGHT1 = 156,   	//rename this.
    VOC_JPOWER1 = 157,   	//rename this.
    VOC_JSHOCK1 = 158,   	//rename this.
    VOC_JYES1 = 159,   	//rename this.

    VOC_CHROTNK1 = 160,   	//rename this.
    VOC_FIXIT1 = 161,   	//rename this.
    VOC_MADCHRG2 = 162,   	//rename this.
    VOC_MADEXPLO = 163,   	//rename this.
    VOC_SHKTROP1 = 164,   	//rename this.

    VOC_LAST = 164,

    VOC_COUNT = 165
};

DEFINE_ENUMERATION_OPERATORS(VocType);
DEFINE_ENUMERATION_BITWISE_OPERATORS(VocType);

VocType Voc_From_Name(char const *name);
char const *Name_From_Voc(VocType voc);

#endif // VOC_H
