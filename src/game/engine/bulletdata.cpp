/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Static instances of bullet type objects.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "bulletdata.h"

/**
 * For reference, the constructor argument order is;
 *
 * Type (BulletType)
 * Name
 * 
 */

const BulletTypeClass BulletInvisible(BULLET_INVISIBLE, "Invisible");

const BulletTypeClass BulletCannon(BULLET_CANNON, "Cannon");

const BulletTypeClass BulletAck(BULLET_ACK, "Ack");

const BulletTypeClass BulletTorpedo(BULLET_TORPEDO, "Torpedo");

const BulletTypeClass BulletFROG(BULLET_FROG, "FROG");

const BulletTypeClass BulletHeatSeeker(BULLET_HEATSEEKER, "HeatSeeker");

const BulletTypeClass BulletLaserGuided(BULLET_LASERGUIDED, "LaserGuided");

const BulletTypeClass BulletLobbed(BULLET_LOBBED, "Lobbed");

const BulletTypeClass BulletBomblet(BULLET_BOMBLET, "Bomblet");

const BulletTypeClass BulletBallistic(BULLET_BALLISTIC, "Ballistic");

const BulletTypeClass BulletParachute(BULLET_PARACHUTE, "Parachute");

const BulletTypeClass BulletFireball(BULLET_FIREBALL, "Fireball");

const BulletTypeClass BulletLeapDog(BULLET_LEAPDOG, "LeapDog");

const BulletTypeClass BulletCatapult(BULLET_CATAPULT, "Catapult");

const BulletTypeClass BulletAAMissile(BULLET_AAMISSILE, "AAMissile");

const BulletTypeClass BulletGPSSatellite(BULLET_GPSSATELLITE, "GPSSatellite");

const BulletTypeClass BulletNukeUp(BULLET_NUKEUP, "NukeUp");

const BulletTypeClass BulletNukeDown(BULLET_NUKEDOWN, "NukeDown");
