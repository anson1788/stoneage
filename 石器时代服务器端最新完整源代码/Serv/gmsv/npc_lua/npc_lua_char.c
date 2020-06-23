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
#include "longzoro/sasql.h"
#include "pet_skill.h"
#include "enemy.h"
#include "family.h"

extern char hanzibuf[5000][8];

#ifdef _JZ_NEWSCRIPT_LUA

//////////////////////////////////////////////////////////////////////////////
//设置数据的接口
int NPC_Lua_Char_ClrEvtEnd(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, LUA_MINSTACK);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Max = lua_gettop(_NLL);
	int TM_EveNo = -1;
	int i = 0;
	for(i = 2; i <= TM_Max; i++)
	{
		TM_EveNo = (int)lua_tointeger(_NLL, i);
		NPC_EndEventSetFlgCls(TM_Index, TM_EveNo);
	}

	LRetInt(_NLL, TM_EveNo);
}

int NPC_Lua_Char_ClrEvtNow(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, LUA_MINSTACK);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Max = lua_gettop(_NLL);
	int TM_EveNo = -1;
	int i = 0;
	for(i = 2; i <= TM_Max; i++)
	{
		TM_EveNo = (int)lua_tointeger(_NLL, i);
		NPC_NowEventSetFlgCls(TM_Index, TM_EveNo);
	}
	LRetInt(_NLL, TM_EveNo);
}

int NPC_Lua_Char_ClrEvt(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, LUA_MINSTACK);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Max = lua_gettop(_NLL);
	int TM_EveNo = -1;
	int i = 0;
	for(i = 2; i <= TM_Max; i++)
	{
		TM_EveNo = (int)lua_tointeger(_NLL, i);
		NPC_NowEndEventSetFlgCls(TM_Index, TM_EveNo);
	}
	LRetInt(_NLL, TM_EveNo);
}

int NPC_Lua_Char_SetEvtEnd(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, LUA_MINSTACK);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Max = lua_gettop(_NLL);
	int TM_EveNo = -1;
	int i = 0;
	for(i = 2; i <= TM_Max; i++)
	{
		TM_EveNo = (int)lua_tointeger(_NLL, i);
		NPC_EventSetFlg(TM_Index, TM_EveNo);
	}

	LRetInt(_NLL, TM_EveNo);
}

int NPC_Lua_Char_SetEvtNow(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, LUA_MINSTACK);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Max = lua_gettop(_NLL);
	int TM_EveNo = -1;
	int i = 0;
	for(i = 2; i <= TM_Max; i++)
	{
		TM_EveNo = (int)lua_tointeger(_NLL, i);
		NPC_NowEventSetFlg(TM_Index, TM_EveNo);
	}

	LRetInt(_NLL, TM_EveNo);
}

int NPC_Lua_Char_SetData(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Flg = (int)lua_tointeger(_NLL, 2);

	if(TM_Flg >= 0)
	{
		if(TM_Flg < LUA_DATALINE1)
		{
			TM_Flg -= LUA_DATALINE0;
			if(TM_Flg == CHAR_WHICHTYPE)
			{
				LRetInt(_NLL, 0);
			}
			int TM_IntVal = (int)lua_tointeger(_NLL, 3);
#ifdef _JZ_BILLING_SYSTEM
			if(TM_Flg == CHAR_VIPPOINT)
			{
				saacproto_IncreaseMPoint_send(acfd, -1, CHAR_getChar( TM_Index, CHAR_CDKEY ), "增加金币", TM_IntVal - CHAR_getInt(TM_Index, TM_Flg));
			}
#endif
			int TM_RetInt = CHAR_setInt(TM_Index, TM_Flg, TM_IntVal);
#ifdef _JZ_SAVE
			if(TM_Flg == CHAR_MEMBERORDER)
			{
				CHAR_charSaveFromConnect(TM_Index, FALSE);
			}
#endif
			LRetInt(_NLL, TM_RetInt);
		}else if(TM_Flg < LUA_DATALINE2)
		{
			TM_Flg -= LUA_DATALINE1;
			char *TM_CharPoint = (char *)lua_tostring(_NLL, 3);
			BOOL TM_RetBOOL = CHAR_setChar(TM_Index, TM_Flg, TM_CharPoint);
			LRetBool(_NLL, TM_RetBOOL);
		}else if(TM_Flg < LUA_DATALINE3)
		{
			TM_Flg -= LUA_DATALINE2;
			if(TM_Flg == CHAR_WORKOBJINDEX || TM_Flg == CHAR_WORKACTION)
			{
				LRetInt(_NLL, 0);
			}
			int TM_WorkIntVal = (int)lua_tointeger(_NLL, 3);
			int TM_RetInt = CHAR_setWorkInt(TM_Index, TM_Flg, TM_WorkIntVal);
			LRetInt(_NLL, TM_RetInt);
		}else if(TM_Flg < LUA_DATALINE4)
		{
			TM_Flg -= LUA_DATALINE3;
			char *TM_WorkCharPoint = (char *)lua_tostring(_NLL, 3);
			BOOL TM_RetBOOL = CHAR_setWorkChar(TM_Index, TM_Flg, TM_WorkCharPoint);
			LRetBool(_NLL, TM_RetBOOL);
		}else if(TM_Flg < LUA_DATALINE5)
		{
			TM_Flg -= LUA_DATALINE4;
			BOOL TM_BOOLVal = (BOOL)lua_toboolean(_NLL, 3);
			int TM_RetInt = CHAR_setFlg(TM_Index, TM_Flg, TM_BOOLVal);
			LRetInt(_NLL, TM_RetInt);
		}else if(TM_Flg < LUA_DATALINE6){
			TM_Flg -= LUA_DATALINE5;
			if(TM_Flg==1){
				int TM_WorkIntVal = (int)lua_tointeger(_NLL, 3);
				sasql_vippoint(CHAR_getUseID(TM_Index),TM_WorkIntVal,520);
			}else LRetErrNull(_NLL, "传入的标志是错误的。");
		}else
		{
			LRetErrNull(_NLL, "传入的标志是错误的。");
		}
	}
	LRetErrNull(_NLL, "传入的标志是错误的。");
}

//////////////////////////////////////////////////////////////////////////////
//事件设置的接口
int NPC_Lua_Char_SetWalkPreEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);
	
	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_WALKPREFUNC] != (void *)NPC_Lua_WalkPreCallBack)
		{
			if(TM_char->functable[CHAR_WALKPREFUNC] != NULL)
			{
				TM_char->sur_functable[CHAR_WALKPREFUNC] = TM_char->functable[CHAR_WALKPREFUNC];
				strcpy_s(TM_char->sur_charfunctable[CHAR_WALKPREFUNC].string, sizeof(TM_char->sur_charfunctable[CHAR_WALKPREFUNC].string), TM_char->charfunctable[CHAR_WALKPREFUNC].string);
			}else
			{
				TM_char->sur_functable[CHAR_WALKPREFUNC] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_WALKPREFUNC].string[0] = '\0';
			}
	
			TM_char->functable[CHAR_WALKPREFUNC] = (void *)NPC_Lua_WalkPreCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_WALKPREFUNC].string, sizeof(TM_char->lua_charfunctable[CHAR_WALKPREFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_WALKPREFUNC].string, sizeof(TM_char->charfunctable[CHAR_WALKPREFUNC].string), FUNCNAME_WALKPRECALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_WALKPREFUNC] == (void *)NPC_Lua_WalkPreCallBack)
		{
			TM_char->lua_charfunctable[CHAR_WALKPREFUNC].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_WALKPREFUNC] == NULL)
			{
				TM_char->functable[CHAR_WALKPREFUNC] = (void *)NULL;
				TM_char->charfunctable[CHAR_WALKPREFUNC].string[0] = '\0';
			}else
			{
				TM_char->functable[CHAR_WALKPREFUNC] = TM_char->sur_functable[CHAR_WALKPREFUNC];
				TM_char->sur_functable[CHAR_WALKPREFUNC] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_WALKPREFUNC].string, sizeof(TM_char->charfunctable[CHAR_WALKPREFUNC].string), TM_char->sur_charfunctable[CHAR_WALKPREFUNC].string);
			}
		}
	}
	return TM_Ret;
}

int NPC_Lua_Char_SetWalkPostEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);
	
	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_WALKPOSTFUNC] != (void *)NPC_Lua_WalkPostCallBack)
		{
			if(TM_char->functable[CHAR_WALKPOSTFUNC] != NULL)
			{
				TM_char->sur_functable[CHAR_WALKPOSTFUNC] = TM_char->functable[CHAR_WALKPOSTFUNC];
				strcpy_s(TM_char->sur_charfunctable[CHAR_WALKPOSTFUNC].string, sizeof(TM_char->sur_charfunctable[CHAR_WALKPOSTFUNC].string), TM_char->charfunctable[CHAR_WALKPOSTFUNC].string);
			}else
			{
				TM_char->sur_functable[CHAR_WALKPOSTFUNC] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_WALKPOSTFUNC].string[0] = '\0';
			}
	
			TM_char->functable[CHAR_WALKPOSTFUNC] = (void *)NPC_Lua_WalkPostCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_WALKPOSTFUNC].string, sizeof(TM_char->lua_charfunctable[CHAR_WALKPOSTFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_WALKPOSTFUNC].string, sizeof(TM_char->charfunctable[CHAR_WALKPOSTFUNC].string), FUNCNAME_WALKPOSTCALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_WALKPOSTFUNC] == (void *)NPC_Lua_WalkPostCallBack)
		{
			TM_char->lua_charfunctable[CHAR_WALKPOSTFUNC].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_WALKPOSTFUNC] == NULL)
			{
				TM_char->functable[CHAR_WALKPOSTFUNC] = (void *)NULL;
				TM_char->charfunctable[CHAR_WALKPOSTFUNC].string[0] = '\0';
			}else
			{
				TM_char->functable[CHAR_WALKPOSTFUNC] = TM_char->sur_functable[CHAR_WALKPOSTFUNC];
				TM_char->sur_functable[CHAR_WALKPOSTFUNC] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_WALKPOSTFUNC].string, sizeof(TM_char->charfunctable[CHAR_WALKPOSTFUNC].string), TM_char->sur_charfunctable[CHAR_WALKPOSTFUNC].string);
			}
		}
	}
	return TM_Ret;
}

