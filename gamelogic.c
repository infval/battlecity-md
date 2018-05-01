#include "genesis.h"
#include "defs.h"
#include "gamelogic.h"
#include "menu.h"
#include "map.h"
#include "collision.h"
#include "sprite.h"
#include "resmap.h"
#include "map_editor.h"
#include "game.h"
#include "gameover.h"
//#include "audio.h"
#include "resources.h"
#include "mutator.h"

#define TANK_SPEED_1 1
#define TANK_SPEED_2 3
#define TANK_SPEED_3 0xaa

#define BULLET_SPEED_1 1
#define BULLET_SPEED_2 2
#define BULLET_DELAY 12

#define MAX_BONUS  8
#define ARMOR_STAFF_TIME 1024


#define START_LIVES 3
#define START_ENEMYS 20
#define BIRTH_TIME 64



#define VDP_setTile(plan, tile, x, y) (VDP_setTileMapXY(plan, tile, x, y))
#include "defs.h"

void GLog_updateMove();
void GLog_updatebullets();
void GLog_updateControl(u8 gamer_idx, u16 joy, u16 speed);
void GLog_updateAI();
void GLog_initEnemy();
void GLog_shot(_tank *tank);
void GLog_initGamer(u8 pl);
void GLog_makeExplode(u16 type, s16 x, s16 y);
void updateGameMenu();
void drawStage();
void generateBonus(u8 bns);
void setBonus(u8 player);
void updateArmorStaff();

u8 enemy_num;
u16 birth_timer;

u8 gameover;
u8 victory;

const s8 speed_x[] = {0, -1, 0, 1};
const s8 speed_y[] = {-1, 0, 1, 0};

//_bullet bullets[config.max_bullets];
//_explode explodes[config.max_bullets];
//_tank game_player[config.units_on_map];

_bullet bullets[28];
_explode explodes[28];
_tank game_player[14];

_tank enemy_types[4];
_tank gamer_types[4];
_bonus bonus;

const u16 enemy_pos_x[3] = {START_X_EN_A * 8, START_X_EN_B * 8, START_X_EN_C * 8,};
u8 speed_counter;
u16 victory_timer;
u16 bullet_speed_strobe;
s16 old_enemy_num;
s16 old_lives1;
s16 old_lives2;

s16 freeze;
s16 freeze_players;
s16 armor_staff;
u16 current_birth_time;
u16 pause;
_scor scor;



void GLog_updateMove() {


    u16 i;

    u8 delta_1;
    u8 delta_2;
    u16 speed;
    for (i = 0; i < config.units_on_map; i++) {

        if (!game_player[i].hitpoint) continue;
        if (game_player[i].birth) {
            game_player[i].birth--;
            continue;
        }

        if (game_player[i].god) {
            game_player[i].god--;
        }
        if (game_player[i].dirt) game_player[i].dirt--;

        speed = speed_counter & game_player[i].speed;
        if (!game_player[i].speed && game_player[i].dirt && i < 2) {
            speed = speed_counter & 1;
        }
        if (speed) {

            delta_1 = moveAvailableInUnits(&game_player[i]);
            game_player[i].posx += speed_x[game_player[i].rotate];
            game_player[i].posy += speed_y[game_player[i].rotate];
            if (!moveAvailableInWalls(&game_player[i])) {
                game_player[i].posx -= speed_x[game_player[i].rotate];
                game_player[i].posy -= speed_y[game_player[i].rotate];
                game_player[i].collision++;
            }
            delta_2 = moveAvailableInUnits(&game_player[i]);
            if (delta_1 > delta_2) {
                game_player[i].posx -= speed_x[game_player[i].rotate];
                game_player[i].posy -= speed_y[game_player[i].rotate];
                game_player[i].collision++;
            }
        }

    }

}

void GLog_updatebullets() {

    u16 i;
    _bullet *bull_buff;

    for (i = 0; i < config.max_bullets; i++) {

        bull_buff = &bullets[i];

        if (bull_buff->speed == 0)continue;
        bull_buff->posx += speed_x[bull_buff->rotate] << 1;
        bull_buff->posy += speed_y[bull_buff->rotate] << 1;
        detectBulletToWallCollision(bull_buff);
        if (bull_buff->speed == 0)continue;
        detectBulletToUnitsCollision(bull_buff);
        if (bull_buff->speed == 0)continue;
        detectBulletToStaffCollision(bull_buff);
        if (bull_buff->speed == 0)continue;
        detectBulletToBulletCollision(bull_buff);

        if (bull_buff->speed != BULLET_SPEED_2)continue;

        bull_buff->posx += speed_x[bull_buff->rotate] << 1;
        bull_buff->posy += speed_y[bull_buff->rotate] << 1;
        detectBulletToWallCollision(bull_buff);
        if (bull_buff->speed == 0)continue;
        detectBulletToUnitsCollision(bull_buff);
        if (bull_buff->speed == 0)continue;
        detectBulletToStaffCollision(bull_buff);
        if (bull_buff->speed == 0)continue;
        detectBulletToBulletCollision(bull_buff);

    }

}

