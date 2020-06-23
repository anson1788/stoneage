/************************/
/*	battleMenu.h		*/
/************************/
#ifndef _BATTLE_MENU_H_
#define _BATTLE_MENU_H_

#define AI_NONE		0
#define AI_ATTACK	1
#define AI_GUARD	2
#define AI_SELECT	3
#define AI_ESCAPE	4

extern int AI;
extern DWORD PauseAI;

// BattleBpFlag?????
#define BATTLE_BP_JOIN				( 1 << 0 )		// ·
#define BATTLE_BP_PLAYER_MENU_NON	( 1 << 1 )		// ???????????
#define BATTLE_BP_BOOMERANG			( 1 << 2 )		// ?????
#define BATTLE_BP_PET_MENU_NON		( 1 << 3 )		// ?????????
#define BATTLE_BP_ENEMY_SURPRISAL	( 1 << 4 )		// ??自?▄?????自???????
#define BATTLE_BP_PLAYER_SURPRISAL	( 1 << 5 )		// ??????自?▄?????

// ??????????
#define BATTLE_BUF_SIZE	4
#define BATTLE_COMMAND_SIZE			4096
// ???????更??
extern char BattleCmd[ BATTLE_COMMAND_SIZE ];
extern char BattleCmdBak[ BATTLE_BUF_SIZE ][ BATTLE_COMMAND_SIZE ];
// ???????更???????
extern int BattleCmdReadPointer;
extern int BattleCmdWritePointer;
// ???????更????????
//extern char BattleCmdBak[];
// ???┤更??
extern char BattleStatus[ BATTLE_COMMAND_SIZE ];
extern char BattleStatusBak[ BATTLE_BUF_SIZE ][ BATTLE_COMMAND_SIZE ];
// ???┤更???????
extern int BattleStatusReadPointer;
extern int BattleStatusWritePointer;

// ???┤更????????
//extern char BattleStatusBak[];
// ???┤更????????
//extern char BattleStatusBak2[];
// ??????????
extern int battleMenuFlag2;
// 希???吻?
extern int BattleMyNo;
// ·卯??希???
extern int BattleMyMp;
// ·卯?????
extern int BattleEscFlag;
// ????????????
extern int BattlePetStMenCnt;
// ·???吻???????
extern int battlePetNoBak;
// ·???吻????????
extern int battlePetNoBak2;

// ?????
extern int BattleBpFlag;
// ????向??????
extern int BattleAnimFlag;
// ??????
extern BOOL BattleTurnReceiveFlag;
#ifdef PK_SYSTEM_TIMER_BY_ZHU
extern BOOL BattleCntDownRest;
extern BOOL SendToServer;
extern DWORD BattleCntDown;
#define BATTLE_CNT_DOWN_TIME 30000
#endif
extern int BattleCliTurnNo;
// ??????????吻?
extern int BattleSvTurnNo;

// ·卯??????????
extern BOOL BattleResultWndFlag;

#ifdef	_HUNDRED_KILL
extern BOOL BattleHundredFlag;
#endif

// ????????? *********************************************************/
void BattleMenuProc( void );
// ??????「???  *****************************************************/
void ClearBattleButton( void );
// ?????????? ********************************************************/
void InitBattleMenu( void );
// ????向??????????? ********************************************/
void CheckBattleAnimFlag( void );
// ???匠?????? ********************************************/
int CheckBattle1P2P( void );
// ▊??????牙 ***********************************************************/
void HpMeterDisp( int no );
// ?〈?牙 ************************************************************************/
void BattleNameDisp( void );
#ifdef _BATTLESKILL				// (祥褫羲) Syu ADD 桵須撮夔賡醱
void BattleSetWazaHitBox( int no , int typeflag ) ;
#endif
#ifdef PK_SYSTEM_TIMER_BY_ZHU
void BattleDown();
#endif
void battleMenuFix(void);		// 党葩桵須AI蛌遙腔BUG

#endif

