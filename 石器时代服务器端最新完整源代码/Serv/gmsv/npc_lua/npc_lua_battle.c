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

#ifdef _JZ_NEWSCRIPT_LUA

int NPC_Lua_Battle_GetPlayIndex(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	assert(BattleArray != NULL);
	CheckBattleIndexNull(_NLL, 1);
	int TM_battleindex = (int)lua_tointeger(_NLL, 1);
	int TM_side = (int)lua_tointeger(_NLL, 2);
	int TM_num = (int)lua_tointeger(_NLL, 3);
	
	if( BATTLE_CHECKINDEX( TM_battleindex ) == FALSE )
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}

	BATTLE *TM_Battle = &BattleArray[TM_battleindex];

	if(TM_Battle == NULL)
	{
		LRetErrInt(_NLL , -2, "无法获取指针。");
	}
	int TM_Ret = -1;

	if(TM_side >= 0 && TM_side < 2 && TM_num >=0 && TM_num < BATTLE_ENTRY_MAX)
	{
		TM_Ret = TM_Battle->Side[TM_side].Entry[TM_num].charaindex;
	}else
	{
		LRetErrInt(_NLL , -3, "范围错误。");
	}
	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_Battle_SetNORisk(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	assert(BattleArray != NULL);
	CheckBattleIndexNull(_NLL, 1);
	int TM_battleindex = (int)lua_tointeger(_NLL, 1);
	int TM_vl = (int)lua_tointeger(_NLL, 2);
	
	if( BATTLE_CHECKINDEX( TM_battleindex ) == FALSE )
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}

	BATTLE *TM_Battle = &BattleArray[TM_battleindex];

	if(TM_Battle == NULL)
	{
		LRetErrInt(_NLL , -2, "无法获取指针。");
	}
	
	int TM_Ret = TM_Battle->norisk;
	TM_Battle->norisk = TM_vl;

	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_Battle_SetMod(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	assert(BattleArray != NULL);
	CheckBattleIndexNull(_NLL, 1);
	int TM_battleindex = (int)lua_tointeger(_NLL, 1);
	int TM_vl = (int)lua_tointeger(_NLL, 2);
	
	if( BATTLE_CHECKINDEX( TM_battleindex ) == FALSE )
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}

	BATTLE *TM_Battle = &BattleArray[TM_battleindex];

	if(TM_Battle == NULL)
	{
		LRetErrInt(_NLL , -2, "无法获取指针。");
	}
	
	int TM_Ret = TM_Battle->mode;
	TM_Battle->mode = TM_vl;

	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_Battle_SetType(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	assert(BattleArray != NULL);
	CheckBattleIndexNull(_NLL, 1);
	int TM_battleindex = (int)lua_tointeger(_NLL, 1);
	int TM_vl = (int)lua_tointeger(_NLL, 2);
	
	if( BATTLE_CHECKINDEX( TM_battleindex ) == FALSE )
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}

	BATTLE *TM_Battle = &BattleArray[TM_battleindex];

	if(TM_Battle == NULL)
	{
		LRetErrInt(_NLL , -2, "无法获取指针。");
	}
	
	int TM_Ret = TM_Battle->type;
	TM_Battle->type = TM_vl;

	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_Battle_SetWinEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	assert(BattleArray != NULL);
	CheckBattleIndexNull(_NLL, 3);
	int TM_battleindex = (int)lua_tointeger(_NLL, 3);

	if( BATTLE_CHECKINDEX( TM_battleindex ) == FALSE )
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}

	BATTLE *TM_Battle = &BattleArray[TM_battleindex];

	if(TM_Battle == NULL)
	{
		LRetErrInt(_NLL , -2, "无法获取指针。");
	}

	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);

	if(lua_tointeger(_NLL, -1) > 0)
	{
		if( (void *)(TM_Battle->WinFunc) != (void *)NPC_Lua_BattleWinCallBack)
		{
			TM_Battle->BakFunc = TM_Battle->WinFunc;
			TM_Battle->WinFunc = (void *)NPC_Lua_BattleWinCallBack;
			strcpy_s(TM_Battle->BakLuaFuncName, sizeof(TM_Battle->BakLuaFuncName), lua_tostring(_NLL, 2));
		}
	}
	else
	{
		if((void *)(TM_Battle->WinFunc) == (void *)NPC_Lua_BattleWinCallBack)
		{
			TM_Battle->BakLuaFuncName[0] = '\0';
			TM_Battle->WinFunc = TM_Battle->BakFunc;
		}
	}
	return TM_Ret;
}

