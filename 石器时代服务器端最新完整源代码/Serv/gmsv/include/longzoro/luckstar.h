#ifndef __LUCK_STAR_H__
#define __LUCK_STAR_H__

#ifdef _LUCK_STAR
#define MAXLUCKSTAR 128
typedef struct LUCK_STAR_t{
	int pet;	
	int item;
//	int gold;
//	int vippoint;
}Luck_Star_t;

BOOL LuckStar_init();

void LuckStar();


#endif
#endif
