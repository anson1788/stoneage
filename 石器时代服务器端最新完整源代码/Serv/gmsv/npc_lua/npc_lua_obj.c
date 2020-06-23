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

int NPC_Lua_Obj_GetType(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckObjIndexNull(_NLL, 1);
	int TM_index = (int)lua_tointeger(_NLL, 1);
	
	int TM_Ret = OBJECT_getType(TM_index);

	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_Obj_GetCharType(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckObjIndexNull(_NLL, 1);
	int TM_index = (int)lua_tointeger(_NLL, 1);
	
	int TM_Ret = OBJECT_getchartype(TM_index);

	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_Obj_GetCharIndex(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckObjIndexNull(_NLL, 1);
	int TM_index = (int)lua_tointeger(_NLL, 1);
	
	int TM_Ret = OBJECT_getIndex(TM_index);

	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_Obj_GetX(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckObjIndexNull(_NLL, 1);
	int TM_index = (int)lua_tointeger(_NLL, 1);
	
	int TM_Ret = OBJECT_getX(TM_index);

	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_Obj_GetY(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckObjIndexNull(_NLL, 1);
	int TM_index = (int)lua_tointeger(_NLL, 1);
	
	int TM_Ret = OBJECT_getY(TM_index);

	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_Obj_GetFloor(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckObjIndexNull(_NLL, 1);
	int TM_index = (int)lua_tointeger(_NLL, 1);
	
	int TM_Ret = OBJECT_getFloor(TM_index);

	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_Obj_SetType(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckObjIndexNull(_NLL, 1);
	int TM_index = (int)lua_tointeger(_NLL, 1);
	int TM_vl = (int)lua_tointeger(_NLL, 2);

	int TM_Ret = OBJECT_setType(TM_index, TM_vl);

	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_Obj_SetCharType(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckObjIndexNull(_NLL, 1);
	int TM_index = (int)lua_tointeger(_NLL, 1);
	int TM_vl = (int)lua_tointeger(_NLL, 2);

	OBJECT_setchartype(TM_index, TM_vl);

	LRetNull(_NLL);
}

int NPC_Lua_Obj_SetX(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckObjIndexNull(_NLL, 1);
	int TM_index = (int)lua_tointeger(_NLL, 1);
	int TM_vl = (int)lua_tointeger(_NLL, 2);

	int TM_Ret = OBJECT_setX(TM_index, TM_vl);

	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_Obj_SetY(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckObjIndexNull(_NLL, 1);
	int TM_index = (int)lua_tointeger(_NLL, 1);
	int TM_vl = (int)lua_tointeger(_NLL, 2);

	int TM_Ret = OBJECT_setY(TM_index, TM_vl);

	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_Obj_SetFloor(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckObjIndexNull(_NLL, 1);
	int TM_index = (int)lua_tointeger(_NLL, 1);
	int TM_vl = (int)lua_tointeger(_NLL, 2);

	int TM_Ret = OBJECT_setFloor(TM_index, TM_vl);

	LRetInt(_NLL, TM_Ret);
}
#endif //#ifdef _JZ_NEWSCRIPT_LUA
