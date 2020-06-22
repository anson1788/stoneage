#include <string.h>
#include "lua.h"
#include "longzoro/version.h"
#include "lauxlib.h"
#include "lualib.h"
#include "net.h"
#include "log.h"
#include "longzoro/sasql.h"

#ifdef _ALLBLUES_LUA
#ifdef _ALLBLUES_LUA_1_4
#ifdef _SASQL
#include <mysql/mysql.h>
extern MYSQL mysql;
static MYSQL_RES *mysql_result;
static MYSQL_ROW mysql_row;
static MYSQL_FIELD *fields;
int numrow = 0;
int numfields = 0;
static int setVipPoint(lua_State *L) 
{
	const int charaindex = luaL_checkint(L, 1);
	const int point = luaL_checkint(L, 2);

  sasql_setVipPoint(charaindex, point);
	return 1;
}

static int getVipPoint(lua_State *L) 
{
	const int charaindex = luaL_checkint(L, 1);

	lua_pushinteger(L, sasql_getVipPoint(charaindex));
	return 1;
}

static int setVipPointForCdkey(lua_State *L) 
{
	size_t l;
	char *id=luaL_checklstring(L, 1, &l);
	const int point = luaL_checkint(L, 2);

  sasql_setVipPointForCdkey(id, point);
	return 1;
}

static int getVipPointForCdkey(lua_State *L) 
{
	size_t l;
	char *id=luaL_checklstring(L, 1, &l);
	lua_pushinteger(L, sasql_getVipPointForCdkey(id));
	return 1;
}

static int setPayPoint(lua_State *L) 
{
	const int charaindex = luaL_checkint(L, 1);
	const int point = luaL_checkint(L, 2);

  sasql_setPayPoint(charaindex, point);
	return 1;
}

static int getPayPoint(lua_State *L) 
{
	const int charaindex = luaL_checkint(L, 1);

	lua_pushinteger(L, sasql_getPayPoint(charaindex));
	return 1;
}


#ifdef _GAMBLE_POINT
static int setGamblePoint(lua_State *L)
{
	const int charaindex = luaL_checkint(L, 1);
	const int point = luaL_checkint(L, 2);

  sasql_setGamblePoint(charaindex, point);
	return 1;
}

static int getGamblePoint(lua_State *L)
{
	const int charaindex = luaL_checkint(L, 1);

	lua_pushinteger(L, sasql_getGamblePoint(charaindex));
	return 1;
}
#endif

#ifdef _TRIAL_POINT
static int setTrialPoint(lua_State *L)
{
	const int charaindex = luaL_checkint(L, 1);
	const int point = luaL_checkint(L, 2);

  sasql_setTrialPoint(charaindex, point);
	return 1;
}

static int getTrialPoint(lua_State *L)
{
	const int charaindex = luaL_checkint(L, 1);

	lua_pushinteger(L, sasql_getTrialPoint(charaindex));
	return 1;
}
#endif

#ifdef _GLORY_POINT
static int setGloryPoint(lua_State *L)
{
	const int charaindex = luaL_checkint(L, 1);
	const int point = luaL_checkint(L, 2);

  sasql_setGloryPoint(charaindex, point);
	return 1;
}

static int getGloryPoint(lua_State *L)
{
	const int charaindex = luaL_checkint(L, 1);

	lua_pushinteger(L, sasql_getGloryPoint(charaindex));
	return 1;
}
#endif

static int setPayPointForCdkey(lua_State *L) 
{
	size_t l;
	char *id=luaL_checklstring(L, 1, &l);
	const int point = luaL_checkint(L, 2);

  sasql_setPayPointForCdkey(id, point);
	return 1;
}

static int getPayPointForCdkey(lua_State *L) 
{
	size_t l;
	char *id=luaL_checklstring(L, 1, &l);
	lua_pushinteger(L, sasql_getPayPointForCdkey(id));
	return 1;
}


static int query(lua_State *L) 
{
	size_t l;
	char *data=luaL_checklstring(L, 1, &l);

	if(!sasql_mysql_query(data)){
		lua_pushinteger(L, 1);
	}else{
		lua_pushinteger(L, 0);
	}
	return 1;
}

static int free_result(lua_State *L) 
{
	mysql_free_result(mysql_result);
	return 1;
}

static int store_result(lua_State *L) 
{
	 mysql_result=mysql_store_result(&mysql);
	return 1;
}

static int num_rows(lua_State *L) 
{
	numrow=mysql_num_rows(mysql_result);
	lua_pushinteger(L, numrow);
	return 1;
}

