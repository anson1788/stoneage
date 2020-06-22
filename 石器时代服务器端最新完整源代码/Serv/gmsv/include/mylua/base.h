#ifndef __CHARBASE_H__
#define __CHARBASE_H__
#include "char_base.h"
#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"

typedef struct tagCharBase
{
    char  field[128];
    int 	element;
}CharBase;

const int getCharBaseValue(lua_State *L, int narg, CharBase *charbase, int num);

LUALIB_API int luaopen_Char (lua_State *L);
LUALIB_API int luaopen_NPC (lua_State *L);
LUALIB_API int luaopen_Lssproto (lua_State *L);
LUALIB_API int luaopen_Battle (lua_State *L);
LUALIB_API int luaopen_Object (lua_State *L);

#ifdef _ALLBLUES_LUA_1_1
LUALIB_API int luaopen_Map (lua_State *L);
LUALIB_API int luaopen_Other (lua_State *L);
LUALIB_API int luaopen_Config (lua_State *L);
#endif
#ifdef _ALLBLUES_LUA_1_2
LUALIB_API int luaopen_Item (lua_State *L);
LUALIB_API int luaopen_Magic (lua_State *L);
#endif
#ifdef _ALLBLUES_LUA_1_2
LUALIB_API int luaopen_Offline (lua_State *L);
#endif
#ifdef _ALLBLUES_LUA_1_4
LUALIB_API int luaopen_Enemytemp (lua_State *L);
LUALIB_API int luaopen_Sasql (lua_State *L);
#endif

LUALIB_API int luaopen_Net (lua_State *L);

LUALIB_API int luaopen_Saacproto (lua_State *L);

#ifdef _ALLBLUES_LUA_1_8 
LUALIB_API int luaopen_Family (lua_State *L);
LUALIB_API int luaopen_PetSkill (lua_State *L);
#ifdef _PROFESSION_SKILL			// WON ADD 人物职业技能
LUALIB_API int luaopen_ProfessionSkill (lua_State *L);
#endif
#endif
#endif
