#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../wgs/tea.h"
#include "../systeminc/action.h"
#include "../newproto/autil.h"
#include "winlua.h"
#include <locale.h>
#include <io.h>
#ifdef _WIN_LUAJIT_
static const char *progname = "lua";
MY_Lua MYLua;
lua_State* FindWinIntLua( int wintype, char *data )
{
	lua_State *lua = NULL;
	if (lua == NULL){
		char filename[256];
#ifdef _RELUA_
		sprintf_s(filename, "%s//win//%d.lua", _LUA_PATCH_, wintype);
#else
#ifdef _SA_VERSION_25
		sprintf_s(filename, "%s//win//%d.es", _LUA_PATCH_, wintype);
#endif

#endif
		lua = FindLua(filename);
		if (lua == NULL)return NULL;
	}
	lua_getglobal(lua, "WindowInit");
	if (!lua_isfunction(lua, -1)){
		lua_pop(lua, 1);
		return NULL;
	}
	lua_pushstring(lua, data); 
	docall(lua, 1, 1);
	return lua;
}

ACTION * FreeCreateWinTypeLua( lua_State *lua )
{
	if (lua == NULL){
		return NULL;
	}

	lua_getglobal(lua, "CreateWinType");

	if (!lua_isfunction(lua, -1)) {
		lua_pop(lua, 1);
		return NULL;
	}

	int TM_Ret = lua_pcall(lua, 0, 1, 0);

	if (TM_Ret != 0){
		printf("入口：FreeCreateWinTypeLua 错误内容:%d(%s)\n", TM_Ret, lua_tostring(lua, -1));
		lua_pop(lua, 1);
		return NULL;
	}

	if (!lua_isnumber(lua, -1))
		return NULL;

	int ret = (int)lua_tonumber(lua, -1);
	
	lua_pop(lua, 1);

	return (ACTION *)ret;
}

ACTION * FreeServerWinTypeLua( lua_State *lua )
{
	if (lua == NULL){
		return NULL;
	}
	lua_getglobal(lua, "ServerWindowType");
	int TM_Ret = lua_pcall(lua, 0, 1, 0);

	if (TM_Ret != 0){
		printf("入口：FreeServerWinTypeLua 错误内容:%d(%s)\n", TM_Ret, lua_tostring(lua, -1));
		lua_pop(lua, 1);
		return NULL;
	}
	if (!lua_isnumber(lua, -1))
		return NULL;
	int ret = (int)lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	return (ACTION *)ret;
}


