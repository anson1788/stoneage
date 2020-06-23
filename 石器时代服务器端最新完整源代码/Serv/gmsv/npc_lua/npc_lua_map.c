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
#include "item.h"
#include "npc_lua.h"
#define _JZ_NEWSCRIPT_LUA

#ifdef _ALLBLUE_LUAscript
/*
char *MAP_getFloorName( int floor)
*/
int NPC_ABLua_map_getFloorName(lua_State *_NLL)
{
		CheckEx(_NLL, 1);
		CheckIndexNull(_NLL, 1);
		
		int Floorid = (int)lua_tointeger(_NLL, 1);
	
		LRetMsg(_NLL , MAP_getFloorName(Floorid));
		
}
int NPC_ABLua_map_getfloorX(lua_State *_NLL)
{
		CheckEx(_NLL, 1);
		CheckIndexNull(_NLL, 1);
		
		int Floorid = (int)lua_tointeger(_NLL, 1);
	
		LRetMsg(_NLL , MAP_getfloorX(Floorid));
		
}
int NPC_ABLua_map_getfloorY(lua_State *_NLL)
{
		CheckEx(_NLL, 1);
		CheckIndexNull(_NLL, 1);
		
		int Floorid = (int)lua_tointeger(_NLL, 1);
	
		LRetMsg(_NLL , MAP_getfloorY(Floorid));
		
}


#endif


int NPC_Lua_Map_CheckCoordinates(lua_State *_NLL)           //检测某点是否在地图范围内。
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 1);
	int map = (int)lua_tointeger(_NLL, 1);
	int x = (int)lua_tointeger(_NLL, 2);
	int y = (int)lua_tointeger(_NLL, 3);
	BOOL RETBOOL = MAP_checkCoordinates(map,x,y);
	LRetBool(_NLL, RETBOOL);
}


int NPC_Lua_Map_GetExitFloorXY(lua_State *_NLL)           //检测地图是否支持登出
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int map = (int)lua_tointeger(_NLL, 1);
	int maparray[3];
	int map_type=0;
	unsigned int point;
	point = MAP_getExFloor_XY( map , &map_type);
	if( point > 0 )	{
		if( map_type >= 0 )	{	// map_type >= 0 优先回纪录点
			if( map == map_type && map >= 0 )	{//回纪录点
			}else	{
				maparray[0] = (point>>16)&0xffffff;
				maparray[1] = (point>>8)&0xff;
				maparray[2] = (point>>0)&0xff;
			}
		}
	}
	else if( map == 10032 ){
		maparray[0] = 10030;
		maparray[1] = 52;
		maparray[2] = 36;
	}
#ifdef _MAP_TIME
	else if (map >= 30017 && map >= 30021) 
	{
		maparray[0]= 30008;
		maparray[1] = 39;
		maparray[2] = 39;
	}
#endif
	else {
		maparray[0] = map;
		maparray[1] = 0;
		maparray[2] = 0;
	}
	LRetArray(_NLL,maparray,arraysizeof(maparray));
}

int NPC_Lua_Map_GetfloorX(lua_State *_NLL)           //获取地图X长度
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int map = (int)lua_tointeger(_NLL, 1);
	int x;
	x = MAP_getfloorX(map);
	LRetInt(_NLL, x);
}

int NPC_Lua_Map_GetfloorY(lua_State *_NLL)           //获取地图Y长度
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int map = (int)lua_tointeger(_NLL, 1);
	int y;
	y = MAP_getfloorX(map);
	LRetInt(_NLL, y);
}

int NPC_Lua_Map_GetTileAndObjId(lua_State *_NLL)          //获取地图某点的地板和装饰层
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 1);
	int map = (int)lua_tointeger(_NLL, 1);
	int x = (int)lua_tointeger(_NLL, 2);
	int y = (int)lua_tointeger(_NLL, 3);
	int *tile = -1, *obj = -1;
	MAP_getTileAndObjData(map,x,y,&tile,&obj);
	int maparray[2];
	maparray[0]=tile;
	maparray[1]=obj;
	LRetArray(_NLL,maparray,arraysizeof(maparray));
}

int NPC_Lua_Map_SetTileAndObjId(lua_State *_NLL)           //设置地图某点的地板和装饰层
{
	CheckEx(_NLL, 5);
	CheckIndexNull(_NLL, 1);
	int map = (int)lua_tointeger(_NLL, 1);
	int x = (int)lua_tointeger(_NLL, 2);
	int y = (int)lua_tointeger(_NLL, 3);
	int tile = (int)lua_tointeger(_NLL, 4);
	int obj = (int)lua_tointeger(_NLL, 5);
	if (MAP_setTileAndObjData(map,x,y,tile,obj)) 	LRetInt(_NLL, 0);
	LRetInt(_NLL, -1);
}

int NPC_Lua_Map_GetWalkAbleFromPoint(lua_State *_NLL)           //获取某点是否可以行走
{
	CheckEx2(_NLL, 3,4);
	CheckIndexNull(_NLL, 1);
	int TM_Top = lua_gettop(_NLL);
	int map = (int)lua_tointeger(_NLL, 1);
	int x = (int)lua_tointeger(_NLL, 2);
	int y = (int)lua_tointeger(_NLL, 3);
	BOOL IsFly = FALSE;
	if(TM_Top==4){
		IsFly = (BOOL)lua_toboolean(_NLL, 4);
	}
	IsFly = MAP_walkAbleFromPoint(map,x,y,IsFly);
	LRetBool(_NLL, IsFly);
}

