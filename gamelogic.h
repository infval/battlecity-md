
#ifndef _GAMELOGIC_H
#define	_GAMELOGIC_H

#include "defs.h"


#define EXPLODE_BIG 1
#define EXPLODE_SMALL 2

//#define MAX_EXPLODE (PLAYERS_ON_MAP << 1)
//#define MAX_BULLETES (PLAYERS_ON_MAP << 1)

typedef struct {
    s16 posx;
    s16 posy;
    s16 timer;
    s16 val;
} _scor;

typedef struct {
    s16 tank1;
    s16 tank2;
    s16 tank3;
    s16 tank4;
    s16 tank5;
    s16 birth_time;
} _level_config;

typedef struct {
    s16 posx;
    s16 posy;
    s16 type;
} _bonus;

typedef struct {
    s16 posx;
    s16 posy;
    s16 ani_counter;
    s16 strobe;
    s16 type;
} _explode;

typedef struct {
    s16 posx;
    s16 posy;
    s16 color;
    s16 rotate;
    s16 hitpoint;
    s16 god;
    s16 speed;
    s16 lives;
    s16 birth;
    s16 fire;
    s16 bullets_count;
    s16 freeze;
    s16 bullet_speed;
    s16 bullet_limit;
    s16 bonus;
    s16 collision;
    s16 type;
    s16 fire_timer;
    s16 uranium_bullets;
    s16 dirt;
    s32 scor;
	s16 hp;
	s16 ship;

    s16 bullet_delay;
    s16 on_dirt;
    s16 grass_trim;
    s16 player;
    s16 tank_type;
} _tank;

typedef struct {
    s16 posx;
    s16 posy;
    s16 rotate;
    s16 speed;
    s16 ricocheted;
    _tank *maker;
} _bullet;

extern _tank game_player[];
extern _bullet bullets[];
extern _explode explodes[];
extern const s8 speed_x[];
extern const s8 speed_y[];
extern _bonus bonus;
extern u8 gameover;


void GLog_updateGame();
void GLog_initLEvel(u16 level);
void GLog_killBullet(_bullet *bull, u8 explode);
void GLog_killPLayer(_tank *victim, _tank *killer);
void GLog_killStaff();
void GLog_initGameLogic();
u8 GLog_gameover();
u8 GLog_victory();
void GLog_setVictory();
void GLog_removeEnemy();

extern u16 pause;
void showScoreQuad(u16 val, u16 posx, u16 posy);


extern _scor scor;
#endif	/* _GAMELOGIC_H */