int NPC_Lua_Char_SetPreOverEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);
	
	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_PREOVERFUNC] != (void *)NPC_Lua_PreOverCallBack)
		{
			if(TM_char->functable[CHAR_PREOVERFUNC] != NULL)
			{
				TM_char->sur_functable[CHAR_PREOVERFUNC] = TM_char->functable[CHAR_PREOVERFUNC];
				strcpy_s(TM_char->sur_charfunctable[CHAR_PREOVERFUNC].string, sizeof(TM_char->sur_charfunctable[CHAR_PREOVERFUNC].string), TM_char->charfunctable[CHAR_PREOVERFUNC].string);
			}else
			{
				TM_char->sur_functable[CHAR_PREOVERFUNC] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_PREOVERFUNC].string[0] = '\0';
			}
	
			TM_char->functable[CHAR_PREOVERFUNC] = (void *)NPC_Lua_PreOverCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_PREOVERFUNC].string, sizeof(TM_char->lua_charfunctable[CHAR_PREOVERFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_PREOVERFUNC].string, sizeof(TM_char->charfunctable[CHAR_PREOVERFUNC].string), FUNCNAME_PREOVERCALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_PREOVERFUNC] == (void *)NPC_Lua_PreOverCallBack)
		{
			TM_char->lua_charfunctable[CHAR_PREOVERFUNC].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_PREOVERFUNC] == NULL)
			{
				TM_char->functable[CHAR_PREOVERFUNC] = (void *)NULL;
				TM_char->charfunctable[CHAR_PREOVERFUNC].string[0] = '\0';
			}else
			{
				TM_char->functable[CHAR_PREOVERFUNC] = TM_char->sur_functable[CHAR_PREOVERFUNC];
				TM_char->sur_functable[CHAR_PREOVERFUNC] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_PREOVERFUNC].string, sizeof(TM_char->charfunctable[CHAR_PREOVERFUNC].string), TM_char->sur_charfunctable[CHAR_PREOVERFUNC].string);
			}
		}
	}
	return TM_Ret;
}

int NPC_Lua_Char_SetPostOverEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);
	
	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_POSTOVERFUNC] != (void *)NPC_Lua_PostOverCallBack)
		{
			if(TM_char->functable[CHAR_POSTOVERFUNC] != NULL)
			{
				TM_char->sur_functable[CHAR_POSTOVERFUNC] = TM_char->functable[CHAR_POSTOVERFUNC];
				strcpy_s(TM_char->sur_charfunctable[CHAR_POSTOVERFUNC].string, sizeof(TM_char->sur_charfunctable[CHAR_POSTOVERFUNC].string), TM_char->charfunctable[CHAR_POSTOVERFUNC].string);
			}else
			{
				TM_char->sur_functable[CHAR_POSTOVERFUNC] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_POSTOVERFUNC].string[0] = '\0';
			}
	
			TM_char->functable[CHAR_POSTOVERFUNC] = (void *)NPC_Lua_PostOverCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_POSTOVERFUNC].string, sizeof(TM_char->lua_charfunctable[CHAR_POSTOVERFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_POSTOVERFUNC].string, sizeof(TM_char->charfunctable[CHAR_POSTOVERFUNC].string), FUNCNAME_POSTOVERCALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_POSTOVERFUNC] == (void *)NPC_Lua_PostOverCallBack)
		{
			TM_char->lua_charfunctable[CHAR_POSTOVERFUNC].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_POSTOVERFUNC] == NULL)
			{
				TM_char->functable[CHAR_POSTOVERFUNC] = (void *)NULL;
				TM_char->charfunctable[CHAR_POSTOVERFUNC].string[0] = '\0';
			}else
			{
				TM_char->functable[CHAR_POSTOVERFUNC] = TM_char->sur_functable[CHAR_POSTOVERFUNC];
				TM_char->sur_functable[CHAR_POSTOVERFUNC] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_POSTOVERFUNC].string, sizeof(TM_char->charfunctable[CHAR_POSTOVERFUNC].string), TM_char->sur_charfunctable[CHAR_POSTOVERFUNC].string);
			}
		}
	}
	return TM_Ret;
}

int NPC_Lua_Char_SetWatchEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);
	
	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_WATCHFUNC] != (void *)NPC_Lua_WatchCallBack)
		{
			if(TM_char->functable[CHAR_WATCHFUNC] != NULL)
			{
				TM_char->sur_functable[CHAR_WATCHFUNC] = TM_char->functable[CHAR_WATCHFUNC];
				strcpy_s(TM_char->sur_charfunctable[CHAR_WATCHFUNC].string, sizeof(TM_char->sur_charfunctable[CHAR_WATCHFUNC].string), TM_char->charfunctable[CHAR_WATCHFUNC].string);
			}else
			{
				TM_char->sur_functable[CHAR_WATCHFUNC] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_WATCHFUNC].string[0] = '\0';
			}
	
			TM_char->functable[CHAR_WATCHFUNC] = (void *)NPC_Lua_WatchCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_WATCHFUNC].string, sizeof(TM_char->lua_charfunctable[CHAR_WATCHFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_WATCHFUNC].string, sizeof(TM_char->charfunctable[CHAR_WATCHFUNC].string), FUNCNAME_WATCHCALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_WATCHFUNC] == (void *)NPC_Lua_WatchCallBack)
		{
			TM_char->lua_charfunctable[CHAR_WATCHFUNC].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_WATCHFUNC] == NULL)
			{
				TM_char->functable[CHAR_WATCHFUNC] = (void *)NULL;
				TM_char->charfunctable[CHAR_WATCHFUNC].string[0] = '\0';
			}else
			{
				TM_char->functable[CHAR_WATCHFUNC] = TM_char->sur_functable[CHAR_WATCHFUNC];
				TM_char->sur_functable[CHAR_WATCHFUNC] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_WATCHFUNC].string, sizeof(TM_char->charfunctable[CHAR_WATCHFUNC].string), TM_char->sur_charfunctable[CHAR_WATCHFUNC].string);
			}
		}
	}
	return TM_Ret;
}

int NPC_Lua_Char_SetLoopEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 4);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);
	int TM_LoopInterval = (int)lua_tointeger(_NLL, 4);
	int TM_RetLoopInterval = -1;

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);

	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_LOOPFUNC] != (void *)NPC_Lua_LoopCallBack)
		{
			if(TM_char->functable[CHAR_LOOPFUNC] != NULL)
			{
				TM_char->sur_functable[CHAR_LOOPFUNC] = TM_char->functable[CHAR_LOOPFUNC];
				strcpy_s(TM_char->sur_charfunctable[CHAR_LOOPFUNC].string, sizeof(TM_char->sur_charfunctable[CHAR_LOOPFUNC].string), TM_char->charfunctable[CHAR_LOOPFUNC].string);
			}else
			{
				TM_char->sur_functable[CHAR_LOOPFUNC] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_LOOPFUNC].string[0] = '\0';
			}
			TM_RetLoopInterval = CHAR_getInt(TM_index, CHAR_LOOPINTERVAL);
			CHAR_setInt(TM_index, CHAR_LOOPINTERVAL, TM_LoopInterval);
			TM_char->functable[CHAR_LOOPFUNC] = (void *)NPC_Lua_LoopCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_LOOPFUNC].string, sizeof(TM_char->lua_charfunctable[CHAR_LOOPFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_LOOPFUNC].string, sizeof(TM_char->charfunctable[CHAR_LOOPFUNC].string), FUNCNAME_LOOPCALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_LOOPFUNC] == (void *)NPC_Lua_LoopCallBack)
		{
			TM_char->lua_charfunctable[CHAR_LOOPFUNC].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_LOOPFUNC] == NULL)
			{
				CHAR_setInt(TM_index, CHAR_LOOPINTERVAL, -1);
				TM_char->functable[CHAR_LOOPFUNC] = (void *)NULL;
				TM_char->charfunctable[CHAR_LOOPFUNC].string[0] = '\0';
			}else
			{
				CHAR_setInt(TM_index, CHAR_LOOPINTERVAL, TM_LoopInterval);
				TM_char->functable[CHAR_LOOPFUNC] = TM_char->sur_functable[CHAR_LOOPFUNC];
				TM_char->sur_functable[CHAR_LOOPFUNC] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_LOOPFUNC].string, sizeof(TM_char->charfunctable[CHAR_LOOPFUNC].string), TM_char->sur_charfunctable[CHAR_LOOPFUNC].string);
			}
		}
	}
	lua_pushinteger(_NLL, TM_RetLoopInterval);
	LRet(TM_Ret + 1);
}

int NPC_Lua_Char_SetTalkedEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);

	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_TALKEDFUNC] != (void *)NPC_Lua_TalkedCallBack)
		{
			if(TM_char->functable[CHAR_TALKEDFUNC] != NULL)
			{
				TM_char->sur_functable[CHAR_TALKEDFUNC] = TM_char->functable[CHAR_TALKEDFUNC];
				strcpy_s(TM_char->sur_charfunctable[CHAR_TALKEDFUNC].string, sizeof(TM_char->sur_charfunctable[CHAR_TALKEDFUNC].string), TM_char->charfunctable[CHAR_TALKEDFUNC].string);
			}else
			{
				TM_char->sur_functable[CHAR_TALKEDFUNC] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_TALKEDFUNC].string[0] = '\0';
			}
	
			TM_char->functable[CHAR_TALKEDFUNC] = (void *)NPC_Lua_TalkedCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_TALKEDFUNC].string, sizeof(TM_char->lua_charfunctable[CHAR_TALKEDFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_TALKEDFUNC].string, sizeof(TM_char->charfunctable[CHAR_TALKEDFUNC].string), FUNCNAME_TALKEDCALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_TALKEDFUNC] == (void *)NPC_Lua_TalkedCallBack)
		{
			TM_char->lua_charfunctable[CHAR_TALKEDFUNC].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_TALKEDFUNC] == NULL)
			{
				TM_char->functable[CHAR_TALKEDFUNC] = (void *)NULL;
				TM_char->charfunctable[CHAR_TALKEDFUNC].string[0] = '\0';
			}else
			{
				TM_char->functable[CHAR_TALKEDFUNC] = TM_char->sur_functable[CHAR_TALKEDFUNC];
				TM_char->sur_functable[CHAR_TALKEDFUNC] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_TALKEDFUNC].string, sizeof(TM_char->charfunctable[CHAR_TALKEDFUNC].string), TM_char->sur_charfunctable[CHAR_TALKEDFUNC].string);
			}
		}
	}
	return TM_Ret;
}

int NPC_Lua_Char_SetOFFEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);
	
	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_OFFFUNC] != (void *)NPC_Lua_OFFCallBack)
		{
			if(TM_char->functable[CHAR_OFFFUNC] != NULL)
			{
				TM_char->sur_functable[CHAR_OFFFUNC] = TM_char->functable[CHAR_OFFFUNC];
				strcpy_s(TM_char->sur_charfunctable[CHAR_OFFFUNC].string, sizeof(TM_char->sur_charfunctable[CHAR_OFFFUNC].string), TM_char->charfunctable[CHAR_OFFFUNC].string);
			}else
			{
				TM_char->sur_functable[CHAR_OFFFUNC] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_OFFFUNC].string[0] = '\0';
			}
	
			TM_char->functable[CHAR_OFFFUNC] = (void *)NPC_Lua_OFFCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_OFFFUNC].string, sizeof(TM_char->lua_charfunctable[CHAR_OFFFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_OFFFUNC].string, sizeof(TM_char->charfunctable[CHAR_OFFFUNC].string), FUNCNAME_OFFCALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_OFFFUNC] == (void *)NPC_Lua_OFFCallBack)
		{
			TM_char->lua_charfunctable[CHAR_OFFFUNC].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_OFFFUNC] == NULL)
			{
				TM_char->functable[CHAR_OFFFUNC] = (void *)NULL;
				TM_char->charfunctable[CHAR_OFFFUNC].string[0] = '\0';
			}else
			{
				TM_char->functable[CHAR_OFFFUNC] = TM_char->sur_functable[CHAR_OFFFUNC];
				TM_char->sur_functable[CHAR_OFFFUNC] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_OFFFUNC].string, sizeof(TM_char->charfunctable[CHAR_OFFFUNC].string), TM_char->sur_charfunctable[CHAR_OFFFUNC].string);
			}
		}
	}
	return TM_Ret;
}

