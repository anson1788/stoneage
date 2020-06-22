#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "winlua.h"
#include "../systeminc/menu.h"
#include "../systeminc/character.h"
#include <locale.h>
#include <io.h>
#ifdef _AIDENGLU_
#include "../systeminc/pc.h"
extern Landed PcLanded;
#endif
#include "../systeminc/field.h"
#ifdef _WIN_LUAJIT_

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

int SaLuaPrint(lua_State *L)
{
  int arg = 1;
  size_t sfl;
  const char *strfrmt = luaL_checklstring(L, arg, &sfl);
  const char *strfrmt_end = strfrmt+sfl;
  luaL_Buffer b;
  memset(b.buffer,0,LUAL_BUFFERSIZE);
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
          sprintf(buff, form, (int)luaL_checknumber(L, arg));
          break;
        }
        case 'd':  case 'i': {
          addintlen(form);
          sprintf(buff, form, (LUA_INTFRM_T)luaL_checknumber(L, arg));
          break;
        }
        case 'o':  case 'u':  case 'x':  case 'X': {
          addintlen(form);
          sprintf(buff, form, (unsigned LUA_INTFRM_T)luaL_checknumber(L, arg));
          break;
        }
        case 'e':  case 'E': case 'f':
        case 'g': case 'G': {
          sprintf(buff, form, (double)luaL_checknumber(L, arg));
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
            sprintf(buff, form, s);
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
  printf(b.buffer); 
  return 1;
}

static int SaStockFontBuffer(lua_State *L) 
{
	size_t l;
	const int winX = luaL_checkint(L, 1);
	const int winY = luaL_checkint(L, 2);
	const int fontPrio = luaL_checkint(L, 3);
	const int color = luaL_checkint(L, 4);
	const char *data=luaL_checklstring(L, 5, &l);
	const int hitFlag = luaL_checkint(L, 6);
	lua_pushinteger(L, StockFontBuffer( winX, winY, fontPrio, color, (char *)data, hitFlag ));
	return 1;
}

static int SaMakeHitBox(lua_State *L) 
{
	const int x1 = luaL_checkint(L, 1);
	const int y1 = luaL_checkint(L, 2);
	const int x2 = luaL_checkint(L, 3);
	const int y2 = luaL_checkint(L, 4);
	const int dispPrio = luaL_checkint(L, 5);

	lua_pushinteger(L, MakeHitBox( x1, y1 - 3, x2, y2 - 3, dispPrio ));
	return 1;
}

static int SaDeathAction(lua_State *L) 
{
	const int addr = luaL_checkint(L, 1);
	DeathAction((ACTION *)addr);
	return 1;
}

static int SaMakeWindowDisp(lua_State *L) 
{
	const int x = luaL_checkint(L, 1);
	const int y = luaL_checkint(L, 2);
	const int sizeX = luaL_checkint(L, 3);
	const int sizeY = luaL_checkint(L, 4);
	const int titleNo = luaL_checkint(L, 5);
	const int wndType = luaL_checkint(L, 6);
	lua_pushinteger(L, (int)MakeWindowDisp(x,y,sizeX,sizeY,titleNo,wndType));
	return 1;
}


static int SaMakeAnimDisp(lua_State *L) 
{
	const int x = luaL_checkint(L, 1);
	const int y = luaL_checkint(L, 2);
	const int sprNo = luaL_checkint(L, 3);
	const int mode = luaL_checkint(L, 4);
	lua_pushinteger(L, (int)MakeAnimDisp(x,y,sprNo,mode));
	return 1;
}

static int SaStockDispBuffer(lua_State *L) 
{
	const int x = luaL_checkint(L, 1);
	const int y = luaL_checkint(L, 2);
	const int dispPrio = luaL_checkint(L, 3);
	const int bmpNo = luaL_checkint(L, 4);
	const int hitFlag = luaL_checkint(L, 5);
	lua_pushinteger(L, (int)StockDispBuffer(x,y,dispPrio,bmpNo, hitFlag));
	return 1;
}

static int SaStockBoxDispBuffer(lua_State *L) 
{
#define STOCK_DISP_BUFFER_NO_BMP	( 1 << 31 ) 	// ???????
#define STOCK_DISP_BUFFER_LINE		( 1 << 30 ) 	// ???
#define STOCK_DISP_BUFFER_BOX		( 1 << 29 ) 	// ????
#define STOCK_DISP_BUFFER_BOX_FILL	( 1 << 28 )		// ????•D????
#define STOCK_DISP_BUFFER_CIRCLE	( 1 << 27 ) 	// ?
	const int x1 = luaL_checkint(L, 1);
	const int y1 = luaL_checkint(L, 2);
	const int x2 = luaL_checkint(L, 3);
	const int y2 = luaL_checkint(L, 4);
	const int dispPrio = luaL_checkint(L, 5);
	const int color = luaL_checkint(L, 6);
	const int fill = luaL_checkint(L, 7);
	
	int col;
	int x = ( x1 << 16 ) | x2;
	int y = ( y1 << 16 ) | y2;

	if( fill == 0 ) col = color | STOCK_DISP_BUFFER_BOX | STOCK_DISP_BUFFER_NO_BMP ;
	else if( fill == 1 ) col = color | STOCK_DISP_BUFFER_BOX_FILL | STOCK_DISP_BUFFER_NO_BMP ;
	else if( fill == 2 ) col = color | STOCK_DISP_BUFFER_LINE | STOCK_DISP_BUFFER_NO_BMP ;

	StockDispBuffer( x, y, dispPrio, col, 0 );
	return 1;
}

