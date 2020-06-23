#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/netmain.h"
#include "winlua.h"

#include <locale.h>
#include <io.h>
#ifdef _WIN_LUAJIT_
extern int nGroup;
extern struct gameserver gmsv[];
extern struct gamegroup  gmgroup[];
static int SetGroupMaxNum(lua_State *L) 
{
	const int num = luaL_checkint(L, 1);
	nGroup = num;
	return 1;
}

static int SetGroupList(lua_State *L) 
{
	size_t l;
	const int id = luaL_checkint(L, 1);
	const int GroupUse = luaL_checkint(L, 2);
	const int ServerMaxNum = luaL_checkint(L, 3);
	const int ServerIndex = luaL_checkint(L, 4);
	const char *GroupName = luaL_checklstring(L, 5, &l);
	gmgroup[id].used = GroupUse;
	gmgroup[id].num = ServerMaxNum;
	gmgroup[id].startindex = ServerIndex;
	strcpy(gmgroup[id].name, GroupName);
	return 1;
}



static int SetServerList(lua_State *L) 
{
	size_t l;
	const int id = luaL_checkint(L, 1);
	const char *ServerIP = luaL_checklstring(L, 2, &l);
	const char *ServerPort = luaL_checklstring(L, 3, &l);
	const char *ServerName = luaL_checklstring(L, 4, &l);

	strcpy(gmsv[id].ipaddr, ServerIP);
	strcpy(gmsv[id].port, ServerPort);
	strcpy(gmsv[id].name, ServerName);

	gmsv[id].used = '1';
	return 1;
}



static const luaL_Reg ServerListlib[] = {
	{"SetGroupList",		SetGroupList},
	{"SetGroupMaxNum",		SetGroupMaxNum},
	{"SetServerList",		SetServerList},
	{NULL,					NULL}
};

LUALIB_API int luaopen_ServerList (lua_State *L) {
	luaL_register(L, "ServerList", ServerListlib);
	return 1;
}
#endif


