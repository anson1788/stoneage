#ifndef _NETPROC_H_
#define _NETPROC_H_

#ifdef _THEATER
enum
{
    E_DATA_TYPE_PLAYER,		// 玩家
    E_DATA_TYPE_TALK,		// 台词 
    E_DATA_TYPE_ASIDE,		// 旁白
    E_DATA_TYPE_ACTION,		// 动作
    E_DATA_TYPE_MOVE,		// 移动
    E_DATA_TYPE_SCENERY,	// 布景
    E_DATA_TYPE_CHANGE,		// 变身
    E_DATA_TYPE_DIR,		// 方向
    E_DATA_TYPE_EFFECT,		// 特效
    E_DATA_TYPE_FACE,		// 表情
    E_DATA_TYPE_NOTE,		// 注解
    E_DATA_TYPE_BGM,		// 背景音乐
	E_DATA_TYPE_NPC,		// NPC
    E_DATA_TYPE_END
};

// lssproto_TheaterData_recv 使用
enum
{
	E_THEATER_SEND_DATA_THEATER_MODE = E_DATA_TYPE_END + 1,	// 剧院模式 参数: 0:取消剧场模式 1:观众 2:表演者
	E_THEATER_SEND_DATA_DISPLAY_SCORE,						// 显示分数 参数: 分数
	E_THEATER_SEND_DATA_END
};
#endif

extern short selectServerIndex;
extern short clientLoginStatus;
extern short charListStatus;
extern short charDelStatus;

#ifdef _NPC_MAGICCARD
extern bool bShowflag[20];
extern int iPosition[20];	
extern int iShowdamage[20];
extern int iOffsetY[20];
extern int ioffsetsx;
extern int ioffsetsy;
extern unsigned int inextexet ;
#endif

extern char gamestate_chooseserver_name[];
extern char gamestate_login_charname[];
extern char gamestate_deletechar_charname[];

extern char netprocErrmsg[];

extern int connectServer2Counter;
/*
#ifdef _TELLCHANNEL				// (不可开) ROG ADD 密语频道
extern char ReTellName[];
#endif
*/
void initConnectServer( void );
int ConnectWGS();
int connectServer( void );

void charListStart( void );
int  charListProc( void );
void charLoginStart( void );
int charLoginProc( void );

void createNewCharStart( void );
int createNewCharProc( void );

void charLogoutStart( void );
int charLogoutProc( void );

void walkSendForServer( int, int, char * );
void noChecksumWalkSendForServer( int, int, char * );

void chatStrSendForServer( char *, int );

void delCharStart( void );
int delCharProc( void );

#ifdef _PK2007
void pkListStart( void );
int  pkListProc( void );
#endif

#endif  /* ifndef _NETPROC_H_ */
