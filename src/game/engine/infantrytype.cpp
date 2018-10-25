/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Class holding static info on infantry objects.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "infantrytype.h"
#include "gameini.h"
#include "facing.h"
#include "globals.h"
#include "lists.h"
#include "rules.h"

#ifndef CHRONOSHIFT_STANDALONE
TFixedIHeapClass<InfantryTypeClass> &g_InfantryTypes = Make_Global<TFixedIHeapClass<InfantryTypeClass> >(0x0065DE08);
#else
TFixedIHeapClass<InfantryTypeClass> g_InfantryTypes;
#endif

// Do controls for Dog.
DoInfoStruct DogDoControls[DO_COUNT] = {
    { 0, 1, 1 }, // DO_READY
    { 0, 1, 1 }, // DO_GUARD
    { 0, 1, 1 }, // DO_PRONE
    { 8, 6, 6 }, // DO_WALK
    { 104, 14, 14 }, // DO_FIRE_UP
    { 0, 0, 0 }, // DO_DOWN
    { 56, 6, 6 }, // DO_CRAWL
    { 0, 0, 0 }, // DO_UP
    { 104, 14, 14 }, // DO_FIRE_PRONE
    { 216, 18, 0 }, // DO_IDLE1
    { 216, 18, 0 }, // DO_IDLE2
    { 235, 7, 0 }, // DO_DIE1
    { 242, 9, 0 }, // DO_DIE2
    { 242, 9, 0 }, // DO_DIE3
    { 242, 9, 0 }, // DO_DIE4
    { 251, 14, 0 }, // DO_DIE5
    { 0, 1, 0 }, // DO_16
    { 0, 1, 0 }, // DO_17
    { 0, 1, 0 }, // DO_18
    { 0, 1, 0 }, // DO_19
    { 106, 12, 14 }, // DO_20
};

// Do controls for E1.
DoInfoStruct E1DoControls[DO_COUNT] = {
    { 0, 1, 1 }, // DO_READY
    { 8, 1, 1 }, // DO_GUARD
    { 192, 1, 8 }, // DO_PRONE
    { 16, 6, 6 }, // DO_WALK
    { 64, 8, 8 }, // DO_FIRE_UP
    { 128, 2, 2 }, // DO_DOWN
    { 144, 4, 4 }, // DO_CRAWL
    { 176, 2, 2 }, // DO_UP
    { 192, 6, 8 }, // DO_FIRE_PRONE
    { 256, 16, 0 }, // DO_IDLE1
    { 272, 16, 0 }, // DO_IDLE2
    { 288, 8, 0 }, // DO_DIE1
    { 304, 8, 0 }, // DO_DIE2
    { 304, 8, 0 }, // DO_DIE3
    { 312, 12, 0 }, // DO_DIE4
    { 324, 18, 0 }, // DO_DIE5
    { 342, 3, 3 }, // DO_16
    { 366, 3, 3 }, // DO_17
    { 390, 3, 3 }, // DO_18
    { 414, 3, 3 }, // DO_19
    { 0, 0, 0 }, // DO_20
};

// Do controls for Grenadier.
DoInfoStruct GrenadierDoControls[DO_COUNT] = {
    { 0, 1, 1 }, // DO_READY
    { 8, 1, 1 }, // DO_GUARD
    { 288, 1, 12 }, // DO_PRONE
    { 16, 6, 6 }, // DO_WALK
    { 64, 20, 20 }, // DO_FIRE_UP
    { 224, 2, 2 }, // DO_DOWN
    { 240, 4, 4 }, // DO_CRAWL
    { 272, 2, 2 }, // DO_UP
    { 288, 8, 12 }, // DO_FIRE_PRONE
    { 384, 16, 0 }, // DO_IDLE1
    { 400, 16, 0 }, // DO_IDLE2
    { 416, 8, 0 }, // DO_DIE1
    { 432, 8, 0 }, // DO_DIE2
    { 432, 8, 0 }, // DO_DIE3
    { 440, 12, 0 }, // DO_DIE4
    { 452, 18, 0 }, // DO_DIE5
    { 470, 3, 3 }, // DO_16
    { 494, 3, 3 }, // DO_17
    { 518, 3, 3 }, // DO_18
    { 542, 3, 3 }, // DO_19
    { 0, 0, 0 }, // DO_20
};

