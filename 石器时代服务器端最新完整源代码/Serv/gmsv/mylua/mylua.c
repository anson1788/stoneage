#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>  
#include  "autil.h"
#include "buf.h"
#include "util.h"
#define lua_c
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "longzoro/version.h"
#include "mylua/mylua.h"


#ifdef _ALLBLUES_LUA   

MY_Lua MYLua;

static const char *progname = "lua";

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


int _docall(lua_State *L, int narg, int clear ,char *file) {
  int status;
  int base = lua_gettop(L) - narg;  /* function index */
  lua_pushcfunction(L, traceback);  /* push traceback function */
  lua_insert(L, base);  /* put it under chunk and args */

  status = lua_pcall(L, narg, (clear ? 0 : LUA_MULTRET), base);

  lua_remove(L, base);  /* remove traceback function */
  /* force a complete garbage collection in case of errors */
  if (status != 0){
	printf("º¯Êý´íÎó=%s  ´íÎóÌáÊ¾£º%s\n",file,(char *)lua_tostring(L, -1));
	 lua_gc(L, LUA_GCCOLLECT, 0);
  }
  return status;
}


int dofile (lua_State *L, const char *name) {
  int status = luaL_loadfile(L, name) || docall(L, 0, 1);
  return report(L, status);
}

char crypto[]=DEFAULTTABLE;

void DecryptLua(char* buff, int len, int id)
{
	
	int i;
	int cryptolen = strlen(crypto);
  for(i=0;i<len;i++)   
  {
  	buff[i] ^= crypto[(i) % cryptolen];
  	buff[i] += id;
  }
}

#ifdef _CRYPTO_LUA
void CryptoLua(char* buff, int len, int id)
{
	
	int i;
	int cryptolen = strlen(crypto);
  for(i=0;i<len;i++)   
  {
  	buff[i] -= id;
  	buff[i] ^= crypto[(i) % cryptolen];
  }
}

void CryptoAllbluesLUA(char *path, int flg, int id)
{
	struct dirent* ent = NULL;
	char filename[256];
	DIR *pDir;
	pDir=opendir(path);
	FILE *f;
	while(NULL != (ent=readdir(pDir)))
	{
		if(ent->d_name[0] == '.')continue;
		if (ent->d_type==8){

		  if( (strcmptail( ent->d_name, ".lua" ) == 0 && flg == 1)
		  	 || (strcmptail( ent->d_name, ".allblues") == 0  && flg == 0)){
		  	char filename[256];
		  	char token[256];
		  	sprintf(filename, "%s/%s", path, ent->d_name);

				char *luabuff = NULL;
				int luamaxlen = 0;

      	if ((f=fopen(filename,"r"))!=NULL) {
					fseek(f,0,SEEK_END);
					luamaxlen = ftell(f);
					luabuff = (char*)malloc(luamaxlen + 1); 
					memset(luabuff, 0, luamaxlen);
					fseek(f,0,SEEK_SET);
					fread(luabuff, luamaxlen, 1, f);
					fclose(f);
				}

				if( flg == 1 ){
					CryptoLua(luabuff, luamaxlen, id);
					sprintf(token, "%s.allblues", filename);
				}else if( flg == 0 ){
					DecryptLua(luabuff, luamaxlen, id);
					sprintf(token, "%s", filename);

					token[strlen(token)-9] = '\0';
				}
				if ((f=fopen(token,"w+"))!=NULL) {
					fwrite(luabuff,1,luamaxlen,f);
					fclose(f);
				}
				free(luabuff);
    	}
		}else{
			sprintf(filename, "%s/%s", path, ent->d_name);
			CryptoAllbluesLUA(filename, flg, id);
		}
	}
}
#endif
void Cryptodofile(lua_State *L, char *filename)
{
	FILE *f;
	
	char *luabuff;
	char *cfbuff = NULL;
	char *buff;
	int luamaxlen = 0;
	int cfmaxlen = 0;
	char head[]="\nfunction init()\n";
	char end[]="\nend\n";
	char filenamecf[256];
	char loadfilename[256];
	strcpy(loadfilename, filename);
	
	if ((f=fopen(loadfilename,"r"))!=NULL) {
		fseek(f,0,SEEK_END);
		luamaxlen = ftell(f);
		luabuff = (char*)malloc(luamaxlen + 1); 
		memset(luabuff, 0, luamaxlen + 1);
		fseek(f,0,SEEK_SET);
		fread(luabuff, luamaxlen, 1, f);
		fclose(f);
	}else{
		return;
	}
/*
	CryptoLua(buff, maxlen, _ATTESTAION_ID);
	char token[256];
	sprintf(token, "%s.allblues", filename);
		if ((f=fopen(token,"w+"))!=NULL) {
			fwrite(buff,1,maxlen,f);
			fclose(f);
		}
*/


	DecryptLua(luabuff, luamaxlen, _ATTESTAION_ID);
	loadfilename[strlen(loadfilename)-9] = '\0';

	sprintf(filenamecf, "%s.cf", filename);
	if ((f=fopen(filenamecf,"r"))!=NULL) {
		fseek(f,0,SEEK_END);
		cfmaxlen = ftell(f);
		cfbuff = (char*)malloc(cfmaxlen + 1); 
		memset(cfbuff, 0, cfmaxlen + 1);
		fseek(f,0,SEEK_SET);
		fread(cfbuff, cfmaxlen, 1, f);

		fclose(f);
	}
	
	int len = luamaxlen + cfmaxlen + strlen(head) + strlen(end) + 1;
	buff = (char*)malloc(len); 
	memset(buff, 0, len);
	len = 0;
	memcpy( buff + len, head, strlen(head));
	len += strlen(head);
	if(cfmaxlen > 0){
		memcpy( buff + len, cfbuff, cfmaxlen);
		free(cfbuff);
	}
	len += cfmaxlen;
	memcpy( buff + len, end, strlen(end));
	len += strlen(end);
	memcpy( buff + len, luabuff, luamaxlen);

  luaL_dostring(L, buff);
	
	free(buff);
	free(luabuff);
}

