#include <stdio.h>
#include <stdlib.h>

/* WIN32_LEAN_AND_MEAN?define???WINDOWS.H???
???????????????????????
????????????????? */
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <Mmsystem.h>
#include <math.h>

#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/action.h"
#include "../systeminc/map.h"
#include "../systeminc/pattern.h"
#include "../systeminc/loadrealbin.h"
#include "../systeminc/loadsprbin.h"
#include "../systeminc/sprdisp.h"
#include "../systeminc/math2.h"

#include "../systeminc/character.h"
#include "../systeminc/pc.h"
#include "../systeminc/menu.h"
#include "../systeminc/main.h"
#include "../systeminc/anim_tbl.h"
#include "../systeminc/chat.h"

extern BOOL g_bUseAlpha;

// ?????????
enum
{
	CHAROBJ_USE_FREE,
	CHAROBJ_USE_STAY,
	CHAROBJ_USE_VIEW
};
// Robin
//#define MAX_CHAROBJ	1000
#define MAX_CHAROBJ	1500
CHAROBJ charObj[MAX_CHAROBJ];
int maxCharObj;		// ??
int tailCharObj;	// ????????
int freeCharObj;	// ???????
int searchCharObj;	// ???

#ifdef _AniCharBubble	   // Syu ADD 动画层人物吐出气泡
#define RAND(x,y)   ((x-1)+1+ (int)( (double)(y-(x-1))*rand()/(RAND_MAX+1.0)) )
bool popflag = false;		//人物产生气泡与否旗标
bool waittimeflag = false;  //气泡结束后等待时间旗标
int waittime = 0 ;			//气泡结束后等待时间
static ACTION *popAct;		//气泡动画
#endif

// ???????????????
int charIconOffsetY[12] =
{
	-71, -88, -84, -88, -96, -102, -88, -88, -88, -85, -85, -88
};


// ???????
int battleIconTbl[][2] =
{
	{  CG_VS_MARK_1A,  CG_VS_MARK_1B },
	{  CG_VS_MARK_2A,  CG_VS_MARK_2B },
	{  CG_VS_MARK_3A,  CG_VS_MARK_3B },
	{  CG_VS_MARK_4A,  CG_VS_MARK_4B },
	{  CG_VS_MARK_5A,  CG_VS_MARK_5B },
	{  CG_VS_MARK_6A,  CG_VS_MARK_6B },
	{  CG_VS_MARK_7A,  CG_VS_MARK_7B },
	{  CG_VS_MARK_8A,  CG_VS_MARK_8B },
	{  CG_VS_MARK_9A,  CG_VS_MARK_9B },
	{ CG_VS_MARK_10A, CG_VS_MARK_10B }
};

#ifdef _NPC_MAGICCARD
struct Posstruct Positiontable[]=
{	
	{19,12,17,12,18,11},	//0 玩家
	{18,17,16,15,17,14},	//1 玩家
	{13,18,13,16,14,15},	//2 玩家
	{ 9,14,11,14,12,13},	//3 玩家
	{10, 9,12,11,13,10},	//4 玩家
	{15, 8,15,10,16, 9},	//5 玩家
	{15,12,14,13,15,12},	//6 庄家
/*
	{23, 9,21, 9,22, 8},	//0 玩家
	{22,14,20,12,21,11},	//1 玩家
	{17,15,17,13,18,12},	//2 玩家
	{13,11,15,11,16,10},	//3 玩家
	{14, 6,16, 8,17, 7},	//4 玩家
	{19, 5,19, 7,20, 6},	//5 玩家
	{18,10,18,10,19, 9},	//6 庄家	
*/
};

int offsetYtbl[60]={
	 20,38,54,68,80,104,104,104,104,104,104,104,104,104,104,104, 
	 104,104,104,104,104, 104,104,104,104,104,
	 104,104,104,104,104, 104,104,104,104,104,
	 104,104,104,104,104, 104,104,104,104,104,
	 104,104,104,104,104, 104,104,104,104,104,104,104,104,104,
};

int fonttbl[60]={
	 0,0,0,0,0,0,0,0,0,0,
	 1,2,3,4,5,6,7,8,9,10,
	 11,12,13,14,15,16,17,18,19,20,
	 21,22,23,24,25,26,27,28,29,30,
	 31,32,33,34,35,36,37,38,39,40,
	 40,40,40,40,40,40,40,40,40,40,

};
#endif
/*
	{23, 9,21, 9,22, 8, 5,-1},	//0 玩家
	{22,14,20,12,21,11, 4, 4},//1 玩家
	{17,15,17,13,18,12,-1, 5},	//2 玩家
	{13,11,15,11,16,10,-5, 1},	//3 玩家
	{18,10,16, 8,17, 7,-4,-4},	//4 玩家
	{19, 5,19, 7,20, 6, 1,-5},	//5 玩家
	{18,18,18,10,19, 9, 0, 0},	//6 庄家	
*/

short nameOverTheHeadFlag = 1;

void charProc( ACTION * );

// ????????????????
void charProc( ACTION *ptAct )
{
	float mx, my;
	int animLoop;
	int no;
	CHAREXTRA *ext;
	int pFlag = 0;
	// ???
	// PC???????????
	// PC??????????????????
	if( pc.ptAct != ptAct ){
		ext = (CHAREXTRA *)ptAct->pYobi;
		no = ext->charObjTblId;
		// ???????????????????
		if( (charObj[no].status & CHR_STATUS_PARTY) == 0 ){
			charMove( ptAct );
			pFlag = 0;
		}else
			pFlag = 1;
		if( ptAct->vx == 0 && ptAct->vy == 0 && charObj[no].stockDir != -1
		 && charObj[no].stockDirX == ptAct->gx && charObj[no].stockDirY == ptAct->gy ){
			ptAct->anim_ang = charObj[no].stockDir;
			charObj[no].stockDir = -1;
			charObj[no].stockDirX = 0;
			charObj[no].stockDirY = 0;
		}
	}
	// ??
#ifdef _MOVE_SCREEN
	if (pc.bMoveScreenMode)
		camMapToGamen(ptAct->mx - iScreenMoveX * GRID_SIZE, ptAct->my - iScreenMoveY * GRID_SIZE, &mx, &my);
	else
#endif
	camMapToGamen( ptAct->mx, ptAct->my, &mx, &my );
	
	ptAct->x = (int)(mx+.5);
	ptAct->y = (int)(my+.5);
	// ??????????
#ifdef _NPC_PICTURE
#endif
	drawCharStatus( ptAct );
	// ?????????
	if( ptAct->anim_no == ANIM_HAND
	 || ptAct->anim_no == ANIM_HAPPY
	 || ptAct->anim_no == ANIM_ANGRY
	 || ptAct->anim_no == ANIM_SAD
	 || ptAct->anim_no == ANIM_WALK
	 || ptAct->anim_no == ANIM_STAND
	 || ptAct->anim_no == ANIM_NOD )
		animLoop = ANM_LOOP;
	else
		animLoop = ANM_NO_LOOP;
	pattern( ptAct, ANM_NOMAL_SPD, animLoop );
	if( pFlag ){
		if( nowSpdRate >= 1.2F )
			pattern( ptAct, ANM_NOMAL_SPD, animLoop );
		if( nowSpdRate >= 1.6F )
			pattern( ptAct, ANM_NOMAL_SPD, animLoop );
	}else{
		if( ptAct->bufCount >= 2 )
			pattern( ptAct, ANM_NOMAL_SPD, animLoop );
		if( ptAct->bufCount >= 4 )
			pattern( ptAct, ANM_NOMAL_SPD, animLoop );
	}
	if( pc.ptAct != ptAct ){
		if( charObj[no].newFoundFlag ){
			S2 xx, yy, ww, hh;
			// ???????
			realGetPos( ptAct->bmpNo, &xx, &yy );
			realGetWH( ptAct->bmpNo, &ww, &hh );
			xx += ptAct->x;
			yy += ptAct->y;
			if( 0 <= xx && xx+ww <= DEF_APPSIZEX
			 && 0 <= yy && yy+hh <= DEF_APPSIZEY ){
				CheckNewPet( ptAct->anim_chr_no );
				charObj[no].newFoundFlag = 0;
			}
		}
	}
	if( 20000 <= ptAct->anim_chr_no && ptAct->anim_chr_no <= 24999 ){
		// ????
		if( pc.ptAct != ptAct ){
			// ??????????
			if( ptAct->gx < nowGx-16 || nowGx+16 < ptAct->gx
			 || ptAct->gy < nowGy-16 || nowGy+16 < ptAct->gy ){
				delCharObj( charObj[no].id );
				return;
			}
		}
		// ??????????
		if( !itemOverlapCheck( ptAct->bmpNo, ptAct->gx, ptAct->gy ) ){
			// ??????????????
			setCharPrio( ptAct->bmpNo, ptAct->x, ptAct->y, 0, 0, ptAct->mx, ptAct->my
#ifdef _SFUMATO
				, ptAct->sfumato
#endif
				);
			ptAct->atr &= (~ACT_ATR_HIDE);
		}else
			ptAct->atr |= ACT_ATR_HIDE;
	}else{
		// ????????
		// ??????????????
		setCharPrio( ptAct->bmpNo, ptAct->x, ptAct->y, 0, 0, ptAct->mx, ptAct->my
#ifdef _SFUMATO
			, ptAct->sfumato
#endif
 );
	}
}
//水世界Action套用Char的Action修改
#ifdef _WATERANIMATION //Syu ADD 泪之海动画层
void waterAniProc( ACTION *ptAct )
{
	float mx, my;
	int animLoop;
	int no;
	CHAREXTRA *ext;
	int pFlag;
	// ???
	// PC???????????
	// PC??????????????????
	if( pc.ptAct != ptAct ){
		ext = (CHAREXTRA *)ptAct->pYobi;
		no = ext->charObjTblId;
		// ???????????????????
		if( (charObj[no].status & CHR_STATUS_PARTY) == 0 ){
			charMove( ptAct );
			pFlag = 0;
		}else
			pFlag = 1;
		if( ptAct->vx == 0 && ptAct->vy == 0 && charObj[no].stockDir != -1
		 && charObj[no].stockDirX == ptAct->gx && charObj[no].stockDirY == ptAct->gy ){
			ptAct->anim_ang = charObj[no].stockDir;
			charObj[no].stockDir = -1;
			charObj[no].stockDirX = 0;
			charObj[no].stockDirY = 0;
		}
	}
	// ??
	camMapToGamen( ptAct->mx, ptAct->my, &mx, &my );
	ptAct->x = (int)(mx+.5);
	ptAct->y = (int)(my+.5);
	// ??????????
	//  不取消掉人物状态会被复制到其他ACTION
	//	drawCharStatus( ptAct );
	// ?????????
	if( ptAct->anim_no == ANIM_HAND
	 || ptAct->anim_no == ANIM_HAPPY
	 || ptAct->anim_no == ANIM_ANGRY
	 || ptAct->anim_no == ANIM_SAD
	 || ptAct->anim_no == ANIM_WALK
	 || ptAct->anim_no == ANIM_STAND
	 || ptAct->anim_no == ANIM_NOD )
		animLoop = ANM_LOOP;
	else
		animLoop = ANM_NO_LOOP;
	pattern( ptAct, ANM_NOMAL_SPD, animLoop );
	if( pFlag ){
		if( nowSpdRate >= 1.2F )
			pattern( ptAct, ANM_NOMAL_SPD, animLoop );
		if( nowSpdRate >= 1.6F )
			pattern( ptAct, ANM_NOMAL_SPD, animLoop );
	}else{
		if( ptAct->bufCount >= 2 )
			pattern( ptAct, ANM_NOMAL_SPD, animLoop );
		if( ptAct->bufCount >= 4 )
			pattern( ptAct, ANM_NOMAL_SPD, animLoop );
	}
	if( pc.ptAct != ptAct ){
		if( charObj[no].newFoundFlag ){
			S2 xx, yy, ww, hh;
			// ???????
			realGetPos( ptAct->bmpNo, &xx, &yy );
			realGetWH( ptAct->bmpNo, &ww, &hh );
			xx += ptAct->x;
			yy += ptAct->y;
			if( 0 <= xx && xx+ww <= DEF_APPSIZEX
			 && 0 <= yy && yy+hh <= DEF_APPSIZEY ){
				CheckNewPet( ptAct->anim_chr_no );
				charObj[no].newFoundFlag = 0;
			}
		}
	}
	if( 20000 <= ptAct->anim_chr_no && ptAct->anim_chr_no <= 24999 ){
		// ????
		if( pc.ptAct != ptAct ){
			// ??????????
			if( ptAct->gx < nowGx-16 || nowGx+16 < ptAct->gx
			 || ptAct->gy < nowGy-16 || nowGy+16 < ptAct->gy ){
				delCharObj( charObj[no].id );
				return;
			}
		}
		// ??????????
		if( !itemOverlapCheck( ptAct->bmpNo, ptAct->gx, ptAct->gy ) ){
			// ??????????????
#ifdef _SPECIALSPACEANIM_FIX  //ROG ADD 修改图层
			setPartsPrio( ptAct->bmpNo, ptAct->x, ptAct->y, 0, 0, ptAct->mx, ptAct->my,ptAct->dispPrio);
#else
			setCharPrio( ptAct->bmpNo, ptAct->x, ptAct->y, 0, 0, ptAct->mx, ptAct->my
#ifdef _SFUMATO
				, ptAct->sfumato
#endif
 );
#endif
			ptAct->atr &= (~ACT_ATR_HIDE);
		}else
			ptAct->atr |= ACT_ATR_HIDE;
	}else{
		// ????????
		// ??????????????
#ifdef _SPECIALSPACEANIM_FIX  //ROG ADD 修改图层
		setPartsPrio( ptAct->bmpNo, ptAct->x, ptAct->y, 0, 0, ptAct->mx, ptAct->my,ptAct->dispPrio);
#else
		setCharPrio( ptAct->bmpNo, ptAct->x, ptAct->y, 0, 0, ptAct->mx, ptAct->my
#ifdef _SFUMATO
			, ptAct->sfumato
#endif
 );
#endif
	}
}
/* graNo	 Spr图号
   gx		 产生的x座标
   gy	  	 产生的y座标
   dispprio  图案的覆盖顺序
*/

ACTION *createWaterAnimation( int graNo, int gx, int gy, int dispprio )
{

	ACTION *ptAct;
	float mx, my;
	ptAct = GetAction( DISP_PRIO_BOX3, sizeof( CHAREXTRA ) );
//	ptAct = GetAction( PRIO_JIKI, NULL );
	if( ptAct == NULL )	return NULL;
	ptAct->func = waterAniProc;
	ptAct->anim_chr_no = graNo;
	ptAct->anim_no = ANIM_STAND;
	ptAct->anim_ang = 1;
	ptAct->dispPrio = dispprio;
	ptAct->atr |= ACT_ATR_HIT;
	ptAct->nextGx = gx;	
	ptAct->nextGy = gy;
	ptAct->bufCount = 0;
	ptAct->gx = gx;
	ptAct->gy = gy;
	ptAct->mx = (float)gx * GRID_SIZE;	
	ptAct->my = (float)gy * GRID_SIZE;
	ptAct->vx = 0;			
	ptAct->vy = 0;
	camMapToGamen( ptAct->mx, ptAct->my, &mx, &my );
	ptAct->x = (int)(mx+.5);
	ptAct->y = (int)(my+.5);
	pattern( ptAct, ANM_NOMAL_SPD, ANM_NO_LOOP );
	return ptAct;
}

#endif
// ????????????
ACTION *createCharAction( int graNo, int gx, int gy, int dir )
{
	ACTION *ptAct;
	float mx, my;
	/* ?????????? */
	ptAct = GetAction( PRIO_CHR, sizeof( CHAREXTRA ) );
	if( ptAct == NULL )
		return NULL;
	// ???
	ptAct->func = charProc;
	// ????????
	ptAct->anim_chr_no = graNo;
	// ??
	ptAct->anim_no = ANIM_STAND;
	// ?????????( ??? )( ??????? )
	ptAct->anim_ang = dir;
	// ?
//	ptAct->dispPrio =	DISP_PRIO_BOX3;
	ptAct->dispPrio = DISP_PRIO_CHAR;
	// 1????????
	ptAct->atr = ACT_ATR_INFO |	ACT_ATR_HIT | ACT_ATR_HIDE2;
	// ???
	ptAct->nextGx = gx;					// ???????????
	ptAct->nextGy = gy;
	ptAct->bufCount = 0;
	ptAct->gx = gx;						// ???????????佋?
	ptAct->gy = gy;
	ptAct->mx = (float)gx * GRID_SIZE;	// ????
	ptAct->my = (float)gy * GRID_SIZE;
	ptAct->vx = 0;						// ??
	ptAct->vy = 0;
	// ??
	camMapToGamen( ptAct->mx, ptAct->my, &mx, &my );
	ptAct->x = (int)(mx+.5);
	ptAct->y = (int)(my+.5);
	return ptAct;
}

// ??????????????
void stockCharMovePoint( ACTION *ptAct, int nextGx, int nextGy )
{
	if( ptAct == NULL )
		return;
	// ??????????????
	if( ptAct->bufCount < sizeof( ptAct->bufGx )/sizeof( int ) ){
		ptAct->bufGx[ptAct->bufCount] = nextGx;
		ptAct->bufGy[ptAct->bufCount] = nextGy;
		ptAct->bufCount++;
	}else{
	// ???????????????
		ptAct->bufCount = 0;
		setCharWarpPoint( ptAct, nextGx, nextGy );
	}
}