void GLog_updateAI() {

    u8 max_enemys = config.max_enemies_on_map;
//    if (mods.en_on_map_inc) max_enemys = config.max_enemies_on_map + 4;

    if (config.game_mode == MODE_BC) {
        if (menuGetSelectedItem() == ITEM_ONE_PLAYER) max_enemys -= 2;
    }

    u8 enemys_on_map = 0;
    u16 i;
    u16 rotate;

    for (i = 2; i < config.units_on_map; i++) {
        if (game_player[i].hitpoint) enemys_on_map++;
    }

    if (enemys_on_map == 0 && enemy_num == 0) {
        victory_timer++;
        if (victory_timer > 256) victory = 1;
        return;
    }
    if (enemys_on_map < max_enemys && enemy_num && !birth_timer) GLog_initEnemy();

    if (birth_timer) birth_timer--;


    for (i = 2; i < config.units_on_map; i++) {
        if (game_player[i].hitpoint) enemys_on_map++;
    }

//    if (freeze) {
//        freeze--;
//        for (i = 2; i < config.units_on_map; i++) {
//            game_player[i].speed = 0;
//            game_player[i].fire = 0;
//        }
//        return;
//    }

//    for (i = 2; i < config.units_on_map; i++) {
//        if (game_player[i].freeze) {
//            game_player[i].freeze--;
//            game_player[i].speed = 0;
//            game_player[i].fire = 0;
//        }
//        else {
//            game_player[i].speed = enemy_types[game_player[i].tank_type].speed;
//        }
//    }

    if (freeze_players) {
        freeze_players--;
        for (i = 0; i < 2; i++) {
            game_player[i].freeze = 1;
        }
    }

//    if (!game_player[2].speed) {
//        for (i = 2; i < config.units_on_map; i++) {
//            game_player[i].speed = 1;
//        }
//    }

    for (i = 2; i < config.units_on_map; i++) {
        if (game_player[i].freeze || freeze) {
            if (game_player[i].freeze) game_player[i].freeze--;
            if (freeze && (i == 2)) freeze--;
            game_player[i].speed = 0;
            game_player[i].fire = 0;
            continue;
        }
        else {
            game_player[i].speed = enemy_types[game_player[i].tank_type].speed;
        }


//        if (game_player[i].freeze) continue;
        if (game_player[i].collision == 1) game_player[i].fire_timer >>= 1;
        if (game_player[i].collision > 5 || random() < 300 || (game_player[i].collision && random() < 2000)) {
            game_player[i].collision = 0;
            rotate = 1 << (random() & 3);
            GLog_updateControl(i, rotate, game_player[i].speed);
        }
        if (game_player[i].fire_timer) {
            game_player[i].fire_timer--;
        } else {
            game_player[i].fire = 1;
            game_player[i].fire_timer = 64 + (random() & 63);
        }

    }

}

void GLog_updateBonus() {
    if (mods.en_invul) {
        if (getTimer(1, 0) > 2000000) {
//        if (getTimer(1, 0) > 900000) {
            generateBonus(5);
//            generateBonus(2);
            startTimer(1);
        }
    }
    if (bonus.type) {
		if (game_player[0].hitpoint)
        if (detectBonusColision(0)) {
            setBonus(0);
        }
		if (game_player[1].hitpoint)
        if (detectBonusColision(1)) {
            setBonus(1);
        }

        if (config.en_bonus_collector) {
            int i;
            for(i = 2; i < config.units_on_map; i++){
                if (game_player[i].hitpoint)
                if (detectBonusColision(i)) {
                    setBonus(i);break;
                }
            }
		}
    }
}

void GLog_killBullet(_bullet *bull, u8 explode) {

    u16 x = bull->posx;
    u16 y = bull->posy;
    u16 r = bull->rotate;
    if (r & 1) {
        x -= 8;
    } else {
        y -= 8;
    }

    bull->speed = 0;
    if (explode)GLog_makeExplode(EXPLODE_SMALL, x, y);
    bull->maker->bullets_count--;
    if (bull->maker->bullets_count < 0) bull->maker->bullets_count = 0;


}

