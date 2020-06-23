#include <string.h>
#include "version.h"
#include "longzoro/luckstar.h"
#include "char_base.h"
#include "char.h"
#include "autil.h"
#include "enemy.h"
#include "item.h"

#ifdef _LUCK_STAR

Luck_Star_t luckstar[MAXLUCKSTAR];

static int lucknum=0;
BOOL LuckStar_init()
{
	FILE* fp;
	int i;
#ifdef _CRYPTO_DATA		
	BOOL crypto = FALSE;
	fp = fopen( "data/luckstar.txt.allblues", "r");
	if( fp != NULL ){
		crypto = TRUE;
	}else
#endif
{
	fp = fopen("data/luckstar.txt", "r");
}
	if (fp == NULL)
	{
		print("无法打开文件\n");
		return FALSE;
	}
	memset( &luckstar, 0, sizeof( luckstar ) );
	lucknum = 0;
	for(i=0;i<MAXLUCKSTAR;i++){
		char line[64], buf[16];
		if (fgets(line, sizeof(line), fp) == NULL){
			continue;
		}

#ifdef _CRYPTO_DATA
		if(crypto==TRUE){
			DecryptKey(line);
		}
#endif

		if( line[0] == '#' )continue;
    if( line[0] == '\n' )continue;

		chop(line);

		if(getStringFromIndexWithDelim(line,",", 1, buf, sizeof(buf))==TRUE){
			luckstar[lucknum].pet=atoi(buf);
		}else{
			luckstar[lucknum].pet=-1;
		}
		if(getStringFromIndexWithDelim(line,",", 2, buf, sizeof(buf))==TRUE){
			luckstar[lucknum].item=atoi(buf);
		}else{
			luckstar[lucknum].item=-1;
		}
/*
		if(getStringFromIndexWithDelim(line,",", 3, buf, sizeof(buf))==TRUE){
			luckstar[lucknum].gold=atoi(buf);
		}else{
			luckstar[lucknum].gold=0;
		}
		if(getStringFromIndexWithDelim(line,",", 4, buf, sizeof(buf))==TRUE){
			luckstar[lucknum].vippoint=atoi(buf);
		}else{
			luckstar[lucknum].vippoint=0;
		}
*/
		lucknum++;
	}
	lucknum--;
	fclose(fp);
	return TRUE;
}