// ??????????????????????
//?????????????????????????
void correctCharMovePoint( ACTION *ptAct, int nextGx, int nextGy )
{
	int dx, dy;
	int nGx[2], nGy[2], nCnt = 0;
	int i;
	int preCnt;
	int nextGx2, nextGy2;
	if( ptAct == NULL )
		return;
	// ???????
	// ??????????????next????
	if( ptAct->bufCount <= 0 ){
		nextGx2 = ptAct->nextGx;
		nextGy2 = ptAct->nextGy;
	}else{
		preCnt = ptAct->bufCount - 1;
		nextGx2 = ptAct->bufGx[preCnt];
		nextGy2 = ptAct->bufGy[preCnt];
	}
	dx = nextGx - nextGx2;
	dy = nextGy - nextGy2;
	if( ABS( dx ) == 2 && ABS( dy ) == 2 ){
		nGx[nCnt] = nextGx2+dx/2;
		nGy[nCnt] = nextGy2+dy/2;
		nCnt++;
	}else if( ABS( dx ) == 2 ){
		nGx[nCnt] = nextGx2+dx/2;
		nGy[nCnt] = nextGy;
		nCnt++;
	}else if( ABS( dy ) == 2 ){
		nGx[nCnt] = nextGx;
		nGy[nCnt] = nextGy2+dy/2;
		nCnt++;
	}
	nGx[nCnt] = nextGx;
	nGy[nCnt] = nextGy;
	nCnt++;
	// ??????????????
	if( ptAct->bufCount+nCnt <= sizeof( ptAct->bufGx )/sizeof( int ) ){
		for( i = 0; i < nCnt; i++ ){
			ptAct->bufGx[ptAct->bufCount] = nGx[i];
			ptAct->bufGy[ptAct->bufCount] = nGy[i];
			ptAct->bufCount++;
		}
	}else{
	// ???????????????
		ptAct->bufCount = 0;
		nCnt--;
		setCharWarpPoint( ptAct, nGx[nCnt], nGy[nCnt] );
	}
}

// ?????
void setCharMovePoint( ACTION *ptAct, int nextGx, int nextGy )
{
	float dir1;
	int dir;
	float dx, dy;
	float len;
	float rate = 1.0F;
	if( ptAct == NULL )
		return;
	if( ptAct->bufCount > 5 )
		rate = 2.0F;
	else if( ptAct->bufCount >= 4 )
		rate = 1.6F;
	else if( ptAct->bufCount >= 2 )
		rate = 1.2F;
	dx = nextGx*GRID_SIZE-ptAct->mx;
	dy = nextGy*GRID_SIZE-ptAct->my;
	len = (float)sqrt( (double)(dx*dx+dy*dy) );
	if( len > 0 ){
		dx /= len;
		dy /= len;
	}else{
		dx = 0;
		dy = 0;
	}
	ptAct->vx = dx * MOVE_SPEED * rate;
	ptAct->vy = dy * MOVE_SPEED * rate;
	ptAct->nextGx = nextGx;
	ptAct->nextGy = nextGy;
	if( dx != 0 || dy != 0 ){
		dir1 = Atan( dx, dy ) + 22.5F;
		AdjustDir( &dir1 );
		dir = (int)(dir1/45);
		ptAct->anim_ang = dir;
		ptAct->walkFlag = 1;
	}
}

void _setCharMovePoint( ACTION *ptAct, int nextGx, int nextGy )
{
	float dir1;
	int dir;
	float dx, dy;
	float len;
	if( ptAct == NULL )
		return;
	dx = nextGx*GRID_SIZE-ptAct->mx;
	dy = nextGy*GRID_SIZE-ptAct->my;
	len = (float)sqrt( (double)(dx*dx+dy*dy) );
	if( len > 0 ){
		dx /= len;
		dy /= len;
	}else{
		dx = 0;
		dy = 0;
	}
	ptAct->vx = dx * MOVE_SPEED;
	ptAct->vy = dy * MOVE_SPEED;
	ptAct->nextGx = nextGx;
	ptAct->nextGy = nextGy;
	if( dx != 0 || dy != 0 ){
		dir1 = Atan( dx, dy ) + 22.5F;
		AdjustDir( &dir1 );
		dir = (int)(dir1/45);
		ptAct->anim_ang = dir;
		ptAct->walkFlag = 1;
	}
}

// ?????????????
void shiftBufCount( ACTION *ptAct )
{
	int i;
	if( ptAct == NULL )
		return;
	if( ptAct->bufCount > 0 )
		ptAct->bufCount--;
	for( i = 0; i < ptAct->bufCount; i++ ){
		ptAct->bufGx[i] = ptAct->bufGx[i+1];
		ptAct->bufGy[i] = ptAct->bufGy[i+1];
	}
}

// ??????
void charMove( ACTION *ptAct )
{
	float mx, my;
	if( ptAct == NULL )
		return;
	mx = (float)ptAct->nextGx*GRID_SIZE;
	my = (float)ptAct->nextGy*GRID_SIZE;
	// ????????????????
	if( mx == ptAct->mx && my == ptAct->my ){
		// ?????????????
		if( ptAct->bufCount > 0 ){
			setCharMovePoint( ptAct, ptAct->bufGx[0], ptAct->bufGy[0] );
			shiftBufCount( ptAct );
		}
	}
	mx = (float)ptAct->nextGx*GRID_SIZE;
	my = (float)ptAct->nextGy*GRID_SIZE;
	// ????
	if( ptAct->vx != 0 || ptAct->vy != 0 ){
		// ?佋?????????????
		if( pointLen2( ptAct->mx, ptAct->my, mx, my )
			<= ptAct->vx*ptAct->vx+ptAct->vy*ptAct->vy ){
			ptAct->mx = mx;
			ptAct->my = my;
			ptAct->vx = 0;
			ptAct->vy = 0;
		}else{
		// ?
			ptAct->mx += ptAct->vx;
			ptAct->my += ptAct->vy;
		}
		// ????
		ptAct->anim_no = ANIM_WALK;
	}else{
		// ??????
		if( ptAct->walkFlag != 0 )
			ptAct->anim_no = ANIM_STAND;
		ptAct->walkFlag = 0;
	}
	ptAct->gx = (int)(ptAct->mx/GRID_SIZE);
	ptAct->gy = (int)(ptAct->my/GRID_SIZE);
}

// ?????????????
void charMove2( ACTION *ptAct )
{
	float mx, my;
	if( ptAct == NULL )
		return;
	mx = (float)ptAct->nextGx*GRID_SIZE;
	my = (float)ptAct->nextGy*GRID_SIZE;
	// ????
	if( ptAct->vx != 0 || ptAct->vy != 0 ){
		// ?佋?????????????
		if( pointLen2( ptAct->mx, ptAct->my, mx, my )
			<= ptAct->vx*ptAct->vx+ptAct->vy*ptAct->vy ){
			ptAct->mx = mx;
			ptAct->my = my;
			ptAct->vx = 0;
			ptAct->vy = 0;
		}else{
		// ?
			ptAct->mx += ptAct->vx;
			ptAct->my += ptAct->vy;
		}
		// ????
		ptAct->anim_no = ANIM_WALK;
	}else{
		// ??????
		if( ptAct->walkFlag != 0 )
			ptAct->anim_no = ANIM_STAND;
		ptAct->walkFlag = 0;
	}
	ptAct->gx = (int)(ptAct->mx/GRID_SIZE);
	ptAct->gy = (int)(ptAct->my/GRID_SIZE);
}

// ?????????????
void _charMove( ACTION *ptAct )
{
	float mx, my;
	float vx, vy;
	if( ptAct == NULL )
		return;
	mx = (float)ptAct->nextGx*GRID_SIZE;
	my = (float)ptAct->nextGy*GRID_SIZE;
	// ????
	if( ptAct->vx != 0 || ptAct->vy != 0 ){
		vx = ptAct->vx * nowSpdRate;
		vy = ptAct->vy * nowSpdRate;
		// ?佋?????????????
		if( pointLen2( ptAct->mx, ptAct->my, mx, my )
			<= vx*vx+vy*vy ){
			ptAct->mx = mx;
			ptAct->my = my;
			ptAct->vx = 0;
			ptAct->vy = 0;
		}else{
		// ?
			ptAct->mx += vx;
			ptAct->my += vy;
		}
		// ????
		ptAct->anim_no = ANIM_WALK;
	}else{
		// ??????
		if( ptAct->walkFlag != 0 )
			ptAct->anim_no = ANIM_STAND;
		ptAct->walkFlag = 0;
	}
	ptAct->gx = (int)(ptAct->mx/GRID_SIZE);
	ptAct->gy = (int)(ptAct->my/GRID_SIZE);
}

// ???????
void setCharWarpPoint( ACTION *ptAct, int gx, int gy )
{
	if( ptAct == NULL )
		return;
	ptAct->gx = gx;
	ptAct->gy = gy;
	ptAct->nextGx = gx;
	ptAct->nextGy = gy;
	ptAct->mx = (float)gx*GRID_SIZE;
	ptAct->my = (float)gy*GRID_SIZE;
	ptAct->vx = 0;
	ptAct->vy = 0;
}

// ?????????
//
//   status : ?????????????
//   smsg   : ???????
//
//  smsg???status???
//
void setCharStatus( unsigned short *status, char *smsg )
{
	// 
	if( strstr( smsg, "P" ) )
		*status |= CHR_STATUS_P;
	else
		*status &= (~CHR_STATUS_P);
	// ??
	if( strstr( smsg, "N" ) )
		*status |= CHR_STATUS_N;
	else
		*status &= (~CHR_STATUS_N);
	// ?
	if( strstr( smsg, "Q" ) )
		*status |= CHR_STATUS_Q;
	else
		*status &= (~CHR_STATUS_Q);
	// ?
	if( strstr( smsg, "S" ) )
		*status |= CHR_STATUS_S;
	else
		*status &= (~CHR_STATUS_S);
	// ??
	if( strstr( smsg, "D" ) )
		*status |= CHR_STATUS_D;
	else
		*status &= (~CHR_STATUS_D);
	// ??
	if( strstr( smsg, "C" ) )
		*status |= CHR_STATUS_C;
	else
		*status &= (~CHR_STATUS_C);
}

// ???????????
void setCharLeader( ACTION *ptAct )
{
	CHAREXTRA *ext;
	int no;
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	no = ext->charObjTblId;
	charObj[no].status |= CHR_STATUS_LEADER;
}


int getCharType(ACTION *ptAct)
{
	extern int 人物屏蔽开关;
	if(ProcNo!=PROC_GAME) return 0;
	if(!人物屏蔽开关 ) return 0;
	CHAREXTRA *ext;
	int no;
	if( ptAct == NULL )
		return 0;
	ext = (CHAREXTRA *)ptAct->pYobi;
	if(ext){
		no = ext->charObjTblId;
		if(no >=1500) return 0;
		if(strcmp(ptAct->name,charObj[no].name)==0)
			if(charObj[no].charType == 256){
				return 1;
			}
	}
	return 0;
}




void delCharLeader( ACTION *ptAct )
{
	CHAREXTRA *ext;
	int no;
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	no = ext->charObjTblId;
	charObj[no].status &= (~CHR_STATUS_LEADER);
}

// ???????????????
void setCharParty( ACTION *ptAct )
{
	CHAREXTRA *ext;
	int no;
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	no = ext->charObjTblId;
	charObj[no].status |= CHR_STATUS_PARTY;
}

// ?????????????????
void delCharParty( ACTION *ptAct )
{
	CHAREXTRA *ext;
	int no;
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	no = ext->charObjTblId;
	charObj[no].status &= (~CHR_STATUS_PARTY);
}

// ????????
void setCharWatch( ACTION *ptAct )
{
	CHAREXTRA *ext;
	int no;
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	no = ext->charObjTblId;
	charObj[no].status |= CHR_STATUS_WATCH;
}

// ????????????
void delCharWatch( ACTION *ptAct )
{
	CHAREXTRA *ext;
	int no;
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	no = ext->charObjTblId;
	charObj[no].status &= (~CHR_STATUS_WATCH);
}
#ifdef _MIND_ICON
void setCharMind( ACTION *ptAct, int MindNo)
{
	CHAREXTRA *ext;
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	if(MindNo < SPR_001em ) {
		realGetNo( CG_ICON_FUKIDASI, (U4*)&MindNo );
	}
	ptAct->sMindIcon = MindNo;
	if(ext->ptMindIcon){
		DeathAction( ext->ptMindIcon);
		ext->ptMindIcon = NULL;
	}
	ext->ptMindIcon = createCommmonEffectAction( MindNo, ptAct->gx, ptAct->gy, 0, 0, DISP_PRIO_CHAR);
}
void delCharMind( ACTION *ptAct)
{
	CHAREXTRA *ext;	
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;	
}
#endif

#ifdef _SHOWFAMILYBADGE_
void setCharFamily( ACTION *ptAct, int MindNo)
{
	CHAREXTRA *ext;
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	ptAct->sFamilyIcon = MindNo;
	if(ext->ptFamilyIcon){
		DeathAction( ext->ptFamilyIcon);
		ext->ptFamilyIcon = NULL;
	}
	ext->ptFamilyIcon = createCommmonEffectAction( MindNo, ptAct->gx, ptAct->gy, 0, 0, DISP_PRIO_CHAR);
}
void delCharFamily( ACTION *ptAct)
{
	CHAREXTRA *ext;	
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;	
	if(ext->ptFamilyIcon){
		DeathAction( ext->ptFamilyIcon);
		ext->ptFamilyIcon = NULL;
	}
}
#endif


#ifdef _CHARTITLE_
void setCharmTitle( ACTION *ptAct, int MindNo)
{
	CHAREXTRA *ext;
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	ptAct->TitleIcon = MindNo;
	if(ext->ptTitleIcon){
		DeathAction( ext->ptTitleIcon);
		ext->ptTitleIcon = NULL;
	}
	ext->ptTitleIcon = createCommmonEffectAction( MindNo, ptAct->gx, ptAct->gy, 0, 0, DISP_PRIO_CHAR);
}

void delCharmTitle( ACTION *ptAct)
{
	CHAREXTRA *ext;	
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	if(ext->ptTitleIcon){
		DeathAction( ext->ptTitleIcon);
		ext->ptTitleIcon = NULL;
	}
}
#endif



#ifdef FAMILY_MANOR_
void setCharmFamily( ACTION *ptAct, int MindNo)
{
	CHAREXTRA *ext;
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	ptAct->mFamilyIcon = MindNo;
	if(ext->ptmFamilyIcon){
		DeathAction( ext->ptmFamilyIcon);
		ext->ptmFamilyIcon = NULL;
	}
	ext->ptmFamilyIcon = createCommmonEffectAction( MindNo, ptAct->gx, ptAct->gy, 0, 0, DISP_PRIO_CHAR);
}
void delCharmFamily( ACTION *ptAct)
{
	CHAREXTRA *ext;	
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	if(ext->ptmFamilyIcon){
		DeathAction( ext->ptmFamilyIcon);
		ext->ptmFamilyIcon = NULL;
	}
}
#endif
#ifdef _CHAR_MANOR_
void setCharmManor( ACTION *ptAct, int MindNo)
{
	CHAREXTRA *ext;
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	ptAct->mManorIcon = MindNo;
	if(ext->ptmManorIcon){
		DeathAction( ext->ptmManorIcon);
		ext->ptmManorIcon = NULL;
	}
	ext->ptmManorIcon = createCommmonEffectAction( MindNo, ptAct->gx, ptAct->gy, 0, 0, DISP_PRIO_CHAR-1);
}
void delCharmManor( ACTION *ptAct)
{
	CHAREXTRA *ext;	
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	if(ext->ptmManorIcon){
		DeathAction( ext->ptmManorIcon);
		ext->ptmManorIcon = NULL;
	}
}
#endif

#ifdef _NPC_EVENT_NOTICE
void setNpcNotice( ACTION *ptAct, int MindNo)
{
	CHAREXTRA *ext;
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	ptAct->noticeNo = MindNo;
	if(ext->ptNoticeIcon){
		DeathAction( ext->ptNoticeIcon);
		ext->ptNoticeIcon = NULL;
	}
	ext->ptNoticeIcon = createCommmonEffectAction( MindNo, ptAct->gx, ptAct->gy, 0, 0, DISP_PRIO_CHAR);
}
void delNpcNotice( ACTION *ptAct)
{
	CHAREXTRA *ext;	
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	if(ext->ptNoticeIcon){
		DeathAction( ext->ptNoticeIcon);
		ext->ptNoticeIcon = NULL;
	}
}
#endif

// ?????
void setCharBattle( ACTION *ptAct, int battleNo, short sideNo, short helpMode )
{
	CHAREXTRA *ext;
	int no;
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	no = ext->charObjTblId;
	charObj[no].status |= CHR_STATUS_BATTLE;
	charObj[no].battleNo = battleNo;
	charObj[no].sideNo = sideNo;
	charObj[no].helpMode = helpMode;
	if( helpMode )
		charObj[no].status |= CHR_STATUS_HELP;
	else
		charObj[no].status &= ~CHR_STATUS_HELP;
}

// ?????
void delCharBattle( ACTION *ptAct )
{
	CHAREXTRA *ext;
	int no;
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	no = ext->charObjTblId;
	charObj[no].status &= (~(CHR_STATUS_BATTLE | CHR_STATUS_HELP));
}

// 交易中
//void setCharBattle( ACTION *ptAct, int battleNo, short sideNo, short helpMode )
void setCharTrade( ACTION *ptAct, int battleNo )
{
	CHAREXTRA *ext;
	int no;
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	no = ext->charObjTblId;
	charObj[no].status |= CHR_STATUS_TRADE;
	//charObj[no].battleNo = battleNo;
}

// ?????
void delCharTrade( ACTION *ptAct )
{
	CHAREXTRA *ext;
	int no;
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	no = ext->charObjTblId;
	charObj[no].status &= (~CHR_STATUS_TRADE|CHR_STATUS_LEADER);
}

#ifdef _ANGEL_SUMMON
void setCharAngel( ACTION *ptAct )
{
	CHAREXTRA *ext;
	int no;
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	no = ext->charObjTblId;
	charObj[no].status |= CHR_STATUS_ANGEL;
}

