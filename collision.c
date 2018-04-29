
#include "genesis.h"
#include "collision.h"
#include "map.h"
#include "gamelogic.h"
#include "resmap.h"
#include "defs.h"
#include "resources.h"
//#include "audio.h"

void tileReaction(_bullet* bull, s16* after_hit, u8* kill_bull, s16* x0, s16* y0, u8 xShift, u8 yShift);

u8 moveAvailableInUnits(_tank *tank) {



    u16 i = config.units_on_map;
    _tank *buff;

    u16 x = tank->posx;
    u16 y = tank->posy;
    s16 delta_x;
    s16 delta_y;
    while (i--) {

        buff = &game_player[i];
        if (tank == buff)continue;
        if (!buff->hitpoint)continue;
        delta_x = x - buff->posx;
        delta_y = y - buff->posy;
        if (delta_x < 0)delta_x = -delta_x;
        if (delta_y < 0)delta_y = -delta_y;
        if (delta_x < 16 && delta_y < 16)return delta_x + delta_y;
    }

    return 32;
}

void detectBulletToWallCollision(_bullet *bull) {

    u16 tile_idx = 0;
    s16 x = bull->posx;
    s16 y = bull->posy;
    x += speed_x[bull->rotate] << 1;
    y += speed_y[bull->rotate] << 1;

    s16 x0 = x >> 3;
    s16 y0 = y >> 3;


    s16 after_hit;
    s16 mod = (((x | y) & 7) - 1) >> 2;
    u8 kill_bull = 0;
    u16 rotate = bull->rotate;

    if (bull->speed == 0) return;

    if (x0 < 0 || x0 >= MAP_W || y0 < 0 || y0 >= MAP_H) {

        if (mods.bul_ricochet && (!bull->ricocheted)) {
            bull->ricocheted = TRUE;

            u8 rot = bull->rotate + 2;

//            u8 rot = (random() % 3);
//            rot %= 3;
//            if (rot == bull->rotate) GLog_killBullet(bull, 1);

            bull->rotate = rot;
            if (bull->rotate > 3) bull->rotate = bull->rotate - 4;
        }
        else {
            GLog_killBullet(bull, 1);
            if (bull->maker == &game_player[0] || bull->maker == &game_player[1]) soundPlay(snd_bull_stop, sizeof (snd_bull_stop), SOUND_PCM_CH3, 0);
//            return;
        }
        return;
    }

    tile_idx = mapGetTile(x0, y0);
    after_hit = tileAfterHit(tile_idx, rotate, mod);
    if (after_hit >= 0) {
        tileReaction(bull, &after_hit, &kill_bull, &x0, &y0, 0, 0);
    }
    if ((rotate & 1)) {
        tile_idx = mapGetTile(x0, y0 + 1);
        after_hit = tileAfterHit(tile_idx, rotate, mod);

        if (after_hit >= 0) {
            tileReaction(bull, &after_hit, &kill_bull, &x0, &y0, 0, 1);
        }
    }
    else {
        tile_idx = mapGetTile(x0 + 1, y0);
        after_hit = tileAfterHit(tile_idx, rotate, mod);

        if (after_hit >= 0) {
            tileReaction(bull, &after_hit, &kill_bull, &x0, &y0, 1, 0);
        }

    }

    if (kill_bull) {
        GLog_killBullet(bull, 1);
        if (bull->maker == &game_player[0] || bull->maker == &game_player[1]) {
            if (after_hit == 16) {
//                startPlaySample(snd_bull_stop, sizeof (snd_bull_stop), 11000, AUDIO_PAN_CENTER, 6);
                soundPlay(snd_bull_stop, sizeof (snd_bull_stop), SOUND_PCM_CH3, 0);
            } else {
//                startPlaySample(snd_bull_stop_briks, sizeof (snd_bull_stop_briks), 11000, AUDIO_PAN_CENTER, 3);
                soundPlay(snd_brick_hit, sizeof (snd_brick_hit), SOUND_PCM_CH3, 0);
            }
        }
    }


}

s16 tileAfterHit(u16 tile_idx, u16 bullet_rotate, u16 dip) {

    u16 mask;

    if (tile_idx == RES_TILE_ARMOR) return tile_idx;
    if (tile_idx == (RES_TILE_GRASS | TILE_ATTR(0, 1, 0, 0))) {
            return RES_TILE_GRASS;
    }
    else {
        if (tile_idx > 16 || tile_idx == 0) return -1;
    }

    if (bullet_rotate & 1) {
        mask = 10 >> dip;
    } else {
        mask = 12 >> (dip << 1);
    }



    if ((tile_idx & mask) == tile_idx) return -1;

    return tile_idx & mask;
}

