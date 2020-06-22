#ifndef __ALLBLUES_H__
#define __ALLBLUES_H__

#include "longzoro.h"


//私服版本控制----------------------------------------

#if _ATTESTAION_ID == 1
	#define _VERSION_NEW
	#define _VERSION_NEW60
//#define _VERSION_GF
#define _VERSION_60
#define _VERSION_80
//#define _NEW_SA80                // 新石器8.0
#else
#if _ATTESTAION_ID == 2
	#define _VERSION_25
#else
	#define _VERSION_80
#endif
#endif


#ifdef _VERSION_80
	#if _ATTESTAION_ID == 0 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 8 || _ATTESTAION_ID == 21 || _ATTESTAION_ID == 23 || _ATTESTAION_ID == 28 || _ATTESTAION_ID == 29 || _ATTESTAION_ID == 30 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 32 || _ATTESTAION_ID == 33 || _ATTESTAION_ID == 34 || _ATTESTAION_ID == 35 || _ATTESTAION_ID == 36 || _ATTESTAION_ID == 37 || _ATTESTAION_ID >= 39
		#if  _ATTESTAION_ID != 49
			#define _NEW_SA80                // 新石器8.0
		#endif
	#endif
	
	#ifdef _NEW_SA80
		#if _ATTESTAION_ID != 43
			#define _ONLINE_SHOP             // 8.0在线商城
		#endif
		#define _ONLINE_TALK_IP          // 玩家举报
		#define _ONLINE_TALK_GM          // 在线联系GM
		#define _NEW_ONLINE_SHOP         // 新在线商城
	#endif
	#define _ANGLE_EMAIL             // 精召后自动交换名片
#endif

#ifdef _VERSION_NEW
	#define _CHATROOMPROTOCOL        // (可开放) Syu ADD 聊天室频道 8/27
	#define _OBJSEND_C               // (可开放) ANDY _OBJSEND_C
	#define _ADD_SHOW_ITEMDAMAGE     // (可开放) WON  显示物品耐久度
	#define _CHANGETRADERULE         // (可开放) Syu ADD 交易规则修订
	#define _TRADE_SHOWHP            // (可开放) Syu ADD 交易显示血量
	#define _TRADESYSTEM2            // (可开放) Syu ADD 新交易系统
	#define _NPC_MAKEPAIR            // (可开放) ANDY NPC 配对
	#define _ITEM_PILENUMS           // (可开放) ANDY 道具素材堆叠 需 itemset4
	#define _ITEM_PILEFORTRADE       // (可开放) ANDY 交易堆叠
	#define _ITEM_EQUITSPACE         // (可开放) ANDY 新装备栏位
	#define _EQUIT_ARRANGE           // (可开放) ANDY 挡格 需要 itemset5.txt
	#define _EQUIT_ADDPILE           // (可开放) ANDY 增加可堆叠数 需要 itemset5.txt
	#define _EQUIT_NEWGLOVE          // (可开放) ANDY 手套栏位
	// 新功能：职业
	#define _NEWREQUESTPROTOCOL      // (可开放) Syu ADD 新增Protocol要求细项
	#define _OUTOFBATTLESKILL        // (可开放) Syu ADD 非战斗时技能Protocol
	#define _NPC_ADDLEVELUP          // (可开放) ANDY NPC增加玩家等级
	#define _CHAR_NEWLOGOUT          // (可开放) ANDY 登出新机制
	#define _NPC_ProfessionTrans     // (可开) Change 替职业NPC加入判断转生
	#define _CHANNEL_MODIFY          // 频道功能整合(开放)
	//--------------------------------------------------------------------------
	#define _STREET_VENDOR           // 摆摊功能(开放)
	#define _WATCH_EVENT             // 查询任务功能(开放)
	#define _ADD_DUNGEON             // (可开) Change 追加地牢
	#define _ADD_STATUS_2            // 新增传送人物状态第二类
	#define  _ALCHEMIST              // Robin 精  功能
	#define _JOBDAILY                // cyg 任务日志功能
	#define _FONT_SIZE               // (可开) Robin 字型大小功能
	#define _PETSKILL_FIXITEM        // (可开放) ANDY 修复 && 同道具损坏
	#define _CHECK_ITEMDAMAGE        // (可开放) 道具损坏修正  7/26
	#define _TEAM_KICKPARTY          // (可开放) ANDY 队长踢人(非战斗状态)
	#define _TELLCHANNEL             // (可开放) Syu ADD 密语频道
	#define _EMENY_CHANCEMAN         // (可开放) ANDY EMENY 选择
	#define _STANDBYPET              // Robin 待机宠封包
	#define _CHAR_POOLITEM           // (可开放) ANDY 人物道具仓库共用
	#define _CHAR_POOLPET            // (可开放) Robin 人物宠物仓库共用
	#define _NPC_DEPOTPET            // (可开放) Robin 人物宠物仓库共用
	#define _NPC_DEPOTITEM           // (可开放) ANDY 人物道具仓库共用
	#define _NEW_MANOR_LAW           // 新庄园规则(开放)
	#define _CHANNEL_MODIFY          // 频道功能整合(开放)
	#define _TAKE_ITEMDAMAGE         // (可开放) ANDY 道具损坏  ITEMSET2
#ifdef _VERSION_NEW60
	#define _SKILL_NOCAST            // (可开放) vincent  宠技:沉默 //需要开#define _MAGIC_NOCAST
	#define _PET_FUSION              // (可开放) ANDY 宠物融合 enemybase1.txt
	#define _NPC_FUSION              // (可开放) ANDY NPC宠物融合
	#define _ITEM_EDITBASES          // (可开放) ANDY 营养剂
	#define _PET_EVOLUTION           // (可开放) ANDY 进化表
	#define _STATUS_WATERWORD        // (可开放) ANDY 水世界状态
	#define _ITEM_WATERWORDSTATUS    // (可开放) ANDY 水世界状态道具
	#define _PETSKILL_ACUPUNCTURE    // (可开放) Change 宠技:针刺外皮 (client端的_PETSKILL_ACUPUNCTURE也要开)
	#define _SHOW_FUSION             // (可开) Change 融合宠物在CLIENT端只显示转生数,修改为融合宠显示为融合(client端也要开)
	#define _TREASURE_BOX            // (可开放) ANDY 宝箱
	#define _SHOOTCHESTNUT           // (可开放) Syu ADD 宠技：丢栗子
#endif

#endif