// Do controls for E3.
DoInfoStruct E3DoControls[DO_COUNT] = {
    { 0, 1, 1 }, // DO_READY
    { 8, 1, 1 }, // DO_GUARD
    { 192, 1, 10 }, // DO_PRONE
    { 16, 6, 6 }, // DO_WALK
    { 64, 8, 8 }, // DO_FIRE_UP
    { 128, 2, 2 }, // DO_DOWN
    { 144, 4, 4 }, // DO_CRAWL
    { 176, 2, 2 }, // DO_UP
    { 192, 10, 10 }, // DO_FIRE_PRONE
    { 272, 16, 0 }, // DO_IDLE1
    { 288, 16, 0 }, // DO_IDLE2
    { 304, 8, 0 }, // DO_DIE1
    { 320, 8, 0 }, // DO_DIE2
    { 320, 8, 0 }, // DO_DIE3
    { 328, 12, 0 }, // DO_DIE4
    { 340, 18, 0 }, // DO_DIE5
    { 358, 3, 3 }, // DO_16
    { 382, 3, 3 }, // DO_17
    { 406, 3, 3 }, // DO_18
    { 430, 3, 3 }, // DO_19
    { 0, 0, 0 }, // DO_20
};

// Do controls for Flame Trooper.
DoInfoStruct FlameTrooperDoControls[DO_COUNT] = {
    { 0, 1, 1 }, // DO_READY
    { 8, 1, 1 }, // DO_GUARD
    { 256, 1, 16 }, // DO_PRONE
    { 16, 6, 6 }, // DO_WALK
    { 64, 16, 16 }, // DO_FIRE_UP
    { 192, 2, 2 }, // DO_DOWN
    { 208, 4, 4 }, // DO_CRAWL
    { 240, 2, 2 }, // DO_UP
    { 256, 16, 16 }, // DO_FIRE_PRONE
    { 384, 16, 0 }, // DO_IDLE1
    { 400, 16, 0 }, // DO_IDLE2
    { 416, 8, 0 }, // DO_DIE1
    { 432, 8, 0 }, // DO_DIE2
    { 432, 8, 0 }, // DO_DIE3
    { 440, 12, 0 }, // DO_DIE4
    { 452, 18, 0 }, // DO_DIE5
    { 470, 3, 3 }, // DO_16
    { 494, 3, 3 }, // DO_17
    { 518, 3, 3 }, // DO_18
    { 542, 3, 3 }, // DO_19
    { 0, 0, 0 }, // DO_20
};

// Do controls for Engineer.
DoInfoStruct EngineerDoControls[DO_COUNT] = {
    { 0, 1, 1 }, // DO_READY
    { 8, 1, 1 }, // DO_GUARD
    { 82, 1, 4 }, // DO_PRONE
    { 16, 6, 6 }, // DO_WALK
    { 0, 0, 0 }, // DO_FIRE_UP
    { 67, 2, 2 }, // DO_DOWN
    { 82, 4, 4 }, // DO_CRAWL
    { 114, 2, 2 }, // DO_UP
    { 0, 0, 0 }, // DO_FIRE_PRONE
    { 130, 16, 0 }, // DO_IDLE1
    { 130, 16, 0 }, // DO_IDLE2
    { 146, 8, 0 }, // DO_DIE1
    { 154, 8, 0 }, // DO_DIE2
    { 162, 8, 0 }, // DO_DIE3
    { 162, 12, 0 }, // DO_DIE4
    { 182, 18, 0 }, // DO_DIE5
    { 200, 3, 3 }, // DO_16
    { 224, 3, 3 }, // DO_17
    { 200, 3, 3 }, // DO_18
    { 224, 3, 3 }, // DO_19
    { 0, 0, 0 }, // DO_20
};

// Do controls for E7.
DoInfoStruct E7DoControls[DO_COUNT] = {
    { 0, 1, 1 }, // DO_READY
    { 0, 1, 1 }, // DO_GUARD
    { 128, 1, 4 }, // DO_PRONE
    { 8, 6, 6 }, // DO_WALK
    { 56, 7, 7 }, // DO_FIRE_UP
    { 113, 2, 2 }, // DO_DOWN
    { 128, 4, 4 }, // DO_CRAWL
    { 161, 2, 2 }, // DO_UP
    { 176, 7, 7 }, // DO_FIRE_PRONE
    { 232, 17, 0 }, // DO_IDLE1
    { 249, 13, 0 }, // DO_IDLE2
    { 262, 8, 0 }, // DO_DIE1
    { 270, 8, 0 }, // DO_DIE2
    { 278, 8, 0 }, // DO_DIE3
    { 286, 12, 0 }, // DO_DIE4
    { 298, 18, 0 }, // DO_DIE5
    { 0, 1, 0 }, // DO_16
    { 0, 1, 0 }, // DO_17
    { 0, 1, 0 }, // DO_18
    { 0, 1, 0 }, // DO_19
    { 0, 0, 0 }, // DO_20
};