int NPC_Lua_Char_SetLookedEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);
	
	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_LOOKEDFUNC] != (void *)NPC_Lua_LookedCallBack)
		{
			if(TM_char->functable[CHAR_LOOKEDFUNC] != NULL)
			{
				TM_char->sur_functable[CHAR_LOOKEDFUNC] = TM_char->functable[CHAR_LOOKEDFUNC];
				strcpy_s(TM_char->sur_charfunctable[CHAR_LOOKEDFUNC].string, sizeof(TM_char->sur_charfunctable[CHAR_LOOKEDFUNC].string), TM_char->charfunctable[CHAR_LOOKEDFUNC].string);
			}else
			{
				TM_char->sur_functable[CHAR_LOOKEDFUNC] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_LOOKEDFUNC].string[0] = '\0';
			}
	
			TM_char->functable[CHAR_LOOKEDFUNC] = (void *)NPC_Lua_LookedCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_LOOKEDFUNC].string, sizeof(TM_char->lua_charfunctable[CHAR_LOOKEDFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_LOOKEDFUNC].string, sizeof(TM_char->charfunctable[CHAR_LOOKEDFUNC].string), FUNCNAME_LOOKEDCALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_LOOKEDFUNC] == (void *)NPC_Lua_LookedCallBack)
		{
			TM_char->lua_charfunctable[CHAR_LOOKEDFUNC].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_LOOKEDFUNC] == NULL)
			{
				TM_char->functable[CHAR_LOOKEDFUNC] = (void *)NULL;
				TM_char->charfunctable[CHAR_LOOKEDFUNC].string[0] = '\0';
			}else
			{
				TM_char->functable[CHAR_LOOKEDFUNC] = TM_char->sur_functable[CHAR_LOOKEDFUNC];
				TM_char->sur_functable[CHAR_LOOKEDFUNC] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_LOOKEDFUNC].string, sizeof(TM_char->charfunctable[CHAR_LOOKEDFUNC].string), TM_char->sur_charfunctable[CHAR_LOOKEDFUNC].string);
			}
		}
	}
	return TM_Ret;
}

int NPC_Lua_Char_SetItemPutEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);
	
	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_ITEMPUTFUNC] != (void *)NPC_Lua_ItemPutCallBack)
		{
			if(TM_char->functable[CHAR_ITEMPUTFUNC] != NULL)
			{
				TM_char->sur_functable[CHAR_ITEMPUTFUNC] = TM_char->functable[CHAR_ITEMPUTFUNC];
				strcpy_s(TM_char->sur_charfunctable[CHAR_ITEMPUTFUNC].string, sizeof(TM_char->sur_charfunctable[CHAR_ITEMPUTFUNC].string), TM_char->charfunctable[CHAR_ITEMPUTFUNC].string);
			}else
			{
				TM_char->sur_functable[CHAR_ITEMPUTFUNC] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_ITEMPUTFUNC].string[0] = '\0';
			}
	
			TM_char->functable[CHAR_ITEMPUTFUNC] = (void *)NPC_Lua_ItemPutCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_ITEMPUTFUNC].string, sizeof(TM_char->lua_charfunctable[CHAR_ITEMPUTFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_ITEMPUTFUNC].string, sizeof(TM_char->charfunctable[CHAR_ITEMPUTFUNC].string), FUNCNAME_ITEMPUTCALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_ITEMPUTFUNC] == (void *)NPC_Lua_ItemPutCallBack)
		{
			TM_char->lua_charfunctable[CHAR_ITEMPUTFUNC].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_ITEMPUTFUNC] == NULL)
			{
				TM_char->functable[CHAR_ITEMPUTFUNC] = (void *)NULL;
				TM_char->charfunctable[CHAR_ITEMPUTFUNC].string[0] = '\0';
			}else
			{
				TM_char->functable[CHAR_ITEMPUTFUNC] = TM_char->sur_functable[CHAR_ITEMPUTFUNC];
				TM_char->sur_functable[CHAR_ITEMPUTFUNC] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_ITEMPUTFUNC].string, sizeof(TM_char->charfunctable[CHAR_ITEMPUTFUNC].string), TM_char->sur_charfunctable[CHAR_ITEMPUTFUNC].string);
			}
		}
	}
	return TM_Ret;
}

int NPC_Lua_Char_SetWindowTalkedEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);

	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_WINDOWTALKEDFUNC] != (void *)NPC_Lua_WindowTalkedCallBack)
		{
			if(TM_char->functable[CHAR_WINDOWTALKEDFUNC] != NULL)
			{
				TM_char->sur_functable[CHAR_WINDOWTALKEDFUNC] = TM_char->functable[CHAR_WINDOWTALKEDFUNC];
				strcpy_s(TM_char->sur_charfunctable[CHAR_WINDOWTALKEDFUNC].string, sizeof(TM_char->sur_charfunctable[CHAR_WINDOWTALKEDFUNC].string), TM_char->charfunctable[CHAR_WINDOWTALKEDFUNC].string);
			}else
			{
				TM_char->sur_functable[CHAR_WINDOWTALKEDFUNC] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_WINDOWTALKEDFUNC].string[0] = '\0';
			}

			TM_char->functable[CHAR_WINDOWTALKEDFUNC] = (void *)NPC_Lua_WindowTalkedCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_WINDOWTALKEDFUNC].string, sizeof(TM_char->lua_charfunctable[CHAR_WINDOWTALKEDFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_WINDOWTALKEDFUNC].string, sizeof(TM_char->charfunctable[CHAR_WINDOWTALKEDFUNC].string), FUNCNAME_WINDOWTALKEDCALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_WINDOWTALKEDFUNC] == (void *)NPC_Lua_WindowTalkedCallBack)
		{
			TM_char->lua_charfunctable[CHAR_WINDOWTALKEDFUNC].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_WINDOWTALKEDFUNC] == NULL)
			{
				TM_char->functable[CHAR_WINDOWTALKEDFUNC] = (void *)NULL;
				TM_char->charfunctable[CHAR_WINDOWTALKEDFUNC].string[0] = '\0';
			}else
			{
				TM_char->functable[CHAR_WINDOWTALKEDFUNC] = TM_char->sur_functable[CHAR_WINDOWTALKEDFUNC];
				TM_char->sur_functable[CHAR_WINDOWTALKEDFUNC] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_WINDOWTALKEDFUNC].string, sizeof(TM_char->charfunctable[CHAR_WINDOWTALKEDFUNC].string), TM_char->sur_charfunctable[CHAR_WINDOWTALKEDFUNC].string);
			}
		}
	}
	return TM_Ret;
}

#ifdef _USER_CHARLOOPS
int NPC_Lua_Char_SetCharLoopsEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 4);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);
	int TM_LoopInterval = (int)lua_tointeger(_NLL, 4);
	int TM_RetLoopInterval = -1;

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);

	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_LOOPFUNCTEMP1] != (void *)NPC_Lua_CharLoopsCallBack)
		{
			if(TM_char->functable[CHAR_LOOPFUNCTEMP1] != NULL)
			{
				TM_char->sur_functable[CHAR_LOOPFUNCTEMP1] = TM_char->functable[CHAR_LOOPFUNCTEMP1];
				strcpy_s(TM_char->sur_charfunctable[CHAR_LOOPFUNCTEMP1].string, sizeof(TM_char->sur_charfunctable[CHAR_LOOPFUNCTEMP1].string), TM_char->charfunctable[CHAR_LOOPFUNCTEMP1].string);
			}else
			{
				TM_char->sur_functable[CHAR_LOOPFUNCTEMP1] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_LOOPFUNCTEMP1].string[0] = '\0';
			}
			TM_RetLoopInterval = CHAR_getInt(TM_index, CHAR_LOOPINTERVAL);
			CHAR_setInt(TM_index, CHAR_LOOPINTERVAL, TM_LoopInterval);
			TM_char->functable[CHAR_LOOPFUNCTEMP1] = (void *)NPC_Lua_CharLoopsCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_LOOPFUNCTEMP1].string, sizeof(TM_char->lua_charfunctable[CHAR_LOOPFUNCTEMP1].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_LOOPFUNCTEMP1].string, sizeof(TM_char->charfunctable[CHAR_LOOPFUNCTEMP1].string), FUNCNAME_CHARLOOPSCALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_LOOPFUNCTEMP1] == (void *)NPC_Lua_CharLoopsCallBack)
		{
			TM_char->lua_charfunctable[CHAR_LOOPFUNCTEMP1].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_LOOPFUNCTEMP1] == NULL)
			{
				CHAR_setInt(TM_index, CHAR_LOOPINTERVAL, -1);
				TM_char->functable[CHAR_LOOPFUNCTEMP1] = (void *)NULL;
				TM_char->charfunctable[CHAR_LOOPFUNCTEMP1].string[0] = '\0';
			}else
			{
				CHAR_setInt(TM_index, CHAR_LOOPINTERVAL, TM_LoopInterval);
				TM_char->functable[CHAR_LOOPFUNCTEMP1] = TM_char->sur_functable[CHAR_LOOPFUNCTEMP1];
				TM_char->sur_functable[CHAR_LOOPFUNCTEMP1] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_LOOPFUNCTEMP1].string, sizeof(TM_char->charfunctable[CHAR_LOOPFUNCTEMP1].string), TM_char->sur_charfunctable[CHAR_LOOPFUNCTEMP1].string);
			}
		}
	}
	lua_pushinteger(_NLL, TM_RetLoopInterval);
	LRet(TM_Ret + 1);
}

