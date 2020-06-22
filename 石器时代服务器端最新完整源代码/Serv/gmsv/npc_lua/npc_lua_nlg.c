#include "npc_lua.h"
#include "util.h"
#include "char.h"
#include "char_base.h"
#include "anim_tbl.h"
#include "object.h"
#include "net.h"
#include "npcutil.h"
#include "npc_eventaction.h"
#include "battle.h"
#include "readmap.h"
#include "configfile.h"

#ifdef _JZ_NEWSCRIPT_LUA
extern int StateTable[];

//////////////////////////////////////////////////////////////////////////////
//功能接口
int NPC_Lua_NLG_CheckInFront(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 1);
	CheckIndexNull(_NLL, 2);
	int TM_SurIndex = (int)lua_tointeger(_NLL, 1);
	int TM_DesIndex = (int)lua_tointeger(_NLL, 2);
	int TM_Distance = (int)lua_tointeger(_NLL, 3);

	BOOL TM_Ret = NPC_Util_charIsInFrontOfChar(TM_SurIndex, TM_DesIndex, TM_Distance);
	
	LRetBool(_NLL, TM_Ret);
}

int NPC_Lua_NLG_CheckObj(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	int TM_Floor = (int)lua_tointeger(_NLL, 1);
	int TM_X = (int)lua_tointeger(_NLL, 2);
	int TM_Y = (int)lua_tointeger(_NLL, 3);
	int TM_ObjIndex = -1;
	OBJECT TM_Object = NULL;
	
	for( TM_Object = MAP_getTopObj( TM_Floor, TM_X, TM_Y) ; TM_Object != NULL ; TM_Object = NEXT_OBJECT(TM_Object))
	{
		TM_ObjIndex = GET_OBJINDEX(TM_Object);
		if( !CHECKOBJECTUSE(TM_ObjIndex) )
		{
			continue;
		}
		LRetInt(_NLL, 1);
	}
	LRetInt(_NLL, 0);
}

int NPC_Lua_NLG_CharLook(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_DIR = (int)lua_tointeger(_NLL, 2);
	CHAR_Look(TM_Index, TM_DIR);
	LRetNull(_NLL);
}

int NPC_Lua_NLG_CreateBattle(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, 7);
	CheckIndexNull(_NLL, 1);
	CheckIndexNull(_NLL, 2);
	int TM_Top = lua_gettop(_NLL);
	int TM_i = 0;
	int TM_Flg = 0;

	CREATEENEMY TM_CreateEnemy[BATTLE_ENTRY_MAX];
	int TM_ArLen = 0;

	memset(&TM_CreateEnemy, NULL, sizeof(TM_CreateEnemy));

	int TM_CharIndex = (int)lua_tointeger(_NLL, 1);
	int TM_NpcIndex = (int)lua_tointeger(_NLL, 2);
	char *TM_DoFunc = NULL;

	if(TM_Top >= 3)
	{
		TM_DoFunc = lua_tostring(_NLL, 3);
	}

	if(TM_Top >= 5)
	{
		TM_ArLen = luaL_getn(_NLL, 5);
		TM_ArLen = min(TM_ArLen, BATTLE_ENTRY_MAX);
		for(TM_i = 1; TM_i <= TM_ArLen; TM_i++)
		{
			lua_rawgeti(_NLL, 5, TM_i);
			TM_CreateEnemy[TM_i - 1].BaseLevel = (int)lua_tointeger(_NLL, -1);
			lua_pop(_NLL, 1);
		}
	}
	
	if(TM_Top >= 6)
	{
		TM_ArLen = luaL_getn(_NLL, 6);
		TM_ArLen = min(TM_ArLen, BATTLE_ENTRY_MAX);
		for(TM_i = 1; TM_i <= TM_ArLen; TM_i++)
		{
			lua_rawgeti(_NLL, 6, TM_i);
			TM_CreateEnemy[TM_i - 1].SkillType = (int)lua_tointeger(_NLL, -1);
			lua_pop(_NLL, 1);
		}
	}
	if(TM_Top >= 7)
	{
		TM_Flg = (int)lua_tointeger(_NLL, 7);
	}
//这个要放在最后
	if(TM_Top >= 4)
	{
		TM_ArLen = luaL_getn(_NLL, 4);
		TM_ArLen = min(TM_ArLen, BATTLE_ENTRY_MAX);
		for(TM_i = 1; TM_i <= TM_ArLen; TM_i++)
		{
			lua_rawgeti(_NLL, 4, TM_i);
			TM_CreateEnemy[TM_i - 1].EnemyId = ENEMY_getEnemyArrayFromId((int)lua_tointeger(_NLL, -1));
			lua_pop(_NLL, 1);
		}
	}
//然后再调用创建战斗的函数
	int TM_Ret = NPC_Lua_CreateVsEnemy(_NLL, TM_CharIndex, TM_NpcIndex, TM_DoFunc, &TM_CreateEnemy, TM_ArLen, TM_Flg);
//返回负数表示失败
	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_NLG_DelPet(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, 3);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_PetID = (int)lua_tointeger(_NLL, 2);
	int TM_Num = 1;
	int TM_PetIndex = -1;
	int TM_Ret = 0;
	int TM_Count = 0;

	if(lua_gettop(_NLL) == 3)
	{
		TM_Num = (int)lua_tointeger(_NLL, 3);
	}
	int i = 0;
	for(i = 0; i < CHAR_MAXPETHAVE ; i++)
	{
		TM_PetIndex = CHAR_getCharPet( TM_Index, i);
		
		if(!CHAR_CHECKINDEX(TM_PetIndex))
		{
			continue;
		}
		if(CHAR_getInt( TM_PetIndex, CHAR_PETID) != TM_PetID)
		{
			continue;
		}

		TM_Ret = NPC_DelPet( TM_Index, i);
		if(TM_Ret < 0)
		{
			char TM_MsgBuff[128];
			snprintf(TM_MsgBuff, sizeof(TM_MsgBuff), "删除宠物时产生错误 错误号: %d。", TM_Ret);
			LRetErrInt(_NLL , -1, TM_MsgBuff);
		}

		TM_Count++;
		if(TM_Count == TM_Num)
		{
			break;
		}
	}
	LRetInt(_NLL, TM_Count);
}

int NPC_Lua_NLG_DelHaveIndexPet(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_PetHaveIndex = (int)lua_tointeger(_NLL, 2);
	int TM_Num = 1;
	int TM_PetIndex = -1;
	int TM_Ret = 0;
	
	if(TM_PetHaveIndex<0 || TM_PetHaveIndex>4){
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
		TM_PetIndex = CHAR_getCharPet( TM_Index, TM_PetHaveIndex);
		
		if(!CHAR_CHECKINDEX(TM_PetIndex))
		{
			LRetErrInt(_NLL, -1, "传入的索引是无效的。");
		}

		TM_Ret = NPC_DelPet( TM_Index, TM_PetHaveIndex);
		if(TM_Ret < 0)
		{
			char TM_MsgBuff[128];
			snprintf(TM_MsgBuff, sizeof(TM_MsgBuff), "删除宠物时产生错误 错误号: %d。", TM_Ret);
			LRetErrInt(_NLL , -1, TM_MsgBuff);
		}

	LRetInt(_NLL, 1);
}

int NPC_Lua_NLG_DelItem(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, 3);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_ItemID = (int)lua_tointeger(_NLL, 2);
	int TM_Num = 1;
	
	if(lua_gettop(_NLL) == 3)
	{
		TM_Num = (int)lua_tointeger(_NLL, 3);
	}

	BOOL TM_Ret = NPC_ActionDoPileDelItem(TM_Index, TM_ItemID, TM_Num);
	
	LRetBool(_NLL, TM_Ret);
}

