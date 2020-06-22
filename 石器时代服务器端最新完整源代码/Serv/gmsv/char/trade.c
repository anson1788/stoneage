#include "version.h"
#include "correct_bug.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "readmap.h"
#include "object.h"
#include "char.h"
#include "char_base.h"
#include "lssproto_serv.h"
#include "npcutil.h"
#include "trade.h"
#include "log.h"
#include "handletime.h"
#include "buf.h"
#include "net.h"
#include "char_base.h"
#include "battle.h"
#include "npc_bus.h"
#include "char_talk.h"
#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
#include "pet_skill.h"
#endif
#ifdef _NEW_ITEM_
extern int CheckCharMaxItem(int charindex);
#endif
#define TRADE_WAIT		"请稍候，连络%s中···"
#define TRADE_REFUSE   "%s拒绝了你的交易请求！"
#define TRADE_NONE		"前方什麽人也没有！"
#define	TRADE_OVERPLAYER	"前方不只一位玩家喔！"
#define	TRADE_NOWILL		"真是抱歉，对方不愿意跟你交易！"
#define	TRADE_TRADING		"%s正在与其他人交易···"
#define	TRADE_VANISHITEM	"丢在地上会消失的物品无法交易！"
#define TRADE_PETFULL		"%s宠物栏已满，交易取消！"
#define	TRADE_ITEMFULL		"%s物品栏已满，请整理後再交易～"
#define TRADE_CANCEL		"%s将交易取消了！"
#define TRADE_POOR		"%s钱不够，无法交易！"
#define	TRADE_RICH		"%s金钱交易请勿超过一百万！"
#define TRADE_TOORICH		"%s金钱将会超过一百万！"
#define TRADE_POORLV		"%s等级不够，无法照顾交易後的宠物！"
#define	TRADE_LOCK		"交易锁定···"
#define	TRADE_SUCCESS		"交易ＯＫ！"
#define	TRADE_FAILED		"交易失败！"
#define TRADE_FMPET		"家族守护兽无法交易！"
#ifdef _LOCK_PET_ITEM
#define TRADE_LOCKPET		"绑定的宠物无法交易！"
#define TRADE_LOCKITEM	"绑定的道具无法交易！"
#define TRADE_LOCKPETITEM	"宠物身上有绑定的道具无法交易！"
#endif
#define TRADE_NOITEM	"存在无法交易的物品！"
#define TRADE_NOPET	"存在无法交易的宠物！"
#define TRADE_RDCANCEL	"交易取消！"



#ifdef _ITEM_PILEFORTRADE
enum{
	TRADE_ITEM=0,
	TRADE_PET,
	TRADE_GOLD,
};

#define MAX_TRADELISTNUM 100

STradeList TradeList[MAX_TRADELISTNUM][2];

void TRADE_ResetTradeList( int ti);
int TRADE_getTradeListIndex( void);
int TRADE_getMyTarget( int meindex );
BOOL TRADE_setTradeListIndex( int ti, int meindex, int toindex);
BOOL TRADE_addSomeTradeList( int meindex, int usTi, int *num, int type);
BOOL TRADE_getSomeTradeList( int meindex, STradeList *temp);

void TRADE_InitTradeList( void)
{
	int i;
	print("初始化交易系统...");
	for( i=0; i<MAX_TRADELISTNUM; i++){
		TRADE_ResetTradeList( i);
	}
	print("完成\n");
}

void TRADE_ResetTradeList( int ti)
{
	int i, fd;
	if( ti<0 || ti >= MAX_TRADELISTNUM )return;
	for( i=0; i<5; i++){
		TradeList[ti][0].PetTi[i] = -1;
		TradeList[ti][1].PetTi[i] = -1;
	}
	for( i=0; i<15; i++){
		TradeList[ti][0].ItemTi[i] = -1;
		TradeList[ti][0].ItemNum[i] = -1;
		TradeList[ti][1].ItemTi[i] = -1;
		TradeList[ti][1].ItemNum[i] = -1;
	}

	TradeList[ti][0].Golds = 0;
	TradeList[ti][1].Golds = 0;

	
	if( CHAR_CHECKINDEX(TradeList[ti][0].charaindex)){
		fd = getfdFromCharaIndex( TradeList[ti][0].charaindex);
		CONNECT_setTradeList( fd, -1);
	}
	if( CHAR_CHECKINDEX(TradeList[ti][1].charaindex)){
		fd = getfdFromCharaIndex( TradeList[ti][1].charaindex);
		CONNECT_setTradeList( fd, -1);
	}

	TradeList[ti][0].charaindex = -1;
	TradeList[ti][1].charaindex = -1;
	TradeList[ti][0].fd = -1;
	TradeList[ti][1].fd = -1;
	
	TradeList[ti][0].use = 0;
	TradeList[ti][1].use = 0;

}

int TRADE_getTradeListIndex( void)
{
	int i;
	static int tradeTi=0;
	if( tradeTi < 0 ) tradeTi = 0;
	for( i=0; i<MAX_TRADELISTNUM; i++){
		if( TradeList[tradeTi][0].use == 0 && TradeList[tradeTi][1].use == 0 ){
			return tradeTi;
		}
		tradeTi++;
		if( tradeTi >= MAX_TRADELISTNUM ) tradeTi = 0;

	}
	return -1;
}

BOOL TRADE_setTradeListIndex( int ti, int meindex, int toindex)
{
	int fd;
	if( ti<0 || ti >= MAX_TRADELISTNUM ) return FALSE;
	if( TradeList[ti][0].use == 1 || TradeList[ti][1].use == 1 ) return FALSE;
	TRADE_ResetTradeList( ti);
	TradeList[ti][0].charaindex = meindex;
	TradeList[ti][1].charaindex = toindex;
	TradeList[ti][0].use = 1;
	TradeList[ti][1].use = 1;

	fd = getfdFromCharaIndex( meindex);
	TradeList[ti][0].fd = fd;
	CONNECT_setTradeList( fd, ti);

	fd = getfdFromCharaIndex( toindex);
	TradeList[ti][1].fd = fd;
	CONNECT_setTradeList( fd, ti);
	return TRUE;
}

