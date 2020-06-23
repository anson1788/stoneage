#ifndef __NPC_LUA_INTERFACE__
#define __NPC_LUA_INTERFACE__

#include "version.h"
#include "common.h"

/////////////////////////////////////////外部接口/////////////////////////////////////////////////////////////////
#define FUNCNAME_INITCALLBACK						"NPC_Lua_InitCallBack" 							//CHAR_INITFUNC
#define FUNCNAME_WALKPRECALLBACK				"NPC_Lua_WalkPreCallBack" 					//CHAR_WALKPREFUNC
#define FUNCNAME_WALKPOSTCALLBACK				"NPC_Lua_WalkPostCallBack"					//CHAR_WALKPOSTFUNC
#define FUNCNAME_PREOVERCALLBACK				"NPC_Lua_PreOverCallBack"						//CHAR_PREOVERFUNC
#define FUNCNAME_POSTOVERCALLBACK				"NPC_Lua_PostOverCallBack"					//CHAR_POSTOVERFUNC
#define FUNCNAME_WATCHCALLBACK					"NPC_Lua_WatchCallBack"							//CHAR_WATCHFUNC
#define FUNCNAME_LOOPCALLBACK						"NPC_Lua_LoopCallBack"							//CHAR_LOOPFUNC
#define FUNCNAME_TALKEDCALLBACK					"NPC_Lua_TalkedCallBack"						//CHAR_TALKEDFUNC
#define FUNCNAME_OFFCALLBACK						"NPC_Lua_OFFCallBack"								//CHAR_OFFFUNC
#define FUNCNAME_LOOKEDCALLBACK					"NPC_Lua_LookedCallBack"						//CHAR_LOOKEDFUNC
#define FUNCNAME_ITEMPUTCALLBACK				"NPC_Lua_ItemPutCallBack"						//CHAR_ITEMPUTFUNC
#define FUNCNAME_WINDOWTALKEDCALLBACK		"NPC_Lua_WindowTalkedCallBack"			//CHAR_WINDOWTALKEDFUNC
#ifdef _USER_CHARLOOPS
#define FUNCNAME_CHARLOOPSCALLBACK			"NPC_Lua_CharLoopsCallBack"					//CHAR_LOOPFUNCTEMP1	宠物蛋喂药和原地遇敌的LOOP
#define FUNCNAME_BATTLEPROPERTYCALLBACK	"NPC_Lua_BattleProPertyCallBack"		//CHAR_BATTLEPROPERTY
#endif
/*
	CHAR_DYINGFUNC,
	,
	CHAR_PREATTACKEDFUNC,
	CHAR_POSTATTACKEDFUNC,

	CHAR_SPECIALTALKEDFUNC,
	,
#ifdef _USER_CHARLOOPS
	,
	CHAR_LOOPFUNCTEMP2,
	,
#endif
*/
#define FUNCNAME_ITEMPEROVERCALLBACK		"NPC_Lua_ItemPerOverCallBack"				//LUAITEM_PREOVERFUNC = ITEM_PREOVERFUNC
#define FUNCNAME_ITEMPOSTOVERCALLBACK		"NPC_Lua_ItemPostOverCallBack"			//LUAITEM_POSTOVERFUNC = ITEM_POSTOVERFUNC
#define FUNCNAME_ITEMWATCHCALLBACK			"NPC_Lua_ItemWatchCallBack"					//LUAITEM_WATCHFUNC = ITEM_WATCHFUNC
#define FUNCNAME_ITEMUSECALLBACK				"NPC_Lua_ItemUseCallBack"						//LUAITEM_USEFUNC = ITEM_USEFUNC
#define FUNCNAME_ITEMATTACHCALLBACK			"NPC_Lua_ItemAttachCallBack"				//LUAITEM_ATTACHFUNC = ITEM_ATTACHFUNC
#define FUNCNAME_ITEMDETACHCALLBACK			"NPC_Lua_ItemDetachCallBack"				//LUAITEM_DETACHFUNC = ITEM_DETACHFUNC
#define FUNCNAME_ITEMDROPCALLBACK				"NPC_Lua_ItemDropCallBack"					//LUAITEM_DROPFUNC = ITEM_DROPFUNC
#define FUNCNAME_ITEMPICKUPCALLBACK			"NPC_Lua_ItemPickUPCallBack"				//LUAITEM_PICKUPFUNC = ITEM_PICKUPFUNC
#ifdef _Item_ReLifeAct
#define FUNCNAME_ITEMDIERELIFECALLBACK	"NPC_Lua_ItemDieReLifeCallBack"			//LUAITEM_DIERELIFEFUNC = ITEM_DIERELIFEFUNC
#endif