int NPC_Lua_NLG_DelItemByPos(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_ItemPos = (int)lua_tointeger(_NLL, 2);
	int TM_Num = 1;
	int TM_ItemId= -1;
	int TM_ItemIndex = -1;
	int itemMax =  CheckCharMaxItem(TM_Index);
	if( TM_ItemPos < CHAR_STARTITEMARRAY || TM_ItemPos > itemMax -1 ) {
		CHAR_talkToCli( TM_Index, -1, "位置不正确。",  CHAR_COLORWHITE);
		return;
	}
   TM_ItemId = CHAR_getItemIndex(TM_Index, TM_ItemPos);

	if( TM_ItemId == -1 ) {
		LRetBool(_NLL, FALSE);
	}
	TM_ItemIndex = CHAR_getItemIndex(TM_Index,TM_ItemPos);
	char TM_Buff[128];
	snprintf(TM_Buff, sizeof(TM_Buff), "交出 1 个 %s。", ITEM_getChar( TM_ItemIndex, ITEM_NAME));
	CHAR_talkToCli( TM_Index, -1, TM_Buff, CHAR_COLORYELLOW);
	CHAR_DelItem( TM_Index, TM_ItemPos);
	LRetBool(_NLL, TRUE);
}

int NPC_Lua_NLG_DischargeParty(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_index = (int)lua_tointeger(_NLL, 1);

	BOOL TM_Ret = CHAR_DischargeParty( TM_index, 0);

	LRetBool(_NLL, TM_Ret);
}

int NPC_Lua_NLG_GivePet(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, 4);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_EnemyId = (int)lua_tointeger(_NLL, 2);
	int TM_Level = -1;
	int TM_Num = 1;
	int TM_Count = 0;
	int TM_PetIndex = -1;
	int TM_Int = -1;

	if(lua_gettop(_NLL) >= 3)
	{
		TM_Level = (int)lua_tointeger(_NLL, 3);
	}
	
	if(lua_gettop(_NLL) == 4)
	{
		TM_Num = (int)lua_tointeger(_NLL, 4);
	}
	if(TM_Num <= 0)
	{
		LRetErrInt(_NLL, -1, "给予数量不能少于等于0。");
	}
	int i = 0;
	for(i = 0; i < TM_Num; i++)
	{
		TM_Int = NPC_GivePet(TM_Index, TM_Level, TM_EnemyId);
		if(TM_Int < 0)
		{
			break;
		}
		TM_PetIndex = TM_Int;
		TM_Count++;
	}
	if(TM_Count > 0)
	{
		char TM_Buff[128];
		snprintf(TM_Buff, sizeof(TM_Buff), "获得 %d 只 %s。", TM_Count, CHAR_getChar( TM_PetIndex, CHAR_NAME));
		CHAR_talkToCli( TM_Index, -1, TM_Buff, CHAR_COLORYELLOW);
	}
	LRetInt(_NLL, TM_Count);
}

int NPC_Lua_NLG_GiveOnePet(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, 3);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_EnemyId = (int)lua_tointeger(_NLL, 2);
	int TM_Level = -1;
	int TM_PetIndex = -1;


	if(lua_gettop(_NLL) >= 3)
	{
		TM_Level = (int)lua_tointeger(_NLL, 3);
	}
	TM_PetIndex = NPC_GivePet(TM_Index, TM_Level, TM_EnemyId);
	char TM_Buff[128];
	snprintf(TM_Buff, sizeof(TM_Buff), "获得1只 %s。", CHAR_getChar( TM_PetIndex, CHAR_NAME));
	CHAR_talkToCli( TM_Index, -1, TM_Buff, CHAR_COLORYELLOW);
	LRetInt(_NLL, TM_PetIndex);
}


int NPC_Lua_NLG_GiveItem(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, 3);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_ItemId = (int)lua_tointeger(_NLL, 2);
	int TM_Num = 1;
	int TM_ItemIndex = -1;
	int TM_Int = -1;
	int TM_Count = 0;

	if(lua_gettop(_NLL) == 3)
	{
		TM_Num = (int)lua_tointeger(_NLL, 3);
	}
	int i = 0;
	for(i = 0; i < TM_Num; i++)
	{
		TM_Int = NPC_GiveItem( TM_Index, TM_ItemId);
		if(TM_Int < 0)
		{
			break;
		}
		TM_ItemIndex = TM_Int;
		TM_Count++;
	}
	if(TM_Count > 0)
	{
		char TM_Buff[128];
		snprintf(TM_Buff, sizeof(TM_Buff), "获得 %d 个 %s。", TM_Count, ITEM_getChar( TM_ItemIndex, ITEM_NAME));
		CHAR_talkToCli( TM_Index, -1, TM_Buff, CHAR_COLORYELLOW);
	}
	LRetInt(_NLL, TM_Count);
}

int NPC_Lua_NLG_GiveOneItem(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_ItemId = (int)lua_tointeger(_NLL, 2);
	int TM_Num = 1;
	int TM_ItemIndex = -1;

		TM_ItemIndex = NPC_GiveItem( TM_Index, TM_ItemId);
	if(TM_ItemIndex >= 0)
	{
		char TM_Buff[128];
		snprintf(TM_Buff, sizeof(TM_Buff), "获得 1 个 %s。", ITEM_getChar( TM_ItemIndex, ITEM_NAME));
		CHAR_talkToCli( TM_Index, -1, TM_Buff, CHAR_COLORYELLOW);
	}else{
		LRetInt(_NLL, -1);
	}
	LRetInt(_NLL, TM_ItemIndex);
}

