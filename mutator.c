#include "genesis.h"
#include "mutator.h"
//#include "stack.h"
#include "defs.h"

void generateModsList();
extern _mods mods;
#define MAX_MODS (sizeof(_mods) / sizeof(u8))

void initMods() {
    generateModsList();
}

static const char* mods_text[] = {
      "enemy's speed increased"
    , "enemy's armor increased"
    , "enemy crushes concrete walls"
    , "enemy's shells speed increased"
    , "enemy's spawn speed increased"
    , "number of enemies increased"
    , "enemy used player skin"
    , "player's speed decrease"
    , "player used enemy skin"
    , "rubber concrete walls"
    , "enemy goes through walls"
    , "player kills player"
    , "no bonuses"
    , "enemies on map increased"
    , "enemies start with shield"
    , "player start without shield"
    , "player is ass killer"
    , "the enemy is invulnerable"
};

void showModText() {
    const char *text[] = { "", "", "", "", "", "", "" };
    u16 pos[] = { 0, 0, 0, 0, 0, 0, 0 };
    u16 nums = 0;

    s16 i;
    u8* p_mods = (u8*)&mods;
    for (i = 0; i < MAX_MODS; i++) {
        if (p_mods[i]) {
            nums++;
            text[nums] = mods_text[i];
            pos[nums] = strlen(mods_text[i]);
        }
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

// Fisher-Yates shuffle
static void shuffle_u16(u16 array[], u16 n) {
    while (n > 1) {
        u16 k = random() % n;
        n--;
        SWAP_u16(array[k], array[n]);
    }
}

void generateModsList() {
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

    setRandomSeed(getTime(getTick()));

    u8 i;
    u16 mod_indexes[MAX_MODS];
    for (i = 0; i < MAX_MODS; i++) {
        mod_indexes[i] = i;
    }
    shuffle_u16(mod_indexes, MAX_MODS);
    for (i = 0; i < config.mods_count; i++) {
        switch (mod_indexes[i]) {
            case  0: mods.en_speed = 1;       break;
            case  1: mods.en_armor = 1;       break;
            case  2: mods.en_uranium = 1;     break;
            case  3: mods.en_bull_speed = 1;  break;
            case  4: mods.en_spawn_speed = 1; break;
            case  5: mods.en_doubles = 1;     break;
            case  6: mods.en_pl_skin = 1;     break;
            case  7: mods.pl_speed_dec = 1;   break;
            case  8: mods.pl_en_tank = 1;     break;
            case  9: mods.bul_ricochet = 1;   break;
            case 10: mods.en_no_walls = 1;    break;
            case 11: mods.pvp_kills = 1;      break;
            case 12: mods.no_bonuses = 1;     break;
            case 13: mods.en_on_map_inc = 1;
                config.max_enemies_on_map = MAX_ENEMIES + 2;
                config.units_on_map = config.max_enemies_on_map + 2;
                config.max_explode = config.units_on_map << 1;
                config.max_bullets = config.units_on_map << 1;
                break;
            case 14: mods.en_shielded = 1;    break;
            case 15: mods.pl_shieldless = 1;  break;
            case 16: mods.pl_asskiller = 1;   break;
            case 17: mods.en_invul = 1;       break;
        }
    }
}