void GLog_killPLayer(_tank *victim, _tank *killer) {


    if (victim->god) {
        return;
    }
    if ((victim == &game_player[0] || victim == &game_player[1]) && (killer == &game_player[0] || killer == &game_player[1]) && (!mods.pvp_kills)) {

        switch (config.pl_friend_fire) {
            case BLOCK:
                victim->freeze = 256;
                soundPlay(snd_bull_stop, sizeof (snd_bull_stop), SOUND_PCM_CH3, 0);
                break;
            case BULLETS_STOP:
                soundPlay(snd_bull_stop, sizeof (snd_bull_stop), SOUND_PCM_CH3, 0);
                break;
        }
    }
    else {
        if (victim->bonus) {
            generateBonus(0);
            victim->bonus = 0;
        }
        if ((victim != &game_player[0] && victim != &game_player[1]) && (killer != &game_player[0] && killer != &game_player[1]) && (config.en_friend_fire != KILL)) {

            switch (config.en_friend_fire) {
                case BULLETS_STOP:
//                    startPlaySample(snd_bull_stop, sizeof (snd_bull_stop), 11000, AUDIO_PAN_CENTER, 6);
                    soundPlay(snd_bull_stop, sizeof (snd_bull_stop), SOUND_PCM_CH3, 0);
                    break;
            }
        }
        else{

            if(victim->hitpoint) soundPlay(snd_bull_stop, sizeof (snd_bull_stop), SOUND_PCM_CH3, 0);

            if ((!mods.pl_asskiller && !mods.en_invul) || (killer != &game_player[0] && killer != &game_player[1]) ) {
                if (victim->ship == 1) {victim->ship = 0;}
                else if (victim->hp == 1)
                {
                    victim->type = 0;
                    victim->hitpoint = 1;
                    victim->speed = TANK_SPEED_1;
                    victim->bullet_limit = 1;
                    victim->bullet_speed = BULLET_SPEED_1;
                    victim->uranium_bullets = 0;
                    victim->hp=0;
                }
                else if (victim->hp != 1 && victim->ship != 1)
                    victim->hitpoint--;

            }
            else if (killer == &game_player[0] || killer == &game_player[1]) {
                if (victim->ship == 1) {
                    victim->ship = 0;
                }
                else {
                    if (killer->rotate == victim->rotate) {
                        if (mods.en_invul) {
                            victim->freeze = 256;
                        }
                        else {
                            victim->hitpoint--;
                        }
                    }
                    else {
                        victim->freeze = 256;
                    }
                }

            }
        }




        //if(victim->hitpoint)startPlaySample(snd_bull_stop, sizeof (snd_bull_stop), 11000, AUDIO_PAN_CENTER, 6);
        if(victim->hitpoint) soundPlay(snd_bull_stop, sizeof (snd_bull_stop), SOUND_PCM_CH3, 0);
    }
    if (!victim->hitpoint) {

        if (killer == &game_player[0]) {
            kills_1[victim->type - 4]++;
            showScoreQuad(victim->type - 4, victim->posx, victim->posy);
        }
        if (killer == &game_player[1]) {
            kills_2[victim->type - 4]++;
            showScoreQuad(victim->type - 4, victim->posx, victim->posy);
        }

        killer->scor += (victim->type - 3) * 100;
        GLog_makeExplode(EXPLODE_BIG, victim->posx, victim->posy);

        if(victim == &game_player[0] || victim == &game_player[1]){
//            startPlaySample(snd_explode, sizeof (snd_explode), 11000, AUDIO_PAN_CENTER, 7);
            soundPlay(snd_pl_explode, sizeof (snd_pl_explode), SOUND_PCM_CH3, 0);
        }else{
            soundPlay(snd_en_explode, sizeof (snd_en_explode), SOUND_PCM_CH3, 0);
        }

        victim->lives--;
    }
}

void GLog_shot(_tank *tank) {

    u16 i;
    _bullet *bull_buff;

    if (tank->bullets_count >= tank->bullet_limit || !tank->hitpoint || tank->birth) return;


    for (i = 0; i < config.max_bullets; i++) {

        bull_buff = &bullets[i];
        if (bull_buff->speed) continue;
        bull_buff->speed = tank->bullet_speed;
        bull_buff->rotate = tank->rotate;
        bull_buff->posx = tank->posx;
        bull_buff->posy = tank->posy;
        bull_buff->ricocheted = FALSE;
        if ((bull_buff->posx & 3) == 0 && (tank->rotate & 1))bull_buff->posx += 1;
        if ((bull_buff->posy & 3) == 0 && !(tank->rotate & 1))bull_buff->posy += 1;
        bull_buff->maker = tank;
        tank->bullets_count++;
        if (tank == &game_player[0] || tank == &game_player[1]) soundPlay(snd_shot, sizeof(snd_shot), SOUND_PCM_CH2, 0);
        return;
    }
}

