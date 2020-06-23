#ifndef __REMEMOY_H_
#define __REMEMOY_H_
#ifdef _RED_MEMOY_
typedef struct
{
	int flg;
	int type;
	unsigned int time;
	int vip;
	char name[42];
	int index;
	int colortime;
}RED_MEMOY;
void RedMemoyCall();
extern RED_MEMOY redMemoy;
void RedMemoySetup();
void RedMemoyInit();
#endif
#endif