int NPC_Lua_NLG_GiveRandItem(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, 3);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	char *TM_ItemIdData = lua_tostring(_NLL, 2);
	int TM_Num = 1;
	int TM_ItemIndex = -1;
	int TM_Int = -1;
	int TM_Type = 0;
	int TM_ItemId = -1;
	if(lua_gettop(_NLL) == 3)
	{
		TM_Type = (int)lua_tointeger(_NLL, 3);
	}
	int TM_ItemIdStart = -1;
	int TM_ItemIdEnd = -1;
	char ItemIdBuf[32];
	if(getStringFromIndexWithDelim(TM_ItemIdData,",", 2, ItemIdBuf, sizeof(ItemIdBuf)) != FALSE){
		int TM_ItemNum = 0;
		while(1){
			if(getStringFromIndexWithDelim(TM_ItemIdData,",", TM_ItemNum+1, ItemIdBuf, sizeof(ItemIdBuf)) == FALSE)
				break;
			TM_ItemNum++;
		}
		TM_ItemNum = RAND(1,TM_ItemNum);
		getStringFromIndexWithDelim(TM_ItemIdData,",", TM_ItemNum, ItemIdBuf, sizeof(ItemIdBuf));
		char ItemIdBuf2[16];
		if( getStringFromIndexWithDelim(ItemIdBuf,"-", 2, ItemIdBuf2, sizeof(ItemIdBuf2)) != FALSE){
			int TM_ItemIdStart,TM_ItemIdEnd;
			getStringFromIndexWithDelim(ItemIdBuf,"-", 1, ItemIdBuf2, sizeof(ItemIdBuf2));
			TM_ItemIdStart = atoi(ItemIdBuf2);
			getStringFromIndexWithDelim(ItemIdBuf,"-", 2, ItemIdBuf2, sizeof(ItemIdBuf2));
			TM_ItemIdEnd = atoi(ItemIdBuf2);
			TM_ItemId = RAND(TM_ItemIdStart,TM_ItemIdEnd);
		}else{
			TM_ItemId = atoi(ItemIdBuf);
		}
	}else if(getStringFromIndexWithDelim(TM_ItemIdData,"-", 2, ItemIdBuf, sizeof(ItemIdBuf)) != FALSE){
		if(getStringFromIndexWithDelim(TM_ItemIdData,"-", 1, ItemIdBuf, sizeof(ItemIdBuf)) != FALSE)
			TM_ItemIdStart = atoi(ItemIdBuf);
		if(getStringFromIndexWithDelim(TM_ItemIdData,"-", 2, ItemIdBuf, sizeof(ItemIdBuf)) != FALSE)
			TM_ItemIdEnd = atoi(ItemIdBuf);
		if(TM_ItemIdStart<=TM_ItemIdEnd){
			TM_ItemId = RAND(TM_ItemIdStart,TM_ItemIdEnd);
		}
	}else{
		TM_ItemId = atoi(TM_ItemIdData);
	}
	if(TM_ItemId>-1){
		TM_Int = NPC_GiveItem( TM_Index, TM_ItemId);
	}
	if(TM_Int>=0){
		char TM_Buff[256];
		snprintf(TM_Buff, sizeof(TM_Buff), "获得 %s。", ITEM_getChar( TM_Int, ITEM_NAME));
		CHAR_talkToCli( TM_Index, -1, TM_Buff, CHAR_COLORYELLOW);
/*		if(TM_Type>0){
			time_t timep;
			struct tm *p;
			time(&timep);
			p=localtime(&timep);
			timep = mktime(p);
			TM_Type = timep + TM_Type;
			ITEM_setInt(TM_Int,ITEM_TIME,TM_Type);
			sprintf(TM_Buff,"[限时]%s",ITEM_getChar(TM_Int,ITEM_NAME));
			ITEM_setChar(TM_Int,ITEM_NAME,TM_Buff);
			ITEM_setChar(TM_Int,ITEM_SECRETNAME,TM_Buff);
			CHAR_sendStatusString(TM_Index,"I");
//			timep = TM_Type;
//			p=localtime(&timep);
//			int year=1900+p->tm_year;
//			int mon=1+p->tm_mon;
//			int date=p->tm_mday;
//			int hour=p->tm_hour;
//			int min=p->tm_min;
//			int sec=p->tm_sec;
//			char monbuf[5],datebuf[5],hourbuf[5],minbuf[5],secbuf[5];
//			if(mon<10) sprintf(monbuf,"0%d",mon);
//			else sprintf(monbuf,"%d",mon);
//			if(date<10) sprintf(datebuf,"0%d",date);
//			else sprintf(datebuf,"%d",date);
//			if(hour<10) sprintf(hourbuf,"0%d",hour);
//			else sprintf(hourbuf,"%d",hour);
//			if(min<10) sprintf(minbuf,"0%d",min);
//			else sprintf(minbuf,"%d",min);
//			if(sec<10) sprintf(secbuf,"0%d",sec);
//			else sprintf(secbuf,"%d",sec);
//			sprintf(TM_Buff,"有效期:%d.%s.%s/%s:%s:%s  %s",year,monbuf,datebuf,hourbuf,minbuf,secbuf,ITEM_getChar(TM_Int,ITEM_EFFECTSTRING));
//			ITEM_setChar(TM_Int,ITEM_EFFECTSTRING,TM_Buff);
		}*/
	}
	LRetInt(_NLL, TM_Int);
}

int NPC_Lua_NLG_ShowWindowTalked(lua_State *_NLL)
{
	CheckEx2(_NLL, 5, 6);
	CheckIndexNull(_NLL, 1);
	int TM_Top = lua_gettop(_NLL);
	int TM_index = (int)lua_tointeger(_NLL, 1);
	int TM_windowtype = (int)lua_tointeger(_NLL, 2);
	int TM_buttontype = (int)lua_tointeger(_NLL, 3);
	int TM_seqno = (int)lua_tointeger(_NLL, 4);
	char *TM_data = lua_tostring(_NLL, 5);

	if(TM_data == NULL)
	{
		LRetErrInt(_NLL , -1, "对话框内容不能为nil");
	}

	int TM_MeIndex = -1;
	int TM_fd = getfdFromCharaIndex(TM_index);

	if(TM_Top == 6)
	{
		CheckIndexNull(_NLL, 6);
		TM_MeIndex = (int)lua_tointeger(_NLL, 6);
	}

	lssproto_WN_send(TM_fd, TM_windowtype, TM_buttontype, TM_seqno, CHAR_getWorkInt( TM_MeIndex, CHAR_WORKOBJINDEX), TM_data);
	LRetInt(_NLL, 0);
}

int NPC_Lua_NLG_SetAction(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Action = (int)lua_tointeger(_NLL, 2);

	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	CHAR_sendWatchEvent( CHAR_getWorkInt( TM_Index, CHAR_WORKOBJINDEX),	TM_Action, NULL, 0,TRUE);
	CHAR_setWorkInt( TM_Index, CHAR_WORKACTION, TM_Action);

	LRetInt(_NLL, 0);
}

int NPC_Lua_NLG_TalkToCli(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, 5);
	CheckIndexNull(_NLL, 1);
	int TM_Top = lua_gettop(_NLL);
	int TM_toindex = (int)lua_tointeger(_NLL, 1);
	char *TM_Msg = lua_tostring(_NLL, 2);
	if(TM_Msg == NULL)
	{
		LRetErrInt(_NLL , -1, "说话内容不能为nil。");
	}

	int TM_color = CHAR_COLORWHITE;
	int TM_fontsize = 0;
	int TM_Talkindex = -1;

	if(TM_Top >= 3)
	{
		TM_color = (int)lua_tointeger(_NLL, 3);
	}
	if(TM_Top >= 4)
	{
		TM_fontsize = (int)lua_tointeger(_NLL, 4);
	}
	if(TM_Top == 5)
	{
		CheckIndexNull(_NLL, 5);
		TM_Talkindex = (int)lua_tointeger(_NLL, 5);
	}
	BOOL TM_Ret = FALSE;

	if(TM_toindex == -1)
	{
		//对全服务器说话
		int TM_playernum = CHAR_getPlayerMaxNum();
		int i = 0;

		for( i = 0 ; i < TM_playernum ; i++)
		{
			if( CHAR_getCharUse(i) != FALSE )
			{
				#ifdef _FONT_SIZE
				TM_Ret |= CHAR_talkToCliExt( i, TM_Talkindex, TM_Msg, TM_color, TM_fontsize);
				#else
				TM_Ret |= CHAR_talkToCli( i, TM_Talkindex, TM_Msg, TM_color);
				#endif
			}
		}
	}else
	{
		//对指定玩家说话
	#ifdef _FONT_SIZE
		TM_Ret = CHAR_talkToCliExt(TM_toindex, TM_Talkindex, TM_Msg, TM_color, TM_fontsize);
	#else
		TM_Ret = CHAR_talkToCli(TM_toindex, TM_Talkindex, TM_Msg, TM_color);
	#endif
	}
	if(TM_Ret == TRUE)
	{
		LRetInt(_NLL, 0);
	}else
	{
		LRetErrInt(_NLL , -2, "未知原因导致发送说话失败。");
	}
}

