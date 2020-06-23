/************************/
/*	oft.c				*/
/************************/

#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "vg410.h"
#include "work.h"
#include "../systeminc/loadrealbin.h"
#include "../systeminc/loadsprbin.h"
#include "../systeminc/battleMenu.h"
#include "../systeminc/anim_tbl.h"
#include "../systeminc/anim_tbl.h"
#include "../systeminc/chat.h"
#include "../systeminc/tool.h"
#include "../systeminc/pc.h"
#include "../systeminc/t_music.h"
#include "../systeminc/battlemap.h"
#include "../systeminc/menu.h"
#include "../systeminc/battleproc.h"
#include <math.h>
#include "../systeminc/version.h"
extern int MessageBoxNew(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
extern BOOL g_bUseAlpha;

// Robin 0804 ride Pet
extern	BOOL	bNewServer;

void petfallChangeGraph(ACTION *a0);
#ifdef _PETSKILL_RIDE
void petrideChangeGraph(ACTION *a0,int ridebmp);
#endif
void LogToBattleError(char *data, int line);
void katino(ACTION *a0);
static int command_point;
extern ACTION *MakeAnimDisp(int x, int y, int sprNo, int mode);
#ifdef _SYUTEST
static ACTION *Light1;
#endif
#ifdef _PETSKILL_LER
extern BOOL g_bUseAlpha;
#endif

#ifdef _SHOOTCHESTNUT	// Syu ADD 宠技：丢栗子
//做额外处理
int ShooterNum = -1;
/*
int nuty[20] = { 384 , 408 , 360 , 432 , 336 , 312 , 360 , 264 , 408 , 216 , 150 , 174 ,
126 , 198 , 102 , 202 , 250 , 154 , 298 , 106 } ;
*/
#endif

#ifdef _MAGIC_NOCAST//沉默
extern BOOL NoCastFlag;
#endif
//ACTION *set_bg( void );
ACTION *oft_test(void);
//ACTION *set_teki( void );
//void teki( ACTION *pAct );
void monster(ACTION *a0);
static int get_num(void);

/* ??????? ****************************************************/
#define HIT_STOP_TIM		8
#define VCT_NO_DIE			250
#define VCT_NO_APPEAR		VCT_NO_DIE - 1
#ifdef __NEW_BATTLE_EFFECT
//#define VCT_NO_PAUSE		254
#define VCT_NO_EFFECT		255
#endif
#define SCREEN_OUT			106
#define ATTACK_MAGIC_CASE	200
#define TOCALL_MAGIC_CASE	210
//#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
#define PROSKILL			230
int AttPreMagicNum = -1;
int AttNextMagicNum = -1;
short NoMiss = -1; //额外处理双重攻击
short StarLoop = 0;
//#endif
#ifdef _FIREHUNTER_SKILL				// (不可开) ROG ADD 朱雀技能_火线猎杀
#define FIRE_HUNTER_SKILL   240
#endif
#ifdef _WAVETRACK					// (不可开) Syu ADD 音波轨道
int BeAttNum = -1 ; //被攻击者的位置编号
#endif

#ifdef _FIREHUNTER_SKILL				// (不可开) ROG ADD 朱雀技能_火线猎杀
BOOL FireSkillEnd = FALSE;
BOOL bFireInit = FALSE;
int tempPosH = 0;
int tempPosV = 0;
int  iBeAttNum = -1;                     //被攻击者编号
int counter;
int tarpos = -1;
int tarMgiDem = 0;
#endif

/* ?????? ****************************************************/
//????????
char att_select_flg;

/* ??????? ****************************************************/
static ACTION *p_kanji;
static char kanji_buf[4][128];
static ACTION *p_master;
static ACTION *p_attrib;
static ACTION *p_missile[BATTLKPKPLYAERNUM + 1];
unsigned char crs_change_tbl[32] = {
	4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4 };
unsigned char crs_change_tbl2[8] = {
	16, 20, 24, 28, 0, 4, 8, 12 };
unsigned char crs_bound_tbl[4][32] = {
	//	  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
	{ 16, 15, 14, 13, 12, 11, 10, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 22, 21, 20, 19, 18, 17 },
	{ 0, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 6, 5, 4, 3, 2, 1, 0, 31, 30, 29, 28, 27, 26, 25, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 },
};
typedef struct{
	int		v_pos;
	ACTION	*work;
} SORT_CHR_EQU;
static SORT_CHR_EQU sort_chr_buf[40];
static char pet_nix_tbl[] =
{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
-1, 0, 0, 0, 0,
-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
1, 0, 0, 0, 0,
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
-1, 0, 0, 0, 0,
-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
1, 0, 0, 0, 0,
-2,
};
static int monster_start_pos[BATTLKPKPLYAERNUM * 2];
static int monster_place_pos[BATTLKPKPLYAERNUM * 2];
#ifdef _BATTLE_PK_PLAYER_FOR_40
static int monster_place_no[BATTLKPKPLYAERNUM] = { 2,1,3,0,4,7,6,8,5,9,12,11,13,10,14,17,16,18,15,19,22,21,23,20,24,27,26,28,25,29,32,31,33,30,34,37,36,38,35,39};
#else
#ifdef _BATTLE_PK_PLAYER_FOR_6VS6
static int monster_place_no[24] = { 2, 1, 3, 0, 4, 5, 8, 7, 9, 6, 10, 11, 14, 13, 15, 12, 16, 17, 20, 19, 21, 18, 22, 23};
#else
static int monster_place_no[20] = { 2, 1, 3, 0, 4, 7, 6, 8, 5, 9, 12, 11, 13, 10, 14, 17, 16, 18, 15, 19 };
#endif
#endif
#ifdef __ATTACK_MAGIC

// Global vars
int			g_iRunEarthQuake = 0;		// 地震的状况: 0 --> 没地震 , 1 --> 初始化地震 , 2 --> 地震中
int			g_iCurRunEarthQuake = 0;		// 目前地震的位置线
int			g_iNumRunEarthQuake;			// 地震的总次数



#define		ATTACK_MAGIC_ID		12345678	// 攻击魔法的识别代码
#define		MAX_NUM_ATTACKED	16			// 最大被攻击者的数目



// 位置定义
struct POS
{
	WORD	x;							// X轴的位置
	WORD	y;							// Y轴的位置
};



// BJ | 攻击者的编号 | 12345678 | 攻击者在施此咒术后的剩余MP | 前置动画的动画编号 | 攻击咒术的动画编号 | 
// 后置动画的动画编号 | 咒术的方式 | 咒术的时间差 | 显示咒术的位置的方式( 绝对或居中 ) | 
// 位置( 咒术敌方，绝对显示位置方式 ) | 位置( 前置动画，相对位置方式 ) | 位置( 后置动画，相对位置方式 ) |
// 前置动画显示在人物的前面或后方 | 咒术动画显示在人物的前面或后方 | 后置动画显示在人物的前面或后方 | 
// 震动画面 | 震动的起始时间 | 震动的结束时间 | 攻击的对象X1 | 攻击的对象X2 | ...攻击的对象Xn|FF
// 12345678:		表示为攻击性的魔法，否则便为睡眠咒术
struct ATTACK_MAGIC
{
	DWORD	dwCurFrame1;						// 目前已播放过的Frames相对于尚未播放过前置咒术前
	DWORD	dwCurFrame2;						// 目前已播放过的Frames相对于已经播放完前置咒术后
	DWORD	dwEQuake;							// 是否会震动画面
	DWORD	dwEQuakeSTime;						// 地震的开始时间
	DWORD	dwEQuakeETime;						// 地震的结束时间
	DWORD	dwEQuakeSFrame;						// 播放地震的开始Frame，0XFFFFFFFF表示没有地震的特效
	DWORD	dwEQuakeEFrame;						// 播放地震的结束Frame，0XFFFFFFFF表示没有地震的特效
	int		iPreMgcNum;							// 前置动画的动画编号
	int		iCurMgcNum;							// 咒术动画的动画编号
	int		iPostMgcNum;						// 后置动画的动画编号
	WORD	wRunPreMgc;							// 是否已经执行了前置咒术
	WORD	wAttackType;						// 咒术的方式: 个体( 单 )，整排( 轮流攻击 )，整排( 同时攻击 )，全体( 轮流攻击 )，全体( 同时攻击 )
	WORD	wAttackTimeSlice;					// 咒术的时间差，以毫秒为单位
	WORD	wShowType;							// 显示咒术的位置的方式，绝对及居中两种
	WORD	wScreenX;							// 咒术显示位置的X轴，在绝对显示方式时
	WORD	wScreenY;							// 咒术显示位置的Y轴，在绝对显示方式时
	WORD	wPreMgcX;							// 前置动画的相对位置
	WORD	wPreMgcY;							// 前置动画的相对位置
	WORD	wPostMgcX;							// 后置动画的相对位置
	WORD	wPostMgcY;							// 后置动画的相对位置
	WORD	wPreMgcOnChar;						// 前置动画显示在地板上，或人物上
	WORD	wCurMgcOnChar;						// 咒术动画显示在地板上，或人物上
	WORD	wPostMgcOnChar;						// 后置动画显示在地板上，或人物上
	WORD	wMgcFrameCount[MAX_NUM_ATTACKED];	// 播放攻击动画在敌物时的Frame Count
	WORD	wAttackedIndex[MAX_NUM_ATTACKED];	// 被攻击者的索引号阵列，0 - 19: 其中一位 , 20: 前排 , 21: 后排  , 22: 全体敌方
	POS		posAttacked[MAX_NUM_ATTACKED];		// 被攻击者的被攻击位置
	WORD	wNumAttacks;						// 攻击的总次数
	WORD	wNumAttackeds;						// 攻击播放完毕的次数
	WORD	wCurAttackNum;						// 目前攻击的索引号
};



static ATTACK_MAGIC	AttMgc;						// 攻击咒术的变数
static BOOL			bRunAttMgc = FALSE;			// 是否已初始化了一个攻击咒术
static int			AttackedInfo[4 * 10];		// 十个人的被攻击的资讯，每个人的栏位资讯( 导火线人物索引|被点着的人物索引|人扣的血|宠物扣的血 )
static int			iAttackedNum;				// 被攻击的总数目
static int			iCurAttackedFinishNum;		// 被完成的导火线



static BOOL	BuildAttackMagicData(ACTION *pMaster, ACTION *pAttacker);	// 攻击性咒术的资料建立
static BOOL RunTimeMagic();													// 攻击性咒术的监控函式

#endif


#ifdef __TOCALL_MAGIC
// kjl 02/06/24
#define TOCALL_MAGIC_ID		5711438			// 召唤术识别代码

struct TOCALL_MAGIC
{
	DWORD	dwCurFrame1;						// 目前已播放过的Frames相对于尚未播放过前置咒术前
	DWORD	dwCurFrame2;						// 目前已播放过的Frames相对于已经播放完前置咒术后
	DWORD	dwEQuake;							// 是否会震动画面
	DWORD	dwEQuakeSTime;						// 地震的开始时间
	DWORD	dwEQuakeETime;						// 地震的结束时间
	DWORD	dwEQuakeSFrame;						// 播放地震的开始Frame，0XFFFFFFFF表示没有地震的特效
	DWORD	dwEQuakeEFrame;						// 播放地震的结束Frame，0XFFFFFFFF表示没有地震的特效
	int		iPreMgcNum;							// 前置动画的动画编号
	int		iCurMgcNum;							// 咒术动画的动画编号
	int		iPostMgcNum;						// 后置动画的动画编号
	WORD	wRunPreMgc;							// 是否已经执行了前置咒术
	WORD	wAttackType;						// 咒术的方式: 个体( 单 )，整排( 轮流攻击 )，整排( 同时攻击 )，全体( 轮流攻击 )，全体( 同时攻击 )
	WORD	wAttackTimeSlice;					// 咒术的时间差，以毫秒为单位
	WORD	wShowType;							// 显示咒术的位置的方式，绝对及居中两种
	WORD	wScreenX;							// 咒术显示位置的X轴，在绝对显示方式时
	WORD	wScreenY;							// 咒术显示位置的Y轴，在绝对显示方式时
	WORD	wPreMgcX;							// 前置动画的相对位置
	WORD	wPreMgcY;							// 前置动画的相对位置
	WORD	wPostMgcX;							// 后置动画的相对位置
	WORD	wPostMgcY;							// 后置动画的相对位置
	WORD	wPreMgcOnChar;						// 前置动画显示在地板上，或人物上
	WORD	wCurMgcOnChar;						// 咒术动画显示在地板上，或人物上
	WORD	wPostMgcOnChar;						// 后置动画显示在地板上，或人物上
	WORD	wMgcFrameCount[MAX_NUM_ATTACKED];	// 播放攻击动画在敌物时的Frame Count
	WORD	wAttackedIndex[MAX_NUM_ATTACKED];	// 被攻击者的索引号阵列，0 - 19: 其中一位 , 20: 前排 , 21: 后排  , 22: 全体敌方
	POS		posAttacked[MAX_NUM_ATTACKED];		// 被攻击者的被攻击位置
	WORD	wNumAttacks;						// 攻击的总次数
	WORD	wNumAttackeds;						// 攻击播放完毕的次数
	WORD	wCurAttackNum;						// 目前攻击的索引号
	WORD	wIsPostDisappear;					// 攻击完主体是否马上消失
	WORD	wToCallMagicNo;						// 召唤术编号
};

static TOCALL_MAGIC	ToCallMgc;						// 攻击咒术的变数
static BOOL			bRunToCallMgc = FALSE;			// 是否已初始化了一个攻击咒术

static BOOL	BuildToCallMagicData(ACTION *pMaster, ACTION *pAttacker);	// 召唤咒术的资料建立
//static BOOL RunTimeMagic();													// 攻击性咒术的监控函式

#endif

#ifdef _PROFESSION_ADDSKILL
static int bRunBoundaryMgc_l = 0, bRunBoundaryMgc_r = 0;			// 特效状态
ACTION *boundary_2,*boundary_mark[2];
static BOOL	BuildBoundaryMagicData( int state );	// 特效的资料建立
static void RunTimeMagicBoundary(int state);
#endif

//处理优先顺序
enum {
	T_PRIO_TOP,						/* ?? 	*/
	T_PRIO_JIKI = 20,				/* ? 	*/
	T_PRIO_JIKI_MISSILE,			/* ? 	*/
	T_PRIO_MONSTER = 30,			/*  		*/
	T_PRIO_UFO = 60,				/* ??? 	*/
	T_PRIO_MONSTER_MISSILE = 70,	/*  	*/
	T_PRIO_MISSILE,					/* ????	*/
	T_PRIO_MAGIC_EFFECT,			/* ???	*/
	T_PRIO_HIT_MARK,				/* ??????	*/
	T_PRIO_DAMAGE_NUM,				/* ???? */
	T_PRIO_MASTER = 100,			/* ???? */
	T_PRIO_BOW,						/* ? 	*/
	T_PRIO_BTM 						/* ?? 	*/
};

enum {
	D_PRIO_MASTER = 80,					/* ???? */
	D_PRIO_MONSTER_MISSILE,			/*  	*/
	D_PRIO_UFO,						/* ??? 	*/
	D_PRIO_MONSTER,					/*  		*/
	D_PRIO_JIKI_MISSILE,			/* ? 	*/
	D_PRIO_JIKI,					/* ? 	*/
	D_PRIO_MISSILE,					/* ????	*/
	D_PRIO_MAGIC_EFFECT,			/* ???	*/
	D_PRIO_HIT_MARK,				/* ??????	*/
	D_PRIO_DAMAGE_NUM,				/* ???? */
};

#ifdef _ATTACK_EFFECT
void DisplayAttackEffect(ACTION *a0)
{
	if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		DeathAction(a0);
}

void SetDisplayAttackEffect(ACTION *a0, int iEffectNumber)
{
	ACTION *a1, *a2;

	a2 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));	// 建立新的 ACTION (攻击时特效)
	if (a2 == NULL)
		return;
	ATR_NAME(a2) = DisplayAttackEffect;
	ATR_CHR_NO(a2) = iEffectNumber;
	a1 = ATR_BODY_WORK(0, a0);							// 取出被攻击方
	ATR_DISP_PRIO(a2) = ATR_DISP_PRIO(a1) + 1;			// 显示在被攻击方图的上层
	ATR_H_POS(a2) = ATR_H_POS(a1);						// 显示在被攻击方的位置上
	ATR_V_POS(a2) = ATR_V_POS(a1);
}
#endif
/* ???????? *******************************************************************/
void kakushi_command(void)
{
	//??????
	if (LowResoCmdFlag)		// ??????????
		return;
	//????
	if ((joy_con[0] & JOY_LEFT) && (joy_con[0] & JOY_RIGHT))
	{
		LowResoCmdFlag = 1;		// ????????????
		//???????
		play_se(211, 320, 240);
	}
}

#define DAMAGE_SPD	24
//被攻击时的显示处理
void damage_num(ACTION *a0)
{
	int d0, d1, dx, d2 = 0;
	char szMoji[256];
	char szMojiP[256];
	//andy_mp
	char szMojMp[256];
	char szMojHp[256];

	d0 = 0; dx = 0;
	if (slow_flg)		//假如在slow状态
	{
		if (s_timer & 3)		//不移动
			d0 = 1;
	}
	if (d0 == 0)		//移动情况下
	{
		switch (ATR_INT_WORK1(a0))
		{
		case 0:
		case 36:
		case 37:
		case 38:
		case 39:
		case 40:
#ifdef _SKILL_ADDBARRIER
		case 43:
#endif 
#ifdef _PETSKILL_PEEL
		case 44:
#endif
#ifdef _PETSKILL_JUSTICE
		case 45:
#endif
#ifdef _PETSKILL_ADDATTCRAZED
		case 46:
		case 47:
#endif
#ifdef _PETSKILL_PROVOKEFIGHT
		case 48:
#endif
#ifdef _PRO3_ADDSKILL
		case 49:
		case 50:
		case 51:
#endif
		case 6:
		case 14:
		case 15:
		case 16:
			gemini(a0);
		}
		switch (ATR_VCT_NO(a0))
		{
		case 0:
			ATR_SPD(a0) -= 2;		//???????
			if (ATR_SPD(a0))		//???????
				break;
			ATR_CRS(a0) = 16;
			ATR_VCT_NO(a0) = 1;
			break;

		case 1:
			ATR_SPD(a0) += 2;
			if (ATR_SPD(a0) >= DAMAGE_SPD)		//???
			{
				ATR_STIMER(a0) = 60;
				ATR_VCT_NO(a0) = 2;
			}
			break;

		case 2:
			ATR_SPD(a0) = 0;		//?
			if (--ATR_STIMER(a0))		//???
				break;
			DeathAction(a0);		//?
			return;
		}
	}

	switch (ATR_INT_WORK1(a0))
	{
	case 0:		//Miss处理
		sprintf_s(szMoji, "Miss");
		d1 = 0;		//白色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_MISS;
		break;
	case 1:		//反击处理
		sprintf_s(szMoji, "Counter");
		d1 = 1;		//水色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_COUNTER;
		break;
	case 3:		//防御处理
		sprintf_s(szMoji, "Guard");
		d1 = 3;		//青色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_GUARD;
		break;
	case 5:		//捕捉处理
		sprintf_s(szMoji, "Capture");
		d1 = 5;		//绿色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_CAPTURE;
		break;

	case 6:		//受伤处理
		sprintf_s(szMoji, "%d", ATR_INT_WORK0(a0));
		sprintf_s(szMojiP, "%d", ATR_INT_WORKp(a0));
		//andy_mp
		if (ATR_MPDFLG(a0) == 1)
			sprintf_s(szMojMp, "%d", ATR_MPDAMAGE(a0));
		if (ATR_ADDHPFLG(a0) == 1)
			sprintf_s(szMojHp, "%d", ATR_ADDHP(a0));
		d1 = 6;		//红色Set
		break;
	case 7:		//捕捉成功处理
		sprintf_s(szMoji, "Success");
		d1 = 5;		//绿色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_GET;
		break;
	case 8:		//捕捉失败处理
		sprintf_s(szMoji, "Fail");
		d1 = 6;		//红色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_FAIL;
		break;
	case 9:		//逃跑处理
		sprintf_s(szMoji, "Escape");
		d1 = 5;		//绿色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_ESCAPE;
		break;
	case 10:		//收回宠物处理
		sprintf_s(szMoji, "Come!");
		d1 = 5;		//绿色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_COME_ON;
		break;
	case 11:		//放出宠物处理
		sprintf_s(szMoji, "Go!");
		d1 = 5;		//绿色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_GO;
		break;
	case 12:		//破除防御处理
		sprintf_s(szMoji, "Guard break");
		d1 = 5;		//绿色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_GUARD_BREAK;
		break;
	case 13:		//???
		sprintf_s(szMoji, "Danger");
		d1 = 6;		//???
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_DANGER;
		break;
	case 14:		//回复
		sprintf_s(szMoji, "%d", ATR_INT_WORK0(a0));
		sprintf_s(szMojiP, "%d", ATR_INT_WORKp(a0));
		d1 = 5;		//绿色Set
		break;
	case 15:		//MP回复
		sprintf_s(szMoji, "%d", ATR_INT_WORK0(a0));
		sprintf_s(szMojiP, "%d", ATR_INT_WORKp(a0));
		d1 = 4;		//黄色Set
		break;
	case 16:		//MP下降
		sprintf_s(szMoji, "%d", ATR_INT_WORK0(a0));
		sprintf_s(szMojiP, "%d", ATR_INT_WORKp(a0));
		d1 = 3;		//青色Set
		break;
	case 17:		//宠物逃跑
		sprintf_s(szMoji, "Leave");
		d1 = 2;		//紫色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_LEAVE;
		break;
	case 18:		//宠物NONO
		sprintf_s(szMoji, "No");
		d1 = 2;		//紫色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_NO;
		break;
	case 19:		//CRUSH
		sprintf_s(szMoji, "No");
		d1 = 2;		//紫色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_CRUSH;
		break;
	case 20:		//?????????
		sprintf_s(szMoji, "No");
		d1 = 2;		//紫色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_CAPTURE_UP;
		break;

#ifdef _SKILL_ROAR  //宠技:大吼(克年兽)
	case 22:
		sprintf_s(szMoji, "Roar");
		d1 = 5;		//绿色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_ROAR;
		break;
#endif

#ifdef _SKILL_SELFEXPLODE //自爆
	case 23:
		sprintf_s(szMoji, "SelfExplode");
		d1 = 5;		//绿色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_selfexplod; //动画
		break;
#endif

#ifdef _ATTDOUBLE_ATTACK	//	andy_add
	case 25:
		sprintf_s(szMoji, "No");
		d1 = 2;		//紫色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_land_att;
		break;
	case 26:
		sprintf_s(szMoji, "No");
		d1 = 2;		//紫色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_water_att;
		break;
	case 27:
		sprintf_s(szMoji, "No");
		d1 = 2;		//紫色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_fire_att;
		break;
	case 28:
		sprintf_s(szMoji, "No");
		d1 = 2;		//紫色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_wind_att;
		break;
#endif
	case 29:
		sprintf_s(szMoji, "No");
		d1 = 2;		//紫色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_tooth;
		break;
	case 30:
		sprintf_s(szMoji, "No");
		d1 = 2;		//紫色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_mic_def;
		break;
	case 31:
		sprintf_s(szMoji, "No");
		d1 = 2;		//紫色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_ironwall;
		break;

	case 32:	//属性转换
		sprintf_s(szMoji, "No");
		d1 = 2;		//紫色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_ch_earth;
		break;
	case 33:
		sprintf_s(szMoji, "No");
		d1 = 2;		//紫色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_ch_water;
		break;
	case 34:
		sprintf_s(szMoji, "No");
		d1 = 2;		//紫色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_ch_fire;
		break;
	case 35:
		sprintf_s(szMoji, "No");
		d1 = 2;		//紫色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_ch_wind;
		break;
	case 36:	//andy_add 回合补血
		memset(szMoji, 0, sizeof(szMoji));
		memset(szMojiP, 0, sizeof(szMojiP));
		if (ATR_INT_WORK0(a0) != 0)
		{
			sprintf_s(szMoji, "%4d", ATR_INT_WORK0(a0));
			sprintf_s(szMojiP, "%4d", ATR_INT_WORKp(a0));
		}
		if (ATR_MPDAMAGE(a0) != 0)
			sprintf_s(szMojMp, "%4d", ATR_MPDAMAGE(a0));
		d2 = FONT_PAL_YELLOW;
		d1 = FONT_PAL_GREEN;
		break;
	case 37:
		memset(szMoji, 0, sizeof(szMoji));
		sprintf_s(szMoji, "回避 %s", (ATR_INT_WORK0(a0) > 0) ? "上升" : "下降");
		d1 = FONT_PAL_GREEN;
		break;
	case 38:
		memset(szMoji, 0, sizeof(szMoji));
		sprintf_s(szMoji, "攻 %s%d％", (ATR_INT_WORK0(a0) > 0) ? "上升" : "下降", ATR_INT_WORK0(a0));
		d1 = FONT_PAL_GREEN;
		break;
	case 39:
		memset(szMoji, 0, sizeof(szMoji));
		sprintf_s(szMoji, "防 %s%d％", (ATR_INT_WORK0(a0) > 0) ? "上升" : "下降", ATR_INT_WORK0(a0));
		d1 = FONT_PAL_GREEN;
		break;
	case 40:
		memset(szMoji, 0, sizeof(szMoji));
		sprintf_s(szMoji, "敏 %s%d％", (ATR_INT_WORK0(a0) > 0) ? "上升" : "下降", ATR_INT_WORK0(a0));
		d1 = FONT_PAL_GREEN;
		break;
#ifdef _SYUTEST
	case 41:
		sprintf_s(szMoji, "LightTake");
		d1 = 5;		
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_lightget;//动画
		break;
#endif
		//#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
	case 41:
		sprintf_s(szMoji, "LightTake");
		d1 = 5;
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = AttNextMagicNum;//动画
#ifdef _WAVETRACK					// (不可开) Syu ADD 音波轨道
		if (AttNextMagicNum == 101703 && BeAttNum >= 15 && BeAttNum <= 19)
		{
			if (BeAttNum == 15)
				ATR_H_POS(a0) = ATR_H_POS(a0) - 1 ; 
			else if (BeAttNum == 17)
				ATR_H_POS(a0) = ATR_H_POS(a0) - 2  ; 
			else if (BeAttNum == 19)
				ATR_H_POS(a0) = ATR_H_POS(a0) - 3  ; 
			else if (BeAttNum == 18)
				ATR_V_POS(a0) = ATR_V_POS(a0) - 1  ; 
		}
		if (AttNextMagicNum == 101704 && BeAttNum >= 5 && BeAttNum <= 9)
		{
			if (BeAttNum == 5)
			{
				if (ATR_CHR_CNT(a0) == 0)
				{
					ATR_H_POS(a0) = 441;
					ATR_V_POS(a0) = 322;
				}
				else
					ATR_H_POS(a0) = ATR_H_POS(a0) + 1;
			}
			else if (BeAttNum == 6)
			{
				if (ATR_CHR_CNT(a0) == 0)
				{
					ATR_H_POS(a0) = 377;
					ATR_V_POS(a0) = 370;
				}
				else
					ATR_H_POS(a0) = ATR_H_POS(a0) + 2;
			}
			else if (BeAttNum == 7)
			{
				if (ATR_CHR_CNT(a0) == 0)
				{
					ATR_H_POS(a0) = 505;
					ATR_V_POS(a0) = 274;
				}
			}
			else if (BeAttNum == 8)
			{
				if (ATR_CHR_CNT(a0) == 0)
				{
					ATR_H_POS(a0) = 313;
					ATR_V_POS(a0) = 418;
				}
				else
					ATR_H_POS(a0) = ATR_H_POS(a0) + 3;
			}
			else if (BeAttNum == 9)
			{
				if (ATR_CHR_CNT(a0) == 0)
				{
					ATR_H_POS(a0) = 569;
					ATR_V_POS(a0) = 226;
				}
				else
					ATR_V_POS(a0) = ATR_V_POS(a0) + 1;
			}
		}
#endif
		break;
		//#endif

#ifdef _EQUIT_ARRANGE
	case 42:		//挡格处理
		sprintf_s(szMoji, "Guard");
		d1 = 3;		//青色Set
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_GUARD;
		break;
#endif

#ifdef _SKILL_ADDBARRIER		// Change 宠技:为魔障增加异常抗性功能
	case 43:
		memset(szMoji, 0, sizeof(szMoji));
		if (ATR_INT_WORK0(a0) == 0)
			sprintf_s(szMoji, "异常抗性回复");
		else
			sprintf_s(szMoji, "异常抗性 %s%d％", (ATR_INT_WORK0(a0) > 0) ? "上升":"下降", ATR_INT_WORK0(a0));
		d1 = FONT_PAL_GREEN;
		break;
#endif
#ifdef _PETSKILL_PEEL
	case 44:
	{
			   char *tempstr[] = { "头部", "铠部", "右手", "左饰品", "右饰品", "腰带", "左手", "鞋子", "手套"};
			   memset(szMoji, 0, sizeof(szMoji));
			   if (ATR_INT_WORK0(a0) > 8)
				   break;
			   sprintf_s(szMoji, "卸下 %s 装备", tempstr[ATR_INT_WORK0(a0)]);
			   d1 = FONT_PAL_GREEN;
	}
		break;
#endif
#ifdef _PETSKILL_JUSTICE
	case 45:
	{
			   sprintf_s(szMoji, "审判 %d 回合", ATR_INT_WORK0(a0));
			   d1 = FONT_PAL_GREEN;
	}
		break;
#endif
#ifdef _PETSKILL_ADDATTCRAZED
	case 46:
	{
			   sprintf_s(szMoji, "敏捷回避下降", ATR_INT_WORK0(a0));
			   d1 = FONT_PAL_GREEN;
	}
		break;
	case 47:
	{
			   sprintf_s(szMoji, "敏捷回避回复", ATR_INT_WORK0(a0));
			   d1 = FONT_PAL_GREEN;
	}
		break;
#endif
#ifdef _PETSKILL_PROVOKEFIGHT
	case 48:
	{
			   d1 = FONT_PAL_GREEN;
			   switch(ATR_INT_WORK0(a0)){
			   case 0:
				   sprintf_s(szMoji, "吼!", ATR_INT_WORK0(a0));
				   break;
			   case 1:
				   sprintf_s(szMoji, "受到挑衅!", ATR_INT_WORK0(a0));
				   break;
			   case 2:
				   sprintf_s(szMoji, "被黏液黏住!", ATR_INT_WORK0(a0));
				   break;
			   case 3:
				   sprintf_s(szMoji, "攻击次数减少!", ATR_INT_WORK0(a0));
				   break;
			   case 4:
				   sprintf_s(szMoji, "吸收光之精灵", ATR_INT_WORK0(a0));
				   break;
			   case 5:
				   sprintf_s(szMoji, "吸收镜之精灵", ATR_INT_WORK0(a0));
				   break;
			   case 6:
				   sprintf_s(szMoji, "吸收守之精灵", ATR_INT_WORK0(a0));
				   break;
			   case 7:
				   sprintf_s(szMoji, "光之精灵失效", ATR_INT_WORK0(a0));
				   break;
			   case 8:
				   sprintf_s(szMoji, "镜之精灵失效", ATR_INT_WORK0(a0));
				   break;
			   case 9:
				   sprintf_s(szMoji, "守之精灵失效", ATR_INT_WORK0(a0));
				   break;
			   case 10:
				   sprintf_s(szMoji, "解除黏液效果!", ATR_INT_WORK0(a0));
				   break;
#ifdef _PRO3_ADDSKILL
			   case 11:
				   sprintf_s(szMoji, "解除裂骨断筋!");
				   break;
			   case 12:
				   sprintf_s(szMoji, "中了毒素之网!");
				   break;
			   case 13:
				   sprintf_s(szMoji, "毒素之网解除!");
				   break;
			   case 14:
				   sprintf_s(szMoji, "抚慰心灵解除!");
				   break;
			   case 15:
				   sprintf_s(szMoji, "封印闇灵法术!");
				   break;
			   case 16:
				   sprintf_s(szMoji, "回复闇灵法术!");
				   break;
			   case 17:
				   sprintf_s(szMoji, "中了恶魔诅咒!");
				   break;
			   case 18:
				   sprintf_s(szMoji, "解除恶魔诅咒!");
				   break;
			   case 19:
				   sprintf_s(szMoji, "受到了惊吓!");
				   break;
#endif
			   }
			   break;
	}
#endif
#ifdef _PRO3_ADDSKILL
	case 49: //抚慰心灵
	{
				 sprintf_s(szMoji, "忠诚度 %d ", ATR_INT_WORK0(a0));
				 d1 = FONT_PAL_GREEN;
	}
		break;
	case 50: //闇灵封印
	{
				 char *tempstr[] = { "", "火山泉", "针针相对", "世界末日", "冰爆术", "附身术", "召雷术", "暴风雨", "电流术", "火星球",
					 "嗜血蛊", "嗜血成性", "冰箭术", "火龙枪", "冰镜术", "火附体", "雷附体", "冰附体", "火熟练度", "雷熟练度", "冰熟练度", 
					 "移形换位", "暴击", "连环攻击", "双重攻击", "回避", "枪熟练度", "斧熟练度", "棍熟练度", "弓熟练度",
					 "精通回力镖", "精通投掷石", "精通投掷斧", "状态回复", "舍已为友", "激化攻击", "能量聚集", "专注战斗", 
					 "盾击", "贯穿攻击", "濒死攻击", "回旋攻击", "混乱攻击", "二刀流", "追寻敌踪", "回避战斗", "树根缠绕", 
					 "陷阱", "天罗地网", "尸体掠夺", "毒素武器", "弱点攻击", "挑拨", "格档", "座骑攻击", "加工", "驯服宠物", "激怒宠物", 
					 "自给自足", "雷抗性", "火抗性", "冰抗性", "遗忘", "自我强化", "气力充沛", "负重增加", "自然威能", "号召自然", 
					 "地结界", "水结界", "火结界", "风结界", "破除结界", "诱敌", "野性征服", 
					 "四方防御", "裂骨断筋", "战狼怒吼", "斗气导引", 
					 "魔力咒印", "恶魔诅咒", "神灵之赐", "多重冰箭", 
					 "多重冰箭", "毒素之网", "抚慰心灵", 
					 "战场急救", "制药学" };
				 memset(szMoji, 0, sizeof(szMoji));
				 sprintf_s(szMoji, "%s 被封印", tempstr[ATR_INT_WORK0(a0)]);
				 d1 = FONT_PAL_GREEN;
	}
		break;
	case 51:
	{
			   sprintf_s(szMoji, "攻下降%d", ATR_INT_WORK0(a0) );
			   d1 = FONT_PAL_GREEN;
	}
		break;
#endif
	}

	switch (ATR_INT_WORK1(a0))
	{
	case 36:	//andy_add 回合补血
		if (ATR_RIDE_FLAG(a0) == 1)
			StockFontBuffer(ATR_H_POS(a0) - 20, ATR_V_POS(a0) + 32, FONT_PRIO_BACK, d1, szMojiP, 0);
	case 37:
	case 38:
	case 39:
	case 40:
#ifdef _SKILL_ADDBARRIER
	case 43:
#endif
#ifdef _PETSKILL_PEEL
	case 44:
#endif
#ifdef _PETSKILL_JUSTICE
	case 45:
#endif
#ifdef _PETSKILL_ADDATTCRAZED
	case 46:
	case 47:
#endif
#ifdef _PETSKILL_PROVOKEFIGHT
	case 48:
#endif
#ifdef _PRO3_ADDSKILL
	case 49:
	case 50:
	case 51:
#endif
		StockFontBuffer(ATR_H_POS(a0) - 20, ATR_V_POS(a0), FONT_PRIO_BACK, d1, szMoji, 0);
		if (ATR_MPDAMAGE(a0) != 0)
			StockFontBuffer(ATR_H_POS(a0) - 20, ATR_V_POS(a0) + 12, FONT_PRIO_BACK, d2, szMojMp, 0);
		break;
	case 6:
	case 14:
	case 15:
	case 16:
		//andy_mp
		if (ATR_ADDHPFLG(a0) == 1)//嗜血技
		{
			StockFontBuffer(ATR_H_POS(a0) - d0, ATR_V_POS(a0), FONT_PRIO_BACK, FONT_PAL_GREEN, szMojHp, 0);
			break;
		}
		d0 = GetStrWidth(szMoji) >> 1;
		StockFontBuffer(ATR_H_POS(a0) - d0, ATR_V_POS(a0), FONT_PRIO_BACK, d1, szMoji, 0);

		if (ATR_MPDAMAGE(a0) != 0 || ATR_MPDFLG(a0) == 1)//伤害
			StockFontBuffer(ATR_H_POS(a0) - d0, ATR_V_POS(a0) + 12, FONT_PRIO_BACK, FONT_PAL_YELLOW, szMojMp, 0);
		if (ATR_RIDE_FLAG(a0) == 1)
		{
			dx = GetStrWidth(szMojiP) >> 1;
			StockFontBuffer(ATR_H_POS(a0) - dx, ATR_V_POS(a0) + 60, FONT_PRIO_BACK, d1, szMojiP, 0);
		}
		break;
	default:
		pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP);
	}
}

//伤害值设定处理
void set_damage_num(ACTION *a0, int color, int v_pos)
{
	ACTION *a1;
	a1 = GetAction(T_PRIO_DAMAGE_NUM, sizeof(ATR_EQU));
	if (a1 == NULL)
		return;
	/* ??? */
#ifdef _WAVETRACK					// (不可开) Syu ADD 音波轨道
	BeAttNum = a0->hitDispNo ; 
#endif
	ATR_NAME(a1) = damage_num;
	//表示优先度
	ATR_DISP_PRIO(a1) = D_PRIO_DAMAGE_NUM;
	/* ??? */
	ATR_H_POS(a1) = ATR_H_POS(a0);
	ATR_V_POS(a1) = ATR_V_POS(a0) + v_pos;
	ATR_SPD(a1) = DAMAGE_SPD;
	ATR_ATTRIB(a1) = ACT_ATR_HIDE;
	//Syu mark 下面这行跟颜色根本没关系，是damage_num的代码，damage_num的switch才会决定色码
	ATR_INT_WORK1(a1) = color;		//文字颜色设定
	if (ATR_ADDHPFLG(a0) == 1)
	{
		ATR_ADDHP(a1) = ATR_ADDHP(a0);
		ATR_ADDHPFLG(a1) = ATR_ADDHPFLG(a0);
	}
	int dddd = ATR_DAMAGE(a0);
	ATR_INT_WORK0(a1) = ATR_DAMAGE(a0);		//伤害设定
	ATR_INT_WORKp(a1) = ATR_PET_DAMAGE(a0);		//伤害设定
	ATR_RIDE_FLAG(a1) = ATR_RIDE(a0);
	//andy_mp
	ATR_MPDAMAGE(a1) = ATR_MPDAMAGE(a0);
	ATR_MPDFLG(a1) = ATR_MPDFLG(a0);

	switch (ATR_INT_WORK1(a1))
	{
	case 0:
	case 36:
	case 37:
	case 38:
	case 40:
#ifdef _SKILL_ADDBARRIER
	case 43:
#endif
#ifdef _PETSKILL_PEEL
	case 44:
#endif
#ifdef _PETSKILL_JUSTICE
	case 45:
#endif
#ifdef _PETSKILL_ADDATTCRAZED
	case 46:
	case 47:
#endif
#ifdef _PETSKILL_PROVOKEFIGHT
	case 48:
#endif
#ifdef _PRO3_ADDSKILL
	case 49:
	case 50:
	case 51:
#endif
	case 6:
	case 14:
	case 15:
	case 16:
	case 19:
		break;
	default:
#ifdef _SKILL_SELFEXPLODE //自爆
		if (ATR_SELFEXPLODE(a0) == 1)
			ATR_V_POS(a1) = ATR_V_POS(a0) - SpriteInfo[a0->bmpNo].height / 2 + 20;
		else
#endif
#ifdef _SYUTEST
		if (color == 41)
			ATR_V_POS(a1) = ATR_V_POS(a0) + v_pos;
		else
			ATR_V_POS(a1) = ATR_V_POS(a0) - 80;
#else
			ATR_V_POS(a1) = ATR_V_POS(a0) - 80;
#endif
	}
#ifdef _PETSKILL_LER
	// 雷尔防御时的防护盾
	if (color == 3 && ATR_CHR_NO(a0) == 101815)
	{
		ACTION *a2;
		a2 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
		if (a2 == NULL)
			return;
		ATR_NAME(a2) = katino;
		ATR_BODY_WORK(0, a2) = a0;
		ATR_JUJUTSU_WORK(a0) = a2;
		ATR_DISP_PRIO(a2) = ATR_DISP_PRIO(a0) + 1;
		ATR_H_POS(a2) = ATR_H_POS(a0) + 10;
		ATR_V_POS(a2) = ATR_V_POS(a0) + 10;
		if (g_bUseAlpha)
			ATR_CHR_NO(a2) = 101805;
		else
			ATR_CHR_NO(a2) = 101858;
	}
#endif
#ifdef _PETSKILL_RIDE
	//petrideChangeGraph( a0 );
#endif
}

/* ???????? *******************************************************************/
#if 0
???
?????	
?????	
?????  ?
?????	?
?????	?
?????	?
?????	??????
?????	

?????
?????	
?????	
?????  ?
?????	?
?????	?
?????	?
?????	??????
?????	
#endif
void play_damage(int no, int x)
{
	//?????????
	if (no >= 10100)
		no -= 100;
	//效果音分类
	switch (no)
	{
	case 10000:		//空手
		no = 250;
		break;
	case 10001:		//爪
		no = 254;
		break;
	case 10002:		//斧
		no = 251;
		break;
	case 10003:		//棍棒
		no = 251;
		break;
	case 10004:		//枪
		no = 252;
		break;
	case 10005:		//弓
		no = -1;
		break;
	case 10006:		//投掷斧
		no = -1;
		break;
	case 10007:		//咒术
		no = 254;
		break;
	default:
		no = 250;
	}
	//???䲡????
	if (no != -1)
		play_se(no, x, 240);
}

/* ???? *******************************************************************/
void disp_kanji(ACTION *a0)
{
	int d1, d7;

	ACTION *a1;
	a1 = ATR_BODY_WORK(0, a0);		//?????
	if (!--ATR_INT_WORK0(a0))		//?????
	{
		DeathAction(a1);		//????
		DeathAction(a0);		//?
		p_kanji = NULL;
		ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
	}
	else
	{
		if (ATR_LIFE(a1))		//????
		{
			for (d1 = 4 + 13, d7 = 0; d7 < 4; d1 += 18, d7++)
			{
				if (kanji_buf[d7][0])		//??????
					StockFontBuffer(640 - 4 + -64 * 4 + 24, d1, FONT_PRIO_FRONT, FONT_PAL_WHITE, kanji_buf[d7], 0);
			}
		}
	}
}

/* ????? *******************************************************************/
void magic_effect(ACTION *a0)
{
	ACTION *a1;

	switch (ATR_VCT_NO(a0))
	{
	case 0:
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))	//??????
		{
			ATR_VCT_NO(a0)++;
			ATR_CHR_ACT(a0)++;
		}
		break;
	case 1:
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))	//??????
		{
			ATR_VCT_NO(a0)++;
			ATR_CHR_ACT(a0)++;
		}
		break;
	case 2:
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))	//??????
		{
			DeathAction(a0);		//?
			return;
		}
		break;
	}
	a1 = ATR_BODY_WORK(0, a0);		//?
	ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1) + 1;
	ATR_H_POS(a0) = ATR_H_POS(a1);
	ATR_V_POS(a0) = ATR_V_POS(a1);
}

//HIT_MARK处理 ( 必杀 )
void hit_mark_critical(ACTION *a0)
{
	if (ATR_STIMER(a0) == ATR_FIRST_FLG(a0) >> 1)		//?????
		ATR_CHR_NO(a0)--;
	if (--ATR_STIMER(a0))
	{
		gemini(a0);
		pattern(a0, 0, 0);
	}
	else
	{
		DeathAction(a0);		//终了
		return;
	}
}
//HIT_MARK处理
void hit_mark(ACTION *a0)
{
	//对象死亡ATR_KAISHIN为TRUE
	if (ATR_KAISHIN(a0))
	{
		if (ATR_STIMER(a0) == ATR_FIRST_FLG(a0) * 1 / 3)		//?????
			ATR_CHR_NO(a0)--;
		if (ATR_STIMER(a0) == ATR_FIRST_FLG(a0) * 2 / 3)		//?????
			ATR_CHR_NO(a0)--;
	}
	else
	{
		if (ATR_STIMER(a0) == ATR_FIRST_FLG(a0) * 1 / 3)		//?????
			ATR_CHR_NO(a0)--;
		if (ATR_STIMER(a0) == ATR_FIRST_FLG(a0) * 2 / 3)		//?????
			ATR_CHR_NO(a0)--;
	}
	if (--ATR_STIMER(a0))
		pattern(a0, 0, 0);
	else
	{
		DeathAction(a0);		//终了
		return;
	}
}

void set_hit_mark(ACTION *a0)
{
	ACTION *a1, *a2;
	int d0;

	//必杀时产生五颗星星，不管是不是必杀都先做，不是才break
	for (d0 = 5; d0 > 0; d0--)
	{
		//设定HIT_MARK
		a1 = GetAction(T_PRIO_HIT_MARK, sizeof(ATR_EQU));
		if (a1 == NULL)
			return;
		/* ??? */
		ATR_NAME(a1) = hit_mark;
		//表示优先度
		ATR_DISP_PRIO(a1) = D_PRIO_HIT_MARK;
		//假设被攻击对象防御用青色图
		if (ATR_ATTACK_KIND(0, a0) & ATT_GUARD)
			//设定图号
			ATR_CHR_NO(a1) = CG_HIT_MARK_12;		//青色
		else
			//设定图号
			ATR_CHR_NO(a1) = CG_HIT_MARK_22;		//红色
		//初期位置配置
		a2 = ATR_BODY_WORK(0, a0);
		ATR_H_POS(a1) = ATR_H_POS(a2);
		ATR_V_POS(a1) = ATR_V_POS(a2) - 32;
		ATR_SPD(a1) = Rnd(4, 7);
		ATR_CRS(a1) = d0 * 6;
		if (ATR_HIT_STOP(a0) == HIT_STOP_TIM)		//??????????
			ATR_FIRST_FLG(a1) = ATR_STIMER(a1) = ATR_HIT_STOP(a0);		//表示时间设定
		else
		{
			ATR_FIRST_FLG(a1) = ATR_STIMER(a1) = ATR_HIT_STOP(a0);		//表示时间设定
			ATR_KAISHIN(a1) = 1;
		}
		//必杀时处理下一颗星星，非必杀跳出
		if (ATR_ATTACK_KIND(0, a0) & ATT_SATISFACTORY)
		{
			ATR_NAME(a1) = hit_mark_critical;
			ATR_CHR_NO(a1) = CG_HIT_MARK_01;

		}
		else
			break;
	}
}

/* 㘎?????? *******************************************************************/
void disp_guard_mark(ACTION *a0)
{
	ACTION *a1;
	a1 = ATR_BODY_WORK(0, a0);
	ATR_H_POS(a0) = ATR_H_POS(a1);
	ATR_V_POS(a0) = ATR_V_POS(a1) - 32;
	if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))	//??????
		DeathAction(a0);		//?
}

void set_guard_mark(ACTION *a0)
{
	ACTION *a1;

	a1 = GetAction(T_PRIO_HIT_MARK, sizeof(ATR_EQU));
	if (a1 == NULL)
		return;
	ATR_NAME(a1) = disp_guard_mark;
	ATR_DISP_PRIO(a1) = D_PRIO_HIT_MARK;
	if (ATR_ATTACK_KIND(0, a0) & ATT_REFLEX)
		ATR_CHR_NO(a1) = SPR_mirror;
	else if (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA)
		ATR_CHR_NO(a1) = SPR_barrior;
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
	else if (ATR_ATTACK_KIND(0, a0) & ATT_TRAP)
	{		//陷阱
		if ( a0->hitDispNo >= 10 && a0->hitDispNo <= 19 )
			ATR_CHR_NO(a1) = 101630;
		else if ( a0->hitDispNo >= 0 && a0->hitDispNo <= 9 )
			ATR_CHR_NO(a1) = 101696;
#ifdef _PETSKILL_BATTLE_MODEL
		if (ATR_INT_WORK0(a0) == ATT_BATTLE_MODEL)
		{
			if (ATR_GROUP_FLG(a0) == 0) ATR_CHR_NO(a1) = 101696;
			else ATR_CHR_NO(a1) = 101630;
		}
#endif
	}
#endif
#ifdef _PETSKILL_ACUPUNCTURE
	else if (ATR_ATTACK_KIND(0, a0) & ATT_ACUPUNCTURE)
		//目前不显示效果图
#ifdef _FIX_ACUPUNCTURE
		ATR_DAMAGE(a0) = ATR_ATTACK_POW(1, a0);
#else
		ATR_DAMAGE(a0) = ATR_ATTACK_POW(0, a0)*2;//设定伤害值
#endif
#endif
#ifdef _PET_ITEM
	else if (ATR_ATTACK_KIND(0, a0) & ATT_ATTACKBACK)
		ATR_DAMAGE(a0) = ATR_ATTACK_POW(1, a0);
#endif
	else
		ATR_CHR_NO(a1) = SPR_kyusyu;

	ATR_BODY_WORK(0, a1) = ATR_BODY_WORK(0, a0);
}

void set_jujutsu_hit_mark(ACTION *a0)
{
	ACTION *a1;

	//?????????
	a1 = GetAction(T_PRIO_HIT_MARK, sizeof(ATR_EQU));
	if (a1 == NULL)
		return;
	/* ??? */
	ATR_NAME(a1) = hit_mark;
	/* ? */
	ATR_DISP_PRIO(a1) = D_PRIO_HIT_MARK;
	/* ?????? */
	ATR_CHR_NO(a1) = CG_HIT_MARK_32;		//?
	/* ??? */
	ATR_H_POS(a1) = ATR_H_POS(a0);
	ATR_V_POS(a1) = ATR_V_POS(a0) - 32;
	ATR_FIRST_FLG(a1) = ATR_STIMER(a1) = HIT_STOP_TIM;		//?????
}

static int piyo_loop_v_tbl[] = {
	0, 1, 2, 3, 4, 4, 5, 6, 7, 8, 8, 9, 9, 9, 9, 10, 9,
	9, 9, 9, 8, 8, 7, 6, 5, 5, 4, 3, 2, 1, 0, -1, -2,
	-3, -4, -4, -5, -6, -7, -8, -8, -9, -9, -9, -9, -10, -9, -9, -9,
	-9, -8, -8, -7, -6, -5, -5, -4, -3, -2, -1,
};
static int piyo_loop_h_tbl[] = {
	-24, -23, -23, -22, -21, -20, -19, -17, -16, -14, -11, -9, -7, -4, -2, 0, 2,
	4, 7, 9, 11, 14, 16, 17, 19, 20, 21, 22, 23, 23, 24, 23, 23,
	22, 21, 20, 19, 17, 16, 14, 12, 9, 7, 4, 2, 0, -2, -4, -7,
	-9, -11, -14, -16, -17, -19, -20, -21, -22, -23, -23,
};

//??????????  ???????  ??????????
void piyo_loop(ACTION *a0)
{
	ACTION *a1;
	int d0;

	a1 = ATR_BODY_WORK(0, a0);		//䁖??????
	if (ATR_NAME(a1) == NULL || ATR_LIFE(a1) != 0)		//???????
	{
		DeathAction(a0);		//?
		return;
	}
	/* ? */
	ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1) + 1;
	d0 = piyo_loop_h_tbl[ATR_STIMER(a0)];
	ATR_H_POS(a0) = ATR_H_POS(a1) + d0 + ATR_INT_WORK0(a0);
	d0 = piyo_loop_v_tbl[ATR_STIMER(a0)];
	ATR_V_POS(a0) = ATR_V_POS(a1) + d0 + ATR_INT_WORK1(a0);
	ATR_STIMER(a0)++;
	if (ATR_STIMER(a0) == 60)
		ATR_STIMER(a0) = 0;
	pattern(a0, 0, 0);
}

//??????????  ?????????  ??????????
#define PIYOPIYO_CNT	3
void set_piyo_loop(ACTION *a0)
{
	ACTION *a1;
	int d7;

	for (d7 = 0; d7 < PIYOPIYO_CNT; d7++)
	{
		//?????????
		a1 = GetAction(T_PRIO_HIT_MARK, sizeof(ATR_EQU));
		if (a1 == NULL)
			return;
		/* ??? */
		ATR_NAME(a1) = piyo_loop;
		ATR_CHR_NO(a1) = SPR_star;
		/* ??? */
		ATR_BODY_WORK(0, a1) = a0;
		ATR_STIMER(a1) = d7 * (60 / PIYOPIYO_CNT);
		//???
#ifndef __CARYTEST
		ATR_INT_WORK0(a1) = a0->anim_x + SpriteInfo[ATR_PAT_NO(a0)].width / 2;
#else
		ATR_INT_WORK0(a1) = a0->anim_x + g_lpRealAdrn[ATR_PAT_NO(a0)].width / 2;
#endif
		ATR_INT_WORK1(a1) = a0->anim_y;
	}
}

/* ?䞍?? *******************************************************************/
void katino(ACTION *a0)
{
	ACTION *a1;

	a1 = ATR_BODY_WORK(0, a0);		//??????????
	if (ATR_NAME(a1) == NULL || ATR_VCT_NO(a1) == VCT_NO_DIE + 2 || ATR_LIFE(a1) == 0)		//???
	{
		ATR_JUJUTSU_WORK(a1) = NULL;		//?
		DeathAction(a0);		//?
		return;
	}
	ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1) + 1;
	ATR_H_POS(a0) = ATR_H_POS(a1);
#ifdef _FIXSTATUS					// (不可开) Syu ADD 修正战斗状态显示方式
	if (ATR_CHR_NO(a0) == SPR_shock)			//麻痹
		ATR_V_POS(a0) = ATR_V_POS(a1);
	else if (ATR_CHR_NO(a0) == 35120)		//树根缠绕
		ATR_V_POS(a0) = ATR_V_POS(a1) - 34 ;
	else if (ATR_CHR_NO(a0) == 101702)	//毒Sars蔓延
		ATR_V_POS(a0) = ATR_V_POS(a1) - 34 ;
	else if (ATR_CHR_NO(a0) == 27692)		//冰箭术、冰爆术
		ATR_V_POS(a0) = ATR_V_POS(a1) - 34 ;
	else if (ATR_CHR_NO(a0) == 35110)		//天罗地网
		ATR_V_POS(a0) = ATR_V_POS(a1) - 34 ;
	else if (ATR_CHR_NO(a0) == 26517)		//遗忘
		ATR_V_POS(a0) = ATR_V_POS(a1) - 34 ;
	else if (ATR_CHR_NO(a0) == CG_HIT_MARK_00)
	{	//晕眩
		StarLoop++; 
		if (StarLoop <= 20 && StarLoop > 0) 
			ATR_V_POS(a0) = ATR_V_POS(a1) - 46;
		else if (StarLoop <= 40 && StarLoop > 20)
			ATR_V_POS(a0) = ATR_V_POS(a1) - 56;
		else if (StarLoop <= 60 && StarLoop > 40)
		{
			ATR_V_POS(a0) = ATR_V_POS(a1) - 56;
			ATR_H_POS(a0) = ATR_H_POS(a0) + 16;
		}
		else if (StarLoop <= 80 && StarLoop > 60)
		{
			ATR_V_POS(a0) = ATR_V_POS(a1) - 46;
			ATR_H_POS(a0) = ATR_H_POS(a0) + 16;
		}
		if (StarLoop >= 80)
			StarLoop = 0 ; 
	}
#ifdef _PETSKILL_LER
	// 雷尔变身时的雾动画的位置不动
	else if (ATR_CHR_NO(a0) == 101810 || ATR_CHR_NO(a0) == 101811 || ATR_CHR_NO(a0) == 101863 || ATR_CHR_NO(a0) == 101864);
	// 雷尔防御时的防护盾动画位置要变
	else if (ATR_CHR_NO(a0) == 101805 || ATR_CHR_NO(a0) == 101858)
	{
		ATR_H_POS(a0) = ATR_H_POS(a1) + 10;
		ATR_V_POS(a0) = ATR_V_POS(a1) + 10;
	}
#endif
	else 
		ATR_V_POS(a0) = ATR_V_POS(a1) - 64;
#else
	if (ATR_CHR_NO(a0) == SPR_shock)		//????
		ATR_V_POS(a0) = ATR_V_POS(a1);
	else
		ATR_V_POS(a0) = ATR_V_POS(a1) - 64;
#endif
#ifdef _PETSKILL_LER
	// 当动画为雾或防护盾时
	if (ATR_CHR_NO(a0) == 101810 || ATR_CHR_NO(a0) == 101811 || ATR_CHR_NO(a0) == 101805 ||
		ATR_CHR_NO(a0) == 101863 || ATR_CHR_NO(a0) == 101864 || ATR_CHR_NO(a0) == 101858)
	{
		// 动画播放完了,清除动画
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{
			ATR_JUJUTSU_WORK(a1) = NULL;
			DeathAction(a0);
			if (ATR_CHR_ACT(a1) == ANIM_DEAD && ATR_CHR_NO(a1) == 101813)
				ATR_CHR_NO(a1) = 101814;
			else if (ATR_CHR_ACT(a1) == ANIM_DEAD && ATR_CHR_NO(a1) == 101814)
				ATR_CHR_NO(a1) = 101815;
			ATR_VCT_NO(a1) = 0;
			int i;
			for (i = 0; i < BATTLKPKPLYAERNUM; i++)
			{
				// 找出雷尔在那一边
				if (ATR_CHR_NO(p_party[i]) == ATR_CHR_NO(a1))
					break;
			}
			a1->dir = ATR_CHR_ANG(a1) = i < 10 ? 3:7;
			ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];
		}
	}
	else 
#endif
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);		//???
}

//产生持续性人物状态动画
void set_single_jujutsu(int d0, ACTION *a1)
{
	if (ATR_LIFE(a1) == 0)		//??????
		return;

	ACTION *a2;

	a2 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
	if (a2 == NULL)
		return;
	/* ??? */
	ATR_NAME(a2) = katino;
	ATR_BODY_WORK(0, a2) = a1;
	ATR_JUJUTSU_WORK(a1) = a2;
	/* ? */
	ATR_DISP_PRIO(a2) = ATR_DISP_PRIO(a1) + 1;
	/* ??? */
	ATR_H_POS(a2) = ATR_H_POS(a1);
	ATR_V_POS(a2) = ATR_V_POS(a1) - 64;
	ATR_STIMER(a2) = d0;
	/* ?????? */
	switch (d0)
	{
		//  "", 
	case 1:
		ATR_CHR_NO(a2) = SPR_poison;
		break;
		//  "??", 
	case 2:
		ATR_CHR_NO(a2) = SPR_shock;
		break;

		//  "䞍?", 
	case 3:
		ATR_CHR_NO(a2) = SPR_sleep;
		break;
		//  "?", 
	case 4:
		ATR_CHR_NO(a2) = SPR_stone;
		break;
		//  "?????", 
	case 5:
		ATR_CHR_NO(a2) = SPR_drunk;
		break;
		//  "??" 
	case 6:
		ATR_CHR_NO(a2) = SPR_conf;
		break;
#ifdef _MAGIC_WEAKEN  //虚弱
	case 7:
		ATR_CHR_NO(a2) = SPR_weaken;
		break;
#endif
#ifdef _MAGIC_DEEPPOISION   //剧毒
	case 8:
		ATR_CHR_NO(a2) = SPR_deeppoison;
		break;
#endif
#ifdef _MAGIC_BARRIER
	case 9:
		ATR_CHR_NO(a2) = SPR_barrier; //魔障 
		break;
#endif
#ifdef _MAGIC_NOCAST
	case 10:
		ATR_CHR_NO(a2) = SPR_nocast; //沉默 
		break;
#endif

#ifdef _SARS						// WON ADD 毒煞蔓延
	case 11:
		ATR_CHR_NO(a2) = 101702; //毒煞蔓延
		break;
#endif

#ifdef _CHAR_PROFESSION						// WON ADD
	case 12:
		ATR_CHR_NO(a2) = CG_HIT_MARK_00; //晕眩
		break;
	case 13:
		ATR_CHR_NO(a2) = 35120; //树根缠绕
		break;
	case 14:
		ATR_CHR_NO(a2) = 35110; //天罗地网
		break;
	case 15:
		ATR_CHR_NO(a2) = 27692;	//冰爆术
		break;
	case 16: 
		ATR_CHR_NO(a2) = 26517; //遗忘
		break;
	case 17:
		ATR_CHR_NO(a2) = 27692;	//冰箭
		break;	
	case 18:
		ATR_CHR_NO(a2) = 27012; //嗜血蛊
		break;	
	case 19:
		ATR_CHR_NO(a2) = 27012; //一针见血
		break;
	case 20:
		ATR_CHR_NO(a2) = SPR_conf; //挑拨
		break;
	case 21:
		ATR_CHR_NO(a2) = 0; //火附体
		break;
	case 22:
		ATR_CHR_NO(a2) = 0; //冰附体
		break;
	case 23:
		ATR_CHR_NO(a2) = 100551;   //雷附体
		break;
#ifdef _PROFESSION_ADDSKILL
	case 32:
		ATR_CHR_NO(a2) = SPR_barrier; // 水附体
		break;
	case 33:
		ATR_CHR_NO(a2) = SPR_shock; //恐惧
		//case 24:
		//	ATR_CHR_NO(a2) = 0; // 火冰雷附体
		break;
#endif
#endif
#ifdef _PETSKILL_LER
	case 34:
		if (ATR_CHR_NO(a1) == 101814)
		{
			// 第一段变身
			if (g_bUseAlpha)
				ATR_CHR_NO(a2) = 101810; // 雾动画1
			else
				ATR_CHR_NO(a2) = 101863;	// 雾动画1
			ATR_V_POS(a2) = ATR_V_POS(a1);
			if (ATR_CHR_ACT_OLD(a1) != ANIM_DEAD)
			{
				ATR_VCT_NO(a1) = VCT_NO_DIE;
				ATR_CHR_NO(a1) = 101813;
			}
		}
		else if (ATR_CHR_NO(a1) == 101815)
		{
			// 第二段变身
			if (g_bUseAlpha)
				ATR_CHR_NO(a2) = 101811;	// 雾动画2
			else 
				ATR_CHR_NO(a2) = 101864; // 雾动画2
			ATR_V_POS(a2) = ATR_V_POS(a1);
			if (ATR_CHR_ACT_OLD(a1) != ANIM_DEAD)
			{
				ATR_VCT_NO(a1) = VCT_NO_DIE;
				ATR_CHR_NO(a1) = 101814;
			}
		}
		break;
#endif
#ifdef _PRO_KILLME
	case 35:
		ATR_CHR_NO(a2) = 101292; // 怒
		break;
#endif
#ifdef _PRO_ABSOLUTE_DEFENSE 
	case 36:
		ATR_CHR_NO(a2) = 102043;
		break;
#endif
#ifdef _PRO3_ADDSKILL
	case 37:
		ATR_CHR_NO(a2) = 102044;//37 恶魔诅咒
		break;
	case 38:
		ATR_CHR_NO(a2) = 102046;//38 毒素之网
		break;
	case 39:
		ATR_CHR_NO(a2) = 101296;//39 抚慰心灵
		break;
#endif
	}
}

/* ?? *******************************************************************/
void attrib_reverse(ACTION *a0)
{
	ACTION *a1;

	a1 = ATR_BODY_WORK(0, a0);		//??????????
	if (ATR_NAME(a1) == NULL || ATR_VCT_NO(a1) == VCT_NO_DIE + 2)		//???
	{
		ATR_ATTRIB_WORK(a1) = NULL;		//?
		DeathAction(a0);		//?
		return;
	}
	ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1) + 1;
	ATR_H_POS(a0) = ATR_H_POS(a1);
	ATR_V_POS(a0) = ATR_V_POS(a1) - 64;
	ATR_CHR_NO(a0) = SPR_zokusei;
	pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);		//???
}

/* ????? *******************************************************************/
void set_attrib_reverse(ACTION *a1)
{
	ACTION *a2;

	a2 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
	if (a2 == NULL)
		return;
	/* ??? */
	ATR_NAME(a2) = attrib_reverse;
	ATR_BODY_WORK(0, a2) = a1;
	ATR_ATTRIB_WORK(a1) = a2;
	/* ? */
	ATR_DISP_PRIO(a2) = ATR_DISP_PRIO(a1) + 1;
	/* ?????? */
	ATR_CHR_NO(a2) = CG_ATR_ICON_EARTH_BATTLE;
	/* ??? */
	ATR_H_POS(a2) = ATR_H_POS(a1);
	ATR_V_POS(a2) = ATR_V_POS(a1) - 64;
}


//int boomerang_pos_tbl[]= {0x320,	0x140,	0x2EE,	0xDC,	0x186,  0x6A,	0x214,	0xAA};
//int boomerang_pos_tbl2[]={0x2EE,	0x118,	0x2BC,	0xB4,   0x154,	0x32,	0x1BE,	0x64};
////嵍忋僌儖乕僾偺屻楍
//int boomerang_pos_tbl3a[]={0x104,	0x1A4,     0x64,	0x14A,	-0x60,	0xA0,	-0x92,	0x104};
//int boomerang_pos_tbl4a[]={0x154,	0x1C2,     0xB4,	0x190,	-0x92,	0xC8,	-0xC4,	0x12C};
////嵍忋僌儖乕僾偺慜楍
//int boomerang_pos_tbl3b[]={0xC4,	0x1A4,	0x44,	0x16A,	-0x60,	0xA0,	-0x92,	0x104};
//int boomerang_pos_tbl4b[]={0x154,	0x1C2,	0xB4,	0x1D0,	-0x92,	0xC8,	-0xC4,	0x12C};


#ifdef _BATTLE_PK_PLAYER_FOR_6VS6
int boomerang_pos_tbl[] = { 0x320, 0x140, 0x2EE, 0xDC, 0x186, 0x6A, 0x214, 0xAA };
int boomerang_pos_tbl2[] = { 0x2EE, 0x118, 0x2BC, 0xB4, 0x154, 0x32, 0x1BE, 0x64 };
//左上角位置索引小于15时   xiezi
int boomerang_pos_tbl3a[] = { 0x104 + 160, 0x1A4 + 120, 0x64 + 160, 0x14A + 120, -0x60 + 160, 0xA0 + 120, -0x92 + 160, 0x104 + 120 };
int boomerang_pos_tbl4a[] = { 0x154 + 160, 0x1C2 + 120, 0xB4 + 160, 0x190 + 120, -0x92 + 160, 0xC8 + 120, -0xC4 + 160, 0x12C + 120 };
//左上角位置索引大于等于15时	xiezi
int boomerang_pos_tbl3b[] = { 0xC4 + 160, 0x1A4 + 120, 0x44 + 160, 0x16A + 120, -0x60 + 160, 0xA0 + 120, -0x92 + 160, 0x104 + 120 };
int boomerang_pos_tbl4b[] = { 0x154 + 160, 0x1C2 + 120, 0xB4 + 160, 0x1D0 + 120, -0x92 + 160, 0xC8 + 120, -0xC4 + 160, 0x12C + 120 };
#else
int boomerang_pos_tbl[] = { 0x320, 0x140, 0x2EE, 0xDC, 0x186, 0x6A, 0x214, 0xAA };
int boomerang_pos_tbl2[] = { 0x2EE, 0x118, 0x2BC, 0xB4, 0x154, 0x32, 0x1BE, 0x64 };
//左上角位置索引小于15时   xiezi
int boomerang_pos_tbl3a[] = { 0x104 + 160, 0x1A4 + 120, 0x64 + 160, 0x14A + 120, -0x60 + 160, 0xA0 + 120, -0x92 + 160, 0x104 + 120 };
int boomerang_pos_tbl4a[] = { 0x154 + 160, 0x1C2 + 120, 0xB4 + 160, 0x190 + 120, -0x92 + 160, 0xC8 + 120, -0xC4 + 160, 0x12C + 120 };
//左上角位置索引大于等于15时	xiezi
int boomerang_pos_tbl3b[] = { 0xC4 + 160, 0x1A4 + 120, 0x44 + 160, 0x16A + 120, -0x60 + 160, 0xA0 + 120, -0x92 + 160, 0x104 + 120 };
int boomerang_pos_tbl4b[] = { 0x154 + 160, 0x1C2 + 120, 0xB4 + 160, 0x1D0 + 120, -0x92 + 160, 0xC8 + 120, -0xC4 + 160, 0x12C + 120 };
#endif


void boomerang(ACTION *a0)
{
	ACTION *a1, *a2;
	int d0, d1;
	float dx, dy;

	if (ATR_HIT_STOP(a0))
	{
		if (--ATR_HIT_STOP(a0))
			return;
	}
	switch (ATR_VCT_NO(a0))
	{
	case 0:
		if (ATR_GROUP_FLG(a0) == 0)
		{
			d0 = boomerang_pos_tbl[ATR_LONG_WORK(0, a0) * 2];
			d1 = boomerang_pos_tbl[ATR_LONG_WORK(0, a0) * 2 + 1];
		}
		else
		{
			if (ATR_PLACE_NO(a0) >= 15)//这里也是？
			{
				d0 = boomerang_pos_tbl3b[ATR_LONG_WORK(0, a0) * 2];
				d1 = boomerang_pos_tbl3b[ATR_LONG_WORK(0, a0) * 2 + 1];
			}
			else
			{
				d0 = boomerang_pos_tbl3a[ATR_LONG_WORK(0, a0) * 2];
				d1 = boomerang_pos_tbl3a[ATR_LONG_WORK(0, a0) * 2 + 1];
			}
		}

		if (ATR_FIRST_FLG(a0) == 0)
		{
			ATR_SPD(a0) = 40;
			radar(a0, &d0, &d1);
			ATR_CRS(a0) = d0;
			ATR_FIRST_FLG(a0) = 1;
		}
		else
			radar2(a0, d0, d1, ATR_LONG_WORK(1, a0));
		gemini(a0);
		dx = (float)(d0 - ATR_H_POS(a0));
		dy = (float)(d1 - ATR_V_POS(a0));
		d0 = (int)sqrt((double)(dx * dx + dy * dy));
		if (d0 < 20)
			ATR_VCT_NO(a0) = 1;

		break;
	case 1:
		if (ATR_SPD(a0) != 32)
			ATR_SPD(a0)--;
		if (ATR_GROUP_FLG(a0) == 0)
		{
			d0 = boomerang_pos_tbl2[ATR_LONG_WORK(0, a0) * 2];
			d1 = boomerang_pos_tbl2[ATR_LONG_WORK(0, a0) * 2 + 1];
		}
		else
		{
			if (ATR_PLACE_NO(a0) >= 15)
			{
				d0 = boomerang_pos_tbl4b[ATR_LONG_WORK(0, a0) * 2];
				d1 = boomerang_pos_tbl4b[ATR_LONG_WORK(0, a0) * 2 + 1];
			}
			else
			{
				d0 = boomerang_pos_tbl4a[ATR_LONG_WORK(0, a0) * 2];
				d1 = boomerang_pos_tbl4a[ATR_LONG_WORK(0, a0) * 2 + 1];
			}
		}
		radar2(a0, d0, d1, ATR_LONG_WORK(1, a0));
		gemini(a0);
		dx = (float)(d0 - ATR_H_POS(a0));
		dy = (float)(d1 - ATR_V_POS(a0));
		d0 = (int)sqrt((double)(dx * dx + dy * dy));
		if (d0 < 40)
		{
			d0 = get_num();
			if (d0 == 255)
			{
				ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);
				DeathAction(a0);
				p_missile[0] = NULL;
				return;
			}
			else
			{
				ATR_BODY_WORK(0, a0) = p_party[d0];
				ATR_ATTACK_KIND(0, a0) = get_num();
				ATR_ATTACK_POW(0, a0) = get_num();
				if (BattleCmd[command_point] == 'p')
					ATR_ATTACK_PET_POW(0, a0) = get_num();

				//修正技能特效 xiezi
#ifdef _ATTACK_EFFECT
				if (BattleCmd[command_point] == 's')
					ATR_LONG_WORK(0, a0) = get_num();	// 记录特效编号
#endif

				ATR_VCT_NO(a0) = 2;
			}
		}
		break;
	case 2:

		if (ATR_SPD(a0) != 40)
			ATR_SPD(a0)++;
		a1 = ATR_BODY_WORK(0, a0);
		d0 = ATR_H_POS(a1);
		d1 = ATR_V_POS(a1);
		radar2(a0, d0, d1, ATR_LONG_WORK(1, a0));
		gemini(a0);
		dx = (float)(d0 - ATR_H_POS(a0));
		dy = (float)(d1 - ATR_V_POS(a0));
		d0 = (int)sqrt((double)(dx * dx + dy * dy));

		if (d0 > 1000) //当回旋镖距离大于1000时，直接结束动画
		{
			ATR_COUNTER_FLG(a0) = 1;
			ATR_LONG_WORK(2, a0) = 0;

			if (ATR_PLACE_NO(a1) == 17)
			{
				ATR_H_POS(a0) = ATR_H_POS(a1) + 128;
				ATR_V_POS(a0) = ATR_V_POS(a1) - 96;
			}
			else if (ATR_PLACE_NO(a1) == 16)
			{
				ATR_H_POS(a0) = ATR_H_POS(a1) + 256;
				ATR_V_POS(a0) = ATR_V_POS(a1) - 192;
			}
			else
			{
				ATR_H_POS(a0) = ATR_H_POS(a1) + 256;
				ATR_V_POS(a0) = ATR_V_POS(a1) - 192;
			}
			return;
		}

		if (ATR_COUNTER_FLG(a0) == 1)
		{
			if (BattleMapNo >= 148 && BattleMapNo <= 150)
				d1 = 10 + 5;
			else
				d1 = 10;
		}
		else
		{
			if (BattleMapNo >= 148 && BattleMapNo <= 150)
				d1 = 20 + 10;
			else
				d1 = 20;
		}
		if (d0 < 80)
		{
			if (ATR_COUNTER_FLG(a0) == 1 && ATR_LONG_WORK(2, a0) == 0)
			{
				ATR_LONG_WORK(2, a0) = 1;
				a1 = ATR_BODY_WORK(1, a0);
				ATR_VCT_NO(a1) = 72;
				play_se(14, ATR_H_POS(a0), ATR_V_POS(a0));
			}
		}
		if (d0 < d1)
		{
			if (ATR_COUNTER_FLG(a0) == 1)
			{
				DeathAction(a0);
				p_missile[0] = NULL;
				return;
			}
			ATR_LONG_WORK(2, a0) = 0;
			a2 = ATR_BODY_WORK(0, a0);
			if (ATR_LIFE(a2) <= 0)
			{
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;
				goto boomerang_200;
			}
#ifdef _PETSKILL_ACUPUNCTURE
			////////////////////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_TRAP | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
				| ATT_ATTACKBACK
#endif
				))
#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
				| ATT_ATTACKBACK
#endif
				))
#endif
				////////////////////////////////////
#else
			////////////////////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_TRAP))
#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA))
#endif
				////////////////////////////////////
#endif
				set_guard_mark(a0);

			ATR_COMBO(a1) = 0;

			if (ATR_ATTACK_KIND(0, a0) & ATT_DODGE)
			{
				ATR_VCT_NO(a1) = 16;
				if (ATR_GROUP_FLG(a0) == 0)
					ATR_CRS(a1) = crs_change_tbl2[3];
				else
					ATR_CRS(a1) = crs_change_tbl2[7];
				ATR_DAMAGE_ANG(a1) = crs_change_tbl[ATR_CRS(a1)];
				ATR_STIMER(a1) = 0;
				ATR_FIRST_FLG(a1) = 0;
				goto boomerang_200;
			}

			if (!((ATR_ATTACK_KIND(0, a0) & ATT_BALLIA) | (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION)))
			{
				if ((ATR_ATTACK_POW(0, a0) == 0) && (ATR_ATTACK_PET_POW(0, a0) == 0))
				{
					a2 = ATR_BODY_WORK(0, a0);

					set_damage_num(a2, 0, -64);
					goto boomerang_200;
				}
			}

			if (ATR_ATTACK_KIND(0, a0) & ATT_CRUSH)
				set_damage_num(a1, 19, -112);

			ATR_AKO_FLG(a1) = 0;
			if (ATR_ATTACK_KIND(0, a0) & ATT_AKO1)
				ATR_AKO_FLG(a1) = 1;
			if (ATR_ATTACK_KIND(0, a0) & ATT_AKO2)
				ATR_AKO_FLG(a1) = 2;
			if (ATR_ATTACK_KIND(0, a0) & ATT_DEATH)
				ATR_LIFE(a1) = 0;
			if (ATR_GROUP_FLG(a0) == 0)
				ATR_DAMAGE_ANG(a1) = 1;
			else
				ATR_DAMAGE_ANG(a1) = 5;
			if (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION)
			{
				if (!ATR_COMBO(a1))
				{

					ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
					ATR_LIFE(a1) += ATR_DAMAGE(a1);
					if (ATR_LIFE(a1) > ATR_MAX_LIFE(a1))
						ATR_LIFE(a1) = ATR_MAX_LIFE(a1);
					ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
					ATR_PET_LIFE(a1) += ATR_PET_DAMAGE(a1);
					if (ATR_PET_LIFE(a1) > ATR_PET_MAX_LIFE(a1))
						ATR_PET_LIFE(a1) = ATR_PET_MAX_LIFE(a1);
					set_damage_num(a1, 14, -64);
					ATR_ATTACK_POW(0, a0) = 0;
					ATR_ATTACK_PET_POW(0, a0) = 0;
				}
			}
			else if (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA)
			{
				ATR_ATTACK_POW(0, a0) = 0;
				ATR_ATTACK_PET_POW(0, a0) = 0;
			}
			else
				ATR_VCT_NO(a1) = 10;
			if (ATR_ATTACK_KIND(0, a0) & ATT_SATISFACTORY)
				ATR_KAISHIN(a1) = 1;
			else
				ATR_KAISHIN(a1) = 0;
			ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
			ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);

			if (ATR_ATTACK_KIND(0, a0) & ATT_GUARD)
				ATR_GUARD_FLG(a1) = 1;
			else
				ATR_GUARD_FLG(a1) = 0;
			if (ATR_LIFE(a1) - ATR_DAMAGE(a1) <= 0)
			{

				if (ATR_COMBO(a1) == 0)
					ATR_KAISHIN(a1) = 1;
			}
			if (ATR_KAISHIN(a1))
				ATR_HIT_STOP(a0) = HIT_STOP_TIM * 4;
			else
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;

			if (ATR_DAMAGE(a1) || (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA) || (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION))
			{
				if (ATR_ATTACK_KIND(0, a0) & ATT_COUNTER)
				{
					if (ATR_COMBO(a1) == 0)
						set_damage_num(a0, 1, -64 + 16);
				}

				play_se(252, ATR_H_POS(a0), ATR_V_POS(a0));

				set_hit_mark(a0);
			}
		boomerang_200:
			ATR_HIT_STOP(a0) /= 4;
			d0 = get_num();
			if (d0 == 255)
			{
				ATR_BODY_WORK(0, a0) = ATR_BODY_WORK(1, a0);
				ATR_COUNTER_FLG(a0) = 1;
			}
			else
			{
				ATR_BODY_WORK(0, a0) = p_party[d0];
				ATR_ATTACK_KIND(0, a0) = get_num();
				ATR_ATTACK_POW(0, a0) = get_num();
				if (BattleCmd[command_point] == 'p')
					ATR_ATTACK_PET_POW(0, a0) = get_num();

				//修正技能特效 xiezi
#ifdef _ATTACK_EFFECT
				if (BattleCmd[command_point] == 's')
					ATR_LONG_WORK(0, a0) = get_num();	// 记录特效编号
#endif
			}
		}
		break;
	}
	pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
}


static char stick_ang_tbl[] = { 8, 10, 12, 14, 0, 2, 4, 6 };
/* ????? *******************************************************************/
void stick_bow(ACTION *a0)
{
	ACTION *a1, *a2;
	int d0, d1;

	a1 = ATR_BODY_WORK(0, a0);		//䁖??????
	if (ATR_NAME(a1) == NULL)		//䴓???
	{
		DeathAction(a0);		//?
		ATR_STIMER(ATR_BODY_WORK(1, a0))++;		//???
		return;
	}

	switch (ATR_VCT_NO(a0))
	{
	case 0:
		a1 = ATR_BODY_WORK(1, a0);		//????????
		if (ATR_VCT_NO(a1) == 1)
		{
			DeathAction(a0);		//?
			return;
		}
		ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1) + 1;
		a2 = ATR_BODY_WORK(0, a0);		//???????
		d0 = ATR_H_POS(a2);
		d1 = ATR_V_POS(a2);
		radar(a1, &d0, &d1);	//????
		ATR_STIMER(a0)++;
		if (ATR_CRS(a0) == 0)		//??
		{
			if (ATR_SPD(a0) == 0)		//?????
				ATR_CRS(a0) = 16;
			else
				ATR_SPD(a0) -= 1;
		}
		else
			ATR_SPD(a0) += 1;
		gemini(a0);
		ATR_H_POS(a0) += ATR_INT_WORK2(a1);
		ATR_V_POS(a0) += ATR_INT_WORK3(a1);
		break;
	case 1:
	case 2:
	case 3:
		a1 = ATR_BODY_WORK(0, a0);		//???????
#if 0
		if (ATR_VCT_NO(a0) == 2)		//????
		{
			if (ATR_VCT_NO(a1) >= VCT_NO_DIE + 1)		//?????
			{
				DeathAction(a0);		//?
				return;
			}
		}
		else
		{
			if (ATR_VCT_NO(a1) >= VCT_NO_DIE || ATR_VCT_NO(a1) == 0)		//???????
			{
				if (ATR_VCT_NO(a1) == 0)		//???
				{
					DeathAction(a0);		//?
					ATR_STIMER(ATR_BODY_WORK(1, a0))++;		//???
					return;
				}
				else
				{
					ATR_VCT_NO(a0) = 2;
					break;
				}
			}
		}
#else
		if (ATR_VCT_NO(a0) == 2)		//????
		{
			if (ATR_VCT_NO(a1) >= VCT_NO_DIE + 1)		//?????
			{
				DeathAction(a0);		//?
				return;
			}
		}
		else if (ATR_VCT_NO(a0) == 3)
		{
			if (ATR_VCT_NO(ATR_BODY_WORK(1, a0)) == 0)		//?????
			{
				DeathAction(a0);		//?
				return;
			}
		}
		else
		{
			if (ATR_VCT_NO(a1) >= VCT_NO_DIE || ATR_VCT_NO(a1) == 0)		//???????
			{
				ATR_STIMER(ATR_BODY_WORK(1, a0))++;		//???
				if (ATR_VCT_NO(a1) == 0)		//???
				{
					ATR_VCT_NO(a0) = 3;
					return;
				}
				else
				{
					ATR_VCT_NO(a0) = 2;
					break;
				}
			}
		}
#endif
		ATR_H_POS(a0) = ATR_H_POS(a1) + ATR_INT_WORK0(a0);
		ATR_V_POS(a0) = ATR_V_POS(a1) + ATR_INT_WORK1(a0);
		ATR_CHR_ANG(a0) = (ATR_CHR_ANG(a1) + 4) & 7;		//?????
		ATR_CHR_NO(a0) = CG_ARROW_00 + stick_ang_tbl[ATR_CHR_ANG(a0)];		//
		ATR_H_MINI(a0) = 0;		//?
		ATR_V_MINI(a0) = 0;
		ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a1)];
		gemini(a0);
		gemini(a0);
		if (ATR_CHR_ANG(a1) == 3 || ATR_CHR_ANG(a1) == 4 || ATR_CHR_ANG(a1) == 5)		//??????
			ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1);		//????
		else
			ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1) + 1;		//????
		break;
	}
	pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
}

#ifdef _SHOOTCHESTNUT	// Syu ADD 宠技：丢栗子
void shoot(ACTION *a0)
{
	ACTION *a1, *a2;
	int d0, d1;

	a1 = ATR_BODY_WORK(0, a0);
	if (ATR_NAME(a1) == NULL)
	{
		DeathAction(a0);
		ATR_STIMER(ATR_BODY_WORK(1, a0))++;
		return;
	}
	switch (ATR_VCT_NO(a0))
	{
	case 0:
		a1 = ATR_BODY_WORK(1, a0);
		if (ATR_VCT_NO(a1) == 1)
		{
			DeathAction(a0);
			return;
		}
		ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1) + 1;
		a2 = ATR_BODY_WORK(0, a0);
		d0 = ATR_H_POS(a2);
		d1 = ATR_V_POS(a2);
		radar(a1, &d0, &d1);
		ATR_STIMER(a0)++;
		if (ATR_CRS(a0) == 0)
		{
			if (ATR_SPD(a0) == 0)
				ATR_CRS(a0) = 16;
			else
				ATR_SPD(a0) -= 1;
		}
		else
			ATR_SPD(a0) += 1;
		gemini(a0);
		ATR_H_POS(a0) += ATR_INT_WORK2(a1);
		ATR_V_POS(a0) += ATR_INT_WORK3(a1);
		break;
	case 1:
	case 2:
	case 3:
		a1 = ATR_BODY_WORK(0, a0);
		if (ATR_VCT_NO(a0) == 2)
		{
			if (ATR_VCT_NO(a1) >= VCT_NO_DIE + 1)
			{
				DeathAction(a0);
				return;
			}
		}
		else if (ATR_VCT_NO(a0) == 3)
		{
			if (ATR_VCT_NO(ATR_BODY_WORK(1, a0)) == 0)
			{
				DeathAction(a0);
				return;
			}
		}
		else
		{
			if (ATR_VCT_NO(a1) >= VCT_NO_DIE || ATR_VCT_NO(a1) == 0)
			{
				ATR_STIMER(ATR_BODY_WORK(1, a0))++;
				if (ATR_VCT_NO(a1) == 0)
				{
					DeathAction(a0);
					ATR_VCT_NO(a0) = 3;
					return;
				}
				else
				{
					ATR_VCT_NO(a0) = 2;
					break;
				}
			}
		}
		ATR_H_POS(a0) = ATR_H_POS(a1) + ATR_INT_WORK0(a0);
		ATR_V_POS(a0) = ATR_V_POS(a1) + ATR_INT_WORK1(a0) + 32 + (a0->anim_cnt * 4);
		ATR_CHR_ANG(a0) = (ATR_CHR_ANG(a1) + 4) & 7;
		if (a0->anim_cnt == 5)
			ATR_CHR_NO(a0) = 0;
		else if (ATR_CHR_NO(a0) != 0)
			ATR_CHR_NO(a0) = 101611;
		ATR_H_MINI(a0) = 0;
		ATR_V_MINI(a0) = 0;
		ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a1)];
		gemini(a0);
		gemini(a0);
		if (ATR_CHR_ANG(a1) == 3 || ATR_CHR_ANG(a1) == 4 || ATR_CHR_ANG(a1) == 5)
			ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1);
		else
			ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1) + 1;
		break;
	}
	pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
}
#endif

/* ???? *******************************************************************/
void bow(ACTION *a0)
{
	ACTION *a1, *a2;
	int d0, d1;

	switch (ATR_VCT_NO(a0))
	{
	case 0:
		a1 = ATR_BODY_WORK(0, a0);		//????????
		d0 = ATR_H_POS(a1);
		d1 = ATR_V_POS(a1);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		//???????
		if (!--ATR_GROUP_FLG(a0))
		{
			a2 = ATR_BODY_WORK(0, a0);		//????????
			if (ATR_LIFE(a2) <= 0)		//??????
			{
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
				ATR_VCT_NO(a0) = 1;		//????????
				ATR_CHR_NO(a0) = ATR_LONG_WORK(0, a0);		//??????
				pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
				break;
			}
#ifdef _PETSKILL_ACUPUNCTURE
			///////////////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_TRAP | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
				| ATT_ATTACKBACK
#endif
				))
#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
				| ATT_ATTACKBACK
#endif
				))
#endif
				///////////////////////////////
#else
			///////////////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_TRAP))
#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA))
#endif
				///////////////////////////////
#endif
				set_guard_mark(a0);
			ATR_COMBO(a1) = 0;
			if (ATR_ATTACK_KIND(0, a0) & ATT_DODGE)
			{
				ATR_HIT_STOP(a0) = 32;		//??????????
				ATR_VCT_NO(a0) = 1;		//????????
				ATR_CHR_NO(a0) = ATR_LONG_WORK(0, a0);		//??????
				pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
				ATR_VCT_NO(a1) = 16;		//????
				ATR_CRS(a1) = ATR_CRS(a0);		//?????????????
				ATR_DAMAGE_ANG(a1) = ATR_CHR_ANG(a0);		//
				ATR_STIMER(a1) = 0;
				ATR_FIRST_FLG(a1) = 0;		//??????????
				break;
			}

			if (!((ATR_ATTACK_KIND(0, a0) & ATT_BALLIA) | (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION)))
			{
				if ((ATR_ATTACK_POW(0, a0) == 0) && (ATR_ATTACK_PET_POW(0, a0) == 0))		//????
				{
					a2 = ATR_BODY_WORK(0, a0);		//????????
					set_damage_num(a2, 0, -64);		//??
					ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
					ATR_VCT_NO(a0) = 1;		//????????
					ATR_CHR_NO(a0) = ATR_LONG_WORK(0, a0);		//??????
					pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
					break;
				}
			}
			//?????????
			if (ATR_ATTACK_KIND(0, a0) & ATT_CRUSH)
				//?????
				set_damage_num(a1, 19, -112);
			//?????????
			ATR_AKO_FLG(a1) = 0;
			if (ATR_ATTACK_KIND(0, a0) & ATT_AKO1)		//??????
				ATR_AKO_FLG(a1) = 1;
			if (ATR_ATTACK_KIND(0, a0) & ATT_AKO2)		//??????
				ATR_AKO_FLG(a1) = 2;
			if (ATR_ATTACK_KIND(0, a0) & ATT_DEATH)		//?????
				ATR_LIFE(a1) = 0;		//?????
			ATR_DAMAGE_ANG(a1) = ATR_CHR_ANG(a0);		//
			if (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION)		//??????
			{
				if (!ATR_COMBO(a1))			//????????
				{
					//??
					ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
					ATR_LIFE(a1) += ATR_DAMAGE(a1);
					if (ATR_LIFE(a1) > ATR_MAX_LIFE(a1))		//????
						ATR_LIFE(a1) = ATR_MAX_LIFE(a1);		//????
					ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
					ATR_PET_LIFE(a1) += ATR_PET_DAMAGE(a1);
					if (ATR_PET_LIFE(a1) > ATR_PET_MAX_LIFE(a1))		//????
						ATR_PET_LIFE(a1) = ATR_PET_MAX_LIFE(a1);		//????
					set_damage_num(a1, 14, -64);
					ATR_ATTACK_POW(0, a0) = 0;		//????䥺?
					ATR_ATTACK_PET_POW(0, a0) = 0;		//????䥺?
				}
			}
			else if (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA)		//????????
			{
				ATR_ATTACK_POW(0, a0) = 0;		//????䥺?
				ATR_ATTACK_PET_POW(0, a0) = 0;		//????䥺?
			}
			else
				ATR_VCT_NO(a1) = 10;		//?????
			//??????
			if (ATR_ATTACK_KIND(0, a0) & ATT_SATISFACTORY)
				ATR_KAISHIN(a1) = 1;		//???????
			else
				ATR_KAISHIN(a1) = 0;		//?????
			ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
			ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
			//?????
			if (ATR_ATTACK_KIND(0, a0) & ATT_GUARD)
				ATR_GUARD_FLG(a1) = 1;
			else
				ATR_GUARD_FLG(a1) = 0;
			if (ATR_LIFE(a1) - ATR_DAMAGE(a1) <= 0)		//?????
			{
				//????????
				if (ATR_COMBO(a1) == 0)
					ATR_KAISHIN(a1) = 1;		//???????
			}
			if (ATR_KAISHIN(a1))		//??????
				ATR_HIT_STOP(a0) = HIT_STOP_TIM * 4;		//??????????
			else
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
			//?????????????
			if (ATR_DAMAGE(a1) || (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA) || (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION))
			{
				if (ATR_ATTACK_KIND(0, a0) & ATT_COUNTER)		//???????
				{
					//????????
					if (ATR_COMBO(a1) == 0)
						set_damage_num(a0, 1, -64 + 16);	//?????
				}
				//???
				play_se(12, ATR_H_POS(a0), ATR_V_POS(a0));
				//?????????
				set_hit_mark(a0);
			}
			ATR_STIMER(a0) = ATR_HIT_STOP(a0) + 64;		//???????
			ATR_INT_WORK0(a0) = Rnd(0, 8) - 4;		//???????
			ATR_INT_WORK1(a0) = Rnd(0, 8) - 4 - 28;		//???????!!!
#ifdef _SHOOTCHESTNUT	// Syu ADD 宠技：丢栗子
			if (ShooterNum == 101578)
				ATR_NAME(a0) = shoot;		//?????
			else
				ATR_NAME(a0) = stick_bow;		//?????
#else
			ATR_NAME(a0) = stick_bow;		//?????
#endif
			ATR_VCT_NO(a0) = 1;				//
			ATR_CHR_NO(a0) = ATR_LONG_WORK(0, a0);		//??????
			ATR_V_POS(a0) -= 28;		//!!!
			ATR_CHR_ACT(a0) = 0;
			p_missile[0] = NULL;		//???????
			ATR_SPD(a0) = 40;		//?
			//???
		}
		else
		{
			ATR_INT_WORK2(a0) = ATR_H_POS(a0);		//???
			ATR_INT_WORK3(a0) = ATR_V_POS(a0);		//
			gemini(a0);		//?
			ATR_INT_WORK2(a0) = ATR_H_POS(a0) - ATR_INT_WORK2(a0);
			ATR_INT_WORK3(a0) = ATR_V_POS(a0) - ATR_INT_WORK3(a0);
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 1:
		if (--ATR_HIT_STOP(a0))		//??????????
			break;
		//???????
		if (ATR_ATTACK_KIND(0, a0) & ATT_DODGE)
			ATR_STIMER(a0) = 32;
		else
			ATR_STIMER(a0) = 64;
		ATR_VCT_NO(a0) = 2;
		a0->atr = ACT_ATR_HIDE;
		p_missile[0] = NULL;
		break;
	case 2:
		if (--ATR_STIMER(a0))
			break;
		ATR_STIMER(ATR_BODY_WORK(1, a0))++;		//???
		DeathAction(a0);		//?
		break;
	}
}

/* ????? *******************************************************************/
void axe(ACTION *a0)
{
	ACTION *a1, *a2;
	int d0, d1;

	switch (ATR_VCT_NO(a0))
	{
	case 0:
		a1 = ATR_BODY_WORK(1, a0);		//????????
		if (ATR_VCT_NO(a1) == 1)
		{
			DeathAction(a0);		//?
			return;
		}
		ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1) + 1;
		a2 = ATR_BODY_WORK(0, a0);		//???????
		d0 = ATR_H_POS(a2);
		d1 = ATR_V_POS(a2);
		radar(a1, &d0, &d1);	//????
		ATR_STIMER(a0)++;
		if (ATR_CRS(a0) == 0)		//??
		{
			if (ATR_SPD(a0) == 0)		//?????
				ATR_CRS(a0) = 16;
			else
				ATR_SPD(a0) -= 1;
		}
		else
			ATR_SPD(a0) += 1;
		gemini(a0);
		ATR_H_POS(a0) += ATR_INT_WORK2(a1);
		ATR_V_POS(a0) += ATR_INT_WORK3(a1);
		break;
	case 1:
		break;
	case 2:
		ATR_VCT_NO(a0)++;
		break;
	case 3:
		a1 = ATR_BODY_WORK(1, a0);		//????????
		if (ATR_VCT_NO(a1) == 1)
		{
			DeathAction(a0);		//?
			return;
		}
		ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1) + 1;
		if (ATR_CRS(a0) == 0)		//??
		{
			if (ATR_SPD(a0) == 0)		//?????
				ATR_CRS(a0) = 16;
			else
				ATR_SPD(a0) -= 2;
		}
		else
			ATR_SPD(a0) += 2;
		gemini(a0);
		ATR_H_POS(a0) += ATR_INT_WORK2(a1);
		ATR_V_POS(a0) += ATR_INT_WORK3(a1);
		break;
	case 4:
		a1 = ATR_BODY_WORK(1, a0);		//????????
		ATR_H_POS(a0) = ATR_H_POS(a1);
		ATR_V_POS(a0) = ATR_V_POS(a1);
		ATR_CHR_ACT(a0) = 1;		//?????????
		break;
	}
	pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
}

#ifdef _FIREHUNTER_SKILL				// (不可开) ROG ADD 朱雀技能_火线猎杀
void fireHunter(ACTION *a0)
{
	ACTION *a1, *a2;
	int i = 0;
	int j = 0;
	int d0, d1;

	a1 = ATR_BODY_WORK(0, a0);		//敌人的位址

	switch (ATR_VCT_NO(a0))
	{
	case 0:								//发射火焰
		if (bFireInit)
		{
			int EnemyPos[4][2] = {{403 , 422}, {223 , 388},{7  , 198},{19  , 318},};
			int refPos, difPosH, difPosV;
			int interval = 0;
			int difPos_v = 0;   //火线猎杀位址修正
			int difPos_h = 0;   //火线猎杀位址修正
			int j = 1;

			ATR_INT_WORK2(a0) = ATR_H_POS(a0);
			ATR_INT_WORK3(a0) = ATR_V_POS(a0);
			for (i = 2; i < 8 ; i++)
			{
				p_missile[i] = GetAction(T_PRIO_BOW, sizeof(ATR_EQU));
				ATR_INT_WORK2(p_missile[i]) = ATR_H_POS(a0);
				ATR_INT_WORK3(p_missile[i]) = ATR_V_POS(a0);
				ATR_CHR_NO(p_missile[i]) = 101734;	 //图号
				ATR_CHR_ANG(p_missile[i]) = ATR_CHR_ANG(a0);
				ATR_CRS(p_missile[i]) = ATR_CRS(a0);
				ATR_SPD(p_missile[i]) = 60;
			}
			if (iBeAttNum < 5)
			{
				refPos = 0;
				difPosH = 32;
				difPosV = 24;
			}
			else if (iBeAttNum < 10)
			{
				refPos = 1;
				difPosH = 64;
				difPosV = 48;
			}
			else if (iBeAttNum < 15)
			{
				refPos = 2;
				difPosH = 32;
				difPosV = 24;
			}
			else if (iBeAttNum < 20)
			{
				refPos = 3;
				difPosH = 64;
				difPosV = 48;
			}
			for (i = 1; i < 9; i++)
			{
				p_missile[i + 7] = GetAction(T_PRIO_BOW, sizeof(ATR_EQU));
				ATR_H_POS(p_missile[i + 7]) = EnemyPos[refPos][0] + difPosH * i;
				ATR_V_POS(p_missile[i + 7]) = EnemyPos[refPos][1] - difPosV * i;
				ATR_CHR_NO(p_missile[i + 7]) = 101735;	 //图号
			}
			ATR_STIMER(a0) = 80;

			if (iBeAttNum == 18 || iBeAttNum == 11 || iBeAttNum == 19)
				interval = ATR_GROUP_FLG(a0) / 7;
			else			
				interval = ATR_GROUP_FLG(a0) / 6;
			for (i = 0; i < ATR_GROUP_FLG(a0); i++)
			{
				d0 = ATR_H_POS(a1);
				d1 = ATR_V_POS(a1);
				radar(a0, &d0, &d1);	//雷达(计算路线)
				ATR_CRS(a0) = d0;		//动画路线
				gemini(a0);
				if (i == interval * j)
				{
					switch (iBeAttNum)		//火线猎杀位址修正
					{
					case 10:
					case 12:
						difPos_v += 7;  //长度
						difPos_h += 2;	//角度
						break;
					case 14:
						difPos_v += 10;
						difPos_h += 2;
						break;
					case 15:
						difPos_v += 4;
						break;
					case 16:
						difPos_v += 5;
						difPos_h -= 5;
						break;
					case 17:
						difPos_v += 3;
						break;
					case 18:
						difPos_v += 5;
						difPos_h -= 5;
						break;
					case 19:
						difPos_v += 17;
						difPos_h -= 2;
						break;
					case 11:
						difPos_v += 5;
						difPos_h -= 5;
						break;
						/*			case 8:
										difPos_v += 7;
										difPos_h -= 5;
										break;
										case 5:
										difPos_v +=7;
										difPos_h -=10;
										break;*/
					}
					if (iBeAttNum < 10)
					{
						//difPos_h -= 10;//角度
						difPos_v += 10;  //长度
					}
					ATR_V_POS(p_missile[j+1]) = ATR_V_POS(a0) - difPos_v;
					ATR_H_POS(p_missile[j+1]) = ATR_H_POS(a0) + difPos_h;
					if (j < 6)
						j++;
				}
			}
			ATR_V_POS(a0) -= difPos_v;
			ATR_H_POS(a0) += difPos_h;
			bFireInit = FALSE;
		}
		//准备命中
		if (!FireSkillEnd)
		{
#ifdef _PETSKILL_ACUPUNCTURE
			/////////////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_TRAP | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
				| ATT_ATTACKBACK
#endif
				))
#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
				| ATT_ATTACKBACK
#endif
				))
#endif
				/////////////////////////////
#else
			/////////////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_TRAP))
#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA))
#endif
				/////////////////////////////
#endif
				set_guard_mark(a0);		//防御咒
			ATR_COMBO(a1) = 0;   //组队
			if (ATR_ATTACK_KIND(0, a0) & ATT_DODGE)  //闪避
			{
				for (i = 0; i < ATR_BODY_CNT(a0); i++) //处理全部队友
				{
					if ((i + 2) != tarpos)
					{
						a2 = ATR_BODY_WORK(i + 2, a0);
						ATR_DAMAGE(a2) = ATR_ATTACK_POW(i + 2, a0);
						j = ATR_DAMAGE(a2);
						ATR_PET_DAMAGE(a2) = ATR_ATTACK_PET_POW(i + 2, a0);
						ATR_VCT_NO(a2) = 10;		//送出伤害讯息
						ATR_STIMER(a2) = 0;
						ATR_FIRST_FLG(a2) = 0;		
					}
				}
				ATR_HIT_STOP(a0) = 32;				//攻击停止
				ATR_VCT_NO(a0) = 1;					//停止
				gemini(a0);
				ATR_VCT_NO(a1) = 16;		//闪避动作
				ATR_CRS(a1) = ATR_CRS(a0);		//路径
				ATR_DAMAGE_ANG(a1) = ATR_CHR_ANG(a0);
				ATR_STIMER(a1) = 0;
				ATR_FIRST_FLG(a1) = 0;		
				break;
			}
			if (!((ATR_ATTACK_KIND(0, a0) & ATT_BALLIA) | (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION)))
			{
				if ((ATR_ATTACK_POW(0, a0) == 0) && (ATR_ATTACK_PET_POW(0, a0) == 0))		//????
				{
					//伤害
					set_damage_num(a1, 0, -64);
					ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//攻击停止
					ATR_VCT_NO(a0) = 1;		//攻击停止
					for (i = 0; i < ATR_BODY_CNT(a0); i++) //处理全部队友
					{
						if ((i + 2) != tarpos)
						{
							a2 = ATR_BODY_WORK(i + 2, a0);
							ATR_DAMAGE(a2) = ATR_ATTACK_POW(i + 2, a0);
							j = ATR_DAMAGE(a2);
							ATR_PET_DAMAGE(a2) = ATR_ATTACK_PET_POW(i + 2, a0);
							ATR_VCT_NO(a2) = 10;		//送出伤害讯息
							ATR_STIMER(a2) = 0;
							ATR_FIRST_FLG(a2) = 0;		
						}
					}
					break;
				}
			}
			//压碎
			if (ATR_ATTACK_KIND(0, a0) & ATT_CRUSH)
				//压碎
				set_damage_num(a1, 19, -112);
			//设定KO
			ATR_AKO_FLG(a1) = 0;
			if (ATR_ATTACK_KIND(0, a0) & ATT_AKO1)		//KO1
				ATR_AKO_FLG(a1) = 1;
			if (ATR_ATTACK_KIND(0, a0) & ATT_AKO2)		//KO2
				ATR_AKO_FLG(a1) = 2;
			if (ATR_ATTACK_KIND(0, a0) & ATT_DEATH)		//死亡
				ATR_LIFE(a1) = 0;		//设定死亡
			ATR_DAMAGE_ANG(a1) = ATR_CHR_ANG(a0);		//设定伤害角度
			if (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION)		//如果是吸收
			{
				if (!ATR_COMBO(a1))		//组队
				{
					//??
					ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
					ATR_LIFE(a1) += ATR_DAMAGE(a1);
					if (ATR_LIFE(a1) > ATR_MAX_LIFE(a1))		//????
						ATR_LIFE(a1) = ATR_MAX_LIFE(a1);		//????
					ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
					ATR_PET_LIFE(a1) += ATR_PET_DAMAGE(a1);
					if (ATR_PET_LIFE(a1) > ATR_PET_MAX_LIFE(a1))		//????
						ATR_PET_LIFE(a1) = ATR_PET_MAX_LIFE(a1);		//????
					set_damage_num(a1, 14, -64);
					ATR_ATTACK_POW(0, a0) = 0;		
					ATR_ATTACK_PET_POW(0, a0) = 0;		
				}
			}
			else if (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA)			//????????
			{
				ATR_ATTACK_POW(0, a0) = 0;		//????䥺?
				ATR_ATTACK_PET_POW(0, a0) = 0;		//????䥺?
			}
			else
				ATR_VCT_NO(a1) = 10;		//送出伤害讯息
			/*			//会心一击
						if (ATR_ATTACK_KIND(0, a0) & ATT_SATISFACTORY){
						ATR_KAISHIN(a1) = 1;		//会心一击
						} else {
						ATR_KAISHIN(a1) = 0;		//通常攻击
						}*/
			ATR_KAISHIN(a1) = 0;		//通常攻击
			ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
			ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
			//防卫
			if (ATR_ATTACK_KIND(0, a0) & ATT_GUARD)
				ATR_GUARD_FLG(a1) = 1;
			else
				ATR_GUARD_FLG(a1) = 0;
			if (ATR_LIFE(a1) - ATR_DAMAGE(a1) <= 0)		//死亡
			{
				ATR_LONG_WORK(1, a0) = 1;		//死亡设定
				//组队
				if (ATR_COMBO(a1) == 0)
					ATR_KAISHIN(a1) = 1;		//设定会心一击
			}
			if (ATR_KAISHIN(a1))		//设定会心一击
				ATR_HIT_STOP(a0) = HIT_STOP_TIM * 4;		//设定攻击停止时间
			else
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//设定攻击停止时间
			if (ATR_DAMAGE(a1) || (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA) || (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION))
			{
				if (ATR_ATTACK_KIND(0, a0) & ATT_COUNTER)		//???????
				{
					//是否组队
					if (ATR_COMBO(a1) == 0)
						set_damage_num(a0, 1, -64 + 16);	//伤害表示
				}
				//???
				play_se(252, ATR_H_POS(a0), ATR_V_POS(a0));
				//必杀星星
				set_hit_mark(a0);
			}
			ATR_HIT_STOP(a0) /= 4;		//??????????
			ATR_VCT_NO(a0) = 3;				//
			for (i = 0; i < ATR_BODY_CNT(a0); i++) //处理全部队友
			{
				if ((i + 2) != tarpos)
				{
					a2 = ATR_BODY_WORK(i + 2, a0);
					ATR_DAMAGE(a2) = ATR_ATTACK_POW(i + 2, a0);
					j = ATR_DAMAGE(a2);
					ATR_PET_DAMAGE(a2) = ATR_ATTACK_PET_POW(i + 2, a0);
					ATR_VCT_NO(a2) = 10;		//送出伤害讯息
					ATR_STIMER(a2) = 0;
					ATR_FIRST_FLG(a2) = 0;
				}
			}
		}
		else
		{
			if (ATR_STIMER(a0))
				ATR_STIMER(a0)--;

			if (p_missile[2] != NULL)
			{
				if (pattern(p_missile[2], ANM_NOMAL_SPD, ANM_NO_LOOP))
				{
					DeathAction(p_missile[2]);
					p_missile[2] = NULL;
				}
			}
			if (ATR_STIMER(a0) < 70 && p_missile[3] != NULL)
			{
				if (pattern(p_missile[3], ANM_NOMAL_SPD, ANM_NO_LOOP))
				{
					DeathAction(p_missile[3]);
					p_missile[3] = NULL;
				}
			}
			if (ATR_STIMER(a0) < 60 && p_missile[4] != NULL)
			{
				if (pattern(p_missile[4], ANM_NOMAL_SPD, ANM_NO_LOOP))
				{
					DeathAction(p_missile[4]);
					p_missile[4] = NULL;
				}
			}
			if (ATR_STIMER(a0) < 50 && p_missile[5] != NULL)
			{
				if (pattern(p_missile[5], ANM_NOMAL_SPD, ANM_NO_LOOP))
				{
					DeathAction(p_missile[5]);
					p_missile[5] = NULL;
				}
			}
			if (ATR_STIMER(a0) < 40 && p_missile[6] != NULL)
			{
				if (pattern(p_missile[6], ANM_NOMAL_SPD, ANM_NO_LOOP))
				{
					DeathAction(p_missile[6]);
					p_missile[6] = NULL;
				}
			}
			if (iBeAttNum > 9)
			{
				if (ATR_STIMER(a0) < 30 && p_missile[7] != NULL)
				{
					if (pattern(p_missile[7], ANM_NOMAL_SPD, ANM_NO_LOOP))
					{
						DeathAction(p_missile[7]);
						p_missile[7] = NULL;
					}
				}
			}
			if (ATR_STIMER(a0) < 20)
			{
				if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP) && p_missile[6] == NULL)
				{
					counter = 0;
					ATR_VCT_NO(a0) = 8;
				}
			}
		}
		break;
	case 1:
		if (--ATR_HIT_STOP(a0))		//攻击停止中
			break;
		ATR_CHR_ACT(a0) = 0;		//移动
		//闪避
		if (ATR_ATTACK_KIND(0, a0) & ATT_DODGE)
			ATR_STIMER(a0) = 32;
		else
			ATR_STIMER(a0) = 64;
		if (tarpos != -1)
		{
			a2 = ATR_BODY_WORK(tarpos, a0);
			ATR_DAMAGE(a2) = ATR_ATTACK_POW(tarpos, a0);
			ATR_PET_DAMAGE(a2) = ATR_ATTACK_PET_POW(tarpos, a0);
			ATR_VCT_NO(a2) = 10;		//送出伤害讯息
			ATR_STIMER(a2) = 0;
			ATR_FIRST_FLG(a2) = 0;
			tarpos = -1;
		}
		ATR_VCT_NO(a0) = 2;
		a0->atr = ACT_ATR_HIDE;
		p_missile[0] = NULL;
		p_missile[1] = NULL;
		break;
	case 2:
		if (--ATR_STIMER(a0))
			break;
		ATR_STIMER(ATR_BODY_WORK(1, a0)) = 1;		//到达
		p_missile[0] = NULL;
		DeathAction(a0);		//?
		break;
	case 3:
		if (--ATR_HIT_STOP(a0))		//攻击停止中
			break;
		/*	if ( tarMgiDem )
			{
			a2 = ATR_BODY_WORK(2 ,a0);
			ATR_DAMAGE(a2) = ATR_ATTACK_POW(2 ,a0);
			ATR_PET_DAMAGE(a2) = ATR_ATTACK_PET_POW(2 ,a0);
			ATR_VCT_NO(a2) = 10;		//送出伤害讯息
			ATR_STIMER(a2) = 0;
			ATR_FIRST_FLG(a2) = 0;		
			int k = ATR_DAMAGE(a2);
			set_damage_num(a2, 0, -64);
			tarMgiDem = 0;
			}*/
		if (tarpos != -1)
		{
			a2 = ATR_BODY_WORK(tarpos, a0);
			ATR_DAMAGE(a2) = ATR_ATTACK_POW(tarpos, a0);
			ATR_PET_DAMAGE(a2) = ATR_ATTACK_PET_POW(tarpos, a0);
			ATR_VCT_NO(a2) = 10;		//送出伤害讯息
			ATR_STIMER(a2) = 0;
			ATR_FIRST_FLG(a2) = 0;
			tarpos = -1;
		}
		ATR_GROUP_FLG(a0) = 35;
		ATR_SPD(a0) = 16;
		ATR_VCT_NO(a0)++;
		break;
	case 4:
		ATR_INT_WORK2(a0) = ATR_H_POS(a0);		//???
		ATR_INT_WORK3(a0) = ATR_V_POS(a0);		//
		gemini(a0);		//?
		ATR_INT_WORK2(a0) = ATR_H_POS(a0) - ATR_INT_WORK2(a0);
		ATR_INT_WORK3(a0) = ATR_V_POS(a0) - ATR_INT_WORK3(a0);
		if (!--ATR_GROUP_FLG(a0))		//???
		{
			if (ATR_LONG_WORK(1, a0))		//???????
				ATR_STIMER(ATR_BODY_WORK(1, a0)) = 1;		//???
			ATR_VCT_NO(a0)++;
		}
		break;
	case 5:
		play_se(251, ATR_H_POS(a0), ATR_V_POS(a0));
		ATR_VCT_NO(a0)++;
		ATR_STIMER(a0) = 30;
		break;
	case 6:
		if (ATR_STIMER(a0))
		{
			ATR_STIMER(a0)--;
			break;
		}
		ATR_ATTRIB(a0) = ACT_ATR_HIDE;		//??
		a1 = ATR_BODY_WORK(0, a0);		//????????
		if (ATR_NAME(a1) == NULL || ATR_VCT_NO(a1) >= VCT_NO_DIE || ATR_VCT_NO(a1) == 0)		//???????
		{
			if (!ATR_LONG_WORK(1, a0))		//??????
				ATR_STIMER(ATR_BODY_WORK(1, a0)) = 1;		//???
			DeathAction(a0);		//?
			p_missile[0] = NULL;	//
			p_missile[1] = NULL;	//
		}
		break;
	case 7:
		ATR_VCT_NO(a0)++;
		ATR_STIMER(a0) =60;
		break;
	case 8:
		for (i = 8; i < 13; i++)
		{
			if (p_missile[i] != NULL)
			{
				if (pattern(p_missile[i], ANM_NOMAL_SPD, ANM_NO_LOOP))
				{
					DeathAction(p_missile[i]);
					p_missile[i] = NULL;
					counter++;
				}
			}
		}
		if (counter == 5)
		{
			ATR_ATTRIB(a0) = ACT_ATR_HIDE;		//??
			FireSkillEnd = FALSE;
			ATR_VCT_NO(a0) = 0;		
		}
		break;
	}
}
#endif

/* ?????? *******************************************************************/
void axe_shadow(ACTION *a0)
{
	ACTION *a1, *a2;
	int d0, d1;

	switch (ATR_VCT_NO(a0))
	{
	case 0:
		a1 = ATR_BODY_WORK(0, a0);		//????????
		d0 = ATR_H_POS(a1);
		d1 = ATR_V_POS(a1);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		//???????
		if (!--ATR_GROUP_FLG(a0))
		{
			a2 = ATR_BODY_WORK(0, a0);		//????????
			if (ATR_LIFE(a2) <= 0)		//??????
			{
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
				ATR_VCT_NO(a0) = 1;		//????????
				ATR_CHR_NO(a0) = ATR_LONG_WORK(0, a0);		//??????
				break;
			}
#ifdef _PETSKILL_ACUPUNCTURE
			/////////////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_TRAP | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
				| ATT_ATTACKBACK
#endif
				))
#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
				| ATT_ATTACKBACK
#endif
				))
#endif
				/////////////////////////////
#else
			/////////////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_TRAP))
#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA))
#endif
				/////////////////////////////
#endif
				set_guard_mark(a0);		//㘎??
			//?????????
			ATR_COMBO(a1) = 0;
			//???????
			if (ATR_ATTACK_KIND(0, a0) & ATT_DODGE)
			{
				ATR_HIT_STOP(a0) = 32;		//??????????
				ATR_VCT_NO(a0) = 1;		//????????
				ATR_CHR_NO(a0) = ATR_LONG_WORK(0, a0);		//??????
				ATR_CHR_ACT(a0) = 1;		//?????????
				gemini(a0);
				ATR_VCT_NO(a1) = 16;		//????
				ATR_CRS(a1) = ATR_CRS(a0);		//?????????????
				ATR_DAMAGE_ANG(a1) = ATR_CHR_ANG(a0);		//
				ATR_STIMER(a1) = 0;
				ATR_FIRST_FLG(a1) = 0;		//??????????
				break;
			}
			//????????????
			if (!((ATR_ATTACK_KIND(0, a0) & ATT_BALLIA) | (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION)))
			{
				if ((ATR_ATTACK_POW(0, a0) == 0) && (ATR_ATTACK_PET_POW(0, a0) == 0))		//????
				{
					a2 = ATR_BODY_WORK(0, a0);		//????????
					//??
					set_damage_num(a2, 0, -64);
					ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
					ATR_VCT_NO(a0) = 1;		//????????
					ATR_CHR_ACT(a0) = 1;		//?????????
					ATR_CHR_NO(a0) = ATR_LONG_WORK(0, a0);		//??????
					break;
				}
			}
			//?????????
			if (ATR_ATTACK_KIND(0, a0) & ATT_CRUSH)
				set_damage_num(a1, 19, -112);	//?????
			//?????????
			ATR_AKO_FLG(a1) = 0;
			if (ATR_ATTACK_KIND(0, a0) & ATT_AKO1)		//??????
				ATR_AKO_FLG(a1) = 1;
			if (ATR_ATTACK_KIND(0, a0) & ATT_AKO2)		//??????
				ATR_AKO_FLG(a1) = 2;
			if (ATR_ATTACK_KIND(0, a0) & ATT_DEATH)		//?????
				ATR_LIFE(a1) = 0;		//?????
			ATR_DAMAGE_ANG(a1) = ATR_CHR_ANG(a0);		//
			if (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION)		//??????
			{
				if (!ATR_COMBO(a1))		//????????
				{
					//??
					ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
					ATR_LIFE(a1) += ATR_DAMAGE(a1);
					if (ATR_LIFE(a1) > ATR_MAX_LIFE(a1))		//????
						ATR_LIFE(a1) = ATR_MAX_LIFE(a1);		//????
					ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
					ATR_PET_LIFE(a1) += ATR_PET_DAMAGE(a1);
					if (ATR_PET_LIFE(a1) > ATR_PET_MAX_LIFE(a1))		//????
						ATR_PET_LIFE(a1) = ATR_PET_MAX_LIFE(a1);		//????
					set_damage_num(a1, 14, -64);
					ATR_ATTACK_POW(0, a0) = 0;		//????䥺?
					ATR_ATTACK_PET_POW(0, a0) = 0;		//pet????䥺?
				}
			}
			else if (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA)		//????????
			{
				ATR_ATTACK_POW(0, a0) = 0;		//????䥺?
				ATR_ATTACK_PET_POW(0, a0) = 0;		//????䥺?
			}
			else
				ATR_VCT_NO(a1) = 10;		//?????
			//??????
			if (ATR_ATTACK_KIND(0, a0) & ATT_SATISFACTORY)
				ATR_KAISHIN(a1) = 1;		//???????
			else
				ATR_KAISHIN(a1) = 0;		//?????
			ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
			ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
			//?????
			if (ATR_ATTACK_KIND(0, a0) & ATT_GUARD)
				ATR_GUARD_FLG(a1) = 1;
			else
				ATR_GUARD_FLG(a1) = 0;
			if (ATR_LIFE(a1) - ATR_DAMAGE(a1) <= 0)		//?????
			{
				ATR_LONG_WORK(1, a0) = 1;		//???????
				//????????
				if (ATR_COMBO(a1) == 0)
					ATR_KAISHIN(a1) = 1;		//???????
			}
			if (ATR_KAISHIN(a1))		//??????
				ATR_HIT_STOP(a0) = HIT_STOP_TIM * 4;		//??????????
			else
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
			//?????????????
			if (ATR_DAMAGE(a1) || (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA) || (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION))
			{
				if (ATR_ATTACK_KIND(0, a0) & ATT_COUNTER)		//???????
				{
					//????????
					if (ATR_COMBO(a1) == 0)
						set_damage_num(a0, 1, -64 + 16);//?????
				}
				//???
				play_se(252, ATR_H_POS(a0), ATR_V_POS(a0));
				//?????????
				set_hit_mark(a0);
			}
			ATR_HIT_STOP(a0) /= 4;		//??????????
			ATR_VCT_NO(a0) = 3;				//????????
			a1 = ATR_BODY_WORK(2, a0);		//???????
			ATR_VCT_NO(a1) = 1;				//????????
			ATR_CHR_ACT(a1) = 1;		//?????????
		}
		else
		{
			ATR_INT_WORK2(a0) = ATR_H_POS(a0);		//???
			ATR_INT_WORK3(a0) = ATR_V_POS(a0);		//
			gemini(a0);		//?
			ATR_INT_WORK2(a0) = ATR_H_POS(a0) - ATR_INT_WORK2(a0);
			ATR_INT_WORK3(a0) = ATR_V_POS(a0) - ATR_INT_WORK3(a0);
		}
		break;
	case 1:
		if (--ATR_HIT_STOP(a0))		//??????????
			break;
		ATR_CHR_ACT(a0) = 0;		//????????
		//???????
		if (ATR_ATTACK_KIND(0, a0) & ATT_DODGE)
			ATR_STIMER(a0) = 32;
		else
			ATR_STIMER(a0) = 64;
		ATR_VCT_NO(a0) = 2;
		a0->atr = ACT_ATR_HIDE;
		p_missile[0] = NULL;
		break;
	case 2:
		if (--ATR_STIMER(a0))
			break;
		ATR_STIMER(ATR_BODY_WORK(1, a0)) = 1;		//???
		DeathAction(a0);		//?
		break;
	case 3:
		if (--ATR_HIT_STOP(a0))		//??????????
			break;
		a1 = ATR_BODY_WORK(2, a0);		//???????
		ATR_CHR_ACT(a1) = 0;		//????????
		//???????????
		ATR_GROUP_FLG(a0) = 35;
		ATR_SPD(a0) = 16;
		a1 = ATR_BODY_WORK(2, a0);		//???????
		ATR_VCT_NO(a1) = 2;				//???
		ATR_SPD(a1) = 32;
		ATR_CRS(a1) = 0;
		ATR_VCT_NO(a0)++;
		break;
	case 4:
		ATR_INT_WORK2(a0) = ATR_H_POS(a0);		//???
		ATR_INT_WORK3(a0) = ATR_V_POS(a0);		//
		gemini(a0);		//?
		ATR_INT_WORK2(a0) = ATR_H_POS(a0) - ATR_INT_WORK2(a0);
		ATR_INT_WORK3(a0) = ATR_V_POS(a0) - ATR_INT_WORK3(a0);
		if (!--ATR_GROUP_FLG(a0))		//???
		{
			if (ATR_LONG_WORK(1, a0))		//???????
				ATR_STIMER(ATR_BODY_WORK(1, a0)) = 1;		//???
			ATR_VCT_NO(a0)++;
		}
		break;
	case 5:
		a1 = ATR_BODY_WORK(2, a0);		//???????
		ATR_VCT_NO(a1) = 4;
		//???
		play_se(251, ATR_H_POS(a0), ATR_V_POS(a0));
		ATR_VCT_NO(a0)++;
		ATR_STIMER(a0) = 30;
		break;
	case 6:
		if (ATR_STIMER(a0))
		{
			ATR_STIMER(a0)--;
			break;
		}
		ATR_ATTRIB(a0) = ACT_ATR_HIDE;		//??
		a1 = ATR_BODY_WORK(2, a0);		//???????
		ATR_ATTRIB(a1) = ACT_ATR_HIDE;		//??
		a1 = ATR_BODY_WORK(0, a0);		//????????
		if (ATR_NAME(a1) == NULL || ATR_VCT_NO(a1) >= VCT_NO_DIE || ATR_VCT_NO(a1) == 0)		//???????
		{
			if (!ATR_LONG_WORK(1, a0))		//??????
				ATR_STIMER(ATR_BODY_WORK(1, a0)) = 1;		//???
			DeathAction(a0);		//?
			p_missile[0] = NULL;	//
			a1 = ATR_BODY_WORK(2, a0);		//???????
			DeathAction(a1);		//?
		}
	}
	pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
}

/* ???? *******************************************************************/
void stone(ACTION *a0)
{
	ACTION *a1, *a2;
	int d0, d1;

	switch (ATR_VCT_NO(a0))
	{
	case 0:
		a1 = ATR_BODY_WORK(1, a0);		//????????
		if (ATR_VCT_NO(a1) == 1)
		{
			DeathAction(a0);		//?
			return;
		}
		ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1) + 1;
		a2 = ATR_BODY_WORK(0, a0);		//???????
		d0 = ATR_H_POS(a2);
		d1 = ATR_V_POS(a2);
		radar(a1, &d0, &d1);	//????
		ATR_STIMER(a0)++;
		if (ATR_CRS(a0) == 0)		//??
		{
			if (ATR_SPD(a0) == 0)		//?????
				ATR_CRS(a0) = 16;
			else
				ATR_SPD(a0) -= 1;
		}
		else
			ATR_SPD(a0) += 1;
		gemini(a0);
		ATR_H_POS(a0) += ATR_INT_WORK2(a1);
		ATR_V_POS(a0) += ATR_INT_WORK3(a1);
		break;
	case 1:
		break;
	case 2:
		ATR_VCT_NO(a0)++;
		break;
	case 3:
		a1 = ATR_BODY_WORK(1, a0);		//????????
		if (ATR_VCT_NO(a1) == 1)
		{
			DeathAction(a0);		//?
			return;
		}
		ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1) + 1;
		ATR_LONG_WORK(0, a0)++;
		if (ATR_LONG_WORK(0, a0) & 1)
			ATR_SPD(a0) += 2;
		gemini(a0);
		ATR_H_POS(a0) += ATR_INT_WORK2(a1);
		ATR_V_POS(a0) += ATR_INT_WORK3(a1);
		break;
	case 4:
		if (ATR_LONG_WORK(1, a0))
		{
			//????
			if (!--ATR_LONG_WORK(1, a0))
				ATR_ATTRIB(a0) = ACT_ATR_HIDE;		//??
		}
		a1 = ATR_BODY_WORK(1, a0);		//????????
		ATR_H_POS(a0) = ATR_H_POS(a1);
		ATR_V_POS(a0) = ATR_V_POS(a1);
		break;
	}
	pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
}

/* ????? *******************************************************************/
void stone_shadow(ACTION *a0)
{
	ACTION *a1, *a2, *a3;
	int d0, d1;

	switch (ATR_VCT_NO(a0))
	{
	case 0:
		a1 = ATR_BODY_WORK(0, a0);		//????????
		d0 = ATR_H_POS(a1);
		d1 = ATR_V_POS(a1);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		//???????
		if (!--ATR_GROUP_FLG(a0))
		{
			a2 = ATR_BODY_WORK(0, a0);		//????????
			if (ATR_LIFE(a2) <= 0)		//??????
			{
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
				ATR_VCT_NO(a0) = 1;		//????????
				ATR_CHR_NO(a0) = ATR_LONG_WORK(0, a0);		//??????
				pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
				break;
			}
#ifdef _PETSKILL_ACUPUNCTURE
			/////////////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_TRAP | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
				| ATT_ATTACKBACK
#endif
				))
#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
				| ATT_ATTACKBACK
#endif
				))
#endif
				/////////////////////////////
#else
			/////////////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_TRAP))
#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA))
#endif
				/////////////////////////////
#endif
				set_guard_mark(a0);		//㘎??
			//?????????
			ATR_COMBO(a1) = 0;
			//???????
			if (ATR_ATTACK_KIND(0, a0) & ATT_DODGE)
			{
				ATR_HIT_STOP(a0) = 32;		//??????????
				ATR_VCT_NO(a0) = 1;		//????????
				ATR_CHR_NO(a0) = ATR_LONG_WORK(0, a0);		//??????
				pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
				gemini(a0);
				ATR_VCT_NO(a1) = 16;		//????
				ATR_CRS(a1) = ATR_CRS(a0);		//?????????????
				ATR_DAMAGE_ANG(a1) = ATR_CHR_ANG(a0);		//
				ATR_STIMER(a1) = 0;
				ATR_FIRST_FLG(a1) = 0;		//??????????
				break;
			}
			//????????????
			if (!((ATR_ATTACK_KIND(0, a0) & ATT_BALLIA) | (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION)))
			{
				if ((ATR_ATTACK_POW(0, a0) == 0) && (ATR_ATTACK_PET_POW(0, a0) == 0))		//????
				{
					a2 = ATR_BODY_WORK(0, a0);		//????????
					//??
					set_damage_num(a2, 0, -64);
					ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
					ATR_VCT_NO(a0) = 1;		//????????
					ATR_CHR_NO(a0) = ATR_LONG_WORK(0, a0);		//??????
					pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
					break;
				}
			}
			//?????????
			if (ATR_ATTACK_KIND(0, a0) & ATT_CRUSH)
				set_damage_num(a1, 19, -112);//?????
			//?????????
			ATR_AKO_FLG(a1) = 0;
			if (ATR_ATTACK_KIND(0, a0) & ATT_AKO1)		//??????
				ATR_AKO_FLG(a1) = 1;
			if (ATR_ATTACK_KIND(0, a0) & ATT_AKO2)		//??????
				ATR_AKO_FLG(a1) = 2;
			if (ATR_ATTACK_KIND(0, a0) & ATT_DEATH)		//?????
				ATR_LIFE(a1) = 0;		//?????
			ATR_DAMAGE_ANG(a1) = ATR_CHR_ANG(a0);		//
			if (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION)		//??????
			{
				if (!ATR_COMBO(a1))		//????????
				{
					//??
					ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
					ATR_LIFE(a1) += ATR_DAMAGE(a1);
					if (ATR_LIFE(a1) > ATR_MAX_LIFE(a1))		//????
						ATR_LIFE(a1) = ATR_MAX_LIFE(a1);		//????
					ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
					ATR_PET_LIFE(a1) += ATR_PET_DAMAGE(a1);
					if (ATR_PET_LIFE(a1) > ATR_PET_MAX_LIFE(a1))		//????
						ATR_PET_LIFE(a1) = ATR_PET_MAX_LIFE(a1);		//????
					set_damage_num(a1, 14, -64);
					ATR_ATTACK_POW(0, a0) = 0;		//????䥺?
					ATR_ATTACK_PET_POW(0, a0) = 0;		//pet????䥺?
				}
			}
			else if (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA)		//????????
			{
				ATR_ATTACK_POW(0, a0) = 0;		//????䥺?
				ATR_ATTACK_PET_POW(0, a0) = 0;		//????䥺?
			}
			else
				ATR_VCT_NO(a1) = 10;		//?????
			//??????
			if (ATR_ATTACK_KIND(0, a0) & ATT_SATISFACTORY)
				ATR_KAISHIN(a1) = 1;		//???????
			else
				ATR_KAISHIN(a1) = 0;		//?????
			ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
			ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
			//?????
			if (ATR_ATTACK_KIND(0, a0) & ATT_GUARD)
				ATR_GUARD_FLG(a1) = 1;
			else
				ATR_GUARD_FLG(a1) = 0;
			if (ATR_LIFE(a1) - ATR_DAMAGE(a1) <= 0)		//?????
			{
				ATR_LONG_WORK(1, a0) = 1;		//???????
				//????????
				if (ATR_COMBO(a1) == 0)
					ATR_KAISHIN(a1) = 1;		//???????
			}
			if (ATR_KAISHIN(a1))		//??????
				ATR_HIT_STOP(a0) = HIT_STOP_TIM * 4;		//??????????
			else
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
			//?????????????
			if (ATR_DAMAGE(a1) || (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA) || (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION))
			{
				if (ATR_ATTACK_KIND(0, a0) & ATT_COUNTER)		//???????
				{
					//????????
					if (ATR_COMBO(a1) == 0)
						set_damage_num(a0, 1, -64 + 16);		//?????
				}
				//???
				play_se(252, ATR_H_POS(a0), ATR_V_POS(a0));
				//?????????
				set_hit_mark(a0);
			}
			ATR_VCT_NO(a0) = 3;				//????????
			a1 = ATR_BODY_WORK(2, a0);		//??????
			ATR_VCT_NO(a1) = 1;				//????????
		}
		else
		{
			ATR_INT_WORK2(a0) = ATR_H_POS(a0);		//???
			ATR_INT_WORK3(a0) = ATR_V_POS(a0);		//
			gemini(a0);		//?
			ATR_INT_WORK2(a0) = ATR_H_POS(a0) - ATR_INT_WORK2(a0);
			ATR_INT_WORK3(a0) = ATR_V_POS(a0) - ATR_INT_WORK3(a0);
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 1:
		if (--ATR_HIT_STOP(a0))		//??????????
			break;
		//???????
		if (ATR_ATTACK_KIND(0, a0) & ATT_DODGE)
			ATR_STIMER(a0) = 32;
		else
			ATR_STIMER(a0) = 64;
		ATR_VCT_NO(a0) = 2;
		a0->atr = ACT_ATR_HIDE;
		p_missile[0] = NULL;
		break;
	case 2:
		if (--ATR_STIMER(a0))
			break;
		ATR_STIMER(ATR_BODY_WORK(1, a0)) = 1;		//???
		DeathAction(a0);		//?
		break;
	case 3:
		a1 = ATR_BODY_WORK(2, a0);		//??????
		ATR_CHR_NO(a1) = SPR_isiware;		//?????
		if (pattern(a1, ANM_NOMAL_SPD, ANM_NO_LOOP))	//??????
			goto stone_shadow_100;
		if (!pattern(a1, ANM_NOMAL_SPD, ANM_NO_LOOP))	//?????????
			break;
	stone_shadow_100:
		//???????????
		ATR_GROUP_FLG(a0) = 10;
		ATR_SPD(a0) = 6;
		ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
		d0 = ATR_CRS(a0);
		if (d0 >= 4 && d0 < BATTLKPKPLYAERNUM)
			ATR_CHR_NO(a1) = 25784;		//???
		else
			ATR_CHR_NO(a1) = 25783;		//???
		ATR_CRS(a0) -= 8;
		ATR_CRS(a0) &= 31;
		a1 = ATR_BODY_WORK(2, a0);		//??????
		ATR_VCT_NO(a1) = 2;				//???
		ATR_SPD(a1) = 0;
		ATR_CRS(a1) = 16;
		ATR_LONG_WORK(1, a1) = 30;
		ATR_VCT_NO(a0)++;
		//?????
		a3 = GetAction(T_PRIO_BOW, sizeof(ATR_EQU));
		//?????
		a2 = GetAction(T_PRIO_BOW - 1, sizeof(ATR_EQU));
		ATR_NAME(a2) = ATR_NAME(a0);
		ATR_VCT_NO(a2) = ATR_VCT_NO(a0);
		/* ? */
		ATR_DISP_PRIO(a2) = ATR_DISP_PRIO(a0);
		/* ?????? */
		ATR_CHR_NO(a2) = ATR_CHR_NO(a0);
		/* ??? */
		ATR_CHR_ANG(a2) = ATR_CHR_ANG(a0);
		ATR_BODY_WORK(0, a2) = ATR_BODY_WORK(0, a0);		//????
		ATR_BODY_WORK(1, a2) = ATR_BODY_WORK(1, a0);		//?????
		ATR_BODY_WORK(2, a2) = a3;		//??????
		ATR_ATTACK_POW(0, a2) = ATR_ATTACK_POW(0, a0);		//???
		ATR_ATTACK_PET_POW(0, a2) = ATR_ATTACK_PET_POW(0, a0);		//???
		ATR_ATTACK_KIND(0, a2) = ATR_ATTACK_KIND(0, a0);		//?????
		ATR_H_POS(a2) = ATR_H_POS(a0);
		ATR_V_POS(a2) = ATR_V_POS(a0);
		ATR_CRS(a2) = (ATR_CRS(a0) + 15) & 31;
		ATR_SPD(a2) = ATR_SPD(a0);
		ATR_BODY_CNT(a2) = ATR_BODY_CNT(a0);
		ATR_GROUP_FLG(a2) = ATR_GROUP_FLG(a0);
		pattern(a2, ANM_NOMAL_SPD, ANM_LOOP);
		p_missile[1] = a1;
		p_missile[2] = NULL;
		/* ?????? */
		if (d0 >= 4 && d0 < BATTLKPKPLYAERNUM)
			ATR_CHR_NO(a3) = 25783;		//???
		else
			ATR_CHR_NO(a3) = 25784;		//???
		ATR_NAME(a3) = ATR_NAME(a1);
		ATR_VCT_NO(a3) = ATR_VCT_NO(a1);
		ATR_BODY_WORK(1, a3) = a2;		//????????
		ATR_VCT_NO(a3) = ATR_VCT_NO(a1);
		ATR_DISP_PRIO(a3) = ATR_DISP_PRIO(a1);
		ATR_SPD(a3) = ATR_SPD(a1);
		ATR_CRS(a3) = ATR_CRS(a1);
		ATR_H_POS(a3) = ATR_H_POS(a1);
		ATR_V_POS(a3) = ATR_V_POS(a1);
		ATR_LONG_WORK(1, a3) = 30;
		break;
	case 4:
		ATR_INT_WORK2(a0) = ATR_H_POS(a0);		//???
		ATR_INT_WORK3(a0) = ATR_V_POS(a0);		//
		gemini(a0);		//?
		ATR_INT_WORK2(a0) = ATR_H_POS(a0) - ATR_INT_WORK2(a0);
		ATR_INT_WORK3(a0) = ATR_V_POS(a0) - ATR_INT_WORK3(a0);
		if (!--ATR_GROUP_FLG(a0))		//???
		{
			//????
			ATR_LONG_WORK(2, a0) = 60;
			ATR_LONG_WORK(3, a0) = 1;
			ATR_VCT_NO(a0) = 7;
		}
		break;
	case 5:
		ATR_ATTRIB(a0) = ACT_ATR_HIDE;		//??
		a1 = ATR_BODY_WORK(2, a0);		//??????
		ATR_VCT_NO(a1) = 4;
		//???
		ATR_VCT_NO(a0)++;
		break;
	case 6:
		a1 = ATR_BODY_WORK(0, a0);		//????????
		if (ATR_NAME(a1) == NULL || ATR_VCT_NO(a1) >= VCT_NO_DIE || ATR_VCT_NO(a1) == 0)		//???????
		{
			if (!ATR_LONG_WORK(1, a0))		//??????
				ATR_STIMER(ATR_BODY_WORK(1, a0)) = 1;		//???
			DeathAction(a0);		//?
			p_missile[0] = NULL;	//
			p_missile[1] = NULL;	//
			a1 = ATR_BODY_WORK(2, a0);		//??????
			DeathAction(a1);		//?
		}
	case 7:
		if (ATR_LONG_WORK(3, a0))
		{
			ATR_LONG_WORK(3, a0) = 0;
			//???
			play_se(251, ATR_H_POS(a0), ATR_V_POS(a0));
			ATR_ATTRIB(a0) = ACT_ATR_HIDE;		//??
			a1 = ATR_BODY_WORK(2, a0);		//??????
			ATR_VCT_NO(a1) = 4;
		}
		if (!--ATR_LONG_WORK(2, a0))		//???
		{
			if (ATR_LONG_WORK(1, a0))		//???????
				ATR_STIMER(ATR_BODY_WORK(1, a0)) = 1;		//???
			ATR_VCT_NO(a0) = 5;
		}
	}
}

//Terry add 2001/12/31
/* 鞭炮影子处理 *******************************************************************/
#ifdef _ITEM_FIRECREAKER
void firecracker_shadow(ACTION *a0)
{
	ACTION *a1;
	int d0, d1;

	switch (ATR_VCT_NO(a0))
	{
	case 0:
		a1 = ATR_BODY_WORK(0, a0);		//敌人的位址
		d0 = ATR_H_POS(a1);
		d1 = ATR_V_POS(a1);
		radar(a0, &d0, &d1);	//雷达(计算路线)
		ATR_CRS(a0) = d0;		//动画路线
		//准备命中
		if (!--ATR_GROUP_FLG(a0))
		{
			ATR_VCT_NO(a0) = 1;				//????????
			a1 = ATR_BODY_WORK(2, a0);		//??????
			ATR_VCT_NO(a1) = 1;				//????????
			ATR_STIMER(ATR_BODY_WORK(1, a0)) = 0;		//???
			ATR_ATTRIB(a0) = ACT_ATR_HIDE;	//影子结束
			ATR_CHR_NO(a1) = 101121;		//鞭炮炸开
			ATR_H_POS(a1) += 50;
		}
		else
		{
			ATR_INT_WORK2(a0) = ATR_H_POS(a0);		//???
			ATR_INT_WORK3(a0) = ATR_V_POS(a0);		//
			gemini(a0);		//?
			ATR_INT_WORK2(a0) = ATR_H_POS(a0) - ATR_INT_WORK2(a0);
			ATR_INT_WORK3(a0) = ATR_V_POS(a0) - ATR_INT_WORK3(a0);
			ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
			pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		}
		break;
	case 1:
		a1 = ATR_BODY_WORK(2, a0);		//鞭炮位址
		if (!pattern(a1, 36, ANM_NO_LOOP))	//动画未播完
			break;
		a1 = ATR_BODY_WORK(0, a0);		//????????
		if (ATR_NAME(a1) == NULL || ATR_VCT_NO(a1) >= VCT_NO_DIE || ATR_VCT_NO(a1) == 0)
		{		//???????
			if (!ATR_LONG_WORK(1, a0))			//??????
				ATR_STIMER(ATR_BODY_WORK(1, a0)) = 1;		//???
			DeathAction(a0);		//?
			p_missile[0] = NULL;	//
			p_missile[1] = NULL;	//
			a1 = ATR_BODY_WORK(2, a0);		//??????
			DeathAction(a1);		//?
		}
		break;
	}
}
#endif
//Terry end

/* ?????? *******************************************************************/
void missile(ACTION *a0)
{
	ACTION *a1, *a2;
	int d0, d1;

	switch (ATR_VCT_NO(a0))
	{
	case 0:
		a1 = ATR_BODY_WORK(0, a0);		//????????
		d0 = ATR_H_POS(a1);
		d1 = ATR_V_POS(a1);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		//???????
		if (d1 <= 32)
		{
			a2 = ATR_BODY_WORK(0, a0);		//????????
			if (ATR_LIFE(a2) <= 0)		//??????
			{
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
				ATR_VCT_NO(a0) = 1;		//????????
				break;
			}
			//????
			if ((ATR_ATTACK_POW(0, a0) == 0) && (ATR_ATTACK_PET_POW(0, a0) == 0))
			{
				a2 = ATR_BODY_WORK(0, a0);		//????????
				//??
				set_damage_num(a2, 0, -64);
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
				ATR_VCT_NO(a0) = 1;		//????????
				break;
			}
			ATR_CRS(a1) = d0;		//?????????????
			ATR_DAMAGE_ANG(a1) = ATR_CHR_ANG(a0);		//
			if (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION)		//??????
			{
				if (!ATR_COMBO(a1))		//????????
				{
					//??
					ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
					ATR_LIFE(a1) += ATR_DAMAGE(a1);
					if (ATR_LIFE(a1) > ATR_MAX_LIFE(a1))		//????
						ATR_LIFE(a1) = ATR_MAX_LIFE(a1);		//????
					ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
					ATR_PET_LIFE(a1) += ATR_PET_DAMAGE(a1);
					if (ATR_PET_LIFE(a1) > ATR_PET_MAX_LIFE(a1))		//????
						ATR_PET_LIFE(a1) = ATR_PET_MAX_LIFE(a1);		//????
					set_damage_num(a1, 14, -64);
					ATR_ATTACK_POW(0, a0) = 0;		//????䥺?
					ATR_ATTACK_PET_POW(0, a0) = 0;		//pet????䥺?
				}
			}
			else if (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA)		//????????
			{
				ATR_ATTACK_POW(0, a0) = 0;		//????䥺?
				ATR_ATTACK_PET_POW(0, a0) = 0;		//????䥺?
			}
			else
				ATR_VCT_NO(a1) = 10;		//?????
			ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);		//
			ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);		//
			//?????
			if (ATR_ATTACK_KIND(0, a0) & ATT_GUARD)
				ATR_GUARD_FLG(a1) = 1;
			else
				ATR_GUARD_FLG(a1) = 0;
			if (ATR_LIFE(a1) - ATR_DAMAGE(a1) <= 0)		//?????
			{
				ATR_KAISHIN(a1) = 1;		//???????
				ATR_HIT_STOP(a0) = HIT_STOP_TIM * 4;		//??????????
			}
			else
			{
				ATR_KAISHIN(a1) = 0;		//?????
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
			}
			ATR_VCT_NO(a0) = 1;		//????????
			//???
			play_se(103, 320, 240);
			//??????
			a1 = GetAction(T_PRIO_MAGIC_EFFECT, sizeof(ATR_EQU));
			if (a1 == NULL)
				return;
			/* ??? */
			ATR_NAME(a1) = magic_effect;
			/* ? */
			ATR_DISP_PRIO(a1) = D_PRIO_MAGIC_EFFECT;
			ATR_CHR_NO(a1) = 36009;
			/* ??? */
			ATR_BODY_WORK(0, a1) = ATR_BODY_WORK(0, a0);
		}
		else
			gemini(a0);		//?
		ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 1:
		if (--ATR_HIT_STOP(a0))		//??????????
			break;
		ATR_STIMER(a0) = 64;
		ATR_VCT_NO(a0) = 2;
		a0->atr = ACT_ATR_HIDE;
		p_missile[ATR_BODY_CNT(a0)] = NULL;
		break;
	case 2:
		if (--ATR_STIMER(a0))
			break;
		ATR_DAMAGE(p_master)++;		//???
		DeathAction(a0);		//?
		break;
	}
}

/* ???? *******************************************************************/
void amelioration(ACTION *a0)
{
	ACTION *a1;
	int d0, d1;

	switch (ATR_VCT_NO(a0))
	{
	case 0:
		if (ATR_INT_WORK0(a0) == 0)		//????
		{
			d0 = 310;
			d1 = 222;
			radar(a0, &d0, &d1);	//????
			ATR_CRS(a0) = d0;		//??????
			gemini(a0);		//?
			if (d1 <= (ATR_SPD(a0) >> 2))		//????
				ATR_INT_WORK0(a0) = 1;		//?????
		}
		else
		{
			a1 = ATR_BODY_WORK(0, a0);		//????????
			d0 = ATR_H_POS(a1);
			d1 = ATR_V_POS(a1);
			radar2(a0, d0, d1, 1);	//????
			if (ATR_INT_WORK1(a0) == 0)		//????????
				ATR_CHR_ANG(a0) = crs_change_tbl[ATR_CRS(a0)];		/* ????? */
			d0 = ATR_H_POS(a1);
			d1 = ATR_V_POS(a1);
			radar(a0, &d0, &d1);	//????
			if (d0 == ATR_CRS(a0))		//??????
				ATR_INT_WORK1(a0) = 1;		//????????
			//???????
			if (d1 <= 32)
			{
				a1 = ATR_BODY_WORK(0, a0);		//????????
				if (ATR_LIFE(a1) <= 0)		//??????
				{
					ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
					ATR_VCT_NO(a0) = 1;		//????????
					break;
				}
				ATR_COUNTER(a1) = NULL;		//????????
				ATR_VCT_NO(a1) = 15;		//???
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
				ATR_KAISHIN(a1) = 0;		//???????
				ATR_VCT_NO(a0) = 1;		//????????
				//???
				play_se(100, 320, 240);
				//??????
				a1 = GetAction(T_PRIO_MAGIC_EFFECT, sizeof(ATR_EQU));
				if (a1 == NULL)
					return;
				/* ??? */
				ATR_NAME(a1) = magic_effect;
				/* ? */
				ATR_DISP_PRIO(a1) = D_PRIO_MAGIC_EFFECT;
				/* ?????? */
				ATR_CHR_NO(a1) = 36009;
				/* ??? */
				ATR_BODY_WORK(0, a1) = ATR_BODY_WORK(0, a0);
			}
			else
				gemini(a0);		//?
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 1:
		if (--ATR_HIT_STOP(a0))		//??????????
			break;
		ATR_STIMER(a0) = 64;
		ATR_VCT_NO(a0) = 2;
		a0->atr = ACT_ATR_HIDE;
		p_missile[ATR_BODY_CNT(a0)] = NULL;
		break;
	case 2:
		if (--ATR_STIMER(a0))
			break;
		ATR_DAMAGE(p_master)++;		//???
		DeathAction(a0);		//?
		break;
	}
}

// ?????? //////////////////////////////////////////////////////////////
typedef int CMPFUNC(const void*, const void*);
int sort_chr(SORT_CHR_EQU *a0, SORT_CHR_EQU *a1)
{
	if (a0->v_pos > a1->v_pos)
		return 1;
	else
		return -1;
}

extern int piyo_tbl[];
extern int piyo_point;
//static int command_point;

/*-------------------------------------------
	??????????????
	--------------------------------------------*/
void set_raster_pos(ACTION *a1)
{
	int d0;

#ifdef __ATTACK_MAGIC
	int value = 0;

	// 如果正在地震中或特殊的地图编号时
	if ((g_iRunEarthQuake < 2) && (BattleMapNo < 148 || BattleMapNo > 150))
		return;
	if (2 == g_iRunEarthQuake)
		value = g_iCurRunEarthQuake;
	else if (3 == g_iRunEarthQuake)
	{
		ATR_INT_WORK3(a1) = 0;
		return;
	}
	d0 = ATR_V_POS(a1) >> 3;
	d0 += piyo_point + value;
	d0 &= 63;
	ATR_V_POS(a1) -= piyo_tbl[d0];
	ATR_INT_WORK1(a1) -= piyo_tbl[d0];
	ATR_INT_WORK3(a1) = piyo_tbl[d0];
#else
	//??????????????
	if (BattleMapNo < 148 || BattleMapNo > 150)
		return;
	d0 = ATR_V_POS(a1) >> 3;
	d0 += piyo_point;
	d0 &= 63;
	ATR_V_POS(a1) -= piyo_tbl[d0];
	ATR_INT_WORK1(a1) -= piyo_tbl[d0];
	ATR_INT_WORK3(a1) = piyo_tbl[d0];
#endif
}

/*-------------------------------------------
	????????
	--------------------------------------------*/
static int get_num(void)
{
	int d0 = 0;
	int d1 = 0;

	while (1)
	{
		d0 = BattleCmd[command_point++];
		if (d0 == NULL)
			return -1;
		if (d0 >= '0' && d0 <= '9')		//???
			break;						//??????
		if (d0 >= 'A' && d0 <= 'F')		//?????
			break;						//??????
	}
	if (d0 >= 'A')		//?????
		d1 = d0 - 'A' + 10;
	else
		d1 = d0 - '0';
	while (1)
	{
		d0 = BattleCmd[command_point++];		//???????
		if (d0 == NULL)		//???
		{
			command_point--;
			break;
		}
		if (d0 < '0' || d0 > '9')		//??????
		{
			if (d0 < 'A' || d0 > 'F')		//????????
				break;						//?
		}
		d1 = d1 << 4;		//???????
		if (d0 >= 'A')		//?????
			d1 += d0 - 'A' + 10;
		else
			d1 += d0 - '0';
	}
	return d1;
}
//andy_add
static int get_mpnum(char code)
{
	int d0, d1, num = 0;

	while (1)
	{
		d0 = BattleCmd[num++];
		if (d0 == NULL)
			return -1;
		if (d0 == (int)code)
			break;
	}

	while (1)
	{
		d0 = BattleCmd[num++];		//????
		if (d0 == NULL)		//???
			return -1;
		if (d0 >= '0' && d0 <= '9')		//???
			break;						//??????
		if (d0 >= 'A' && d0 <= 'F')		//?????
			break;						//??????
	}
	if (d0 >= 'A')		//?????
		d1 = d0 - 'A' + 10;
	else
		d1 = d0 - '0';

	while (1)
	{
		d0 = BattleCmd[num++];		//???????
		if (d0 == NULL)		//???
		{
			command_point--;
			break;
		}
		if (d0 < '0' || d0 > '9')		//??????
		{
			if (d0 < 'A' || d0 > 'F')		//????????
				break;						//?
		}
		d1 = d1 << 4;		//???????
		if (d0 >= 'A')		//?????
			d1 += d0 - 'A' + 10;
		else
			d1 += d0 - '0';
	}
	return d1;
}
/*-------------------------------------------
	?????????
	--------------------------------------------*/
static char get_command(void)
{
	int now_point = 0;
	char d0;

	while (1)
	{
		now_point = command_point;
		d0 = BattleCmd[command_point++];
		if (d0 == NULL)
			return -1;
		if (d0 == 'B')
		{
			//andy_reEdit 2003/07/18 21:54
			if (BattleCmd[now_point + 2] == '|' && (BattleCmd[now_point - 1] == '|' || BattleCmd[now_point - 1] == NULL))
				break;
		}
	}
	d0 = BattleCmd[command_point++];
	return d0;
}

static char get_char()
{
	char d0;

	d0 = BattleCmd[command_point++];		//???
	if (d0 == NULL)		//??????
		return -1;		//?
	return d0;
}

/* 䦶??? *******************************************************************/
void get_name(ACTION *a1)
{
	char d0;
	char *a2;

	a2 = ATR_HANDLE(a1);
	while (1)
	{
		d0 = BattleCmd[command_point++];		//????
		if (d0 == NULL)		//???
			break;
		//????
		if (d0 == '|')
			break;
		if (IsDBCSLeadByte(d0))
		{
			*a2++ = d0;
			*a2++ = BattleCmd[command_point++];
		}
		else
			*a2++ = d0;
	}
	//????
	*a2 = 0;
	makeStringFromEscaped(ATR_HANDLE(a1));
}

/*-------------------------------------------
	??????????
	--------------------------------------------*/
static int get_next_flg(void)
{
	int d0, d1;

	d1 = command_point;		//?????????
	d0 = get_num();		//??
	if (d0 == -1)		//???
		goto get_next_flg_500;
	if (d0 == 255)		//???
		goto get_next_flg_500;
	d0 = get_num();		//?????
	goto get_next_flg_700;
get_next_flg_500:
	d0 = 0;
get_next_flg_700:
	command_point = d1;		//???????????

	return d0;
}

/*-------------------------------------------
	???????
	--------------------------------------------*/
static int get_body_cnt(void)
{
	int d0, d1, d2 = 0;

	d1 = command_point;		//?????????
	while (1)
	{
		d0 = get_num();		//??
		if (d0 == -1)		//???
			break;
		if (d0 == 255)		//???
			break;
		get_num();		//???????
		get_num();		//????????
		if (BattleCmd[command_point] == 'p')
			get_num();		//pet????????
		d2++;		//??
	}
	command_point = d1;		//???????????

	return d2;
}

/*-------------------------------------------
	???????
	--------------------------------------------*/
static ACTION *get_body_target(void)
{
	int d0, d1;

	d1 = command_point;		//?????????
	d0 = get_num();		//??
	command_point = d1;		//???????????
	if (d0 == -1)
		return 0;
	else
		return p_party[d0];
}

//全灭确认处理
int check_all_dead(void)
{
	int d0, d7;

	d0 = 10;		//检查人数设定
	for (d7 = 0; d7 < 5; d7++)
	{
		if (ATR_NAME(p_party[d7]) != NULL)		//????????????
		{
			if (ATR_PET_OK(p_party[d7]))
				d0 = 5;		//检查人数设定
			break;
		}
	}
	for (d7 = 0; d7 < d0; d7++)
	{
		if (ATR_NAME(p_party[d7]) != NULL)
		{
			if (ATR_LIFE(p_party[d7]) > 0)		//假如有一个人是活着的
				break;
#ifdef _PETSKILL_LER
			// 雷尔第一段及第二段死亡时不能算全灭,因为会变身
			else if (ATR_CHR_NO(p_party[d7]) == 101813 || ATR_CHR_NO(p_party[d7]) == 101814)
				break;
#endif
#ifdef __NEW_BATTLE_EFFECT
			if (1 == ATR_EFFECT_FLAG(p_party[d7]))
				break;
#endif
#ifdef _HUNDRED_KILL
			//代表百人npc还没真正死掉
			if( BattleHundredFlag == TRUE ) 
				break;
#endif
		}
	}
	if (d7 == d0)		//全灭
		return 1;
	d0 = 20;		//检查人数设定

	for (d7 = 10; d7 < 15; d7++)
	{
		if (ATR_NAME(p_party[d7]) != NULL)		//????????????
		{
			if (ATR_PET_OK(p_party[d7]))
				d0 = 15;		//检查人数设定
			break;
		}
	}
	for (d7 = 10; d7 < d0; d7++)
	{
		if (ATR_NAME(p_party[d7]) != NULL)
		{
			if (ATR_LIFE(p_party[d7]) > 0)		//假如有一个人是活着的
				break;
#ifdef _PETSKILL_LER
			// 雷尔第一段及第二段死亡时不能算全灭,因为会变身
			else if (ATR_CHR_NO(p_party[d7]) == 101813 || ATR_CHR_NO(p_party[d7]) == 101814)
				break;
#endif
#ifdef __NEW_BATTLE_EFFECT
			if (1 == ATR_EFFECT_FLAG(p_party[d7]))
				break;
#endif
#ifdef _HUNDRED_KILL
			//代表百人npc还没真正死掉
			if( BattleHundredFlag == TRUE ) 
				break;
#endif
		}
	}
	if (d7 == d0)		//全灭 return 1
		return 1;
	return 0;		//还有人活着 return 0 
}

//属性表示处理
void disp_attrib(ACTION *a0)
{
	ACTION *a1;

	a1 = p_attrib;
	switch (ATR_ATTRIB(a0))
	{
		//无属性
	case 0:
		ATR_ATTRIB(a1) = ACT_ATR_HIDE;		//不表示
		break;
		//地属性
	case 1:
		ATR_ATTRIB(a1) &= ~ACT_ATR_HIDE;
		ATR_CHR_NO(a1) = CG_ATR_ICON_EARTH_BATTLE;
		break;
		//水属性
	case 2:
		ATR_ATTRIB(a1) &= ~ACT_ATR_HIDE;
		ATR_CHR_NO(a1) = CG_ATR_ICON_WATER_BATTLE;
		break;
		//火属性
	case 3:
		ATR_ATTRIB(a1) &= ~ACT_ATR_HIDE;
		ATR_CHR_NO(a1) = CG_ATR_ICON_FIRE_BATTLE;
		break;
		//风属性
	case 4:
		ATR_ATTRIB(a1) &= ~ACT_ATR_HIDE;
		ATR_CHR_NO(a1) = CG_ATR_ICON_WIND_BATTLE;
		break;
	}
	pattern(a1, ANM_NOMAL_SPD, ANM_NO_LOOP);
}

/* ???????? *******************************************************************/
void get_command_asc(void)
{
	char d0, d1 = 0;
	char *a2;

	kanji_buf[0][0] = kanji_buf[1][0] = kanji_buf[2][0] = kanji_buf[3][0] = 0;
	a2 = kanji_buf[d1];
	while (1)
	{
		d0 = BattleCmd[command_point++];		//????
		if (d0 == NULL)		//???
		{
			command_point--;
			break;
		}
		//????
		if (d0 == '\t')
		{
			//????
			*a2 = 0;
			d1++;
			if (d1 == 4)		//?????
				break;
			a2 = kanji_buf[d1];
			continue;
		}
		//????
		if (d0 == '|')
			break;
		if (IsDBCSLeadByte(d0))
		{
			*a2++ = d0;
			*a2++ = BattleCmd[command_point++];
		}
		else
			*a2++ = d0;
	}
	//????
	*a2 = 0;
}

#ifdef __TOCALL_MAGIC
// 召唤咒术的资料建立
BOOL BuildToCallMagicData(ACTION *pMaster, ACTION *pAttacker)
{
	int	i, idx = 0, midx, midy, count = 0;
	int	charidx[MAX_NUM_ATTACKED];

	memset(&ToCallMgc, 0, sizeof(ToCallMgc));
	memset(charidx, 0, sizeof(charidx));

	iAttackedNum = 0;
	iCurAttackedFinishNum = 0;

	// B$|Attacker's No( 0 - 19 )|5711438|Attacker's MP|

	ATR_VCT_NO(pAttacker) = TOCALL_MAGIC_CASE;
	ATR_MP(pAttacker) = get_num();
	//	ToCallMgc.iPreMgcNum			= get_num();
	//	ToCallMgc.iCurMgcNum			= get_num();
	ToCallMgc.iPreMgcNum = get_num();
#ifdef _PETSKILL_LER			// 雷尔技能
	if (ToCallMgc.iPreMgcNum == 101808 || ToCallMgc.iPreMgcNum == 101809)
	{
		if (!g_bUseAlpha)
			ToCallMgc.iPreMgcNum += 53;
	}
#endif
	ToCallMgc.iCurMgcNum = get_num();
#ifdef _PROFESSION_ADDSKILL
	if (ToCallMgc.iCurMgcNum >= 101770 && ToCallMgc.iCurMgcNum <= 101797)
	{
		if (!g_bUseAlpha)
			ToCallMgc.iCurMgcNum += 55;
	}
#endif
#ifdef _PETSKILL_LER			// 雷尔技能
	if (ToCallMgc.iCurMgcNum == 101798)
	{
		if (!g_bUseAlpha)
			ToCallMgc.iCurMgcNum = 101853;
	}
	if (ToCallMgc.iCurMgcNum == 101800)
	{
		if (!g_bUseAlpha)
			ToCallMgc.iCurMgcNum = 101854;
	}
	if (ToCallMgc.iCurMgcNum == 101806 || ToCallMgc.iCurMgcNum == 101807)
	{
		if (!g_bUseAlpha)
			ToCallMgc.iCurMgcNum += 53;
	}
#endif
	ToCallMgc.iPostMgcNum = get_num();
	ToCallMgc.wAttackType = (WORD)get_num();
	ToCallMgc.wAttackTimeSlice = (WORD)get_num();
	ToCallMgc.wShowType = (WORD)get_num();
	//	ToCallMgc.wShowType =0;
	ToCallMgc.wScreenX = (WORD)get_num();
	//	ToCallMgc.wScreenX				= 0;//-150;
	ToCallMgc.wScreenY = (WORD)get_num();
	//	ToCallMgc.wScreenY				= 0;//-20;
	ToCallMgc.wPreMgcX = (WORD)get_num();
	ToCallMgc.wPreMgcY = (WORD)get_num();
	ToCallMgc.wPostMgcX = (WORD)get_num();
	ToCallMgc.wPostMgcY = (WORD)get_num();
	ToCallMgc.wPreMgcOnChar = (WORD)get_num();
	ToCallMgc.wCurMgcOnChar = (WORD)get_num();
	ToCallMgc.wPostMgcOnChar = (WORD)get_num();
	ToCallMgc.dwEQuake = (DWORD)get_num();
	//	ToCallMgc.dwEQuake = 1;
	ToCallMgc.dwEQuakeSTime = (DWORD)get_num();
	//	ToCallMgc.dwEQuakeSTime			= 1000;
	ToCallMgc.dwEQuakeETime = (DWORD)get_num();
	//	ToCallMgc.dwEQuakeETime			= 4000;
	ToCallMgc.wIsPostDisappear = (WORD)get_num();
	ToCallMgc.wToCallMagicNo = (WORD)get_num();

	do
	{
		charidx[idx] = get_num();
	} while (0XFF != charidx[idx] && ++idx);

	if (0 == idx)
		return FALSE;

	while (0X5711438 != (AttackedInfo[iAttackedNum * 4] = get_num()))
	{
		AttackedInfo[iAttackedNum * 4 + 1] = get_num();
		AttackedInfo[iAttackedNum * 4 + 2] = get_num();
		AttackedInfo[iAttackedNum * 4 + 3] = get_num();
		iAttackedNum++;
	}
	ToCallMgc.dwCurFrame1 = 0;
	ToCallMgc.dwCurFrame2 = 0;
	ToCallMgc.dwEQuakeSFrame = ToCallMgc.dwEQuakeSTime >> 4;
	ToCallMgc.dwEQuakeEFrame = ToCallMgc.dwEQuakeETime >> 4;

	// 是否有前置咒术
	(0XFFFFFFFF == ToCallMgc.iPreMgcNum) ? ToCallMgc.wRunPreMgc = TRUE : ToCallMgc.wRunPreMgc = FALSE;

	// 绝对位置显示法
	if (1 == ToCallMgc.wShowType)
	{
		ToCallMgc.wNumAttacks = 1;
		ToCallMgc.wCurAttackNum = 0;
		ToCallMgc.posAttacked[0].x = ToCallMgc.wScreenX;
		ToCallMgc.posAttacked[0].y = ToCallMgc.wScreenY;
		ToCallMgc.wAttackedIndex[0] = 20;		// 全部人员皆同时呈受伤状态
		ToCallMgc.wMgcFrameCount[0] = 0;

		// 被攻击的敌人索引号
		for (i = 0; i < idx; i++)
			ToCallMgc.wAttackedIndex[i + 1] = charidx[i];

		ToCallMgc.wAttackedIndex[i + 1] = 0XFF;
	}
	// 居中位置显示法
	else
	{
		// 咒术的方式为全体同时攻击
		if (2 == ToCallMgc.wAttackType || 4 == ToCallMgc.wAttackType)
		{
			midx = 0;
			midy = 0;

			for (i = 0; i < idx; i++)
			{
				midx += p_party[charidx[i]]->x;
				midy += p_party[charidx[i]]->y;
			}
			midx = int(midx / idx);
			midy = int(midy / idx);
			ToCallMgc.wNumAttacks = 1;
			ToCallMgc.wCurAttackNum = 0;
			ToCallMgc.posAttacked[0].x = midx + ToCallMgc.wScreenX;
			ToCallMgc.posAttacked[0].y = midy + ToCallMgc.wScreenY;
			ToCallMgc.wAttackedIndex[0] = 20;		// 全部人员皆同时呈受伤状态
			ToCallMgc.wMgcFrameCount[0] = 0;

			// 被攻击的敌人索引号
			for (i = 0; i < idx; i++)
				ToCallMgc.wAttackedIndex[i + 1] = charidx[i];
			ToCallMgc.wAttackedIndex[i + 1] = 0XFF;
		}
		// 单独攻击
		else
		{
			// 被攻击的敌人索引号
			for (i = 0; i < idx; i++)
			{
				ToCallMgc.wNumAttacks++;
				ToCallMgc.posAttacked[i].x = ToCallMgc.wScreenX;
				ToCallMgc.posAttacked[i].y = ToCallMgc.wScreenY;
				ToCallMgc.wAttackedIndex[i] = charidx[i];
				ToCallMgc.wMgcFrameCount[i] = count;
				count += ToCallMgc.wAttackTimeSlice >> 4;
			}
		}
	}
	ATR_VCT_NO(pAttacker) = TOCALL_MAGIC_CASE;
	ATR_BODY_CNT(pMaster) = 1;
	bRunToCallMgc = TRUE;

	return TRUE;
}


// 动态产生新的魔法
BOOL RunTimeMagicToCall()
{
	// 开始地震
	if (ToCallMgc.dwEQuakeSFrame == ToCallMgc.dwCurFrame1)
	{
		g_iRunEarthQuake = 1;
		g_iNumRunEarthQuake = ToCallMgc.dwEQuakeEFrame - ToCallMgc.dwEQuakeSFrame;
	}

	// 结束地震
	if (ToCallMgc.dwEQuakeEFrame == ToCallMgc.dwCurFrame1)
		g_iRunEarthQuake = 3;

	ToCallMgc.dwCurFrame1++;
	// 前置魔法是否已经播完了
	if (FALSE == ToCallMgc.wRunPreMgc)
		return TRUE;

	if (ToCallMgc.wToCallMagicNo != 2)
	{
		// 是否仍有尚未播放的咒术
		if (ToCallMgc.wCurAttackNum < ToCallMgc.wNumAttacks)
		{
			while (1)
			{
				if (ToCallMgc.dwCurFrame2 == ToCallMgc.wMgcFrameCount[ToCallMgc.wCurAttackNum])
				{
					ACTION	*a0;

					a0 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
					ATR_NAME(a0) = monster;
					ATR_VCT_NO(a0) = TOCALL_MAGIC_CASE + 2;		// 咒术的执行
					ATR_DISP_PRIO(a0) = (1 == ToCallMgc.wCurMgcOnChar) ? D_PRIO_HIT_MARK : DISP_PRIO_TILE + 1;
					ATR_CHR_NO(a0) = ToCallMgc.iCurMgcNum;
					ATR_LONG_WORK(0, a0) = ToCallMgc.wAttackedIndex[ToCallMgc.wCurAttackNum];
					ATR_H_POS(a0) = (20 == ToCallMgc.wAttackedIndex[ToCallMgc.wCurAttackNum]) ? ToCallMgc.posAttacked[ToCallMgc.wCurAttackNum].x : ATR_H_POS(p_party[ToCallMgc.wAttackedIndex[ToCallMgc.wCurAttackNum]]) + ToCallMgc.posAttacked[ToCallMgc.wCurAttackNum].x;
					ATR_V_POS(a0) = (20 == ToCallMgc.wAttackedIndex[ToCallMgc.wCurAttackNum]) ? ToCallMgc.posAttacked[ToCallMgc.wCurAttackNum].y : ATR_V_POS(p_party[ToCallMgc.wAttackedIndex[ToCallMgc.wCurAttackNum]]) + ToCallMgc.posAttacked[ToCallMgc.wCurAttackNum].y;

					if (ToCallMgc.wAttackedIndex[ToCallMgc.wCurAttackNum] < 10)
						ATR_CHR_ANG(a0) = (7);
					else if (ToCallMgc.wAttackedIndex[ToCallMgc.wCurAttackNum] < 20)
						ATR_CHR_ANG(a0) = (3);
#ifdef _BATTLE_PK_PLAYER_FOR_40
					else if (ToCallMgc.wAttackedIndex[ToCallMgc.wCurAttackNum] < 30)
						ATR_CHR_ANG(a0) = (1);
					else if (ToCallMgc.wAttackedIndex[ToCallMgc.wCurAttackNum] < 40)
						ATR_CHR_ANG(a0) = (5);
#endif
					ToCallMgc.wCurAttackNum++;
				}
				if (ToCallMgc.wCurAttackNum >= ToCallMgc.wNumAttacks || ToCallMgc.wMgcFrameCount[ToCallMgc.wCurAttackNum] > ToCallMgc.dwCurFrame2)
					break;
			}
		}
	}
	else
	{
		// 是否仍有尚未播放的咒术
		if (ToCallMgc.wCurAttackNum < ToCallMgc.wNumAttacks)
		{
			while (1)
			{
				if (ToCallMgc.dwCurFrame2 == ToCallMgc.wMgcFrameCount[ToCallMgc.wCurAttackNum])
				{
					ACTION	*a0;

					a0 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
					ATR_NAME(a0) = monster;
					ATR_VCT_NO(a0) = TOCALL_MAGIC_CASE + 2;		// 咒术的执行
					ATR_DISP_PRIO(a0) = (1 == ToCallMgc.wCurMgcOnChar) ? D_PRIO_HIT_MARK : DISP_PRIO_TILE + 1;
					ATR_CHR_NO(a0) = ToCallMgc.iCurMgcNum;
#ifdef _PROFESSION_ADDSKILL
					if (g_bUseAlpha)
					{
						if (ToCallMgc.iCurMgcNum == 101770 || ToCallMgc.iCurMgcNum == 101771)//为了让破除结界图档显示于其它结界之上
							ATR_DISP_PRIO(a0)	= DISP_PRIO_TILE + ToCallMgc.wCurMgcOnChar;
					}
					else
					{
						if (ToCallMgc.iCurMgcNum == 101825 || ToCallMgc.iCurMgcNum == 101826)//为了让破除结界图档显示于其它结界之上
							ATR_DISP_PRIO(a0)	= DISP_PRIO_TILE + ToCallMgc.wCurMgcOnChar;
					}
#endif
					ATR_LONG_WORK(0, a0) = ToCallMgc.wAttackedIndex[ToCallMgc.wCurAttackNum];
					ATR_H_POS(a0) = (20 == ToCallMgc.wAttackedIndex[ToCallMgc.wCurAttackNum]) ? ToCallMgc.posAttacked[ToCallMgc.wCurAttackNum].x : ATR_H_POS(p_party[ToCallMgc.wAttackedIndex[ToCallMgc.wCurAttackNum]]) + ToCallMgc.posAttacked[ToCallMgc.wCurAttackNum].x;
					ATR_V_POS(a0) = (20 == ToCallMgc.wAttackedIndex[ToCallMgc.wCurAttackNum]) ? ToCallMgc.posAttacked[ToCallMgc.wCurAttackNum].y : ATR_V_POS(p_party[ToCallMgc.wAttackedIndex[ToCallMgc.wCurAttackNum]]) + ToCallMgc.posAttacked[ToCallMgc.wCurAttackNum].y;
					ToCallMgc.wCurAttackNum++;
				}
				if (ToCallMgc.wCurAttackNum >= ToCallMgc.wNumAttacks || ToCallMgc.wMgcFrameCount[ToCallMgc.wCurAttackNum] > ToCallMgc.dwCurFrame2)
					break;
			}
		}
	}
	ToCallMgc.dwCurFrame2++;

	return TRUE;
}
#endif

#ifdef _PROFESSION_ADDSKILL
BOOL BuildBoundaryMagicData(int state)
{
	int stateno[] = {101789,101777,101783,101795,101786,101774,101780,101792};
	int mark=-1;

	if (!g_bUseAlpha)
	{
		if (state > 1)
			state += 55;
		stateno[0] = 101844, stateno[1] = 101832, stateno[2] = 101838, stateno[3] = 101850,
			stateno[4] = 101841, stateno[5] = 101829, stateno[6] = 101835, stateno[7] = 101847;
	}
	if (state == 0)//左清除
	{
		if (boundary_mark[0])
		{
			DeathAction(boundary_mark[0]);
			boundary_mark[0] = NULL;
		}
		bRunBoundaryMgc_l = 0;    
	}
	if (state == 1)//右清除
	{
		if (boundary_mark[1])
		{
			DeathAction(boundary_mark[1]);
			boundary_mark[1] = NULL;
		}
		bRunBoundaryMgc_r = 0;
	}
	if (state == stateno[0] //左地
		|| state == stateno[1]
		|| state == stateno[2]
		|| state == stateno[3])
	{
		if (bRunBoundaryMgc_l != state)
		{
			if (boundary_mark[0])
			{
				DeathAction(boundary_mark[0]);
				boundary_mark[0] = NULL;
			}
			bRunBoundaryMgc_l = state;
			if (boundary_mark[0] == NULL)
			{
				boundary_mark[0] = MakeAnimDisp(320, 240, state + 2, 0);
				boundary_mark[0]->actNo = 0;
				boundary_mark[0]->anim_ang = 3;
				boundary_mark[0]->dispPrio = DISP_PRIO_TILE + 1;//图层
			}
		}
	}

	if (state == stateno[4] //右地
		|| state == stateno[5]
		|| state == stateno[6]
		|| state == stateno[7])
	{
		if (bRunBoundaryMgc_l != state)
		{
			if (boundary_mark[1])
			{
				DeathAction(boundary_mark[1]);
				boundary_mark[1] = NULL;
			}
			bRunBoundaryMgc_r = state;
			if (boundary_mark[1] == NULL)
			{
				boundary_mark[1] = MakeAnimDisp(320, 240, state + 2, 0);
				boundary_mark[1]->actNo = 0;
				boundary_mark[1]->anim_ang = 3;
				boundary_mark[1]->dispPrio = DISP_PRIO_TILE + 1;//图层
			}
		}
	}

	return TRUE;
}

void RunTimeMagicBoundary(int state)
{
	int stateno[] = { 101774, 101797};

	if (!g_bUseAlpha)
	{
		stateno[0] = 101829;
		stateno[1] = 101852;
	}
	if (boundary_2 == NULL && state >= stateno[0] && state <= stateno[1])
	{
		int mark = state + 1;

		boundary_2 = MakeAnimDisp(320, 240, mark, 0);
		boundary_2->actNo = 0;
		boundary_2->anim_ang = 3;
		boundary_2->dispPrio = DISP_PRIO_TILE + 1;//图层
	}

	/*if ( boundary_r->anim_cnt == 10 ){//拨放到最后一张
			DeathAction(boundary_r);
			boundary_r = NULL;
			bRunBoundaryMgc = FALSE;
			}*/
}
#endif

#ifdef __ATTACK_MAGIC
// 排序显示的前后位置
static int SortIdx(const void *pElement1, const void *pElement2)
{
	int	nth1 = *((int*)pElement1);
	int	nth2 = *((int*)pElement2);

	if (p_party[nth1]->y < p_party[nth2]->y)
		return -1;
	else if (p_party[nth1]->y > p_party[nth2]->y)
		return 1;

	return 0;
}

// 攻击性咒术的资料建立
BOOL BuildAttackMagicData(ACTION *pMaster, ACTION *pAttacker)
{
	int	i, idx = 0, midx, midy, count = 0;
	int	charidx[MAX_NUM_ATTACKED];

	memset(&AttMgc, 0, sizeof(AttMgc));
	memset(charidx, 0, sizeof(charidx));

	iAttackedNum = 0;
	iCurAttackedFinishNum = 0;

	// BJ|Attacker's No( 0 - 19 )|12345678|Attacker's MP|

	ATR_VCT_NO(pAttacker) = ATTACK_MAGIC_CASE;
	ATR_MP(pAttacker) = get_num();
	AttMgc.iPreMgcNum = get_num();
	AttMgc.iCurMgcNum = get_num();
	AttMgc.iPostMgcNum = get_num();
	AttMgc.wAttackType = (WORD)get_num();
	AttMgc.wAttackTimeSlice = (WORD)get_num();
	AttMgc.wShowType = (WORD)get_num();
	AttMgc.wScreenX = (WORD)get_num();
	AttMgc.wScreenY = (WORD)get_num();
	AttMgc.wPreMgcX = (WORD)get_num();
	AttMgc.wPreMgcY = (WORD)get_num();
	AttMgc.wPostMgcX = (WORD)get_num();
	AttMgc.wPostMgcY = (WORD)get_num();
	AttMgc.wPreMgcOnChar = (WORD)get_num();
	AttMgc.wCurMgcOnChar = (WORD)get_num();
	AttMgc.wPostMgcOnChar = (WORD)get_num();
	AttMgc.dwEQuake = (DWORD)get_num();
	AttMgc.dwEQuakeSTime = (DWORD)get_num();
	AttMgc.dwEQuakeETime = (DWORD)get_num();

	do
	{
		charidx[idx] = get_num();
	} while (0XFF != charidx[idx] && ++idx);

	if (0 == idx)
		return FALSE;

	while (0X12345678 != (AttackedInfo[iAttackedNum * 4] = get_num()))
	{
		AttackedInfo[iAttackedNum * 4 + 1] = get_num();
		AttackedInfo[iAttackedNum * 4 + 2] = get_num();
		AttackedInfo[iAttackedNum * 4 + 3] = get_num();
		iAttackedNum++;
	}

	AttMgc.dwCurFrame1 = 0;
	AttMgc.dwCurFrame2 = 0;
	AttMgc.dwEQuakeSFrame = AttMgc.dwEQuakeSTime >> 4;
	AttMgc.dwEQuakeEFrame = AttMgc.dwEQuakeETime >> 4;

	// 是否有前置咒术
	(0XFFFFFFFF == AttMgc.iPreMgcNum) ? AttMgc.wRunPreMgc = TRUE : AttMgc.wRunPreMgc = FALSE;

	// 绝对位置显示法
	if (1 == AttMgc.wShowType)
	{
		AttMgc.wNumAttacks = 1;
		AttMgc.wCurAttackNum = 0;
		AttMgc.posAttacked[0].x = AttMgc.wScreenX;
		AttMgc.posAttacked[0].y = AttMgc.wScreenY;
		AttMgc.wAttackedIndex[0] = 20;		// 全部人员皆同时呈受伤状态
		AttMgc.wMgcFrameCount[0] = 0;

		// 被攻击的敌人索引号
		for (i = 0; i < idx; i++)
			AttMgc.wAttackedIndex[i + 1] = charidx[i];

		AttMgc.wAttackedIndex[i + 1] = 0XFF;
	}
	// 居中位置显示法
	else
	{
		// 咒术的方式为全体同时攻击
		if (2 == AttMgc.wAttackType || 4 == AttMgc.wAttackType)
		{
			midx = 0;
			midy = 0;
			for (i = 0; i < idx; i++)
			{
				midx += p_party[charidx[i]]->x;
				midy += p_party[charidx[i]]->y;
			}
			midx = int(midx / idx);
			midy = int(midy / idx);
			AttMgc.wNumAttacks = 1;
			AttMgc.wCurAttackNum = 0;
			AttMgc.posAttacked[0].x = midx + AttMgc.wScreenX;
			AttMgc.posAttacked[0].y = midy + AttMgc.wScreenY;
			AttMgc.wAttackedIndex[0] = 20;		// 全部人员皆同时呈受伤状态
			AttMgc.wMgcFrameCount[0] = 0;
			// 被攻击的敌人索引号
			for (i = 0; i < idx; i++)
				AttMgc.wAttackedIndex[i + 1] = charidx[i];

			AttMgc.wAttackedIndex[i + 1] = 0XFF;
		}
		// 单独攻击
		else
		{
			// 被攻击的敌人索引号
			for (i = 0; i < idx; i++)
			{
				AttMgc.wNumAttacks++;
				AttMgc.posAttacked[i].x = AttMgc.wScreenX;
				AttMgc.posAttacked[i].y = AttMgc.wScreenY;
				AttMgc.wAttackedIndex[i] = charidx[i];
				AttMgc.wMgcFrameCount[i] = count;
				count += AttMgc.wAttackTimeSlice >> 4;
			}
		}
	}

	ATR_VCT_NO(pAttacker) = ATTACK_MAGIC_CASE;
	ATR_BODY_CNT(pMaster) = 1;
	bRunAttMgc = TRUE;

	return TRUE;
}

// 动态产生新的魔法
BOOL RunTimeMagic()
{
	// 开始地震
	if (AttMgc.dwEQuakeSFrame == AttMgc.dwCurFrame1)
	{
		g_iRunEarthQuake = 1;
		g_iNumRunEarthQuake = AttMgc.dwEQuakeEFrame - AttMgc.dwEQuakeSFrame;
	}
	// 结束地震
	if (AttMgc.dwEQuakeEFrame == AttMgc.dwCurFrame1)
		g_iRunEarthQuake = 3;
	AttMgc.dwCurFrame1++;
	// 前置魔法是否已经播完了
	if (FALSE == AttMgc.wRunPreMgc)
		return TRUE;
	// 是否仍有尚未播放的咒术
	if (AttMgc.wCurAttackNum < AttMgc.wNumAttacks)
	{
		while (1)
		{
			if (AttMgc.dwCurFrame2 == AttMgc.wMgcFrameCount[AttMgc.wCurAttackNum])
			{
				ACTION	*a0;

				a0 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
				ATR_NAME(a0) = monster;
				ATR_VCT_NO(a0) = ATTACK_MAGIC_CASE + 2;		// 咒术的执行
				ATR_DISP_PRIO(a0) = (1 == AttMgc.wCurMgcOnChar) ? D_PRIO_HIT_MARK : DISP_PRIO_TILE + 1;
				ATR_CHR_NO(a0) = AttMgc.iCurMgcNum;
				ATR_LONG_WORK(0, a0) = AttMgc.wAttackedIndex[AttMgc.wCurAttackNum];
				ATR_H_POS(a0) = (20 == AttMgc.wAttackedIndex[AttMgc.wCurAttackNum]) ? AttMgc.posAttacked[AttMgc.wCurAttackNum].x : ATR_H_POS(p_party[AttMgc.wAttackedIndex[AttMgc.wCurAttackNum]]) + AttMgc.posAttacked[AttMgc.wCurAttackNum].x;
				ATR_V_POS(a0) = (20 == AttMgc.wAttackedIndex[AttMgc.wCurAttackNum]) ? AttMgc.posAttacked[AttMgc.wCurAttackNum].y : ATR_V_POS(p_party[AttMgc.wAttackedIndex[AttMgc.wCurAttackNum]]) + AttMgc.posAttacked[AttMgc.wCurAttackNum].y;
				AttMgc.wCurAttackNum++;
			}
			if (AttMgc.wCurAttackNum >= AttMgc.wNumAttacks || AttMgc.wMgcFrameCount[AttMgc.wCurAttackNum] > AttMgc.dwCurFrame2)
				break;
		}
	}
	AttMgc.dwCurFrame2++;

	return TRUE;
}
#endif

//人物行动处理
void master(ACTION *a0)
{
	ACTION *a1, *a2, *a3;
	int d0, d1, d2, d3, d6, d7;
	int z = 0;
	int jjj = 0;
	int command_no;
	int sav_command_point, castflg = 0;
	int petfall_flg = 0;

	//属性表示
	disp_attrib(a0);
	if (ATR_VCT_NO(a0) == 0)	// 命令确认
	{
		if (BattleCmd[0] == NULL)		//战斗命令确认
			goto master_500;
	}
	switch (ATR_VCT_NO(a0))
	{
	case 0:		//?????
		sav_command_point = command_point;		//??????????
		command_no = get_command();		//??????
		if (command_no == -1)		//???
		{
			ATR_VCT_NO(a0) = 4;		//??????
			command_point = 0;		//?????????????
			break;
		}
		if (command_no == ATT_KANJI)		//????
		{
			if (p_kanji != NULL)		//???
			{
				a1 = ATR_BODY_WORK(0, p_kanji);		//??????
				DeathAction(a1);					//
			}
			else
			{
				p_kanji = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
				if (p_kanji == NULL)
					return;
			}
			//中文BOX做成
			a1 = MakeWindowDisp(640 - 4 - 64 * 4, 4, 4, 2, 0, 2);
			//记忆体不足产生失败
			if (a1 == NULL)
				return;
			ATR_BODY_WORK(0, p_kanji) = a1;		//?????
			ATR_NAME(p_kanji) = disp_kanji;		//??????
			ATR_ATTRIB(p_kanji) = ACT_ATR_HIDE;		//??
			// ????????
			play_se(202, 320, 240);
			command_point++;		//??????????
			//?????????
			get_command_asc();
			d0 = 60;		//???
			for (d7 = 0; d7 < 4; d7++)
			{
				if (kanji_buf[d7][0])		//??????
					d0 += 60;		//???
			}
			ATR_INT_WORK0(p_kanji) = d0;
			ATR_VCT_NO(a0) = 1;		//?????
			ATR_BODY_CNT(a0) = 1;
			ATR_DAMAGE(a0) = 0;
			ATR_PET_DAMAGE(a0) = 0;
			break;
		}
		if (command_no == ATT_EFFECT)
		{
#ifdef __NEW_BATTLE_EFFECT
			// Bj | target(who) | effect(effect no) | effect_anim | param | .... | FF
			ACTION *target = NULL;
			int who;
			int effect;
			int effect_anim;
			int param;

			ATR_VCT_NO(a0) = 1;
			while (1)
			{
				who = get_num();
				if (who == 255)
					break;
				effect = get_num();			//特效的编号
				effect_anim = get_num();	//特效的动画
				param = get_num();			//参数
				target = NULL;
				if (who < BATTLKPKPLYAERNUM)
					target = p_party[who];
				if (target)
				{
					ATR_EFFECT_FLAG(target) = effect;
					ATR_EFFECT_ANIM(target) = effect_anim;
					ATR_EFFECT_PARAM(target) = param;
					ATR_BODY_CNT(a0)++;
				}
			}
#endif
			break;
		}
#ifdef _PETSKILL_BATTLE_MODEL
		if (command_no == ATT_BATTLE_MODEL)			// 宠物技能战斗模组
		{
			ATR_VCT_NO(a0) = 1;
			ATR_PET_DAMAGE(a0) = 0;
			for (d7 = 0; d7 < BATTLKPKPLYAERNUM; d7++)
			{
				ATR_SYNC_FLG(p_party[d7]) = 0;
				ATR_ATTACK_KIND(0, p_party[d7]) = 0;
				ATR_ATTACK_KIND(1, p_party[d7]) = 0;
				ATR_ATTACK_POW(0, p_party[d7]) = 0;
				ATR_ATTACK_PET_POW(0, p_party[d7]) = 0;
				ATR_MPDAMAGE(p_party[d7]) = 0;
				ATR_MPDFLG(p_party[d7]) = 0;
				ATR_ADDHP(p_party[d7]) = 0;
				ATR_ADDHPFLG(p_party[d7])=0;
				ATR_SHOWADDHP(p_party[d7])=0;
				ATR_LONG_WORK(0, p_party[d7]) = 0;
				ATR_BATTLE_MODEL(p_party[d7]) = 0;
				memset(&ATR_BODY_WORK(0, p_party[d7]), 0, sizeof(ACTION*) * 20);
				memset(p_missile, 0, sizeof(p_missile));
			}
			d7 = 0;
			// 先取出攻击的宠物编号
			d0 = get_num();
			a3 = p_party[d0];
			// 判断攻击方是左上还右下来决定初始位置
			if (ATR_GROUP_FLG(a3) == 0)
			{
				d2 = 3;	// 右下
				d0 = 0;
			}
			else
			{
				d2 = 7; // 左上
				d0 = 10;
			}
			// 取出目标攻击物件编号
			a1 = NULL;
			while ((d6 = get_num()) != 255)
			{
				a1 = p_party[d6];
				if (ATR_VCT_NO(a1) != 55)
					ATR_FIRST_FLG(a1) = 0;
				ATR_ATTACK_KIND(2, a1) = 0;
				d3 = get_num();			// 取出攻击物件编号
				// 判断 p_missile[d3] 有没有在使用
				// 有使用,表示这个攻击物件要攻击复数目标
				if (p_missile[d3] != NULL)
				{
					a2 = p_missile[d3];
					ATR_BODY_WORK(ATR_INT_WORK1(a2), a2) = a1;				// 被攻方
					ATR_BATTLE_MODEL(a1) = ATT_BATTLE_MODEL;				// 此目标是被 ATT_BATTLE_MODEL 型态攻击
					ATR_ATTACK_KIND(ATR_INT_WORK1(a2), a2) = get_num();		// 攻击种类
					ATR_ATTACK_POW(ATR_INT_WORK1(a2), a2) = get_num();		// 攻击力
					ATR_ATTACK_PET_POW(ATR_INT_WORK1(a2), a2) = get_num();	// pet攻击力
					ATR_CHR_NO(a2) = get_num();																	// 攻击物件图号
					// 如果有忠犬效果,记录使用忠犬的目标
					if (BattleCmd[command_point] == 'g')
						ATR_LONG_WORK(ATR_INT_WORK1(a2), a2) = get_num();
					ATR_INT_WORK1(a2)++;
					// 记录目标有被此攻击物件攻击
					if (ATR_LONG_WORK(0, a1) == 0)
					{
						ATR_LONG_WORK(0, a1) = 1;		// ATR_LONG_WORK 栏位 : 位址 0 : 是设定目标是否被攻击过,所以都会设定为1
						ATR_LONG_WORK(d3, a1) = d7 + 1; //						位址 d3: d3 是攻击物件的编号,而 d3 的位址是用来
						//								 纪录攻击物件在第 d7 + 1 的回合攻击目标
					}
					else
						ATR_LONG_WORK(d3, a1) = d7 + 1;	// 当 ATR_LONG_WORK 栏位 0 不为 0 时,表示这个目标已被别的攻击物件攻击,
					// 所以直接在 d3 的位址记录要在 d7 + 1 的回合攻击目标
				}
				// 没使用过
				else
				{
					p_missile[d3] = GetAction(T_PRIO_BOW, sizeof(ATR_EQU));
					if (p_missile[d3] == NULL)
						break;	// 不太可能
					a2 = p_missile[d3];
					// 初始化
					ATR_NAME(a2) = monster;
					ATR_CHR_ANG(a2) = d2;
					ATR_H_POS(a2) = monster_start_pos[monster_place_no[d3 + d0] * 2];
					ATR_V_POS(a2) = monster_start_pos[monster_place_no[d3 + d0] * 2 + 1];

					ATR_CHR_ACT(a2) = ANIM_STAND;
					ATR_GROUP_FLG(a2) = ATR_GROUP_FLG(a3);
					a2->hitDispNo = a3->hitDispNo;
					ATR_VCT_NO(a2) = 1;							// 前进
					ATR_BODY_WORK(0, a2) = a1;					// 被攻击目标
					ATR_BATTLE_MODEL(a1) = ATT_BATTLE_MODEL;	// 此目标是被 ATT_BATTLE_MODEL 型态攻击
					// 记录目标有被此攻击物件攻击
					if (ATR_LONG_WORK(0, a1) == 0)
					{
						ATR_LONG_WORK(0, a1) = 1;			// ATR_LONG_WORK 栏位 : 位址 0 : 是设定目标是否被攻击过,所以都会设定为1
						ATR_LONG_WORK(d3, a1) = d7 + 1;		//						位址 d3: d3 是攻击物件的编号,而 d3 的位址是用来
						//								 纪录攻击物件在第 d7 + 1 的回合攻击目标
					}
					else
						ATR_LONG_WORK(d3, a1) = d7 + 1;		// 当 ATR_LONG_WORK 栏位 0 不为 0 时,表示这个目标已被别的攻击物件攻击,
					// 所以直接在 d3 的位址记录要在 d7 + 1 的回合攻击目标
					ATR_ATTACK_KIND(0, a2) = get_num();		// 攻击种类
					ATR_ATTACK_POW(0, a2) = get_num();		// 攻击力
					ATR_ATTACK_PET_POW(0, a2) = get_num();	// pet攻击力
					ATR_CHR_NO(a2) = get_num();				// 攻击物件图号
					ATR_PLACE_NO(a2) = d3;					// 记录攻击物件编号
					ATR_INT_WORK0(a2) = ATT_BATTLE_MODEL;	// 设定此 action 是 ATT_BATTLE_MODEL 在使用,以方便之后处理的辨识,使用 work0 记录
					ATR_INT_WORK1(a2) = 1;					// 设定此攻击物件目前攻击目标数量,使用 work1记录
					ATR_INT_WORK2(a2) = 0;					// 作为是否第一次执行离开动画旗标
					ATR_INT_WORK3(a2) = 0;					// 作为若有忠犬则检查过后不再检查忠犬的旗标
					// 如果有忠犬效果,记录使用忠犬的目标
					if (BattleCmd[command_point] == 'g')
						ATR_LONG_WORK(0, a2) = get_num();
					ATR_SPD(a2) = 32;						// 移动速度
				}
				d7++;
			}
			ATR_COUNTER(a0) = a1;
			ATR_DAMAGE(a0) = -d7;	// 设为负数,让正常时候的 ATR_DAMAGE(p_master) 不会等于 ATR_BODY_CNT(p_master)
			ATR_INT_WORK1(a0) = 0;	// 确认 p_missile 是否都被清空
			ATR_INT_WORK2(a0) = 1;	// 攻击物件攻击时的顺序,先从 ATR_LONG_WORK 为 1 的先执行
			ATR_BODY_CNT(a0) = d7;	// 攻击总次数,每完成一次动作 ATR_DAMAGE(p_master) 都会递增,直到 ATR_DAMAGE(p_master) ==  ATR_BODY_CNT(p_master)
			// p_master 才会去继续处理下一笔 BattleCmd 里的资料 (这里的 a0 就是 p_master)
			break;
		}
#endif
		d0 = get_num();		//??????
		if (d0 < 0 || d0 >= BATTLKPKPLYAERNUM)		//䥺???????
		{
			if (command_no == ATT_VARIABLE)		//???
			{
				ATR_ATTRIB(p_master) = get_num();		//??
				break;
			}
			else
			{
#ifdef _STONDEBUG_
				MessageBoxNew(hWnd, "command_no != ATT_VARIABLE", "Error", MB_OK);
#endif
				break;
			}
		}
		a1 = p_party[d0];		//???????
		if (ATR_NAME(a1) == NULL)		//???????????????
		{
#ifdef _STONDEBUG_
			MessageBoxNew(hWnd, "ATR_NAME(a1) == NULL", "Error", MB_OK);
#endif
			command_no = get_command();		//??????
			if (command_no == -1)		//???
			{
				ATR_VCT_NO(a0) = 4;		//??????
				command_point = 0;		//?????????????
				break;
			}
			command_point -= 2;		//??????????
			break;
		}
		if (command_no != ATT_MALFUNCTION)		//??????
		{
			if (ATR_LIFE(a1) <= 0)		//??????
				ATR_VCT_NO(a1) = 0;		//??
		}
		if (command_no != ATT_MALFUNCTION)		//??????
		{
			if (ATR_VCT_NO(a1) != 0)		//??			command_point = sav_command_point;		//?????
				break;
		}
		ATR_VCT_NO(a0) = 1;		//⺳???
		ATR_DAMAGE(a0) = 0;
		ATR_PET_DAMAGE(a0) = 0;
		ATR_COUNTER(a0) = a1;		//?????
		//?????????????
		if (ATR_VCT_NO(a1) != 55)
			ATR_FIRST_FLG(a1) = 0;		//????????
		for (d7 = 0; d7 < BATTLKPKPLYAERNUM; d7++)		//??????
		{
			ATR_SYNC_FLG(p_party[d7]) = 0;
			ATR_ATTACK_KIND(0, p_party[d7]) = 0;
			ATR_ATTACK_KIND(1, p_party[d7]) = 0;
			ATR_ATTACK_POW(0, p_party[d7]) = 0;
			ATR_ATTACK_PET_POW(0, p_party[d7]) = 0;
			//andy_mp
			ATR_MPDAMAGE(p_party[d7]) = 0;
			ATR_MPDFLG(p_party[d7]) = 0;
			ATR_ADDHP(p_party[d7]) = 0;
			ATR_ADDHPFLG(p_party[d7]) = 0;
			ATR_SHOWADDHP(p_party[d7]) = 0;
#ifdef _PETSKILL_BATTLE_MODEL
			ATR_BATTLE_MODEL(p_party[d7]) = 0;
			ATR_BODY_WORK(0, p_party[d7]) = NULL;
#endif
		}
		//??????
		ATR_ATTACK_KIND(2, a1) = 0;
		switch (command_no)
		{
			// BI jibun_5 teki_F flg_2 damage_1 teki_9 flg_2 damage_1 FF
		case ATT_IN:		//??????????  佋???  ??????????
			if (ATR_GROUP_FLG(a1) == 0)		//????????
			{
				ATR_V_POS(a1) = 8;
				ATR_H_POS(a1) = -SCREEN_OUT;
			}
			else
			{
				ATR_V_POS(a1) = lpDraw->ySize - 8;
				ATR_H_POS(a1) = lpDraw->xSize + SCREEN_OUT;
			}
		case ATT_HIT:		//====================直接攻击===============
			ATR_VCT_NO(a1) = 1;							//前进
			ATR_BODY_WORK(0, a1) = p_party[get_num()];	//被攻方
			ATR_ATTACK_KIND(0, a1) = get_num();			//攻击种类
			ATR_ATTACK_POW(0, a1) = get_num();			//攻击力
			if (BattleCmd[command_point] == 'p')
				ATR_ATTACK_PET_POW(0, a1) = get_num();	//pet攻击力
#ifdef _STONDEBUG_
			else
				LogToBattleError( BattleCmd, __LINE__ );
#endif
			ATR_BODY_CNT(a0) = 1;
#ifdef _ATTDOUBLE_ATTACK
			//andy_add
			if (ATR_ATTACK_KIND(0, a1) & ATT_ATTDOUBLE || ATR_ATTACK_KIND(0, a1) & BCF_MODIFY)
				ATR_BATTLEGRANO(a1) = get_num();
#endif
#ifdef _ATTACK_EFFECT
			//修正忠犬  xiezi
			if (BattleCmd[command_point] == 's')
				ATR_LONG_WORK(0, a1) = get_num();	// 记录特效编号
#endif
			break;
#ifdef _MAGIC_DEEPPOISION
		case ATT_DEEPPOISION:	//剧毒
			ATR_VCT_NO(a1) = 1;	//攻方动作
			ATR_BODY_WORK(0, a1) = p_party[get_num()];//攻方编号		//????
			ATR_ATTACK_KIND(0, a1) = get_num();		//我方防御种类??????
			ATR_ATTACK_POW(0, a1) = get_num();		//攻击力??????
			ATR_ATTACK_PET_POW(0, a1) = get_num();//骑宠损伤
			ATR_BODY_CNT(a0) = 1;//攻击次数
			ATR_DEEPPOISION(a1) = 1;
			break;
#endif
			//andy_mp
		case ATT_MPDAMAGE://MP伤害
			ATR_VCT_NO(a1) = 1;
			ATR_BODY_WORK(0, a1) = p_party[get_num()];
			ATR_ATTACK_KIND(0, a1) = get_num();
			ATR_ATTACK_POW(0, a1) = get_num();
			ATR_ATTACK_PET_POW(0, a1) = get_num();
			ATR_MPDAMAGE(a1) = get_num();
			ATR_BODY_CNT(a0) = 1;
			ATR_MPDFLG(a1) = 1;
			break;	//set_damage_num( a1, 16, -64 )

		case ATT_DAMAGETOHP:
			ATR_VCT_NO(a1) = 1;
			ATR_BODY_WORK(0, a1) = p_party[get_num()];
			ATR_ATTACK_KIND(0, a1) = get_num();
			ATR_ATTACK_POW(0, a1) = get_num();
			ATR_ATTACK_PET_POW(0, a1) = get_num();
			ATR_ADDHP(a1) = get_num();
			ATR_BODY_CNT(a0) = 1;
			ATR_SHOWADDHP(a1) = 1;
			ATR_ADDHPFLG(a1) = 1;
			break;
			//BF jibun_5
		case ATT_FADE_OUT:		//??????????  佋???  ??????????
			ATR_VCT_NO(a1) = 80;		//??
			//??????
			ATR_ATTACK_KIND(2, a1) = 1;
			ATR_BODY_CNT(a0) = 1;
			break;
			//BN|????|??????|
		case ATT_NEXT_EQUIP:		//??????????  ??  ??????????
			ATR_VCT_NO(a1) = 82;		//??
			ATR_BODY_CNT(a0) = 1;
			break;
			//BJ|Attacker's No( 0 - 19 )|Attacker's MP|Attacker's animation index|
			//                                                Opposite's animation index|Opposite's index ...
			//BJ|????|??|?????????|???????????|?????|?????|FF
		case ATT_JUJUTSU:		//??????????    ??????????
			ATR_VCT_NO(a1) = 75;		//?䞍???
			ATR_MP(a1) = get_num();		//?????
			ATR_BODY_CNT(a0) = 1;
#ifdef __ATTACK_MAGIC
			if (ATTACK_MAGIC_ID == ATR_MP(a1))
				BuildAttackMagicData(a0, a1);
#endif
			break;
		case ATT_TOCALL:
			ATR_VCT_NO(a1) = 2;		//?䞍???
			ATR_MP(a1) = get_num();		//?????
			ATR_BODY_CNT(a0) = 1;
#ifdef __TOCALL_MAGIC
			if (TOCALL_MAGIC_ID == ATR_MP(a1))
				BuildToCallMagicData(a0, a1);
#endif
			break;
		case ATT_MALFUNCTION:		//??????????    ??????????
			d0 = get_num();		//???
			ATR_STATUS(a1) = d0;		//????????
			switch (d0)
			{
				//??
			case 0:
				ATR_VCT_NO(a0) = 0;		//????
				a2 = ATR_JUJUTSU_WORK(a1);
				ATR_JUJUTSU_WORK(a1) = NULL;
				DeathAction(a2);		//?
				break;
				//??
			case 2:
				//?????
				if (ATR_LIFE(a1) > 0)
				{
					ATR_BODY_CNT(a0) = 1;
					ATR_VCT_NO(a1) = 105;		//????
					ATR_STIMER(a1) = 60;
					set_single_jujutsu(d0, a1);		//???
				}
				else
					ATR_VCT_NO(a0) = 0;		//????
				break;
				//?䞍
			default:
				ATR_VCT_NO(a0) = 0;		//????
				set_single_jujutsu(d0, a1);		//???
				break;
			}
			break;
			//ATR_ATTACK_KIND(a1);
			/*
					case ATT_MAGICSTATUS:	//andy_add magicstatus
					d0 = get_num();
					ATR_STATUS(a1) = d0;
					switch (d0){
					case 0:
					ATR_VCT_NO(a0) = 0;
					a2 = ATR_MAGICSU_WORK(a1);
					ATR_MAGICSU_WORK(a1) = NULL;
					DeathAction( a2 );
					break;

					default:
					ATR_VCT_NO(a0) = 0;
					set_magic_status(d0, a1);
					break;
					}
					break;
					*/

			// ???????????????????
		case ATT_DAMAGE:
			ATR_BODY_CNT(a0) = 1;
			ATR_VCT_NO(a1) = 79;
			ATR_LONG_WORK(0, a1) = 0;
			d2 = d0 = get_num();
			d3 = d1 = get_num();
		att_damage_loop:
			ATR_DAMAGE(a1) = get_num();
			if (BattleCmd[command_point] == 'p')
				ATR_PET_DAMAGE(a1) = get_num();		//pet damage

			switch (d0)
			{
			case 0:		//??
				switch (d1)
				{
				case 0:		//?
					set_damage_num(a1, 6, -64);
					ATR_LIFE(a1) -= ATR_DAMAGE(a1);
					set_jujutsu_hit_mark(a1);		//??????
					if (ATR_LIFE(a1) <= 0)		//??????
					{
						ATR_LIFE(a1) = 0;
						slow_flg++;		//????
					}
					ATR_PET_LIFE(a1) -= ATR_PET_DAMAGE(a1);
					set_jujutsu_hit_mark(a1);		//
					//andy_log
					if (ATR_PETFALL(a0) == 1)
					{
						if (ATR_LIFE(a0) > 0)
							petfallChangeGraph(a0);
					}
					if (ATR_PET_LIFE(a1) <= 0 && ATR_RIDE(a1))		//??????
					{
						ATR_PET_LIFE(a1) = 0;
						//slow_flg++;		//????
						if (ATR_LIFE(a1) > 0)
							petfallChangeGraph(a1);
					}
					ATR_CHR_ACT(a1) = ANIM_DAMAGE;		//???????????
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a1) = 78;		//???????
					break;
				case 1:		//?
					//??
					set_damage_num(a1, 14, -64);
					ATR_LIFE(a1) += ATR_DAMAGE(a1);
					if (ATR_LIFE(a1) > ATR_MAX_LIFE(a1))		//????
						ATR_LIFE(a1) = ATR_MAX_LIFE(a1);		//????
					ATR_PET_LIFE(a1) += ATR_PET_DAMAGE(a1);
					if (ATR_PET_LIFE(a1) > ATR_PET_MAX_LIFE(a1))
						ATR_PET_LIFE(a1) = ATR_PET_MAX_LIFE(a1);
					play_se(102, ATR_H_POS(a1), 240);
					break;
					//andy_add	回合补血
				case 2:
					if (BattleCmd[command_point] == 'm')
						ATR_MPDAMAGE(a1) = get_num();
					set_damage_num(a1, 36, -64);
					ATR_LIFE(a1) += ATR_DAMAGE(a1);
					ATR_MP(a1) += ATR_MPDAMAGE(a1);
					if (ATR_LIFE(a1) > ATR_MAX_LIFE(a1))
						ATR_LIFE(a1) = ATR_MAX_LIFE(a1);
					ATR_PET_LIFE(a1) += ATR_PET_DAMAGE(a1);
					if (ATR_PET_LIFE(a1) >= ATR_PET_MAX_LIFE(a1))
					{
						int k;

						k = ATR_PET_MAX_LIFE(a1);
						k = ATR_PET_LIFE(a1);
						ATR_PET_LIFE(a1) = ATR_PET_MAX_LIFE(a1);
						k = ATR_PET_LIFE(a1);
					}
					//ATR_CHR_ACT(a1) = ANIM_DAMAGE;
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a1) = 78;
					break;
				case 3:
					set_damage_num(a1, 37, -64);
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a1) = 78;
					break;
				case 4://攻
					set_damage_num(a1, 38, -64);
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a1) = 78;
					break;
				case 5://防
					set_damage_num(a1, 39, -64);
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a1) = 78;
					break;
				case 6://敏
					set_damage_num(a1, 40, -64);
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a1) = 78;
					break;
#ifdef _SKILL_ADDBARRIER
				case 7://抗性
					set_damage_num(a1, 43, -64);
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a1) = 78;
					break;
#endif 
#ifdef _PETSKILL_PEEL
				case 8://被卸下装备
					set_damage_num(a1, 44, -64);
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a1) = 78;
					break;
#endif
#ifdef _PETSKILL_JUSTICE
				case 9:
					set_damage_num(a1, 45, -64);
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a1) = 78;
					break;
#endif
#ifdef _PETSKILL_PROVOKEFIGHT
				case 10:
					set_damage_num(a1, 48, -64);
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a1) = 78;
					break;
#endif
#ifdef _PRO3_ADDSKILL
				case 11:
					set_damage_num(a1, 49, -64);
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a1) = 78;
					break;
				case 12:
					set_damage_num(a1, 50, -64);
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a1) = 78;
					break;
				case 13:
					set_damage_num(a1, 51, -64);
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a1) = 78;
					break;
#endif
#ifdef _PETSKILL_ADDATTCRAZED
					/*	case 10:
							set_damage_num(a1, 46, -64);
							ATR_CHR_ACT_OLD(a1) = -1;
							pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
							ATR_VCT_NO(a1) = 78;
							break;*/
#endif
				}
				break;

			case 1:		//??
				switch (d1)
				{
				case 0:		//?
					set_damage_num(a1, 16, -64);
					ATR_MP(a1) -= ATR_DAMAGE(a1);		//??????
					if (ATR_MP(a1) < 0)		//?⺶???
						ATR_MP(a1) = 0;		//????
					break;
				case 1:		//?
					set_damage_num(a1, 15, -64);
					ATR_MP(a1) += ATR_DAMAGE(a1);		//?????

#ifdef _FIXSHOWMPERR //Syu ADD 修正补气时动画显示错误
					if (ATR_MP(a1) > pc.maxMp)
						ATR_MP(a1) = pc.maxMp;
#else
					if (ATR_MP(a1) > 100)		//??????
						ATR_MP(a1) = 100;		//??????
#endif
					//???
					play_se(102, ATR_H_POS(a1), 240);
					break;
#ifdef _PETSKILL_ADDATTCRAZED
				case 3:
					set_damage_num(a1, 46, -64);
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a1) = 78;
					break;
				case 4:
					set_damage_num(a1, 47, -64);
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a1) = 78;
					break;
#endif
				}
				break;
#ifdef _PETSKILL_LER
			case 3:	// 闪开攻击
				set_damage_num(a1, 0, -64);
				break;
#endif
			}
			d6 = command_point;		//????????
			d0 = get_command();		//????????
			if (d0 == ATT_DAMAGE)		//??????
			{
				a1 = p_party[d0 = get_num()];		//???????
				d0 = get_num();		//???????
				if (d2 == d0)		//???????
				{
					d1 = get_num();		//???????
					if (d3 == d1)		//???????
						goto att_damage_loop;
				}
			}
			command_point = d6;		//??????????
			break;
			//BV|?????????|?
		case ATT_VARIABLE:		//??????????  ??????  ??????????
			ATR_BODY_CNT(a0) = 1;
			ATR_VCT_NO(a1) = 85;		//???????
			break;
			//BR|????|????????
		case ATT_REVERSE:		//??????????    ??????????
			ATR_VCT_NO(a0) = 0;		//????
			d0 = get_num();		//???????????
			switch (d0)
			{
				//???
			case 0:
				a2 = ATR_ATTRIB_WORK(a1);
				ATR_ATTRIB_WORK(a1) = NULL;
				DeathAction(a2);		//?
				break;
				//??
			default:
				set_attrib_reverse(a1);		//???
				break;
			}
			break;
			//BL|????|??
		case ATT_LIFE:		//??????????  ??  ??????????
			ATR_VCT_NO(a0) = 0;		//????
			ATR_DAMAGE(a1) = get_num();		//????
			//??
			ATR_LIFE(a1) = ATR_DAMAGE(a1);
			if (ATR_LIFE(a1) > ATR_MAX_LIFE(a1))		//????
				ATR_LIFE(a1) = ATR_MAX_LIFE(a1);		//????
			ATR_VCT_NO(a1) = 0;		//?
			ATR_CHR_ACT(a1) = ANIM_STAND;		//???????
			pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
			break;
			//BQ|????
		case ATT_QUIT:		//??????????  ?????  ??????????
			ATR_BODY_CNT(a0) = 1;
			ATR_VCT_NO(a1) = 87;		//??????
			break;
			//BX|????
		case ATT_NIX:		//??????????  ????  ??????????
			ATR_BODY_CNT(a0) = 1;
			ATR_VCT_NO(a1) = 90;		//????
			break;
			//B!|??????????????????
		case ATT_COMPANIONS:	//??????????  ????  ??????????
			ATR_BODY_CNT(a0) = 1;
			ATR_VCT_NO(a1) = 95;		//??????
			break;
			//B#|????????????
		case ATT_STEAL:	//??????????  ????  ??????????
			ATR_BODY_CNT(a0) = 1;
			ATR_VCT_NO(a1) = 100;		//??????
			break;
			//B%|?????????
		case ATT_TALK:	//??????????  ????  ??????????
			ATR_BODY_CNT(a0) = 1;
			ATR_VCT_NO(a1) = 110;		//?????
			ATR_STIMER(a1) = 60;
			d0 = get_num();		//??????
			//?????????
			set_damage_num(a1, 20, -64);
			break;
#ifdef _FIREHUNTER_SKILL				// (不可开) ROG ADD 朱雀技能_火线猎杀
		case ATT_FIRESKILL:
			ATR_VCT_NO(a1) = FIRE_HUNTER_SKILL;		//火线猎杀
			ATR_FIRST_FLG(a1) = 0;
			iBeAttNum = get_num();
			ATR_BODY_WORK(0, a1) = p_party[iBeAttNum];		//敌人位址
			ATR_ATTACK_KIND(0, a1) = get_num();				//攻击种类
			ATR_ATTACK_POW(0, a1) = get_num();				//攻击力
			//			if ( BattleCmd[command_point] == 'p' )	{
			ATR_ATTACK_PET_POW(0, a1) = get_num();		//pet攻击力??
			//			}
			d0 = get_num();							//人数
			ATR_BODY_CNT(a1) = d0;					//多人攻击
			for (d7 = 2; d7 < d0 + 2; d7++)
			{
				int x = get_num();

				if (x == iBeAttNum)               //被攻击目标的魔法伤害
				{
					tarMgiDem = 1;
					tarpos = d7;//ATR_BODY_CNT(a1)--;
				}
				ATR_BODY_WORK(d7 ,a1) = p_party[x];		//敌人位址
				ATR_ATTACK_KIND(d7 ,a1) = get_num();		//攻击种类
				ATR_ATTACK_POW(d7 ,a1) = get_num();				//攻击力
				ATR_ATTACK_PET_POW(d7 ,a1) = get_num();		//pet攻击力
				//}
			}
			break;
#endif
#ifdef _PROFESSION_ADDSKILL
		case ATT_BOUNDARY:
			ATR_VCT_NO(a0) = 0;
			ATR_CHR_ACT(a0) = ANIM_STAND;
			ATR_FIRST_FLG(a0) = 0;	
			ATR_VCT_NO(a1) = 0;		//?
			ATR_CHR_ACT(a1) = ANIM_STAND;		//???????
			ATR_FIRST_FLG(a1) = 0;
			d0 = get_num();		//???????????
			BuildBoundaryMagicData( d0 );
			d0 = get_num();
			break;
#endif
#ifdef _PETSKILL_RIDE
		case ATT_RIDE:
			ATR_VCT_NO(a0) = 0;
			ATR_CHR_ACT(a0) = ANIM_STAND;
			ATR_FIRST_FLG(a0) = 0;	
			ATR_VCT_NO(a1) = 0;		//?
			ATR_CHR_ACT(a1) = ANIM_STAND;		//???????
			ATR_FIRST_FLG(a1) = 0;
			d0 = get_num(); //上马图号
			petrideChangeGraph(a1,d0);
			d0 = get_num();
			break;
#endif
		case ATT_BOW:		//??????????  ???  ??????????
			ATR_VCT_NO(a1) = 30;		//????
			ATR_LONG_WORK(0, a1) = get_num();		//????
			if (ATR_LONG_WORK(0, a1) == 0)		//???
			{
				ATR_VCT_NO(a1) = 32;		//????
				ATR_STIMER(a1) = 0;		//????????
				ATR_DAMAGE(a1) = 1;		//
				ATR_PET_DAMAGE(a1) = 1;
			}
			ATR_FIRST_FLG(a1) = 0;		//
			ATR_BODY_WORK(0, a1) = p_party[get_num()];		//????
			ATR_ATTACK_KIND(0, a1) = get_num();		//??????
			ATR_ATTACK_POW(0, a1) = get_num();		//??????
			if (BattleCmd[command_point] == 'p')
				ATR_ATTACK_PET_POW(0, a1) = get_num();		//pet??????

			//修正技能特效 xiezi
#ifdef _ATTACK_EFFECT
			if (BattleCmd[command_point] == 's')
				ATR_LONG_WORK(0, a1) = get_num();	// 记录特效编号
#endif

			ATR_BODY_CNT(a0) = 1;
#ifdef _SHOOTCHESTNUT	// Syu ADD 宠技：丢栗子
			(ATR_CHR_NO(a1) == 101578) ? ShooterNum = 101578 : ShooterNum = -1;
#endif
			break;
		case ATT_BOOMERANG:		//??????????  ???????  ??????????
			ATR_VCT_NO(a1) = 70;		//????????
			ATR_FIRST_FLG(a1) = 0;		//
			ATR_BODY_CNT(a0) = 1;
			break;
		case ATT_FIRE:		//??????????  ????  ??????????
			d0 = get_body_cnt();		//?????
			if (d0 != 1)		//????
			{
				ATR_VCT_NO(a1) = 20;		//????
				ATR_BODY_CNT(a0) = d0;		//????
				ATR_BODY_CNT(a1) = d0;		//
				if (ATR_GROUP_FLG(a1) == 0)
					ATR_CHR_ANG(a1) = 3;
				else if (ATR_GROUP_FLG(a1) == 1)
					ATR_CHR_ANG(a1) = 7;
#ifdef _BATTLE_PK_PLAYER_FOR_40
				else if (ATR_GROUP_FLG(a1) == 2)
					ATR_CHR_ANG(a1) = 5;
				else if (ATR_GROUP_FLG(a1) == 3)	
					ATR_CHR_ANG(a1) = 1;
#endif

			}
			else
			{	//?????
				ATR_VCT_NO(a1) = 20;		//????
				ATR_BODY_CNT(a0) = 1;		//???
				ATR_BODY_CNT(a1) = 1;		//
				a2 = ATR_BODY_WORK(0, a1) = p_party[get_num()];		//?
			}
			break;
		case ATT_SYNCHRONOUS:		//??????????  ???  ??????????
			a3 = a2 = a1;		//?
			d1 = 0;		//????????
			d2 = 0;
			d7 = 0;		//??????
			while (1)
			{
				d0 = get_num();		//???????
				if (d0 == 255)		//???
					break;
				a1 = p_party[d0];		//???????
				if (ATR_NAME(a1) == NULL)		//????????
				{
#ifdef _STONDEBUG_
					MessageBoxNew(hWnd, "没有合体攻击的名字紧急连络日本！", "Error", MB_OK);
#endif
					get_num();		//??????
					get_num();		//??????
					get_num();		//????
					if (BattleCmd[command_point] == 'p')
						get_num();		//pet????

					continue;
				}
				if (d7 == 0)		//???????
					ATR_COUNTER(a0) = a1;		//??????
				ATR_VCT_NO(a1) = 1;		//?
				ATR_BODY_WORK(0, a1) = a3;		//?
				ATR_BODY_WORK(1, a2) = a1;		//????????????
				ATR_ATTACK_KIND(0, a1) = get_num();
#ifdef _PETSKILL_ACUPUNCTURE
				///////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
				if (ATR_ATTACK_KIND(0, a1) & ATT_REFLEX+ATT_ABSORPTION+ATT_BALLIA+ATT_TRAP+ATT_ACUPUNCTURE
#ifdef _PET_ITEM
					+ ATT_ATTACKBACK
#endif
					)
#else
				if (ATR_ATTACK_KIND(0, a1) & ATT_REFLEX+ATT_ABSORPTION+ATT_BALLIA+ATT_ACUPUNCTURE
#ifdef _PET_ITEM
					+ ATT_ATTACKBACK
#endif
					)
#endif
					///////////////////////
#else
				///////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
				if (ATR_ATTACK_KIND(0, a1) & ATT_REFLEX+ATT_ABSORPTION+ATT_BALLIA+ATT_TRAP)
#else
				if (ATR_ATTACK_KIND(0, a1) & ATT_REFLEX + ATT_ABSORPTION + ATT_BALLIA)
#endif
					///////////////////////
#endif
				{
					ATR_ATTACK_POW(0, a1) = get_num();		//??????
					if (BattleCmd[command_point] == 'p')
						ATR_ATTACK_PET_POW(0, a1) = get_num();		//pet??????
					//修正技能特效 xiezi
#ifdef _ATTACK_EFFECT
					if (BattleCmd[command_point] == 's')
						ATR_LONG_WORK(0, a1) = get_num();	// 记录特效编号
#endif
				}
				else
				{
					d1 += ATR_ATTACK_POW(0, a1) = get_num();		//????
					if (BattleCmd[command_point] == 'p')
						d2 += ATR_ATTACK_PET_POW(0, a1) = get_num();		//pet????
					else
						LogToBattleError(BattleCmd, __LINE__);

					//修正技能特效 xiezi
#ifdef _ATTACK_EFFECT
					if (BattleCmd[command_point] == 's')
						ATR_LONG_WORK(0, a1) = get_num();	// 记录特效编号
#endif
				}
				ATR_FIRST_FLG(a1) = 0;		//??????
				ATR_SYNC_FLG(a1) = 1;		//
				a2 = a1;		//????
				d7++;		//????
			}
			ATR_SYNC_FLG(a1) = 2;		//?????
			if (d1)		//???????
				ATR_ATTACK_POW(0, a1) = d1;		//????????
			if (d2)		//???????
				ATR_ATTACK_PET_POW(0, a1) = d2;		//????????
			ATR_LONG_WORK(0, a3) = d7;		//??????
			ATR_BODY_CNT(a0) = d7;
			break;
		case 2:		//??????????  ????  ??????????
			d0 = get_num();		//?????
			if (d0 != 1)		//??????
			{
				ATR_VCT_NO(a1) = 25;		//????
				ATR_BODY_CNT(a0) = d0;		//????
				ATR_BODY_CNT(a1) = d0;		//
				for (d7 = 0; d7 < d0; d7++)
				{
					ATR_BODY_WORK(d7, a1) = p_party[get_num()];		//?
					ATR_ATTACK_KIND(d7, a1) = get_num();		//??????
				}
				d0 = 320;
				d1 = 240;
				radar(a1, &d0, &d1);	//????
				ATR_CRS(a1) = d0;		/* ?????? */
				ATR_CHR_ANG(a1) = crs_change_tbl[d0];		/* ????? */
			}
			else
			{
				ATR_VCT_NO(a1) = 25;		//????
				ATR_BODY_CNT(a0) = 1;		//???
				ATR_BODY_CNT(a1) = 1;		//
				ATR_BODY_WORK(0, a1) = p_party[get_num()];		//?
				ATR_ATTACK_KIND(0, a1) = get_num();		//??????
				d0 = 320;
				d1 = 240;
				radar(a1, &d0, &d1);	//????
				ATR_CRS(a1) = d0;		/* ?????? */
				ATR_CHR_ANG(a1) = crs_change_tbl[d0];		/* ????? */
			}
			break;
		case 3:		//??????????  ????  ??????????
			d0 = get_num();		//?????
			ATR_VCT_NO(a1) = 30;		//????
			ATR_BODY_CNT(a0) = d0;		//???
			ATR_BODY_CNT(a1) = d0;		//
			for (d7 = 0; d7 < d0; d7++)
			{
				ATR_BODY_WORK(d7, a1) = p_party[get_num()];		//?
				ATR_ATTACK_KIND(d7, a1) = get_num();		//??????
			}
			break;
		case ATT_TAKE:		//??????????  ?  ??????????
			ATR_VCT_NO(a1) = 35;		//??
			ATR_BODY_WORK(0, a1) = p_party[get_num()];		//????
			ATR_ATTACK_KIND(0, a1) = get_num();		//????
			ATR_BODY_CNT(a0) = 1;
			break;
		case ATT_ESCAPE:		//??????????    ??????????
			ATR_BODY_CNT(a0) = 1;		//????
			d1 = ATR_ATTACK_KIND(0, a1) = get_num();		//????
			ATR_VCT_NO(a1) = 52;		//?
			if (BattleMyNo == ATR_PLACE_NO(a1))		//??
				d0 = 1;
			else
				d0 = 0;
			//ACTION_INF??????
			ATR_GUARD_FLG(a1) = 0;
			//?????ＯＫ??
			if (ATR_PET_OK(a1))
			{
				a2 = p_party[ATR_PLACE_NO(a1) + 5];		//??????
				//??????????????????
				if (ATR_NAME(a2) != NULL && ATR_AKO_FLG(a2) == 0)
				{
					//?????
					if (ATR_ATTACK_KIND(2, a2) == 1)
					{
						ATR_ATTRIB(a2) |= ACT_ATR_HIDE;		//??
						//????
						ATR_NAME(a2) = NULL;
						ATR_VCT_NO(a2) = VCT_NO_APPEAR;
					}
					else
					{
						//ACTION_INF??????
						ATR_GUARD_FLG(a2) = 0;
						ATR_BODY_CNT(a0)++;		//??
						if (d0 == 1)		//?????
							d0 = 2;
					}
				}
			}
			//ACTION_INF
			switch (d0)
			{
			case 1:
				ATR_GUARD_FLG(a1) = 1;
				break;
			case 2:
				ATR_GUARD_FLG(a2) = 1;
				ATR_GUARD_FLG(a1) = 1;
				break;
			}
			break;
		case ATT_SELECT:		//??????????  ???  ??????????
			ATR_BODY_CNT(a0) = 1;
			d0 = ATR_PLACE_NO(a1) + 5;		//?????????
			a2 = p_party[ATR_PLACE_NO(a1) + 5];		//?????????
			ATR_BODY_WORK(0, a2) = a1;		//????????
			ATR_FIRST_FLG(a2) = 0;		//??????
			if (get_num() == 0)		//????
				ATR_VCT_NO(a2) = 60;		//?????
			else
			{	//???
				ATR_NAME(a2) = monster;		//????
				ATR_VCT_NO(a2) = 62;		//
				if (BattleMyNo == ATR_PLACE_NO(a1))		//??
					att_select_flg = TRUE;
			}
			break;
			//#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
		case ATT_PROSKILL:
			ATR_VCT_NO(a1) = PROSKILL;					//勇士职业技能
			ATR_BODY_WORK(0, a1) = p_party[get_num()];	//取出被攻击对象No
			ATR_ATTACK_KIND(0, a1) = get_num();			//攻击种类
			ATR_ATTACK_POW(0, a1) = get_num();			//攻击力
			if (BattleCmd[command_point] == 'p')
				ATR_ATTACK_PET_POW(0, a1) = get_num();		//pet??????
			else
				LogToBattleError(BattleCmd, __LINE__);

			//修正技能特效 xiezi
#ifdef _ATTACK_EFFECT
			if (BattleCmd[command_point] == 's')
				ATR_LONG_WORK(0, a1) = get_num();	// 记录特效编号
#endif

			ATR_WARRIOR_EFFECT(a1) = get_num();
			AttPreMagicNum = get_num();
			AttNextMagicNum = get_num();
			ATR_BODY_CNT(a0) = 1;
#ifdef _ATTDOUBLE_ATTACK
			//andy_add
			if (ATR_ATTACK_KIND(0, a1) & ATT_ATTDOUBLE || ATR_ATTACK_KIND(0, a1) & BCF_MODIFY)
				ATR_BATTLEGRANO(a1) = get_num();
#endif
			break;
			//#endif
		default:	//???????
			action_inf = -1;
			//?????????????
#ifdef _STONDEBUG_
			{
				char errbuf[256];

				sprintf_s(errbuf, "errbuf:%c", command_no);
				MessageBoxNew(hWnd, errbuf, "Error", MB_OK);
			}
#endif
			command_point = 0;
			break;
		}
#ifdef _PET_ITEM
		if (ATR_ATTACK_KIND(0, a1) & ATT_ATTACKBACK)
		{
			ATR_ATTACK_POW(1, a1) = ATR_ATTACK_POW(0, a1) >> 16;
			ATR_ATTACK_POW(0, a1) = ATR_ATTACK_POW(0, a1) & 0x0000ffff;
		}
#endif
#ifdef _FIX_ACUPUNCTURE
		if (ATR_ATTACK_KIND(0, a1) & ATT_ACUPUNCTURE)
		{
			ATR_ATTACK_POW(1, a1) = ATR_ATTACK_POW(0, a1) >> 16;
			ATR_ATTACK_POW(0, a1) = ATR_ATTACK_POW(0, a1) & 0x0000ffff;
		}
#endif
	case 1:		//⺳??
#ifdef __ATTACK_MAGIC
		if (TRUE == bRunAttMgc)
			RunTimeMagic();
#endif
#ifdef __TOCALL_MAGIC
		if (TRUE == bRunToCallMgc)
			RunTimeMagicToCall();
#endif
#ifdef _PROFESSION_ADDSKILL
		//		RunTimeMagicBoundary();
#endif
		if (ATR_DAMAGE(a0) == ATR_BODY_CNT(a0))
		{	//????
			for (d7 = 0; d7 < BATTLKPKPLYAERNUM; d7++)
			{
				if (ATR_NAME(p_party[d7]) != NULL)
				{
					//??????????
					if (ATR_LIFE(p_party[d7]) > 0 && ATR_VCT_NO(p_party[d7]) != 0 && ATR_VCT_NO(p_party[d7]) < VCT_NO_DIE)
						break;
				}
			}
			if (d7 == BATTLKPKPLYAERNUM)		//???
				ATR_VCT_NO(a0) = 0;		//⺳??
		}
		break;
	case 2:		//䴓
		for (d7 = 0; d7 < BATTLKPKPLYAERNUM; d7++)
		{
			if (ATR_NAME(p_party[d7]) != NULL)
			{
				//??????
				if (ATR_VCT_NO(p_party[d7]) != 0 && ATR_VCT_NO(p_party[d7]) != VCT_NO_DIE + 2)
					break;
			}
		}
		if (d7 == BATTLKPKPLYAERNUM)
		{	//???
			action_inf = 2;
			command_point = 0;		//?????????????
		}
		break;
	case 3:		//?
		for (d7 = 0; d7 < BATTLKPKPLYAERNUM; d7++)
		{
			if (ATR_NAME(p_party[d7]) != NULL)
			{
				if (ATR_VCT_NO(p_party[d7]) != 0 && ATR_VCT_NO(p_party[d7]) != VCT_NO_DIE + 2)		//??????????
					break;
			}
		}
		if (d7 == BATTLKPKPLYAERNUM)
		{
			ATR_VCT_NO(a0) = 0;
			action_inf = 3;		//??
			if (BattleBpFlag & BATTLE_BP_JOIN)// ??
			{
				//䴓??
				if (check_all_dead())
					action_inf = 2;
			}
		}
		break;
	case 4:		//?????
		for (d7 = 0; d7 < BATTLKPKPLYAERNUM; d7++)
		{
			if (ATR_NAME(p_party[d7]) != NULL)
			{
				//䴓??
				if (check_all_dead())
				{
					//??????????
					if (ATR_VCT_NO(p_party[d7]) != 0 && ATR_VCT_NO(p_party[d7]) != VCT_NO_DIE + 2)
						break;
				}
				else
				{
					//?????????????
					if (BattleMyNo == d7)
					{
						if (ATR_VCT_NO(p_party[d7]) == 55)
							break;
					}
					//??????????
					if (ATR_LIFE(p_party[d7]) > 0 && ATR_VCT_NO(p_party[d7]) != 0 && ATR_VCT_NO(p_party[d7]) != VCT_NO_DIE + 2)
						break;
				}
			}
		}
		if (d7 == BATTLKPKPLYAERNUM)
		{
			ATR_VCT_NO(a0) = 0;
			//⺶??
			if (!action_inf)
				action_inf = 1;		//????
			//䴓??
			if (check_all_dead())
				action_inf = 2;
		}
		break;
	}
master_500:
	//???????????
#ifdef __ATTACK_MAGIC
	if ((BattleMapNo >= 148 && BattleMapNo <= 150) || g_iRunEarthQuake >= 2)
#else
	if (BattleMapNo >= 148 && BattleMapNo <= 150)		//???????????
#endif
	{
		//????
		for (d7 = 0; d7 < BATTLKPKPLYAERNUM; d7++)
		{
			a1 = p_party[d7];		//???????
			if (ATR_NAME(a1) == NULL)		//???
				continue;
			ATR_V_POS(a1) += ATR_INT_WORK3(a1);
			ATR_INT_WORK1(a1) += ATR_INT_WORK3(a1);
			//????????????
			set_raster_pos(a1);
		}
		if (g_iRunEarthQuake == 3)
			g_iRunEarthQuake = 0;
	}
	//?????????
	d6 = 0;
	for (d7 = 0; d7 < BATTLKPKPLYAERNUM; d7++)
	{
		a1 = p_party[d7];		//???????		
		if (ATR_NAME(a1) == NULL)		//???
			continue;
		sort_chr_buf[d6].work = a1;		//???????
		sort_chr_buf[d6++].v_pos = ATR_V_POS(a1);		//?????
	}
	for (d7 = 0; d7 < BATTLKPKPLYAERNUM; d7++)
	{
		if ((a1 = p_missile[d7]) == NULL)		//???
			continue;
		sort_chr_buf[d6].work = a1;		//???????
		sort_chr_buf[d6++].v_pos = ATR_V_POS(a1);		//?????
	}
	//???????
	qsort(sort_chr_buf,			// ????
		d6,						// ?????
		sizeof(SORT_CHR_EQU),	// ???
		(CMPFUNC*)sort_chr		// ?????????
		);
	d0 = 30;		//????
	for (d7 = 0; d7 < d6; d7++)
		ATR_DISP_PRIO(sort_chr_buf[d7].work) = d0++;
}

//宠物行动处理
void monster(ACTION *a0)
{
	int d0, d1, d6, d7, sav_command_point, idx;
	ACTION *a1, *a2, *a3;
	static ACTION *a0tmp[10];
	static int a0mark[10];				// 0:已将action release 1:未
	static int a0tmpcount;

	switch (ATR_VCT_NO(a0))
	{
	case 0:		//待机
		ATR_CHR_ACT(a0) = ANIM_STAND;		//???????
		if (ATR_VCT_NO(p_master) == 1)
		{
			a1 = ATR_COUNTER(p_master);		//?????????
			if (a1 != NULL && ATR_GROUP_FLG(a0) != ATR_GROUP_FLG(a1) && ATR_NAME(a1) != NULL)
			{
				//?䴘??
				if (ATR_H_POS(a1) < lpDraw->xSize && ATR_H_POS(a1) > 64 && ATR_V_POS(a1) < lpDraw->ySize && ATR_V_POS(a1) > 0 - 64)
				{
					if (ATR_STATUS(a1) <= JUJUTSU_1)		//?????????
					{
						a1 = ATR_COUNTER(p_master);		//????????
						d0 = ATR_H_POS(a1);				//???
						d1 = ATR_V_POS(a1);
						radar2(a0, d0, d1, 2);	//????
						//????????
						d0 = ATR_CRS(a0) - ATR_CRS_OLD(a0);
						if (d0 < 0)
							d0 = 0 - d0;
						if (d0 >= 2)
						{
							ATR_CHR_ANG(a0) = crs_change_tbl[ATR_CRS(a0)];		/* ????? */
							ATR_CRS_OLD(a0) = ATR_CRS(a0);
						}
					}
				}
			}
		}
		//䞍??????????
		d0 = ATR_STATUS(a0);
		if (ATR_STATUS(a0) != 3 && ATR_STATUS(a0) != 4)
		{
			//???????????
			if (BattleMyNo >= 20 || (ATR_ATTRIB(a0) & ACT_ATR_BTL_CMD_END)
#ifdef _PETSKILL_BATTLE_MODEL
				|| ATR_INT_WORK0(a0) == ATT_BATTLE_MODEL
#endif
				){
				pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
			}
			//这里将图片重新加载和渲染 wxy
			//pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		}
#ifdef _PETSKILL_BATTLE_MODEL
		// 检查换我攻击了没
		if (ATR_INT_WORK0(a0) == ATT_BATTLE_MODEL)
		{
			a1 = ATR_BODY_WORK(0, a0);	// 取出被攻击目标
			// 照先后顺序执行
			if (ATR_LONG_WORK(ATR_PLACE_NO(a0), a1) == ATR_INT_WORK2(p_master))
			{
				// 当目标回到待机时再攻击
				if (ATR_VCT_NO(a1) == 0)
					ATR_VCT_NO(a0) = 2; // 开始攻击
				// 当目标离开战场或死亡时结束攻击
				if (ATR_VCT_NO(a1) == VCT_NO_APPEAR || ATR_LIFE(a1) <= 0)
				{
					ATR_VCT_NO(a0) = 3; // 结束攻击
					ATR_BODY_WORK(0, a0) = NULL;	// 清空
				}
			}
		}
#endif
		break;
	case 1:		//前进
		ATR_SPD(a0) = 32;		//移动速度
		a1 = ATR_BODY_WORK(0, a0);		//敌方的位置
		d0 = ATR_H_POS(a1);
		d1 = ATR_V_POS(a1);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		//????????
		d0 = ATR_CRS(a0) - ATR_CRS_OLD(a0);
		if (d0 < 0)
			d0 = 0 - d0;
		if (d0 >= 2)
		{
			ATR_CHR_ANG(a0) = crs_change_tbl[ATR_CRS(a0)];		/* ????? */
			ATR_CRS_OLD(a0) = ATR_CRS(a0);
		}
		if (d1 <= 32 * 2)
		{	//到达
#ifdef _PIRATE_ANM			
			if (ATR_CHR_NO(a0) == 101491)
				ATR_CHR_NO(a0) = 101490;
#endif
			ATR_FIRST_FLG(a0) = 0;		//????????
			ATR_HIT_STOP(a0) = 0;		//??????????
#ifdef _PETSKILL_BATTLE_MODEL
			if (ATR_INT_WORK0(a0) == ATT_BATTLE_MODEL)
				ATR_VCT_NO(a0) = 0; // 到达之后先待机
			else
#endif
			{
				if (ATR_ATTACK_KIND(0, a0) & ATT_VICARIOUS)
				{	//??????
					ATR_VCT_NO(a0) = 65;		//???????
					ATR_BODY_WORK(1, a0) = ATR_BODY_WORK(0, a0);		//????
					a1 = ATR_BODY_WORK(0, a0);	//????????
					ATR_BODY_WORK(1, a1) = a0;	//
					a1 = p_party[get_num()];	//??????????
#ifdef _ATTACK_EFFECT
					//修正忠犬  xiezi
					if (BattleCmd[command_point] == 's')
						ATR_LONG_WORK(0, a0) = get_num();	// 记录特效编号
#endif
					ATR_VCT_NO(a1) = 69;		//???????
					ATR_FIRST_FLG(a1) = 0;		//
					ATR_BODY_WORK(0, a1) = ATR_BODY_WORK(0, a0);		//???????
				}
				else if (ATR_SYNC_FLG(a0))		//?????
				{
					ATR_VCT_NO(a0) = 19;		//?????
					a1 = ATR_BODY_WORK(0, a0);		//????
					ATR_LONG_WORK(0, a1)--;		//???
				}
				else	//andy_bug
					ATR_VCT_NO(a0) = 2;		//???
			}
		}
		else
		{
#ifndef _PIRATE_ANM			
			gemini(a0);		//?
#else
			if (ATR_CHR_NO(a0) != 101490)
				gemini(a0);		//?
#endif
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
#ifndef _PIRATE_ANM
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);	
#else		
		if (ATR_CHR_NO(a0) == 101490)
		{
			pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP);
			if (a0->bmpNo == 301137)
				ATR_CHR_NO(a0) = 101491;
		}
		else
		{
			pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
			pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		}
#endif
		break;
	case 2:		//攻击
		if (ATR_HIT_STOP(a0))			//??????????
		{
			if (--ATR_HIT_STOP(a0))		//??????????
				break;
		}
		a1 = ATR_BODY_WORK(0, a0);
#ifdef _PETSKILL_BATTLE_MODEL
		if (ATR_INT_WORK0(a0) == ATT_BATTLE_MODEL && ATR_FIRST_FLG(a0) == 0 && ATR_INT_WORK3(a0) == 0)
		{
			if (ATR_ATTACK_KIND(0, a0) & ATT_VICARIOUS)			// 忠犬
			{
				ATR_VCT_NO(a0) = 65;							// 攻击物件先进行等待
				ATR_BODY_WORK(1, a1) = a0;						// 把攻击物件记录到主人身上,之后会用到
				a1 = p_party[ATR_LONG_WORK(0, a0)];				// 取出宠物的 action
				ATR_VCT_NO(a1) = 69;							// 显示 damage 的字
				ATR_FIRST_FLG(a1) = 0;
				ATR_BODY_WORK(0, a1) = ATR_BODY_WORK(0, a0);	// 宠物的前进目标是主人
				ATR_INT_WORK3(a0) = 1;
				break;
			}
			ATR_SYNC_FLG(a1) = 1;
		}
#endif
		if (ATR_FIRST_FLG(a0) == 0)			//???????
		{
			ATR_HIT(a0) = 0;				//?????????
			ATR_HIT_TIMING(a0) = 0;			//??????????

			d0 = ATR_H_POS(a1);				//???
			d1 = ATR_V_POS(a1);
			radar(a0, &d0, &d1);	//????
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = d0;		//???
			ATR_CHR_ANG(a0) = crs_change_tbl[d0];		/* ????? */
			//??????????
			if (ATR_ATTACK_KIND(0, a0) & ATT_G_CRASH)
				//????????
				set_damage_num(a0, 12, -64 + 16);
			else if (ATR_ATTACK_KIND(0, a0) & ATT_TOOEH)
				set_damage_num(a0, 29, -64 + 16);
#ifdef _ATTDOUBLE_ATTACK
			else if (ATR_ATTACK_KIND(0, a0) & ATT_ATTDOUBLE)
			{
				int skill = ATR_BATTLEGRANO(a0);
				switch (skill)
				{
				case 70:
					set_damage_num(a0, 25, -64 + 16);
					break;
				case 71:
					set_damage_num(a0, 26, -64 + 16);
					break;
				case 72:
					set_damage_num(a0, 27, -64 + 16);
					break;
				case 73:
					set_damage_num(a0, 28, -64 + 16);
					break;
				}
			}
			else if (ATR_ATTACK_KIND(0, a0) & BCF_MODIFY)
			{
				int skill = ATR_BATTLEGRANO(a0);

				switch (skill)
				{
				case 74://74
					set_damage_num(a0, 32, -64 + 16);
					break;
				case 75://75
					set_damage_num(a0, 33, -64 + 16);
					break;
				case 76://76
					set_damage_num(a0, 34, -64 + 16);
					break;
				case 77://77
					set_damage_num(a0, 35, -64 + 16);
					break;
				}
			}
#endif
#ifdef _SKILL_ROAR  //宠技:大吼(克年兽)
			else if (ATR_ATTACK_KIND(0, a0) & ATT_ATTROAR)
			{
				set_damage_num(a0, 22, -64 + 16);
				ATR_ROAR(a1) = 2;
			}
#endif
#ifdef _SKILL_SELFEXPLODE //自爆
			else if (ATR_ATTACK_KIND(0, a0) & ATT_ATTSELFEXPLODE)
			{
				ATR_SELFEXPLODE(a0) = 1;
				set_damage_num(a0, 23, -64 + 16);
			}
#endif
#ifdef _PETSKILL_EXPLODE //爆弹
			else if (ATR_ATTACK_KIND(0, a0) & ATT_EXPLODE)
			{
				//ATR_SELFEXPLODE(a0) = 1;
				ATR_LIFE(a0) = 1;
				set_damage_num(a0, 23, -64 + 16);
			}
#endif
			else
			{
				//andy_fall
				if (ATR_ATTACK_KIND(0, a0) & ATT_FALL)
				{
					if (ATR_RIDE(a1) == 1)
					{
						//set_damage_num(a0, 12, -64+16); //落马术暂不设图
						ATR_PETFALL(a1) = 1;
					}
				}
			}
			//#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
			if (ATR_ATTACK_KIND(0, a0) & ATT_ATTPREPROSKILL)
				set_damage_num(a1, 41, -64 + 25);
			//#endif
			//????????
			if (ATR_SYNC_FLG(a0))
				d0 = 0;
			else
			{
#ifdef _PETSKILL_BATTLE_MODEL
				if (ATR_INT_WORK0(a0) == ATT_BATTLE_MODEL)
					d0 = 0;
				else
#endif
					d0 = get_next_flg();		//??????????
			}
			if (d0 & ATT_COUNTER)			//??????????
			{
				ATR_COUNTER_FLG(a0) = 1;
				ATR_COUNTER_FLG(a1) = 1;
				ATR_STIMER(a0) = 255;
			}
			else
			{
				ATR_COUNTER_FLG(a0) = 0;
				ATR_COUNTER_FLG(a1) = 0;
				ATR_STIMER(a0) = 20;
			}
			ATR_FIRST_FLG(a0) = 1;		//??????
		}
		//ATT_AKO1				
#ifdef _PETSKILL_ANTINTER
		if (ATR_VCT_NO(a1) >= VCT_NO_DIE && (ATR_ATTACK_KIND(0, a1) & (1 << 28)))		//???
#else
		if (ATR_VCT_NO(a1) >= VCT_NO_DIE)		//???
#endif
		{
			if (ATR_ATTACK_KIND(0, a0) & ATT_COUNTER && ATR_COUNTER(p_master) != a0)		//???????
				ATR_VCT_NO(a0) = 0;		//??
			else
			{
				ATR_VCT_NO(a0) = 3;
				ATR_STIMER(a0) = 20;
#ifdef _PETSKILL_BATTLE_MODEL
				// 攻击完毕
				if (ATR_INT_WORK0(a0) == ATT_BATTLE_MODEL)
					ATR_LONG_WORK(ATR_PLACE_NO(a0), a1) = 0;
#endif
			}
			break;
		}
#ifdef _SKILL_SELFEXPLODE //自爆
		if (ATR_SELFEXPLODE(a0) == 1)
		{
			ATR_ATTRIB(a0) |= ACT_ATR_HIDE;		//??
			ATR_CHR_ACT(a0) = ANIM_ATTACK;
		}
		else
#endif
			ATR_CHR_ACT(a0) = ANIM_ATTACK;		//?????????
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{	//????????
			if (ATR_ATTACK_KIND(0, a0) & ATT_COUNTER && ATR_COUNTER(p_master) != a0)		//???????
				ATR_VCT_NO(a0) = 0;		//??
			else
			{
				ATR_VCT_NO(a0) = 3;
#ifdef _PETSKILL_BATTLE_MODEL
				// 攻击完毕
				if (ATR_INT_WORK0(a0) == ATT_BATTLE_MODEL)
					ATR_LONG_WORK(ATR_PLACE_NO(a0), a1) = 0;
#endif
			}
			break;
		}
		//???????
		if (ATR_HIT(a0))
		{
#ifdef _ATTACK_EFFECT
			if (ATR_LONG_WORK(0, a0) > 0)	// 放特效
			{
				SetDisplayAttackEffect(a0, ATR_LONG_WORK(0, a0));
				ATR_LONG_WORK(0, a0) = 0;
			}
#endif
			//andy_mp
			if (ATR_SHOWADDHP(a0) == 1)
			{
				set_damage_num(a0, 6, -64);
				ATR_SHOWADDHP(a0) = 0;
				ATR_LIFE(a0) += ATR_ADDHP(a0);
			}
			ATR_ATTACK_KIND(1, a1) = ATR_ATTACK_KIND(0, a0) & ATT_DEATH;
#ifdef _PET_ITEM
			if (ATR_ATTACK_KIND(0, a0) & ATT_ATTACKBACK)
				ATR_ATTACK_KIND(1, a1) = 0;
#endif
#ifdef _PETSKILL_ACUPUNCTURE
			//////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_TRAP | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
				| ATT_ATTACKBACK
#endif
				))
#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA  | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
				| ATT_ATTACKBACK
#endif
				))
#endif
				//////////////////////
#else
			//////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_TRAP))
#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA))
#endif
				//////////////////////
#endif
			{
				set_guard_mark(a0);
#ifdef _PETSKILL_ACUPUNCTURE
				if (ATR_ATTACK_KIND(0, a0) & (ATT_ACUPUNCTURE
#ifdef _PET_ITEM
					| ATT_ATTACKBACK
#endif
					))
				{
#ifdef _PETSKILL_BATTLE_MODEL
					if (ATR_INT_WORK0(a0) != ATT_BATTLE_MODEL)
#endif
					{
						ATR_VCT_NO(a1) = 10; //设定自己受伤
						ATR_DAMAGE(a1) = ATR_DAMAGE(a0);//ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0)*2;
						//ATR_ATTACK_KIND(0, a1) &= ATT_AKO2;
						if ((float)ATR_DAMAGE(a1) >= (float)(ATR_MAX_LIFE(a1) * 1.2 + 20.0))
						{
							ATR_LIFE(a1) = 0;
							ATR_AKO_FLG(a1) = 2; //打飞
						}
						a1 = ATR_BODY_WORK(0, a0) = a0;
					}
				}
#endif
				if (ATR_ATTACK_KIND(0, a0) & ATT_VICARIOUS)
				{
					a1 = ATR_BODY_WORK(0, a0);
					ATR_VCT_NO(a1) = 6;
					ATR_STIMER(a1) = 60;
				}
			}
#ifdef _PETSKILL_ACUPUNCTURE
			///////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
				| ATT_ATTACKBACK
#endif
				))
#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
				| ATT_ATTACKBACK
#endif
				))
#endif
				///////////////////
#else
			///////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP) )
#else
			if (ATR_ATTACK_KIND(0, a0) & ATT_REFLEX)
#endif
				///////////////////
#endif
			{
#ifdef _PETSKILL_BATTLE_MODEL
				// 检查是不是攻击物件
				if (ATR_INT_WORK0(a0) != ATT_BATTLE_MODEL)
#endif
					a1 = ATR_BODY_WORK(0, a0) = a0;
			}
			else
				a1 = ATR_BODY_WORK(0, a0);		//????????xxx
			//?????????
			ATR_COMBO(a1) = 0;
			//???????
			if (ATR_ATTACK_KIND(0, a0) & ATT_DODGE)
			{
				//???????????
				if (ATR_HIT_TIMING(a0) == 0)
				{
					ATR_HIT_TIMING(a0) = 1;		//??????????
					ATR_VCT_NO(a1) = 16;		//????
					ATR_CRS(a1) = ATR_CRS(a0);		//?????????????
					ATR_DAMAGE_ANG(a1) = ATR_CHR_ANG(a0);		//
					ATR_STIMER(a1) = 0;
					ATR_FIRST_FLG(a1) = 1;		//?????????
					ATR_BODY_WORK(0, a1) = a0;	//
					if (ATR_ATTACK_KIND(0, a0) & ATT_COUNTER)		//???????
						//?????
						set_damage_num(a0, 1, -64 + 16);
				}
				ATR_HIT(a0) = 0;
				play_se(8, ATR_H_POS(a0), ATR_V_POS(a0));
				break;
			}

			//????????
			if (ATR_HIT(a0) >= 10000 && ATR_HIT(a0) < 10100)
			{
				//?????????
				if (ATR_ATTACK_KIND(0, a0) & ATT_CRUSH)
					//?????
					set_damage_num(a1, 19, -112);
			}
			//?????????//knock out
			ATR_AKO_FLG(a1) = 0;
			if (ATR_ATTACK_KIND(0, a0) & ATT_AKO1)		//??????
				ATR_AKO_FLG(a1) = 1;
			if (ATR_ATTACK_KIND(0, a0) & ATT_AKO2)		//??????
				ATR_AKO_FLG(a1) = 2;
			//连击 ?????
			if (ATR_HIT(a0) >= 10100)
			{
#ifdef _PETSKILL_ACUPUNCTURE
				///////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
				if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
					| ATT_ATTACKBACK
#endif
					))
#else
				if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
					| ATT_ATTACKBACK
#endif
					))
#endif
					///////////////////////
#else
				///////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
				if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP) )
#else

				if (ATR_ATTACK_KIND(0, a0) & ATT_REFLEX)
#endif
					///////////////////////
#endif
				{
					ATR_HIT(a0) -= 100;
					if (ATR_SYNC_FLG(a0) == 1)
					{
						a2 = ATR_BODY_WORK(1, a0);
						ATR_VCT_NO(a2) = 2;
						ATR_COUNTER(p_master) = a2;
					}
				}
				else
					//?????????
					ATR_COMBO(a1) = 1;
			}
			else if (ATR_SYNC_FLG(a0) == 1)
			{
#ifdef _PETSKILL_ACUPUNCTURE
				//////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
				if (!(ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
					| ATT_ATTACKBACK
#endif
					)))
#else
				if (!(ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
					| ATT_ATTACKBACK
#endif
					)))
#endif
					/////////////////////
#else
				//////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
				if (!(ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP) ))
#else
				if (!(ATR_ATTACK_KIND(0, a0) & ATT_REFLEX))
#endif
					/////////////////////
#endif
					ATR_COMBO(a1) = 1;
				a2 = ATR_BODY_WORK(1, a0);		//??????
				ATR_VCT_NO(a2) = 2;		//???
				ATR_COUNTER(p_master) = a2;		//??????
			}
#ifdef _PETSKILL_ACUPUNCTURE
			/////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
				| ATT_ATTACKBACK
#endif
				))
#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
				| ATT_ATTACKBACK
#endif
				))
#endif
				////////////////////
#else
			/////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP) )
#else
			if (ATR_ATTACK_KIND(0, a0) & ATT_REFLEX)
#endif
				////////////////////
#endif
			{
#ifdef _PETSKILL_BATTLE_MODEL
				// 检查是不是攻击物件
				if (ATR_INT_WORK0(a0) == ATT_BATTLE_MODEL)
				{
					ATR_ATTACK_POW(0, a0) = 0;		// 无伤害
					ATR_DAMAGE_ANG(a1) = ATR_CHR_ANG(a0);
					ATR_ATTACK_KIND(0, a0) |= ATT_NOMISS;
				}
				else
#endif
					ATR_DAMAGE_ANG(a1) = (ATR_CHR_ANG(a0) + 4) & 7;
			}
			else
				ATR_DAMAGE_ANG(a1) = ATR_CHR_ANG(a0);		//
			if (ATR_ATTACK_KIND(0, a0) & BCF_DEFMAGICATT)
				set_damage_num(a1, 30, -64);
			if (ATR_ATTACK_KIND(0, a0) & BCF_SUPERWALL)
				set_damage_num(a1, 31, -64);
			//#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
			if (ATR_ATTACK_KIND(0, a0) & ATT_ATTNEXTPROSKILL)
				set_damage_num(a1, 41, -64 + 25);
			//#endif
			if (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION)	//??????
			{
				if (ATR_SYNC_FLG(a0) == 0)		//????????
				{
					if (!ATR_COMBO(a1))			//????????
					{
						//??
#ifdef _SYUTEST
						ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
						ATR_LIFE(a1) -= ATR_DAMAGE(a1);
						if (Light1 == NULL)
							Light1 = MakeAnimDisp(ATR_H_POS(a0), ATR_V_POS(a0), 101581, 0);
						set_damage_num(a1, 41, -64 + 25);
						// 采光术预备
#else
						ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
						ATR_LIFE(a1) += ATR_DAMAGE(a1);
						if (ATR_LIFE(a1) > ATR_MAX_LIFE(a1))		//????
							ATR_LIFE(a1) = ATR_MAX_LIFE(a1);		//????
#endif
						ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
						ATR_PET_LIFE(a1) += ATR_PET_DAMAGE(a1);
						if (ATR_PET_LIFE(a1) > ATR_PET_MAX_LIFE(a1))		//????
							ATR_PET_LIFE(a1) = ATR_PET_MAX_LIFE(a1);		//????
						set_damage_num(a1, 14, -64);
						ATR_ATTACK_POW(0, a0) = 0;		//????䥺?
						ATR_ATTACK_PET_POW(0, a0) = 0;		//????䥺?
					}
				}
				else
				{
					//????????
					if (ATR_HIT(a0) >= 10000 && ATR_HIT(a0) < 10100)
					{
						//??
						ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
#ifdef _SYUTEST
						// 采光术预备
#endif
						ATR_LIFE(a1) += ATR_DAMAGE(a1);
						if (ATR_LIFE(a1) > ATR_MAX_LIFE(a1))		//????
							ATR_LIFE(a1) = ATR_MAX_LIFE(a1);		//????
						ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
						ATR_PET_LIFE(a1) += ATR_PET_DAMAGE(a1);
						if (ATR_PET_LIFE(a1) > ATR_PET_MAX_LIFE(a1))		//????
							ATR_PET_LIFE(a1) = ATR_PET_MAX_LIFE(a1);		//????
						set_damage_num(a1, 14, -64);
						ATR_ATTACK_POW(0, a0) = 0;		//????䥺?
						ATR_ATTACK_PET_POW(0, a0) = 0;		//????䥺?
					}
				}
			}
			else if (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA)		//????????
			{
				ATR_ATTACK_POW(0, a0) = 0;		//????䥺?
				ATR_ATTACK_PET_POW(0, a0) = 0;		//????䥺?
			}
			else
			{
				ATR_VCT_NO(a1) = 10;		//?????

#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
				if (ATR_ATTACK_KIND(0, a0) & ATT_NOMISS)
					NoMiss = 1 ; 
				else
					NoMiss = -1 ;
#endif
			}
			//??????
			if (ATR_ATTACK_KIND(0, a0) & ATT_SATISFACTORY)
				ATR_KAISHIN(a1) = 1;		//???????
			else
				ATR_KAISHIN(a1) = 0;		//?????
			ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
			ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
			//andy_mp
			ATR_MPDAMAGE(a1) = ATR_MPDAMAGE(a0);
			ATR_MPDFLG(a1) = ATR_MPDFLG(a0);
			//?????
			if (ATR_ATTACK_KIND(0, a0) & ATT_GUARD)
				ATR_GUARD_FLG(a1) = 1;
			else
				ATR_GUARD_FLG(a1) = 0;
#ifdef _EQUIT_ARRANGE
			if (ATR_ATTACK_KIND(0, a0) & ATT_ARRANGE)
			{
				set_damage_num(a1, 42, -64);
				ATR_CHR_ACT(a1) = ANIM_GUARD;
			}
#endif
			if (ATR_LIFE(a1) - ATR_DAMAGE(a1) <= 0 || (ATR_ATTACK_KIND(0, a0) & ATT_DEATH))
			{
				if (ATR_COMBO(a1) == 0)
					ATR_KAISHIN(a1) = 1;
			}
			if (ATR_KAISHIN(a1))
				ATR_HIT_STOP(a0) = HIT_STOP_TIM * 4;
			else
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;
			if (ATR_DAMAGE(a1) || (ATR_ATTACK_KIND(0, a0) & (ATT_BALLIA | ATT_ABSORPTION)))
			{
				if (ATR_ATTACK_KIND(0, a0) & ATT_COUNTER)		//???????
				{
					//????????
					if (ATR_COMBO(a1) == 0)
						//?????
						set_damage_num(a0, 1, -64 + 16);
				}
				//受伤效果音
				play_damage(ATR_HIT(a0), ATR_H_POS(a0));
				//必杀星星
				set_hit_mark(a0);
				if (ATR_COMBO(a1))		//小队
					ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//动作停止
			}
			ATR_HIT(a0) = 0;				//攻击旗标清除
			if (!(ATR_ATTACK_POW(0, a0) || ATR_ATTACK_PET_POW(0, a0)))		//????
			{
				ATR_HIT_STOP(a0) = 0;		//动作停止
				play_se(8, ATR_H_POS(a0), ATR_V_POS(a0));
			}
		}
		break;

	case 3:		//攻击等待
#ifdef _PETSKILL_BATTLE_MODEL
		// 检查是不是攻击物件
		if (ATR_INT_WORK0(a0) == ATT_BATTLE_MODEL)
		{
			// 检查此攻击物件还有没有目标
			if (ATR_INT_WORK1(a0) > 1)
			{
				// 找出下一个目标
				for (d0 = 1; d0 < ATR_INT_WORK1(a0); d0++)
				{
					if (ATR_BODY_WORK(d0, a0) != NULL)	// 找到目标
					{
						a1 = ATR_BODY_WORK(d0, a0);		// 取出目标
						ATR_BODY_WORK(d0, a0) = NULL;	// 清除
						if (ATR_LIFE(a1) <= 0)			// 检查目标有无死亡
							continue;
						if (ATR_VCT_NO(a1) == VCT_NO_APPEAR)	// 目标离开战场
							continue;
						else
						{
							ATR_INT_WORK3(a0) = 0;
							ATR_BODY_WORK(0, a0) = a1;								// 被攻方
							ATR_ATTACK_KIND(0, a0) = ATR_ATTACK_KIND(d0, a0);		// 攻击种类
							ATR_ATTACK_POW(0, a0) = ATR_ATTACK_POW(d0, a0);			// 攻击力
							ATR_ATTACK_PET_POW(0, a0) = ATR_ATTACK_PET_POW(d0, a0);	// pet攻击力
							ATR_LONG_WORK(0, a0) = ATR_LONG_WORK(d0, a0);			// 设定使用忠犬的目标
							ATR_VCT_NO(a0) = 1;										// 前进下一个目标
							ATR_INT_WORK2(p_master)++;								// 换下一个攻击物件攻击
							ATR_INT_WORK1(a0)--;
							break;
						}
					}
				}
				if (d0 == ATR_INT_WORK1(a0))
					ATR_INT_WORK1(a0) = 0;
			}
			// 没目标了,离开
			else
			{
				// 设定离开
				if (ATR_INT_WORK2(a0) == 0)	// 第一次执行
				{
					ATR_SPD(a0) = 16;
					// 往左上离开
					if (ATR_GROUP_FLG(a0) == 0)
						ATR_CHR_ANG(a0) = 3;
					else if (ATR_GROUP_FLG(a0) == 1)
						ATR_CHR_ANG(a0) = 7;
#ifdef _BATTLE_PK_PLAYER_FOR_40
					else if (ATR_GROUP_FLG(a0) == 2)
						ATR_CHR_ANG(a0) = 5;
					else if (ATR_GROUP_FLG(a0) == 3)	
						ATR_CHR_ANG(a0) = 1;
#endif
					ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
					ATR_INT_WORK2(a0) = 1;
					ATR_INT_WORK2(p_master)++;	// 换下一个攻击物件攻击
				}
				gemini(a0);		// 移动
				ATR_CHR_ACT(a0) = ANIM_WALK;
				pattern(a0, 1, ANM_LOOP);
				// 移动超出荧幕
				if (ATR_H_POS(a0) > lpDraw->xSize + SCREEN_OUT || ATR_H_POS(a0) < 0 - SCREEN_OUT)
				{
					DeathAction(a0);
					p_missile[ATR_PLACE_NO(a0)] = NULL;
					// 检查是否所有的 p_missile 都清空了
					for (d7 = 0; d7 < BATTLKPKPLYAERNUM; d7++)
					{
						if (p_missile[d7] != NULL)
							break;
					}
					if (d7 >= BATTLKPKPLYAERNUM)
						ATR_INT_WORK1(p_master) = 1;	// 所有的攻击物件都行动完了
				}
				// 所有的攻击物件都行动完了
				if (ATR_INT_WORK1(p_master))
				{
					ATR_DAMAGE(p_master) = 0;
					ATR_BODY_CNT(p_master) = 1;
					if (ATR_BODY_WORK(0, a0) == NULL)	// 这个蛋的目标不在场上或已死亡
						ATR_DAMAGE(p_master)++;
					else // 如果最后一个攻击物件打的目标的状态是待机或死亡
					if (ATR_VCT_NO(ATR_BODY_WORK(0, a0)) == 0 || ATR_LIFE(ATR_BODY_WORK(0, a0)) <= 0)
						ATR_DAMAGE(p_master)++;
				}
			}
			break;
		}
#endif
		if (--ATR_STIMER(a0))		//?????
			break;
		if (ATR_SYNC_FLG(a0))
		{	//?????
			d0 = ATR_INT_WORK0(a0);
			d1 = ATR_INT_WORK1(a0);
			radar(a0, &d0, &d1);	//????
			ATR_CHR_ANG(a0) = crs_change_tbl[d0];
			ATR_VCT_NO(a0) = 4;		//??
			ATR_SPD(a0) = 32;
			break;
		}
		d0 = get_num();		//?????
		if (d0 == 255)
		{	//???
			d0 = ATR_INT_WORK0(a0);
			d1 = ATR_INT_WORK1(a0);
			radar(a0, &d0, &d1);	//????
			ATR_CHR_ANG(a0) = crs_change_tbl[d0];
#ifdef _SKILL_SELFEXPLODE //自爆
			if (ATR_SELFEXPLODE(a0) == 1)
				ATR_VCT_NO(a0) = 114;
			else
#endif
				ATR_VCT_NO(a0) = 4;		//??
			ATR_SPD(a0) = 32;
			break;
		}
		if (ATR_ATTACK_KIND(0, a0) & ATT_VICARIOUS)		//?????????
			a2 = ATR_BODY_WORK(1, a0);
		else
			a2 = ATR_BODY_WORK(0, a0);		//??
		a1 = ATR_BODY_WORK(0, a0) = p_party[d0];		//????

		ATR_ATTACK_KIND(0, a0) = get_num();		//??????
		ATR_ATTACK_POW(0, a0) = get_num();		//??????
#ifdef _PET_ITEM
		if (ATR_ATTACK_KIND(0, a0) & ATT_ATTACKBACK)
		{
			ATR_ATTACK_POW(1, a0) = ATR_ATTACK_POW(0, a0) >> 16;
			ATR_ATTACK_POW(0, a0) = ATR_ATTACK_POW(0, a0) & 0x0000ffff;
		}
#endif
#ifdef _FIX_ACUPUNCTURE
		if (ATR_ATTACK_KIND(0, a0) & ATT_ACUPUNCTURE)
		{
			ATR_ATTACK_POW(1, a0) = ATR_ATTACK_POW(0, a0) >> 16;
			ATR_ATTACK_POW(0, a0) = ATR_ATTACK_POW(0, a0) & 0x0000ffff;
		}
#endif
		if (BattleCmd[command_point] == 'p')
			ATR_ATTACK_PET_POW(0, a0) = get_num();		//pet??????
		else
			LogToBattleError(BattleCmd, __LINE__);

#ifdef _ATTACK_EFFECT
		if (BattleCmd[command_point] == 's')
			ATR_LONG_WORK(0, a0) = get_num();	// 记录特效编号
#endif

		if (a1 == a2)		//?????
			ATR_VCT_NO(a0) = 5;		//???
		else
			ATR_VCT_NO(a0) = 1;		//??
		d0 = ATR_H_POS(a1);
		d1 = ATR_V_POS(a1);
		radar(a0, &d0, &d1);	//????
		ATR_CRS_OLD(a0) = ATR_CRS(a0) = d0;		//??????
		ATR_CHR_ANG(a0) = crs_change_tbl[d0];		/* ????? */
		break;
	case 5:		//???
		a1 = ATR_BODY_WORK(0, a0);
		if (ATR_VCT_NO(a1) == 0)		//????????
			ATR_VCT_NO(a0) = 1;		//??
		break;
	case 4:		//后退
		d0 = ATR_INT_WORK0(a0);
		d1 = ATR_INT_WORK1(a0);
		radar(a0, &d0, &d1);	//????
		ATR_CRS_OLD(a0) = ATR_CRS(a0) = d0;		//??????
#ifndef _PIRATE_ANM			
		gemini(a0);		//?
#else
		if (ATR_CHR_NO(a0) != 101490)
			gemini(a0);		//?
#endif			
		if (d1 <= (ATR_SPD(a0) >> 2))
		{	//????
			if (ATR_DAMAGE(p_master) != ATR_BODY_CNT(p_master))		//??????
				ATR_DAMAGE(p_master)++;		//??????
			if (ATR_GROUP_FLG(a0) == 0)
				ATR_CHR_ANG(a0) = 3;
			else if (ATR_GROUP_FLG(a0) == 1)
				ATR_CHR_ANG(a0) = 7;
#ifdef _BATTLE_PK_PLAYER_FOR_40
			else if (ATR_GROUP_FLG(a0) == 2)
				ATR_CHR_ANG(a0) = 5;
			else if (ATR_GROUP_FLG(a0) == 3)	
				ATR_CHR_ANG(a0) = 1;
#endif//
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
			ATR_H_POS(a0) = ATR_INT_WORK0(a0);		//?
			ATR_V_POS(a0) = ATR_INT_WORK1(a0);
#ifdef _PIRATE_ANM			
			if (ATR_CHR_NO(a0) == 101492)
				ATR_CHR_NO(a0) = 101490;
#endif
			ATR_VCT_NO(a0) = 0;		//??
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
#ifndef _PIRATE_ANM
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);	
#else		
		if (ATR_CHR_NO(a0) == 101490)
		{
			pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP);
			if (a0->bmpNo == 301026 || a0->bmpNo == 301138)
				ATR_CHR_NO(a0) = 101492;
		}
		else
		{
			pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
			pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		}
#endif	
		break;
	case 6:		//?㘎?
		if (--ATR_STIMER(a0))
			break;
		ATR_SPD(a0) = 10;
		ATR_VCT_NO(a0) = 14;		//????????
		ATR_FIRST_FLG(a0) = 0;		//??
		break;
	case 10:		//被攻击
		if (!ATR_DAMAGE(a0) && ATR_LIFE(a0) > 0 && !ATR_PET_DAMAGE(a0))	//????????
		{
			//????????
			if (!ATR_COMBO(a0))
			{
				//??
#ifdef _SKILL_ROAR  //宠技:大吼(克年兽)//tuen "miss"  off
				if (ATR_ROAR(a0) == 2){
					//	set_damage_num(a0, 18, -64);
				}
				else
				{
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
					if (NoMiss == 1);//set_damage_num(a0, 1, -64);
					else if (NoMiss == -1)
						set_damage_num(a0, 0, -64);
#else
					set_damage_num(a0, 0, -64);
#endif
				}
#else
				set_damage_num(a0, 0, -64);
#endif
				d0 = get_next_flg();		//??????????
				if (d0 & ATT_COUNTER)		//???????
					ATR_VCT_NO(a0) = 11;		//?????
				else
				{
					pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a0) = 0;		//??
				}
			}
			else
			{
				pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
				ATR_VCT_NO(a0) = 0;		//??
			}
			break;
		}
		if (ATR_GUARD_FLG(a0))
		{	//?????
			//????????
			if (!ATR_COMBO(a0))
				//???
				set_damage_num(a0, 3, -64 + 16);
			ATR_CHR_ACT(a0) = ANIM_GUARD;		//??????????//防
		}
		else
			ATR_CHR_ACT(a0) = ANIM_DAMAGE;		//???????????//受伤
		ATR_CHR_ANG(a0) = (ATR_DAMAGE_ANG(a0) + 4) & 7;
		//??????????????????
		if (!ATR_COMBO(a0) || ATR_CHR_ACT_OLD(a0) != ANIM_DAMAGE)
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_DAMAGE_ANG(a0)];
		pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP);
		ATR_VCT_NO(a0) = 11;
		//?????
		if (ATR_COMBO(a0))
			ATR_STIMER(a0) = 255;
		else
		{
			if (ATR_KAISHIN(a0))		//??????
				ATR_STIMER(a0) = HIT_STOP_TIM * 4;
			else
				ATR_STIMER(a0) = HIT_STOP_TIM;
		}
#ifdef _PETSKILL_ACUPUNCTURE
		/////////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
		if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
			| ATT_ATTACKBACK
#endif
			) && ATR_SYNC_FLG(a0) == 0)
#else		
		if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
			| ATT_ATTACKBACK
#endif
			) && ATR_SYNC_FLG(a0) == 0)
#endif
			/////////////////////////
#else
		/////////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
		if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP) && ATR_SYNC_FLG(a0) == 0)
#else		
		if (ATR_ATTACK_KIND(0, a0) & ATT_REFLEX && ATR_SYNC_FLG(a0) == 0)
#endif
			/////////////////////////
#endif
			ATR_STIMER(a0) = HIT_STOP_TIM * 4;//817333333
		ATR_SPD(a0) = 16;
#ifdef _MAGIC_DEEPPOISION   //剧毒
		if ((ATR_DEEPPOISION(a0) == 1))
			ATR_STIMER(a0) = 50;
#endif
		//????????
		if (!ATR_COMBO(a0))
		{
			//????
			set_damage_num(a0, 6, -64);
			ATR_LIFE(a0) -= ATR_DAMAGE(a0);		//?????
			//andy_mp
			ATR_MP(a0) -= ATR_MPDAMAGE(a0);
			ATR_PET_LIFE(a0) -= ATR_PET_DAMAGE(a0);
			//andy_fall
			if (ATR_PETFALL(a0) == 1)
			{
				if (ATR_LIFE(a0) > 0)
					petfallChangeGraph(a0);
			}
			if (ATR_PET_LIFE(a0) <= 0 && ATR_RIDE(a0))
			{	//???????
				ATR_PET_LIFE(a0) = 0;
				if (ATR_LIFE(a0) > 0)
					petfallChangeGraph(a0);
			}
			if (ATR_LIFE(a0) <= 0 || (ATR_ATTACK_KIND(1, a0) & ATT_DEATH))
			{	//???????
				ATR_LIFE(a0) = 0;
				slow_flg++;		//????
				flash_vct_no = 1;		//?????????
			}
		}
		break;
	case 11:		//?????
		if (ATR_DAMAGE(a0))
		{	//???????
			gemini(a0);		//??????
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = (ATR_CRS(a0) + 16) & 31;
			if (--ATR_STIMER(a0))		//?????
				break;
		}
#ifdef _PETSKILL_ACUPUNCTURE
		//////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
		if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
			| ATT_ATTACKBACK
#endif
			) && ATR_SYNC_FLG(a0) == 0 )
#else		
		if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
			| ATT_ATTACKBACK
#endif
			) && ATR_SYNC_FLG(a0) == 0)
#endif
			//////////////////////
#else
		//////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
		if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP) && ATR_SYNC_FLG(a0) == 0)
#else		
		if (ATR_ATTACK_KIND(0, a0) & ATT_REFLEX && ATR_SYNC_FLG(a0) == 0)
#endif
			//////////////////////
#endif
		{
			sav_command_point = command_point;		//??????????
			d0 = get_num();
			command_point = sav_command_point;		//????????????
			if (d0 != 255)
			{	//????
				a1 = ATR_BODY_WORK(0, a0) = p_party[get_num()];		//????
				ATR_ATTACK_KIND(0, a0) = get_num();		//??????
				ATR_ATTACK_POW(0, a0) = get_num();		//??????
#ifdef _PET_ITEM
				if (ATR_ATTACK_KIND(0, a0) & ATT_ATTACKBACK)
				{
					ATR_ATTACK_POW(1, a0) = ATR_ATTACK_POW(0, a0) >> 16;
					ATR_ATTACK_POW(0, a0) = ATR_ATTACK_POW(0, a0) & 0x0000ffff;
				}
#endif
#ifdef _FIX_ACUPUNCTURE
				if (ATR_ATTACK_KIND(0, a0) & ATT_ACUPUNCTURE)
				{
					ATR_ATTACK_POW(1, a0) = ATR_ATTACK_POW(0, a0) >> 16;
					ATR_ATTACK_POW(0, a0) = ATR_ATTACK_POW(0, a0) & 0x0000ffff;
				}
#endif
				if (BattleCmd[command_point] == 'p')
					ATR_ATTACK_PET_POW(0, a0) = get_num();		//pet??????
				else
					LogToBattleError(BattleCmd, __LINE__);

				//修正战斗特效  xiezi
#ifdef _ATTACK_EFFECT
				if (BattleCmd[command_point] == 's')
					ATR_LONG_WORK(0, a0) = get_num();	// 记录特效编号
#endif

				ATR_HIT_STOP(a0) = 0;		//??????????
				ATR_VCT_NO(a0) = 2;		//???
				ATR_FIRST_FLG(a0) = 0;		//????????
				ATR_CHR_ACT_OLD(a0) = -1;
				if (ATR_ATTACK_KIND(0, a0) & ATT_VICARIOUS)		//??????
					ATR_VCT_NO(a0) = 1;		//?
				break;
			}
		}
		if (ATR_COUNTER_FLG(a0))
		{	//???????//counter
			a1 = ATR_BODY_WORK(0, a0) = p_party[get_num()];		//????
			ATR_ATTACK_KIND(0, a0) = get_num();		//??????
			ATR_ATTACK_POW(0, a0) = get_num();		//??????
#ifdef _PET_ITEM
			if (ATR_ATTACK_KIND(0, a0) & ATT_ATTACKBACK)
			{
				ATR_ATTACK_POW(1, a0) = ATR_ATTACK_POW(0, a0) >> 16;
				ATR_ATTACK_POW(0, a0) = ATR_ATTACK_POW(0, a0) & 0x0000ffff;
			}
#endif
#ifdef _FIX_ACUPUNCTURE
			if (ATR_ATTACK_KIND(0, a0) & ATT_ACUPUNCTURE)
			{
				ATR_ATTACK_POW(1, a0) = ATR_ATTACK_POW(0, a0) >> 16;
				ATR_ATTACK_POW(0, a0) = ATR_ATTACK_POW(0, a0) & 0x0000ffff;
			}
#endif
			if (BattleCmd[command_point] == 'p')
				ATR_ATTACK_PET_POW(0, a0) = get_num();		//pet??????
			else
				LogToBattleError(BattleCmd, __LINE__);

			//修正技能特效 xiezi
#ifdef _ATTACK_EFFECT
			if (BattleCmd[command_point] == 's')
				ATR_LONG_WORK(0, a0) = get_num();	// 记录特效编号
#endif

			ATR_CHR_ANG(a0) = (ATR_DAMAGE_ANG(a0) + 4) & 7;
			pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
			ATR_BODY_CNT(a0) = 1;			//
			ATR_BODY_WORK(0, a0) = a1;		//?
			ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
			ATR_VCT_NO(a0) = 2;		//???
			ATR_FIRST_FLG(a0) = 0;		//????????
			//?????
			ATR_CHR_ACT_OLD(a0) = -1;
			break;
		}
		if (ATR_LIFE(a0) <= 0)		//???
			slow_flg--;		//?????
		//打飞 ?????
		if (ATR_AKO_FLG(a0))
		{
			//????
			ATR_FIRST_FLG(a0) = 0;
			ATR_VCT_NO(a0) = 55;
			//?????ＯＫ??
			if (ATR_PET_OK(a0))
			{
				a1 = p_party[ATR_PLACE_NO(a0) + 5];		//????????
				if (ATR_NAME(a1) != NULL)
				{
					ATR_BODY_WORK(0, a0) = a1;		//?????????
					if (ATR_LIFE(a1) > 0)		//????????
					{
						ATR_VCT_NO(a1) = 56;		//????????
						ATR_BODY_WORK(0, a1) = a0;		//????????
					}
				}
				else
					ATR_BODY_WORK(0, a0) = NULL;		//pet work clear?????????
			}
			else
				ATR_BODY_WORK(0, a0) = NULL;		//?????????
			//??????
			if (BattleMyNo + 5 == ATR_PLACE_NO(a0))
			{
				// ????????????
				if (att_select_flg == TRUE)
				{
					//????
					pc.selectPetNo[pc.battlePetNo] = FALSE;
					pc.battlePetNo = -1;
				}
				else
				{
					// ?????
					if (battlePetNoBak2 == pc.battlePetNo)
					{
						//????
						pc.selectPetNo[pc.battlePetNo] = FALSE;
						pc.battlePetNo = -1;
					}
					else
						//????
						pc.selectPetNo[battlePetNoBak2] = FALSE;
				}
			}
			break;
		}
		if (ATR_KAISHIN(a0))		//??????
			ATR_SPD(a0) = 35;//后退距离
		else
			ATR_SPD(a0) = 28;
#ifdef _MAGIC_DEEPPOISION   //剧毒
		if (ATR_DEEPPOISION(a0) == 1)
		{
			ATR_SPD(a0) = 0;
			ATR_LIFE(a0) = 0;
		}
#endif
		ATR_VCT_NO(a0) = 12;		//???????
		break;
	case 12:		//??????
		//????
		if ((ATR_CHR_NO(a0) >= SPR_mwood1 && ATR_CHR_NO(a0) <= SPR_mstone3) ||
			(ATR_CHR_NO(a0) == 15323) || (ATR_CHR_NO(a0) == 10812) || (ATR_CHR_NO(a0) == 15527) ||
			(ATR_CHR_NO(a0) == 15543) || (ATR_CHR_NO(a0) == 15575) || (ATR_CHR_NO(a0) == 15607)
			)
			ATR_SPD(a0) -= 2;
		else
		{
			//后退动画
			if (ATR_CHR_ACT(a0) == ANIM_DAMAGE && ATR_CHR_NO(a0) >= SPR_pet061 && ATR_CHR_NO(a0) <= SPR_pet064 || ATR_CHR_NO(a0) == SPR_pet065)
			{
				pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
				gemini(a0);
				ATR_SPD(a0) -= 1;
			}
			else
			{
				pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP);
				gemini(a0);		//?
				ATR_SPD(a0) -= 2;
			}
		}
		if (ATR_SPD(a0) < 0)		//?????????
		{
			ATR_SPD(a0) = 10;
			ATR_VCT_NO(a0) = 13;		//???????
			ATR_STIMER(a0) = 16;
#ifdef _SHOOTCHESTNUT	// Syu ADD 宠技：丢栗子
			a1 = ATR_COUNTER(p_master);
			if (a1->anim_chr_no_bak == 101578)
				ATR_STIMER(a0) = 26;
#endif
		}
		break;
	case 13:		//???????
		if (--ATR_STIMER(a0))		//?????
			break;
#ifdef _SHOOTCHESTNUT	// Syu ADD 宠技：丢栗子
		a1 = ATR_COUNTER(p_master);
		if (a1->anim_chr_no_bak == 101578)
		{
			if ((abs(ATR_H_POS(a0) - ATR_INT_WORK0(a0)) + abs(ATR_V_POS(a0) - ATR_INT_WORK1(a0)) > 80))
			{
				ATR_H_POS(a0) = ATR_INT_WORK0(a0);
				ATR_V_POS(a0) = ATR_INT_WORK1(a0);
			}
		}
#endif
		if (ATR_LIFE(a0) > 0)			//?????
		{
			if (ATR_COUNTER(p_master) == a0 || ATR_SYNC_FLG(a0) == 1)		//??????????
			{
				d0 = ATR_INT_WORK0(a0);		//???
				d1 = ATR_INT_WORK1(a0);
				radar(a0, &d0, &d1);	//????
				ATR_CHR_ANG(a0) = crs_change_tbl[d0];
				ATR_SPD(a0) = 32;
				ATR_VCT_NO(a0) = 4;		//??
			}
			else
			{
				ATR_VCT_NO(a0) = 14;		//???
				ATR_FIRST_FLG(a0) = 0;		//??????
			}
		}
		else
		{
#ifdef _PETSKILL_ACUPUNCTURE
			////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
			if (ATR_COUNTER(p_master) == a0 || ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
				| ATT_ATTACKBACK
#endif
				))
			{
				if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
					| ATT_ATTACKBACK
#endif
					))
#else
			if (ATR_COUNTER(p_master) == a0 || ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
				| ATT_ATTACKBACK
#endif
				))
			{
				if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ACUPUNCTURE
#ifdef _PET_ITEM
					| ATT_ATTACKBACK
#endif
					))
#endif
					////////////////////
#else
			////////////////////
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
			if (ATR_COUNTER(p_master) == a0 || ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP))
			{
				if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP))
#else
			if (ATR_COUNTER(p_master) == a0 || ATR_ATTACK_KIND(0, a0) & ATT_REFLEX)
			{
				if (ATR_ATTACK_KIND(0, a0) & ATT_REFLEX)
#endif
					////////////////////
#endif
					ATR_DAMAGE(p_master)++;		//???
				else
				{
#ifdef _PETSKILL_BATTLE_MODEL
					if (ATR_BATTLE_MODEL(a0) != ATT_BATTLE_MODEL)	//我是不是被攻击物件攻击
#endif
						ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???
				}
				d0 = ATR_INT_WORK0(a0);		//???
				d1 = ATR_INT_WORK1(a0);
				radar(a0, &d0, &d1);	//????
				ATR_CHR_ANG(a0) = crs_change_tbl[d0];
			}
#ifdef _MAGIC_DEEPPOISION   //剧毒
			if (ATR_DEEPPOISION(a0) == 1)
				ATR_VCT_NO(a0) = VCT_NO_DIE + 1;
			else
#endif
				ATR_VCT_NO(a0) = VCT_NO_DIE;		//??
		}
		break;
	case 14:		//????????
		d0 = ATR_INT_WORK0(a0);
		d1 = ATR_INT_WORK1(a0);
		radar(a0, &d0, &d1);	//????
		if (ATR_FIRST_FLG(a0) == 0)
		{
			ATR_CHR_ANG(a0) = crs_change_tbl[d0];
			ATR_FIRST_FLG(a0) = 1;
		}
		ATR_CRS(a0) = d0;		//??????
		//????
		if ((ATR_CHR_NO(a0) >= SPR_mwood1 && ATR_CHR_NO(a0) <= SPR_mstone3) ||
			(ATR_CHR_NO(a0) == 15323) || (ATR_CHR_NO(a0) == 10812) || (ATR_CHR_NO(a0) == 15527) ||
			(ATR_CHR_NO(a0) == 15543) || (ATR_CHR_NO(a0) == 15575) || (ATR_CHR_NO(a0) == 15607)
			)
			d1 = ATR_SPD(a0) = 0;		//????
		else
			gemini(a0);		//?
		if (d1 <= (ATR_SPD(a0) >> 2))
		{	//????
			ATR_H_POS(a0) = ATR_INT_WORK0(a0);		//?
			ATR_V_POS(a0) = ATR_INT_WORK1(a0);
			ATR_VCT_NO(a0) = 0;		//??
			ATR_SPD(a0) = 32;
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 15:		//??
		ATR_VCT_NO(a0) = 0;		//??
		//??
		ATR_DAMAGE(a0) = Rnd(1, 500);
		ATR_PET_DAMAGE(a0) = Rnd(1, 500);
		set_damage_num(a0, 5, -64);
		ATR_LIFE(a0) += ATR_DAMAGE(a0);		//???
		ATR_PET_LIFE(a0) += ATR_PET_DAMAGE(a0);		//???
		break;
	case 16:		//????
		ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
		ATR_SPD(a0) = 16;
		ATR_CHR_ANG(a0) = (ATR_DAMAGE_ANG(a0) + 4) & 7;
		gemini(a0);
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		ATR_STIMER(a0)++;
		if (ATR_STIMER(a0) == 20)
		{
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = (ATR_CRS(a0) + 16) & 31;		//??
			ATR_VCT_NO(a0)++;		//?????
			ATR_STIMER(a0) = 0;
		}
		break;
	case 17:		//????
		ATR_CHR_ACT(a0) = ANIM_STAND;		//???????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		ATR_STIMER(a0)++;
		d0 = 0;
		if (ATR_FIRST_FLG(a0) == 0)
		{	//????????
			if (ATR_STIMER(a0) == 10)		//????
				d0 = 1;
		}
		else
		{
			a1 = ATR_BODY_WORK(0, a0);		//?????
			if (ATR_VCT_NO(a1) != 2)		//??????????
				d0 = 1;
		}
		if (d0)		//??????
		{
			if (ATR_COUNTER(p_master) == a0)
			{		//?????
				sav_command_point = command_point;		//??????????
				d0 = get_num();
				command_point = sav_command_point;		//????????????
				if (d0 == 255)		//???
				{
					ATR_STIMER(a0) = 1;		//?
					ATR_VCT_NO(a0) = 3;		//????
					break;
				}
			}
			ATR_VCT_NO(a0)++;		//?????
			ATR_STIMER(a0) = 0;
		}
		break;
	case 18:		//????
		gemini(a0);
		ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		ATR_STIMER(a0)++;
		if (ATR_STIMER(a0) == 20)
		{
#ifdef _PETSKILL_BATTLE_MODEL
			a1 = ATR_BODY_WORK(0, a0);
			if (a1 && ATR_INT_WORK0(a1) == ATT_BATTLE_MODEL)
				ATR_VCT_NO(a0) = 4;
			else
#endif
			if (ATR_COUNTER(p_master) == a0)
			{	//?????
				ATR_STIMER(a0) = 1;		//?
				ATR_VCT_NO(a0) = 3;		//????
			}
			else
			{
				if (ATR_COUNTER_FLG(a0))
				{	//???????
					ATR_VCT_NO(a0) = 2;		//???
					ATR_BODY_WORK(0, a0) = p_party[get_num()];		//????
					ATR_ATTACK_KIND(0, a0) = get_num();		//??????
					ATR_ATTACK_POW(0, a0) = get_num();		//??????
#ifdef _PET_ITEM
					if (ATR_ATTACK_KIND(0, a0) & ATT_ATTACKBACK)
					{
						ATR_ATTACK_POW(1, a0) = ATR_ATTACK_POW(0, a0) >> 16;
						ATR_ATTACK_POW(0, a0) = ATR_ATTACK_POW(0, a0) & 0x0000ffff;
					}
#endif
#ifdef _FIX_ACUPUNCTURE
					if (ATR_ATTACK_KIND(0, a0) & ATT_ACUPUNCTURE)
					{
						ATR_ATTACK_POW(1, a0) = ATR_ATTACK_POW(0, a0) >> 16;
						ATR_ATTACK_POW(0, a0) = ATR_ATTACK_POW(0, a0) & 0x0000ffff;
					}
#endif
					if (BattleCmd[command_point] == 'p')
						ATR_ATTACK_PET_POW(0, a0) = get_num();		//pet??????
					else
						LogToBattleError(BattleCmd, __LINE__);
#ifdef _ATTACK_EFFECT
					if (BattleCmd[command_point] == 's')
						ATR_LONG_WORK(0, a0) = get_num();	// 记录特效编号
#endif
					ATR_FIRST_FLG(a0) = 0;		//????????
				}
				else
					ATR_VCT_NO(a0) = 0;		//??
			}
		}
		break;
	case 19:		//????
		a1 = ATR_BODY_WORK(0, a0);		//????
		if (!ATR_LONG_WORK(0, a1))		//?????
		{
			if (ATR_BODY_WORK(1, a1) == a0)		//??????
			{
				ATR_VCT_NO(a0) = 2;		//???
				break;
			}
		}
		ATR_CHR_ACT(a0) = ANIM_STAND;		//???????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 20:		//??????
		if (ATR_BODY_CNT(a0) == 1 && ATR_FIRST_FLG(a0) == 0)		//????????
		{
			a2 = ATR_BODY_WORK(0, a0);		//???
			d0 = ATR_H_POS(a2);
			d1 = ATR_V_POS(a2);
			radar(a0, &d0, &d1);	//????
			ATR_CHR_ANG(a0) = crs_change_tbl[d0];		/* ????? */
			ATR_FIRST_FLG(a0) = 1;		//??????
		}
		ATR_CHR_ACT(a0) = ANIM_ATTACK;		//?????????
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{	//????????
			ATR_CHR_ACT_OLD(a0) = -1;
			ATR_VCT_NO(a0) = 0;		//??
			break;
		}
		//????????
		if (ATR_CHR_CNT(a0) == ATR_HIT_TIMING(a0) && ATR_CHR_TIM(a0) == 0)
		{
			if (ATR_BODY_CNT(a0) == 1)		//??
			{
				//???????
				a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
				if (a1 == NULL)
				{
					ATR_DAMAGE(p_master) = 1;		//???
					ATR_VCT_NO(a0) = 0;		//??
					return;
				}
				/* ??? */
				ATR_NAME(a1) = missile;
				/* ? */
				ATR_DISP_PRIO(a1) = D_PRIO_MISSILE;
				/* ?????? */
				if (ATR_CHR_NO(a0) >= 31027)		//??????
					ATR_CHR_NO(a1) = 32053;
				else
					ATR_CHR_NO(a1) = 32055;
				ATR_ATTACK_KIND(0, a1) = get_num();		//??????
				ATR_ATTACK_POW(0, a1) = get_num();		//??????
				if (BattleCmd[command_point] == 'p')
					ATR_ATTACK_PET_POW(0, a1) = get_num();		//pet??????
				else
					LogToBattleError(BattleCmd, __LINE__);

				//修正战斗特效   xiezi
#ifdef _ATTACK_EFFECT
				if (BattleCmd[command_point] == 's')
					ATR_LONG_WORK(0, a1) = get_num();	// 记录特效编号
#endif

				/* ??? */
				ATR_CHR_ANG(a1) = ATR_CHR_ANG(a0);
				ATR_BODY_WORK(0, a1) = ATR_BODY_WORK(0, a0);
				ATR_H_POS(a1) = ATR_H_POS(a0);
				ATR_V_POS(a1) = ATR_V_POS(a0);
				ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];
				ATR_SPD(a1) = 32;
				gemini(a1);
				gemini(a1);
				gemini(a1);
				ATR_BODY_CNT(a1) = 0;
				p_missile[0] = a1;
				p_missile[1] = NULL;
			}
			else
			{
				for (d7 = 0; d7 < ATR_BODY_CNT(a0); d7++)
				{
					//???????
					a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
					if (a1 == NULL)
					{
						ATR_DAMAGE(p_master) = ATR_BODY_CNT(a0);		//???
						ATR_VCT_NO(a0) = 0;		//??
						return;
					}
					/* ??? */
					ATR_NAME(a1) = missile;
					/* ? */
					ATR_DISP_PRIO(a1) = D_PRIO_MISSILE;
					/* ?????? */
					if (ATR_CHR_NO(a0) >= 31027)		//??????
						ATR_CHR_NO(a1) = 32053;
					else
						ATR_CHR_NO(a1) = 32055;
					/* ??? */
					a2 = ATR_BODY_WORK(0, a1) = p_party[get_num()];		//?
					ATR_ATTACK_KIND(0, a1) = get_num();		//??????
					ATR_ATTACK_POW(0, a1) = get_num();		//??????
					if (BattleCmd[command_point] == 'p')
						ATR_ATTACK_PET_POW(0, a1) = get_num();		//pet??????
					else
						LogToBattleError(BattleCmd, __LINE__);

					//修正战斗特效   xiezi
#ifdef _ATTACK_EFFECT
					if (BattleCmd[command_point] == 's')
						ATR_LONG_WORK(0, a1) = get_num();	// 记录特效编号
#endif

					ATR_H_POS(a1) = ATR_H_POS(a0);
					ATR_V_POS(a1) = ATR_V_POS(a0);
					d0 = ATR_H_POS(a2);
					d1 = ATR_V_POS(a2);
					radar(a1, &d0, &d1);
					ATR_CHR_ANG(a1) = crs_change_tbl[d0];		/* ????? */
					ATR_SPD(a1) = 32;		//????
					ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];
					gemini(a1);
					gemini(a1);
					gemini(a1);
					ATR_CRS(a1) = d0;
					ATR_BODY_CNT(a1) = d7;
					p_missile[d7] = a1;
				}
				p_missile[d7] = NULL;
			}
		}
		break;
	case 25:		//??
		if (ATR_GROUP_FLG(a0) == 0)
			ATR_CHR_ANG(a0) = 3;
		else if (ATR_GROUP_FLG(a0) == 1)
			ATR_CHR_ANG(a0) = 7;
#ifdef _BATTLE_PK_PLAYER_FOR_40
		else if (ATR_GROUP_FLG(a0) == 2)
			ATR_CHR_ANG(a0) = 5;
		else if (ATR_GROUP_FLG(a0) == 3)	
			ATR_CHR_ANG(a0) = 1;
#endif
		ATR_CHR_ACT(a0) = ANIM_ATTACK;		//?????????
		ATR_HIT(a0) = 0;		//?????????
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{	//????????
			ATR_CHR_ACT_OLD(a0) = -1;
			ATR_VCT_NO(a0) = 0;		//??
			break;
		}
		//???????
		if (ATR_HIT(a0) >= 10000 && ATR_HIT(a0) < 10100)
		{
			//????
			a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
			if (a1 == NULL)
			{
				ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
				ATR_VCT_NO(a0) = 0;		//??
				return;
			}
			/* ??? */
			ATR_NAME(a1) = monster;
			ATR_VCT_NO(a1) = 26;
			/* ? */
			ATR_DISP_PRIO(a1) = D_PRIO_HIT_MARK;
			/* ?????? */
			ATR_CHR_NO(a1) = SPR_effect01;
			/* ??? */
			LoadBmp(ATR_PAT_NO(a0));
#ifndef __CARYTEST
			ATR_V_POS(a1) = ATR_V_POS(a0) + a0->anim_y + SpriteInfo[ATR_PAT_NO(a0)].height / 2;
#else
			ATR_V_POS(a1) = ATR_V_POS(a0) + a0->anim_y + g_lpRealAdrn[ATR_PAT_NO(a0)].height / 2;
#endif
			ATR_H_POS(a1) = ATR_H_POS(a0);
		}
		break;
	case 26:		//????
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{	//??????
			//??????
			while (1)
			{
				int dx;

				d0 = get_num();		//??????
				if (d0 == 255)		//???
					break;
				a2 = p_party[d0];		//????????????
				d0 = get_num();		//????????
				if (BattleCmd[command_point] == 'p')
					dx = get_num();		//pet????????
				else
					LogToBattleError(BattleCmd, __LINE__);

				//???????
				a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
				if (a1 == NULL)
				{
					ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
					break;
				}
				/* ??? */
				ATR_NAME(a1) = monster;
				ATR_VCT_NO(a1) = 27;
				ATR_BODY_WORK(0, a1) = a2;
				ATR_INT_WORK0(a1) = d0;		//?????
				if (BattleCmd[command_point] == 'p')
					ATR_INT_WORKp(a1) = dx;		//pet?????
				else
					LogToBattleError(BattleCmd, __LINE__);
				/* ? */
				ATR_DISP_PRIO(a1) = D_PRIO_HIT_MARK;
				/* ?????? */
				d0 = get_num();
				d0 = 0;
				ATR_CHR_NO(a1) = SPR_heal + d0;
				/* ??? */
				LoadBmp(ATR_PAT_NO(a2));
				ATR_H_POS(a1) = ATR_H_POS(a2);
#ifndef __CARYTEST
				ATR_V_POS(a1) = ATR_V_POS(a2) + a2->anim_y + SpriteInfo[ATR_PAT_NO(a2)].height / 2;
#else
				ATR_V_POS(a1) = ATR_V_POS(a2) + a2->anim_y + g_lpRealAdrn[ATR_PAT_NO(a2)].height / 2;
#endif
			}
			DeathAction(a0);		//?
			return;
		}
		break;
	case 27:		//??????
		if (ATR_HIT_STOP(a0))
		{
			ATR_HIT_STOP(a0)--;
			break;
		}
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{	//??????
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
			DeathAction(a0);		//?
			return;
		}
		if (ATR_CHR_CNT(a0) == 9 && ATR_CHR_TIM(a0) == 0)
		{
			a2 = ATR_BODY_WORK(0, a0);
			ATR_DAMAGE(a2) = ATR_INT_WORK0(a0);		//?????
			ATR_LIFE(a2) += ATR_DAMAGE(a2);		//
			ATR_HIT_STOP(a0) = 40;
			//??
			if (ATR_LIFE(a2) > ATR_MAX_LIFE(a2))		//????
				ATR_LIFE(a2) = ATR_MAX_LIFE(a2);		//????
			ATR_PET_DAMAGE(a2) = ATR_INT_WORKp(a0);		//?????
			ATR_PET_LIFE(a2) += ATR_PET_DAMAGE(a2);		//
			//??
			if (ATR_PET_LIFE(a2) > ATR_PET_MAX_LIFE(a2))		//????
				ATR_PET_LIFE(a2) = ATR_PET_MAX_LIFE(a2);		//????
			set_damage_num(a2, 14, -64);
		}
		break;
#ifdef _FIREHUNTER_SKILL				// (不可开) ROG ADD 朱雀技能_火线猎杀
	case FIRE_HUNTER_SKILL:
		if (ATR_FIRST_FLG(a0) == 0)
		{	//最初的一回合
			a2 = ATR_BODY_WORK(0, a0);		//敌人位址
			d0 = ATR_H_POS(a2);
			d1 = ATR_V_POS(a2);
			radar(a0, &d0, &d1);	//雷达
			ATR_CRS(a0) = d0;		//路线储存
			ATR_CHR_ANG(a0) = crs_change_tbl[d0];		//方向
			ATR_CHR_ACT(a0) = ANIM_ATTACK;		//攻击动作
		}
		ATR_HIT(a0) = 30;		//攻击清除
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{	//攻击动画终了
			ATR_CHR_ACT_OLD(a0) = -1;
			//Change fix 原本为 31 改成 FIRE_HUNTER_SKILL+1
			ATR_VCT_NO(a0) = FIRE_HUNTER_SKILL+1;	//攻击终了待机
			ATR_FIRST_FLG(a0) = 0;		//设定为最初一回合
			break;
		}
		if (ATR_HIT(a0) >= 10000 && ATR_HIT(a0) < 10100)
		{
			a1 = GetAction(T_PRIO_BOW, sizeof(ATR_EQU));		//火焰
			if (a1 == NULL)
			{
				ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//到达的讯息
				ATR_VCT_NO(a0) = 0;		//待机
				return;
			}
			ATR_STIMER(a0) = 0;		//????????
			ATR_NAME(a1) = fireHunter;   //火焰的function
			ATR_CHR_NO(a1) = 101734;	 //图号
			ATR_DISP_PRIO(a1) = D_PRIO_MISSILE;  //优先度
			/*初始位置*/
			ATR_CHR_ANG(a1) = ATR_CHR_ANG(a0);						//方向
			ATR_BODY_WORK(0, a1) = ATR_BODY_WORK(0, a0);				//敌人位址
			ATR_BODY_WORK(1, a1) = a0;								//发射火焰的宠
			for (int i = 2; i < ATR_BODY_CNT(a0) + 2; i++)
			{
				ATR_BODY_WORK(i, a1) = ATR_BODY_WORK(i, a0);			//敌人位址
				ATR_ATTACK_POW(i, a1) = ATR_ATTACK_POW(i, a0);			//攻击力
				ATR_ATTACK_PET_POW(i, a1) = ATR_ATTACK_PET_POW(i, a0);	//宠物攻击力
			}
			ATR_ATTACK_POW(0, a1) = ATR_ATTACK_POW(0, a0);			//攻击力
			ATR_ATTACK_PET_POW(0, a1) = ATR_ATTACK_PET_POW(0, a0);	//宠物攻击力
			ATR_ATTACK_KIND(0, a1) = ATR_ATTACK_KIND(0, a0);			//攻击种类
			ATR_SPD(a1) = 60;										//速度	
			ATR_H_POS(a1) = ATR_H_POS(a0) ;								
			ATR_V_POS(a1) = ATR_V_POS(a0) - 50;
			ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];			//路径
			ATR_COUNTER_FLG(a1) = ATR_CRS(a1);						//敌人攻击路线设定
			ATR_BODY_CNT(a1) = ATR_BODY_CNT(a0);									//到达旗标
			bFireInit = TRUE;
			FireSkillEnd = TRUE;
			p_missile[0] = a1;
			p_missile[1] = NULL;
			d6 = 0;		//计算到达时间
			while (1)
			{
				d0 = ATR_H_POS(ATR_BODY_WORK(0, a1));
				d1 = ATR_V_POS(ATR_BODY_WORK(0, a1));
				radar(a1, &d0, &d1);	//雷达
				//到达
				if (d1 <= 10)
					break;
				ATR_CRS(a1) = d0;
				gemini(a1);
				d6++;
			}
			if (!(ATR_GROUP_FLG(a1) = d6))		//到达时间储存
				ATR_GROUP_FLG(a1) = 1;
			ATR_H_POS(a1) = ATR_H_POS(a0) ;								
			ATR_V_POS(a1) = ATR_V_POS(a0) - 30;
			ATR_CHR_ANG(a1) = ATR_CHR_ANG(a0);						//方向
			ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];			//路径
			ATR_COUNTER_FLG(a1) = ATR_CRS(a1);						//敌人攻击路线设定
		}
		break;
		//Change fix 敌方睡眠后再火线,会跑去 31 (ATR_VCT_NO(a0) = 31;),结果会射箭出去,改为 ATR_VCT_NO(a0) = FIRE_HUNTER_SKILL+1; 改成这一段
	case FIRE_HUNTER_SKILL + 1:
		ATR_CHR_ACT(a0) = ANIM_STAND;		//???????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		if (!ATR_STIMER(a0))		//???????
			break;
		ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
		ATR_VCT_NO(a0) = 0;		//??
		break;
#endif
	case 30:		//?????
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			a2 = ATR_BODY_WORK(0, a0);		//???
			d0 = ATR_H_POS(a2);
			d1 = ATR_V_POS(a2);
			radar(a0, &d0, &d1);	//????
			ATR_CRS(a0) = d0;		//???
			ATR_CHR_ANG(a0) = crs_change_tbl[d0];		/* ????? */
			if (ATR_LONG_WORK(0, a0) == 0)		//?????
				ATR_CHR_ACT(a0) = ANIM_ATTACK;		//?????????
			else
				ATR_CHR_ACT(a0) = ANIM_THROW;		//?????????
			ATR_FIRST_FLG(a0) = 1;		//??????
		}
		ATR_HIT(a0) = 0;		//?????????
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{	//????????
			ATR_CHR_ACT_OLD(a0) = -1;
			ATR_VCT_NO(a0) = 31;		//??????
			ATR_FIRST_FLG(a0) = 0;		//????????
			break;
		}
		//???????
		//?????
		if (ATR_HIT(a0) >= 10000 && ATR_HIT(a0) < 10100)
		{
			//??????
			a1 = GetAction(T_PRIO_BOW, sizeof(ATR_EQU));
			if (a1 == NULL)
			{
				ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
				ATR_VCT_NO(a0) = 0;		//??
				return;
			}
			ATR_STIMER(a0) = 0;		//????????
			/* ??? */
			switch (ATR_LONG_WORK(0, a0))
			{
			case 0:		//?
				ATR_NAME(a1) = bow;
				ATR_CHR_NO(a1) = CG_ARROW_00 + ATR_CRS(a0) / 2 + 20;
				break;
			case 1:		//???
				ATR_NAME(a1) = axe_shadow;
				ATR_CHR_NO(a1) = SPR_onokage;
				break;
			case 2:		//??
				ATR_NAME(a1) = stone_shadow;
				ATR_CHR_NO(a1) = 25786;
				break;
				//Terry add 2001/12/28
#ifdef _ITEM_FIRECREAKER
			case 3:		//鞭炮
				ATR_NAME(a1) = firecracker_shadow;
				ATR_CHR_NO(a1) = 25786;
				break;
#endif
				//Terry end
			default:
				ATR_NAME(a1) = bow;
			}
			/* ? */
			ATR_DISP_PRIO(a1) = D_PRIO_MISSILE;
			/* ??? */
			ATR_CHR_ANG(a1) = ATR_CHR_ANG(a0);
			ATR_BODY_WORK(0, a1) = ATR_BODY_WORK(0, a0);		//?????
			ATR_BODY_WORK(1, a1) = a0;		//?????
			ATR_ATTACK_POW(0, a1) = ATR_ATTACK_POW(0, a0);		//???
			ATR_ATTACK_PET_POW(0, a1) = ATR_ATTACK_PET_POW(0, a0);		//???
			ATR_ATTACK_KIND(0, a1) = ATR_ATTACK_KIND(0, a0);		//?????
			ATR_H_POS(a1) = ATR_H_POS(a0);
			ATR_V_POS(a1) = ATR_V_POS(a0);
			ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];
			ATR_SPD(a1) = 32;
			gemini(a1);
			gemini(a1);
			gemini(a1);
			ATR_BODY_CNT(a1) = 0;
			ATR_COUNTER_FLG(a1) = ATR_CRS(a0);		//?????????????
			p_missile[0] = a1;
			p_missile[1] = NULL;
			//????
			a2 = GetAction(T_PRIO_BOW, sizeof(ATR_EQU));
			if (a2 == NULL)
			{
				ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
				ATR_VCT_NO(a0) = 0;		//??
				return;
			}
			/* ??? */
			switch (ATR_LONG_WORK(0, a0))
			{
			case 0:		//?
				ATR_NAME(a2) = stick_bow;
				ATR_CHR_NO(a2) = CG_ARROW_00 + ATR_CRS(a0) / 2;
				break;
			case 1:		//???
				ATR_NAME(a2) = axe;
				ATR_CHR_NO(a2) = SPR_ono;
				break;
			case 2:		//??
				ATR_NAME(a2) = stone;
				ATR_CHR_NO(a2) = 25785;
				break;
				//Terry add 2001/12/28
#ifdef _ITEM_FIRECREAKER
			case 3:		//鞭炮
				ATR_NAME(a2) = stone;
				ATR_CHR_NO(a2) = 24350;
				break;
#endif
				//Terry end
			default:
				ATR_NAME(a2) = stick_bow;
			}
			/* ? */
			ATR_DISP_PRIO(a2) = D_PRIO_MISSILE;
			//?????????
			ATR_LONG_WORK(0, a1) = ATR_CHR_NO(a2);
			ATR_BODY_WORK(2, a1) = a2;
			/* ??? */
			ATR_CHR_ANG(a2) = ATR_CHR_ANG(a1);
			a3 = ATR_BODY_WORK(0, a2) = ATR_BODY_WORK(0, a1);		//?????
			ATR_BODY_WORK(1, a2) = a1;		//??????
			ATR_H_POS(a2) = ATR_H_POS(a1);
			ATR_V_POS(a2) = ATR_V_POS(a1);

			d6 = 0;		//????
			ATR_SPD(a2) = 32;
			while (1){
				d0 = ATR_H_POS(a3);
				d1 = ATR_V_POS(a3);
				radar(a2, &d0, &d1);	//????
				//???????
				if (d1 <= 20){
					break;
				}
				ATR_CRS(a2) = d0;
				gemini(a2);
				d6++;
			}
			ATR_INT_WORK0(a2) = d1;		//?????
			if (!(ATR_GROUP_FLG(a1) = d6)){		//???????
				ATR_GROUP_FLG(a1) = 1;
			}
			d6 = d6 >> 1;
			ATR_SPD(a2) = d6;		//
			ATR_CRS(a2) = 0;		//
			ATR_H_MINI(a2) = 0;		//
			ATR_V_MINI(a2) = 0;		//
			ATR_H_POS(a2) = ATR_H_POS(a1);		//?
			ATR_V_POS(a2) = ATR_V_POS(a1) - 28;		//
		}
		break;

	case 31:		//???????
		ATR_CHR_ACT(a0) = ANIM_STAND;		//???????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		if (!ATR_STIMER(a0)){		//???????
			break;
		}
		d0 = get_num();		//?????
		if (d0 == 255){		//???
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
			ATR_VCT_NO(a0) = 0;		//??
		}
		else {
			ATR_VCT_NO(a0) = 30;		//???
			ATR_FIRST_FLG(a0) = 0;		//
			ATR_BODY_WORK(0, a0) = p_party[d0];		//????
			ATR_ATTACK_KIND(0, a0) = get_num();		//??????
			ATR_ATTACK_POW(0, a0) = get_num();		//??????
#ifdef _PET_ITEM
			if (ATR_ATTACK_KIND(0, a0) & ATT_ATTACKBACK)
			{
				ATR_ATTACK_POW(1, a0) = ATR_ATTACK_POW(0, a0) >> 16;
				ATR_ATTACK_POW(0, a0) = ATR_ATTACK_POW(0, a0) & 0x0000ffff;
			}
#endif
#ifdef _FIX_ACUPUNCTURE
			if (ATR_ATTACK_KIND(0, a0) & ATT_ACUPUNCTURE)
			{
				ATR_ATTACK_POW(1, a0) = ATR_ATTACK_POW(0, a0) >> 16;
				ATR_ATTACK_POW(0, a0) = ATR_ATTACK_POW(0, a0) & 0x0000ffff;
			}
#endif
			if (BattleCmd[command_point] == 'p')
				ATR_ATTACK_PET_POW(0, a0) = get_num();		//pet??????
			else
				LogToBattleError(BattleCmd, __LINE__);

			//修正技能特效 xiezi
#ifdef _ATTACK_EFFECT
			if (BattleCmd[command_point] == 's')
				ATR_LONG_WORK(0, a0) = get_num();	// 记录特效编号
#endif

		}
		break;

	case 32:		//弓攻击
		// a2为主、a1为影
		if (ATR_FIRST_FLG(a0) == 0){		//???????
			a2 = ATR_BODY_WORK(0, a0);		//???
			d0 = ATR_H_POS(a2);
			d1 = ATR_V_POS(a2);
			radar(a0, &d0, &d1);	//????
			ATR_CRS(a0) = d0;		//???
			ATR_CHR_ANG(a0) = crs_change_tbl[d0];		/* ????? */
			if (ATR_LONG_WORK(0, a0) == 0){		//?????
				ATR_CHR_ACT(a0) = ANIM_ATTACK;		//?????????
			}
			else {
				ATR_CHR_ACT(a0) = ANIM_THROW;		//?????????
			}
			ATR_FIRST_FLG(a0) = 1;		//??????
		}

#ifdef _SHOOTCHESTNUT	// Syu ADD 宠技：丢栗子
		else if (ShooterNum == 101578) {
			a2 = ATR_BODY_WORK(0, a0);		//???
			//不定点	
			//d0 = /*ATR_H_POS(a2) =*/ nutx[ a2->hitDispNo ] ;
			//d1 = /*ATR_V_POS(a2) =*/ nuty[ a2->hitDispNo ] ;
			//定点
			d0 = ATR_H_POS(a2) = ATR_INT_WORK0(a2);
			d1 = ATR_V_POS(a2) = ATR_INT_WORK1(a2);

			radar(a0, &d0, &d1);	//????
			//			ATR_CRS(a0) = d0;		//???
			//			ATR_CHR_ANG(a0) = crs_change_tbl[ d0 ];		/* ????? */
		}
#endif


		ATR_HIT(a0) = 0;		//?????????
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP)){	//????????
			ATR_VCT_NO(a0) = 33;		//??????
			break;
		}
		//???????
		//?????
		if (ATR_HIT(a0) >= 10000 && ATR_HIT(a0) < 10100){
			//??????
			a1 = GetAction(T_PRIO_BOW, sizeof(ATR_EQU));
			if (a1 == NULL){
				ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
				ATR_VCT_NO(a0) = 0;		//??
				return;
			}
			/* ??? */
			switch (ATR_LONG_WORK(0, a0)){
			case 0:		//?
				ATR_NAME(a1) = bow;
				break;
			case 1:		//???
				ATR_NAME(a1) = axe_shadow;
				break;
			case 2:		//??
				ATR_NAME(a1) = stone_shadow;
				break;
			default:
				ATR_NAME(a1) = bow;
			}
			/* ? */
			ATR_DISP_PRIO(a1) = D_PRIO_MISSILE;
			/* ?????? */
#ifdef _SHOOTCHESTNUT	// Syu ADD 宠技：丢栗子
			if (ShooterNum == 101578)
				ATR_CHR_NO(a1) = 27001;
			else
				ATR_CHR_NO(a1) = CG_ARROW_00 + ATR_CRS(a0) / 2 + 20;
#else
			ATR_CHR_NO(a1) = CG_ARROW_00 + ATR_CRS(a0) / 2 + 20;
#endif
			/* ??? */
			ATR_CHR_ANG(a1) = ATR_CHR_ANG(a0);
			ATR_BODY_WORK(0, a1) = ATR_BODY_WORK(0, a0);		//?????
			ATR_BODY_WORK(1, a1) = a0;		//?????
			ATR_ATTACK_POW(0, a1) = ATR_ATTACK_POW(0, a0);		//???
			ATR_ATTACK_PET_POW(0, a1) = ATR_ATTACK_PET_POW(0, a0);		//???
			ATR_ATTACK_KIND(0, a1) = ATR_ATTACK_KIND(0, a0);		//?????
			ATR_H_POS(a1) = ATR_H_POS(a0);
			ATR_V_POS(a1) = ATR_V_POS(a0);
			ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];
			ATR_SPD(a1) = 32;
			gemini(a1);
			gemini(a1);
			gemini(a1);
			ATR_BODY_CNT(a1) = 0;
			ATR_COUNTER_FLG(a1) = ATR_CRS(a0);		//?????????????
			p_missile[0] = a1;
			p_missile[1] = NULL;

			//????
			a2 = GetAction(T_PRIO_BOW, sizeof(ATR_EQU));
			if (a2 == NULL){
				ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
				ATR_VCT_NO(a0) = 0;		//??
				return;
			}
			/* ??? */
			switch (ATR_LONG_WORK(0, a0)){
			case 0:		//?
				ATR_NAME(a2) = stick_bow;
				break;
			case 1:		//???
				ATR_NAME(a2) = axe;
				break;
			case 2:		//??
				ATR_NAME(a2) = stone;
				break;
			default:
				ATR_NAME(a2) = stick_bow;
			}
			/* ? */
			ATR_DISP_PRIO(a2) = D_PRIO_MISSILE;
			/* ?????? */
#ifdef _SHOOTCHESTNUT	// Syu ADD 宠技：丢栗子
			//弓的部分
			if (ShooterNum == 101578)
				ATR_CHR_NO(a2) = 26995;
			else
				ATR_CHR_NO(a2) = CG_ARROW_00 + ATR_CRS(a0) / 2;
#else
			ATR_CHR_NO(a2) = CG_ARROW_00 + ATR_CRS(a0) / 2;
#endif
			//?????????
			ATR_LONG_WORK(0, a1) = ATR_CHR_NO(a2);
			ATR_BODY_WORK(2, a1) = a2;
			/* ??? */
			ATR_CHR_ANG(a2) = ATR_CHR_ANG(a1);
			a3 = ATR_BODY_WORK(0, a2) = ATR_BODY_WORK(0, a1);		//?????
			ATR_BODY_WORK(1, a2) = a1;		//??????
			ATR_H_POS(a2) = ATR_H_POS(a1);
			ATR_V_POS(a2) = ATR_V_POS(a1);


			d6 = 0;		//????
			ATR_SPD(a2) = 32;
			while (1)
			{
				d0 = ATR_H_POS(a3);
				d1 = ATR_V_POS(a3);
				radar(a2, &d0, &d1);	//????
				//???????
				if (d1 <= 20)
					break;
				ATR_CRS(a2) = d0;
				gemini(a2);
				d6++;
			}
			ATR_INT_WORK0(a2) = d1;		//?????
			if (!(ATR_GROUP_FLG(a1) = d6))		//???????
				ATR_GROUP_FLG(a1) = 1;
			d6 = d6 >> 1;
			ATR_SPD(a2) = d6;		//
			ATR_CRS(a2) = 0;		//
			ATR_H_MINI(a2) = 0;		//
			ATR_V_MINI(a2) = 0;		//
			ATR_H_POS(a2) = ATR_H_POS(a1);		//?
			ATR_V_POS(a2) = ATR_V_POS(a1) - 28;		//

			d0 = get_num();		//?????
			if (d0 != 255)
			{	//??????
				if (ATR_CHR_CNT(a0) >= 3)		//???
					ATR_CHR_CNT(a0) -= 3;
				else
					ATR_CHR_ACT_OLD(a0) = -1;		//???
				ATR_DAMAGE(a0)++;
				ATR_FIRST_FLG(a0) = 0;
				ATR_BODY_WORK(0, a0) = p_party[d0];		//攻击对象设定
				ATR_ATTACK_KIND(0, a0) = get_num();		//攻击种类设定
				ATR_ATTACK_POW(0, a0) = get_num();		//攻击力设定
				if (BattleCmd[command_point] == 'p')
					ATR_ATTACK_PET_POW(0, a0) = get_num();		//宠物攻击力设定
				else
					LogToBattleError(BattleCmd, __LINE__);

				//修正技能特效 xiezi
#ifdef _ATTACK_EFFECT
				if (BattleCmd[command_point] == 's')
					ATR_LONG_WORK(0, a0) = get_num();	// 记录特效编号
#endif
			}
		}
		break;
	case 33:		//弓攻击结束等待
		ATR_CHR_ACT(a0) = ANIM_STAND;		//???????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		if (ATR_STIMER(a0) == ATR_DAMAGE(a0))
		{	//?????
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
			ATR_VCT_NO(a0) = 0;		//待机
		}
		break;
	case 35:		//捕获移动
		ATR_SPD(a0) = 32;		//速度
		a1 = ATR_BODY_WORK(0, a0);		//????????
		d0 = ATR_H_POS(a1);
		d1 = ATR_V_POS(a1);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			ATR_CHR_ANG(a0) = crs_change_tbl[d0];
			ATR_FIRST_FLG(a0) = 1;		//??????
		}
		if (d1 <= 32 * 2)
		{	//????
			ATR_VCT_NO(a0)++;		//???
			ATR_FIRST_FLG(a0) = 0;		//????????
		}
		else
			gemini(a0);		//?
		ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 36:		//??
		a1 = ATR_BODY_WORK(0, a0);		//????????
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			ATR_HIT(a0) = 0;				//?????????
			d0 = ATR_H_POS(a1);				//???
			d1 = ATR_V_POS(a1);
			radar(a0, &d0, &d1);	//????
			ATR_CRS(a0) = d0;		//???
			ATR_CHR_ANG(a0) = crs_change_tbl[d0];		/* ????? */
			ATR_FIRST_FLG(a0) = 1;		//??????
		}
		ATR_CHR_ACT(a0) = ANIM_ATTACK;		//?????????
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{	//????????
			ATR_VCT_NO(a0)++;		//??????
			ATR_FIRST_FLG(a0) = 0;		//????????
			ATR_STIMER(a0) = 60;
			break;
		}
		//???????
		if (ATR_HIT(a0) != 0 && ATR_HIT(a0) < 10100)
		{
			//?????
			play_damage(ATR_HIT(a0), ATR_H_POS(a0));
			//?
			set_damage_num(a0, 5, -64);
			//???????
			ATR_VCT_NO(a1) = 45;
			//?????????
			ATR_BODY_WORK(0, a1) = a0;
			//?
			ATR_SPD(a1) = ATR_SPD(a0) = 8;
			//?????????
			ATR_HIT(a0) = 0;
		}
		break;
	case 37:		//?????
		if (ATR_STIMER(a0))
		{
			ATR_STIMER(a0)--;
			break;
		}
		ATR_VCT_NO(a0)++;		//?????
		break;
	case 38:		//????
		d0 = ATR_INT_WORK0(a0);
		d1 = ATR_INT_WORK1(a0);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		gemini(a0);		//?
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			ATR_CHR_ANG(a0) = crs_change_tbl[d0];
			ATR_FIRST_FLG(a0) = 1;		//??????
		}
		if (d1 <= (ATR_SPD(a0) >> 2))
		{	//????
			if (ATR_GROUP_FLG(a0) == 0)
				ATR_CHR_ANG(a0) = 3;
			else if (ATR_GROUP_FLG(a0) == 1)
				ATR_CHR_ANG(a0) = 7;
#ifdef _BATTLE_PK_PLAYER_FOR_40
			else if (ATR_GROUP_FLG(a0) == 2)
				ATR_CHR_ANG(a0) = 5;
			else if (ATR_GROUP_FLG(a0) == 3)	
				ATR_CHR_ANG(a0) = 1;
#endif
			ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
			ATR_H_POS(a0) = ATR_INT_WORK0(a0);		//?
			ATR_V_POS(a0) = ATR_INT_WORK1(a0);
			a1 = ATR_BODY_WORK(0, a0);		//????????
			if (ATR_ATTACK_KIND(0, a0) == 0)
			{	//??
				if (ATR_VCT_NO(a1) == 45)
				{	//???????
					ATR_VCT_NO(a0) = 40;		//????????
					ATR_DAMAGE(a0) = 0;		//?????
				}
				else
					ATR_VCT_NO(a0) = 42;		//?
			}
			else
			{
				ATR_VCT_NO(a0) = 44;		//??
				//?
				set_damage_num(a0, 7, -64);
				ATR_STIMER(a0) = 255;
				ATR_DAMAGE(a0) = t_music_bgm_no;		//????
				play_bgm(0);		//???????????
			}
		}
		a1 = ATR_BODY_WORK(0, a0);		//????????
		if (ATR_ATTACK_KIND(0, a0) == 0 && ATR_VCT_NO(a1) == 45)
		{	//??
			if (!Rnd(0, 100))
			{	//??????
				ATR_VCT_NO(a0) = 40;		//????????
				ATR_DAMAGE(a0) = 1;		//?????
			}
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		if (ATR_SPD(a0) == 32)		//??????
			pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 40:		//???????
		a1 = ATR_BODY_WORK(0, a0);		//????????
		ATR_VCT_NO(a1)++;		//?????
		ATR_FIRST_FLG(a1) = 0;		//????????
		//
		set_damage_num(a0, 8, -64);
		ATR_STIMER(a0) = 100;		//????????
		if (ATR_DAMAGE(a0) == 1)		//????
			ATR_CHR_ANG(a0) = (ATR_CHR_ANG(a0) + 4) & 7;		//??
		ATR_VCT_NO(a0)++;		//?????
		break;
	case 41:		//????
		if (!--ATR_STIMER(a0))
		{
			if (ATR_DAMAGE(a0) == 1)
			{		//????
				ATR_VCT_NO(a0) = 38;		//????
				ATR_CHR_ANG(a0) = (ATR_CHR_ANG(a0) + 4) & 7;		//??
				ATR_SPD(a0) = 32;		//?????
			}
			else
				ATR_VCT_NO(a0)++;		//?
			ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
		}
		else
			ATR_CHR_ACT(a0) = ANIM_ANGRY;		//夂????????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 42:		//失败
		ATR_CHR_ACT(a0) = ANIM_STAND;		//停止动作
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		a1 = ATR_BODY_WORK(0, a0);		//取出敌方指标
		if (ATR_VCT_NO(a1) == 0)
		{	//??????????
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
			ATR_VCT_NO(a0) = 0;		//??
		}
		break;
	case 44:		//成功
		if (!--ATR_STIMER(a0))
		{	//时间到
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
			ATR_VCT_NO(a0) = 0;		//待机
			play_bgm(ATR_DAMAGE(a0));		//?????
		}
		ATR_CHR_ACT(a0) = ANIM_HAPPY;		//???????????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 45:		//??????
		a1 = ATR_BODY_WORK(0, a0);		//??????????
		if (ATR_VCT_NO(a1) == 38)
		{	//???????
			d0 = ATR_H_POS(a1);				//???
			d1 = ATR_V_POS(a1);
			radar(a0, &d0, &d1);	//????
			ATR_CRS(a0) = d0;		//???
			gemini(a0);
		}
		if (ATR_VCT_NO(a1) == 0)
		{	//???????
			ATR_ATTRIB(a0) = ACT_ATR_HIDE;		//??
			ATR_NAME(a0) = NULL;
			return;
		}
		ATR_CHR_ACT(a0) = ANIM_DAMAGE;		//???????????
		ATR_CHR_ANG(a0) = (ATR_CHR_ANG(a0) + 1) & 7;
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 46:		//??????????
		d0 = ATR_INT_WORK0(a0);
		d1 = ATR_INT_WORK1(a0);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		ATR_SPD(a0) = 16;
		gemini(a0);		//?
		if (ATR_FIRST_FLG(a0) == 0)
		{		//???????
			ATR_CHR_ANG(a0) = crs_change_tbl[d0];
			ATR_FIRST_FLG(a0) = 1;		//??????
		}
		if (d1 <= (ATR_SPD(a0) >> 2))
		{	//????
			a1 = ATR_BODY_WORK(0, a0);		//??????????
			if (ATR_GROUP_FLG(a0) == 0)
				ATR_CHR_ANG(a0) = 3;
			else if (ATR_GROUP_FLG(a0) == 1)
				ATR_CHR_ANG(a0) = 7;
#ifdef _BATTLE_PK_PLAYER_FOR_40
			else if (ATR_GROUP_FLG(a0) == 2)
				ATR_CHR_ANG(a0) = 5;
			else if (ATR_GROUP_FLG(a0) == 3)	
				ATR_CHR_ANG(a0) = 1;
#endif
			ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
			ATR_H_POS(a0) = ATR_INT_WORK0(a0);		//?
			ATR_V_POS(a0) = ATR_INT_WORK1(a0);
			ATR_VCT_NO(a0) = 0;		//??
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 50:		//?
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			ATR_SPD(a0) = 16;
			if (ATR_LIFE(a0) <= 0)
			{	//??????
				if (ATR_GROUP_FLG(a0) == 0)		//????????
					//????
					ATR_CRS(a0) = crs_change_tbl2[7];
				else
					ATR_CRS(a0) = crs_change_tbl2[3];
			}
			else
			{
				if (ATR_GROUP_FLG(a0) == 0)
					ATR_CHR_ANG(a0) = 7;
				else if (ATR_GROUP_FLG(a0) == 1)
					ATR_CHR_ANG(a0) = 3;
#ifdef _BATTLE_PK_PLAYER_FOR_40
				else if (ATR_GROUP_FLG(a0) == 2)
					ATR_CHR_ANG(a0) = 1;
				else if (ATR_GROUP_FLG(a0) == 3)	
					ATR_CHR_ANG(a0) = 5;
#endif
				ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
			}
			ATR_FIRST_FLG(a0) = 1;		//??????
		}
		gemini(a0);		//?
		if (ATR_LIFE(a0) > 0)
		{		//?????
			ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
			pattern(a0, 1, ANM_LOOP);
		}
		if (ATR_H_POS(a0) > lpDraw->xSize + SCREEN_OUT || ATR_H_POS(a0) < 0 - SCREEN_OUT)
		{	//?䴘???
			ATR_NAME(a0) = NULL;
			ATR_ATTRIB(a0) |= ACT_ATR_HIDE;		//??
			ATR_VCT_NO(a0) = VCT_NO_APPEAR;
			ATR_DAMAGE(p_master)++;		//?
			if (ATR_DAMAGE(p_master) == ATR_BODY_CNT(p_master) && ATR_GUARD_FLG(a0) == 1)
			{	//ACTION_INF??
				action_inf = 2;
				command_point = 0;		//?????????????
			}
		}
		break;
	case 52:		//战斗逃跑失败(动画加速)
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			if (ATR_LIFE(a0) <= 0)
			{	//??????
				//?????ＯＫ??
				if (ATR_PET_OK(a0))
				{
					a2 = p_party[ATR_PLACE_NO(a0) + 5];		//????????
					if (ATR_NAME(a2) != NULL)
					{
						ATR_VCT_NO(a2) = 50;		//??????
						ATR_FIRST_FLG(a2) = 0;		//????????
					}
				}
				ATR_VCT_NO(a0) = 50;		//逃跑成功
				break;
			}
			//显示逃跑
			set_damage_num(a0, 9, -64);
			if (ATR_GROUP_FLG(a0) == 0)
				ATR_CHR_ANG(a0) = 7;
			else if (ATR_GROUP_FLG(a0) == 1)
				ATR_CHR_ANG(a0) = 3;
#ifdef _BATTLE_PK_PLAYER_FOR_40
			else if (ATR_GROUP_FLG(a0) == 2)
				ATR_CHR_ANG(a0) = 1;
			else if (ATR_GROUP_FLG(a0) == 3)	
				ATR_CHR_ANG(a0) = 5;
#endif
			ATR_STIMER(a0) = 12;		//???????
			ATR_COUNTER_FLG(a0) = 0;		//
			ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
			ATR_FIRST_FLG(a0) = 1;		//??????
		}
		pattern(a0, ATR_STIMER(a0), ANM_LOOP);
		//??????????
		ATR_COUNTER_FLG(a0) = (ATR_COUNTER_FLG(a0) + 1) & 7;
		if (!ATR_COUNTER_FLG(a0))
		{
			ATR_STIMER(a0)--;
			if (ATR_STIMER(a0) == 1)
			{	//??????
				if (ATR_ATTACK_KIND(0, a0) == 0)		//??
					ATR_VCT_NO(a0)++;		//?
				else
				{
					ATR_VCT_NO(a0) = 50;		//??
					ATR_FIRST_FLG(a0) = 0;		//????????
					//?????ＯＫ??
					if (ATR_PET_OK(a0))
					{
						a2 = p_party[ATR_PLACE_NO(a0) + 5];		//????????
						if (ATR_NAME(a2) != NULL)
						{
							ATR_VCT_NO(a2) = 50;		//??????
							ATR_FIRST_FLG(a2) = 0;		//????????
						}
					}
				}
			}
		}
		break;
	case 53:		//战斗逃跑失败(动画减速)
		pattern(a0, ATR_STIMER(a0), ANM_LOOP);
		//??????????
		ATR_COUNTER_FLG(a0) = (ATR_COUNTER_FLG(a0) + 1) & 7;
		if (!ATR_COUNTER_FLG(a0))
		{
			ATR_STIMER(a0)++;
			if (ATR_STIMER(a0) == 12)
			{	//????????
				ATR_STIMER(a0) = 60;
				ATR_VCT_NO(a0)++;
				//显示失败
				set_damage_num(a0, 8, -64);
			}
		}
		break;
	case 54:		//??????
		ATR_CHR_ACT(a0) = ANIM_SAD;		//???????????
		pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP);
		if (!--ATR_STIMER(a0))
		{	//??????
			ATR_DAMAGE(p_master)++;		//?
			//?????ＯＫ??
			if (ATR_PET_OK(a0))
			{
				a2 = p_party[ATR_PLACE_NO(a0) + 5];		//????????
				if (ATR_NAME(a2) != NULL)		//
					ATR_DAMAGE(p_master)++;		//????
			}
			if (ATR_GROUP_FLG(a0) == 0)
				ATR_CHR_ANG(a0) = 3;
			else if (ATR_GROUP_FLG(a0) == 1)
				ATR_CHR_ANG(a0) = 7;
#ifdef _BATTLE_PK_PLAYER_FOR_40
			else if (ATR_GROUP_FLG(a0) == 2)
				ATR_CHR_ANG(a0) = 5;
			else if (ATR_GROUP_FLG(a0) == 3)	
				ATR_CHR_ANG(a0) = 1;
#endif
			ATR_VCT_NO(a0) = 0;		//??
		}
		break;
	case 55:		//打飞
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			ATR_SPD(a0) = 63;		//????
			if (ATR_AKO_FLG(a0) == 2)		//??????
				ATR_LONG_WORK(0, a0) = 0;		//???????
			else
			{
				ATR_LONG_WORK(0, a0) = 3;		//???????
				//打飞的声音
				play_se(11, ATR_H_POS(a0), ATR_V_POS(a0));
			}
			ATR_FIRST_FLG(a0) = 1;		//??????
		}
		gemini(a0);
		if (ATR_LONG_WORK(0, a0) < 3)
		{	//????
			if (ATR_CRS(a0) < 8 || ATR_CRS(a0) > 24)
			{
				if (ATR_V_POS(a0) <= 0)
				{		//?
					d0 = 0;
					goto monster_case55_100;		//??????
				}
			}
			if (ATR_CRS(a0) < 16 && ATR_CRS(a0) > 0)
			{
				if (ATR_H_POS(a0) >= lpDraw->xSize)
				{		//??
					d0 = 1;
					goto monster_case55_100;		//??????
				}
			}
			if (ATR_CRS(a0) > 8 && ATR_CRS(a0) < 24)
			{
				if (ATR_V_POS(a0) >= lpDraw->ySize)
				{		//??
					d0 = 2;
					goto monster_case55_100;		//??????
				}
			}
			if (ATR_CRS(a0) > 16)
			{
				if (ATR_H_POS(a0) <= 0)
				{		//??
					d0 = 3;
					goto monster_case55_100;		//??????
				}
			}
			goto monster_case55_200;		//??????

		monster_case55_100:
			d1 = ATR_CRS(a0);
			d1 = ATR_CRS(a0) = crs_bound_tbl[d0][ATR_CRS(a0)];		//??
			ATR_LONG_WORK(0, a0)++;
			//???
			play_se(66, ATR_H_POS(a0), ATR_V_POS(a0));

		monster_case55_200:
			;
		}
		else
		{
			if (ATR_H_POS(a0) > lpDraw->xSize + SCREEN_OUT || ATR_H_POS(a0) < 0 - SCREEN_OUT || ATR_V_POS(a0) > lpDraw->ySize + SCREEN_OUT || ATR_V_POS(a0) < 0 - SCREEN_OUT)
			{		//?䴘???
				ATR_NAME(a0) = NULL;
				ATR_ATTRIB(a0) |= ACT_ATR_HIDE;		//??
				ATR_VCT_NO(a0) = VCT_NO_APPEAR;
				if (BattleMyNo == ATR_PLACE_NO(a0))
				{	//??
					action_inf = 2;		////ACTION_INF
					command_point = 0;		//?????????????
					break;
				}
				//????????
				a1 = ATR_BODY_WORK(0, a0);
				if (a1 != NULL)
				{
					//?????
					if (ATR_ATTACK_KIND(2, a1) == 1)
					{
						//????
						ATR_ATTRIB(a1) |= ACT_ATR_HIDE;		//??
						ATR_NAME(a1) = NULL;
						ATR_VCT_NO(a1) = VCT_NO_APPEAR;
						if (ATR_COUNTER(p_master) == a0)		//?????
#ifdef _PETSKILL_BATTLE_MODEL
						if (ATR_BATTLE_MODEL(a0) != ATT_BATTLE_MODEL)
#endif
							ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???
					}
					else
					{
						ATR_VCT_NO(a1) = 57;		//?????
						ATR_FIRST_FLG(a1) = 0;
					}
				}
				else
				{
					if (ATR_COUNTER(p_master) == a0)		//?????
					{
#ifdef _PETSKILL_BATTLE_MODEL
						if (ATR_BATTLE_MODEL(a0) != ATT_BATTLE_MODEL)
#endif
							ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???
					}
				}
				break;
			}
		}
		if (ATR_AKO_FLG(a0) == 2)
		{	//??????
			if (ATR_LONG_WORK(0, a0) & 1)
				ATR_CHR_ANG(a0)--;		//??
			else
				ATR_CHR_ANG(a0)++;		//??
			ATR_CHR_ANG(a0) &= 7;		//?
			pattern(a0, 1, ANM_NO_LOOP);
		}
		break;
	case 56:		//?????????
		a1 = ATR_BODY_WORK(0, a0);		//????????
		//?䴘??
		if (ATR_H_POS(a1) < lpDraw->xSize && ATR_H_POS(a1) > 64 && ATR_V_POS(a1) < lpDraw->ySize && ATR_V_POS(a1) > 0 - 64)
		{
			d0 = ATR_H_POS(a1);				//?????
			d1 = ATR_V_POS(a1);
			radar2(a0, d0, d1, 2);	//????
			//????????
			d0 = ATR_CRS(a0) - ATR_CRS_OLD(a0);
			if (d0 < 0)
				d0 = 0 - d0;
			if (d0 >= 2)
			{
				ATR_CHR_ANG(a0) = crs_change_tbl[ATR_CRS(a0)];		/* ????? */
				ATR_CRS_OLD(a0) = ATR_CRS(a0);
			}
		}
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 57:		//???????
		if (ATR_FIRST_FLG(a0) == 0)		//???????
		{
			ATR_SPD(a0) = 16;
			if (ATR_LIFE(a0) <= 0)
			{		//??????
				if (ATR_GROUP_FLG(a0) == 0)		//????????
					//????
					ATR_CRS(a0) = crs_change_tbl2[7];
				else
					ATR_CRS(a0) = crs_change_tbl2[3];
			}
			else
			{
				if (ATR_GROUP_FLG(a0) == 0)
					ATR_CHR_ANG(a0) = 7;
				else if (ATR_GROUP_FLG(a0) == 1)
					ATR_CHR_ANG(a0) = 3;
#ifdef _BATTLE_PK_PLAYER_FOR_40
				else if (ATR_GROUP_FLG(a0) == 2)
					ATR_CHR_ANG(a0) = 1;
				else if (ATR_GROUP_FLG(a0) == 3)	
					ATR_CHR_ANG(a0) = 5;
#endif
				ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
			}
			//??????????????
			ATR_FIRST_FLG(a0) = 1;		//??????
		}
		gemini(a0);		//?
		//??????????????
		if (ATR_LIFE(a0) > 0)		//?????
		{
			ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
			pattern(a0, 1, ANM_LOOP);
		}
		if (ATR_H_POS(a0) > lpDraw->xSize + SCREEN_OUT || ATR_H_POS(a0) < 0 - SCREEN_OUT)		//?䴘???
		{
			ATR_NAME(a0) = NULL;
			ATR_ATTRIB(a0) |= ACT_ATR_HIDE;		//??
			ATR_VCT_NO(a0) = VCT_NO_APPEAR;
			//??????????????????????????????????????????????
			if (check_all_dead())
				ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???
			if (ATR_COUNTER(p_master) == ATR_BODY_WORK(0, a0))		//????????
			{
#ifdef _PETSKILL_BATTLE_MODEL
				if (ATR_BATTLE_MODEL(a0) != ATT_BATTLE_MODEL)
#endif
					ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???
			}
			if (ATR_COUNTER(p_master) == a0)		//?????
			{
#ifdef _PETSKILL_BATTLE_MODEL
				if (ATR_BATTLE_MODEL(a0) != ATT_BATTLE_MODEL)
#endif
					ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???
			}
		}
		break;
	case 60:		//?????
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			a1 = ATR_BODY_WORK(0, a0);		//????????
			set_damage_num(a1, 10, -64);		//??
			ATR_STIMER(a0) = 40;
			play_se(216, ATR_H_POS(a0), 240);		//??????
			if (ATR_GROUP_FLG(a1) == 0)
				ATR_CHR_ANG(a1) = 3;
			else if (ATR_GROUP_FLG(a1) == 1)
				ATR_CHR_ANG(a1) = 7;
#ifdef _BATTLE_PK_PLAYER_FOR_40
			else if (ATR_GROUP_FLG(a1) == 2)
				ATR_CHR_ANG(a1) = 5;
			else if (ATR_GROUP_FLG(a1) == 3)	
				ATR_CHR_ANG(a1) = 1;
#endif
			ATR_CRS_OLD(a1) = ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];
			ATR_FIRST_FLG(a0) = 1;		//??????
		}
		//????
		if (!--ATR_STIMER(a0))
		{
			ATR_FIRST_FLG(a0) = 0;		//??????
			ATR_VCT_NO(a0)++;		//??
		}
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;

	case 61:		//?????
		a1 = ATR_BODY_WORK(0, a0);		//????????
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			ATR_SPD(a0) = 12;
			if (ATR_LIFE(a0) > 0)
			{	//?????
				d0 = ATR_INT_WORK0(a1);
				d1 = ATR_INT_WORK1(a1);
				radar(a0, &d0, &d1);	//????
				ATR_CRS(a0) = d0;		//??????
				ATR_CHR_ANG(a0) = crs_change_tbl[ATR_CRS(a0)];		/* ????? */
				ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
			}
			ATR_FIRST_FLG(a0) = 1;		//??????
		}
		d0 = ATR_INT_WORK0(a1);
		d1 = ATR_INT_WORK1(a1);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		gemini(a0);		//?
		if (d1 <= (ATR_SPD(a0) >> 2))
		{	//????
			//????
			ATR_ATTRIB(a0) |= ACT_ATR_HIDE;		//??
			ATR_NAME(a0) = NULL;
			ATR_VCT_NO(a0) = VCT_NO_APPEAR;
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???
			break;
		}
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 62:		//????
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			a1 = ATR_BODY_WORK(0, a0);		//????????
			set_damage_num(a1, 11, -64);		//??
			if (ATR_GROUP_FLG(a1) == 0)
				ATR_CHR_ANG(a1) = 3;
			else if (ATR_GROUP_FLG(a1) == 1)
				ATR_CHR_ANG(a1) = 7;
#ifdef _BATTLE_PK_PLAYER_FOR_40
			else if (ATR_GROUP_FLG(a1) == 2)
				ATR_CHR_ANG(a1) = 5;
			else if (ATR_GROUP_FLG(a1) == 3)	
				ATR_CHR_ANG(a1) = 1;
#endif
			ATR_CRS_OLD(a1) = ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];
			ATR_STIMER(a0) = 40;
			//???????
			ATR_CHR_NO(a0) = get_num();
			//??????
			ATR_LEVEL(a0) = get_num();
			//?????
			ATR_LIFE(a0) = get_num();
			//䦶???
			get_name(a0);
			//??????
			ATR_MAX_LIFE(a0) = get_num();
			//????????
			a1 = ATR_BODY_WORK(0, a0);
			//????
			ATR_H_POS(a0) = ATR_H_POS(a1);
			ATR_V_POS(a0) = ATR_V_POS(a1) - 1;
			//??
			ATR_ATTRIB(a0) &= ~ACT_ATR_HIDE;
			ATR_SPD(a0) = 12;
			d0 = ATR_INT_WORK0(a0);
			d1 = ATR_INT_WORK1(a0);
			radar(a0, &d0, &d1);	//????
			ATR_CRS(a0) = d0;		//??????
			ATR_CHR_ANG(a0) = crs_change_tbl[ATR_CRS(a0)];		/* ????? */
			ATR_CHR_ACT(a0) = ANIM_STAND;		//???????
			ATR_CHR_ACT_OLD(a0) = -1;
			ATR_AKO_FLG(a0) = 0;		//?????????
			ATR_ATTACK_KIND(2, a0) = 0;
			ATR_STATUS(a0) = 0;		//?????????
			play_se(216, ATR_H_POS(a0), 240);		//??????
			ATR_FIRST_FLG(a0) = 1;		//??????
		}
		//????
		if (!--ATR_STIMER(a0))
		{
			ATR_FIRST_FLG(a0) = 0;		//??????
			ATR_VCT_NO(a0)++;		//??
		}
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 63:		//????
		d0 = ATR_INT_WORK0(a0);
		d1 = ATR_INT_WORK1(a0);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		gemini(a0);		//?
		if (d1 <= (ATR_SPD(a0) >> 2))
		{		//????
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
			ATR_H_POS(a0) = ATR_INT_WORK0(a0);		//?
			ATR_V_POS(a0) = ATR_INT_WORK1(a0);
			ATR_VCT_NO(a0) = 0;		//??
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 64:		//??????
		a1 = ATR_BODY_WORK(1, a0);		//?????
		//??????????
		if (ATR_VCT_NO(a1) != 2)
		{
			ATR_VCT_NO(a0) = 13;		//???????
			ATR_STIMER(a0) = 30;
		}
		break;
	case 65:		//????
		break;
	case 66:		//???
		a1 = ATR_BODY_WORK(0, a0);		//????????
		if (ATR_FIRST_FLG(a0) == 0)		//???????
		{
			ATR_SPD(a0) = 28;
			ATR_FIRST_FLG(a0) = 1;		//??????
		}
		d0 = ATR_INT_WORK0(a1);
		d1 = ATR_INT_WORK1(a1);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		gemini(a0);		//?
		if (d1 <= (ATR_SPD(a0) >> 1))		//????
		{
			//?????
			ATR_VCT_NO(a1) = 67;
			ATR_CRS(a1) = ATR_CRS(a0);		//???????
			ATR_SPD(a1) = 40;
			ATR_STIMER(a1) = 8;
			//???
#ifdef _PETSKILL_BATTLE_MODEL
			// 若是被攻击物件攻击
			if (ATR_BATTLE_MODEL(a1) == ATT_BATTLE_MODEL)
				a1 = ATR_BODY_WORK(1, a1);	// 从 ATR_BODY_WORK(1,a1) 取出攻击物件的 action
			else 
#endif
				a1 = ATR_COUNTER(p_master);
			ATR_BODY_WORK(0, a1) = a0;		//?????
			//???????
			ATR_VCT_NO(a0) = 65;
			//????
			play_se(5, ATR_H_POS(a0), 240);
		}
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 67:		//??????
		gemini(a0);		//??????
		ATR_CRS(a0) = (ATR_CRS(a0) + 16) & 31;
		if (--ATR_STIMER(a0))		//?????
			break;
		ATR_VCT_NO(a0) = 68;
		//????
#ifdef _PETSKILL_BATTLE_MODEL
		// 若是被攻击物件攻击
		if (ATR_BATTLE_MODEL(a0) == ATT_BATTLE_MODEL)
			a1 = ATR_BODY_WORK(1, a0);	// 从 ATR_BODY_WORK(1, a0) 取出攻击物件的 action
		else 
#endif
			a1 = ATR_COUNTER(p_master);		//?????????
		ATR_VCT_NO(a1) = 2;
		break;
	case 68:		//??????
		gemini(a0);		//?
		ATR_SPD(a0) -= 2;
		if (ATR_SPD(a0) < 0)		//?????????
		{
			ATR_SPD(a0) = 10;
			ATR_VCT_NO(a0) = 64;		//???????
		}
		break;
	case 69:		//??????
		if (ATR_FIRST_FLG(a0) == 0)
		{		//???????
			a1 = ATR_BODY_WORK(0, a0);		//????????
			d0 = ATR_INT_WORK0(a1);
			d1 = ATR_INT_WORK1(a1);
			radar(a0, &d0, &d1);	//????
			ATR_CRS(a0) = d0;		//??????
			ATR_CHR_ANG(a0) = crs_change_tbl[ATR_CRS(a0)];		/* ????? */
			ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
			pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
			//??
			set_damage_num(a0, 13, -64);
			ATR_STIMER(a0) = 35;
			ATR_FIRST_FLG(a0) = 1;		//??????
		}
		if (--ATR_STIMER(a0))		//?????
			break;
		ATR_VCT_NO(a0) = 66;		//???
		ATR_FIRST_FLG(a0) = 0;		//??????
		break;
	case 70:		//???????
		if (ATR_FIRST_FLG(a0) == 0)		//???????
		{
			int d2;

			d1 = command_point;		//?????????
			d0 = get_num();		//??
			command_point = d1;		//???????????
			d0 /= 5;
			if (ATR_GROUP_FLG(a0) == 0)		//????????
			{
				d1 = boomerang_pos_tbl[d0 * 2];
				d2 = boomerang_pos_tbl[d0 * 2 + 1];
			}
			else
			{
				//?????????
				if (ATR_PLACE_NO(a0) >= 15)
				{
					d1 = boomerang_pos_tbl3b[d0 * 2];
					d2 = boomerang_pos_tbl3b[d0 * 2 + 1];
				}
				else
				{
					d1 = boomerang_pos_tbl3a[d0 * 2];
					d2 = boomerang_pos_tbl3a[d0 * 2 + 1];
				}
			}
			radar(a0, &d1, &d2);	//????
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = d1;
			ATR_CHR_ANG(a0) = crs_change_tbl[ATR_CRS(a0)];		/* ????? */
#if 0
			ATR_CHR_ACT(a0) = ANIM_THROW;	//?????????
#else
			if (ATR_LONG_WORK(0, a0) == 0)		//?????
				ATR_CHR_ACT(a0) = ANIM_ATTACK;		//?????????
			else
				ATR_CHR_ACT(a0) = ANIM_THROW;		//?????????
#endif

			ATR_FIRST_FLG(a0) = 1;		//??????
		}
		ATR_HIT(a0) = 0;		//?????????
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))	//????????
		{
			ATR_CHR_ACT_OLD(a0) = -1;
			ATR_VCT_NO(a0) = 71;		//?????????
			break;
		}
		//???????
		//?????????
		if(ATR_HIT(a0) >= 10000 && ATR_HIT(a0) < 10100)
		{
			//????????
			a1 = GetAction(T_PRIO_BOW, sizeof(ATR_EQU));
			if (a1 == NULL)
			{
				ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
				ATR_VCT_NO(a0) = 0;		//??
				return;
			}
			//?????????
			ATR_BODY_WORK(0, a0) = a1;		//?????????
			/* ??? */
			ATR_NAME(a1) = boomerang;
			/* ? */
			ATR_DISP_PRIO(a1) = D_PRIO_MISSILE;
			/* ?????? */
			ATR_CHR_NO(a1) = SPR_boomerang;//SPR_ono;//
			/* ??? */
			ATR_CHR_ANG(a1) = ATR_CHR_ANG(a0);
			ATR_BODY_WORK(1, a1) = a0;		//?????
			d1 = command_point;		//?????????
			d0 = get_num();		//??
			command_point = d1;		//???????????
			//?????
			ATR_LONG_WORK(0, a1) = d0 / 5;
			//?????
			if (ATR_GROUP_FLG(a0) == 0)
			{
				if (ATR_LONG_WORK(0, a1) <= 1)
					ATR_LONG_WORK(1, a1) = 1;
				else
					ATR_LONG_WORK(1, a1) = 2;
			}
			else
			{
				if (ATR_LONG_WORK(0, a1) <= 1)
					ATR_LONG_WORK(1, a1) = 2;
				else
					ATR_LONG_WORK(1, a1) = 1;
			}
			ATR_GROUP_FLG(a1) = ATR_GROUP_FLG(a0);
			ATR_PLACE_NO(a1) = ATR_PLACE_NO(a0);
			ATR_H_POS(a1) = ATR_H_POS(a0);
			ATR_V_POS(a1) = ATR_V_POS(a0);
			ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];
			ATR_SPD(a1) = 32;
			gemini(a1);
			gemini(a1);
			gemini(a1);
			p_missile[0] = a1;
			p_missile[1] = NULL;
		}
		break;
	case 71:		//????????
		ATR_CHR_ACT(a0) = ANIM_STAND;		//???????
		a1 = ATR_BODY_WORK(0, a0);		//????????????

		//?䴘??
		d0 = ATR_H_POS(a1);				//???
		d1 = ATR_V_POS(a1);
		radar2(a0, d0, d1, 2);	//????
		//????????
		d0 = ATR_CRS(a0) - ATR_CRS_OLD(a0);
		if (d0 < 0)
			d0 = 0 - d0;
		if (d0 >= 2)
		{
			ATR_CHR_ANG(a0) = crs_change_tbl[ATR_CRS(a0)];		/* ????? */
			ATR_CRS_OLD(a0) = ATR_CRS(a0);
		}
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 72:		//????????
		ATR_CHR_ACT(a0) = ANIM_HAND;		//?????????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		ATR_VCT_NO(a0) = 73;		//??????????
		ATR_STIMER(a0) = 16;
		break;
	case 73:		//?????????
		if (!(--ATR_STIMER(a0)))		//???
		{
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
			ATR_VCT_NO(a0) = 0;		//?
		}
		break;
	case 75:		//?䞍
		if (ATR_GROUP_FLG(a0) == 0)
			ATR_CHR_ANG(a0) = 3;
		else if (ATR_GROUP_FLG(a0) == 1)
			ATR_CHR_ANG(a0) = 7;
#ifdef _BATTLE_PK_PLAYER_FOR_40
		else if (ATR_GROUP_FLG(a0) == 2)
			ATR_CHR_ANG(a0) = 5;
		else if (ATR_GROUP_FLG(a0) == 3)	
			ATR_CHR_ANG(a0) = 1;
#endif
		ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
		//?????
		a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
		if (a1 == NULL)
		{
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
			ATR_VCT_NO(a0) = 0;		//??
			return;
		}
		/* ??? */
		ATR_NAME(a1) = monster;
		ATR_VCT_NO(a1) = 76;
		/* ? */
		ATR_DISP_PRIO(a1) = D_PRIO_HIT_MARK;
		/* ?????? */
		ATR_CHR_NO(a1) = get_num();		//?????????
#ifdef _VARY_WOLF
		if (ATR_CHR_NO(a1) == 101120)
			ATR_DISP_PRIO(a1) = ATR_DISP_PRIO(a0) - 1;
#endif
#ifdef _PROFESSION_ADDSKILL
		if ( !g_bUseAlpha ){
			if ( ATR_CHR_NO(a1) == 101769 || ATR_CHR_NO(a1) == 101772 || ATR_CHR_NO(a1) == 101773 )
				ATR_CHR_NO(a1) += 55;
			if ( ATR_CHR_NO(a1) == 101802 || ATR_CHR_NO(a1) == 101803 || ATR_CHR_NO(a1) == 101804 )
				ATR_CHR_NO(a1) += 53;
		}
#endif
		/* ??? */
		LoadBmp(ATR_PAT_NO(a0));
		//???????
#ifndef __CARYTEST
		ATR_INT_WORK0(a1) = a0->anim_y + SpriteInfo[ATR_PAT_NO(a0)].height / 2;
#else
		ATR_INT_WORK0(a1) = a0->anim_y + g_lpRealAdrn[ATR_PAT_NO(a0)].height / 2;
#endif
		//???????????
		ATR_BODY_WORK(0, a1) = a0;
		ATR_V_POS(a1) = ATR_V_POS(a0) + ATR_INT_WORK0(a1);
		ATR_H_POS(a1) = ATR_H_POS(a0);
		//??
		ATR_VCT_NO(a0) = 0;
		ATR_CHR_ACT_OLD(a0) = -1;
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);	//???
		break;
	case 76:		//?䞍?
		//?
		a1 = ATR_BODY_WORK(0, a0);
		ATR_V_POS(a0) = ATR_V_POS(a1) + ATR_INT_WORK0(a0);
		//?????????
		if (ATR_CHR_NO(a0) == SPR_effect01)
			d0 = pattern(a0, 5, ANM_NO_LOOP);
		else
			d0 = pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP);
		if (d0)
		{	//??????
			DeathAction(a0);		//?
			d0 = get_num();		//?????????
			d6 = 0;		//?????????
#ifdef _VARY_WOLF
			if (d0 >= 101429 && d0 <= 101436)
				a1->anim_chr_no = 0;
#endif
#ifdef _PROFESSION_ADDSKILL
			if (!g_bUseAlpha)
			{
				if (d0 == 101769 || d0 == 101772 || d0 == 101773)
					d0 += 55;
				if (d0 == 101802 || d0 == 101803 || d0 == 101804)
					d0 += 53;
			}
#endif
			while (1)
			{
				d1 = get_num();		//???
				if (d1 == 255)
				{	//???
					if (d6 == 0)		//???
						ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
					break;
				}
				a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
				if (a1 == NULL)
				{
					ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
					return;
				}
				//???????
				if (d1 < 0 || d1 >= BATTLKPKPLYAERNUM)
				{
					return;
				}
				a2 = p_party[d1];		//??????????
				ATR_DISP_PRIO(a1) = D_PRIO_HIT_MARK;
				ATR_NAME(a1) = monster;
				ATR_VCT_NO(a1) = 77;
				ATR_CHR_NO(a1) = d0;		//??????
				/* ??? */
				LoadBmp(ATR_PAT_NO(a2));
				//???????
#ifndef __CARYTEST
				ATR_INT_WORK0(a1) = a2->anim_y + SpriteInfo[ATR_PAT_NO(a2)].height / 2;
#else
				ATR_INT_WORK0(a1) = a2->anim_y + g_lpRealAdrn[ATR_PAT_NO(a2)].height / 2;
#endif
				//???????????
				ATR_BODY_WORK(0, a1) = a2;
				ATR_V_POS(a1) = ATR_V_POS(a2) + ATR_INT_WORK0(a1);
				ATR_H_POS(a1) = ATR_H_POS(a2);
				d6++;		//???????
			}
			return;
		}
		break;
	case 77:		//?䞍?
		//?
		a1 = ATR_BODY_WORK(0, a0);
		ATR_V_POS(a0) = ATR_V_POS(a1) + ATR_INT_WORK0(a0);
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{	//??????
			DeathAction(a0);		//?
#ifdef _VARY_WOLF
			if (a0->anim_chr_no_bak >= 101429 && a0->anim_chr_no_bak <= 101436)
				a1->anim_chr_no = 101428;
#endif
#ifdef _PETSKILL_EVOLUTION
			if ( a0->anim_chr_no_bak==101863 ){ //碰到烟雾图案就直接变身
				if( a1->anim_chr_no == 102009 )
					a1->anim_chr_no = 102011;
				else if( a1->anim_chr_no == 102010 )
					a1->anim_chr_no = 102012;
			}
#endif
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
			return;
		}
		break;
	case 78:		//???????
		ATR_FIRST_FLG(a0)++;
		if (ATR_FIRST_FLG(a0) == 60)
		{
			ATR_FIRST_FLG(a0) = 0;
			if (ATR_LIFE(a0) <= 0)
			{	//??????
				ATR_VCT_NO(a0) = VCT_NO_DIE + 1;		//??
				slow_flg--;		//?????
			}
			else
				ATR_VCT_NO(a0) = 0;
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
		}
		break;
	case 79:		//??
		ATR_LONG_WORK(0, a0)++;
		if (ATR_LONG_WORK(0, a0) == 60)
		{
			ATR_VCT_NO(a0) = 0;
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
		}
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);	//???
		break;
	case 80:		//佋??
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			if (ATR_GROUP_FLG(a0) == 0)
				ATR_CHR_ANG(a0) = 7;
			else if (ATR_GROUP_FLG(a0) == 1)
				ATR_CHR_ANG(a0) = 3;
#ifdef _BATTLE_PK_PLAYER_FOR_40
			else if (ATR_GROUP_FLG(a0) == 2)
				ATR_CHR_ANG(a0) = 1;
			else if (ATR_GROUP_FLG(a0) == 3)	
				ATR_CHR_ANG(a0) = 5;
#endif
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
			ATR_SPD(a0) = 32;
			ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
			ATR_FIRST_FLG(a0) = 1;		//??????
		}
		gemini(a0);		//?
		if (ATR_H_POS(a0) > lpDraw->xSize + SCREEN_OUT || ATR_H_POS(a0) < 0 - SCREEN_OUT)
		{	//?䴘???
			ATR_VCT_NO(a0) = 0;		//?
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
		}
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);	//???
		break;
	case 82:		//???
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			if (ATR_GROUP_FLG(a0) == 0)
				ATR_CHR_ANG(a0) = 7;
			else if (ATR_GROUP_FLG(a0) == 1)
				ATR_CHR_ANG(a0) = 3;
#ifdef _BATTLE_PK_PLAYER_FOR_40
			else if (ATR_GROUP_FLG(a0) == 2)
				ATR_CHR_ANG(a0) = 1;
			else if (ATR_GROUP_FLG(a0) == 3)	
				ATR_CHR_ANG(a0) = 5;
#endif
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
			ATR_CHR_ACT(a0) = ANIM_SIT;		//?????????
			ATR_CHR_ACT_OLD(a0) = -1;
			pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);	//???
			ATR_STIMER(a0) = 60;
			ATR_FIRST_FLG(a0) = 1;		//??????
		}
		if (!--ATR_STIMER(a0))
		{	//???
			ATR_CHR_NO(a0) = get_num();
			ATR_CHR_ACT_OLD(a0) = -1;
			if (ATR_GROUP_FLG(a0) == 0)
				ATR_CHR_ANG(a0) = 3;
			else if (ATR_GROUP_FLG(a0) == 1)
				ATR_CHR_ANG(a0) = 7;
#ifdef _BATTLE_PK_PLAYER_FOR_40
			else if (ATR_GROUP_FLG(a0) == 2)
				ATR_CHR_ANG(a0) = 5;
			else if (ATR_GROUP_FLG(a0) == 3)	
				ATR_CHR_ANG(a0) = 1;
#endif
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
			ATR_CHR_ACT(a0) = ANIM_STAND;
			ATR_CHR_ACT_OLD(a0) = -1;
			pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);	//???
			ATR_VCT_NO(a0) = 0;
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
		}
		break;
	case 85:		//??????
		if (ATR_GROUP_FLG(a0) == 0)
			ATR_CHR_ANG(a0) = 3;
		else if (ATR_GROUP_FLG(a0) == 1)
			ATR_CHR_ANG(a0) = 7;
#ifdef _BATTLE_PK_PLAYER_FOR_40
		else if (ATR_GROUP_FLG(a0) == 2)
			ATR_CHR_ANG(a0) = 5;
		else if (ATR_GROUP_FLG(a0) == 3)	
			ATR_CHR_ANG(a0) = 1;
#endif
		ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
		//?????
		a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
		if (a1 == NULL)
		{
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
			ATR_VCT_NO(a0) = 0;		//??
			return;
		}
		/* ??? */
		ATR_NAME(a1) = monster;
		ATR_VCT_NO(a1) = 86;
		/* ? */
		ATR_DISP_PRIO(a1) = D_PRIO_HIT_MARK;
		/* ?????? */
		ATR_CHR_NO(a1) = SPR_effect01;
		/* ??? */
		LoadBmp(ATR_PAT_NO(a0));
		//???????
#ifndef __CARYTEST
		ATR_INT_WORK0(a1) = a0->anim_y + SpriteInfo[ATR_PAT_NO(a0)].height / 2;
#else
		ATR_INT_WORK0(a1) = a0->anim_y + g_lpRealAdrn[ATR_PAT_NO(a0)].height / 2;
#endif
		//???????????
		ATR_BODY_WORK(0, a1) = a0;
		ATR_V_POS(a1) = ATR_V_POS(a0) + ATR_INT_WORK0(a1);
		ATR_H_POS(a1) = ATR_H_POS(a0);
		//??
		ATR_VCT_NO(a0) = 0;
		ATR_CHR_ACT_OLD(a0) = -1;
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);	//???
		break;
	case 86:		//???????
		//?????????
		if (ATR_CHR_NO(a0) == SPR_effect01)
			d0 = pattern(a0, 5, ANM_NO_LOOP);
		else
			d0 = pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP);
		if (d0)
		{	//??????
			//?
			ATR_ATTRIB(p_master) = get_num();		//??
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???
			DeathAction(a0);		//?
			return;
		}
		break;
	case 87:		//?????
		ATR_SPD(a0) = 32;		//?speed
		a1 = p_party[ATR_PLACE_NO(a0) - 5];		//????//敌方
		d0 = ATR_H_POS(a1);//y
		d1 = ATR_V_POS(a1);//x
		radar(a0, &d0, &d1);	//????//d0:course1  d1:distance
		ATR_CRS(a0) = d0;		//??????
		//????????
		d0 = ATR_CRS(a0) - ATR_CRS_OLD(a0);
		if (d0 < 0)
			d0 = 0 - d0;
		if (d0 >= 2)
		{
#ifdef _SKILL_ROAR  //宠技:大吼(克年兽)//修正逃跑方向
#else
			ATR_CHR_ANG(a0) = crs_change_tbl[ATR_CRS(a0)];		/* ????? */
			ATR_CRS_OLD(a0) = ATR_CRS(a0);
#endif
		}
		if (d1 <= 32 * 2)
		{	//????
			ATR_FIRST_FLG(a0) = 0;		//????????
			ATR_VCT_NO(a0) = 88;		//??????
		}
		else
			gemini(a0);		//?
		ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 88:		//?????
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			ATR_STIMER(a0) = 40;
#ifdef _SKILL_ROAR  //宠技:大吼(克年兽)//修正逃跑方向
			//????
			ATR_SPD(a0) = 60;
			ATR_CHR_ACT(a0) = ANIM_DAMAGE;		//???????
#else
			//????
			ATR_SPD(a0) = 40;
			ATR_CHR_ACT(a0) = ANIM_STAND;		//???????
#endif
			ATR_FIRST_FLG(a0) = 1;		//??????
		}
		//????
		if (!ATR_STIMER(a0)--)
		{
			ATR_VCT_NO(a0) = 89;		//???????
			ATR_STIMER(a0) = 40;
			//????
			set_damage_num(a0, 17, -64);
#ifdef _SKILL_ROAR  //宠技:大吼(克年兽)修正逃跑方向
			if (ATR_GROUP_FLG(a0) == 0)
				ATR_CHR_ANG(a0) = 7;
			else if (ATR_GROUP_FLG(a0) == 1)
				ATR_CHR_ANG(a0) = 3;
#ifdef _BATTLE_PK_PLAYER_FOR_40
			else if (ATR_GROUP_FLG(a0) == 2)
				ATR_CHR_ANG(a0) = 1;
			else if (ATR_GROUP_FLG(a0) == 3)	
				ATR_CHR_ANG(a0) = 5;
#endif
#else
			if (ATR_GROUP_FLG(a0) == 0)
				ATR_CHR_ANG(a0) = 3;
			else if (ATR_GROUP_FLG(a0) == 1)
				ATR_CHR_ANG(a0) = 7;
#ifdef _BATTLE_PK_PLAYER_FOR_40
			else if (ATR_GROUP_FLG(a0) == 2)
				ATR_CHR_ANG(a0) = 5;
			else if (ATR_GROUP_FLG(a0) == 3)	
				ATR_CHR_ANG(a0) = 1;
#endif
#endif
			//????
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = (crs_change_tbl2[ATR_CHR_ANG(a0)]) & 31;
		}
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 89:		//??????
		if (ATR_STIMER(a0))
		{
			ATR_STIMER(a0)--;
			break;
		}
		gemini(a0);
		if (ATR_H_POS(a0) > lpDraw->xSize + SCREEN_OUT || ATR_H_POS(a0) < 0 - SCREEN_OUT)
		{	//?䴘???
			ATR_NAME(a0) = NULL;
			ATR_ATTRIB(a0) |= ACT_ATR_HIDE;		//??
			ATR_VCT_NO(a0) = VCT_NO_APPEAR;
			ATR_DAMAGE(p_master)++;		//?
			//??????
			if (BattleMyNo + 5 == ATR_PLACE_NO(a0))
			{
				// ????????????
				if (att_select_flg == TRUE)
				{
					//????
					pc.selectPetNo[pc.battlePetNo] = FALSE;
					pc.battlePetNo = -1;
				}
				else
				{
					// ?????
					if (battlePetNoBak2 == pc.battlePetNo)
					{
						//????
						pc.selectPetNo[pc.battlePetNo] = FALSE;
						pc.battlePetNo = -1;
					}
					else
						//????
						pc.selectPetNo[battlePetNoBak2] = FALSE;
				}
			}
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
		pattern(a0, 1, ANM_LOOP);
		break;
	case 90:		//????
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			//????
			set_damage_num(a0, 18, -64);
			ATR_STIMER(a0) = 0;		//????????
			ATR_CHR_ACT(a0) = ANIM_STAND;		//???????
			ATR_FIRST_FLG(a0) = 1;		//??????
		}
		//???
		d0 = pet_nix_tbl[ATR_STIMER(a0)++];
		if (d0 == -2)
		{	//???
			ATR_VCT_NO(a0) = 0;		//?
			ATR_DAMAGE(p_master)++;		//?
			break;
		}
		ATR_CHR_ANG(a0) = (ATR_CHR_ANG(a0) + d0) & 7;		//???
		pattern(a0, 1, ANM_LOOP);
		break;
	case 95:		//?????
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			ATR_LONG_WORK(0, a0) = get_num();		//????
			ATR_LONG_WORK(1, a0) = get_num();		//???????
			a1 = ATR_BODY_WORK(0, a0) = p_party[ATR_LONG_WORK(0, a0)];		//????
			d0 = ATR_INT_WORK0(a1);
			d1 = ATR_INT_WORK1(a1);
			radar(a0, &d0, &d1);	//????
			ATR_CRS(a0) = d0;		//??????
			ATR_CHR_ANG(a0) = crs_change_tbl[ATR_CRS(a0)];		/* ????? */
			ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
			ATR_SPD(a0) = 32;		//?
			ATR_FIRST_FLG(a0) = 1;		//??????
		}
		a1 = ATR_BODY_WORK(0, a0);		//????????
		d0 = ATR_H_POS(a1);
		d1 = ATR_V_POS(a1);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		gemini(a0);
		if (d1 <= 32 * 2)
		{	//????
			ATR_VCT_NO(a0) = 96;		//??????
			if (ATR_LONG_WORK(1, a0))
			{	//???
				a1 = ATR_BODY_WORK(0, a0);		//????????
				ATR_VCT_NO(a1) = 98;			//
				ATR_SPD(a1) = ATR_SPD(a0);
				ATR_CRS(a1) = ATR_CRS(a0);
			}
		}
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 96:		//?????
		gemini(a0);
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		if (ATR_H_POS(a0) > lpDraw->xSize + SCREEN_OUT || ATR_H_POS(a0) < 0 - SCREEN_OUT		//?䴘???
			|| ATR_V_POS(a0) > lpDraw->ySize + SCREEN_OUT || ATR_V_POS(a0) < 0 - SCREEN_OUT)
		{	//?䴘???
			ATR_NAME(a0) = NULL;
			ATR_ATTRIB(a0) |= ACT_ATR_HIDE;		//??
			ATR_VCT_NO(a0) = VCT_NO_APPEAR;
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???
			//??????
			if (BattleMyNo + 5 == ATR_PLACE_NO(a0))
			{
				// ????????????
				if (att_select_flg == TRUE)
				{
					//????
					pc.selectPetNo[pc.battlePetNo] = FALSE;
					pc.battlePetNo = -1;
				}
				else
				{
					// ?????
					if (battlePetNoBak2 == pc.battlePetNo)
					{
						//????
						pc.selectPetNo[pc.battlePetNo] = FALSE;
						pc.battlePetNo = -1;
					}
					else
						//????
						pc.selectPetNo[battlePetNoBak2] = FALSE;
				}
			}
			if (ATR_LONG_WORK(1, a0))
			{	//???
				a1 = ATR_BODY_WORK(0, a0);		//?????
				ATR_NAME(a1) = NULL;
				ATR_ATTRIB(a1) |= ACT_ATR_HIDE;		//??
				ATR_VCT_NO(a1) = VCT_NO_APPEAR;
				//??????
				if (BattleMyNo + 5 == ATR_LONG_WORK(0, a0))
				{
					// ????????????
					if (att_select_flg == TRUE)
					{
						//????
						pc.selectPetNo[pc.battlePetNo] = FALSE;
						pc.battlePetNo = -1;
					}
					else
					{
						// ?????
						if (battlePetNoBak2 == pc.battlePetNo)
						{
							//????
							pc.selectPetNo[pc.battlePetNo] = FALSE;
							pc.battlePetNo = -1;
						}
						else
							//????
							pc.selectPetNo[battlePetNoBak2] = FALSE;
					}
				}
			}
		}
		break;
	case 98:		//????????????
		gemini(a0);
		break;
	case 100:		//?????
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			ATR_LONG_WORK(0, a0) = get_num();		//????
			ATR_LONG_WORK(1, a0) = get_num();		//???????
			a1 = ATR_BODY_WORK(0, a0) = p_party[ATR_LONG_WORK(0, a0)];		//????
			d0 = ATR_INT_WORK0(a1);
			d1 = ATR_INT_WORK1(a1);
			radar(a0, &d0, &d1);	//????
			ATR_CRS(a0) = d0;		//??????
			ATR_CHR_ANG(a0) = crs_change_tbl[ATR_CRS(a0)];		/* ????? */
			ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
			ATR_SPD(a0) = 32;		//?
			ATR_FIRST_FLG(a0) = 1;		//??????
		}
		a1 = ATR_BODY_WORK(0, a0);		//????????
		d0 = ATR_H_POS(a1);
		d1 = ATR_V_POS(a1);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		gemini(a0);
		if (d1 <= 32 * 2)
		{	//????
			ATR_VCT_NO(a0) = 101;		//?????
			ATR_FIRST_FLG(a0) = 0;		//???????
		}
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 101:		//????
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			ATR_CHR_ACT(a0) = ANIM_ATTACK;		//?????????
			ATR_FIRST_FLG(a0) = 1;		//??????
		}
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{	//????????
			ATR_CHR_ACT_OLD(a0) = -1;
			ATR_VCT_NO(a0) = 102;		//??????
			ATR_FIRST_FLG(a0) = 0;		//???????
		}
		break;
	case 102:		//?????
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			ATR_STIMER(a0) = 30;		//伡?????
			if (ATR_LONG_WORK(1, a0))		//???
				//?
				set_damage_num(a0, 7, -64);
			else
				//
				set_damage_num(a0, 8, -64);
			ATR_FIRST_FLG(a0) = 1;		//??????
		}
		ATR_STIMER(a0)--;
		if (ATR_STIMER(a0) == 0)
		{	//???
			if (ATR_LONG_WORK(1, a0))
			{	//???
				ATR_VCT_NO(a0) = 103;		//????
				if (ATR_GROUP_FLG(a0) == 0)
					ATR_CHR_ANG(a0) = 7;
				else if (ATR_GROUP_FLG(a0) == 1)
					ATR_CHR_ANG(a0) = 3;
#ifdef _BATTLE_PK_PLAYER_FOR_40
				else if (ATR_GROUP_FLG(a0) == 2)
					ATR_CHR_ANG(a0) = 1;
				else if (ATR_GROUP_FLG(a0) == 3)	
					ATR_CHR_ANG(a0) = 5;
#endif
				//????
				ATR_CRS_OLD(a0) = ATR_CRS(a0) = (crs_change_tbl2[ATR_CHR_ANG(a0)]) & 31;
				a1 = ATR_BODY_WORK(0, a0);		//?????????
				ATR_VCT_NO(a1) = 104;
			}
			else
			{
				d0 = ATR_INT_WORK0(a0);		//???
				d1 = ATR_INT_WORK1(a0);
				radar(a0, &d0, &d1);	//????
				ATR_CHR_ANG(a0) = crs_change_tbl[d0];
				ATR_SPD(a0) = 32;
				ATR_VCT_NO(a0) = 4;		//??
			}
		}
		break;
	case 103:		//???
		gemini(a0);
		if (ATR_H_POS(a0) > lpDraw->xSize + SCREEN_OUT || ATR_H_POS(a0) < 0 - SCREEN_OUT)
		{	//?䴘???
			ATR_NAME(a0) = NULL;
			ATR_ATTRIB(a0) |= ACT_ATR_HIDE;		//??
			ATR_VCT_NO(a0) = VCT_NO_APPEAR;
			ATR_DAMAGE(p_master)++;		//?
			a1 = ATR_BODY_WORK(0, a0);		//??????
			ATR_VCT_NO(a1) = 0;
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
		pattern(a0, 1, ANM_LOOP);
		break;
	case 104:		//?????
		ATR_CHR_ACT(a0) = ANIM_ANGRY;		//夂????????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 105:		//???
		if (!--ATR_STIMER(a0))
		{
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???
			ATR_VCT_NO(a0) = 0;		//?
		}
		break;
	case 110:		//????
		if (!--ATR_STIMER(a0))
		{
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???
			ATR_VCT_NO(a0) = 0;		//?
		}
		break;
#ifdef _SKILL_SELFEXPLODE //自爆
	case 114:		//自爆
		ATR_SPD(a0) = 63;
		ATR_ATTRIB(a0) |= ACT_ATR_HIDE;		//??
		d0 = ATR_INT_WORK0(a0);
		d1 = ATR_INT_WORK1(a0);
		gemini(a0);		//?
		if (ATR_H_POS(a0) > lpDraw->xSize + SCREEN_OUT || ATR_H_POS(a0) < 0 - SCREEN_OUT)
		{	//?䴘???
			ATR_VCT_NO(a0) = 0;		//?
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??????
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
#endif
		//249:
	case VCT_NO_APPEAR:		//
		if (ATR_GROUP_FLG(a0) == 0)
			ATR_CHR_ANG(a0) = 3;
		else if (ATR_GROUP_FLG(a0) == 1)
			ATR_CHR_ANG(a0) = 7;
#ifdef _BATTLE_PK_PLAYER_FOR_40
		else if (ATR_GROUP_FLG(a0) == 2)
			ATR_CHR_ANG(a0) = 5;
		else if (ATR_GROUP_FLG(a0) == 3)	
			ATR_CHR_ANG(a0) = 1;
#endif
		ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
		ATR_SPD(a0) = 12;
		d0 = ATR_INT_WORK0(a0);
		d1 = ATR_INT_WORK1(a0);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		gemini(a0);		//?
		if (d1 <= (ATR_SPD(a0) >> 2) || ATR_CHR_NO(a0) == 101490)
		{	//????
			ATR_H_POS(a0) = ATR_INT_WORK0(a0);		//?
			ATR_V_POS(a0) = ATR_INT_WORK1(a0);
			ATR_VCT_NO(a0) = 0;		//??
			ATR_CHR_ACT(a0) = ANIM_STAND;		//???????
			ATR_SPD(a0) = 32;
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
		}
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case VCT_NO_DIE:		//?
		ATR_SPD(a0) = 4;
		d0 = ATR_INT_WORK0(a0);
		d1 = ATR_INT_WORK1(a0);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		gemini(a0);		//?
		if (d1 <= (ATR_SPD(a0) >> 2))
		{	//????
			ATR_H_POS(a0) = ATR_INT_WORK0(a0);		//?
			ATR_V_POS(a0) = ATR_INT_WORK1(a0);
			ATR_VCT_NO(a0) = VCT_NO_DIE + 1;		//?????
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
		if (s_timer & 1)
			pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case VCT_NO_DIE + 1:		//?
		ATR_CHR_ACT(a0) = ANIM_DEAD;		//????????
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{	//??????
#ifdef __NEW_BATTLE_EFFECT
			int effect = ATR_EFFECT_FLAG(a0);
			int effect_anim = ATR_EFFECT_ANIM(a0);

			switch (effect)
			{
			case 1:	//替身娃娃
				a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
				if (a1 == NULL)
				{
					ATR_EFFECT_FLAG(a0) = 0;
					ATR_EFFECT_ANIM(a0) = 0;
					ATR_EFFECT_PARAM(a0) = 0;
					return;
				}
				//???????
				ATR_DISP_PRIO(a1) = D_PRIO_HIT_MARK;
				ATR_NAME(a1) = monster;
				ATR_VCT_NO(a1) = VCT_NO_EFFECT;
				ATR_CHR_NO(a1) = effect_anim;		//??????
				/* ??? */
				LoadBmp(ATR_PAT_NO(a0));
				//???????
#ifndef __CARYTEST
				ATR_INT_WORK0(a1) = a0->anim_y + SpriteInfo[ATR_PAT_NO(a0)].height / 2;
#else
				ATR_INT_WORK0(a1) = a0->anim_y + g_lpRealAdrn[ATR_PAT_NO(a0)].height / 2;
#endif
				//???????????
				ATR_BODY_WORK(0, a1) = a0;
				ATR_V_POS(a1) = ATR_V_POS(a0) + ATR_INT_WORK0(a1);
				ATR_H_POS(a1) = ATR_H_POS(a0);
				break;
			}
#endif
			ATR_VCT_NO(a0) = VCT_NO_DIE + 2;		//??
			//????
			set_piyo_loop(a0);
			//?????
			play_se(6, ATR_H_POS(a0), 240);
		}
		break;
	case VCT_NO_DIE + 2:		//?
		break;
#ifdef __NEW_BATTLE_EFFECT
		//	case VCT_NO_PAUSE:
		//		break;
	case VCT_NO_EFFECT:
	{
						  a1 = ATR_BODY_WORK(0, a0);
						  int effect = ATR_EFFECT_FLAG(a1);

						  switch (effect)
						  {
						  case 1:			//替身娃娃
							  ATR_V_POS(a0) = ATR_V_POS(a1) + ATR_INT_WORK0(a0);
							  if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
							  {	//??????
								  //??
								  ATR_LIFE(a1) = ATR_EFFECT_PARAM(a1);
								  if (ATR_LIFE(a1) > ATR_MAX_LIFE(a1))		//????
									  ATR_LIFE(a1) = ATR_MAX_LIFE(a1);		//????
								  ATR_VCT_NO(a1) = 0;		//?
								  ATR_CHR_ACT(a1) = ANIM_STAND;		//???????
								  pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
								  ATR_EFFECT_FLAG(a1) = 0;
								  ATR_EFFECT_ANIM(a1) = 0;
								  ATR_EFFECT_PARAM(a1) = 0;
								  DeathAction(a0);		//?
								  ATR_DAMAGE(p_master)++;
							  }
							  return;
						  default:
							  ATR_EFFECT_FLAG(a1) = 0;
							  ATR_EFFECT_ANIM(a1) = 0;
							  ATR_EFFECT_PARAM(a1) = 0;
							  DeathAction(a0);		//?
							  ATR_DAMAGE(p_master)++;
							  break;
						  }
	}
		break;
#endif
#ifdef __ATTACK_MAGIC
		// 产生前置动画
	case ATTACK_MAGIC_CASE:
		// 是否在右下方
		(0 == ATR_GROUP_FLG(a0)) ? ATR_CHR_ANG(a0) = 3 : ATR_CHR_ANG(a0) = 7;
		ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
		if (0XFFFFFFFF != AttMgc.iPreMgcNum)
		{
			if (NULL == (a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU))))
			{
				ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);
				ATR_VCT_NO(a0) = 0;
				return;
			}
			ATR_NAME(a1) = monster;
			ATR_VCT_NO(a1) = ATTACK_MAGIC_CASE + 1;		// 前置动画
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
			if (AttMgc.iPreMgcNum == 101120)
				ATR_DISP_PRIO(a1)	= (1 == AttMgc.wPreMgcOnChar) ? D_PRIO_HIT_MARK : DISP_PRIO_TILE + 1;
			else 
				ATR_DISP_PRIO(a1)	= ATR_DISP_PRIO(a0) + 1;
#else
			ATR_DISP_PRIO(a1) = (1 == AttMgc.wPreMgcOnChar) ? D_PRIO_HIT_MARK : DISP_PRIO_TILE + 1;
#endif
			ATR_CHR_NO(a1) = AttMgc.iPreMgcNum;
			ATR_BODY_WORK(0, a1) = a0;
			ATR_H_POS(a1) = ATR_H_POS(a0) + AttMgc.wPreMgcX;
			ATR_V_POS(a1) = ATR_V_POS(a0) + AttMgc.wPreMgcY;
		}
		ATR_VCT_NO(a0) = 0;
		ATR_CHR_ACT_OLD(a0) = -1;
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
		// 执行前置动画
	case ATTACK_MAGIC_CASE + 1:
		a1 = ATR_BODY_WORK(0, a0);
		ATR_H_POS(a0) = ATR_H_POS(a1) + AttMgc.wPreMgcX;
		ATR_V_POS(a0) = ATR_V_POS(a1) + AttMgc.wPreMgcY;
		if (d0 = pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{
			DeathAction(a0);
			AttMgc.wRunPreMgc = TRUE;
			//ToCallMgc.wRunPreMgc = TRUE;
		}
		break;
		// 执行目前的咒术
	case ATTACK_MAGIC_CASE + 2:
	{
								  idx = ATR_LONG_WORK(0, a0);

								  // 播放的是相对于人物的位置
								  if (20 != idx)
								  {
									  a1 = p_party[idx];
									  ATR_H_POS(a0) = ATR_H_POS(a1) + AttMgc.posAttacked[0].x;
									  ATR_V_POS(a0) = ATR_V_POS(a1) + AttMgc.posAttacked[0].y;
								  }
								  // 播放绝对座标的咒术
								  else
								  {
									  int value = 0;

									  // 目前正在地震中
									  if (2 == g_iRunEarthQuake)
										  value = piyo_tbl[(piyo_point + g_iCurRunEarthQuake) & 63] + 12;

									  ATR_V_POS(a0) = AttMgc.posAttacked[0].y + value;
								  }

								  if (d0 = pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
								  {
									  DeathAction(a0);
									  // 如果没有后置动画
									  if (0XFFFFFFFF == AttMgc.iPostMgcNum)
									  {
										  AttMgc.wNumAttackeds++;
										  if (20 == idx)
										  {
											  // wAttackedIndex[0] 放置  20 -- 代表全体攻击
											  // wAttackedIndex[n] 放置0XFF -- 代表结束
											  int i = 0, j, charahurt, pethurt;

											  while (++i && 0XFF != AttMgc.wAttackedIndex[i])
											  {
												  for (j = 0; j < iAttackedNum; j++)
												  {
													  if (AttMgc.wAttackedIndex[i] == AttackedInfo[j * 4])
													  {
														  a1 = p_party[AttackedInfo[j * 4 + 1]];
														  charahurt = AttackedInfo[j * 4 + 2];
														  pethurt = AttackedInfo[j * 4 + 3];
														  ATR_LONG_WORK(0, a1) = 0;
														  ATR_DAMAGE(a1) = charahurt;
														  ATR_PET_DAMAGE(a1) = pethurt;
														  if (charahurt == 0)
														  {
															  set_damage_num(a1, 0, -64);
															  ATR_VCT_NO(a1) = ATTACK_MAGIC_CASE + 5;
														  }
														  else
														  {
															  set_damage_num(a1, 6, -64);
															  ATR_VCT_NO(a1) = ATTACK_MAGIC_CASE + 4;
														  }
														  ATR_LIFE(a1) -= ATR_DAMAGE(a1);
														  set_jujutsu_hit_mark(a1);
														  if (ATR_LIFE(a1) <= 0)
														  {
															  ATR_LIFE(a1) = 0;
															  slow_flg++;
														  }
														  ATR_PET_LIFE(a1) -= ATR_PET_DAMAGE(a1);
														  //andy_fall
														  if (ATR_PETFALL(a1) == 1)
														  {
															  if (ATR_LIFE(a1) > 0)
																  petfallChangeGraph(a1);
														  }
														  if (ATR_PET_LIFE(a1) <= 0 && ATR_RIDE(a1))
														  {
															  ATR_PET_LIFE(a1) = 0;
															  if (ATR_LIFE(a1) > 0)
																  petfallChangeGraph(a1);
														  }
														  if (charahurt == 0)
															  ATR_CHR_ACT(a1) = ANIM_STAND;
														  else
															  ATR_CHR_ACT(a1) = ANIM_DAMAGE;
														  ATR_CHR_ACT_OLD(a1) = -1;
														  pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
													  }
												  }
											  }
										  }
										  else
										  {
											  int j, charahurt, pethurt;

											  for (j = 0; j < iAttackedNum; j++)
											  {
												  if (idx == AttackedInfo[j * 4])
												  {
													  a1 = p_party[AttackedInfo[j * 4 + 1]];
													  charahurt = AttackedInfo[j * 4 + 2];
													  pethurt = AttackedInfo[j * 4 + 3];
													  ATR_VCT_NO(a1) = ATTACK_MAGIC_CASE + 4;
													  ATR_LONG_WORK(0, a1) = 0;
													  ATR_DAMAGE(a1) = charahurt;
													  ATR_PET_DAMAGE(a1) = pethurt;
													  if (charahurt == 0)
													  {
														  set_damage_num(a1, 0, -64);
														  ATR_VCT_NO(a1) = ATTACK_MAGIC_CASE + 5;
													  }
													  else
													  {
														  set_damage_num(a1, 6, -64);
														  ATR_VCT_NO(a1) = ATTACK_MAGIC_CASE + 4;
													  }
													  ATR_LIFE(a1) -= ATR_DAMAGE(a1);
													  set_jujutsu_hit_mark(a1);
													  if (ATR_LIFE(a1) <= 0)
													  {
														  ATR_LIFE(a1) = 0;
														  slow_flg++;
													  }
													  ATR_PET_LIFE(a1) -= ATR_PET_DAMAGE(a1);
													  //andy_fall
													  if (ATR_PETFALL(a1) == 1)
													  {
														  if (ATR_LIFE(a1) > 0)
															  petfallChangeGraph(a1);
													  }
													  if (ATR_PET_LIFE(a1) <= 0 && ATR_RIDE(a1))
													  {
														  ATR_PET_LIFE(a1) = 0;
														  if (ATR_LIFE(a1) > 0)
															  petfallChangeGraph(a1);
													  }
													  if (charahurt == 0)
														  ATR_CHR_ACT(a1) = ANIM_STAND;
													  else
														  ATR_CHR_ACT(a1) = ANIM_DAMAGE;
													  ATR_CHR_ACT_OLD(a1) = -1;
													  pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
												  }
											  }
										  }
									  }
									  // 拥有后置动画
									  else
									  {
										  // 针对所有的敌人施后置动画
										  if (20 == idx) // idx = wAttackedIndex[0]
										  {
											  // wAttackedIndex[0] 放置  20 -- 代表全体攻击
											  // wAttackedIndex[n] 放置0XFF -- 代表结束
											  int i = 0;

											  while (++i && 0XFF != AttMgc.wAttackedIndex[i])
											  {
												  a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));

												  ATR_NAME(a1) = monster;
												  ATR_VCT_NO(a1) = ATTACK_MAGIC_CASE + 3;		// 后置动画
												  ATR_DISP_PRIO(a1) = (1 == AttMgc.wPostMgcOnChar) ? D_PRIO_HIT_MARK : DISP_PRIO_TILE + 1;
												  ATR_CHR_NO(a1) = AttMgc.iPostMgcNum;
												  ATR_LONG_WORK(0, a1) = AttMgc.wAttackedIndex[i];
												  ATR_H_POS(a1) = ATR_H_POS(p_party[AttMgc.wAttackedIndex[i]]) + AttMgc.wPostMgcX;
												  ATR_V_POS(a1) = ATR_V_POS(p_party[AttMgc.wAttackedIndex[i]]) + AttMgc.wPostMgcY;
											  }
										  }
										  // 针对某一个敌人施后置动画
										  else
										  {
											  a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));

											  ATR_NAME(a1) = monster;
											  ATR_VCT_NO(a1) = ATTACK_MAGIC_CASE + 3;		// 后置动画
											  ATR_DISP_PRIO(a1) = (1 == AttMgc.wPostMgcOnChar) ? D_PRIO_HIT_MARK : DISP_PRIO_TILE + 1;
											  ATR_CHR_NO(a1) = AttMgc.iPostMgcNum;
											  ATR_LONG_WORK(0, a1) = idx;
											  ATR_H_POS(a1) = ATR_H_POS(p_party[idx]) + AttMgc.wPostMgcX;
											  ATR_V_POS(a1) = ATR_V_POS(p_party[idx]) + AttMgc.wPostMgcY;
										  }
									  }
								  }

								  // 己经播放所有的动画了
								  if (AttMgc.wNumAttackeds == AttMgc.wNumAttacks && 0 == iAttackedNum)
								  {
									  ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);
									  bRunAttMgc = FALSE;
									  g_iRunEarthQuake = 0;
								  }
								  break;
	}
		// 执行后置动画
	case ATTACK_MAGIC_CASE + 3:
		idx = ATR_LONG_WORK(0, a0);
		a1 = p_party[idx];
		ATR_H_POS(a0) = ATR_H_POS(a1) + AttMgc.wPostMgcX;
		ATR_V_POS(a0) = ATR_V_POS(a1) + AttMgc.wPostMgcY;

		if (d0 = pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{
			DeathAction(a0);
			AttMgc.wNumAttackeds++;
			if (20 == idx)
			{
				// wAttackedIndex[0] 放置  20 -- 代表全体攻击
				// wAttackedIndex[n] 放置0XFF -- 代表结束
				int i = 0, j, charahurt, pethurt;

				while (++i && 0XFF != AttMgc.wAttackedIndex[i])
				{
					for (j = 0; j < iAttackedNum; j++)
					{
						if (AttMgc.wAttackedIndex[i] == AttackedInfo[j * 4])
						{
							a1 = p_party[AttackedInfo[j * 4 + 1]];
							charahurt = AttackedInfo[j * 4 + 2];
							pethurt = AttackedInfo[j * 4 + 3];

							ATR_VCT_NO(a1) = ATTACK_MAGIC_CASE + 4;
							ATR_LONG_WORK(0, a1) = 0;
							ATR_DAMAGE(a1) = charahurt;
							ATR_PET_DAMAGE(a1) = pethurt;
							set_damage_num(a1, 6, -64);
							ATR_LIFE(a1) -= ATR_DAMAGE(a1);
							set_jujutsu_hit_mark(a1);
							if (ATR_LIFE(a1) <= 0)
							{
								ATR_LIFE(a1) = 0;
								slow_flg++;
							}

							ATR_PET_LIFE(a1) -= ATR_PET_DAMAGE(a1);
							//andy_fall
							if (ATR_PETFALL(a1) == 1)
							{
								if (ATR_LIFE(a1) > 0)
									petfallChangeGraph(a1);
							}
							if (ATR_PET_LIFE(a1) <= 0 && ATR_RIDE(a1))
							{
								ATR_PET_LIFE(a1) = 0;

								if (ATR_LIFE(a1) > 0)
									petfallChangeGraph(a1);
							}
							ATR_CHR_ACT(a1) = ANIM_DAMAGE;
							ATR_CHR_ACT_OLD(a1) = -1;
							pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
						}
					}
				}
			}
			else
			{
				int j, charahurt, pethurt;

				for (j = 0; j < iAttackedNum; j++)
				{
					if (idx == AttackedInfo[j * 4])
					{
						a1 = p_party[AttackedInfo[j * 4 + 1]];
						charahurt = AttackedInfo[j * 4 + 2];
						pethurt = AttackedInfo[j * 4 + 3];

						ATR_VCT_NO(a1) = ATTACK_MAGIC_CASE + 4;
						ATR_LONG_WORK(0, a1) = 0;
						ATR_DAMAGE(a1) = charahurt;

						ATR_PET_DAMAGE(a1) = pethurt;
						set_damage_num(a1, 6, -64);
						ATR_LIFE(a1) -= ATR_DAMAGE(a1);
						set_jujutsu_hit_mark(a1);
						if (ATR_LIFE(a1) <= 0)
						{
							ATR_LIFE(a1) = 0;
							slow_flg++;
						}
						ATR_PET_LIFE(a1) -= ATR_PET_DAMAGE(a1);
						//andy_fall
						if (ATR_PETFALL(a1) == 1)
						{
							if (ATR_LIFE(a1) > 0)
								petfallChangeGraph(a1);
						}
						if (ATR_PET_LIFE(a1) <= 0 && ATR_RIDE(a1))
						{
							ATR_PET_LIFE(a1) = 0;
							if (ATR_LIFE(a1) > 0)
								petfallChangeGraph(a1);
						}
						ATR_CHR_ACT(a1) = ANIM_DAMAGE;
						ATR_CHR_ACT_OLD(a1) = -1;
						pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					}
				}
			}
		}
		// 己经播放所有的动画了
		if (AttMgc.wNumAttackeds == AttMgc.wNumAttacks && 0 == iAttackedNum)
		{
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);
			bRunAttMgc = FALSE;
			g_iRunEarthQuake = 0;
		}
		break;
		// 带人物受伤的图
	case ATTACK_MAGIC_CASE + 4:
		if (ATR_FIRST_FLG(a0) > 40)
			ATR_FIRST_FLG(a0) = 0;
		ATR_FIRST_FLG(a0)++;
		ATR_CRS(a0) = (ATR_CRS(a0) + 16) & 31;
		gemini(a0);
		if (40 == ATR_FIRST_FLG(a0))
		{
			if (ATR_LIFE(a0) <= 0)
			{
				ATR_VCT_NO(a0) = VCT_NO_DIE + 1;
				slow_flg--;
			}
			else
				ATR_VCT_NO(a0) = 0;

			if (ATR_GROUP_FLG(a0) == 1)
				ATR_CRS(a0) = crs_change_tbl2[7];
			else
				ATR_CRS(a0) = crs_change_tbl2[3];
			gemini(a0);
			++iCurAttackedFinishNum;
		}
		// 己经播放所有的动画了
		if (iAttackedNum == iCurAttackedFinishNum)
		{
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);
			bRunAttMgc = FALSE;
			g_iRunEarthQuake = 0;
		}
		break;
	case ATTACK_MAGIC_CASE + 5:
		if (ATR_FIRST_FLG(a0) > 60)
			ATR_FIRST_FLG(a0) = 0;
		ATR_FIRST_FLG(a0)++;
		if (ATR_FIRST_FLG(a0) == 60)
		{
			ATR_VCT_NO(a0) = 0;
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);
			bRunAttMgc = FALSE;
			g_iRunEarthQuake = 0;
		}
		break;
#endif
#ifdef __TOCALL_MAGIC
		// 产生前置动画
	case TOCALL_MAGIC_CASE:
		// 是否在右下方
		(0 == ATR_GROUP_FLG(a0)) ? ATR_CHR_ANG(a0) = 3 : ATR_CHR_ANG(a0) = 7;

		ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];

		if (0XFFFFFFFF != ToCallMgc.iPreMgcNum)
		{
			if (NULL == (a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU))))
			{
				ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);
				ATR_VCT_NO(a0) = 0;
				return;
			}
			ATR_NAME(a1) = monster;
			ATR_VCT_NO(a1) = TOCALL_MAGIC_CASE + 1;		// 前置动画
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
			if (ToCallMgc.iPreMgcNum == 101120)
				ATR_DISP_PRIO(a1) = (1 == ToCallMgc.wPreMgcOnChar) ? D_PRIO_HIT_MARK : DISP_PRIO_TILE + 1;
			else 
				ATR_DISP_PRIO(a1) = ATR_DISP_PRIO(a0) + 1;
#else
			ATR_DISP_PRIO(a1) = (1 == ToCallMgc.wPreMgcOnChar) ? D_PRIO_HIT_MARK : DISP_PRIO_TILE + 1;
#endif
			ATR_CHR_NO(a1) = ToCallMgc.iPreMgcNum;
			ATR_BODY_WORK(0, a1) = a0;
			ATR_H_POS(a1) = ATR_H_POS(a0) + ToCallMgc.wPreMgcX;
			ATR_V_POS(a1) = ATR_V_POS(a0) + ToCallMgc.wPreMgcY;
		}
		ATR_VCT_NO(a0) = 0;
		ATR_CHR_ACT_OLD(a0) = -1;
#ifdef _PETSKILL_LER			// 雷尔技能
		if (ATR_CHR_NO(a0) == 101815)
			ATR_CHR_NO(a0) = ATR_CHR_NO(a0) * -1;
#endif
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
		// 执行前置动画
	case TOCALL_MAGIC_CASE + 1:

		a1 = ATR_BODY_WORK(0, a0);
		ATR_H_POS(a0) = ATR_H_POS(a1) + ToCallMgc.wPreMgcX;
		ATR_V_POS(a0) = ATR_V_POS(a1) + ToCallMgc.wPreMgcY;

		if (d0 = pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{
			DeathAction(a0);
			ToCallMgc.wRunPreMgc = TRUE;
#ifdef _PETSKILL_LER			// 雷尔技能
			if (ATR_CHR_NO_OLD(a1) < 0)
			{
				int side;

				ATR_CHR_NO(a1) = ATR_CHR_NO(a1) * -1;
				if (ToCallMgc.iCurMgcNum == 101798 || ToCallMgc.iCurMgcNum == 101800 || ToCallMgc.iCurMgcNum == 101853 || ToCallMgc.iCurMgcNum == 101854)
				{
					int i;
					for (i = 0; i < BATTLKPKPLYAERNUM; i++)
					{
						// 先找到雷尔位置
						if (ATR_CHR_NO(p_party[i]) == ATR_CHR_NO(a1))
							break;
					}
					side = i = (i < 10 ? 0:10);
					// 雷尔侧的人全都消失
					for (; i < side + 10; i++)
						ATR_H_POS(p_party[i]) += -lpDraw->xSize;
				}
			}
#endif
		}
		break;
		// 执行目前的咒术
	case TOCALL_MAGIC_CASE + 2:
	{
								  idx = ATR_LONG_WORK(0, a0);
#ifdef _PROFESSION_ADDSKILL
								  RunTimeMagicBoundary(ATR_CHR_NO(a0));
#endif
								  // 播放的是相对于人物的位置
								  if (20 != idx)
								  {
									  a1 = p_party[idx];
									  if (idx < 10)
									  {
										  ATR_H_POS(a0) = ATR_H_POS(a1) - ToCallMgc.posAttacked[0].x;
										  ATR_V_POS(a0) = ATR_V_POS(a1) - ToCallMgc.posAttacked[0].y;
									  }
									  else
									  {
										  ATR_H_POS(a0) = ATR_H_POS(a1) + ToCallMgc.posAttacked[0].x;
										  ATR_V_POS(a0) = ATR_V_POS(a1) + ToCallMgc.posAttacked[0].y;
									  }
								  }
								  // 播放绝对座标的咒术
								  else
								  {
									  int value = 0;

									  // 目前正在地震中
									  if (2 == g_iRunEarthQuake)
										  value = piyo_tbl[(piyo_point + g_iCurRunEarthQuake) & 63] + 12;
									  ATR_V_POS(a0) = ToCallMgc.posAttacked[0].y + value;
								  }
								  if (d0 = pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
								  {
#ifdef _PETSKILL_LER			// 雷尔技能
									  int side;

									  if (ATR_CHR_NO(a0) == 101798 || ATR_CHR_NO(a0) == 101800 || ATR_CHR_NO(a0) == 101853 || ATR_CHR_NO(a0) == 101854)
									  {
										  int i;
										  for (i = 0; i < BATTLKPKPLYAERNUM; i++)
										  {
											  // 先找到雷尔位置
											  if (ATR_CHR_NO(p_party[i]) == 101815)
												  break;
										  }
										  side = i = (i < 10 ? 0:10);
										  // 雷尔侧的人出现
										  for (; i < side + 10; i++)
											  ATR_H_POS(p_party[i]) += lpDraw->xSize;
									  }
#endif
									  // Terry add fix 修正召雷动画
									  if (ToCallMgc.wCurAttackNum == 101628)
										  DeathAction(a0);
									  else
										  // end
									  if (ToCallMgc.wIsPostDisappear)
									  {
										  a0tmp[a0tmpcount] = a0;
										  a0mark[a0tmpcount] = 1;
										  a0tmpcount++;
									  }
									  else
									  {
										  DeathAction(a0);
#ifdef _PROFESSION_ADDSKILL
										  if (boundary_2)
										  {
											  DeathAction(boundary_2);
											  boundary_2 = NULL;
										  }
#endif
									  }
									  // 如果没有后置动画
									  if (0XFFFFFFFF == ToCallMgc.iPostMgcNum)
									  {
										  ToCallMgc.wNumAttackeds++;
										  if (20 == idx)
										  {
											  // wAttackedIndex[0] 放置  20 -- 代表全体攻击
											  // wAttackedIndex[n] 放置0XFF -- 代表结束
											  int i = 0, j, charahurt, pethurt;

											  while (++i && 0XFF != ToCallMgc.wAttackedIndex[i])
											  {
												  for (j = 0; j < iAttackedNum; j++)
												  {
													  if (ToCallMgc.wAttackedIndex[i] == AttackedInfo[j * 4])
													  {
														  a1 = p_party[AttackedInfo[j * 4 + 1]];
														  charahurt = AttackedInfo[j * 4 + 2];
														  pethurt = AttackedInfo[j * 4 + 3];
														  ATR_LONG_WORK(0, a1) = 0;
														  ATR_DAMAGE(a1) = charahurt;
														  ATR_PET_DAMAGE(a1) = pethurt;
														  if (charahurt == 0)//伤害值为0 
														  {
#ifdef _BATTLESKILL				// (可开) Syu ADD 战斗技能介面
															  if (a0->anim_chr_no == 101651 || a0->anim_chr_no == 101650) //冰爆术第一下Miss不秀
																  ATR_VCT_NO(a1) = TOCALL_MAGIC_CASE + 5;
															  else
															  {
																  set_damage_num(a1, 0, -64);
																  ATR_VCT_NO(a1) = TOCALL_MAGIC_CASE + 5;
																  if ( ToCallMgc.iCurMgcNum == 101676 //贯穿攻击的伤害若为0,被打的人要有闪避的动作
																	  || ToCallMgc.iCurMgcNum == 101675
																	  || ToCallMgc.iCurMgcNum == 101674
																	  || ToCallMgc.iCurMgcNum == 101673
																	  || ToCallMgc.iCurMgcNum == 101672
																	  || ToCallMgc.iCurMgcNum == 101665
																	  || ToCallMgc.iCurMgcNum == 101664
																	  || ToCallMgc.iCurMgcNum == 101663
																	  || ToCallMgc.iCurMgcNum == 101662
																	  || ToCallMgc.iCurMgcNum == 101661
																	  || ToCallMgc.iCurMgcNum == 101656)
																  {	//回旋攻击
																	  ATR_STIMER(a1) = 0;
																	  ATR_FIRST_FLG(a1) = 0;		//?????????
																	  ATR_VCT_NO(a1) = TOCALL_MAGIC_CASE + 6;
																  }
															  }
#else
															  set_damage_num(a1, 0, -64);
															  ATR_VCT_NO(a1) = TOCALL_MAGIC_CASE + 5;
#endif
														  }
														  else
														  {
															  set_damage_num(a1, 6, -64);
															  ATR_VCT_NO(a1) = TOCALL_MAGIC_CASE + 4;
														  }
														  ATR_LIFE(a1) -= ATR_DAMAGE(a1);
														  set_jujutsu_hit_mark(a1);
														  if (ATR_LIFE(a1) <= 0)
														  {
															  ATR_LIFE(a1) = 0;
															  slow_flg++;
														  }
														  ATR_PET_LIFE(a1) -= ATR_PET_DAMAGE(a1);
														  //andy_fall
														  if (ATR_PETFALL(a1) == 1)
														  {
															  if (ATR_LIFE(a1) > 0)
																  petfallChangeGraph(a1);
														  }
														  if (ATR_PET_LIFE(a1) <= 0 && ATR_RIDE(a1))
														  {
															  ATR_PET_LIFE(a1) = 0;
															  if (ATR_LIFE(a1) > 0)
																  petfallChangeGraph(a1);
														  }
														  if (charahurt == 0)
															  ATR_CHR_ACT(a1) = ANIM_STAND;
														  else
															  ATR_CHR_ACT(a1) = ANIM_DAMAGE;
														  ATR_CHR_ACT_OLD(a1) = -1;
														  pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
													  }
												  }
											  }
										  }
										  else
										  {
											  int j, charahurt, pethurt;

											  for (j = 0; j < iAttackedNum; j++)
											  {
												  if (idx == AttackedInfo[j * 4])
												  {
													  a1 = p_party[AttackedInfo[j * 4 + 1]];
													  charahurt = AttackedInfo[j * 4 + 2];
													  pethurt = AttackedInfo[j * 4 + 3];
													  ATR_VCT_NO(a1) = TOCALL_MAGIC_CASE + 4;
													  ATR_LONG_WORK(0, a1) = 0;
													  ATR_DAMAGE(a1) = charahurt;
													  ATR_PET_DAMAGE(a1) = pethurt;
													  if (charahurt == 0)
													  {
														  //#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
														  //								if ( a0->anim_chr_no == 101651 || a0->anim_chr_no == 101650 ) //冰爆术第一下Miss不秀
														  //									ATR_VCT_NO(a1)		= TOCALL_MAGIC_CASE + 5;
														  //								else {
														  //									set_damage_num( a1 , 0 , -64 );
														  //									ATR_VCT_NO(a1)		= TOCALL_MAGIC_CASE + 5;
														  //								}
														  //#else
														  set_damage_num(a1, 0, -64);
														  ATR_VCT_NO(a1) = TOCALL_MAGIC_CASE + 5;
														  //#endif
													  }
													  else
													  {
														  set_damage_num(a1, 6, -64);
														  ATR_VCT_NO(a1) = TOCALL_MAGIC_CASE + 4;
													  }
													  ATR_LIFE(a1) -= ATR_DAMAGE(a1);
													  set_jujutsu_hit_mark(a1);
													  if (ATR_LIFE(a1) <= 0)
													  {
														  ATR_LIFE(a1) = 0;
														  slow_flg++;
													  }
													  ATR_PET_LIFE(a1) -= ATR_PET_DAMAGE(a1);
													  //andy_fall
													  if (ATR_PETFALL(a1) == 1)
													  {
														  if (ATR_LIFE(a1) > 0)
															  petfallChangeGraph(a1);
													  }
													  if (ATR_PET_LIFE(a1) <= 0 && ATR_RIDE(a1))
													  {
														  ATR_PET_LIFE(a1) = 0;
														  if (ATR_LIFE(a1) > 0)
															  petfallChangeGraph(a1);
													  }
													  if (charahurt == 0)
														  ATR_CHR_ACT(a1) = ANIM_STAND;
													  else
														  ATR_CHR_ACT(a1) = ANIM_DAMAGE;
													  ATR_CHR_ACT_OLD(a1) = -1;
													  pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
												  }
											  }
										  }
									  }
									  // 拥有后置动画
									  else
									  {
										  // 针对所有的敌人施后置动画
										  if (20 == idx) // idx = wAttackedIndex[0]
										  {
											  // wAttackedIndex[0] 放置  20 -- 代表全体攻击
											  // wAttackedIndex[n] 放置0XFF -- 代表结束
											  int i = 0;

											  while (++i && 0XFF != ToCallMgc.wAttackedIndex[i])
											  {
												  a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));

												  ATR_NAME(a1) = monster;
												  ATR_VCT_NO(a1) = TOCALL_MAGIC_CASE + 3;		// 后置动画
												  ATR_DISP_PRIO(a1) = (1 == ToCallMgc.wPostMgcOnChar) ? D_PRIO_HIT_MARK : DISP_PRIO_TILE + 1;
												  ATR_CHR_NO(a1) = ToCallMgc.iPostMgcNum;
												  ATR_LONG_WORK(0, a1) = ToCallMgc.wAttackedIndex[i];
												  ATR_H_POS(a1) = ATR_H_POS(p_party[ToCallMgc.wAttackedIndex[i]]) + ToCallMgc.wPostMgcX;
												  ATR_V_POS(a1) = ATR_V_POS(p_party[ToCallMgc.wAttackedIndex[i]]) + ToCallMgc.wPostMgcY;
											  }
										  }
										  // 针对某一个敌人施后置动画
										  else
										  {
											  a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));

											  ATR_NAME(a1) = monster;
											  ATR_VCT_NO(a1) = TOCALL_MAGIC_CASE + 3;		// 后置动画
											  ATR_DISP_PRIO(a1) = (1 == ToCallMgc.wPostMgcOnChar) ? D_PRIO_HIT_MARK : DISP_PRIO_TILE + 1;
											  ATR_CHR_NO(a1) = ToCallMgc.iPostMgcNum;
											  ATR_LONG_WORK(0, a1) = idx;
											  ATR_H_POS(a1) = ATR_H_POS(p_party[idx]) + ToCallMgc.wPostMgcX;
											  ATR_V_POS(a1) = ATR_V_POS(p_party[idx]) + ToCallMgc.wPostMgcY;
										  }
									  }
								  }

								  // 己经播放所有的动画了
								  if (ToCallMgc.wNumAttackeds == ToCallMgc.wNumAttacks && 0 == iAttackedNum)
								  {
									  ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);
									  bRunToCallMgc = FALSE;
									  g_iRunEarthQuake = 0;
								  }

								  break;
	}
		// 执行后置动画
	case TOCALL_MAGIC_CASE + 3:

		idx = ATR_LONG_WORK(0, a0);
		a1 = p_party[idx];
		ATR_H_POS(a0) = ATR_H_POS(a1) + ToCallMgc.wPostMgcX;
		ATR_V_POS(a0) = ATR_V_POS(a1) + ToCallMgc.wPostMgcY;

		if (d0 = pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{
			DeathAction(a0);
			ToCallMgc.wNumAttackeds++;

			if (20 == idx)
			{
				// wAttackedIndex[0] 放置  20 -- 代表全体攻击
				// wAttackedIndex[n] 放置0XFF -- 代表结束
				int i = 0, j, charahurt, pethurt;

				while (++i && 0XFF != ToCallMgc.wAttackedIndex[i])
				{
					for (j = 0; j < iAttackedNum; j++)
					{
						if (ToCallMgc.wAttackedIndex[i] == AttackedInfo[j * 4])
						{
							a1 = p_party[AttackedInfo[j * 4 + 1]];
							charahurt = AttackedInfo[j * 4 + 2];
							pethurt = AttackedInfo[j * 4 + 3];

							ATR_VCT_NO(a1) = TOCALL_MAGIC_CASE + 4;
							ATR_LONG_WORK(0, a1) = 0;
							ATR_DAMAGE(a1) = charahurt;


							ATR_PET_DAMAGE(a1) = pethurt;

							set_damage_num(a1, 6, -64);
							ATR_LIFE(a1) -= ATR_DAMAGE(a1);
							set_jujutsu_hit_mark(a1);
							if (ATR_LIFE(a1) <= 0)
							{
								ATR_LIFE(a1) = 0;
								slow_flg++;
							}


							ATR_PET_LIFE(a1) -= ATR_PET_DAMAGE(a1);
							//andy_fall
							if (ATR_PETFALL(a1) == 1)	{
								if (ATR_LIFE(a1) > 0)	{
									petfallChangeGraph(a1);
								}
							}
							if (ATR_PET_LIFE(a1) <= 0 && ATR_RIDE(a1)){
								ATR_PET_LIFE(a1) = 0;

								if (ATR_LIFE(a1) > 0)
									petfallChangeGraph(a1);
							}

							ATR_CHR_ACT(a1) = ANIM_DAMAGE;
							ATR_CHR_ACT_OLD(a1) = -1;
							pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
						}
					}
				}
			}
			else
			{
				int j, charahurt, pethurt;

				for (j = 0; j < iAttackedNum; j++)
				{
					if (idx == AttackedInfo[j * 4])
					{
						a1 = p_party[AttackedInfo[j * 4 + 1]];
						charahurt = AttackedInfo[j * 4 + 2];
						pethurt = AttackedInfo[j * 4 + 3];
						ATR_VCT_NO(a1) = TOCALL_MAGIC_CASE + 4;
						ATR_LONG_WORK(0, a1) = 0;
						ATR_DAMAGE(a1) = charahurt;
						ATR_PET_DAMAGE(a1) = pethurt;
						set_damage_num(a1, 6, -64);
						ATR_LIFE(a1) -= ATR_DAMAGE(a1);
						set_jujutsu_hit_mark(a1);
						if (ATR_LIFE(a1) <= 0)
						{
							ATR_LIFE(a1) = 0;
							slow_flg++;
						}
						ATR_PET_LIFE(a1) -= ATR_PET_DAMAGE(a1);
						//andy_fall
						if (ATR_PETFALL(a1) == 1)
						{
							if (ATR_LIFE(a1) > 0)
								petfallChangeGraph(a1);
						}
						if (ATR_PET_LIFE(a1) <= 0 && ATR_RIDE(a1))
						{
							ATR_PET_LIFE(a1) = 0;
							if (ATR_LIFE(a1) > 0)
								petfallChangeGraph(a1);
						}
						ATR_CHR_ACT(a1) = ANIM_DAMAGE;
						ATR_CHR_ACT_OLD(a1) = -1;
						pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					}
				}
			}

		}

		// 己经播放所有的动画了
		if (ToCallMgc.wNumAttackeds == ToCallMgc.wNumAttacks && 0 == iAttackedNum)
		{
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);
			bRunToCallMgc = FALSE;
			g_iRunEarthQuake = 0;
		}
		break;
		// 带人物受伤的图
	case TOCALL_MAGIC_CASE + 4:
		if (ATR_FIRST_FLG(a0) > 40)
			ATR_FIRST_FLG(a0) = 0;
		ATR_FIRST_FLG(a0)++;
		if (ATR_LIFE(a0) <= 0)
		{
			ATR_CRS(a0) = (ATR_CRS(a0) + 16) & 31;
			gemini(a0);
		}
		if (40 == ATR_FIRST_FLG(a0))		// 代表会振动40次
		{
			if (ATR_LIFE(a0) <= 0)
			{
				ATR_VCT_NO(a0) = VCT_NO_DIE + 1;
				slow_flg--;
			}
			else
				ATR_VCT_NO(a0) = 0;

			if (ATR_GROUP_FLG(a0) == 1)
				ATR_CRS(a0) = crs_change_tbl2[7];
			else
				ATR_CRS(a0) = crs_change_tbl2[3];
			gemini(a0);
			++iCurAttackedFinishNum;
		}
		// 己经播放所有的动画了
		if (iAttackedNum == iCurAttackedFinishNum)
		{
			for (int i = 0; i < 10; i++)
			{
				if (a0mark[i] != 0)
				{
					DeathAction(a0tmp[i]);
					a0mark[i] = 0;
				}
			}
			a0tmpcount = 0;
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);
			bRunToCallMgc = FALSE;
			g_iRunEarthQuake = 0;
		}
		break;
	case TOCALL_MAGIC_CASE + 5:
		if (ATR_FIRST_FLG(a0) > 60)
			ATR_FIRST_FLG(a0) = 0;
		ATR_FIRST_FLG(a0)++;
		if (ATR_FIRST_FLG(a0) == 60)
		{
			if (ToCallMgc.wIsPostDisappear)
			{
				for (int i = 0; i < 10; i++)
				{
					if (a0mark[i] != 0)
					{
						DeathAction(a0tmp[i]);
						a0mark[i] = 0;
					}
				}
			}
			a0tmpcount = 0;
			ATR_VCT_NO(a0) = 0;
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);
			bRunToCallMgc = FALSE;
			g_iRunEarthQuake = 0;
		}
		break;
		//人物闪避
	case TOCALL_MAGIC_CASE + 6:
		ATR_CHR_ACT(a0) = ANIM_WALK;		//????????
		ATR_SPD(a0) = 16;
		ATR_CHR_ANG(a0) = (ATR_DAMAGE_ANG(a0) + 4) & 7;
		if (ATR_GROUP_FLG(a0) == 1)
			ATR_CRS(a0) = crs_change_tbl2[3];
		else
			ATR_CRS(a0) = crs_change_tbl2[7];
		gemini(a0);
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		ATR_STIMER(a0)++;
		if (ATR_STIMER(a0) == 20)
		{
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = (ATR_CRS(a0) + 16) & 31;//??
			ATR_VCT_NO(a0) = 17;		//?????
			ATR_STIMER(a0) = 0;
			++iCurAttackedFinishNum;
		}
		// 己经播放所有的动画了
		if (iAttackedNum == iCurAttackedFinishNum)
		{
			for (int i = 0; i < 10; i++)
			{
				if (a0mark[i] != 0)
				{
					DeathAction(a0tmp[i]);
					a0mark[i] = 0;
				}
			}
			a0tmpcount = 0;
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);
			bRunToCallMgc = FALSE;
			g_iRunEarthQuake = 0;
		}
		break;
#endif
		//#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
	case PROSKILL:
		// 是否在右下方
		(0 == ATR_GROUP_FLG(a0)) ? ATR_CHR_ANG(a0) = 3 : ATR_CHR_ANG(a0) = 7;

		ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];

		if (0XFFFFFFFF != AttMgc.iPreMgcNum)
		{
			if (NULL == (a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU))))
			{
				ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);
				ATR_VCT_NO(a0) = 0;
				return;
			}
			if (AttPreMagicNum == 0)
			{
				ATR_VCT_NO(a0) = 1;
				return;
			}
			ATR_NAME(a1) = monster;
			ATR_VCT_NO(a1) = PROSKILL + 1;		// 前置动画
			ATR_CHR_NO(a1) = AttPreMagicNum;
			ATR_DISP_PRIO(a1) = ATR_DISP_PRIO(a0) + 1;
			ATR_BODY_WORK(0, a1) = a0;
			ATR_H_POS(a1) = ATR_H_POS(a0);
			ATR_V_POS(a1) = ATR_V_POS(a0);
		}
		if (ATR_WARRIOR_EFFECT(a0) == 1)
			ATR_VCT_NO(a0) = 0;
		else if (ATR_WARRIOR_EFFECT(a0) == 2)
			ATR_VCT_NO(a0) = 1;
		ATR_CHR_ACT_OLD(a0) = -1;
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case PROSKILL + 1:
		a1 = ATR_BODY_WORK(0, a0);
		ATR_H_POS(a0) = ATR_H_POS(a1);
		ATR_V_POS(a0) = ATR_V_POS(a1) - 35;
		if (ATR_CHR_NO(a0) == 101631 || ATR_CHR_NO(a0) == 101632)
			ATR_V_POS(a0) = ATR_V_POS(a1) - 65;
		if (d0 = pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{
			DeathAction(a0);
			if (ATR_WARRIOR_EFFECT(a1) == 1)
				ATR_VCT_NO(a1) = 1;
		}
		break;
		//#endif
	}
#ifdef _SYUTEST
	if (Light1 != NULL)
	{
		if (Light1->anim_cnt == 5)
		{	//拨放到最后一张
			DeathAction(Light1);
			Light1 = NULL;
		}
	}
#endif
}

ACTION *oft_test(void)
{
	int d0, d1, d2, d7;
	int id_no = 0;
	ACTION *a1;

	p_kanji = NULL;
	for (d7 = 0; d7 < BATTLKPKPLYAERNUM + 1; d7++)		//清除
		p_missile[d7] = NULL;
	for (d7 = 0; d7 < BATTLKPKPLYAERNUM; d7++)		//清除
		p_party[d7] = NULL;

	// 产生 master action list /* ?????????????? */
	a1 = GetAction(T_PRIO_MASTER, sizeof(ATR_EQU));
	if (a1 == NULL)
		return NULL;
	//要执行的Func
	ATR_NAME(a1) = master;
	//出场先给予等待
	ATR_VCT_NO(a1) = 3;
	//表示优先顺序
	ATR_DISP_PRIO(a1) = D_PRIO_MASTER;
	// sprite 编号/* ?????? */
	ATR_CHR_NO(a1) = 31027;
	ATR_CHR_ANG(a1) = 3;		//左上
	//初期位置
	ATR_H_POS(a1) = 320;
	ATR_V_POS(a1) = 240;
	// 攻击种类
	ATR_CHR_ACT(a1) = ANIM_ATTACK;		// 移动动画
	// 记录 a1 于 p_master/* ??????? */
	p_master = a1;
	// 产生 field 在左下的 action list/* ????????????????? */
	a1 = GetAction(T_PRIO_MASTER, sizeof(ATR_EQU));
	if (a1 == NULL)
		return NULL;
	ATR_ATTRIB(a1) = ACT_ATR_HIDE;	// 不显示	//??
	//表示优先顺序
	ATR_DISP_PRIO(a1) = D_PRIO_MASTER;
	//初期位置
	ATR_H_POS(a1) = lpDraw->xSize - 32;
	ATR_V_POS(a1) = lpDraw->ySize - 64;
	// 记录 a1 于 p_attrib/* ??????? */
	p_attrib = a1;

#ifdef _BATTLE_PK_PLAYER_FOR_6VS6
	//X座标
	d0 = 416 + 5 + DISPLACEMENT_X;
	//Y座标
	d1 = 408 + 24 + DISPLACEMENT_Y;
#else
	//X座标
	d0 = 416 + 32 + 5 + DISPLACEMENT_X;
	//Y座标
	d1 = 408 + 24 + DISPLACEMENT_Y;
#endif
	// 预设为图号为 SPR_021em
	d2 = SPR_021em;		/*?????*/

	//_BATTLE_PK_PLAYER_FOR_6VS6  XIEZI  修改
	for (d7 = 0; d7 < MAX_BATTLE_ROW_CHARS; d7++)
	{	//我方人物
		// 产生我方的 monster action list /* ??????????????? */
		a1 = GetAction(T_PRIO_JIKI, sizeof(ATR_EQU));
		if (a1 == NULL)
			return NULL;
		//要执行的Func /* ??? */
		ATR_NAME(a1) = NULL;
		// 动作为登场
		ATR_VCT_NO(a1) = VCT_NO_APPEAR;
		/* ? */
		ATR_DISP_PRIO(a1) = D_PRIO_MASTER;
		// 显示出相关讯息 // ??????????
		a1->atr |= ACT_ATR_INFO;
		// 可以被点选 // ?????
		a1->atr |= ACT_ATR_HIT;
		// sprite 编号 /* ?????? */
		ATR_CHR_NO(a1) = d2;
		ATR_CHR_ANG(a1) = 3;		//左上
		ATR_HIT_TIMING(a1) = 5;
		// 初期位置 /* ??? */
		monster_place_pos[id_no] = ATR_H_POS(a1) = ATR_INT_WORK0(a1) = d0;
		monster_place_pos[id_no + 1] = ATR_V_POS(a1) = ATR_INT_WORK1(a1) = d1;
		ATR_H_POS(a1) += 300;
		ATR_V_POS(a1) += 300;
		// 一开始在画面外的位置
		monster_start_pos[id_no++] = ATR_H_POS(a1);
		monster_start_pos[id_no++] = ATR_V_POS(a1);
		/* ???? */
		ATR_CHR_ACT(a1) = ANIM_STAND;		//????????
		ATR_SPD(a1) = 32;		//移动速度
		ATR_GROUP_FLG(a1) = 0;		// 右下 group set //?????????
		ATR_PLACE_NO(a1) = d7;		// 记录开始位置 //??
		// 记录 a1 于 队伍列表中/* ??????? */
		p_party[d7] = a1;
		pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
		/* ?? */
		d0 += 32;
		d1 -= 24;
	}

#ifdef _BATTLE_PK_PLAYER_FOR_6VS6
	//X座标
	d0 = 320 - 64 + 5 + DISPLACEMENT_X;
	//Y座标
	d1 = 432 - 24 + DISPLACEMENT_Y;
#else
	d0 = 320 - 32 + 5 + DISPLACEMENT_X;		//??
	d1 = 432 - 24 + DISPLACEMENT_Y;		//??
#endif
	d2 = SPR_pet001;		/*???*/

	//_BATTLE_PK_PLAYER_FOR_6VS6  XIEZI  修改
	for (; d7 < 2 * MAX_BATTLE_ROW_CHARS; d7++)
	{	//我方宠物
		/* ??????????????? */
		a1 = GetAction(T_PRIO_JIKI, sizeof(ATR_EQU));
		if (a1 == NULL)
			return NULL;
		ATR_NAME(a1) = NULL;
		ATR_VCT_NO(a1) = VCT_NO_APPEAR;
		/* ? */
		ATR_DISP_PRIO(a1) = D_PRIO_MASTER;
		// ??????????
		a1->atr |= ACT_ATR_INFO;
		// ?????
		a1->atr |= ACT_ATR_HIT;
		/* ?????? */
		ATR_CHR_NO(a1) = d2;
		ATR_CHR_ANG(a1) = 3;		//?
		ATR_HIT_TIMING(a1) = 3 + 2;
		/* ??? */
		monster_place_pos[id_no] = ATR_H_POS(a1) = ATR_INT_WORK0(a1) = d0;
		monster_place_pos[id_no + 1] = ATR_V_POS(a1) = ATR_INT_WORK1(a1) = d1;
		ATR_H_POS(a1) += 300;
		ATR_V_POS(a1) += 300;
		monster_start_pos[id_no++] = ATR_H_POS(a1);
		monster_start_pos[id_no++] = ATR_V_POS(a1);
		/* ???? */
		ATR_CHR_ACT(a1) = ANIM_STAND;		//????????
		ATR_SPD(a1) = 32;		//?
		ATR_GROUP_FLG(a1) = 0;		//?????????
		ATR_PLACE_NO(a1) = d7;		//??
		/* ??????? */
		p_party[d7] = a1;
		pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
		/* ?? */
		d0 += 32 * 2;
		d1 -= 24 * 2;
	}
	/* ???? */
	p_party[d7] = NULL;
#ifdef _BATTLE_PK_PLAYER_FOR_6VS6
	//X座标
	d0 = 64 - 32 + 5;
	//Y座标
	d1 = 192 + 30;
#else
	d0 = 64 - 32 + 5;		//??
	d1 = 192 - 24 + 30;		//??
#endif
	d2 = SPR_021em;		/*?????*/

	//_BATTLE_PK_PLAYER_FOR_6VS6  XIEZI  修改
	for (; d7 < 3 * MAX_BATTLE_ROW_CHARS; d7++)
	{	//敌方人物
		/* ??????????????? */
		a1 = GetAction(T_PRIO_JIKI, sizeof(ATR_EQU));
		if (a1 == NULL)
			return NULL;
		ATR_NAME(a1) = NULL;
		ATR_VCT_NO(a1) = VCT_NO_APPEAR;
		/* ? */
		ATR_DISP_PRIO(a1) = D_PRIO_MASTER;
		// ??????????
		a1->atr |= ACT_ATR_INFO;
		// ?????
		a1->atr |= ACT_ATR_HIT;
		/* ?????? */
		ATR_CHR_NO(a1) = d2;
		ATR_CHR_ANG(a1) = 7;		//??
		ATR_HIT_TIMING(a1) = 5;
		/* ??? */
		monster_place_pos[id_no] = ATR_H_POS(a1) = ATR_INT_WORK0(a1) = d0;
		monster_place_pos[id_no + 1] = ATR_V_POS(a1) = ATR_INT_WORK1(a1) = d1;
		ATR_H_POS(a1) -= 300;
		ATR_V_POS(a1) -= 300;
		monster_start_pos[id_no++] = ATR_H_POS(a1);
		monster_start_pos[id_no++] = ATR_V_POS(a1);
		/* ???? */
		ATR_CHR_ACT(a1) = ANIM_STAND;		//????????
		ATR_SPD(a1) = 32;		//?
		ATR_GROUP_FLG(a1) = 1;		//????????
		ATR_PLACE_NO(a1) = d7;		//??
		/* ??????? */
		p_party[d7] = a1;
		pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
		/* ?? */
		d0 += 32;
		d1 -= 24;
	}

#ifdef _BATTLE_PK_PLAYER_FOR_6VS6
	//X座标
	d0 = 32 + 32 + 5;
	//Y座标
	d1 = 264 + 48 + 10;
#else
	d0 = 32 + 32 + 5;		//??
	d1 = 264 + 24 + 10;	//??
#endif
	d2 = SPR_pet001;		/*???*/

	//_BATTLE_PK_PLAYER_FOR_6VS6  XIEZI  修改
	for (; d7 < 4 *	MAX_BATTLE_ROW_CHARS; d7++)
	{	//敌方宠物
		/* ??????????????? */
		a1 = GetAction(T_PRIO_JIKI, sizeof(ATR_EQU));
		if (a1 == NULL)
			return NULL;
		ATR_NAME(a1) = NULL;
		ATR_VCT_NO(a1) = VCT_NO_APPEAR;
		/* ? */
		ATR_DISP_PRIO(a1) = D_PRIO_MASTER;
		// ??????????
		a1->atr |= ACT_ATR_INFO;
		// ?????
		a1->atr |= ACT_ATR_HIT;
		/* ?????? */
		ATR_CHR_NO(a1) = d2;
		ATR_CHR_ANG(a1) = 7;		//??
		ATR_HIT_TIMING(a1) = 3 + 2;
		/* ??? */
		monster_place_pos[id_no] = ATR_H_POS(a1) = ATR_INT_WORK0(a1) = d0;
		monster_place_pos[id_no + 1] = ATR_V_POS(a1) = ATR_INT_WORK1(a1) = d1;
		ATR_H_POS(a1) -= 300;
		ATR_V_POS(a1) -= 300;
		monster_start_pos[id_no++] = ATR_H_POS(a1);
		monster_start_pos[id_no++] = ATR_V_POS(a1);
		/* ???? */
		ATR_CHR_ACT(a1) = ANIM_STAND;		//????????
		ATR_SPD(a1) = 32;		//?
		ATR_GROUP_FLG(a1) = 1;		//????????
		ATR_PLACE_NO(a1) = d7;		//??
		/* ??????? */
		p_party[d7] = a1;
		pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
		/* ?? */
		d0 += 32 * 2;
		d1 -= 24 * 2;
	}
#ifdef _BATTLE_PK_PLAYER_FOR_40
	p_party[d7] = NULL;
	d0 = 128 + 32 + 5;
	d1 = 408 + 24 + DISPLACEMENT_Y;
	d2 = SPR_021em;		/*?????*/
	for (d7 = 20; d7 < 25; d7++)
	{	//敌方人物
		/* ??????????????? */
		a1 = GetAction(T_PRIO_JIKI, sizeof(ATR_EQU));
		if (a1 == NULL)
			return NULL;
		ATR_NAME(a1) = NULL;
		ATR_VCT_NO(a1) = VCT_NO_APPEAR;
		/* ? */
		ATR_DISP_PRIO(a1) = D_PRIO_MASTER;
		// ??????????
		a1->atr |= ACT_ATR_INFO;
		// ?????
		a1->atr |= ACT_ATR_HIT;
		/* ?????? */
		ATR_CHR_NO(a1) = d2;
		ATR_CHR_ANG(a1) = 5;		//??
		ATR_HIT_TIMING(a1) = 5;
		/* ??? */
		monster_place_pos[id_no] = ATR_H_POS(a1) = ATR_INT_WORK0(a1) = d0;
		monster_place_pos[id_no + 1] = ATR_V_POS(a1) = ATR_INT_WORK1(a1) = d1;
		ATR_H_POS(a1) -= 300;
		ATR_V_POS(a1) += 300;
		monster_start_pos[id_no++] = ATR_H_POS(a1);
		monster_start_pos[id_no++] = ATR_V_POS(a1);
		/* ???? */
		ATR_CHR_ACT(a1) = ANIM_STAND;		//????????
		ATR_SPD(a1) = 32;		//?
		ATR_GROUP_FLG(a1) = 2;		//????????
		ATR_PLACE_NO(a1) = d7;		//??
		/* ??????? */
		p_party[d7] = a1;
		pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
		/* ?? */
		d0 -= 32;
		d1 -= 24;
	}
	d0 = 288 + 32 + 5;		//??
	d1 = 432 - 24 + DISPLACEMENT_Y;		//??
	d2 = SPR_pet001;		/*???*/
	for (d7 = 25; d7 < 30; d7++)
	{	//敌方宠物
		/* ??????????????? */
		a1 = GetAction(T_PRIO_JIKI, sizeof(ATR_EQU));
		if (a1 == NULL)
			return NULL;
		ATR_NAME(a1) = NULL;
		ATR_VCT_NO(a1) = VCT_NO_APPEAR;
		/* ? */
		ATR_DISP_PRIO(a1) = D_PRIO_MASTER;
		// ??????????
		a1->atr |= ACT_ATR_INFO;
		// ?????
		a1->atr |= ACT_ATR_HIT;
		/* ?????? */
		ATR_CHR_NO(a1) = d2;
		ATR_CHR_ANG(a1) = 5;		//??
		ATR_HIT_TIMING(a1) = 3 + 2;
		/* ??? */
		monster_place_pos[id_no] = ATR_H_POS(a1) = ATR_INT_WORK0(a1) = d0;
		monster_place_pos[id_no + 1] = ATR_V_POS(a1) = ATR_INT_WORK1(a1) = d1;
		ATR_H_POS(a1) -= 300;
		ATR_V_POS(a1) += 300;
		monster_start_pos[id_no++] = ATR_H_POS(a1);
		monster_start_pos[id_no++] = ATR_V_POS(a1);
		/* ???? */
		ATR_CHR_ACT(a1) = ANIM_STAND;		//????????
		ATR_SPD(a1) = 32;		//?
		ATR_GROUP_FLG(a1) = 2;		//????????
		ATR_PLACE_NO(a1) = d7;		//??
		/* ??????? */
		p_party[d7] = a1;
		pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
		/* ?? */
		d0 -= 32 * 2;
		d1 -= 24 * 2;
	}

	//X座标
	d0 = 608 - 32 + 5 + DISPLACEMENT_X;
	//Y座标
	d1 = 190 - 24 + 30;
	d2 = SPR_021em;		/*?????*/
	for (d7 = 30; d7 < 35; d7++)
	{	//左上角敌方人物
		/* ??????????????? */
		a1 = GetAction(T_PRIO_JIKI, sizeof(ATR_EQU));
		if (a1 == NULL)
			return NULL;
		ATR_NAME(a1) = NULL;
		ATR_VCT_NO(a1) = VCT_NO_APPEAR;
		/* ? */
		ATR_DISP_PRIO(a1) = D_PRIO_MASTER;
		// ??????????
		a1->atr |= ACT_ATR_INFO;
		// ?????
		a1->atr |= ACT_ATR_HIT;
		/* ?????? */
		ATR_CHR_NO(a1) = d2;
		ATR_CHR_ANG(a1) = 1;		//??
		ATR_HIT_TIMING(a1) = 5;
		/* ??? */
		monster_place_pos[id_no] = ATR_H_POS(a1) = ATR_INT_WORK0(a1) = d0;
		monster_place_pos[id_no + 1] = ATR_V_POS(a1) = ATR_INT_WORK1(a1) = d1;
		ATR_H_POS(a1) += 300;
		ATR_V_POS(a1) -= 300;
		monster_start_pos[id_no++] = ATR_H_POS(a1);
		monster_start_pos[id_no++] = ATR_V_POS(a1);
		/* ???? */
		ATR_CHR_ACT(a1) = ANIM_STAND;		//????????
		ATR_SPD(a1) = 32;		//?
		ATR_GROUP_FLG(a1) = 3;		//????????
		ATR_PLACE_NO(a1) = d7;		//??
		/* ??????? */
		p_party[d7] = a1;
		pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
		/* ?? */
		d0 -= 32;
		d1 -= 24;
	}
	d0 = 608 - 32 + 5 + DISPLACEMENT_X;		//??
	d1 = 236 + 24 + 30;		//??
	d2 = SPR_pet001;		/*???*/
	for (d7 = 35; d7 < 40; d7++)
	{	//左上角敌方宠物
		/* ??????????????? */
		a1 = GetAction(T_PRIO_JIKI, sizeof(ATR_EQU));
		if (a1 == NULL)
			return NULL;
		ATR_NAME(a1) = NULL;
		ATR_VCT_NO(a1) = VCT_NO_APPEAR;
		/* ? */
		ATR_DISP_PRIO(a1) = D_PRIO_MASTER;
		// ??????????
		a1->atr |= ACT_ATR_INFO;
		// ?????
		a1->atr |= ACT_ATR_HIT;
		/* ?????? */
		ATR_CHR_NO(a1) = d2;
		ATR_CHR_ANG(a1) = 1;		//??
		ATR_HIT_TIMING(a1) = 3 + 2;
		/* ??? */
		monster_place_pos[id_no] = ATR_H_POS(a1) = ATR_INT_WORK0(a1) = d0;
		monster_place_pos[id_no + 1] = ATR_V_POS(a1) = ATR_INT_WORK1(a1) = d1;
		ATR_H_POS(a1) += 300;
		ATR_V_POS(a1) -= 300;
		monster_start_pos[id_no++] = ATR_H_POS(a1);
		monster_start_pos[id_no++] = ATR_V_POS(a1);
		/* ???? */
		ATR_CHR_ACT(a1) = ANIM_STAND;		//????????
		ATR_SPD(a1) = 32;		//?
		ATR_GROUP_FLG(a1) = 3;		//????????
		ATR_PLACE_NO(a1) = d7;		//??
		/* ??????? */
		p_party[d7] = a1;
		pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
		/* ?? */
		d0 -= 32 * 2;
		d1 -= 24 * 2;
	}

#endif
	// 初期位置再配置 //????
	for (d7 = 0; d7 < BATTLKPKPLYAERNUM; d7++)
	{
		a1 = p_party[d7];
		ATR_H_POS(a1) = monster_start_pos[monster_place_no[d7] * 2];
		ATR_V_POS(a1) = monster_start_pos[monster_place_no[d7] * 2 + 1];
		ATR_INT_WORK0(a1) = monster_place_pos[monster_place_no[d7] * 2];
		ATR_INT_WORK1(a1) = monster_place_pos[monster_place_no[d7] * 2 + 1];
	}
	command_point = 0;		// clear command point //?????????????
	return a1;
}

static int bc_pointer;
/* ??????? *******************************************************************/
int get_bc_num(void)
{
	int d0, d1;

	while (1)
	{
		d0 = BattleStatus[bc_pointer++];		//????
		if (d0 == NULL)
		{	//???
			bc_pointer--;
			return -1;
		}
		if (d0 >= '0' && d0 <= '9')		//???
			break;						//??????
		if (d0 >= 'A' && d0 <= 'F')		//?????
			break;						//??????
	}
	if (d0 >= 'A')		//?????
		d1 = d0 - 'A' + 10;
	else
		d1 = d0 - '0';

	while (1)
	{
		d0 = BattleStatus[bc_pointer++];		//???????
		if (d0 == NULL)
		{	//???
			bc_pointer--;
			break;
		}
		if (d0 < '0' || d0 > '9')
		{	//??????
			if (d0 < 'A' || d0 > 'F')		//????????
				break;						//?
		}
		d1 = d1 << 4;		//???????
		if (d0 >= 'A')		//?????
			d1 += d0 - 'A' + 10;
		else
			d1 += d0 - '0';
	}
	return d1;
}

/* ?????? *******************************************************************/
void get_bc_asc(ACTION *a1, int flg)
{
	char d0;
	char *a2;

	if (flg == 0)
		a2 = ATR_HANDLE(a1);
	else
		a2 = ATR_TITLE(a1);
	while (1)
	{
		d0 = BattleStatus[bc_pointer++];		//????
		if (d0 == NULL)
		{	//???
			bc_pointer--;
			break;
		}
		//????
		if (d0 == '|')
			break;
		if (IsDBCSLeadByte(d0))
		{
			*a2++ = d0;
			*a2++ = BattleStatus[bc_pointer++];
		}
		else
			*a2++ = d0;
	}
	//????
	*a2 = 0;
	if (flg == 0)
		makeStringFromEscaped(ATR_HANDLE(a1));
	else
		makeStringFromEscaped(ATR_TITLE(a1));
}

// Robin 0728
/* ?????? for Ride Pet*******************************************************************/
void get_bc_asc_ridepet(ACTION *a1)
{
	char d0;
	char *a2;

	a2 = ATR_PETNAME(a1);
	while (1)
	{
		d0 = BattleStatus[bc_pointer++];		//????
		if (d0 == NULL)
		{	//???
			bc_pointer--;
			break;
		}
		//????
		if (d0 == '|')
			break;
		if (IsDBCSLeadByte(d0))
		{
			*a2++ = d0;
			*a2++ = BattleStatus[bc_pointer++];
		}
		else
			*a2++ = d0;
	}
	//????
	*a2 = 0;
	makeStringFromEscaped(ATR_PETNAME(a1));
}

//人物是否有状态在身上确认
//填入p_party内所有的值及所有的初始化
void set_bc(void)
{
	ACTION *a1;
	int d0, d2, d3, d1 = 0;

	//????????
	bc_pointer = 3;
	//?????
	ATR_ATTRIB(p_master) = get_bc_num();
	//????????
	while (BattleStatus[bc_pointer] != NULL)
	{
		//??????
		d2 = get_bc_num();
#ifdef _STONDEBUG_
		if (d2 >= BATTLKPKPLYAERNUM)
			MessageBoxNew(hWnd, "初始化失败,超过设定数量", "Error", MB_OK);
#endif
		//?????????????
		a1 = p_party[d2];
		/* ??? */
		ATR_NAME(a1) = monster;
		//䦶???
		get_bc_asc(a1, 0);
		//????
		get_bc_asc(a1, 1);
		//???????
		ATR_CHR_NO(a1) = get_bc_num();
		//??????
		ATR_LEVEL(a1) = get_bc_num();
		//?????
		ATR_LIFE(a1) = get_bc_num();

#ifdef _SFUMATO
		ATR_SFUMATO(a1) = 0xff0000;	
#endif

		//??????
		ATR_MAX_LIFE(a1) = get_bc_num();
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
		ATR_MAX_MANA(a1) = pc.maxMp ;
#endif
		//??????
		d0 = get_bc_num();
		// Robin 0728 ridePet
		if (bNewServer)
		{
			ATR_RIDE(a1) = get_bc_num();
			get_bc_asc_ridepet(a1);
			ATR_PETLEVEL(a1) = get_bc_num();
			ATR_PET_LIFE(a1) = get_bc_num();
			int k = ATR_PET_LIFE(a1);
			ATR_PET_MAX_LIFE(a1) = get_bc_num();
			int o = ATR_PET_MAX_LIFE(a1);
		}
		else
		{
			ATR_RIDE(a1) = 0;
			ATR_PETNAME(a1)[0] = NULL;
			ATR_PETLEVEL(a1) = 0;
			ATR_PET_LIFE(a1) = 0;
			ATR_PET_MAX_LIFE(a1) = 0;
		}
		//???
		if (d0 & BC_FRESH)
		{
			//????
			if ((ATR_CHR_NO(a1) >= SPR_mwood1 && ATR_CHR_NO(a1) <= SPR_mstone3) ||
				(ATR_CHR_NO(a1) == 15323) || (ATR_CHR_NO(a1) == 10812) || (ATR_CHR_NO(a1) == 15527) ||
				(ATR_CHR_NO(a1) == 15543) || (ATR_CHR_NO(a1) == 15575) || (ATR_CHR_NO(a1) == 15607))
			{
				ATR_ATTRIB(a1) &= ~ACT_ATR_HIDE;		//??
				//??
				ATR_VCT_NO(a1) = 0;
				ATR_CHR_ACT(a1) = ANIM_STAND;
				ATR_H_POS(a1) = ATR_INT_WORK0(a1) = monster_place_pos[monster_place_no[d2] * 2];
				ATR_V_POS(a1) = ATR_INT_WORK1(a1) = monster_place_pos[monster_place_no[d2] * 2 + 1];
				//???
				if (ATR_GROUP_FLG(a1) == 0)
					ATR_CHR_ANG(a1) = 3;
				else if (ATR_GROUP_FLG(a1) == 1)
					ATR_CHR_ANG(a1) = 7;
#ifdef _BATTLE_PK_PLAYER_FOR_40
				else if (ATR_GROUP_FLG(a1) == 2)
					ATR_CHR_ANG(a1) = 5;
				else if (ATR_GROUP_FLG(a1) == 3)	
					ATR_CHR_ANG(a1) = 1;
#endif
				ATR_CRS_OLD(a1) = ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];
				ATR_CHR_ACT(a1) = ANIM_WALK;
				ATR_CHR_ACT_OLD(a1) = -1;
				pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
			}
			else
			{
				//?
				ATR_VCT_NO(a1) = VCT_NO_APPEAR;
				ATR_ATTRIB(a1) &= ~ACT_ATR_HIDE;		//??
				//?
				ATR_VCT_NO(p_master) = 3;
				/* ???? */
				ATR_H_POS(a1) = monster_start_pos[monster_place_no[d2] * 2];
				ATR_V_POS(a1) = monster_start_pos[monster_place_no[d2] * 2 + 1];
				ATR_INT_WORK0(a1) = monster_place_pos[monster_place_no[d2] * 2];
				ATR_INT_WORK1(a1) = monster_place_pos[monster_place_no[d2] * 2 + 1];
			}
			//????????????
			set_raster_pos(a1);
			//148 149 150
		}
		else
		{
			if (ATR_LIFE(a1) > 0)
			{	//?????
				if (d0 & BC_FADE_OUT)
				{	// ??s???
					ATR_ATTRIB(a1) |= ACT_ATR_TRAVEL;
					//??????
					ATR_ATTACK_KIND(2, a1) = 1;
					if (ATR_GROUP_FLG(a1) == 0)
					{	//????????
						ATR_V_POS(a1) = lpDraw->ySize + 16;
						ATR_H_POS(a1) = lpDraw->xSize + SCREEN_OUT;
					}
					else
					{
						ATR_V_POS(a1) = -18;
						ATR_H_POS(a1) = -SCREEN_OUT;
					}
				}
				else
				{
					ATR_ATTRIB(a1) &= ~ACT_ATR_TRAVEL;
					ATR_H_POS(a1) = ATR_INT_WORK0(a1) = monster_place_pos[monster_place_no[d2] * 2];
					ATR_V_POS(a1) = ATR_INT_WORK1(a1) = monster_place_pos[monster_place_no[d2] * 2 + 1];
					//????????????
					set_raster_pos(a1);
				}
				//??
				ATR_VCT_NO(a1) = 0;
				ATR_CHR_ACT(a1) = ANIM_STAND;
				if (BattleBpFlag & BATTLE_BP_JOIN)
				{	// ??
					//???
					if (ATR_GROUP_FLG(a1) == 0)
						ATR_CHR_ANG(a1) = 3;
					else if (ATR_GROUP_FLG(a1) == 1)
						ATR_CHR_ANG(a1) = 7;
#ifdef _BATTLE_PK_PLAYER_FOR_40
					else if (ATR_GROUP_FLG(a1) == 2)
						ATR_CHR_ANG(a1) = 5;
					else if (ATR_GROUP_FLG(a1) == 3)	
						ATR_CHR_ANG(a1) = 1;
#endif
					ATR_CRS_OLD(a1) = ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];
					ATR_CHR_ACT(a1) = ANIM_WALK;
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
				}
			}
		}
		//???
		if (d0 & BC_DEATH)
		{
			if (ATR_VCT_NO(a1) == VCT_NO_APPEAR)
			{	//???????
				//????
				ATR_VCT_NO(a1) = VCT_NO_DIE + 2;
				ATR_H_POS(a1) = ATR_INT_WORK0(a1) = monster_place_pos[monster_place_no[d2] * 2];
				ATR_V_POS(a1) = ATR_INT_WORK1(a1) = monster_place_pos[monster_place_no[d2] * 2 + 1];
				//????????????
				set_raster_pos(a1);
				//????
				ATR_LIFE(a1) = 0;
				//????????
				if (ATR_GROUP_FLG(a1) == 0)
					ATR_CHR_ANG(a1) = 3;
				else if (ATR_GROUP_FLG(a1) == 1)
					ATR_CHR_ANG(a1) = 7;
#ifdef _BATTLE_PK_PLAYER_FOR_40
				else if (ATR_GROUP_FLG(a1) == 2)
					ATR_CHR_ANG(a1) = 5;
				else if (ATR_GROUP_FLG(a1) == 3)	
					ATR_CHR_ANG(a1) = 1;
#endif
				ATR_CRS_OLD(a1) = ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];
				ATR_CHR_ACT(a1) = ANIM_DEAD;		//????????
				ATR_CHR_ACT_OLD(a1) = -1;
				while (1)
				{
					if (pattern(a1, 1, ANM_NO_LOOP))	//??????
						break;
				}
				//????
				LoadBmp(ATR_PAT_NO(a1));
#ifndef _PETSKILL_ANTINTER
				set_piyo_loop(a1);
#endif			
				//??`
				ATR_VCT_NO(p_master) = 3;
			}
		}
		//?????ＯＫ??
		if (d0 & BC_PET_OK)
			ATR_PET_OK(a1) = 1;
		//??????????????
		if (ATR_JUJUTSU_WORK(a1) == NULL)
		{
			d3 = 0;
			if (d0 & BC_BIT3)		//  ""??
				d3 = 1;
			else if (d0 & BC_BIT4)		//  "??"??
				d3 = 2;
			else if (d0 & BC_BIT5)		//  "䞍?"??
				d3 = 3;
			else if (d0 & BC_BIT6)		//  "?"??
				d3 = 4;
			else if (d0 & BC_BIT7)		//  "???"??
				d3 = 5;
			else if (d0 & BC_BIT8)		//  "??"??
				d3 = 6;
#ifdef _MAGIC_WEAKEN
			if (d0 & BC_WEAKEN)    //虚弱
				d3 = 7;
#endif
#ifdef _MAGIC_DEEPPOISION
			if (d0 & BC_DEEPPOISON)  //剧毒
				d3 = 8;
#endif
#ifdef _MAGIC_BARRIER
			if (d0 & BC_BARRIER)  //魔障
				d3 = 9;
#endif
#ifdef _MAGIC_NOCAST
			if (d0 & BC_NOCAST)   //沉默
				d3 = 10;
#endif
#ifdef _SARS						// WON ADD 毒煞蔓延
			if (d0 & BC_SARS)		// 毒煞蔓延
				d3 = 11;
#endif
#ifdef _CHAR_PROFESSION						// WON ADD	晕眩
			if (d0 & BC_DIZZY)		// 晕眩
				d3 = 12;
			if (d0 & BC_ENTWINE)	// 树根缠绕
				d3 = 13;
			if (d0 & BC_DRAGNET)	// 天罗地网
				d3 = 14;
			if (d0 & BC_ICECRACK)	// 冰爆术
				d3 = 15;
			if (d0 & BC_OBLIVION)	// 遗忘
				d3 = 16;
			if (d0 & BC_ICEARROW)	// 冰箭
				d3 = 17;
			if (d0 & BC_BLOODWORMS)// 嗜血蛊
				d3 = 18;
			if (d0 & BC_SIGN)		// 一针见血
				d3 = 19;
			if (d0 & BC_CRAZY)		// 挑拨
				d3 = 20;
			if (d0 & BC_F_ENCLOSE)		// 火附体
				d3 = 21;
			if (d0 & BC_I_ENCLOSE)		// 冰附体
				d3 = 22;
			if (d0 & BC_T_ENCLOSE)		// 雷附体
				d3 = 23;
#ifdef _PROFESSION_ADDSKILL
			if (d0 & BC_WATER)
				d3 = 32;		// 水附体
			if (d0 & BC_WATER)
				d3 = 33;		// 恐惧
			//if (d0 & BC_F_I_T_ENCLOSE)  // 火冰雷附体
			//    d3 = 24;
#endif
#ifdef _PETSKILL_LER
			if (d0 & BC_CHANGE)
				d3 = 34; // 雷尔变身
#endif
#ifdef _PRO_KILLME
			if (d0 & BC_ANGER)
				d3 = 35; // 怒
#endif

#endif
#ifdef _PETSKILL_LER
			if (d0 & BC_CHANGE)
				d3 = 34; // 雷尔变身
#endif
			if (d3)
			{	//??????
				set_single_jujutsu(d3, a1);		//???
				ATR_STATUS(a1) = d3;		//????????
			}
			else
				ATR_STATUS(a1) = 0;		//?????
		}
		//????????
		if (ATR_ATTRIB_WORK(a1) == NULL)
		{
			if (d0 & BC_REVERSE)		//??
				set_attrib_reverse(a1);		//???
		}
	}
	//??????
	if (ATR_VCT_NO(p_master) != 3)
		action_inf = 3;		//??
}

// ???????? ????? ?************************************/
void disp_shild(void)
{
	char box[] = { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
		0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
		0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
		0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0,
		0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0,
		0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
		0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0,
		0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	DDSURFACEDESC ddsd;	// ??????
	char d6, d7;
	char *psource;		// ??????

	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC));
	ddsd.dwSize = sizeof(DDSURFACEDESC);

	// ?????????????????( ?? ddsd ?⿳?????? )
	if (lpDraw->lpBACKBUFFER->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK)
	{
		//MessageBoxNew( hWnd, "Surface的lock失败！", "确定", MB_OK | MB_ICONSTOP );
		return;
	}

	psource = box;		// ???????
	_asm{
		//		push	eax
		//		push	ebx
		//		push	ecx
		//		push	edx
		//		push	esi
		//		push	edi

		mov		edx, [ddsd.lpSurface]		//????
			add		edx, 640 - 24						//

			mov		ebx, [ddsd.lPitch]		//?????????
			sub		ebx, 16

			mov		al, 10 - 9		//????????
			mov[d6], al	//

		disp_shild_100 :
		mov		al, 30				//????????
			mov[d7], al			//
			mov		edi, edx		//?????

		disp_shild_200 :
		mov		cl, 16		//?????????
			//		mov		esi,edx
			mov		esi, [psource]

		disp_shild_300 :
					   mov		ch, 16		//?????????

				   loop_x :
						  mov		al, [esi]
						  or		al, al
						  je		skip

						  mov[edi], al

					  skip :
		inc		esi
			inc		edi
			dec		ch
			jne		loop_x

			add		edi, ebx
			dec		cl
			jne		disp_shild_300

			dec[d7]
			jne		disp_shild_200

			add		edx, 16						//
			dec[d6]
			jne		disp_shild_100

			//		pop		edi
			//		pop		esi
			//		pop		edx
			//		pop		ecx
			//		pop		ebx
			//		pop		eax
	}

	// ???????????????????
	if (lpDraw->lpBACKBUFFER->Unlock(NULL) != DD_OK)
	{
		extern int MessageBoxNew(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
		MessageBoxNew(hWnd, "Surface的Unlock失败！", "确定", MB_OK | MB_ICONSTOP);
		return;
	}

	return;
}

void petfallChangeGraph(ACTION *a0)
{
	//andy_fall
	if (ATR_PETFALL(a0) != 1)
		return;
	if (ATR_RIDE(a0) != 1)
		return;

#ifdef _PETSKILL_RIDE
	a0->saveride = ATR_CHR_NO(a0);
#endif

	for (int j = 0; j < sizeof(ridePetTable) / sizeof(tagRidePetTable); j++)
	{
		if (ridePetTable[j].rideNo == ATR_CHR_NO(a0))
		{
			ATR_CHR_NO(a0) = ridePetTable[j].charNo;
			break;
		}
	}
	ATR_PETFALL(a0) = 2;
	//andy_fall
	ATR_RIDE(a0) = 0;
}

#ifdef _PETSKILL_RIDE
void petrideChangeGraph(ACTION *a0,int ridebmp)
{
	//andy_fall
	//if (ATR_PETFALL(a0) != 2)
	//	return;
	//if (ATR_RIDE(a0) != 0)
	//	return;

	//if( a0->saveride == -1 )
	//	return;

	ATR_CHR_NO(a0) = ridebmp;

	ATR_PETFALL(a0) = 0;
	//andy_fall
	ATR_RIDE(a0) = 1;
	ATR_RIDE_FLAG(a0) = 1;
}
#endif

void LogToBattleError(char *data, int line)
{
#ifdef _STONDEBUG_
	FILE *rfp;
	rfp = fopen("battleerror.txt", "a+");
	if (rfp)
	{
		fprintf(rfp, "%d: %s\n", line, data);
		fclose(rfp);
	}
#endif
}
