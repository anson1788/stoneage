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
#include "item.h"

#ifdef _JZ_NEWSCRIPT_LUA

typedef enum
{
	LUAITEM_PREOVERFUNC = ITEM_PREOVERFUNC - ITEM_FIRSTFUNCTION,
	LUAITEM_POSTOVERFUNC = ITEM_POSTOVERFUNC - ITEM_FIRSTFUNCTION,
	LUAITEM_WATCHFUNC = ITEM_WATCHFUNC - ITEM_FIRSTFUNCTION,
	LUAITEM_USEFUNC = ITEM_USEFUNC - ITEM_FIRSTFUNCTION,
	LUAITEM_ATTACHFUNC = ITEM_ATTACHFUNC - ITEM_FIRSTFUNCTION,
	LUAITEM_DETACHFUNC = ITEM_DETACHFUNC - ITEM_FIRSTFUNCTION,
	LUAITEM_DROPFUNC = ITEM_DROPFUNC - ITEM_FIRSTFUNCTION,
	LUAITEM_PICKUPFUNC = ITEM_PICKUPFUNC - ITEM_FIRSTFUNCTION,
#ifdef _Item_ReLifeAct
	LUAITEM_DIERELIFEFUNC = ITEM_DIERELIFEFUNC - ITEM_FIRSTFUNCTION,
#endif
}LUAITEM_FUNC;

//获取数据的接口
int NPC_Lua_Item_GetData(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckItemIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Flg = (int)lua_tointeger(_NLL, 2);

	if(TM_Flg >= 0)
	{
		if(TM_Flg < LUA_DATALINE1)
		{
			TM_Flg -= LUA_DATALINE0;
			int TM_RetInt = ITEM_getInt(TM_Index, TM_Flg);
			LRetInt(_NLL, TM_RetInt);
		}else if(TM_Flg < LUA_DATALINE2)
		{
			TM_Flg -= LUA_DATALINE1;
			char *TM_RetPoint = ITEM_getChar(TM_Index, TM_Flg);
			LRetMsg(_NLL, TM_RetPoint);
		}else if(TM_Flg < LUA_DATALINE3)
		{
			TM_Flg -= LUA_DATALINE2;
			int TM_RetWorkInt = ITEM_getWorkInt(TM_Index, TM_Flg);
			LRetInt(_NLL, TM_RetWorkInt);
		}else
		{
			LRetErrNull(_NLL, "传入的标志是错误的。");
		}
	}
	LRetErrNull(_NLL, "传入的标志是错误的。");
}

//设置数据的接口
int NPC_Lua_Item_SetData(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckItemIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Flg = (int)lua_tointeger(_NLL, 2);

	if(TM_Flg >= 0)
	{
		if(TM_Flg < LUA_DATALINE1)
		{
			TM_Flg -= LUA_DATALINE0;
			int TM_IntVal = (int)lua_tointeger(_NLL, 3);
			int TM_RetInt = ITEM_setInt(TM_Index, TM_Flg, TM_IntVal);
			LRetInt(_NLL, TM_RetInt);
		}else if(TM_Flg < LUA_DATALINE2)
		{
			TM_Flg -= LUA_DATALINE1;
			char *TM_CharPoint = (char *)lua_tostring(_NLL, 3);
			BOOL TM_RetBOOL = ITEM_setChar(TM_Index, TM_Flg, TM_CharPoint);
			LRetBool(_NLL, TM_RetBOOL);
		}else if(TM_Flg < LUA_DATALINE3)
		{
			TM_Flg -= LUA_DATALINE2;
			if(TM_Flg == ITEM_WORKOBJINDEX || TM_Flg == ITEM_WORKCHARAINDEX)
			{
				LRetInt(_NLL, 0);
			}
			int TM_WorkIntVal = (int)lua_tointeger(_NLL, 3);
			int TM_RetInt = ITEM_setWorkInt(TM_Index, TM_Flg, TM_WorkIntVal);
			LRetInt(_NLL, TM_RetInt);
		}else
		{
			LRetErrNull(_NLL, "传入的标志是错误的。");
		}
	}
	LRetErrNull(_NLL, "传入的标志是错误的。");
}