static int SaPlaySe(lua_State *L) 
{
	const int tone = luaL_checkint(L, 1);
	const int x = luaL_checkint(L, 2);
	const int y = luaL_checkint(L, 3);
	extern int play_se(int tone, int x, int y);
	play_se(tone, x, y);
	return 1;
}

static int SaLeftShift(lua_State *L) 
{
	int mask = (int)luaL_checkint(L, 1);
	const int id = luaL_checkint(L, 2);

	lua_pushinteger(L, mask <<= id);
	return 1;
}

static int SaRightShift(lua_State *L) 
{
	int mask = (int)luaL_checkint(L, 1);
	const int id = luaL_checkint(L, 2);

	lua_pushinteger(L, mask >>= id);
	return 1;
}

static int SaD1AndD2(lua_State *L) 
{
	const int d1 = luaL_checkint(L, 1);
	const int d2 = luaL_checkint(L, 2);

	lua_pushinteger(L, d1 & d2);
	return 1;
}
extern SA_WINDOWS windows;
static int SaGetKeyInputFocus(lua_State *L) 
{
	GetKeyInputFocus(&MyChatBuffer);
	return 1;
}

static int getTickCount(lua_State *L) 
{
	lua_pushinteger(L, GetTickCount());
	
	return 1;
}

static int SaShellExecute(lua_State *L) 
{
	size_t l;
	const char *data=luaL_checklstring(L, 1, &l);
	ShellExecute(NULL,"open",data,NULL,NULL,SW_SHOWNORMAL);
	return 1;
}

static int SaShowMessage(lua_State *L) 
{
	size_t l;
	const char *str=luaL_checklstring(L, 1, &l);
	const int color=luaL_checkint(L, 2);
	StockChatBufferLine( (char *)str, color);
	return 1;
}
extern char szUser[], szPassword[];
static int SaGetCdkey(lua_State *L) 
{
	lua_pushstring(L, szUser);
	return 1;
}

static int SAtimeGetTime(lua_State *L) 
{
	lua_pushinteger(L, timeGetTime());
	return 1;
}

extern int getTextLength(char * str);
static int Lua_getTextLength(lua_State *L) 
{
	size_t l;
	const char *str=luaL_checklstring(L, 1, &l);
	int ret = getTextLength((char *)str);
	lua_pushinteger(L,ret);
	return 1;
}

static int Lua_TeaDecryption(lua_State *L) 
{
	char temp[2024];
	memset(temp,0,2024);
	size_t l;
	const char *str=luaL_checklstring(L, 1, &l);
	extern long TEAΩ‚√‹(long* v, long n, long* k);
	extern int getfilesize(FILE* fp);
	FILE *fp=NULL;
	errno_t err = fopen_s(&fp,str,"rb");
	if(err==0){
		int len = getfilesize(fp);
		fread(temp,1,len,fp);
		fclose(fp);
		TEAΩ‚√‹((long*)temp,len/4,(long*)_LUA_KEY_);
	}
	lua_pushstring(L, temp);
	return 1;
}

static int Lua_getStringFromIndexWithDelim_body(lua_State *L)
{
	extern BOOL getStringFromIndexWithDelim_body( char* src ,char* delim ,int index,char* buf , int buflen);
	char temp[2048];
	memset(temp,0,2024);
	size_t l;
	const char *str=luaL_checklstring(L, 1, &l);
	const int index=luaL_checkint(L, 2);
	const char *delim=luaL_checklstring(L, 3, &l);
	getStringFromIndexWithDelim_body((char *)str,(char *)delim,index,temp,2024);
	lua_pushstring(L, temp);
	return 1;
}

static int Lua_strmemcpy(lua_State *L)
{
	char temp[2048];
	memset(temp,0,2024);
	size_t l;
	const char *str=luaL_checklstring(L, 1, &l);
	const int index1=luaL_checkint(L, 2);
	const int index2=luaL_checkint(L, 3);
	memcpy(temp,str+index1-1,index2);
	lua_pushstring(L, temp);
	return 1;
}


static const luaL_Reg salib[] = {

	{"getStringFromIndexWithDelim",		Lua_getStringFromIndexWithDelim_body},
	{"strmemcpy",		Lua_strmemcpy},
	{"TeaDecryption",		Lua_TeaDecryption},
	{"StockFontBuffer",		SaStockFontBuffer},
	{"MakeHitBox",			SaMakeHitBox},
	{"DeathAction",			SaDeathAction},
	{"MakeWindowDisp",		SaMakeWindowDisp},
	{"MakeAnimDisp",		SaMakeAnimDisp},
	{"StockDispBuffer",		SaStockDispBuffer},
	{"StockBoxDispBuffer",	SaStockBoxDispBuffer},
	{"PlaySe",				SaPlaySe},
	{"LeftShift",			SaLeftShift},
	{"RightShift",			SaRightShift},
	{"SaD1AndD2",			SaD1AndD2},
	{"GetKeyInputFocus",	SaGetKeyInputFocus},
	{"getTickCount",		getTickCount},
	{"ShellExecute",		SaShellExecute},
	{"ShowMessage",			SaShowMessage},
	{"GetCdkey",			SaGetCdkey},
	{"print",			SaLuaPrint},
	{"timeGetTime",			SAtimeGetTime},
	{"getTextLength",			Lua_getTextLength},
	{NULL,					NULL}
};

LUALIB_API int luaopen_Sa (lua_State *L) {
	luaL_register(L, "sa", salib);
	return 1;
}

#endif


