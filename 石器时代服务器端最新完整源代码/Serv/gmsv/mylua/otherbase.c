#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "char.h"
#include "char_base.h"
#include "mylua/base.h"
#include "util.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "lssproto_serv.h"
#include "npc_scheduleman.h"
#include "mylua/mylua.h"

#ifdef _ALLBLUES_LUA   
extern MY_Lua MYLua;

static int nowtime(lua_State *L) 
{
	lua_pushinteger(L, (int) time( NULL ));
	return 1;
}

static int strtoi(lua_State *L) 
{
	size_t l;
	char *data=luaL_checklstring(L, 1, &l);
	lua_pushinteger(L, atoi(data));
	return 1;
}

static int getString(lua_State *L) 
{
	size_t l;
	char *data = luaL_checklstring(L, 1, &l);
	char *delim = luaL_checklstring(L, 2, &l);
	const int index = luaL_checkint(L, 3);
	
	char token[256];
	if(getStringFromIndexWithDelim( data, delim, index, token, sizeof( token))){
		lua_pushstring(L, token);
	}else{
		lua_pushstring(L, "");
	}
	return 1;
}

static int CallFunction(lua_State *L) 
{
	size_t l;
	char *funcname = luaL_checklstring(L, 1, &l);
	char *filename = luaL_checklstring(L, 2, &l);
	char newfilename[256];
	luaL_checktype(L, 3, LUA_TTABLE);
	int n = luaL_getn(L, 3);

	lua_State *lua = NULL;

	MY_Lua *mylua = &MYLua;
  while(mylua->next != NULL){
  	if(strcmptail( mylua->luapath, ".allblues" ) == 0 
  		&& strcmptail( filename, ".lua" ) == 0 ){
  		sprintf(newfilename, "%s.allblues", filename);
  	}else{
  		sprintf(newfilename, "%s", filename);
  	}

  	if(strcmp(newfilename, mylua->luapath) == 0){
  		lua = mylua->lua;
			break;
		}
  	mylua = mylua->next;
  }
  if (lua == NULL) {
		return FALSE;
	}

	lua_getglobal(lua, funcname);

	if (!lua_isfunction(lua, -1)) {
    return FALSE;
  }

	int i;

	for(i = 0; i < n; i++){
		lua_pushnumber(lua, getArrayInt(L, i)); 
	}
	int TM_Ret = lua_pcall(lua, n, 1, 0);
	if(TM_Ret != 0)
	{
		//失败-输出错误信息
		print("CallFunction Lua Err :%d(%s)\n", TM_Ret, lua_tostring(lua, -1));
		//出栈
		lua_pop(lua, 1);
		return FALSE;
	}
	lua_isnumber(lua, -1);

	int ret = lua_tonumber(lua, -1);
	lua_pushinteger(L, ret);
  return TRUE;
}


static int c10to62(lua_State *L) 
{
	const int index = luaL_checkint(L, 1);
	
	char token[256];
	cnv10to62( index,	token, sizeof(token));
	
	lua_pushstring(L, token);
	return 1;
}
#ifdef _OFFLINE_SYSTEM
int luaplayernum=0;
static int setLuaPLayerNum(lua_State *L) 
{
	const int num = luaL_checkint(L, 1);
	
	luaplayernum = num;
	return 1;
}

static int getLuaPLayerNum(lua_State *L) 
{
	lua_pushinteger(L, luaplayernum);
	return 1;
}
extern playeronlinenum;

static int getOnlinePlayer(lua_State *L) 
{
	lua_pushinteger(L, playeronlinenum + luaplayernum + getPlayerNum());
	return 1;
}
#endif

static int NumToAlpha(lua_State *L) 
{
	const int Num = luaL_checkint(L, 1);
	
	char Alpha[10];
	int i;
	for(i = 0; i < 9; i ++){
		int validation = Num;
		validation /= pow((double)26, i);
		if(validation == 0)break;
		Alpha[i] = 'A' + (validation- 1) % 26;
	}
	Alpha[i] = '\0';
	lua_pushstring(L, Alpha);
	return 1;
}

