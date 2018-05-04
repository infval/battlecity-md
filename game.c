

#include "genesis.h"
#include "game.h"
#include "map.h"
#include "resmap.h"
#include "map_editor.h"
#include "gameover.h"
#include "sprite.h"
#include "menu.h"
#include "collision.h"
#include "gamelogic.h"
#include "gamerender.h"
#include "defs.h"
//#include "audio.h"
#include "resources.h"
#include "option.h"
#include "mutator.h"



#define VDP_setTile(plan, tile, x, y) (VDP_setTileMapXY(plan, tile, x, y))

void selectLevel();
s16 selected_stage;
s16 game_over_timer = 0;
u8 playing_engine;
u8 start_snd_engine_once;
u8 stop_sound_engine;

extern u8 show_message;

void drawStageNum(s16 num);
void startLevel();
void showGameover();
void drawGameover();
void joyEventGame(u16 joy, u16 changed, u16 state);
void updateAudio();
void soundPlayEngine();

void sleepFrames(int i){
	int a;
	for ( a = 0; a < i; a++) {
      VDP_waitVSync ();
	}
}


u8 kills_1[4];
u8 kills_2[4];
u32 top_scor;
u16 level_timer;

// main.c
extern void battlecity_init();
extern void tank1990_init();
extern void mods_mode_init();

void setGameMode() {
    switch (config.game_mode) {
        case MODE_BC:
            battlecity_init();
            break;
        case MODE_TANK:
            tank1990_init();
            break;
        case MODE_MODS:
            mods_mode_init();
            break;
        case MODE_CUSTOM:
            mods_mode_init();
            customGameOption();

            break;
    }
}

void startGame() {

    setGameMode();

    VDP_setPalette(0, palette_black);
    VDP_resetSprites();
	VDP_updateSprites(1, 1);
	VDP_fillTileMapRect (PLAN_B, 0, 0, 0, planWidth, planHeight);

    VDP_setPalette(0, pal_red);
    VDP_setPalette(1, pal_yellow);
    VDP_setPalette(2, pal_green);
    VDP_setPalette(3, pal_grey);
    selected_stage = -1;
    gameover = 0;

    JOY_setEventHandler(joyEventGame);

    while (!GLog_gameover()) {

        level_timer = 0;
        selectLevel();
        startLevel();

        if (!GLog_gameover()) {

            showScore();
            selected_stage++;
            switch (config.levels_pack) {
                case LEVELPACK_BC_TANK:
                    if (selected_stage == 35) setMapsData(maps_data2);
                    break;
                case LEVELPACK_TANK_BC:
                    if (selected_stage == 35) setMapsData(maps_data);
                    break;
            }
        }
    }


}

void selectLevel() {

    u16 joy = 0;
    s16 stage_ctr = 0;
    u8 init_logic = 0;

    start_snd_engine_once = 0;

    updateSprite();

    if (selected_stage >= 0) stage_ctr = selected_stage;

    closeScreenEffect();

	VDP_fillTileMapRectInc (PLAN_B, RES_TILE_STAGE, 12, 13, 5, 1);

	if ((config.game_mode == MODE_CUSTOM || config.game_mode == MODE_MODS) && config.random_mods) initMods();
	if (config.mods_inc_lvl != 0) {
        if ((stage_ctr + 1) % config.mods_inc_lvl == 0) {
            if (config.mods_count < 6) config.mods_count++;
        }
	}

    drawStageNum(stage_ctr + 1);
    updateSprite();
    joy = JOY_readJoypad(JOY_1);

    while (joy) joy = JOY_readJoypad(JOY_1);

    while (selected_stage < 0) {

        init_logic = 1;
        joy = JOY_readJoypad(JOY_1);

        if (joy & BUTTON_B) {
            stage_ctr++;
            if (stage_ctr >= MAP_AVAILABLE) stage_ctr = MAP_AVAILABLE - 1;
            sleepFrames(10);
            drawStageNum(stage_ctr + 1);
        }
        if (joy & BUTTON_C) {
            stage_ctr--;
            if (stage_ctr < 0) stage_ctr = 0;
            sleepFrames(10);
            drawStageNum(stage_ctr + 1);
        }

        if (joy & BUTTON_START) {
            selected_stage = stage_ctr;
        }

    }

    while (config.game_mode == MODE_MODS || config.game_mode == MODE_CUSTOM) {

        joy = JOY_readJoypad(JOY_1);

        if (joy & BUTTON_START) {
            break;
        }

    }

    if (init_logic) GLog_initGameLogic();


    SND_startPlay_PCM(snd_begin, sizeof(snd_begin), SOUND_RATE_22050, SOUND_PAN_CENTER, 0);
    startTimer(0);

	sleepFrames(64);
    startTimer(1);

}

