/*
 * File:   collision.h
 * Author: KRIK
 *
 * Created on 11 Èþëü 2009 ã., 16:23
 */

#include "gamelogic.h"

#ifndef _COLLISION_H
#define _COLLISION_H

void detectBulletToWallCollision(_bullet *bull);
void detectBulletToUnitsCollision(_bullet *bull);
void detectBulletToStaffCollision(_bullet *bull);
void detectBulletToBulletCollision(_bullet *bull);
u8 detectBonusCollision(u8 player);
u8 moveAvailableInWalls(_tank *tank);
u8 moveAvailableInUnits(_tank *tank);

#endif /* _COLLISION_H */

