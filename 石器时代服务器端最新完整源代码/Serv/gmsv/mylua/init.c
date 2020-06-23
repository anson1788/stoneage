/*
** $Id: linit.c,v 1.14.1.1 2007/12/27 13:02:25 roberto Exp $
** Initialization of libraries for lua.c
** See Copyright Notice in lua.h
*/


#define linit_c
#define LUA_LIB

#include "lua.h"

#include "lualib.h"
#include "lauxlib.h"
#include "mylua/base.h"

static const luaL_Reg lualibs[] = {
	{"char", 								luaopen_Char},
	{"npc", 								luaopen_NPC},
	{"lssproto", 						luaopen_Lssproto},
	{"battle", 							luaopen_Battle},
	{"obj", 								luaopen_Object},
#ifdef _ALLBLUES_LUA_1_1
	{"map", 								luaopen_Map},
	{"other", 							luaopen_Other},
	{"config", 							luaopen_Config},
#endif
#ifdef _ALLBLUES_LUA_1_2
	{"item", 								luaopen_Item},
	{"magic", 							luaopen_Magic},
#endif
#ifdef _OFFLINE_SYSTEM
	{"offline", 						luaopen_Offline},
#endif
#ifdef _ALLBLUES_LUA_1_4
	{"enemytemp", 					luaopen_Enemytemp},
#ifdef _SASQL
	{"sasql", 							luaopen_Sasql},
#endif
#endif
	{"net", 								luaopen_Net},
	{"saacproto", 					luaopen_Saacproto},
#ifdef _ALLBLUES_LUA_1_8
#ifdef _PROFESSION_SKILL			// WON ADD 人物职业技能
	{"Professionskill", 		luaopen_ProfessionSkill},
#endif
	{"family", 							luaopen_Family},
	{"petskill", 						luaopen_PetSkill},
#endif
  {NULL, NULL}
};


LUALIB_API void luaAB_openlibs (lua_State *L) {
  const luaL_Reg *lib = lualibs;
  for (; lib->func; lib++) {
    lua_pushcfunction(L, lib->func);
    lua_pushstring(L, lib->name);
    lua_call(L, 1, 0);
  }
}

