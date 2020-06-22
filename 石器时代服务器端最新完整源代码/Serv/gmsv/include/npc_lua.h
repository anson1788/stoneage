#ifndef __NPC_LUA__
#define __NPC_LUA__

#include "npc_lua_interface.h"
#include "../lua/lua.h"
#include "../lua/lauxlib.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define LUA_DATALINE0 0
#define LUA_DATALINE1 2000
#define LUA_DATALINE2 4000
#define LUA_DATALINE3 6000
#define LUA_DATALINE4 8000
#define LUA_DATALINE5 10000
#define LUA_DATALINE6 10100
typedef struct __CREATEENEMY
{
	int EnemyId;
	int BaseLevel;
	int SkillType;
}CREATEENEMY,*PCREATEENEMY;

typedef struct __SCRIPTREGLIB
{
	const char *LibName;				//函数库名
	luaL_reg *FuncRegList;			//需要注册的函数库函数列表
}SCRIPTREGLIB,*PSCRIPTREGLIB;

typedef struct __SCRIPTREGCLASS
{
	const char *ClassName;			//类型名
	PSCRIPTREGLIB NewLib;				//用于申请类实例的 SCRIPTREGLIB
	luaL_reg *FuncRegList;			//需要注册的类函数列表
}SCRIPTREGCLASS,*PSCRIPTREGCLASS;

typedef struct __SCRIPTREGARRAY
{
	const char *ArrayName;			//数组名
	const char **SetMetaTable;	//用于设置数组响应动作
	const char **SetFuncName;		//用于设置数组响应动作的函数名字
	PSCRIPTREGLIB FuncList;			//用于响应对应 SetMetaTable 的处理函数列表
}SCRIPTREGARRAY,*PSCRIPTREGARRAY;

typedef struct __ARRAY_NTINT
{
	size_t Size;
	int Num[1];
}ARRAY_NTINT,*PARRAY_NTINT;

//创建NPC-返回NPC唯一索引
int NPC_Lua_Create(const char *_DoFile, const char *_InitFuncName, char *_seek, BOOL _IsFly);

//删除NPC 传入NPC唯一索引
int NPC_Lua_Del(int _index);

//创建战斗
int NPC_Lua_CreateVsEnemy(lua_State *_NLL, int _CharaIndex, int _NpcIndex, const char *_DoFunc, PCREATEENEMY _CreateEnemy, int _ARLen, int _Flg);

const char *NPC_Lua_SetErrorStr(const char *ErrStr);

//注册扩展调用接口
int NPC_Lua_RegCallEx(lua_State * _NLL, luaL_Reg *_RegList);
//注册扩展函数库接口
int NPC_Lua_RegLibEx(lua_State * _NLL, PSCRIPTREGLIB _RegLib);
//注册扩展类接口
int NPC_Lua_RegClassEx(lua_State * _NLL, PSCRIPTREGCLASS _RegClass);
//注册扩展数组接口
int NPC_Lua_RegArrayEx(lua_State * _NLL, PSCRIPTREGARRAY _RegArray);

//扩展类型接口函数
//NTInt
int NPC_Lua_NTInt_New(lua_State *_NLL);
int NPC_Lua_NTInt_Get(lua_State *_NLL);
int NPC_Lua_NTInt_Set(lua_State *_NLL);
int NPC_Lua_NTInt_Len(lua_State *_NLL);

//接口函数
////////////////////////////////////////////////NL/////////////////////////////////////////////////
int NPC_Lua_NL_GetErrorStr(lua_State *_NLL);						//返回错误信息
int NPC_Lua_NL_CreateNpc(lua_State *_NLL);							//创建NPC
int NPC_Lua_NL_CreateSpecialNpc(lua_State *L);
extern int copyChar(lua_State *L);
extern int createPet(lua_State *L);
extern int setCharPet(lua_State *L);
extern int getCharPet(lua_State *L);
extern int ToAroundChar(lua_State *L);
extern int Attack(lua_State *L);
extern int Recovery(lua_State *L);
extern int CreateVsEnemy (lua_State *L);
extern int talkToAllServer(lua_State *L);
extern int dengon(lua_State *L);
extern int RandRandWalk(lua_State *L);