void GLog_updateControl(u8 gamer_idx, u16 joy, u16 speed) {


    u16 temp;
    if (game_player[gamer_idx].birth || !game_player[gamer_idx].hitpoint)return;


    if (config.turbo_b) {
        if (game_player[gamer_idx].bullet_delay < BULLET_DELAY) game_player[gamer_idx].bullet_delay += 1;

        if ((joy & BUTTON_B) && game_player[gamer_idx].fire_timer == 0 ) {
            game_player[gamer_idx].fire = 1;
            game_player[gamer_idx].fire_timer = 1;
        } else if ((joy & BUTTON_B) && game_player[gamer_idx].fire_timer == 1 && game_player[gamer_idx].bullet_delay >= BULLET_DELAY) {
    //        game_player[0].fire_timer = 0;
    //        game_player[0].bullet_delay = 0;
        }

        if (game_player[gamer_idx].bullet_delay >= BULLET_DELAY) {
            game_player[gamer_idx].fire_timer = 0;
            game_player[gamer_idx].bullet_delay = 0;
        }
    }


    if (game_player[gamer_idx].freeze) {
        game_player[gamer_idx].freeze--;
        return;
    }

    game_player[gamer_idx].speed = 0;

    if (joy & BUTTON_UP) {

        game_player[gamer_idx].speed = speed;
        game_player[gamer_idx].rotate = 0;
        temp = game_player[gamer_idx].posx;
        if ((temp & 7) > 4)game_player[gamer_idx].posx = (temp >> 3 << 3) + 8;
        else game_player[gamer_idx].posx -= (temp & 7);
        if (game_player[gamer_idx].posx >> 3 > MAP_W - 2)game_player[gamer_idx].posx = (MAP_W - 2) << 3;

    } else
        if (joy & BUTTON_LEFT) {

        game_player[gamer_idx].speed = speed;
        game_player[gamer_idx].rotate = 1;
        temp = game_player[gamer_idx].posy;
        if ((temp & 7) > 4)game_player[gamer_idx].posy = (temp >> 3 << 3) + 8;
        else game_player[gamer_idx].posy -= (temp & 7);
        if (game_player[gamer_idx].posy >> 3 > MAP_H - 2)game_player[gamer_idx].posy = (MAP_H - 2) << 3;
    } else
        if (joy & BUTTON_DOWN) {

        game_player[gamer_idx].speed = speed;
        game_player[gamer_idx].rotate = 2;
        temp = game_player[gamer_idx].posx;
        if ((temp & 7) > 4)game_player[gamer_idx].posx = (temp >> 3 << 3) + 8;
        else game_player[gamer_idx].posx -= (temp & 7);
        if (game_player[gamer_idx].posx >> 3 > MAP_W - 2)game_player[gamer_idx].posx = (MAP_W - 2) << 3;
    } else
        if (joy & BUTTON_RIGHT) {
        game_player[gamer_idx].speed = speed;
        game_player[gamer_idx].rotate = 3;
        temp = game_player[gamer_idx].posy;
        if ((temp & 7) > 4)game_player[gamer_idx].posy = (temp >> 3 << 3) + 8;
        else game_player[gamer_idx].posy -= (temp & 7);
        if (game_player[gamer_idx].posy >> 3 > MAP_H - 2)game_player[gamer_idx].posy = (MAP_H - 2) << 3;
    }

}

void GLog_updateGame() {

    if (pause)return;
    u16 i;
    speed_counter++;
    speed_counter |= 128;
    u16 joy1 = JOY_readJoypad(JOY_1);
    u16 joy2 = JOY_readJoypad(JOY_2);


    if (game_player[0].hitpoint == 0) {
        GLog_initGamer(0);
    }
    if (game_player[1].hitpoint == 0) {
        GLog_initGamer(1);
    }

    if (game_player[0].hitpoint == 0 && game_player[1].hitpoint == 0) {
        gameover = 1;
    }

    for (i = 0; i < config.units_on_map; i++) {
        if (game_player[i].fire) {
            game_player[i].fire = 0;
            GLog_shot(&game_player[i]);
        }
    }


    if (gameover) {
        GLog_updateControl(0, 0, 0);
        GLog_updateControl(1, 0, 0);
    } else {
        if (mods.pl_speed_dec) {
            GLog_updateControl(0, joy1, TANK_SPEED_1);
            GLog_updateControl(1, joy2, TANK_SPEED_1);
        }
        else {
            GLog_updateControl(0, joy1, TANK_SPEED_2);
            GLog_updateControl(1, joy2, TANK_SPEED_2);
        }
    }

    GLog_updateAI();
    GLog_updatebullets();
    GLog_updateMove();
    GLog_updateBonus();

    updateGameMenu();
    updateArmorStaff();

}

void GLog_initGamer(u8 player_idx) {

    if (!game_player[player_idx].lives) return;

    if (game_player[player_idx].hitpoint == 0) {
        game_player[player_idx].bullet_limit = 1;
        game_player[player_idx].bullet_speed = BULLET_SPEED_1;
        game_player[player_idx].uranium_bullets = 0;
        game_player[player_idx].type = 0;
        if (mods.pl_en_tank) game_player[player_idx].type = 4;
        game_player[player_idx].ship = 0;
        game_player[player_idx].grass_trim = 0;
    }
    freeze_players = 0;
    game_player[player_idx].hitpoint = 1;
    game_player[player_idx].god = 256;
    if (mods.pl_shieldless) game_player[player_idx].god = 0;
    game_player[player_idx].rotate = 0;

    game_player[player_idx].speed = 0;
    game_player[player_idx].birth = BIRTH_TIME;
    game_player[player_idx].bullets_count = 0;
    game_player[player_idx].freeze = 0;
    game_player[player_idx].hp = 0;
    game_player[player_idx].bonus = 0;

    if (player_idx == 0) {
        game_player[player_idx].color = TANK_COLOR_YELLOW;
        game_player[player_idx].posx = START_X_PL_A << 3;
        game_player[player_idx].posy = START_Y_PL_A << 3;
        game_player[player_idx].player = 1;
    } else {
        game_player[player_idx].color = TANK_COLOR_GREEN;
        game_player[player_idx].posx = START_X_PL_B << 3;
        game_player[player_idx].posy = START_Y_PL_B << 3;
        game_player[player_idx].player = 2;
    }
    victory_timer = 0;

    if ((config.debug & BONUS_GOD) == BONUS_GOD) {
//        game_player[player_idx].bonus = BONUS_GOD;
//        game_player[0].god = 1024*100;
    }
}

