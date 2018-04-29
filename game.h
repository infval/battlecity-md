/*
 * File:   game.h
 * Author: KRIK
 *
 * Created on 8 Èþëü 2009 ã., 10:55
 */

#ifndef _GAME_H
#define	_GAME_H



void sleepFrames(int i);
void startGame();

extern s16 selected_stage;
extern u8 kills_1[4];
extern u8 kills_2[4];
extern u32 top_scor;
extern u8 stop_sound_engine;
extern u8 playing_engine;
extern u16 level_timer;

void soundStopEngine();
#endif	/* _GAME_H */