int NPC_Lua_NL_DelNpc(lua_State *_NLL);									//删除NPC
int NPC_Lua_NL_Mod(lua_State *_NLL);									  //求余数
int NPC_Lua_NL_GetStringFromIndexWithDelim(lua_State *_NLL);									//使用指定字符分割字符串，并返回调用者指定index位置的字符串
//int NPC_Lua_NL_ANSI_PlayerLoop(lua_State *_NLL);		    //遍历所有存在人物，返回索引号
int NPC_Lua_NL_PetLoopGetNext(lua_State *_NLL);		      //遍历所有存在宠物，返回宠物索引号 写法类似定时器
int NPC_Lua_NL_ItemLoopGetNext(lua_State *_NLL);		    //遍历所有存在道具，返回道具索引号 如上
int NPC_Lua_NL_PlayerLoopGetNext(lua_State *_NLL);		  //遍历所有存在人物，返回人物索引号 如上
int NPC_Lua_NL_GetConfigLineType(lua_State *_NLL);	  	//返回0为字符串形。1为SHORT形。2为INT型。3为DOUBLE型。如果不存在该字段，返回-1.
int NPC_Lua_NL_GetConfigLineVal(lua_State *_NLL);	  	  //总是返回[字符串]。如果需要转换为数字，请用tonumber()函数。找不到该字段返回nil
int NPC_Lua_NL_getGameservername(lua_State *_NLL);
BOOL NPC_Lua_CheckNpcEventFree(lua_State *_NLL);									//检测对象是否满足某条件
int NPC_Lua_DoNpcEventAction(lua_State *_NLL);									//执行NPC脚本命令
int NPC_Lua_NL_SetTimer(lua_State *_NLL);	  	          //设定一个计时器，所设的函数将按设定的间隔时间周期调用。
int NPC_Lua_NL_DelTimer(lua_State *_NLL);	  	          //删除一个计时器。一个计时器被删除，就不会再被周期调用；
int NPC_Lua_NL_RunSaFuncII(lua_State *_NLL);	  	      //调用一个服务端内定义好的2个INT参数的函数。
int NPC_Lua_NL_RunSaFuncIII(lua_State *_NLL);	  	      //调用一个服务端内定义好的3个INT参数的函数(如ITEM_USE等使用道具后的回调函数）。
int NPC_Lua_NL_ANSI_PlayerLoop(lua_State *_NLL);		    //遍历所有存在人物，返回索引号
int NPC_Lua_NL_ANSI_PetLoop(lua_State *_NLL);		        //遍历所有存在宠物，返回索引号
int NPC_Lua_NL_ANSI_ItemLoop(lua_State *_NLL);		      //遍历所有存在道具，返回索引号
int NPC_Lua_NL_ClsMk(lua_State *_NLL);
int NPC_Lua_NL_AddCheckNum(lua_State *_NLL);
int NPC_Lua_NL_Mkstring(lua_State *_NLL);
int NPC_Lua_NL_Mkint(lua_State *_NLL);
int NPC_Lua_NL_SendMesg(lua_State *_NLL);
int NPC_Lua_NL_WriteShopData(lua_State *_NLL);
////////////////////////////////////////////////NLG////////////////////////////////////////////////
//功能接口
int NPC_Lua_NLG_UpItem(lua_State *_NLL);
int NPC_Lua_NLG_GiveItemOne(lua_State *_NLL);
int NPC_Lua_NLG_GivePetOne(lua_State *_NLL);
int NPC_Lua_NLG_DelItemByIndex(lua_State *_NLL);	
int NPC_Lua_NLG_FindPet(lua_State *_NLL);								//查找宠物栏空位
int NPC_Lua_NLG_GetMap(lua_State *_NLL);						    //获得当前位置的建筑物
int NPC_Lua_NLG_PetUp(lua_State *_NLL);								  //宠物升级
int NPC_Lua_NLG_AddSk(lua_State *_NLL);								  //给职业技能
int NPC_Lua_NLG_SetZy(lua_State *_NLL);								  //设定职业
int NPC_Lua_Char_GetPlayerMaxNum(lua_State *_NLL);			//获取最大人数
int NPC_Lua_NLG_GetXY(lua_State *_NLL);							  	//获取一个地图的坐标
int NPC_Lua_NLG_WalkJc(lua_State *_NLL);							  //检查障碍物
int NPC_Lua_NLG_KickPlayer(lua_State *_NLL);				  	//T除指定玩家
int NPC_Lua_NLG_HealOne(lua_State *_NLL);				  	    //指定一个对象 回复满HP 满MP
int NPC_Lua_NLG_HealAll(lua_State *_NLL);				  	    //指定一个玩家MP 回复所有包括宠物HP
int NPC_Lua_NLG_LevelUpTo(lua_State *_NLL);				  	  //提升对象的等级。
int NPC_Lua_NLG_AddExp(lua_State *_NLL);			  	  	  //给与一个对象经验。
int NPC_Lua_NLG_UpPet(lua_State *_NLL);			  	  	    //更新对象的宠物，用于更新【客户端】宠物数据，在某些后台道具属性被修改的情况下不能把更新数据发送到前台；
int NPC_Lua_NLG_DelPetByPos(lua_State *_NLL);						//删除 对像 身上指定栏位的宠物
int NPC_Lua_NLG_DelPetByIndex(lua_State *_NLL);					//删除 对像 身上指定索引号（非ID）的宠物
int NPC_Lua_NLG_GivePetByIndex(lua_State *_NLL);				//给予一只指定索引号的宠物给指定 对像， 返回宠物栏位置
int NPC_Lua_NLG_GiveItemByIndex(lua_State *_NLL);				//给与一个道具index指定玩家 返回道具栏位
int NPC_Lua_NLG_WarpToSpecificPoint(lua_State *_NLL);	  //移动NPC 可做团体移动
BOOL NPC_Lua_NLG_UpSkillupPoint(lua_State *_NLL);       //更新人物点数
int NPC_Lua_NLG_StayEncount(lua_State *_NLL);	          //原地