//事件设置的接口
int NPC_Lua_Item_SetPreOverEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckItemIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	ITEM_Item *TM_Item = ITEM_getItemPointer(TM_index);

	if(TM_Item == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);

	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_Item->functable[LUAITEM_PREOVERFUNC] != (void *)NPC_Lua_ItemPerOverCallBack)
		{
			if(TM_Item->functable[LUAITEM_PREOVERFUNC] != NULL)
			{
				TM_Item->sur_functable[LUAITEM_PREOVERFUNC] = TM_Item->functable[LUAITEM_PREOVERFUNC];
				strcpy_s(TM_Item->sur_charfunctable[LUAITEM_PREOVERFUNC].string, sizeof(TM_Item->sur_charfunctable[LUAITEM_PREOVERFUNC].string), TM_Item->string[ITEM_PREOVERFUNC].string);
			}else
			{
				TM_Item->sur_functable[LUAITEM_PREOVERFUNC] = (void *)NULL;
				TM_Item->sur_charfunctable[LUAITEM_PREOVERFUNC].string[0] = '\0';
			}

			TM_Item->functable[LUAITEM_PREOVERFUNC] = (void *)NPC_Lua_ItemPerOverCallBack;
			strcpy_s(TM_Item->lua_charfunctable[LUAITEM_PREOVERFUNC].string, sizeof(TM_Item->lua_charfunctable[LUAITEM_PREOVERFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_Item->string[ITEM_PREOVERFUNC].string, sizeof(TM_Item->string[ITEM_PREOVERFUNC].string), FUNCNAME_ITEMPEROVERCALLBACK);
		}
	}else
	{
		if(TM_Item->functable[LUAITEM_PREOVERFUNC] == (void *)NPC_Lua_ItemPerOverCallBack)
		{
			TM_Item->lua_charfunctable[LUAITEM_PREOVERFUNC].string[0] = '\0';
			if(TM_Item->sur_functable[LUAITEM_PREOVERFUNC] == NULL)
			{
				TM_Item->functable[LUAITEM_PREOVERFUNC] = (void *)NULL;
				TM_Item->string[ITEM_PREOVERFUNC].string[0] = '\0';
			}else
			{
				TM_Item->functable[LUAITEM_PREOVERFUNC] = TM_Item->sur_functable[LUAITEM_PREOVERFUNC];
				TM_Item->sur_functable[LUAITEM_PREOVERFUNC] = (void *)NULL;
				strcpy_s(TM_Item->string[ITEM_PREOVERFUNC].string, sizeof(TM_Item->string[ITEM_PREOVERFUNC].string), TM_Item->sur_charfunctable[LUAITEM_PREOVERFUNC].string);
			}
		}
	}
	return TM_Ret;
}

