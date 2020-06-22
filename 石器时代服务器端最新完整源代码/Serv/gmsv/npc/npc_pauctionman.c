#include "version.h"
#include <string.h>

#include "char.h"
#include "object.h"
#include "char_base.h"
#include "npcutil.h"
#include "npc_pauctionman.h"
#include "lssproto_serv.h"
#include "saacproto_cli.h"
#include "readmap.h"
#include "battle.h"
#include "log.h"
#include "enemy.h"
#include "handletime.h"
#include "npc_eventaction.h"
#include "longzoro/sasql.h"
#include "pet.h"
//特地从天堂高薪挖角来的"传送师"
#ifdef _PAUCTION_MAN
enum {
	WINDOW_PAUCTION_START=1,
	WINDOW_PAUCTION_SELECT,
	WINDOW_PAUCTION_NEW,
	WINDOW_PAUCTION_AUCTIONSURVEY,
	WINDOW_PAUCTION_LIST_BUY_ALL,
	WINDOW_PAUCTION_LIST_BUY_PET,
	WINDOW_PAUCTION_LIST_BUY_ITEM,
	WINDOW_PAUCTION_LIST_MODIFY,
};

enum {
	NPC_WORK_CURRENTTIME = CHAR_NPCWORKINT1,
/*
	NPC_WORK_ROUTETOY = CHAR_NPCWORKINT2,
	NPC_WORK_ROUTEPOINT = CHAR_NPCWORKINT3,
	NPC_WORK_ROUNDTRIP = CHAR_NPCWORKINT4,
	NPC_WORK_MODE = CHAR_NPCWORKINT5,
	NPC_WORK_CURRENTROUTE = CHAR_NPCWORKINT6, 
	NPC_WORK_ROUTEMAX = CHAR_NPCWORKINT7,
	NPC_WORK_WAITTIME = CHAR_NPCWORKINT8,
	NPC_WORK_CURRENTTIME = CHAR_NPCWORKINT9,
	NPC_WORK_SEFLG = CHAR_NPCWORKINT10,
*/
};

#define STANDBYTIME 50

static void NPC_Pauction_selectWindow( int meindex, int toindex, int seqno, int select, char *data);