static int AlphaToNum(lua_State *L) 
{
	size_t l;
	char *Alpha = luaL_checklstring(L, 1, &l);
	
	int Num = 0;
	int i;
	
	for(i=0; i < strlen(Alpha); i ++){
		if(Alpha[i] >= 'A' && Alpha[i] <= 'Z'){
			Num += (((Alpha[i] - 'A') + 1) * pow(26, i));
		}else if(Alpha[i] >= 'a' && Alpha[i] <= 'z'){
			Num += (((Alpha[i] - 'a') + 1) * pow(26, i));
		}
	}
	
	lua_pushinteger(L, Num);
	return 1;
}

static int DataAndData(lua_State *L) 
{
	const int data1 = luaL_checkint(L, 1);
	const int data2 = luaL_checkint(L, 2);
	
	lua_pushinteger(L, (data1 & 1 << data2));
	return 1;
}

static int DataOrData(lua_State *L)
{
	const int data1 = luaL_checkint(L, 1);
	const int data2 = luaL_checkint(L, 2);

	lua_pushinteger(L, (data1 | 1 << data2));
	return 1;
}

static int getFmPKsGindex(lua_State *L)
{
	const int fmpks_pos = luaL_checkint(L, 1) * MAX_SCHEDULE;

	lua_pushinteger(L, ( fmpks[fmpks_pos+1].guest_index));
	return 1;
}

static int getFmPKsHindex(lua_State *L)
{
	const int fmpks_pos = luaL_checkint(L, 1) * MAX_SCHEDULE;

	lua_pushinteger(L, ( fmpks[fmpks_pos+1].host_index));
	return 1;
}

static int getFmPKsDueltime(lua_State *L)
{
	const int fmpks_pos = luaL_checkint(L, 1) * MAX_SCHEDULE;

	lua_pushinteger(L, ( fmpks[fmpks_pos+1].dueltime));
	return 1;
}

static int getFmPKsPreparetime(lua_State *L)
{
	const int fmpks_pos = luaL_checkint(L, 1) * MAX_SCHEDULE;

	lua_pushinteger(L, ( fmpks[fmpks_pos+1].prepare_time));
	return 1;
}


static int Hash(lua_State *L)
{
	size_t l;
	char *charId = luaL_checklstring(L, 1, &l);

	int hash = 0;
	int j;
	for( j=0; j<strlen(charId); j++) {
		hash += (int)charId[j];
		hash = hash % 256;
	}

	lua_pushinteger(L, hash);

	return 1;
}

static int NumLeftToNum(lua_State *L)
{
	const int value = luaL_checkint(L, 1);
	const int flg = luaL_checkint(L, 2);

	lua_pushinteger(L, (value << flg));

	return 1;
}

static int NumRightToNum(lua_State *L)
{
	const int value = luaL_checkint(L, 1);
	const int flg = luaL_checkint(L, 2);

	lua_pushinteger(L, ((value >> flg) & 0xFF));

	return 1;
}

static int Random(lua_State *L)
{
	const int rand1 = luaL_checkint(L, 1);
	const int rand2 = luaL_checkint(L, 2);

	lua_pushinteger(L, RAND(rand1,rand2));
	return 1;

}

static const luaL_Reg otherlib[] = {
	{"time", 										nowtime},
	{"atoi", 										strtoi},
	{"getString", 							getString},
	{"CallFunction", 					CallFunction},
	{"c10to62", 								c10to62},
	{"NumToAlpha", 						NumToAlpha},
	{"AlphaToNum", 						AlphaToNum},
#ifdef _OFFLINE_SYSTEM
	{"setLuaPLayerNum", 			setLuaPLayerNum},
	{"getLuaPLayerNum", 			getLuaPLayerNum},
	{"getOnlinePlayer", 			getOnlinePlayer},
#endif
	{"DataAndData", 						DataAndData},
	{"DataOrData",							DataOrData},
	{"getFmPKsGindex", 				getFmPKsGindex},
	{"getFmPKsHindex", 				getFmPKsHindex},
	{"getFmPKsDueltime", 		getFmPKsDueltime},
	{"getFmPKsPreparetime", getFmPKsPreparetime},
	{"Hash", 										Hash},
	{"NumRightToNum", 				NumRightToNum},
	{"NumLeftToNum", 					NumLeftToNum},
	{"Random", 									Random},
  {NULL, 											NULL}
};

LUALIB_API int luaopen_Other (lua_State *L) {
  luaL_register(L, "other", otherlib);
  return 1;
}

#endif

