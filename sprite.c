#include <genesis.h>
#include "sprite.h"
#include "map.h"
#include "resmap.h"

#define TILE_PAL(pal)((pal << 13))

VDPSprite sprites[SPRITE_TOTAL_NUM];
//_spritedef sprites[SPRITE_TOTAL_NUM];
u16 sprite_counter;
u16 color_strobe;
u8 ani_counter;

u16 spr_kl;

void spriteInit() {

    ani_counter = 0;
    color_strobe = 0;
    sprite_counter = 0;
    VDP_resetSprites();
    VDP_updateSprites(1, TRUE);
}

void drawSprite2x2(u16 sprite_atr, s16 x, s16 y) {

    VDP_setSpriteFull(sprite_counter, x + MAP_PIXEL_X, y + MAP_PIXEL_Y, SPRITE_SIZE(2, 2), sprite_atr, sprite_counter+1);
    sprite_counter++;
}

void drawSprite8x8(u16 sprite_atr, s16 x, s16 y) {

    VDP_setSpriteFull(sprite_counter, x + MAP_PIXEL_X, y + MAP_PIXEL_Y, SPRITE_SIZE(8, 8), sprite_atr, sprite_counter+1);
    sprite_counter++;
}

void drawSprite1x2(u16 sprite_atr, s16 x, s16 y) {

    VDP_setSpriteFull(sprite_counter, x + MAP_PIXEL_X, y + MAP_PIXEL_Y, SPRITE_SIZE(1, 2), sprite_atr, sprite_counter+1);
    sprite_counter++;
}

void drawSprite1x1(u16 sprite_atr, s16 x, s16 y) {

    VDP_setSpriteFull(sprite_counter, x + MAP_PIXEL_X, y + MAP_PIXEL_Y, SPRITE_SIZE(1, 1), sprite_atr, sprite_counter+1);
    sprite_counter++;
}

void drawSprite4x1(u16 sprite_atr, s16 x, s16 y) {

    VDP_setSpriteFull(sprite_counter, x + MAP_PIXEL_X, y + MAP_PIXEL_Y, SPRITE_SIZE(4, 1), sprite_atr, sprite_counter+1);
    sprite_counter++;
}

void updateSprite() {

    ani_counter++;
    color_strobe++;

    VDP_setSpriteFull(sprite_counter, -8, -8, SPRITE_SIZE(1, 1), 0, sprite_counter+1);
    sprite_counter++;

    if (sprite_counter < spr_kl) {
        u16 ttt;
        u16 spr_kl2 = sprite_counter;

        for (ttt = 0; ttt < (spr_kl - spr_kl2); ttt++) {
            VDP_setSpriteFull(sprite_counter, -8, -8, SPRITE_SIZE(1, 1), 0, sprite_counter+1);
            sprite_counter++;
        }
        spr_kl = spr_kl2;
    }

//    char a[22];
//    intToStr(sprite_counter, a, 1);
//    VDP_drawText(a, 0, 2);

    if (sprite_counter > spr_kl) spr_kl = sprite_counter;

    VDP_updateSprites(sprite_counter, TRUE);

    sprite_counter = 0;
}

void drawTank(_tank *tank) {

    u16 color;
    u16 attr;

    if (!tank->hitpoint)
        return;
    if (tank->freeze && (ani_counter & 31) < 16)
        return;

    if (tank->birth) {
        s16 b = (tank->birth >> 2) % 6;
        if (b > 3) b = 6 - b; // Ping Pong
        b = 3 - b; // Max star if (tank->birth / 4) == 0
        drawSprite2x2(SPRITE_ADDR_BIRTH + (b << 2), tank->posx, tank->posy);
        return;
    }

    if (tank->god) {
        drawSprite2x2(SPRITE_ADDR_ARMOR + ((tank->god >> 1 & 1) << 2), tank->posx, tank->posy);
    }

    color = tank->color;
    if (tank->bonus) {
        color = TANK_COLOR_GREY;
        if ((color_strobe & 0xf) < 8)
            color = TANK_COLOR_RED;
    }
    else {
        if (tank->hitpoint > 1 && (color_strobe & 1)) {
            color = tank->hitpoint == 4 ? TANK_COLOR_GREEN : TANK_COLOR_YELLOW;
        }
        if (tank->hitpoint == 2 && !(color_strobe & 1))
            color = TANK_COLOR_GREEN;
    }

    attr = SPRITE_ADDR_TANK | TILE_PAL(color);
    attr += tank->type << 5;
    attr += tank->rotate << 3;
    if (tank->speed && !pause)
        attr += ((ani_counter >> 2) & 1) << 2;
    
    if (tank->ship == 1) {
        drawSprite2x2(SPRITE_ADDR_SHIP | TILE_PAL(color), tank->posx, tank->posy);
    }

    drawSprite2x2(attr, tank->posx, tank->posy);
}

void drawBonus(_bonus *bonus) {

    if ((ani_counter & 15) < 8)
        return;

    drawSprite2x2((SPRITE_BONUS + ((bonus->type - 1) << 2)) | TILE_ATTR(PAL3, 1, 0, 0), bonus->posx, bonus->posy);
}
