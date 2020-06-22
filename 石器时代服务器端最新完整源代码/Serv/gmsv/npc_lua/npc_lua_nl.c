#include "npc_lua.h"
#include "util.h"
#include "char.h"
#include "char_base.h"
#include "anim_tbl.h"
#include "object.h"
#include "net.h"
#include "npcutil.h"
#include "npc_eventaction.h"
#include "battle.h"
#include "readmap.h"
#include <stdio.h>
#include <string.h>
#include "autil.h"
#include "configfile.h"
#include "enemy.h"

#ifdef _JZ_NEWSCRIPT_LUA

extern char M_OutErrMsg[1024];
extern lua_State *M_Script_Lua;
extern unsigned int M_Create_Num;
static char Buff_Data[1024*64];
static int Check_Num=0;


//////////////////////////////////////////////////////////////////////////////
int NPC_Lua_NL_GetErrorStr(lua_State *_NLL)
{
	CheckEx(_NLL, 0);

	LRetMsg(_NLL, M_OutErrMsg);
}

BOOL NPC_Lua_NL_Mod(lua_State *_NLL)
{
	CheckEx2(_NLL, 1, 2);
	int num = (int)lua_tointeger(_NLL, 1);
	int num2 = 2;
	int TM_Top = lua_gettop(_NLL);
	if (TM_Top == 2)
	{
		int num2 = (int)lua_tointeger(_NLL, 2);
	}
	if(num%num2==0)
		{
			LRetBool(_NLL,TRUE);
		}
	else
		{
			LRetBool(_NLL,FALSE);
		}
}

BOOL NPC_Lua_CheckNpcEventFree(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	char *TM_NpcStr = lua_tostring(_NLL, 3);
	int TM_NPCindex = (int)lua_tointeger(_NLL, 1);
	int TM_index = (int)lua_tointeger(_NLL, 2);
	char magicname[256];
	int i = 1;
	BOOL ret;
	BOOL ret1;
	while (TRUE) {
		 ret = getStringFromIndexWithDelim( TM_NpcStr, "|", i, magicname,  sizeof( magicname));
		 if (!ret) 
		 {
		 	ret = getStringFromIndexWithDelim( TM_NpcStr, "/", i, magicname,  sizeof( magicname));
		 	if (!ret) 
		 	{
		 		ret = getStringFromIndexWithDelim( TM_NpcStr, "\\", i, magicname,  sizeof( magicname));
		 		if (!ret)
		 		{
		 			break;
		 		}
		 	}
		 }
		 ret1 = ActionNpc_CheckFree(TM_NPCindex,TM_index,magicname);
		 if (!ret1) 
		 {
		 		break;
		 }
		 i +=1;
	}
	LRetBool(_NLL, ret1);
}

BOOL NPC_Lua_DoNpcEventAction(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	char *TM_NpcStr = lua_tostring(_NLL, 3);
	int TM_NPCindex = (int)lua_tointeger(_NLL, 1);
	int TM_index = (int)lua_tointeger(_NLL, 2);
	char magicname[256];
	int i = 1;
	BOOL ret;
	BOOL ret1;
	while (TRUE) {
		 ret = getStringFromIndexWithDelim( TM_NpcStr, "|", i, magicname,  sizeof( magicname));
		 if (!ret) 
		 {
		 	ret = getStringFromIndexWithDelim( TM_NpcStr, "/", i, magicname,  sizeof( magicname));
		 	if (!ret) 
		 	{
		 		ret = getStringFromIndexWithDelim( TM_NpcStr, "\\", i, magicname,  sizeof( magicname));
		 		if (!ret)
		 		{
		 			break;
		 		}
		 	}
		 }
		 ret1 = Action_RunDoEventAction(TM_NPCindex,TM_index,magicname);
		 i +=1;
	}
	LRetBool(_NLL, ret1);
}


int NPC_Lua_NL_CreateNpc(lua_State *_NLL)
{
	//参数有4个
	CheckEx2(_NLL, 2, 4);
	char *TM_DoFile = lua_tostring(_NLL, 1);
	char *TM_InitFuncName = lua_tostring(_NLL, 2);
	BOOL TM_IsFly = FALSE;
	char *TM_seek = NULL;
	int TM_Top = lua_gettop(_NLL);

	if(TM_Top >= 3)
	{
		TM_IsFly = (BOOL)lua_toboolean(_NLL, 3);
	}
	if(TM_Top == 4)
	{
		TM_seek = lua_tostring(_NLL, 4);
	}

	int TM_Ret = NPC_Lua_Create(TM_DoFile, TM_InitFuncName, TM_seek, TM_IsFly);

	LRet(TM_Ret);
}