void delCharAngel( ACTION *ptAct )
{
	CHAREXTRA *ext;
	int no;
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	no = ext->charObjTblId;
	charObj[no].status &= ~CHR_STATUS_ANGEL;
}
#endif

// ???????????
void setCharUseMagic( ACTION *ptAct )
{
	CHAREXTRA *ext;
	int no;
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	no = ext->charObjTblId;
	charObj[no].status |= CHR_STATUS_USE_MAGIC;
}

// ???????????????????
// ???????
extern int 人物屏蔽开关;

void delCharUseMagic( ACTION *ptAct )
{
	CHAREXTRA *ext;
	int no;
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	no = ext->charObjTblId;
	charObj[no].status &= (~CHR_STATUS_USE_MAGIC);
}

// ?????????
void setCharFukidashi( ACTION *ptAct, unsigned int offTime )
{
	CHAREXTRA *ext;
	int no;
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	no = ext->charObjTblId;
	charObj[no].status |= CHR_STATUS_FUKIDASHI;
	ext->drawFukidashiTime = offTime + TimeGetTime();
}

// ??????
void drawCharStatus( ACTION *ptAct )
{
	CHAREXTRA *ext;
	int no;
#ifdef _ANGEL_SUMMON
	unsigned status;
#else
	unsigned short status;
#endif
	int battleNo;
	int sideNo;
	int helpMode;
	char msg[256];
#ifdef _AniCharBubble	   // Syu ADD 动画层人物吐出气泡
	int RandAnimate;
#endif



	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	no = ext->charObjTblId;

	if( pc.ptAct != NULL && pc.ptAct == ptAct ){
		// ???????
		status = pc.status;
		battleNo = 0;
		sideNo = 0;
		helpMode = 0;
	}else{
		status = charObj[no].status;
		battleNo = charObj[no].battleNo;
		sideNo = charObj[no].sideNo;
		helpMode = charObj[no].helpMode;
	}


#ifdef _CHAR_MANOR_
	if( ext->ptmManorIcon ){
		{
			if(人物屏蔽开关 && ProcNo==PROC_GAME  && pc.ptAct != ptAct ){
				if(ATR_PAT_NO(ext->ptmManorIcon)){
					ATR_PAT_BAK_NO(ext->ptmManorIcon) = ATR_PAT_NO(ext->ptmManorIcon);
					ATR_PAT_NO(ext->ptmManorIcon) = 0;
				}
			}else{
				if(!ATR_PAT_NO(ext->ptmManorIcon) ){
					if(ATR_PAT_BAK_NO(ext->ptmManorIcon)){
						ATR_PAT_NO(ext->ptmManorIcon) = ATR_PAT_BAK_NO(ext->ptmManorIcon);
					}
				}
				short x1,y1;
				if(获取动画尺寸(ptAct,&x1,&y1)){
					ext->ptmManorIcon->x  = ptAct->x;
					ext->ptmManorIcon->y  = ptAct->y;
					ext->ptmManorIcon->mx = ptAct->mx;
					ext->ptmManorIcon->my = ptAct->my;
					pattern( ext->ptmManorIcon, ANM_NOMAL_SPD, ANM_LOOP );
					StockDispBuffer2(ext->ptmManorIcon->x, ext->ptmManorIcon->y, ext->ptmManorIcon->dispPrio, ext->ptmManorIcon->bmpNo, 0);
				}	
			}
		}
	}
#endif




	if( (status & CHR_STATUS_BATTLE) != 0 ){
		if(人物屏蔽开关 && ProcNo==PROC_GAME  && pc.ptAct != ptAct ){
			;//ATR_PAT_NO(ptAct) = 0;
		}else{
			int no;
			U4 bmpNo;
			short x1,y1;
			if(获取动画尺寸(ptAct,&x1,&y1)){
				realGetNo( battleIconTbl[(battleNo%10)][(sideNo%1)], &bmpNo );
				// ??????????????
				setCharPrio( bmpNo, ptAct->x, ptAct->y, 0,ptAct->sFamilyIcon?-(y1/2+_OTHERTEXIAOY_):-(y1/2+_FANILYTEXIAOY_), ptAct->mx, ptAct->my
	#ifdef _SFUMATO
					, ptAct->sfumato
	#endif
					);
			}
		}


	}

	// Trade Mark
	if( (status & CHR_STATUS_TRADE) != 0 ){
		if(人物屏蔽开关 && ProcNo==PROC_GAME  && pc.ptAct != ptAct ){
			;//ATR_PAT_NO(ptAct) = 0;
		}else{

			U4 bmpNo;
			short x1,y1;
			if(获取动画尺寸(ptAct,&x1,&y1)){
				realGetNo( CG_TRADE_MARK, &bmpNo );		
				setCharPrio( bmpNo, ptAct->x, ptAct->y, 0,ptAct->sFamilyIcon?-(y1/2+_OTHERTEXIAOY_):-(y1/2+_FANILYTEXIAOY_), ptAct->mx, ptAct->my
	#ifdef _SFUMATO
					, ptAct->sfumato
	#endif
					);
			}
		}
	}

#ifdef _ANGEL_SUMMON
	if( (status & CHR_STATUS_ANGEL) != 0  && ext->ptActAngelMark == NULL ) {
		if( g_bUseAlpha )
			ext->ptActAngelMark =createCommmonEffectAction( 101812, ptAct->gx, ptAct->gy, 0, 0, DISP_PRIO_CHAR );
		else
			ext->ptActAngelMark =createCommmonEffectAction( 101865, ptAct->gx, ptAct->gy, 0, 0, DISP_PRIO_CHAR );

	}else if( (status & CHR_STATUS_ANGEL) == 0 && ext->ptActAngelMark != NULL ) {
		// ??????????????
		DeathAction( ext->ptActAngelMark );
		ext->ptActAngelMark = NULL;
	}else if( (status & CHR_STATUS_ANGEL) != 0 && ext->ptActAngelMark != NULL ) {
		//int no;
		//no = (ptAct->anim_chr_no - SPR_001em) / 20;
		//no %= 12;	// ?????????
		// ???????????????
		short x1,y1;
		if(获取动画尺寸(ptAct,&x1,&y1)){
			ext->ptActAngelMark->x  = ptAct->x;
			ext->ptActAngelMark->y  = ptAct->y + ptAct->anim_y + 140;
			ext->ptActAngelMark->mx = ptAct->mx;
			ext->ptActAngelMark->my = ptAct->my;
			// ?????????
			pattern( ext->ptActAngelMark, ANM_NOMAL_SPD, ANM_LOOP );
			// ??????????????
			setCharPrio( ext->ptActAngelMark->bmpNo,
				ext->ptActAngelMark->x, ext->ptActAngelMark->y, 0,ptAct->sFamilyIcon?-(y1/2+_OTHERTEXIAOY_):-(y1/2+_FANILYTEXIAOY_),
				ext->ptActAngelMark->mx, ext->ptActAngelMark->my
#ifdef _SFUMATO
				, ptAct->sfumato
#endif
				);

		}
	}
#endif

	// ????
	if( (status & CHR_STATUS_WATCH) != 0 ){
		if(人物屏蔽开关 && ProcNo==PROC_GAME  && pc.ptAct != ptAct ){
			;//ATR_PAT_NO(ptAct) = 0;
		}else{
			int no;
			U4 bmpNo;
			short x1,y1;
			if(获取动画尺寸(ptAct,&x1,&y1)){
				realGetNo( CG_ICON_WATCHING, &bmpNo );
				// ??????????????
				setCharPrio( bmpNo, ptAct->x, ptAct->y, 0,ptAct->sFamilyIcon?-(y1/2+_OTHERTEXIAOY_):-(y1/2+_FANILYTEXIAOY_), ptAct->mx, ptAct->my
	#ifdef _SFUMATO
					, ptAct->sfumato
	#endif
					);

			}
		}
	}
	// ??????
	if( (status & CHR_STATUS_HELP) != 0 ){
		if(人物屏蔽开关 && ProcNo==PROC_GAME  && pc.ptAct != ptAct ){
			;//ATR_PAT_NO(ptAct) = 0;
		}else{
			int no;
			U4 bmpNo;
			short x1,y1;
			if(获取动画尺寸(ptAct,&x1,&y1)){
				realGetNo( CG_SPEECH_HELP, &bmpNo );
				// ??????????????
				setCharPrio( bmpNo, ptAct->x, ptAct->y, 0,ptAct->sFamilyIcon?-(y1/2+_OTHERTEXIAOY_-20):-(y1/2+_FANILYTEXIAOY_-20), ptAct->mx, ptAct->my
	#ifdef _SFUMATO
					, ptAct->sfumato
	#endif
					);
			}
		}
	}
	// ???????
	//????????????
	//??????????
	if( (status & CHR_STATUS_LEADER) != 0
	 && ext->ptActLeaderMark == NULL
	 && ((status & CHR_STATUS_BATTLE) == 0 || (status & CHR_STATUS_WATCH) == 0) ){
		// ??????????????
		ext->ptActLeaderMark =
			createCommmonEffectAction( SPR_leader, ptAct->gx, ptAct->gy, 0, 0, DISP_PRIO_CHAR );
	}else if( ((status & CHR_STATUS_LEADER) == 0 || (status & CHR_STATUS_BATTLE) != 0)
	 && ext->ptActLeaderMark != NULL ){
		// ??????????????
		DeathAction( ext->ptActLeaderMark );
		ext->ptActLeaderMark = NULL;
	}else if( (status & CHR_STATUS_LEADER) != 0 && ext->ptActLeaderMark != NULL ){
		if(人物屏蔽开关 && ProcNo==PROC_GAME  && pc.ptAct != ptAct){
			if(ATR_PAT_NO(ext->ptActLeaderMark)){
				ATR_PAT_BAK_NO(ext->ptActLeaderMark) = ATR_PAT_NO(ext->ptActLeaderMark);
				ATR_PAT_NO(ext->ptActLeaderMark) = 0;
			}
		}else{
			if(!ATR_PAT_NO(ext->ptActLeaderMark) ){
				if(ATR_PAT_BAK_NO(ext->ptActLeaderMark)){
					ATR_PAT_NO(ext->ptActLeaderMark) = ATR_PAT_BAK_NO(ext->ptActLeaderMark);
				}
			}
			short x1,y1;
			if(获取动画尺寸(ptAct,&x1,&y1)){
				ext->ptActLeaderMark->x  = ptAct->x;
				ext->ptActLeaderMark->y  = ptAct->y;
				ext->ptActLeaderMark->mx = ptAct->mx;
				ext->ptActLeaderMark->my = ptAct->my;
				// ?????????
				pattern( ext->ptActLeaderMark, ANM_NOMAL_SPD, ANM_LOOP );
				// ??????????????
				setCharPrio( ext->ptActLeaderMark->bmpNo,
					ext->ptActLeaderMark->x, ext->ptActLeaderMark->y, 0,ptAct->sFamilyIcon?-(y1/2+_OTHERTEXIAOY_):-(y1/2+_FANILYTEXIAOY_),
					ext->ptActLeaderMark->mx, ext->ptActLeaderMark->my
	#ifdef _SFUMATO
					, ptAct->sfumato
	#endif
					);


			}
		}
	}
	// ?????
	if( (status & CHR_STATUS_USE_MAGIC) != 0 && ext->ptActMagicEffect == NULL ){
		// ????????????
		ext->ptActMagicEffect =
			createCommmonEffectAction( SPR_effect01, ptAct->gx, ptAct->gy+1, 0, 0, DISP_PRIO_CHAR );
		if( pc.ptAct != NULL && pc.ptAct == ptAct )
			delPcUseMagic();
		else
			delCharUseMagic( ptAct );
	}else if( ext->ptActMagicEffect != NULL ){
		// ?????????????
		ext->ptActMagicEffect->x  = ptAct->x;
		ext->ptActMagicEffect->y  = ptAct->y;
		ext->ptActMagicEffect->mx = ptAct->mx;
		ext->ptActMagicEffect->my = ptAct->my;
		// ?????????
		if( pattern( ext->ptActMagicEffect, ANM_NOMAL_SPD, ANM_NO_LOOP ) == 0 ){
			// ??????????????
			setCharPrio( ext->ptActMagicEffect->bmpNo,
				ext->ptActMagicEffect->x, ext->ptActMagicEffect->y+1, 0, 0,
				ext->ptActMagicEffect->mx, ext->ptActMagicEffect->my
#ifdef _SFUMATO
				, ptAct->sfumato
#endif
 );
		}else{
			// ????????????
			DeathAction( ext->ptActMagicEffect );
			ext->ptActMagicEffect = NULL;
		}
	}
	// ??????
	if( (status & CHR_STATUS_FUKIDASHI) != 0 ){
		if( ext->drawFukidashiTime > TimeGetTime() ){
			int no;
			U4 bmpNo;
			realGetNo( CG_ICON_FUKIDASI, &bmpNo );
			if( SPR_001em <= ptAct->anim_chr_no && ptAct->anim_chr_no <= SPR_114bw ){
				if(人物屏蔽开关 && ProcNo==PROC_GAME  && pc.ptAct != ptAct){
					;//ATR_PAT_NO(ptAct) = 0;
				}else{
					short x1,y1;
					if(获取动画尺寸(ptAct,&x1,&y1)){
						setCharPrio( bmpNo, ptAct->x, ptAct->y, 20,ptAct->sFamilyIcon?-(y1/2+_OTHERTEXIAOY_):-(y1/2+_FANILYTEXIAOY_), ptAct->mx, ptAct->my
	#ifdef _SFUMATO
							, ptAct->sfumato
	#endif
							);

					}
				}
			}else{
				// ??????????????
				setCharPrio( bmpNo, ptAct->x, ptAct->y, 20, -84, ptAct->mx, ptAct->my
#ifdef _SFUMATO
					, ptAct->sfumato
#endif
 );
			}
		}else
			status &= (~CHR_STATUS_FUKIDASHI);
	}
#ifdef _CHARTITLE_STR_
	extern void PutTitleText(int x,int y,char fontPrio,TITLE_STR str,BOOL hitFlag);
	CHAREXTRA *ext1 = (CHAREXTRA *)ptAct->pYobi;
	if(ext1){
		if(!ext1->ptStreetVendor){
			if(ptAct->TitleText.flg==1){
				short x1,y1;
				if(获取动画尺寸(ptAct,&x1,&y1)){
					PutTitleText(ptAct->x-ptAct->TitleText.len/2,ptAct->y-(y1/2+45)-12,0,ptAct->TitleText,0);
				}
			}	
		}
	}
#endif
	if( nameOverTheHeadFlag
	 && ptAct != pc.ptAct
	 && (ptAct->atr & (ACT_ATR_TYPE_PC | ACT_ATR_TYPE_OTHER_PC | ACT_ATR_TYPE_PET |ACT_ATR_TYPE_OTHER))
	 && ptAct->anim_chr_no != 999 && ptAct->anim_chr_no != 0 ){
		if( (ptAct->atr & ACT_ATR_TYPE_PET) && strlen( ptAct->freeName ) > 0 )
			sprintf_s( msg, "%s", ptAct->freeName );
		else{
			sprintf_s( msg, "%s", ptAct->fmname);
			StockFontBuffer( ptAct->x-GetStrWidth( msg )/2, ptAct->y-23, FONT_PRIO_BACK, ptAct->charNameColor, msg, 0 );
			sprintf_s( msg, "%s", ptAct->name );
		}
		StockFontBuffer( ptAct->x-GetStrWidth( msg )/2, ptAct->y-10, FONT_PRIO_BACK, ptAct->charNameColor, msg, 0 );
	}

#ifdef _GM_MARK				   // WON ADD GM 识别
	{
		int graNo = ptAct->anim_chr_no ;

		//三种ＧＭ造形
		if( (graNo == 100420) || (graNo == 100425) || (graNo == 100445) ){
			static count1=1, count2=0;
			char msg[256];

			if(count1 >10 ) count1=1;
			if(count2 >15 ){				
				count1++;
				count2=0;
			}

			count2++;
			sprintf_s( msg , "ＧＭ");
			StockFontBuffer( ptAct->x-GetStrWidth( msg )/2, ptAct->y-110, FONT_PRIO_BACK, count1, msg, 1);	
		}
	}
#endif

#ifdef _GM_IDENTIFY		// Rog ADD GM识别
		int i;
		sprintf_s( msg , ptAct->gm_name);
        for(i=2;i>0;i--)
	        StockFontBuffer( ptAct->x-GetStrWidth( msg )/2, ptAct->y-110-i, FONT_PRIO_BACK, i , msg, 1);
#endif

#ifdef _CHAR_PROFESSION			// WON ADD 人物职业

//	if( ptAct->profession_class != 0 ){
	{
		char msg[10];

		switch(pc.profession_class){
		case 0:		sprintf_s( msg , "");	break;
		case 1:		sprintf_s( msg , "白狼勇士");	break;
		case 2:		sprintf_s( msg , "闇灵巫师");	break;
		case 3:		sprintf_s( msg , "追猎者");	break;
		}
		sprintf_s( pc.profession_class_name , "%s" , msg ) ; 
		//StockFontBuffer( ptAct->x-GetStrWidth( msg )/2, ptAct->y-110, FONT_PRIO_BACK, FONT_PAL_YELLOW, msg, 1);
		//StockFontBuffer( ptAct->x-GetStrWidth( msg )/2, ptAct->y-110, FONT_PRIO_BACK, FONT_PAL_YELLOW, msg, 1);
		//StockDispBuffer( ptAct->x-GetStrWidth( msg )/2 + 30, ptAct->y-90 , DISP_PRIO_IME3, CG_WAR_ICON_SMALL + pc.profession_class - 1, 0 );
						
	}

#endif

#ifdef _AniCharBubble	   // Syu ADD 动画层人物吐出气泡
	if ( ( (nowFloor == 817) || (nowFloor == 8007) || (nowFloor == 8101) || (nowFloor == 8100) ||
		(nowFloor == 8027) || (nowFloor == 8028) || (nowFloor == 8029) || nowFloor == 8113 || nowFloor == 8114 ||
		(nowFloor == 8015) ) && (popflag == false) && (waittimeflag == false))
	{
		RandAnimate = RAND ( 101508 , 101509 );  //随机选取左气泡或右气泡
		if ( popAct == NULL )  
		{
			//做左、右气泡产生时需要的偏移值
			if( RandAnimate == 101508 )  
				popAct = createWaterAnimation( RandAnimate , nowGx + 2 , nowGy - 1 , 30 );
			if( RandAnimate == 101509 )
				popAct = createWaterAnimation( RandAnimate , nowGx + 1 , nowGy - 2 , 30 );
			popflag = true;
		}
	}
	if ( waittimeflag == false && popAct != NULL)
	if ( popAct->anim_cnt == 6 ) //拨放到最后一张
	{
		if ( popAct != NULL)
		{
			DeathAction( popAct );
			popAct = NULL;
		}
		popflag = false;
		waittimeflag = true;
	}
	if ( waittimeflag == true ) //计时等待下一次产生时间
	{
		waittime ++ ;
		if (waittime > 350 ) 
		{
			waittime = 0;
			waittimeflag = false;
		}
	}
	if ( ( (nowFloor != 817) && (nowFloor != 8007) && (nowFloor != 8101) && (nowFloor != 8100) && (nowFloor != 8113) && (nowFloor != 8114) &&
		(nowFloor != 8027) && (nowFloor != 8028) && (nowFloor != 8029) && (nowFloor != 8015)) && (popflag == true))
	{
		popflag = false;
		waittimeflag = false;  
		if ( popAct != NULL)
		{
			DeathAction ( popAct ) ;
			popAct = NULL;
		}
	}

#endif
#ifdef __EMOTION
	if( ext->ptActEmotion){
		if( ext->ptActEmotion->mx != ptAct->mx ||
			ext->ptActEmotion->my != ptAct->my ||
			ext->ptActEmotion->x != ptAct->x ||
			ext->ptActEmotion->y != ptAct->y ){
			DeathAction( ext->ptActEmotion);
			ext->ptActEmotion = NULL;
			//pattern( ext->ptActEmotion, ANM_NOMAL_SPD, ANM_LOOP );
		}else{
			if(人物屏蔽开关 && ProcNo==PROC_GAME  && pc.ptAct != ptAct){
				if(ATR_PAT_NO(ext->ptActEmotion)){
					ATR_PAT_BAK_NO(ext->ptActEmotion) = ATR_PAT_NO(ext->ptActEmotion);
					ATR_PAT_NO(ext->ptActEmotion) = 0;
				}
			}else{
				if(!ATR_PAT_NO(ext->ptActEmotion) ){
					if(ATR_PAT_BAK_NO(ext->ptActEmotion)){
						ATR_PAT_NO(ext->ptActEmotion) = ATR_PAT_BAK_NO(ext->ptActEmotion);
					}
				}
				short x1,y1;
				if(获取动画尺寸(ptAct,&x1,&y1)){
					pattern( ext->ptActEmotion, ANM_NOMAL_SPD, ANM_LOOP );
					setCharPrio( ext->ptActEmotion->bmpNo,
						ext->ptActEmotion->x, ext->ptActEmotion->y, 0,ptAct->sFamilyIcon?-(y1/2+_OTHERTEXIAOY_):-(y1/2+_FANILYTEXIAOY_),
						ext->ptActEmotion->mx, ext->ptActEmotion->my
	#ifdef _SFUMATO
						, ptAct->sfumato
	#endif
					);
				}
			}
		}
	}
#endif
#ifdef _STREET_VENDOR
	extern int 人物屏蔽开关;
	if(ext->ptStreetVendor){
		BOOL pand = FALSE;

		if(人物屏蔽开关){
			if(pc.ptAct == ptAct) pand = TRUE;
		}else pand = TRUE;
		if(pand){
			ext->ptStreetVendor->x  = ptAct->x;
			ext->ptStreetVendor->y  = ptAct->y;
			ext->ptStreetVendor->mx = ptAct->mx;
			ext->ptStreetVendor->my = ptAct->my;
	#ifdef _STREET_VENDOR_CHANGE_ICON
			StockDispBuffer(ext->ptStreetVendor->x,ext->ptStreetVendor->y + 35,DISP_PRIO_TILE+1,35343,0);
			StockDispBuffer(ext->ptStreetVendor->x - 10,ext->ptStreetVendor->y + 10,DISP_PRIO_CHAR-1,35344,0);
			StockDispBuffer(ext->ptStreetVendor->x,ext->ptStreetVendor->y - 15,DISP_PRIO_CHAR+1,35345,0);
	#else
		#ifdef _READ16BITBMP
			StockDispBuffer(ext->ptStreetVendor->x,ext->ptStreetVendor->y - 80,DISP_PRIO_CHAR,OLD_GRAPHICS_START+58,10);
		#endif
	#endif
				StockFontBuffer(ext->ptStreetVendor->x - 65,ext->ptStreetVendor->y - 92,
											FONT_PRIO_BACK,MyChatBuffer.color,ptAct->szStreetVendorTitle,0);
		
		}
	}
#endif
#ifdef _MIND_ICON
	if( ext->ptMindIcon){
		
	//这里删除掉就可以走动显示
		if( ((ptAct->sMindIcon != SPR_asleep) &&
		     (ptAct->sMindIcon != SPR_cafe) &&
		     (ptAct->sMindIcon != SPR_accent) ) &&
			 pattern( ext->ptMindIcon, ANM_NOMAL_SPD, ANM_NO_LOOP)
		    ){
			DeathAction( ext->ptMindIcon);
			ext->ptMindIcon = NULL;
		}
		else{
			if(人物屏蔽开关 && ProcNo==PROC_GAME  && pc.ptAct != ptAct){
				if(ATR_PAT_NO(ext->ptMindIcon)){
					ATR_PAT_BAK_NO(ext->ptMindIcon) = ATR_PAT_NO(ext->ptMindIcon);
					ATR_PAT_NO(ext->ptMindIcon) = 0;
				}
			}else{
				if(!ATR_PAT_NO(ext->ptMindIcon) ){
					if(ATR_PAT_BAK_NO(ext->ptMindIcon)){
						ATR_PAT_NO(ext->ptMindIcon) = ATR_PAT_BAK_NO(ext->ptMindIcon);
					}
				}
				short x1,y1;
				if(获取动画尺寸( ptAct,&x1,&y1)){
					ext->ptMindIcon->x  = ptAct->x;
					ext->ptMindIcon->y  = ptAct->y;
					ext->ptMindIcon->mx = ptAct->mx;
					ext->ptMindIcon->my = ptAct->my;
					pattern( ext->ptMindIcon, ANM_NOMAL_SPD, ANM_LOOP );
					setCharPrio( ext->ptMindIcon->bmpNo,
						ext->ptMindIcon->x, ext->ptMindIcon->y, 0,ptAct->sFamilyIcon?-(y1/2+_OTHERTEXIAOY_):-(y1/2+_FANILYTEXIAOY_),
						ext->ptMindIcon->mx, ext->ptMindIcon->my
	#ifdef _SFUMATO
						, ptAct->sfumato
	#endif
					);			
				}
			}
		}
	}
#endif

#ifdef _NPC_EVENT_NOTICE
	if( ext->ptNoticeIcon){//这里删除掉就可以走动显示
		{
{
				if(!ATR_PAT_NO(ext->ptNoticeIcon) ){
					if(ATR_PAT_BAK_NO(ext->ptNoticeIcon)){
						ATR_PAT_NO(ext->ptNoticeIcon) = ATR_PAT_BAK_NO(ext->ptNoticeIcon);
					}
				}
				short x1,y1,x2,y2;
				y2=x2=0;
				if(获取动画尺寸(ptAct,&x1,&y1)){
					ext->ptNoticeIcon->x  = ptAct->x;
					ext->ptNoticeIcon->y  = ptAct->y;
					ext->ptNoticeIcon->mx = ptAct->mx;
					ext->ptNoticeIcon->my = ptAct->my;
					pattern( ext->ptNoticeIcon, ANM_NOMAL_SPD, ANM_LOOP );

					setCharPrio( ext->ptNoticeIcon->bmpNo,
						ext->ptNoticeIcon->x-x2, ext->ptNoticeIcon->y, 0,-(y1/2+_FANILYTEXIAOY_+10),
						ext->ptNoticeIcon->mx, ext->ptNoticeIcon->my
	#ifdef _SFUMATO
						, ptAct->sfumato
	#endif
						);		

				}		
			}
		}
	}
#endif

#ifdef _SHOWFAMILYBADGE_
	if( ext->ptFamilyIcon){//这里删除掉就可以走动显示
		{
			if(人物屏蔽开关 && ProcNo==PROC_GAME  && pc.ptAct != ptAct ){
				if(ATR_PAT_NO(ext->ptFamilyIcon)){
					ATR_PAT_BAK_NO(ext->ptFamilyIcon) = ATR_PAT_NO(ext->ptFamilyIcon);
					ATR_PAT_NO(ext->ptFamilyIcon) = 0;
				}
			}else{
				if(!ATR_PAT_NO(ext->ptFamilyIcon) ){
					if(ATR_PAT_BAK_NO(ext->ptFamilyIcon)){
						ATR_PAT_NO(ext->ptFamilyIcon) = ATR_PAT_BAK_NO(ext->ptFamilyIcon);
					}
				}
				short x1,y1,x2,y2;
				y2=x2=0;
				if(获取动画尺寸(ptAct,&x1,&y1)){
					ext->ptFamilyIcon->x  = ptAct->x;
					ext->ptFamilyIcon->y  = ptAct->y;
					ext->ptFamilyIcon->mx = ptAct->mx;
					ext->ptFamilyIcon->my = ptAct->my;
					pattern( ext->ptFamilyIcon, ANM_NOMAL_SPD, ANM_LOOP );
#ifdef _CHARTITLE_
					if(ext->ptTitleIcon){
						获取动画尺寸(ext->ptTitleIcon,&x2,&y2);
						x2=x2/2;
					}
#endif

#ifdef _CHARTITLE_STR_
					if(ptAct->TitleText.flg){
						x2 = ptAct->TitleText.len/2+15;
					}
					setCharPrio( ext->ptFamilyIcon->bmpNo,
						ext->ptFamilyIcon->x-x2, ext->ptFamilyIcon->y, 0,-(y1/2+_FANILYTEXIAOY_)-2,
						ext->ptFamilyIcon->mx, ext->ptFamilyIcon->my
#ifdef _SFUMATO
						, ptAct->sfumato
#endif
						);
#else
					setCharPrio( ext->ptFamilyIcon->bmpNo,
						ext->ptFamilyIcon->x-x2, ext->ptFamilyIcon->y, 0,-(y1/2+_FANILYTEXIAOY_),
						ext->ptFamilyIcon->mx, ext->ptFamilyIcon->my
#ifdef _SFUMATO
						, ptAct->sfumato
#endif
						);
#endif
		

				}		
			}
		}
	}
#endif

#ifdef FAMILY_MANOR_
	if( ext->ptmFamilyIcon ){//这里删除掉就可以走动显示
		{
			if(人物屏蔽开关 && ProcNo==PROC_GAME  && pc.ptAct != ptAct ){
				if(ATR_PAT_NO(ext->ptmFamilyIcon)){
					ATR_PAT_BAK_NO(ext->ptmFamilyIcon) = ATR_PAT_NO(ext->ptmFamilyIcon);
					ATR_PAT_NO(ext->ptmFamilyIcon) = 0;
				}
			}else{
				if(!ATR_PAT_NO(ext->ptmFamilyIcon) ){
					if(ATR_PAT_BAK_NO(ext->ptmFamilyIcon)){
						ATR_PAT_NO(ext->ptmFamilyIcon) = ATR_PAT_BAK_NO(ext->ptmFamilyIcon);
					}
				}
				short x1,y1;
				if(获取动画尺寸(ptAct,&x1,&y1)){
					ext->ptmFamilyIcon->x  = ptAct->x;
					ext->ptmFamilyIcon->y  = ptAct->y;
					ext->ptmFamilyIcon->mx = ptAct->mx;
					ext->ptmFamilyIcon->my = ptAct->my;
					pattern( ext->ptmFamilyIcon, ANM_NOMAL_SPD, ANM_LOOP );
					setCharPrio( ext->ptmFamilyIcon->bmpNo,
						ext->ptmFamilyIcon->x, ext->ptmFamilyIcon->y, 0,-(y1/2+_FANILYTEXIAOY_)+100,
						ext->ptmFamilyIcon->mx, ext->ptmFamilyIcon->my
	#ifdef _SFUMATO
						, ptAct->sfumato
	#endif
						);		

				}	
			}
		}
	}
#endif

#ifdef _CHARTITLE_
	if( ext->ptTitleIcon ){//这里删除掉就可以走动显示
		{
			if(人物屏蔽开关 && ProcNo==PROC_GAME && pc.ptAct != ptAct ){
				if(ATR_PAT_NO(ext->ptTitleIcon)){
					ATR_PAT_BAK_NO(ext->ptTitleIcon) = ATR_PAT_NO(ext->ptTitleIcon);
					ATR_PAT_NO(ext->ptTitleIcon) = 0;
				}
			}else{
				if(!ATR_PAT_NO(ext->ptTitleIcon) ){
					if(ATR_PAT_BAK_NO(ext->ptTitleIcon)){
						ATR_PAT_NO(ext->ptTitleIcon) = ATR_PAT_BAK_NO(ext->ptTitleIcon);
					}
				}
				short x1,y1,x2,y2;
				y2=x2=0;
				if(获取动画尺寸(ptAct,&x1,&y1)){
					ext->ptTitleIcon->x  = ptAct->x;
					ext->ptTitleIcon->y  = ptAct->y;
					ext->ptTitleIcon->mx = ptAct->mx;
					ext->ptTitleIcon->my = ptAct->my;
					pattern( ext->ptTitleIcon, ANM_NOMAL_SPD, ANM_LOOP );
					if(ext->ptFamilyIcon){
						x2=10;
					}
					setCharPrio( ext->ptTitleIcon->bmpNo,
						ext->ptTitleIcon->x+x2, ext->ptTitleIcon->y, 0,-(y1/2+_FANILYTEXIAOY_),
						ext->ptTitleIcon->mx, ext->ptTitleIcon->my
#ifdef _SFUMATO
						, ptAct->sfumato
#endif
						);		

				}	
			}
		}
	}
#endif


#ifdef _ITEM_FIREWORK
	if (ext->pActFirework[0])	// 有要放烟火
	{
		float	fX, fY;

		camMapToGamen((float)ext->pActFirework[0]->gx * GRID_SIZE, (float)ext->pActFirework[0]->gy * GRID_SIZE, &fX, &fY);
		ext->pActFirework[0]->x	= (int)fX;
		ext->pActFirework[0]->y	= (int)fY;
		// 如果第一个烟火放完了,再来以type来决定接下来要怎么放
		if (pattern(ext->pActFirework[0], ANM_NOMAL_SPD, ANM_NO_LOOP))
		{
			if (ext->pActFirework[0]->damage == 0)	// 一个放中间二个左右
			{
				int nGX, nGY;
				
				nGX = ext->pActFirework[0]->gx;
				nGY = ext->pActFirework[0]->gy;
				ext->pActFirework[1] = GetAction(PRIO_CHR, 0);
				ext->pActFirework[1]->damage			= 1;
				ext->pActFirework[1]->anim_chr_no		= ext->pActFirework[0]->anim_chr_no;
				ext->pActFirework[1]->anim_no			= ANIM_STAND;
				ext->pActFirework[1]->anim_ang			= 1;
				ext->pActFirework[1]->actNo				= 0;
				ext->pActFirework[1]->dispPrio			= DISP_PRIO_RESERVE;
				ext->pActFirework[0]->gx				= nGX - 2;
				ext->pActFirework[0]->gy				= nGY - 2;
				ext->pActFirework[0]->anim_frame_cnt	= 0;
				ext->pActFirework[0]->anim_cnt			= 0;
				ext->pActFirework[1]->gx				= nGX + 2;
				ext->pActFirework[1]->gy				= nGY + 2;
				ext->pActFirework[0]->damage			= -1;
			}
			else if (ext->pActFirework[0]->damage == 1)	// 中间三次
			{
				ext->pActFirework[1] = GetAction(PRIO_CHR, 0);
				ext->pActFirework[1]->anim_chr_no	= ext->pActFirework[0]->anim_chr_no;
				ext->pActFirework[1]->anim_no		= ANIM_STAND;
				ext->pActFirework[1]->anim_ang		= 1;
				ext->pActFirework[1]->actNo			= 0;
				ext->pActFirework[1]->dispPrio		= DISP_PRIO_RESERVE;
				ext->pActFirework[1]->level			= 1;					// level 拿来当计数用
				ext->pActFirework[1]->gx			= ext->pActFirework[0]->gx;
				ext->pActFirework[1]->gy			= ext->pActFirework[0]->gy;
				ext->pActFirework[0]->damage		= -2;
			}
			else if (ext->pActFirework[0]->damage == 2 || ext->pActFirework[0]->damage == -1)
				ext->pActFirework[0]->damage = -2;
			else if (ext->pActFirework[0]->damage == 3)	// 绕人物四周一圈, 共三次
			{
				ext->pActFirework[1] = GetAction(PRIO_CHR, 0);
				ext->pActFirework[1]->anim_chr_no	= ext->pActFirework[0]->anim_chr_no;
				ext->pActFirework[1]->anim_no		= ANIM_STAND;
				ext->pActFirework[1]->anim_ang		= 1;
				ext->pActFirework[1]->actNo			= 0;
				ext->pActFirework[1]->dispPrio		= DISP_PRIO_RESERVE;
				ext->pActFirework[1]->level			= 10;					// level 拿来当计数用
				ext->pActFirework[1]->damage		= ext->pActFirework[0]->damage;
				ext->pActFirework[1]->gx			= ext->pActFirework[0]->bufGx[0] - 2;
				ext->pActFirework[1]->gy			= ext->pActFirework[0]->bufGy[0] - 2;
				ext->pActFirework[1]->bufGx[0]		= ext->pActFirework[0]->bufGx[0];
				ext->pActFirework[1]->bufGy[0]		= ext->pActFirework[0]->bufGy[0];
				ext->pActFirework[0]->damage		= -2;
			}
		}
		if (ext->pActFirework[0]->damage == -2 || warpEffectStart)
		{
			DeathAction(ext->pActFirework[0]);
			ext->pActFirework[0] = NULL;
		}
	}
	if (ext->pActFirework[1])
	{
		float	fX, fY;

		camMapToGamen((float)ext->pActFirework[1]->gx * GRID_SIZE, (float)ext->pActFirework[1]->gy * GRID_SIZE, &fX, &fY);
		ext->pActFirework[1]->x	= (int)fX;
		ext->pActFirework[1]->y	= (int)fY;
		if (pattern(ext->pActFirework[1], ANM_NOMAL_SPD, ANM_NO_LOOP))
		{
			if (--ext->pActFirework[1]->level >= 0)	// 计算还要跑几次
			{
				ext->pActFirework[1]->anim_frame_cnt	= 0;
				ext->pActFirework[1]->anim_cnt			= 0;
				if (ext->pActFirework[1]->damage == 3)
				{
					int nXY[4][2] = { {3,-3}, {2,2}, {-3,3}, {-2,-2}};
					int nDir = ext->pActFirework[1]->level % 4;

					ext->pActFirework[1]->gx	= ext->pActFirework[1]->bufGx[0] + nXY[nDir][0];
					ext->pActFirework[1]->gy	= ext->pActFirework[1]->bufGy[0] + nXY[nDir][1];
				}
			}
			else
			{
				DeathAction(ext->pActFirework[1]);
				ext->pActFirework[1] = NULL;
			}
		}
		if (warpEffectStart)
		{
			DeathAction(ext->pActFirework[1]);
			ext->pActFirework[1] = NULL;
		}
	}
#endif
#ifdef _THEATER
	if (pc.iTheaterMode & 0x00000004)
	{
		int iScore = pc.iTheaterMode >> 16, iDisplayNum;

		if (iScore == 100)
		{
			StockDispBuffer(280, 240, DISP_PRIO_MENU, CG_CNT_DOWN_1, 0);
			StockDispBuffer(310, 240, DISP_PRIO_MENU, CG_CNT_DOWN_0, 0);
			StockDispBuffer(340, 240, DISP_PRIO_MENU, CG_CNT_DOWN_0, 0);
		}
		else
		{
			iDisplayNum = iScore / 10 + CG_CNT_DOWN_0;
			StockDispBuffer(300, 240, DISP_PRIO_MENU, iDisplayNum, 0);
			iDisplayNum = iScore % 10 + CG_CNT_DOWN_0;
			StockDispBuffer(330, 240, DISP_PRIO_MENU, iDisplayNum, 0);
		}
	}
#endif
#ifdef _NPC_MAGICCARD
	{
		float	fX, fY;
		char buf[32];
		int i,color;
		
		if( TimeGetTime() > inextexet ){	
			inextexet = TimeGetTime() + 35;
			for( i=0;i<20;i++){
				if( bShowflag[i] == FALSE ) continue;
				iOffsetY[i]+=1;		
				if( iOffsetY[i] >= 60 ){
					iOffsetY[i] = 0;
					iPosition[i] = 0;
					bShowflag[i] = FALSE;
				}				
			}
		}
		//sprintf_s(buf,"%d",MAX_PAL);//iTotalRunCount);
		//PaletteChange(0,10);
		//StockFontBufferExt( 400 , 240 , FONT_PRIO_FRONT, 5 , buf , 0 ,16);
		//StockDispBuffer( 320 , 228 , DISP_PRIO_MENU , 26274 , 1);
		//StockBoxDispBuffer( 400, 350 , 450, 400, DISP_PRIO_BOX, 249, 0 );

/*
		if( iTotalRunCount % 20 == 0 ){
			iPosition[0] +=1;
			if( iPosition[0] > 44 ){
			 	iPosition[0] = 0;
			}
		}
*/		
		//StockFontBufferExt( 400 , 240-offsetYtbl[iPosition[0]] , FONT_PRIO_FRONT, 6 , "-1" , 0 , 18+iPosition[0] );
		//StockFontBufferExt( 10 , 240 , FONT_PRIO_FRONT, 2 , "-8" , 0 , 18 );

		
		for( i=0;i<20;i++){	
			if( bShowflag[i] == TRUE ){
				//if( iShowdamage[i] == 0) continue;
				if( iShowdamage[i] > 0 ){
					if( iShowdamage[i] == 1111){
						sprintf_s( buf,"成功");
						color = FONT_PAL_BLUE;
					}else if( iShowdamage[i] == 2222 ){
						sprintf_s( buf,"失败");
						color = FONT_PAL_PURPLE;
					}else{
						sprintf_s( buf,"-%d",iShowdamage[i]);
						color = FONT_PAL_PURPLE ;
					}
				}else{ 
					sprintf_s( buf,"+%d",ABS(iShowdamage[i]));
					color = FONT_PAL_GREEN;
				}
#ifdef _MOVE_SCREEN
				if(pc.bMoveScreenMode) 
					camMapToGamen((float)(Positiontable[iPosition[i]].humanX-iScreenMoveX + ioffsetsx) * GRID_SIZE, (float)(Positiontable[iPosition[i]].humanY-iScreenMoveY + ioffsetsy) * GRID_SIZE, &fX, &fY);
				else
					//camMapToGamen((float)Positiontable[iPosition[i]].humanX * GRID_SIZE, (float)Positiontable[iPosition[i]].humanY * GRID_SIZE, &fX, &fY);
#endif
				camMapToGamen((float)(Positiontable[iPosition[i]].humanX + ioffsetsx ) * GRID_SIZE, (float)(Positiontable[iPosition[i]].humanY + ioffsetsy ) * GRID_SIZE, &fX, &fY);
				//StockFontBuffer( (int)fX+30 , (int)fY-20-offsetYtbl[iOffsetY[i]] , FONT_PRIO_FRONT, color , buf , 0 );					
				//StockFontBufferExt( (int)fX-20 , (int)fY , FONT_PRIO_FRONT, color , buf , 0 , 36);
				//StockFontBufferExt( (int)fX-10 , (int)fY-20-offsetYtbl[iOffsetY[i]] , FONT_PRIO_FRONT, color , buf , 0 , 36);
				//StockFontBufferExt( (int)fX-10 , (int)fY-20 , FONT_PRIO_FRONT, color , buf , 0 , 18+iOffsetY[i]);
				//StockFontBufferExt( (int)fX-80+offsetYtbl[iOffsetY[i]] , (int)fY-50 , FONT_PRIO_FRONT, color , buf , 0 , 18+fonttbl[iOffsetY[i]]);
				//StockFontBufferExt( (int)fX+4 , (int)fY-iOffsetY[i] , FONT_PRIO_FRONT, color , buf , 0 , 18+fonttbl[iOffsetY[i]]);
				StockFontBuffer((int)fX+4 , (int)fY-iOffsetY[i] , FONT_PRIO_FRONT, color , buf , 0);
			}				
		}
		
		if( ext->pActMagiccard[0] )
		{
			camMapToGamen((float)ext->pActMagiccard[0]->gx * GRID_SIZE, (float)ext->pActMagiccard[0]->gy * GRID_SIZE, &fX, &fY);
			ext->pActMagiccard[0]->x	= (int)fX;
			ext->pActMagiccard[0]->y	= (int)fY;
			if(pattern( ext->pActMagiccard[0], ANM_NOMAL_SPD, ANM_NO_LOOP))
			{
				DeathAction(ext->pActMagiccard[0]);
				ext->pActMagiccard[0] = NULL;			
			}/*
			if( warpEffectStart ){
				DeathAction(ext->pActMagiccard[0]);
				ext->pActMagiccard[0] = NULL;		
			}*/
		}	
		if( ext->pActMagiccard[1] && ext->pActMagiccard[0] == NULL )
		{
			camMapToGamen((float)ext->pActMagiccard[1]->gx * GRID_SIZE, (float)ext->pActMagiccard[1]->gy * GRID_SIZE, &fX, &fY);
			ext->pActMagiccard[1]->x	= (int)fX;
			ext->pActMagiccard[1]->y	= (int)fY;
			if(pattern( ext->pActMagiccard[1], ANM_NOMAL_SPD, ANM_NO_LOOP))
			{
				DeathAction(ext->pActMagiccard[1]);
				ext->pActMagiccard[1] = NULL;			
			}/*
			if( warpEffectStart ){
				DeathAction(ext->pActMagiccard[1]);
				ext->pActMagiccard[1] = NULL;		
			}*/
		}
		if( ext->pActMagiccard[2] && ext->pActMagiccard[0] == NULL && ext->pActMagiccard[1] == NULL )
		{
			camMapToGamen((float)ext->pActMagiccard[2]->gx * GRID_SIZE, (float)ext->pActMagiccard[2]->gy * GRID_SIZE, &fX, &fY);
			ext->pActMagiccard[2]->x	= (int)fX;
			ext->pActMagiccard[2]->y	= (int)fY;
			if(pattern( ext->pActMagiccard[2], ANM_NOMAL_SPD, ANM_NO_LOOP))
			{
				DeathAction(ext->pActMagiccard[2]);
				ext->pActMagiccard[2] = NULL;			
			}/*
			if( warpEffectStart ){
				DeathAction(ext->pActMagiccard[2]);
				ext->pActMagiccard[2] = NULL;		
			}*/
		}
		if( ext->pActMagiccard[3] && ext->pActMagiccard[0] == NULL && ext->pActMagiccard[1] == NULL && ext->pActMagiccard[2] == NULL )
		{
			camMapToGamen((float)ext->pActMagiccard[3]->gx * GRID_SIZE, (float)ext->pActMagiccard[3]->gy * GRID_SIZE, &fX, &fY);
			ext->pActMagiccard[3]->x	= (int)fX;
			ext->pActMagiccard[3]->y	= (int)fY;
			if(pattern( ext->pActMagiccard[3], ANM_NOMAL_SPD, ANM_NO_LOOP))
			{
				DeathAction(ext->pActMagiccard[3]);
				ext->pActMagiccard[3] = NULL;			
			}
			/*
			if( warpEffectStart ){
				DeathAction(ext->pActMagiccard[3]);
				ext->pActMagiccard[3] = NULL;		
			}*/
		}
		if( warpEffectStart ){
			for( i=0;i<4;i++){
				DeathAction(ext->pActMagiccard[i]);
				ext->pActMagiccard[i] = NULL;
			}
		}
	}
#endif
#ifdef _NPC_PICTURE
	if( ((ptAct->picture>>24) & 0x000000ff) == 2 && ext->ptActPicture == NULL ) {
		if( g_bUseAlpha )
			ext->ptActPicture =
				createCommmonEffectAction( 101812, ptAct->gx, ptAct->gy, 0, 0, DISP_PRIO_CHAR );
		else
			ext->ptActPicture =
				createCommmonEffectAction( 101865, ptAct->gx, ptAct->gy, 0, 0, DISP_PRIO_CHAR );
	}
	else if( ((ptAct->picture>>24) & 0x000000ff) != 2 && ext->ptActPicture != NULL ) {
		// ??????????????
		DeathAction( ext->ptActAngelMark );
		ext->ptActAngelMark = NULL;
	}
	else if( ((ptAct->picture>>24) & 0x000000ff) == 2 && ext->ptActPicture != NULL ) {
		int pictable[9]={100388,100841,101178,100854,101570,100353,101759,101424,101489};
		if( ptAct->anim_chr_no == pictable[((ptAct->picture&0x00ff0000)>>16)&0x000000ff] ){
			//int no;
			//no = (ptAct->anim_chr_no - SPR_001em) / 20;
			//no %= 12;	// ?????????
			// ???????????????


			short x1,y1;
			if(获取动画尺寸(ptAct,&x1,&y1)){
				ext->ptActPicture->x  = ptAct->x;
				ext->ptActPicture->y  = ptAct->y + ptAct->anim_y + 140;
				ext->ptActPicture->mx = ptAct->mx;
				ext->ptActPicture->my = ptAct->my;
				// ?????????
				pattern( ext->ptActPicture, ANM_NOMAL_SPD, ANM_LOOP );
				// ??????????????
				setCharPrio( ext->ptActPicture->bmpNo, ext->ptActPicture->x, ext->ptActPicture->y, 0,ptAct->sFamilyIcon?-(y1/2+_OTHERTEXIAOY_):-(y1/2+_FANILYTEXIAOY_), ext->ptActPicture->mx, ext->ptActPicture->my
#ifdef _SFUMATO
					, ptAct->sfumato
#endif
					);

			}
		}
	}
	/*if( ((ptAct->picture>>24) & 0x000000ff) == 2 ){
		int pictable[9]={100388,100841,101178,100854,101570,100353,101759,101424,101489};
		if( ptAct->anim_chr_no == pictable[((ptAct->picture&0x00ff0000)>>16)&0x000000ff] ){
			int no;
			U4 bmpNo;
			no = (ptAct->anim_chr_no - SPR_001em) / 20;
			no %= 12;	// ?????????
			realGetNo( CG_TRADE_MARK, &bmpNo );
			// ??????????????
			setCharPrio( bmpNo, ptAct->x, ptAct->y, 0, charIconOffsetY[no], ptAct->mx, ptAct->my
			#ifdef _SFUMATO
			, ptAct->sfumato
			#endif
 );
		}
	}
	*/
#endif

}
#ifdef _AniCharBubble	   // Syu ADD 动画层人物吐出气泡
void CharbubbleRelease()
{
	if ( popAct != NULL )
	{
		DeathAction(popAct);
		popAct = NULL;
	}
	popflag = false;
	waittimeflag = false;  
}
#endif
// ?????????????
void setCharNameColor( ACTION *ptAct, int color )
{
	CHAREXTRA *ext;
	int no;
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	no = ext->charObjTblId;
	ptAct->charNameColor = color;
	charObj[no].charNameColor = color;
}