// Do controls for Spy.
DoInfoStruct SpyDoControls[DO_COUNT] = {
    { 0, 1, 1 }, // DO_READY
    { 8, 1, 1 }, // DO_GUARD
    { 144, 1, 4 }, // DO_PRONE
    { 16, 6, 6 }, // DO_WALK
    { 64, 8, 8 }, // DO_FIRE_UP
    { 128, 2, 2 }, // DO_DOWN
    { 144, 4, 4 }, // DO_CRAWL
    { 176, 2, 2 }, // DO_UP
    { 192, 8, 8 }, // DO_FIRE_PRONE
    { 256, 14, 0 }, // DO_IDLE1
    { 270, 18, 0 }, // DO_IDLE2
    { 288, 8, 0 }, // DO_DIE1
    { 296, 8, 0 }, // DO_DIE2
    { 304, 8, 0 }, // DO_DIE3
    { 312, 12, 0 }, // DO_DIE4
    { 324, 18, 0 }, // DO_DIE5
    { 0, 1, 0 }, // DO_16
    { 0, 1, 0 }, // DO_17
    { 0, 1, 0 }, // DO_18
    { 0, 1, 0 }, // DO_19
    { 0, 0, 0 }, // DO_20
};

// Do controls for Thief.
DoInfoStruct ThiefDoControls[DO_COUNT] = {
    { 0, 1, 1 }, // DO_READY
    { 8, 1, 1 }, // DO_GUARD
    { 72, 1, 4 }, // DO_PRONE
    { 8, 6, 6 }, // DO_WALK
    { 0, 0, 0 }, // DO_FIRE_UP
    { 56, 2, 2 }, // DO_DOWN
    { 72, 4, 4 }, // DO_CRAWL
    { 108, 2, 2 }, // DO_UP
    { 0, 0, 0 }, // DO_FIRE_PRONE
    { 120, 19, 0 }, // DO_IDLE1
    { 120, 19, 0 }, // DO_IDLE2
    { 139, 8, 0 }, // DO_DIE1
    { 147, 8, 0 }, // DO_DIE2
    { 155, 8, 0 }, // DO_DIE3
    { 163, 12, 0 }, // DO_DIE4
    { 175, 18, 0 }, // DO_DIE5
    { 0, 1, 0 }, // DO_16
    { 0, 1, 0 }, // DO_17
    { 0, 1, 0 }, // DO_18
    { 0, 1, 0 }, // DO_19
    { 0, 0, 0 }, // DO_20
};

// Do controls for Medic.
DoInfoStruct MedicDoControls[DO_COUNT] = {
    { 0, 1, 1 }, // DO_READY
    { 0, 1, 1 }, // DO_GUARD
    { 130, 1, 4 }, // DO_PRONE
    { 8, 6, 6 }, // DO_WALK
    { 56, 28, 0 }, // DO_FIRE_UP
    { 114, 2, 2 }, // DO_DOWN
    { 130, 4, 4 }, // DO_CRAWL
    { 162, 2, 2 }, // DO_UP
    { 56, 28, 0 }, // DO_FIRE_PRONE
    { 178, 15, 0 }, // DO_IDLE1
    { 178, 15, 0 }, // DO_IDLE2
    { 193, 8, 0 }, // DO_DIE1
    { 210, 8, 0 }, // DO_DIE2
    { 202, 8, 0 }, // DO_DIE3
    { 217, 12, 0 }, // DO_DIE4
    { 229, 18, 0 }, // DO_DIE5
    { 0, 1, 0 }, // DO_16
    { 0, 1, 0 }, // DO_17
    { 0, 1, 0 }, // DO_18
    { 0, 1, 0 }, // DO_19
    { 0, 0, 0 }, // DO_20
};

