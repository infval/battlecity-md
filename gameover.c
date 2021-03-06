

#include <genesis.h>
#include <sram.h>
#include "gameover.h"
#include "map.h"
#include "resmap.h"
#include "menu.h"
#include "sprite.h"
#include "gamerender.h"
#include "game.h"
#include "defs.h"
#include "resources.h"
#include "gamelogic.h"

void showGameoverWord();
void setOrangeColor();
void drawTankIcons();

u8 num_buff[6];
u8 tnk;
u8 score1[5];
u8 score2[5];

void startGameOver() {

    showScore();
    if (top_scor < game_player[0].scor)
        top_scor = game_player[0].scor;
    if (top_scor < game_player[1].scor)
        top_scor = game_player[1].scor;
    SRAM_writeLong(0, top_scor);
    SRAM_writeLong(sizeof(u32), ~top_scor);
    showGameoverWord();
}

void showScore() {

    VDP_resetSprites();
    VDP_updateSprites(1, FALSE);

    u8 i;
    if (menuGetSelectedItem() == ITEM_ONE_PLAYER) {
        for (i = 0; i < sizeof(kills_2) / sizeof(kills_2[0]); i++) {
            kills_2[i] = 0;
        }
    }

    for (i = 0; i < sizeof(score1) / sizeof(score1[0]); i++) {
        score1[i] = 0;
        score2[i] = 0;
    }

    u16 joy;
    u8 play_sound;

    VDP_setPalette(0, palette_black);
    VDP_fillTileMapRect(PLAN_A, 0, 0, 0, planWidth, planHeight);
    VDP_fillTileMapRect(PLAN_B, 0, 0, 0, planWidth, planHeight);

    if (menuGetSelectedItem() == ITEM_ONE_PLAYER) {
        setMap(PLAN_B, &maps_data[MAP_SCORE1 * MAP_LEN], MAP_GAMEMODE_FALSE);
    }
    else {
        setMap(PLAN_B, &maps_data[MAP_SCORE2 * MAP_LEN], MAP_GAMEMODE_FALSE);
    }

    setOrangeColor();
    drawTankIcons();
    VDP_setPalette(0, pal_red);
    VDP_setPalette(1, pal_red_text);
    VDP_setPalette(2, pal_orange_text);

    score1[4] = kills_1[0] + kills_1[1] + kills_1[2] + kills_1[3];
    score2[4] = kills_2[0] + kills_2[1] + kills_2[2] + kills_2[3];
    game_player[0].scor += kills_1[0] * 100 + kills_1[1] * 200 + kills_1[2] * 300 + kills_1[3] * 400;
    game_player[1].scor += kills_2[0] * 100 + kills_2[1] * 200 + kills_2[2] * 300 + kills_2[3] * 400;
    // Bonus
    game_player[0].scor += kills_1[4] * 500;
    game_player[1].scor += kills_2[4] * 500;

    for (;;) {
        drawTankIcons();
        drawNum(top_scor, 160 / 8, 1);
        drawNum(selected_stage + 1, 128 / 8, 24 / 8);

        drawNum(game_player[0].scor, 56 / 8, 56 / 8);
        if (menuGetSelectedItem() == ITEM_TWO_PLAYER) {
            drawNum(game_player[1].scor, 200 / 8, 56 / 8);
        }

        switch (tnk) {
            case 0:
                drawNum(score1[0], 80 / 8, 80 / 8);
                drawNum(score1[0] * 100, 24 / 8, 80 / 8);
                if (menuGetSelectedItem() == ITEM_TWO_PLAYER) {
                    drawNum(score2[0] * 100, 168 / 8, 80 / 8);
                    drawNum(score2[0], 128 / 8, 80 / 8);
                }
                break;
            case 1:
                drawNum(score1[1], 80 / 8, 104 / 8);
                drawNum(score1[1] * 200, 24 / 8, 104 / 8);
                if (menuGetSelectedItem() == ITEM_TWO_PLAYER) {
                    drawNum(score2[1] * 200, 168 / 8, 104 / 8);
                    drawNum(score2[1], 128 / 8, 104 / 8);
                }
                break;
            case 2:
                drawNum(score1[2], 80 / 8, 128 / 8);
                drawNum(score1[2] * 300, 24 / 8, 128 / 8);
                if (menuGetSelectedItem() == ITEM_TWO_PLAYER) {
                    drawNum(score2[2] * 300, 168 / 8, 128 / 8);
                    drawNum(score2[2], 128 / 8, 128 / 8);
                }
                break;
            case 3:
                drawNum(score1[3], 80 / 8, 152 / 8);
                drawNum(score1[3] * 400, 24 / 8, 152 / 8);
                if (menuGetSelectedItem() == ITEM_TWO_PLAYER) {
                    drawNum(score2[3] * 400, 168 / 8, 152 / 8);
                    drawNum(score2[3], 128 / 8, 152 / 8);
                }
                break;
            case 4:
                drawNum(score1[4], 80 / 8, 168 / 8);
                if (menuGetSelectedItem() == ITEM_TWO_PLAYER) {
                    drawNum(score2[4], 128 / 8, 168 / 8);
                }
                break;
        }

        if (tnk <= 3) {
            if  ((score1[tnk] == kills_1[tnk]) && (score2[tnk] == kills_2[tnk])) {
                tnk += 1;
                sleepFrames(18);
            }
            else {
                play_sound = FALSE;
                if (score1[tnk] < kills_1[tnk]) {
                    score1[tnk] += 1;
                    play_sound = TRUE;
                }
                if (score2[tnk] < kills_2[tnk]) {
                    score2[tnk] += 1;
                    play_sound = TRUE;
                }

                if (play_sound) {
                    SND_stopPlay_4PCM(SOUND_PCM_CH2);
                    soundPlay(snd_score_tic, sizeof(snd_score_tic), SOUND_PCM_CH2, FALSE);
                }

            }

            sleepFrames(8);
        }
        else {
            if (tnk > 4) {
                sleepFrames(40);
                tnk = 0;
                break;
            }
            tnk += 1;
            sleepFrames(6);
        }

        joy = JOY_readJoypad(JOY_1);
        joy |= JOY_readJoypad(JOY_2);

        VDP_waitVSync();
    }
}

