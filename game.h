/*
 * File:   game.h
 * Author: KRIK
 *
 * Created on 8 Èþëü 2009 ã., 10:55
 */

#ifndef _GAME_H
#define _GAME_H

void sleepFrames(u16 i);
void startGame();
void soundStopEngine();

extern s16 selected_stage;
extern u8 kills_1[5];
extern u8 kills_2[5];
extern u32 top_scor;

#endif /* _GAME_H */

