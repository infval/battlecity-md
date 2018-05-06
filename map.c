
#include <genesis.h>
#include "map.h"
#include "resmap.h"
#include "defs.h"


u16 current_map[MAP_LEN];
VDPPlan map_plan;
//u16 map_plan;

void setMapEx(VDPPlan plan, const u8 *map, u8 game_mode, u8 fake) {

    u16 i = 0;
    map_plan = plan;

    for (i = 0; i < MAP_LEN; i++) {
        current_map[i] = map[i];
        if (!fake) {
            if (game_mode && current_map[i] == RES_TILE_GRASS) {
                current_map[i] |= TILE_ATTR(0, 1, 0, 0);
            }
        }
    }

    if (game_mode) {
        mapSetTile(0, START_X_EN_A + 0, 0);
        mapSetTile(0, START_X_EN_A + 1, 0);
        mapSetTile(0, START_X_EN_A + 0, 1);
        mapSetTile(0, START_X_EN_A + 1, 1);

        mapSetTile(0, START_X_EN_B + 0, 0);
        mapSetTile(0, START_X_EN_B + 1, 0);
        mapSetTile(0, START_X_EN_B + 0, 1);
        mapSetTile(0, START_X_EN_B + 1, 1);

        mapSetTile(0, START_X_EN_C + 0, 0);
        mapSetTile(0, START_X_EN_C + 1, 0);
        mapSetTile(0, START_X_EN_C + 0, 1);
        mapSetTile(0, START_X_EN_C + 1, 1);

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
    //VDP_setTile(map_plan, val, x + MAP_X, y + MAP_Y);
}

u16 mapGetTile(u16 x, u16 y) {

    return current_map[x + y * MAP_W];
}

void mapRepaint() {

    VDP_setTileMapDataRect(map_plan, current_map, MAP_X, MAP_Y, MAP_W, MAP_H);
    //VDP_setTileRectDma(map_plan, current_map, MAP_X, MAP_Y, MAP_W, MAP_H);
}

void setMap(VDPPlan plan, const u8 *map, u8 game_mode) {
    setMapEx(plan, map, game_mode, 0);
}

void setFakeMap(const u8 *map, u8 game_mode) {
    setMapEx(PLAN_B, map, game_mode, 1);
}

void setMapLevel(u8 lvl) {
    setMap(PLAN_B, MAP_GAME_MAP * MAP_LEN + (config.maps_data + lvl % MAP_AVAILABLE * MAP_LEN), 1);
}

void setFakeMapLevel(u8 lvl) {
    setFakeMap(MAP_GAME_MAP * MAP_LEN + (config.maps_data + lvl % MAP_AVAILABLE * MAP_LEN), 1);
}

void setMenuMap() {
    setMap(PLAN_B, maps_data, MAP_START_MENU);
}
void setEmptyEditorMap() {
    setMap(PLAN_B, maps_data, MAP_EDITOR);
}
void setGameOverMap() {
    setMap(PLAN_B, maps_data, MAP_GAMEOVER);
}
void setScore1Map() {
    setMap(PLAN_B, maps_data, MAP_SCORE1);
}
void setScore2Map() {
    setMap(PLAN_B, maps_data, MAP_SCORE2);
}

void setMapsData(const u8 *map) {
    config.maps_data = map;
}


void openScreenEffect() {
    u8 i;
    VDP_fillTileMapRect(PLAN_A, RES_TILE_GREY, 0, 0, planWidth, planHeight);
    for (i = 14; i > 0; i--) {
        VDP_fillTileMapRect(PLAN_A, 0, 0, i, 32, 1);
        VDP_fillTileMapRect(PLAN_A, 0, 0, 28 - i, 32, 1);
        VDP_waitVSync();
    }
}

void closeScreenEffect() {
    u8 i;
    for (i = 0; i < 15; i++) {
        VDP_fillTileMapRect (PLAN_A, RES_TILE_GREY, 0, i, 32, 1);
        VDP_fillTileMapRect (PLAN_A, RES_TILE_GREY, 0, 28 - i, 32, 1);
        VDP_waitVSync();
    }

    VDP_fillTileMapRect(PLAN_B, RES_TILE_GREY, 0, 0, 32, 28);
    VDP_fillTileMapRect(PLAN_A, 0, 0, 0, planWidth, planHeight);

    VDP_waitVSync();
}