int NPC_Lua_Item_SetPostOverEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckItemIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	ITEM_Item *TM_Item = ITEM_getItemPointer(TM_index);

	if(TM_Item == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);

	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_Item->functable[LUAITEM_POSTOVERFUNC] != (void *)NPC_Lua_ItemPostOverCallBack)
		{
			if(TM_Item->functable[LUAITEM_POSTOVERFUNC] != NULL)
			{
				TM_Item->sur_functable[LUAITEM_POSTOVERFUNC] = TM_Item->functable[LUAITEM_POSTOVERFUNC];
				strcpy_s(TM_Item->sur_charfunctable[LUAITEM_POSTOVERFUNC].string, sizeof(TM_Item->sur_charfunctable[LUAITEM_POSTOVERFUNC].string), TM_Item->string[ITEM_POSTOVERFUNC].string);
			}else
			{
				TM_Item->sur_functable[LUAITEM_POSTOVERFUNC] = (void *)NULL;
				TM_Item->sur_charfunctable[LUAITEM_POSTOVERFUNC].string[0] = '\0';
			}

			TM_Item->functable[LUAITEM_POSTOVERFUNC] = (void *)NPC_Lua_ItemPostOverCallBack;
			strcpy_s(TM_Item->lua_charfunctable[LUAITEM_POSTOVERFUNC].string, sizeof(TM_Item->lua_charfunctable[LUAITEM_POSTOVERFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_Item->string[ITEM_POSTOVERFUNC].string, sizeof(TM_Item->string[ITEM_POSTOVERFUNC].string), FUNCNAME_ITEMPOSTOVERCALLBACK);
		}
	}else
	{
		if(TM_Item->functable[LUAITEM_POSTOVERFUNC] == (void *)NPC_Lua_ItemPostOverCallBack)
		{
			TM_Item->lua_charfunctable[LUAITEM_POSTOVERFUNC].string[0] = '\0';
			if(TM_Item->sur_functable[LUAITEM_POSTOVERFUNC] == NULL)
			{
				TM_Item->functable[LUAITEM_POSTOVERFUNC] = (void *)NULL;
				TM_Item->string[ITEM_POSTOVERFUNC].string[0] = '\0';
			}else
			{
				TM_Item->functable[LUAITEM_POSTOVERFUNC] = TM_Item->sur_functable[LUAITEM_POSTOVERFUNC];
				TM_Item->sur_functable[LUAITEM_POSTOVERFUNC] = (void *)NULL;
				strcpy_s(TM_Item->string[ITEM_POSTOVERFUNC].string, sizeof(TM_Item->string[ITEM_POSTOVERFUNC].string), TM_Item->sur_charfunctable[LUAITEM_POSTOVERFUNC].string);
			}
		}
	}
	return TM_Ret;
}

int NPC_Lua_Item_SetWatchEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckItemIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	ITEM_Item *TM_Item = ITEM_getItemPointer(TM_index);

	if(TM_Item == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);

	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_Item->functable[LUAITEM_WATCHFUNC] != (void *)NPC_Lua_ItemWatchCallBack)
		{
			if(TM_Item->functable[LUAITEM_WATCHFUNC] != NULL)
			{
				TM_Item->sur_functable[LUAITEM_WATCHFUNC] = TM_Item->functable[LUAITEM_WATCHFUNC];
				strcpy_s(TM_Item->sur_charfunctable[LUAITEM_WATCHFUNC].string, sizeof(TM_Item->sur_charfunctable[LUAITEM_WATCHFUNC].string), TM_Item->string[ITEM_WATCHFUNC].string);
			}else
			{
				TM_Item->sur_functable[LUAITEM_WATCHFUNC] = (void *)NULL;
				TM_Item->sur_charfunctable[LUAITEM_WATCHFUNC].string[0] = '\0';
			}

			TM_Item->functable[LUAITEM_WATCHFUNC] = (void *)NPC_Lua_ItemWatchCallBack;
			strcpy_s(TM_Item->lua_charfunctable[LUAITEM_WATCHFUNC].string, sizeof(TM_Item->lua_charfunctable[LUAITEM_WATCHFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_Item->string[ITEM_WATCHFUNC].string, sizeof(TM_Item->string[ITEM_WATCHFUNC].string), FUNCNAME_ITEMWATCHCALLBACK);
		}
	}else
	{
		if(TM_Item->functable[LUAITEM_WATCHFUNC] == (void *)NPC_Lua_ItemWatchCallBack)
		{
			TM_Item->lua_charfunctable[LUAITEM_WATCHFUNC].string[0] = '\0';
			if(TM_Item->sur_functable[LUAITEM_WATCHFUNC] == NULL)
			{
				TM_Item->functable[LUAITEM_WATCHFUNC] = (void *)NULL;
				TM_Item->string[ITEM_WATCHFUNC].string[0] = '\0';
			}else
			{
				TM_Item->functable[LUAITEM_WATCHFUNC] = TM_Item->sur_functable[LUAITEM_WATCHFUNC];
				TM_Item->sur_functable[LUAITEM_WATCHFUNC] = (void *)NULL;
				strcpy_s(TM_Item->string[ITEM_WATCHFUNC].string, sizeof(TM_Item->string[ITEM_WATCHFUNC].string), TM_Item->sur_charfunctable[LUAITEM_WATCHFUNC].string);
			}
		}
	}
	return TM_Ret;
}

