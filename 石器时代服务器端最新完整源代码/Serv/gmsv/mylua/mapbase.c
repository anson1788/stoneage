#include <string.h>
#include <stdio.h>
#include "char.h"
#include "object.h"
#include "readmap.h"
#include "map_deal.h"
#include "char_base.h"
#include "mylua/base.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "lssproto_serv.h"

#ifdef _ALLBLUES_LUA   
#ifdef _ALLBLUES_LUA_1_1
static int RandMap(lua_State *L) 
{
	lua_pushinteger(L, MAP_getfloorId(rand()% MAP_getMapNum()));
	return 1;
}

static int RandXAndY(lua_State *L) 
{
	const int ff = luaL_checkint(L, 1);
	int fx,fy;
	int i;
	for(i=0;i<50;i++){
		fx = rand() % MAP_getfloorX(ff);
		fy = rand() % MAP_getfloorY(ff);
#ifdef _ITEM_PATH
		if(MAP_walkAbleFromPoint( ff,fx,fy,0, -1)==TRUE){
			lua_pushinteger(L, (fx << 16)|fy );
			return 1;
		}
#else
		if(MAP_walkAbleFromPoint( ff,fx,fy,0)==TRUE){
			lua_pushinteger(L, (fx << 16)|fy );
			return 1;
		}
#endif
	}
	lua_pushinteger(L, -1);
	return 1;
}

static int getX(lua_State *L) 
{
	const int XandY = luaL_checkint(L, 1);
	lua_pushinteger(L, (XandY >> 16)&0xFFFF );
	return 1;
}

static int getY(lua_State *L) 
{
	const int XandY = luaL_checkint(L, 1);
	lua_pushinteger(L, XandY&0xFFFF );
	return 1;
}
static int getfloorX(lua_State *L) 
{
	const int floorid = luaL_checkint(L, 1);
	lua_pushinteger(L, MAP_getfloorX(floorid) );
	return 1;
}

static int getfloorY(lua_State *L) 
{
	const int floorid = luaL_checkint(L, 1);
	lua_pushinteger(L, MAP_getfloorY(floorid) );
	return 1;
}


static int getFrontTile(lua_State *L)
{
	const int charaindex = luaL_checkint(L, 1);

	int 	dir, ff, fx, fy;
	int		tile, obj;
	int		fixtile, fixobj;
	char  buf[16];

	ff = CHAR_getInt( charaindex, CHAR_FLOOR );
	fx = CHAR_getInt( charaindex, CHAR_X );
	fy = CHAR_getInt( charaindex, CHAR_Y );
	dir = CHAR_getInt( charaindex, CHAR_DIR );

	fx += CHAR_getDX(dir);
	fy += CHAR_getDY(dir);

	if( MAP_getTileAndObjData( ff, fx, fy, &tile, &obj)	== FALSE ){
		lua_pushstring(L, 0);
		return 1;
	}
	lua_pushinteger(L, tile );
		return 1;
}

static int getFloorName(lua_State *L) 
{
	const int floorid = luaL_checkint(L, 1);
	
	char escapeshowstring[256];
	char *showstr = MAP_getfloorShowstring(floorid);

	getStringFromIndexWithDelim( showstr, "|", 1, escapeshowstring, sizeof( escapeshowstring));

	lua_pushstring(L, escapeshowstring);
	return 1;
}

static int getCharaindex(lua_State *L) 
{
	const int charaindex = luaL_checkint(L, 1);
	const int distance = luaL_checkint(L, 2);
	const int type = luaL_checkint(L, 3);
	
	int fl ,x,y;
	OBJECT  object;
	fl = CHAR_getInt(charaindex,CHAR_FLOOR);
	int i;
	for(i=1; i <=distance; i ++)
	{
		CHAR_getCoordinationDir( CHAR_getInt(charaindex,CHAR_DIR),
								 CHAR_getInt(charaindex,CHAR_X),
								 CHAR_getInt(charaindex,CHAR_Y),
								 i, &x, &y );

		for( object=MAP_getTopObj(fl,x,y) ; object ;
			 object = NEXT_OBJECT(object) ){
			int objindex = GET_OBJINDEX(object);
//			printf("\ntype=%d objindex=%d chartype=%d",OBJECT_getType(objindex),objindex,CHAR_getInt( OBJECT_getIndex(objindex), CHAR_WHICHTYPE));
			if( OBJECT_getType(objindex) == OBJTYPE_CHARA ){
				if( CHAR_getInt( OBJECT_getIndex(objindex), CHAR_WHICHTYPE) == type){
					lua_pushinteger(L, OBJECT_getIndex(objindex));
					return 1;
				}
			}
		}
	}
	lua_pushinteger(L, -1);
	return 1;
}