// ???????? /////////////////////////////////////////////////
ACTION *createCommmonEffectAction( int graNo, int gx, int gy, int anim, int dir, int prio )
{
	ACTION *ptAct;
	float mx, my;
	/* ?????????? */
	ptAct = GetAction( PRIO_CHR, 0 );
	if( ptAct == NULL )
		return NULL;
	// ????????
	ptAct->anim_chr_no = graNo;
	// ??
	ptAct->anim_no = anim;
	// ?????????( ??? )( ??????? )
	ptAct->anim_ang = dir;
	// ?
	ptAct->dispPrio = prio;
	// 1????????
	ptAct->atr = ACT_ATR_HIDE2;
	// ???
	ptAct->nextGx = gx;					// ???????????
	ptAct->nextGy = gy;
	ptAct->gx = gx;						// ???????????佋?
	ptAct->gy = gy;
	ptAct->mx = (float)gx * GRID_SIZE;	// ????
	ptAct->my = (float)gy * GRID_SIZE;
	ptAct->vx = 0;						// ??
	ptAct->vy = 0;
	// ??
	camMapToGamen( ptAct->mx, ptAct->my, &mx, &my );
	ptAct->x = (int)(mx+.5);
	ptAct->y = (int)(my+.5);
	return ptAct;
}

