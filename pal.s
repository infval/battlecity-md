.globl pal_menu
pal_menu:
    dc.w    0xf000
    dc.w    0x13c
    dc.w    0xfff
    dc.w    0x13c
    dc.w    0xccc
    dc.w    0x888
    dc.w    0xf23
    dc.w    0xffa
    dc.w    0xf95
    dc.w    0x07c
    dc.w    0x13c
    dc.w    0xfff
    dc.w    0x230
    dc.w    0x0d6
    dc.w    0x730
    dc.w    0x404

.globl pal_red
pal_red:
    dc.w    0x000
    dc.w    0x006
    dc.w    0x666
    dc.w    0x14a
    dc.w    0xccc
    dc.w    0x888
    dc.w    0xf23
    dc.w    0xffa
    dc.w    0xf95
    dc.w    0x07c
    dc.w    0x13c
    dc.w    0xfff
    dc.w    0x230
    dc.w    0x0d6
    dc.w    0x730
    dc.w    0x404

.globl pal_green
pal_green:
    dc.w    0x000
    dc.w    0x006
    dc.w    0x666
    dc.w    0x03a
    dc.w    0xccc
    dc.w    0x888
    dc.w    0xf23
    dc.w    0xffa
    dc.w    0xf95
    dc.w    0x07c
    dc.w    0x480
    dc.w    0xcfb
    dc.w    0x230
    dc.w    0x0d6
    dc.w    0x730
    dc.w    0x030

.globl pal_yellow
pal_yellow:
    dc.w    0x000
    dc.w    0x39d
    dc.w    0x666
    dc.w    0x03a
    dc.w    0xccc
    dc.w    0x888
    dc.w    0xf23
    dc.w    0xffa
    dc.w    0xf95
    dc.w    0x07c
    dc.w    0x39d
    dc.w    0x9ff
    dc.w    0x230
    dc.w    0x0d6
    dc.w    0x730
    dc.w    0x067

.globl pal_grey
pal_grey:
    dc.w    0x000;
    dc.w    0x006
    dc.w    0x666
    dc.w    0x03a
    dc.w    0xccc
    dc.w    0x888
    dc.w    0xf23
    dc.w    0xffa
    dc.w    0xf95
    dc.w    0x07c
    dc.w    0xccc;
    dc.w    0xfff
    dc.w    0x230
    dc.w    0x0d6
    dc.w    0x730
    dc.w    0x730;

.globl pal_logo
pal_logo:
    dc.w    0xf00
    dc.w    0xf20
    dc.w    0xf40
    dc.w    0xf60
    dc.w    0xf80
    dc.w    0xfa0
    dc.w    0xfc0
    dc.w    0xfe0
    dc.w    0xff0
    dc.w    0xfe0
    dc.w    0xfc0
    dc.w    0xfa0
    dc.w    0xf80
    dc.w    0xf60
    dc.w    0xf40
    dc.w    0xf20

.globl pal_orange_text
pal_orange_text:
    dc.w    0x000
    dc.w    0x006
    dc.w    0x666
    dc.w    0x14a
    dc.w    0xccc
    dc.w    0x888
    dc.w    0xf23
    dc.w    0xffa
    dc.w    0xf95
    dc.w    0x07c
    dc.w    0x13c
    dc.w    0x39d
    dc.w    0x230
    dc.w    0x0d6
    dc.w    0x730
    dc.w    0x404

.globl pal_red_text
pal_red_text:
    dc.w    0x000
    dc.w    0x006
    dc.w    0x666
    dc.w    0x14a
    dc.w    0xccc
    dc.w    0x888
    dc.w    0xf23
    dc.w    0xffa
    dc.w    0xf95
    dc.w    0x07c
    dc.w    0x13c
    dc.w    0x13c
    dc.w    0x230
    dc.w    0x0d6
    dc.w    0x730
    dc.w    0x404

.globl pal_info_screen
pal_info_screen:

    dc.w    0x000
    dc.w    0x0f0
    dc.w    0x200
    dc.w    0x000
    dc.w    0x000
    dc.w    0x000
    dc.w    0x000
    dc.w    0x000
    dc.w    0x000
    dc.w    0x000
    dc.w    0x000
    dc.w    0x000
    dc.w    0x000
    dc.w    0x000
    dc.w    0x000
    dc.w    0xfff 
