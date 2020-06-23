
#ifndef __MYLUA_FUNCTION_H__
#define __MYLUA_FUNCTION_H__

BOOL RunCharTalkedEvent(int meindex, int toindex, char *messageeraseescape, int color, int channel);
BOOL RunCharLoopEvent(int meindex);
BOOL RunCharOverlapEvent( int meindex, int toindex);
BOOL RunCharBattleOverEvent( int meindex, int toindex, int iswin);
BOOL RunCharWindowTalked( int meindex, int talkerindex, int seqno, int select, char *data);

#ifdef _ALLBLUES_LUA_1_2 
BOOL RunItemUseEvent( int itemindex, int charaindex, int toindex, int haveitemindex );
BOOL RunItemDieReLifeEvent( int toindex, int itemindex, int haveitemindex );
BOOL RunItemDetachEvent( int charaindex, int itemindex );
BOOL RunItemAttachEvent( int charaindex, int itemindex );
BOOL RunItemPickupEvent( int charaindex, int itemindex );
BOOL RunItemPostOverEvent( int itemindex, int charaindex );
BOOL RunItemPreOverEvent( int itemindex, int charaindex );
BOOL RunItemDropEvent( int charaindex, int itemindex );
BOOL RunUseChatMagic( int charaindex, char *data, lua_State *lua);
#endif
#ifdef _PETSKILL_SHOP_LUA   
BOOL FreePetSkillShop( int talkerindex, int petindex, int oldSkillID, int newSkillID);
#endif
void ABNPC_Lua_NEWSHOP_Recv( int charaindex);
#ifdef _PETSKILL_SHOP_LUA   
BOOL OffLineCommand( int battleindex, int charindex, int side);
#endif
#ifdef _ALLBLUES_LUA_1_4
BOOL BattleFinish( int battleindex, int charaindex);
BOOL BattleEscape( int battleindex, int charaindex);
BOOL RunCharLogOutEvent( int charaindex);
#endif
#ifdef _ALLBLUES_LUA_1_5
BOOL FreePartyJoin( int charaindex, int toindex );
BOOL FreeVsPlayer( int charaindex, int toindex );
BOOL FreeCharLogin( int charaindex );
BOOL FreeCharCreate( int charaindex );
BOOL NetLoopFunction( void );
#endif
#ifdef _ALLBLUES_LUA_1_6
BOOL FamilyRideFunction( int meindex, int petindex, int petid );
BOOL CharTalkFunction( int charaindex, char *message, int color );
#endif
#ifdef _ALLBLUES_LUA_1_7
BOOL CharVsEnemyFunction( int charaindex );
#endif
#ifdef _ALLBLUES_LUA_1_8
BOOL CaptureOkFunction( int attackindex, int defindex );
BOOL CaptureCheckFunction( int attackindex, int defindex );
#endif
#ifdef _ALLBLUES_LUA_1_9
BOOL SetBattleEnmeyFunction(int meindex, int enemyindex, int id );
BOOL BattleFinishFunction( int charaindex, int battletime, int battleturn, int battletype );
void FreeCharExpSave( int charindex, int exp );
BOOL BattleCommand( int charaindex, int battleindex );
#endif
#endif

#ifdef _RIDEQUERY_
void FreeRideQuery( int charaindex);
#endif

