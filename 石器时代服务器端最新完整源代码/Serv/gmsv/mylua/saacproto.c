#include <string.h>
#include "char.h"
#include "char_base.h"
#include "mylua/base.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "saacproto_cli.h"

#ifdef _ALLBLUES_LUA
static int ACFixFMPoint(lua_State *L) 
{
	size_t l;
  char *winfmname=luaL_checklstring(L, 1, &l);
	const int winfmindex = luaL_checkint(L, 2);
	const int winindex = luaL_checkint(L, 3);
	char *losefmname=luaL_checklstring(L, 4, &l);
	const int losefmindex = luaL_checkint(L, 5);
	const int loseindex = luaL_checkint(L, 6);
	const int village = luaL_checkint(L, 7);

	saacproto_ACFixFMPoint_send(acfd, winfmname, winfmindex, winindex, losefmname, losefmindex, loseindex, village);
	
	return 1;
}

static int ACFMDetail(lua_State *L) 
{
	size_t l;
	char *fmname=luaL_checklstring(L, 1, &l);
	const int fmindex = luaL_checkint(L, 2);
	const int tempindex = luaL_checkint(L, 3);
	const int fd = luaL_checkint(L, 4);
	saacproto_ACFMDetail_send( acfd, fmname, fmindex, tempindex, CONNECT_getFdid(fd));
	return 1;
}

static int ItemPetLocked(lua_State *L) 
{
	size_t l;
	int charaindex=luaL_checkint(L, 1);
	int fd = getfdFromCharaIndex( charaindex);
	char *data=luaL_checklstring(L, 2, &l);
	saacproto_ItemPetLocked_send( fd, CHAR_getChar(charaindex, CHAR_CDKEY), data);
	return 1;
}

static const luaL_Reg Saacprotolib[] = {
	{"ACFixFMPoint", 	ACFixFMPoint},
	{"ACFMDetail", 		ACFMDetail},
	{"ItemPetLocked", ItemPetLocked},
  {NULL, 						NULL}
};

LUALIB_API int luaopen_Saacproto (lua_State *L) {
  luaL_register(L, "saacproto", Saacprotolib);
  return 1;
}

#endif