BOOL NPC_PauctionInit( int meindex )
{
	char npcarg[NPC_UTIL_GETARGSTR_BUFSIZE];
		
	if(NPC_Util_GetArgStr( meindex, npcarg, sizeof(npcarg))==NULL){
		print("TRANSER_MAN: GetArgStrErr!!");
		return FALSE;
	}

//   	CHAR_setInt( meindex, CHAR_WHICHTYPE, CHAR_TRANSERMANS);

	//CHAR_setInt( meindex, CHAR_LOOPINTERVAL, NEWNPCMAN_STANDBY);
	CHAR_setWorkInt( meindex, NPC_WORK_CURRENTTIME, NowTime.tv_sec);
	return TRUE;
}
//CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANT,1);
void NPC_PauctionTalked( int meindex, int talkerindex, char *msg, int color )
{
	if( CHAR_getInt( talkerindex , CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER )
    	return;
	if(NPC_Util_isFaceToFace( meindex ,talkerindex , 2) == FALSE) {
		if(NPC_Util_isFaceToChara( talkerindex, meindex, 1) == FALSE)
			return;
	}

	CHAR_setWorkInt( talkerindex, CHAR_WORKSHOPRELEVANT, 0);
	NPC_Pauction_selectWindow( meindex, talkerindex, WINDOW_PAUCTION_START, 0, 0);
}

void NPC_PauctionWindowTalked( int meindex, int talkerindex, int seqno,
								int select, char *data)
{
	if( select == WINDOW_BUTTONTYPE_CANCEL || select == WINDOW_BUTTONTYPE_NO)
		return;
	switch( seqno)	{
		case NPC_PAUCTION_START:
			break;
		case NPC_PAUCTION_SELECT:
			NPC_Pauction_selectWindow( meindex, talkerindex, WINDOW_PAUCTION_SELECT, select, data);
			break;
		case NPC_PAUCTION_NEW:
			NPC_Pauction_selectWindow( meindex, talkerindex, WINDOW_PAUCTION_NEW, select, data);
			break;
		case NPC_PAUCTION_AUCTIONSURVEY:
			NPC_Pauction_selectWindow( meindex, talkerindex, WINDOW_PAUCTION_AUCTIONSURVEY, select, data);
			break;
		case NPC_PAUCTION_LIST_BUY_ALL:
			NPC_Pauction_selectWindow( meindex, talkerindex, WINDOW_PAUCTION_LIST_BUY_ALL, select, data);
			break;
		case NPC_PAUCTION_LIST_BUY_PET:
			NPC_Pauction_selectWindow( meindex, talkerindex, WINDOW_PAUCTION_LIST_BUY_PET, select, data);
			break;
		case NPC_PAUCTION_LIST_BUY_ITEM:
			NPC_Pauction_selectWindow( meindex, talkerindex, WINDOW_PAUCTION_LIST_BUY_ITEM, select, data);
			break;
		case NPC_PAUCTION_LIST_MODIFY:
			NPC_Pauction_selectWindow( meindex, talkerindex, WINDOW_PAUCTION_LIST_MODIFY, select, data);
			break;
	}
}

static void NPC_Pauction_selectWindow( int meindex, int toindex, int seqno, int select, char *data)
{
	char npcarg[NPC_UTIL_GETARGSTR_BUFSIZE];
	char token[1024];
	char buf[1024];
	int buttontype = 0, windowtype = 0, windowno = 0;
	int fd = getfdFromCharaIndex( toindex);
	windowtype = WINDOW_MESSAGETYPE_MESSAGE;
	buttontype = WINDOW_BUTTONTYPE_OK;
	if(NPC_Util_isFaceToFace( meindex ,toindex , 2) == FALSE) {
		if(NPC_Util_isFaceToChara( toindex, meindex, 1) == FALSE)
			return;
	}

	memset( npcarg, 0, sizeof( npcarg));
	if(NPC_Util_GetArgStr( meindex, npcarg, sizeof(npcarg))==NULL){
		print("Pauction_MAN: GetArgStrErr!!");
		return;
	}
	memset( token, 0, sizeof( token));
	switch( seqno)	{
	case WINDOW_PAUCTION_START:
		sprintf(token, "3\n              ★委托拍卖交易大王★\n\n"
											"有什么需要我为你服务呢？\n"
											"                《我要委托一项交易》\n"
											"                《查看所有委托列表》\n"
											"                《查看宠物委托列表》\n"
											"                《查看物品委托列表》\n"
											"                《查看个人委托列表》\n"
											);

		windowtype = WINDOW_MESSAGETYPE_SELECT;
		buttontype = WINDOW_BUTTONTYPE_YESNO;
		windowno = NPC_PAUCTION_SELECT;
		break;
	case WINDOW_PAUCTION_SELECT:
		{
			int num = 0;
			if(atoi(data)==1){
				windowtype = WINDOW_MESSAGETYPE_AUCTIONNEW;
				windowno = NPC_PAUCTION_NEW;
			}else if(atoi(data)==2 || atoi(data)==3 || atoi(data)==4){
				CHAR_setWorkInt( toindex, CHAR_WORK_LISTPAGE, 0);
				if(atoi(data)==2){
					num = sasql_getPauctionList(token, 0, 0, "");
					windowno = NPC_PAUCTION_LIST_BUY_ALL;
				}else if(atoi(data)==3){ 
					num = sasql_getPauctionList(token, 0, 1, "");
					windowno = NPC_PAUCTION_LIST_BUY_PET;
				}else if(atoi(data)==4){ 
					num = sasql_getPauctionList(token, 0, 2, "");
					windowno = NPC_PAUCTION_LIST_BUY_ITEM;
				}
				windowtype = WINDOW_MESSAGETYPE_AUCTIONLIST_BUY;
				
			}else if(atoi(data)==5){
				num = sasql_getPauctionList(token, 0, 0, CHAR_getChar(toindex, CHAR_CDKEY));
				windowtype = WINDOW_MESSAGETYPE_AUCTIONLIST_MODIFY;
				windowno = NPC_PAUCTION_LIST_MODIFY;
			}
			
			if(num>10){
				buttontype = WINDOW_BUTTONTYPE_NEXT;
			}
		}
		break;
	case WINDOW_PAUCTION_NEW:
		{
			char effect[128];
			char info[128];
			char tmp[128];
			char *string = "";
			makeStringFromEscaped(data);
			
			
			getStringFromIndexWithDelim( data, "|", 1, tmp, sizeof( tmp));
			int type = atoi(tmp);
			getStringFromIndexWithDelim( data, "|", 2, tmp, sizeof( tmp));
			int id = atoi(tmp);
			getStringFromIndexWithDelim( data, "|", 3, effect, sizeof( effect));
			if(strlen(effect)==0){
				sprintf(token, "委托失败,请填写委托叙述~");
				break;
			}
			
			getStringFromIndexWithDelim( data, "|", 4, tmp, sizeof( tmp));
			int cost = atoi(tmp);
			if(cost<=0){
				sprintf(token, "委托价格不能为0~");
				break;
			}
			if(type == 1){
				int petindex = CHAR_getCharPet( toindex, id);
			  if( !CHAR_CHECKINDEX(petindex)){
			  	sprintf(token, "你并没有你要委托的宠物~");
					break;
			  }
#ifdef _PET_ITEM
			  int i;
			  for(i=0;i<CHAR_PETITEMNUM;i++){
			  	if(CHAR_getItemIndex(petindex, id)){
			  		break;
			  	}
			  }
			  if(i<CHAR_PETITEMNUM){
			  	sprintf(token, "请装宠物身上的装备取下~");
					break;
			  }
#endif
			  sprintf(info, "%s|%d|%d|%d|%d|%d|%d|%d|", CHAR_getChar(petindex, CHAR_NAME), 
			  																							CHAR_getInt(petindex, CHAR_BASEBASEIMAGENUMBER), 
																						  				CHAR_getInt(petindex, CHAR_LV),
																											CHAR_getWorkInt(petindex, CHAR_WORKATTACKPOWER),
																											CHAR_getWorkInt(petindex, CHAR_WORKDEFENCEPOWER),
																											CHAR_getWorkInt(petindex, CHAR_WORKQUICK),
																											CHAR_getWorkInt(petindex, CHAR_WORKMAXHP),
																											CHAR_getWorkInt(petindex, CHAR_WORKFIXAI));

				string = CHAR_makePetStringFromPetIndex( petindex);
				if( string == "\0" ){
					sprintf(token, "委托宠物失败,请与管理员联系~");
			  	break;
			  }
				char category[12];
				CHAR_endCharOneArray( petindex );
				CHAR_setCharPet( toindex, id, -1);
				snprintf( category, sizeof( category), "K%d",id);
				CHAR_sendStatusString( toindex, category );
				snprintf( category, sizeof( category), "W%d",id);
				CHAR_sendStatusString( toindex, category );
			}else{
        int itemindex = CHAR_getItemIndex(toindex, id);
        
        if( !ITEM_CHECKINDEX(itemindex)){
			  	sprintf(token, "你并没有你要委托的物品~");
					break;
			  }
			  
			  if(ITEM_getInt( itemindex, ITEM_USEPILENUMS)>1){
			  	sprintf(token, "叠加物品无法拍卖~");
					break;
			  }
			  
			  sprintf(info, "%s|%d|%d|%s|", ITEM_getChar(itemindex, ITEM_NAME), 
			  																							ITEM_getInt(itemindex, ITEM_BASEIMAGENUMBER), 
#ifdef _ITEM_COLOER
																											ITEM_getInt( itemindex, ITEM_COLOER),
#else
																											CHAR_COLORWHITE,
#endif
																						  				ITEM_getChar(itemindex, ITEM_EFFECTSTRING));															
        
        string = ITEM_makeStringFromItemIndex(itemindex, 0);
        if( string == "\0" ){
			  	sprintf(token, "委托物品失败,请与管理员联系~");
			  	break;
			  }
			  CHAR_DelItem( toindex, id);
			}
			char *cdkey = CHAR_getChar(toindex, CHAR_CDKEY);
			char *name = CHAR_getChar(toindex, CHAR_NAME);
			sasql_addPauctionInfo(cdkey, name, effect, cost, type, info, string);
			sprintf(token, "成功为您登记了一条委托交易信息,我们会尽心尽力为您服务,如有一切动态,我们会第一时间通知您!");
		}
		break;
	case WINDOW_PAUCTION_LIST_BUY_ALL:
	case WINDOW_PAUCTION_LIST_BUY_PET:
	case WINDOW_PAUCTION_LIST_BUY_ITEM:
	case WINDOW_PAUCTION_LIST_MODIFY:
		{
			if(select == WINDOW_BUTTONTYPE_PREV || select == WINDOW_BUTTONTYPE_NEXT){
				int page = 0;
				int num=0;
				windowtype = WINDOW_MESSAGETYPE_AUCTIONLIST_BUY;
				if(select == WINDOW_BUTTONTYPE_PREV){
					page = CHAR_getWorkInt( toindex, CHAR_WORK_LISTPAGE) - 1;
					if(page<0) page = 0;
				}else if(select == WINDOW_BUTTONTYPE_NEXT){
					page = CHAR_getWorkInt( toindex, CHAR_WORK_LISTPAGE) + 1;
					if(page>8) page = 8;
				}
				if(seqno==WINDOW_PAUCTION_LIST_BUY_ALL){
					num = sasql_getPauctionList(token, page * 10, 0, "");
					windowno = NPC_PAUCTION_LIST_BUY_ALL;
				}else if(seqno==WINDOW_PAUCTION_LIST_BUY_PET){
					num = sasql_getPauctionList(token, page * 10, 1, "");
					windowno = NPC_PAUCTION_LIST_BUY_PET;
				}else if(seqno==WINDOW_PAUCTION_LIST_BUY_ITEM){
					num = sasql_getPauctionList(token, page * 10, 2, "");
					windowno = NPC_PAUCTION_LIST_BUY_ITEM;
				}else if(seqno==WINDOW_PAUCTION_LIST_MODIFY){
					num = sasql_getPauctionList(token, page * 10, 0, CHAR_getChar(toindex, CHAR_CDKEY));
					windowno = NPC_PAUCTION_LIST_MODIFY;
					windowtype = WINDOW_MESSAGETYPE_AUCTIONLIST_MODIFY;
				}
				buttontype = 0;
				if(num>(page + 1) * 10){
					buttontype |= WINDOW_BUTTONTYPE_NEXT;
				}
				if(page > 0){
					buttontype |= WINDOW_BUTTONTYPE_PREV;
				}
				CHAR_setWorkInt( toindex, CHAR_WORK_LISTPAGE, page);
			}else	if(select==1){
				char string[1024];
				char buff[256];
				int type = sasql_getMyPauction(string, atoi(data), CHAR_getChar(toindex, CHAR_CDKEY));
				if(type == 1){
					int havepetelement = CHAR_getCharPetElement( toindex);
    			if( havepetelement < 0 ) {
    				sprintf(token, "很抱歉,你身上宠物已满,请空出一个宠物栏位!");
    				break;
    			}
    			Char petone;
          int ret = CHAR_makePetFromStringToArg( string , &petone, 0 );

          if( ret == TRUE ){
            int existpetindex;
            existpetindex = PET_initCharOneArray( &petone );
            if( existpetindex < 0 ){
            	sprintf(token, "取回宠物失败!");
    					break;
            }
						CHAR_setWorkInt( existpetindex, CHAR_WORKPLAYERINDEX, toindex);
						CHAR_setCharPet( toindex, havepetelement, existpetindex);
						CHAR_complianceParameter( existpetindex);
						snprintf( buff, sizeof( buff ), "K%d", havepetelement );
						CHAR_sendStatusString( toindex, buff );
						snprintf( buff, sizeof( buff ), "W%d", havepetelement );
						CHAR_sendStatusString( toindex, buff );
						sasql_delPauctionBuy(atoi(data), CHAR_getChar(toindex, CHAR_CDKEY));
          }
					sprintf(token, "成功取回你的宠物!");
					break;
				}else if(type == 2){
		    	int emptyitemindexinchara = CHAR_findEmptyItemBox( toindex );
					if( emptyitemindexinchara < 0 ){
						sprintf(token, "很抱歉,你身上物品已满,请空出一个物品栏位!");
						break;
					}
					ITEM_Item   itmone;
          BOOL ret = ITEM_makeExistItemsFromStringToArg( string, &itmone, 0);

          if( ret == TRUE ){
              int itemindex = ITEM_initExistItemsOne( &itmone );
              if( itemindex == -1 ){
              	sprintf(token, "取回物品失败!");
    						break;
              }else{
					    	CHAR_setItemIndex( toindex, emptyitemindexinchara, itemindex );
					    	ITEM_setWorkInt(itemindex, ITEM_WORKOBJINDEX,-1);
					    	ITEM_setWorkInt(itemindex, ITEM_WORKCHARAINDEX,toindex);
					    	CHAR_sendItemDataOne( toindex, emptyitemindexinchara);
					    	sasql_delPauctionBuy(atoi(data), CHAR_getChar(toindex, CHAR_CDKEY));
					    }
          }
					sprintf(token, "成功取回你的物品!");
					break;
				}else{
					sprintf(token, "??????????");
				}
			}else if(select==4){
				sasql_getPauctionSurvey(token, atoi(data));
				windowno = NPC_PAUCTION_AUCTIONSURVEY;
				windowtype = WINDOW_MESSAGETYPE_AUCTIONSURVEY;
			}
		}
		break;
	case WINDOW_PAUCTION_AUCTIONSURVEY:
		{
			char string[1024];
			char buff[256];
			int type = sasql_getMyPauction(string, atoi(data), "");

			if(type == 1){
				int havepetelement = CHAR_getCharPetElement( toindex);
    		if( havepetelement < 0 ) {
    			sprintf(token, "很抱歉,你身上宠物已满,请空出一个宠物栏位!");
    			break;
    		}
    		int flg = sasql_PauctionOK(atoi(data), CHAR_getChar(toindex, CHAR_CDKEY));
				if(flg == 1){
					Char petone;
          int ret = CHAR_makePetFromStringToArg( string , &petone, 0 );

          if( ret == TRUE ){
            int existpetindex;
            existpetindex = PET_initCharOneArray( &petone );
            if( existpetindex < 0 ){
            	sprintf(token, "取回宠物失败!");
    					break;
            }
						CHAR_setWorkInt( existpetindex, CHAR_WORKPLAYERINDEX, toindex);
						CHAR_setCharPet( toindex, havepetelement, existpetindex);
						CHAR_complianceParameter( existpetindex);
						snprintf( buff, sizeof( buff ), "K%d", havepetelement );
						CHAR_sendStatusString( toindex, buff );
						snprintf( buff, sizeof( buff ), "W%d", havepetelement );
						CHAR_sendStatusString( toindex, buff );
						sasql_delPauctionBuy(atoi(data), "");
          }
					sprintf(token, "成功购买委托宠物!");
				}else if(flg == -1){
					sprintf(token, "你所要购买的委托宠物不存在或已被他人购买了!");
				}else if(flg == -2){
					sprintf(token, "很抱歉,你身上的重回币数不够购买!");
				}else{
					sprintf(token, "数据库操作错误,请与管理员联系!");
				}
    	}else if(type == 2){
		    int emptyitemindexinchara = CHAR_findEmptyItemBox( toindex );
				if( emptyitemindexinchara < 0 ){
					sprintf(token, "很抱歉,你身上物品已满,请空出一个物品栏位!");
					break;
				}
				int flg = sasql_PauctionOK(atoi(data), CHAR_getChar(toindex, CHAR_CDKEY));
				if(flg == 1){
					ITEM_Item   itmone;
          BOOL ret = ITEM_makeExistItemsFromStringToArg( string, &itmone, 0);

          if( ret == TRUE ){
              int itemindex = ITEM_initExistItemsOne( &itmone );
              if( itemindex == -1 ){
              	sprintf(token, "取回物品失败!");
    						break;
              }else{
					    	CHAR_setItemIndex( toindex, emptyitemindexinchara, itemindex );
					    	ITEM_setWorkInt(itemindex, ITEM_WORKOBJINDEX,-1);
					    	ITEM_setWorkInt(itemindex, ITEM_WORKCHARAINDEX,toindex);
					    	CHAR_sendItemDataOne( toindex, emptyitemindexinchara);
					    	sasql_delPauctionBuy(atoi(data), "");
					    }
          }
					sprintf(token, "成功购买委托物品!");
				}else if(flg == -1){
					sprintf(token, "你所要购买的委托物品不存在或已被他人购买了!");
				}else if(flg == -2){
					sprintf(token, "很抱歉,你身上的重回币数不够购买!");
				}else{
					sprintf(token, "数据库操作错误,请与管理员联系!");
				}
    	}else{
				sprintf(token, "??????????");
				break;
			}
		}
		break;
	}

	lssproto_WN_send( fd, windowtype, buttontype, windowno,
		CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX), makeEscapeString(token, buf, sizeof( buf) ));
}


void NPC_PauctionLoop( int meindex)
{

}

#endif