// ???????????????? /////////////////////////////////
// ????????????????????????
void commmonEffectNoLoop( ACTION *ptAct )
{
	float mx, my;
	// ??
	camMapToGamen( ptAct->mx, ptAct->my, &mx, &my );
	ptAct->x = (int)(mx+.5);
	ptAct->y = (int)(my+.5);
	// ?????????
	if( pattern( ptAct, ANM_NOMAL_SPD, ANM_NO_LOOP ) == 0 )
		// ??????????????
		setCharPrio( ptAct->bmpNo, ptAct->x, ptAct->y+1, 0, 0, ptAct->mx, ptAct->my
#ifdef _SFUMATO
		, ptAct->sfumato
#endif
 );
	else{
		// ???????????????????
		DeathAction( ptAct );
		ptAct = NULL;
	}
}

ACTION *createCommmonEffectNoLoop( int graNo, int gx, int gy, int anim, int dir, int prio )
{
	ACTION *ptAct;
	float mx, my;
	/* ?????????? */
	ptAct = GetAction( PRIO_CHR, 0 );
	if( ptAct == NULL )
		return NULL;
	// ???
	ptAct->func = commmonEffectNoLoop;
	// ????????
	ptAct->anim_chr_no = graNo;
	// ??
	ptAct->anim_no = anim;
	// ?????????( ??? )( ??????? )
	ptAct->anim_ang = dir;
	// ?
	ptAct->dispPrio = prio;
	// 1????????
	ptAct->atr = ACT_ATR_HIDE2;
	// ???
	ptAct->nextGx = gx;					// ???????????
	ptAct->nextGy = gy;
	ptAct->gx = gx;						// ???????????佋?
	ptAct->gy = gy;
	ptAct->mx = (float)gx * GRID_SIZE;	// ????
	ptAct->my = (float)gy * GRID_SIZE;
	ptAct->vx = 0;						// ??
	ptAct->vy = 0;
	// ??
	camMapToGamen( ptAct->mx, ptAct->my, &mx, &my );
	ptAct->x = (int)(mx+.5);
	ptAct->y = (int)(my+.5);
	return ptAct;
}

