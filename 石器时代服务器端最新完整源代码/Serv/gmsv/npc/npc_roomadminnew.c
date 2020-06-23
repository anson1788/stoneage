#include "version.h"

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "object.h"
#include "char.h"
#include "char_base.h"
#include "char_data.h"
#include "net.h"
#include "npcutil.h"
#include "handletime.h"
#include "readmap.h"
#include "npc_roomadminnew.h"
#include "npc_door.h"


/* 
 *
 */


enum{
	CHAR_WORKINT_RANGE     = CHAR_NPCWORKINT1,
	/* 戚及蠡濠凛棉 */
	CHAR_WORKINT_NEXTTIME  = CHAR_NPCWORKINT2,
	CHAR_WORKINT_PRINTCLOSETIME = CHAR_NPCWORKINT3,
	CHAR_WORKINT_SEARCHROOMTIME = CHAR_NPCWORKINT4,
	
};
enum {
	CHAR_WORKCHAR_MYNAME   = CHAR_NPCWORKCHAR1,
	CHAR_WORKCHAR_NEXTROOM = CHAR_NPCWORKCHAR2,
	
};

enum {
	CHAR_WORKCHAR_ADMNAME = CHAR_NPCWORKCHAR1,
};


#define     NPC_ROOMADMINNEW_DATADIR        "roomadmin/"
/* 犯□正白央奶伙及傀舰阂 */
#define     NPC_ROOMADMINNEW_EXTENSION      ".room"
#define     NPC_ROOMADMINNEW_RANKING_EXTENSION      ".score"
#define     NPC_ROOMADMINNEW_RANKING_EXTENSION_TMP  ".score.tmp"
#define     NPC_ROOMADMINNEW_SCOREFILE      ".score"
#define     NPC_ROOMADMINNEW_PRINTCLOSETIME_DEFAULT     5

#define     NPC_ROOMADMINNEW_MSG_CLOSE_BEFORE       \
"%s %s %s的活动，截止日%d分前。"
#define     NPC_ROOMADMINNEW_MSG_CLOSE              \
"。。。已截止。"
#define     NPC_ROOMADMINNEW_MSG_NONE               \
"很可惜本次没有人获得房间的拥有权。"
#define     NPC_ROOMADMINNEW_MSG_CONGRATULATION     \
"恭喜您！%s获得房间的所有权！%s的金钱全部被徵收了。%s请向管理员询问房间暗号。"
#define     NPC_ROOMADMINNEW_MSG_PRINTPASSWD_NONE    \
"由於你未持有房间因此没有暗号。"
#define     NPC_ROOMADMINNEW_MSG_DSPPASSWD1          \
"你的房间是 %s %s %s。"
#define     NPC_ROOMADMINNEW_MSG_DSPPASSWD2          \
"房间的暗号是 「%s 」。只要在对着门说话时在与尾加上「...」，别人就看不见了唷！"
#define     NPC_ROOMADMINNEW_MSG_DSPNEXTAUCTION_NONE        \
"本日的活动结束。"
#define     NPC_ROOMADMINNEW_MSG_DSPNEXTAUCTION1     \
"接下来的房间是 %s %s %s。"
#define     NPC_ROOMADMINNEW_MSG_DSPNEXTAUCTION2     \
"截止日是 %d:%d 。到截止日为止还有%2d小时%02d分。"
#define     NPC_ROOMADMINNEW_MSG_DSPNEXTAUCTION3     \
"最低得标金额是 %s 。"

#define     NPC_ROOMADMINNEW_MSG_RANKING_INFO       \
"这是高额得标者排行榜。"
#define     NPC_ROOMADMINNEW_MSG_RANKING            \
"Rank %2d:%4d/%02d/%02d %8d GOLD %s%s "

/* 仇及NPC及丢永本□斥缙 */
#define     NPC_ROOMADMINNEW_MSGCOLOR           CHAR_COLORWHITE
#define     NPC_RANKING_MSGCOLOR                CHAR_COLORWHITE
#define		NPC_ROOMADMINNEW_2WEEK				60*60*24*14

static void NPC_RoomAdminNew_DspNextAuction( int meindex, int talkerindex);
static void NPC_RoomAdminNew_DspRoomPasswd( int meindex, int adminindex, int talkerindex);
static BOOL NPC_RoomAdminNew_SetNextAuction( int meindex);
static BOOL NPC_RoomAdminNew_WriteDataFile( int meindex, int pindex, char *roomname,
											char *passwd, BOOL flg);
static void NPC_RoomAdminNew_CreatePasswd( char *passwd, int passlen);
static void NPC_RoomAdminNew_SendMsgThisFloor( int index, char *buff);
static int NPC_RoomAdminNew_SearchPlayerHasMostMoney( int meindex);
static void NPC_RoomAdminNew_CloseProc( int meindex);
static int NPC_RoomAdminNew_WriteScoreFile( int meindex, int pindex);
static void NPC_RoomAdminNew_ReadScoreFile( int meindex, int talkerindex, 
											char *admname);

/************************************************
 ************************************************/