int NPC_Lua_Char_SetBattleProPertyEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	Char *TM_char = CHAR_getCharPointer(TM_index);

	if(TM_char == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);

	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_char->functable[CHAR_BATTLEPROPERTY] != (void *)NPC_Lua_BattleProPertyCallBack)
		{
			if(TM_char->functable[CHAR_BATTLEPROPERTY] != NULL)
			{
				TM_char->sur_functable[CHAR_BATTLEPROPERTY] = TM_char->functable[CHAR_BATTLEPROPERTY];
				strcpy_s(TM_char->sur_charfunctable[CHAR_BATTLEPROPERTY].string, sizeof(TM_char->sur_charfunctable[CHAR_BATTLEPROPERTY].string), TM_char->charfunctable[CHAR_BATTLEPROPERTY].string);
			}else
			{
				TM_char->sur_functable[CHAR_BATTLEPROPERTY] = (void *)NULL;
				TM_char->sur_charfunctable[CHAR_BATTLEPROPERTY].string[0] = '\0';
			}

			TM_char->functable[CHAR_BATTLEPROPERTY] = (void *)NPC_Lua_BattleProPertyCallBack;
			strcpy_s(TM_char->lua_charfunctable[CHAR_BATTLEPROPERTY].string, sizeof(TM_char->lua_charfunctable[CHAR_BATTLEPROPERTY].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_char->charfunctable[CHAR_BATTLEPROPERTY].string, sizeof(TM_char->charfunctable[CHAR_BATTLEPROPERTY].string), FUNCNAME_BATTLEPROPERTYCALLBACK);
		}
	}else
	{
		if(TM_char->functable[CHAR_BATTLEPROPERTY] == (void *)NPC_Lua_BattleProPertyCallBack)
		{
			TM_char->lua_charfunctable[CHAR_BATTLEPROPERTY].string[0] = '\0';
			if(TM_char->sur_functable[CHAR_BATTLEPROPERTY] == NULL)
			{
				TM_char->functable[CHAR_BATTLEPROPERTY] = (void *)NULL;
				TM_char->charfunctable[CHAR_BATTLEPROPERTY].string[0] = '\0';
			}else
			{
				TM_char->functable[CHAR_BATTLEPROPERTY] = TM_char->sur_functable[CHAR_BATTLEPROPERTY];
				TM_char->sur_functable[CHAR_BATTLEPROPERTY] = (void *)NULL;
				strcpy_s(TM_char->charfunctable[CHAR_BATTLEPROPERTY].string, sizeof(TM_char->charfunctable[CHAR_BATTLEPROPERTY].string), TM_char->sur_charfunctable[CHAR_BATTLEPROPERTY].string);
			}
		}
	}
	return TM_Ret;
}
#endif
//////////////////////////////////////////////////////////////////////////////
//获取数据的接口
int NPC_Lua_Char_GetData(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Flg = (int)lua_tointeger(_NLL, 2);

	if(TM_Flg >= 0)
	{
		if(TM_Flg < LUA_DATALINE1)
		{
			TM_Flg -= LUA_DATALINE0;
			if(TM_Flg == CHAR_WHICHTYPE)
			{
				LRetInt(_NLL, 0);
			}
			int TM_RetInt = CHAR_getInt(TM_Index, TM_Flg);
			LRetInt(_NLL, TM_RetInt);
		}else if(TM_Flg < LUA_DATALINE2)
		{
			TM_Flg -= LUA_DATALINE1;
			char *TM_RetPoint = CHAR_getChar(TM_Index, TM_Flg);
			LRetMsg(_NLL, TM_RetPoint);
		}else if(TM_Flg < LUA_DATALINE3)
		{
			TM_Flg -= LUA_DATALINE2;
			int TM_RetWorkInt = CHAR_getWorkInt(TM_Index, TM_Flg);
			LRetInt(_NLL, TM_RetWorkInt);
		}else if(TM_Flg < LUA_DATALINE4)
		{
			TM_Flg -= LUA_DATALINE3;
			char *TM_RetWorkPoint = CHAR_getWorkChar(TM_Index, TM_Flg);
			LRetMsg(_NLL, TM_RetWorkPoint);
		}else if(TM_Flg < LUA_DATALINE5)
		{
			TM_Flg -= LUA_DATALINE4;
			BOOL TM_RetBOOL = CHAR_getFlg(TM_Index, TM_Flg);
			LRetBool(_NLL, TM_RetBOOL);
		}else if(TM_Flg < LUA_DATALINE6){
			TM_Flg -= LUA_DATALINE5;
			if(TM_Flg==1){
				LRetInt(_NLL, sasql_vippoint(CHAR_getUseID(TM_Index),0,0));
			}else LRetErrNull(_NLL, "传入的标志是错误的。");
		}else
		{
			LRetErrNull(_NLL, "传入的标志是错误的。");
		}
	}
	LRetErrNull(_NLL, "传入的标志是错误的。");
}

int NPC_Lua_Char_IsEventEnd(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_EventNo = (int)lua_tointeger(_NLL, 2);
	
	BOOL TM_Ret = NPC_EventCheckFlg(TM_Index, TM_EventNo);

	LRetBool(_NLL, TM_Ret);
}

int NPC_Lua_Char_IsEventNow(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_EventNo = (int)lua_tointeger(_NLL, 2);
	
	BOOL TM_Ret = NPC_NowEventCheckFlg(TM_Index, TM_EventNo);

	LRetBool(_NLL, TM_Ret);
}

int NPC_Lua_Char_FindItemId(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_ItemId = (int)lua_tointeger(_NLL, 2);
	int TM_ItemIndex = -1;
	int i = 0;
	int j = 0;
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	int itemMax =  CheckCharMaxItem(TM_Index);
	for( i = CHAR_STARTITEMARRAY ; i<itemMax ; i++ )
	{
		TM_ItemIndex = CHAR_getItemIndex(TM_Index, i);
		if( ITEM_CHECKINDEX(TM_ItemIndex) == FALSE )continue;
		if( ITEM_getInt(TM_ItemIndex, ITEM_ID) == TM_ItemId) {
			//LRetInt(_NLL, i);
			j++;
		}
	}
	LRetInt(_NLL, j);
	//LRetErrInt(_NLL, -2, "找不到指定道具。");
}

int NPC_Lua_Char_VipPoint(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Point = (int)lua_tointeger(_NLL, 2);
	int TM_Flag = (int)lua_tointeger(_NLL, 3);
	char *TM_Cdkey;
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	
	TM_Cdkey = CHAR_getChar(TM_Index,CHAR_CDKEY);

	int TM_MyPoint = sasql_vippoint(TM_Cdkey,TM_Point,TM_Flag);
	
	if(TM_MyPoint<0){
		LRetErrInt(_NLL, -2, "数据出错");
	}
	LRetInt(_NLL, TM_MyPoint);
}

int NPC_Lua_Char_FindPetEnemyId(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_PetId = (int)lua_tointeger(_NLL, 2);
	int TM_PetIndex = -1;
	int i = 0;

	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	for( i = 0 ; i<CHAR_MAXPETHAVE ; i++ )
	{
		TM_PetIndex = CHAR_getCharPet( TM_Index, i);
		if( CHAR_CHECKINDEX(TM_PetIndex) == FALSE )continue;
		if( CHAR_getInt(TM_PetIndex, CHAR_PETID) == TM_PetId) {
			LRetInt(_NLL, i);
		}
	}
	LRetErrInt(_NLL, -2, "找不到指定宠物。");
}

int NPC_Lua_Char_GetItemId(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Pos = (int)lua_tointeger(_NLL, 2);

	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	int itemMax =  CheckCharMaxItem(TM_Index);
	if(TM_Pos >= 0 && TM_Pos < itemMax)
	{
		int TM_ItemIndex = CHAR_getItemIndex(TM_Index, TM_Pos);
		
		if( ITEM_CHECKINDEX(TM_ItemIndex) == FALSE ) LRetErrInt(_NLL, -2, "该道具栏没有道具。");
		LRetInt(_NLL, ITEM_getInt(TM_ItemIndex, ITEM_ID));
	}
	LRetErrInt(_NLL, -3, "传入的位置超出道具栏范围。");
}

int NPC_Lua_Char_GetItemIndex(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Pos = (int)lua_tointeger(_NLL, 2);

	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	int itemMax =  CheckCharMaxItem(TM_Index);
	if(TM_Pos >= 0 && TM_Pos < itemMax)
	{
		int TM_ItemIndex = CHAR_getItemIndex(TM_Index, TM_Pos);
		
		if( ITEM_CHECKINDEX(TM_ItemIndex) == FALSE ) LRetErrInt(_NLL, -2, "该道具栏没有道具。");
		LRetInt(_NLL, TM_ItemIndex);
	}
	LRetErrInt(_NLL, -3, "传入的位置超出道具栏范围。");
}

int NPC_Lua_Char_GetPetEnemyId(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Pos = (int)lua_tointeger(_NLL, 2);

	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	if(TM_Pos >= 0 && TM_Pos < CHAR_MAXPETHAVE)
	{
		int TM_PetIndex = CHAR_getCharPet(TM_Index, TM_Pos);
		
		if( CHAR_CHECKINDEX(TM_PetIndex) == FALSE ) LRetErrInt(_NLL, -2, "该宠物栏没有宠物。");
		LRetInt(_NLL, CHAR_getInt(TM_PetIndex, CHAR_PETID));
	}
	LRetErrInt(_NLL, -3, "传入的位置超出宠物栏范围。");
}

int NPC_Lua_Char_GetPetIndex(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Pos = (int)lua_tointeger(_NLL, 2);

	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	if(TM_Pos >= 0 && TM_Pos < CHAR_MAXPETHAVE)
	{
		int TM_PetIndex = CHAR_getCharPet(TM_Index, TM_Pos);
		
		if( CHAR_CHECKINDEX(TM_PetIndex) == FALSE ) LRetErrInt(_NLL, -2, "该宠物栏没有宠物。");
		LRetInt(_NLL, TM_PetIndex);
	}
	LRetErrInt(_NLL, -3, "传入的位置超出宠物栏范围。");
}

int NPC_Lua_Char_GetTeamIndex(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Pos = (int)lua_tointeger(_NLL, 2);

	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	if(TM_Pos >= 1 && TM_Pos < getPartyNum(TM_Index))
	{
		int TM_ParIndex = CHAR_getWorkInt( TM_Index, TM_Pos + CHAR_WORKPARTYINDEX1);

		if( CHAR_CHECKINDEX(TM_ParIndex) == FALSE ) LRetErrInt(_NLL, -2, "该位置没有队员。");
		LRetInt(_NLL, TM_ParIndex);
	}
	LRetErrInt(_NLL, -3, "传入的位置超出队员位置范围。");
}

int NPC_Lua_Char_HealAll(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);

	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	NPC_HealerAllHeal(TM_Index);
	LRetInt(_NLL, 1);
}