void GLog_makeExplode(u16 type, s16 x, s16 y) {

    u16 i = config.max_explode;

    while (i--) {
        if (explodes[i].type)continue;
        explodes[i].posx = x;
        explodes[i].posy = y;
        explodes[i].strobe = 0;
        explodes[i].type = type;
        explodes[i].ani_counter = 0;

        return;
    }
}


void GLog_initGameLogic() {

    u16 i;
    pause = 0;
    speed_counter = 128;
    scor.timer = 0;
    gameover = 0;
    bullet_speed_strobe = 0;

    enemy_types[0].type = 4;
    enemy_types[0].hitpoint = 1;
    enemy_types[0].speed = TANK_SPEED_1;
    enemy_types[0].bullet_limit = 1;
    enemy_types[0].bullet_speed = BULLET_SPEED_1;

    enemy_types[1].type = 5;
    enemy_types[1].hitpoint = 1;
    enemy_types[1].speed = TANK_SPEED_3;
    enemy_types[1].bullet_limit = 1;
    enemy_types[1].bullet_speed = BULLET_SPEED_1;

    enemy_types[2].type = 6;
    enemy_types[2].hitpoint = 1;
    enemy_types[2].speed = TANK_SPEED_1;
    enemy_types[2].bullet_limit = 2;
    enemy_types[2].bullet_speed = BULLET_SPEED_2;

    enemy_types[3].type = 7;
    enemy_types[3].hitpoint = 4;
    enemy_types[3].speed = TANK_SPEED_1;
    enemy_types[3].bullet_limit = 1;
    enemy_types[3].bullet_speed = BULLET_SPEED_1;


    for (i = 0; i < config.units_on_map; i++) {
        game_player[i].scor = 0;
        game_player[i].lives = 0;
        game_player[i].hitpoint = 0;
        game_player[i].uranium_bullets = 0;
    }
    for (i = 0; i < config.max_explode; i++) {
        explodes[i].type = 0;
    }

    game_player[0].lives = config.start_lives;
    if (menuGetSelectedItem() == ITEM_TWO_PLAYER) {
        game_player[1].lives = config.start_lives;
    }
}

void GLog_initEnemy() {

    u16 i;
    _tank *buff;
    u8 tank_type = 2;

    if (selected_stage == 0) {
        tank_type = 0;
    } else
        if (selected_stage == 1) {
        tank_type = random() % 3;
    } else
        if (selected_stage == 2) {
        tank_type = random() % 4;
    } else
        if (selected_stage < 10) {
        tank_type = random() % 4;
        if (tank_type == 0)tank_type = random() % 4;
    } else
        if (selected_stage < 20) {
        tank_type = random() % 4;
        if (tank_type == 0 || tank_type == 1)tank_type = random() % 4;
        if (tank_type == 0 || tank_type == 1)tank_type = random() % 4;
    } else {
        tank_type = 1 + random() % 3;
        if (tank_type != 2)tank_type = 1 + random() % 3;
    }


    //tank_type = enemy_num & 3;
    for (i = 2; i < config.units_on_map; i++) {
        buff = &game_player[i];
        if (buff->hitpoint) {
            buff = 0;
            continue;
        }
        break;
    }
    if (buff == 0) return;

    enemy_num--;
    buff->bullet_limit = enemy_types[tank_type].bullet_limit;
    buff->bullet_speed = enemy_types[tank_type].bullet_speed;
    buff->hitpoint = enemy_types[tank_type].hitpoint;

    buff->speed = enemy_types[tank_type].speed;

    buff->type = enemy_types[tank_type].type;
    buff->tank_type = tank_type;

    if (mods.en_pl_skin) buff->type = 0;

    buff->posy = 0;
    buff->speed = enemy_types[tank_type].speed;

    buff->rotate = 2;
    buff->posx = enemy_pos_x[enemy_num % 3];
    buff->bullets_count = 0;
    buff->birth = BIRTH_TIME;
    buff->hitpoint = enemy_types[tank_type].hitpoint;
    buff->color = TANK_COLOR_GREY;
    buff->bonus = (enemy_num & 3) ? 0 : 1;

//    buff->bonus = 1;
    if (mods.no_bonuses) buff->bonus = 0;

    birth_timer = current_birth_time;

	buff->uranium_bullets = 0;
	buff->grass_trim = 0;
    buff->god = 0;
    buff->player = 0;


    if (mods.en_speed) buff->speed = TANK_SPEED_3;
    if (mods.en_armor) buff->hitpoint = 4;
    if (mods.en_uranium) buff->uranium_bullets = 1;
    if (mods.en_bull_speed) buff->bullet_speed = BULLET_SPEED_2;
    if (mods.en_shielded) buff->god = 256;


}