BOOL NPC_RoomAdminNewInit( int meindex )
{
	int     i;
	char    buf[256];
	char    argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	
	for( i = 3; ; i ++ ) {
		if( getStringFromIndexWithDelim(argstr, "|" , i , buf,sizeof(buf) )) {
			if( NPC_DoorSearchByName( buf) < 0 ) {
				print( "admin not created.在密码门做好之前先等待。\n");
				return FALSE;
			}
		}
		else{ 
			break;
		}
	}
	
	NPC_Util_GetArgStr( meindex, argstr, sizeof( argstr));
	
	
	/* type本永玄 */
	CHAR_setInt( meindex, CHAR_WHICHTYPE, CHAR_TYPEROOMADMIN);
	getStringFromIndexWithDelim(argstr, "|" , 1 , buf,sizeof(buf) );
	CHAR_setWorkInt( meindex , CHAR_WORKINT_RANGE , atoi( buf ));
	if( !getStringFromIndexWithDelim(argstr, "|" , 2 , buf,sizeof(buf) )) {
		print( "arg err.admin not created.\n");
		return( FALSE);
	}
	CHAR_setWorkChar( meindex , CHAR_WORKCHAR_MYNAME , buf);
	
	CHAR_setWorkInt( meindex, CHAR_WORKINT_PRINTCLOSETIME, 0);
	
	NPC_RoomAdminNew_SetNextAuction( meindex);
	
	return TRUE;
}
/************************************************
 ************************************************/
void NPC_RoomAdminNewLoop( int meindex )
{
	int     hun;
	if( CHAR_getWorkInt( meindex, CHAR_WORKINT_NEXTTIME) == 0 ) {
		struct  tm      tm1,tm2;
		time_t          t;
		memcpy( &tm1, localtime( (time_t *)&NowTime.tv_sec), sizeof( tm1));
		t = CHAR_getWorkInt( meindex, CHAR_WORKINT_SEARCHROOMTIME);
		memcpy( &tm2, localtime( &t), 
								sizeof( tm2));
		if( tm1.tm_year != tm2.tm_year ||
			tm1.tm_mday != tm2.tm_mday ||
			tm1.tm_mon != tm2.tm_mon )
		{
			NPC_RoomAdminNew_SetNextAuction( meindex);
		}
		return;
	}
	
	hun = ceil((double)(CHAR_getWorkInt( meindex, CHAR_WORKINT_NEXTTIME) - 
		   NowTime.tv_sec) / 60.0);
	if( hun < 0 ) hun = 0;
	
	if( hun < NPC_ROOMADMINNEW_PRINTCLOSETIME_DEFAULT ) {
		if( hun != CHAR_getWorkInt( meindex, CHAR_WORKINT_PRINTCLOSETIME)) {
			char    buff[256];
			if( hun != 0 ) {
				char    buf2[1024];
				char	msgbuff[16];
				char    argstr2[NPC_UTIL_GETARGSTR_BUFSIZE];
				int		doorindex;

				doorindex = NPC_DoorSearchByName( CHAR_getWorkChar( meindex, CHAR_WORKCHAR_NEXTROOM));
				NPC_Util_GetArgStr( doorindex, argstr2, sizeof( argstr2));
				getStringFromIndexWithDelim( argstr2, "|" , 9 , 
											 buf2,sizeof(buf2) );
				if( strlen(CHAR_getChar( doorindex, CHAR_NAME) ) != 0 ) {
					strcpy( msgbuff, "号室");
				}
				else {
					msgbuff[0] = '\0';
				}
				snprintf( buff, sizeof(buff), 
							NPC_ROOMADMINNEW_MSG_CLOSE_BEFORE, 
							buf2,
							CHAR_getChar( doorindex, CHAR_NAME),
							msgbuff,
							hun);
				
				CHAR_setWorkInt( meindex, CHAR_WORKINT_PRINTCLOSETIME, hun);
			}
			else {
				/* ＞蠡户濠曰引仄凶＝ */
				strcpy( buff, NPC_ROOMADMINNEW_MSG_CLOSE);
				CHAR_setWorkInt( meindex, CHAR_WORKINT_PRINTCLOSETIME, 0);
			}
			/* 仇及白夫失卞中月谛蝈够卞霜耨 */
			NPC_RoomAdminNew_SendMsgThisFloor( meindex, buff);
		}
	}
	if( hun == 0 ) {
		NPC_RoomAdminNew_CloseProc( meindex);
	}
}

/************************************************
 ************************************************/
void NPC_RoomAdminNewTalked( int meindex , int talkerindex , char *msg ,
							 int color )
{
	
	if(NPC_Util_CharDistance( talkerindex, meindex ) > 1)return;
	
	/*NPC_RoomAdminNew_DspRoomPasswd( meindex, talkerindex);*/
	
	NPC_RoomAdminNew_DspNextAuction( meindex, talkerindex);
}
/*-----------------------------------------------
 *----------------------------------------------*/