int NPC_Lua_NLG_TalkToFloor(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, 5);
	
	int TM_Top = lua_gettop(_NLL);
	int TM_floor = (int)lua_tointeger(_NLL, 1);
	char *TM_Msg = lua_tostring(_NLL, 2);
	if(TM_Msg == NULL)
	{
		LRetErrInt(_NLL , -1, "说话内容不能为nil。");
	}

	int TM_color = CHAR_COLORWHITE;
	int TM_fontsize = 0;
	int TM_Talkindex = -1;

	if(TM_Top >= 3)
	{
		TM_color = (int)lua_tointeger(_NLL, 3);
	}
	if(TM_Top >= 4)
	{
		TM_fontsize = (int)lua_tointeger(_NLL, 4);
	}
	if(TM_Top == 5)
	{
		CheckIndexNull(_NLL, 5);
		TM_Talkindex = (int)lua_tointeger(_NLL, 5);
	}
	//指定地图说话
	BOOL TM_Ret = FALSE;
	int TM_playernum = CHAR_getPlayerMaxNum();
	int i = 0;

	for( i = 0 ; i < TM_playernum ; i++)
	{
		if( CHAR_getCharUse(i) != FALSE )
		{
			if (CHAR_getInt(i, CHAR_FLOOR) == TM_floor)
			{
			#ifdef _FONT_SIZE
				TM_Ret |= CHAR_talkToCliExt( i, TM_Talkindex, TM_Msg, TM_color, TM_fontsize);
			#else
				TM_Ret |= CHAR_talkToCli( i, TM_Talkindex, TM_Msg, TM_color);
			#endif
				
			}
		}
	}

	if(TM_Ret == TRUE)
	{
		LRetInt(_NLL, 0);
	}else
	{
		LRetErrInt(_NLL , -2, "未知原因导致发送说话失败。");
	}
}

int NPC_Lua_NLG_UpChar(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	CHAR_complianceParameter( TM_Index );
	
	if(CHAR_getInt(TM_Index,CHAR_WHICHTYPE)==CHAR_TYPEPET){
		int playindex = CHAR_getWorkInt(TM_Index,CHAR_WORKPLAYERINDEX);
		if(!CHAR_CHECKINDEX(playindex)){
			LRetErrInt(_NLL, -1, "传入的索引是无效的。");
		}else{
			int i;
			for( i = 0; i < CHAR_MAXPETHAVE; i ++ ) {
				int workindex = CHAR_getCharPet( playindex, i );
				if( workindex == TM_Index ){
					CHAR_send_K_StatusString( playindex, i, CHAR_K_STRING_BASEIMAGENUMBER|
																									CHAR_K_STRING_HP|
																									CHAR_K_STRING_MAXHP|
																									CHAR_K_STRING_MP|
																									CHAR_K_STRING_MAXMP|
																									CHAR_K_STRING_EXP|
																									CHAR_K_STRING_NEXTEXP|
																									CHAR_K_STRING_LV|
																									CHAR_K_STRING_ATK|
																									CHAR_K_STRING_DEF|
																									CHAR_K_STRING_QUICK|
																									CHAR_K_STRING_AI|
																									CHAR_K_STRING_EARTH|
																									CHAR_K_STRING_WATER|
																									CHAR_K_STRING_FIRE|
																									CHAR_K_STRING_WIND|
																									CHAR_K_STRING_SLOT|
																									CHAR_K_STRING_CHANGENAMEFLG|
																									CHAR_K_STRING_NAME|
																									CHAR_K_STRING_USERPETNAME);
					char msgbuf[128];
					snprintf( msgbuf, sizeof( msgbuf ), "W%d",i);
					CHAR_sendStatusString( playindex, msgbuf );
					CHAR_sendStatusString( playindex, "P");
					break;
				}
			}
		}
	}else if(CHAR_getInt(TM_Index,CHAR_WHICHTYPE)==CHAR_TYPEPLAYER){
		CHAR_send_P_StatusString( TM_Index, CHAR_P_STRING_GOLD|CHAR_P_STRING_CHARSINGMODE );
	}

	if(CHAR_getFlg(TM_Index,	CHAR_ISVISIBLE) == FALSE)
	{
		int TM_objindex = CHAR_getWorkInt(TM_Index,CHAR_WORKOBJINDEX);

		CHAR_sendCDArroundChar
		(
			OBJECT_getFloor(TM_objindex),
			OBJECT_getX(TM_objindex),
			OBJECT_getY(TM_objindex),
			TM_objindex
		);
	}else
	{
		CHAR_sendCToArroundCharacter( CHAR_getWorkInt( TM_Index , CHAR_WORKOBJINDEX ));
	}
	if(CHAR_getWorkInt( TM_Index ,CHAR_WORKPARTYMODE)==CHAR_PARTY_LEADER)
		CHAR_sendLeader( CHAR_getWorkInt( TM_Index, CHAR_WORKOBJINDEX), 1);
	LRetNull(_NLL);
}

int NPC_Lua_NLG_Warp(lua_State *_NLL)
{
	CheckEx(_NLL, 4);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Floor = (int)lua_tointeger(_NLL, 2);
	int TM_X = (int)lua_tointeger(_NLL, 3);
	int TM_Y = (int)lua_tointeger(_NLL, 4);

	BOOL TM_Ret = CHAR_warpToSpecificPoint(TM_Index, TM_Floor, TM_X, TM_Y);
	
	LRetBool(_NLL, TM_Ret);
}