#ifdef _VERSION_60
	#define _PETSKILL_FIXITEM        // (可开放) ANDY 修复 && 同道具损坏
	#define _CHECK_ITEMDAMAGE        // (可开放) 道具损坏修正  7/26
	#define _TAKE_ITEMDAMAGE         // (可开放) ANDY 道具损坏  ITEMSET2
	#define _SKILL_NOCAST            // (可开放) vincent  宠技:沉默 //需要开#define _MAGIC_NOCAST
	#define _EMENY_CHANCEMAN         // (可开放) ANDY EMENY 选择
	#define _PET_FUSION              // (可开放) ANDY 宠物融合 enemybase1.txt
	#define _NPC_FUSION              // (可开放) ANDY NPC宠物融合
	#define _ITEM_EDITBASES          // (可开放) ANDY 营养剂
	#define _PET_EVOLUTION           // (可开放) ANDY 进化表
	#define _TEAM_KICKPARTY          // (可开放) ANDY 队长踢人(非战斗状态)
	#define _STATUS_WATERWORD        // (可开放) ANDY 水世界状态
	#define _ITEM_WATERWORDSTATUS    // (可开放) ANDY 水世界状态道具
	#define _TELLCHANNEL             // (可开放) Syu ADD 密语频道
	#define _PETSKILL_ACUPUNCTURE    // (可开放) Change 宠技:针刺外皮 (client端的_PETSKILL_ACUPUNCTURE也要开)
	#define _SHOW_FUSION             // (可开) Change 融合宠物在CLIENT端只显示转生数,修改为融合宠显示为融合(client端也要开)
	#define _FONT_SIZE               // (可开) Robin 字型大小功能
	#define _TREASURE_BOX            // (可开放) ANDY 宝箱
	#define _DP_NPC                  // DP脚本
	#define _SHOOTCHESTNUT           // (可开放) Syu ADD 宠技：丢栗子
#endif

#ifdef _VERSION_80
	#define _PETSKILL_FIXITEM        // (可开放) ANDY 修复 && 同道具损坏
	#define _CHECK_ITEMDAMAGE        // (可开放) 道具损坏修正  7/26
	#define _TAKE_ITEMDAMAGE         // (可开放) ANDY 道具损坏  ITEMSET2
	#define _SKILL_NOCAST            // (可开放) vincent  宠技:沉默 //需要开#define _MAGIC_NOCAST
	#define _EMENY_CHANCEMAN         // (可开放) ANDY EMENY 选择
	#define _PET_FUSION              // (可开放) ANDY 宠物融合 enemybase1.txt
	#define _NPC_FUSION              // (可开放) ANDY NPC宠物融合
	#define _ITEM_EDITBASES          // (可开放) ANDY 营养剂
	#define _PET_EVOLUTION           // (可开放) ANDY 进化表
	#define _TEAM_KICKPARTY          // (可开放) ANDY 队长踢人(非战斗状态)
	#define _STATUS_WATERWORD        // (可开放) ANDY 水世界状态
	#define _ITEM_WATERWORDSTATUS    // (可开放) ANDY 水世界状态道具
	#define _TELLCHANNEL             // (可开放) Syu ADD 密语频道
	#define _PETSKILL_ACUPUNCTURE    // (可开放) Change 宠技:针刺外皮 (client端的_PETSKILL_ACUPUNCTURE也要开)
	#define _SHOW_FUSION             // (可开) Change 融合宠物在CLIENT端只显示转生数,修改为融合宠显示为融合(client端也要开)
	#define _FONT_SIZE               // (可开) Robin 字型大小功能
	#define _TREASURE_BOX            // (可开放) ANDY 宝箱
	#define _DP_NPC                  // DP脚本
	
	//#define _PREVENT_TEAMATTACK      // (可开放) ANDY 防止同队互打 12/04 更新
	#define _OBJSEND_C               // (可开放) ANDY _OBJSEND_C
	#define _ADD_SHOW_ITEMDAMAGE     // (可开放) WON  显示物品耐久度
	#define _CHANGETRADERULE         // (可开放) Syu ADD 交易规则修订
	#define _TRADE_SHOWHP            // (可开放) Syu ADD 交易显示血量
	#define _SHOOTCHESTNUT           // (可开放) Syu ADD 宠技：丢栗子
	#define _SKILLLIMIT              // (可开放) Syu ADD 不得攻击我方限制
	#define _TRADESYSTEM2            // (可开放) Syu ADD 新交易系统
	#define _NPC_MAKEPAIR            // (可开放) ANDY NPC 配对
	#define _ITEM_PILENUMS           // (可开放) ANDY 道具素材堆叠 需 itemset4
	#define _ITEM_PILEFORTRADE       // (可开放) ANDY 交易堆叠
	#define _ITEM_EQUITSPACE         // (可开放) ANDY 新装备栏位
	#define _EQUIT_ARRANGE           // (可开放) ANDY 挡格 需要 itemset5.txt
	#define _EQUIT_ADDPILE           // (可开放) ANDY 增加可堆叠数 需要 itemset5.txt
	#define _EQUIT_NEWGLOVE          // (可开放) ANDY 手套栏位
	// 新功能：职业
	#define _NEWREQUESTPROTOCOL      // (可开放) Syu ADD 新增Protocol要求细项
	#define _OUTOFBATTLESKILL        // (可开放) Syu ADD 非战斗时技能Protocol
	#define _CHAR_PROFESSION         // (可开放) WON ADD 人物职业栏位
	#define _PROFESSION_SKILL        // (可开放) WON ADD 人物职业技能
	#define _NPC_WELFARE             // (可开放) WON ADD 职业NPC
	#define _NPC_ADDLEVELUP          // (可开放) ANDY NPC增加玩家等级
	#define _CHAR_NEWLOGOUT          // (可开放) ANDY 登出新机制
	#define _CHATROOMPROTOCOL        // (可开放) Syu ADD 聊天室频道 8/27
	#define _CHAR_POOLITEM           // (可开放) ANDY 人物道具仓库共用
	#define _CHAR_POOLPET            // (可开放) Robin 人物宠物仓库共用
	#define _NPC_DEPOTPET            // (可开放) Robin 人物宠物仓库共用
	#define _NPC_DEPOTITEM           // (可开放) ANDY 人物道具仓库共用
	#define _NPC_ProfessionTrans     // (可开) Change 替职业NPC加入判断转生
	#define _CHANNEL_MODIFY          // 频道功能整合(开放)
	#define _STANDBYPET              // Robin 待机宠封包
	#define  _PROSKILL_OPTIMUM       // Robin 职业技能处理最佳化
	//2004/05/12
	
	#define _MAGIC_RESIST_EQUIT      // (可开) WON ADD 职业抗性装备
	#define _ALLDOMAN                // (可开放) Syu ADD 排行榜NPC
	#define _ALLDOMAN_DEBUG          // (可开) Change 修正重启server後排行榜消失bug
	// -------------------------------------------------------------------
	// 新功能：GM密频功能扩充
	//复刻版
	#define _REDHAT_V9               //(可开) Change 在REDHAT9.0的版本下编译时打开
	//--------------------------------------------------------------------------
	#define _NPC_EXCHANGEMANTRANS    // (可开) Change 替exchangeman加入判断转生
	#define  _ANGEL_SUMMON           // Robin 天使召唤
	#define _STREET_VENDOR           // 摆摊功能(开放)
	#define _WATCH_EVENT             // 查询任务功能(开放)
	#define _NEW_MANOR_LAW           // 新庄园规则(开放)
	#define _PETSKILL_LER            // 雷尔技能(开放)
	#define _FIX_MAGIC_RESIST        // (可开) Change 职业魔法抗性
	#define _ADD_DUNGEON             // (可开) Change 追加地牢
	#define _PROFESSION_ADDSKILL     // (可开) Change 追加职业技能
	#define _EQUIT_RESIST            // (可开) Change 单一装备抗性(可抗单一功能异常状态)有抗火 雷 冰 虚弱 魔障 沉默 落马
	#define _PROSK99                 // (可开) Change 检查职技等级送白狼
	#define _ADD_STATUS_2            // 新增传送人物状态第二类
	#define _SUIT_ADDPART4           // (可开) Change 套装功能第四弹 中毒攻击% 加强法师魔法(机率30%) 抵抗法师的魔法


	#define _ITEM_TYPETABLE          // (可开) Change 追加typetable
	#define _RIGHTCLICK              // Robin 滑鼠右键功能
	#define _JOBDAILY                // cyg 任务日志功能
	#define  _ALCHEMIST              // Robin 精  功能
	#define _TEACHER_SYSTEM          // 导师系统(开放)
	#define _PET_ITEM                // 宠物装备