void showGameoverWord() {

    VDP_setPalette(0, palette_black);
    VDP_fillTileMapRect(PLAN_B, 0, 0, 0, planWidth, planHeight);
    setMap(PLAN_B, &maps_data[MAP_GAMEOVER * MAP_LEN], MAP_GAMEMODE_FALSE);
    VDP_resetSprites();
    VDP_updateSprites(1, FALSE);
    VDP_setPalette(0, pal_red);
    //startPlaySample(snd_gameover, sizeof(snd_gameover), 10000, AUDIO_PAN_CENTER, 9);
    SND_startPlay_PCM(snd_gameover, sizeof(snd_gameover), SOUND_RATE_22050, SOUND_PAN_CENTER, 0);
    sleepFrames(128);
}

void setOrangeColor() {

    u16 i;
    u16 tile;
    for (i = 0; i < MAP_W; i++) {

        tile = mapGetTile(i, 1);
        if (i < MAP_W >> 1) {
            tile |= TILE_ATTR(PAL1, 0, 0, 0);
        }
        else {
            tile |= TILE_ATTR(PAL2, 0, 0, 0);
        }
        mapSetTile(tile, i, 1);
        tile = mapGetTile(i, 5);
        tile |= TILE_ATTR(PAL1, 0, 0, 0);
        mapSetTile(tile, i, 5);

        tile = mapGetTile(i, 7);
        tile |= TILE_ATTR(PAL2, 0, 0, 0);
        mapSetTile(tile, i, 7);
    }
}

void drawTankIcons() {

    _tank t;
    memset(&t, 0, sizeof(_tank));
    t.type = 4;
    if (mods.en_pl_skin)
        t.type = 0;
    t.posx = 97;
    t.posy = 77;
    t.color = TANK_COLOR_GREY;
    t.hitpoint = 1;

    u16 i;
    updateSprite();
    for (i = 0; i < 4; i++) {
        drawTank(&t);
        t.type = t.type + 1;
        t.posy += 24;
    }
    GRend_repaint();
}

void drawNum(u32 num, u8 x, u8 y) {

    u8 len = 0;
    u16 i;
    u16 tile;

    num_buff[0] = num % 10;
    num_buff[1] = num % 100 / 10;
    num_buff[2] = num % 1000 / 100;
    num_buff[3] = num % 10000 / 1000;
    num_buff[4] = num % 100000 / 10000;
    num_buff[5] = num % 1000000 / 100000;

    for (i = 0; i < 6; i++) {
        if (num_buff[i] != 0)
            len = i + 1;
    }
    if (len < 1)
        len = 1;

    for (i = 0; i < len; i++) {
        tile = mapGetTile(x, y);
        tile &= 0xff00;
        tile |= '0' + num_buff[i];
        mapSetTile(tile, x, y);
        x -= 1;
    }
}