int NPC_Lua_NL_DelNpc(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int TM_index = (int)lua_tointeger(_NLL, 1);
	int TM_Ret = NPC_Lua_Del(TM_index);
	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_NL_GetStringFromIndexWithDelim(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 1);
	CheckIndexNull(_NLL, 2);
	char* string = (char*)lua_tostring(_NLL, 1);
	char* delim = (char*)lua_tostring(_NLL, 2);
	int index = (int)lua_tointeger(_NLL, 3);
	char token[128];
	getStringFromIndexWithDelim( string,delim,index,token,sizeof(token));
	LRetMsg(_NLL,token);
}

BOOL NPC_Lua_NL_ANSI_PlayerLoop(lua_State *_NLL)
{
	CheckEx2(_NLL,0, 1);
	int i;
	static int indexn = 0;
	int TM_Top = lua_gettop(_NLL);
	BOOL TM_IsFly = FALSE;
	if(TM_Top >= 1)
	{
		TM_IsFly = (BOOL)lua_toboolean(_NLL, 1);	
	}
	if (TM_IsFly){
		indexn = 0;
		LRetBool(_NLL, TRUE);
		return;
	}
	int playernum = CHAR_getPlayerMaxNum();
	for( i = indexn ; i < playernum ; i++) {
		if( CHAR_CHECKINDEX(i) ){
			indexn++;
			LRetInt(_NLL, i);
			break;
		}
	}
	LRetInt(_NLL, -1);
}

BOOL NPC_Lua_NL_ANSI_PetLoop(lua_State *_NLL)
{
	CheckEx2(_NLL,0, 1);
	int i;
	int MAXPLAYER = CHAR_getPlayerMaxNum();
	int MAXPET = CHAR_getPetMaxNum();
	static int indexn = 0;
	int TM_Top = lua_gettop(_NLL);
	BOOL TM_IsFly = FALSE;
	if(TM_Top >= 1)
	{
		TM_IsFly = (BOOL)lua_toboolean(_NLL, 1);	
	}
	if (TM_IsFly){
		indexn = MAXPLAYER;
		LRetBool(_NLL, TRUE);
		return;
	}

	for( i = indexn ; i < MAXPLAYER+MAXPET ; i++) {
		if( CHAR_CHECKINDEX(i) ){
			indexn++;
			LRetInt(_NLL, i);
			break;
		}
	}
	LRetInt(_NLL, -1);
}



BOOL NPC_Lua_NL_ANSI_ItemLoop(lua_State *_NLL)
{
	CheckEx2(_NLL,0, 1);
	int i;
	static int indexn = 0;
	int TM_Top = lua_gettop(_NLL);
	BOOL TM_IsFly = FALSE;
	if(TM_Top >= 1)
	{
		TM_IsFly = (BOOL)lua_toboolean(_NLL, 1);	
	}
	if (TM_IsFly){
		indexn = 0;
		LRetBool(_NLL, TRUE);
		return;
	}
	int itemnum = ITEM_getITEM_itemnum();
	for( i = indexn ; i < itemnum ; i++) {
		if( ITEM_CHECKINDEX(i) ){
			indexn++;
			LRetInt(_NLL, i);
			break;
		}
	}
	LRetInt(_NLL, -1);
}

BOOL NPC_Lua_NL_PetLoopGetNext(lua_State *_NLL)
{
	int MAXPLAYER = CHAR_getPlayerMaxNum();
	int MAXPET = CHAR_getPetMaxNum();

	CheckEx(_NLL, 1);
	int i;
	char *TM_FuncName = lua_tostring(_NLL, 1);
	char TM_RetBuff[128];

	for( i = MAXPLAYER ; i < MAXPLAYER+MAXPET ; i++) {
		if( CHAR_CHECKINDEX(i) ){
			NPC_Lua_CallFunc(TM_FuncName, &TM_RetBuff, sizeof(TM_RetBuff), i);
		}
	}
	LRetBool(_NLL, TRUE);

}

int NPC_Lua_NL_ItemLoopGetNext(lua_State *_NLL)
{
	int itemnum = ITEM_getITEM_itemnum();
	CheckEx(_NLL, 1);
	int i;
	char *TM_FuncName = lua_tostring(_NLL, 1);
	char TM_RetBuff[128];

	for( i = 0 ; i < itemnum ; i++) {
		if( ITEM_CHECKINDEX(i) ){
			NPC_Lua_CallFunc(TM_FuncName, &TM_RetBuff, sizeof(TM_RetBuff), i);
		}
	}
	LRetBool(_NLL, TRUE);

}