int NPC_Lua_Battle_GetData(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	int TM_Ret;
	assert(BattleArray != NULL);
	CheckBattleIndexNull(_NLL, 1);
	int TM_battleindex = (int)lua_tointeger(_NLL, 1);
	int TM_FLG = (int)lua_tointeger(_NLL, 2);
	char* TM_Buff = "\0";
	if( BATTLE_CHECKINDEX( TM_battleindex ) == FALSE )
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}

	BATTLE *TM_Battle = &BattleArray[TM_battleindex];

	if(TM_Battle == NULL)
	{
		LRetErrInt(_NLL , -2, "无法获取指针。");
	}

	if (TM_FLG == 0)
	{
		TM_Ret = TM_Battle->battleindex;
	}
	else if (TM_FLG == 1)
	{
		TM_Ret = TM_Battle->dpbattle;
	}
	else if (TM_FLG == 2)
	{
		TM_Ret = TM_Battle->turn;
	}
	else if (TM_FLG == 3)
	{
		TM_Ret = TM_Battle->timer;
	}
	else if (TM_FLG == 4)
	{
		TM_Ret = TM_Battle->leaderindex;
	}
	else if (TM_FLG == 5)
	{
		TM_Ret = TM_Battle->rivalindex;
	}
	else if (TM_FLG == 6)
	{
		TM_Buff = TM_Battle->leadercdkey;
	}
	else if (TM_FLG == 7)
	{
		TM_Buff = TM_Battle->leadername;
	}
	else if (TM_FLG == 8)
	{
		TM_Buff = TM_Battle->rivalcdkey;
	}
	else if (TM_FLG == 9)
	{
		TM_Buff = TM_Battle->rivalname;
	}
	else if (TM_FLG == 10)
	{
		TM_Ret = TM_Battle->CreateTime;
	}
	else if (TM_FLG == 11)
	{
		TM_Ret = TM_Battle->EndTime;
	}
	else if (TM_FLG == 12)
	{
		TM_Ret = TM_Battle->PartTime;
	}
	else if (TM_FLG == 13)
	{
		TM_Ret = TM_Battle->flgTime;
	}
	else if (TM_FLG == 14)
	{
		TM_Ret = TM_Battle->createindex;
	}
	else if (TM_FLG == 15)
	{
		TM_Ret = TM_Battle->mode;
	}
	else if (TM_FLG == 16)
	{
		TM_Ret = TM_Battle->type;
	}
	else if (TM_FLG == 17)
	{
		TM_Ret = TM_Battle->norisk;
	}
	else if (TM_FLG == 18)
	{
		TM_Ret = TM_Battle->winside;
	}
	if (TM_FLG >=6 && TM_FLG <= 9) {
		LRetMsg(_NLL, TM_Buff);
	}
	else LRetInt(_NLL, TM_Ret);

	LRetErrNull(_NLL, "传入的标志是错误的。");
}