void GLog_initLEvel(u16 level) {

    u16 i = 0;
    current_birth_time = 256 * (MAP_AVAILABLE - level % MAP_AVAILABLE) / MAP_AVAILABLE;
    if (current_birth_time < BIRTH_TIME + 10)current_birth_time = BIRTH_TIME + 10;
    if (mods.en_spawn_speed) {
        current_birth_time = BIRTH_TIME + 10;
    }
    birth_timer = 0;
    victory = 0;
    enemy_num = START_ENEMYS;
    if (mods.en_doubles) enemy_num = START_ENEMYS*2;
    for (i = 0; i < config.units_on_map; i++) {
        game_player[i].bullets_count = 0;
        game_player[i].freeze = 0;
        game_player[i].fire = 0;
    }
    GLog_initGamer(0);
    GLog_initGamer(1);


//    VDP_fillTileMapRect(PLAN_B, RES_TILE_GREY, 0, 0, 32, 28);

    if (map_editor_map_ready) {
		setMap(PLAN_B, editor_map, 1);
        map_editor_map_ready = 0;
	} else {
//		setMap(PLAN_B, MAP_GAME_MAP * MAP_LEN + (maps_data + level % MAP_AVAILABLE * MAP_LEN), 1);
        setMapLevel(selected_stage);
    }


    old_enemy_num = -1;
    old_lives1 = -1;
    old_lives2 = -1;
    armor_staff = 0;
    drawStage();
    bonus.type = 0;
    freeze = 0;
    VDP_setPalette(0, pal_red);
    VDP_setPalette(1, pal_yellow);
    VDP_setPalette(2, pal_green);
    VDP_setPalette(3, pal_grey);


}

u8 GLog_gameover() {
    return gameover;
}

u8 GLog_victory() {
    return victory;
}

void GLog_setVictory() {
    GLog_removeEnemy();
    victory = 1;
}

void GLog_removeEnemy() {
    u16 i;
    enemy_num = 0;

    for (i = 2; i < config.units_on_map; i++) {
        game_player[i].hitpoint = 0;
        game_player[i].posx = -30;
    }

}

void GLog_killStaff() {

    GLog_makeExplode(EXPLODE_BIG, START_X_ST << 3, START_Y_ST << 3);
    mapSetTile(RES_TILE_DEATH_STAFF + 0, START_X_ST + 0, START_Y_ST + 0);
    mapSetTile(RES_TILE_DEATH_STAFF + 1, START_X_ST + 0, START_Y_ST + 1);
    mapSetTile(RES_TILE_DEATH_STAFF + 2, START_X_ST + 1, START_Y_ST + 0);
    mapSetTile(RES_TILE_DEATH_STAFF + 3, START_X_ST + 1, START_Y_ST + 1);
    gameover = 1;
    //startPlaySample(snd_explode, sizeof (snd_explode), 11000, AUDIO_PAN_CENTER, 8);
    soundPlay(snd_pl_explode, sizeof (snd_pl_explode), SOUND_PCM_CH3, 0);
}

void updateGameMenu() {

    u16 i;
    u16 x = MAP_X + MAP_W + 1;
    u16 y = MAP_Y + 1;
    u16 tile = 0;
    u16 lives;

    if (enemy_num != old_enemy_num) {
        for (i = 0; i < START_ENEMYS; i++) {
            tile = i < enemy_num ? RES_TILE_TANK_ICON_BLACK : RES_TILE_GREY;
			VDP_setTile(PLAN_B, tile, x + (i & 1), y + (i >> 1));

        }
        old_enemy_num = enemy_num;
    }

    y = 15;
    lives = game_player[0].lives;
    if (old_lives1 != lives) {
        if (lives > 9)lives = 9;
        if (old_lives1 == -1) {
			VDP_setTile(PLAN_B, RES_TILE_P1, x, y);
			VDP_setTile(PLAN_B, RES_TILE_P, x + 1, y);
			VDP_setTile(PLAN_B, RES_TILE_TANK_ICON_OGANGE, x, y + 1);
        }
		VDP_setTile(PLAN_B, RES_TILE_STAGE_NUM + lives, x + 1, y + 1);
        old_lives1 = lives;
    }

    y = 18;
    lives = game_player[1].lives;
    if (old_lives2 != lives && menuGetSelectedItem() == ITEM_TWO_PLAYER) {
        if (lives > 9)lives = 9;
        if (old_lives2 == -1) {
			VDP_setTile(PLAN_B, RES_TILE_P1, x, y);
			VDP_setTile(PLAN_B, RES_TILE_P, x + 1, y);
			VDP_setTile(PLAN_B, RES_TILE_TANK_ICON_OGANGE, x, y + 1);
        }
        VDP_setTile(PLAN_B, RES_TILE_STAGE_NUM + lives, x + 1, y + 1);
        old_lives2 = lives;
    }

}