void drawStageNum(s16 num) {

    u16 num_1 = num / 10;
    u16 num_2 = num % 10;

    if (num_1 == 0) {
		VDP_setTile(PLAN_B, RES_TILE_GREY, 18, 13);
    }
    else {
		VDP_setTile(PLAN_B, RES_TILE_STAGE_NUM + num_1, 18, 13);
    }
	VDP_setTile(PLAN_B, RES_TILE_STAGE_NUM + num_2, 19, 13);

    if (config.game_mode == MODE_MODS || config.game_mode == MODE_CUSTOM) showModText();

}

void startLevel() {

	u16 i;

	for (i = 0; i < config.max_bullets; i++)bullets[i].speed = 0;

//	VDP_fillTileMapRect(PLAN_B, 0, 0, 0, planWidth, planHeight);
	VDP_fillTileMapRect(PLAN_A, RES_TILE_GREY, 0, 0, 32, 28);
	VDP_resetSprites();
	VDP_updateSprites(1, 1);


    // fake background for screen opening
    if (map_editor_map_ready) {
        setFakeMap(editor_map, 1);
    } else {
        setFakeMapLevel(selected_stage);
    }

    // screen opening
    openScreenEffect();

    game_over_timer = 0;

    GLog_initLEvel(selected_stage);

    for (i = 0; i < 4; i++) {
        kills_1[i] = 0;
        kills_2[i] = 0;
    }


    VDP_setPalette(0, pal_red);

	JOY_setEventHandler(joyEventGame);

//	showText();
    while (!GLog_victory()) {

        GLog_updateGame();

        if (GLog_gameover()) {
            SND_stopPlay_4PCM (SOUND_PCM_CH1);

            drawGameover();
            game_over_timer++;
            if (game_over_timer >= 256) break;
        }

        GRend_prepareRepaint();
        updateAudio();

        if(level_timer < 65000) level_timer++;

        GRend_repaint();
        VDP_waitVSync();
    }


    JOY_setEventHandler(0);
}