int NPC_Lua_Map_GetImageData(lua_State *_NLL)          //获取图片数据
{
	CheckEx(_NLL, 2);
	CheckIndexNull(_NLL, 1);
	int MapImageid = (int)lua_tointeger(_NLL, 1);
	int flg = (int)lua_tointeger(_NLL, 2);
	if(flg >= 0)
	{
		if(flg < LUA_DATALINE1)
		{
			flg -= LUA_DATALINE0;
			int TM_RetInt = MAP_getImageInt(MapImageid, flg);
			LRetInt(_NLL, TM_RetInt);
		}
	}
	LRetErrNull(_NLL, "传入的标志是错误的。");
}
int NPC_Lua_Map_SetImageData(lua_State *_NLL)           //设置图片数据
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 1);
	int MapImageid = (int)lua_tointeger(_NLL, 1);
	int flg = (int)lua_tointeger(_NLL, 2);
	int val = (int)lua_tointeger(_NLL, 3);
	if(flg >= 0)
	{
		if(flg < LUA_DATALINE1)
		{
			flg -= LUA_DATALINE0;
			BOOL TM_RetBool = MAP_setImageInt(MapImageid, flg, val);
			LRetBool(_NLL, TM_RetBool);
		}
	}
	LRetErrNull(_NLL, "传入的标志是错误的。");
}
static OBJECT TM_Object = NULL;
int NPC_Lua_Map_GetTopObj(lua_State *_NLL)           //用来获取地图某点上对象
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 1);
	int map = (int)lua_tointeger(_NLL, 1);
	int x = (int)lua_tointeger(_NLL, 2);
	int y = (int)lua_tointeger(_NLL, 3);
	TM_Object = MAP_getTopObj( map, x, y) ;
	LRetBool(_NLL, TRUE);
}

int NPC_Lua_Map_GetNextObj(lua_State *_NLL)           //获取下一个对象索引
{
	CheckEx(_NLL, 0);
	int TM_ObjIndex = GET_OBJINDEX(TM_Object);
	TM_Object = NEXT_OBJECT(TM_Object);
	if( !CHECKOBJECTUSE(TM_ObjIndex) )
	{
		LRetInt(_NLL, -1);
		TM_Object = NULL;
	}
	LRetInt(_NLL, TM_ObjIndex);
}

int NPC_Lua_Map_CheckImageIndex(lua_State *_NLL)           //检测某地图号图片是否存在
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int map = (int)lua_tointeger(_NLL, 1);
	BOOL TM_Ret = IsValidImagenumber( map) ;
	LRetBool(_NLL, TM_Ret);
}

int NPC_Lua_Map_CheckIndex(lua_State *_NLL)          //检测某地图号地图是否存在
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int map = (int)lua_tointeger(_NLL, 1);
	BOOL TM_Ret = CHECKFLOORID( map) ;
	LRetBool(_NLL, TM_Ret);
}

int NPC_Lua_Map_MakeNewMap(lua_State *_NLL)          //制造一个副本地图，并返回新地图号
{
	CheckEx2(_NLL, 1, 2);
	CheckIndexNull(_NLL, 1);
	int TM_Top = lua_gettop(_NLL);
	int map = (int)lua_tointeger(_NLL, 1);
	char *name = "";
	if(TM_Top == 2)
	{
		CheckIndexNull(_NLL, 2);
		name = lua_tostring(_NLL, 2);
	}
	int TM_Ret = MAP_makenew( map,name) ;
	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_Map_DelNewMap(lua_State *_NLL)          //删除一个副本地图
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int map = (int)lua_tointeger(_NLL, 1);
	BOOL TM_Ret = MAP_DelMap( map) ;
	LRetBool(_NLL, TM_Ret);
}

int NPC_Lua_Map_SetExWarp(lua_State *_NLL)          //设置一个地图的退出传送点
{
	CheckEx(_NLL, 5);
	CheckIndexNull(_NLL, 1);
	int map = (int)lua_tointeger(_NLL, 1);
	int exfl = (int)lua_tointeger(_NLL, 2);
	int exx = (int)lua_tointeger(_NLL, 3);
	int exy = (int)lua_tointeger(_NLL, 4);
	int type = (int)lua_tointeger(_NLL, 4);//地图类型
	BOOL TM_Ret = MAP_SetExWarp( map, exfl,exx,exy,type) ;
	LRetBool(_NLL, TM_Ret);
}

int NPC_Lua_Map_SetMapPoint(lua_State *_NLL)          //设置一个地图传送点 返回一个传送点索引
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	char* mappoint = lua_tostring(_NLL, 1);
	int TM_Ret = SetMapPoint( mappoint) ;
	LRetInt(_NLL, TM_Ret);
}

int NPC_Lua_Map_DelMapPoint(lua_State *_NLL)          //删除一个传送点
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int ps = (int)lua_tointeger(_NLL, 1);
	BOOL TM_Ret = DelMapPoint( ps) ;
	LRetBool(_NLL, TM_Ret);
}

int NPC_Lua_Map_getFloorName(lua_State *_NLL)          //返回一个地图名
{
	CheckEx(_NLL, 1);
	CheckIndexNull(_NLL, 1);
	int MapID = (int)lua_tointeger(_NLL, 1);
	char *MapName = MAP_getFloorName( MapID) ;
	LRetMsg(_NLL, MapName);
}
/*
int NPC_Lua_Map_Upmap(lua_State *_NLL)          //更新地图
{
	CheckEx(_NLL, 3);
	CheckIndexNull(_NLL, 1);
	int TM_Index = (int)lua_tointeger(_NLL, 1);
	int formatmap = (int)lua_tointeger(_NLL, 2);
	int tomap = (int)lua_tointeger(_NLL, 3);
	lssproto_Upmap_send(TM_Index,formatmap,tomap);
	LRetBool(_NLL, TRUE);
}
*/


