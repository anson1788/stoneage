#include <string.h>
#include "battle.h"
#include "char_base.h"
#include "npccreate.h"
#include "object.h"
#include "char.h"
#include "enemy.h"
#include "char_base.h"
#include "npc_eventaction.h"
#include "lua.h"
#include "longzoro/version.h"
#include "lauxlib.h"
#include "lualib.h"
#include "npcutil.h"
#include "log.h"
#ifdef _ALLBLUES_LUA   
#ifdef _NEW_ITEM_
extern int CheckCharMaxItem(int charindex);
#endif
static int CreateNpc (lua_State *L) {
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
	
	strcpysafe( one.string[CHAR_NAME].string ,
				sizeof(one.string[CHAR_NAME].string),
				Name);
	one.data[CHAR_BASEBASEIMAGENUMBER] = Image;
	one.data[CHAR_BASEIMAGENUMBER] = Image;
	one.data[CHAR_FLOOR]=Floor;
	one.data[CHAR_X]=x;
	one.data[CHAR_Y]=y;
	one.data[CHAR_DIR] = dir;
	
	int npcindex = CHAR_initCharOneArray(&one);

	if( npcindex < 0 ) {
	 	print( "NPC制作失败。\n");
	}
  
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

#ifdef _DEL_FAME
static int DelFame (lua_State *L) 
{
	size_t l;
  const int index = luaL_checkint(L, 1);
  char *data = luaL_checklstring(L, 2, &l);
  
	NPC_ActionDelFame(index, data);
  return 1;
}
#endif
static int AddGold (lua_State *L) 
{
	size_t l;
  const int index = luaL_checkint(L, 1);
  char *data = luaL_checklstring(L, 2, &l);
  
	NPC_ActionAddGold2(index, data);
  return 1;
}


static int DelGold (lua_State *L) 
{
	size_t l;
  const int index = luaL_checkint(L, 1);
  char *data = luaL_checklstring(L, 2, &l);
  
	NPC_ActionDelGold(index, data);
  return 1;
}
#ifdef _MOMENTUM_NPC
static int DelMom (lua_State *L) 
{
	size_t l;
  const int index = luaL_checkint(L, 1);
  char *data = luaL_checklstring(L, 2, &l);
  
	NPC_ActionMomentum(index, data);
  return 1;
}
#endif
#ifdef _DP_NPC
static int DelDP (lua_State *L) 
{
	size_t l;
  const int index = luaL_checkint(L, 1);
  char *data = luaL_checklstring(L, 2, &l);
  
	NPC_ActionDP(index, data);
  return 1;
}
#endif
static int DelItem (lua_State *L) 
{
	size_t l;
  const int index = luaL_checkint(L, 1);
  char *data = luaL_checklstring(L, 2, &l);
  
	NPC_ActionDelItem(index, data);
  return 1;
}


static int AddItem (lua_State *L) 
{
	size_t l;
  const int index = luaL_checkint(L, 1);
  char *data = luaL_checklstring(L, 2, &l);
  
	NPC_ActionAddItem(index, data);
  return 1;
}
static int AddRandItem (lua_State *L)
{
	size_t l;
  const int TM_Index = luaL_checkint(L, 1);
  char *TM_ItemIdData = luaL_checklstring(L, 2, &l);
  int TM_Num = 1;
	int TM_ItemIndex = -1;
	int TM_Int = -1;
	int TM_ItemId = -1;
	int TM_ItemIdStart = -1;
	int TM_ItemIdEnd = -1;
	char ItemIdBuf[32];
	if(getStringFromIndexWithDelim(TM_ItemIdData,",", 2, ItemIdBuf, sizeof(ItemIdBuf)) != FALSE){
		int TM_ItemNum = 0;
		while(1){
			if(getStringFromIndexWithDelim(TM_ItemIdData,",", TM_ItemNum+1, ItemIdBuf, sizeof(ItemIdBuf)) == FALSE)
				break;
			TM_ItemNum++;
		}
		TM_ItemNum = RAND(1,TM_ItemNum);
		getStringFromIndexWithDelim(TM_ItemIdData,",", TM_ItemNum, ItemIdBuf, sizeof(ItemIdBuf));
		char ItemIdBuf2[16];
		if( getStringFromIndexWithDelim(ItemIdBuf,"-", 2, ItemIdBuf2, sizeof(ItemIdBuf2)) != FALSE){
			int TM_ItemIdStart,TM_ItemIdEnd;
			getStringFromIndexWithDelim(ItemIdBuf,"-", 1, ItemIdBuf2, sizeof(ItemIdBuf2));
			TM_ItemIdStart = atoi(ItemIdBuf2);
			getStringFromIndexWithDelim(ItemIdBuf,"-", 2, ItemIdBuf2, sizeof(ItemIdBuf2));
			TM_ItemIdEnd = atoi(ItemIdBuf2);
			TM_ItemId = RAND(TM_ItemIdStart,TM_ItemIdEnd);
		}else{
			TM_ItemId = atoi(ItemIdBuf);
		}
	}else if(getStringFromIndexWithDelim(TM_ItemIdData,"-", 2, ItemIdBuf, sizeof(ItemIdBuf)) != FALSE){
		if(getStringFromIndexWithDelim(TM_ItemIdData,"-", 1, ItemIdBuf, sizeof(ItemIdBuf)) != FALSE)
			TM_ItemIdStart = atoi(ItemIdBuf);
		if(getStringFromIndexWithDelim(TM_ItemIdData,"-", 2, ItemIdBuf, sizeof(ItemIdBuf)) != FALSE)
			TM_ItemIdEnd = atoi(ItemIdBuf);
		if(TM_ItemIdStart<=TM_ItemIdEnd){
			TM_ItemId = RAND(TM_ItemIdStart,TM_ItemIdEnd);
		}
	}else{
		TM_ItemId = atoi(TM_ItemIdData);
	}
  if(TM_ItemId > -1){
		TM_ItemIndex = NPC_GiveItem(TM_Index, TM_ItemId);
	}
	lua_pushinteger(L, TM_ItemIndex);
  return 1;
}


static int AddPet (lua_State *L) 
{
	size_t l;
  const int index = luaL_checkint(L, 1);
  char *data = luaL_checklstring(L, 2, &l);
  
	NPC_ActionAddPet(index, data);
  return 1;
}

static int EvEnd (lua_State *L) 
{
	size_t l;
  const int index = luaL_checkint(L, 1);
  char *data = luaL_checklstring(L, 2, &l);
  
	NPC_ActionSetEend(index, data);
  return 1;
}

static int EvNow (lua_State *L) 
{
	size_t l;
  const int index = luaL_checkint(L, 1);
  char *data = luaL_checklstring(L, 2, &l);
  
	NPC_ActionSetNow(index, data);
  return 1;
}

static int Event_End (lua_State *L) 
{
	size_t l;
  const int index = luaL_checkint(L, 1);
  char *data = luaL_checklstring(L, 2, &l);
  
	NPC_ActionSetEend(index, data);
  return 1;
}

static int Event_Now (lua_State *L) 
{
	size_t l;
  const int index = luaL_checkint(L, 1);
  char *data = luaL_checklstring(L, 2, &l);
  
	NPC_ActionSetNow(index, data);
  return 1;
}

static int EvClr (lua_State *L) 
{
	size_t l;
  const int index = luaL_checkint(L, 1);
  char *data = luaL_checklstring(L, 2, &l);
  
	NPC_ActionClearEvent(index, data);
  return 1;
}

#ifdef _VIP_SHOP
static int VipItem (lua_State *L) 
{
	size_t l;
  const int index = luaL_checkint(L, 1);
  char *data = luaL_checklstring(L, 2, &l);
  
	NPC_ActionVipItem(index, data);
  return 1;
}

static int VipPet (lua_State *L) 
{
	size_t l;
  const int index = luaL_checkint(L, 1);
  char *data = luaL_checklstring(L, 2, &l);
  
	NPC_ActionVipPet(index, data);
  return 1;
}
#endif

#ifdef _NEW_VIP_SHOP
static int Vip_Item (lua_State *L) 
{
	size_t l;
  const int index = luaL_checkint(L, 1);
  char *data = luaL_checklstring(L, 2, &l);
  
	NPC_ActionVipItem(index, data);
  return 1;
}

static int Vip_Pet (lua_State *L) 
{
	size_t l;
  const int index = luaL_checkint(L, 1);
  char *data = luaL_checklstring(L, 2, &l);
  
	NPC_ActionVipPet(index, data);
  return 1;
}
#endif

#ifdef  _NPC_ADDLEVELUP
static int SetLevel (lua_State *L) 
{
	size_t l;
  const int index = luaL_checkint(L, 1);
  char *data = luaL_checklstring(L, 2, &l);
  
	NPC_ActionLevelAndTransUp( 0, index, atoi(data), 0, 0, -1);
  return 1;
}

static int AddExps (lua_State *L) 
{
	size_t l;
  const int index = luaL_checkint(L, 1);
  char *data = luaL_checklstring(L, 2, &l);
  
	NPC_ActionLevelAndTransUp( 0, index, 0, 0, atoi(data), -1);
  return 1;
}

static int AddSkillPoint (lua_State *L) 
{
	size_t l;
  const int index = luaL_checkint(L, 1);
  char *data = luaL_checklstring(L, 2, &l);
  
	NPC_ActionLevelAndTransUp( 0, index, 0, atoi(data), 0, -1);
  return 1;
}

static int SetRideType (lua_State *L) 
{
	size_t l;
  const int index = luaL_checkint(L, 1);
  char *data = luaL_checklstring(L, 2, &l);
  
	NPC_ActionLevelAndTransUp( 0, index, 0, 0, 0, atoi(data));
  return 1;
}
#endif

static int isFaceToFace (lua_State *L) 
{
  const int meindex = luaL_checkint(L, 1);
  const int talkerindex = luaL_checkint(L, 2);
  
  
  if(NPC_Util_isFaceToFace( meindex ,talkerindex ,2)==FALSE){
		if( NPC_Util_CharDistance( talkerindex ,meindex ) > 1) {
			lua_pushinteger(L, 0);
			return 1;
		}
	}

  lua_pushinteger(L, 1);
  return 1;
}

static int ActionPassCheck (lua_State *L) 
{
	size_t l;
  const int meindex = luaL_checkint(L, 1);
  const int toindex = luaL_checkint(L, 2);
  char *data = luaL_checklstring(L, 3, &l);
  
  if( NPC_ActionPassCheck( meindex, toindex, data) == FALSE )	{
			lua_pushinteger(L, 0);
			return 1;
	}

  lua_pushinteger(L, 1);
  return 1;
}


#ifdef _ALLBLUES_LUA_1_1
static int SetPoint (lua_State *L) 
{
  const int meindex = luaL_checkint(L, 1);
  const int ff = luaL_checkint(L, 2);
  const int fx = luaL_checkint(L, 3);
  const int fy = luaL_checkint(L, 4);
  
  OBJECT_setFloor(CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX), ff);
  OBJECT_setX(CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX), fx);
  OBJECT_setY(CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX), fy);
  CHAR_setInt(meindex, CHAR_FLOOR, ff);
  CHAR_setInt(meindex, CHAR_X, fx);
  CHAR_setInt(meindex, CHAR_Y, fy);
  CHAR_sendCDArroundChar( ff, fx, fy, CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX));
  return 1;
}


