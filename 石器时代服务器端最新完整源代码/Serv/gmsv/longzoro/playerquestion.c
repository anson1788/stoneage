#include <string.h>
#include "version.h"
#include "longzoro/playerquestion.h"
#include "char_base.h"
#include "char.h"
#include "autil.h"
#include "enemy.h"
#include "family.h"
#include "log.h"

#ifdef _PLAYER_QUESTION_ONLIEN
PlayerQuestion_t PlayerQuestion;

BOOL PlayQuestionOnline(int charaindex, char *messageeraseescape)
{
	if(messageeraseescape[0] == '/' && messageeraseescape[1] == 'h' && messageeraseescape[2] == 'd'){
		
		if(strlen(PlayerQuestion.question)==0 || strlen(PlayerQuestion.result)==0){
			CHAR_talkToCli(charaindex,-1,"GM还未在线发问题!",CHAR_COLORRED);
			return FALSE;
		}
		
		char token[32];
		easyGetTokenFromString( messageeraseescape , 2 , token, sizeof(token));
		
		if(strcmp( token, PlayerQuestion.result) == 0){
			switch(PlayerQuestion.type){
				case 0:
					// 加钱
				  CHAR_setInt(charaindex,CHAR_GOLD, CHAR_getInt(charaindex,CHAR_GOLD) + PlayerQuestion.value);
					CHAR_send_P_StatusString(charaindex,CHAR_P_STRING_GOLD);
					
					break;
				case 1:
					{
					  int itemindex=-1;
					  char token[256];
					  int ret;

					  itemindex = CHAR_findEmptyItemBox( charaindex );
						if( itemindex < 0 )	{
							CHAR_talkToCli( charaindex, -1, "很抱歉,你的物品栏已满!",  CHAR_COLORYELLOW);
							break;
						}
	
						itemindex = ITEM_makeItemAndRegist( PlayerQuestion.value );
					  if( itemindex != -1 ){
							ret = CHAR_addItemSpecificItemIndex( charaindex, itemindex);
#ifdef _NEW_ITEM_
extern int CheckCharMaxItem(int charindex);
#endif
							if( ret < 0 ||
#ifdef _NEW_ITEM_
								ret >= CheckCharMaxItem(charaindex)
#else
								ret >= CHAR_MAXITEMHAVE
#endif
								) {
								ITEM_endExistItemsOne( itemindex);
								CHAR_talkToCli( charaindex, -1, "很抱歉,无法赠送你物品给你!",  CHAR_COLORYELLOW);
							break;
							}
					
							sprintf( token,"恭喜你获得:%s",ITEM_getChar( itemindex, ITEM_NAME));
							CHAR_talkToCli( charaindex, -1, token, CHAR_COLORYELLOW );
	
							CHAR_sendItemDataOne( charaindex, ret);
					  }
					}
					break;
				case 2:
					{
						char msgbuf[64];
						int	enemynum;
						int	i,j;
						int petindex;
					
						for( i = 0 ;i < CHAR_MAXPETHAVE ; i++) {
							if( CHAR_getCharPet( charaindex, i) == -1  )
								break;
						}
					
					  if( i == CHAR_MAXPETHAVE ){
							CHAR_talkToCli( charaindex, -1, "很抱歉,你身上宠物已满!",  CHAR_COLORYELLOW);
							break;
						}
					
						enemynum = ENEMY_getEnemyNum();
						for( i = 0; i < enemynum; i ++ ) {
							if( ENEMY_getInt( i, ENEMY_ID) == PlayerQuestion.value) {
									break;
								}
						}
					
						if( i == enemynum ){
							CHAR_talkToCli( charaindex, -1, "很抱歉,该宠物不存在",  CHAR_COLORYELLOW);
							break;
						}
					
						petindex = ENEMY_createPetFromEnemyIndex( charaindex, i);
						for( i = 0; i < CHAR_MAXPETHAVE; i ++ )	{
							if( CHAR_getCharPet( charaindex, i ) == petindex )
								break;
						}
		
						if( !CHAR_CHECKINDEX( petindex) ){
							CHAR_talkToCli( charaindex, -1, "很抱歉,该宠物不存在",  CHAR_COLORYELLOW);
						}
						snprintf( msgbuf,sizeof( msgbuf), "恭喜你获得宠物:%s。", CHAR_getChar(petindex,CHAR_NAME));
						CHAR_talkToCli( charaindex, -1, msgbuf,  CHAR_COLORYELLOW);
						CHAR_setInt(petindex,CHAR_VARIABLEAI,10000);
						for(j = 0; j < CHAR_MAXPETHAVE; j++){
							petindex = CHAR_getCharPet(charaindex, j);
							if( !CHAR_CHECKINDEX( petindex) )
								continue;
							CHAR_complianceParameter( petindex );
							snprintf( msgbuf, sizeof( msgbuf ), "K%d", j );
							CHAR_sendStatusString( charaindex, msgbuf );
							snprintf( msgbuf, sizeof( msgbuf ), "W%d", j );
							CHAR_sendStatusString( charaindex, msgbuf );
						}
					}
					break;
				case 3:
					CHAR_setInt(charaindex,CHAR_FAME,CHAR_getInt(charaindex,CHAR_FAME) + PlayerQuestion.value * 100);
/*
					char buf[64];
					sprintf(buf,"%d", CHAR_getInt(charaindex,CHAR_FAME));
					saacproto_ACFixFMData_send(acfd,
							CHAR_getChar(charaindex,CHAR_FMNAME),CHAR_getInt(charaindex,CHAR_FMINDEX),CHAR_getWorkInt(charaindex,CHAR_WORKFMINDEXI),
							FM_FIX_FMFEED,buf,"",CHAR_getWorkInt(charaindex,CHAR_WORKFMCHARINDEX),CONNECT_getFdid(getfdFromCharaIndex(charaindex)));
*/
					sprintf( token, "恭喜你获得个人声望：%d", PlayerQuestion.value);
					CHAR_talkToCli(charaindex,-1,token,CHAR_COLORRED);
					break;
				case 4:
					CHAR_setInt(charaindex,CHAR_AMPOINT,CHAR_getInt(charaindex,CHAR_AMPOINT) + PlayerQuestion.value);
#ifdef _AMPOINT_LOG
					LogAmPoint(CHAR_getChar( charaindex, CHAR_NAME ),
									         	CHAR_getChar( charaindex, CHAR_CDKEY ),
									          PlayerQuestion.value,
									   				CHAR_getInt( charaindex, CHAR_AMPOINT ),
									         	"(在线问题)",
									         	CHAR_getInt( charaindex,CHAR_FLOOR),
									         	CHAR_getInt( charaindex,CHAR_X ),
									         	CHAR_getInt( charaindex,CHAR_Y ));
#endif
					sprintf( token, "恭喜你获得个人积分：%d", PlayerQuestion.value);
					CHAR_talkToCli(charaindex,-1,token,CHAR_COLORRED);
					break;
			}
			{
				char type[5][32]={"石币","道具编号为","宠物编号为","声望","积分"};
				
				int i;
				int playernum = CHAR_getPlayerMaxNum();
			  char token1[128];
			  char token2[128];
			  sprintf(token1, "恭喜玩家 %s 回答正确,答案是:%s", CHAR_getChar(charaindex, CHAR_NAME), PlayerQuestion.result);
				sprintf(token2, "让我们恭喜他获得%s %d", type[PlayerQuestion.type], PlayerQuestion.value);

				for( i = 0 ; i < playernum ; i++) {
				  if( CHAR_getCharUse(i) != FALSE ){
				  	CHAR_talkToCli( i, -1, token1, CHAR_COLORRED);
				  	CHAR_talkToCli( i, -1, token2, CHAR_COLORRED);
					}
				}
			}
			strcpy(PlayerQuestion.question,"");
			strcpy(PlayerQuestion.result,"");
		}else{
			CHAR_talkToCli(charaindex,-1,"很抱歉,你回答不正确!",CHAR_COLORRED);
			return FALSE;
		}
	}
	return FALSE;
}

#endif


