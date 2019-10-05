/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Static instances of anim type objects.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "animdata.h"

/**
 * For reference, the constructor argument order is;
 *
 * Type (AnimType)
 * Name
 * A3
 * Mid Point
 * Theater
 * Normalized
 * A7
 * Scorch
 * Crater
 * Sticky
 * Surface
 * Translucent
 * Flamer
 * Damage
 * Rate
 * Start
 * Loop Start
 * Loop End
 * End
 * Loop Count
 * Report (VocType)
 * Next (AnimType
 *
 */

const AnimTypeClass AnimAtomBomb(ANIM_ATOMSFX, "ATOMSFX", 72, 3, false, false, false, true, true, false, false, false, false,
    fixed_t(0, 1), 1, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimSputDoor(ANIM_SPUTDOOR, "SPUTDOOR", 42, 1, false, true, false, false, false, false, false, false,
    false, fixed_t(0, 1), 1, 0, 0, -1, -1, 1, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimElectricDie(
    ANIM_ELECTRO, "ELECTRO", 16, 0, true, 0, 0, 1, 0, 0, 1, 0, 0, fixed_t(0, 1), 1, 0, 0, 3, -1, 5, VOC_NONE, ANIM_FIRE2);

const AnimTypeClass AnimDogElectricDie(
    ANIM_ELECTDOG, "ELECTDOG", 17, 0, false, 0, 0, 1, 0, 0, 1, 0, 0, fixed_t(0, 1), 1, 0, 0, 3, -1, 5, VOC_NONE, ANIM_FIRE2);

const AnimTypeClass AnimSAMN(ANIM_SAM_N, "SAMFIRE", 55, 4, false, false, false, false, false, false, false, false, false,
    fixed_t(0, 1), 1, 0, 0, 0, 18, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimSAMNW(ANIM_SAM_NW, "SAMFIRE", 55, 22, false, false, false, false, false, false, false, false, false,
    fixed_t(0, 1), 1, 18, 0, 0, 18, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimSAMW(ANIM_SAM_W, "SAMFIRE", 55, 40, false, false, false, false, false, false, false, false, false,
    fixed_t(0, 1), 1, 36, 0, 0, 18, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimSAMSW(ANIM_SAM_SW, "SAMFIRE", 55, 58, false, false, false, false, false, false, false, false, false,
    fixed_t(0, 1), 1, 54, 0, 0, 18, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimSAMS(ANIM_SAM_S, "SAMFIRE", 55, 76, false, false, false, false, false, false, false, false, false,
    fixed_t(0, 1), 1, 72, 0, 0, 18, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimSAMSE(ANIM_SAM_SE, "SAMFIRE", 55, 94, false, false, false, false, false, false, false, false, false,
    fixed_t(0, 1), 1, 90, 0, 0, 18, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimSAME(ANIM_SAM_E, "SAMFIRE", 55, 112, false, false, false, false, false, false, false, false, false,
    fixed_t(0, 1), 1, 108, 0, 0, 18, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimSAMNE(ANIM_SAM_NE, "SAMFIRE", 55, 130, false, false, false, false, false, false, false, false, false,
    fixed_t(0, 1), 1, 126, 0, 0, 18, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimLZSmoke(
    ANIM_LZ_SMOKE, "SMOKLAND", 32, 72, 0, 1, 0, 0, 0, 0, 1, 0, 0, fixed_t(0, 1), 2, 0, 72, 91, -1, -1, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimBurnSmall(
    ANIM_BURN_SMALL, "BURN-S", 11, 13, 0, 0, 0, 0, 0, 0, 1, 0, 0, fixed_t(1, 32), 2, 0, 30, 62, -1, 4, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimBurnMed(
    ANIM_BURN_MED, "BURN-M", 14, 13, 0, 0, 0, 0, 0, 0, 1, 0, 0, fixed_t(1, 16), 2, 0, 30, 62, -1, 4, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimBurnBig(
    ANIM_BURN_BIG, "BURN-L", 23, 13, 0, 0, 0, 1, 0, 0, 1, 0, 0, fixed_t("0.03"), 2, 0, 30, 62, -1, 4, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimOnFireSmall(ANIM_ON_FIRE_SMALL, "BURN-S", 11, 13, 0, 0, 0, 0, 0, 0, 1, 0, 0, fixed_t(1, 32), 2, 0, 30,
    62, -1, 4, VOC_NONE, ANIM_SMOKE_M);

const AnimTypeClass AnimOnFireMed(ANIM_ON_FIRE_MED, "BURN-M", 14, 13, 0, 0, 0, 0, 0, 0, 1, 0, 0, fixed_t(1, 16), 2, 0, 30, 62,
    -1, 4, VOC_NONE, ANIM_ON_FIRE_SMALL);

const AnimTypeClass AnimOnFireBig(ANIM_ON_FIRE_BIG, "BURN-L", 23, 13, 0, 0, 0, 1, 0, 0, 1, 0, 0, fixed_t(1, 10), 2, 0, 30, 62,
    -1, 4, VOC_NONE, ANIM_ON_FIRE_MED);

const AnimTypeClass AnimParachute(ANIM_PARACHUTE, "PARACH", 32, 15, false, false, false, false, false, false, false, false,
    false, fixed_t(0, 1), 4, 0, 7, -1, -1, 15, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimParaBomb(ANIM_PARABOMB, "PARABOMB", 32, 8, false, false, false, false, false, false, false, false,
    false, fixed_t(0, 1), 4, 0, 7, -1, -1, 15, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimFBall1(
    ANIM_FBALL1, "FBALL1", 67, 6, 0, 1, 0, 0, 1, 0, 0, 0, 0, fixed_t(0, 1), 1, 0, 0, -1, -1, 1, (VocType)77, ANIM_NONE);

const AnimTypeClass AnimFrag1(
    ANIM_FRAG1, "FRAG1", 45, 3, 0, 1, 0, 0, 1, 1, 1, 0, 0, fixed_t(0, 1), 1, 0, 0, -1, -1, 1, (VocType)76, ANIM_NONE);

const AnimTypeClass AnimVehHit1(
    ANIM_VEH_HIT1, "VEH-HIT1", 30, 4, 0, 1, 0, 0, 1, 1, 0, 0, 0, fixed_t(0, 1), 1, 0, 0, -1, -1, 1, (VocType)77, ANIM_NONE);

const AnimTypeClass AnimVehHit2(
    ANIM_VEH_HIT2, "VEH-HIT2", 21, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, fixed_t(0, 1), 1, 0, 0, -1, -1, 1, (VocType)50, ANIM_NONE);

const AnimTypeClass AnimVehHit3(
    ANIM_VEH_HIT3, "VEH-HIT3", 19, 3, 0, 1, 0, 0, 0, 1, 0, 0, 0, fixed_t(0, 1), 1, 0, 0, -1, -1, 1, (VocType)50, ANIM_NONE);

const AnimTypeClass AnimArtExp1(
    ANIM_ART_EXP1, "ART-EXP1", 41, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, fixed_t(0, 1), 1, 0, 0, -1, -1, 1, (VocType)53, ANIM_NONE);

const AnimTypeClass AnimNapalm1(
    ANIM_NAPALM1, "NAPALM1", 21, 5, 0, 0, 0, 1, 0, 0, 0, 0, true, fixed_t(0, 1), 1, 0, 0, -1, -1, 1, (VocType)40, ANIM_NONE);

const AnimTypeClass AnimNapalm2(
    ANIM_NAPALM2, "NAPALM2", 41, 5, 0, 0, 0, 1, 0, 0, 0, 0, true, fixed_t(0, 1), 1, 0, 0, -1, -1, 1, (VocType)40, ANIM_NONE);

const AnimTypeClass AnimNapalm3(
    ANIM_NAPALM3, "NAPALM3", 78, 5, 0, 0, 0, 1, 0, 0, 0, 0, true, fixed_t(0, 1), 1, 0, 0, -1, -1, 1, (VocType)39, ANIM_NONE);

const AnimTypeClass AnimSmokePuff(
    ANIM_SMOKE_PUFF, "SMOKEY", 24, 2, 0, 1, 0, 0, 0, 0, 0, 1, 0, fixed_t(0, 1), 1, 0, 0, -1, -1, 1, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimFireBallFade(
    ANIM_FIREBALLFADE, "FB2", 24, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed_t(0, 1), 1, 0, 0, -1, -1, 1, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimPiff(
    ANIM_PIFF, "PIFF", 13, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed_t(0, 1), 1, 0, 0, -1, -1, 1, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimPiffPiff(
    ANIM_PIFFPIFF, "PIFFPIFF", 20, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed_t(0, 1), 1, 0, 0, -1, -1, 1, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimFire3(
    ANIM_FIRE3, "FIRE3", 23, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, fixed_t(1, 32), 1, 0, 0, -1, -1, 2, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimFire1(
    ANIM_FIRE1, "FIRE1", 23, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, fixed_t(1, 16), 1, 0, 0, -1, -1, 3, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimFire4(
    ANIM_FIRE4, "FIRE4", 7, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, fixed_t(1, 32), 1, 0, 0, -1, -1, 3, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimFire2(
    ANIM_FIRE2, "FIRE2", 23, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, fixed_t(1, 16), 1, 0, 0, -1, -1, 3, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimOilFieldBurn(ANIM_OILFIELD_BURN, "FLMSPT", 42, 58, 0, 1, 0, 0, 0, 0, 1, 0, 0, fixed_t(0, 1), 1, 0, 33,
    99, 66, 65535, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimGunfire(
    ANIM_MUZZLE_FLASH, "GUNFIRE", 16, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, fixed_t(0, 1), 1, 0, 0, 0, 1, 1, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimSmokeM(
    ANIM_SMOKE_M, "SMOKE_M", 28, 30, 0, 1, 0, 0, 0, 0, 1, 0, 0, fixed_t(0, 1), 1, 0, 67, -1, -1, 6, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimGUNN(ANIM_GUN_N, "MINIGUN", 18, false, false, false, false, false, false, false, false, false, 0,
    fixed_t(0, 1), 1, 0, 0, 0, 6, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimGUNNW(ANIM_GUN_NW, "MINIGUN", 18, false, false, false, false, false, false, false, false, false, 0,
    fixed_t(0, 1), 1, 6, 0, 0, 6, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimGUNW(ANIM_GUN_W, "MINIGUN", 18, false, false, false, false, false, false, false, false, false, 0,
    fixed_t(0, 1), 1, 12, 0, 0, 6, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimGUNSW(ANIM_GUN_SW, "MINIGUN", 18, false, false, false, false, false, false, false, false, false, 0,
    fixed_t(0, 1), 1, 18, 0, 0, 6, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimGUNS(ANIM_GUN_S, "MINIGUN", 18, false, false, false, false, false, false, false, false, false, 0,
    fixed_t(0, 1), 1, 24, 0, 0, 6, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimGUNSE(ANIM_GUN_SE, "MINIGUN", 18, false, false, false, false, false, false, false, false, false, 0,
    fixed_t(0, 1), 1, 30, 0, 0, 6, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimGUNE(ANIM_GUN_E, "MINIGUN", 18, false, false, false, false, false, false, false, false, false, 0,
    fixed_t(0, 1), 1, 36, 0, 0, 6, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimGUNNE(ANIM_GUN_NE, "MINIGUN", 18, false, false, false, false, false, false, false, false, false, 0,
    fixed_t(0, 1), 1, 42, 0, 0, 6, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimCrateDeviator(
    ANIM_CRATE_DEVIATOR, "DEVIATOR", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed_t(0, 1), 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimCrateArmor(
    ANIM_CRATE_ARMOR, "ARMOR", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed_t(0, 1), 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimCrateSpeed(
    ANIM_CRATE_SPEED, "SPEED", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed_t(0, 1), 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimCrateFPower(
    ANIM_CRATE_FPOWER, "FPOWER", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed_t(0, 1), 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimCrateTQuake(
    ANIM_CRATE_TQUAKE, "TQUAKE", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed_t(0, 1), 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimCrateDollar(
    ANIM_CRATE_DOLLAR, "DOLLAR", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed_t(0, 1), 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimCrateEarth(
    ANIM_CRATE_EARTH, "EARTH", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed_t(0, 1), 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimCrateEmpulse(
    ANIM_CRATE_EMPULSE, "EMPULSE", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed_t(0, 1), 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimCrateInvun(
    ANIM_CRATE_INVUN, "INVUN", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed_t(0, 1), 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimCrateMine(
    ANIM_CRATE_MINE, "MINE", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed_t(0, 1), 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimCrateRapid(
    ANIM_CRATE_RAPID, "RAPID", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed_t(0, 1), 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimCrateStealth(
    ANIM_CRATE_STEALTH, "STEALTH2", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed_t(0, 1), 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimChronoBox(
    ANIM_CHRONOBOX, "CHRONBOX", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed_t(0, 1), 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimGPSBox(
    ANIM_GPSBOX, "GPSBOX", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed_t(0, 1), 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimInvulBox(
    ANIM_INVULBOX, "INVULBOX", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed_t(0, 1), 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimParaBox(
    ANIM_PARABOX, "PARABOX", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed_t(0, 1), 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimSonarBox(
    ANIM_SONARBOX, "SONARBOX", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed_t(0, 1), 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimCrateMissile(
    ANIM_CRATE_MISSILE, "MISSILE2", 48, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed_t(0, 1), 2, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimMoveFlash(
    ANIM_MOVEFLASH, "MOVEFLSH", 24, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, fixed_t(0, 1), 1, 0, 0, 0, -1, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimCorpse1(ANIM_CORPSE1, "CORPSE1", 24, true, true, true, 0, 0, 0, 0, true, true, 0, fixed_t(0, 1), 15, 0,
    0, 0, -1, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimCorpse2(ANIM_CORPSE2, "CORPSE2", 24, true, true, true, 0, 0, 0, 0, true, true, 0, fixed_t(0, 1), 15, 0,
    0, 0, -1, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimCorpse3(ANIM_CORPSE3, "CORPSE3", 24, true, true, true, 0, 0, 0, 0, true, true, 0, fixed_t(0, 1), 15, 0,
    0, 0, -1, 0, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimTwinkle1(
    ANIM_TWINKLE1, "TWINKLE1", 8, true, 0, true, 0, 0, 0, 0, 0, 0, 0, fixed_t(0, 1), 1, 0, 0, -1, -1, 1, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimTwinkle2(
    ANIM_TWINKLE2, "TWINKLE2", 8, true, 0, true, 0, 0, 0, 0, 0, 0, 0, fixed_t(0, 1), 1, 0, 0, -1, -1, 1, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimTwinkle3(
    ANIM_TWINKLE3, "TWINKLE3", 8, true, 0, true, 0, 0, 0, 0, 0, 0, 0, fixed_t(0, 1), 1, 0, 0, -1, -1, 1, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimFlak(
    ANIM_FLAK, "FLAK", 8, 7, 0, 1, 0, 0, 0, 0, 0, 0, 0, fixed_t(0, 1), 1, 0, 0, -1, -1, 1, VOC_NONE, ANIM_NONE);

const AnimTypeClass AnimWaterExp1(
    ANIM_WATER_EXP1, "H2O_EXP1", 64, 3, 0, 1, 0, 0, 0, 0, 1, 0, 0, fixed_t(0, 1), 1, 0, 0, -1, -1, 1, VOC_SPLASH9, ANIM_NONE);

const AnimTypeClass AnimWaterExp2(
    ANIM_WATER_EXP2, "H2O_EXP2", 40, 3, 0, 1, 0, 0, 0, 0, 1, 0, 0, fixed_t(0, 1), 1, 0, 0, -1, -1, 1, VOC_SPLASH9, ANIM_NONE);

const AnimTypeClass AnimWaterExp3(
    ANIM_WATER_EXP3, "H2O_EXP3", 32, 3, 0, 1, 0, 0, 0, 0, 1, 0, 0, fixed_t(0, 1), 1, 0, 0, -1, -1, 1, VOC_SPLASH9, ANIM_NONE);

const AnimTypeClass AnimMineExp1(
    ANIM_MINEEXP1, "VEH-HIT2", 21, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, fixed_t(0, 1), 1, 0, 0, -1, -1, 1, VOC_MINEBLO1, ANIM_NONE);

const AnimTypeClass AnimAntDeath(
    ANIM_ANTDEATH, "ANTDIE", 28, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, fixed_t(0, 1), 4, 0, 0, -1, -1, 1, VOC_ANT_DIE, ANIM_NONE);