int NPC_Lua_NLG_WalkMove(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_DIR = (int)lua_tointeger(_NLL, 2);

	POINT	start, end;
	
	end.x = CHAR_getInt( TM_Index, CHAR_X);
	end.y = CHAR_getInt( TM_Index, CHAR_Y);

	int TM_Ret = (int)CHAR_walk(TM_Index, TM_DIR, 0);

	if( TM_Ret == CHAR_WALKSUCCESSED )
	{
		int	i;
		int	mefl=CHAR_getInt( TM_Index, CHAR_FLOOR);
		int toindex = -1;
		int fl = 0;
		int xx = 0;
		int yy = 0;
		int	parent_dir;
	
		for( i = 1; i < getPartyNum(TM_Index); i ++ )
		{
			toindex = CHAR_getWorkInt( TM_Index, i + CHAR_WORKPARTYINDEX1);
			fl = CHAR_getInt( toindex, CHAR_FLOOR);
			xx = CHAR_getInt( toindex, CHAR_X);
			yy = CHAR_getInt( toindex, CHAR_Y);

			if( CHAR_CHECKINDEX(toindex) && (mefl==fl) && (abs(xx-end.x)+abs(yy-end.y)<10) )
			{

				start.x = xx;
				start.y = yy;
				parent_dir = NPC_Util_getDirFromTwoPoint( &start,&end );

				end.x = start.x;
				end.y = start.y;

				if( parent_dir != -1 )
				{
					CHAR_walk( toindex, parent_dir, 0);
				}
			}
		}
	}
	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_NLG_WatchEntry(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	CheckIndexNull(_NLL, 2);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_ToIndex = (int)lua_tointeger(_NLL, 2);
	if(	BATTLE_WatchEntry(TM_Index, TM_ToIndex) == 0)
	{
		LRetInt(_NLL, 0);
	}else
	{
		LRetInt(_NLL, -1);
	}
}

int NPC_Lua_Char_GetOnLinePlayer(lua_State *_NLL)
{
	CheckEx(_NLL, 0);
	LRetInt(_NLL, abs(StateTable[LOGIN]));
}

int NPC_Lua_NLG_UpStateBySecond(lua_State *_NLL)
{
	CheckEx2(_NLL, 1, 3);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	char TM_Buff[128];
	int TM_fd = getfdFromCharaIndex(TM_Index);
#ifdef _SAF_MAKE
	snprintf(TM_Buff, sizeof(TM_Buff), "FAME|%d|%d|%d", CHAR_getInt(TM_Index,CHAR_FAME)/100, (int)lua_tointeger(_NLL, 2), (int)lua_tointeger(_NLL, 3));
#else
	snprintf(TM_Buff, sizeof(TM_Buff), "FAME|%d", CHAR_getInt(TM_Index,CHAR_FAME)/100);
#endif
	lssproto_S2_send(TM_fd,TM_Buff);
	LRetInt(_NLL, 0);
}

int NPC_Lua_NLG_UpStateByThird(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Flg = (int)lua_tointeger(_NLL, 2);
	BOOL TM_Ret = CHAR_send_P_StatusString(TM_Index, TM_Flg);
	LRetBool(_NLL, TM_Ret);
}

int NPC_Lua_NLG_Update_Party(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Flg = (int)lua_tointeger(_NLL, 2);
	CHAR_PartyUpdate(TM_Index, TM_Flg);
	LRetNull(_NLL);
}

int NPC_Lua_NLG_CreateBattlePvP(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	CheckIndexNull(_NLL, 2);
	int TM_Index1 = (int)lua_tointeger(_NLL, 1);
	int TM_Index2 = (int)lua_tointeger(_NLL, 2);
	int TM_Ret = -1;
	int TM_BattleIndex = -1;

	if( (CHAR_getInt( TM_Index2, CHAR_WHICHTYPE) != CHAR_TYPEPLAYER) || (CHAR_getInt( TM_Index1, CHAR_WHICHTYPE) != CHAR_TYPEPLAYER)){
		LRetErrInt(_NLL, -1, "对象类型不是玩家");
	}
	if( (CHAR_getWorkInt( TM_Index2, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE) || (CHAR_getWorkInt( TM_Index1, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE))
	{
		LRetErrInt(_NLL, -2, "对象正在对战中");
	}
	if( CHAR_getWorkInt( TM_Index2, CHAR_WORKPARTYMODE ) == CHAR_PARTY_CLIENT )
	{
		int TM_Index = CHAR_getWorkInt( TM_Index2, CHAR_WORKPARTYINDEX1 );

		if( TM_Index < 0 )
		{
			LRetErrInt(_NLL, -3, "对象的队伍异常");
		}
	}
	if( CHAR_getWorkInt( TM_Index1, CHAR_WORKPARTYMODE ) == CHAR_PARTY_CLIENT )
	{
		int TM_Index = CHAR_getWorkInt( TM_Index1, CHAR_WORKPARTYINDEX1 );

		if( TM_Index < 0 )
		{
			LRetErrInt(_NLL, -3, "对象的队伍异常");
		}
	}
	TM_Ret = NPC_Lua_CreateVsPlayer(&TM_BattleIndex, TM_Index1, TM_Index2);
	if(TM_Ret != 0)
	{
		char TM_Buff[64];
		snprintf(TM_Buff, sizeof(TM_Buff), "遭遇失败！错误号:%d", TM_Ret);
		LRetErrInt(_NLL, -4, TM_Buff);
	}
	LRetInt(_NLL, TM_BattleIndex);
}

int NPC_Lua_NLG_SearchWatchBattleRandIndex(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Floor = (int)lua_tointeger(_NLL, 2);
	int TM_Ret = -1;

	if( (CHAR_getInt( TM_Index, CHAR_WHICHTYPE) != CHAR_TYPEPLAYER) ){
		LRetErrInt(_NLL, -1, "对象类型不是玩家");
	}
	if( (CHAR_getWorkInt( TM_Index, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE) )
	{
		LRetErrInt(_NLL, -2, "对象正在对战中");
	}
	if( CHAR_getWorkInt( TM_Index, CHAR_WORKPARTYMODE ) == CHAR_PARTY_CLIENT )
	{
		int TM_Index2 = CHAR_getWorkInt( TM_Index, CHAR_WORKPARTYINDEX1 );

		if( TM_Index2 < 0 )
		{
			LRetErrInt(_NLL, -3, "对象的队伍异常");
		}
	}
	TM_Ret = SearchFmWarRandIndex(TM_Index, TM_Floor);
	if(TM_Ret != 0)
	{
		char TM_Buff[64];
		snprintf(TM_Buff, sizeof(TM_Buff), "无战斗可看！错误号:%d", TM_Floor);
		LRetErrInt(_NLL, -4, TM_Buff);
	}
	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_NLG_GetMaxPlayNum(lua_State *_NLL)
{
	int TM_playernum = CHAR_getPlayerMaxNum();
	LRetInt(_NLL, TM_playernum);
}

int NPC_Lua_NLG_CheckPlayIndex(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	LRetInt(_NLL, TM_Index);
}

int NPC_Lua_NLG_Save(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	CHAR_charSaveFromConnect(TM_Index, FALSE);
	LRetInt(_NLL, TM_Index);
}

//////////////// add by Mo.
int NPC_Lua_NLG_SetExp(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	int TM_Exp = (int)lua_tointeger(_NLL, 1);
	if( TM_Exp <= 0 )
	{
		LRetErrInt(_NLL, -3, "经验参数错误");
	}
	setBattleexp(TM_Exp);
	LRetNull(_NLL);
}

int NPC_Lua_NLG_GetExp(lua_State *_NLL)
{
	int ret = getBattleexp();
	LRetInt(_NLL, ret);
}

/*
int NPC_Lua_NLG_SetPetTransRange(lua_State *_NLL)
{
	CheckEx(_NLL, 3);

	int TM_X = (int)lua_tointeger(_NLL, 1);
	int TM_Y = (int)lua_tointeger(_NLL, 2);
	int TM_Z = (int)lua_tointeger(_NLL, 3);
	setPetTransRangeX(TM_X);
	setPetTransRangeY(TM_Y);
	setPetTransRangeZ(TM_Z);
	LRetNull(_NLL);
}


int NPC_Lua_NLG_GetPetTransRange(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	if (TM_Index == 1){
		int ret = getPetTransRangeX();
		LRetInt(_NLL, ret);
	}
	if (TM_Index == 2){
		int ret = getPetTransRangeY();
		LRetInt(_NLL, ret);
	}
	if (TM_Index == 3){
		int ret = getPetTransRangeZ();
		LRetInt(_NLL, ret);
	}
		LRetErrInt(_NLL, -1, "传入的参数是无效的。");
}
*/

int NPC_Lua_NLG_InputCard(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	
	int TM_Index1 = (int)lua_tointeger(_NLL, 1);
	ITEM_OnlineCost(TM_Index1, -1, -1);
	LRetNull(_NLL);
}

int NPC_Lua_NLG_UpItem(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	
	int charaindex = (int)lua_tointeger(_NLL,1);
	if( !CHAR_CHECKINDEX( charaindex)){
		return 0;
	}
	int MAX = 27;
	int i= 0;
	for ( i = 0; i <= MAX; i++){
		CHAR_sendItemDataOne( charaindex, i);
	}
	return 0;
}

int NPC_Lua_NLG_GivePetOne(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, 4);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_EnemyId = (int)lua_tointeger(_NLL, 2);
	int TM_Level = 1;
	int TM_PetIndex = -1;
	int TM_Int = -1;

	if(lua_gettop(_NLL) >= 3)
	{
		TM_Level = (int)lua_tointeger(_NLL, 3);
	}
	
	TM_Int = NPC_GivePet(TM_Index, TM_Level, TM_EnemyId);
	if(TM_Int >= 0)
	{
		TM_PetIndex = TM_Int;
		char TM_Buff[128];
		snprintf(TM_Buff, sizeof(TM_Buff), "获得 1 只 %s。", CHAR_getChar( TM_PetIndex, CHAR_NAME));
		CHAR_talkToCli( TM_Index, -1, TM_Buff, CHAR_COLORYELLOW);
	}
	LRetInt(_NLL, TM_PetIndex);
}

int NPC_Lua_NLG_GiveItemOne(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, 3);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_ItemId = (int)lua_tointeger(_NLL, 2);
	int TM_ItemIndex = -1;
	int TM_Int = -1;
	
	TM_Int = NPC_GiveItem( TM_Index, TM_ItemId);
	
	if(TM_Int >= 0)
	{
		TM_ItemIndex = TM_Int;
		char TM_Buff[128];
		snprintf(TM_Buff, sizeof(TM_Buff), "获得 1 个 %s。", ITEM_getChar( TM_ItemIndex, ITEM_NAME));
		CHAR_talkToCli( TM_Index, -1, TM_Buff, CHAR_COLORYELLOW);
	}
	LRetInt(_NLL, TM_ItemIndex);
}

int NPC_Lua_NLG_DelItemByIndex(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	CheckIndexNull(_NLL, 2);
	int charaindex = (int)lua_tointeger(_NLL, 1);
	if( CHAR_CHECKINDEX( charaindex ) == FALSE ){
		LRetInt(_NLL, -1);
		return -1;
	}
	int haveitemindex = (int)lua_tointeger(_NLL, 2);
	int itemindex = CHAR_getItemIndex( charaindex, haveitemindex);
	if(!ITEM_CHECKINDEX(itemindex)) {
		LRetInt(_NLL, -1);
		return -1;
	}
	CHAR_DelItem( charaindex, haveitemindex);
	LRetInt(_NLL, 0);
}

int NPC_Lua_NLG_FindPet(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, 3);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_PetID = (int)lua_tointeger(_NLL, 2);
	int TM_Count = 0;

	int i = 0;
	for(i = 0; i < CHAR_MAXPETHAVE ; i++)
	{
		if (CHAR_getCharPet( TM_Index, i) == -1) TM_Count++;
	}
	LRetInt(_NLL, TM_Count);
}

int NPC_Lua_NLG_GetMap(lua_State *_NLL)
{
	CheckEx2(_NLL, 1, 3);
	CheckIndexNull(_NLL, 1);

	int TM_Index = (int)lua_tointeger(_NLL, 1);

	BOOL TM_Ret = CHAR_CHAT_GetMap( TM_Index, "");

	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_NLG_PetUp(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, 3);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_PetIndex = (int)lua_tointeger(_NLL, 2);
	int TM_LV = 1;

	if(lua_gettop(_NLL) == 3)
	{
		TM_LV = (int)lua_tointeger(_NLL, 3);
	}

	BOOL TM_Ret = NPC_PetUp(TM_Index, TM_LV, TM_PetIndex);

	LRetBool(_NLL, TM_Ret);
}

#ifdef _CHAR_PROFESSION
int NPC_Lua_NLG_AddSk(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, 3);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_SkIndex = (int)lua_tointeger(_NLL, 2);
	int TM_SkLV = 1;

	if(lua_gettop(_NLL) == 3)
	{
		TM_SkLV = (int)lua_tointeger(_NLL, 3);
	}

	BOOL TM_Ret = NPC_AddSk(TM_Index, TM_SkIndex, TM_SkLV);

	LRetInt(_NLL, TM_Ret);
}


int NPC_Lua_NLG_SetZy(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, 3);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_ZyIndex = (int)lua_tointeger(_NLL, 2);

	BOOL TM_Ret = NPC_SetZy(TM_Index, TM_ZyIndex);

	LRetBool(_NLL, TM_Ret);
}
#endif

