
#include <genesis.h>
#include "map.h"
#include "map_editor.h"
#include "resmap.h"
#include "sprite.h"
#include "game.h"

u8 editor_map[MAP_W * MAP_H];
u8 map_editor_map_ready;

s8 object_selector;
s8 selector_x;
s8 selector_y;
u8 moved;
#define MAX_OBJECTS 14

void joyEventEditor(u16 joy, u16 changed, u16 state);
void setObject();

void startMapEditor() {

    u16 joy;
    u16 cross_press_timer = 0;
    u8 selector_blink_counter = 0;
    moved = 0;
    VDP_setPalette(0, palette_black);
    VDP_resetSprites();
    VDP_updateSprites(1, TRUE);

    VDP_fillTileMapRect(PLAN_B, RES_TILE_GREY, 0, 0, 32, 28);

    if (!map_editor_map_ready) {
        memset(editor_map, 0, sizeof(editor_map));
        setMap(PLAN_B, editor_map, MAP_GAMEMODE_FALSE);
        mapSetTile(RES_TILE_STAFF + 0, START_X_ST + 0, START_Y_ST + 0);
        mapSetTile(RES_TILE_STAFF + 1, START_X_ST + 0, START_Y_ST + 1);
        mapSetTile(RES_TILE_STAFF + 2, START_X_ST + 1, START_Y_ST + 0);
        mapSetTile(RES_TILE_STAFF + 3, START_X_ST + 1, START_Y_ST + 1);
        s8 brick_x[8] = { -1, -1, -1,  0,  1,  2,  2,  2 };
        s8 brick_y[8] = {  1,  0, -1, -1, -1, -1,  0,  1 };
        u8 i;
        for (i = 0; i < 8; i++) {
            mapSetTile(RES_TILE_BRICK, START_X_ST + brick_x[i], START_Y_ST + brick_y[i]);
        }
    }
    else {
        setMap(PLAN_B, editor_map, MAP_GAMEMODE_FALSE);
    }

    map_editor_map_ready = FALSE;
    VDP_setPalette(0, pal_red);
    VDP_setPalette(1, pal_yellow);
    object_selector = 0;

    selector_x = 0;
    selector_y = 0;
    JOY_setEventHandler(joyEventEditor);
    s16 posx = 0;
    s16 posy = 0;

    for (; !map_editor_map_ready;) {

        joy = JOY_readJoypad(JOY_1);
        if (joy & BUTTON_DIR) {
            cross_press_timer++;
        }
        else {
            cross_press_timer = 0;
        }
        if (cross_press_timer > 32) {
            if ((cross_press_timer & 3) == 0)
                joyEventEditor(JOY_1, 0, joy);
        }

        posx = selector_x << 4;
        posy = selector_y << 4;
        selector_blink_counter++;
        if ((selector_blink_counter >> 4) & 1) {
            drawSprite2x2(SPRITE_ADDR_TANK | TILE_ATTR(TANK_COLOR_YELLOW, 0, 0, 0), posx, posy);
        }
        updateSprite();

        VDP_waitVSync();
    }
    JOY_setEventHandler(NULL);
}

void joyEventEditor(u16 joy, u16 changed, u16 state) {

    u16 i;

    if (BUTTON_DIR & changed) {
        moved = 1;
    }

    if (BUTTON_UP & state) {
        selector_y--;
        moved = 1;
        if (selector_y < 0)
            selector_y = 0;
    }
    if (BUTTON_DOWN & state) {
        selector_y++;
        moved = 1;
        if (selector_y > MAP_H / 2 - 1)
            selector_y = MAP_H / 2 - 1;
    }
    if (BUTTON_LEFT & state) {
        selector_x--;
        moved = 1;
        if (selector_x < 0)
            selector_x = 0;
    }
    if (BUTTON_RIGHT & state) {
        selector_x++;
        moved = 1;
        if (selector_x > MAP_W / 2 - 1)
            selector_x = MAP_W / 2 - 1;
    }
    if (BUTTON_START & state) {
        for (i = 0; i < MAP_LEN; i++)
            editor_map[i] = (u8) current_map[i];
        map_editor_map_ready = TRUE;
    }

    if (BUTTON_B & state) {
        if (!moved) {
            object_selector++;
            if (object_selector >= MAX_OBJECTS)
                object_selector = 0;
        }
        setObject();
        moved = 0;
    }
    if (BUTTON_C & state) {
        if (!moved) {
            object_selector--;
            if (object_selector < 0)
                object_selector = MAX_OBJECTS - 1;
        }
        setObject();
        moved = 0;
    }
}

void setObject() {

    mapSetTile(map_objects[(object_selector << 2)    ],  selector_x << 1,       selector_y << 1);
    mapSetTile(map_objects[(object_selector << 2) + 1], (selector_x << 1) + 1,  selector_y << 1);
    mapSetTile(map_objects[(object_selector << 2) + 2],  selector_x << 1,      (selector_y << 1) + 1);
    mapSetTile(map_objects[(object_selector << 2) + 3], (selector_x << 1) + 1, (selector_y << 1) + 1);
}