// ????? /////////////////////////////////////////////////////////
void changeCharAct( ACTION *ptAct, int x, int y, int dir, int action,
								int effectno, int effectparam1, int effectparam2 )
{
	CHAREXTRA *ext;
	int no;
	int i;
	if( ptAct == NULL )
		return;
	ext = (CHAREXTRA *)ptAct->pYobi;
	no = ext->charObjTblId;
	switch( action ){
		// ??
		case 0:
			ptAct->bufCount = 0;
			ptAct->anim_no = ANIM_STAND;
			ptAct->anim_no_bak = -1;
			ptAct->anim_ang = dir;
			break;

		// ???
		case 31:
			// ????????????????
			// ?????????
			setCharWarpPoint( ptAct, x, y );
			ptAct->bufCount = 0;
			ptAct->anim_no = ANIM_STAND;
			ptAct->anim_no_bak = -1;
			ptAct->anim_ang = dir;
			charObj[no].stockDir = -1;
			break;

		// ??
		case 30:
			// ??佋??????????
			charObj[no].stockDir = dir;
			charObj[no].stockDirX = x;
			charObj[no].stockDirY = y;
			if( (x == ptAct->nextGx && y == ptAct->nextGy) || ABS( ABS( x - ptAct->nextGx ) - ABS( y - ptAct->nextGy ) ) > 1 )
				break;

		// ?
		case 1:
			// NPC?????????????????????
			if( (charObj[no].status & CHR_STATUS_PARTY) != 0 && (charObj[no].status & CHR_STATUS_LEADER) != 0 ){
				// ??????????????
				if( ptAct->bufCount < sizeof( ptAct->bufGx )/sizeof( int ) )
					correctCharMovePoint( ptAct, x, y );
				else{
					// ????????????????????????
					stockCharMovePoint( ptAct, x, y );
					charObj[no].stockDir = -1;
					// ?????????
					for( i = 1; i < MAX_PARTY; i++ ){
						if( party[i].useFlag != 0 ){
							if( party[i].id != pc.id ){
								// ?????
								party[i].ptAct->bufCount = 0;
								setCharWarpPoint( party[i].ptAct, x, y );
							}else{
								// ????
								if( pc.ptAct != NULL )
									pc.ptAct->bufCount = 0;
								setPcWarpPoint( x, y );
							}
						}
					}
				}
			}else
				stockCharMovePoint( ptAct, x, y );
			ptAct->anim_no = ANIM_WALK;
			break;

		// ??
		case 2:
			ptAct->anim_no = ANIM_ATTACK;
			ptAct->anim_no_bak = -1;
			ptAct->anim_ang = dir;
			break;

		// ???
		case 3:
			ptAct->anim_no = ANIM_THROW;
			ptAct->anim_no_bak = -1;
			ptAct->anim_ang = dir;
			break;

		// ????
		case 4:
			ptAct->anim_no = ANIM_DAMAGE;
			ptAct->anim_no_bak = -1;
			ptAct->anim_ang = dir;
			break;

		// ?
		case 5:
			ptAct->anim_no = ANIM_DEAD;
			ptAct->anim_no_bak = -1;
			ptAct->anim_ang = dir;
			break;

		// ??
		case 6:
			break;

		// ?????
		case 7:
			break;

		// ?????
		case 8:		/* UsedMagic */
			break;

		// ??
		case 10:
			ptAct->anim_no = ANIM_DEAD;
			ptAct->anim_no_bak = -1;
			ptAct->anim_ang = dir;
			break;

		// ??
		case 11:
			ptAct->anim_no = ANIM_SIT;
			ptAct->anim_no_bak = -1;
			ptAct->anim_ang = dir;
			break;

		// ??
		case 12:
			ptAct->anim_no = ANIM_HAND;
			ptAct->anim_no_bak = -1;
			ptAct->anim_ang = dir;
			break;

		// ??
		case 13:
			ptAct->anim_no = ANIM_HAPPY;
			ptAct->anim_no_bak = -1;
			ptAct->anim_ang = dir;
			break;

		// ?
		case 14:
			ptAct->anim_no = ANIM_ANGRY;
			ptAct->anim_no_bak = -1;
			ptAct->anim_ang = dir;
			break;

		// ??
		case 15:
			ptAct->anim_no = ANIM_SAD;
			ptAct->anim_no_bak = -1;
			ptAct->anim_ang = dir;
			break;

		// ???
		case 16:
			ptAct->anim_no = ANIM_GUARD;
			ptAct->anim_no_bak = -1;
			ptAct->anim_ang = dir;
			break;

		// ??????
		case 17:
			ptAct->anim_no = ANIM_WALK;
			ptAct->anim_no_bak = -1;
			ptAct->anim_ang = dir;
			break;

		// ????
		case 18:
			ptAct->anim_no = ANIM_NOD;
			ptAct->anim_no_bak = -1;
			ptAct->anim_ang = dir;
			break;

		// ?????????
		case 19:
			ptAct->anim_no = ANIM_STAND;
			ptAct->anim_no_bak = -1;
			ptAct->anim_ang = dir;
			break;

		// 
		case 20:
			if( effectno >= 0 ){
				setCharBattle( ptAct, effectno, effectparam1, effectparam2 );
				// ?????????
				ptAct->anim_no = ANIM_STAND;
				ptAct->anim_no_bak = -1;
			}else
				delCharBattle( ptAct );
			setCharWarpPoint( ptAct, x, y );
			ptAct->anim_ang = dir;
			break;

		// ????
		case 21:
			if( effectno == 1 )
				setCharLeader( ptAct );
			else
				delCharLeader( ptAct );
			ptAct->anim_ang = dir;
			break;

		// ?
		case 22:
			if( effectno == 1 )
				setCharWatch( ptAct );
			else
				delCharWatch( ptAct );
			setCharWarpPoint( ptAct, x, y );
			ptAct->anim_ang = dir;
			break;

		// ????????????
		case 23:
			setCharNameColor( ptAct, effectno );
			break;

		// 交易中
		case 32:
			if( effectno >= 0 ){
				setCharTrade( ptAct, effectno );
				// ?????????
				ptAct->anim_no = ANIM_STAND;
				ptAct->anim_no_bak = -1;
			}else
				delCharTrade( ptAct );
			//setCharWarpPoint( ptAct, x, y );
			ptAct->anim_ang = dir;
			break;

#ifdef _ANGEL_SUMMON
		case 34:
			if( effectno > 0 ){
				setCharAngel( ptAct );
				// ?????????
				ptAct->anim_no = ANIM_STAND;
				ptAct->anim_no_bak = -1;
			}else
				delCharAngel( ptAct );
			//setCharWarpPoint( ptAct, x, y );
			ptAct->anim_ang = dir;
			break;
#endif

#ifdef _STREET_VENDOR
		case 41:
			if(ext->ptStreetVendor){
				DeathAction(ext->ptStreetVendor);
				ext->ptStreetVendor = NULL;
			}
			ext->ptStreetVendor = GetAction(PRIO_CHR,0);
			break;
		case 42:
			if(ext->ptStreetVendor){
				DeathAction(ext->ptStreetVendor);
				ext->ptStreetVendor = NULL;
			}
			break;
#endif
#ifdef _MIND_ICON
		case 40:
			if(effectno >= 0)
				setCharMind( ptAct, effectno);				
			else
				delCharMind( ptAct);
			break;
#endif
#ifdef _SHOWFAMILYBADGE_
		case 43:
			if(effectno >= 0){
				setCharFamily( ptAct, effectno);	
			}else
				delCharFamily( ptAct);
			break;
#endif
#ifdef FAMILY_MANOR_
		case 44:
			if(effectno >= 0)
				setCharmFamily( ptAct, effectno);				
			else
				delCharmFamily( ptAct);
			break;
#endif
#ifdef _CHARTITLE_
		case 45:
			if(effectno >= 0)
				setCharmTitle( ptAct, effectno);				
			else
				delCharmTitle( ptAct);
			break;
#endif
#ifdef _CHAR_MANOR_
		case 46:
			if(effectno >= 0)
				setCharmManor( ptAct, effectno);				
			else
				delCharmManor( ptAct);
			break;
#endif


#ifdef _ITEM_CRACKER
		case 50:
			if(effectno >= 0)
				setCharMind( ptAct, effectno);				
			else
				delCharMind( ptAct);
			break;
#endif
#ifdef _ITEM_FIREWORK
		case 51:
			if (ext->pActFirework[0])
			{
				DeathAction(ext->pActFirework[0]);
				ext->pActFirework[0] = NULL;
			}
			if (ext->pActFirework[1])
			{
				DeathAction(ext->pActFirework[1]);
				ext->pActFirework[1] = NULL;
			}
			ext->pActFirework[0] = GetAction(PRIO_CHR, 0);
			ext->pActFirework[0]->damage		= effectno;	// 借用 damage 来储存 type (effectno = type)
			ext->pActFirework[0]->anim_chr_no	= effectparam1;
			ext->pActFirework[0]->anim_no		= ANIM_STAND;
			ext->pActFirework[0]->anim_ang		= 1;
			ext->pActFirework[0]->dispPrio		= DISP_PRIO_RESERVE;
			if (effectno == 3)
			{
				ext->pActFirework[0]->gx		= ptAct->gx + 3;
				ext->pActFirework[0]->gy		= ptAct->gy - 3;
				ext->pActFirework[0]->bufGx[0]	= pc.ptAct->gx;
				ext->pActFirework[0]->bufGy[0]	= pc.ptAct->gy;
			}
			else
			{
				ext->pActFirework[0]->gx		= ptAct->gx;
				ext->pActFirework[0]->gy		= ptAct->gy;
			}
			ext->pActFirework[0]->actNo			= 0;
	#ifdef _MOVE_SCREEN
			if (pc.bMoveScreenMode)
			{
				ext->pActFirework[0]->gx -= iScreenMoveX;
				ext->pActFirework[0]->gy -= iScreenMoveY;
			}
	#endif
			break;
#endif
#ifdef _NPC_MAGICCARD
		case 60:	
			{
				int i;

				for( i=0;i<4;i++){
					if(ext->pActMagiccard[i]){
						DeathAction(ext->pActMagiccard[i]);
						ext->pActMagiccard[i] = NULL;
					}
				}
				//changePcAct(x, y, dir, 60 , giver , actionNum , petaction)
				//changePcAct( card , damage , dir, 60 , player , actionNum , action);
				//effectno 传来的值代表施放者的位置编号
				if( effectparam2 == 3 ){ //魔法牌
					ext->pActMagiccard[0] = GetAction(PRIO_CHR,0);
					ext->pActMagiccard[0]->anim_chr_no	= effectparam1;
					ext->pActMagiccard[0]->dispPrio	= DISP_PRIO_RESERVE;
					if( effectparam1 == 101120 || effectparam1 == 101628){
						ext->pActMagiccard[0]->gx		= Positiontable[effectno].humanX+1 + x; //.magic1X; 
						ext->pActMagiccard[0]->gy		= Positiontable[effectno].humanY-1 + y; //.magic1Y; 
					}else{
						ext->pActMagiccard[0]->gx		= Positiontable[effectno].petX+1 + x; 
						ext->pActMagiccard[0]->gy		= Positiontable[effectno].petY-1 + y; 
					}
				}else if( effectparam2 == 0 ){ //被封印或是放弃出牌 
					ext->pActMagiccard[0] = GetAction(PRIO_CHR,0);
					ext->pActMagiccard[0]->anim_chr_no	= 101290;
					ext->pActMagiccard[0]->dispPrio	= DISP_PRIO_RESERVE;
					ext->pActMagiccard[0]->gx		= Positiontable[effectno].humanX+1 + x; 
					ext->pActMagiccard[0]->gy		= Positiontable[effectno].humanY-1 + y; 
				}else{
					for( i=0;i<4;i++){
						ext->pActMagiccard[i] = GetAction(PRIO_CHR,0);
						if( i == 0 ){
							ext->pActMagiccard[i]->anim_chr_no	= 101942;
						}else if( i == 3 ){
							ext->pActMagiccard[i]->anim_chr_no	= 101943;
						}else{
							ext->pActMagiccard[i]->anim_chr_no	= effectparam1;
						}
						ext->pActMagiccard[i]->gx		= Positiontable[effectno].petX + x; 
						ext->pActMagiccard[i]->gy		= Positiontable[effectno].petY + y; 
						ext->pActMagiccard[i]->dispPrio	= DISP_PRIO_RESERVE;
						ext->pActMagiccard[i]->anim_ang	= dir;//1;	//方向
					}
					
					ext->pActMagiccard[1]->anim_no		= ANIM_STAND;

					if( effectparam2 == 1 )	//宠牌攻击			
						ext->pActMagiccard[2]->anim_no		= ANIM_ATTACK;
					else if( effectparam2 == 2 )	//宠牌防御	
					 	ext->pActMagiccard[2]->anim_no		= ANIM_GUARD;
				}
#ifdef _MOVE_SCREEN				
				if(pc.bMoveScreenMode)
				{
					for( i=0;i<4;i++){
						if(ext->pActMagiccard[i]){
							ext->pActMagiccard[i]->gx -= iScreenMoveX;
							ext->pActMagiccard[i]->gy -= iScreenMoveY;
						}
					}
				}
#endif				
			}
			break;			
#endif

	}
}

// ID??
// ??： 0? ... ID???? / -1 ... ???
int searchCharObjId( int id )
{
	int i;
	int no = -1;
	for( i = searchCharObj; i < tailCharObj; i++ ){
		if( charObj[i].use != CHAROBJ_USE_FREE && charObj[i].id == id ){
			no = i;
			searchCharObj = i;
			break;
		}
	}
	if( no < 0 && searchCharObj > 0 ){
		for( i = 0; i < searchCharObj; i++ ){
			if( charObj[i].use != CHAROBJ_USE_FREE && charObj[i].id == id ){
				no = i;
				searchCharObj = i;
				break;
			}
		}
	}
	return no;
}

// ???
// ??：TRUE ... ???????
BOOL checkCharObjPoint( int gx, int gy, short type )
{
	int i;
	int no = -1;
	for( i = 0; i < tailCharObj; i++ ){
		if( charObj[i].use != CHAROBJ_USE_FREE && charObj[i].ptAct != NULL ){
			if( charObj[i].ptAct->gx == gx && charObj[i].ptAct->gy == gy && (charObj[i].type & type) != 0 )
				return TRUE;
		}
	}
	return FALSE;
}

// ?????????
// ??：1 ... ??????????????
//         0 ... ????
#ifdef _ANGEL_SUMMON
int checkCharObjPointStatus( int gx, int gy, short type, unsigned status )
#else
int checkCharObjPointStatus( int gx, int gy, short type, unsigned short status )
#endif
{
	int i;
	int no = -1;
	for( i = 0; i < tailCharObj; i++ ){
		if( charObj[i].use != CHAROBJ_USE_FREE && charObj[i].ptAct != NULL ){
			if( charObj[i].ptAct->gx == gx && charObj[i].ptAct->gy == gy && (charObj[i].type & type) != 0 ){
				if( (charObj[i].status & status ) != 0 )
					return 1;
			}
		}
	}
	return 0;
}

// ?????????
// ??：1 ... ????????????????
//         0 ... ????
#ifdef _ANGEL_SUMMON
int checkCharObjPointNotStatus( int gx, int gy, short type, unsigned status )
#else
int checkCharObjPointNotStatus( int gx, int gy, short type, unsigned short status )
#endif
{
	int i;
	int no = -1;
	for( i = 0; i < tailCharObj; i++ ){
		if( charObj[i].use != CHAROBJ_USE_FREE && charObj[i].ptAct != NULL ){
			if( charObj[i].ptAct->gx == gx && charObj[i].ptAct->gy == gy && (charObj[i].type & type) != 0 ){
				if( (charObj[i].status & status ) == 0 )
					return 1;
			}
		}
	}
	return 0;
}

#ifdef _CHAR_PROFESSION			// WON ADD 人物职业
#ifdef _GM_IDENTIFY		// Rog ADD GM识别
void setNpcCharObj( int id, int graNo, int gx, int gy, int dir, char *fmname,
	                        char *name, char *freeName, int level, char *petname, int petlevel,
	                        int nameColor, int walk, int height, int charType, int profession_class, char *gm_name )
#else
#ifdef _NPC_PICTURE
void setNpcCharObj( int id, int graNo, int gx, int gy, int dir, char *fmname,
	                        char *name, char *freeName, int level, char *petname, int petlevel,
	                        int nameColor, int walk, int height, int charType, int profession_class, int picture )
#else							
void setNpcCharObj( int id, int graNo, int gx, int gy, int dir, char *fmname,
	                        char *name, char *freeName, int level, char *petname, int petlevel,
	                        int nameColor, int walk, int height, int charType, int profession_class )
#endif
#endif
#else
#ifdef _NPC_EVENT_NOTICE
void setNpcCharObj( int id, int graNo, int gx, int gy, int dir, char *fmname,
								char *name, char *freeName, int level, char *petname, int petlevel,
								int nameColor, int walk, int height, int charType, int noticeNo
#ifdef _CHARTITLE_STR_
								,char *title
#endif
								)
#else
void setNpcCharObj( int id, int graNo, int gx, int gy, int dir, char *fmname,
								char *name, char *freeName, int level, char *petname, int petlevel,
								int nameColor, int walk, int height, int charType)
