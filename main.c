#include <genesis.h>
#include "resmap.h"
#include "defs.h"
#include "map.h"
#include "menu.h"
#include "map_editor.h"
#include "game.h"
#include "gameover.h"
#include "sprite.h"
#include "resources.h"
#include "gamelogic.h"
#include "option.h"


void init();
void showSegaLogo();
void showMessage();
void config_init();

u16 pal_buff[16];
s8 do_scroll = TRUE;

extern _mods mods;

int main() {


    init();
    config_init();

    if (SHOW_LOGOS) {
        showSegaLogo();
        showMessage();
    }

    VDP_setScreenWidth256();
    VDP_setPlanSize(32, 64);
//    VDP_setScreenHeight224();
//    VDP_setWindowVPos(FALSE, 5);



    for (;;) {

        startMenu(do_scroll);

        if (menuGetSelectedItem() == ITEM_MAP_EDITOR) {
            startMapEditor();
            do_scroll = FALSE;
            continue;
        }
        else if (menuGetSelectedItem() == ITEM_MAP_OPTION)  {
            startOption();
            do_scroll = FALSE;
            continue;
        }
        else if ((menuGetSelectedItem() == ITEM_ONE_PLAYER) || (menuGetSelectedItem() == ITEM_TWO_PLAYER)) {
            modeMenu();
        }


//                showScore();
        startGame();


        startGameOver();
        do_scroll = TRUE;
    }



    return 0;
}

void init() {

	u32 *save_ptr = (u32*) 0x20000;
	VDP_loadTileData(gfx_bonus, 800, 8*4+4, 0);
	VDP_loadTileData(game_gfx, RES_GFX_ADDR_GAME, RES_GFX_SIZE_GAME, 0);
	VDP_loadTileData(logo_gfx, RES_GFX_ADDR_LOGO, RES_GFX_SIZE_LOGO, 0);

    spriteInit();
    map_editor_map_ready = 0;

    top_scor = save_ptr[0];
    if (top_scor != ~save_ptr[1]) top_scor = 0;

    if (top_scor < 20000) {
        top_scor = 20000;
        save_ptr[0] = top_scor;
        save_ptr[1] = ~top_scor;
    }
    game_player[0].scor = 0;
    game_player[1].scor = 0;

    setMapsData(maps_data);

}

void showSegaLogo() {
    u16 joy;
    u16 scroll = 0;

    u16 i = 0;
    u16 u = 0;
    u16 x = (40 - SEGA_LOGO_W) / 2;
    u16 y = (28 - SEGA_LOGO_H) / 2;
	VDP_setPalette(0, palette_black);
	VDP_fillTileMapRect (PLAN_B, 0, 0, 0, planWidth, planHeight);
	//VDP_fillTileRect(APLAN, 0, 0, 0, vdp_plan_w, vdp_plan_h);
	VDP_fillTileMapRectInc (PLAN_B, RES_GFX_ADDR_LOGO, x, y, SEGA_LOGO_W, SEGA_LOGO_H);
	//VDP_fillTileRectInc(APLAN, RES_GFX_ADDR_LOGO, x, y, SEGA_LOGO_W, SEGA_LOGO_H);
	pal_buff[0] = 0;
	pal_buff[1] = 0xfff;
	for (i = 0; i < 9; i++) pal_buff[i + 2] = pal_logo[(i + scroll) & 0xF];
	VDP_fadePalTo(0, pal_buff, 16, 0);
	//VDP_fadeTo(0, pal_buff, 16, 0);
    sleepFrames(16);

	for (u = 0; u < 40; u++) {

        for (i = 0; i < 9; i++) pal_buff[i + 2] = pal_logo[(i + scroll) & 0xF];

        scroll--;

		VDP_setPalette(0, pal_buff);
        sleepFrames(4);
        joy = JOY_readJoypad(JOY_1);
        if ((joy & (BUTTON_A | BUTTON_B | BUTTON_C | BUTTON_START)))break;
	}
	VDP_fadePalTo(0, palette_black, 16, 0);
	//VDP_fadeTo(0, palette_black, 16, 0);
	sleepFrames(32);
}