static int getCharaNearBy(lua_State *L) {
	const int charaindex = luaL_checkint(L, 1);
	const int area = luaL_checkint(L, 2);
	const int type = luaL_checkint(L, 3);


	lua_newtable(L); //新建一个表，并压入栈顶
	lua_pushnumber(L,-1); //这行和下面一行，没搞懂为什么需要这样，但如果不执行，lua中获取的数据就会出错
	lua_rawseti(L,-2,0);

	int o, chara_index;
	int getnum = 0;
	int fl, x, y;
	OBJECT object;
	int i,j;
	fl = CHAR_getInt(charaindex, CHAR_FLOOR);
	x = CHAR_getInt(charaindex, CHAR_X);
	y = CHAR_getInt(charaindex, CHAR_Y);
	for (i = y - area; i <= y + area; i++) {
		for (j = x - area; j <= x + area; j++) {
			for (object = MAP_getTopObj(fl, j, i); object; object =
					NEXT_OBJECT(object)) {
				o = GET_OBJINDEX(object);
				if (OBJECT_getType(o) == OBJTYPE_CHARA) {
					chara_index = OBJECT_getIndex(o);
					if (CHAR_CHECKINDEX(chara_index) == FALSE)
						continue;
					if ( CHAR_getInt(chara_index, CHAR_WHICHTYPE)
							!= type)
						continue;
					if (chara_index == charaindex)
						continue;  //找到自己了

					lua_pushinteger(L,chara_index);
					getnum++;
					lua_rawseti(L,-2,getnum);
				}
			}
		}
	}
	return 1;
}

static int getCharaAhead(lua_State *L) {
	const int charaindex = luaL_checkint(L, 1);
	const int distance = luaL_checkint(L, 2);
	const int area = luaL_checkint(L, 3);
	const int type = luaL_checkint(L, 4);


	lua_newtable(L); //新建一个表，并压入栈顶
	lua_pushnumber(L,-1); //这行和下面一行，没搞懂为什么需要这样，但如果不执行，lua中获取的数据就会出错
	lua_rawseti(L,-2,0);

	int o, chara_index;
	int getnum = 0;
	int fl, x, y;
	OBJECT object;
	int i,j;
	fl = CHAR_getInt(charaindex, CHAR_FLOOR);

	CHAR_getCoordinationDir( CHAR_getInt(charaindex,CHAR_DIR),
							 CHAR_getInt(charaindex,CHAR_X),
							 CHAR_getInt(charaindex,CHAR_Y),
							 distance, &x, &y );


	for (i = y - area; i <= y + area; i++) {
		for (j = x - area; j <= x + area; j++) {
			for (object = MAP_getTopObj(fl, j, i); object; object =
					NEXT_OBJECT(object)) {
				o = GET_OBJINDEX(object);
				if (OBJECT_getType(o) == OBJTYPE_CHARA) {
					chara_index = OBJECT_getIndex(o);
					if (CHAR_CHECKINDEX(chara_index) == FALSE)
						continue;
					if ( CHAR_getInt(chara_index, CHAR_WHICHTYPE)
							!= type)
						continue;
					if (chara_index == charaindex)
						continue;  //找到自己了

					lua_pushinteger(L,chara_index);
					getnum++;
					lua_rawseti(L,-2,getnum);
				}
			}
		}
	}
	return 1;
}

static int getCharaByCoord(lua_State *L) {
	const int charaindex = luaL_checkint(L, 1);
	const int fl = luaL_checkint(L, 2);
	const int x = luaL_checkint(L, 3);
	const int y = luaL_checkint(L, 4);
	const int area = luaL_checkint(L, 5);
	const int type = luaL_checkint(L, 6);

	lua_newtable(L); //新建一个表，并压入栈顶
	lua_pushnumber(L,-1); //这行和下面一行，没搞懂为什么需要这样，但如果不执行，lua中获取的数据就会出错
	lua_rawseti(L,-2,0);

	int o, chara_index;
	int getnum = 0;
	OBJECT object;
	int i,j;
	for (i = y - area; i <= y + area; i++) {
		for (j = x - area; j <= x + area; j++) {
			for (object = MAP_getTopObj(fl, j, i); object; object =
					NEXT_OBJECT(object)) {
				o = GET_OBJINDEX(object);
				if (OBJECT_getType(o) == OBJTYPE_CHARA) {
					chara_index = OBJECT_getIndex(o);
					if (CHAR_CHECKINDEX(chara_index) == FALSE)
						continue;
					if ( CHAR_getInt(chara_index, CHAR_WHICHTYPE)
							!= type)
						continue;
					if (chara_index == charaindex)
						continue;  //找到自己了

					lua_pushinteger(L,chara_index);
					getnum++;
					lua_rawseti(L,-2,getnum);
				}
			}
		}
	}
	return 1;
}



static const luaL_Reg maplib[] = {
	{"RandMap", 			RandMap},
	{"RandXAndY", 		RandXAndY},
	{"getfloorX", 		getfloorX},
	{"getfloorY", 		getfloorY},
	{"getFloorName", 	getFloorName},
	{"getX", 					getX},
	{"getY", 					getY},
	{"getCharaindex", getCharaindex},
	{"getCharaNearBy", getCharaNearBy},
	{"getCharaAhead", getCharaAhead},
	{"getCharaByCoord", getCharaByCoord},
	{"getFrontTile",getFrontTile},
  {NULL, 						NULL}
};

LUALIB_API int luaopen_Map (lua_State *L) {
  luaL_register(L, "map", maplib);
  return 1;
}
#endif

#endif