#endif


//普通版新增功能-------------------------------------
#define _PUB_ALL
#ifdef _PUB_ALL
	//#define _PROSKILL_ERR_KICK       // 非法技能自动T下线
	#define _ITEM_SETLOVER           // 结婚功能
	#define _CREATE_MM_1_2           // 制作极品玛蕾菲雅
	#define _SendTo                  // 传送到玩家身边
	#define _GET_BATTLE_EXP          // 经验设置
	#define _NEW_PLAYER_CF           // 新手状态设置
	#define _USER_EXP_CF             // 用户自定义经验
	#define _TRANS_LEVEL_CF          // 转生超越160级设置
	#define _UNLAW_WARP_FLOOR        // 禁止传送地区
	#define _UNREG_NEMA              // 禁止人物名称
	#if _ATTESTAION_ID != 1
		#define _GM_ITEM                 // 玩家GM命令物品
	#endif
	#define _GM_METAMO_RIDE          // GM人物骑所有宠
	#ifdef _NEW_RIDEPETS
		#define _GM_RIDE                 // GM命令设置骑宠权限
	#endif
	#define _LOCK_IP                 // 锁定IP
	#define _POINT                   // 点数上限
	#define _VIP_SERVER              // 会员服务
	#define _WATCH_FLOOR             // 自定义可观战地图
	#define _BATTLE_FLOOR            // 自定义强制战斗地图
	#define _VIP_SHOP                // 会员商店
	#if _ATTESTAION_ID != 1
		#define _LOOP_ANNOUNCE           // 循环公告
	#endif
	#define _SKILLUPPOINT_CF         // 自定义升级点数
	#define _RIDELEVEL               // 自定义骑宠相差等级
	#define _REVLEVEL                // 自动还原等级
	#define _FM_ITEM                 // 声望道具
	#define _LUCK_ITEM               // 运气道具
	#define _NEW_PLAYER_RIDE         // 新手送配套骑宠
	#define _FIX_CHARLOOPS           // 修正恶魔宝石遇敌时间
	#if _ATTESTAION_ID != 1
		#define _PLAYER_ANNOUNCE         // 玩家喇叭
	#endif
	#define _BATTLE_GOLD             // 获取经验同时获得金钱
	#define _ITEM_GOLD               // 金钱大物品
	#define _PLAYER_NUM              // 玩家在线人数
	#define _PET_MM                  // 获得满石MM
	#define _MYSTERIOUS_GIFT         // 神秘礼物
	#define _RELOAD_CF               // 重读CF
	#define _PET_AND_ITEM_UP         // 可捡别人丢出的宠物或道具
	#define _TRANS                   // 人物转生命令
	#ifdef _NEW_RIDEPETS
		#define _RIDEMODE_20             // 2.0骑宠模式
	#endif
	#define _TALK_ACTION             // 说话动作
	#define _ENEMY_ACTION            // 玩家步行遇敌机率
	#define _CHAR_CDKEY              // 防止串号问题
//	#define _GMSV_DEBUG              // GMSV调试信息
	#define _CHECK_PEPEAT            // 非法宠物检查
	#define _SILENTZERO              // 黄莲物品
	#define _SAVE_GAME_ID            // 显示保存的服务器名
#endif

//会员版追加功能--------------------------------------
#define _VIP_ALL
#ifdef _VIP_ALL
	#define _NEW_RIDEPETS            // (可开放) ANDY 新骑宠
	#define _RIDEMODE_20             // 2.0骑宠模式
	#define _VIP_RIDE                // 会员骑黑鸡
	#define _AUTO_PK                 // 自动化PK系统
	//#define _FM_METAMO               // 家族专用变身戒指
	#define _ITEM_METAMO_TIME        // 可自定义变身的戒指
	#define _SHOW_VIP_CF             // 自定义调整VIP，爱人，家族，名称显示样式
	#define _GM_KING                 // GM大魔王
	#define _ANGEL_TIME              // 自定义精灵召唤时间
	#define _FM_POINT_PK             // 庄园家族互抢庄园
	#if _ATTESTAION_ID != 1
		#define _PLAYER_MOVE             // 玩家顺移功能
	#endif
	#define _BATTLE_PK               // 强制PK道具，输了回记录点
	#ifdef _PET_FUSION
		#define _FUSIONBEIT_TRANS        // 融合宠物转生
		#define _FUSIONBEIT_FIX          // 取消融合宠的限制
	#endif
	#define _TRANS_7_NPC             // 人物七转NPC
	#ifdef _NEW_MANOR_LAW
		#define _MOMENTUM_NPC            // 气势脚本
	#endif
	
	#ifdef _NEW_RIDEPETS
		#define _RIDE_CF                 // 自定义骑宠
		#define _FM_LEADER_RIDE          // 族长专用骑宠
	#endif
	#define _PET_LEVEL_ITEM          // 宠物等级提升物品
	//#define _DBSAVE_CHAR             // 双向存档
	#define _FM_JOINLIMIT            // 退族后再次加入家族的时间限制
	#define _PET_BEATITUDE           // 宠物祝福
	#define _ITEM_EFMETAMO           // 永久变身戒指
	#define _ITEM_RIDE               // 驯骑装备
	#define _MAKE_MAP                // 地图制作
	#define _MAKE_PET_CF             // 自定义宠物成长
	#define _MAKE_PET_ABILITY        // 自定义宠物四围
	#define _GET_MULTI_ITEM          // 一次给多件物品
	#define _RE_GM_COMMAND           // 自定义GM命令设置
	#define _ONLINE_TIME           // 在线时间
	#define _NPC_INT_SET           // NPC整型设置
	#define _VIP_ONLINE              // 会员上线提示

