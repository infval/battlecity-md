/*
 * File:   sprite.h
 * Author: KRIK
 *
 * Created on 8 Èþëü 2009 ã., 13:37
 */
#include "gamelogic.h"

#ifndef _SPRITE_H
#define _SPRITE_H

#define SPRITE_POS_SELECTOR_X 48
#define SPRITE_POS_SELECTOR_Y 116
#define SPRITE_TOTAL_NUM      96
#define SPRITE_ADDR_TANK      256
#define SPRITE_ADDR_SHIP      40
#define SPRITE_ADDR_ARMOR     204
#define SPRITE_ADDR_BIRTH     160
#define SPRITE_ADDR_BULLET    176
#define SPRITE_ADDR_SCORE     184
#define SPRITE_EXPLODE_SMALL  244
#define SPRITE_EXPLODE_BIG    212
#define SPRITE_GAMEOVER       120
#define SPRITE_BONUS          128
#define SPRITE_PAUSE          105

#define TANK_COLOR_RED           0
#define TANK_COLOR_YELLOW        1
#define TANK_COLOR_GREEN         2
#define TANK_COLOR_GREY          3
#define TANK_COLOR_GREY_STROBE   4
#define TANK_COLOR_YELLOW_STROBE 5
#define TANK_COLOR_GREEN_STROBE  6

void spriteInit();
void drawSprite8x8(u16 sprite_atr, s16 x, s16 y);
void drawSprite2x2(u16 sprite_atr, s16 x, s16 y);
void drawSprite1x2(u16 sprite_atr, s16 x, s16 y);
void drawSprite1x1(u16 sprite_atr, s16 x, s16 y);
void drawTank(_tank *sprite);
void updateSprite();
void drawSprite4x1(u16 sprite_atr, s16 x, s16 y);
void drawBonus(_bonus *bonus);

#endif /* _SPRITE_H */