int NPC_Lua_Item_SetUseEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckItemIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	ITEM_Item *TM_Item = ITEM_getItemPointer(TM_index);

	if(TM_Item == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);

	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_Item->functable[LUAITEM_USEFUNC] != (void *)NPC_Lua_ItemUseCallBack)
		{
			if(TM_Item->functable[LUAITEM_USEFUNC] != NULL)
			{
				TM_Item->sur_functable[LUAITEM_USEFUNC] = TM_Item->functable[LUAITEM_USEFUNC];
				strcpy_s(TM_Item->sur_charfunctable[LUAITEM_USEFUNC].string, sizeof(TM_Item->sur_charfunctable[LUAITEM_USEFUNC].string), TM_Item->string[ITEM_USEFUNC].string);
			}else
			{
				TM_Item->sur_functable[LUAITEM_USEFUNC] = (void *)NULL;
				TM_Item->sur_charfunctable[LUAITEM_USEFUNC].string[0] = '\0';
			}

			TM_Item->functable[LUAITEM_USEFUNC] = (void *)NPC_Lua_ItemUseCallBack;
			strcpy_s(TM_Item->lua_charfunctable[LUAITEM_USEFUNC].string, sizeof(TM_Item->lua_charfunctable[LUAITEM_USEFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_Item->string[ITEM_USEFUNC].string, sizeof(TM_Item->string[ITEM_USEFUNC].string), FUNCNAME_ITEMUSECALLBACK);
		}
	}else
	{
		if(TM_Item->functable[LUAITEM_USEFUNC] == (void *)NPC_Lua_ItemUseCallBack)
		{
			TM_Item->lua_charfunctable[LUAITEM_USEFUNC].string[0] = '\0';
			if(TM_Item->sur_functable[LUAITEM_USEFUNC] == NULL)
			{
				TM_Item->functable[LUAITEM_USEFUNC] = (void *)NULL;
				TM_Item->string[ITEM_USEFUNC].string[0] = '\0';
			}else
			{
				TM_Item->functable[LUAITEM_USEFUNC] = TM_Item->sur_functable[LUAITEM_USEFUNC];
				TM_Item->sur_functable[LUAITEM_USEFUNC] = (void *)NULL;
				strcpy_s(TM_Item->string[ITEM_USEFUNC].string, sizeof(TM_Item->string[ITEM_USEFUNC].string), TM_Item->sur_charfunctable[LUAITEM_USEFUNC].string);
			}
		}
	}
	return TM_Ret;
}

