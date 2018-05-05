
#include "genesis.h"
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
void drawOn(u8 x, u8 y);
void drawOff(u8 x, u8 y);
void drawOnBegin(u8 x, u8 y);
void drawBetLevels(u8 x, u8 y);
void drawKill(u8 x, u8 y);
void drawBulletsStop(u8 x, u8 y);
void drawBlock(u8 x, u8 y);
void drawBC(u8 x, u8 y);
void drawTANK(u8 x, u8 y);
void drawBC_TANK(u8 x, u8 y);
void drawTANK_BC(u8 x, u8 y);
void drawCustom(u8 x, u8 y);
void drawRandom(u8 x, u8 y);
void drawDefault(u8 x, u8 y);

void drawText(u8 seq[], u8 count, u8 y);

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


    VDP_fillTileMapRect (PLAN_B, 0, 0, 0, planWidth, planHeight);

    setMap(PLAN_B, option_data, 0);

    VDP_setPalette(0, pal_info_screen);

    optionUpdateState(-1);

    updateSprite();


    sleepFrames(1);

    VDP_setPalette(0, pal_menu);
    VDP_setPalette(1, pal_yellow);

    _tank tank;
    tank.color = TANK_COLOR_RED;
    tank.posx = 4;
    tank.posy = 40 + (selector << 4);
    tank.rotate = 3;
    tank.type = 0;
    tank.hitpoint = 1;
    tank.god = 0;
    tank.birth = 0;
    tank.speed = 1;
    tank.freeze = 0;
    tank.bonus = 0;

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
            if (selector == 2)
                selector = 1;
            if (selector < 0)
                selector = 3;
            break;
        case BUTTON_DOWN:
            selector++;
            if (selector == 2)
                selector = 3;
            if (selector > 3)
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
//
//            if (config.difficulty == 0) {
//                clearText(10, 6);
//                u8 seq[] = {69,65,83,89};
//                drawText(seq, 4, 6);
//            }
//            else if (config.difficulty == 1) {
//                clearText(10, 6);
//                u8 seq[] = {78,79,82,77,65,76};
//                drawText(seq, 6, 6);
//            }
//            else if (config.difficulty == 2) {
//                clearText(10, 6);
//                u8 seq[] = {72,65,82,68};
//                drawText(seq, 4, 6);
//            }
//            else if (config.difficulty == 3) {
//                clearText(10, 6);
//                u8 seq[] = {86,69,82,89,0,72,65,82,68};
//                drawText(seq, 9, 6);
//            }
//            else if (config.difficulty == 4) {
//                clearText(10, 6);
//                u8 seq[] = {77,65,68,78,69,83,83};
//                drawText(seq, 7, 6);
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
            if (config.turbo_b) {
                clearText(4, 10);
                drawOn(24, 10);
            }
            else {
                clearText(4, 10);
                drawOff(23, 10);
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
    for (i = 26 - width; i < 26; i++)
        mapSetTile(0, i, y);
}

void drawOn(u8 x, u8 y) {
    mapSetTile(79, x, y);
    mapSetTile(78, x + 1, y);
}

void drawOff(u8 x, u8 y) {
    mapSetTile(79, x, y);
    mapSetTile(70, x + 1, y);
    mapSetTile(70, x + 2, y);
}

void drawBetLevels(u8 x, u8 y) {
    mapSetTile(66, x, y);
    mapSetTile(69, x + 1, y);
    mapSetTile(84, x + 2, y);
    mapSetTile(105, x + 3, y);
    mapSetTile(76, x + 4, y);
    mapSetTile(69, x + 5, y);
    mapSetTile(86, x + 6, y);
    mapSetTile(69, x + 7, y);
    mapSetTile(76, x + 8, y);
    mapSetTile(83, x + 9, y);
}

void drawOnBegin(u8 x, u8 y) {
    mapSetTile(79, x, y);
    mapSetTile(78, x + 1, y);
    mapSetTile(0, x + 2, y);
    mapSetTile(66, x + 3, y);
    mapSetTile(69, x + 4, y);
    mapSetTile(71, x + 5, y);
    mapSetTile(73, x + 6, y);
    mapSetTile(78, x + 7, y);

}

void drawKill(u8 x, u8 y) {
    mapSetTile(75, x, y);
    mapSetTile(73, x + 1, y);
    mapSetTile(76, x + 2, y);
    mapSetTile(76, x + 3, y);
}

void drawBulletsStop(u8 x, u8 y) {
    mapSetTile(66, x, y);
    mapSetTile(85, x + 1, y);
    mapSetTile(76, x + 2, y);
    mapSetTile(105, x + 3, y);
    mapSetTile(83, x + 4, y);
    mapSetTile(84, x + 5, y);
    mapSetTile(79, x + 6, y);
    mapSetTile(80, x + 7, y);
}

void drawBlock(u8 x, u8 y) {
    mapSetTile(66, x, y);
    mapSetTile(76, x + 1, y);
    mapSetTile(79, x + 2, y);
    mapSetTile(67, x + 3, y);
    mapSetTile(75, x + 4, y);
}

void drawBC(u8 x, u8 y) {
    mapSetTile(66, x, y);
    mapSetTile(65, x + 1, y);
    mapSetTile(84, x + 2, y);
    mapSetTile(84, x + 3, y);
    mapSetTile(76, x + 4, y);
    mapSetTile(69, x + 5, y);
    mapSetTile(0, x + 6, y);
    mapSetTile(67, x + 7, y);
    mapSetTile(73, x + 8, y);
    mapSetTile(84, x + 9, y);
    mapSetTile(89, x + 10, y);
}