// Do controls for General.
DoInfoStruct GeneralDoControls[DO_COUNT] = {
    { 0, 1, 1 }, // DO_READY
    { 0, 1, 1 }, // DO_GUARD
    { 104, 1, 4 }, // DO_PRONE
    { 8, 6, 6 }, // DO_WALK
    { 56, 4, 4 }, // DO_FIRE_UP
    { 88, 2, 2 }, // DO_DOWN
    { 104, 4, 4 }, // DO_CRAWL
    { 136, 2, 2 }, // DO_UP
    { 152, 4, 4 }, // DO_FIRE_PRONE
    { 184, 26, 0 }, // DO_IDLE1
    { 184, 26, 0 }, // DO_IDLE2
    { 210, 8, 0 }, // DO_DIE1
    { 226, 8, 0 }, // DO_DIE2
    { 218, 8, 0 }, // DO_DIE3
    { 234, 12, 0 }, // DO_DIE4
    { 246, 18, 0 }, // DO_DIE5
    { 0, 1, 0 }, // DO_16
    { 0, 1, 0 }, // DO_17
    { 0, 1, 0 }, // DO_18
    { 0, 1, 0 }, // DO_19
    { 0, 0, 0 }, // DO_20
};

// Defualt Do controls for Civilian(s).
DoInfoStruct CivilianDoControls[DO_COUNT] = {
    { 0, 1, 1 }, // DO_READY
    { 0, 1, 1 }, // DO_GUARD
    { 0, 1, 1 }, // DO_PRONE
    { 56, 6, 6 }, // DO_WALK
    { 120, 4, 4 }, // DO_FIRE_UP
    { 0, 1, 1 }, // DO_DOWN
    { 8, 6, 6 }, // DO_CRAWL
    { 0, 1, 1 }, // DO_UP
    { 120, 4, 4 }, // DO_FIRE_PRONE
    { 104, 10, 0 }, // DO_IDLE1
    { 114, 6, 0 }, // DO_IDLE2
    { 152, 8, 0 }, // DO_DIE1
    { 160, 8, 0 }, // DO_DIE2
    { 160, 8, 0 }, // DO_DIE3
    { 168, 12, 0 }, // DO_DIE4
    { 180, 18, 0 }, // DO_DIE5
    { 0, 1, 0 }, // DO_16
    { 0, 1, 0 }, // DO_17
    { 0, 1, 0 }, // DO_18
    { 0, 1, 0 }, // DO_19
    { 0, 0, 0 }, // DO_20
};

// Do controls for Einstein.
DoInfoStruct EinsteinDoControls[DO_COUNT] = {
    { 0, 1, 1 }, // DO_READY
    { 0, 1, 1 }, // DO_GUARD
    { 0, 1, 1 }, // DO_PRONE
    { 56, 6, 6 }, // DO_WALK
    { 113, 4, 4 }, // DO_FIRE_UP
    { 0, 1, 1 }, // DO_DOWN
    { 8, 6, 6 }, // DO_CRAWL
    { 0, 1, 1 }, // DO_UP
    { 0, 0, 0 }, // DO_FIRE_PRONE
    { 104, 16, 0 }, // DO_IDLE1
    { 104, 16, 0 }, // DO_IDLE2
    { 120, 8, 0 }, // DO_DIE1
    { 128, 8, 0 }, // DO_DIE2
    { 136, 12, 0 }, // DO_DIE3
    { 136, 12, 0 }, // DO_DIE4
    { 148, 17, 0 }, // DO_DIE5
    { 0, 1, 0 }, // DO_16
    { 0, 1, 0 }, // DO_17
    { 0, 1, 0 }, // DO_18
    { 0, 1, 0 }, // DO_19
    { 0, 0, 0 }, // DO_20
};

// Do controls for Shock Trooper.
DoInfoStruct ShockTrooperDoControls[DO_COUNT] = {
    { 0, 1, 1 }, // DO_READY
    { 8, 1, 1 }, // DO_GUARD
    { 256, 1, 16 }, // DO_PRONE
    { 16, 6, 6 }, // DO_WALK
    { 64, 16, 16 }, // DO_FIRE_UP
    { 192, 2, 2 }, // DO_DOWN
    { 208, 4, 4 }, // DO_CRAWL
    { 240, 2, 2 }, // DO_UP
    { 256, 16, 16 }, // DO_FIRE_PRONE
    { 384, 16, 0 }, // DO_IDLE1
    { 400, 16, 0 }, // DO_IDLE2
    { 416, 8, 0 }, // DO_DIE1
    { 432, 8, 0 }, // DO_DIE2
    { 432, 8, 0 }, // DO_DIE3
    { 440, 12, 0 }, // DO_DIE4
    { 452, 18, 0 }, // DO_DIE5
    { 470, 3, 3 }, // DO_16
    { 494, 3, 3 }, // DO_17
    { 518, 3, 3 }, // DO_18
    { 542, 3, 3 }, // DO_19
    { 0, 0, 0 }, // DO_20
};