#endif

#if _ATTESTAION_ID != 9 && _ATTESTAION_ID != 11
	#define _LOGNZORO_FUNC_ALL       // 龙zoro新增功能
#else
	#define _NEW_VIP_SHOP            // 会员商店(需SAAC配合)
	#define _ONLINE_COST             // 在线充值(需SAAC配合)
	#define _NULL_CHECK_ITEM         // 空头支票道具
	#define _COST_ITEM               // 充值道具
	#define _CRYPTO_DATA             // 服务端数据加密系统
#endif
#ifdef _LOGNZORO_FUNC_ALL
	#if _ATTESTAION_ID != 1
		#define _TRANS_7_SHOW            // 人物7转显示
	#endif
	#define _ITEM_COLOER             // 物品颜色调配
	#define _TALK_MOVE_FLOOR         // 说话移动功能
	#ifdef _TRADESYSTEM2
		#define _TRADE_PK                // 交易PK
	#endif
	#if _ATTESTAION_ID != 1
		#define _VIP_POINT_PK            // 积分点PK
	#endif
	#define _MAP_HEALERALLHEAL       // 指定地图战斗后自动回复体力
	#if _ATTESTAION_ID != 1
		#define _NEW_AUTO_PK             // 新的自动化PK系统
	#endif
	#define _OPEN_E_PETSKILL         // 开放宠物E技能
	#if _ATTESTAION_ID != 1
		#define _VIP_ONLINE              // 会员上线提示
	#endif
	#define _VIP_LOGOUT              // 会员最后离线时间提示
	#if _ATTESTAION_ID != 1
		#define _THE_WORLD_SEND          // 世界说话频道
	#endif
