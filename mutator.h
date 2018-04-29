#ifndef _MUTATOR_H
#define	_MUTATOR_H



extern u8 show_message;


enum {
    EN_ARMOR = 1,
    EN_SPEED,
    EN_URANIUM
};



void initMods();
void showModText();



#endif	/* _MUTATOR_H */

