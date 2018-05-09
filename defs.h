
#ifndef _DEFS_H
#define _DEFS_H

#define MODE_BC                     1
#define MODE_TANK                   2
#define MODE_MODS                   3
#define MODE_CUSTOM                 4

#define OFF                         0
#define ON                          1

#define BLOCK                       1
#define BULLETS_STOP                2
#define KILL                        3


#define LEVELPACK_BC                0
#define LEVELPACK_TANK              1
#define LEVELPACK_BC_TANK           2
#define LEVELPACK_TANK_BC           3

#define LEVELS_ORDER_SERIAL         0
#define LEVELS_ORDER_RANDOM         1

#define SHIP_BONUS                  1
#define GUN_BONUS                   2
#define BOTH_BONUS                  3

#define BONUS_GOD                   1
#define BONUS_TIME                  2
#define BONUS_ARM_STAFF             3
#define BONUS_STAR                  4
#define BONUS_BOMB                  5
#define BONUS_LIFE                  6
#define BONUS_GUN                   7
#define BONUS_SHIP                  8

#define GAME_VERSION                "1.08"
#define GAME_BUILD                  "13"


// ================== GAME CONFIG ===================


#define SHOW_LOGOS                  1
#define SOUND_ON                    1
#define LEVEL_PACK                  LEVELPACK_BC
#define TURBO_B                     ON
#define PL_FRIENDLY_FIRE            BLOCK
#define EN_FRIENDLY_FIRE            OFF
#define EN_BONUS_COLLECTOR          OFF

#define START_LIVES                 3
#define MAX_LIVES                   5
#define CHEAT_LIVES                 9

#define ADDITIONAL_BONUS            OFF
#define CHEATS                      OFF


#define RANDOM_MODS                 1
#define MODS_COUNT                  1
#define MODS_INC                    3

#define DEBUG                       OFF



#define MAX_ENEMIES                 6
//#define PLAYERS_ON_MAP              MAX_ENEMIES + 2



// ==================================================


typedef struct {
    const u8* maps_data;
    u8 game_mode;
    u8 difficulty;
    u8 pl_friend_fire;
    u8 en_friend_fire;
    u8 levels_pack;
    u8 turbo_b;
    u8 start_lives;
    u8 en_bonus_collector;
    u8 levels_order;
    u8 addition_bonus;
    u8 cheats_on;
    u8 debug;
    u8 mods_count;
    u8 mods_inc_lvl;
    u8 random_mods;

    u8 max_enemies_on_map;
    u8 units_on_map;
    u8 max_explode;
    u8 max_bullets;

} _config;


typedef struct {

    u8 en_speed;
    u8 en_armor;
    u8 en_uranium;
    u8 en_bull_speed;
    u8 en_spawn_speed;
    u8 en_doubles;
    u8 en_pl_skin;
    u8 pl_speed_dec;
    u8 pl_en_tank;
    u8 bul_ricochet;
    u8 en_no_walls;
    u8 pvp_kills;
    u8 no_bonuses;
    u8 en_on_map_inc;
    u8 en_shielded;
    u8 pl_shieldless;
    u8 pl_asskiller;
    u8 en_invul;


} _mods;



_config config;
_mods mods;

#endif /* _DEFS_H */