#define _THE_WORLD_SEND          // 世界说话频道
	#define _SUPER_FLOOR_MIC         // 地图麦克风
	#define _NPC_GET_PETABI          // NPC给自定义成长的宠物
	#define _SAMETHING_SAVEPOINT     // 玩家某些事件自动存档
	#define _NEW_VIP_SHOP            // 会员商店(需SAAC配合)
	#if _ATTESTAION_ID != 8 && _ATTESTAION_ID != 10 && _ATTESTAION_ID != 21 
		#define _ITEM_PET_LOCKED         // 物品宠物锁(需SAAC配合)
	#endif
	#define _ONLINE_COST             // 在线充值(需SAAC配合)
	#if _ATTESTAION_ID != 3
		#define _VIPPOINT_OLD_TO_NEW     // 积分点旧转新(需SAAC配合)
	#endif
	#define _LOGIN_DISPLAY           // 玩家上线提示开关
	//#define _NEED_ITEM_ENEMY         // 抓宠所需物品自定义
	#define _SUPER_MAN_ITEM          // 极品人道具
	#define _AUTO_DEL_PET            // 自动删除宠物
	#define _AUTO_DEL_ITEM           // 自动删除物品
	#define _POOL_ITEM_BUG           // 禁止强存的BUG
	
	#if _ATTESTAION_ID != 5
		#if _ATTESTAION_ID != 1
			#define _BOUND_TIME              // 玩家包时功能
		#endif
		#define _SPECIAL_MAP             // 特殊地图
		#ifdef _FONT_SIZE
			#define _TALK_SIZE_ITEM          // 大声公道具
		#endif
		#define _NEW_NAME                // 自定义称号
		#define _CRYPTO_DATA             // 服务端数据加密系统
		#ifdef _NEW_VIP_SHOP
			#define _COST_ITEM             // 充值道具
		#endif
		#define _ITEM_EXP                // 神奇药水(经验道具)
		#define _NULL_CHECK_ITEM         // 空头支票道具
		#define _MEMORY_ITEM             // 记忆戒指
		#define _NPC_SCRIPT              // NPC脚本包
		//#define _BT_PET                  // BT宠物倍数设置
		#define _BT_ITEM                 // BT物品倍数设置
		#define _NO_JOIN_FLOOR           // 禁止传送地区
		#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 4 || _ATTESTAION_ID == 6 || _ATTESTAION_ID == 8 || _ATTESTAION_ID == 10 || _ATTESTAION_ID == 14 || _ATTESTAION_ID == 15 || _ATTESTAION_ID == 17 || _ATTESTAION_ID >= 19
			#ifdef _STREET_VENDOR
				#define _NEW_STREET_VENDOR       // 新摆摊功能
			#endif
		#endif

		#if _ATTESTAION_ID != 1 && _ATTESTAION_ID != 18
			#define _LUCK_STAR               // 幸运星(随机随时给随机玩家随机物品或宠物)
		#endif
		#if _ATTESTAION_ID != 1 
			#define _BT_ITEM                 // BT物品倍数设置
		#endif
		#ifdef _CHATROOMPROTOCOL
			#define _NPC_SAVEPOINT           // 纪录点存人物资料
		#endif
		#define _SPECIAL_SUIT            // 特殊套装
		#define _MANOR_EQUIP             // 庄园专属套装
		#define _ITEM_UPLEVEL            // 装备升级
		#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 4 || _ATTESTAION_ID == 6 || _ATTESTAION_ID == 8 || _ATTESTAION_ID == 10 || _ATTESTAION_ID == 14 || _ATTESTAION_ID == 15 || _ATTESTAION_ID >= 17
			//#define _TRANS_POINT_UP          // 每转点数上限
			#if _ATTESTAION_ID != 8
				#if _ATTESTAION_ID != 1 
					#define _FIND_TREASURES          // 寻找宝物
				#endif
				#define _UNLAW_THIS_LOGOUT       // 禁止原登
				#define _ITEM_PERCENTAGE         // 设置物品百分比
			#endif
			#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 4 || _ATTESTAION_ID == 6 || _ATTESTAION_ID == 8 || _ATTESTAION_ID == 10 || _ATTESTAION_ID == 14 || _ATTESTAION_ID == 17 || _ATTESTAION_ID >= 20 
				#define _ITEM_POOLITEM           // 随身道具仓库
				#define _ITEM_POOLPET            // 随身宠物仓库
				#define _ADD_NEWEVENT_1024       // 增加旗标至1024
				#define _SUPER                   // 紫云端的垃圾功能支持
				#define _NEW_SKILL               // 新技能开发
				#if _ATTESTAION_ID != 1
					#define _NEW_GM_ITEM             // 新玩家GM命令物品
				#endif
				#define _SHOW_PET_ABL            // 显示宠物能力
				#define _NEWEVENT_ITEM           // 新增任务道具
				#ifdef _NEW_RIDEPETS
					#define _ADD_RIDE_CF             // 增加自定义骑宠设置
				#endif
				#define _NO_STW_ENEMY            // 禁止外挂快速遇敌
				#define _NEW_PET_BEATITUDE       // 新一代宠物祝福
				#define _BATTLE_GETITEM_RATE     // 战斗获得物品机率
				#define _INSLAY_NOT_PILENUMS     // 不能精工叠加物品
				#if _ATTESTAION_ID == 14 || _ATTESTAION_ID == 28
					#define _ONLINE_TIME           // 在线时间
					#define _NPC_INT_SET           // NPC整型设置
				#endif
				#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 4 || _ATTESTAION_ID == 6 || _ATTESTAION_ID == 8 || _ATTESTAION_ID == 14 || _ATTESTAION_ID == 20 || _ATTESTAION_ID == 21 || _ATTESTAION_ID == 23 || _ATTESTAION_ID >= 25
					#if _ATTESTAION_ID != 1 || _ATTESTAION_ID != 25 
						#define _TALK_SAVE               // 说话存档
						#define _PLAYER_QUESTION_ONLIEN  // 玩家在线答题
					#endif
				#endif
				#if _ATTESTAION_ID == 28
					#define _LOTTERY_SYSTEM          // 彩票系统
					#define _NPC_CHECK_SCRIPT          // 判断脚本包
				#endif
				#if _ATTESTAION_ID == 38
					//#define _PET_TRANS_ABILITY       // 宠物转生能力定义上限
				#endif
				#if _ATTESTAION_ID != 38 && _ATTESTAION_ID != 49
					#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 8 || _ATTESTAION_ID == 21 || _ATTESTAION_ID == 23 || _ATTESTAION_ID >= 30
						#define _OLYMPIC_TORCH           // 奥林匹克火举
						#if _ATTESTAION_ID != 1
							#define _LOTTERY_SYSTEM          // 彩票系统
							#define _PLAYER_DIY_MAP          // 玩家DIY地图功能
						#endif
						#define _AMPOINT_LOG             // 积分点记录
						#define _SQL_VIPPOINT_LOG        // 重回币记录
						#define _NPC_CHECK_SCRIPT        // 判断脚本包
						#define _TALK_CHECK              // 说话验证
						#define _ONE_PET_SKILL           // 一次性宠技
						#define _MAGIC_SPECIAL           // 特殊魔法
						#if _ATTESTAION_ID != 1 
							#define _BATTLE_PK_TYPE          // PK类型选择
						#endif
						#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 21 || _ATTESTAION_ID == 23 || _ATTESTAION_ID >= 31
							#if _ATTESTAION_ID != 1
								#define _NPC_AUTO_MOVE			   // 能够自已移动的NPC
							#endif
							#define _SYSTEM_SPEAD          // 系统加速  暂不开放，会影响战斗挂机
							#define _FILTER_TALK    		   // 过滤说话
							#if _ATTESTAION_ID != 34
									#define _ALL_SERV_SEND       // 星球频道
									#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 32 || _ATTESTAION_ID == 33 || _ATTESTAION_ID >= 35 || _ATTESTAION_ID != 39
										#define _PET_TRANS_ABILITY       // 宠物转生能力定义上限
										#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 32 || _ATTESTAION_ID == 35 || _ATTESTAION_ID >= 45
											#define _CANCEL_ANGLE_TRANS      // 取消精灵召唤
										#endif
										#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 33 || _ATTESTAION_ID == 35 || _ATTESTAION_ID == 36 || _ATTESTAION_ID == 37 || _ATTESTAION_ID == 40 || _ATTESTAION_ID >= 43
											#define _COMPOUND_EXP          // 合成经验
											#define _ASSESS_ABILITY        // 评定能力
											#define _ONLINE_TIME           // 在线时间
											#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 35 || _ATTESTAION_ID == 37 || _ATTESTAION_ID >= 43
												#ifndef _VERSION_25
													#define _PAUCTION_MAN          // 拍卖系统
												#endif
											#endif
											#define _NOT_ESCAPE          	 // 禁止逃跑
											#define _PLAYER_OVERLAP_PK     // 玩家重叠时PK
											#define _FIMALY_PK_TIME        // 家族开片时间设定
										#endif
									#endif
							#endif
						#endif
					#endif
				#endif
			#endif
		#endif
	#else
		#define _FORMULATE_AUTO_PK       // 制定自动化PK系统(需SAAC配合)
	#endif
	
	#ifdef _MAGIC_SPECIAL
//		#define _MAGIC_FEATHERS          // 魔法羽毛
	#endif
	
	#ifdef _TALK_CHECK
		#define _NPC_TALK_CHECK          // NPC说话验证
		#define _ITEM_TALK_CHECK         // 道具说话验证
	#endif
	
	#ifdef _NPC_SCRIPT
		#define _NPC_CHECKTIME           // NPC时间判断
		#define _FAME_CHECK_NPC          // NPC声望判断
		#define _SQL_BUY_FUNC            // SQL购买功能
		#define _DEL_FAME                // 扣减声望
		#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 35 || _ATTESTAION_ID == 36 || _ATTESTAION_ID == 37 || _ATTESTAION_ID == 40 || _ATTESTAION_ID >= 43
			#define _NPC_INT_SET             // NPC整型设置
		#endif
	#endif

	#ifdef _NEW_SKILL
		#define _ITEM_ATTSKILLMAGIC      // (可开放) ANDY 道具技能
		#define _MASSAGE_PETSKILL        // 马杀鸡技能
		#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 23 || _ATTESTAION_ID >= 30
			#define _EXPANSION_ITEM_INSLAY      // 扩展镶宝石
			#define _STRENGTH_PETSKILL          // 元气宠技
			#define _EXPANSION_POWERBALANCE     // 扩展PETSKILL_PowerBalance
			#define _RESURRECTION_PETSKILL      // 复活宠技
			#define _LOSTLOST_PETSKILL          // 两败俱伤
			#define _GRAPPLING_PETSKILL         // 擒拿手
			#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 23 || _ATTESTAION_ID >= 31
				#define _PETSKILL_EXPLODE		        // (不可开) Change 宠技:爆裂攻击
				#define _PETOUT_PETSKILL		        // 换休息宠技
				#define _FIX_PETSKILL_HECTOR		    // 修复该技能无法设置turn
			#endif
		#endif
	#endif