int NPC_Lua_NLG_CheckInFront(lua_State *_NLL);					//用于检查某个对象是否在某个对象面前
int NPC_Lua_NLG_CheckObj(lua_State *_NLL);							//检查某个地图位置是否有对象存在
int NPC_Lua_NLG_CharLook(lua_State *_NLL);							//设置人物方向(发送更新封包)
int NPC_Lua_NLG_CreateBattle(lua_State *_NLL);					//创建战斗
int NPC_Lua_NLG_CreateVsEnemyAB (lua_State *L);
int NPC_Lua_NLG_InputCard(lua_State *_NLL);							//创建一个对话框让玩家输入虚拟卡号
int NPC_Lua_NLG_CreateBattlePvP(lua_State *_NLL);

int NPC_Lua_NLG_SearchWatchBattleRandIndex(lua_State *_NLL);

int NPC_Lua_NLG_DelPet(lua_State *_NLL);								//删除指定玩家宠物栏一个或多个宠物
int NPC_Lua_NLG_DelHaveIndexPet(lua_State *_NLL);
int NPC_Lua_NLG_DelItem(lua_State *_NLL);								//删除指定玩家道具栏一个或多个宠物
int NPC_Lua_NLG_DelItemByPos(lua_State *_NLL);								//删除指定玩家道具栏一个或多个宠物
int NPC_Lua_NLG_DischargeParty(lua_State *_NLL);				//解散 团队

int NPC_Lua_NLG_GivePet(lua_State *_NLL);								//给指定玩家一个或多个宠物
int NPC_Lua_NLG_GiveOnePet(lua_State *_NLL);								//给指定玩家一个宠物
int NPC_Lua_NLG_GiveItem(lua_State *_NLL);							//给指定玩家一个或多个宠物
int NPC_Lua_NLG_GiveOneItem(lua_State *_NLL);							//给指定玩家一个道具
int NPC_Lua_NLG_GiveRandItem(lua_State *_NLL);
int NPC_Lua_Char_GetOnLinePlayer(lua_State *_NLL);			//获取当前在线人数

int NPC_Lua_NLG_ShowWindowTalked(lua_State *_NLL);			//在指定玩家客户端显示指定内容、类型的对话框
int NPC_Lua_NLG_SetAction(lua_State *_NLL);							//设置对象的动作

int NPC_Lua_NLG_TalkToCli(lua_State *_NLL);							//普通说话，可以对全服务器人说
int NPC_Lua_NLG_TalkToFloor(lua_State *_NLL);						//对在某一指定地图内的玩家说话

int NPC_Lua_NLG_UpChar(lua_State *_NLL);								//向NPC周围的玩家发送NPC的数据更新封包
int NPC_Lua_NLG_UpStateBySecond(lua_State *_NLL);				//向指定玩家发送第2类状态更新封包
int NPC_Lua_NLG_UpStateByThird(lua_State *_NLL);				//向指定玩家发送第3类状态更新封包
int NPC_Lua_NLG_Update_Party(lua_State *_NLL);					//向玩家所在的团队成员发送玩家的状态更新封包