void drawTANK(u8 x, u8 y) {
    mapSetTile(84, x, y);
    mapSetTile(65, x + 1, y);
    mapSetTile(78, x + 2, y);
    mapSetTile(75, x + 3, y);
    mapSetTile(0, x + 4, y);
    mapSetTile(49, x + 5, y);
    mapSetTile(57, x + 6, y);
    mapSetTile(57, x + 7, y);
    mapSetTile(48, x + 8, y);
}
void drawBC_TANK(u8 x, u8 y) {
    mapSetTile(66, x, y);
    mapSetTile(67, x + 1, y);
    mapSetTile(93, x + 2, y);
    mapSetTile(84, x + 3, y);
    mapSetTile(65, x + 4, y);
    mapSetTile(78, x + 5, y);
    mapSetTile(75, x + 6, y);

}
void drawTANK_BC(u8 x, u8 y) {
    mapSetTile(84, x , y);
    mapSetTile(65, x + 1, y);
    mapSetTile(78, x + 2, y);
    mapSetTile(75, x + 3, y);
    mapSetTile(93, x + 4, y);
    mapSetTile(66, x + 5, y);
    mapSetTile(67, x + 6, y);

}

void drawCustom(u8 x, u8 y) {
    mapSetTile(67, x, y);
    mapSetTile(85, x + 1, y);
    mapSetTile(83, x + 2, y);
    mapSetTile(84, x + 3, y);
    mapSetTile(79, x + 4, y);
    mapSetTile(77, x + 5, y);
}

void drawRandom(u8 x, u8 y) {
    mapSetTile(82, x, y);
    mapSetTile(65, x + 1, y);
    mapSetTile(78, x + 2, y);
    mapSetTile(68, x + 3, y);
    mapSetTile(79, x + 4, y);
    mapSetTile(77, x + 5, y);
}

void drawDefault(u8 x, u8 y) {
    mapSetTile(68, x, y);
    mapSetTile(69, x + 1, y);
    mapSetTile(70, x + 2, y);
    mapSetTile(65, x + 3, y);
    mapSetTile(85, x + 4, y);
    mapSetTile(76, x + 5, y);
    mapSetTile(84, x + 6, y);
}

void drawGun(u8 x, u8 y) {
    mapSetTile(71, x, y);
    mapSetTile(85, x + 1, y);
    mapSetTile(78, x + 2, y);
}

void drawShip(u8 x, u8 y) {
    mapSetTile(83, x, y);
    mapSetTile(72, x + 1, y);
    mapSetTile(73, x + 2, y);
    mapSetTile(80, x + 3, y);
}

//83,72,73,80,0,65,78,68,0,71,85,78
void drawText(u8 *seq, u8 count, u8 y) {
    u8 i;
    for (i = 0; i < count; i++) {
        mapSetTile(seq[i], 26 - count + i, y);
    }
}

void customGameOption() {

    opt_selected_item = -1;
    selector = 0;

    VDP_resetSprites();


    VDP_fillTileMapRect (PLAN_B, 0, 0, 0, planWidth, planHeight);

    setMap(PLAN_B, game_config_data, 0);

    VDP_setPalette(0, pal_info_screen);

    customGameOptionUpdateState(-1);

    updateSprite();


    sleepFrames(1);

    VDP_setPalette(0, pal_menu);
    VDP_setPalette(1, pal_yellow);

    _tank tank;
    tank.color = TANK_COLOR_RED;
    tank.posx = 4;
    tank.posy = 40 + (selector << 4);
    tank.rotate = 3;
    tank.type = 0;
    tank.hitpoint = 1;
    tank.god = 0;
    tank.birth = 0;
    tank.speed = 1;
    tank.freeze = 0;
    tank.bonus = 0;

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

            if (config.levels_pack == LEVELPACK_BC) {
                clearText(11, 6);
                drawBC(15, 6);
                setMapsData(maps_data);
            }
            else if (config.levels_pack == LEVELPACK_TANK) {
                clearText(11, 6);
                drawTANK(17, 6);
                setMapsData(maps_data2);
            }
            else if (config.levels_pack == LEVELPACK_BC_TANK) {
                clearText(11, 6);
                drawBC_TANK(19, 6);
                setMapsData(maps_data);
            }
            else if (config.levels_pack == LEVELPACK_TANK_BC) {
                clearText(11, 6);
                drawTANK_BC(19, 6);
                setMapsData(maps_data2);
            }
            if (!all) break;
        case 1:
            config.addition_bonus = shiftState(config.addition_bonus, 3);

            if (config.addition_bonus == OFF) {
                clearText(12, 8);
                drawOff(23, 8);
            }
            else if (config.addition_bonus == SHIP_BONUS) {
                clearText(12, 8);
                drawShip(22, 8);
            }
            else if (config.addition_bonus == GUN_BONUS) {
                clearText(12, 8);
                drawGun(23, 8);
            }
            else if (config.addition_bonus == BOTH_BONUS) {
                clearText(12, 8);
                //ship and gun
                u8 ar[] = {83,72,73,80,0,65,78,68,0,71,85,78};
                drawText(ar, 12, 8);
            }
            if (!all) break;
        case 2:
            config.pl_friend_fire = shiftState(config.pl_friend_fire, 1);

            if (config.pl_friend_fire == OFF) {
                clearText(10, 10);
                drawOff(23, 10);
            }
            else if (config.pl_friend_fire == BLOCK) {
                clearText(10, 10);
                u8 seq[] = {66,76,73,78,75};
                drawText(seq, 5, 10);
            }
            if (!all) break;
        case 3:
            config.en_bonus_collector = shiftState(config.en_bonus_collector, 1);
            if (config.en_bonus_collector) {
                clearText(4, 12);
                drawOn(24, 12);
            }
            else {
                clearText(4, 12);
                drawOff(23, 12);
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