#endif

#if _ATTESTAION_ID == 1 && _ATTESTAION_ID != 2 && _ATTESTAION_ID != 3 && _ATTESTAION_ID != 5 && _ATTESTAION_ID != 6 && _ATTESTAION_ID != 9 && _ATTESTAION_ID != 10 && _ATTESTAION_ID != 11 && _ATTESTAION_ID != 14 && _ATTESTAION_ID != 15 && _ATTESTAION_ID != 19 && _ATTESTAION_ID != 26 && _ATTESTAION_ID != 27 && _ATTESTAION_ID != 28 &&  _ATTESTAION_ID != 31 && _ATTESTAION_ID != 33 && _ATTESTAION_ID != 34 && _ATTESTAION_ID != 36 && _ATTESTAION_ID != 38 && _ATTESTAION_ID != 39 && _ATTESTAION_ID != 45
//	#define MANOR_MAX_NUM 			2        // (4.0) 家族据点
#else
	#define MANOR_MAX_NUM       4        // (4.0) 庄园数量
#endif

#if _ATTESTAION_ID != 5 && _ATTESTAION_ID != 11
	//#define FAMILY_MAX_MEMBER 			50 	// 家族人数
#else
	#define FAMILY_MAX_MEMBER       50		// 家族人数
#endif

#define _FIX_COPY_MM_BUG         //修复复制MM的BUG
#define _FIX_PETSKILL_BUG        //修复宠物技能重叠使用

#if _ATTESTAION_ID != 3 && _ATTESTAION_ID != 5 && _ATTESTAION_ID != 11
//#define _CHARADATA_SAVE_SQL      //将数据写入SQL
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 23 || _ATTESTAION_ID == 28 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 33 || _ATTESTAION_ID == 34 || _ATTESTAION_ID == 35 || _ATTESTAION_ID == 36 || _ATTESTAION_ID == 37 || _ATTESTAION_ID >= 40
	#if _ATTESTAION_ID != 49
		#define _SASQL                // 加入MYSQL功能
	#endif
#endif
#ifdef _SASQL
#ifdef _STREET_VENDOR
	#if _ATTESTAION_ID != 43
		#if _ATTESTAION_ID != 28 && _ATTESTAION_ID != 34
			#define _ONLINE_SHOP_MYSQL       // 在线商城改成MYSQL重回币
		#endif
		#define _STREET_VENDOR_MYSQL     // MYSQL重回币摆摊
	#endif
#endif
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 23 || _ATTESTAION_ID == 33
	#define _EXPANSION_VARY_WOLF      // 扩展暗月变身
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 21
	#define _SHOW_ITEM                // 秀装备
	#define _SHOW_ITEM_NAME           // 秀装备玩家名
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 32
#ifdef _VERSION_80
	#define _DISABLE_PROFESSION_SKILL      // 禁止使用职业技能技
	#define _TAKE_ITEMDAMAGE_FIX           // 下身装备损坏
	#define _TAKE_ITEMDAMAGE_FOR_PET       // 宠装装备损坏
#endif
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 35
	#define _GM_SAVE_ALL_CHAR      // GM命令存档
#endif

#if _ATTESTAION_ID == 35
	//#define _SHOW_FMPOINT					//显示庄园
	#define _NOT_FMPK_WAIT				//族战无休战期
#endif

#if _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 16 || _ATTESTAION_ID == 33 || _ATTESTAION_ID == 34 || _ATTESTAION_ID == 41
	#ifdef _BOUND_TIME
		#define _VIP_BOUND_TIME
		#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 41
			#define _BOUND_TIME_EXPANSION      // 包时功能扩展
		#endif
	#endif
#endif

//#define _ABSOLUTE_DEBUG              // 绝对调试


#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 14 || _ATTESTAION_ID == 21 || _ATTESTAION_ID == 28 || _ATTESTAION_ID == 30 || _ATTESTAION_ID == 31 || _ATTESTAION_ID == 32 || _ATTESTAION_ID == 33 || _ATTESTAION_ID == 35 || _ATTESTAION_ID == 36 || _ATTESTAION_ID == 37 || _ATTESTAION_ID == 40 || _ATTESTAION_ID == 44 || _ATTESTAION_ID == 45 || _ATTESTAION_ID == 46
	#define _ALLBLUES_LUA      // 使用LUA脚本
#endif

#ifdef _ALLBLUES_LUA
	#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 32 
		#define _ALLBLUES_LUA_1_1      // LUA1.1版
	#endif
	#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 32 
		#define _ALLBLUES_LUA_1_2      // LUA1.2版
		#define _ALLBLUES_LUA_1_3      // LUA1.3版
	#endif
	#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 1 || _ATTESTAION_ID == 32 
		#define _ALLBLUES_LUA_1_4      // LUA1.4版
	#endif
	
	#if  _ATTESTAION_ID == 1
		#define _ALLBLUES_LUA_1_5      // LUA1.5版
	#endif
	
	#if  _ATTESTAION_ID == 1
		#define _ALLBLUES_LUA_1_6      // LUA1.6版
	#endif
	#if  _ATTESTAION_ID == 1
		#define _ALLBLUES_LUA_1_7      // LUA1.7版
	#endif
	#if  _ATTESTAION_ID == 1
		#define _ALLBLUES_LUA_1_8      // LUA1.8版
	#endif
	
	#if  _ATTESTAION_ID == 1
		#define _ALLBLUES_LUA_1_9      // LUA1.9版
	#endif
	
	#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 32 
		#define _PETSKILL_SHOP_LUA       // 宠物技能商店LUA
	#endif
	#if  _ATTESTAION_ID == 1 || _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 32 
		#define _OFFLINE_SYSTEM       // 离线系统
	#endif
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 1 || _ATTESTAION_ID == 3  || _ATTESTAION_ID == 32 
	#define _PLAYER_NPC      // 虚假玩家
#endif

#if _ATTESTAION_ID == 0 || _ATTESTAION_ID == 3 
	#define _OTHER_SAAC_LINK  // 另一个SAAC连接
#endif

#if _ATTESTAION_ID == 1
	#define _ONE_SERVER       // 单机绑定
#endif

#if _ATTESTAION_ID == 32
	#define _NOT_NOCRYPTO_LUA // 不让读取非加密LUA脚本
#endif

#if _ATTESTAION_ID == 1
	#define _CRYPTO_LUA    		// lua脚本加密
#endif