static int num_fields(lua_State *L) 
{
	lua_pushinteger(L, numfields);
	return 1;
}

static int fetch_row(lua_State *L) 
{
	mysql_row = mysql_fetch_row(mysql_result);
	return 1;
}

static int data(lua_State *L) 
{
	int id = luaL_checkint(L, 1) - 1;
	lua_pushstring(L, mysql_row[id]);
	return 1;
}
#ifdef _NEW_SQL_MONEY
static int setSQLPoint(lua_State *L)
{
	const int charaindex = luaL_checkint(L, 1);
	const int point = luaL_checkint(L, 2);

  sasql_setSQLPoint(charaindex, point);
	return 1;
}

static int getSQLPoint(lua_State *L)
{
	const int charaindex = luaL_checkint(L, 1);

	lua_pushinteger(L, sasql_getSQLPoint(charaindex));
	return 1;
}

static int setGoldPoint(lua_State *L)
{
	const int charaindex = luaL_checkint(L, 1);
	const int point = luaL_checkint(L, 2);

  sasql_setGoldPoint(charaindex, point);
	return 1;
}

static int getGoldPoint(lua_State *L)
{
	const int charaindex = luaL_checkint(L, 1);

	lua_pushinteger(L, sasql_getGoldPoint(charaindex));
	return 1;
}
static int setSQLPointByCdkey(lua_State *L)
{
	size_t l;
	char *cdkey=luaL_checklstring(L, 1, &l);

	const int point = luaL_checkint(L, 2);

  sasql_setSQLPointByCdkey(cdkey, point);
	return 1;
}

static int getSQLPointByCdkey(lua_State *L)
{
	size_t l;
	char *cdkey=luaL_checklstring(L, 1, &l);

	lua_pushinteger(L, sasql_getSQLPointByCdkey(cdkey));
	return 1;
}

static int setGoldPointByCdkey(lua_State *L)
{
	size_t l;
	char *cdkey=luaL_checklstring(L, 1, &l);
	const int point = luaL_checkint(L, 2);

  sasql_setGoldPointByCdkey(cdkey, point);
	return 1;
}

static int getGoldPointByCdkey(lua_State *L)
{
	size_t l;
	char *cdkey=luaL_checklstring(L, 1, &l);

	lua_pushinteger(L, sasql_getGoldPointByCdkey(cdkey));
	return 1;
}

#endif

static const luaL_Reg sasqllib[] = {
  {"setVipPoint", 						setVipPoint},
  {"getVipPoint", 						getVipPoint},
	{"query", 									query},
	{"free_result", 						free_result},
	{"store_result", 						store_result},
	{"num_rows", 								num_rows},
	{"fetch_row", 							fetch_row},
	{"num_fields", 							num_fields},
	{"data", 										data},
	{"setVipPointForCdkey", 		setVipPointForCdkey},
  {"getVipPointForCdkey", 		getVipPointForCdkey},
  {"setPayPoint", 						setPayPoint},
  {"getPayPoint", 						getPayPoint},
	{"setPayPointForCdkey", 		setPayPointForCdkey},
  {"getPayPointForCdkey", 		getPayPointForCdkey},
#ifdef _GAMBLE_POINT
  {"setGamblePoint", 						setGamblePoint},
  {"getGamblePoint", 						getGamblePoint},
#endif
#ifdef _TRIAL_POINT
  {"setTrialPoint", 						setTrialPoint},
  {"getTrialPoint", 						getTrialPoint},
#endif
#ifdef _GLORY_POINT
  {"setGloryPoint", 						setGloryPoint},
  {"getGloryPoint", 						getGloryPoint},
#endif
#ifdef _NEW_SQL_MONEY
  {"setSQLPoint", 						setSQLPoint},
  {"getSQLPoint", 						getSQLPoint},
  {"setGoldPoint", 						setGoldPoint},
  {"getGoldPoint", 						getGoldPoint},
  {"setSQLPointByCdkey", 						setSQLPointByCdkey},
  {"getSQLPointByCdkey", 						getSQLPointByCdkey},
  {"setGoldPointByCdkey", 						setGoldPointByCdkey},
  {"getGoldPointByCdkey", 						getGoldPointByCdkey},
#endif
  {NULL, 							NULL}
};

LUALIB_API int luaopen_Sasql (lua_State *L) {
  luaL_register(L, "sasql", sasqllib);
  return 1;
}
#endif
#endif
#endif