void tileReaction(_bullet* bull, s16* after_hit, u8* kill_bull, s16* x0, s16* y0, u8 xShift, u8 yShift) {
    switch (*after_hit) {
        case RES_TILE_GRASS:
            if (bull->maker->uranium_bullets && bull->maker->grass_trim) {
                *after_hit = 0;
                mapSetTile(0, *x0 + xShift, *y0 + yShift);
            }
            break;
        case RES_TILE_ARMOR:

            if (mods.bul_ricochet) {
                if (!bull->ricocheted) {
                    bull->ricocheted = TRUE;
                    bull->rotate += 2;
                    if (bull->rotate > 3) bull->rotate = bull->rotate - 4;
                }
                else if (bull->ricocheted && !xShift && !yShift) {
                    *kill_bull = 1;
                }
            }
            else {
                if (bull->maker->uranium_bullets) {
                    *after_hit = 0;
                }
                *kill_bull = 1;

            }



            mapSetTile(*after_hit, *x0 + xShift, *y0 + yShift);
            break;
        default:
            mapSetTile(*after_hit, *x0 + xShift, *y0 + yShift);
            *kill_bull = 1;
            break;
    }
}

u8 moveAvailableInWalls(_tank *tank) {

    u16 r = tank->rotate;
    s16 x1 = tank->posx + speed_x[r];
    s16 y1 = tank->posy + speed_y[r];
    s16 x2 = (x1 + 15) >> 3;
    s16 y2 = (y1 + 15) >> 3;
    x1 >>= 3;
    y1 >>= 3;
    if (x1 < 0 || x2 >= MAP_W || y1 < 0 || y2 >= MAP_H) return 0;

    u16 tile_idx = 0;


    tile_idx = mapGetTile(x1, y1);

    if (tile_idx == RES_TILE_DIRT && tank->speed == 0) {
        tank->on_dirt = TRUE;
    }
    else {
        tank->on_dirt = FALSE;
    }

    if (tile_idx == RES_TILE_DIRT && tank->speed) {
        tank->dirt = 32;
    }
    else if (tile_idx != RES_TILE_DIRT) {
        tank->dirt = 0;
    }

	int i1 = 0;
	int i2 = 0;
	int i3 = 0;
	int i4 = 0;
	//, i2, i3, i4;
	tile_idx &= 0x1FF;
	if (tile_idx == 0 || tile_idx == 35 || tile_idx == 34 || tile_idx == 33 || (tank->ship == 1 && tile_idx == 18))i1 = 1;
	tile_idx = mapGetTile(x2, y1);
	tile_idx &= 0x1FF;
	if (tile_idx == 0 || tile_idx == 35 || tile_idx == 34 || tile_idx == 33 || (tank->ship == 1 && tile_idx == 18))i2 = 1;
	tile_idx = mapGetTile(x1, y2);
	tile_idx &= 0x1FF;
	if (tile_idx == 0 || tile_idx == 35 || tile_idx == 34 || tile_idx == 33 || (tank->ship == 1 && tile_idx == 18))i3 = 1;
	tile_idx = mapGetTile(x2, y2);
	tile_idx &= 0x1FF;
	if (tile_idx == 0 || tile_idx == 35 || tile_idx == 34 || tile_idx == 33 || (tank->ship == 1 && tile_idx == 18))i4 = 1;
	if(i1 == 1 && i2 == 1 && i3 == 1 && i4 == 1) return 1;



//	i1 = 0;
//	i2 = 0;
//	i3 = 0;
//	i4 = 0;
//
//    tile_idx = mapGetTile(x1, y1);
//	tile_idx &= 0x1FF;
//	if ((tile_idx >= 1) && (tile_idx <= 15)) i1 = 1;
//	tile_idx = mapGetTile(x2, y1);
//	tile_idx &= 0x1FF;
//    if ((tile_idx >= 1) && (tile_idx <= 15)) i2 = 1;
//	tile_idx = mapGetTile(x1, y2);
//	tile_idx &= 0x1FF;
//    if ((tile_idx >= 1) && (tile_idx <= 15)) i3 = 1;
//	tile_idx = mapGetTile(x2, y2);
//	tile_idx &= 0x1FF;
//    if ((tile_idx >= 1) && (tile_idx <= 15)) i4 = 1;
//	if(i1 == 1 && i2 == 1 && i3 == 1 && i4 == 1) return 1;




    if (mods.en_no_walls) {
        if (tank->player == 0) return 1;
        return 0;
    }
    else {
        return 0;
    }

}

