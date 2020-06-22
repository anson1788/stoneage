#ifndef _SASQL_H_
#define _SASQL_H_

#include "version.h"
#ifdef _SASQL

#define BOOL int
#define FALSE 0
#define TRUE 1

void sasql_close( void );
BOOL sasql_init( void );
int sasql_query(char *nm, char *pas);
BOOL sasql_update(char *nm, char *path);
BOOL sasql_logindate(char *nm);
BOOL sasql_online( char *ID, char *NM, char *MAC, char *IP, int flag );
BOOL sasql_register(char *id, char *ps);
BOOL sasql_craete_userinfo( void );
BOOL sasql_craete_lock( void );
BOOL sasql_chehk_lock( char *idip );
BOOL sasql_add_lock( char *idip );
BOOL sasql_del_lock( char *idip );
int sasql_onlinenum( char *MAC );
#ifdef _NEW_VIP_SHOP
BOOL sasql_add_vippoint( char *ID, int point );
int sasql_query_point( char *name );
#endif
#ifdef _ITEM_PET_LOCKED
char *sasql_ItemPetLocked( char *id, char *safepasswd );
char *sasql_ItemPetLocked_Passwd( char *id, char *safepasswd );
BOOL sasql_ItemPetLocked_Char( char *id, char *safepasswd );
#endif
#ifdef _ONLINE_COST
char *sasql_OnlineCost( char *id, char *costpasswd, int fmindex, char *fmname );
void sasql_OnlineCost_add( int cost, int num, int point);
#endif
#ifdef _SQL_BUY_FUNC
char *sasql_OnlineBuy( char *id, char *costpasswd );
void sasql_OnlineBuy_add( char *coststr, int type, int num );
#endif
#endif
#ifdef _FORMULATE_AUTO_PK
BOOL sasql_add_FormulateAutoPk( char *ID, int point );
#endif
#ifdef _OLDPS_TO_MD5PS
void sasql_OldpsToMd5ps();
#endif
#ifdef _CHARADATA_SAVE_SQL
void sasql_charadata_Save(char *id, char* table, char *data, int saveindex, int flag);



enum{
	INSERT,
	SELECT,
	UPDATE,
	DELETE,
};

#endif






#endif