int NPC_Lua_Char_GetPetSkillId(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_SkillIndex = (int)lua_tointeger(_NLL, 2);

	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	if( !CHAR_CHECKPETSKILLINDEX( TM_SkillIndex)){
	 	LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	
	int TM_SkillId=CHAR_getPetSkill( TM_Index, TM_SkillIndex);
	LRetInt(_NLL, TM_SkillId);
}

int NPC_Lua_Char_GetPetSkillName(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int skillarray = PETSKILL_getPetskillArray( TM_Index);
	if( PETSKILL_CHECKINDEX( skillarray) == FALSE ){
		LRetErrNull(_NLL, "传入的标志是错误的。");
	}
	LRetMsg(_NLL, PETSKILL_getChar( skillarray, PETSKILL_NAME));
}

int NPC_Lua_Char_GetPetSkillMsg(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int skillarray = PETSKILL_getPetskillArray( TM_Index);
	if( PETSKILL_CHECKINDEX( skillarray) == FALSE ){
		LRetErrNull(_NLL, "传入的标志是错误的。");
	}
	LRetMsg(_NLL, PETSKILL_getChar( skillarray, PETSKILL_COMMENT));
}

int NPC_Lua_Char_SetPetSkill(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Slod = (int)lua_tointeger(_NLL, 2);
	int TM_SkillId = (int)lua_tointeger(_NLL, 3);
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	if( !CHAR_CHECKPETSKILLINDEX( TM_Slod)){
	 	LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	int skillarray = PETSKILL_getPetskillArray( TM_SkillId);
	if( PETSKILL_CHECKINDEX( skillarray) == FALSE ){
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	CHAR_setPetSkill( TM_Index, TM_Slod, TM_SkillId);
	LRetInt(_NLL, 1);
}
/*
int NPC_Lua_Char_RandMsg(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_NpcIndex = (int)lua_tointeger(_NLL, 1);
	int TM_TalkIndex = (int)lua_tointeger(_NLL, 2);
	time_t timep;
	struct tm *p;
	time(&timep);
	p=localtime(&timep);
	timep = mktime(p);
	char *randtypetime = CHAR_getChar(TM_TalkIndex,CHAR_RANDTYPE);
	char tempbuff[64];
	int randtype = 0;
	int randtime = 0;
	int randwrong = 0;
	if(strlen(randtypetime)>2){
		if(getStringFromIndexWithDelim(randtypetime,"-", 3, tempbuff, sizeof(tempbuff)) != FALSE){
			if(getStringFromIndexWithDelim(randtypetime,"-", 1, tempbuff, sizeof(tempbuff))== FALSE)
				LRetInt(_NLL, 0);
			randwrong = atoi(tempbuff);
			if(getStringFromIndexWithDelim(randtypetime,"-", 2, tempbuff, sizeof(tempbuff))== FALSE)
				LRetInt(_NLL, 0);
			randtype = atoi(tempbuff);
			if(getStringFromIndexWithDelim(randtypetime,"-", 3, tempbuff, sizeof(tempbuff))== FALSE)
				LRetInt(_NLL, 0);
			randtime = atoi(tempbuff);
			if(timep - randtime < getEditBaseTime()*randwrong){
				char errbuf[256];
				sprintf(errbuf,"您还有%d秒才可以继续此操作！",getEditBaseTime()*randwrong-(timep - randtime));
				CHAR_talkToCli(TM_TalkIndex, -1, errbuf, CHAR_COLORRED);
				LRetInt(_NLL, 0);
			}
		}
	}
	char *arg = NULL;
	arg = CHAR_getWorkChar(TM_TalkIndex,CHAR_WORKRANDMSG);
	int MyNpcIndex,MyTalkIndex,ret,flag;
	if(strlen(arg)>2){
		char buftest[255];
		if(getStringFromIndexWithDelim(arg,"|", 1, buftest, sizeof(buftest)) == FALSE){
			CHAR_setWorkChar(TM_TalkIndex,CHAR_WORKRANDMSG,"");
			LRetInt(_NLL, 0);;
		}
		ret = atoi(buftest);
		if(getStringFromIndexWithDelim(arg,"|", 2, buftest, sizeof(buftest)) == FALSE){
			CHAR_setWorkChar(TM_TalkIndex,CHAR_WORKRANDMSG,"");
			LRetInt(_NLL, 0);;
		}
		MyNpcIndex = atoi(buftest);
		if(getStringFromIndexWithDelim(arg,"|", 3, buftest, sizeof(buftest)) == FALSE){
			CHAR_setWorkChar(TM_TalkIndex,CHAR_WORKRANDMSG,"");
			LRetInt(_NLL, 0);;
		}
		MyTalkIndex = atoi(buftest);
		if(getStringFromIndexWithDelim(arg,"|", 4, buftest, sizeof(buftest)) == FALSE){
			CHAR_setWorkChar(TM_TalkIndex,CHAR_WORKRANDMSG,"");
			LRetInt(_NLL, 0);;
		}
		flag = atoi(buftest);
		
		if(TM_NpcIndex!=MyNpcIndex || TM_TalkIndex!=MyTalkIndex){
			CHAR_setWorkChar(TM_TalkIndex,CHAR_WORKRANDMSG,"");
			LRetInt(_NLL, 0);
		}
		
		if(flag == 0){
			CHAR_setWorkChar(TM_TalkIndex,CHAR_WORKRANDMSG,"");
			LRetInt(_NLL, 2);
		}
		CHAR_setWorkChar(TM_TalkIndex,CHAR_WORKRANDMSG,"");
		LRetInt(_NLL, 1);
	}else{
		char tempbuf[64];
		char randquestion[64];
		char randrightanswer[64];
		char randwronganswer1[64];
		char randwronganswer2[64];
		char randwronganswer3[64];
		char randwronganswer4[64];
		memset(tempbuf, 0, sizeof(tempbuf));
		memset(randquestion, 0, sizeof(randquestion));
		memset(randrightanswer, 0, sizeof(randrightanswer));
		memset(randwronganswer1, 0, sizeof(randwronganswer1));
		memset(randwronganswer2, 0, sizeof(randwronganswer2));
		memset(randwronganswer3, 0, sizeof(randwronganswer3));
		memset(randwronganswer4, 0, sizeof(randwronganswer4));
		if(RAND(1,100)<=75){
					char randtmpbuf[4][10];
					sprintf(randtmpbuf[0],"%s",hanzibuf[RAND(1,5000)-1]);
					sprintf(randtmpbuf[1],"%s",hanzibuf[RAND(1,5000)-1]);
					sprintf(randtmpbuf[2],"%s",hanzibuf[RAND(1,5000)-1]);
					sprintf(randtmpbuf[3],"%s",hanzibuf[RAND(1,5000)-1]);
					sprintf(randquestion,"%s",str_hanzi_ganrao(randtmpbuf[0],randtmpbuf[1],randtmpbuf[2],randtmpbuf[3],0,0,0,0,0,0));
					strcat(randquestion,"（请找出相同词组）");
					int newrand1 = RAND(1,100)-1;
					int newrand2 = RAND(1,100)-1;
					int newrand3 = RAND(1,100)-1;
					int newrand4 = RAND(1,100)-1;
					int newrand5 = RAND(1,100)-1;
					sprintf(randrightanswer,"%s",str_hanzi_ganrao(randtmpbuf[0],randtmpbuf[1],randtmpbuf[2],randtmpbuf[3],1,newrand1,newrand2,newrand3,newrand4,newrand5));
					sprintf(randwronganswer1,"%s",str_hanzi_ganrao(randtmpbuf[0],randtmpbuf[1],randtmpbuf[3],randtmpbuf[2],1,newrand1,newrand2,newrand3,newrand4,newrand5));
					sprintf(randwronganswer2,"%s",str_hanzi_ganrao(randtmpbuf[3],randtmpbuf[1],randtmpbuf[2],randtmpbuf[0],1,newrand1,newrand2,newrand3,newrand4,newrand5));
					sprintf(randwronganswer3,"%s",str_hanzi_ganrao(randtmpbuf[1],randtmpbuf[0],randtmpbuf[2],randtmpbuf[3],1,newrand1,newrand2,newrand3,newrand4,newrand5));
					sprintf(randwronganswer4,"%s",str_hanzi_ganrao(randtmpbuf[2],randtmpbuf[3],randtmpbuf[0],randtmpbuf[1],1,newrand1,newrand2,newrand3,newrand4,newrand5));
					
		}else{
			int tii;
			char timubuf[36][5] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","0","1","2","3","4","5","6","7","8","9"};
			for(tii=0;tii<5;tii++){
				if(tii==0)
					strcpy(tempbuf,timubuf[RAND(1,36)-1]);
				else{
					int timunum = 0;
					while(1){
						timunum = RAND(1,36)-1;
						if(strstr(tempbuf,timubuf[timunum])==NULL)
							break;
					}
					strcat(tempbuf,timubuf[timunum]);
				}

			}
			sprintf(randwronganswer1,"[%c%c%c%c%c]",tempbuf[0],tempbuf[2],tempbuf[1],tempbuf[3],tempbuf[4]);
			sprintf(randwronganswer2,"[%c%c%c%c%c]",tempbuf[0],tempbuf[1],tempbuf[4],tempbuf[2],tempbuf[3]);
			sprintf(randwronganswer3,"[%c%c%c%c%c]",tempbuf[4],tempbuf[1],tempbuf[2],tempbuf[0],tempbuf[3]);
			sprintf(randwronganswer4,"[%c%c%c%c%c]",tempbuf[1],tempbuf[0],tempbuf[2],tempbuf[3],tempbuf[4]);
			sprintf(randquestion,"[%s]（请找出相同英文）",tempbuf);
			sprintf(randrightanswer,"[%s]",tempbuf);
			char* randstr = NULL;
			randstr = str_ganrao(randquestion);
			strcpy(randquestion,randstr);
			randstr = str_ganraoan(randrightanswer);
			strcpy(randrightanswer,randstr);
			randstr = str_ganraoan(randwronganswer1);
			strcpy(randwronganswer1,randstr);
			randstr = str_ganraoan(randwronganswer2);
			strcpy(randwronganswer2,randstr);
			randstr = str_ganraoan(randwronganswer3);
			strcpy(randwronganswer3,randstr);
			randstr = str_ganraoan(randwronganswer4);
			strcpy(randwronganswer4,randstr);
		}
		int n,m;
		int randnum1;
		m=1;
		char tempret[5][64];
		randnum1 = RAND(1,5);
		int rightnum,wrongnum1,wrongnum2,wrongnum3,wrongnum4;
		rightnum = RAND(getRandMin(),getRandMax());
		wrongnum1 = RAND(getRandMin(),getRandMax());
		wrongnum2 = RAND(getRandMin(),getRandMax());
		wrongnum3 = RAND(getRandMin(),getRandMax());
		wrongnum4 = RAND(getRandMin(),getRandMax());
		int qianhounum = RAND(1,100);
		for(n=1;n<=5;n++){
			if(n==randnum1){
				if(qianhounum<=50)
					sprintf(tempret[n-1],"选项( %d %s )",rightnum,randrightanswer);
				else
					sprintf(tempret[n-1],"选项( %s %d )",randrightanswer,rightnum);
			}else{
				if(m==1){
					if(qianhounum<=50)
						sprintf(tempret[n-1],"选项( %d %s )",wrongnum1,randwronganswer1);
					else
						sprintf(tempret[n-1],"选项( %s %d )",randwronganswer1,wrongnum1);
					m = 2;
				}else if(m==2){
					if(qianhounum<=50)
						sprintf(tempret[n-1],"选项( %d %s )",wrongnum2,randwronganswer2);
					else
						sprintf(tempret[n-1],"选项( %s %d )",randwronganswer2,wrongnum2);
					m = 3;
				}else if(m==3){
					if(qianhounum<=50)
						sprintf(tempret[n-1],"选项( %d %s )",wrongnum3,randwronganswer3);
					else
						sprintf(tempret[n-1],"选项( %s %d )",randwronganswer3,wrongnum3);
					m = 4;
				}else if(m==4){
					if(qianhounum<=50)
						sprintf(tempret[n-1],"选项( %d %s )",wrongnum4,randwronganswer4);
					else
						sprintf(tempret[n-1],"选项( %s %d )",randwronganswer4,wrongnum4);
					m = 5;
				}
			}
		}
		int windowtype = 0;
		int selecttype = RAND(1,100);
		char buf[64];
		if(selecttype<=30){
			sprintf(buf,"%d|%d|%d|%d",rightnum,TM_NpcIndex,TM_TalkIndex,0);
			windowtype = WINDOW_MESSAGETYPE_MESSAGEANDLINEINPUT;
		}else{
			sprintf(buf,"%d|%d|%d|%d",randnum1,TM_NpcIndex,TM_TalkIndex,0);
			windowtype = WINDOW_MESSAGETYPE_SELECT;
		}
		CHAR_setWorkChar(TM_TalkIndex, CHAR_WORKRANDMSG, buf);
		int fd = getfdFromCharaIndex( TM_TalkIndex );
		sprintf(buf,"%s\n%s\n%s\n%s\n%s\n%s\n输入正确答案括号内的5位数字或者点选答案",randquestion,tempret[0],tempret[1],tempret[2],tempret[3],tempret[4]);
		lssproto_WN_send( fd, windowtype, 
			WINDOW_BUTTONTYPE_OK,
			CHAR_WINDOWTYPE_LUANPC_RANDMSG,
			-1,
			buf);
		CHAR_setWorkInt(TM_TalkIndex,CHAR_WORKLUANPCINT,0);
		if(randwrong<=0){
			sprintf(tempbuff,"1-%d-%d",randtype,timep);
		}else{
			sprintf(tempbuff,"%d-%d-%d",randwrong*3,randtype,timep);
		}
		CHAR_setChar(TM_TalkIndex,CHAR_RANDTYPE,tempbuff);
		LRetInt(_NLL, 0);
	}
}
*/
int NPC_Lua_Char_GetBeatitudeNum(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	int petbeatitude = CHAR_getInt( TM_Index, CHAR_BEATITUDE);
	int i=0;
	if(CHAR_getInt( TM_Index, CHAR_BEATITUDE)&BEATITUDE_VITAL)
		i++;
	if(CHAR_getInt( TM_Index, CHAR_BEATITUDE)&BEATITUDE_STR)
		i++;
	if(CHAR_getInt( TM_Index, CHAR_BEATITUDE)&BEATITUDE_TOUGH)
		i++;
	if(CHAR_getInt( TM_Index, CHAR_BEATITUDE)&BEATITUDE_DEX)
		i++;
	LRetInt(_NLL, i);
}

//通过帐号查询人物索引
int NPC_Lua_Char_GetPlayerIndexByCdkey(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	char *TM_Cdkey = (char *)lua_tostring(_NLL, 1);

	int TM_playernum = CHAR_getPlayerMaxNum();
	int i = 0;
	for( i = 0 ; i < TM_playernum ; i++)
	{
		if( CHAR_getCharUse(i) != FALSE )
		{
			if (CHAR_getChar(i, CHAR_CDKEY) == TM_Cdkey)
			{
				LRetInt(_NLL, i);
			}
		}
	}

	LRetErrInt(_NLL, -3, "该帐号不存在或者未上线！");
}

int NPC_Lua_Char_getVipPoint(lua_State *_NLL)
{
  CheckEx(_NLL, 1);
  int TM_Index = (int)lua_tointeger(_NLL, 1);
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}  
   int TM_point = sasql_getVipPoint(TM_Index);
	if(TM_point<0){
		LRetErrInt(_NLL, -2, "Char_getVipPoint数据出错");
	}
  LRetInt(_NLL, TM_point);
}

/*
int NPC_Lua_Char_getjfPoint(lua_State *_NLL)
{
  CheckEx(_NLL, 1);
  int TM_Index = (int)lua_tointeger(_NLL, 1);
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}  
   int TM_point = sasql_jifenPoint(TM_Index);
	if(TM_point<0){
		LRetErrInt(_NLL, -2, "Char_getjfPoint数据出错");
	}
  LRetInt(_NLL, TM_point);
}

int NPC_Lua_Char_getxjPoint(lua_State *_NLL)
{
  CheckEx(_NLL, 1);
  int TM_Index = (int)lua_tointeger(_NLL, 1);
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}  
   int TM_point = sasql_xjPoint(TM_Index);
	if(TM_point<0){
		LRetErrInt(_NLL, -2, "Char_getxjPoint数据出错");
	}
  LRetInt(_NLL, TM_point);
}
*/

int NPC_Lua_Char_setVipPoint(lua_State *_NLL)
{
  CheckEx(_NLL, 2);
  int TM_Index = (int)lua_tointeger(_NLL, 1);
  int TM_point = (int)lua_tointeger(_NLL, 2);
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}  
   int TM_kouj = sasql_setVipPoint(TM_Index,TM_point);
	if(TM_point<0){
		LRetErrInt(_NLL, -2, "Char_setVipPoint数据出错");
	}
  LRetInt(_NLL, TM_kouj);
}
/*
int NPC_Lua_Char_setjfPoint(lua_State *_NLL)
{
  CheckEx(_NLL, 2);
  int TM_Index = (int)lua_tointeger(_NLL, 1);
  int TM_point = (int)lua_tointeger(_NLL, 2);
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}  
   int TM_kouj = sasql_setJfPoint(TM_Index,TM_point);
	if(TM_point<0){
		LRetErrInt(_NLL, -2, "Char_setjfPoint数据出错");
	}
  LRetInt(_NLL, TM_kouj);
}

int NPC_Lua_Char_setxjPoint(lua_State *_NLL)
{
  CheckEx(_NLL, 2);
  int TM_Index = (int)lua_tointeger(_NLL, 1);
  int TM_point = (int)lua_tointeger(_NLL, 2);
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}  
   int TM_kouj = sasql_setxjPoint(TM_Index,TM_point);
	if(TM_point<0){
		LRetErrInt(_NLL, -2, "Char_setxjPoint数据出错");
	}
  LRetInt(_NLL, TM_kouj);
}
*/
int NPC_Lua_Char_GetAllocPoint(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int PET_Index = (int)lua_tointeger(_NLL, 1);
	int PET_flg = (int)lua_tointeger(_NLL, 2);
	int PET_Ret = -1;
	if(!CHAR_CHECKINDEX(PET_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	PET_Ret = CHAR_getInt( PET_Index, CHAR_ALLOCPOINT);
	switch (PET_flg)
	{
		case 0:
		PET_Ret = ((PET_Ret>> 0) & 0xFF);
		break;
		case 1:
		PET_Ret = ((PET_Ret>> 8) & 0xFF);
		break;
		case 2:
		PET_Ret = ((PET_Ret>> 16) & 0xFF);
		break;
		case 3:
		PET_Ret = ((PET_Ret>>24) & 0xFF);
		break;
	}
	LRetInt(_NLL, PET_Ret);
}

int NPC_Lua_Char_GetDataFromEnemyBase(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int PET_ID = (int)lua_tointeger(_NLL, 1);
	int PET_flg = (int)lua_tointeger(_NLL, 2);
	if(ENEMYTEMP_getEnemyTempArrayFromTempNo(PET_ID) == -1)
	{
		LRetErrInt(_NLL, -1, "传入的EnemyBase宠物ID不存在。");
	}
	PET_ID = ENEMY_getEnemyArrayFromTempNo( PET_ID);
	PET_ID = ENEMYTEMP_getEnemyTempArray( PET_ID);
	if(PET_flg >= 0)
	{
		if(PET_flg >= LUA_DATALINE1)
		{
			PET_flg = PET_flg - LUA_DATALINE1;
			char *TM_RetPoint = ENEMYTEMP_getChar( PET_ID, PET_flg);
			LRetMsg(_NLL, TM_RetPoint);
		}
		else 
		{
			int PET_Ret = ENEMYTEMP_getInt( PET_ID, PET_flg);
			LRetInt(_NLL, PET_Ret);
		}
	}
	else 
	{
		LRetErrInt(_NLL, -1, "传入的标志是错误的。");
	}
}

int NPC_Lua_Char_GetPlayerIndex(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	char *TM_PlayerID = lua_tostring(_NLL, 1);

		if(TM_PlayerID == NULL )
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	
	int TM_playernum = CHAR_getPlayerMaxNum();
		int i;
		for( i = 0 ; i < TM_playernum ; i++){
			if( CHAR_getCharUse(i) != FALSE ){
				char *TM_ServerPlayerID = CHAR_getChar(i, CHAR_CDKEY);
				if (strcmp(TM_ServerPlayerID,TM_PlayerID)==0)
				{
					LRetInt(_NLL, i);
				}
			}
		}
         LRetErrNull(_NLL, "传入的标志是错误的。");
}

int NPC_Lua_Char_FindItemIndex(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	CheckIndexNull(_NLL, 2);

	int TM_CharaIndex = (int)lua_tointeger(_NLL, 1);
	int TM_ItemIndex = (int)lua_tointeger(_NLL, 2);
	int tmpTM_ItemIndex = 0;
	
	if(!CHAR_CHECKINDEX(TM_CharaIndex))
	{
		LRetErrInt(_NLL, -1, "传入人物的索引是无效的。");
	}
	if(!ITEM_CHECKINDEX(TM_ItemIndex))
	{
		LRetErrInt(_NLL, -1, "传入道具的索引是无效的。");
	}
	int itemMax =  CheckCharMaxItem(TM_CharaIndex);
	int i = 0;
	for( i = 0 ; i<itemMax ; i++ )
	{
		tmpTM_ItemIndex = CHAR_getItemIndex(TM_CharaIndex, i);
		if( ITEM_CHECKINDEX(tmpTM_ItemIndex) == FALSE )
			continue;
		if( tmpTM_ItemIndex == TM_ItemIndex ) {
			LRetInt(_NLL, i);
		}
	}
 
	LRetErrInt(_NLL, -1, "没有找到对应 ItemIndex 的道具");
	
}

int NPC_Lua_Char_FindPetIndex(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	CheckIndexNull(_NLL, 2);

	int TM_CharaIndex = (int)lua_tointeger(_NLL, 1);
	int TM_PetIndex = (int)lua_tointeger(_NLL, 2);
	int tmpTM_PetIndex = 0;
	
	if(!CHAR_CHECKINDEX(TM_CharaIndex))
	{
		LRetErrInt(_NLL, -1, "传入人物的索引是无效的。");
	}
	if(!CHAR_CHECKINDEX(TM_PetIndex))
	{
		LRetErrInt(_NLL, -1, "传入宠物的索引是无效的。");
	}
	
	int i = 0;
	for( i = 0 ; i<CHAR_MAXPETHAVE ; i++ )
	{
		tmpTM_PetIndex = CHAR_getCharPet(TM_CharaIndex, i);
		if( CHAR_CHECKINDEX(tmpTM_PetIndex) == FALSE )
			continue;
		if( tmpTM_PetIndex == TM_PetIndex ) {
			LRetInt(_NLL, i);
		}
	}
 
	LRetErrInt(_NLL, -1, "没有找到对应 PetIndex 的宠物");
}

int NPC_Lua_CHAR_CHECKINDEX(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	int Charindex = (int)lua_tointeger(_NLL, 1);
	if(Charindex == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	BOOL TM_Ret = CHAR_CHECKINDEX(Charindex);
	LRetBool(_NLL, TM_Ret);
}

int NPC_Lua_Char_GetPetSkill(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	CheckIndexNull(_NLL, 2);
	
	int TM_PetIndex = (int)lua_tointeger(_NLL, 1);
	int TM_PetSkillIndex = (int)lua_tointeger(_NLL, 2);

	if(CHAR_CHECKPETSKILLINDEX(TM_PetSkillIndex)){	
		LRetInt(_NLL, CHAR_getPetSkill(TM_PetIndex,TM_PetSkillIndex));
	}else{
		LRetErrInt(_NLL, -1, "传入的位置超出宠物技能范围。");
	}
}

int NPC_Lua_Char_GetPetSkillNo(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	CheckIndexNull(_NLL, 2);
	
	int TM_PetIndex = (int)lua_tointeger(_NLL, 1);
	int TM_PetSkillIndex = (int)lua_tointeger(_NLL, 2);

	if(CHAR_CHECKPETSKILLINDEX(TM_PetSkillIndex)){	
		LRetInt(_NLL, CHAR_getPetSkill(TM_PetIndex,TM_PetSkillIndex));
	}else{
		LRetErrInt(_NLL, -1, "传入的位置超出宠物技能范围。");
	}
}

int NPC_Lua_Char_GetEmptyItemBoxNum(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);

	int TM_CharIndex = (int)lua_tointeger(_NLL, 1);

	if(!CHAR_CHECKINDEX(TM_CharIndex))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	LRetInt(_NLL, CHAR_getEmptyItemBoxNum(TM_CharIndex));
}

int NPC_Lua_Char_GetEmptyPetBoxNum(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);

	int TM_CharIndex = (int)lua_tointeger(_NLL, 1);

	if(!CHAR_CHECKINDEX(TM_CharIndex))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	LRetInt(_NLL, CHAR_getEmptyPetBoxNum(TM_CharIndex));
}

int NPC_Lua_Char_GetPlayerPetNum(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	CheckIndexNull(_NLL, 2);

	int TM_CharaIndex = (int)lua_tointeger(_NLL, 1);
	int TM_Petid = (int)lua_tointeger(_NLL, 2);

	if(!CHAR_CHECKINDEX(TM_CharaIndex))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	LRetInt(_NLL, CHAR_getPlayerPetNum(TM_CharaIndex,TM_Petid));
	
	
}

int NPC_Lua_Char_GetPlayerItemNum(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, LUA_MINSTACK);
	CheckIndexNull(_NLL, 1);
	CheckIndexNull(_NLL, 2);
	int TM_CharaIndex = (int)lua_tointeger(_NLL, 1);
	int TM_ItemID = (int)lua_tointeger(_NLL, 2);
	int TM_Max = lua_gettop(_NLL);
	BOOL TM_IsContainEquip = FALSE;
	BOOL TM_IsContainPile = FALSE;
	if( 3 <= TM_Max)
	{
		TM_IsContainEquip = (BOOL)lua_toboolean(_NLL, 3);
	}
	if( 4 <= TM_Max)
	{
		TM_IsContainPile = (BOOL)lua_toboolean(_NLL, 4);
	}
	
	if(!CHAR_CHECKINDEX(TM_CharaIndex))
	{
			LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	LRetInt(_NLL, CHAR_getPlayerItemNum(TM_CharaIndex,TM_ItemID,TM_IsContainEquip,TM_IsContainPile));
	
	
}

int NPC_Lua_Char_RidNo(lua_State *_NLL)
{
	CheckEx2(_NLL, 2, LUA_MINSTACK);
	CheckIndexNull(_NLL, 1);
	CheckIndexNull(_NLL, 2);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_RidNo = (int)lua_tointeger(_NLL, 2);

	if( CHAR_CHECKINDEX( TM_Index ) == FALSE ){
		LRetBool(_NLL, FALSE);
		}
	else{
		int LRCode = CHAR_getInt( TM_Index, CHAR_LOWRIDEPETS);
		if(LRCode & TM_RidNo){
			LRetBool(_NLL, TRUE);
		}
		else{
			LRetBool(_NLL, FALSE);
		}
	}
}

int NPC_ABLua_char_getPlayerMaxNum(lua_State *_NLL)
{
	LRetInt(_NLL, getFdnum());
}

int NPC_ABLua_char_getBattleexp(lua_State *_NLL)
{
	LRetInt(_NLL, getBattleexp());
}
/*
int NPC_Lua_Char_SetPetSkill(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 1);
	CheckIndexNull(_NLL, 2);
	CheckIndexNull(_NLL, 3);
	
	int TM_PetIndex = (int)lua_tointeger(_NLL, 1);
	int TM_PetSkillIndex = (int)lua_tointeger(_NLL, 2);
	int TM_PetSkillNo = (int)lua_tointeger(_NLL, 3);

	if(CHAR_CHECKPETSKILLINDEX(TM_PetSkillIndex)){	
		LRetInt(_NLL, CHAR_setPetSkill(TM_PetIndex,TM_PetSkillIndex,TM_PetSkillNo));
	}else{
		LRetErrInt(_NLL, -1, "传入的位置超出宠物技能范围。");
	}
}
*/

int NPC_Lua_Char_SetPetSkillNo(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 1);
	CheckIndexNull(_NLL, 2);
	CheckIndexNull(_NLL, 3);
	
	int TM_PetIndex = (int)lua_tointeger(_NLL, 1);
	int TM_PetSkillIndex = (int)lua_tointeger(_NLL, 2);
	int TM_PetSkillNo = (int)lua_tointeger(_NLL, 3);

	if(CHAR_CHECKPETSKILLINDEX(TM_PetSkillIndex)){	
		LRetInt(_NLL, CHAR_setPetSkill(TM_PetIndex,TM_PetSkillIndex,TM_PetSkillNo));
	}else{
		LRetErrInt(_NLL, -1, "传入的位置超出宠物技能范围。");
	}
}

int NPC_Lua_Char_ComplianceParameter(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);

	int TM_CharaIndex = (int)lua_tointeger(_NLL, 1);
	
	if(!CHAR_CHECKINDEX(TM_CharaIndex))
	{
		LRetErrInt(_NLL, -1, "传入的人物索引是无效的。");
	}

	LRetInt(_NLL, CHAR_complianceParameter(TM_CharaIndex));
	
}