void LuckStar()
{
	if(lucknum==0)return;
	if(getLuckStarChances()<0)return;
  if((rand() % 100) <= getLuckStarChances()){
		int k;
		char token[256]="";
		char strPet[128]="";
		char strItem[128]="";
//		char strGold[128]="";
//		char strVippoint[128]="";
		int luckid = rand() % lucknum;
		int charaindex;
		for(k=0;k<3;k++){
			charaindex = rand() % CHAR_getPlayerMaxNum();
			if(CHAR_CHECKINDEX(charaindex) == TRUE){
				if(luckstar[luckid].pet > 0){
					int	ret;
					int	enemynum;
					int	i;
					enemynum = ENEMY_getEnemyNum();
					for( i = 0; i < enemynum; i ++ ){
						if( ENEMY_getInt( i, ENEMY_ID) == luckstar[luckid].pet) {
							break;
						}
					}
					if( i != enemynum ){
						ret = ENEMY_createPetFromEnemyIndex(charaindex, i);
						if( CHAR_CHECKINDEX( ret)){
							snprintf( token,sizeof( token), "恭喜你，获得本次幸运赠送宠物 %s ！",ENEMY_getChar( i, ENEMY_NAME));
							CHAR_talkToCli( charaindex, -1, token,  CHAR_COLORGREEN);
							int j;
							for( j = 0; j < CHAR_MAXPETHAVE; j ++ ){
								if( CHAR_getCharPet( charaindex, j ) == ret )break;
							}
							if( j != CHAR_MAXPETHAVE ){
								if( CHAR_CHECKINDEX( ret ) == TRUE ){
									CHAR_setMaxExpFromLevel( ret, CHAR_getInt( ret, CHAR_LV ));
									CHAR_setInt(ret, CHAR_VARIABLEAI, 10000);
								}
	
								CHAR_setInt( ret, CHAR_HP, CHAR_getWorkInt( ret, CHAR_WORKMAXHP ));
								CHAR_complianceParameter( ret );
								snprintf( token, sizeof( token ), "K%d", j );
								CHAR_sendStatusString( charaindex, token );
								snprintf( token, sizeof( token ), "W%d", j );
								CHAR_sendStatusString( charaindex, token );
							}
						}else{
							CHAR_talkToCli( charaindex, -1, "很抱歉，由于你身上宠物已满，已错过这次幸运赠送宠物！",  CHAR_COLORYELLOW);
						}
						snprintf( strPet,sizeof( strPet), "宠物：%s", ENEMY_getChar( i, ENEMY_NAME));
					}else{
						snprintf( strPet,sizeof( strPet), "宠物：编号 %d 不存在", luckstar[luckid].pet);
					}
				}
				if(luckstar[luckid].item > 0){
					int itemindex = ITEM_makeItemAndRegist( luckstar[luckid].item );
					if( ITEM_CHECKINDEX( itemindex) ){
						int emptyitemindexinchara = CHAR_findEmptyItemBox( charaindex );
						if( emptyitemindexinchara >= 0 ){
							if( itemindex != -1 ){
							  CHAR_setItemIndex( charaindex, emptyitemindexinchara, itemindex );
							  ITEM_setWorkInt(itemindex, ITEM_WORKOBJINDEX,-1);
							  ITEM_setWorkInt(itemindex, ITEM_WORKCHARAINDEX,charaindex);
							  CHAR_sendItemDataOne( charaindex, emptyitemindexinchara);
	
								snprintf( token, sizeof( token), "恭喜你，获得本次幸运赠送物品 %s", ITEM_getChar( itemindex, ITEM_NAME));
								CHAR_talkToCli( charaindex, -1,token, CHAR_COLORGREEN);
							}
						}else{
							CHAR_talkToCli( charaindex, -1, "很抱歉，由于你身上物品已满，已错过这次幸运赠送物品！",  CHAR_COLORYELLOW);
						}
						snprintf( strItem,sizeof( strItem), "物品：%s", ITEM_getChar( itemindex, ITEM_NAME));
					}else{
						snprintf( strPet,sizeof( strPet), "物品：编号 %d 不存在", luckstar[luckid].item);
					}
				}
/*
				if(luckstar[luckid].gold > 0){
					CHAR_setInt( charaindex , CHAR_GOLD , CHAR_getInt( charaindex , CHAR_GOLD ) + luckstar[luckid].gold);
					CHAR_complianceParameter( charaindex );
					CHAR_send_P_StatusString( charaindex , CHAR_P_STRING_GOLD);
					snprintf( strGold,sizeof( strGold), "石币：%d", luckstar[luckid].gold);
					sprintf( token, "恭喜你，获得本次幸运赠送 %d 石币！", luckstar[luckid].gold);
					CHAR_talkToCli( charaindex, -1,token, CHAR_COLORPURPLE );
				}
				if(luckstar[luckid].vippoint > 0){
					CHAR_setInt( charaindex , CHAR_AMPOINT , CHAR_getInt( charaindex , CHAR_AMPOINT ) + luckstar[luckid].vippoint);
					snprintf( strVippoint,sizeof( strVippoint), "积分点：%d", luckstar[luckid].vippoint);
					sprintf( token, "恭喜你，获得本次幸运赠送 %d 积分点！", luckstar[luckid].vippoint);
					CHAR_talkToCli( charaindex, -1,token, CHAR_COLORPURPLE );
				}
*/
				break;
			}
		}
		
		int playernum = CHAR_getPlayerMaxNum();

		if(k < 3){
			sprintf(token, "让我们一起祝贺 %s 成为本次幸运星玩家~获得以下奖品：", CHAR_getChar( charaindex, CHAR_NAME));
		}else{
			sprintf(token, "很遗憾，本次幸运星未产生~祝愿您成为下一次幸运星获得者！");
		}

		for(k=0;k<playernum;k++){
			if(CHAR_CHECKINDEX(k) == FALSE) continue;
			CHAR_talkToCli( k, -1, token, CHAR_COLORPURPLE );
			if(strlen(strPet) > 0){
				CHAR_talkToCli( k, -1, strPet, CHAR_COLORPURPLE );
			}
			if(strlen(strItem) > 0){
				CHAR_talkToCli( k, -1, strItem, CHAR_COLORPURPLE );
			}
/*
			if(strlen(strGold) > 0){
				CHAR_talkToCli( k, -1, strGold, CHAR_COLORPURPLE );
			}
			if(strlen(strVippoint)> 0){
				CHAR_talkToCli( k, -1, strVippoint, CHAR_COLORPURPLE );
			}
*/
		}
	}
}
#endif
