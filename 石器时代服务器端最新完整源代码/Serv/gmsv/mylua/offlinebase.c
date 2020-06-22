#include <string.h>
#include "longzoro/version.h"
#include "char_base.h"
#include "enemy.h"
#include "object.h"
#include "char.h"
#include "battle.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "mylua/mylua.h"

#ifdef _OFFLINE_SYSTEM   
 int Attack(lua_State *L)
{
	const int battleindex = luaL_checkint(L, 1);
	const int charaindex = luaL_checkint(L, 2);
	const int side = 1 - luaL_checkint(L, 3);

	

	BATTLE_ENTRY *pEntry = BattleArray[battleindex].Side[side].Entry;
	int iNum[10] = {9,7,5,6,8,4,2,0,1,3};
	int i;
	for( i = 0; i < 10; i ++ ){
		if( pEntry[iNum[i]].charaindex != -1 ){
			break;
		}
	}

	CHAR_setWorkInt( charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_ATTACK );
	CHAR_setWorkInt( charaindex, CHAR_WORKBATTLECOM2, iNum[i] + side * BATTLE_ENTRY_MAX);
	int At_SoubiIndex =  CHAR_getItemIndex( charaindex, CHAR_ARM );
	if( ITEM_CHECKINDEX( At_SoubiIndex ) == TRUE )
	{
		if( ITEM_getInt( At_SoubiIndex, ITEM_TYPE ) == ITEM_BOOMERANG ){
			CHAR_setWorkInt( charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_BOOMERANG );
#ifdef _PETSKILL_BECOMEFOX
			if( CHAR_getInt( charaindex, CHAR_BASEIMAGENUMBER)==101749
				|| CHAR_getWorkInt( charaindex, CHAR_WORKFOXROUND ) != -1 ){ //若是变成小狐狸不可以使用回力标
			    CHAR_setWorkInt( charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_NONE );
			}
#endif
#ifdef _PETSKILL_BECOMEPIG
			if( CHAR_getInt( charaindex, CHAR_BECOMEPIG) > -1 ){//变成乌力了
			    CHAR_setWorkInt( charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_ATTACK );
			}
#endif
		}
	}
	CHAR_SETWORKINT_LOW( charaindex, CHAR_WORKBATTLECOM3, 1 );
	CHAR_setWorkInt( charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK );
	
	return 1;
}

 int Recovery(lua_State *L)
{
	const int battleindex = luaL_checkint(L, 1);
	const int charaindex = luaL_checkint(L, 2);
	const int HP = luaL_checkint(L, 3);


	int toNO = BATTLE_Index2No(battleindex, charaindex);
	CHAR_setWorkInt( charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_S_OFFLINE_RECOVERY );
	CHAR_setWorkInt( charaindex, CHAR_WORKBATTLECOM2, toNO);
	CHAR_SETWORKINT_LOW( charaindex, CHAR_WORKBATTLECOM3, HP );
	
	CHAR_setWorkInt( charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK );
	
	return 1;
}



static const luaL_Reg offlinelib[] = {
  {"Attack", 			Attack},
  {"Recovery", 		Recovery},
  {NULL, NULL}
};


LUALIB_API int luaopen_Offline (lua_State *L) {
  luaL_register(L, "offline", offlinelib);
  return 1;
}

#endif

