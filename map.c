
#include <genesis.h>
#include "map.h"
#include "resmap.h"
#include "defs.h"

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

    0, RES_TILE_STEEL,
    0, RES_TILE_STEEL,

    0, 0,
    RES_TILE_STEEL, RES_TILE_STEEL,

    RES_TILE_STEEL, 0,
    RES_TILE_STEEL, 0,

    RES_TILE_STEEL, RES_TILE_STEEL,
    0, 0,

    RES_TILE_STEEL, RES_TILE_STEEL,
    RES_TILE_STEEL, RES_TILE_STEEL,

    RES_TILE_RIVER, RES_TILE_RIVER,
    RES_TILE_RIVER, RES_TILE_RIVER,

    RES_TILE_WOODS, RES_TILE_WOODS,
    RES_TILE_WOODS, RES_TILE_WOODS,

    RES_TILE_ICE, RES_TILE_ICE,
    RES_TILE_ICE, RES_TILE_ICE,
};


u16 current_map[MAP_LEN];
VDPPlan map_plan;

void setMapEx(VDPPlan plan, const u8 *map, u8 game_mode, u8 fake) {

    u16 i = 0;
    map_plan = plan;

    // Compressed
    if (game_mode == MAP_GAMEMODE_COMPRESSED) {
        for (i = 0; i < MAP_LEN / 4; i++) {
            u8 obj_index = map[i >> 1];
            if (i & 1) {
                obj_index >>= 4;
            }
            else {
                obj_index &= 0xF;
            }

            u16 x = (i * 2) % MAP_W + (i * 2 / MAP_W) * MAP_W * 2;
            current_map[x + 0        ] = map_objects[(obj_index << 2) + 0];
            current_map[x + 1        ] = map_objects[(obj_index << 2) + 1];
            current_map[x + 0 + MAP_W] = map_objects[(obj_index << 2) + 2];
            current_map[x + 1 + MAP_W] = map_objects[(obj_index << 2) + 3];
        }

        mapSetTile(RES_TILE_BRICK, START_X_ST - 1, START_Y_ST - 1);
        mapSetTile(RES_TILE_BRICK, START_X_ST + 2, START_Y_ST - 1);
    }
    else {
        for (i = 0; i < MAP_LEN; i++) {
            current_map[i] = map[i];
        }
    }

    if (game_mode != MAP_GAMEMODE_FALSE) {
        if (!fake) {
            for (i = 0; i < MAP_LEN; i++) {
                if (current_map[i] == RES_TILE_WOODS) {
                    current_map[i] |= TILE_ATTR(PAL0, 1, 0, 0);
                }
            }
        }

        mapSetTile(0, START_X_EN_A + 0, 0); mapSetTile(0, START_X_EN_A + 1, 0);
        mapSetTile(0, START_X_EN_A + 0, 1); mapSetTile(0, START_X_EN_A + 1, 1);

        mapSetTile(0, START_X_EN_B + 0, 0); mapSetTile(0, START_X_EN_B + 1, 0);
        mapSetTile(0, START_X_EN_B + 0, 1); mapSetTile(0, START_X_EN_B + 1, 1);

        mapSetTile(0, START_X_EN_C + 0, 0); mapSetTile(0, START_X_EN_C + 1, 0);
        mapSetTile(0, START_X_EN_C + 0, 1); mapSetTile(0, START_X_EN_C + 1, 1);

        mapSetTile(0, START_X_PL_A + 0, START_Y_PL_A + 0);
        mapSetTile(0, START_X_PL_A + 1, START_Y_PL_A + 0);
        mapSetTile(0, START_X_PL_A + 0, START_Y_PL_A + 1);
        mapSetTile(0, START_X_PL_A + 1, START_Y_PL_A + 1);

        mapSetTile(0, START_X_PL_B + 0, START_Y_PL_B + 0);
        mapSetTile(0, START_X_PL_B + 1, START_Y_PL_B + 0);
        mapSetTile(0, START_X_PL_B + 0, START_Y_PL_B + 1);
        mapSetTile(0, START_X_PL_B + 1, START_Y_PL_B + 1);

        mapSetTile(RES_TILE_STAFF + 0, START_X_ST + 0, START_Y_ST + 0);
        mapSetTile(RES_TILE_STAFF + 1, START_X_ST + 0, START_Y_ST + 1);
        mapSetTile(RES_TILE_STAFF + 2, START_X_ST + 1, START_Y_ST + 0);
        mapSetTile(RES_TILE_STAFF + 3, START_X_ST + 1, START_Y_ST + 1);
    }

    mapRepaint();
}

void mapSetTile(u16 val, u8 x, u8 y) {

    current_map[x + y * MAP_H] = val;
    VDP_setTileMapXY(map_plan, val, x + MAP_X, y + MAP_Y);
}

u16 mapGetTile(u16 x, u16 y) {

    return current_map[x + y * MAP_W];
}

void mapRepaint() {

    VDP_setTileMapDataRect(map_plan, current_map, MAP_X, MAP_Y, MAP_W, MAP_H);
}

void setMap(VDPPlan plan, const u8 *map, u8 game_mode) {
    setMapEx(plan, map, game_mode, FALSE);
}

void setFakeMap(const u8 *map, u8 game_mode) {
    setMapEx(PLAN_B, map, game_mode, TRUE);
}

void setMapLevel(u8 lvl) {
    setMap(PLAN_B, MAP_GAME_MAP * MAP_LEN + (config.maps_data + lvl % MAP_AVAILABLE * MAP_COMPRESSED_LEN), MAP_GAMEMODE_COMPRESSED);
}

void setFakeMapLevel(u8 lvl) {
    setFakeMap(MAP_GAME_MAP * MAP_LEN + (config.maps_data + lvl % MAP_AVAILABLE * MAP_COMPRESSED_LEN), MAP_GAMEMODE_COMPRESSED);
}

void setMapsData(const u8 *map) {
    config.maps_data = map;
}


void openScreenEffect() {
    u8 i;
    for (i = 14; i > 0; i--) {
        VDP_fillTileMapRect(PLAN_A, 0, 0,  i - 1, 32, 1);
        VDP_fillTileMapRect(PLAN_A, 0, 0, 28 - i, 32, 1);
        VDP_waitVSync();
    }
}

void closeScreenEffect() {
    u8 i;
    for (i = 0; i < 15; i++) {
        VDP_fillTileMapRect(PLAN_A, RES_TILE_GREY, 0,  i - 1, 32, 1);
        VDP_fillTileMapRect(PLAN_A, RES_TILE_GREY, 0, 28 - i, 32, 1);
        VDP_waitVSync();
    }

    VDP_fillTileMapRect(PLAN_B, RES_TILE_GREY, 0, 0, 32, 28);
    VDP_fillTileMapRect(PLAN_A, 0, 0, 0, planWidth, planHeight);
    VDP_waitVSync();
}