int NPC_Lua_NLG_Warp(lua_State *_NLL);									//传送一个对象
int NPC_Lua_NLG_WalkMove(lua_State *_NLL);							//移动一个对象
int NPC_Lua_NLG_WatchEntry(lua_State *_NLL);						//观战
int NPC_Lua_NLG_HealerAllHeal(lua_State *_NLL);         //加血
int NPC_Lua_NLG_GetMaxPlayNum(lua_State *_NLL);					//获取服务器最大在线数
int NPC_Lua_NLG_CheckPlayIndex(lua_State *_NLL);				//检查玩家索引是否正常
int NPC_Lua_NLG_Save(lua_State *_NLL);
////add by mo
int NPC_Lua_NLG_SetExp(lua_State *_NLL);							//设置服务器经验倍率
int NPC_Lua_NLG_GetExp(lua_State *_NLL);							//获取服务器经验倍率
int NPC_Lua_NLG_SetPetTransRange(lua_State *_NLL);				//设置宠物转生范围参数
int NPC_Lua_NLG_GetPetTransRange(lua_State *_NLL);				//获得宠物转生范围参数
///////////////////////////////////////////////Char////////////////////////////////////////////////
//设置数据的接口
int NPC_Lua_Char_ClrEvtEnd(lua_State *_NLL);						//设置清除结束任务标志
int NPC_Lua_Char_ClrEvtNow(lua_State *_NLL);						//设置清除正在做任务标志
int NPC_Lua_Char_ClrEvt(lua_State *_NLL);								//设置清除任务标志
int NPC_Lua_Char_GetAllocPoint(lua_State *_NLL);	      //获取宠物CharIndex的四维成长
int NPC_Lua_Char_GetDataFromEnemyBase(lua_State *_NLL);	//获取对应 标志 的宠物模板数据（enemybaseX.txt）；
int NPC_Lua_Char_GetPlayerIndex(lua_State *_NLL);       //玩家索引
int NPC_Lua_Char_FindItemIndex(lua_State *_NLL);				//搜索指定对像身上是否拥有某道具为指定的索引号
int NPC_Lua_Char_FindPetIndex(lua_State *_NLL);		      //搜索指定对像身上是否拥有宠物为指定的索引号
int NPC_Lua_CHAR_CHECKINDEX(lua_State *_NLL);				     //玩家索引
int NPC_Lua_Char_GetPetSkill(lua_State *_NLL);				  //取得宠物指定技能栏的技能编号
int NPC_Lua_Char_GetPetSkillNo(lua_State *_NLL);	      //获取Char数据
int NPC_Lua_Char_RidNo(lua_State *_NLL);								//设置清除任务标志
int NPC_Lua_Char_GetEmptyItemBoxNum(lua_State *_NLL);	  //搜索指定对像身上道具栏空位的数量
int NPC_Lua_Char_GetEmptyPetBoxNum(lua_State *_NLL);	  //搜索指定对像身上宠物栏空位的数量
int NPC_Lua_Char_GetPlayerItemNum(lua_State *_NLL);	    //搜索指定对像身上为某ID的道具的数量
int NPC_Lua_Char_GetPlayerPetNum(lua_State *_NLL);	    //搜索指定对像身上为某ID的宠物的数量
int NPC_ABLua_char_getPlayerMaxNum(lua_State *_NLL);    //玩家最大人数
int NPC_ABLua_char_getBattleexp(lua_State *_NLL);       //战斗经验倍数
int NPC_Lua_Char_SetPetSkill(lua_State *_NLL);				  //设置宠物指定技能栏的技能编号
int NPC_Lua_Char_SetPetSkillNo(lua_State *_NLL);	
int NPC_Lua_Char_ComplianceParameter(lua_State *_NLL);	//根据对象属性来重新计算并设置WORK属性，以达到生效的目的。
int NPC_Lua_Char_HcItemId(lua_State *_NLL);					  	//合成用到的搜索身上符合物品。
int NPC_Lua_Char_DelHcItem(lua_State *_NLL);					  //删除身上某个位置的物品。
int NPC_Lua_Char_GETFM(lua_State *_NLL);					      //获取家族气势 声望 资金
int NPC_Lua_Char_FindPetEnemyBaseId(lua_State *_NLL);		//搜索指定对象身上是否拥有宠物EnemyBase文件的对应ID
int NPC_Lua_Char_ITEMID_NAME(lua_State *_NLL);					//获得指定道具ID的名字
int NPC_Lua_Char_ITEMID_NAME2(lua_State *_NLL);					//获得指定道具ID的说明
int NPC_Lua_Char_ITEMID_LEVEL(lua_State *_NLL);					//获得指定道具ID的等级
int NPC_Lua_Char_ITEMID_GOLD(lua_State *_NLL);					//获得指定道具ID的价格
int NPC_Lua_Char_ITEMID_RANO(lua_State *_NLL);					//获得指定道具ID的图形
int NPC_Lua_Char_CharRidNo(lua_State *_NLL);					  //检查对像是否已经吃了某骑宠证；
int NPC_Lua_Char_GetPetOwner(lua_State *_NLL);				  //获得宠物主人的索引号
int NPC_Lua_Char_GetEnemyBaseIdFromEnemyId(lua_State *_NLL);	//通过enemy.txt上所设置的宠物编号来查找enemybase.txt上所设定的宠物编号。
int NPC_Lua_Char_GetEnemyIdFromEnemyBaseId(lua_State *_NLL);	//通过enemybase.txt上所设置的宠物编号来查找enemy.txt上所设定的宠物编号。
int NPC_Lua_Char_GetIp(lua_State *_NLL);	              //获取指定对像的IP地址
int NPC_Lua_Char_SetAllocPoint(lua_State *_NLL);	      //设置宠物CharIndex的四维成长
int NPC_Lua_Char_GetMaxPetIndex(lua_State *_NLL);	      //获取最大宠物索引
int NPC_Lua_Char_GetMaxPlayerIndex(lua_State *_NLL);	  //获取最大人物索引
int NPC_Lua_Char_SavePetToString(lua_State *_NLL);	    //生成一个描述宠物的字符串。
int NPC_Lua_Char_LoadPetFromString(lua_State *_NLL);	  //根据一个描述宠物的字符串，生成一个宠物。
int NPC_Lua_Char_GetPlayerFromAccAndName(lua_State *_NLL);//根据玩家的帐号和人物名来寻找玩家。帐号和人物名留空也可查找。不可都留空
int NPC_Lua_Char_CheckPet(lua_State *_NLL);             //检查宠物索引是否有效。
int NPC_Lua_Char_CheckPlayer(lua_State *_NLL);          //检查人物索引是否有效。
int NPC_Lua_Char_CheckItemIndex(lua_State *_NLL);           //检查道具索引是否有效。
int NPC_Lua_Char_GetfdFromCharaIndex(lua_State *_NLL);

