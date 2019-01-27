
#include <genesis.h>
#include "menu.h"
#include "map.h"
#include "resmap.h"
#include "sprite.h"
#include "map_editor.h"
#include "gameover.h"
#include "game.h"
#include "defs.h"

void joyEventOption(u16 joy, u16 changed, u16 state);
void joyEventCustomGameOption(u16 joy, u16 changed, u16 state);

void drawText(u8 seq[], u8 count, u8 y);
void drawState(const char *string, u8 y);

void optionUpdateState(s8 item_to_update) ;
void customGameOptionUpdateState(s8 item_to_update) ;

void clearText(u8 width, u8 y);

u8 shiftState(u8 val, u8 max_state);
u8 shiftStateEx(u8 val, u8 min_state, u8 max_state);

s8 opt_selected_item;
s8 selector;
s8 item_state;
s8 item_state_shift;

void startOption() {

    opt_selected_item = -1;
    selector = 0;

    VDP_resetSprites();


    VDP_fillTileMapRect(PLAN_B, 0, 0, 0, planWidth, planHeight);

    setMap(PLAN_B, option_data, MAP_GAMEMODE_FALSE);

    VDP_setPalette(0, pal_info_screen);

    optionUpdateState(-1);

    updateSprite();


    sleepFrames(1);

    VDP_setPalette(0, pal_menu);
    VDP_setPalette(1, pal_yellow);

    _tank tank;
    memset(&tank, 0, sizeof(_tank));
    tank.posx = 4;
    tank.posy = 40 + (selector << 4);
    tank.color = TANK_COLOR_RED;
    tank.speed = 1;
    tank.rotate = 3;
    tank.hitpoint = 1;

    JOY_setEventHandler(joyEventOption);

    for (;;) {
        tank.posy = 60 + (selector << 4);

        drawTank(&tank);

        if (opt_selected_item == 3) {
            VDP_resetSprites();
            updateSprite();
            VDP_waitVSync();
            break;
        }

        updateSprite();
        VDP_waitVSync();
    }

    JOY_setEventHandler(NULL);
}

void joyEventOption(u16 joy, u16 changed, u16 state) {
//    item_state_shift = 0;

    switch (state & changed) {
        case BUTTON_UP:
            selector--;
            if (selector == 3)
                selector = 2;
            if (selector < 0)
                selector = 4;
            break;
        case BUTTON_DOWN:
            selector++;
            if (selector == 3)
                selector = 4;
            if (selector > 4)
                selector = 0;
            break;
        case BUTTON_START:
            opt_selected_item = 3;
            break;
        case BUTTON_A: case BUTTON_B: case BUTTON_C:
            opt_selected_item = selector;
            item_state_shift = 1;
            break;
        case BUTTON_RIGHT:
            item_state_shift = 1;
            break;
        case BUTTON_LEFT:
            item_state_shift = -1;
            break;
    }

    if (item_state_shift == 1 || item_state_shift == -1) {
        optionUpdateState(selector);
    }
}

void optionUpdateState(s8 item_to_update) {
    u8 all;
    all = 0;
    if (item_to_update < 0) {
        all = 1;
        item_to_update = 0;
    }

    switch (item_to_update) {
//        case 0:
//            config.difficulty = shiftState(config.difficulty, 4);
//            clearText(10, 6);
//            if (config.difficulty == 0) {
//                drawState("EASY", 6);
//            }
//            else if (config.difficulty == 1) {
//                drawState("NORMAL", 6);
//            }
//            else if (config.difficulty == 2) {
//                drawState("HARD", 6);
//            }
//            else if (config.difficulty == 3) {
//                drawState("VERY HARD", 6);
//            }
//            else if (config.difficulty == 4) {
//                drawState("MADNESS", 6);
//            }
//            if (!all) break;
        case 0:
            if (config.cheats_on) {
                config.start_lives = shiftStateEx(config.start_lives, 3, CHEAT_LIVES);
            }
            else {
                config.start_lives = shiftStateEx(config.start_lives, 3, MAX_LIVES);
            }
            clearText(4, 8);
            drawNum(config.start_lives, 25, 8);

            if (!all) break;
        case 1:
            config.turbo_b = shiftState(config.turbo_b, 1);
            clearText(4, 10);
            if (config.turbo_b) {
                drawState("ON", 10);
            }
            else {
                drawState("OFF", 10);
            }

            if (!all) break;
        case 2:
            config.show_fps = shiftState(config.show_fps, 1);
            clearText(4, 12);
            if (config.show_fps) {
                drawState("ON", 12);
            }
            else {
                drawState("OFF", 12);
            }

            if (!all) break;
    }
}

u8 shiftState(u8 val, u8 max_state) {
    item_state = (s8)val;
    item_state += item_state_shift;
    if (item_state > max_state)
        item_state = 0;
    if (item_state < 0)
        item_state = max_state;
    item_state_shift = 0;
    return (u8)item_state;
}

u8 shiftStateEx(u8 val, u8 min_state, u8 max_state) {
    item_state = (s8)val;
    item_state += item_state_shift;
    if (item_state > max_state)
        item_state = min_state;
    if (item_state < min_state)
        item_state = max_state;
    item_state_shift = 0;
    return (u8)item_state;
}