int NPC_Lua_Char_HcItemId(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_ItemId = (int)lua_tointeger(_NLL, 2);
	int TM_ItemIndex = -1;
	int i = 0;
	int k = 0;
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	int itemMax =  CheckCharMaxItem(TM_Index);
	for( i = 0 ; i<itemMax ; i++ )
	{
		TM_ItemIndex = CHAR_getItemIndex(TM_Index, i);
		if( ITEM_CHECKINDEX(TM_ItemIndex) == FALSE )continue;
		if( ITEM_getInt(TM_ItemIndex, ITEM_ID) == TM_ItemId ) {
			k = k + ITEM_getInt( TM_ItemIndex, ITEM_USEPILENUMS);
		}
	}
	if (k > 0) LRetInt(_NLL, k);
	LRetErrInt(_NLL, -2, "找不到指定道具。");
}

int NPC_Lua_Char_DelHcItem(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_ItemId = (int)lua_tointeger(_NLL, 2);

	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	if( TM_ItemId < 9 || TM_ItemId > 23){ 
		LRetErrInt(_NLL, -2, "找不到指定的道具位置。");
	}
	else {
		CHAR_DelItem( TM_Index, TM_ItemId);
		LRetInt(_NLL, 1);
	}
}

int NPC_Lua_Char_GETFM(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_ID = (int)lua_tointeger(_NLL, 1);
	int TM_X = (int)lua_tointeger(_NLL, 2);
	LRetInt(_NLL, Char_GetFm( TM_ID, TM_X));
}