void joyEventGame(u16 joy, u16 changed, u16 state) {


    if (GLog_gameover())return;
    if ((state & BUTTON_START) && (changed & BUTTON_START) && !gameover && level_timer > 180) {

//        pause = pause ? 0 : 1;
        pause = !pause;
        if (pause) {
            soundStopEngine();
            soundPlay(snd_pause, sizeof (snd_pause), SOUND_PCM_CH1, 0);
        }
        if (!pause) {
            soundPlayEngine();
            if (config.cheats_on) {
                if (joy == JOY_1 && (state & (BUTTON_C))) {
                        GLog_setVictory();
                };
                if (joy == JOY_1 && (state & (BUTTON_A))) {
                        bonus.type = 5;
                        setBonus(0);
                }
                if (joy == JOY_1 && (state & (BUTTON_B))) {
                        bonus.type = 7;
                        setBonus(0);
                }
            }
        }
    }


    if (pause) return;

    if (config.turbo_b) {
        if (joy == JOY_1 && (changed & (BUTTON_A | BUTTON_C)) && (state & (BUTTON_A | BUTTON_C)) && game_player[0].fire_timer == 0) {
            game_player[0].fire = 1;
            game_player[0].fire_timer = 1;
        }
        if (joy == JOY_2 && (changed & (BUTTON_A | BUTTON_C)) && (state & (BUTTON_A | BUTTON_C)) && game_player[1].fire_timer == 0) {
            game_player[1].fire = 1;
            game_player[1].fire_timer = 1;
        }
    }
    else {
        if (joy == JOY_1 && (state & (BUTTON_A | BUTTON_B | BUTTON_C)) && game_player[0].fire_timer == 0) {
            game_player[0].fire = 1;
            game_player[0].fire_timer = 1;
        }
        if (joy == JOY_2 && (state & (BUTTON_A | BUTTON_B | BUTTON_C)) && game_player[1].fire_timer == 0) {
            game_player[1].fire = 1;
            game_player[1].fire_timer = 1;
        }

        if (joy == JOY_1 && !(state & (BUTTON_A | BUTTON_B | BUTTON_C))) {
            game_player[0].fire_timer = 0;
        }
        if (joy == JOY_2 && !(state & (BUTTON_A | BUTTON_B | BUTTON_C))) {
            game_player[1].fire_timer = 0;
        }
    }


    if (joy == JOY_1) {
        moveAvailableInWalls(&game_player[0]);
        switch (state & changed) {
            case BUTTON_UP: case BUTTON_DOWN: case BUTTON_LEFT: case BUTTON_RIGHT:
                if (game_player[0].on_dirt && !game_player[0].dirt) {
                    soundPlay(snd_chuh, sizeof (snd_chuh), SOUND_PCM_CH3, 0);
                }
                break;
        }
    }
    if (joy == JOY_2) {
        moveAvailableInWalls(&game_player[1]);
        switch (state & changed) {
            case BUTTON_UP: case BUTTON_DOWN: case BUTTON_LEFT: case BUTTON_RIGHT:
                if (game_player[1].on_dirt && !game_player[1].dirt) {
                    soundPlay(snd_chuh, sizeof (snd_chuh), SOUND_PCM_CH3, 0);
                }
                break;
        }
    }


}


void drawGameover() {

    s16 word_x = MAP_W / 2 * 8 - 16;
    s16 word_y;

    word_y = MAP_H * 8 - game_over_timer;
    if (word_y < MAP_H / 2 * 8 - 14) word_y = MAP_H / 2 * 8 - 14;
    drawSprite4x1(SPRITE_GAMEOVER | TILE_ATTR(0, 1, 0, 0), word_x, word_y);
    drawSprite4x1((SPRITE_GAMEOVER + 4) | TILE_ATTR(0, 1, 0, 0), word_x, word_y + 8);

}


void updateAudio() {
    int speed = 0;

    if (GLog_gameover() || pause) {
        return;
    }
    if (game_player[0].hitpoint || game_player[1].hitpoint != 0) speed = 1;

    if (game_player[0].hitpoint && game_player[0].speed != 0) speed = 2;
    if (game_player[1].hitpoint && game_player[1].speed != 0) speed = 2;

    if (stop_sound_engine) {
        if (!SND_isPlaying_4PCM (SOUND_PCM_CH1_MSK)) {
            stop_sound_engine = FALSE;
        }
    }

    if ((getTimer(0, FALSE) > 330000) && (!start_snd_engine_once)) {
        soundStopEngine();
        start_snd_engine_once = TRUE;
    }

    if (!stop_sound_engine) {
        if (playing_engine != speed) {
            if (speed == 1) soundPlay(snd_engine_stop, sizeof(snd_engine_stop), SOUND_PCM_CH1, 1);
            if (speed == 2) soundPlay(snd_engine_move, sizeof(snd_engine_move), SOUND_PCM_CH1, 1);
            playing_engine = speed;
            stop_sound_engine = FALSE;
        }
    }

    if (GLog_victory() || GLog_gameover()) {
        SND_stopPlay_4PCM (SOUND_PCM_CH1);
    }

}


void soundStopEngine() {
    stop_sound_engine = TRUE;
    playing_engine = 0;
}

void soundPlayEngine() {
    stop_sound_engine = FALSE;
}
