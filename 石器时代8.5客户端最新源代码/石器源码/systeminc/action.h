/************************/
/*	action.h			*/
/************************/
#ifndef _ACTION_H_
#define _ACTION_H_

#include "version.h"
#include "../newproto/autil.h"
//处理优先顺序
enum{
	PRIO_TOP,			//最优先

	PRIO_CHR,			//同Char

	PRIO_BG,			/* ?? 	*/
	PRIO_JIKI,			/* ? 	*/
	PRIO_ENEMY,			/*  		*/
	PRIO_ENEMY_TAMA,	/*  	*/
	PRIO_JIKI_TAMA,		/* ? 	*/
	PRIO_ITEM,			/* ????	*/
	PRIO_BOSS,			/* ??		*/
	PRIO_GAME_OVER,		/* GAME OVER */
	PRIO_BTM = 255		/* ?? 	*/
};

//Action状态
enum{
	ACT_STATE_ACT = 0,	//通常状态
	ACT_STATE_DEAD		//死亡状态
};
//属性
#define ACT_ATR_HIDE 			( 1 <<  1 )	// 不显示/*  */
#define ACT_ATR_HIT 			( 1 <<  2 )	// 可以被点选/* ????? */
#define ACT_ATR_HIT_BOX 		( 1 <<  3 )	// 可以被点选的外框/* ??? + ???? */
#define ACT_ATR_INFO 			( 1 <<  4 )	// 显示出相关讯息/* ?????? */
#define ACT_ATR_HIDE2 			( 1 <<  5 )	// 不显示 + 可以被点选/* +????? */
#define ACT_ATR_BTL_CMD_END 	( 1 <<  6 )	// 战斗指令输入完成旗标,未完成输入的玩家会动,完成则不会动/* ????????? */
#define ACT_ATR_TYPE_PC 		( 1 <<  7 )	// 玩家/* ?? */
#define ACT_ATR_TYPE_OTHER_PC 	( 1 <<  8 )	// 别的玩家/* ??? */
#define ACT_ATR_TYPE_PET 		( 1 <<  9 )	/* ??? */
#define ACT_ATR_TYPE_ITEM 		( 1 << 10 )	/* ???? */
#define ACT_ATR_TYPE_GOLD 		( 1 << 11 )	/* ?? */
#define ACT_ATR_TYPE_OTHER 		( 1 << 12 )	/* ??????伡???? */
#define ACT_ATR_HIT_BOX_ALL1 	( 1 << 13 )	// 可以被点选 + 外框显示1
#define ACT_ATR_HIT_BOX_ALL2 	( 1 << 14 )	// 可以被点选 + 外框显示2
#define ACT_ATR_HIT_BOX_ALL3 	( 1 << 15 )	// 可以被点选 + 外框显示3
#define ACT_ATR_HIT_BOX_ALL4 	( 1 << 16 )	// 可以被点选 + 外框显示3
#define ACT_ATR_TRAVEL 			( 1 << 17 )	/* ??? */

#define ACT_ATR_HIT_BOX_COL1	( 1 << 18 )	// 左上第一列
#define ACT_ATR_HIT_BOX_COL2	( 1 << 19 ) // 左上第二列
#define ACT_ATR_HIT_BOX_COL3	( 1 << 20 ) // 右下第一列
#define ACT_ATR_HIT_BOX_COL4	( 1 << 21 ) // 右下第二列
#define ACT_ATR_HIT_BOX_COL5	( 1 << 22 )	// 左上第一列
#define ACT_ATR_HIT_BOX_COL6	( 1 << 23 ) // 左上第二列
#define ACT_ATR_HIT_BOX_COL7	( 1 << 24 ) // 右下第一列
#define ACT_ATR_HIT_BOX_COL8	( 1 << 25 ) // 右下第二列




/* ?????? **********************************************************/
struct action{
	struct 	action *pPrev, *pNext;			//上一个及下一个action指标
	void 	( *func )( struct action * );	//action所执行的function的指标
	void 	*pYobi;							//备用的struct指标
	void 	*pOther;						//其它用途struct指标
	UCHAR 	prio;							//action处理时的优先顺序
	UCHAR 	dispPrio;						//秀图时的优先顺序	
	int 	x, y;							//图的座标
	int		hitDispNo;						//是否命中目标编号
	BOOL	deathFlag;						//此action是否死亡旗标	
	int 	dx, dy;							//秀图座标位移量
	int 	dir;							//方向
	int 	delta;  						//合成向量
	