static const luaL_Reg lualibs[] = {
	{"Win",			luaopen_Win},
	{"lssproto",	luaopen_Lssproto},
	{"sa",			luaopen_Sa},
	{"ServerList",	luaopen_ServerList},
	{"data",		luaopen_Data},
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

int getArrayInt(lua_State *L, int idx) {
	int result = 0;
  lua_pushnumber(L, idx + 1);
  lua_gettable(L, -2);
  result = (int)lua_tonumber(L, -1);
  lua_pop(L, 1);
  return result;
}

static void l_message (const char *pname, const char *msg) {
  if (pname) fprintf(stderr, "%s: ", pname);
  fprintf(stderr, "%s\n", msg);
  fflush(stderr);
}

static int report (lua_State *L, int status) {
  if (status && !lua_isnil(L, -1)) {
    const char *msg = lua_tostring(L, -1);
    if (msg == NULL) msg = "(error object is not a string)";
    l_message(progname, msg);
    lua_pop(L, 1);
  }
  return status;
}

static int traceback (lua_State *L) {
  if (!lua_isstring(L, 1))  /* 'message' not a string? */
    return 1;  /* keep it intact */
  lua_getfield(L, LUA_GLOBALSINDEX, "debug");
  if (!lua_istable(L, -1)) {
    lua_pop(L, 1);
    return 1;
  }
  lua_getfield(L, -1, "traceback");
  if (!lua_isfunction(L, -1)) {
    lua_pop(L, 2);
    return 1;
  }
  lua_pushvalue(L, 1);  /* pass error message */
  lua_pushinteger(L, 2);  /* skip this function and traceback */
  lua_call(L, 2, 1);  /* call debug.traceback */
  return 1;
}


int docall (lua_State *L, int narg, int clear) {
  int status;
  int base = lua_gettop(L) - narg;  /* function index */
  lua_pushcfunction(L, traceback);  /* push traceback function */
  lua_insert(L, base);  /* put it under chunk and args */
  status = lua_pcall(L, narg, (clear ? 0 : LUA_MULTRET), base);
  lua_remove(L, base);
  if(status != 0){
	printf("入口：docall 错误内容:%d(%s)\n", status, lua_tostring(L, -1));
	int iTop = lua_gettop(L);
	lua_pop(L,iTop);
	lua_gc(L, LUA_GCCOLLECT, 0);
  }

  return status;
}

int dofile (lua_State *L, const char *name) {
  int status = luaL_loadfile(L, name) || docall(L, 0, 1);
  return report(L, status);
}

int decryptLUA(lua_State *L, char *filename){
	FILE *f;
	char loadfilename[256];
	int luamaxlen = 0;
	unsigned char *luabuff;
	strcpy(loadfilename, filename);
	fopen_s(&f,loadfilename,"rb");//打开补丁1
	if (f!=NULL){
		fseek(f,0,SEEK_END);
		luamaxlen = ftell(f);
		luabuff = (unsigned char*)malloc(luamaxlen+1);
		fseek(f,0,SEEK_SET);
		fread(luabuff, 1, luamaxlen, f);
		fclose(f);
		luabuff[luamaxlen]=0;
	}else{
		return -100;
	}
	TEA解密((long*)luabuff,(luamaxlen/4),(long *)_LUA_KEY_);
	int status = luaL_dostring(L,(const char *)luabuff);
	free(luabuff);
	return status;
}


int myluaload (char *filename) 
{
	MY_Lua *salua = &MYLua;
	while(salua->next!=NULL){
		if(strcmp(salua->luapath, filename) == 0){
			return FALSE;
		}
		salua = salua->next;
	}
	salua->luapath = new char[strlen(filename)+1];
	memset( salua->luapath, 0 , strlen(filename)+1);
	strcpy(salua->luapath, filename);
	salua->next = (tagMYLua *)new MY_Lua;
	memset( salua->next, 0 , sizeof(MY_Lua) );
	if(salua->next == NULL)return EXIT_FAILURE;

	salua->lua = lua_open();
	if (salua->lua == NULL) {
		return FALSE;
	}
	lua_gc(salua->lua, LUA_GCSTOP, 0);
	luaL_openlibs(salua->lua);
	luaAB_openlibs(salua->lua);
	lua_gc(salua->lua, LUA_GCRESTART, 0);
#ifdef _RELUA_
	int re = dofile(salua->lua, filename);
	if(re!=0){
		printf("文件：%s 错误提示：%s 行数：%d\n",filename,(char *)lua_tostring(salua->lua, -1),re);
		lua_gc(salua->lua, LUA_GCCOLLECT, 0);
		return TRUE;	
	}
#else
	int re = decryptLUA(salua->lua, filename);
	if(re != 0){
		if(re==-100) return TRUE;
		printf("文件：%s 错误提示：%s 行数：%d\n",filename,(char *)lua_tostring(salua->lua, -1),re);
		lua_gc(salua->lua, LUA_GCCOLLECT, 0);
		return TRUE;
	}
#endif
	lua_getglobal(salua->lua, "main");
	if (lua_isfunction(salua->lua, -1)) {
		docall(salua->lua, 0, 1);
	}
	return TRUE;
}

int remyluaload (char *filename) 
{
	MY_Lua *salua = &MYLua;

	while(salua->next!=NULL){
		if(strlen(salua->luapath) > 0){
			if(strlen(filename)>0){
				if(strstr(salua->luapath, filename) == 0){
					salua = salua->next;
					continue;
				}
			}
			lua_gc(salua->lua, LUA_GCSTOP, 0); 
			luaL_openlibs(salua->lua); 
			luaAB_openlibs(salua->lua);
			lua_gc(salua->lua, LUA_GCRESTART, 0);
			dofile(salua->lua, salua->luapath);
			lua_getglobal(salua->lua, "data");
			if (lua_isfunction(salua->lua, -1)) {
			 docall(salua->lua, 0, 1);
			}
		}
		salua = salua->next;
	}
  return EXIT_SUCCESS;
}


int closemyluaload()
{
	MY_Lua *salua = &MYLua;
	while(salua->next!=NULL){
		lua_pop(salua->lua, 1);
		lua_close(salua->lua);
		salua = salua->next;
	}
	
  return EXIT_SUCCESS;
}

void LoadStoneAgeLUA(char *path)
{
	char filename[256];

	WIN32_FIND_DATA wfd; 
	HANDLE hFind; 
	lstrcpy(filename, path); 
	lstrcat(filename, "/"); 
	lstrcat(filename, "*.*"); // 找所有文件 
	
	hFind = FindFirstFile(filename, &wfd); 
	if (hFind == INVALID_HANDLE_VALUE) // 如果没有找到或查找失败 
		return; 
	do{
		
		if (wfd.cFileName[0] == '.') 
			continue; // 过滤这两个目录 
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{ 
			sprintf_s(filename, "%s//%s", path, wfd.cFileName);
			LoadStoneAgeLUA(filename);
		}else{
#ifdef _RELUA_
			if( strcmptail( wfd.cFileName, ".lua" ) == 0)
#else
#ifdef _SA_VERSION_25
			if( strcmptail( wfd.cFileName, "es" ) == 0)
#endif

#endif
			{
					char filename[256];
					memset(filename, 0, 256);
					sprintf_s(filename, "%s//%s", path, wfd.cFileName);
					myluaload(filename);
			}
		}
	}while (FindNextFile(hFind, &wfd)); 
	FindClose(hFind); // 关闭查找句柄 
}

void ReLoadStoneAgeLUA(char *filename)
{
	remyluaload(filename);
}

void NewLoadStoneAgeLUA(char *filename)
{
	char token[256];
	if(strlen(filename)>0){
		sprintf_s(token, "%s//%s", _LUA_PATCH_,filename);
		myluaload(token);
	}else{
		LoadStoneAgeLUA(_LUA_PATCH_);
	}
}

lua_State *FindLua(char *filename)
{
	MY_Lua *salua = &MYLua;
	char newfilename[256];
	sprintf_s(newfilename, "%s", filename);
	while(salua->next != NULL){
		if(strcmp(newfilename, salua->luapath) == 0){
			return salua->lua;
		}
		salua = salua->next;
	}
	return NULL;
}



int FreeGetBattleMap(int floor)
{
	lua_State *lua = NULL;
	if (lua == NULL){
		char filename[256];
#ifdef _RELUA_
		sprintf_s(filename, "%s//map//battlemap.lua",_LUA_PATCH_);
#else
#ifdef _SA_VERSION_25
		sprintf_s(filename, "%s//map//battlemap.es",_LUA_PATCH_);
#endif

#endif
		lua = FindLua(filename);
		if (lua == NULL)return NULL;
	}
	lua_getglobal(lua, "FreeGetBattleMap");
	if (!lua_isfunction(lua, -1)) {
		printf("FreeGetBattleMap 错误内容 (%s)\n", lua_tostring(lua, -1));
		lua_pop(lua, 1);
		return NULL;
	}
	lua_pushnumber(lua, floor); 
	int TM_Ret = lua_pcall(lua, 1, 1, 0);

	if (TM_Ret != 0){
		printf("入口：FreeGetBattleMap 错误内容:%d(%s)\n", TM_Ret, lua_tostring(lua, -1));
		lua_pop(lua, 1);
		return NULL;
	}
	if (!lua_isnumber(lua, -1))
		return NULL;
	int ret = (int)lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	return ret;
}


#endif


#ifdef _CHARTITLE_STR_
char* FreeGetTitleStr(int id)
{
	lua_State *lua = NULL;
	if (lua == NULL){
		char filename[256];
#ifdef _RELUA_
		sprintf_s(filename, "%s//title.lua",_LUA_PATCH_);
#else
#ifdef _SA_VERSION_25
		sprintf_s(filename, "%s//title.es",_LUA_PATCH_);
#endif

#endif
		lua = FindLua(filename);
		if (lua == NULL)return NULL;
	}
	lua_getglobal(lua, "GetTitleStr");
	if (!lua_isfunction(lua, -1)) {
		printf("GetTitleStr 错误内容 (%s)\n", lua_tostring(lua, -1));
		lua_pop(lua, 1);
		return NULL;
	}
	lua_pushnumber(lua, id); 
	int TM_Ret = lua_pcall(lua, 1, 1, 0);

	if (TM_Ret != 0){
		printf("入口：GetTitleStr 错误内容:%d(%s)\n", TM_Ret, lua_tostring(lua, -1));
		lua_pop(lua, 1);
		return NULL;
	}
	if (!lua_isstring(lua, -1))
		return NULL;
	char *ret = (char *)lua_tostring(lua, -1);
	lua_pop(lua, 1);
	return ret;
}
#endif

