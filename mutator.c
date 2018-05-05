#include "genesis.h"
#include "mutator.h"
//#include "stack.h"
#include "defs.h"

void generateModsList();
extern _mods mods;

void initMods() {
    generateModsList();
}

void showModText() {
    char *text[] = { "", "", "", "", "", "", "" };
//    u16 mods[MODS_COUNT];
    u16 pos[] = { 0, 0, 0, 0, 0, 0, 0 };
    u16 nums = 0;
//    char *txt[] = { "", "", "", "", "", "", "", "", "", "" };
//    int16ToStr(rnd, txt, 10);

    if (mods.en_armor) {
        nums += 1;
        text[nums] = "enemy's armor increased";
        pos[nums] = sizeof("enemy's armor increased");
    }
    if (mods.en_speed) {
        nums += 1;
        text[nums] = "enemy's speed increased";
        pos[nums] = sizeof("enemy's speed increased");
    }
    if (mods.en_uranium) {
        nums += 1;
        text[nums] = "enemy crushes concrete walls";
        pos[nums] = sizeof("enemy crushes concrete walls");
    }
    if (mods.en_bull_speed) {
        nums += 1;
        text[nums] = "enemy's shells speed increased";
        pos[nums] = sizeof("enemy's shells speed increased");
    }
    if (mods.pl_speed_dec) {
        nums += 1;
        text[nums] = "player's speed decrease";
        pos[nums] = sizeof("player's speed decrease");
    }
    if (mods.bul_ricochet) {
        nums += 1;
        text[nums] = "rubber concrete walls";
        pos[nums] = sizeof("rubber concrete walls");
    }
    if (mods.en_no_walls) {
        nums += 1;
        text[nums] = "enemy goes through walls";
        pos[nums] = sizeof("enemy goes through walls");
    }
    if (mods.en_spawn_speed) {
        nums += 1;
        text[nums] = "enemy's spawn speed increased";
        pos[nums] = sizeof("enemy's spawn speed increased");
    }
    if (mods.en_doubles) {
        nums += 1;
        text[nums] = "number of enemies increased";
        pos[nums] = sizeof("number of enemies increased");
    }
    if (mods.pl_en_tank) {
        nums += 1;
        text[nums] = "player used enemy skin";
        pos[nums] = sizeof("player used enemy skin");
    }
    if (mods.en_pl_skin) {
        nums += 1;
        text[nums] = "enemy used player skin";
        pos[nums] = sizeof("enemy used player skin");
    }
    if (mods.pvp_kills) {
        nums += 1;
        text[nums] = "player kills player";
        pos[nums] = sizeof("player kill player");
    }
    if (mods.no_bonuses) {
        nums += 1;
        text[nums] = "no bonuses";
        pos[nums] = sizeof("no bonuses");
    }
    if (mods.en_on_map_inc) {
        nums += 1;
        text[nums] = "enemies on map increased";
        pos[nums] = sizeof("enemies on map increased");
    }
    if (mods.en_shielded) {
        nums += 1;
        text[nums] = "enemies start with shield";
        pos[nums] = sizeof("enemies start with shield");
    }
    if (mods.pl_shieldless) {
        nums += 1;
        text[nums] = "player start without shield";
        pos[nums] = sizeof("player start without shield");
    }
    if (mods.pl_asskiller) {
        nums += 1;
        text[nums] = "player is ass killer";
        pos[nums] = sizeof("player is ass killer");
    }
    if (mods.en_invul) {
        nums += 1;
        text[nums] = "the enemy is invulnerable";
        pos[nums] = sizeof("the enemy is invulnerable");
    }

    switch (nums) {
        case 6:
            VDP_drawTextBG(PLAN_A, text[6], 16-pos[6]/2, 26);
        case 5:
            VDP_drawTextBG(PLAN_A, text[5], 16-pos[5]/2, 24);
        case 4:
            VDP_drawTextBG(PLAN_A, text[4], 16-pos[4]/2, 22);
        case 3:
            VDP_drawTextBG(PLAN_A, text[3], 16-pos[3]/2, 20);
        case 2:
            VDP_drawTextBG(PLAN_A, text[2], 16-pos[2]/2, 18);
        case 1:
            VDP_drawTextBG(PLAN_A, text[1], 16-pos[1]/2, 16);
            break;
        case 0:
            break;
    }
}

void generateModsList() {
    s8 max_mods = 18;
    s8 mod_ind;
    s16 randomize;
    u8 i;
    u8 j;
    u16 mod_indexes[] = { -1, -1, -1, -1, -1, -1 };
    u8 repeat = FALSE;

    mods.en_speed = 0;
    mods.en_armor = 0;
    mods.en_uranium = 0;
    mods.en_bull_speed = 0;
    mods.en_spawn_speed = 0;
    mods.en_doubles = 0;
    mods.en_pl_skin = 0;
    mods.pl_speed_dec = 0;
    mods.pl_en_tank = 0;
    mods.bul_ricochet = 0;
    mods.en_no_walls = 0;
    mods.pvp_kills = 0;
    mods.no_bonuses = 0;
    mods.en_on_map_inc = 0;
    mods.en_shielded = 0;
    mods.pl_shieldless = 0;
    mods.pl_asskiller = 0;
    mods.en_invul = 0;

    randomize = getTime(getTick());
    setRandomSeed (randomize);

    for (i = 0; i < config.mods_count; i++) {
        do {
            repeat = FALSE;
            mod_ind = (random() % max_mods);
            mod_ind %= max_mods;

            for (j = 0; j < 6; j++) {
                if (mod_ind == mod_indexes[j]) {
                    repeat = TRUE;
                }
            }
            if (!repeat)
                mod_indexes[i] = mod_ind;

//            mod_ind ++;
            switch (mod_ind) {
                case 0:
                    mods.en_speed = 1;
                    break;
                case 1:
                    mods.en_armor = 1;
                    break;
                case 2:
                    mods.en_uranium = 1;
                    break;
                case 3:
                    mods.en_bull_speed = 1;
                    break;
                case 4:
                    mods.pl_speed_dec = 1;
                    break;
                case 5:
                    mods.bul_ricochet = 1;
                    break;
                case 6:
                    mods.en_no_walls = 1;
                    break;
                case 7:
                    mods.en_spawn_speed = 1;
                    break;
                case 8:
                    mods.en_doubles = 1;
                    break;
                case 9:
                    mods.pl_en_tank = 1;
                    break;
                case 10:
                    mods.en_pl_skin = 1;
                    break;
                case 11:
                    mods.pvp_kills = 1;
                    break;
                case 12:
                    mods.no_bonuses = 1;
                    break;
                case 13:
                    mods.en_on_map_inc = 1;
                    config.max_enemies_on_map = MAX_ENEMIES + 2;
                    config.units_on_map = config.max_enemies_on_map + 2;
                    config.max_explode = config.units_on_map << 1;
                    config.max_bullets = config.units_on_map << 1;
                    break;
                case 14:
                    mods.en_shielded = 1;
                    break;
                case 15:
                    mods.pl_shieldless = 1;
                    break;
                case 16:
                    mods.pl_asskiller = 1;
                    break;
                case 17:
                    mods.en_invul = 1;
                    break;
            }
        } while (repeat);
    }
//    return mod_indexes;
}