int NPC_Lua_Item_SetAttachEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckItemIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	ITEM_Item *TM_Item = ITEM_getItemPointer(TM_index);

	if(TM_Item == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);

	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_Item->functable[LUAITEM_ATTACHFUNC] != (void *)NPC_Lua_ItemAttachCallBack)
		{
			if(TM_Item->functable[LUAITEM_ATTACHFUNC] != NULL)
			{
				TM_Item->sur_functable[LUAITEM_ATTACHFUNC] = TM_Item->functable[LUAITEM_ATTACHFUNC];
				strcpy_s(TM_Item->sur_charfunctable[LUAITEM_ATTACHFUNC].string, sizeof(TM_Item->sur_charfunctable[LUAITEM_ATTACHFUNC].string), TM_Item->string[ITEM_ATTACHFUNC].string);
			}else
			{
				TM_Item->sur_functable[LUAITEM_ATTACHFUNC] = (void *)NULL;
				TM_Item->sur_charfunctable[LUAITEM_ATTACHFUNC].string[0] = '\0';
			}

			TM_Item->functable[LUAITEM_ATTACHFUNC] = (void *)NPC_Lua_ItemAttachCallBack;
			strcpy_s(TM_Item->lua_charfunctable[LUAITEM_ATTACHFUNC].string, sizeof(TM_Item->lua_charfunctable[LUAITEM_ATTACHFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_Item->string[ITEM_ATTACHFUNC].string, sizeof(TM_Item->string[ITEM_ATTACHFUNC].string), FUNCNAME_ITEMATTACHCALLBACK);
		}
	}else
	{
		if(TM_Item->functable[LUAITEM_ATTACHFUNC] == (void *)NPC_Lua_ItemAttachCallBack)
		{
			TM_Item->lua_charfunctable[LUAITEM_ATTACHFUNC].string[0] = '\0';
			if(TM_Item->sur_functable[LUAITEM_ATTACHFUNC] == NULL)
			{
				TM_Item->functable[LUAITEM_ATTACHFUNC] = (void *)NULL;
				TM_Item->string[ITEM_ATTACHFUNC].string[0] = '\0';
			}else
			{
				TM_Item->functable[LUAITEM_ATTACHFUNC] = TM_Item->sur_functable[LUAITEM_ATTACHFUNC];
				TM_Item->sur_functable[LUAITEM_ATTACHFUNC] = (void *)NULL;
				strcpy_s(TM_Item->string[ITEM_ATTACHFUNC].string, sizeof(TM_Item->string[ITEM_ATTACHFUNC].string), TM_Item->sur_charfunctable[LUAITEM_ATTACHFUNC].string);
			}
		}
	}
	return TM_Ret;
}

int NPC_Lua_Item_SetDetachEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckItemIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	ITEM_Item *TM_Item = ITEM_getItemPointer(TM_index);

	if(TM_Item == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);

	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_Item->functable[LUAITEM_DETACHFUNC] != (void *)NPC_Lua_ItemDetachCallBack)
		{
			if(TM_Item->functable[LUAITEM_DETACHFUNC] != NULL)
			{
				TM_Item->sur_functable[LUAITEM_DETACHFUNC] = TM_Item->functable[LUAITEM_DETACHFUNC];
				strcpy_s(TM_Item->sur_charfunctable[LUAITEM_DETACHFUNC].string, sizeof(TM_Item->sur_charfunctable[LUAITEM_DETACHFUNC].string), TM_Item->string[ITEM_DETACHFUNC].string);
			}else
			{
				TM_Item->sur_functable[LUAITEM_DETACHFUNC] = (void *)NULL;
				TM_Item->sur_charfunctable[LUAITEM_DETACHFUNC].string[0] = '\0';
			}

			TM_Item->functable[LUAITEM_DETACHFUNC] = (void *)NPC_Lua_ItemDetachCallBack;
			strcpy_s(TM_Item->lua_charfunctable[LUAITEM_DETACHFUNC].string, sizeof(TM_Item->lua_charfunctable[LUAITEM_DETACHFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_Item->string[ITEM_DETACHFUNC].string, sizeof(TM_Item->string[ITEM_DETACHFUNC].string), FUNCNAME_ITEMDETACHCALLBACK);
		}
	}else
	{
		if(TM_Item->functable[LUAITEM_DETACHFUNC] == (void *)NPC_Lua_ItemDetachCallBack)
		{
			TM_Item->lua_charfunctable[LUAITEM_DETACHFUNC].string[0] = '\0';
			if(TM_Item->sur_functable[LUAITEM_DETACHFUNC] == NULL)
			{
				TM_Item->functable[LUAITEM_DETACHFUNC] = (void *)NULL;
				TM_Item->string[ITEM_DETACHFUNC].string[0] = '\0';
			}else
			{
				TM_Item->functable[LUAITEM_DETACHFUNC] = TM_Item->sur_functable[LUAITEM_DETACHFUNC];
				TM_Item->sur_functable[LUAITEM_DETACHFUNC] = (void *)NULL;
				strcpy_s(TM_Item->string[ITEM_DETACHFUNC].string, sizeof(TM_Item->string[ITEM_DETACHFUNC].string), TM_Item->sur_charfunctable[LUAITEM_DETACHFUNC].string);
			}
		}
	}
	return TM_Ret;
}