#if _ATTESTAION_ID == 1
	#define _CAMEO_MONEY      		// 贝壳货币
	#define _MISSION_TRAIN    		// 任务链
	#define _DEL_DROP_PET    			// 删除地上宠物
	#define _PET_EVOLVE    				// 宠物进化
	//#define _PET_ADD_EXP      		// 宠物经验提升道具
	#define _FIX_ADD_EXP      		// 修复豆子经验叠加
	#define _FMRIDE_NOT_FMSPRITE	// 家族骑宠不分光明黑暗
	#define _NOT_SHOW_ONLINE_NUM	// 不显示在线人数
	#define _ITEM_COLOER_DIFFER		// 道具颜色不同
	#define _SPECIAL_COUNTER			// 计数器
	#define _SPECIAL_TIMER				// 计时器
	#define _RECORD_IP						// 记录IP
	#define _FIX_STW_SPEED_ENEMY	// 修改外挂快速遇敌
	//#define _DEL_NOT_25_NEED_ITEM	// 删除非2.5需道具抓宠
	#define _LOCK_PET_ITEM				// 绑定宠物与道具
	#define _ULTIMATE_ANNOUNCE		// 飞人通知
	#define _SHIP_MATEMO					// 船的变身形像
	#define _FIX_CORE_LOOP				// 修改CORE_LOOP
	#define _VIP_BATTLE_EXP				// 会员战斗经验
	#define _FLOOR_DIFI_FIELD			// 楼层字段
	#define _FM_FMPOINTPK_LIST		// 庄园对战列表
	//#define _BATTLE_BOUT_TIME			// 战斗回合时间
	#define _ACTIVE_GAME			    // 活力值
	#define _INVERSION_PETSKILL   // 反转技能
	#define _NO_HELP_MAP					// 禁止HELP
	#define _BATTLE_TIME					// 战斗时间
	//#define _SAME_IP_ONLINE_NUM		// 同IP登陆数
	#define _VIP_TRANS						// VIP转生服务
	#define _STREET_VENDOR_TRANS	// 摆摊限制转数
	#define _KEEP_UP_NO_LOGIN			// 维护禁止登陆
	#define _FIX_CHAR_LOOP				// 修正数据循环
	#define _FIX_SAVE_CHAR				// 修正保存档问题
	#define _FIX_P_VS_P_PARENT		// 修正PVP加入战斗
	#define _FMPOINT_PKTIME_SELECT// 自主选择族战时间
	#define _CHECK_SEVER_IP				// 判断服务器IP
	#define _DAMMAGE_CALC					// 自定义伤害
	#define _PLAYER_TITLE					// 玩家称号
	#define _VIP_PET_TRANS				// 会员转宠
	//#define _UNIFIDE_MALINASI			// 统一玛丽娜丝鱼村出生
	//#define _FIX_PET_SKILL				// 修复宠物技能
	//#define _NO1_B2_NONO					// B2学在第一位时自动锁定21忠
	//#define _FMPOINT_APPEND_ABI		// 庄园家族成员附加能力
	#define _ITEM_OVER_LAP				// 道具重复功能
	#define _NEW_LOAD_NPC					// 读取NPC
	#define _NEW_LOAD_MAP					// 读取地图
	#define _NO_CLS_EVENT_255			// 不清楚255后的旗标
	#define _MISSION_TIME					// 任务计时
	#define _MVP_LOCK_JP					// 绑定MVP极品宠
	//#define _FMPOINTPK_FOR_TWO		// 每天族战最多为两场
	#define _BURST_LOG						// 分段日志
	#define _STREET_VENDOR_EFFECT // 摆摊属性显示
	//#define _MASK_ENCOUNTER 			// 屏蔽原地封包
	#define _NEWCLISETMAC		 			// 记录客户端的MAC地址
	#define _NEWCLISETSERVID 			// 记录客户端服务器ID
	//#define _FMPOINT_PK_MESSAGE		// 族战全服通告
	//#define _FMPOINT_WELFARE			// 庄园福利
	#define _REGISTRATION_TIME		// 签到时间
	#define _FAME_REG_TIME				// 家族签到时间
#ifdef _OFFLINE_SYSTEM
	#define _OFFLINE_TIME					// 离线时间
#endif
	#define _DATA_INFO_SHOW				// 资料信息显示
	#define _ITEM_USE_TIME				// 道具使用时间
//	#define _PET_VALIDITY  				// 宠物有效期
	//#define _PET_FUSION_LUA  			// 融合宠LUA
	//#define _PET_ENEMY_DEVELOP_UP //全面提升宠物成长
	#define _FIRST_LOCK_ITEM			//第一次交易后自动绑定物品
	#define _CTRL_TRANS_DEVELOP		//控制成长
	#define _CANCEL_STREET_VENDOR		//防止卡摆摊
	//#define _ABDUCT_NO_PETOUT			//旅程伙伴禁直同时换宠
	//#define _SUPER_FMPOINT				//超级庄园
	#define _ATTACK_RAND_SORT			//攻击顺序随机
	#define _POWER_UP							//能力提升
	#define _FM_POINT_APPLY_FAME	//庄园申请所需声望
	#define _PET_MAKE_2_TRANS						// Terry □□退ネ□GM□□

#endif
#if _ATTESTAION_ID == 0
	#define _DEBUG_ALL
#endif

#if _ATTESTAION_ID == 35
	#define _DAMMAGE_CALC					// 自定义伤害
#endif

#endif

//                       SA_REVERSION NEW ADD
//======================================================================================
#define _BRAND_NEW_25				//2.5版本总控制  开启之后将打开2.5独有的功能
//======================================================================================

//###############                                            服务器基本信息定义                                    ####################
#define _SA_VERSION 'N'
#define _DEFAULT_PKEY "starttwneo"		// 默认的密钥
#define _RUNNING_KEY  "runtwtrinity"		// 运行中密钥
//###############                                            服务器基本信息定义                                    ####################

//###############                                            通用版本功能定义区                                    ####################
#define		_JZ_NEWSCRIPT_LUA				 // 吉仔 新LUA脚本引擎
#ifdef		_JZ_NEWSCRIPT_LUA
	#define _MO_LNS_NLSUOXU            // LNS-NL库所需
	#define _MO_LNS_CHARSUOXU          // LNS-CHAR库所需
	#define _MO_LNS_NLGSUOXU           // LNS-NLG库所需
	#define _MO_LNS_MAPSUOXU           // LNS-MAP库所需
	#define _MO_LNS_MYSQLSUOXU         // LNS-MYSQL库所需
	#define _MO_LUA_BETT01			        // LUA增加接口与对象
	#define _LUA_Debug                  // LUA错误调试
	#define _MO_ADD_LUADX					    // LUA一些拓展
	#define _ITEM_LUA				            // 道具LUA支持