// Do controls for Mechanic.
DoInfoStruct MechanicDoControls[DO_COUNT] = {
    { 0, 1, 1 }, // DO_READY
    { 0, 1, 1 }, // DO_GUARD
    { 130, 1, 4 }, // DO_PRONE
    { 8, 6, 6 }, // DO_WALK
    { 56, 28, 0 }, // DO_FIRE_UP
    { 114, 2, 2 }, // DO_DOWN
    { 130, 4, 4 }, // DO_CRAWL
    { 162, 2, 2 }, // DO_UP
    { 56, 28, 0 }, // DO_FIRE_PRONE
    { 178, 15, 0 }, // DO_IDLE1
    { 178, 15, 0 }, // DO_IDLE2
    { 193, 8, 0 }, // DO_DIE1
    { 210, 8, 0 }, // DO_DIE2
    { 202, 8, 0 }, // DO_DIE3
    { 217, 12, 0 }, // DO_DIE4
    { 229, 18, 0 }, // DO_DIE5
    { 0, 1, 0 }, // DO_16
    { 0, 1, 0 }, // DO_17
    { 0, 1, 0 }, // DO_18
    { 0, 1, 0 }, // DO_19
    { 0, 0, 0 }, // DO_20
};

// clang-format off
const uint8_t RemapCiv2[256] = {
    0, 1, 2, 3, 4, 5, 6, 209, 8, 9, 10, 11, 12, 13, 12, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
    64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
    96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
    112, 113, 114, 115, 116, 117, 187, 188, 120, 121, 122, 123, 124, 125, 126, 127,
    128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
    144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 209,
    160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
    176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 167, 13, 189, 190, 191,
    192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207,
    208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
    224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
};

const uint8_t RemapCiv4[256] = {
    0, 1, 2, 3, 4, 5, 6, 187, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
    64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
    96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 118, 110, 119,
    112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
    128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
    144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
    160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
    176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
    192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 188, 207,
    208, 209, 182, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
    224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
};

const uint8_t RemapCiv5[256] = {
    0, 1, 2, 3, 4, 5, 6, 109, 8, 9, 10, 11, 131, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
    64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
    96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 177, 110, 178,
    112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
    128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
    144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
    160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
    176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
    192, 193, 194, 195, 196, 197, 198, 199, 111, 201, 202, 203, 204, 205, 111, 207,
    208, 209, 182, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
    224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
};

const uint8_t RemapCiv6[256] = {
    0, 1, 2, 3, 4, 5, 6, 120, 8, 9, 10, 11, 12, 13, 238, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
    64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
    96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
    112, 113, 114, 115, 116, 117, 236, 206, 120, 121, 122, 123, 124, 125, 126, 127,
    128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
    144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 111,
    160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
    176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
    192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207,
    208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
    224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
};

const uint8_t RemapCiv7[256] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 131, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
    64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
    96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
    112, 113, 114, 115, 116, 117, 157, 212, 120, 121, 122, 123, 124, 125, 126, 127,
    128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
    144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 7,
    160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
    176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 118, 119, 189, 190, 191,
    192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207,
    208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
    224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
};

const uint8_t RemapCiv8[256] = {
    0, 1, 2, 3, 4, 5, 6, 182, 8, 9, 10, 11, 12, 13, 131, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
    64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
    96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
    112, 113, 114, 115, 116, 117, 215, 7, 120, 121, 122, 123, 124, 125, 126, 127,
    128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
    144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 182,
    160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
    176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 198, 199, 189, 190, 191,
    192, 193, 194, 195, 196, 197, 198, 199, 111, 201, 202, 203, 204, 205, 206, 207,
    208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
    224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
};

const uint8_t RemapCiv9[256] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 7, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
    64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
    96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
    112, 113, 114, 115, 116, 117, 163, 165, 120, 121, 122, 123, 124, 125, 126, 127,
    128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
    144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 200,
    160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
    176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 111, 13, 189, 190, 191,
    192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207,
    208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
    224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
};

