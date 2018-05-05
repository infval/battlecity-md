#ifndef _MAP_H
#define _MAP_H

#define MAP_W 26
#define MAP_H 26
#define MAP_X 2
#define MAP_Y 1
#define MAP_PIXEL_X (MAP_X << 3)
#define MAP_PIXEL_Y (MAP_Y << 3)
#define MAP_LEN MAP_W * MAP_H
#define MAP_START_MENU 0
#define MAP_EDITOR 1
#define MAP_GAMEOVER 2
#define MAP_SCORE1 3
#define MAP_SCORE2 4
#define MAP_GAME_MAP 5
#define MAP_AVAILABLE 35
#define MAP_OPTION 40

#define MAP_COUNT 70


#define START_X_PL_A 8
#define START_Y_PL_A 24
#define START_X_PL_B 16
#define START_Y_PL_B 24

#define START_X_EN_A 0
#define START_X_EN_B (MAP_W / 2 - 1)
#define START_X_EN_C (MAP_W -  2)

#define START_X_ST 12
#define START_Y_ST 24

extern u16 current_map[];

void setMap(VDPPlan plan, const u8 *map, u8 game_mode);
void setMapEx(VDPPlan plan, const u8 *map, u8 game_mode, u8 fake);
void setFakeMap(const u8 *map, u8 game_mode);
void setMapLevel(u8 lvl);
void setFakeMapLevel(u8 lvl);
void setMenuMap();
void setEmptyEditorMap();
void setGameOverMap();
void setScore1Map();
void setScore2Map();
void setMapsData(const u8 *map);
void mapSetTile(u16 val, u8 x, u8 y);
u16 mapGetTile(u16 x, u16 y);
void mapRepaint();


void openScreenEffect();
void closeScreenEffect();

#endif /* _MAP_H */