#endif
//#define _MO_SHOW_DEBUG			//显示封包信息
//#define _NO_ATTACK						 // 防御CC攻击
//#define	_NO_FULLPLAYER_ATT	     // 防满人攻击
//#define _NEW_VERIFICAITON 	//沫修改新的LUA验证码算法
#define _SHARE_EXP					  // 经验共享
#define EXPGET_MAXLEVEL 5			//经验不衰减最大等级
#define EXPGET_DIV 15				//经验分割等级
#ifdef _SHARE_EXP
#define _COMBO_EXP					// 合击加成
#endif
#define _GAMBLE_POINT				//SQL赌博积分
#define _TRIAL_POINT				//SQL试炼积分
#define 	_MO_LUA_FMPOINT			//新增据点相关的lua接口
#define _DEFEND_BIGBAO				//防御大包
#define _SERVER_DEF                 // 封包安全机制
//#define _DEBUG_RET_CLI
#define _NOT_FMPK_WAIT				//族战无休战期
#define _SINGLE_ONLINE				//单号在线
#define _CHAR_LOOP_TIME				//char运行时间
#define _MO_IMAGE_EXTENSION			//形象扩展

#define _CHANGE_PETNAME_FIX			//禁止宠物非法改名
#define		_MO_RELOAD_NPC			//重读NPC
#define _ROOKIE_ITEM				//新手道具 1转之后将删除 且无法装备
#define _NO_TRANS_ITEM				//禁止转生携带的道具
#define _MAX_MERGE_LEVEL			//最高合成道具等级
#define _MO_MAP_AUTO_UPDATE				//自动更新地图
#define _NEW_FUNC_DECRYPT		//新解密机制

#define _PLAYER_EFFECT									//人物效果
#define _MO_PLAYER_EFFECT							//增加人物效果相关字段以扩充
#define _MO_LUA_KS_CALLBACK					//KS接口LUA回调 用于拦截战宠选择事件
#define _MO_LOGIN_NO_KICK							//登录不顶人配置
#define _ATTACK_EFFECT								  // Terry 攻击效果
#define _STREET_VENDOR_CHECK					//摆摊标题长度限制
#define _MO_LUA_TALK_EX								//ablua说话函数扩展
#define _MO_LUA_ENCOUNTER_CONTROL		//ablua遇敌控制
#define _MO_ILLEGAL_NAME            // 禁止人物名称
//#define _MO_NO_WALK											//禁止走路
#define _MO_FIX_ULTIMATE							//修复飞人判定
#define _TENSE_FIELD										//限时字段

#define _CHAR_NANOR_             //人物脚下光环
#define FAMILY_MANOR_         //庄园光环
#define _NETLOG_              //网络T人日志
#define _PETCOM_              //宠物计算
#define _MORECHARACTERS_    //多页人物
#define _FAMILYBADGE_				//家族徽章
#define _SECONDARY_WINDOW_DATA_       //二级窗口内容封包
#define _TRUNTABLE_                 //转盘更新封包     注：这里主要是增加个封包宏，该封包宏主要做为数据更新
#define _PETFOLLOW_NEW_                 //开放5只宠的项圈
#define _TRUMP_EQUIPMENT					//法宝装备
#define _FIX_SUPERPERSON_STRANS //修复封包6转BUG
#define _PETMAIL_TIME				     // 宠物邮件自定义时间（也就是宠物敏捷）
#define _MO_LUA_RESIST							//额外抗性的lua接口及字段
#define _MO_FIX_STREET_VENDOR	//修正摆摊接口攻击导致的卡端
#define _MO_LUA_BATTLE_START		//战斗初始化接口回调
#define _REDUCE_MERGE_FAME				//降低无挂服的家族合成声望等级所需声望
#define _ALLDOMAN                // (可开放) Syu ADD 排行榜NPC
#define _ALLDOMAN_DEBUG          // (可开) Change 修正重启server後排行榜消失bug
#define _MO_LUA_MERGE_CALLBACK		//合成料理的lua回调
#define _ALWAYS_TEAM_OPEN					//登录默认开启组队
#define _FIX_LER_IMG								//修复雷尔动画
#define _NPC_MAGICCARD							// 魔法卡NPC
#define _MOVE_SCREEN								//移动屏幕
#define _BATTLE_OVER_STAR_TIME// 战斗后重新开始时间间隔

#define _NEW_SQL_MONEY										//新增SQL货币

//###############                                            通用版本功能定义区结束                                    ####################

//###############                                            2.5特有功能定义区                                    ####################
#ifdef _BRAND_NEW_25
#define _DEFAULT_PKEY "shiqi"		// 默认的密钥
#define _RUNNING_KEY  "shiqi.hk"		// 运行中密钥
#define DENGLUKEY1 "6D27AEB8326A2A8E7D04B9F9B44C59C7"

#define _NEW_TITLE											//新称号系统
#define _NEW_UPDATETITLE							//更新称号封包
#define CHAR_RECORD_POINT               //人物记录点客户端读取
#define _FREE_SAVE    				// 免条件记录
#define FAMILY_MAX_MEMBER 			50 	// 家族人数
#define MANOR_MAX_NUM 			2        // (4.0) 家族据点
#define _LEVELUP_RECOVERY					   //升级满血
#define _NEW_ITEM_          //新道具栏

#define _VISUAL_BEATITUDE							//新可视化祝福机制
#define _RIDEFLG_        						 //骑宠开关
#define _RIDEQUERY_                            //骑宠查询
#define _PREVENT_TEAMATTACK				//防止同队互打
#define _SHOW_VISUAL_BEATITUDE		//交易摆摊显示可视化祝福
#define _MO_REDUCE_PVP_COMBO				//降低PVP合击率
#define _BATTLE_OVER_STAR_TIME// 战斗后重新开始时间间隔
//#define _CANCEL_BORN_PET			// 取消出生宠

#define _NPC_EVENT_NOTICE							//沫新增NPC任务状态提示功能，需客户端开启该宏功能
#ifdef _NPC_EVENT_NOTICE
	#define EVENT_NOT_RESPOND					120138	//灰色叹号 任务已完成或者尚未达到完成条件
	#define EVENT_CAN_ACCECPT					120137	//黄色叹号 任务可以交接
	#define EVENT_READY_TO_START			120139	//黄色问号 可以接任务
	#define EVENT_CANNOT_START				120140	//灰色问号 不能接任务
#endif
#define _CHAR_TITLE_STR_                //文字称号
#define _NEW_UPDATETITLE							//更新称号封包

#define _NO_TEAMWARP_SKYLAND			//禁止团体带入天空
#define _GLORY_POINT								//SQL名声积分
#define _MO_LUA_GOLRYCALLBACK		//名声回调
#define _RED_MEMOY_                 //红包
#endif
//###############                                            2.5特有功能定义区 结束                                   ####################