static void NPC_RoomAdminNew_CloseProc( int meindex)
{
	int     pindex;
	char    buff[256];
	char    pname[32];
	int     failed = TRUE;
	
	pindex = NPC_RoomAdminNew_SearchPlayerHasMostMoney( meindex);
	/* 竟癫樊失伉 */
	if( pindex != -1) {
		while( 1) {
			char    passwd[9];
			int		doorindex;
			doorindex = NPC_DoorSearchByName( 
							CHAR_getWorkChar( meindex, CHAR_WORKCHAR_NEXTROOM));
			/* 由旦伐□玉毛戏岳允月 */
			NPC_RoomAdminNew_CreatePasswd( passwd, sizeof( passwd));
			if( NPC_RoomAdminNew_WriteDataFile( meindex, pindex,
							CHAR_getWorkChar( meindex, CHAR_WORKCHAR_NEXTROOM),
												passwd, 1)
				== FALSE) 
			{
				break;
			}
			/* 玉失卞由旦毛本永玄 */
			if( NPC_DoorSetPasswd( doorindex, passwd) == -1 ) {
				break;
			}
			CHAR_setWorkInt( doorindex, CHAR_WORKDOOREXPIRETIME, 
							CHAR_getWorkInt( meindex, CHAR_WORKINT_NEXTTIME)
							+ NPC_ROOMADMINNEW_2WEEK);
			NPC_RoomAdminNew_WriteScoreFile( meindex, pindex);

			CHAR_setInt( pindex, CHAR_GOLD, 0);

			CHAR_send_P_StatusString( pindex , CHAR_P_STRING_GOLD);
			
			strcpy( pname, CHAR_getChar( pindex, CHAR_NAME));
			snprintf( buff, sizeof(buff), 
						NPC_ROOMADMINNEW_MSG_CONGRATULATION, pname, pname, pname);
			failed = FALSE;
			break;
		}
	}
	
	if( failed) {
		NPC_RoomAdminNew_WriteDataFile( meindex, -1, CHAR_getWorkChar( meindex, CHAR_WORKCHAR_NEXTROOM),	NULL, 0);
		/* 竟癫樊反中引六氏丢永本□斥 */
		snprintf( buff, sizeof(buff), 
				NPC_ROOMADMINNEW_MSG_NONE);
	}
	/* 仇及白夫失卞中月谛蝈够卞霜耨 */
	NPC_RoomAdminNew_SendMsgThisFloor( meindex, buff);
	
	NPC_RoomAdminNew_SetNextAuction( meindex);
	
}
/*-----------------------------------------------
 *----------------------------------------------*/
