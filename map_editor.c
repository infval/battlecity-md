
#include <genesis.h>
#include "map.h"
#include "map_editor.h"
#include "resmap.h"
#include "sprite.h"
#include "game.h"

u8 editor_map[MAP_W * MAP_H];
u8 map_editor_map_ready;
#define JOY_CROSS_MASK  (BUTTON_UP | BUTTON_DOWN | BUTTON_LEFT | BUTTON_RIGHT)

s8 object_selector;
s8 selector_x;
s8 selector_y;
u8 moved;
#define MAX_OBJECTS 14

void joyEventEditor(u16 joy, u16 changed, u16 state);
void setObjrct();

const u8 map_objects[] = {

    0, 0,
    0, 0,

    0, RES_TILE_BRICK,
    0, RES_TILE_BRICK,

    0, 0,
    RES_TILE_BRICK, RES_TILE_BRICK,

    RES_TILE_BRICK, 0,
    RES_TILE_BRICK, 0,

    RES_TILE_BRICK, RES_TILE_BRICK,
    0, 0,

    RES_TILE_BRICK, RES_TILE_BRICK,
    RES_TILE_BRICK, RES_TILE_BRICK,

    0, RES_TILE_ARMOR,
    0, RES_TILE_ARMOR,

    0, 0,
    RES_TILE_ARMOR, RES_TILE_ARMOR,

    RES_TILE_ARMOR, 0,
    RES_TILE_ARMOR, 0,

    RES_TILE_ARMOR, RES_TILE_ARMOR,
    0, 0,

    RES_TILE_ARMOR, RES_TILE_ARMOR,
    RES_TILE_ARMOR, RES_TILE_ARMOR,

    RES_TILE_WATER, RES_TILE_WATER,
    RES_TILE_WATER, RES_TILE_WATER,

    RES_TILE_GRASS, RES_TILE_GRASS,
    RES_TILE_GRASS, RES_TILE_GRASS,

    RES_TILE_DIRT, RES_TILE_DIRT,
    RES_TILE_DIRT, RES_TILE_DIRT,


};

void startMapEditor() {

    u16 joy;
    u16 cross_press_timer = 0;
    u8 selector_blink_counter = 0;
    moved = 0;
    _tank tank;
    VDP_setPalette(0, palette_black);
    VDP_resetSprites();
    VDP_updateSprites(1, TRUE);
    //VDP_updateSpritesDma();

    VDP_fillTileMapRect (PLAN_B, RES_TILE_GREY, 0, 0, 32, 28);

    if (!map_editor_map_ready) {
        setMap(PLAN_B, maps_data + MAP_LEN * MAP_EDITOR, 0);
    }
    else {
        setMap(PLAN_B, editor_map, 0);
    }

    map_editor_map_ready = FALSE;
    VDP_setPalette(0, pal_red);
    VDP_setPalette(1, pal_yellow);
    object_selector = 0;

    selector_x = 0;
    selector_y = 0;
    JOY_setEventHandler(joyEventEditor);
    tank.color = TANK_COLOR_RED;
    tank.posx = 0;
    tank.posy = 0;
    tank.rotate = 0;
    tank.type = 0;
    tank.color = TANK_COLOR_YELLOW;

    for (; !map_editor_map_ready;) {

        joy = JOY_readJoypad(JOY_1);
        if (joy & JOY_CROSS_MASK) {
            cross_press_timer++;
        }
        else {
            cross_press_timer = 0;
        }
        if (cross_press_timer > 32) {
            if ((cross_press_timer & 3) == 0)
                joyEventEditor(JOY_1, 0, joy);
        }

        tank.posx = selector_x << 4;
        tank.posy = selector_y << 4;
        selector_blink_counter++;
        if (((selector_blink_counter >> 4) & 1)) {
            drawTank(&tank);
            updateSprite();
        }
        else {
            VDP_resetSprites();
            //VDP_updateSprites  ();
            //VDP_updateSpritesDma();
        }

        VDP_waitVSync();
    }
    JOY_setEventHandler(NULL);
}

void joyEventEditor(u16 joy, u16 changed, u16 state) {

    u16 i;

    if (JOY_CROSS_MASK & changed) {
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
        setObjrct();
        moved = 0;
    }
    if (BUTTON_C & state) {
        if (!moved) {
            object_selector--;
            if (object_selector < 0)
                object_selector = MAX_OBJECTS - 1;
        }
        setObjrct();
        moved = 0;
    }
}

void setObjrct() {

    mapSetTile(map_objects[(object_selector << 2)    ],  selector_x << 1,       selector_y << 1);
    mapSetTile(map_objects[(object_selector << 2) + 1], (selector_x << 1) + 1,  selector_y << 1);
    mapSetTile(map_objects[(object_selector << 2) + 2],  selector_x << 1,      (selector_y << 1) + 1);
    mapSetTile(map_objects[(object_selector << 2) + 3], (selector_x << 1) + 1, (selector_y << 1) + 1);
}