int NPC_Lua_Battle_IndexToNo(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckBattleIndexNull(_NLL, 1);
	int TM_battleindex = (int)lua_tointeger(_NLL, 1);
	int TM_index = (int)lua_tointeger(_NLL, 2);

	if( BATTLE_CHECKINDEX( TM_battleindex ) == FALSE )
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	if( CHAR_CHECKINDEX( TM_index ) == FALSE )
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = BATTLE_Index2No( TM_battleindex, TM_index );
	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_Battle_NoToIndex(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckBattleIndexNull(_NLL, 1);
	int TM_battleindex = (int)lua_tointeger(_NLL, 1);
	int TM_No = (int)lua_tointeger(_NLL, 2);

	if( BATTLE_CHECKINDEX( TM_battleindex ) == FALSE )
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = BATTLE_No2Index( TM_battleindex, TM_No );
	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_Battle_CheckIndex(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckBattleIndexNull(_NLL, 1);
	int TM_battleindex = (int)lua_tointeger(_NLL, 1);

	BOOL TM_Ret = BATTLE_CHECKINDEX( TM_battleindex );
	LRetBool(_NLL, TM_Ret);
}

int NPC_Lua_Battle_SetData(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	int TM_Ret;
	char * TM_Buff = "\0";
	assert(BattleArray != NULL);
	CheckBattleIndexNull(_NLL, 1);
	int TM_battleindex = (int)lua_tointeger(_NLL, 1);
	int TM_FLG = (int)lua_tointeger(_NLL, 2);
	int TM_INT = -1;
	char *TM_MSG = "\0";
	if( BATTLE_CHECKINDEX( TM_battleindex ) == FALSE )
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	if (TM_FLG >=6 && TM_FLG <= 9) {
		TM_MSG = lua_tostring(_NLL, 3);
	}else{
		TM_INT = (int)lua_tointeger(_NLL, 3);
	}
	BATTLE *TM_Battle = &BattleArray[TM_battleindex];

	if(TM_Battle == NULL)
	{
		LRetErrInt(_NLL , -2, "无法获取指针。");
	}

	if (TM_FLG == 0)
	{
		TM_Ret = TM_Battle->battleindex;
		TM_Battle->battleindex = TM_INT;
	}
	else if (TM_FLG == 1)
	{
		TM_Ret = TM_Battle->dpbattle;
		TM_Battle->dpbattle = TM_INT;
	}
	else if (TM_FLG == 2)
	{
		TM_Ret = TM_Battle->turn;
		TM_Battle->turn = TM_INT;
	}
	else if (TM_FLG == 3)
	{
		TM_Ret = TM_Battle->timer;
		TM_Battle->timer = TM_INT;
	}
	else if (TM_FLG == 4)
	{
		TM_Ret = TM_Battle->leaderindex;
		TM_Battle->leaderindex = TM_INT;
	}
	else if (TM_FLG == 5)
	{
		TM_Ret = TM_Battle->rivalindex;
		TM_Battle->rivalindex = TM_INT;
	}
	else if (TM_FLG == 6)
	{
		TM_Buff = TM_Battle->leadercdkey;
		strcpy(TM_Battle->leadercdkey,TM_MSG);
	}
	else if (TM_FLG == 7)
	{
		TM_Buff = TM_Battle->leadername;
		strcpy(TM_Battle->leadername,TM_MSG);
	}
	else if (TM_FLG == 8)
	{
		TM_Buff = TM_Battle->rivalcdkey;
		strcpy(TM_Battle->rivalcdkey,TM_MSG);
	}
	else if (TM_FLG == 9)
	{
		TM_Buff = TM_Battle->rivalname;
		strcpy(TM_Battle->rivalname,TM_MSG);
	}
	else if (TM_FLG == 10)
	{
		TM_Ret = TM_Battle->CreateTime;
		TM_Battle->CreateTime = TM_INT;
	}
	else if (TM_FLG == 11)
	{
		TM_Ret = TM_Battle->EndTime;
		TM_Battle->EndTime = TM_INT;
	}
	else if (TM_FLG == 12)
	{
		TM_Ret = TM_Battle->PartTime;
		TM_Battle->PartTime = TM_INT;
	}
	else if (TM_FLG == 13)
	{
		TM_Ret = TM_Battle->flgTime;
		TM_Battle->flgTime = TM_INT;
	}
	else if (TM_FLG == 14)
	{
		TM_Ret = TM_Battle->createindex;
		TM_Battle->createindex = TM_INT;
	}
	else if (TM_FLG == 15)
	{
		TM_Ret = TM_Battle->mode;
		TM_Battle->mode = TM_INT;
	}
	else if (TM_FLG == 16)
	{
		TM_Ret = TM_Battle->type;
		TM_Battle->type = TM_INT;
	}
	else if (TM_FLG == 17)
	{
		TM_Ret = TM_Battle->norisk;
		TM_Battle->norisk = TM_INT;
	}
	else if (TM_FLG == 18)
	{
		TM_Ret = TM_Battle->winside;
		TM_Battle->winside = TM_INT;
	}
	if (TM_FLG >=6 && TM_FLG <= 9) {
		LRetMsg(_NLL, TM_Buff);
	}
	else LRetInt(_NLL, TM_Ret);

	LRetErrNull(_NLL, "传入的标志是错误的。");
}

int NPC_Lua_Battle_Exit(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckBattleIndexNull(_NLL, 1);
	int TM_index = (int)lua_tointeger(_NLL, 1);
	int TM_battleindex = (int)lua_tointeger(_NLL, 2);

	if( BATTLE_CHECKINDEX( TM_battleindex ) == FALSE )
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	if( CHAR_CHECKINDEX( TM_index ) == FALSE )
	{
		LRetErrInt(_NLL , -2, "传入的索引是无效的。");
	}
	BATTLE_Exit( TM_index, TM_battleindex );
	// 由□  奴  仃月
	CHAR_DischargePartyNoMsg( TM_index );

	CHAR_talkToCli( TM_index, -1,
		"战斗中止。", CHAR_COLORYELLOW );

	lssproto_B_send( getfdFromCharaIndex( TM_index ), "BU" );


	LRetInt(_NLL, 0);
}


int NPC_Lua_Battle_SetEndEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	assert(BattleArray != NULL);
	CheckBattleIndexNull(_NLL, 3);
	int TM_battleindex = (int)lua_tointeger(_NLL, 3);

	if( BATTLE_CHECKINDEX( TM_battleindex ) == FALSE )
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}

	BATTLE *TM_Battle = &BattleArray[TM_battleindex];

	if(TM_Battle == NULL)
	{
		LRetErrInt(_NLL , -2, "无法获取指针。");
	}

	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);

	TM_Battle->EndFunc = (void *)NPC_Lua_BattleEndCallBack;

	strcpy_s(TM_Battle->EndLuaFuncName, sizeof(TM_Battle->EndLuaFuncName), lua_tostring(_NLL, 2));
	return TM_Ret;
}


