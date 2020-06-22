#ifndef __CHAR_TALK_H__
#define __CHAR_TALK_H__

#include "common.h"
#include "util.h"
#include "net.h"
#ifdef _ALLBLUES_LUA
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#endif
void CHAR_getMessageBody(char* message, char* kind, int kindlen,
            char** body);

int CHAR_getChatMagicFuncLevel(char* name,BOOL isDebug);
int CHAR_getChatMagicFuncNameAndString( int ti, char* name, char *usestring, int level, BOOL isDebug);
int CHAR_getChatMagicFuncMaxNum( void);

void OneByOneTkChannel ( int fd , char *tmp1 , char *tmp2 , int color) ; 

typedef void (*CHATMAGICFUNC)(int,char*);
CHATMAGICFUNC CHAR_getChatMagicFuncPointer(char* name, BOOL isDebug);


#ifdef _RE_GM_COMMAND
int re_gm_command();
#endif


#ifdef _FILTER_TALK
int ReadFilterTalk();
char *getFilterTalk(int index);
int getFilterTalkNum();
#endif

#ifdef _ALLBLUES_LUA_1_2 
typedef struct tagMAGIC_LuaFunc
{
	lua_State *lua;
	char *luafuncname;
	char *luafunctable;
	int gmlevel;
	char *usestring;
  struct tagMAGIC_LuaFunc	*next;
}MAGIC_LuaFunc;

BOOL MAGIC_addLUAListFunction( lua_State *L, const char *luafuncname, const char *luafunctable, int gmlevel, char *usestring );
BOOL MAGIC_getLUAListFunction( char *luafuncname, int gmlevel, int charaindex, char *data );

#endif

#endif 