const uint8_t RemapCiv10[256] = {
    0, 1, 2, 3, 4, 5, 6, 137, 8, 9, 10, 11, 12, 13, 15, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
    64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
    96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
    112, 113, 114, 115, 116, 117, 129, 131, 120, 121, 122, 123, 124, 125, 126, 127,
    128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
    144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 137,
    160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
    176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 163, 165, 189, 190, 191,
    192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207,
    208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
    224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
};

//i think this is a fire death remap, though never used.
const uint8_t RemapEmber[256] = {
    0, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 12, 103, 103, 103,
    103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
    103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
    103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
    103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
    103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
    103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
    103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
    103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
    103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
    103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
    103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
    103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
    103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
    103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
    103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103
};
// clang-format on

// Attack Dog
const InfantryTypeClass InfantryDog(
    INFANTRY_DOG, TXT_DOG, "DOG", 21, 16, false, false, false, false, false, false, PIP_GREEN, DogDoControls, 1, 1, nullptr);

// Rifle Infantry
const InfantryTypeClass InfantryE1(
    INFANTRY_E1, TXT_E1, "E1", 53, 16, false, true, false, false, false, false, PIP_GREEN, E1DoControls, 2, 2, nullptr);

// Grenadier
const InfantryTypeClass InfantryGrenadier(INFANTRY_E2, TXT_E2, "E2", 53, 16, false, true, false, false, false, false,
    PIP_GREEN, GrenadierDoControls, 14, 6, nullptr);

// Rocket Soldier
const InfantryTypeClass InfantryE3(
    INFANTRY_E3, TXT_E3, "E3", 53, 16, false, true, false, false, false, false, PIP_GREEN, E3DoControls, 3, 3, nullptr);

// Flame Trooper
const InfantryTypeClass InfantryFlameTrooper(INFANTRY_FLAME_TROOPER, TXT_E4, "E4", 53, 16, false, true, false, false, false,
    false, PIP_GREEN, FlameTrooperDoControls, 2, 0, nullptr);

// Engineer
const InfantryTypeClass InfantryEngineer(INFANTRY_ENGINEER, TXT_E6, "E6", 53, 16, false, false, false, false, false, false,
    PIP_YELLOW, EngineerDoControls, 3, 3, nullptr);

// Spy
const InfantryTypeClass InfantrySpy(INFANTRY_SPY, TXT_SPY, "SPY", 53, 16, false, false, false, false, false, false,
    PIP_YELLOW, SpyDoControls, 3, 3, nullptr);

// Thief
const InfantryTypeClass InfantryThief(INFANTRY_THIEF, TXT_THIEF, "THF", 53, 16, false, false, false, false, false, false,
    PIP_YELLOW, ThiefDoControls, 3, 3, nullptr);

// Tanya
const InfantryTypeClass InfantryE7(
    INFANTRY_TANYA, TXT_E7, "E7", 53, 16, false, true, false, false, false, false, PIP_RED, E7DoControls, 2, 2, nullptr);

// Medic
const InfantryTypeClass InfantryMedic(INFANTRY_MEDIC, TXT_MEDIC, "MEDI", 53, 16, false, true, false, false, false, false,
    PIP_YELLOW, MedicDoControls, 25, 25, nullptr);

// General
const InfantryTypeClass InfantryGeneral(INFANTRY_GENERAL, TXT_GENERAL, "GNRL", 53, 16, false, true, false, false, false,
    false, PIP_YELLOW, GeneralDoControls, 2, 2, nullptr);

// Civilian 1
const InfantryTypeClass InfantryC1(
    INFANTRY_C1, TXT_C1, "C1", 53, 16, false, false, true, false, true, false, PIP_WHITE, CivilianDoControls, 2, 0, nullptr);

// Civilian 2
const InfantryTypeClass InfantryC2(INFANTRY_C2, TXT_C2, "C2", 53, 16, false, false, true, true, true, false, PIP_WHITE,
    CivilianDoControls, 2, 0, RemapCiv2);

// Civilian 3
const InfantryTypeClass InfantryC3(
    INFANTRY_C3, TXT_C3, "C3", 53, 16, true, false, true, false, true, false, PIP_WHITE, CivilianDoControls, 2, 0, nullptr);

// Civilian 4
const InfantryTypeClass InfantryC4(
    INFANTRY_C4, TXT_C4, "C4", 53, 16, true, false, true, true, true, false, PIP_WHITE, CivilianDoControls, 2, 0, RemapCiv4);

// Civilian 5
const InfantryTypeClass InfantryC5(INFANTRY_C5, TXT_C5, "C5", 53, 16, false, false, true, true, true, false, PIP_WHITE,
    CivilianDoControls, 2, 0, RemapCiv5);