#define FUNCNAME_BATTLEWINCALLBACK "NPC_Lua_BattleWinCallBack" //BATTLE WinFunc
#define FUNCNAME_BATTLEENDCALLBACK "NPC_Lua_BattleEndCallBack" //BATTLE WinFunc
//初始化LUA引擎
int NPC_Lua_Init(const char *_DoFile);

//关闭LUA引擎
int NPC_Lua_Close(void);

//动态加载脚本文件
int NPC_Lua_DoFile(const char *_DoFile);

//从LUA脚本引擎出栈一个参数
const char *NPC_Lua_popstring(int _ArgNum);

//调用指定的脚本函数
const char *NPC_Lua_CallFunc(const char *_FuncName, char *_RetBuff, size_t _n, int index);

//战斗场景随机
int NPC_Lua_getBattleFieldNo(int _Floor, int _X, int _Y);

//获取宠物
int NPC_GivePet(int charaindex, int lv, int enemyid);
//获取道具
int NPC_GiveItem(int charaindex, int itemid);
//删除宠物
int NPC_DelPet(int charaindex, int petsel);

//回调函数
BOOL NPC_Lua_InitCallBack(int _meindex);
BOOL NPC_Lua_WalkPreCallBack(int _meindex, int *_dir, int *_mode);
void NPC_Lua_WalkPostCallBack(int _meindex);
void NPC_Lua_PreOverCallBack(int _meindex, int _desindex);
void NPC_Lua_PostOverCallBack(int _meindex, int _desindex);
void NPC_Lua_WatchCallBack(int _meindex, int _objindex, int _chac, int _x, int _y, int _dir, int *_watchopt, int _watchoptlen);
int NPC_Lua_LoopCallBack(int _meindex);
void NPC_Lua_TalkedCallBack(int _meindex, int _tomeindex,const char *_messageeraseescape, int _color, int _channel);
void NPC_Lua_OFFCallBack(int _meindex, int _desindex);
void NPC_Lua_LookedCallBack(int _meindex, int _desindex);
BOOL NPC_Lua_ItemPutCallBack(int _meindex, int _itemindex);
void NPC_Lua_WindowTalkedCallBack(int _meindex, int _talkindex, int _seqno, int _select,const char *_data);
#ifdef _USER_CHARLOOPS
int NPC_Lua_CharLoopsCallBack( int _meindex);
int NPC_Lua_BattleProPertyCallBack(int _attackindex, int _defindex, int *_damage, int *_powarray, int _arraynum);
#endif

/* 暂时未使用的事件
int NPC_Lua_DyingCallBack(int meindex);
NPC_Lua_PreAttackedCallBack
NPC_Lua_PostAttackedCallBack
NPC_Lua_SpecialTalkedCallBack
*/

void NPC_Lua_ItemPerOverCallBack(int _itemindex, int _playindex);
void NPC_Lua_ItemPostOverCallBack(int _itemindex, int _playindex);
void NPC_Lua_ItemWatchCallBack(int _meindex, int _objindex, int _chac, int _x, int _y, int _dir, int *_watchopt, int _watchoptlen);
void NPC_Lua_ItemUseCallBack(int _playindex, int _to_charindex, int _haveitemindex);
void NPC_Lua_ItemAttachCallBack(int _playindex, int _itemindex);
void NPC_Lua_ItemDetachCallBack(int _playindex, int _itemindex);
void NPC_Lua_ItemDropCallBack(int _playindex, int _itemindex);
void NPC_Lua_ItemPickUPCallBack(int _playindex, int _itemindex);
#ifdef _Item_ReLifeAct
void NPC_Lua_ItemDieReLifeCallBack(int _playindex, int _itemindex, int _haveitem);
#endif

void NPC_Lua_BattleWinCallBack(int _battleindex, int _createindex);
void NPC_Lua_BattleEndCallBack(int _battleindex);
#ifdef _MO_LNS_MYSQLSUOXU
void NPC_Lua_SQLPushCallBack(char* _result,char* _filename,char* _function,int _npcindex,int _playerindex, char* _msg);
void NPC_Lua_SQLPushAdvCallBack(int luaresult,int luaflg,int luaerrnono,char* luaerrstr,int luafileCount,int rowCount,int rowAt,char* row,char* _filename,char* _function,int npcindex,int charaindex,char* msg);
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //#ifndef __NPC_LUA_INTERFACE__
void NPC_Lua_NEWSHOP_Recv(char* function, int charaindex);
#ifdef _CHARSIGNDAY_
void NPC_Lua_CharSignDay_Recv(char* function,int charaindex);
#endif