void detectBulletToUnitsCollision(_bullet *bull) {

    u16 end = 2;
    u16 i = 0;
    s16 delta_x;
    s16 delta_y;
    _tank *player1 = &game_player[0];
    _tank *player2 = &game_player[1];
    //s16 gamer_bull_flag = 0;

    if ((config.en_friend_fire == KILL) || (config.en_friend_fire == BULLETS_STOP)){
            i = 0;
            end = config.units_on_map;
            //gamer_bull_flag = 1;
    }
    else {
        if (bull->maker == player1 || bull->maker == player2) {
            i = 0;
            end = config.units_on_map;
            //gamer_bull_flag = 1;
        }
    }
    for (; i < end; i++) {

//        if (!game_player[i].hitpoint || bull->maker == &game_player[i] || game_player[i].birth) continue;
        if (!game_player[i].hitpoint || game_player[i].birth) continue;
        if (mods.bul_ricochet) {
            if ((!bull->ricocheted) && (bull->maker == &game_player[i])) continue;
        }
        else {
            if (bull->maker == &game_player[i]) continue;
        }

        s16 bx = bull->posx;
        s16 by = bull->posy;

        if (bull->rotate == 3) {
            bx -= 9;
        } else if (bull->rotate == 2) {
            by -= 9;
        }

        delta_x = bx - game_player[i].posx;
        delta_y = by - game_player[i].posy;

        if (delta_x < 0) delta_x = -delta_x;
        if (delta_y < 0) delta_y = -delta_y;

        if (delta_x < 15 && delta_y < 15) {

            // detect player to player friendly fire
            if ((bull->maker == &game_player[0]) || (bull->maker == &game_player[1])) {
                if ((&game_player[i] == &game_player[0]) || (&game_player[i] == &game_player[1])) {
                    if (config.pl_friend_fire == OFF) {
                        continue;
                    }
                    else {
                        GLog_killBullet(bull, 1);
                    }
                }
                else {
                    GLog_killBullet(bull, 1);
                }
            }
            else {
                GLog_killBullet(bull, 1);
            }

            GLog_killPLayer(&game_player[i], bull->maker);
            return;
        }
    }

}

void detectBulletToStaffCollision(_bullet *bull) {

    u16 tile_idx = 0;
    s16 x = bull->posx;
    s16 y = bull->posy;
    x += speed_x[bull->rotate] << 1;
    y += speed_y[bull->rotate] << 1;

    s16 x0 = x >> 3;
    s16 y0 = y >> 3;
//    u16 tile_idx = mapGetTile(bull->posx >> 3, bull->posy >> 3);
    tile_idx = mapGetTile(x0, y0);
    if (tile_idx >= RES_TILE_STAFF && tile_idx <= RES_TILE_STAFF + 4) {
        GLog_killStaff();
        GLog_killBullet(bull, 1);
    }
}

void detectBulletToBulletCollision(_bullet *bull) {

    u16 i = config.max_bullets;
    _bullet *buff;
    s16 delta_x;
    s16 delta_y;
    if (bull->maker != &game_player[0] && bull->maker != &game_player[1]) {
        return;
    }

    while (i--) {
        buff = &bullets[i];
        if (!buff->speed || bull == buff)continue;
        delta_x = bull->posx - buff->posx;
        delta_y = bull->posy - buff->posy;
        if (delta_x < 0)delta_x = -delta_x;
        if (delta_y < 0)delta_y = -delta_y;
        if (delta_x < 6 && delta_y < 6) {
            GLog_killBullet(bull, 0);
            GLog_killBullet(buff, 0);
        }
    }
}

u8 detectBonusColision(u8 player) {

    s16 delta_x = game_player[player].posx - bonus.posx;
    s16 delta_y = game_player[player].posy - bonus.posy;
    if (delta_x < 0)delta_x = -delta_x;
    if (delta_y < 0)delta_y = -delta_y;

    if (delta_x < 16 && delta_y < 15)return 1;

    return 0;
}