BOOL NPC_Lua_Char_JoinParty(lua_State *_NLL);           //加入团队
int NPC_Lua_Char_getFamilyPlayIndex(lua_State *_NLL);   //获得家族里边人数的索引。

int NPC_Lua_Char_SetEvtEnd(lua_State *_NLL);						//设置任务标志为结束状态
int NPC_Lua_Char_SetEvtNow(lua_State *_NLL);						//设置任务标志为正在做状态
int NPC_Lua_Char_SetData(lua_State *_NLL);							//设置Char数据
int NPC_Lua_Char_logout(lua_State *_NLL);

int NPC_Lua_Char_getVipPoint(lua_State *_NLL);          //金币更新
int NPC_Lua_Char_getjfPoint(lua_State *_NLL);           //积分更新
int NPC_Lua_Char_getxjPoint(lua_State *_NLL);           //现金更新
int NPC_Lua_Char_setVipPoint(lua_State *_NLL);          //金币操作
int NPC_Lua_Char_setjfPoint(lua_State *_NLL);           //积分操作
int NPC_Lua_Char_setxjPoint(lua_State *_NLL);           //现金操作

int NPC_Lua_Char_GetVipPoint(lua_State *_NLL);
int NPC_Lua_Char_SetVipPoint(lua_State *_NLL);
int NPC_Lua_Char_GetAmPoint(lua_State *_NLL);
int NPC_Lua_Char_SetAmPoint(lua_State *_NLL);
int NPC_Lua_Char_GetRmbPoint(lua_State *_NLL);
int NPC_Lua_Char_SetRmbPoint(lua_State *_NLL);

//事件设置的接口
int NPC_Lua_Char_SetWalkPreEvent(lua_State *_NLL);			//设置WalkPre事件响应
int NPC_Lua_Char_SetWalkPostEvent(lua_State *_NLL);			//设置WalkPost事件响应
int NPC_Lua_Char_SetPreOverEvent(lua_State *_NLL);			//设置PreOver事件响应
int NPC_Lua_Char_SetPostOverEvent(lua_State *_NLL);			//设置PostOver事件响应
int NPC_Lua_Char_SetWatchEvent(lua_State *_NLL);				//设置Watch事件响应
int NPC_Lua_Char_SetLoopEvent(lua_State *_NLL);					//设置Loop事件响应
int NPC_Lua_Char_SetTalkedEvent(lua_State *_NLL);				//设置Talked事件响应
int NPC_Lua_Char_SetOFFEvent(lua_State *_NLL);					//设置OFF事件响应
int NPC_Lua_Char_SetLookedEvent(lua_State *_NLL);				//设置Looked事件响应
int NPC_Lua_Char_SetItemPutEvent(lua_State *_NLL);			//设置ItemPut事件响应
int NPC_Lua_Char_SetWindowTalkedEvent(lua_State *_NLL);	//设置WindowTalked事件响应
#ifdef _USER_CHARLOOPS
int NPC_Lua_Char_SetCharLoopsEvent(lua_State *_NLL);		//设置CharLoops事件响应
int NPC_Lua_Char_SetBattleProPertyEvent(lua_State *_NLL);//设置BattleProPerty事件响应
#endif

//获取数据的接口
int NPC_Lua_Char_IsEventEnd(lua_State *_NLL);						//检查是否结束的任务标志
int NPC_Lua_Char_IsEventNow(lua_State *_NLL);						//检查是否正在做的任务标志

int NPC_Lua_Char_FindItemId(lua_State *_NLL);						//搜索指定对象身上是否拥有某道具ID
int NPC_Lua_Char_FindPetEnemyId(lua_State *_NLL);				//搜索指定对象身上是否拥有宠物Enemy文件的对应ID

