#include "genesis.h"
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
//spr_kl=0;

    ani_counter = 0;
    color_strobe = 0;
	sprite_counter = 0;
	VDP_resetSprites();
	VDP_updateSprites(1,1);
	//VDP_updateSprites();
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

void drawSpritePixel(u16 sprite_atr, s16 x, s16 y) {

	VDP_setSpriteFull(sprite_counter, x + MAP_PIXEL_X, y + MAP_PIXEL_Y, 0, sprite_atr, sprite_counter+1);
	sprite_counter++;

}

void updateSprite() {

    ani_counter++;
	color_strobe++;

	VDP_setSpriteFull(sprite_counter, -8, -8, SPRITE_SIZE(1, 1), 0, sprite_counter+1);
	sprite_counter++;

	if (sprite_counter < spr_kl) {
        int ttt;
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

    VDP_refreshHighestAllocatedSpriteIndex();


    VDP_updateSprites(sprite_counter, 1);

	sprite_counter = 0;

}

void drawTank(_tank *sprite) {

    u16 color;
    u16 attr;

    if (!sprite->hitpoint)return;
    if (sprite->freeze && (ani_counter & 31) < 16)return;


//        drawSpritePixel(RES_TILE_DIRT | TILE_PAL(color), sprite->posx, sprite->posy);


    if (sprite->birth) {
        drawSprite2x2(SPRITE_ADDR_BIRTH + ((sprite->birth >> 2 & 3) << 2), sprite->posx, sprite->posy);
        return;
    }

    if (sprite->god) {
        drawSprite2x2(SPRITE_ADDR_ARMOR + ((sprite->god >> 1 & 1) << 2), sprite->posx, sprite->posy);

    }

	if (sprite->ship == 1) {
        drawSprite2x2(800+8*4, sprite->posx, sprite->posy);

    }

    color = sprite->color;
    if (sprite->bonus) {
        color = TANK_COLOR_GREY;
        if ((color_strobe & 0xf) < 8)color = TANK_COLOR_RED;
    }
    else {
        if (sprite->hitpoint > 1 && (color_strobe & 1)) {

            color = sprite->hitpoint == 4 ? TANK_COLOR_GREEN : TANK_COLOR_YELLOW;
        }
        if (sprite->hitpoint == 2 && !(color_strobe & 1))color = TANK_COLOR_GREEN;

    }


	attr = SPRITE_ADDR_TANK | TILE_PAL(color);
    attr += sprite->type << 5;
    attr += sprite->rotate << 3;
    if (sprite->speed && !pause)attr += ((ani_counter >> 2) & 1) << 2;


//	VDP_setSpriteFull(sprite_counter++, sprite->posx + MAP_PIXEL_X, sprite->posy + MAP_PIXEL_Y, SPRITE_SIZE(2, 2), attr, sprite_counter+1);

    drawSprite2x2(attr, sprite->posx, sprite->posy);

}

void drawBonus(_bonus *bonus) {

    if ((ani_counter & 15) < 8)return;

    drawSprite2x2((SPRITE_BONUS + ((bonus->type - 1) << 2)) | TILE_ATTR(0, 1, 0, 0), bonus->posx, bonus->posy);
}