int NPC_Lua_Char_GetPlayerMaxNum(lua_State *_NLL)
{
	CheckEx(_NLL, 0);
	LRetInt(_NLL, CHAR_getPlayerMaxNum());
}

int NPC_Lua_NLG_GetXY(lua_State *_NLL)
{
	CheckEx2(_NLL, 1, 2);
	CheckIndexNull(_NLL, 1);
	int TM_MAP = (int)lua_tointeger(_NLL, 1);
	LRetInt(_NLL, QuBiao(TM_MAP));
}

int NPC_Lua_NLG_WalkJc(lua_State *_NLL)//检查前方障碍
{
	CheckEx(_NLL, 5);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Floor = (int)lua_tointeger(_NLL, 2);
	int TM_Ox = (int)lua_tointeger(_NLL, 3);
	int TM_Oy = (int)lua_tointeger(_NLL, 4);
	int TM_DIR = (int)lua_tointeger(_NLL, 5);

	int TM_Ret = (int)CHAR_walk_jjc(TM_Index,TM_Floor,TM_Ox,TM_Oy,TM_DIR);

	if( TM_Ret == CHAR_WALKSUCCESSED )
	{
		LRetInt(_NLL, 1);
	}else LRetInt(_NLL, 0);
}

int NPC_Lua_NLG_KickPlayer(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_index = (int)lua_tointeger(_NLL, 1);
	if( !CHAR_CHECKINDEX( TM_index)) {
		LRetErrInt(_NLL , -1, "人物传入的索引是无效的！");
		return;
	}
#ifdef _NETLOG_
	char cdkey[16];
	char charname[32];
	CONNECT_getCharname(CHAR_getWorkInt( TM_index, CHAR_WORKFD ),charname,32);
	CONNECT_getCdkey(CHAR_getWorkInt( TM_index, CHAR_WORKFD ),cdkey,16);
	LogCharOut(charname,cdkey,__FILE__,__FUNCTION__,__LINE__,"NPC_Lua_NLG_KickPlayer 处理下线");
#endif

	CONNECT_setCloseRequest( getfdFromCharaIndex(TM_index) , 1 );
	LRetBool(_NLL, TRUE);
}

int NPC_Lua_NLG_HealOne(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_index = (int)lua_tointeger(_NLL, 1);
	if( !CHAR_CHECKINDEX( TM_index)) {
		LRetErrInt(_NLL , -1, "传入的索引是无效的！");
		return;
	}
	CHAR_setInt( TM_index , CHAR_HP,99999999 );
	CHAR_setInt( TM_index , CHAR_MP,99999999 );
	CHAR_complianceParameter( TM_index );
	CHAR_send_P_StatusString( TM_index , CHAR_P_STRING_HP);
	CHAR_send_P_StatusString( TM_index , CHAR_P_STRING_MP);
	LRetBool(_NLL, TRUE);
}

int NPC_Lua_NLG_HealAll(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_index = (int)lua_tointeger(_NLL, 1);
	int TM_PetIndex = -1;
	if( !CHAR_CHECKINDEX( TM_index)) {
		LRetErrInt(_NLL , -1, "传入的索引是无效的！");
		return;
	}
	CHAR_setInt( TM_index , CHAR_HP,99999999 );
	CHAR_setInt( TM_index , CHAR_MP,99999999 );
	CHAR_complianceParameter( TM_index );
	CHAR_send_P_StatusString( TM_index , CHAR_P_STRING_HP);
	CHAR_send_P_StatusString( TM_index , CHAR_P_STRING_MP);
	int i = 0;
	for(i = 0; i < CHAR_MAXPETHAVE ; i++)
	{
		TM_PetIndex = CHAR_getCharPet( TM_index, i);

		if(!CHAR_CHECKINDEX(TM_PetIndex))
		{
			continue;
		}
		CHAR_setInt( TM_PetIndex , CHAR_HP,99999999 );
		CHAR_complianceParameter( TM_PetIndex );
		char msgbuf[64];
		snprintf( msgbuf, sizeof( msgbuf ), "K%d", i );
		CHAR_sendStatusString( TM_index, msgbuf );
		snprintf( msgbuf, sizeof( msgbuf ), "W%d", i );
		CHAR_sendStatusString( TM_index, msgbuf );
#ifdef _PET_SKILL2
		snprintf( msgbuf, sizeof( msgbuf ), "Q%d", i );
		CHAR_sendStatusString( TM_index, msgbuf );
#endif
	}
	LRetBool(_NLL, TRUE);
}