int NPC_Lua_Char_GetData(lua_State *_NLL);						//获取Char数据
int NPC_Lua_Char_GetItemId(lua_State *_NLL);						//获取指定对象身上指定位置的道具ID
int NPC_Lua_Char_GetPetEnemyId(lua_State *_NLL);				//获取指定对象身上指定位置的宠物Enemy文件的对应ID
int NPC_Lua_Char_GetItemIndex(lua_State *_NLL);					//获取指定对象身上指定位置的道具索引
int NPC_Lua_Char_GetPetIndex(lua_State *_NLL);					//获取指定对象身上指定位置的宠物对象索引
int NPC_Lua_Char_GetTeamIndex(lua_State *_NLL);					//获取队员的对象索引
int NPC_Lua_Char_GetPlayerIndexByCdkey(lua_State *_NLL);		//通过帐号查询人物索引
int NPC_Lua_Char_VipPoint(lua_State *_NLL);
int NPC_Lua_Char_HealAll(lua_State *_NLL);
int NPC_Lua_Char_GetPetSkillId(lua_State *_NLL);
int NPC_Lua_Char_GetPetSkillName(lua_State *_NLL);
int NPC_Lua_Char_GetPetSkillMsg(lua_State *_NLL);
int NPC_Lua_Char_SetPetSkill(lua_State *_NLL);
int NPC_Lua_Char_RandMsg(lua_State *_NLL);
int NPC_Lua_Char_GetBeatitudeNum(lua_State *_NLL);
///////////////////////////////////////////////GAME////////////////////////////////////////////////
int NPC_Lua_Game_FMPOINT_GetData(lua_State *_NLL);           //获得庄园据点的信息
int NPC_Lua_Game_FMPOINT_ACSetFMPoint(lua_State *_NLL);      //设定家族据点信息。仅当据点未被占据时有效。
int NPC_Lua_Game_FMPOINT_ACFixFMPoint(lua_State *_NLL);      //修改家族据点信息，并清空气势。一般用于族战结束。
int NPC_Lua_Game_FMPOINT_ACCleanFMPoint(lua_State *_NLL);    //无条件清除家族据点。
///////////////////////////////////////////////Item////////////////////////////////////////////////
//获取数据的接口
int NPC_Lua_Item_GetData(lua_State *_NLL);							//获取道具数据
int NPC_Lua_Item_GetDataFromItemSet(lua_State *_NLL);		//获取ITEMSETX文件的数据
int NPC_Lua_Item_GetValidity(lua_State *_NLL);		      //获取指定道具的过期时间
int NPC_Lua_Item_GetItemOwner(lua_State *_NLL);		      //获取指定道具主人的索引
int NPC_Lua_Item_GetMaxItemIndex(lua_State *_NLL);		  //获取最大道具索引
int NPC_Lua_Item_SaveItemToString(lua_State *_NLL);	    //生成一个描述物品的字符串。
int NPC_Lua_Item_SetValidity(lua_State *_NLL);		      //设置指定道具的过期时间。
int NPC_Lua_Item_LoadItemFromString(lua_State *_NLL);	  //根据一个描述物品的字符串，生成一个物品。

//设置数据的接口
int NPC_Lua_Item_SetData(lua_State *_NLL);							//获取道具数据

//事件设置的接口
int NPC_Lua_Item_SetPreOverEvent(lua_State *_NLL);			//设置PreOver事件响应
int NPC_Lua_Item_SetPostOverEvent(lua_State *_NLL);			//设置PostOver事件响应
int NPC_Lua_Item_SetWatchEvent(lua_State *_NLL);				//设置Watch事件响应
int NPC_Lua_Item_SetUseEvent(lua_State *_NLL);					//设置Use事件响应
int NPC_Lua_Item_SetAttachEvent(lua_State *_NLL);				//设置Attach事件响应
int NPC_Lua_Item_SetDetachEvent(lua_State *_NLL);				//设置Detach事件响应
int NPC_Lua_Item_SetDropEvent(lua_State *_NLL);					//设置Drop事件响应
int NPC_Lua_Item_SetPickUPEvent(lua_State *_NLL);				//设置PickUP事件响应
#ifdef _Item_ReLifeAct
int NPC_Lua_Item_SetDieReLifeEvent(lua_State *_NLL);		//设置DieReLife事件响应
#endif
///////////////////////////////////////////////BIT///////////////////////////////////////////////
int NPC_Lua_Bit_band(lua_State *_NLL);
int NPC_Lua_Bit_bor(lua_State *_NLL);
int NPC_Lua_Bit_bxor(lua_State *_NLL);
int NPC_Lua_Bit_lshift(lua_State *_NLL);
int NPC_Lua_Bit_rshift(lua_State *_NLL);
///////////////////////////////////////////////Obj////////////////////////////////////////////////
//获取数据的接口
int NPC_Lua_Obj_GetType(lua_State *_NLL);								//获取OBJ的类型
/*
int NPC_Lua_Obj_GetName(lua_State *_NLL);								//获取OBJ的名称
int NPC_Lua_Obj_GetDIR(lua_State *_NLL);								//获取OBJ的方向
int NPC_Lua_Obj_GetImageNum(lua_State *_NLL);						//获取OBJ的形象号
*/
int NPC_Lua_Obj_GetCharType(lua_State *_NLL);						//获取OBJ对应的Char数据结构的类型
int NPC_Lua_Obj_GetCharIndex(lua_State *_NLL);					//获取OBJ对应的Char数据结构的索引
int NPC_Lua_Obj_GetX(lua_State *_NLL);									//获取OBJ的X坐标
int NPC_Lua_Obj_GetY(lua_State *_NLL);									//获取OBJ的Y坐标
int NPC_Lua_Obj_GetFloor(lua_State *_NLL);							//获取OBJ所在的地图编号
//int NPC_Lua_Obj_GetDelTime(lua_State *_NLL);						//获取OBJ的删除时间