BOOL NPC_Lua_NL_PlayerLoopGetNext(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	int i;
	char *TM_FuncName = lua_tostring(_NLL, 1);
	char TM_RetBuff[128];
	int playernum = CHAR_getPlayerMaxNum();
	for( i = 0 ; i < playernum ; i++) {
		if( CHAR_CHECKINDEX(i) ){
			NPC_Lua_CallFunc(TM_FuncName, &TM_RetBuff, sizeof(TM_RetBuff), i);
		}
	}
	LRetBool(_NLL, TRUE);
}

int NPC_Lua_NL_GetConfigLineType(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	int i;
	char *TM_ConfigName = lua_tostring(_NLL, 1);
	int TM_Type = GetConfigLineType(TM_ConfigName);
	LRetInt(_NLL, TM_Type);
}

int NPC_Lua_NL_GetConfigLineVal(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	int i;
	char *TM_ConfigName = lua_tostring(_NLL, 1);
	char *TM_RETMSG = GetConfigLineVal(TM_ConfigName);
	LRetMsg(_NLL, TM_RETMSG);
}

int NPC_Lua_NL_SetTimer(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	int i;
	char *TM_FileName = lua_tostring(_NLL, 1);
	char *TM_FuncName = lua_tostring(_NLL, 2);
	unsigned int EspTime = (int)lua_tointeger(_NLL, 3);
	int Timer_s = SetTimer_net(TM_FileName,TM_FuncName,EspTime);
	LRetInt(_NLL, Timer_s);
}

int NPC_Lua_NL_DelTimer(lua_State *_NLL)
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int ID = (int)lua_tointeger(_NLL, 1);
	BOOL TM_Ret = DelTimer_net(ID);
	LRetBool(_NLL, TM_Ret);
}

int NPC_Lua_NL_RunSaFuncII(lua_State *_NLL)
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 1);
	char *TM_FuncName = lua_tostring(_NLL, 1);
	int i1 = (int)lua_tointeger(_NLL, 2);
	int i2 = (int)lua_tointeger(_NLL, 3);
	typedef void (*GMSVFUNC)(int,int);
	GMSVFUNC atf;
	atf=(GMSVFUNC)(void*)TM_FuncName;
	if( atf )
		atf( i1,i2 );
}

int NPC_Lua_NL_RunSaFuncIII(lua_State *_NLL)
{
	CheckEx(_NLL, 4);
	CheckIndexNull(_NLL, 1);
	char *TM_FuncName = lua_tostring(_NLL, 1);
	int i1 = (int)lua_tointeger(_NLL, 2);
	int i2 = (int)lua_tointeger(_NLL, 3);
	int i3 = (int)lua_tointeger(_NLL, 3);
	typedef void (*GMSVFUNC)(int,int,int);
	GMSVFUNC atf;
	atf=(GMSVFUNC)(void*) TM_FuncName;
	if( atf )
		atf( i1,i2,i3);
}

int NPC_Lua_NL_ClsMk(lua_State *_NLL)
{
	Check_Num=0;
	memset(Buff_Data,0,sizeof(Buff_Data));
	LRetInt(_NLL, 1);
}

int NPC_Lua_NL_AddCheckNum(lua_State *_NLL)
{

	CheckEx(_NLL,1);
	CheckIndexNull(_NLL, 1);
	int i = (int)lua_tointeger(_NLL, 1);
	Check_Num += i;
	LRetInt(_NLL, Check_Num);
}


int NPC_Lua_NL_Mkstring(lua_State *_NLL)
{

	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int TM_index = (int)lua_tointeger(_NLL, 1);
	int fd = getfdFromCharaIndex(TM_index );
	char *TM_data = lua_tostring(_NLL, 2);
	int checknum=0;
	checknum = util_mkstring(Buff_Data, TM_data);
	LRetInt(_NLL, checknum);
}

int NPC_Lua_NL_Mkint(lua_State *_NLL)
{

	CheckEx(_NLL,2);
	CheckIndexNull(_NLL, 1);
	int TM_index = (int)lua_tointeger(_NLL, 1);
	int fd = getfdFromCharaIndex(TM_index );
	int i = (int)lua_tointeger(_NLL, 2);
	int checknum=util_mkint(Buff_Data, i);
	LRetInt(_NLL, checknum);
}