static int DelItemNum (lua_State *L) 
{
	size_t l;
	int index = luaL_checkint(L, 1);
	char *data = luaL_checklstring(L, 2, &l);
	char tmp[16];
	int itemID = 0;
	int itemNUM = 0;
  
	if( getStringFromIndexWithDelim( data, ",", 1, tmp, sizeof( tmp)) != FALSE ){
		itemID = atoi(tmp);
	}
	
	if( getStringFromIndexWithDelim( data, ",", 2, tmp, sizeof( tmp)) != FALSE ){
		itemNUM = atoi(tmp);
	}
	
	int i, itemindex;
	char token[256];
	int num=0;
#ifdef _NEW_ITEM_
	int itemMax = CheckCharMaxItem(index);
	for( i = CHAR_STARTITEMARRAY ; i < itemMax ; i++ ){
#else
	for( i = CHAR_STARTITEMARRAY ; i < CHAR_MAXITEMHAVE ; i++ ){
#endif
		if(num >= itemNUM)break;
		itemindex = CHAR_getItemIndex( index , i );
		if( !ITEM_CHECKINDEX(itemindex) ) continue;
		if( ITEM_getInt(itemindex, ITEM_ID) != itemID ) continue;
		sprintf( token, "交出道具%s",ITEM_getChar( itemindex, ITEM_NAME));
		CHAR_talkToCli( index, -1, token, CHAR_COLORYELLOW);

						LogItem(
							CHAR_getChar( index, CHAR_NAME ),
							CHAR_getChar( index, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD 在item的log中增加item名称
							itemindex,
#else
							ITEM_getInt( itemindex, ITEM_ID),
#endif
							"任务收取",
							CHAR_getInt( index,CHAR_FLOOR),
							CHAR_getInt( index,CHAR_X ),
							CHAR_getInt( index,CHAR_Y ),
							ITEM_getChar( itemindex, ITEM_UNIQUECODE),
							ITEM_getChar( itemindex, ITEM_NAME),
							ITEM_getInt( itemindex, ITEM_ID)
						);
			CHAR_setItemIndex( index, i, -1);
			ITEM_endExistItemsOne( itemindex);
			CHAR_sendItemDataOne( index, i);
			num++;
	}
	
	lua_pushinteger(L, num);
  return 1;
}

static int DelNpc (lua_State *L)
{
	const int index = luaL_checkint(L, 1);
	
	if( !CHAR_CHECKINDEX( index ) ){
		print( "ANDY index=%d err\n", index);
		return 1;
	}

	CHAR_CharaDeleteHavePet( index);
	CHAR_CharaDelete( index );
	
  return 1;
}

#endif



#ifdef _PLAYER_NPC
static int CreateSpecialNpc (lua_State *L) 
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
#endif

#ifdef _ALLBLUES_LUA_1_4
#ifdef _PLAYER_NPC 
static int CreatePlayer (lua_State *L) {
	Char one;
	size_t l;
	CHAR_getDefaultChar( &one, 0);
	one.data[CHAR_WHICHTYPE] = CHAR_TYPEPLAYERNPC;
	char *Name = luaL_checklstring(L, 1, &l);
	const int Image = luaL_checkinteger(L, 2);
	const int Floor = luaL_checkinteger(L, 3);
	const int x = luaL_checkinteger(L, 4);
	const int y = luaL_checkinteger(L, 5);
	const int dir = luaL_checkinteger(L, 6);
	
	strcpysafe( one.string[CHAR_NAME].string ,
				sizeof(one.string[CHAR_NAME].string),
				Name);
	one.data[CHAR_BASEBASEIMAGENUMBER] = Image;
	one.data[CHAR_BASEIMAGENUMBER] = Image;
	one.data[CHAR_FLOOR]=Floor;
	one.data[CHAR_X]=x;
	one.data[CHAR_Y]=y;
	one.data[CHAR_DIR] = dir;
	
	int npcindex = CHAR_initCharOneArray(&one);

	if( npcindex < 0 ) {
	 	print( "NPC制作失败。\n");
	}
  
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
	   lua_pushinteger(L, -1);
	   return 1;
	}
	
	CHAR_setWorkInt( npcindex,CHAR_WORKOBJINDEX, objindex );
	CHAR_LoginBesideSetWorkInt( npcindex, -1 );
	
	CHAR_setFlg( npcindex, CHAR_ISDUEL, 0);
	CHAR_setFlg( npcindex, CHAR_ISTRADECARD, 0);
	CHAR_setFlg( npcindex, CHAR_ISTRADE, 0);
	CHAR_setFlg( npcindex, CHAR_ISPARTY, 0);

	lua_pushinteger(L, npcindex);
  return 1;
}
#endif
#endif


#ifdef _ONLINE_COST
static int OnlineCost (lua_State *L)
{
  const int charaindex = luaL_checkint(L, 1);

  NPC_ActionOnlineCost(charaindex);
  lua_pushinteger(L, 1);
  return 1;
}

static int OnlineBuy (lua_State *L)
{
  const int charaindex = luaL_checkint(L, 1);

  NPC_ActionOnlineBuy(charaindex);
  lua_pushinteger(L, 1);
  return 1;
}
#endif
static const luaL_Reg npclib[] = {
  {"CreateNpc", 			CreateNpc},
#ifdef _DEL_FAME
	{"DelFame", 				DelFame},
#endif
	{"AddGold", 				AddGold},
	{"DelGold", 				DelGold},
#ifdef _MOMENTUM_NPC
	{"DelMom", 					DelMom},
#endif
#ifdef _DP_NPC
	{"DelDP", 					DelDP},
#endif
	{"DelItem", 				DelItem},
	{"AddItem", 				AddItem},
	{"AddRandItem", 		AddRandItem},
  {"AddPet", 					AddPet},
	{"EvEnd", 					EvEnd},
	{"EvNow", 					EvNow},
	{"Event_End", 			Event_End},
	{"Event_Now", 			Event_Now},
	{"EvClr", 					EvClr},
#ifdef _VIP_SHOP
	{"VipItem", 				VipItem},
	{"VipPet", 					VipPet},
#endif
#ifdef _NEW_VIP_SHOP
	{"Vip_Item", 				Vip_Item},
	{"Vip_Pet", 				Vip_Pet},
#endif
#ifdef  _NPC_ADDLEVELUP
	{"SetLevel", 				SetLevel},
	{"AddExps", 				AddExps},
	{"AddSkillPoint", 	AddSkillPoint},
	{"SetRideType", 		SetRideType},
#endif
	{"isFaceToFace", 		isFaceToFace},
	{"Free", 						ActionPassCheck},
#ifdef  _ALLBLUES_LUA_1_1	
	{"SetPoint", 				SetPoint},
	{"DelItemNum", 			DelItemNum},
	{"DelNpc", 					DelNpc},
#endif
#ifdef _PLAYER_NPC
	{"CreateSpecialNpc", 					CreateSpecialNpc},
#ifdef  _ALLBLUES_LUA_1_4	
	{"CreatePlayer", 		CreatePlayer},
#endif
#endif
#ifdef _ONLINE_COST
	{"OnlineCost", 		OnlineCost},
	{"OnlineBuy", 		OnlineBuy},
#endif
	{NULL, 							NULL}
};


LUALIB_API int luaopen_NPC (lua_State *L) {
  luaL_register(L, "npc", npclib);
  return 1;
}

#endif