int NPC_Lua_Obj_SetType(lua_State *_NLL);								//获取OBJ的类型
/*
int NPC_Lua_Obj_SetName(lua_State *_NLL);								//获取OBJ的名称
int NPC_Lua_Obj_SetDIR(lua_State *_NLL);								//获取OBJ的方向
int NPC_Lua_Obj_SetImageNum(lua_State *_NLL);						//获取OBJ的形象号
*/
int NPC_Lua_Obj_SetCharType(lua_State *_NLL);						//获取OBJ对应的Char数据结构的类型
int NPC_Lua_Obj_SetX(lua_State *_NLL);									//获取OBJ的X坐标
int NPC_Lua_Obj_SetY(lua_State *_NLL);									//获取OBJ的Y坐标
int NPC_Lua_Obj_SetFloor(lua_State *_NLL);							//获取OBJ所在的地图编号
//int NPC_Lua_Obj_SetDelTime(lua_State *_NLL);						//获取OBJ的删除时间

//////////////////////////////////////////////Battle///////////////////////////////////////////////
//获取数据的接口
int NPC_Lua_Battle_GetPlayIndex(lua_State *_NLL);				//获取战斗中的玩家索引
int NPC_Lua_Battle_GetData(lua_State *_NLL);				    //获取战斗数据
int NPC_Lua_Battle_SetData(lua_State *_NLL);				    //设置战斗数据
int NPC_Lua_Battle_IndexToNo(lua_State *_NLL);				  //通过对象索引获取所在的战场编号
int NPC_Lua_Battle_NoToIndex(lua_State *_NLL);				  //通过战场编号获取他的索引
int NPC_Lua_Battle_CheckIndex(lua_State *_NLL);				  //检查战斗索引是否有效。
//设置数据的接口
int NPC_Lua_Battle_SetNORisk(lua_State *_NLL);					//设置是否开启无风险模式
int NPC_Lua_Battle_SetMod(lua_State *_NLL);							//设置战斗模式标志
int NPC_Lua_Battle_SetType(lua_State *_NLL);						//设置战斗类型
int NPC_Lua_Battle_Exit(lua_State *_NLL);				        //退出战斗
//事件设置的接口
int NPC_Lua_Battle_SetWinEvent(lua_State *_NLL);				//设置Win事件响应
int NPC_Lua_Battle_SetEndEvent(lua_State *_NLL);				//设置Win事件响应
///////////////////////////////////////////////Other///////////////////////////////////////////////
int NPC_Lua_GetFuncPoint(lua_State *_NLL);							//获取函数指针

