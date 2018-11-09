/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Contains static data for anim objects.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "animtype.h"
#include "gamefile.h"
#include "coord.h"
#include "mixfile.h"
#include "voc.h"
#include <cstdio>

using std::snprintf;

#ifndef CHRONOSHIFT_STANDALONE
TFixedIHeapClass<AnimTypeClass> &AnimTypes = Make_Global<TFixedIHeapClass<AnimTypeClass> >(0x0065DEA0);
#else
TFixedIHeapClass<AnimTypeClass> AnimTypes;
#endif

AnimTypeClass const AnimAtomBomb(ANIM_ATOMSFX, "ATOMSFX", 72, 3, false, false, false, true, true, false, false, false, false,
    fixed::_0_1, 1, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimSputDoor(ANIM_SPUTDOOR, "SPUTDOOR", 42, 1, false, true, false, false, false, false, false, false,
    false, fixed::_0_1, 1, 0, 0, -1, -1, 1, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimElectricDie(
    ANIM_ELECTRO, "ELECTRO", 16, 0, true, 0, 0, 1, 0, 0, 1, 0, 0, fixed::_0_1, 1, 0, 0, 3, -1, 5, VOC_NONE, ANIM_FIRE2);
AnimTypeClass const AnimDogElectricDie(
    ANIM_ELECTDOG, "ELECTDOG", 17, 0, false, 0, 0, 1, 0, 0, 1, 0, 0, fixed::_0_1, 1, 0, 0, 3, -1, 5, VOC_NONE, ANIM_FIRE2);
AnimTypeClass const AnimSAMN(ANIM_SAM_N, "SAMFIRE", 55, 4, false, false, false, false, false, false, false, false, false,
    fixed::_0_1, 1, 0, 0, 0, 18, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimSAMNW(ANIM_SAM_NW, "SAMFIRE", 55, 22, false, false, false, false, false, false, false, false, false,
    fixed::_0_1, 1, 18, 0, 0, 18, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimSAMW(ANIM_SAM_W, "SAMFIRE", 55, 40, false, false, false, false, false, false, false, false, false,
    fixed::_0_1, 1, 36, 0, 0, 18, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimSAMSW(ANIM_SAM_SW, "SAMFIRE", 55, 58, false, false, false, false, false, false, false, false, false,
    fixed::_0_1, 1, 54, 0, 0, 18, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimSAMS(ANIM_SAM_S, "SAMFIRE", 55, 76, false, false, false, false, false, false, false, false, false,
    fixed::_0_1, 1, 72, 0, 0, 18, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimSAMSE(ANIM_SAM_SE, "SAMFIRE", 55, 94, false, false, false, false, false, false, false, false, false,
    fixed::_0_1, 1, 90, 0, 0, 18, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimSAME(ANIM_SAM_E, "SAMFIRE", 55, 112, false, false, false, false, false, false, false, false, false,
    fixed::_0_1, 1, 108, 0, 0, 18, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimSAMNE(ANIM_SAM_NE, "SAMFIRE", 55, 130, false, false, false, false, false, false, false, false, false,
    fixed::_0_1, 1, 126, 0, 0, 18, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimLZSmoke(
    ANIM_LZ_SMOKE, "SMOKLAND", 32, 72, 0, 1, 0, 0, 0, 0, 1, 0, 0, fixed::_0_1, 2, 0, 72, 91, -1, -1, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimBurnSmall(
    ANIM_BURN_SMALL, "BURN-S", 11, 13, 0, 0, 0, 0, 0, 0, 1, 0, 0, fixed(1, 32), 2, 0, 30, 62, -1, 4, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimBurnMed(
    ANIM_BURN_MED, "BURN-M", 14, 13, 0, 0, 0, 0, 0, 0, 1, 0, 0, fixed(1, 16), 2, 0, 30, 62, -1, 4, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimBurnBig(
    ANIM_BURN_BIG, "BURN-L", 23, 13, 0, 0, 0, 1, 0, 0, 1, 0, 0, fixed("0.03"), 2, 0, 30, 62, -1, 4, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimOnFireSmall(ANIM_ON_FIRE_SMALL, "BURN-S", 11, 13, 0, 0, 0, 0, 0, 0, 1, 0, 0, fixed(1, 32), 2, 0, 30,
    62, -1, 4, VOC_NONE, ANIM_SMOKE_M);
AnimTypeClass const AnimOnFireMed(ANIM_ON_FIRE_MED, "BURN-M", 14, 13, 0, 0, 0, 0, 0, 0, 1, 0, 0, fixed(1, 16), 2, 0, 30, 62,
    -1, 4, VOC_NONE, ANIM_ON_FIRE_SMALL);
AnimTypeClass const AnimOnFireBig(ANIM_ON_FIRE_BIG, "BURN-L", 23, 13, 0, 0, 0, 1, 0, 0, 1, 0, 0, fixed(1, 10), 2, 0, 30, 62,
    -1, 4, VOC_NONE, ANIM_ON_FIRE_MED);
AnimTypeClass const AnimParachute(ANIM_PARACHUTE, "PARACH", 32, 15, false, false, false, false, false, false, false, false,
    false, fixed::_0_1, 4, 0, 7, -1, -1, 15, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimParaBomb(ANIM_PARABOMB, "PARABOMB", 32, 8, false, false, false, false, false, false, false, false,
    false, fixed::_0_1, 4, 0, 7, -1, -1, 15, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimFBall1(
    ANIM_FBALL1, "FBALL1", 67, 6, 0, 1, 0, 0, 1, 0, 0, 0, 0, fixed::_0_1, 1, 0, 0, -1, -1, 1, (VocType)77, ANIM_NONE);
AnimTypeClass const AnimFrag1(
    ANIM_FRAG1, "FRAG1", 45, 3, 0, 1, 0, 0, 1, 1, 1, 0, 0, fixed::_0_1, 1, 0, 0, -1, -1, 1, (VocType)76, ANIM_NONE);
AnimTypeClass const AnimVehHit1(
    ANIM_VEH_HIT1, "VEH-HIT1", 30, 4, 0, 1, 0, 0, 1, 1, 0, 0, 0, fixed::_0_1, 1, 0, 0, -1, -1, 1, (VocType)77, ANIM_NONE);
AnimTypeClass const AnimVehHit2(
    ANIM_VEH_HIT2, "VEH-HIT2", 21, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, fixed::_0_1, 1, 0, 0, -1, -1, 1, (VocType)50, ANIM_NONE);
AnimTypeClass const AnimVehHit3(
    ANIM_VEH_HIT3, "VEH-HIT3", 19, 3, 0, 1, 0, 0, 0, 1, 0, 0, 0, fixed::_0_1, 1, 0, 0, -1, -1, 1, (VocType)50, ANIM_NONE);
AnimTypeClass const AnimArtExp1(
    ANIM_ART_EXP1, "ART-EXP1", 41, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, fixed::_0_1, 1, 0, 0, -1, -1, 1, (VocType)53, ANIM_NONE);
AnimTypeClass const AnimNapalm1(
    ANIM_NAPALM1, "NAPALM1", 21, 5, 0, 0, 0, 1, 0, 0, 0, 0, true, fixed::_0_1, 1, 0, 0, -1, -1, 1, (VocType)40, ANIM_NONE);
AnimTypeClass const AnimNapalm2(
    ANIM_NAPALM2, "NAPALM2", 41, 5, 0, 0, 0, 1, 0, 0, 0, 0, true, fixed::_0_1, 1, 0, 0, -1, -1, 1, (VocType)40, ANIM_NONE);
AnimTypeClass const AnimNapalm3(
    ANIM_NAPALM3, "NAPALM3", 78, 5, 0, 0, 0, 1, 0, 0, 0, 0, true, fixed::_0_1, 1, 0, 0, -1, -1, 1, (VocType)39, ANIM_NONE);
AnimTypeClass const AnimSmokePuff(
    ANIM_SMOKE_PUFF, "SMOKEY", 24, 2, 0, 1, 0, 0, 0, 0, 0, 1, 0, fixed::_0_1, 1, 0, 0, -1, -1, 1, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimFireBallFade(
    ANIM_FIREBALLFADE, "FB2", 24, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed::_0_1, 1, 0, 0, -1, -1, 1, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimPiff(
    ANIM_PIFF, "PIFF", 13, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed::_0_1, 1, 0, 0, -1, -1, 1, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimPiffPiff(
    ANIM_PIFFPIFF, "PIFFPIFF", 20, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed::_0_1, 1, 0, 0, -1, -1, 1, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimFire3(
    ANIM_FIRE3, "FIRE3", 23, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, fixed(1, 32), 1, 0, 0, -1, -1, 2, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimFire1(
    ANIM_FIRE1, "FIRE1", 23, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, fixed(1, 16), 1, 0, 0, -1, -1, 3, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimFire4(
    ANIM_FIRE4, "FIRE4", 7, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, fixed(1, 32), 1, 0, 0, -1, -1, 3, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimFire2(
    ANIM_FIRE2, "FIRE2", 23, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, fixed(1, 16), 1, 0, 0, -1, -1, 3, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimOilFieldBurn(ANIM_OILFIELD_BURN, "FLMSPT", 42, 58, 0, 1, 0, 0, 0, 0, 1, 0, 0, fixed::_0_1, 1, 0, 33,
    99, 66, 65535, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimGunfire(
    ANIM_MUZZLE_FLASH, "GUNFIRE", 16, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, fixed::_0_1, 1, 0, 0, 0, 1, 1, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimSmokeM(
    ANIM_SMOKE_M, "SMOKE_M", 28, 30, 0, 1, 0, 0, 0, 0, 1, 0, 0, fixed::_0_1, 1, 0, 67, -1, -1, 6, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimGUNN(ANIM_GUN_N, "MINIGUN", 18, false, false, false, false, false, false, false, false, false, 0,
    fixed::_0_1, 1, 0, 0, 0, 6, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimGUNNW(ANIM_GUN_NW, "MINIGUN", 18, false, false, false, false, false, false, false, false, false, 0,
    fixed::_0_1, 1, 6, 0, 0, 6, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimGUNW(ANIM_GUN_W, "MINIGUN", 18, false, false, false, false, false, false, false, false, false, 0,
    fixed::_0_1, 1, 12, 0, 0, 6, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimGUNSW(ANIM_GUN_SW, "MINIGUN", 18, false, false, false, false, false, false, false, false, false, 0,
    fixed::_0_1, 1, 18, 0, 0, 6, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimGUNS(ANIM_GUN_S, "MINIGUN", 18, false, false, false, false, false, false, false, false, false, 0,
    fixed::_0_1, 1, 24, 0, 0, 6, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimGUNSE(ANIM_GUN_SE, "MINIGUN", 18, false, false, false, false, false, false, false, false, false, 0,
    fixed::_0_1, 1, 30, 0, 0, 6, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimGUNE(ANIM_GUN_E, "MINIGUN", 18, false, false, false, false, false, false, false, false, false, 0,
    fixed::_0_1, 1, 36, 0, 0, 6, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimGUNNE(ANIM_GUN_NE, "MINIGUN", 18, false, false, false, false, false, false, false, false, false, 0,
    fixed::_0_1, 1, 42, 0, 0, 6, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimCrateDeviator(
    ANIM_CRATE_DEVIATOR, "DEVIATOR", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed::_0_1, 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimCrateArmor(
    ANIM_CRATE_ARMOR, "ARMOR", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed::_0_1, 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimCrateSpeed(
    ANIM_CRATE_SPEED, "SPEED", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed::_0_1, 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimCrateFPower(
    ANIM_CRATE_FPOWER, "FPOWER", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed::_0_1, 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimCrateTQuake(
    ANIM_CRATE_TQUAKE, "TQUAKE", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed::_0_1, 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimCrateDollar(
    ANIM_CRATE_DOLLAR, "DOLLAR", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed::_0_1, 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimCrateEarth(
    ANIM_CRATE_EARTH, "EARTH", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed::_0_1, 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimCrateEmpulse(
    ANIM_CRATE_EMPULSE, "EMPULSE", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed::_0_1, 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimCrateInvun(
    ANIM_CRATE_INVUN, "INVUN", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed::_0_1, 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimCrateMine(
    ANIM_CRATE_MINE, "MINE", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed::_0_1, 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimCrateRapid(
    ANIM_CRATE_RAPID, "RAPID", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed::_0_1, 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimCrateStealth(
    ANIM_CRATE_STEALTH, "STEALTH2", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed::_0_1, 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimChronoBox(
    ANIM_CHRONOBOX, "CHRONBOX", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed::_0_1, 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimGPSBox(
    ANIM_GPSBOX, "GPSBOX", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed::_0_1, 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimInvulBox(
    ANIM_INVULBOX, "INVULBOX", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed::_0_1, 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimParaBox(
    ANIM_PARABOX, "PARABOX", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed::_0_1, 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimSonarBox(
    ANIM_SONARBOX, "SONARBOX", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed::_0_1, 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimCrateMissile(
    ANIM_CRATE_MISSILE, "MISSILE2", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed::_0_1, 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimMoveFlash(
    ANIM_MOVEFLASH, "MOVEFLSH", 24, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, fixed::_0_1, 1, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimCorpse1(ANIM_CORPSE1, "CORPSE1", 24, true, true, true, 0, 0, 0, 0, true, true, 0, fixed::_0_1, 15, 0,
    0, 0, -1, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimCorpse2(ANIM_CORPSE2, "CORPSE2", 24, true, true, true, 0, 0, 0, 0, true, true, 0, fixed::_0_1, 15, 0,
    0, 0, -1, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimCorpse3(ANIM_CORPSE3, "CORPSE3", 24, true, true, true, 0, 0, 0, 0, true, true, 0, fixed::_0_1, 15, 0,
    0, 0, -1, 0, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimTwinkle1(
    ANIM_TWINKLE1, "TWINKLE1", 8, true, 0, true, 0, 0, 0, 0, 0, 0, 0, fixed::_0_1, 1, 0, 0, -1, -1, 1, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimTwinkle2(
    ANIM_TWINKLE2, "TWINKLE2", 8, true, 0, true, 0, 0, 0, 0, 0, 0, 0, fixed::_0_1, 1, 0, 0, -1, -1, 1, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimTwinkle3(
    ANIM_TWINKLE3, "TWINKLE3", 8, true, 0, true, 0, 0, 0, 0, 0, 0, 0, fixed::_0_1, 1, 0, 0, -1, -1, 1, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimFlak(
    ANIM_FLAK, "FLAK", 8, 7, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed::_0_1, 1, 0, 0, -1, -1, 1, VOC_NONE, ANIM_NONE);
AnimTypeClass const AnimWaterExp1(
    ANIM_WATER_EXP1, "H2O_EXP1", 64, 3, 0, 1, 0, 0, 0, 0, 1, 0, 0, fixed::_0_1, 1, 0, 0, -1, -1, 1, VOC_SPLASH9, ANIM_NONE);
AnimTypeClass const AnimWaterExp2(
    ANIM_WATER_EXP2, "H2O_EXP2", 40, 3, 0, 1, 0, 0, 0, 0, 1, 0, 0, fixed::_0_1, 1, 0, 0, -1, -1, 1, VOC_SPLASH9, ANIM_NONE);
AnimTypeClass const AnimWaterExp3(
    ANIM_WATER_EXP3, "H2O_EXP3", 32, 3, 0, 1, 0, 0, 0, 0, 1, 0, 0, fixed::_0_1, 1, 0, 0, -1, -1, 1, VOC_SPLASH9, ANIM_NONE);
AnimTypeClass const AnimMineExp1(
    ANIM_MINEEXP1, "VEH-HIT2", 21, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, fixed::_0_1, 1, 0, 0, -1, -1, 1, VOC_MINEBLO1, ANIM_NONE);
AnimTypeClass const AnimAntDeath(
    ANIM_ANTDEATH, "ANTDIE", 28, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, fixed::_0_1, 4, 0, 0, -1, -1, 1, VOC_ANT_DIE, ANIM_NONE);
/*AnimTypeClass const AnimWWCrate(ANIM_WWCRATE, "WWCRATE", 28, 2, true, true, 0, 0, 0, 0, 1, false, 0, fixed::_0_1, 4, 0, 0,
    -1, -1, 1, VOC_NONE, ANIM_NONE);*/

/**
 * 0x00407388
 */
AnimTypeClass::AnimTypeClass(AnimType anim, const char *name, int a3, int mid_point, BOOL theater,
    BOOL normalized, BOOL a7, BOOL scorch, BOOL crater, BOOL sticky, BOOL surface,
    BOOL translucent, BOOL flamer, fixed damage, int rate, int start, int loop_start,
    int loop_end, int end, int loop_count, VocType report, AnimType next) :
    ObjectTypeClass(RTTI_ANIMTYPE, 0, 0, 0, 0, 0, 0, 0, 0, TXT_NULL, name),
    Normalized(normalized),
    Surface(surface),
    Translucent(translucent),
    ATBit1_8(a7),
    Flamer(flamer),
    Scorch(scorch),
    Crater(crater),
    Sticky(sticky),
    Theater(theater),
    Type(anim),
    field_13D(a3),
    MidPoint(mid_point),
    Damage(damage),
    Rate(rate),
    Start(start),
    LoopStart(loop_start),
    LoopEnd(loop_end),
    End(end),
    LoopCount(loop_count),
    Report(report),
    Next(next)
{
    //Bit64 = true;    //TS shows what classes set this to true, needs debuging once logics are up and running.
    //Bit128 = false;
    return;
}

/**
 * 0x0041C750
 */
AnimTypeClass::AnimTypeClass(AnimTypeClass const &that) :
    ObjectTypeClass(that),
    Normalized(that.Normalized),
    Surface(that.Surface),
    Translucent(that.Translucent),
    ATBit1_8(that.ATBit1_8),
    Flamer(that.Flamer),
    Scorch(that.Scorch),
    Crater(that.Crater),
    Sticky(that.Sticky),
    Theater(that.Theater),
    Type(that.Type),
    field_13D(that.field_13D),
    MidPoint(that.MidPoint),
    Damage(that.Damage),
    Rate(that.Rate),
    Start(that.Start),
    LoopStart(that.LoopStart),
    LoopEnd(that.LoopEnd),
    End(that.End),
    LoopCount(that.LoopCount),
    Report(that.Report),
    Next(that.Next)
{
}

/**
 * 0x00407564
 */
void *AnimTypeClass::operator new(size_t size)
{
    return AnimTypes.Allocate();
}

/**
 * 0x00407578
 */
void AnimTypeClass::operator delete(void *ptr)
{
    AnimTypes.Free(ptr);
}

/**
 * @brief 
 *
 * 0x0041C730
 */
BOOL AnimTypeClass::Create_And_Place(cell_t cellnum, HousesType house) const
{
#if 0
    coord_t coord = Cell_To_Coord(cellnum);
    AnimClass *aptr = new AnimClass(Type, coord);
    DEBUG_ASSERT(aptr != nullptr);

    return aptr != nullptr;
#else 
    return false;
#endif
}

/**
 * @brief 
 *
 * 0x0041C740
 */
ObjectClass *AnimTypeClass::Create_One_Of(HouseClass *house) const
{
#if 0
    AnimClass *aptr = new AnimClass(Type, 0);
    DEBUG_ASSERT(aptr != nullptr);
    return aptr;
#else
    return nullptr;
#endif
}

/**
 * @brief Initialises the memory heap for AnimTypeClass objects.
 * @warning Order of initialisation is important so enum matches position in the heap.
 *
 * 0x00407590
 */
void AnimTypeClass::Init_Heap()
{
    // The order of heap initialisation MUST match the AnimType enum in animtype.h
    new AnimTypeClass(AnimFBall1);
    new AnimTypeClass(AnimFireBallFade);
    new AnimTypeClass(AnimFrag1);
    new AnimTypeClass(AnimVehHit1);
    new AnimTypeClass(AnimVehHit2);
    new AnimTypeClass(AnimVehHit3);
    new AnimTypeClass(AnimArtExp1);
    new AnimTypeClass(AnimNapalm1);
    new AnimTypeClass(AnimNapalm2);
    new AnimTypeClass(AnimNapalm3);
    new AnimTypeClass(AnimSmokePuff);
    new AnimTypeClass(AnimPiff);
    new AnimTypeClass(AnimPiffPiff);
    new AnimTypeClass(AnimFire3);
    new AnimTypeClass(AnimFire2);
    new AnimTypeClass(AnimFire1);
    new AnimTypeClass(AnimFire4);
    new AnimTypeClass(AnimGunfire);
    new AnimTypeClass(AnimSmokeM);
    new AnimTypeClass(AnimBurnSmall);
    new AnimTypeClass(AnimBurnMed);
    new AnimTypeClass(AnimBurnBig);
    new AnimTypeClass(AnimOnFireSmall);
    new AnimTypeClass(AnimOnFireMed);
    new AnimTypeClass(AnimOnFireBig);
    new AnimTypeClass(AnimSAMN);
    new AnimTypeClass(AnimSAMNE);
    new AnimTypeClass(AnimSAME);
    new AnimTypeClass(AnimSAMSE);
    new AnimTypeClass(AnimSAMS);
    new AnimTypeClass(AnimSAMSW);
    new AnimTypeClass(AnimSAMW);
    new AnimTypeClass(AnimSAMNW);
    new AnimTypeClass(AnimGUNN);
    new AnimTypeClass(AnimGUNNE);
    new AnimTypeClass(AnimGUNE);
    new AnimTypeClass(AnimGUNSE);
    new AnimTypeClass(AnimGUNS);
    new AnimTypeClass(AnimGUNSW);
    new AnimTypeClass(AnimGUNW);
    new AnimTypeClass(AnimGUNNW);
    new AnimTypeClass(AnimLZSmoke);
    new AnimTypeClass(AnimCrateDeviator);
    new AnimTypeClass(AnimCrateDollar);
    new AnimTypeClass(AnimCrateEarth);
    new AnimTypeClass(AnimCrateEmpulse);
    new AnimTypeClass(AnimCrateInvun);
    new AnimTypeClass(AnimCrateMine);
    new AnimTypeClass(AnimCrateRapid);
    new AnimTypeClass(AnimCrateStealth);
    new AnimTypeClass(AnimCrateMissile);
    new AnimTypeClass(AnimMoveFlash);
    new AnimTypeClass(AnimOilFieldBurn);
    new AnimTypeClass(AnimElectricDie);
    new AnimTypeClass(AnimParachute);
    new AnimTypeClass(AnimDogElectricDie);
    new AnimTypeClass(AnimCorpse1);
    new AnimTypeClass(AnimCorpse2);
    new AnimTypeClass(AnimCorpse3);
    new AnimTypeClass(AnimSputDoor);
    new AnimTypeClass(AnimAtomBomb);
    new AnimTypeClass(AnimChronoBox);
    new AnimTypeClass(AnimGPSBox);
    new AnimTypeClass(AnimInvulBox);
    new AnimTypeClass(AnimParaBox);
    new AnimTypeClass(AnimSonarBox);
    new AnimTypeClass(AnimTwinkle1);
    new AnimTypeClass(AnimTwinkle2);
    new AnimTypeClass(AnimTwinkle3);
    new AnimTypeClass(AnimFlak);
    new AnimTypeClass(AnimWaterExp1);
    new AnimTypeClass(AnimWaterExp2);
    new AnimTypeClass(AnimWaterExp3);
    new AnimTypeClass(AnimCrateArmor);
    new AnimTypeClass(AnimCrateSpeed);
    new AnimTypeClass(AnimCrateFPower);
    new AnimTypeClass(AnimCrateTQuake);
    new AnimTypeClass(AnimParaBomb);
    new AnimTypeClass(AnimMineExp1);
    new AnimTypeClass(AnimAntDeath);
    // new AnimTypeClass(AnimWWCrate);
}

/**
 * @brief One time initialisation of data.
 *
 * 0x0041C5E4
 */
void AnimTypeClass::One_Time()
{
    char filename[512];

    for (AnimType anim = ANIM_FIRST; anim < ANIM_COUNT; ++anim) {
        AnimTypeClass *aptr = As_Pointer(anim);
        const char *name = aptr->ImageName[0] != '\0' ? aptr->ImageName : aptr->Get_Name();
        snprintf(filename, sizeof(filename), "%s.shp", name);
        aptr->ImageData = GameFileClass::Retrieve(filename);

    }
}

/**
 * @brief Initialisation of theater specific data for current theater.
 *
 * 0x0041C5E4
 */
void AnimTypeClass::Init(TheaterType theater)
{
    DEBUG_ASSERT(theater < THEATER_COUNT);
    DEBUG_ASSERT(theater != THEATER_NONE);

    char filename[512];

    if (theater != g_lastTheater) {
        for (AnimType anim = ANIM_FIRST; anim < ANIM_COUNT; ++anim) {
            AnimTypeClass *aptr = As_Pointer(anim);
            const char *name = aptr->ImageName[0] != '\0' ? aptr->ImageName : aptr->Get_Name();
            // TODO change the theater info to lower case and standardise on that?
            const char *ext = aptr->Theater ? g_theaters[theater].ext : "shp";
            snprintf(filename, sizeof(filename), "%s.%s", name, ext);
            aptr->ImageData = GameFileClass::Retrieve(filename);
        }

    }
}

/**
 * @brief Gets appropriate enum value from provided string.
 */
AnimType AnimTypeClass::From_Name(const char *name)
{
    DEBUG_ASSERT(name != nullptr);

    if (strcasecmp(name, "<none>") == 0 || strcasecmp(name, "none") == 0) {
        return ANIM_NONE;
    }

    if (name != nullptr) {
        for (AnimType anim = ANIM_FIRST; anim < ANIM_COUNT; ++anim) {
            if (strcasecmp(name, Name_From(anim)) == 0) {
                return anim;
            }
        }
    }

    return ANIM_NONE;
}

/**
 * @brief Gets appropriate enum value from provided string.
 */
const char *AnimTypeClass::Name_From(AnimType anim)
{
    return anim != ANIM_NONE && anim < ANIM_COUNT ? As_Reference(anim).Get_Name() : "<none>";
}

/**
 * @brief Gets a reference to an AnimTypeClass object from an AnimType enum value.
 *
 * 0x0041C714
 */
AnimTypeClass &AnimTypeClass::As_Reference(AnimType anim)
{
    AnimTypeClass *ptr = &AnimTypes[anim];
    DEBUG_ASSERT(ptr != nullptr);
    return *ptr;
}

/**
 * @brief Gets a pointer to an AnimTypeClass object from an AnimType enum value.
 */
AnimTypeClass *AnimTypeClass::As_Pointer(AnimType anim)
{
    return anim != ANIM_NONE && anim < ANIM_COUNT ? &AnimTypes[anim] : nullptr;
}