void drawStage() {

    u16 num;
    u16 x = MAP_X + MAP_W + 1;
    u16 y = MAP_Y + 21;
	VDP_setTile(PLAN_B, 108, x, y);
	VDP_setTile(PLAN_B, 109, x, y + 1);
	VDP_setTile(PLAN_B, 44, x + 1, y);
    VDP_setTile(PLAN_B, 45, x + 1, y + 1);

    num = (selected_stage + 1) / 10;
	if (num != 0) VDP_setTile(PLAN_B, RES_TILE_STAGE_NUM + num, x, y + 2);
    num = (selected_stage + 1) % 10;
	VDP_setTile(PLAN_B, RES_TILE_STAGE_NUM + num, x + 1, y + 2);
}

void generateBonus(u8 bns) {

    bonus.posx = random() % (MAP_W - 4) << 3;
    bonus.posy = random() % (MAP_H - 5) << 3;

    soundPlay(snd_bonus_appears, sizeof (snd_bonus_appears), SOUND_PCM_CH1, 0);

    if (bns) {
        bonus.type = bns;
    }
    else {
        bonus.type = (random() % MAX_BONUS);

        // + 2 bonus
        switch (config.addition_bonus) {
            case OFF:
                bonus.type %= 6;
                break;
            case GUN_BONUS:
                bonus.type %= 7;
                break;
            case SHIP_BONUS:
                bonus.type %= 7;
                break;
            case BOTH_BONUS:
                bonus.type %= 8;
                break;
        }

        bonus.type++;

        /*bonus.posx = 2;
        bonus.posy = 1;
        bonus.type = 8;*/

        switch (bonus.type) {
            case 1:
                bonus.type = 4;
                break;
            case 2:
                bonus.type = 5;
                break;
            case 5:
                bonus.type = 2;
                break;
            case 4:
                bonus.type = 1;
                break;
            case 7:
                if (config.addition_bonus == BONUS_SHIP) bonus.type = 8;
                break;
        }

    }
}

