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
#include "pet_skill.h"
#include "profession_skill.h"
#include "magic_base.h"

#define _JZ_NEWSCRIPT_LUA

#ifdef _CHAR_PROFESSION	

int NPC_Lua_Spell_PROFESSION_GetData(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Flg = (int)lua_tointeger(_NLL, 2);

	if(TM_Flg >= 0)
	{
		if(TM_Flg < LUA_DATALINE1)
		{
			int skillarray = PROFESSION_SKILL_getskillArray( TM_Index);
			TM_Flg -= LUA_DATALINE0;
			int TM_RetInt = PROFESSION_SKILL_getInt(skillarray, TM_Flg);
			LRetInt(_NLL, TM_RetInt);
		}else if(TM_Flg < LUA_DATALINE2)
		{
			
			int skillarray = PROFESSION_SKILL_getskillArray( TM_Index );
			TM_Flg -= LUA_DATALINE1;
			char *TM_RetPoint = PROFESSION_SKILL_getChar(skillarray, TM_Flg);
			LRetMsg(_NLL, TM_RetPoint);
		}else
		{
			LRetErrNull(_NLL, "传入的标志是错误的。");
		}
	}
	LRetErrNull(_NLL, "传入的标志是错误的。");
}

int NPC_Lua_Spell_PETSKILL_SetData(lua_State *_NLL)
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
			int TM_RetInt = PROFESSION_SKILL_setInt(TM_Index, TM_Flg, TM_IntVal);
			LRetInt(_NLL, TM_RetInt);
		}else if(TM_Flg < LUA_DATALINE2)
		{
			TM_Flg -= LUA_DATALINE1;
			char *TM_CharPoint = (char *)lua_tostring(_NLL, 3);
			BOOL TM_RetBOOL = PROFESSION_SKILL_setChar(TM_Index, TM_Flg, TM_CharPoint);
			LRetBool(_NLL, TM_RetBOOL);
		}else
		{
			LRetErrNull(_NLL, "传入的标志是错误的。");
		}
	}
	LRetErrNull(_NLL, "传入的标志是错误的。");
}






int NPC_Lua_Spell_PROFESSION_SetData(lua_State *_NLL)
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
			int TM_RetInt = PROFESSION_SKILL_setInt(TM_Index, TM_Flg, TM_IntVal);
			LRetInt(_NLL, TM_RetInt);
		}else if(TM_Flg < LUA_DATALINE2)
		{
			TM_Flg -= LUA_DATALINE1;
			char *TM_CharPoint = (char *)lua_tostring(_NLL, 3);
			BOOL TM_RetBOOL = PROFESSION_SKILL_setChar(TM_Index, TM_Flg, TM_CharPoint);
			LRetBool(_NLL, TM_RetBOOL);
		}else
		{
			LRetErrNull(_NLL, "传入的标志是错误的。");
		}
	}
	LRetErrNull(_NLL, "传入的标志是错误的。");
}


#endif

int NPC_Lua_Spell_MAGIC_GetData(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Flg = (int)lua_tointeger(_NLL, 2);

	if(TM_Flg >= 0)
	{
		if(TM_Flg < LUA_DATALINE1)
		{
			int magic_array = -1;
			magic_array = MAGIC_getMagicArray( TM_Index );
			TM_Flg -= LUA_DATALINE0;
			int TM_RetInt = MAGIC_getInt(magic_array, TM_Flg);
			LRetInt(_NLL, TM_RetInt);
		}else if(TM_Flg < LUA_DATALINE2)
		{
			int magic_array = -1;
			magic_array = MAGIC_getMagicArray( TM_Index );
			TM_Flg -= LUA_DATALINE1;
			char *TM_RetPoint = MAGIC_getChar(magic_array, TM_Flg);
			LRetMsg(_NLL, TM_RetPoint);
		}else
		{
			LRetErrNull(_NLL, "传入的标志是错误的。");
		}
	}
	LRetErrNull(_NLL, "传入的标志是错误的。");
}
int NPC_Lua_Spell_PETSKILL_GetData(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int TM_Flg = (int)lua_tointeger(_NLL, 2);

	if(TM_Flg >= 0)
	{
		if(TM_Flg < LUA_DATALINE1)
		{
			int skillarray = PETSKILL_getPetskillArray( TM_Index);
			TM_Flg -= LUA_DATALINE0;
			int TM_RetInt = PETSKILL_getInt(skillarray, TM_Flg);
			LRetInt(_NLL, TM_RetInt);
		}else if(TM_Flg < LUA_DATALINE2)
		{
			int skillarray = PETSKILL_getPetskillArray( TM_Index);
			TM_Flg -= LUA_DATALINE1;
			char *TM_RetPoint = PETSKILL_getChar(skillarray, TM_Flg);
			LRetMsg(_NLL, TM_RetPoint);
		}else
		{
			LRetErrNull(_NLL, "传入的标志是错误的。");
		}
	}
	LRetErrNull(_NLL, "传入的标志是错误的。");
}

int NPC_Lua_Spell_MAGIC_SetData(lua_State *_NLL)
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
			int TM_RetInt = MAGIC_setInt(TM_Index, TM_Flg, TM_IntVal);
			LRetInt(_NLL, TM_RetInt);
		}else if(TM_Flg < LUA_DATALINE2)
		{
			TM_Flg -= LUA_DATALINE1;
			char *TM_CharPoint = (char *)lua_tostring(_NLL, 3);
			BOOL TM_RetBOOL = MAGIC_setChar(TM_Index, TM_Flg, TM_CharPoint);
			LRetBool(_NLL, TM_RetBOOL);
		}else
		{
			LRetErrNull(_NLL, "传入的标志是错误的。");
		}
	}
	LRetErrNull(_NLL, "传入的标志是错误的。");
}
