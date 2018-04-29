/*
 * File:   menu.h
 * Author: KRIK
 *
 * Created on 8 Èþëü 2009 ã., 10:37
 */

#ifndef _MENU_H
#define	_MENU_H

#define ITEM_ONE_PLAYER 0
#define ITEM_TWO_PLAYER 1
#define ITEM_MAP_EDITOR 2
#define ITEM_MAP_OPTION 3

void startMenu(s8 do_scroll);
void modeMenu();
s8 menuGetSelectedItem();

#endif	/* _MENU_H */