int myluaload (char *filename) 
{
	MY_Lua *mylua = &MYLua;
	while(mylua->next!=NULL){
		if(strcmp(mylua->luapath, filename) == 0){
			return FALSE;
		}
		mylua = mylua->next;
	}
	mylua->luapath = allocateMemory( strlen(filename) );
	memset( mylua->luapath, 0 , strlen(filename) );
  strcpy(mylua->luapath, filename);
	mylua->next = allocateMemory( sizeof(MY_Lua) );
	memset( mylua->next, 0 , sizeof(MY_Lua) );
	if(mylua->next == NULL)return EXIT_FAILURE;

  mylua->lua = lua_open();  /* create state */

  if (mylua->lua == NULL) {
    return FALSE;
  }

  lua_gc(mylua->lua, LUA_GCSTOP, 0);  /* stop collector during initialization */
  luaL_openlibs(mylua->lua);  /* open libraries */
  luaAB_openlibs(mylua->lua);
  lua_gc(mylua->lua, LUA_GCRESTART, 0);

	if(strcmptail( filename, ".allblues" ) == 0){
		Cryptodofile(mylua->lua, filename);
	}else{
		dofile(mylua->lua, filename);
	}

	lua_getglobal(mylua->lua, "init");
  if (lua_isfunction(mylua->lua, -1)) {
    docall(mylua->lua, 0, 1);
  }
  
  lua_getglobal(mylua->lua, "main");
  
	if (lua_isfunction(mylua->lua, -1)) {
    docall(mylua->lua, 0, 1);
  }

  return TRUE;
}

int remyluaload (char *filename) 
{
	MY_Lua *mylua = &MYLua;

	while(mylua->next!=NULL){
		if(strlen(mylua->luapath) > 0){
			if(strlen(filename)>0){
				//printf("luapath=%s  filename=%s\n",mylua->luapath,filename);
				if(strstr(mylua->luapath, filename) == 0){
					mylua = mylua->next;
					continue;
				}
			}
			
			lua_gc(mylua->lua, LUA_GCSTOP, 0); 
			luaL_openlibs(mylua->lua); 
			luaAB_openlibs(mylua->lua);
			lua_gc(mylua->lua, LUA_GCRESTART, 0);
			if(strcmptail( mylua->luapath, ".allblues" ) == 0){
				if ((fopen(mylua->luapath,"r"))==NULL) {
					mylua = mylua->next;
					continue;
				}
				Cryptodofile(mylua->lua, mylua->luapath);
			}else{
				dofile(mylua->lua, mylua->luapath);
			}
			
			lua_getglobal(mylua->lua, "data");
			  
			if (lua_isfunction(mylua->lua, -1)) {
			  docall(mylua->lua, 0, 1);
			}
			
		}
		mylua = mylua->next;
	}
	
  return EXIT_SUCCESS;
}


int closemyluaload()
{
	MY_Lua *mylua = &MYLua;
	while(mylua->next!=NULL){
		lua_pop(mylua->lua, 1);
		lua_close(mylua->lua);
		mylua = mylua->next;
	}
	
  return EXIT_SUCCESS;
}


#endif