void setBonus(u8 player) {

    u16 i;
    switch (bonus.type) {
        case BONUS_GOD://бессмертие type = 1;
            game_player[player].god = 1024;
            break;
        case BONUS_TIME://время type = 2;
			if (player == 0 || player == 1) freeze = 1024;
			else freeze_players = 1024;
            break;
        case BONUS_ARM_STAFF://лопата type = 3;
            if (player == 0 || player == 1) armor_staff = ARMOR_STAFF_TIME;
			else armor_staff = ARMOR_STAFF_TIME+1;
            break;

        case BONUS_STAR://звезда type = 4;

		if ((player < 2 && !mods.pl_en_tank) || (player >= 2 && mods.en_pl_skin)) {
            game_player[player].type++;
            if (game_player[player].type > 3) game_player[player].type = 3;
            if (game_player[player].type == 1) {
                game_player[player].bullet_speed = BULLET_SPEED_2;
            }
            if (game_player[player].type == 2) {
                game_player[player].bullet_limit = 2;
            }
            if (game_player[player].type == 3) {
                game_player[player].uranium_bullets = 1;
		}}
		else if ((player >= 2 && !mods.en_pl_skin) || (player < 2 && mods.pl_en_tank))
        {
				if(game_player[player].type == 4){
					game_player[player].type = 5;
					game_player[player].hitpoint = 1;
					game_player[player].speed = TANK_SPEED_3;
					game_player[player].bullet_limit = 1;
					game_player[player].bullet_speed = BULLET_SPEED_1;
				}
				else if(game_player[player].type == 5){
					game_player[player].type = 6;
					game_player[player].hitpoint = 1;
					game_player[player].speed = TANK_SPEED_1;
					game_player[player].bullet_limit = 2;
					game_player[player].bullet_speed = BULLET_SPEED_2;
				}
				else if(game_player[player].type == 6){
                    game_player[player].type = 7;
                    game_player[player].hitpoint = 4;
                    game_player[player].speed = TANK_SPEED_1;
                    game_player[player].bullet_limit = 1;
                    game_player[player].bullet_speed = BULLET_SPEED_1;
				}
				else if(game_player[player].type == 7){
                    game_player[player].hitpoint += 2;
                    if(game_player[player].speed == TANK_SPEED_1)game_player[player].speed = TANK_SPEED_2;
                    else if(game_player[player].speed == TANK_SPEED_2)game_player[player].speed = TANK_SPEED_3;
                    if(game_player[player].bullet_limit == 1)game_player[player].bullet_limit = 2;
                    if(game_player[player].bullet_speed == BULLET_SPEED_1)game_player[player].bullet_speed = BULLET_SPEED_2;
				}
		}
            break;

        case BONUS_BOMB://бомба type = 5;
            if(player==0||player==1)
			for (i = 2; i < config.units_on_map; i++) {
                if (game_player[i].hitpoint) {
                    GLog_makeExplode(EXPLODE_BIG, game_player[i].posx, game_player[i].posy);
                    game_player[i].hitpoint = 0;
//                    startPlaySample(snd_enemy_explode, sizeof (snd_enemy_explode), 11000, AUDIO_PAN_CENTER, 10);
                    soundPlay(snd_en_explode, sizeof (snd_en_explode), SOUND_PCM_CH3, 0);
                }
            }
			else {
				for (i = 0; i < 2; i++) {
					if (game_player[0].hitpoint)
                    if (game_player[i].hitpoint) {
                        GLog_makeExplode(EXPLODE_BIG, game_player[i].posx, game_player[i].posy);
                        game_player[i].hitpoint = 0;
                        game_player[player].lives--;
    //					startPlaySample(snd_explode, sizeof (snd_explode), 11000, AUDIO_PAN_CENTER, 10);
                        soundPlay(snd_pl_explode, sizeof (snd_pl_explode), SOUND_PCM_CH3, 0);
                    }
                }
			}
            break;

        case BONUS_LIFE://жизнь type = 6;
            if (player==0||player==1) {
                game_player[player].lives++;
                soundPlay(snd_live_got, sizeof (snd_live_got), SOUND_PCM_CH1, 0);
            }
			else {
                enemy_num+=3;
                soundPlay(snd_bonus_got, sizeof (snd_live_got), SOUND_PCM_CH1, 0);
//                stop_sound_engine = TRUE;
			}
            break;

		case BONUS_GUN://пистолет type = 7;
			if(player==0||player==1){
				game_player[player].type = 3;
				game_player[player].bullet_speed = BULLET_SPEED_2;
				game_player[player].bullet_limit = 2;
                game_player[player].uranium_bullets = 1;
                game_player[player].hp = 1;
                game_player[player].grass_trim = 1;
			}
			else {

					game_player[player].type = 7;
					game_player[player].hitpoint = 4;
					game_player[player].speed = TANK_SPEED_2;
					game_player[player].bullet_limit = 2;
					game_player[player].bullet_speed = BULLET_SPEED_2;
					game_player[player].uranium_bullets = 1;
					game_player[player].grass_trim = 1;

			}
            break;

		case BONUS_SHIP://корабль type = 8;
			game_player[player].ship = 1;

			break;


    }
    //startPlaySample(snd_bonus_take, sizeof (snd_bonus_take), 10000, AUDIO_PAN_CENTER, 9);
    if (bonus.type != BONUS_LIFE) {
        soundPlay(snd_bonus_got, sizeof (snd_bonus_got), SOUND_PCM_CH1, 0);
//        stop_sound_engine = TRUE;
    }
    showScoreQuad(4, bonus.posx, bonus.posy);
    bonus.type = 0;
}

void updateArmorStaff() {

    if (!armor_staff)return;

	if (armor_staff == ARMOR_STAFF_TIME+1) {
        armor_staff = 0;
		mapSetTile(0, 11, 23);
        mapSetTile(0, 11, 24);
        mapSetTile(0, 11, 25);
        mapSetTile(0, 12, 23);
        mapSetTile(0, 13, 23);
        mapSetTile(0, 14, 23);
        mapSetTile(0, 14, 24);
        mapSetTile(0, 14, 25);
		return;
		}

    armor_staff--;
    if (armor_staff == ARMOR_STAFF_TIME - 1 || armor_staff < 256) {
        u8 armor_tile = RES_TILE_ARMOR;
        if ((armor_staff & 31) > 16 && armor_staff < ARMOR_STAFF_TIME - 20)armor_tile = RES_TILE_BRICK;
        if (!armor_staff)armor_tile = RES_TILE_BRICK;

        mapSetTile(armor_tile, 11, 23);
        mapSetTile(armor_tile, 11, 24);
        mapSetTile(armor_tile, 11, 25);
        mapSetTile(armor_tile, 12, 23);
        mapSetTile(armor_tile, 13, 23);
        mapSetTile(armor_tile, 14, 23);
        mapSetTile(armor_tile, 14, 24);
        mapSetTile(armor_tile, 14, 25);
    }

}

void showScoreQuad(u16 val, u16 posx, u16 posy) {

    scor.timer = 40;
    scor.posx = posx;
    scor.posy = posy;
    scor.val = val;
}

void soundPlay(const u8 *sample, const u32 len, const u16 channel, const u8 loop) {
    if (getTimer(0, FALSE) > 330000) {
        SND_startPlay_4PCM(sample, len, channel, loop);
    }

    if ((sample == snd_bonus_appears) || (sample == snd_bonus_got) || (sample == snd_live_got)) {
        soundStopEngine();
    }
}