// Civilian 6
const InfantryTypeClass InfantryC6(INFANTRY_C6, TXT_C6, "C6", 53, 16, false, false, true, true, true, false, PIP_WHITE,
    CivilianDoControls, 2, 0, RemapCiv6);

// Civilian 7
const InfantryTypeClass InfantryC7(INFANTRY_C7, TXT_C7, "C7", 53, 16, false, false, true, true, true, false, PIP_WHITE,
    CivilianDoControls, 2, 0, RemapCiv7);

// Civilian 8
const InfantryTypeClass InfantryC8(INFANTRY_C8, TXT_C8, "C8", 53, 16, false, false, true, true, true, false, PIP_WHITE,
    CivilianDoControls, 2, 0, RemapCiv8);

// Civilian 9
const InfantryTypeClass InfantryC9(INFANTRY_C9, TXT_C9, "C9", 53, 16, false, false, true, true, true, false, PIP_WHITE,
    CivilianDoControls, 2, 0, RemapCiv9);

// Civilian 10
const InfantryTypeClass InfantryC10(INFANTRY_C10, TXT_SCIENTIST, "C10", 53, 16, false, false, true, true, true, false,
    PIP_YELLOW, CivilianDoControls, 2, 0, RemapCiv10);

// Einstein
const InfantryTypeClass InfantryEinstein(INFANTRY_EINSTEIN, TXT_EINSTEIN, "EINSTEIN", 53, 16, false, false, true, false,
    true, false, PIP_YELLOW, EinsteinDoControls, 0, 0, nullptr);

// Delphi
const InfantryTypeClass InfantryDelphi(INFANTRY_DELPHI, TXT_DELPHI, "DELPHI", 53, 16, false, false, true, false, true,
    false, PIP_YELLOW, CivilianDoControls, 2, 0, nullptr);

// Dr Chan
const InfantryTypeClass InfantryDrChan(INFANTRY_CHAN, TXT_CHAN, "CHAN", 53, 16, false, false, true, false, true, false,
    PIP_YELLOW, EinsteinDoControls, 2, 0, nullptr);

// Shock Trooper
const InfantryTypeClass InfantryShockTrooper(INFANTRY_SHOCK_TROOPER, TXT_SHOK, "SHOK", 53, 16, false, true, false, false,
    false, false, PIP_GREEN, ShockTrooperDoControls, 2, 0, nullptr);

// Mechanic
const InfantryTypeClass InfantryMechanic(INFANTRY_MECHANIC, TXT_MECH, "MECH", 53, 16, false, true, false, false, false,
    false, PIP_YELLOW, MechanicDoControls, 25, 25, nullptr);

/**
 * 0x004DF5E0
 */
InfantryTypeClass::InfantryTypeClass(InfantryType type, int uiname, const char *name, int def_fire_coord, int rot_count,
    BOOL female, BOOL crawls, BOOL civilian, BOOL alt_remap, BOOL nominal, BOOL theater, PipEnum pip, DoInfoStruct *sequence,
    int fire_up, int fire_prone, const uint8_t *remap) :
    TechnoTypeClass(RTTI_INFANTRYTYPE, type, uiname, name, REMAP_1, def_fire_coord, rot_count, 0, 0, 0, nominal, true, true,
        true, false, false, theater, false, true, true, FACING_COUNT_8, SPEED_FOOT),

    m_FemaleVoice(female),
    m_IsCrawler(crawls),
    m_IsInfiltrator(false),
    m_IsFraidycat(false),
    m_IsCivilian(civilian),
    m_HasC4(false),
    m_IsCanine(false),
    m_HasAltRemap(alt_remap),
    m_Type(type),
    m_Pip(pip),
    m_DoControl(sequence),
    m_FireUp(fire_up),
    m_FireProne(fire_prone),
    m_Remap(remap)
{
    // Set techno flags that don't get set correctly by the TechnoTypeClass ctor.
    Crushable = true;
    IsScanner = true;
    IsRepairable = false;
}

/**
 * 0x004EB2F0
 */
