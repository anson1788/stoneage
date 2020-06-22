#ifndef __MOVE_H__
#define __MOVE_H__

#ifdef _TALK_MOVE_FLOOR
#define MAXTALKMOVEFLOOR 64
typedef struct TALK_MOVE_FLOOR_t{
	char name[32];	
	int id;	
	int x;
	int y;
	int trans;
	int level;
}Talk_Move_Floor_t;

BOOL MoveMap_init();
BOOL talk_move_map(int charaindex, char *messageeraseescape);

#endif
#endif