#endif //#ifdef _JZ_NEWSCRIPT_LUA


int NPC_Lua_Bit_band(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, 5);
	CheckIndexNull(_NLL, 1);
	CheckIndexNull(_NLL, 2);
	int TM_Top = lua_gettop(_NLL);
	int a = (int)lua_tointeger(_NLL, 1);
	int b = (int)lua_tointeger(_NLL, 2);
	int c,d,e;
	if(TM_Top >= 3)
	{
		c = (int)lua_tointeger(_NLL, 3);
		a = a&b;
		LRetInt(_NLL,a);
	}
	if(TM_Top >= 4)
	{
		d = (int)lua_tointeger(_NLL, 4);
		LRetInt(_NLL,a&b&c&d);
	}
	if(TM_Top >= 5)
	{
		e = (int)lua_tointeger(_NLL, 5);
		LRetInt(_NLL,a&b&c&d&e);
	}else LRetInt(_NLL,a&b);
}

int NPC_Lua_Bit_bor(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, 5);
	CheckIndexNull(_NLL, 1);
	CheckIndexNull(_NLL, 2);
	int TM_Top = lua_gettop(_NLL);
	int a = (int)lua_tointeger(_NLL, 1);
	int b = (int)lua_tointeger(_NLL, 2);
	int c,d,e;
	if(TM_Top >= 3)
	{
		c = (int)lua_tointeger(_NLL, 3);
		LRetInt(_NLL,a|b|c);
	}
	if(TM_Top >= 4)
	{
		d = (int)lua_tointeger(_NLL, 4);
		LRetInt(_NLL,a|b|c|d);
	}
	if(TM_Top >= 5)
	{
		e = (int)lua_tointeger(_NLL, 5);
		LRetInt(_NLL,a|b|c|d|e);
	}else LRetInt(_NLL,a|b);
}

int NPC_Lua_Bit_bxor(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, 5);
	CheckIndexNull(_NLL, 1);
	CheckIndexNull(_NLL, 2);
	int TM_Top = lua_gettop(_NLL);
	int a = (int)lua_tointeger(_NLL, 1);
	int b = (int)lua_tointeger(_NLL, 2);
	int c,d,e;
	if(TM_Top >= 3)
	{
		c = (int)lua_tointeger(_NLL, 3);
		LRetInt(_NLL,a^b^c);
	}
	if(TM_Top >= 4)
	{
		d = (int)lua_tointeger(_NLL, 4);
		LRetInt(_NLL,a^b^c^d);
	}
	if(TM_Top >= 5)
	{
		e = (int)lua_tointeger(_NLL, 5);
		LRetInt(_NLL,a^b^c^d^e);
	}else LRetInt(_NLL,a^b);
}

int NPC_Lua_Bit_lshift(lua_State *_NLL)
{
	CheckEx2(_NLL, 2,2);
	CheckIndexNull(_NLL, 1);
	int TM_Top = lua_gettop(_NLL);
	int a = (int)lua_tointeger(_NLL, 1);
	int b = (int)lua_tointeger(_NLL, 2);
	LRetInt(_NLL,a<<b);
}

int NPC_Lua_Bit_rshift(lua_State *_NLL)
{
	CheckEx2(_NLL, 2,2);
	CheckIndexNull(_NLL, 1);
	int TM_Top = lua_gettop(_NLL);
	int a = (int)lua_tointeger(_NLL, 1);
	int b = (int)lua_tointeger(_NLL, 2);
	LRetInt(_NLL,a>>b);
}