int NPC_Lua_Item_SetDropEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckItemIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	ITEM_Item *TM_Item = ITEM_getItemPointer(TM_index);

	if(TM_Item == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);

	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_Item->functable[LUAITEM_DROPFUNC] != (void *)NPC_Lua_ItemDropCallBack)
		{
			if(TM_Item->functable[LUAITEM_DROPFUNC] != NULL)
			{
				TM_Item->sur_functable[LUAITEM_DROPFUNC] = TM_Item->functable[LUAITEM_DROPFUNC];
				strcpy_s(TM_Item->sur_charfunctable[LUAITEM_DROPFUNC].string, sizeof(TM_Item->sur_charfunctable[LUAITEM_DROPFUNC].string), TM_Item->string[ITEM_DROPFUNC].string);
			}else
			{
				TM_Item->sur_functable[LUAITEM_DROPFUNC] = (void *)NULL;
				TM_Item->sur_charfunctable[LUAITEM_DROPFUNC].string[0] = '\0';
			}

			TM_Item->functable[LUAITEM_DROPFUNC] = (void *)NPC_Lua_ItemDropCallBack;
			strcpy_s(TM_Item->lua_charfunctable[LUAITEM_DROPFUNC].string, sizeof(TM_Item->lua_charfunctable[LUAITEM_DROPFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_Item->string[ITEM_DROPFUNC].string, sizeof(TM_Item->string[ITEM_DROPFUNC].string), FUNCNAME_ITEMDROPCALLBACK);
		}
	}else
	{
		if(TM_Item->functable[LUAITEM_DROPFUNC] == (void *)NPC_Lua_ItemDropCallBack)
		{
			TM_Item->lua_charfunctable[LUAITEM_DROPFUNC].string[0] = '\0';
			if(TM_Item->sur_functable[LUAITEM_DROPFUNC] == NULL)
			{
				TM_Item->functable[LUAITEM_DROPFUNC] = (void *)NULL;
				TM_Item->string[ITEM_DROPFUNC].string[0] = '\0';
			}else
			{
				TM_Item->functable[LUAITEM_DROPFUNC] = TM_Item->sur_functable[LUAITEM_DROPFUNC];
				TM_Item->sur_functable[LUAITEM_DROPFUNC] = (void *)NULL;
				strcpy_s(TM_Item->string[ITEM_DROPFUNC].string, sizeof(TM_Item->string[ITEM_DROPFUNC].string), TM_Item->sur_charfunctable[LUAITEM_DROPFUNC].string);
			}
		}
	}
	return TM_Ret;
}

int NPC_Lua_Item_SetPickUPEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckItemIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	ITEM_Item *TM_Item = ITEM_getItemPointer(TM_index);

	if(TM_Item == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);

	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_Item->functable[LUAITEM_PICKUPFUNC] != (void *)NPC_Lua_ItemPickUPCallBack)
		{
			if(TM_Item->functable[LUAITEM_PICKUPFUNC] != NULL)
			{
				TM_Item->sur_functable[LUAITEM_PICKUPFUNC] = TM_Item->functable[LUAITEM_PICKUPFUNC];
				strcpy_s(TM_Item->sur_charfunctable[LUAITEM_PICKUPFUNC].string, sizeof(TM_Item->sur_charfunctable[LUAITEM_PICKUPFUNC].string), TM_Item->string[ITEM_PICKUPFUNC].string);
			}else
			{
				TM_Item->sur_functable[LUAITEM_PICKUPFUNC] = (void *)NULL;
				TM_Item->sur_charfunctable[LUAITEM_PICKUPFUNC].string[0] = '\0';
			}

			TM_Item->functable[LUAITEM_PICKUPFUNC] = (void *)NPC_Lua_ItemPickUPCallBack;
			strcpy_s(TM_Item->lua_charfunctable[LUAITEM_PICKUPFUNC].string, sizeof(TM_Item->lua_charfunctable[LUAITEM_PICKUPFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_Item->string[ITEM_PICKUPFUNC].string, sizeof(TM_Item->string[ITEM_PICKUPFUNC].string), FUNCNAME_ITEMPICKUPCALLBACK);
		}
	}else
	{
		if(TM_Item->functable[LUAITEM_PICKUPFUNC] == (void *)NPC_Lua_ItemPickUPCallBack)
		{
			TM_Item->lua_charfunctable[LUAITEM_PICKUPFUNC].string[0] = '\0';
			if(TM_Item->sur_functable[LUAITEM_PICKUPFUNC] == NULL)
			{
				TM_Item->functable[LUAITEM_PICKUPFUNC] = (void *)NULL;
				TM_Item->string[ITEM_PICKUPFUNC].string[0] = '\0';
			}else
			{
				TM_Item->functable[LUAITEM_PICKUPFUNC] = TM_Item->sur_functable[LUAITEM_PICKUPFUNC];
				TM_Item->sur_functable[LUAITEM_PICKUPFUNC] = (void *)NULL;
				strcpy_s(TM_Item->string[ITEM_PICKUPFUNC].string, sizeof(TM_Item->string[ITEM_PICKUPFUNC].string), TM_Item->sur_charfunctable[LUAITEM_PICKUPFUNC].string);
			}
		}
	}
	return TM_Ret;
}

