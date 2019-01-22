
#include <genesis.h>
#include "gamerender.h"
#include "sprite.h"
#include "gamelogic.h"
#include "resmap.h"
#include "map.h"

static u16 river_strobe = 0;
static const s16 bullet_sprite_offset_x[4] = { 3+0, -4, 3+0, -3 };
static const s16 bullet_sprite_offset_y[4] = {  -8,  0,  -7,  0 };

void GRend_prepareRepaint() {

    u16 i, x, y;

    if (pause && (river_strobe & 16)) {
        x = (MAP_W >> 1 << 3) - 20;
        y = (MAP_H >> 1 << 3) + 1;
        drawSprite4x1(SPRITE_PAUSE | TILE_ATTR(0, 1, 0, 0), x, y);             // PAUS
        drawSprite1x1((SPRITE_PAUSE + 4) | TILE_ATTR(0, 1, 0, 0), x + 8*4, y); // E
    }

    if (scor.timer) {
        if (!pause)
            scor.timer--;
        drawSprite2x2(SPRITE_ADDR_SCORE + (scor.val << 2), scor.posx, scor.posy);
    }

    for (i = 0; i < config.max_bullets; i++) {
        if (!bullets[i].speed)
            continue;
        drawSprite1x2(SPRITE_ADDR_BULLET + (bullets[i].rotate << 1),
            bullets[i].posx + bullet_sprite_offset_x[bullets[i].rotate],
            bullets[i].posy + bullet_sprite_offset_y[bullets[i].rotate]);
    }

    i = config.max_explode;
    while (i--) {
        if (!explodes[i].type)
            continue;
        if (explodes[i].ani_counter > 2) {
            drawSprite8x8(SPRITE_EXPLODE_BIG + ((explodes[i].ani_counter - 3) << 4), explodes[i].posx - 8, explodes[i].posy - 8);
            if (explodes[i].strobe & 8) {
                explodes[i].ani_counter++;
                explodes[i].strobe = 0;
            }
        }
        else {
            drawSprite2x2(SPRITE_EXPLODE_SMALL + (explodes[i].ani_counter << 2), explodes[i].posx, explodes[i].posy);
            if (explodes[i].strobe & 2) {
                explodes[i].ani_counter++;
                explodes[i].strobe = 0;
            }
        }
        if (explodes[i].type == EXPLODE_BIG   && explodes[i].ani_counter > 4) explodes[i].type = 0;
        if (explodes[i].type == EXPLODE_SMALL && explodes[i].ani_counter > 2) explodes[i].type = 0;
        if (!pause)
            explodes[i].strobe++;
    }

    for (i = 0; i < config.units_on_map; i++) {
        drawTank(&game_player[i]);
    }

    if (bonus.type) {
        drawBonus(&bonus);
    }
}

void GRend_repaint() {

    updateSprite();
    river_strobe++;
    if (!pause) {
        if ((river_strobe & 63) == 0) {
            VDP_setPaletteColor(RES_COLOR_RIVER2_ADDR, pal_red[RES_COLOR_RIVER1_ADDR]);
            VDP_setPaletteColor(RES_COLOR_RIVER3_ADDR, pal_red[RES_COLOR_RIVER2_ADDR]);
        }
        else if ((river_strobe & 63) == 32) {
            VDP_setPaletteColor(RES_COLOR_RIVER3_ADDR, pal_red[RES_COLOR_RIVER1_ADDR]);
            VDP_setPaletteColor(RES_COLOR_RIVER2_ADDR, pal_red[RES_COLOR_RIVER2_ADDR]);
        }
    }
}