#endif
#endif
{
	int no;
	BOOL existFlag = FALSE;
	CHAREXTRA *ext;
#ifdef _ANGEL_SUMMON
	unsigned angelMode =0;
#endif

	if( (no = searchCharObjId( id )) >= 0 ) {
#ifdef _ANGEL_SUMMON
		angelMode = charObj[no].status & CHR_STATUS_ANGEL;
#endif
		delCharObj( id);
	}
	if( !existFlag ){
		no = getCharObjBuf();
		if( no < 0 )
			return;
		charObj[no].ptAct = createCharAction( graNo, gx, gy, dir );
		if( charObj[no].ptAct == NULL )
			return;

		charObj[no].use = CHAROBJ_USE_VIEW;
		ext = (CHAREXTRA *)charObj[no].ptAct->pYobi;
		ext->charObjTblId = no;
		charObj[no].type      = getAtrCharObjType( charType );
		charObj[no].id        = id;
		charObj[no].stockDir  = -1;
	}
#ifdef _CHARTITLE_STR_
	getCharTitleSplit(title,&charObj[no].ptAct->TitleText);
#endif
	charObj[no].graNo     = graNo;
	charObj[no].nameColor = nameColor;
	charObj[no].level     = level;
	charObj[no].gx = gx;
	charObj[no].gy = gy;
	charObj[no].dir = dir;
	if( walk != 0 )
		charObj[no].status |= CHR_STATUS_W;
	if( height != 0 )
		charObj[no].status |= CHR_STATUS_H;
	if( strlen( name ) <= CHAR_NAME_LEN )
		strcpy( charObj[no].name, name );		
	// shan add code
    if( strlen( fmname ) <= CHAR_FMNAME_LEN )
	    strcpy( charObj[no].fmname, fmname );	
	if( charType != CHAR_TYPEPET ){
		if( strlen( freeName ) <= CHAR_FREENAME_LEN )
			strcpy( charObj[no].freeName, freeName );		
	}else{
		if( strlen( freeName ) <= PET_FREENAME_LEN )
			strcpy( charObj[no].freeName, freeName );        		    
	}
	// Robin 0730
    if( strlen( petname ) <= CHAR_FREENAME_LEN )
	    strcpy( charObj[no].petName, petname );	
	charObj[no].petLevel = petlevel;

	charObj[no].charType = getAtrCharType( charType );
	charObj[no].newFoundFlag = 1;

#ifdef _ANGEL_SUMMON
	charObj[no].status |= angelMode;
#endif

	if( charObj[no].ptAct == NULL )
		return;

	charObj[no].ptAct->anim_chr_no = graNo;
	charObj[no].ptAct->level = level;
	charObj[no].ptAct->atr |= charObj[no].charType;
	if( strlen( name ) <= CHAR_NAME_LEN )
		strcpy( charObj[no].ptAct->name, name );
	// shan add
	if( charType == CHAR_TYPEPLAYER){
		if( strlen( fmname) <= CHAR_FMNAME_LEN )
			strcpy( charObj[no].ptAct->fmname, fmname );
	}
	else{
		strcpy( charObj[no].ptAct->fmname, " " );
	}

	if( charType != CHAR_TYPEPET ){
		if( strlen( freeName ) <= CHAR_FREENAME_LEN )
			strcpy( charObj[no].ptAct->freeName, freeName );
	}else{
		if( strlen( freeName ) <= PET_FREENAME_LEN )
			strcpy( charObj[no].ptAct->freeName, freeName );
	}
	// Robin 0730
	if( strlen( petname ) <= CHAR_FREENAME_LEN )
		strcpy( charObj[no].ptAct->petName, petname );
	charObj[no].ptAct->petLevel = petlevel;

	charObj[no].ptAct->itemNameColor = nameColor;

#ifdef _CHAR_PROFESSION			// WON ADD 人物职业
	charObj[no].ptAct->profession_class = profession_class;
#endif
#ifdef _NPC_PICTURE
	charObj[no].ptAct->picture = picture;
	charObj[no].ptAct->picturetemp = ((((picture)&0x00ff0000)>>16)&0x000000ff);//(picture&0x0000ffff);
	//charObj[no].ptAct->anim_no = ((((picture)&0x0000ff00)>>8 )&0x000000ff);
#endif
#ifdef _GM_IDENTIFY		// Rog ADD GM识别
    if( strlen( gm_name ) <= GM_NAME_LEN )
	    strcpy( charObj[no].ptAct->gm_name, gm_name );	
#endif
#ifdef _NPC_EVENT_NOTICE
	charObj[no].ptAct->noticeNo = noticeNo;
#endif
}



BOOL setReturnPetObj( int id, int graNo, int gx, int gy, int dir, 
char *name, char *freeName, int level, int nameColor, int walk, int height, int charType )
{
	int no;
	// ???????????
	if( (no = searchCharObjId( id )) >= 0 )
		// ?????
		return FALSE;
	// ????????????
	no = getCharObjBuf();
	if( no < 0 )
		return FALSE;	// ????????????
	charObj[no].ptAct = NULL;
	charObj[no].use = CHAROBJ_USE_VIEW;
	charObj[no].type      = getAtrCharObjType( charType );
	charObj[no].id        = id;
	charObj[no].graNo     = graNo;
	charObj[no].nameColor = nameColor;
	charObj[no].level     = level;
	charObj[no].gx = gx;
	charObj[no].gy = gy;
	charObj[no].dir = dir;
	if( walk != 0 )
		charObj[no].status |= CHR_STATUS_W;
	if( height != 0 )
		charObj[no].status |= CHR_STATUS_H;
	if( strlen( name ) <= CHAR_NAME_LEN )
		strcpy( charObj[no].name, name );
	if( strlen( freeName ) <= PET_FREENAME_LEN )
		strcpy( charObj[no].freeName, freeName );
	charObj[no].charType = getAtrCharType( charType );
	return TRUE;
}

//
//   ?????
//
void setItemCharObj( int id, int graNo, int gx, int gy, int dir, int classNo, char *info )
{
	int no;
	BOOL existFlag = FALSE;
	CHAREXTRA *ext;
	// ???????????
	if( (no = searchCharObjId( id )) >= 0 )
		existFlag = TRUE;
	// ????????????
	if( !existFlag ){
		no = getCharObjBuf();
		if( no < 0 )
			return;	// ????????????
		charObj[no].ptAct = createCharAction( graNo, gx, gy, dir );
		if( charObj[no].ptAct == NULL )
			// ?????????????????????
			return;
		charObj[no].use = CHAROBJ_USE_VIEW;
		ext = (CHAREXTRA *)charObj[no].ptAct->pYobi;
		ext->charObjTblId = no;
		charObj[no].type      = CHAROBJ_TYPE_ITEM;
		charObj[no].id        = id;
		charObj[no].stockDir  = -1;
	}
	charObj[no].graNo   = graNo;
	charObj[no].classNo = classNo;
	charObj[no].gx = gx;
	charObj[no].gy = gy;
	charObj[no].dir = dir;
	if( strlen( info ) <= 60 )
		strcpy( charObj[no].info, info );
	charObj[no].charType = ACT_ATR_TYPE_ITEM;
	charObj[no].newFoundFlag = 0;	// ?????????
	// ??????????????
	if( charObj[no].ptAct == NULL )
		return;
	charObj[no].ptAct->anim_chr_no = graNo;
	// 1??????????name???
	if( strlen( info ) <= ITEM_NAME_LEN )
		strcpy( charObj[no].ptAct->name, info );
	else
		strcpy( charObj[no].ptAct->name, "???" );
	charObj[no].ptAct->atr |= ACT_ATR_TYPE_ITEM;
	charObj[no].ptAct->itemNameColor = classNo;
}

//
//   ???
//
void setMoneyCharObj( int id, int graNo, int gx, int gy, int dir, int money, char *info )
{
	int no;
	BOOL existFlag = FALSE;
	CHAREXTRA *ext;
	// ???????????
	if( (no = searchCharObjId( id )) >= 0 )
		existFlag = TRUE;
	// ????????????
	if( !existFlag ){
		no = getCharObjBuf();
		if( no < 0 )
			return;	// ????????????
		charObj[no].ptAct = createCharAction( graNo, gx, gy, dir );
		if( charObj[no].ptAct == NULL )
			// ?????????????????????
			return;
		charObj[no].use = CHAROBJ_USE_VIEW;
		ext = (CHAREXTRA *)charObj[no].ptAct->pYobi;
		ext->charObjTblId = no;
		charObj[no].type      = CHAROBJ_TYPE_MONEY;
		charObj[no].id        = id;
		charObj[no].stockDir  = -1;
	}
	charObj[no].graNo = graNo;
	charObj[no].money = money;
	charObj[no].gx = gx;
	charObj[no].gy = gy;
	charObj[no].dir = dir;
	if( strlen( info ) <= 60 )
		strcpy( charObj[no].info, info );
	charObj[no].charType = ACT_ATR_TYPE_GOLD;
	charObj[no].newFoundFlag = 0;	// ?????????
	// ??????????????
	if( charObj[no].ptAct == NULL )
		return;
	charObj[no].ptAct->anim_chr_no = graNo;
	// 1??????????name???
	if( strlen( info ) <= CHAR_NAME_LEN )
		strcpy( charObj[no].ptAct->name, info );
	else
		strcpy( charObj[no].ptAct->name, "???" );
	charObj[no].ptAct->atr |= ACT_ATR_TYPE_GOLD;
}

// ??????????
//  ??：charObj????
//          -1 ... ???????
int getCharObjBuf( void )
{
	int i;
	int ret;
	// ????????????
	if( maxCharObj >= MAX_CHAROBJ )
		return -1;
	ret = freeCharObj;
	// ??????
	maxCharObj++;
	// ?????
	if( freeCharObj+1 > tailCharObj )
		tailCharObj = freeCharObj+1;
	// ???????????
	for( i = freeCharObj+1; i < MAX_CHAROBJ; i++ ){
		if( charObj[i].use == CHAROBJ_USE_FREE ){
			freeCharObj = i;
			break;
		}
	}
	if( freeCharObj > 0 && i >= MAX_CHAROBJ ){
		for( i = 0; i < freeCharObj; i++ ){
			if( charObj[i].use == CHAROBJ_USE_FREE ){
				freeCharObj = i;
				break;
			}
		}
	}
	return ret;
}

// ?????????????????
//  ?  ?：type ... ?????????????
//  ??：??????atr???
int getAtrCharType( int type )
{
	switch( type ){
		// ???
		case CHAR_TYPEPLAYER:
		case CHAR_TYPEBUS:
			return ACT_ATR_TYPE_OTHER_PC;
		// ???
		case CHAR_TYPEPET:
			return ACT_ATR_TYPE_PET;
		// ??
		default:
			return ACT_ATR_TYPE_OTHER;
	}
	return 0;
}

// ??????????????????
//  ?  ?：type ... ?????????????
//  ??：charObj?type???
int getAtrCharObjType( int type )
{
	int ret;
	switch( type ){
		case CHAR_TYPEPLAYER:
			ret = CHAROBJ_TYPE_USER_NPC;
			break;
		case CHAR_TYPEDENGON:
		case CHAR_TYPEDUELRANKING:
		case CHAR_TYPEMSG:
			ret = CHAROBJ_TYPE_NPC | CHAROBJ_TYPE_LOOKAT;
			break;
		case CHAR_TYPEBUS:
			ret = CHAROBJ_TYPE_NPC | CHAROBJ_TYPE_PARTY_OK;
			break;
		default:
			ret = CHAROBJ_TYPE_NPC;
			break;
	}
	return ret;
}

// ID??????????????
// ?????NULL
ACTION *getCharObjAct( int id )
{
	int no;
	if( (no = searchCharObjId( id )) < 0 )
		return NULL;
	if( charObj[no].ptAct == NULL )
		return NULL;
	return charObj[no].ptAct;
}


// ????????????
void delCharObj( int id )
{
	int i;
	int no;
	CHAREXTRA *ext;

	if( pc.id == id ){
		resetPc();
		return;
	}
	no = searchCharObjId( id );
	// ID???????
	if( no >= 0 ){
		if( charObj[no].ptAct != NULL ){
			ext = (CHAREXTRA *)charObj[no].ptAct->pYobi;
			if( ext != NULL ){
				if( ext->ptActLeaderMark != NULL ){
					DeathAction( ext->ptActLeaderMark );
					ext->ptActLeaderMark = NULL;
				}
				if( ext->ptActMagicEffect != NULL ){
					DeathAction( ext->ptActMagicEffect );
					ext->ptActMagicEffect = NULL;
				}
#ifdef __EMOTION
				if( ext->ptActEmotion != NULL ){
					DeathAction( ext->ptActEmotion );
					ext->ptActEmotion = NULL;
				}
#endif
#ifdef _STREET_VENDOR
				if(ext->ptStreetVendor != NULL){
					DeathAction(ext->ptStreetVendor);
					ext->ptStreetVendor = NULL;
				}
#endif
#ifdef _MIND_ICON
				if( ext->ptMindIcon != NULL ){
					DeathAction( ext->ptMindIcon );
					ext->ptMindIcon = NULL;
				}
#endif
#ifdef _SHOWFAMILYBADGE_
				if( ext->ptFamilyIcon != NULL ){
					DeathAction( ext->ptFamilyIcon );
					ext->ptFamilyIcon = NULL;
				}
#endif
#ifdef _CHARTITLE_
				if( ext->ptTitleIcon != NULL ){
					DeathAction( ext->ptTitleIcon );
					ext->ptTitleIcon = NULL;
				}
#endif
#ifdef FAMILY_MANOR_
				if( ext->ptmFamilyIcon != NULL ){
					DeathAction( ext->ptmFamilyIcon );
					ext->ptmFamilyIcon = NULL;
				}
#endif
#ifdef _CHAR_MANOR_
				if( ext->ptmManorIcon != NULL ){
					DeathAction( ext->ptmManorIcon );
					ext->ptmManorIcon = NULL;
				}
#endif
#ifdef _ITEM_FIREWORK
				if (ext->pActFirework[0] != NULL)
					DeathAction(ext->pActFirework[0]);
				if (ext->pActFirework[1] != NULL)
					DeathAction(ext->pActFirework[1]);
				ext->pActFirework[0] = NULL;
				ext->pActFirework[1] = NULL;
#endif
#ifdef _NPC_MAGICCARD
				for( i=0;i<4;i++) {
					if( ext->pActMagiccard[i] != NULL ){
						DeathAction( ext->pActMagiccard[i] );
						ext->pActMagiccard[i] = NULL;
					}
				}
#endif
			}
			DeathAction( charObj[no].ptAct );
			charObj[no].ptAct = NULL;
		}
		charObj[no].use = CHAROBJ_USE_FREE;
		charObj[no].status = 0;
		charObj[no].stockDir = -1;
		// ???
		if( maxCharObj > 0 )
			maxCharObj--;
		// ????????????????????
		if( freeCharObj > no )
			freeCharObj = no;
		// ??????????
		for( i = tailCharObj; i > 0; i-- ){
			if( charObj[i-1].use != CHAROBJ_USE_FREE ){
				tailCharObj = i;
				break;
			}
		}
	}
}


// ????????????
void initCharObj( void )
{
	int i;

	maxCharObj = 0;
	tailCharObj = 0;
	freeCharObj = 0;
	for( i = 0; i < MAX_CHAROBJ; i++ )
	{
		charObj[i].use = CHAROBJ_USE_FREE;
		charObj[i].ptAct = NULL;
		charObj[i].status = 0;
		charObj[i].id = 0;
		charObj[i].stockDir = -1;
		charObj[i].name[0] = '\0';
	}

	searchCharObj = 0;
}


// ?????????????
void resetCharObj( void )
{
	int i;
	CHAREXTRA *ext;

	maxCharObj = 0;
	tailCharObj = 0;
	freeCharObj = 0;
	for( i = 0; i < MAX_CHAROBJ; i++ )
	{
		if( charObj[i].use != CHAROBJ_USE_FREE )
		{
			if( charObj[i].ptAct != NULL )
			{
				ext = (CHAREXTRA *)charObj[i].ptAct->pYobi;
				if( ext != NULL )
				{
					if( ext->ptActLeaderMark != NULL )
					{
						DeathAction( ext->ptActLeaderMark );
						ext->ptActLeaderMark = NULL;
					}
					if( ext->ptActMagicEffect != NULL )
					{
						DeathAction( ext->ptActMagicEffect );
						ext->ptActMagicEffect = NULL;
					}
#ifdef __EMOTION
					if( ext->ptActEmotion != NULL ){
						DeathAction( ext->ptActEmotion );
						ext->ptActEmotion = NULL;
					}
#endif
#ifdef _STREET_VENDOR
					if(ext->ptStreetVendor != NULL){
						DeathAction(ext->ptStreetVendor);
						ext->ptStreetVendor = NULL;
					}
#endif
#ifdef _MIND_ICON
    				if( ext->ptMindIcon != NULL ){
	    				DeathAction( ext->ptMindIcon );
		   			    ext->ptMindIcon = NULL;
					}
#endif
#ifdef _SHOWFAMILYBADGE_
					if( ext->ptFamilyIcon != NULL ){
						DeathAction( ext->ptFamilyIcon );
						ext->ptFamilyIcon = NULL;
					}
#endif
#ifdef _CHARTITLE_
					if( ext->ptTitleIcon != NULL ){
						DeathAction( ext->ptTitleIcon );
						ext->ptTitleIcon = NULL;
					}
#endif
#ifdef FAMILY_MANOR_
					if( ext->ptmFamilyIcon != NULL ){
						DeathAction( ext->ptmFamilyIcon );
						ext->ptmFamilyIcon = NULL;
					}
#endif
#ifdef _CHAR_MANOR_
					if( ext->ptmManorIcon != NULL ){
						DeathAction( ext->ptmManorIcon );
						ext->ptmManorIcon = NULL;
					}
#endif
#ifdef _NPC_EVENT_NOTICE
					if( ext->ptNoticeIcon != NULL ){
						DeathAction( ext->ptNoticeIcon );
						ext->ptNoticeIcon = NULL;
					}
#endif
#ifdef _ITEM_FIREWORK
					if (ext->pActFirework[0] != NULL)
						DeathAction(ext->pActFirework[0]);
					if (ext->pActFirework[1] != NULL)
						DeathAction(ext->pActFirework[1]);
					ext->pActFirework[0] = NULL;
					ext->pActFirework[1] = NULL;
#endif
#ifdef _NPC_MAGICCARD					
					if (ext->pActMagiccard[0] != NULL)
						DeathAction(ext->pActMagiccard[0]);
					if (ext->pActMagiccard[1] != NULL)
						DeathAction(ext->pActMagiccard[1]);
					if (ext->pActMagiccard[2] != NULL)
						DeathAction(ext->pActMagiccard[2]);
					if (ext->pActMagiccard[3] != NULL)
						DeathAction(ext->pActMagiccard[3]);
					ext->pActMagiccard[0] = NULL;
					ext->pActMagiccard[1] = NULL;
					ext->pActMagiccard[2] = NULL;
					ext->pActMagiccard[3] = NULL;					
#endif
				}
				DeathAction( charObj[i].ptAct );
				charObj[i].ptAct = NULL;
			}
			charObj[i].use = CHAROBJ_USE_FREE;
			charObj[i].status = 0;
			charObj[i].id = 0;
			charObj[i].stockDir = -1;
		}
	}

	searchCharObj = 0;
}