static int NPC_RoomAdminNew_SearchPlayerHasMostMoney( int meindex)
{
	int     i,j, range;
	int     fl,x,y, gold;
	int     doorindex;
	int     topindex;
	
	fl = CHAR_getInt( meindex, CHAR_FLOOR);
	x  = CHAR_getInt( meindex, CHAR_X);
	y  = CHAR_getInt( meindex, CHAR_Y);
	range = CHAR_getWorkInt( meindex , CHAR_WORKINT_RANGE);
	topindex = -1;
	
	doorindex = NPC_DoorSearchByName( CHAR_getWorkChar( meindex, 
														CHAR_WORKCHAR_NEXTROOM));
	gold = atoi( CHAR_getWorkChar( doorindex, CHAR_WORKDOORGOLDLIMIT));
	for( i = x-range ; i <= x+range ; i++ ){
		for( j = y-range ; j <= y+range ; j ++ ){
			OBJECT  object;
			for( object = MAP_getTopObj( fl, i,j); object; 
				 object = NEXT_OBJECT(object) ) 
			{
				int objindex = GET_OBJINDEX( object);
				if( OBJECT_getType(objindex) == OBJTYPE_CHARA ){
					int     index;
					index = OBJECT_getIndex(objindex);
					if( CHAR_getInt( index,
								 CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER )
					{
						if( CHAR_getInt( index, CHAR_GOLD) >= gold ) {
							if( topindex == -1 ) {
								topindex =  index;
							}
							else {
								int nowgold,topgold;
								nowgold = CHAR_getInt( index, CHAR_GOLD );
								topgold = CHAR_getInt( topindex, CHAR_GOLD);
								if( nowgold > topgold ) {
									topindex = index;
								}
								else if( nowgold == topgold ) {
									/* 域踝卅日伊矛伙互斓中谛 */
									int nowlv, toplv;
									nowlv = CHAR_getInt( index, CHAR_LV );
									toplv = CHAR_getInt( topindex, CHAR_LV);
									if( nowlv < toplv ) {
										topindex = index;
									}
									/* 公木匹手荸瑁仄卅中氏卅日仿件母丞元扎*/
									/* index互燮及谛互切斤匀午尕厍分仃升 */
									else if( nowlv == toplv ) {
										if( RAND( 0,1)) {
											topindex = index;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return( topindex);
}
/*-----------------------------------------------
 * 隙烂今木凶index及白夫失卞中月皿伊奶乩□蝈够卞
 *----------------------------------------------*/
static void NPC_RoomAdminNew_SendMsgThisFloor( int index, char *buff)
{
	int     i, fl;
	int     playernum = CHAR_getPlayerMaxNum();
	
	fl = CHAR_getInt( index, CHAR_FLOOR);
	for( i = 0; i < playernum; i ++ ) {
		if( CHAR_getInt( i, CHAR_FLOOR) == fl ) {
			if( CHAR_getInt( i, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER ) {
				CHAR_talkToCli( i, index, buff, NPC_ROOMADMINNEW_MSGCOLOR);
			}
		}
	}
}
/*-----------------------------------------------
 *----------------------------------------------*/
static void NPC_RoomAdminNew_CreatePasswd( char *passwd, int passlen)
{
#define NPC_ROOMADMINNEW_RAND()    ( (rand() /256) % 32768 )       /* 256匹喃月及反刭醒及  撙及凶户 */
	int     i;
	char *candidates[]= {
		"A","B","D",   "E","F","G","H",   "J","K","L","M",   "N","P","Q","R",
		"T","W","X","Y",   "a","b","d","e",   "f","g","m","n",   "q","r","t",
		"0","1","2",   "3","4","5","6",   "7","8","9"
	};
	strcpy( passwd , "");
	for(i = 0;i < passlen -1 ; i++){
		strcat( passwd , candidates[ NPC_ROOMADMINNEW_RAND() % (sizeof( candidates)/sizeof(candidates[0])) ] );
	}
#undef NPC_ROOMADMINNEW_RAND
}
/*-----------------------------------------------
 *
 *
 * cdkey|charaname|passwd|expiretime
 *
 *
 *----------------------------------------------*/
static BOOL NPC_RoomAdminNew_WriteDataFile( int meindex, int pindex, 
											char *roomname,
											char *passwd, int flg)
{
	char    *cdkey;
	char    charaname[32*2];
	char    expiretime[64];
	char    filename[128];
	char    outbuf[1024];
	FILE    *fp_src;
	
	strcpy( filename, NPC_ROOMADMINNEW_DATADIR);
	/*strcat( filename, CHAR_getWorkChar( meindex, CHAR_WORKCHAR_NEXTROOM));*/
	strcat( filename, roomname);
	strcat( filename, NPC_ROOMADMINNEW_EXTENSION);

	if( flg) {
        //getcdkeyFromCharaIndex( pindex, cdkey, sizeof(cdkey ));
		cdkey = CHAR_getChar( pindex, CHAR_CDKEY);
		makeEscapeString(  CHAR_getChar( pindex, CHAR_NAME),
							charaname, sizeof( charaname));
		/* 渝蜃濠木凛棉 */
		snprintf( expiretime, sizeof( expiretime), "%d", 
					CHAR_getWorkInt( meindex, CHAR_WORKINT_NEXTTIME)
					+ NPC_ROOMADMINNEW_2WEEK   /* ㄡ葱棉詨 */
					);

		/* 犯伉立正匹仁仁月 */
		snprintf( outbuf, sizeof( outbuf), "%s|%s|%s|%s|",
					cdkey, charaname, passwd, expiretime);
		fp_src = fopen( filename, "w");
		if( !fp_src) {
			print( "[%s:%d] File Open Error\n", __FILE__, __LINE__);
			return FALSE;
		}
		fputs( outbuf, fp_src);
		fclose( fp_src);
	}
	else {
		unlink( filename);
	}
	
	return TRUE;
	
}
/*-----------------------------------------------
 *----------------------------------------------*/
BOOL NPC_RoomAdminNew_ReadFile( char *roomname, NPC_ROOMINFO *data)
{
	FILE    *fp;
	char    line[1024];
	char    buf2[128];
	char    filename[128];
	
	strcpy( filename, NPC_ROOMADMINNEW_DATADIR);
	strcat( filename, roomname);
	strcat( filename, NPC_ROOMADMINNEW_EXTENSION);
	
	fp = fopen( filename, "r");
	if( !fp) return FALSE;
	fgets( line, sizeof( line), fp);
	/* 犯□正本永玄 */
	getStringFromIndexWithDelim( line, "|" , 1 , buf2,sizeof(buf2) );
	strcpy( data->cdkey, buf2);
	getStringFromIndexWithDelim( line, "|" , 2 , buf2,sizeof(buf2) );
	strcpy( data->charaname, makeStringFromEscaped( buf2));
	getStringFromIndexWithDelim( line, "|" , 3 , buf2,sizeof(buf2) );
	strcpy( data->passwd, buf2);
	getStringFromIndexWithDelim( line, "|" , 4 , buf2,sizeof(buf2) );
	if( strcmp( buf2, "max") == 0 ) {
		data->expire = 0xffffffff;
	}
	else {
		data->expire = atoi( buf2);
	}
	fclose( fp);
	return TRUE;
}
/*-----------------------------------------------
 *----------------------------------------------*/
static BOOL NPC_RoomAdminNew_SetNextAuction( int meindex)
{
	int     i, j;
	int     hitindex = -1;
	int     charnum = CHAR_getCharNum();
	char    argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	struct  tm  tmwk;
	
	memcpy( &tmwk, localtime( ( time_t *)&NowTime.tv_sec), sizeof( tmwk));
	
	NPC_Util_GetArgStr( meindex, argstr, sizeof( argstr));

	 */
	for( i = CHAR_getPlayerMaxNum()+ CHAR_getPetMaxNum(); 
		 i < charnum;
		 i ++ )
	{
		if( CHAR_getInt( i, CHAR_WHICHTYPE) != CHAR_TYPEDOOR) {
			continue;
		}
		if( CHAR_getWorkInt( i, CHAR_WORKDOORPASSFLG) == 0 ) {
			continue;
		}
		for( j = 3; ; j ++ ) {
			int rc;
			char    *doorname;
			char    buf[256];
			int     dataexist;
			NPC_ROOMINFO data;
			
			rc = getStringFromIndexWithDelim( argstr, "|" , j,
												buf,sizeof(buf) );
			if( rc != TRUE ) {
				break;
			}
			doorname = CHAR_getWorkChar( i, CHAR_WORKDOORNAME);
			if( strcmp( buf, doorname ) != 0 ) {
				continue;
			}
			if( atoi( CHAR_getWorkChar( i, CHAR_WORKDOORWEEK)) != tmwk.tm_wday) {
				break;
			}
			/* 域杀赓渝祭 */
			memset( &data, 0, sizeof( data));
			if( NPC_RoomAdminNew_ReadFile( doorname, &data) ) {
				struct tm   tmdoor;
				 * 渝蜃互濠木化中凶日白央奶伙毛绰轮允月
				 */
				if( data.expire < NowTime.tv_sec) {
					NPC_RoomAdminNew_WriteDataFile( meindex, -1,doorname, NULL, 0);
					/* 玉失手五木中卞*/
					NPC_DoorSetPasswd( i, "8hda8iauia90494jasd9asodfasdf89");
					CHAR_setWorkInt( i, CHAR_WORKDOOREXPIRETIME, 0xffffffff);
					
					break;
				}
				memcpy( &tmdoor, localtime( ( time_t *)&data.expire), sizeof( tmdoor));
				if( tmdoor.tm_mon != tmwk.tm_mon ||
					tmdoor.tm_mday != tmwk.tm_mday)
				{
					break;
				}
				dataexist = TRUE;
			}
			else {
				dataexist = FALSE;
			}
			if( tmwk.tm_hour > atoi( CHAR_getWorkChar( i, CHAR_WORKDOORHOUR))){
				break;
			}
			if( tmwk.tm_hour == atoi( CHAR_getWorkChar( i, CHAR_WORKDOORHOUR))) {
				if( tmwk.tm_min >= atoi( CHAR_getWorkChar( i, CHAR_WORKDOORMINUTE))) {
					break;
				}
			}
			/* 仇仇引匹五凶日午曰丐尹内仇及玉失反戚钒疡卞请月凤伞互
			 */
			if( hitindex == -1 ) {
				hitindex = i;
			}
			else {
				if( atoi( CHAR_getWorkChar( hitindex, CHAR_WORKDOORHOUR)) 
					> atoi( CHAR_getWorkChar( i, CHAR_WORKDOORHOUR)))
				{
					hitindex = i;
				}
				else if( atoi( CHAR_getWorkChar( hitindex, CHAR_WORKDOORHOUR)) 
						== atoi( CHAR_getWorkChar( i, CHAR_WORKDOORHOUR)))
				{
					if( atoi( CHAR_getWorkChar( hitindex, CHAR_WORKDOORMINUTE)) 
						> atoi( CHAR_getWorkChar( i, CHAR_WORKDOORMINUTE)))
					{
						hitindex = i;
					}
					else if( atoi( CHAR_getWorkChar( hitindex, CHAR_WORKDOORMINUTE)) 
						== atoi( CHAR_getWorkChar( i, CHAR_WORKDOORMINUTE)))
					{
						NPC_ROOMINFO    datawk;
						int     hitindexdataexist;
						hitindexdataexist = NPC_RoomAdminNew_ReadFile( 
									CHAR_getWorkChar( hitindex, CHAR_WORKDOORNAME),
									&datawk);
						if( hitindexdataexist < dataexist ) {
							hitindex = i;
						}
						
					}
				}
			}
		}
	}
	if( hitindex > 0 ) {
		/* 钒疡凛棉毛本永玄 */
		tmwk.tm_sec = 0;
		tmwk.tm_min = atoi( CHAR_getWorkChar( hitindex, CHAR_WORKDOORMINUTE));
		tmwk.tm_hour = atoi( CHAR_getWorkChar( hitindex, CHAR_WORKDOORHOUR));
		CHAR_setWorkInt( meindex, CHAR_WORKINT_NEXTTIME, mktime( &tmwk));
		CHAR_setWorkChar( meindex, CHAR_WORKCHAR_NEXTROOM,
						 CHAR_getWorkChar( hitindex, CHAR_WORKDOORNAME));
		
	}
	else {
		CHAR_setWorkInt( meindex, CHAR_WORKINT_NEXTTIME, 0);
		CHAR_setWorkChar( meindex, CHAR_WORKCHAR_NEXTROOM,"" );
	
	}
	/* 腹绸凛棉毛本永玄 */
	CHAR_setWorkInt( meindex, CHAR_WORKINT_SEARCHROOMTIME, NowTime.tv_sec);
	return( TRUE);
}
/*-----------------------------------------------
 *----------------------------------------------*/
static void NPC_RoomAdminNew_DspRoomPasswd( int index, int adminindex, int talkerindex)
{
	int     i, rc, flg = FALSE;
	char    argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	char    argstr2[NPC_UTIL_GETARGSTR_BUFSIZE];
	char    *cdkey;
	char    buf[128];
	char	buff[16];
	char    buf2[1024];

	NPC_ROOMINFO    info;

    //getcdkeyFromCharaIndex( talkerindex, cdkey, sizeof(cdkey));
	cdkey = CHAR_getChar( talkerindex, CHAR_CDKEY);
	
	NPC_Util_GetArgStr( adminindex, argstr, sizeof( argstr));
	for( i = 3; ; i ++ ) {
		if( getStringFromIndexWithDelim(argstr, "|" , i , buf,sizeof(buf) )) {
			rc = NPC_RoomAdminNew_ReadFile( buf, &info);
			if( rc == TRUE ) {
				if( strcmp( info.cdkey, cdkey) == 0 &&
					strcmp( info.charaname, 
							CHAR_getChar( talkerindex, CHAR_NAME)) == 0 )
				{
					int doorindex;
					char    msgbuf[256];
					doorindex = NPC_DoorSearchByName( buf);
					NPC_Util_GetArgStr( doorindex, argstr2, sizeof( argstr2));
					getStringFromIndexWithDelim( argstr2, "|" , 9 , 
												 buf2,sizeof(buf2) );
					if( strlen(CHAR_getChar( doorindex, CHAR_NAME) ) != 0 ) {
						strcpy( buff, "号室");
					}
					else {
						buff[0] = '\0';
					}
					
					/* 丢永本□斥本永玄 */
					snprintf( msgbuf, sizeof( msgbuf), 
								NPC_ROOMADMINNEW_MSG_DSPPASSWD1,
								buf2,
								CHAR_getChar( doorindex, CHAR_NAME),
								buff
							);
					/* 丢永本□斥霜耨 */
					CHAR_talkToCli( talkerindex, index,msgbuf, 
									NPC_ROOMADMINNEW_MSGCOLOR);

					/* 丢永本□斥本永玄 */
					snprintf( msgbuf, sizeof( msgbuf), 
								NPC_ROOMADMINNEW_MSG_DSPPASSWD2,
								CHAR_getWorkChar(doorindex, CHAR_WORKDOORPASSWD)
							);
					/* 丢永本□斥霜耨 */
					CHAR_talkToCli( talkerindex, index,msgbuf, 
									NPC_ROOMADMINNEW_MSGCOLOR);
					flg = TRUE;
				}
			}
		}
		else {
			break;
		}
	}
	if( !flg) {
		CHAR_talkToCli( talkerindex, index,
						NPC_ROOMADMINNEW_MSG_PRINTPASSWD_NONE,
						NPC_ROOMADMINNEW_MSGCOLOR);
	}
}
/*-----------------------------------------------
 *----------------------------------------------*/
static void NPC_RoomAdminNew_DspNextAuction( int meindex, int talkerindex)
{
	char    msgbuf[2048];
	time_t  closetime;
	
	closetime = CHAR_getWorkInt( meindex, CHAR_WORKINT_NEXTTIME);
	if( closetime == 0 ) {
		snprintf( msgbuf, sizeof( msgbuf),
					NPC_ROOMADMINNEW_MSG_DSPNEXTAUCTION_NONE
				);
		CHAR_talkToCli( talkerindex, meindex,msgbuf, NPC_ROOMADMINNEW_MSGCOLOR);
	}
	else {
		int     doorindex;
		char    argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
		char	buff[16];
		struct  tm tmwk;
		int     difftime;
		doorindex = NPC_DoorSearchByName( 
					CHAR_getWorkChar( meindex, CHAR_WORKCHAR_NEXTROOM));
		NPC_Util_GetArgStr( doorindex, argstr, sizeof( argstr));
		getStringFromIndexWithDelim(argstr, "|" , 9 , explain,sizeof(explain));
		if( strlen(CHAR_getChar( doorindex, CHAR_NAME) ) != 0 ) {
			strcpy( buff, "号室");
		}
		else {
			buff[0] = '\0';
		}
		snprintf( msgbuf, sizeof( msgbuf),
					NPC_ROOMADMINNEW_MSG_DSPNEXTAUCTION1,
					explain,
					CHAR_getChar( doorindex, CHAR_NAME),
					buff
				);
		CHAR_talkToCli( talkerindex, meindex,msgbuf, NPC_ROOMADMINNEW_MSGCOLOR);
		/* 蠡户濠曰凛棉 */
		memcpy( &tmwk, localtime( &closetime), sizeof( tmwk));
		difftime = closetime - NowTime.tv_sec;
		snprintf( msgbuf, sizeof( msgbuf),
					NPC_ROOMADMINNEW_MSG_DSPNEXTAUCTION2,
					tmwk.tm_hour,
					tmwk.tm_min,
					difftime / 3600,
					(difftime % 3600) / 60
				);
		CHAR_talkToCli( talkerindex, meindex,msgbuf, NPC_ROOMADMINNEW_MSGCOLOR);
		
		/* 票蜃嗯喊 */
		snprintf( msgbuf, sizeof( msgbuf),
					NPC_ROOMADMINNEW_MSG_DSPNEXTAUCTION3,
					CHAR_getWorkChar( doorindex, CHAR_WORKDOORGOLDLIMIT)
				);
		CHAR_talkToCli( talkerindex, meindex,msgbuf, NPC_ROOMADMINNEW_MSGCOLOR);
	}
	
}
/*-----------------------------------------------
 *
 *
 * gold|time|cdkey|charaname|owntitle
 *
 *----------------------------------------------*/
static int NPC_RoomAdminNew_WriteScoreFile( int meindex, int pindex)
{
#define NPC_RANKING_DEFAULT     10
	int     ret = 0;
	char    *cdkey;
	char    charaname[32*2],owntitle[32*2];
	char    filename_dest[128],filename[128];
	char    outbuf[1024], line[1024];
	FILE    *fp_src, *fp_dest;

	strcpy( filename, NPC_ROOMADMINNEW_DATADIR);
	strcat( filename, CHAR_getWorkChar( meindex, CHAR_WORKCHAR_MYNAME));
	strcpy( filename_dest, filename);
	strcat( filename, NPC_ROOMADMINNEW_RANKING_EXTENSION);
	strcat( filename_dest, NPC_ROOMADMINNEW_RANKING_EXTENSION_TMP);
	
	//getcdkeyFromCharaIndex( pindex , cdkey , sizeof(cdkey ));
	cdkey = CHAR_getChar( pindex, CHAR_CDKEY);
	
	makeEscapeString(  CHAR_getChar( pindex, CHAR_NAME),
						charaname, sizeof( charaname));
	
	makeEscapeString(  CHAR_getChar( pindex, CHAR_OWNTITLE),
						owntitle, sizeof( owntitle));

	/* 犯伉立正匹仁仁月 */
	snprintf( outbuf, sizeof( outbuf), "%d|%d|%s|%s|%s|\n",
				CHAR_getInt( pindex, CHAR_GOLD),
				CHAR_getWorkInt( meindex,CHAR_WORKINT_NEXTTIME),
				cdkey, charaname, owntitle );


	fp_dest = fopen( filename_dest, "w");
	if( !fp_dest) return -1;
	
	fp_src = fopen( filename, "r");
	if( !fp_src ) {
		fclose( fp_dest);
		fp_src = fopen( filename, "w");
		if( !fp_src) return -1;
		fputs( outbuf, fp_src);
		fclose( fp_src);
		return 1;
	}
	else {
		int cnt = 1;
		char buf[128];
		while( fgets( line, sizeof( line), fp_src)) {
			getStringFromIndexWithDelim( line, "|" , 1 , buf,sizeof(buf) );
			if( ret == 0 ) {
				if( atoi( buf) < CHAR_getInt( pindex, CHAR_GOLD) ) {
					fputs( outbuf, fp_dest);
					fputs( line, fp_dest);
					ret = cnt;
					cnt ++;
				}
				else {
					/* 公木动陆反公及引引踏仁 */
					fputs( line, fp_dest);
				}
			}
			else {
				/* 公木动陆反公及引引踏仁 */
				fputs( line, fp_dest);
			}
			cnt ++;
			if( cnt > NPC_RANKING_DEFAULT ) break;
		}
		/* 仿件平件弘互ㄠㄟ蜊公欠匀化中卅中凛 */
		if( ret == 0 && cnt <= NPC_RANKING_DEFAULT ) {
			fputs( outbuf, fp_dest);
			ret = cnt;
		}
	}
	fclose( fp_src);
	fclose( fp_dest);
	fp_src = fopen( filename_dest, "r");
	if( !fp_src) return -1;
	fp_dest = fopen( filename, "w");
	if( !fp_dest) {
		fclose( fp_src);
		return -1;
	}
	while( fgets( line, sizeof( line), fp_src)) {
		fputs( line, fp_dest);
	}
	fclose( fp_dest);
	fclose( fp_src);
	return( ret);
#undef  NPC_RANKING_DEFAULT
}
/*-----------------------------------------------
 *
 *      
 *----------------------------------------------*/
static void NPC_RoomAdminNew_ReadScoreFile( int meindex, int talkerindex, 
											char *admname)
{
	int     cnt;
	char    filename[128];
	char    line[1024];
	FILE    *fp;
	
	strcpy( filename, NPC_ROOMADMINNEW_DATADIR);
	strcat( filename, admname);
	strcat( filename, NPC_ROOMADMINNEW_RANKING_EXTENSION);
	
	fp = fopen( filename, "r");
	if( !fp) return;
	cnt = 1;
	while( fgets( line, sizeof( line), fp)) {
		int     gold;
		time_t bidtime;
		char    charaname[32],owntitle[32];
		char    tmp[64];
		char    outbuf[1024], buf[256];
		
		getStringFromIndexWithDelim( line, "|" , 1 , buf,sizeof(buf) );
		gold = atoi(buf);
		getStringFromIndexWithDelim( line, "|" , 2 , buf,sizeof(buf) );
		bidtime = (time_t)atoi(buf);
		getStringFromIndexWithDelim( line, "|" , 4 , buf,sizeof(buf) );
		strcpy( charaname, makeStringFromEscaped( buf));
		getStringFromIndexWithDelim( line, "|" , 5 , buf,sizeof(buf) );
		strcpy( owntitle, makeStringFromEscaped( buf));
		if( strlen( owntitle) != 0 ) {
			strcpy( tmp, "[");
			strcat( tmp, owntitle);
			strcat( tmp, "] ");
			strcpy( owntitle, tmp);
		}
		else {
			strcpy( owntitle, " ");
		}

		struct tm *tmwk = localtime(&bidtime);
		
		snprintf( outbuf, sizeof( outbuf),
				NPC_ROOMADMINNEW_MSG_RANKING,
				cnt, 
				tmwk->tm_year + 1900,
				tmwk->tm_mon+1,
				tmwk->tm_mday,
				gold,charaname,owntitle
				);
		CHAR_talkToCli( talkerindex, meindex,outbuf, 
						NPC_RANKING_MSGCOLOR);
		cnt ++;
	}
	fclose( fp);
	
}
/************************************************
 ************************************************/
BOOL NPC_RankingInit( int meindex )
{
	char    buf[256];
	char    argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	
	NPC_Util_GetArgStr( meindex, argstr, sizeof( argstr));
	
	
	/* type本永玄 */
	CHAR_setInt( meindex, CHAR_WHICHTYPE, CHAR_TYPERANKING);
	if( !getStringFromIndexWithDelim(argstr, "|" , 1 , buf,sizeof(buf) )) {
		print( "arg err.rankingNPC not created.\n");
		return( FALSE);
	}
	CHAR_setWorkChar( meindex , CHAR_WORKCHAR_ADMNAME , buf);
	
	return TRUE;
}
/************************************************
 ************************************************/
void NPC_RankingTalked( int meindex , int talkerindex , char *msg ,
							 int color )
{
	if(NPC_Util_CharDistance( talkerindex, meindex ) > 1)return;
	
	CHAR_talkToCli( talkerindex, meindex,
					NPC_ROOMADMINNEW_MSG_RANKING_INFO, 
					NPC_RANKING_MSGCOLOR);
	
	NPC_RoomAdminNew_ReadScoreFile( meindex, talkerindex, 
						CHAR_getWorkChar( meindex, CHAR_WORKCHAR_ADMNAME));
	
}
/************************************************
 ************************************************/
BOOL NPC_PrintpassmanInit( int meindex )
{
	char    buf[256];
	char    argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	
	NPC_Util_GetArgStr( meindex, argstr, sizeof( argstr));
	
	
	/* type本永玄 */
	CHAR_setInt( meindex, CHAR_WHICHTYPE, CHAR_TYPEPRINTPASSMAN);
	if( !getStringFromIndexWithDelim(argstr, "|" , 1 , buf,sizeof(buf) )) {
		print( "arg err.passmanNPC not created.\n");
		return( FALSE);
	}
	CHAR_setWorkChar( meindex , CHAR_WORKCHAR_ADMNAME , buf);
	
	return TRUE;
}
/************************************************
 ************************************************/
void NPC_PrintpassmanTalked( int meindex , int talkerindex , char *msg ,
							 int color )
{
	int i;
	int charnum;
	if(NPC_Util_CharDistance( talkerindex, meindex ) > 2)return;
	
	charnum  = CHAR_getCharNum();
	for(i= CHAR_getPlayerMaxNum() +CHAR_getPetMaxNum();
		i<charnum;
		i++){

		if( CHAR_getCharUse(i) &&
			CHAR_getInt( i ,CHAR_WHICHTYPE) == CHAR_TYPEROOMADMIN ){
			if( strcmp( CHAR_getWorkChar(i,CHAR_WORKCHAR_MYNAME),
						CHAR_getWorkChar( meindex, CHAR_WORKCHAR_ADMNAME))
				==0 )
			{
				break;
			}
		}
	}
	if( i < charnum ) {
		NPC_RoomAdminNew_DspRoomPasswd( meindex, i, talkerindex);
	}
	else {
		CHAR_talkToCli( talkerindex, meindex, "。。。", NPC_RANKING_MSGCOLOR);
	}
}
/* end of roomadminnew */