#ifdef _Item_ReLifeAct
int NPC_Lua_Item_SetDieReLifeEvent(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckItemIndexNull(_NLL, 3);
	int TM_index = (int)lua_tointeger(_NLL, 3);

	ITEM_Item *TM_Item = ITEM_getItemPointer(TM_index);

	if(TM_Item == NULL)
	{
		LRetErrInt(_NLL , -1, "传入的索引是无效的。");
	}
	int TM_Ret = NPC_Lua_GetFuncPoint(_NLL);

	if(lua_tointeger(_NLL, -1) > 0)
	{
		if(TM_Item->functable[LUAITEM_DIERELIFEFUNC] != (void *)NPC_Lua_ItemDieReLifeCallBack)
		{
			if(TM_Item->functable[LUAITEM_DIERELIFEFUNC] != NULL)
			{
				TM_Item->sur_functable[LUAITEM_DIERELIFEFUNC] = TM_Item->functable[LUAITEM_DIERELIFEFUNC];
				strcpy_s(TM_Item->sur_charfunctable[LUAITEM_DIERELIFEFUNC].string, sizeof(TM_Item->sur_charfunctable[LUAITEM_DIERELIFEFUNC].string), TM_Item->string[ITEM_DIERELIFEFUNC].string);
			}else
			{
				TM_Item->sur_functable[LUAITEM_DIERELIFEFUNC] = (void *)NULL;
				TM_Item->sur_charfunctable[LUAITEM_DIERELIFEFUNC].string[0] = '\0';
			}

			TM_Item->functable[LUAITEM_DIERELIFEFUNC] = (void *)NPC_Lua_ItemDieReLifeCallBack;
			strcpy_s(TM_Item->lua_charfunctable[LUAITEM_DIERELIFEFUNC].string, sizeof(TM_Item->lua_charfunctable[LUAITEM_DIERELIFEFUNC].string), lua_tostring(_NLL, 2));
			strcpy_s(TM_Item->string[ITEM_DIERELIFEFUNC].string, sizeof(TM_Item->string[ITEM_DIERELIFEFUNC].string), FUNCNAME_ITEMDIERELIFECALLBACK);
		}
	}else
	{
		if(TM_Item->functable[LUAITEM_DIERELIFEFUNC] == (void *)NPC_Lua_ItemDieReLifeCallBack)
		{
			TM_Item->lua_charfunctable[LUAITEM_DIERELIFEFUNC].string[0] = '\0';
			if(TM_Item->sur_functable[LUAITEM_DIERELIFEFUNC] == NULL)
			{
				TM_Item->functable[LUAITEM_DIERELIFEFUNC] = (void *)NULL;
				TM_Item->string[ITEM_DIERELIFEFUNC].string[0] = '\0';
			}else
			{
				TM_Item->functable[LUAITEM_DIERELIFEFUNC] = TM_Item->sur_functable[LUAITEM_DIERELIFEFUNC];
				TM_Item->sur_functable[LUAITEM_DIERELIFEFUNC] = (void *)NULL;
				strcpy_s(TM_Item->string[ITEM_DIERELIFEFUNC].string, sizeof(TM_Item->string[ITEM_DIERELIFEFUNC].string), TM_Item->sur_charfunctable[LUAITEM_DIERELIFEFUNC].string);
			}
		}
	}
	return TM_Ret;
}
#endif

