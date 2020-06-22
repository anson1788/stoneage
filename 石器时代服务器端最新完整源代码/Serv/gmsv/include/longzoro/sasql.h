#ifndef _SASQL_H_
#define _SASQL_H_

#include "version.h"
#ifdef _SASQL

#define BOOL int
#define FALSE 0
#define TRUE 1

void sasql_close( void );
BOOL sasql_init( void );

int sasql_getVipPoint( int charaindex );
int sasql_setVipPoint( int charaindex, int point );
int sasql_getVipPointForCdkey( char *id );
int sasql_setVipPointForCdkey( char *id, int point );
BOOL sasql_addPauctionInfo( char *cdkey, char *name, char *effect, int cost, int type, char *info, char *string );
int sasql_getPauctionList( char *List, int start, int type, char *cdkey );
BOOL sasql_getPauctionSurvey( char *token, int id );
int sasql_getMyPauction( char *token, int id, char *cdkey );
BOOL sasql_delPauctionBuy( int id, char *cdkey );
int sasql_PauctionOK( int id, char *cdkey );

int sasql_getPayPoint( int charaindex );
int sasql_setPayPoint( int charaindex, int point );

#ifdef _GAMBLE_POINT
int sasql_getGamblePoint( int charaindex );
int sasql_setGamblePoint( int charaindex, int point );
#endif
#ifdef _TRIAL_POINT
int sasql_getTrialPoint( int charaindex );
int sasql_setTrialPoint( int charaindex, int point );
#endif

#ifdef _NEW_SQL_MONEY
int sasql_getSQLPoint( int charaindex );
int sasql_setSQLPoint( int charaindex, int point );
int sasql_getGoldPoint( int charaindex );
int sasql_setGoldPoint( int charaindex, int point );
int sasql_setSQLPointByCdkey( char* cdkey, int point );
int sasql_getSQLPointByCdkey( char* cdkey );
int sasql_setGoldPointByCdkey( char* cdkey, int point );
int sasql_getGoldPointByCdkey( char* cdkey );
#endif

int sasql_getPayPointForCdkey( char *id );
int sasql_setPayPointForCdkey( char *id, int point );
enum{
	INSERT,
	SELECT,
	UPDATE,
	DELETE,
};

#endif

#ifdef _JZ_NEWSCRIPT_LUA
int sasql_vippoint( char *ID, int point, int flag );
int sasql_rmbpoint( char *ID, int point, int flag );
int sasql_ampoint( char *ID, int point, int flag );

#endif
#ifdef _MO_LNS_MYSQLSUOXU
void sasql_LUASQLAdv(char* sqlstr,char* filepath,char* function,int npcindex ,int charaindex,char* msg,int flg);
void sasql_LUASQL(char* sqlstr,int row,char* filepath,char* function,int npcindex ,int charaindex,char* msg);

char* sasql_Lua_QueryR(int row,char *sqlstr);
char* sasql_LuaAdv_QueryR(char *sqlstr,int flg);
#endif
int sasql_check_iponline( char *IP );
int sasql_query_online_ip(char *IP);
int sasql_query(char *nm, char *pas);
#ifdef _MO_LOGIN_NO_KICK
BOOL sasql_CheckPasswd(char *id, char *passwd);
#endif
#endif


