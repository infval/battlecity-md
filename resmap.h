#ifndef _RESMAP_H
#define _RESMAP_H

#define SEGA_LOGO_W 12
#define SEGA_LOGO_H 4
#define RES_GFX_SIZE_LOGO 48
#define RES_GFX_SIZE_KZZLOGO 224
#define RES_GFX_SIZE_GAME 516
#define RES_GFX_ADDR_GAME 0
#define RES_GFX_ADDR_LOGO (RES_GFX_ADDR_GAME + RES_GFX_SIZE_GAME)
#define RES_GFX_ADDR_KZZLOGO (RES_GFX_ADDR_LOGO + RES_GFX_SIZE_LOGO)
//#define RES_TILE_BLANK 0
#define RES_TILE_BRICK 15
#define RES_TILE_STEEL 16
#define RES_TILE_WOODS 17
#define RES_TILE_RIVER 18
#define RES_TILE_ICE   19
#define RES_TILE_GREY  34
#define RES_TILE_STAGE 35
#define RES_TILE_STAGE_NUM 110
#define RES_TILE_STAFF 60
#define RES_TILE_DEATH_STAFF 28
#define RES_TILE_TANK_ICON_BLACK  58
#define RES_TILE_TANK_ICON_OGANGE 23
#define RES_TILE_P  20
#define RES_TILE_P1 21
#define RES_TILE_P2 22
#define RES_TILE_FLAG 24

#define RES_COLOR_RIVER1_ADDR 6
#define RES_COLOR_RIVER2_ADDR 7
#define RES_COLOR_RIVER3_ADDR 8

extern u32 game_gfx[];
extern u32 logo_gfx[];
extern const u32 kzzlogo_gfx[];
extern u16 pal_red[];
extern u16 pal_green[];
extern u16 pal_yellow[];
extern u16 pal_grey[];
extern u16 pal_logo[];
extern u16 pal_menu[];
extern u16 pal_orange_text[];
extern u16 pal_red_text[];
extern u16 pal_info_screen[];

extern u8 maps_data[];
extern u8 menu_data[];
extern u8 maps_data2[];
extern u8 option_data[];
extern u8 game_config_data[];

//extern u8 snd_start[45857];
//extern u8 snd_engine_1[1599];
//extern u8 snd_engine_2[1468];
//extern u8 snd_explode[5617];
//extern u8 snd_bull_start[1788];
//extern u8 snd_bull_stop[720];
//extern u8 snd_pause[6761];
//extern u8 snd_enemy_explode[3501];
//extern u8 snd_bull_stop_briks[870];
//extern u8 snd_bonus_make[5872];
//extern u8 snd_bonus_take[7164];
//extern u8 snd_gameover[18207];

#endif /* _RESMAP_H */

