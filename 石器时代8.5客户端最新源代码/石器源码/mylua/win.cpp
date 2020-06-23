#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "winlua.h"
#include "../systeminc/character.h"
#include "../systeminc/tool.h"
#include "../systeminc/action.h"
#include "../systeminc/field.h"
#include "../systeminc/mouse.h"
#include "../systeminc/menu.h"
#include "../systeminc/netmain.h"
#include "../other/caryime.h"
#include "../oft/work.h"
#include <locale.h>
#include <io.h>
#ifdef _WIN_LUAJIT_
extern int cloasewindows;
extern SA_WINDOWS windows;

static int InitWindow(lua_State *L) 
{
	size_t l;
	const int winW = luaL_checkint(L, 1);
	const int winH = luaL_checkint(L, 2);
	const int msgWNLen = luaL_checkint(L, 3);
	const char *data = luaL_checklstring(L, 4, &l);
	extern void getStrSplit( char *, char *, int, int, int );
	makeStringFromEscaped((char *)data);
	getStrSplit(windows.message.data[0], (char *)data, 100, 25, msgWNLen);
	return 1;
}

static int NewPrint(lua_State *L) 
{
	size_t l;
	const char *data = luaL_checklstring(L, 1, &l);
	printf(data);
	return 1;
}



static int getMessageData(lua_State *L) 
{
	const int id = luaL_checkint(L, 1);
	lua_pushstring(L, windows.message.data[id]);
	return 1;
}
static int getMouseLeftCrick(lua_State *L) 
{
	lua_pushinteger(L, (mouse.onceState & MOUSE_LEFT_CRICK));
	return 1;
}

extern int indexWN;
static int getIndexWN(lua_State *L) 
{
	lua_pushinteger(L, indexWN);
	return 1;
}
extern int idWN;
static int getIdWN(lua_State *L) 
{
	lua_pushinteger(L,  idWN);
	return 1;
}
extern int buttonTypeWN;

static int getButtonTypeWN(lua_State *L) 
{
	lua_pushinteger(L,  buttonTypeWN);
	return 1;
}


static int getHitFontNo(lua_State *L) 
{
	lua_pushinteger(L,  HitFontNo);
	return 1;
}

static int getHitDispNo(lua_State *L) 
{
	lua_pushinteger(L,  HitDispNo);
	return 1;
}
extern int windowTypeWN;
extern short wnCloseFlag;
static int CloseWindow(lua_State *L) 
{
	windowTypeWN = -1;
	wnCloseFlag = 1;
	return 1;
}

static int CloseFlg(lua_State *L) 
{
	wnCloseFlag = 0;
	return 1;
}

static int DelWindow(lua_State *L) 
{
	if( CheckMenuFlag()
		|| ((joy_trg[ 0 ] & JOY_ESC) && GetImeString() == NULL)
		|| actBtn == 1
		|| menuBtn == 1
		|| disconnectServerFlag == TRUE
		|| wnCloseFlag == 1 
		|| cloasewindows == 1)
	{
		wnCloseFlag = 0;
		cloasewindows = 0;
		lua_pushinteger(L,  1);
	}else{
		lua_pushinteger(L,  0);
	}
	return 1;
}
void initStrBuffer( STR_BUFFER *, int, int, int, int, int );
static int SaInitStrBuffer(lua_State *L) 
{
	const int x = luaL_checkint(L, 1);
	const int y = luaL_checkint(L, 2);
	const int len = luaL_checkint(L, 3);
	const int color = luaL_checkint(L, 4);
	const int prio = luaL_checkint(L, 5);
	//初始化输入信息
	windows.input.buffer[0] = '\0';
	windows.input.cnt = 0;
	windows.input.cursor=0;
	initStrBuffer( &windows.input, x, y, len, color, prio );
	return 1;
}

static int SaInitNewStrBuffer(lua_State *L) 
{
	size_t l;
	const int x = luaL_checkint(L, 1);
	const int y = luaL_checkint(L, 2);
	const int len = luaL_checkint(L, 3);
	const int color = luaL_checkint(L, 4);
	const int prio = luaL_checkint(L, 5);
	const char *str = luaL_checklstring(L, 6, &l);
	//初始化输入信息
	int slen = strlen((char *)str);
	if(slen > 0){
		strcpy(windows.input.buffer, str);
		windows.input.cnt = slen;
		windows.input.cursor=slen;
	}else{
		windows.input.buffer[0] = '\0';
		windows.input.cnt = 0;
		windows.input.cursor=0;
		
	}	
	initStrBuffer( &windows.input, x, y, len, color, prio );
	
	return 1;
}

static int SaWinGetKeyInputFocus(lua_State *L) 
{
	GetKeyInputFocus( &windows.input );
	return 1;
}

static int SaStockFontBuffer2(lua_State *L) 
{
	StockFontBuffer2(&windows.input);
	return 1;
}
static int SaSetInputMsgNum(lua_State *L) 
{
	for(int i = 0; i < windows.input.cnt;i++)
	{
		if (windows.input.buffer[i] < '0' || windows.input.buffer[i] > '9')
		{
			if(i == 0){
				windows.input.buffer[i] = '1';
			}else{
				windows.input.buffer[i] = '0';
			}
		}
	}
	return 1;
}


static int SaGetInputMsg(lua_State *L) 
{
	lua_pushstring(L,  windows.input.buffer);
	return 1;
}
extern void ShowBottomLineString(int iColor,LPSTR lpstr);
static int SaShowBottomLineString(lua_State *L) 
{
	size_t l;
	const int color = luaL_checkint(L, 1);
	const char *data = luaL_checklstring(L, 2, &l);
	ShowBottomLineString(color,(char *)data);
	return 1;
}



static const luaL_Reg winlib[] = {
	{"InitWindow",			InitWindow},
	{"getMouseLeftCrick",	getMouseLeftCrick},
	{"getIndexWN",			getIndexWN},
	{"getIdWN",				getIdWN},
	{"CloseWindow",			CloseWindow},
	{"DelWindow",			DelWindow},
	{"CloseFlg",			CloseFlg},
	{"getButtonTypeWN",		getButtonTypeWN},
	{"getHitFontNo",		getHitFontNo},
	{"getHitDispNo",		getHitDispNo},
	{"InitStrBuffer",		SaInitStrBuffer},
	{"InitNewStrBuffer",	SaInitNewStrBuffer},
	{"GetKeyInputFocus",	SaWinGetKeyInputFocus},
	{"getMessageData",		getMessageData},
	{"StockFontBuffer2",	SaStockFontBuffer2},
	{"GetInputMsg",			SaGetInputMsg},
	{"SetInputMsgNum",		SaSetInputMsgNum},
	{"print",		NewPrint},
	{"ShowBottomLineString",		SaShowBottomLineString},
	{NULL,					NULL}
};

LUALIB_API int luaopen_Win (lua_State *L) {
	luaL_register(L, "win", winlib);
	return 1;
}

#endif