void showMessage() {

	VDP_setPalette(0, palette_black);
	VDP_fillTileMapRect (PLAN_B, 0, 0, 0, planWidth, planHeight);

//	VDP_fillTileMapRectInc(PLAN_B, RES_GFX_ADDR_KZZLOGO, 5, 4, 28, 7);
	//VDP_fillTileRectInc(PLAN_B, RES_GFX_ADDR_KZZLOGO, 5, 4, 28, 8);
//    u8 *check;
//    u8 check_ctr = 0;
			//char a[22];
			//intToStr(tile_idx, a, 6);
//    uint16ToStr(GAME_VERSION, str_version, 200);
    VDP_drawTextBG(PLAN_B, "battlecity-md v", (40 - sizeof ("battlecity-md vX.XXX"))/2, 1);
    VDP_drawTextBG(PLAN_B, GAME_VERSION, (40 - sizeof ("battlecity-md vX.XX"))/2 + sizeof ("battlecity-md "), 1);
    VDP_drawTextBG(PLAN_B, "game by KRIKzz 2009", (40 - sizeof ("game by KRIKzz 2009")) / 2, 3);
    VDP_drawTextBG(PLAN_B, "mod by werton 2018", (40 - sizeof ("mod by werton 2018")) / 2, 5);

    VDP_drawTextBG(PLAN_B, "thanks to:", (40 - sizeof ("thanks to:")) / 2, 9);
    VDP_drawTextBG(PLAN_B, "Stephane Dallongeville", (40 - sizeof ("Stephane Dallongeville")) / 2, 11);
    VDP_drawTextBG(PLAN_B, "for incredible SGDK", (40 - sizeof ("for incredible SGDK")) / 2, 13);
    VDP_drawTextBG(PLAN_B, "KRIKzz", (40 - sizeof ("KRIKzz")) / 2, 15);
    VDP_drawTextBG(PLAN_B, "for source code of battlecity-md", (40 - sizeof ("for source code of battlecity-md")) / 2, 17);
    VDP_drawTextBG(PLAN_B, "SPOT", (40 - sizeof ("SPOT")) / 2, 19);
    VDP_drawTextBG(PLAN_B, "for help in code adaptation", (40 - sizeof ("for help in code adaptation")) / 2, 21);
    VDP_drawTextBG(PLAN_B, "MASTERMAX", (40 - sizeof ("MASTERMAX")) / 2, 23);
    VDP_drawTextBG(PLAN_B, "for TANK 1990 level's data", (40 - sizeof ("for TANK 1990 level's data")) / 2, 25);

	VDP_fadePalTo(0, pal_info_screen, 16, 0);
	//VDP_fadeTo(0, pal_info_screen, 16, 0);

    u16 i;
    u16 joy;

    for (i = 0; i < 400; i++) {
        joy = JOY_readJoypad(JOY_1);

        if ((joy & (BUTTON_A | BUTTON_B | BUTTON_C | BUTTON_START)))break;
        VDP_waitVSync();
	}
	VDP_fadePalTo(0, palette_black, 16, 0);
	//VDP_fadeTo(0, palette_black, 16, 0);
}


void config_init() {
    config.game_mode = 0;
    config.difficulty = 1;
    config.levels_pack = LEVEL_PACK;
    config.turbo_b = TURBO_B;
    config.pl_friend_fire = PL_FRIENDLY_FIRE;
    config.en_friend_fire = EN_FRIENDLY_FIRE;
    config.start_lives = START_LIVES;
    config.en_bonus_collector = EN_BONUS_COLLECTOR;
    config.addition_bonus = ADDITIONAL_BONUS;
    config.cheats_on = CHEATS;
    config.debug = DEBUG;
    config.mods_count = MODS_COUNT;
    config.mods_inc_lvl = MODS_INC;
    config.random_mods = RANDOM_MODS;

    config.max_enemies_on_map = MAX_ENEMIES;
    config.units_on_map = config.max_enemies_on_map + 2;
    config.max_explode = config.units_on_map << 1;
    config.max_bullets = config.units_on_map << 1;



    if (config.levels_pack == LEVELPACK_BC) {
        setMapsData(maps_data);
    }
    else if (config.levels_pack == LEVELPACK_TANK){
        setMapsData(maps_data2);
    }

    mods.en_speed = 0;
    mods.en_armor = 0;
    mods.en_uranium = 0;
    mods.en_bull_speed = 0;
    mods.pl_speed_dec = 0;
    mods.bul_ricochet = 0;
    mods.en_no_walls = 0;
    mods.en_spawn_speed = 0;
    mods.en_doubles = 0;
    mods.en_on_map_inc = 0;
    mods.pl_en_tank = 0;
    mods.en_pl_skin = 0;
    mods.pvp_kills = 0;
    mods.no_bonuses = 0;
    mods.en_on_map_inc = 0;
    mods.en_shielded = 0;
    mods.pl_shieldless = 0;
    mods.pl_asskiller = 0;
    mods.en_invul = 0;
}


void battlecity_init() {
    config.game_mode = MODE_BC;
    config.levels_pack = LEVELPACK_BC;
    config.pl_friend_fire = BLOCK;
    config.en_friend_fire = OFF;
    config.en_bonus_collector = OFF;
    config.addition_bonus = OFF;
    setMapsData(maps_data);
}

void tank1990_init() {
    config.game_mode = MODE_TANK;
    config.levels_pack = LEVELPACK_TANK;
    config.pl_friend_fire = OFF;
    config.en_friend_fire = OFF;
    config.en_bonus_collector = EN_BONUS_COLLECTOR;
    config.addition_bonus = BOTH_BONUS;
    setMapsData(maps_data2);
}

void mods_mode_init() {
    config.game_mode = MODE_MODS;
    config.levels_pack = LEVELPACK_BC;
    config.pl_friend_fire = BLOCK;
    config.en_friend_fire = OFF;
    config.en_bonus_collector = ON;
    config.addition_bonus = OFF;
    setMapsData(maps_data);
}