int NPC_Lua_Char_FindPetEnemyBaseId(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_EnemyId = (int)lua_tointeger(_NLL, 2);
	int TM_PetIndex = -1;
	int i = 0;
	int petid = -1;
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	int enemynum = ENEMY_getEnemyNum();
	for( i = 0; i < enemynum; i ++ ) 
	{
		if (TM_EnemyId == ENEMY_getInt( i, ENEMY_ID )){
			petid = ENEMY_getInt( i, ENEMY_TEMPNO );
			break;
		}
	}

	for( i = 0 ; i<CHAR_MAXPETHAVE ; i++ )
	{
		TM_PetIndex = CHAR_getCharPet( TM_Index, i);
		if( CHAR_CHECKINDEX(TM_PetIndex))continue;

		if( CHAR_getInt(TM_PetIndex, CHAR_PETID) == petid) {
			LRetInt(_NLL, i);
			break;
		}
	}
	LRetErrInt(_NLL, -2, "找不到指定宠物。");
}

int NPC_Lua_Char_ITEMID_NAME(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_ID = (int)lua_tointeger(_NLL, 1);
	LRetMsg(_NLL, ITEM_getNameFromNumber(TM_ID));
}

int NPC_Lua_Char_ITEMID_NAME2(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_ID = (int)lua_tointeger(_NLL, 1);
	LRetMsg(_NLL, ITEM_getItemInfoFromNumber(TM_ID));
}

int NPC_Lua_Char_ITEMID_LEVEL(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_ID = (int)lua_tointeger(_NLL, 1);
	LRetInt(_NLL, ITEM_getlevelFromITEMtabl(TM_ID));
}

int NPC_Lua_Char_ITEMID_GOLD(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_ID = (int)lua_tointeger(_NLL, 1);
	LRetInt(_NLL, ITEM_getcostFromITEMtabl(TM_ID));
}

int NPC_Lua_Char_ITEMID_RANO(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_ID = (int)lua_tointeger(_NLL, 1);
	LRetInt(_NLL, ITEM_getgraNoFromITEMtabl(TM_ID));
}
/*
int NPC_Lua_Char_CharRidNo(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int RidNo = (int)lua_tointeger(_NLL, 2);

	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	if (RidNo < 0 || RidNo > 63) LRetErrInt(_NLL, -1, "传入的骑宠编号是无效的。");

	if (RIDEPET_getPETID( TM_Index, RidNo) >= 0)
	{
		LRetBool(_NLL, TRUE);
	}else LRetBool(_NLL, FALSE);
}
*/
int NPC_Lua_Char_GetPetOwner(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	if( CHAR_getInt( TM_Index, CHAR_WHICHTYPE ) != CHAR_TYPEPET ){
		LRetErrInt(_NLL, -2, "传入的对象并不是宠物！");
	}
	int charaindex = CHAR_getWorkInt(TM_Index, CHAR_WORKPLAYERINDEX);
	LRetInt(_NLL, charaindex);
}

