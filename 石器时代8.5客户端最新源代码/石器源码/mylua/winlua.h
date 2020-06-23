#pragma comment(lib,"lua51.lib")
extern "C" {
#include "lua.h" 
#include "lauxlib.h"
#include "lualib.h"
}
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN_LUAJIT_
int myluaload (char *filename);
int remyluaload (char *filename);
void LoadStoneAgeLUA(char *path);
int closemyluaload();
int dofile (lua_State *L, const char *name);
int docall (lua_State *L, int narg, int clear);
int getArrayInt(lua_State *L, int idx);
LUALIB_API void luaAB_openlibs (lua_State *L);
LUALIB_API int luaopen_Win(lua_State *L);
LUALIB_API int luaopen_Lssproto(lua_State *L);
LUALIB_API int luaopen_Sa (lua_State *L);
LUALIB_API int luaopen_ServerList (lua_State *L);
LUALIB_API int luaopen_Data (lua_State *L);
lua_State *FindLua(char *filename);
lua_State* FindWinIntLua( int wintype, char *data );
ACTION * FreeCreateWinTypeLua( lua_State *lua );
ACTION * FreeServerWinTypeLua( lua_State *lua );
void ReLoadStoneAgeLUA(char *filename);
void LC_LUA_ITEM_DIRECTIONS(int x,int y,char *name,int iamge);

typedef struct tagMYLua
{
	lua_State *lua;
	char *luapath;
	struct tagMYLua *next;
}MY_Lua;

LUALIB_API int luaopen_Win (lua_State *L);
LUALIB_API int luaopen_ServerList (lua_State *L);
LUALIB_API int luaopen_Sa (lua_State *L);
LUALIB_API int luaopen_Lssproto (lua_State *L);
LUALIB_API int luaopen_Data (lua_State *L);
int FreeGetBattleMap(int floor);
int CheckPlayOnLine(int index,char * account,char *mac);
#endif