void clearText(u8 width, u8 y) {
    u8 i;
    for (i = 26 - width; i < 26; i++) {
        mapSetTile(' ', i, y);
    }
}

void drawText(u8 *seq, u8 count, u8 y) {
    u8 i;
    for (i = 0; i < count; i++) {
        mapSetTile(seq[i], 26 - count + i, y);
    }
}

void drawState(const char *string, u8 y) {
    drawText((u8*)string, strlen(string), y);
}

void customGameOption() {

    opt_selected_item = -1;
    selector = 0;

    VDP_resetSprites();


    VDP_fillTileMapRect(PLAN_B, 0, 0, 0, planWidth, planHeight);

    setMap(PLAN_B, game_config_data, MAP_GAMEMODE_FALSE);

    VDP_setPalette(0, pal_info_screen);

    customGameOptionUpdateState(-1);

    updateSprite();


    sleepFrames(1);

    VDP_setPalette(0, pal_menu);
    VDP_setPalette(1, pal_yellow);

    _tank tank;
    memset(&tank, 0, sizeof(_tank));
    tank.posx = 4;
    tank.posy = 40 + (selector << 4);
    tank.color = TANK_COLOR_RED;
    tank.speed = 1;
    tank.rotate = 3;
    tank.hitpoint = 1;

    JOY_setEventHandler(joyEventCustomGameOption);

    for (;;) {
        tank.posy = 44 + (selector << 4);

        drawTank(&tank);

        if (opt_selected_item == 7) {
            VDP_resetSprites();
            updateSprite();
            VDP_waitVSync();
            break;
        }

        updateSprite();
        VDP_waitVSync();
    }

    JOY_setEventHandler(NULL);
}




void joyEventCustomGameOption(u16 joy, u16 changed, u16 state) {
//    item_state_shift = 0;

    switch (state & changed) {
        case BUTTON_UP:
            selector--;
            if (selector == 6)
                selector = 5;
            if (selector < 0)
                selector = 7;
            break;
        case BUTTON_DOWN:
            selector++;
            if (selector == 6)
                selector = 7;
            if (selector > 7)
                selector = 0;
            break;
        case BUTTON_START:
            opt_selected_item = 7;
            break;
        case BUTTON_A: case BUTTON_B: case BUTTON_C:
            opt_selected_item = selector;
            item_state_shift = 1;
            break;
        case BUTTON_RIGHT:
            item_state_shift = 1;
            break;
        case BUTTON_LEFT:
            item_state_shift = -1;
            break;
    }


    if (item_state_shift == 1 || item_state_shift == -1) {
        customGameOptionUpdateState(selector);
    }
}

void customGameOptionUpdateState(s8 item_to_update) {
        u8 all;
        all = 0;
        if (item_to_update < 0) {
            all = 1;
            item_to_update = 0;
        }
        switch (item_to_update) {
        case 0:
            config.levels_pack = shiftState(config.levels_pack, 3);
            clearText(11, 6);
            if (config.levels_pack == LEVELPACK_BC) {
                drawState("BATTLE CITY", 6);
                setMapsData(maps_data);
            }
            else if (config.levels_pack == LEVELPACK_TANK) {
                drawState("TANK 1990", 6);
                setMapsData(maps_data2);
            }
            else if (config.levels_pack == LEVELPACK_BC_TANK) {
                drawState("BC]TANK", 6);
                setMapsData(maps_data);
            }
            else if (config.levels_pack == LEVELPACK_TANK_BC) {
                drawState("TANK]BC", 6);
                setMapsData(maps_data2);
            }
            if (!all) break;
        case 1:
            config.addition_bonus = shiftState(config.addition_bonus, 3);
            clearText(12, 8);
            if (config.addition_bonus == OFF) {
                drawState("OFF", 8);
            }
            else if (config.addition_bonus == SHIP_BONUS) {
                drawState("SHIP", 8);
            }
            else if (config.addition_bonus == GUN_BONUS) {
                drawState("GUN", 8);
            }
            else if (config.addition_bonus == BOTH_BONUS) {
                drawState("SHIP AND GUN", 8);
            }
            if (!all) break;
        case 2:
            config.pl_friend_fire = shiftState(config.pl_friend_fire, 1);
            clearText(10, 10);
            if (config.pl_friend_fire == OFF) {
                drawState("OFF", 10);
            }
            else if (config.pl_friend_fire == BLOCK) {
                drawState("BLINK", 10);
            }
            if (!all) break;
        case 3:
            config.en_bonus_collector = shiftState(config.en_bonus_collector, 1);
            clearText(4, 12);
            if (config.en_bonus_collector) {
                drawState("ON", 12);
            }
            else {
                drawState("OFF", 12);
            }
            if (!all) break;
        case 4:
            config.mods_count = shiftStateEx(config.mods_count, 0, 6);
            clearText(4, 14);
            drawNum(config.mods_count, 25, 14);
            if (!all) break;
        case 5:
            config.mods_inc_lvl = shiftStateEx(config.mods_inc_lvl, 0, 5);
            clearText(4, 16);
            drawNum(config.mods_inc_lvl, 25, 16);
            if (!all) break;
        }
}