int NPC_Lua_NL_SendMesg(lua_State *_NLL)
{

	CheckEx(_NLL,2);
	CheckIndexNull(_NLL, 1);
	int charaindex = (int)lua_tointeger(_NLL, 1);
	int fengbaohao = (int)lua_tointeger(_NLL, 2);
	int fd = getfdFromCharaIndex(charaindex );
	util_SendMesg(fd, fengbaohao, Buff_Data);
	LRetInt(_NLL, 1);

}
int NPC_Lua_NL_SetCharSignDay(lua_State *_NLL)
{

	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int a = (int)lua_tointeger(_NLL, 1);
	int b = (int)lua_tointeger(_NLL, 2);
	a|=(1<<b);
	LRetInt(_NLL, a);
	return 0;
}


int NPC_Lua_NL_GetCharSignDay(lua_State *_NLL)
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int a = (int)lua_tointeger(_NLL, 1);
	int b = (int)lua_tointeger(_NLL, 2);
	a&=(1<<b);
	LRetInt(_NLL, a);
	return 0;
}

int NPC_Lua_NL_WriteShopData(lua_State *_NLL)
{

	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int shopID = (int)lua_tointeger(_NLL, 1);
	char *shopData = lua_tostring(_NLL, 2);
	WriteShopData(shopData,shopID);
	return 0;
}
#define L_ESC		'%'
#define FLAGS	"-+ #0"
#define MAX_FORMAT	(sizeof(FLAGS) + sizeof(LUA_INTFRMLEN) + 10)
#define MAX_ITEM	512
#define uchar(c)        ((unsigned char)(c))
static const char *scanformat (lua_State *L, const char *strfrmt, char *form) {
  const char *p = strfrmt;
  while (*p != '\0' && strchr(FLAGS, *p) != NULL) p++;
  if ((size_t)(p - strfrmt) >= sizeof(FLAGS))
    luaL_error(L, "invalid format (repeated flags)");
  if (isdigit(uchar(*p))) p++;
  if (isdigit(uchar(*p))) p++;
  if (*p == '.') {
    p++;
    if (isdigit(uchar(*p))) p++;
    if (isdigit(uchar(*p))) p++;
  }
  if (isdigit(uchar(*p)))
    luaL_error(L, "invalid format (width or precision too long)");
  *(form++) = '%';
  strncpy(form, strfrmt, p - strfrmt + 1);
  form += p - strfrmt + 1;
  *form = '\0';
  return p;
}


static void addintlen (char *form) {
  size_t l = strlen(form);
  char spec = form[l - 1];
  strcpy(form + l - 1, LUA_INTFRMLEN);
  form[l + sizeof(LUA_INTFRMLEN) - 2] = spec;
  form[l + sizeof(LUA_INTFRMLEN) - 1] = '\0';
}