int NPC_Lua_NLG_LevelUpTo(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_index = (int)lua_tointeger(_NLL, 1);
	int TM_LV = (int)lua_tointeger(_NLL, 2);
	int i;
	if( !CHAR_CHECKINDEX( TM_index)) {
		LRetErrInt(_NLL , -1, "传入的索引是无效的！");
		return;
	}
	TM_LV = TM_LV - CHAR_getInt( TM_index, CHAR_LV);
	if (TM_LV <= 0) //返回失败
	{
		LRetBool(_NLL, FALSE);
	}
	if (CHAR_getInt( TM_index, CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER)//如果是人物的话
	{
		CHAR_setInt(TM_index,CHAR_SKILLUPPOINT,
			CHAR_getInt(TM_index,CHAR_SKILLUPPOINT)+TM_LV*3);
		CHAR_Skillupsend(TM_index);
		CHAR_setInt(TM_index,CHAR_LV, CHAR_getInt( TM_index, CHAR_LV) + TM_LV);
		CHAR_send_P_StatusString( TM_index , CHAR_P_STRING_LV|CHAR_P_STRING_NEXTEXP);
		LRetBool(_NLL, TRUE);
	}else if (CHAR_getInt( TM_index, CHAR_WHICHTYPE ) == CHAR_TYPEPET)//如果是宠物的话
	{
		char token[128];
		for( i = 1; i <= TM_LV; i ++ ){	//升级
			//CHAR_PetLevelUp( TM_index ,1);
			CHAR_PetLevelUp( TM_index);
			CHAR_PetAddVariableAi( TM_index, AI_FIX_PETLEVELUP );
			CHAR_setInt( TM_index, CHAR_LV, CHAR_getInt( TM_index, CHAR_LV) +1 );
		}
		int PLAYER = CHAR_getWorkInt(TM_index, CHAR_WORKPLAYERINDEX);//获得主人的索引
		for( i = 0; i < CHAR_MAXPETHAVE; i ++ )
		{
			if( CHAR_getCharPet( PLAYER, i ) == TM_index )break;
		}
		CHAR_setInt( TM_index , CHAR_HP ,CHAR_getWorkInt( TM_index, CHAR_WORKMAXHP ) );
		CHAR_setInt( TM_index , CHAR_MP ,CHAR_getWorkInt( TM_index, CHAR_WORKMAXMP ) );
		CHAR_complianceParameter( TM_index );
		snprintf( token, sizeof( token ), "K%d", i );
		CHAR_sendStatusString( PLAYER, token );
		snprintf( token, sizeof( token ), "W%d", i );
		CHAR_sendStatusString( PLAYER, token );
#ifdef _PET_SKILL2
		snprintf( token, sizeof( token ), "Q%d", i );
		CHAR_sendStatusString( TM_index, token );
#endif
	}
	LRetBool(_NLL, TRUE);
}


int NPC_Lua_NLG_AddExp(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int i;
	int TM_index = (int)lua_tointeger(_NLL, 1);
	int TM_EXP = (int)lua_tointeger(_NLL, 2);
	if( !CHAR_CHECKINDEX( TM_index)) {
		LRetErrInt(_NLL , -1, "传入的索引是无效的！");
		return;
	}
	CHAR_setInt( TM_index, CHAR_EXP,CHAR_getInt(TM_index,CHAR_EXP) + TM_EXP);

	if (CHAR_getInt( TM_index, CHAR_WHICHTYPE ) == CHAR_TYPEPET)//如果是宠物的话
	{
		char token[128];
		int PLAYER = CHAR_getWorkInt(TM_index, CHAR_WORKPLAYERINDEX);//获得主人的索引
		for( i = 0; i < CHAR_MAXPETHAVE; i ++ )
		{
			if( CHAR_getCharPet( PLAYER, i ) == TM_index )break;
		}
		CHAR_complianceParameter( TM_index );
		snprintf( token, sizeof( token ), "K%d", i );
		CHAR_sendStatusString( PLAYER, token );
		snprintf( token, sizeof( token ), "W%d", i );
		CHAR_sendStatusString( PLAYER, token );
#ifdef _PET_SKILL2
		snprintf( token, sizeof( token ), "Q%d", i );
		CHAR_sendStatusString( PLAYER, token );
#endif
	}else CHAR_send_P_StatusString( TM_index , CHAR_P_STRING_EXP);
	LRetBool(_NLL, TRUE);
}

int NPC_Lua_NLG_UpPet(lua_State *_NLL)
{
	CheckEx2(_NLL, 1, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Top = lua_gettop(_NLL);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_It = -1;
	int i;
	int petindex;
	char token[64];
	if( !CHAR_CHECKINDEX( TM_Index)) {
		LRetErrInt(_NLL , -1, "传入的索引是无效的！");
		return;
	}
	if (TM_Top == 2)
	{
		TM_It = (int)lua_tointeger(_NLL, 2);
	}
	if(TM_It == -1){
		for (i=0;i<CHAR_MAXPETHAVE;i++)
		{
			petindex = CHAR_getCharPet( TM_Index, i);
			if( !CHAR_CHECKINDEX( petindex) ) continue;
			CHAR_complianceParameter( petindex );
			snprintf( token, sizeof( token ), "K%d", i );
			CHAR_sendStatusString( TM_Index, token );
			snprintf( token, sizeof( token ), "W%d", i );
			CHAR_sendStatusString( TM_Index, token );
#ifdef _PET_SKILL2
			snprintf( token, sizeof( token ), "Q%d", i );
			CHAR_sendStatusString( TM_Index, token );
#endif
		}
		LRetInt(_NLL,0);
	}
	else
	{
		petindex = CHAR_getCharPet( i, TM_It);
		if( !CHAR_CHECKINDEX( petindex) ) {
			LRetErrInt(_NLL , -1, "传入的索引是无效的！");
		}
		CHAR_complianceParameter( petindex );
		snprintf( token, sizeof( token ), "K%d", TM_It );
		CHAR_sendStatusString( TM_Index, token );
		snprintf( token, sizeof( token ), "W%d", TM_It );
		CHAR_sendStatusString( TM_Index, token );
#ifdef _PET_SKILL2
		snprintf( token, sizeof( token ), "Q%d", TM_It );
		CHAR_sendStatusString( TM_Index, token );
#endif
	}
}

int NPC_Lua_NLG_DelPetByPos(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_PetID = (int)lua_tointeger(_NLL, 2);
	int TM_PetIndex = -1;
	int TM_Ret = 0;
	TM_PetID -= 1;
	if( !CHAR_CHECKINDEX( TM_Index)) {
		LRetErrInt(_NLL , -1, "人物传入的索引是无效的！");
		return;
	}
	if (TM_PetID < 0 || TM_PetID > 4) LRetErrInt(_NLL , -1, "指定的宠物栏位没有宠物！");

	TM_PetIndex = CHAR_getCharPet( TM_Index, TM_PetID);

	if(!CHAR_CHECKINDEX(TM_PetIndex))
	{
		LRetInt(_NLL, -1);
		return -1;
	}

	TM_Ret = NPC_DelPet( TM_Index, TM_PetID);
	if(TM_Ret < 0)
	{
		char TM_MsgBuff[128];
		snprintf(TM_MsgBuff, sizeof(TM_MsgBuff), "删除宠物时产生错误 错误号: %d。", TM_Ret);
		LRetErrInt(_NLL , -1, TM_MsgBuff);
	}

	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_NLG_DelPetByIndex(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Pindex = (int)lua_tointeger(_NLL, 2);
	int TM_Num = 1;
	int TM_PetIndex = -1;
	int TM_Ret = 0;
	if( !CHAR_CHECKINDEX( TM_Index)) {
		LRetErrInt(_NLL , -1, "人物传入的索引是无效的！");
		return;
	}
	if( !CHAR_CHECKINDEX( TM_Pindex)) {
		LRetErrInt(_NLL , -1, "宠物传入的索引是无效的！");
		return;
	}
	int i = 0;
	for(i = 0; i < CHAR_MAXPETHAVE ; i++)
	{
		TM_PetIndex = CHAR_getCharPet( TM_Index, i);

		if(!CHAR_CHECKINDEX(TM_PetIndex))
		{
			continue;
		}

		if (TM_PetIndex != TM_Pindex) continue;

		TM_Ret = NPC_DelPet( TM_Index, i);
		if(TM_Ret < 0)
		{
			char TM_MsgBuff[128];
			snprintf(TM_MsgBuff, sizeof(TM_MsgBuff), "删除宠物时产生错误 错误号: %d。", TM_Ret);
			LRetErrInt(_NLL , -1, TM_MsgBuff);
		}

		break;
	}
	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_NLG_GivePetByIndex(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_PetIndex = (int)lua_tointeger(_NLL, 2);
	if( !CHAR_CHECKINDEX( TM_Index)) {
		LRetErrInt(_NLL , -1, "人物传入的索引是无效的！");
		return;
	}
	if( !CHAR_CHECKINDEX( TM_PetIndex)) {
		LRetErrInt(_NLL , -1, "宠物传入的索引是无效的！");
		return;
	}
	int havepetelement = CHAR_getCharPetElement( TM_Index);
	if( havepetelement < 0 ) {
		LRetErrInt(_NLL , -1, "目标宠物栏位已满！");
		return;
	}
	CHAR_setWorkInt( TM_PetIndex, CHAR_WORKPLAYERINDEX, TM_Index);//宠物主人
	CHAR_setCharPet( TM_Index, havepetelement, TM_PetIndex);
	CHAR_setChar( TM_PetIndex, CHAR_OWNERCDKEY,
		CHAR_getChar( TM_Index, CHAR_CDKEY));
	CHAR_setChar( TM_PetIndex, CHAR_OWNERCHARANAME,
		CHAR_getChar( TM_Index, CHAR_NAME));
	CHAR_complianceParameter(TM_PetIndex);

	char msgbuf[64];
	snprintf( msgbuf, sizeof( msgbuf ), "K%d", havepetelement );
	CHAR_sendStatusString( TM_Index, msgbuf );
	snprintf( msgbuf, sizeof( msgbuf ), "W%d", havepetelement );
	CHAR_sendStatusString( TM_Index, msgbuf );
#ifdef _PET_SKILL2
	snprintf( msgbuf, sizeof( msgbuf ), "Q%d", havepetelement );
	CHAR_sendStatusString( TM_Index, msgbuf );
#endif
	LRetInt(_NLL, havepetelement);
}

int NPC_Lua_NLG_GiveItemByIndex(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_ItemIndex = (int)lua_tointeger(_NLL, 2);
	if( !CHAR_CHECKINDEX( TM_Index)) {
		LRetErrInt(_NLL , -1, "人物传入的索引是无效的！");
		return;
	}
	if( !ITEM_CHECKINDEX( TM_ItemIndex)) {
		LRetErrInt(_NLL , -1, "道具传入的索引是无效的！");
		return;
	}
	int emptyitemindexinchara = CHAR_findEmptyItemBox( TM_Index);
	if( emptyitemindexinchara < 0 ) {
		LRetErrInt(_NLL , -1, "目标道具栏位已满！");
		return;
	}
	CHAR_setItemIndex( TM_Index, emptyitemindexinchara, TM_ItemIndex );
	ITEM_setWorkInt(TM_ItemIndex, ITEM_WORKOBJINDEX,-1);
	ITEM_setWorkInt(TM_ItemIndex, ITEM_WORKCHARAINDEX, TM_Index);
	CHAR_sendItemDataOne( TM_Index, emptyitemindexinchara);
	LRetInt(_NLL, emptyitemindexinchara);
}

int NPC_Lua_NLG_WarpToSpecificPoint(lua_State *_NLL)
{

	CheckEx(_NLL, 4);
	CheckIndexNull(_NLL, 1);
	int cindex = (int)lua_tointeger(_NLL, 1);
	int fl = (int)lua_tointeger(_NLL, 2);
	int x = (int)lua_tointeger(_NLL, 3);
	int y = (int)lua_tointeger(_NLL, 4);

	if(!CHAR_CHECKINDEX(cindex))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	LRetBool(_NLL,CHAR_warpToSpecificPoint(cindex,fl,x,y));
}

BOOL NPC_Lua_NLG_UpSkillupPoint(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	if( !CHAR_CHECKINDEX( TM_Index)) {
		LRetErrInt(_NLL , -1, "人物传入的索引是无效的！");
		return;
	}
	CHAR_Skillupsend(TM_Index);
	LRetBool(_NLL, TRUE);
}

//新增
int NPC_Lua_NLG_StayEncount(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int fd,charaindex = (int)lua_tointeger(_NLL, 1);
	fd = CHAR_getWorkInt( charaindex, CHAR_WORKFD);
	setStayEncount(fd);
	CHAR_talkToCli(charaindex, -1, "你感受到周边突然充满了杀气！", CHAR_COLORYELLOW);
#ifdef _USER_CHARLOOPS
	{
		Char 	*ch;
		ch  = CHAR_getCharPointer( charaindex);
		if( ch == NULL ) return;
		strcpysafe( ch->charfunctable[CHAR_LOOPFUNCTEMP1].string,
			sizeof( ch->charfunctable[CHAR_LOOPFUNCTEMP1]), "CHAR_BattleStayLoop");//战斗
		CHAR_setInt( charaindex, CHAR_LOOPINTERVAL, 2500);
		CHAR_constructFunctable( charaindex);
	}
#endif
	LRetBool(_NLL,TRUE);
}

int NPC_Lua_NLG_HealerAllHeal(lua_State *_NLL)
{
	CheckEx(_NLL,1);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	NPC_HealerAllHeal(TM_Index);
	LRetNull(_NLL);
}
extern int getArrayInt(lua_State *L, int idx);
int NPC_Lua_NLG_CreateVsEnemyAB (lua_State *L)
{
	const int charaindex = luaL_checkint(L, 1);
	const int npcindex = luaL_checkint(L, 2);
	int enemytable[11];
	int i;
	for( i = 0; i < 11; i ++ ) {
		enemytable[i] = -1;
	}
	luaL_checktype(L, 3, LUA_TTABLE);
	int n = luaL_getn(L, 3);
	int id = 0;
	for(i = 0; i < n; i++){
		int enemyid = getArrayInt(L, i);
		int curEnemy = ENEMY_getEnemyArrayFromId( enemyid );

		if( ENEMY_CHECKINDEX( curEnemy) ) {
			enemytable[id++] = curEnemy;
		}else{
			enemytable[i] = -1;
		}
	}
	int ret = BATTLE_CreateVsEnemyNew(charaindex, npcindex, enemytable);
	if( ret == 0 ) {
		if(npcindex>-1){
			CHAR_setWorkInt( npcindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_INIT );
			CHAR_sendBattleEffect( npcindex, ON);
		}
		lua_pushinteger(L, CHAR_getWorkInt( charaindex, CHAR_WORKBATTLEINDEX));
		return 1;
	}else{
		return 0;
	}
}

#endif //#ifdef _JZ_NEWSCRIPT_LUA