	char 	name[ 29 ];						//名字
	char 	freeName[ 33 ];					//free name
	int 	hp;		
#ifdef _PET_ITEM
	int		iOldHp;
#endif
	int 	maxHp;						
	int 	mp;							
	int 	maxMp;						
	int 	level;						
	int 	status;						
	int 	itemNameColor;					/* ?????? */
	int		charNameColor;					// ?????????????
	
	int		bmpNo;							//图号
	int		bmpNo_bak;							//备份图号
	int		atr;							//属性
	int		state;							//状态
	int		actNo;							//行动编号
	int		damage;						

	int		gx, gy;							//在目前的地图上的座标
	int		nextGx, nextGy;					//下一个座标
	int		bufGx[10], bufGy[10];			//从目前座标到下一个座标之间座标的buffer
	short	bufCount;						//设定目前要走到那一个座标
	short	walkFlag;						// ??????????????????????
	float	mx, my;							//地图座标
	float	vx, vy;							// ??

//属性
	short 	earth;							// 佋 
	short 	water;							// ? 
	short 	fire;							// ? 
	short 	wind;							// ? 
//rader使用
	int		dirCnt;							// ???????
//gemini使用
	int		spd;							//移动的速度(0~63)( ?????? )
	int		crs;							//方向(0~31)(正上方为0,顺时钟方向) ?( ???? )( ?????? )
	int		h_mini;							// ??
	int		v_mini;							// ?
//pattern使用
	int		anim_chr_no;					//人物的编号(anim_tbl.h的编号)
	int		anim_chr_no_bak;				//上一次的人物编号
	int		anim_no;						//人物的动作编号
	int		anim_no_bak;					//上一次的人物编号
	int		anim_ang;						//动作的方向(0~7)(下0)
	int		anim_ang_bak;					//上一次的方向
	int		anim_cnt;						//第几张frame
	int		anim_frame_cnt;					//这张frame停留时间
	int		anim_x;							//X座标(Sprbin+Adrnbin)
	int		anim_y;							//Y座标(Sprbin+Adrnbin)
	int		anim_hit;						// ???
	// shan add +1
	char    fmname[33];			            // 家族名称
	// Robin 0728 ride Pet
	int		onRide;
	char	petName[16+1];
	int		petLevel;
	int		petHp;
	int		petMaxHp;
	int		petDamage;
	int		petFall;
#ifdef _MIND_ICON
	unsigned int sMindIcon;
#endif
#ifdef _SHOWFAMILYBADGE_
	unsigned int sFamilyIcon;
#endif
#ifdef FAMILY_MANOR_
	unsigned int mFamilyIcon;
#endif
#ifdef _CHAR_MANOR_
	unsigned int mManorIcon;
#endif
#ifdef _CHARTITLE_STR_
	TITLE_STR TitleText;
#endif
#ifdef _CHARTITLE_
	unsigned int TitleIcon;
#endif
#ifdef _NPC_EVENT_NOTICE
	int noticeNo;
#endif

#ifdef _SKILL_ROAR  
	int		petRoar;		//大吼(克年兽)
#endif 
#ifdef _SKILL_SELFEXPLODE //自爆
	int		petSelfExplode;		
#endif 
#ifdef _MAGIC_DEEPPOISION   //剧毒
	int		petDeepPoision;		 
#endif 

#ifdef _CHAR_PROFESSION			// WON ADD 人物职业
	int		profession_class;
#endif
//#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
	int		warrioreffect; 
//#endif
#ifdef _GM_IDENTIFY		// Rog ADD GM识别
	char gm_name[33];
#endif
#ifdef _STREET_VENDOR
	char szStreetVendorTitle[64];
#endif
#ifdef _NPC_PICTURE
	int picture;
	int picturetemp;
#endif
#ifdef _PETSKILL_RIDE
	int saveride;
#endif
#ifdef _MOUSE_DBL_CLICK
	int index;	// 禁断!! Server中的charaindex
#endif

#ifdef _SFUMATO
	int sfumato;		// 二次渲染图层色彩
#endif
};

typedef struct action ACTION;

/* ?????????????????? */
extern ACTION *pActTop;
extern ACTION *pActBtm;

/* ?????? */
extern ACTION *pJiki;

#ifdef _STONDEBUG_		
/* ?????????? */
extern int ActCnt;
#endif

/* ?????????? *****************************************************/
ACTION *GetAction( UCHAR prio, UINT yobiSize );

/* ??????????? *****************************************************/
void InitAction( void );

/* ????????? *********************************************************/
void RunAction( void );

/* ????????????************************************************/
void DeathAction( ACTION *pAct );

/* ??????? *******************************************************/
void DeathAllAction( void );

/* ???????????? ***********************************************/
void ClearAction( ACTION *pAct );

/* ???????? *********************************************************/
void EndAction( void );

#endif