static void addquoted (lua_State *L, luaL_Buffer *b, int arg) {
  size_t l;
  const char *s = luaL_checklstring(L, arg, &l);
  luaL_addchar(b, '"');
  while (l--) {
    switch (*s) {
      case '"': case '\\': case '\n': {
        luaL_addchar(b, '\\');
        luaL_addchar(b, *s);
        break;
      }
      case '\r': {
        luaL_addlstring(b, "\\r", 2);
        break;
      }
      case '\0': {
        luaL_addlstring(b, "\\000", 4);
        break;
      }
      default: {
        luaL_addchar(b, *s);
        break;
      }
    }
    s++;
  }
  luaL_addchar(b, '"');
}
int NPC_Lua_NL_Print(lua_State *L)
{
  int arg = 1;
  size_t sfl;
  const char *strfrmt = luaL_checklstring(L, arg, &sfl);
  const char *strfrmt_end = strfrmt+sfl;
  luaL_Buffer b;
  luaL_buffinit(L, &b);
  while (strfrmt < strfrmt_end) {
    if (*strfrmt != L_ESC)
      luaL_addchar(&b, *strfrmt++);
    else if (*++strfrmt == L_ESC)
      luaL_addchar(&b, *strfrmt++);
    else {
      char form[MAX_FORMAT];
      char buff[MAX_ITEM];
	  memset(form,0,MAX_FORMAT);
	  memset(buff,0,MAX_ITEM);
      arg++;
      strfrmt = scanformat(L, strfrmt, form);
      switch (*strfrmt++) {
        case 'c': {
          mj_sprintf(buff, form, (int)luaL_checknumber(L, arg));
          break;
        }
        case 'd':  case 'i': {
          addintlen(form);
          mj_sprintf(buff, form, (LUA_INTFRM_T)luaL_checknumber(L, arg));
          break;
        }
        case 'o':  case 'u':  case 'x':  case 'X': {
          addintlen(form);
          mj_sprintf(buff, form, (unsigned LUA_INTFRM_T)luaL_checknumber(L, arg));
          break;
        }
        case 'e':  case 'E': case 'f':
        case 'g': case 'G': {
          mj_sprintf(buff, form, (double)luaL_checknumber(L, arg));
          break;
        }
        case 'q': {
          addquoted(L, &b, arg);
          continue;
        }
        case 's': {
          size_t l;
          const char *s = luaL_checklstring(L, arg, &l);
          if (!strchr(form, '.') && l >= 100) {

            lua_pushvalue(L, arg);
            luaL_addvalue(&b);
            continue;
          }
          else {
            mj_sprintf(buff, form, s);
            break;
          }
        }
        default: {
          return luaL_error(L, "invalid option " LUA_QL("%%%c") " to "
                               LUA_QL("format"), *(strfrmt - 1));
        }
      }
      luaL_addlstring(&b, buff, strlen(buff));
    }
  }
  print(b.buffer);
  return 1;
}
int NPC_Lua_NL_CreateSpecialNpc(lua_State *L)
{
	Char one;
	size_t l;
	CHAR_getDefaultChar( &one, 0);
	one.data[CHAR_WHICHTYPE] = CHAR_TYPELUANPC;
	char *Name = luaL_checklstring(L, 1, &l);
	const int Image = luaL_checkinteger(L, 2);
	const int Floor = luaL_checkinteger(L, 3);
	const int x = luaL_checkinteger(L, 4);
	const int y = luaL_checkinteger(L, 5);
	const int dir = luaL_checkinteger(L, 6);
	const int enemyid = luaL_checkint(L, 7);
	const int UpLevel = luaL_checkinteger(L, 8);
	int i;
	for( i = 0; i < ENEMY_getEnemyNum(); i ++ ) {
		if( ENEMY_getInt( i, ENEMY_ID) == enemyid) {
			break;
		}
	}
	if( i == ENEMY_getEnemyNum() ){
		lua_pushinteger(L, -1);
		return 1;
	}

	int npcindex = ENEMY_createEnemy( i, 0);
	if( npcindex < 0 ) {
		print( "NPC制作失败。\n");
	}
	CHAR_setInt(npcindex, CHAR_WHICHTYPE, CHAR_TYPEPLAYERNPC);
	CHAR_setChar(npcindex, CHAR_NAME, Name);
	CHAR_setInt(npcindex, CHAR_BASEBASEIMAGENUMBER, Image);
	CHAR_setInt(npcindex, CHAR_BASEIMAGENUMBER, Image);
	CHAR_setInt(npcindex, CHAR_FLOOR, Floor);
	CHAR_setInt(npcindex, CHAR_X, x);
	CHAR_setInt(npcindex, CHAR_Y, y);
	CHAR_setInt(npcindex, CHAR_DIR, dir);

	CHAR_setInt(npcindex, CHAR_MODAI, 10000);
	CHAR_setInt(npcindex, CHAR_VARIABLEAI,10000);

	for( i = 0; i < UpLevel; i ++ ){
		CHAR_PetLevelUp( npcindex );
		CHAR_PetAddVariableAi( npcindex, AI_FIX_PETLEVELUP );
	}
	CHAR_complianceParameter( npcindex );
	CHAR_setInt( npcindex, CHAR_HP, CHAR_getWorkInt( npcindex, CHAR_WORKMAXHP ));
	Object object;
	int objindex;

	object.type = OBJTYPE_CHARA;
	object.index = npcindex;
	object.x =CHAR_getInt(npcindex, CHAR_X);
	object.y = CHAR_getInt(npcindex, CHAR_Y);
	object.floor = CHAR_getInt(npcindex, CHAR_FLOOR);

	objindex = initObjectOne( &object );

	if( objindex == -1 ) {
		CHAR_endCharOneArray( npcindex );
	}else {
		CHAR_setWorkInt( npcindex,CHAR_WORKOBJINDEX, objindex );
		CHAR_LoginBesideSetWorkInt( npcindex, -1 );
	}

	lua_pushinteger(L, npcindex);
	return 1;
}
int NPC_Lua_NL_getGameservername(lua_State *_NLL)
{
	char* name = getGameservername();
	LRetMsg(_NLL, name);
}

#endif //#ifdef _JZ_NEWSCRIPT_LUA