int NPC_Lua_Item_GetDataFromItemSet(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	int TM_ID = (int)lua_tointeger(_NLL, 1);
	int TM_Flg = (int)lua_tointeger(_NLL, 2);
	if( !ITEM_CHECKITEMTABLE( TM_ID) )
	{
		LRetErrInt(_NLL , -1, "传入的道具ID是无效的。");
	}
	if (TM_Flg<LUA_DATALINE1) 
	{
		LRetInt(_NLL, ITEMTBL_getInt( TM_ID, TM_Flg));
	}
	else 
	{
		LRetMsg(_NLL, ITEMTBL_getChar( TM_ID, TM_Flg-LUA_DATALINE1));
	}
	LRetErrNull(_NLL, "传入的标志是错误的。");
}

int NPC_Lua_Item_GetValidity(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckItemIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	if( !ITEM_CHECKINDEX( TM_Index)) {
		LRetErrInt(_NLL , -1, "传入的索引是无效的！");
	}
	int Validity = ITEM_getInt(TM_Index, Validity);
	LRetInt(_NLL, Validity);
}


int NPC_Lua_Item_GetItemOwner(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckItemIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	if( !ITEM_CHECKINDEX( TM_Index)) {
		LRetErrInt(_NLL , -1, "传入的索引是无效的！");
	}
	int charaindex = ITEM_getWorkInt(TM_Index, ITEM_WORKCHARAINDEX);
	if( !CHAR_CHECKINDEX( charaindex)) {
		LRetErrInt(_NLL , -2, "道具主人的索引是无效的！");
	}
	LRetInt(_NLL, charaindex);
}


int NPC_Lua_Item_GetMaxItemIndex(lua_State *_NLL)
{
	CheckEx(_NLL, 0);
	int itemnum = ITEM_getITEM_itemnum();
	LRetInt(_NLL, itemnum);
}


int NPC_Lua_Item_SaveItemToString(lua_State *_NLL)
{
	char *itemdata = "\0";
	CheckEx(_NLL, 1);
	int itemindex = (int)lua_tointeger(_NLL, 1);
	if(!ITEM_CHECKINDEX(itemindex))
	{
		LRetErrInt(_NLL, -1, "传入的索引是无效的。");
	}
	itemdata = ITEM_makeStringFromItemIndex( itemindex,0);
	LRetMsg(_NLL, itemdata);
}


//新增
int NPC_Lua_Item_SetValidity(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckItemIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int Validity = (int)lua_tointeger(_NLL, 2);
	if( !ITEM_CHECKINDEX( TM_Index)) {
		LRetErrInt(_NLL , -1, "传入的索引是无效的！");
	}
	time_t lt;
	lt =time(NULL);
	ITEM_setInt(TM_Index, Validity,Validity);
	LRetInt(_NLL, 0);
}


int NPC_Lua_Item_LoadItemFromString(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	char *itemdata = (char *)lua_tostring(_NLL, 1);
	ITEM_Item   itmone;
	int itemindex;
	BOOL ret = ITEM_makeExistItemsFromStringToArg( itemdata , &itmone, 0 );
	if( ret == TRUE ){
		itemindex = ITEM_initExistItemsOne( &itmone );
		if( itemindex < 0 ){
			LRetErrInt(_NLL, -1, "制作道具失败");
			return;
		}
	}
	LRetInt(_NLL, itemindex);
}


#endif //#ifdef _JZ_NEWSCRIPT_LUA