//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////MAP////////////////////////////////////////////////
int NPC_Lua_Map_CheckCoordinates(lua_State *_NLL);         //检测某点是否在地图范围内。
int NPC_Lua_Map_GetExitFloorXY(lua_State *_NLL);           //检测地图是否支持登出
int NPC_Lua_Map_GetfloorX(lua_State *_NLL);                //获取地图X长度
int NPC_Lua_Map_GetfloorY(lua_State *_NLL);                 //获取地图Y长度
int NPC_Lua_Map_GetTileAndObjId(lua_State *_NLL);           //获取地图某点的地板和装饰层
int NPC_Lua_Map_SetTileAndObjId(lua_State *_NLL);           //设置地图某点的地板和装饰层
int NPC_Lua_Map_GetWalkAbleFromPoint(lua_State *_NLL);      //获取某点是否可以行走
int NPC_Lua_Map_GetImageData(lua_State *_NLL);              //获取图片数据
int NPC_Lua_Map_SetImageData(lua_State *_NLL);              //设置图片数据
int NPC_Lua_Map_GetTopObj(lua_State *_NLL);                 //用来获取地图某点上对象
int NPC_Lua_Map_GetNextObj(lua_State *_NLL);                //获取下一个对象索引
int NPC_Lua_Map_CheckImageIndex(lua_State *_NLL);           //检测某地图号图片是否存在
int NPC_Lua_Map_CheckIndex(lua_State *_NLL);                //检测某地图号地图是否存在
int NPC_Lua_Map_MakeNewMap(lua_State *_NLL);                //制造一个新的地图 并返回地图的ID
int NPC_Lua_Map_DelNewMap(lua_State *_NLL);                 //删除一个副本地图
int NPC_Lua_Map_SetExWarp(lua_State *_NLL);                 //设置人物登出后地图的传送点
int NPC_Lua_Map_SetMapPoint(lua_State *_NLL);               //添加地图传送点
int NPC_Lua_Map_DelMapPoint(lua_State *_NLL);               //删除地图传送点
int NPC_Lua_Map_getFloorName(lua_State *_NLL);               //获得地图名
int NPC_Lua_Map_Upmap(lua_State *_NLL);                      //更新地图
///////////////////////////////////////////////MAP////////////////////////////////////////////////
///////////////////////////////////////////////Spell////////////////////////////////////////////////
int NPC_Lua_Spell_PETSKILL_GetData(lua_State *_NLL);    //获取宠物技能ID
int NPC_Lua_Spell_PROFESSION_GetData(lua_State *_NLL);  //获取职业技能ID
int NPC_Lua_Spell_MAGIC_GetData(lua_State *_NLL);       //获取精灵技能ID
int NPC_Lua_Spell_PETSKILL_SetData(lua_State *_NLL);    //设置对应ID的技能数据；
int NPC_Lua_Spell_PROFESSION_SetData(lua_State *_NLL);  //设置对应ID的技能数据；
int NPC_Lua_Spell_MAGIC_SetData(lua_State *_NLL);       //设置对应ID的技能数据；
///////////////////////////////////////////////Spell////////////////////////////////////////////////
///////////////////////////////////////////////SQL/////////////////////////////////////////////////
int NPC_Lua_SQL_Push(lua_State *_NLL);						//用于推送MySQL指令
int NPC_Lua_SQL_PushPop(lua_State *_NLL);			//用于推送MySQL指令并设置响应函数
int NPC_Lua_SQL_PushPopAdv(lua_State *_NLL);            //向SAAC发出一个SQL查询请求，并指定当SAAC返回处理结果时对结果进行处理的回调函数,该函数将返回第一行结果，并释放结果集。
int NPC_Lua_SQL_QueryFirstRow(lua_State *_NLL);         //回调函数,该函数将返回第一行结果，并释放结果集。
int NPC_Lua_SQL_FetchRow(lua_State *_NLL);              //下一条结果
int NPC_Lua_SQL_FreeResult(lua_State *_NLL);            //释放结果集
///////////////////////////////////////////////SQL///////////////////////////////////////////////
#define LRet(r) \
{ \
	NPC_Lua_SetErrorStr(NULL); \
	return r; \
}

#define LRetErr(c,r) \
{ \
	NPC_Lua_SetErrorStr(c); \
	return r; \
}

//////////////////////////////////////////////////////////////////////
//返回一个nil
#define LRetNull(L) \
{ \
	lua_pushnil(L); \
	LRet(1); \
}

//返回一个BOOL给LUA引擎
#define LRetBool(L, b) \
{ \
	lua_pushboolean(L, b); \
	LRet(1); \
}

//返回一个int给LUA引擎
#define LRetInt(L, i) \
{ \
	lua_pushinteger(L, i); \
	LRet(1); \
}

#define LRetMsg(L, c) \
{ \
	lua_pushstring(L, c); \
	LRet(1); \
}
////////////////////////////////////////////////////////////////////
//设置一个错误状态和一个错信息
#define LRetErrInt(L, i, c) \
{ \
	lua_pushinteger(L, i); \
	LRetErr(c,1); \
}

//返回一个字符串给LUA引擎
#define LRetErrNull(L, c) \
{ \
	lua_pushnil(L); \
	LRetErr(c,1); \
}

//返回一个字符串给LUA引擎
#define LRetErrMsg(L, c) \
{ \
	lua_pushstring(L, c); \
	LRetErr(c,1); \
}

//用于检查参数是否足够
#define CheckEx(L, n) \
{	\
	if(lua_gettop(L) != n) \
	{ \
		luaL_argerror(L, 1, "参数数量错误"); \
	} \
}

#define CheckEx2(L, t, n) \
{	\
	if(lua_gettop(L) < (t) || lua_gettop(L) > n) \
	{ \
		luaL_argerror(L, 1, "参数数量错误"); \
	} \
}

#define CheckIndexNull(L, n) \
{ \
	if(lua_isnil((L), (n))) \
	{ \
		luaL_argerror((L), 1, "对象索引不能为nil"); \
	} \
}

#define CheckBattleIndexNull(L, n) \
{ \
	if(lua_isnil((L), (n))) \
	{ \
		luaL_argerror((L), 1, "战斗索引不能为nil"); \
	} \
}

#define CheckItemIndexNull(L, n) \
{ \
	if(lua_isnil((L), (n))) \
	{ \
		luaL_argerror((L), 1, "道具索引不能为nil"); \
	} \
}

#define CheckObjIndexNull(L, n) \
{ \
	if(lua_isnil((L), (n))) \
	{ \
		luaL_argerror((L), 1, "物件索引不能为nil"); \
	} \
}

#define LRetArray(L, Array, num) \
{ \
	Lua_ReturnArray(L, Array, num); \
	LRet(1); \
}

#ifdef _LUA_Debug
int pcall_callback_err_fun(lua_State* L,const char *_InitFuncName); //调试LUA脚本出错信息
#endif

#endif //#ifndef __NPC_LUA__