InfantryTypeClass::InfantryTypeClass(InfantryTypeClass const &that) :
    TechnoTypeClass(that),
    m_FemaleVoice(that.m_FemaleVoice),
    m_IsCrawler(that.m_IsCrawler),
    m_IsInfiltrator(that.m_IsInfiltrator),
    m_IsFraidycat(that.m_IsFraidycat),
    m_IsCivilian(that.m_IsCivilian),
    m_HasC4(that.m_HasC4),
    m_IsCanine(that.m_IsCanine),
    m_HasAltRemap(that.m_HasAltRemap),
    m_Type(that.m_Type),
    m_Pip(that.m_Pip),
    m_DoControl(that.m_DoControl),
    m_FireUp(that.m_FireUp),
    m_FireProne(that.m_FireProne),
    m_Remap(that.m_Remap)
{
}

/**
 * 0x004DF728
 */
void *InfantryTypeClass::operator new(size_t size)
{
    DEBUG_ASSERT(size == sizeof(InfantryTypeClass) && size == g_InfantryTypes.Heap_Size());
    return g_InfantryTypes.Allocate();
}

/**
 * 0x004DF73C
 */
void InfantryTypeClass::operator delete(void *ptr)
{
    DEBUG_ASSERT(ptr != nullptr);
    g_InfantryTypes.Free(ptr);
}

/**
 * Fetches the name of the object for display purposes.
 *
 * 0x004EB16C
 */
int InfantryTypeClass::Full_Name() const
{
    if (g_inMapEditor || !IsNominal || Rule.Named_Civilians() || m_Type == INFANTRY_C10 || m_Type == INFANTRY_EINSTEIN
        || m_Type == INFANTRY_DELPHI) {
        return AbstractTypeClass::Full_Name();
    }

    return TXT_CIVILIAN;
}

/**
 * Fetches the width and height of the object.
 *
 * 0x004EB2DC
 */
void InfantryTypeClass::Dimensions(int &w, int &h) const
{
    w = 14;
    h = 20;
}

/**
 * Creates an InfantyClass instance for the specified house and places it at the specified cell.
 *
 * 0x004EAF74
 */
BOOL InfantryTypeClass::Create_And_Place(int16_t cellnum, HousesType house) const
{
#ifndef CHRONOSHIFT_STANDALONE
    BOOL (*func)
    (const InfantryTypeClass *, int16_t, HousesType) =
        reinterpret_cast<BOOL (*)(const InfantryTypeClass *, int16_t, HousesType)>(0x004EAF74);
    return func(this, cellnum, house);
#else
    /*InfantryClass *iptr = new InfantryClass(Type, house);

    if (iptr != nullptr) {
        uint32_t spot = Map[cellnum].Closest_Free_Spot(Cell_To_Coord(cellnum));
        if (spot != 0) {
            return iptr->Unlimbo(spot, DIR_EAST);
        }
    }*/

    return false;
#endif
}

/**
 * Creates an InfantyClass for the specified house.
 *
 * 0x004EAF20
 */
ObjectClass *InfantryTypeClass::Create_One_Of(HouseClass *house) const
{
#ifndef CHRONOSHIFT_STANDALONE
    ObjectClass *(*func)(const InfantryTypeClass *, HouseClass *) =
        reinterpret_cast<ObjectClass *(*)(const InfantryTypeClass *, HouseClass *)>(0x004EAF20);
    return func(this, house);
#else
    /*DEBUG_ASSERT(house != nullptr);
    return new InfantryClass(Type, house->What_Type());*/

    return nullptr;
#endif
}

/**
 * Fetches the occupy list for this type.
 *
 * 0x004EB01C
 */
const int16_t *InfantryTypeClass::Occupy_List(BOOL a1) const
{
    // Infantry always occupy only the one cell
    static int16_t _list[] = { 0, LIST_END };

    return _list;
}

/**
 * Reads an object of this type from an ini file.
 *
 * 0x004EB1D0
 */
BOOL InfantryTypeClass::Read_INI(GameINIClass &ini)
{
    if (TechnoTypeClass::Read_INI(ini)) {
        m_IsFraidycat = ini.Get_Bool(Get_Name(), "Fraidycat", m_IsFraidycat);
        m_IsInfiltrator = ini.Get_Bool(Get_Name(), "Infiltrate", m_IsInfiltrator);
        m_HasC4 = ini.Get_Bool(Get_Name(), "C4", m_HasC4);
        m_IsCanine = ini.Get_Bool(Get_Name(), "IsCanine", m_IsCanine);

        // C4 presumes infiltrate.
        if (m_HasC4) {
            m_IsInfiltrator = true;
        }

        // Dogs can't be leaders.
        if (m_IsCanine) {
            IsLeader = false;
        }

        return true;
    }

    return false;
}
