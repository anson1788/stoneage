#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "winlua.h"

#include <locale.h>
#include <io.h>
#ifdef _WIN_LUAJIT_
void lssproto_WN_send(int fd,int x,int y,int seqno,int objindex,int select,char* data);


extern unsigned int sockfd;
extern int nowGx, nowGy;
static int windows_send(lua_State *L) 
{
	size_t l;
	const int indexWN = luaL_checkint(L, 1);
	const int idWN = luaL_checkint(L, 2);
	const int btn = luaL_checkint(L, 3);
	const char *data=luaL_checklstring(L, 4, &l);
	lssproto_WN_send(sockfd,nowGx,nowGy,indexWN, idWN, btn, (char *)data) ;
	return 1;
}


static const luaL_Reg Lssprotolib[] = {
	{"windows",			windows_send},

	{NULL,					NULL}
};

LUALIB_API int luaopen_Lssproto (lua_State *L) {
	luaL_register(L, "lssproto", Lssprotolib);
	return 1;
}

#endif