// ???????????????????????
void clearPtActCharObj( void )
{
	int i;

	for( i = 0; i < MAX_CHAROBJ; i++ )
	{
		if( charObj[i].use != CHAROBJ_USE_FREE )
		{
			charObj[i].ptAct = NULL;
			charObj[i].stockDir = -1;
		}
	}
}


// ??????????????????????????
void restorePtActCharObjAll( void )
{
	int i;
	CHAREXTRA *ext;

	for( i = 0; i < MAX_CHAROBJ; i++ )
	{
		if( charObj[i].use != CHAROBJ_USE_FREE
		 && charObj[i].ptAct == NULL )
		{
			charObj[i].ptAct =
				createCharAction( charObj[i].graNo, charObj[i].gx, charObj[i].gy, charObj[i].dir );
			if( charObj[i].ptAct == NULL )
			{
				continue;
			}

			charObj[i].use = CHAROBJ_USE_VIEW;

			ext = (CHAREXTRA *)charObj[i].ptAct->pYobi;
			ext->charObjTblId = i;

			// NPC????????
			// ?PC????????
			// ?
			if( charObj[i].type == CHAROBJ_TYPE_NPC
			 || charObj[i].type == CHAROBJ_TYPE_USER_NPC )
			{
				charObj[i].ptAct->level = charObj[i].level;
				strcpy( charObj[i].ptAct->name, charObj[i].name );
				charObj[i].ptAct->atr |= charObj[i].charType;
			}
			else
			// Item????????
			if( charObj[i].type == CHAROBJ_TYPE_ITEM )
			{
				// 1??????????name???
				if( strlen( charObj[i].info ) <= ITEM_NAME_LEN )
				{
					strcpy( charObj[i].ptAct->name, charObj[i].info );
				}
				else
				{
					strcpy( charObj[i].ptAct->name, "???" );
				}
				charObj[i].ptAct->atr |= charObj[i].charType;
			}
			else
			// ??????????
			if( charObj[i].type == CHAROBJ_TYPE_MONEY )
			{
				// 1??????????name???
				if( strlen( charObj[i].info ) <= CHAR_NAME_LEN )
				{
					strcpy( charObj[i].ptAct->name, charObj[i].info );
				}
				else
				{
					strcpy( charObj[i].ptAct->name, "???" );
				}
				charObj[i].ptAct->atr |= charObj[i].charType;
			}
		}
	}
}


// ?????????????????????????
void restorePtActCharObj( int id )
{
	int no;
	CHAREXTRA *ext;

	if( id < 0 )
		return;

	// ???????????
	if( (no = searchCharObjId( id )) < 0 )
	{
		// ??????
		return;
	}

	if( charObj[no].use != CHAROBJ_USE_FREE
	 && charObj[no].ptAct == NULL )
	{
		charObj[no].ptAct =
			createCharAction( charObj[no].graNo, charObj[no].gx, charObj[no].gy, charObj[no].dir );
		if( charObj[no].ptAct == NULL )
		{
			// ???????????????
			return;
		}

		charObj[no].use = CHAROBJ_USE_VIEW;

		ext = (CHAREXTRA *)charObj[no].ptAct->pYobi;
		ext->charObjTblId = no;

		charObj[no].ptAct->level = charObj[no].level;
		strcpy( charObj[no].ptAct->name, charObj[no].name );
		charObj[no].ptAct->atr |= charObj[no].charType;
	}
}


// ?????
void setMovePointCharObj( int id, int nextGx, int nextGy )
{
	int no;

	no = searchCharObjId( id );
	if( no >= 0 )
	{
		stockCharMovePoint( charObj[no].ptAct, nextGx, nextGy );
	}
}




///////////////////////////////////////////////////////////////////////////
// ???????????????????????????

typedef struct
{
	unsigned int bmpNo;
	int gx;
	int gy;
} ITEM_OVERLAP_CHECK;

#define MAX_ITEM_OVERLAP	100
ITEM_OVERLAP_CHECK itemOverlapTbl[MAX_ITEM_OVERLAP];
int itemOverlapCheckCnt;

void initItemOverlapCheck( void )
{
	itemOverlapCheckCnt = 0;
}


// ??： TRUE  ... ????
//          FALSE ... ??
BOOL itemOverlapCheck( unsigned int bmpNo, int gx, int gy )
{
	int i;
	BOOL flag = FALSE;
	for( i = 0; i < itemOverlapCheckCnt; i++ ){
		if( itemOverlapTbl[i].bmpNo == bmpNo
		 && itemOverlapTbl[i].gx == gx
		 && itemOverlapTbl[i].gy == gy ){
			flag = TRUE;
			break;
		}
	}
	if( !flag ){
		if( itemOverlapCheckCnt < MAX_ITEM_OVERLAP ){
			itemOverlapTbl[itemOverlapCheckCnt].bmpNo = bmpNo;
			itemOverlapTbl[itemOverlapCheckCnt].gx    = gx;
			itemOverlapTbl[itemOverlapCheckCnt].gy    = gy;
			itemOverlapCheckCnt++;
		}
	}
	return flag;
}






///////////////////////////////////////////////////////////////////////////
// ????????


// ??????????????
void limitCantClientDir( short *dir )
{
	// dir??????????????
	if( ((*dir) % 2) == 0 )
	{
		(*dir)++;
	}
}

// dir? 0 ?? 7 ???????
void ajustClientDir( short *dir )
{
	if( *dir < 0 )
	{
		do
		{
			(*dir) += 8;
		} while( *dir < 0 );
	}
	else
	if( *dir > 7 )
	{
		do
		{
			(*dir) -= 8;
		} while( *dir > 7 );
	}
}


void getPetRoute( ACTION *ptAct )
{
	short dir;
	int i;
	int dx, dy;
	int gx, gy;
	PETEXTRA *pe;

	if( ptAct->bufCount > 0 )
		return;

	// ????????????
	if( nowGx != oldGx || nowGy != oldGy )
		return;

	pe = (PETEXTRA *)ptAct->pYobi;

	dir = pe->moveDir;

	for( i = 0; i < 4; i++ )
	{
		getRouteData( dir, &dx, &dy );
		gx = ptAct->gx+dx;
		gy = ptAct->gy+dy;
		if( (gx != pe->preGx || gy != pe->preGy)
		 && checkHitMap( gx, gy ) == FALSE )
		{
			break;
		}
		dir += (2*(i+1));
		ajustClientDir( &dir );
	}

	if( i >= 4 )
	{
		pe->preGx = -1;
		pe->preGy = -1;
		pe->moveDir -= 2;
		ajustClientDir( &pe->moveDir );
		return;
	}

	if( pe->dirCnt >= 2 )
	{
		pe->dirCnt = 0;
		pe->moveDir -= 6;
		ajustClientDir( &pe->moveDir );
	}

	if( dir != pe->preDir )
	{
		pe->dirCnt++;
	}
	else
	{
		pe->dirCnt = 0;
	}

	pe->preDir = dir;

	ptAct->bufGx[ptAct->bufCount] = gx;
	ptAct->bufGy[ptAct->bufCount] = gy;
	ptAct->bufCount++;

	pe->preGx = ptAct->gx;
	pe->preGy = ptAct->gy;

	return;
}


// ???
BOOL petMoveProc( ACTION *ptAct )
{
	PETEXTRA *pe = (PETEXTRA *)ptAct->pYobi;

	// ??????????
	// ???8??????
	if( ABS( ptAct->gx - nowGx ) >= 13
	 || ABS( ptAct->gy - nowGy ) >= 13
	 || (pe->createTime+8000 < TimeGetTime() && ptAct->vx == 0 && ptAct->vy == 0) )
	{
		restorePtActCharObj( pe->id );
		DeathAction( ptAct );
		ptAct = NULL;
		return FALSE;
	}

	if( pe->ptAct == NULL && pe->createTime+7200 < TimeGetTime() )
	{
		pe->ptAct = createCommmonEffectNoLoop( SPR_difence, ptAct->gx, ptAct->gy,
			0, 0, ptAct->dispPrio );
	}
	if( pe->ptAct2 == NULL && pe->createTime+7500 < TimeGetTime() )
	{
		pe->ptAct2 = createCommmonEffectNoLoop( SPR_difence, ptAct->gx, ptAct->gy,
			0, 0, ptAct->dispPrio );
	}


	if( ptAct->vx == 0 && ptAct->vy == 0 )
	{
		// ????????????
		getPetRoute( ptAct );
	}
	charMove( ptAct );

	if( pe->ptAct != NULL )
	{
		pe->ptAct->mx = ptAct->mx;
		pe->ptAct->my = ptAct->my;
	}
	if( pe->ptAct2 != NULL )
	{
		pe->ptAct2->mx = ptAct->mx;
		pe->ptAct2->my = ptAct->my;
	}

	return TRUE;
}


// ????
BOOL uprisePetProc( ACTION *ptAct )
{
	PETEXTRA *pe = (PETEXTRA *)ptAct->pYobi;

	if( pe->ptAct == NULL && pe->createTime < TimeGetTime() )
	{
		pe->ptAct = createCommmonEffectNoLoop( SPR_difence, ptAct->gx, ptAct->gy,
			0, 0, ptAct->dispPrio );
	}
	if( pe->ptAct2 == NULL && pe->createTime+500 < TimeGetTime() )
	{
		pe->ptAct2 = createCommmonEffectNoLoop( SPR_difence, ptAct->gx, ptAct->gy,
			0, 0, ptAct->dispPrio );
	}

	// ???????????
	if( pe->createTime+1200 < TimeGetTime() )
	{
		restorePtActCharObj( pe->id );
		DeathAction( ptAct );
		ptAct = NULL;
	}

	return FALSE;
}


// ??????????
BOOL petCircleOutProc( ACTION *ptAct )
{
	PETEXTRA *pe = (PETEXTRA *)ptAct->pYobi;
	float mx, my;
	float dx, dy;
	float angle;
	int i;

	if( pe->ptAct == NULL )
	{
		pe->ptAct = (ACTION *)1;
		pe->angle = 0.0F;
		pe->r = 0;
		createCommmonEffectNoLoop( SPR_hoshi, ptAct->gx, ptAct->gy,
			0, 0, ptAct->dispPrio );
	}

	if( pe->r > 800 )
	{
		restorePtActCharObj( pe->id );
		DeathAction( ptAct );
		ptAct = NULL;
		return FALSE;
	}

	if( pe->ptAct != NULL && pe->createTime+1000 < TimeGetTime() )
	{
		pe->r += 4;
		pe->angle += 6.0F;
	}

	angle = pe->angle;
	for( i = 0; i < 3; i++ )
	{
		dx = (float)pe->r * CosT( angle );
		dy = (float)pe->r * SinT( angle );

		// ??
		camMapToGamen( ptAct->mx+dx, ptAct->my+dy, &mx, &my );
		ptAct->x = (int)(mx+.5);
		ptAct->y = (int)(my+.5);

		// ?????????
		pattern( ptAct, ANM_NOMAL_SPD, ANM_LOOP );

		// ??????????????
		setCharPrio( ptAct->bmpNo, ptAct->x, ptAct->y, 0, 0, ptAct->mx+dx, ptAct->my+dy
#ifdef _SFUMATO
			, ptAct->sfumato
#endif
 );
		angle += 120;
		AdjustDir( &angle );
	}

	return FALSE;
}


// ?????????
BOOL petCircleInProc( ACTION *ptAct )
{
	PETEXTRA *pe = (PETEXTRA *)ptAct->pYobi;
	float mx, my;
	float dx, dy;
	float angle;
	int i;

	if( pe->ptAct == NULL )
	{
		pe->ptAct  = (ACTION *)1;
		pe->ptAct2 = (ACTION *)NULL;
		pe->angle = 0.0F;
		pe->r = 800;
	}
	else
	{
		if( pe->r <= 0 )
		{
			restorePtActCharObj( pe->id );
			DeathAction( ptAct );
			ptAct = NULL;
			return TRUE;
		}
	}

	if( pe->ptAct != NULL )
	{
		if( pe->ptAct2 == NULL && pe->createTime+2300 < TimeGetTime() )
		{
			pe->ptAct2 = createCommmonEffectNoLoop( SPR_hoshi, ptAct->gx, ptAct->gy,
				0, 0, ptAct->dispPrio );
		}

		pe->r -= 4;
		pe->angle += 6.0F;

		angle = pe->angle;
		for( i = 0; i < 3; i++ )
		{
			dx = (float)pe->r * CosT( angle );
			dy = (float)pe->r * SinT( angle );

			// ??
			camMapToGamen( ptAct->mx+dx, ptAct->my+dy, &mx, &my );
			ptAct->x = (int)(mx+.5);
			ptAct->y = (int)(my+.5);

			// ?????????
			pattern( ptAct, ANM_NOMAL_SPD, ANM_LOOP );

			// ??????????????
			setCharPrio( ptAct->bmpNo, ptAct->x, ptAct->y, 0, 0, ptAct->mx+dx, ptAct->my+dy
#ifdef _SFUMATO
				, ptAct->sfumato
#endif
 );
			angle += 120;
			AdjustDir( &angle );
		}
	}

	return FALSE;
}


// ??????????????
void petProc( ACTION *ptAct )
{
	float mx, my;
	int animLoop;
	PETEXTRA *pe = (PETEXTRA *)ptAct->pYobi;

	switch( pe->mode )
	{
		// ?????：?????
		case 0:
			if( !petMoveProc( ptAct ) )
			{
				return;
			}
			break;

		// ?????：
		// ???????????：?
		case 1:
			if( !uprisePetProc( ptAct ) )
			{
				return;
			}
			break;

		// ?????：?????
		case 2:
			if( !petCircleOutProc( ptAct ) )
			{
				return;
			}
			break;

		// ????：??????
		case 3:
			if( !petCircleInProc( ptAct ) )
			{
				return;
			}
			break;
	}

	// ??
	camMapToGamen( ptAct->mx, ptAct->my, &mx, &my );
	ptAct->x = (int)(mx+.5);
	ptAct->y = (int)(my+.5);

	// ?????????
	if( ptAct->anim_no == ANIM_HAND
	 || ptAct->anim_no == ANIM_HAPPY
	 || ptAct->anim_no == ANIM_ANGRY
	 || ptAct->anim_no == ANIM_SAD
	 || ptAct->anim_no == ANIM_WALK
	 || ptAct->anim_no == ANIM_STAND
	 || ptAct->anim_no == ANIM_NOD )
	{
		animLoop = ANM_LOOP;
	}
	else
	{
		animLoop = ANM_NO_LOOP;
	}
	pattern( ptAct, ANM_NOMAL_SPD, animLoop );

	// ??????????????
	setCharPrio( ptAct->bmpNo, ptAct->x, ptAct->y, 0, 0, ptAct->mx, ptAct->my
#ifdef _SFUMATO
		, ptAct->sfumato
#endif
 );
}



// ??????????
ACTION *createPetAction( int graNo, int gx, int gy, int dir, int mode, int moveDir, int id )
{
	ACTION *ptAct;
	float mx, my;
	PETEXTRA *pe;

	/* ?????????? */
	ptAct = GetAction( PRIO_CHR, sizeof( PETEXTRA ) );
	if( ptAct == NULL )
		return NULL;

	// ???
	ptAct->func = petProc;
	// ????????
	ptAct->anim_chr_no = graNo;
	// ??
	ptAct->anim_no = ANIM_STAND;
	// ?????????( ??? )( ??????? )
	ptAct->anim_ang = dir;
	// ?
	ptAct->dispPrio = DISP_PRIO_CHAR;
	// 1????????
	ptAct->atr = ACT_ATR_INFO |	ACT_ATR_HIT | ACT_ATR_HIDE2;
	// ???
	ptAct->nextGx = gx;					// ???????????
	ptAct->nextGy = gy;
	ptAct->bufCount = 0;
	ptAct->gx = gx;						// ???????????佋?
	ptAct->gy = gy;
	ptAct->mx = (float)gx * GRID_SIZE;	// ????
	ptAct->my = (float)gy * GRID_SIZE;
	ptAct->vx = 0;						// ??
	ptAct->vy = 0;

	// ??
	camMapToGamen( ptAct->mx, ptAct->my, &mx, &my );
	ptAct->x = (int)(mx+.5);
	ptAct->y = (int)(my+.5);

	pe = (PETEXTRA *)ptAct->pYobi;
	pe->mode = mode;
	pe->moveDir = moveDir;
	limitCantClientDir( &pe->moveDir );
	pe->preDir = pe->moveDir;
	pe->dirCnt = 0;
	pe->preGx = gx;
	pe->preGy = gy;
	pe->walkCnt = 0;
	pe->createTime = TimeGetTime();
	pe->ptAct  = NULL;
	pe->ptAct2 = NULL;
	pe->angle = 0.0F;
	pe->r = 0;

	pe->id = id;

	return ptAct;
}



void 设置静止的动作为站立( void )
{
	int i=0;
	for(i;i<MAX_CHAROBJ;i++){
		if(charObj[i].charType==256){
			if(charObj[i].ptAct){
				if(charObj[i].ptAct->anim_no == 5 ||
					charObj[i].ptAct->anim_no==2||
					charObj[i].ptAct->anim_no==0||
					charObj[i].ptAct->anim_no==1||
					charObj[i].ptAct->anim_no==12)
						charObj[i].ptAct->anim_no = 3;
			}
		}
	}
}