int NPC_Lua_Char_GetEnemyBaseIdFromEnemyId(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int EnemyId = (int)lua_tointeger(_NLL, 1);
	int enemynum = ENEMY_getEnemyNum();
	int petid = -1;
	int i;
	for( i = 0; i < enemynum; i ++ ) 
	{
		if (EnemyId == ENEMY_getInt( i, ENEMY_ID )){
			petid = ENEMY_getInt( i, ENEMY_TEMPNO );
			break;
		}
	}
	if (i == enemynum) LRetErrInt(_NLL, -1, "传入的enemyID没有找到！");

	LRetInt(_NLL, petid);
}

int NPC_Lua_Char_GetEnemyIdFromEnemyBaseId(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int EnemybaseId = (int)lua_tointeger(_NLL, 1);
	int enemynum = ENEMY_getEnemyNum();
	int EnemyId = -1;
	int i;
	for( i = 0; i < enemynum; i ++ ) 
	{
		if (EnemybaseId == ENEMY_getInt( i, ENEMY_TEMPNO )){
			EnemyId = ENEMY_getInt( i, ENEMY_ID );
			break;
		}
	}
	if (i == enemynum) LRetErrInt(_NLL, -1, "传入的enemybaseID没有找到！");

	LRetInt(_NLL, EnemyId);
}

int NPC_Lua_Char_GetIp(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Flg = (int)lua_tointeger(_NLL, 2);
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	if( CHAR_getInt( TM_Index, CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER ){
		LRetErrInt(_NLL, -2, "传入的对象并不是人物！");
	}
	int fd = getfdFromCharaIndex( TM_Index ); 
	if( fd == -1 ){
		LRetErrInt(_NLL, -2, "该人物可能已经下线！");
	}
	if(TM_Flg == 1)//IP
	{
		LRetMsg(_NLL,CONNECT_get_userip2(fd));
	}else if(TM_Flg == 2)//给FD
	{
		LRetInt(_NLL,fd);
	}else if(TM_Flg == 3)//端口
	{
		LRetInt(_NLL,CONNECT_get_userport(fd));
	}
	LRetErrNull(_NLL, "传入的标志是错误的。");
}

int NPC_Lua_Char_SetAllocPoint(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 1);
	int petindex = (int)lua_tointeger(_NLL, 1);
	int type = (int)lua_tointeger(_NLL, 2);
	int val = (int)lua_tointeger(_NLL, 3);
	if (val < 0 || val > 255) LRetErrInt(_NLL, -1, "val必须为0-255。");
	if(!CHAR_CHECKINDEX(petindex))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	if( CHAR_getInt( petindex, CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER ){
		LRetErrInt(_NLL, -2, "传入的对象不能是人物！");
	}
	int	LevelUpPoint = CHAR_getInt( petindex, CHAR_ALLOCPOINT );
	int t = (int )(( LevelUpPoint >> 24 ) & 0xFF);
	int g = (int )(( LevelUpPoint >> 16 ) & 0xFF);
	int f = (int )(( LevelUpPoint >> 8 ) & 0xFF);
	int m = (int )(( LevelUpPoint >> 0 ) & 0xFF);

	if (type == 1) {
		LevelUpPoint = (val<<24)+(g<<16)+(f<<8)+(m<<0);
		CHAR_setInt( petindex, CHAR_ALLOCPOINT, LevelUpPoint);
		LRetInt(_NLL, t);
	}
	else if (type == 2) {
		LevelUpPoint = (t<<24)+(val<<16)+(f<<8)+(m<<0);
		CHAR_setInt( petindex, CHAR_ALLOCPOINT, LevelUpPoint);
		LRetInt(_NLL, g);
	}
	else if (type == 3) {
		LevelUpPoint = (t<<24)+(g<<16)+(val<<8)+(m<<0);
		CHAR_setInt( petindex, CHAR_ALLOCPOINT, LevelUpPoint);
		LRetInt(_NLL, f);
	}
	else if (type == 4) {
		LevelUpPoint = (t<<24)+(g<<16)+(f<<8)+(val<<0);
		CHAR_setInt( petindex, CHAR_ALLOCPOINT, LevelUpPoint);
		LRetInt(_NLL, m);
	}
}

int NPC_Lua_Char_GetMaxPetIndex(lua_State *_NLL)
{
	CheckEx(_NLL, 0);
	int MAXPLAYER = CHAR_getPlayerMaxNum();
	int MAXPET = CHAR_getPetMaxNum();
	int maxindex = MAXPLAYER + MAXPET;
	LRetInt(_NLL, maxindex);
}

int NPC_Lua_Char_GetMaxPlayerIndex(lua_State *_NLL)
{
	CheckEx(_NLL, 0);
	int maxindex = CHAR_getPlayerMaxNum();
	LRetInt(_NLL, maxindex);
}

int NPC_Lua_Char_SavePetToString(lua_State *_NLL)
{
	char *petdata = "\0";
	CheckEx(_NLL, 1);
	int petindex = (int)lua_tointeger(_NLL, 1);
	if(!CHAR_CHECKINDEX(petindex))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	if( CHAR_getInt( petindex, CHAR_WHICHTYPE ) == CHAR_TYPEPLAYER ){
		LRetErrInt(_NLL, -2, "传入的对象不能是人物！");
		return;
	}
	petdata = CHAR_makePetStringFromPetIndex( petindex);
	LRetMsg(_NLL, petdata);
}

int NPC_Lua_Char_LoadPetFromString(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	char * petdata = (char *)lua_tostring(_NLL, 1);
	Char   petone;
	int petindex;
	BOOL ret = CHAR_makePetFromStringToArg(petdata,&petone, 0);
	if( ret == TRUE ){
		petindex = PET_initCharOneArray( &petone );
		if( petindex < 0 ){
			LRetErrInt(_NLL, -2, "制作宠物失败");
			return;
		}
	}
	LRetInt(_NLL, petindex);
}

int NPC_Lua_Char_GetPlayerFromAccAndName(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	char *account = lua_tostring(_NLL, 1);
	char *name = lua_tostring(_NLL, 2);
	int i;
	int playernum = CHAR_getPlayerMaxNum();
	int xindex = -1;
	for( i = 0 ; i < playernum ; i++) {
		if( CHAR_CHECKINDEX(i) ){
			if (strcmp(CHAR_getChar(i,CHAR_NAME), name) == 0) {
				xindex = i;
				break;
			}
			if (strcmp(CHAR_getChar(i,CHAR_CDKEY), account) == 0) {
				xindex = i;
				break;
			}
		}
	}

	LRetInt(_NLL, xindex);
}

int NPC_Lua_Char_CheckPet(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	BOOL CHECK = CHAR_CHECKINDEX(TM_Index);
	LRetBool(_NLL, CHECK);
}

int NPC_Lua_Char_CheckPlayer(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	BOOL CHECK = CHAR_CHECKINDEX(TM_Index);
	LRetBool(_NLL, CHECK);
}

BOOL NPC_Lua_Char_JoinParty(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 2);
	int charaindex = (int)lua_tointeger(_NLL, 1);
	int toindex = (int)lua_tointeger(_NLL, 2);
	if (CHAR_CHECKINDEX(charaindex)&&CHAR_CHECKINDEX(toindex))
	{
		CHAR_JoinParty_Main( charaindex, toindex);
		LRetBool(_NLL, TRUE);
	}
	LRetBool(_NLL, FALSE);
}

int NPC_Lua_Char_getFamilyPlayIndex(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 3);
	int charaindex = (int)lua_tointeger(_NLL, 1);
	if(!CHAR_CHECKINDEX(charaindex))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	int familyindex = (int)lua_tointeger(_NLL, 1);
	int familymode = (int)lua_tointeger(_NLL, 2);
	int familycharaindex = (int)lua_tointeger(_NLL, 3);
	if(familyindex > FAMILY_MAXNUM){
		LRetErrInt(_NLL, -1, "传入的家族索引是无效的。");
	}else if (familymode > FAMILY_MAXCHANNEL)
	{
		LRetErrInt(_NLL, -1, "传入的家族频道是无效的。");
	}else if (familycharaindex > FAMILY_MAXMEMBER)
	{
		LRetErrInt(_NLL, -1, "传入的家族人物索引无效。");
	}
	int playindex =  channelMember[familyindex][familymode][familycharaindex];
	LRetInt(_NLL,playindex);
}

int NPC_Lua_Char_logout(lua_State *_NLL){
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int charaindex = (int)lua_tointeger(_NLL, 1);
	CHAR_logout(charaindex, TRUE);
	LRetBool(_NLL, TRUE);
}

int NPC_Lua_Char_GetVipPoint(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	int TM_Ret = sasql_vippoint(CHAR_getUseID(TM_Index),0,0);
	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_Char_SetVipPoint(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_NUM = (int)lua_tointeger(_NLL, 2);
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	int TM_Ret = sasql_vippoint(CHAR_getChar(TM_Index,CHAR_CDKEY),TM_NUM,1);
	LRetInt(_NLL, TM_Ret);
}


int NPC_Lua_Char_GetAmPoint(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	int TM_Ret = sasql_ampoint(CHAR_getUseID(TM_Index),0,0);
	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_Char_GetRmbPoint(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	int TM_Ret = sasql_rmbpoint(CHAR_getUseID(TM_Index),0,0);
	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_Char_SetRmbPoint(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_NUM = (int)lua_tointeger(_NLL, 2);
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	int TM_Ret = sasql_rmbpoint(CHAR_getChar(TM_Index,CHAR_CDKEY),TM_NUM,1);
	LRetInt(_NLL, TM_Ret);
}




int NPC_Lua_Char_SetAmPoint(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_NUM = (int)lua_tointeger(_NLL, 2);
	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	int TM_Ret = sasql_ampoint(CHAR_getChar(TM_Index,CHAR_CDKEY),TM_NUM,1);
	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_Char_CheckItemIndex(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	BOOL CHECK = ITEM_CHECKINDEX(TM_Index);
	LRetBool(_NLL, CHECK);
}

int NPC_Lua_Char_GetfdFromCharaIndex(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int charaindex = (int)lua_tointeger(_NLL, 1);
	LRetInt(_NLL, getfdFromCharaIndex(charaindex));
}

int NPC_Lua_Char_CharRidNo(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int RidNo = (int)lua_tointeger(_NLL, 2);

	if(!CHAR_CHECKINDEX(TM_Index))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}

	if (RidNo < 0 || RidNo > 95) LRetErrInt(_NLL, -1, "传入的骑宠编号是无效的。");

	if (CHAR_CheckLearnCode( TM_Index, RidNo) >= 0)
	{
		LRetBool(_NLL, TRUE);
	}else LRetBool(_NLL, FALSE);
}

#endif //#ifdef _JZ_NEWSCRIPT_LUA