void TRADE_CheckTradeListUser( void)
{
	int i, usenum=0, replace=0, freenum=0;
	for( i=0; i<MAX_TRADELISTNUM; i++){
		if( TradeList[i][0].use == 0 && TradeList[i][1].use == 0 ){
			freenum++;
			continue;
		}
		if( CHAR_CHECKINDEX( TradeList[i][0].charaindex ) &&
			CHAR_getWorkInt( TradeList[i][0].charaindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_FREE &&
			CHAR_CHECKINDEX( TradeList[i][1].charaindex ) &&
			CHAR_getWorkInt( TradeList[i][1].charaindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_FREE ){
			usenum++;
			continue;
		}else{
			TRADE_ResetTradeList( i);
			replace ++;
		}
	}
}

int TRADE_getMyTarget( int meindex )
{
	int i, fd, ti, side=-1;
	fd = getfdFromCharaIndex( meindex);
	ti = CONNECT_getTradeList( fd);

	if( ti<0 || ti >= MAX_TRADELISTNUM ) return -1;
	if( TradeList[ti][0].use == 0 || TradeList[ti][1].use == 0 ) return -1;

	for( i=0; i<2; i++){
		if( TradeList[ti][i].charaindex == meindex &&
			TradeList[ti][i].fd == fd ){
			side = (i+1)%2;
			if( !CHAR_CHECKINDEX( TradeList[ti][side].charaindex) ) return -1;
			if( TradeList[ti][side].charaindex == meindex ) return -1;
			if( TradeList[ti][side].fd != getfdFromCharaIndex( TradeList[ti][side].charaindex) ) return -1;

			return TradeList[ti][side].charaindex;
		}
	}
	return -1;
}

BOOL TRADE_addSomeTradeList( int meindex, int usTi, int *num, int type)
{
	int i, fd, ti, side=-1, toindex=-1;
	fd = getfdFromCharaIndex( meindex);
	ti = CONNECT_getTradeList( fd);
	if( ti <0 || ti >= MAX_TRADELISTNUM ) return FALSE;
	toindex =-1;
	if( TradeList[ti][0].use != 1 || TradeList[ti][1].use != 1 ) return FALSE;
	for( i=0; i<2; i++){
		if( TradeList[ti][i].charaindex != meindex ){
			continue;
		}
		side = i;
		toindex = TradeList[ti][((side+1)%2)].charaindex;
		if( !CHAR_CHECKINDEX( toindex) ) return FALSE;
		break;
	}
	if( side == -1 ) return FALSE;
	switch( type){
	case TRADE_ITEM:
		{
			int itemindex, MeMaxPile;
			MeMaxPile = CHAR_getMyMaxPilenum( meindex);//最大堆叠数
			if( usTi < CHAR_STARTITEMARRAY ||
#ifdef _NEW_ITEM_
				usTi >= CheckCharMaxItem(meindex)
#else
				usTi >= CHAR_MAXITEMHAVE
#endif
				) return FALSE;
			itemindex = CHAR_getItemIndex( meindex, usTi );
			if( !ITEM_CHECKINDEX( itemindex))	return FALSE;

			if( ITEM_getInt( itemindex, ITEM_VANISHATDROP) == 1 ){
				char buf[256];
				sprintf( buf, "%s无法交易。", ITEM_getChar( itemindex, ITEM_NAME) );
				CHAR_talkToCli( meindex, -1, buf, CHAR_COLORYELLOW);
				return FALSE;
			}
			
			if(FreeTradeItem(meindex, itemindex)){
				return FALSE;
			}
#ifdef _LOCK_PET_ITEM
			char *arg = ITEM_getChar(itemindex, ITEM_NAME);
			if(arg[0] == '*'){
				char buf[256];
				sprintf( buf, "%s无法交易。", ITEM_getChar( itemindex, ITEM_NAME) );
				CHAR_talkToCli( meindex, -1, buf, CHAR_COLORYELLOW);
				return	FALSE;
			}
#endif
			if( ITEM_getInt( itemindex, ITEM_CANBEPILE) == 1 ){//可堆叠
				int itemMaxPile = ITEM_getInt( itemindex, ITEM_USEPILENUMS);
				if( itemMaxPile < 0  ) return FALSE;
				//ITEM_USEPILENUMS
				for( i=0; i<15; i++){
					if( TradeList[ti][side].ItemTi[i] != usTi ) continue;
					if( TradeList[ti][side].ItemNum[i] >= MeMaxPile ||
						TradeList[ti][side].ItemNum[i] >= itemMaxPile ) return FALSE;

					TradeList[ti][side].ItemNum[i] += 1;
					if( num != NULL ) *num = TradeList[ti][side].ItemNum[i];
					return TRUE;
				}
			}
			for( i=0; i<15; i++){
				if( TradeList[ti][side].ItemTi[i] != -1 ) continue;
				TradeList[ti][side].ItemTi[i] = usTi;
				TradeList[ti][side].ItemNum[i] = 1;
				if( num != NULL ) *num = TradeList[ti][side].ItemNum[i];
				return TRUE;
			}
		}
		break;
	case TRADE_PET:
		{
			int petindex;
			if( usTi < 0 || usTi >= CHAR_MAXPETHAVE ) return FALSE;
			petindex = CHAR_getCharPet( meindex, usTi );
			if( !CHAR_CHECKINDEX(petindex))	return FALSE;
			if( CHAR_getInt( petindex, CHAR_PETFAMILY) == 1 ){
				CHAR_talkToCli( meindex, -1, "家族守护兽无法交易。", CHAR_COLORYELLOW);
				return FALSE;//家族守护兽
			}
			if(FreeTradePet(meindex, petindex) == 1){
				return FALSE;//家族守护兽
			}
			
#ifdef _LOCK_PET_ITEM
{
			char *petname = CHAR_getChar( petindex, CHAR_NAME);
			if( petname[0] == '*' ){
				CHAR_talkToCli( meindex, -1, "绑定宠物无法交易！", CHAR_COLORYELLOW);
				return FALSE;
			}
#ifdef _PET_ITEM
{
			int i;		
			for( i = 0 ; i < CHAR_MAXPETITEMHAVE ; i ++ ){
				int itemindex = CHAR_getItemIndex( petindex, i );
				char *arg = ITEM_getChar(itemindex, ITEM_NAME);
				if(arg[0] == '*'){
					CHAR_talkToCli( meindex, -1, "宠物身上有绑定的道具无法交易！", CHAR_COLORYELLOW);
					return FALSE;
				}
			}
}
#endif
}
#endif
			if( CHAR_getWorkInt( toindex, CHAR_PickAllPet) != TRUE ){
				if( CHAR_getInt( petindex, CHAR_LV) > (CHAR_getInt( toindex, CHAR_LV)+20) &&
					(CHAR_getInt( toindex, CHAR_TRANSMIGRATION ) <= 0 ) ){
					CHAR_talkToCli( meindex, -1, "对方无法照顾该宠物。", CHAR_COLORYELLOW);
					return FALSE;
				}
			}

			for( i=0; i<5; i++){
				if( TradeList[ti][side].PetTi[i] == usTi ) return FALSE;
			}
			for( i=0; i<5; i++){
				if( TradeList[ti][side].PetTi[i] != -1 ) continue;
				TradeList[ti][side].PetTi[i] = usTi;
				return TRUE;
			}
		}
		break;
	case TRADE_GOLD:
		TradeList[ti][side].Golds = usTi;
		return TRUE;
		break;
	}

	return FALSE;
}

BOOL TRADE_getSomeTradeList( int meindex, STradeList *temp)
{
	int i, fd, ti, side=-1;
	fd = getfdFromCharaIndex( meindex);
	ti = CONNECT_getTradeList( fd);
	if( ti <0 || ti >= MAX_TRADELISTNUM ) return FALSE;
	if( TradeList[ti][0].use != 1 || TradeList[ti][1].use != 1 ) return FALSE;
	for( i=0; i<2; i++){
		if( TradeList[ti][i].charaindex != meindex ) continue;
		side = i;
		break;
	}
	if( side == -1 ) return FALSE;
	
	memcpy( temp, &TradeList[ti][side], sizeof( STradeList) );
	return TRUE;
}
BOOL TRADE_CheckTradeList( int meindex, STradeList *temp1, int toindex, STradeList *temp2);
#endif

void TRADE_Will(int fd, int meindex, char* message);
void TRADE_ShowItem(int fd, int meindex, char* message);
void TRADE_Close(int fd, int meindex, char* message);

void TRADE_SwapItem(int meindex, int toindex, char* message, int fd, char* mycharaname, int tofd, char* tocharaname);

int TRADE_CheckItembuf(int fd, int meindex, int toindex, int tofd, char* mycharaname, char* tocharaname);
//BOOL TRADE_ChangeItem(int meindex, int toindex, char *a, char *b, int item1, int item2, int itemindex1, int itemindex2);
BOOL TRADE_HandleItem( int meindex, int showindex, char *message, char *outmess);
BOOL TRADE_HandleGold( int meindex, int showindex, char *message, char *outmess);
BOOL TRADE_HandlePet( int meindex, int showindex, char *message, char *outmess);



void CHAR_Trade(int fd, int index, char* message)
{
   char		firstToken[64];
   // shan 以下注掉是不必要的动作  2002/03/05
   //char		messageeraseescape[512];
   char*	messagebody;
   
   {   		
   	if (*message == 0) 	return;
    if (!CHAR_CHECKINDEX(index))	return;

   	CHAR_getMessageBody( message, firstToken, sizeof(firstToken), &messagebody);

	switch( tolower( firstToken[0]) ){
	   case 'd':	// 寻找前方玩家
		TRADE_Search(fd, index, message);
	   	break;
#ifdef _COMFIRM_TRADE_REQUEST
	   case 'c':
	   	TRADE_Will(fd, index, message);
	   	break;
#endif
	   case 't':	// 显示交易的物品、金钱、宠物
	   	TRADE_ShowItem(fd, index, message);
	   	break;
	   case 'w':	// 关闭交易
		TRADE_Close(fd, index, message);
	   	break;
	   default:
	   	break;
	}
   }
}

BOOL TRADE_Search(int fd, int meindex, char* message)
{
   int		objbuf[16];
   int		front_x, front_y, i, found_count;
   BOOL		found =  FALSE, searchflg = FALSE;
   int 		cnt = 0, tofd = -1, checkfd = -1;
   char		msgbuf[1024], mycharaname[256], tocharaname[256];
   char		token[256];

   if (!CHAR_CHECKINDEX(meindex))	return FALSE;

   // 若玩家状态为交易中或交易锁定中则不予处理
   if (CHAR_getWorkInt(meindex, CHAR_WORKTRADEMODE) == CHAR_TRADE_TRADING
   	|| CHAR_getWorkInt(meindex, CHAR_WORKTRADEMODE) == CHAR_TRADE_LOCK)
   		return FALSE;

   // 若玩家状态为组队或战斗中则不予处理
   if ((CHAR_getWorkInt(meindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE)
   	|| (CHAR_getWorkInt(meindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE))
   		return FALSE;

#ifdef _STREET_VENDOR
	 // 若玩家在摆摊中不处理交易
		if(CHAR_getWorkInt(meindex,CHAR_WORKSTREETVENDOR) > -1) return FALSE;
#endif

   strcpy(mycharaname, CHAR_getChar(meindex, CHAR_NAME));

   if (getStringFromIndexWithDelim(message, "|", 2, token,
	sizeof(token)) == FALSE)	return FALSE;
   // 设定状态交易请求中
//   CHAR_setWorkInt(meindex, CHAR_WORKTRADEMODE, CHAR_TRADE_SENDING);
   
#ifndef _ITEM_PILEFORTRADE
	for (i = 0; i < CONNECT_WINDOWBUFSIZE; i++)
		CONNECT_setTradecharaindex(fd, i, -1);
#endif
   CHAR_getCoordinationDir(CHAR_getInt(meindex, CHAR_DIR),
   		CHAR_getInt(meindex, CHAR_X),
   		CHAR_getInt(meindex, CHAR_Y),
   		1, &front_x, &front_y);

	found_count = CHAR_getSameCoordinateObjects( objbuf, arraysizeof(objbuf),
							CHAR_getInt(meindex, CHAR_FLOOR),front_x, front_y);
	// 前方没有玩家或对方关闭交易选项
	if (found_count == 0){
		CHAR_talkToCli(meindex, -1, TRADE_NONE, CHAR_COLORYELLOW);
		CHAR_setWorkInt(meindex, CHAR_WORKTRADEMODE, CHAR_TRADE_FREE);
		return FALSE;
	}
   
	for (i = 0; i < found_count; i++){
		int	objindex = objbuf[i];
		int	index = OBJECT_getIndex(objindex);

		if (OBJECT_getType(objindex) != OBJTYPE_CHARA)	continue;
		if (CHAR_getInt(index, CHAR_WHICHTYPE) != CHAR_TYPEPLAYER)	continue;
		if (index == meindex)	continue;
		found = TRUE;
		if (CHAR_getWorkInt(index, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE)	continue;
		if (CHAR_getWorkInt(index, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE)	continue;
		if (CHAR_getFlg(index, CHAR_ISTRADE) == FALSE)	continue;
		if (CHAR_getWorkInt(index, CHAR_WORKTRADEMODE) != CHAR_TRADE_FREE)	continue;
		strcpy(tocharaname, CHAR_getChar(index, CHAR_NAME));
		tofd = getfdFromCharaIndex(index);
		if (tofd == -1){
			CHAR_setWorkInt(meindex, CHAR_WORKTRADEMODE, CHAR_TRADE_FREE);
			return	FALSE;
		}	
		if (searchflg){
			if (tofd != checkfd)	continue;
			if (strcmp(token, tocharaname) != 0)	continue;
		}
#ifndef _ITEM_PILEFORTRADE
		CONNECT_setTradecharaindex(fd, cnt, index);
#endif
		cnt ++;
		if (cnt == CONNECT_WINDOWBUFSIZE)	break;
	}

	if (cnt == 0){
		// 前方没有玩家或对方关闭交易选项
		if (found){
			CHAR_talkToCli(meindex, -1, TRADE_NOWILL, CHAR_COLORYELLOW);
		}else
			CHAR_talkToCli(meindex, -1, TRADE_NONE, CHAR_COLORYELLOW);

		sprintf(msgbuf, "C|%d|%s|0", tofd, tocharaname);
		CHAR_setWorkInt(meindex, CHAR_WORKTRADEMODE, CHAR_TRADE_FREE);
		return FALSE;
	}
	// 前方有一位玩家
	if (cnt == 1){
		int toindex;
		toindex = CONNECT_getCharaindex(tofd);
		if( !CHAR_CHECKINDEX(toindex))       return FALSE;
#ifdef _ITEM_PET_LOCKED
		if(CHAR_getInt(meindex, CHAR_LOCKED)==1){
			char message[256];
			char	buf[256];
			sprintf( message, "为了确保你的物品安全，请输入你的安全密码进行解锁！\n");
		
			lssproto_WN_send( getfdFromCharaIndex(meindex), WINDOW_MESSAGETYPE_MESSAGEANDLINEINPUT, 
							WINDOW_BUTTONTYPE_OKCANCEL,
							CHAR_WINDOWTYPE_ITEM_PET_LOCKED,
							-1,
							makeEscapeString( message, buf, sizeof(buf)));
							
			return FALSE;
		}else if(CHAR_getInt(toindex, CHAR_LOCKED)==1){
			char message[256];
			sprintf( message, "对方 %s 安全锁定，无法与你交易！\n", CHAR_getChar(toindex, CHAR_NAME));
			CHAR_talkToCli(meindex, -1, message, CHAR_COLORYELLOW);
			return FALSE;
		}
#endif
		
#ifdef _ITEM_PILEFORTRADE
		int ti;
		
		if( (ti = TRADE_getTradeListIndex()) < 0 ||
			TRADE_setTradeListIndex( ti, meindex, toindex) == FALSE ){

			CHAR_talkToCli( meindex, -1, "系统忙碌中。", CHAR_COLORYELLOW);
			CHAR_setWorkInt( meindex, CHAR_WORKTRADEMODE, CHAR_TRADE_FREE);
			TRADE_CheckTradeListUser();

   			return FALSE;
		}
		
		//TRADE_setTradeListIndex( ti, meindex, toindex); //重复写，去掉
#endif
		snprintf(msgbuf, sizeof(msgbuf), TRADE_WAIT, tocharaname);
   		CHAR_talkToCli(meindex, -1, msgbuf, CHAR_COLORYELLOW);    //主动交易方,发送连络xx中

#ifdef _COMFIRM_TRADE_REQUEST
   		sprintf(msgbuf, "C|%d|%s|2", tofd, tocharaname);
   		lssproto_TD_send(fd, msgbuf);
#else
   		sprintf(msgbuf, "C|%d|%s|1", fd, mycharaname);
   		lssproto_TD_send( tofd, msgbuf);
   		sprintf(msgbuf, "C|%d|%s|1", tofd, tocharaname);
   		lssproto_TD_send(fd, msgbuf);

   		CHAR_setWorkInt(meindex, CHAR_WORKTRADEMODE, CHAR_TRADE_TRADING);
   		CHAR_setWorkInt(toindex, CHAR_WORKTRADEMODE, CHAR_TRADE_TRADING);

		CONNECT_set_confirm(fd, FALSE);
		CONNECT_set_confirm(tofd, FALSE);

        CHAR_sendTradeEffect(meindex, 1);
        CHAR_sendTradeEffect(toindex, 1);
#endif
        return TRUE;
   }else if (cnt > 1){	// 前方不只一位玩家
		CHAR_talkToCli(meindex, -1, TRADE_OVERPLAYER, CHAR_COLORYELLOW);
//		sprintf(msgbuf, "C|%d|%s|0", tofd, tocharaname);
			CHAR_setWorkInt(meindex, CHAR_WORKTRADEMODE, CHAR_TRADE_FREE);
   		return FALSE;
    }

	CHAR_setWorkInt(meindex, CHAR_WORKTRADEMODE, CHAR_TRADE_FREE);
	return FALSE;
}

void TRADE_Will(int fd, int meindex, char* message)
{
	char msg[128], msgbuf[1024], mycharaname[256], tocharaname[256];
	int tofd=-1, toindex=-1;
	if (!CHAR_CHECKINDEX(meindex))	return;
	if (*message == 0)	return;
	strcpy(mycharaname, CHAR_getChar(meindex, CHAR_NAME));
	toindex = TRADE_getMyTarget( meindex);//获取跟自己交易的对像？
	if( CHAR_CHECKINDEX( toindex) ){
		strcpy(tocharaname, CHAR_getChar(toindex, CHAR_NAME));
		if( getStringFromIndexWithDelim( message, "|", 4, msg, sizeof(msg)) == FALSE)	return;

		if(strcmp( msg, "0" )==0)
		{
			snprintf(msgbuf, sizeof(msgbuf), TRADE_REFUSE, tocharaname);
			CHAR_talkToCli(toindex, -1, msgbuf, CHAR_COLORYELLOW);    //拒绝交易
			int ti = CONNECT_getTradeList(fd);
			if(ti !=-1)
			{
				TRADE_ResetTradeList(ti);
			}
		}
		else if(strcmp( msg, "1" )==0)
		{
			sprintf(msgbuf, "C|%d|%s|1", tofd, tocharaname);
			lssproto_TD_send(fd, msgbuf);

			CHAR_setWorkInt(meindex, CHAR_WORKTRADEMODE, CHAR_TRADE_TRADING);
			CHAR_setWorkInt(toindex, CHAR_WORKTRADEMODE, CHAR_TRADE_TRADING);

			CONNECT_set_confirm(fd, FALSE);
			CONNECT_set_confirm(tofd, FALSE);

			CHAR_sendTradeEffect(meindex, 1);
			CHAR_sendTradeEffect(toindex, 1);
		}
	}
}

void TRADE_Close(int fd, int meindex, char* message)
{
	char msg[128], mycharaname[256];
	int tofd=-1, toindex=-1;
	int j;

	if (!CHAR_CHECKINDEX(meindex))	return;
	if (*message == 0)	return;
	strcpy(mycharaname, CHAR_getChar(meindex, CHAR_NAME));
#ifdef _ITEM_PILEFORTRADE
	toindex = TRADE_getMyTarget( meindex);//获取跟自己交易的对像？

	if( CHAR_CHECKINDEX( toindex) ){
		tofd = getfdFromCharaIndex( toindex);
		snprintf( msg, sizeof(msg), TRADE_CANCEL, mycharaname);
		CHAR_talkToCli(toindex, -1, msg, CHAR_COLORYELLOW);
		snprintf( msg, sizeof(msg), "W|%d|%s", fd, mycharaname);
		lssproto_TD_send( tofd, msg);

		CHAR_setWorkInt(meindex, CHAR_WORKTRADEMODE, CHAR_TRADE_FREE);
		CHAR_setWorkInt( toindex, CHAR_WORKTRADEMODE, CHAR_TRADE_FREE);
		CONNECT_setTradeTmp( tofd, "");
		CHAR_sendStatusString( toindex, "i");
		CHAR_sendStatusString( toindex, "P");
		CHAR_sendTradeEffect( toindex, 0);
		CONNECT_set_confirm( tofd, FALSE);

		for(j = 0; j < CHAR_MAXPETHAVE; j++){
			char msgbuf[256];
			int petindex = CHAR_getCharPet( toindex, j);
			if( !CHAR_CHECKINDEX( petindex) ) continue;
			CHAR_complianceParameter( petindex );
			snprintf( msgbuf, sizeof( msgbuf ), "K%d", j );
			CHAR_sendStatusString( toindex, msgbuf );
			snprintf( msgbuf, sizeof( msgbuf ), "W%d", j );
			CHAR_sendStatusString( toindex, msgbuf );
		}
	}

#else
	char token[256];
   if (getStringFromIndexWithDelim(message, "|", 2, token, sizeof(token)) == FALSE)	return;
   tofd = atoi(token);
   toindex = CONNECT_getCharaindex(tofd);
   if (!CHAR_CHECKINDEX(toindex))       return;
   if (getStringFromIndexWithDelim(message, "|", 3, token, sizeof(token)) == FALSE)	return;

   
   snprintf(msg, sizeof(msg), TRADE_CANCEL, mycharaname);
   CHAR_talkToCli(toindex, -1, msg, CHAR_COLORYELLOW);
   snprintf( msg, sizeof(msg), "W|%d|%s", fd, mycharaname);
   lssproto_TD_send( tofd, msg);

   CHAR_setWorkInt(meindex, CHAR_WORKTRADEMODE, CHAR_TRADE_FREE);
   CHAR_setWorkInt(toindex, CHAR_WORKTRADEMODE, CHAR_TRADE_FREE);
   CONNECT_setTradeTmp(tofd, "");
   CHAR_sendStatusString( CONNECT_getCharaindex(tofd), "i");
   CHAR_sendStatusString( CONNECT_getCharaindex(tofd), "P");
   CHAR_sendTradeEffect( toindex, 0);
   CONNECT_set_confirm(tofd, FALSE);
#endif

	if( !CHAR_CHECKINDEX( toindex) ){
		char buf[256];
		if( getStringFromIndexWithDelim( message, "|", 2, buf, sizeof(buf)) == FALSE)	return;
		tofd = atoi( buf);
		toindex = CONNECT_getCharaindex( tofd);
		if (!CHAR_CHECKINDEX(toindex) )
		{
			snprintf( msg, sizeof(msg), TRADE_RDCANCEL);
			CHAR_talkToCli(meindex, -1, msg, CHAR_COLORYELLOW);

			CHAR_setWorkInt( meindex, CHAR_WORKTRADEMODE, CHAR_TRADE_FREE);
			CONNECT_setTradeTmp(fd, "");
			CHAR_sendStatusString(meindex, "i");
			CHAR_sendStatusString(meindex, "P");
			CHAR_sendTradeEffect( meindex, 0);
			CONNECT_set_confirm(fd, FALSE);
			for(j = 0; j < CHAR_MAXPETHAVE; j++){
				char msgbuf[256];
				int petindex = CHAR_getCharPet( meindex, j);
				if( !CHAR_CHECKINDEX( petindex) ) continue;
				CHAR_complianceParameter( petindex );
				snprintf( msgbuf, sizeof( msgbuf ), "K%d", j );
				CHAR_sendStatusString( meindex, msgbuf );
				snprintf( msgbuf, sizeof( msgbuf ), "W%d", j );
				CHAR_sendStatusString( meindex, msgbuf );
			}
#ifdef _ITEM_PILEFORTRADE
			{
				int ti=0;
				ti = CONNECT_getTradeList( fd);
				if( ti <0 || ti >= MAX_TRADELISTNUM ) return;
				TRADE_ResetTradeList( ti);
			}
#endif
			return;
		}
	}

	snprintf( msg, sizeof(msg), TRADE_RDCANCEL);
	CHAR_talkToCli(meindex, -1, msg, CHAR_COLORYELLOW);
	snprintf( msg, sizeof(msg), "W|%d|%s", tofd, CHAR_getChar( toindex, CHAR_NAME) );
	lssproto_TD_send( fd, msg);

	CHAR_setWorkInt( meindex, CHAR_WORKTRADEMODE, CHAR_TRADE_FREE);
	CHAR_setWorkInt( toindex, CHAR_WORKTRADEMODE, CHAR_TRADE_FREE);
	CONNECT_setTradeTmp(fd, "");
	CHAR_sendStatusString(meindex, "i");
	CHAR_sendStatusString(meindex, "P");
	CHAR_sendTradeEffect( meindex, 0);
	CONNECT_set_confirm(fd, FALSE);
// end
	for(j = 0; j < CHAR_MAXPETHAVE; j++){
		char msgbuf[256];
		int petindex = CHAR_getCharPet( meindex, j);
		if( !CHAR_CHECKINDEX( petindex) ) continue;
		CHAR_complianceParameter( petindex );
		snprintf( msgbuf, sizeof( msgbuf ), "K%d", j );
		CHAR_sendStatusString( meindex, msgbuf );
		snprintf( msgbuf, sizeof( msgbuf ), "W%d", j );
		CHAR_sendStatusString( meindex, msgbuf );
	}

#ifdef _ITEM_PILEFORTRADE
	{
		int ti=0;
		ti = CONNECT_getTradeList( fd);
		if( ti <0 || ti >= MAX_TRADELISTNUM ) return;
		TRADE_ResetTradeList( ti);
	}
#endif

}

void TRADE_ShowItem(int fd, int meindex, char* message)
{
   char token[128], mycharaname[256], tocharaname[256], msg[2048];
   int tofd, showindex, toindex;// itemindex, gold, tmpgold, item;

   int MaxGold;

   MaxGold = CHAR_getMaxHaveGold( meindex);
   strcpy( mycharaname, CHAR_getChar(meindex, CHAR_NAME));
#ifdef _ITEM_PILEFORTRADE
	toindex = TRADE_getMyTarget( meindex);
	if( !CHAR_CHECKINDEX( toindex) ){
		print(" TRADE_Close_Err1:%d ", toindex);
		TRADE_Close(fd, meindex, message);
		return;
	}
	if( meindex != TRADE_getMyTarget( toindex) ){
		print(" TRADE_Close_Err2:%d->%d ", meindex, toindex);
		TRADE_Close(fd, meindex, message);
		return;
	}
	tofd = getfdFromCharaIndex(toindex);
#else
   if (getStringFromIndexWithDelim(message, "|", 2, token, sizeof(token)) == FALSE)	return;
   tofd = atoi(token);
   toindex = CONNECT_getCharaindex(tofd);
#endif

   if (!CHAR_CHECKINDEX(toindex))return;
   // 若玩家状态为组队或战斗中则不予处理
   if ((CHAR_getWorkInt(meindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE)
   	|| (CHAR_getWorkInt(meindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE))
   		return;

   if ((CHAR_getWorkInt(toindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE)
   	|| (CHAR_getWorkInt(toindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE))
   		return;

	if (getStringFromIndexWithDelim(message, "|", 3, tocharaname, sizeof(tocharaname)) == FALSE) return;
	if (getStringFromIndexWithDelim(message, "|", 5, token, sizeof(token)) == FALSE) return;
	showindex = atoi(token);
	if (getStringFromIndexWithDelim(message, "|", 4, token, sizeof(token)) == FALSE) return; 
	
#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
	showindex = toindex;
#endif
	strcpy( tocharaname, CHAR_getChar(toindex, CHAR_NAME));

	switch(tolower(token[0])){
	case 'i':
		{
			char buf1[2048];
			if( TRADE_HandleItem( meindex, showindex, message, buf1) == FALSE ){
				print(" TRADE_Close_Err3:%d,%d,%s,%s ", meindex, showindex, message, buf1);
				TRADE_Close(fd, meindex, message);
				return;
			}else{
				snprintf(msg, sizeof(msg), "T|%d|%s|I|%d|%s", fd, mycharaname, showindex, buf1 );
				lssproto_TD_send(tofd, msg);
			}
		}
		break;
   	case 'g':
		{
			char buf1[2048];
			if( TRADE_HandleGold( meindex, showindex, message, buf1) == FALSE ){
				print(" TRADE_Close_Err4:%d,%d,%s,%s ", meindex, showindex, message, buf1);
				TRADE_Close(fd, meindex, message);
				return;
			}else{// 将欲交易之金钱传给对方
				snprintf(msg, sizeof(msg), "T|%d|%s|G|%d|%s", fd, mycharaname, showindex, buf1);
				lssproto_TD_send( tofd, msg);
			}
		}
		break;
	case 'p':
		{
			char buf1[2048];
			if( TRADE_HandlePet( meindex, showindex, message, buf1) == FALSE ){
				print(" TRADE_Close_Err5:%d,%d,%s,%s ", meindex, showindex, message, buf1);
				TRADE_Close(fd, meindex, message);
				return;
			}else{
				snprintf(msg, sizeof(msg), "T|%d|%s|P|%d|%s",
					fd, mycharaname, showindex, buf1);
				lssproto_TD_send(tofd, msg);
			}
		}
		break;
   	case 'k':
   	{
	    if((CONNECT_get_confirm(fd)==TRUE) &&
						(CONNECT_get_confirm(tofd)==TRUE) ){//当双方都按下 lock
				if( CHAR_getWorkInt(meindex, CHAR_WORKTRADEMODE) == CHAR_TRADE_LOCK ){
					//andy_log
					print("ANDY err 防止第二次进入!!\n");
					return;//防止第二次进入
				}
				CHAR_setWorkInt(meindex, CHAR_WORKTRADEMODE, CHAR_TRADE_LOCK);
				//检查对方是否按下 ok
#ifdef _TRADESYSTEM2
			if( CHAR_getWorkInt( toindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_LOCK )return;
#endif
				snprintf(msg, sizeof(msg), "T|%d|%s|A", fd, mycharaname);
				lssproto_TD_send( tofd, msg);
				TRADE_SwapItem(meindex, toindex, message, fd, mycharaname, tofd, tocharaname);
			}
#ifndef _TRADESYSTEM2
			else{
				if( CONNECT_get_confirm( fd) == TRUE ) return;
				CONNECT_set_confirm(fd, TRUE);
				snprintf( msg, sizeof(msg), "T|%d|%s|C", fd, mycharaname);
				lssproto_TD_send( tofd, msg);
			}
#endif
	}
   	break;
	case 'c':
    if( CONNECT_get_confirm( fd) == TRUE ) return;
		CONNECT_set_confirm(fd, TRUE);
		snprintf( msg, sizeof(msg), "T|%d|%s|C", fd, mycharaname);
		lssproto_TD_send( tofd, msg);
	break;
   }
	return;
}

void TRADE_SwapItem( int meindex, int toindex, char* message, int fd, char* mycharaname, int tofd, char* tocharaname)
{
   int result;
   char msg[128], tmpmsg[128];

#ifndef _ITEM_PILEFORTRADE
   CONNECT_setTradeTmp(fd, message);
#endif

   result = TRADE_CheckItembuf(fd, meindex, toindex, tofd, mycharaname, tocharaname);
#ifndef _ITEM_PILEFORTRADE
   toindex = CONNECT_getCharaindex(tofd);
#endif

   if (!CHAR_CHECKINDEX(toindex))       return;
   if (result == -1)
   	strcpy(msg, TRADE_FAILED);
   else if (result == -2)
   	sprintf(msg, TRADE_ITEMFULL, mycharaname);
   else if (result == -3)
   	sprintf(msg, TRADE_ITEMFULL, tocharaname);
   else if (result == -4)
   	sprintf(msg, TRADE_RICH, mycharaname);
   else if (result == -5)
   	sprintf(msg, TRADE_POOR, mycharaname);
   else if (result == -6)
   	sprintf(msg, TRADE_RICH, tocharaname);
   else if (result == -7)
   	sprintf(msg, TRADE_POOR, tocharaname);
   else if (result == -8)
   	sprintf(msg, TRADE_TOORICH, mycharaname);
   else if (result == -9)
   	sprintf(msg, TRADE_TOORICH, tocharaname);
   else if (result == -10)
   	sprintf(msg, TRADE_PETFULL, mycharaname);
   else if (result == -11)
   	sprintf(msg, TRADE_PETFULL, tocharaname);
   else if (result == -12)
   	strcpy(msg, TRADE_VANISHITEM);
   else if (result == -13)
   	sprintf(msg, TRADE_POORLV, mycharaname);
   else if (result == -14)
   	sprintf(msg, TRADE_POORLV, tocharaname);
   else if (result == -15)
   	strcpy(msg, TRADE_FMPET);
#ifdef _LOCK_PET_ITEM
   else if (result == -16)
   	strcpy(msg, TRADE_LOCKPET);
   else if (result == -17)
   	strcpy(msg, TRADE_LOCKITEM);
   else if (result == -18)
   	strcpy(msg, TRADE_LOCKPETITEM);
#endif
   else if (result == -19)
   	strcpy(msg, TRADE_NOITEM);
   else if (result == -20)
   	strcpy(msg, TRADE_NOPET);

   else if (result == 1)
   	strcpy(msg, TRADE_SUCCESS);
   else if (result == 2)
   	strcpy(msg, TRADE_LOCK);
#ifdef _TRADESYSTEM2
   else if (result == 3)
   	strcpy(msg, "开始PK交易");
#endif

   if (result != 2){
   	snprintf(tmpmsg, sizeof(tmpmsg), "W|%d|%s", fd, mycharaname);
   	lssproto_TD_send(tofd, tmpmsg);
   	snprintf(tmpmsg, sizeof(tmpmsg), "W|%d|%s", tofd, tocharaname);
   	lssproto_TD_send(fd, tmpmsg);
   	
	CHAR_setWorkInt(meindex, CHAR_WORKTRADEMODE, CHAR_TRADE_FREE);
	CHAR_setWorkInt(toindex, CHAR_WORKTRADEMODE, CHAR_TRADE_FREE);
	CONNECT_setTradeTmp(fd, "");
	CONNECT_setTradeTmp(tofd, "");
	CHAR_sendTradeEffect( meindex, 0);
	CHAR_sendTradeEffect( toindex, 0);   	
   }
	CHAR_sendStatusString(meindex, "i");
	CHAR_sendStatusString(toindex, "i");
	CHAR_sendStatusString(meindex, "P");
	CHAR_sendStatusString(toindex, "P");
	{
		int j, petindex;
		char msgbuf[256];
		for(j = 0; j < CHAR_MAXPETHAVE; j++){
			petindex = CHAR_getCharPet( meindex, j);
			if( !CHAR_CHECKINDEX( petindex) ) continue;
			CHAR_complianceParameter( petindex );
			snprintf( msgbuf, sizeof( msgbuf ), "K%d", j );
			CHAR_sendStatusString( meindex, msgbuf );
			snprintf( msgbuf, sizeof( msgbuf ), "W%d", j );
			CHAR_sendStatusString( meindex, msgbuf );
		}
		for(j = 0; j < CHAR_MAXPETHAVE; j++){
			petindex = CHAR_getCharPet( toindex, j);
			if( !CHAR_CHECKINDEX( petindex) ) continue;
			CHAR_complianceParameter( petindex );
			snprintf( msgbuf, sizeof( msgbuf ), "K%d", j );
			CHAR_sendStatusString( toindex, msgbuf );
			snprintf( msgbuf, sizeof( msgbuf ), "W%d", j );
			CHAR_sendStatusString( toindex, msgbuf );
		}
	}

   CHAR_talkToCli(meindex, -1, msg, CHAR_COLORYELLOW);
   CHAR_talkToCli(toindex, -1, msg, CHAR_COLORYELLOW);   
   
#ifdef _SAMETHING_SAVEPOINT
	if (result == 1){
			if(CHAR_charSaveFromConnect(meindex, FALSE)){
				CHAR_talkToCli(meindex, -1, "系统自动为您存档!", CHAR_COLORRED);
			}

			if(CHAR_charSaveFromConnect(toindex, FALSE)){
				CHAR_talkToCli(toindex, -1, "系统自动为您存档!", CHAR_COLORRED);
			}
  }
#endif
}

#ifdef _ITEM_PILEFORTRADE

int TRADE_HandleTrade_DelItem( int charaindex, int MyMaxPile, int MaxPile, int *Item, int *nums, int *indexlist)
{
	int i, k;
	
	for( i=0; i<50; i++){
		indexlist[i] = -1;
	}
	k=0;
	for( i=0; i<15; i++ ){
		int pilenum, itemindex, totalnums, newindex;
		if( Item[i] == -1 ) continue;
		itemindex = CHAR_getItemIndex( charaindex, Item[i] );
		if( !ITEM_CHECKINDEX( itemindex)) return -1;
		pilenum = ITEM_getInt( itemindex, ITEM_USEPILENUMS);
		pilenum -= nums[i];
		if( pilenum < 0 ) return -1;
		if( pilenum == 0 ){
			CHAR_setItemIndex( charaindex, Item[i], -1);
			CHAR_sendItemDataOne( charaindex, Item[i]);
			indexlist[k] = itemindex;
			k++;
			totalnums = ITEM_getInt( itemindex, ITEM_USEPILENUMS);
			newindex = itemindex;
		}else{
			ITEM_setInt( itemindex, ITEM_USEPILENUMS, pilenum);
			newindex = ITEM_makeItemAndRegist( ITEM_getInt( itemindex, ITEM_ID));
			if( !ITEM_CHECKINDEX( newindex) ) return -1;
			indexlist[k] = newindex;
			k++;
			totalnums = nums[i];
		}
		if( totalnums > MaxPile ){
			totalnums -= MaxPile;
			ITEM_setInt( newindex, ITEM_USEPILENUMS, MaxPile);
		}else{
			ITEM_setInt( newindex, ITEM_USEPILENUMS, totalnums);
			totalnums = 0;
		}

		while( totalnums > 0 ){
			newindex = ITEM_makeItemAndRegist( ITEM_getInt( itemindex, ITEM_ID));
			if( !ITEM_CHECKINDEX( newindex) ) return -1;
			indexlist[k] = newindex;
			k++;
			if( totalnums > MaxPile ){
				totalnums -= MaxPile;
				ITEM_setInt( newindex, ITEM_USEPILENUMS, MaxPile);
			}else{
				ITEM_setInt( newindex, ITEM_USEPILENUMS, totalnums);
				totalnums = 0;
			}
		}
	}
	return k;
}

int TRADE_HandleTrade_DelPet( int charaindex, int *Pet, int *indexlist)
{
	int i, k, petindex;
	k=0;
	char category[8];
	
	for( i=0; i<5; i++){
		indexlist[i] = -1;
	}
	for( i=0; i<5; i++ ){
		if( Pet[i] == -1 ) continue;
		petindex = CHAR_getCharPet( charaindex, Pet[i] );
		if( !CHAR_CHECKINDEX( petindex)) return -1;
		CHAR_setCharPet( charaindex, Pet[i], -1);
		
		snprintf( category, sizeof( category ), "K%d", Pet[i] );
		CHAR_sendStatusString( charaindex, category );
		snprintf( category, sizeof( category ), "W%d", Pet[i] );
		CHAR_sendStatusString( charaindex, category );
		
		indexlist[k] = petindex;
		k++;
	}
	return k;
}

BOOL TRADE_HandleTrade_DelGold( int charaindex, int sGold, int *Gold)
{
	int MyGold, MyMaxGold;
	
	MyGold = CHAR_getInt( charaindex, CHAR_GOLD);
	MyMaxGold = CHAR_getMaxHaveGold( charaindex);
	
	MyGold -= sGold;
	if( MyGold < 0 || MyGold > MyMaxGold ){
		MyGold = (MyGold<0)?0:MyGold;
		MyGold = (MyGold>MyMaxGold)?MyMaxGold:MyGold;
		CHAR_setInt( charaindex, CHAR_GOLD, MyGold);
		return FALSE;
	}
	CHAR_setInt( charaindex, CHAR_GOLD, MyGold);
	*Gold = sGold;
	
	CHAR_complianceParameter( charaindex );
	CHAR_send_P_StatusString( charaindex , CHAR_P_STRING_GOLD);
	return TRUE;
}

BOOL TRADE_HandleTrade_AddItem( int charaindex, int *Item)
{
	int ret, i;
#ifdef _NEW_ITEM_
	int itemMax =  CheckCharMaxItem(charaindex);
#endif
	for( i=0; i<50; i++){
		if( Item[i] == -1 )break;
		ret = CHAR_addItemSpecificItemIndex( charaindex, Item[i]);
#ifdef _NEW_ITEM_
		if( ret < 0 || ret >= itemMax ) {
#else
			if( ret < 0 || ret >= CHAR_MAXITEMHAVE ) {
#endif
			int j;
			for( j=i; j< 50; j++)
				ITEM_endExistItemsOne( Item[j]);
			return FALSE;
		}
#ifdef _FIRST_LOCK_ITEM
		else{
			FreeFirstLockItem(charaindex, Item[i]);
/*
			int i;
			for(i = 0; i < 32; i ++){
				if(ITEM_getInt(Item[i], ITEM_ID) == getFirstLockItem( i )){
					char *arg = ITEM_getChar(Item[i], ITEM_NAME);
					if(arg[0] != '*'){
						char newname[256];
						sprintf(newname, "*%s", ITEM_getChar(Item[i], ITEM_NAME));
						ITEM_setChar(Item[i], ITEM_NAME, newname);
						ITEM_setChar(Item[i], ITEM_SECRETNAME, newname);
						break;
					}
				}
			}
*/
		}
#endif
	}
	return TRUE;
}

BOOL TRADE_HandleTrade_AddPet( int charaindex, int *Pet)
{
	int havepetelement, i;
	char category[8];
	for( i=0; i<5; i++){
#ifdef _PETFOLLOW_NEW_
		havepetelement = CHAR_getCharPetElementOld( charaindex);
#else
		havepetelement = CHAR_getCharPetElement( charaindex);
#endif
		if( Pet[i] == -1 ) break;
		if( havepetelement < 0 ){
			int j;
			for( j=i; j<5; j++){
				CHAR_endCharOneArray( Pet[j] );
			}
			return FALSE;
		}
		
#ifdef _PET_ITEM
		CHAR_sendPetItemData(charaindex,havepetelement);
#endif

#ifdef _FIRST_LOCK_ITEM
		FreeFirstLockPet(charaindex, Pet[i]);
#endif

		CHAR_setCharPet( charaindex, havepetelement, Pet[i]);
		CHAR_setWorkInt( Pet[i], CHAR_WORKPLAYERINDEX, charaindex);
		CHAR_setChar( Pet[i], CHAR_OWNERCDKEY, CHAR_getChar( charaindex, CHAR_CDKEY));
		CHAR_setChar( Pet[i], CHAR_OWNERCHARANAME, CHAR_getChar( charaindex, CHAR_NAME));
		CHAR_complianceParameter( Pet[i]);
		
		snprintf( category, sizeof( category ), "K%d", havepetelement );
		CHAR_sendStatusString( charaindex, category );
		snprintf( category, sizeof( category ), "W%d", havepetelement );
		CHAR_sendStatusString( charaindex, category );
	}
	return TRUE;
}

BOOL TRADE_HandleTrade_AddGold( int charaindex, int sGold)
{
	int MyGold, MyMaxGold;
	MyGold = CHAR_getInt( charaindex, CHAR_GOLD);
	MyMaxGold = CHAR_getMaxHaveGold( charaindex);
	
	MyGold += sGold;
	
	MyGold = (MyGold<0)?0:MyGold;
	MyGold = (MyGold>MyMaxGold)?MyMaxGold:MyGold;
	
	CHAR_setInt( charaindex, CHAR_GOLD, MyGold);

	CHAR_complianceParameter( charaindex );
	CHAR_send_P_StatusString( charaindex , CHAR_P_STRING_GOLD);
	return TRUE;
}

BOOL TRADE_HandleTrade( int meindex, STradeList *temp1, int toindex, STradeList *temp2 )
{
	int MeMaxPile, ToMaxPile, ret;
	int MeItem[50], ToItem[50];
	int MePet[5], ToPet[5];
	int MeGold=0, ToGold=0;
	
	MeMaxPile = CHAR_getMyMaxPilenum( meindex);//最大堆叠数
	ToMaxPile = CHAR_getMyMaxPilenum( toindex);
	//移除meindex
	//移除toindex
	{
		char buf[256];

		memset( buf, 0, sizeof( buf));
		sprintf( buf, "A.%s[%s] trade B.%s[%s] FXY[%d,%d,%d]",
			CHAR_getChar( meindex, CHAR_NAME), CHAR_getChar(meindex, CHAR_CDKEY),
			CHAR_getChar( toindex, CHAR_NAME), CHAR_getChar(toindex, CHAR_CDKEY),
			CHAR_getInt( meindex, CHAR_FLOOR), CHAR_getInt( meindex, CHAR_X), CHAR_getInt( meindex, CHAR_Y) );
		LogTrade(buf);
	}

	//移除
	if( (ret=TRADE_HandleTrade_DelItem( meindex, MeMaxPile, ToMaxPile, temp1->ItemTi, temp1->ItemNum, ToItem ))== -1 ) return FALSE;
	if( (ret=TRADE_HandleTrade_DelItem( toindex, ToMaxPile, MeMaxPile, temp2->ItemTi, temp2->ItemNum, MeItem ))== -1 ) return FALSE;

	if( (ret=TRADE_HandleTrade_DelPet( meindex, temp1->PetTi, ToPet)) == -1 ) return FALSE;
	if( (ret=TRADE_HandleTrade_DelPet( toindex, temp2->PetTi, MePet)) == -1 ) return FALSE;
	if( TRADE_HandleTrade_DelGold( meindex, temp1->Golds, &ToGold) == FALSE ) return FALSE;
	if( TRADE_HandleTrade_DelGold( toindex, temp2->Golds, &MeGold) == FALSE ) return FALSE;
	//加入
	if( TRADE_HandleTrade_AddItem( meindex, MeItem) == FALSE ) return FALSE;
	if( TRADE_HandleTrade_AddItem( toindex, ToItem) == FALSE ) return FALSE;
	if( TRADE_HandleTrade_AddPet( meindex, MePet) == FALSE) return FALSE;
	if( TRADE_HandleTrade_AddPet( toindex, ToPet) == FALSE) return FALSE;

	TRADE_HandleTrade_AddGold( meindex, MeGold);
	TRADE_HandleTrade_AddGold( toindex, ToGold);

	{
		char buf1[4096], buf2[4096], buf[256];
		int i;
					
		sprintf( buf1, "A.ITEM.%s:%s"
			, CHAR_getChar( meindex, CHAR_NAME), CHAR_getChar(meindex, CHAR_CDKEY)
			, CHAR_getChar( toindex, CHAR_NAME), CHAR_getChar(toindex, CHAR_CDKEY));
		sprintf( buf2, "B.ITEM.%s:%s"
			, CHAR_getChar( meindex, CHAR_NAME), CHAR_getChar(meindex, CHAR_CDKEY)
			, CHAR_getChar( toindex, CHAR_NAME), CHAR_getChar(toindex, CHAR_CDKEY));
		for( i=0; i<50; i++){
			if( ITEM_CHECKINDEX( ToItem[i]) ){
				sprintf( buf, "%s[%s]*%d,",
					ITEM_getChar( ToItem[i], ITEM_NAME), ITEM_getChar( ToItem[i], ITEM_UNIQUECODE),
					ITEM_getInt( ToItem[i], ITEM_USEPILENUMS) );
				strcat( buf1, buf);
			}
			if( ITEM_CHECKINDEX( MeItem[i]) ){
				sprintf( buf, "%s[%s]*%d,",
					ITEM_getChar( MeItem[i], ITEM_NAME), ITEM_getChar( MeItem[i], ITEM_UNIQUECODE),
					ITEM_getInt( MeItem[i], ITEM_USEPILENUMS) );
				strcat( buf2, buf);
			}
		}
		strcat( buf1, "PET.");
		strcat( buf2, "PET.");
		for( i=0; i<5; i++){
			if( CHAR_CHECKINDEX( ToPet[i]) ){
				sprintf( buf, "%s[%s]%s-%d",
					CHAR_getUseName( ToPet[i]), CHAR_getChar( ToPet[i], CHAR_UNIQUECODE),
					CHAR_getChar( ToPet[i], CHAR_NAME), CHAR_getInt( ToPet[i], CHAR_LV) );
				strcat( buf1, buf);
			}
			if( CHAR_CHECKINDEX( MePet[i]) ){
				sprintf( buf, "%s[%s]%s-%d",
					CHAR_getUseName( MePet[i]), CHAR_getChar( MePet[i], CHAR_UNIQUECODE),
					CHAR_getChar( MePet[i], CHAR_NAME), CHAR_getInt( MePet[i], CHAR_LV) );
				strcat( buf2, buf);
			}
		}
		

		sprintf( buf, "GOLD:%s:%s:%d"
		, CHAR_getChar( meindex, CHAR_NAME), CHAR_getChar(meindex, CHAR_CDKEY)
		, ToGold);
		strcat( buf1, buf);
		sprintf( buf, "GOLD:%s:%s:%d"
		,CHAR_getChar( toindex, CHAR_NAME), CHAR_getChar(toindex, CHAR_CDKEY)
		, MeGold);
		strcat( buf2, buf);
		LogTrade( buf1);
		LogTrade( buf2);
	}

	return TRUE;
}

#ifdef _TRADE_PK
BOOL TRADE_HandleTradeForPK( STradeList *temp1, STradeList *temp2 )
{
	int MeMaxPile, ToMaxPile, ret;
	int MeItem[50];
	int MePet[5];
	int MeGold=0;
	int meindex = temp1->charaindex;
	int toindex = temp2->charaindex;
	
	MeMaxPile = CHAR_getMyMaxPilenum( meindex);//最大堆叠数
	ToMaxPile = CHAR_getMyMaxPilenum( toindex);
	//移除
		
	if( (ret=TRADE_HandleTrade_DelItem( toindex, ToMaxPile, MeMaxPile, temp2->ItemTi, temp2->ItemNum, MeItem ))== -1 ) return FALSE;
	if( (ret=TRADE_HandleTrade_DelPet( toindex, temp2->PetTi, MePet)) == -1 ) return FALSE;
	if( TRADE_HandleTrade_DelGold( toindex, temp2->Golds, &MeGold) == FALSE ) return FALSE;
		
	//加入
	if( TRADE_HandleTrade_AddItem( meindex, MeItem) == FALSE ) return FALSE;
	if( TRADE_HandleTrade_AddPet( meindex, MePet) == FALSE) return FALSE;
	TRADE_HandleTrade_AddGold( meindex, MeGold);

	CHAR_talkToCli( meindex, -1, "恭喜您！您赢得对方物品！",  CHAR_COLORWHITE);
	CHAR_talkToCli( toindex, -1, "很遗憾！对方赢得您的物品！",  CHAR_COLORWHITE);

	return TRUE;
}
#endif

int TRADE_CheckItembuf(int fd, int meindex, int toindex, int tofd, char* mycharaname, char* tocharaname)
{
	int Tradeti;
	STradeList TradeListTemp[2];
	if( TRADE_getMyTarget( meindex) != toindex ) return -1;
	Tradeti = CONNECT_getTradeList( fd);
	if( Tradeti <0 || Tradeti >= MAX_TRADELISTNUM ) return -1;
	if( TradeList[Tradeti][0].use != 1 || TradeList[Tradeti][1].use != 1 ) return -1;
	if( TRADE_getSomeTradeList( meindex, &TradeListTemp[0]) == FALSE ) return -1;
	if( TRADE_getSomeTradeList( toindex, &TradeListTemp[1]) == FALSE ) return -1;
	//确定空位
	if( TRADE_CheckTradeList( meindex, &TradeListTemp[0], toindex, &TradeListTemp[1]) == FALSE ) return -1;
	//移除人物身上
	//加入人物身上
	
#ifdef _TRADE_PK
	if(CHAR_getInt(meindex, CHAR_FLOOR)==50000 && CHAR_getInt(toindex, CHAR_FLOOR)==50000 ){
		BATTLE_CreateVsPlayerForTrade(TradeListTemp[0], TradeListTemp[1]);
		//PK交易结束动作
		return 3;
	}
#endif
	if( TRADE_HandleTrade( meindex, &TradeListTemp[0], toindex, &TradeListTemp[1] ) == FALSE )	return -1;
	//交易结束动作
	return 1;
}

#else	
int TRADE_CheckItembuf(int fd, int meindex, int toindex, int tofd, char* mycharaname, char* tocharaname)
{
   char a[16], b[16], c[16], d[16], e[16], f[16];
   char g[16], h[16], i[16], j[16], k[16], l[16], token[16];
   int itemindex1 = 0, itemindex2 = 0, itemindex3 = 0;
   int itemindex4 = 0, itemindex5 = 0, itemindex6 = 0;
   int toitemindex1 = 0, toitemindex2 = 0, toitemindex3 = 0;
   int toitemindex4 = 0, toitemindex5 = 0, toitemindex6 = 0;
   int item1 = 0, item2 = 0, item4 = 0, item5 = 0, pet3 = 0, pet6 = 0;
   int pet1 = 0, pet2 = 0;
   int swapitem1 = 0, swapitem2 = 0, gold1 = 0, gold2 = 0;
   char itembuf[256], toitembuf[256], buf[4048];
   
   CONNECT_getTradeTmp(fd, itembuf, sizeof(itembuf));
   CONNECT_getTradeTmp(tofd, toitembuf, sizeof(toitembuf));

   // 比对双方之最後交易协定
   if (getStringFromIndexWithDelim(itembuf, "|", 5, token, sizeof(token)) == TRUE)
	   	strcpy(a, token);
   if (getStringFromIndexWithDelim(itembuf, "|", 6, token, sizeof(token)) == TRUE)
		itemindex1 = atoi(token);
   if (getStringFromIndexWithDelim(itembuf, "|", 7, token, sizeof(token)) == TRUE)
	   	strcpy(b, token);
   if (getStringFromIndexWithDelim(itembuf, "|", 8, token, sizeof(token)) == TRUE)
		itemindex2 = atoi(token);
   if (getStringFromIndexWithDelim(itembuf, "|", 9, token, sizeof(token)) == TRUE)
	   	strcpy(c, token);
   if (getStringFromIndexWithDelim(itembuf, "|", 10, token, sizeof(token)) == TRUE)
		itemindex3 = atoi(token);
   if (getStringFromIndexWithDelim(itembuf, "|", 11, token, sizeof(token)) == TRUE)
	   	strcpy(d, token);
   if (getStringFromIndexWithDelim(itembuf, "|", 12, token, sizeof(token)) == TRUE)
		itemindex4 = atoi(token);
   if (getStringFromIndexWithDelim(itembuf, "|", 13, token, sizeof(token)) == TRUE)
	   	strcpy(e, token);
   if (getStringFromIndexWithDelim(itembuf, "|", 14, token, sizeof(token)) == TRUE)
		itemindex5 = atoi(token);
   if (getStringFromIndexWithDelim(itembuf, "|", 15, token, sizeof(token)) == TRUE)
		strcpy(f, token);
   if (getStringFromIndexWithDelim(itembuf, "|", 16, token, sizeof(token)) == TRUE)
		itemindex6 = atoi(token);

   if (getStringFromIndexWithDelim(toitembuf, "|", 5, token, sizeof(token)) == TRUE)
	   	strcpy(g, token);
   if (getStringFromIndexWithDelim(toitembuf, "|", 6, token, sizeof(token)) == TRUE)
		toitemindex1 = atoi(token);
   if (getStringFromIndexWithDelim(toitembuf, "|", 7, token, sizeof(token)) == TRUE)
	   	strcpy(h, token);
   if (getStringFromIndexWithDelim(toitembuf, "|", 8, token, sizeof(token)) == TRUE)
		toitemindex2 = atoi(token);
   if (getStringFromIndexWithDelim(toitembuf, "|", 9, token, sizeof(token)) == TRUE)
	   	strcpy(i, token);
   if (getStringFromIndexWithDelim(toitembuf, "|", 10, token, sizeof(token)) == TRUE)
		toitemindex3 = atoi(token);
   if (getStringFromIndexWithDelim(toitembuf, "|", 11, token, sizeof(token)) == TRUE)
	   	strcpy(j, token);
   if (getStringFromIndexWithDelim(toitembuf, "|", 12, token, sizeof(token)) == TRUE)
		toitemindex4 = atoi(token);
   if (getStringFromIndexWithDelim(toitembuf, "|", 13, token, sizeof(token)) == TRUE)
	   	strcpy(k, token);
   if (getStringFromIndexWithDelim(toitembuf, "|", 14, token, sizeof(token)) == TRUE)
		toitemindex5 = atoi(token);
   if (getStringFromIndexWithDelim(toitembuf, "|", 15, token, sizeof(token)) == TRUE)
	   	strcpy(l, token);
   if (getStringFromIndexWithDelim(toitembuf, "|", 16, token, sizeof(token)) == TRUE)
		toitemindex6 = atoi(token);

   // shan hjj add 洗道具
   if (itemindex1==itemindex2){
        if (itemindex1!=-1){
            return -1;
        }
   }
   if (itemindex4==itemindex5){
        if (itemindex4!=-1){  
            return -1;
        }
   }
   // End

   if ((strcmp(a, j) == 0) && (strcmp(b, k) == 0) && (strcmp(c,l) == 0)
   	&& (strcmp(d, g) == 0) && (strcmp(e, h) == 0) && (strcmp(f, i) == 0)
   	&& (itemindex1 == toitemindex4) && (itemindex2 == toitemindex5)
   	&& (itemindex3 == toitemindex6) && (itemindex4 == toitemindex1)
   	&& (itemindex5 == toitemindex2) && (itemindex6 == toitemindex3))
   {
   	// 检验双方物品栏是否有空位
	if ((strcmp(a, "I") == 0) && (itemindex1 != -1)){
		swapitem1--;
		swapitem2++;
	}
	if ((strcmp(b, "I") == 0) && (itemindex2 != -1)){
		swapitem1--;
		swapitem2++;
	}
	if ((strcmp(d, "I") == 0) && (itemindex4 != -1)){
		swapitem1++;
		swapitem2--;
	}
	if ((strcmp(e, "I") == 0) && (itemindex5 != -1)){
		swapitem1++;
		swapitem2--;
	}
	if (swapitem1 > CHAR_findTotalEmptyItem(meindex))
		return	-2;
	toindex = CONNECT_getCharaindex(tofd);
	if (!CHAR_CHECKINDEX(toindex))       return -1;
	if (swapitem2 > CHAR_findTotalEmptyItem(toindex))
		return	-3;
	// 检查双方金钱
	if ((strcmp(a, "G") == 0) && (itemindex1 != -1))	gold1 += itemindex1;
	if ((strcmp(b, "G") == 0) && (itemindex2 != -1))	gold1 += itemindex2;
	if ((strcmp(d, "G") == 0) && (itemindex4 != -1))	gold2 += itemindex4;
	if ((strcmp(e, "G") == 0) && (itemindex5 != -1))	gold2 += itemindex5;
	if (gold1 > CHAR_getMaxHaveGold( meindex) )	return	-4;
	if (gold1 > CHAR_getInt(meindex, CHAR_GOLD))	return	-5;
	if (gold2 > CHAR_getMaxHaveGold( toindex))	return	-6;
	if (gold2 > CHAR_getInt(toindex, CHAR_GOLD))	return	-7;
	if ((gold2 + CHAR_getInt(meindex, CHAR_GOLD) - gold1) > CHAR_getMaxHaveGold( meindex) )	return	-8;
	if ((gold1 + CHAR_getInt(toindex, CHAR_GOLD) - gold2) > CHAR_getMaxHaveGold( toindex))	return	-9;
	// 检查双方宠物
	if ((strcmp(c, "P") == 0) && (itemindex3 != -1))
	{
		pet1--;	pet2++;
	}
	if ((strcmp(f, "P") == 0) && (itemindex6 != -1))
	{
		pet1++;	pet2--;
	}
	if ((pet1 > 1) || (pet2 > 1))	return	-10;
	if ((pet1 == 1) && (CHAR_getCharPetElement( meindex) == -1))	return -10;
	if ((pet2 == 1) && (CHAR_getCharPetElement( toindex) == -1))	return -11;
	if ((strcmp(a, "I") == 0) && (itemindex1 != -1))
		item1 = CHAR_getItemIndex(meindex, itemindex1);
	else	item1 = -1;
	if ((strcmp(b, "I") == 0) && (itemindex2 != -1))
		item2 = CHAR_getItemIndex(meindex, itemindex2);
	else	item2 = -1;
	if ((strcmp(d, "I") == 0) && (itemindex4 != -1))
		item4 = CHAR_getItemIndex(toindex, itemindex4);
	else	item4 = -1;
	if ((strcmp(e, "I") == 0) && (itemindex5 != -1))
		item5 = CHAR_getItemIndex(toindex, itemindex5);
	else	item5 = -1;
	if (itemindex3 != -1)
	{
		if( CHAR_getInt( meindex, CHAR_RIDEPET) == itemindex3 )
			return -1;
		pet3 = CHAR_getCharPet(meindex, itemindex3);
	}
	else 	pet3 = -1;
	if (itemindex6 != -1)
	{
		if( CHAR_getInt( toindex, CHAR_RIDEPET) == itemindex6 )
			return -1;
		pet6 = CHAR_getCharPet(toindex, itemindex6);
	}
	else	pet6 = -1;
	if ((ITEM_getInt(item1, ITEM_VANISHATDROP) == 1)||
		(ITEM_getInt(item2, ITEM_VANISHATDROP) == 1)||
		(ITEM_getInt(item4, ITEM_VANISHATDROP) == 1)||
		(ITEM_getInt(item5, ITEM_VANISHATDROP) ==1))
			return	-12;
#ifdef _LOCK_PET_ITEM
	char *arg;
	arg = ITEM_getChar(item1, ITEM_NAME);
	if(arg[0] == '*'){
		return	-17;
	}
	arg = ITEM_getChar(item2, ITEM_NAME);
	if(arg[0] == '*'){
		return	-17;
	}
	arg = ITEM_getChar(item4, ITEM_NAME);
	if(arg[0] == '*'){
		return	-17;
	}
	arg = ITEM_getChar(item5, ITEM_NAME);
	if(arg[0] == '*'){
		return	-17;
	}
#endif

	if(FreeTradeItem(meindex, item1) == 1
		|| FreeTradeItem(meindex, item2) == 1 
		|| FreeTradeItem(meindex, item3) == 1 
		|| FreeTradeItem(meindex, item4) == 1 
		|| FreeTradeItem(meindex, item5) == 1  ){
		return -19;
	}
	if( CHAR_getWorkInt( meindex, CHAR_PickAllPet) != TRUE )	{
		if ((pet6 != -1) && (CHAR_getInt(pet6, CHAR_LV)
			 - CHAR_getInt(meindex, CHAR_LV) > 20)
			 && (CHAR_getInt(meindex,CHAR_TRANSMIGRATION) <= 0))
		 		return -13;

	}
	if( CHAR_getWorkInt( toindex, CHAR_PickAllPet) != TRUE )	{

		if ((pet3 != -1) && (CHAR_getInt(pet3, CHAR_LV)
			 - CHAR_getInt(toindex, CHAR_LV) > 5)
			 && (CHAR_getInt(toindex,CHAR_TRANSMIGRATION) <= 0))
		 		return -14;
	}
	if ((pet6 != -1) && (CHAR_getInt(pet6, CHAR_PETFAMILY) == 1))
		 	return -15;
	if ((pet3 != -1) && (CHAR_getInt(pet3, CHAR_PETFAMILY) == 1))
		 	return -15;
#ifdef _LOCK_PET_ITEM
{
	if (pet6 != -1) {
		char *petname = CHAR_getChar( pet6, CHAR_NAME);
		if( petname[0] == '*' ){
			return -16;
		}
#ifdef _PET_ITEM
{
		int i;		
		for( i = 0 ; i < CHAR_MAXPETITEMHAVE ; i ++ ){
			int itemindex = CHAR_getItemIndex( pet6, i );
			char *arg = ITEM_getChar(itemindex, ITEM_NAME);
			if(arg[0] == '*'){
				return -18;
			}
		}
}
#endif
	}
	if (pet3 != -1) {
		char *petname = CHAR_getChar( pet3, CHAR_NAME);
		if( petname[0] == '*' ){
			return -16;
		}
#ifdef _PET_ITEM
{
		int i;		
		for( i = 0 ; i < CHAR_MAXPETITEMHAVE ; i ++ ){
			int itemindex = CHAR_getItemIndex( pet3, i );
			char *arg = ITEM_getChar(itemindex, ITEM_NAME);
			if(arg[0] == '*'){
				return -18;
			}
		}
}
#endif
	}
}
#endif
	if (pet6 != -1) {
		if(FreeTradePet(meindex, pet6) == 1){
			return -20;
		}
	}
	if (pet3 != -1) {
		if(FreeTradePet(meindex, pet3) == 1){
			return -20;
		}
	}

	TRADE_ChangeItem(meindex, toindex, a, d, item1, item4,
		itemindex1, itemindex4);
	TRADE_ChangeItem(meindex, toindex, b, e, item2, item5,
		itemindex2, itemindex5);
	TRADE_ChangeItem(meindex, toindex, c, f, pet3, pet6,
		itemindex3, itemindex6);

	// 写入 trade.log
	{
		char petname1[256], petname2[256];
		int pet1lv, pet2lv;
		int logitem1 = 0, logitem2 = 0, logitem4 = 0, logitem5 = 0;

		if ((strcmp(a, "I") == 0) && (item1 != -1))	logitem1 = ITEM_getInt(item1, ITEM_ID);
		if ((strcmp(b, "I") == 0) && (item2 != -1))	logitem2 = ITEM_getInt(item2, ITEM_ID);
		if ((strcmp(a, "G") == 0) && (item1 == -1))	logitem1 = itemindex1;
		if ((strcmp(b, "G") == 0) && (item2 == -1))	logitem2 = itemindex2;
		if ((strcmp(c, "P") == 0) && (pet3 == -1))
		{
			sprintf(petname1, "NONE");
			pet1lv = 0;
		}
		else
		{
			sprintf(petname1, "%s", CHAR_getChar(pet3, CHAR_NAME));
			pet1lv = CHAR_getInt(pet3, CHAR_LV);
		}
		if ((strcmp(f, "P") == 0) && (pet6 == -1))
		{
			sprintf(petname2, "NONE");
			pet2lv = 0;
		}
		else
		{
			sprintf(petname2, "%s", CHAR_getChar(pet6, CHAR_NAME));
			pet2lv = CHAR_getInt(pet6, CHAR_LV);
		}
		if ((strcmp(d, "I") == 0) && (item4 != -1))	logitem4 = ITEM_getInt(item4, ITEM_ID);
		if ((strcmp(e, "I") == 0) && (item5 != -1))	logitem5 = ITEM_getInt(item5, ITEM_ID);
		if ((strcmp(d, "G") == 0) && (item4 == -1))	logitem4 = itemindex4;
		if ((strcmp(e, "G") == 0) && (item5 == -1))	logitem5 = itemindex5;
	        sprintf(buf, "%s\t%s\t(%s[%d,%s],%s[%d,%s],%s[%s,%d,%s]) <-> %s\t%s\t(%s[%d,%s],%s[%d,%s],%s[%s,%d,%s])",
                        CHAR_getChar(meindex, CHAR_CDKEY), mycharaname,
                        a, logitem1, ITEM_getChar( item1, ITEM_UNIQUECODE), b, logitem2, ITEM_getChar( item2, ITEM_UNIQUECODE),
						c, petname1, pet1lv, CHAR_getChar( pet3, CHAR_UNIQUECODE),
                        CHAR_getChar(toindex, CHAR_CDKEY), tocharaname,
                        d, logitem4, ITEM_getChar( item4, ITEM_UNIQUECODE), e, logitem5, ITEM_getChar( item5, ITEM_UNIQUECODE), 
						f, petname2, pet2lv, CHAR_getChar( pet6, CHAR_UNIQUECODE));
		LogTrade(buf);
	}
	
        return 1;
   }
	return 2;
}

#endif
BOOL TRADE_ChangeItem(int meindex, int toindex, char *a, char *b,
	int item1, int item2, int itemindex1, int itemindex2)
{
   int gold1 = 0, gold2 = 0, eptitem = -1;
   if (CHAR_CHECKINDEX(meindex) == FALSE)	return FALSE;
   if (CHAR_CHECKINDEX(toindex) == FALSE)	return FALSE;
   // 道具 <-> 道具
   if ((strcmp(a, "I") == 0) && (strcmp(b, "I") == 0))
   {
   	// 无 <-> 道具
   	if ((itemindex1 == -1) && (itemindex2 > 0))
   	{
   	   if (ITEM_CHECKINDEX(item2) == FALSE)	return FALSE;
   	   eptitem = CHAR_findEmptyItemBox(meindex);
   	   if (eptitem < 0)	return FALSE;
   	   CHAR_setItemIndex(meindex, eptitem, item2);
   	   ITEM_setWorkInt(item2, ITEM_WORKCHARAINDEX, meindex);
   	   ITEM_setWorkInt(item2, ITEM_WORKOBJINDEX, -1);
   	   CHAR_setItemIndex(toindex, itemindex2, -1);
   	}
   	// 道具 <-> 无
   	else if ((itemindex1 > 0) && (itemindex2 == -1))
   	{
   	   if (ITEM_CHECKINDEX(item1) == FALSE)	return FALSE;
   	   eptitem = CHAR_findEmptyItemBox(toindex);
   	   if (eptitem < 0)	return FALSE;
   	   CHAR_setItemIndex(toindex, eptitem, item1);
   	   ITEM_setWorkInt(item1, ITEM_WORKCHARAINDEX, toindex);
   	   ITEM_setWorkInt(item1, ITEM_WORKOBJINDEX, -1);
   	   CHAR_setItemIndex(meindex, itemindex1, -1);
   	}
   	// 道具 <-> 道具
   	else if (itemindex1 > 0 && itemindex2 > 0)
   	{
   	   if (ITEM_CHECKINDEX(item1) == FALSE)	return FALSE;
   	   if (ITEM_CHECKINDEX(item2) == FALSE)	return FALSE;
   	   ITEM_setWorkInt(item1, ITEM_WORKCHARAINDEX, toindex);
   	   ITEM_setWorkInt(item1, ITEM_WORKOBJINDEX, -1);
   	   ITEM_setWorkInt(item2, ITEM_WORKCHARAINDEX, meindex);
   	   ITEM_setWorkInt(item2, ITEM_WORKOBJINDEX, -1);
   	   CHAR_setItemIndex(toindex, itemindex2, item1);
   	   CHAR_setItemIndex(meindex, itemindex1, item2);
   	}
   }
   // 道具 <-> 金钱
   else if ((strcmp(a, "I") == 0) && (strcmp(b, "G") == 0))
   {
   	if (itemindex1 > 0)
   	{
      	   if (ITEM_CHECKINDEX(item1) == FALSE)	return FALSE;
      	   eptitem = CHAR_findEmptyItemBox(toindex);
      	   if (eptitem < 0)	return FALSE;
      	   ITEM_setWorkInt(item1, ITEM_WORKCHARAINDEX, toindex);
      	   ITEM_setWorkInt(item1, ITEM_WORKOBJINDEX, -1);
      	   CHAR_setItemIndex(toindex, eptitem, item1);
      	   CHAR_setItemIndex(meindex, itemindex1, -1);
   	}
	if (itemindex2 < 0)	itemindex2 = 0;
   	gold1 = CHAR_getInt(meindex, CHAR_GOLD);
   	gold1 += itemindex2;
   	CHAR_setInt(meindex, CHAR_GOLD, gold1);
   	gold2 = CHAR_getInt(toindex, CHAR_GOLD);
   	gold2 -= itemindex2;
   	CHAR_setInt(toindex, CHAR_GOLD, gold2);
   }
   // 金钱 <-> 道具
   else if ((strcmp(a, "G") == 0) && (strcmp(b, "I") == 0))
   {
   	if (itemindex2 > 0)
   	{
      	   if (ITEM_CHECKINDEX(item2) == FALSE)	return FALSE;
      	   eptitem = CHAR_findEmptyItemBox(meindex);
      	   if (eptitem < 0)	return FALSE;
      	   ITEM_setWorkInt(item2, ITEM_WORKCHARAINDEX, meindex);
      	   ITEM_setWorkInt(item2, ITEM_WORKOBJINDEX, -1);
      	   CHAR_setItemIndex(meindex, eptitem, item2);
      	   CHAR_setItemIndex(toindex, itemindex2, -1);
   	}
   	if (itemindex1 < 0)	itemindex1 = 0;
   	gold1 = CHAR_getInt(meindex, CHAR_GOLD);
   	gold1 -= itemindex1;
   	CHAR_setInt(meindex, CHAR_GOLD, gold1);
   	gold2 = CHAR_getInt(toindex, CHAR_GOLD);
   	gold2 += itemindex1;
   	CHAR_setInt(toindex, CHAR_GOLD, gold2);
   }
   // 金钱 <-> 金钱
   else if ((strcmp(a, "G") == 0) && (strcmp(b, "G") == 0))
   {
   	if (itemindex1 < 0)	itemindex1 = 0;
   	if (itemindex2 < 0)	itemindex2 = 0;
   	gold1 = CHAR_getInt(toindex, CHAR_GOLD);
   	gold1 = gold1 + itemindex1 - itemindex2;
   	CHAR_setInt(toindex, CHAR_GOLD, gold1);
   	gold2 = CHAR_getInt(meindex, CHAR_GOLD);
   	gold2 = gold2 + itemindex2 - itemindex1;
   	CHAR_setInt(meindex, CHAR_GOLD, gold2);
   }
   // 宠物 <-> 宠物
   else if ((strcmp(a, "P") == 0) && (strcmp(b, "P") == 0))
   {
	char category[8];
   	if ((item1 != -1) && (CHAR_CHECKINDEX(item1) == FALSE))	return FALSE;
   	if ((item2 != -1) && (CHAR_CHECKINDEX(item2) == FALSE))	return FALSE;

   	if ((itemindex1 != -1) && (itemindex1 == CHAR_getInt(meindex, CHAR_DEFAULTPET)))
   	{
   		int fd;
   		CHAR_setInt(meindex, CHAR_DEFAULTPET, -1);
   		fd = getfdFromCharaIndex(meindex);
		if (fd != -1){
			CHAR_setWorkInt( meindex, CHAR_WORK_PET0_STAT+itemindex1-1, 0);
	   		lssproto_KS_send(fd, itemindex1, 0);
		}
   	}
   	if ((itemindex2 != -1)&& (itemindex2 == CHAR_getInt(toindex, CHAR_DEFAULTPET)))
   	{
   		int tofd;
   		CHAR_setInt(toindex, CHAR_DEFAULTPET, -1);
   		tofd = getfdFromCharaIndex(toindex);
		if (tofd != -1){
			CHAR_setWorkInt( toindex, CHAR_WORK_PET0_STAT+itemindex1-1, 0);			
	   		lssproto_KS_send(tofd, itemindex2, 0);
		}
   	}

	if (item1 != -1)
		CHAR_setWorkInt(item1, CHAR_WORKPLAYERINDEX, toindex);
	if (item2 != -1)
		CHAR_setWorkInt(item2, CHAR_WORKPLAYERINDEX, meindex);
	if (itemindex1 == -1)
		itemindex1 = CHAR_getCharPetElement( meindex);
	if (itemindex2 == -1)
		itemindex2 = CHAR_getCharPetElement( toindex);
	CHAR_setCharPet(meindex, itemindex1, item2);
	CHAR_setCharPet(toindex, itemindex2, item1);
	if ((itemindex1 != -1) && (item1 != -1))	{

			CHAR_setChar(item1, CHAR_OWNERCDKEY,
				CHAR_getChar(toindex, CHAR_CDKEY));
			CHAR_setChar(item1, CHAR_OWNERCHARANAME,
				CHAR_getChar(toindex, CHAR_NAME));

		CHAR_complianceParameter(item1);
		CHAR_setInt(item1, CHAR_PUTPETTIME, 0);
	}
	if ((itemindex2 != -1) && (item2 != -1))	{

		CHAR_setChar(item2, CHAR_OWNERCDKEY,
			CHAR_getChar(meindex, CHAR_CDKEY));
		CHAR_setChar(item2, CHAR_OWNERCHARANAME,
			CHAR_getChar(meindex, CHAR_NAME));

		CHAR_complianceParameter(item2);
		CHAR_setInt(item2, CHAR_PUTPETTIME, 0);
	}
	snprintf(category, sizeof(category), "K%d", itemindex1);
	CHAR_sendStatusString(meindex, category);
	snprintf(category, sizeof(category), "W%d", itemindex1);
	CHAR_sendStatusString(meindex, category);
	snprintf(category, sizeof(category), "K%d", itemindex2);
	CHAR_sendStatusString(toindex, category);
	snprintf(category, sizeof(category), "W%d", itemindex2);
	CHAR_sendStatusString(toindex, category);
   }
   return	TRUE;
}

BOOL TRADE_HandleItem( int meindex, int showindex, char *message, char *outmess)
{
	char token[256];
	int item, itemindex;
	int fd = getfdFromCharaIndex( meindex );

	if( CONNECT_get_confirm( fd) == TRUE ) return FALSE;

	if( CHAR_getWorkInt( meindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_TRADING
		|| CHAR_getWorkInt( meindex, CHAR_WORKTRADEMODE) == CHAR_TRADE_LOCK )
		return FALSE;

	if( getStringFromIndexWithDelim( message, "|", 6, token, sizeof( token)) == FALSE) return FALSE;
	item = atoi( token);

#ifdef _NEW_ITEM_
	if( item < CHAR_STARTITEMARRAY  || item > CheckCharMaxItem(meindex) ){
#else
		if( item < CHAR_STARTITEMARRAY  || item > CHAR_MAXITEMHAVE ){
#endif
		return FALSE;
	}
	itemindex = CHAR_getItemIndex(meindex, item);
	if( !ITEM_CHECKINDEX( itemindex))	return FALSE;

	{
		char itemname[256], tmpbuf[256], tmpbuf1[256];

#ifdef _ITEM_PILEFORTRADE
		int nums = 0;
		if( TRADE_addSomeTradeList( meindex, item, &nums, TRADE_ITEM) == FALSE ){
			return FALSE;
		}
#endif
		strcpy( itemname, ITEM_getChar(itemindex, ITEM_SECRETNAME));//ITEM_NAME
		
	//	if(strstr(itemname,"支票")!=NULL){
//		strcat(itemname,ITEM_getChar(itemindex, ITEM_EFFECTSTRING));
//		}
		sprintf( token, "不会损坏");
#ifdef _TAKE_ITEMDAMAGE
		int crushe	= ITEM_getItemDamageCrusheED( itemindex);
		if( crushe >= 0 ) snprintf( token, sizeof(token), "%d%%", crushe );
#endif
#ifdef _ITEM_PILENUMS
		sprintf( outmess, "%d|%s|%s|%s|%d|%s|%d",
#else
		sprintf( outmess, "%d|%s|%s|%d|%s",
#endif
	   					ITEM_getInt( itemindex, ITEM_BASEIMAGENUMBER),
							makeEscapeString( ITEM_getChar( itemindex, ITEM_NAME), tmpbuf1, sizeof(tmpbuf1)),
#ifdef _ITEM_PILENUMS
	   					makeEscapeString( itemname, tmpbuf, sizeof(tmpbuf)),
#endif
							ITEM_getChar(itemindex,ITEM_EFFECTSTRING), item, token
#ifdef _ITEM_PILEFORTRADE
							,nums
#endif
						);
	}
	return TRUE;
}

BOOL TRADE_HandleGold( int meindex, int showindex, char *message, char *outmess)
{
	int gold, tmpgold;
	char token[256];
	int fd = getfdFromCharaIndex( meindex );
	if(CONNECT_get_confirm( fd)==TRUE)return FALSE;
	if (CHAR_getWorkInt(meindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_TRADING
		|| CHAR_getWorkInt(meindex, CHAR_WORKTRADEMODE) == CHAR_TRADE_LOCK)
		return FALSE;
	tmpgold = CHAR_getInt(meindex, CHAR_GOLD);
	if (getStringFromIndexWithDelim(message, "|", 6, token, sizeof(token)) == FALSE) return FALSE;
	if( (gold = atoi(token)) < 0 ) return FALSE;
	if( gold > tmpgold )return FALSE;
#ifdef _ITEM_PILEFORTRADE
	if( TRADE_addSomeTradeList( meindex, gold, NULL, TRADE_GOLD) == FALSE ) return FALSE;
#endif
	sprintf( outmess, "%d", gold);
	return TRUE;
}

BOOL TRADE_HandlePet( int meindex, int showindex, char *message, char *outmess)
{
	int havepetindex, petindex;
	char token[256], buf[256];
	int fd = getfdFromCharaIndex( meindex );
	if(CONNECT_get_confirm( fd)==TRUE)return FALSE;
	if (CHAR_getWorkInt(meindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_TRADING
		|| CHAR_getWorkInt(meindex, CHAR_WORKTRADEMODE) == CHAR_TRADE_LOCK)
		return FALSE;
	if (getStringFromIndexWithDelim(message, "|", 6, token, sizeof(token)) == FALSE) return FALSE;
	if( (havepetindex = atoi(token)) < 0 ) return FALSE;
	petindex = CHAR_getCharPet(meindex, havepetindex);
	if( !CHAR_CHECKINDEX(petindex))	return FALSE;

#ifdef _ITEM_PILEFORTRADE
	if( TRADE_addSomeTradeList( meindex, havepetindex, NULL, TRADE_PET) == FALSE ) return FALSE;
#endif
#ifdef _POWER_UP
{
				char str[][4] = {"", "A", "B", "C", "D"};
				char buff[64];
				int value = 0;
				int type = 0;
				
				if(getStringFromIndexWithDelim( CHAR_getChar(petindex, CHAR_POWER_UP) , "|" , 1, buff , sizeof(buff) ) == TRUE){
					type = atoi(buff);
				}
				if(getStringFromIndexWithDelim( CHAR_getChar(petindex, CHAR_POWER_UP) , "|" , 2, buff , sizeof(buff) ) == TRUE){
					value += atoi(buff);
				}
				if(getStringFromIndexWithDelim( CHAR_getChar(petindex, CHAR_POWER_UP) , "|" , 3, buff , sizeof(buff) ) == TRUE){
					value += atoi(buff);
				}
				if(getStringFromIndexWithDelim( CHAR_getChar(petindex, CHAR_POWER_UP) , "|" , 4, buff , sizeof(buff) ) == TRUE){
					value += atoi(buff);
				}
				
        if(type > 0){
					sprintf( token, "%d|%s %s+%d|%d|%d|%d|%d|%d|%d",
								CHAR_getInt( petindex, CHAR_BASEIMAGENUMBER),
								makeEscapeString( CHAR_getUseName(petindex), buf, sizeof(buf)),str[type], value,
								CHAR_getInt( petindex, CHAR_LV),
								CHAR_getWorkInt( petindex, CHAR_WORKATTACKPOWER),
								CHAR_getWorkInt( petindex, CHAR_WORKDEFENCEPOWER),
								CHAR_getWorkInt( petindex, CHAR_WORKQUICK),
								havepetindex, 
								CHAR_getInt( petindex , CHAR_TRANSMIGRATION));
				}else{
#ifdef _SHOW_VISUAL_BEATITUDE
				char buff2[256];
				int workatt,workdef,workdex;
				workatt = CHAR_getWorkInt(petindex,CHAR_WORKATTACKPOWER);
				workdef = CHAR_getWorkInt(petindex,CHAR_WORKDEFENCEPOWER);
				workdex = CHAR_getWorkInt(petindex,CHAR_WORKQUICK);
				if (CHAR_getChar(petindex,CHAR_VB) != NULL){
					getStringFromIndexWithDelim(CHAR_getChar(petindex,CHAR_VB),"|",2,buff2,sizeof(buff2));
					if (atoi(buff2)>0)workatt = -1*(workatt);
					getStringFromIndexWithDelim(CHAR_getChar(petindex,CHAR_VB),"|",3,buff2,sizeof(buff2));
					if (atoi(buff2)>0)workdef = -1*(workdef);
					getStringFromIndexWithDelim(CHAR_getChar(petindex,CHAR_VB),"|",4,buff2,sizeof(buff2));
					if (atoi(buff2)>0)workdex = -1*(workdex);
				}
				sprintf( token, "%d|%s|%d|%d|%d|%d|%d|%d",
							CHAR_getInt( petindex, CHAR_BASEIMAGENUMBER),
							makeEscapeString( CHAR_getUseName(petindex), buf, sizeof(buf)),
							CHAR_getInt( petindex, CHAR_LV),
							workatt,
							workdef,
							workdex,
							havepetindex,
							CHAR_getInt( petindex , CHAR_TRANSMIGRATION));

#else
					sprintf( token, "%d|%s|%d|%d|%d|%d|%d|%d",
								CHAR_getInt( petindex, CHAR_BASEIMAGENUMBER),
								makeEscapeString( CHAR_getUseName(petindex), buf, sizeof(buf)),
								CHAR_getInt( petindex, CHAR_LV),
								CHAR_getWorkInt( petindex, CHAR_WORKATTACKPOWER),
								CHAR_getWorkInt( petindex, CHAR_WORKDEFENCEPOWER),
								CHAR_getWorkInt( petindex, CHAR_WORKQUICK),
								havepetindex, 
								CHAR_getInt( petindex , CHAR_TRANSMIGRATION));
#endif
				}
}
#else
	sprintf( token, "%d|%s|%d|%d|%d|%d|%d|%d",
			CHAR_getInt( petindex, CHAR_BASEIMAGENUMBER),
			makeEscapeString( CHAR_getUseName(petindex), buf, sizeof(buf)),
			CHAR_getInt( petindex, CHAR_LV),
			CHAR_getWorkInt( petindex, CHAR_WORKATTACKPOWER),
			CHAR_getWorkInt( petindex, CHAR_WORKDEFENCEPOWER),
			CHAR_getWorkInt( petindex, CHAR_WORKQUICK),
			havepetindex, 
			CHAR_getInt( petindex , CHAR_TRANSMIGRATION));
#endif

#ifdef _TRADESYSTEM2
	{
		int i;
		char skillname[7][256];
		char buf1[256],buf2[256];
		for( i=0; i<7; i++){
			int skillarray, skillID;
			memset( skillname[i], 0, sizeof(skillname[i]));
			skillID = CHAR_getPetSkill( petindex, i);
			skillarray = PETSKILL_getPetskillArray( skillID);
			if( !PETSKILL_CHECKINDEX( skillarray)) continue;
			sprintf( skillname[i], "%s", PETSKILL_getChar( skillarray, PETSKILL_NAME) );
		}
#ifdef _POWER_UP
{
				char str[][4] = {"", "A", "B", "C", "D"};
				char buff[64];
				int value = 0;
				int type = 0;
				
				if(getStringFromIndexWithDelim( CHAR_getChar(petindex, CHAR_POWER_UP) , "|" , 1, buff , sizeof(buff) ) == TRUE){
					type = atoi(buff);
				}
				if(getStringFromIndexWithDelim( CHAR_getChar(petindex, CHAR_POWER_UP) , "|" , 2, buff , sizeof(buff) ) == TRUE){
					value += atoi(buff);
				}
				if(getStringFromIndexWithDelim( CHAR_getChar(petindex, CHAR_POWER_UP) , "|" , 3, buff , sizeof(buff) ) == TRUE){
					value += atoi(buff);
				}
				if(getStringFromIndexWithDelim( CHAR_getChar(petindex, CHAR_POWER_UP) , "|" , 4, buff , sizeof(buff) ) == TRUE){
					value += atoi(buff);
				}
				
        if(type > 0){
					sprintf( outmess , "%s|%d|%d|%s|%s|%s|%s|%s|%s|%s|%s %s+%d|%s %s+%d|"
#ifdef _SHOW_FUSION
					"%d|"
#endif
						,token , CHAR_getWorkInt(petindex , CHAR_WORKMAXHP) , CHAR_getInt(petindex , CHAR_SLOT)
						,skillname[0] , skillname[1] , skillname[2] , skillname[3]
						,skillname[4] , skillname[5] , skillname[6]
						,makeEscapeString( CHAR_getChar( petindex, CHAR_NAME ), buf1, sizeof(buf1)),str[type], value
						,makeEscapeString( CHAR_getUseName( petindex), buf2, sizeof(buf2)),str[type], value
#ifdef _SHOW_FUSION
						,CHAR_getInt ( petindex, CHAR_FUSIONBEIT)
#endif
						);
								
				}else{
#ifdef _SHOW_VISUAL_BEATITUDE
					char buff2[256];
					int workhp;
					workhp = CHAR_getWorkInt(petindex,CHAR_WORKMAXHP);
					if (CHAR_getChar(petindex,CHAR_VB) != NULL){
						getStringFromIndexWithDelim(CHAR_getChar(petindex,CHAR_VB),"|",1,buff2,sizeof(buff2));
						if (atoi(buff2)>0)workhp = -1*(workhp);
					}
					sprintf( outmess , "%s|%d|%d|%s|%s|%s|%s|%s|%s|%s|%s|%s|"
#ifdef _SHOW_FUSION
					"%d|"
#endif
						,token , workhp , CHAR_getInt(petindex , CHAR_SLOT)
						,skillname[0] , skillname[1] , skillname[2] , skillname[3]
						,skillname[4] , skillname[5] , skillname[6]
						,makeEscapeString( CHAR_getChar( petindex, CHAR_NAME ), buf1, sizeof(buf1))
						,makeEscapeString( CHAR_getUseName( petindex), buf2, sizeof(buf2))
#ifdef _SHOW_FUSION
						,CHAR_getInt ( petindex, CHAR_FUSIONBEIT)
#endif
						);
#else

					sprintf( outmess , "%s|%d|%d|%s|%s|%s|%s|%s|%s|%s|%s|%s|"
#ifdef _SHOW_FUSION
					"%d|"
#endif
						,token , CHAR_getWorkInt(petindex , CHAR_WORKMAXHP) , CHAR_getInt(petindex , CHAR_SLOT)
						,skillname[0] , skillname[1] , skillname[2] , skillname[3]
						,skillname[4] , skillname[5] , skillname[6]
						,makeEscapeString( CHAR_getChar( petindex, CHAR_NAME ), buf1, sizeof(buf1))
						,makeEscapeString( CHAR_getUseName( petindex), buf2, sizeof(buf2))
#ifdef _SHOW_FUSION
						,CHAR_getInt ( petindex, CHAR_FUSIONBEIT)
#endif
						);
#endif
				}
}
#else
		sprintf( outmess , "%s|%d|%d|%s|%s|%s|%s|%s|%s|%s|%s|%s|"
#ifdef _SHOW_FUSION
		"%d|",
#endif
			,token , CHAR_getWorkInt(petindex , CHAR_WORKMAXHP) , CHAR_getInt(petindex , CHAR_SLOT)
			,skillname[0] , skillname[1] , skillname[2] , skillname[3]
			,skillname[4] , skillname[5] , skillname[6]
			,makeEscapeString( CHAR_getChar( petindex, CHAR_NAME ), buf1, sizeof(buf1))
			,makeEscapeString( CHAR_getUseName( petindex), buf2, sizeof(buf2))
#ifdef _SHOW_FUSION
			,CHAR_getInt ( petindex, CHAR_FUSIONBEIT)
#endif
			);
#endif
			
#ifdef _PET_VALIDITY
	{
		char token[256];
		if(CHAR_getInt ( petindex, CHAR_PETVALIDITY)==0){
			snprintf( token, sizeof( token ), "永久有效" STATUSSENDDELIMITER);
		}else if(CHAR_getInt ( petindex, CHAR_PETVALIDITY)>time(NULL)){
			time_t petvalidity = (time_t)CHAR_getInt(petindex, CHAR_PETVALIDITY);
			struct tm *tm1 = localtime(&petvalidity);

			snprintf( token, sizeof( token ), "%04d-%02d-%02d|", 
																											tm1->tm_year + 1900,
																											tm1->tm_mon + 1,
																											tm1->tm_mday);
		}else{
			snprintf( token, sizeof( token ), "已经过期" STATUSSENDDELIMITER);
		}
		
		strcat( outmess,token );
	}
#endif

#ifdef _PET_ITEM
				char token[512]="";
				int j;
				for( j = 0 ; j < CHAR_MAXPETITEMHAVE ; j ++ )
					strcat(token,ITEM_petmakeItemStatusString(petindex, j));
				strcat(outmess,token);
#endif
	}
#else
	sprintf( outmess , "%s|%d", token , CHAR_getWorkInt(petindex , CHAR_WORKMAXHP) );
#endif //_TRADESYSTEM2
	return TRUE;
}

#ifdef _ITEM_PILEFORTRADE
BOOL TRADE_CheckTradeList( int meindex, STradeList *temp1, int toindex, STradeList *temp2)
{
	int i;
	int MeSurplus=0, MeNeeds=0, MeMaxPile;
	int ToSurplus=0, ToNeeds=0, ToMaxPile;


	MeMaxPile = CHAR_getMyMaxPilenum( meindex);//最大堆叠数
	ToMaxPile = CHAR_getMyMaxPilenum( toindex);

	MeSurplus = CHAR_findSurplusItemBox( meindex);
	ToSurplus = CHAR_findSurplusItemBox( toindex);
	
	//道具
	ToNeeds=0; MeNeeds=0;
	for( i=0; i<15; i++ ){
		int pilenum, itemindex;
		if( temp1->ItemTi[i] == -1 ) continue;
		itemindex = CHAR_getItemIndex( meindex, temp1->ItemTi[i] );
		if( !ITEM_CHECKINDEX( itemindex))	return FALSE;
		pilenum = ITEM_getInt( itemindex, ITEM_USEPILENUMS);
		
#ifdef _TRADE_PK
		if(CHAR_getInt(meindex, CHAR_FLOOR)!=50000 ||	CHAR_getInt(toindex, CHAR_FLOOR)!=50000){
			if( temp1->ItemNum[i] == pilenum ){
				MeSurplus++;
			}
		}
#else
		if( temp1->ItemNum[i] == pilenum ){
			MeSurplus++;
		}
#endif
		if( temp1->ItemNum[i] > ToMaxPile ){
			ToNeeds += (temp1->ItemNum[i]/ToMaxPile) + 1;
		}else{
			ToNeeds++;
		}
	}
	for( i=0; i<15; i++ ){
		int pilenum, itemindex;
		if( temp2->ItemTi[i] == -1 ) continue;
		itemindex = CHAR_getItemIndex( toindex, temp2->ItemTi[i] );
		if( !ITEM_CHECKINDEX( itemindex))	return FALSE;
		pilenum = ITEM_getInt( itemindex, ITEM_USEPILENUMS);
		
#ifdef _TRADE_PK
		if(CHAR_getInt(meindex, CHAR_FLOOR)!=50000 ||	CHAR_getInt(toindex, CHAR_FLOOR)!=50000){
			if( temp2->ItemNum[i] == pilenum){
				ToSurplus++;
			}
		}
#else
		if( temp2->ItemNum[i] == pilenum){
			ToSurplus++;
		}
#endif
			
		
		if( temp2->ItemNum[i] > MeMaxPile ){
			MeNeeds += (temp2->ItemNum[i]/MeMaxPile) + 1;
		}else {
			MeNeeds++;
		}
	}
	
	if( MeSurplus < MeNeeds ){
		CHAR_talkToCli( meindex, -1, "交易後物品栏位不足。", CHAR_COLORYELLOW);
		CHAR_talkToCli( toindex, -1, "对方交易後物品栏位不足。", CHAR_COLORYELLOW);
		return FALSE;
	}
	if( ToSurplus < ToNeeds ){
		CHAR_talkToCli( toindex, -1, "交易後物品栏位不足。", CHAR_COLORYELLOW);
		CHAR_talkToCli( meindex, -1, "对方交易後物品栏位不足。", CHAR_COLORYELLOW);
		return FALSE;
	}

	//宠物
	MeSurplus = CHAR_findSurplusPetBox( meindex);
	ToSurplus = CHAR_findSurplusPetBox( toindex);
	ToNeeds=0; MeNeeds=0;
	

	for( i=0; i<5; i++ ){
		int petindex;
		if( temp1->PetTi[i] == -1 ) continue;
		petindex = CHAR_getCharPet( meindex, temp1->PetTi[i] );
		if( !CHAR_CHECKINDEX( petindex))	return FALSE;
#ifdef _TRADE_PK
		if(CHAR_getInt(meindex, CHAR_FLOOR)!=50000 ||	CHAR_getInt(toindex, CHAR_FLOOR)!=50000){
			MeSurplus++;
		}
#else
		MeSurplus++;
#endif
		ToNeeds++;
	}
	for( i=0; i<5; i++ ){
		int petindex;
		if( temp2->PetTi[i] == -1 ) continue;
		petindex = CHAR_getCharPet( toindex, temp2->PetTi[i] );
		if( !CHAR_CHECKINDEX( petindex))	return FALSE;
#ifdef _TRADE_PK
		if(CHAR_getInt(meindex, CHAR_FLOOR)!=50000 ||	CHAR_getInt(toindex, CHAR_FLOOR)!=50000){
			ToSurplus++;
		}
#else
		ToSurplus++;
#endif
		MeNeeds++;
	}


	if( MeSurplus < MeNeeds ){
		CHAR_talkToCli( meindex, -1, "交易後宠物栏位不足。", CHAR_COLORYELLOW);
		CHAR_talkToCli( toindex, -1, "对方交易後宠物栏位不足。", CHAR_COLORYELLOW);
		return FALSE;
	}
	if( ToSurplus < ToNeeds ){
		CHAR_talkToCli( meindex, -1, "对方交易後宠物栏位不足。", CHAR_COLORYELLOW);
		CHAR_talkToCli( toindex, -1, "交易後宠物栏位不足。", CHAR_COLORYELLOW);
		return FALSE;
	}

	//金钱
	MeMaxPile = CHAR_getMaxHaveGold( meindex);
	ToMaxPile = CHAR_getMaxHaveGold( toindex);
	
#ifdef _TRADE_PK
	if(CHAR_getInt(meindex, CHAR_FLOOR)!=50000 ||	CHAR_getInt(toindex, CHAR_FLOOR)!=50000){
		MeSurplus = MeMaxPile - CHAR_getInt( meindex, CHAR_GOLD);
		ToSurplus = ToMaxPile - CHAR_getInt( toindex, CHAR_GOLD);
	}
#else
	MeSurplus = MeMaxPile - CHAR_getInt( meindex, CHAR_GOLD);
	ToSurplus = ToMaxPile - CHAR_getInt( toindex, CHAR_GOLD);
#endif


	ToNeeds=0; MeNeeds=0;
	MeSurplus += temp1->Golds;
	ToSurplus += temp2->Golds;
	ToNeeds = temp1->Golds;
	MeNeeds = temp2->Golds;
	if( MeSurplus < MeNeeds ){
		CHAR_talkToCli( meindex, -1, "交易後石币超过上限。", CHAR_COLORYELLOW);
		CHAR_talkToCli( toindex, -1, "对方交易後石币超过上限。", CHAR_COLORYELLOW);
		return FALSE;
	}
	if( ToSurplus < ToNeeds ){
		CHAR_talkToCli( meindex, -1, "对方交易後石币超过上限。", CHAR_COLORYELLOW);
		CHAR_talkToCli( toindex, -1, "交易後石币超过上限。", CHAR_COLORYELLOW);
		return FALSE;
	}
	return TRUE;
}
#endif

