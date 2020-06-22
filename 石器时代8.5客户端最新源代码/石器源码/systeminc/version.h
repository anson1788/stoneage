#ifndef __VERSION_H__
#define __VERSION_H__

#ifdef _WIN32
#include <crtdbg.h>
#endif


#ifdef _SA_VERSION_SPECIAL
#define _SA_VERSION_60
#define __ALBUM_47
#define _FONT_SIZE					// Robin (要开) 可改字型大小
#define _ITEM_PILENUMS				// (可开放) ANDY 道具素材堆叠
#define _ITEM_INTENSIFY				// (可开) Change 宠技:强化道具
#define _ALCHEMIST					// Robin 精炼功能 
#define _CHAR_NEWLOGOUT				// (可开放) ANDY 新登出机制
//#define _ITEM_JIGSAW				// (可开) Change 拼图道具
#define	_TELLCHANNEL				// (可开放) ROG ADD 密语频道
#define _TRADETALKWND				// (可开放) Syu ADD 交易新增对话框架
#define _DROPPETWND					// (可开放) Syu ADD 丢弃宠物确认
#define _TEAM_KICKPARTY				// (可开放) ANDY 队长踢人
#define _MONEYINPUT					// (可开放) Syu ADD 手动输入金钱量

#define _STREET_VENDOR				// 摆摊功能
#define _STREET_VENDOR_CHANGE_ICON	// 修改摆摊图示
#define _NEW_MANOR_LAW				// 新庄园规则
#define _PETS_SELECTCON				// (可开放) ANDY 宠物选择
#define _FM_MODIFY					// 家族功能修改
#define _CHANNEL_MODIFY				// 频道功能整合
#define _TRADELOCKBTN				// (可开放) Syu ADD 增加锁定键
#define _MAILSHOWPLANET				// (可开放) Syu ADD 显示名片星球
#define _LOCKPARTYFLOOR				// (可开放) Syu ADD 增加不可组队的地图编号
#define _LOCKHELP_OK				// (可开放) Syu ADD 锁定不可加入战斗
#define _JOBDAILY					// cyg 任务日志功能
//	#define _TALK_WINDOW				// 对话视窗
#define _SHOW_FUSION				// (可开) Change 融合宠物在CLIENT端只显示转生数,修改为融合宠显示为融合
#define _ITEM_FIREWORK				// 烟火功能
#define _FIREWORK_SOUND				// cyg 新增音效(烟火用)
//#define _GET_HOSTNAME				// (可开) Change 取得电脑名称
#define _NPC_MAGICCARD				// cyg 魔法牌游戏
#define _THEATER					// Terry 剧场功能
#define _MOVE_SCREEN				// Terry 移动荧幕到指定的座标
#define _NPC_DANCE					// cyg 动一动npc
#define _CHANNEL_WORLD				// 增加世界频道
#define _STANDBYPET					// Robin 送待机宠列表到Server
#define _MAILSHOWPLANET				// (可开放) Syu ADD 显示名片星球
#define _EXTEND_AB					// cyg 扩充名片功能
#define _ITEM_EQUITSPACE			// (可开放) ANDY 新增装备栏位
#define _EQUIT_ARRANGE				// (可开放) ANDY 格挡
#define _EQUIT_NEWGLOVE				// (可开放) ANDY 手套栏位
//#define _ALCHEMIST					// Robin 精炼功能 
//#define _ALCHEPLUS					// Robin 鉴定功能 
//#define _NPC_ITEMUP					// (可开) Change 让道具升级的NPC
//#define _ITEM_COUNTDOWN				// cyg 道具倒数功能
//#define _CHATROOMPROTOCOL			// (可开放) Syu ADD 聊天室频道
//#define _FRIENDCHANNEL				// (可开放) ROG ADD 好友频道
//#define __ONLINEGM
#endif
//#define _BATTLE_PK_PLAYER_FOR_40		// 40人战斗
//#define  _BATTLE_PK_PLAYER_FOR_6VS6		// 6 vs 6       XIEZI 添加

#ifdef _BATTLE_PK_PLAYER_FOR_40
#define BATTLESIDENUM 4					//战斗队伍数量
#define BATTLKPKPLYAERNUM 40			//最大参战角色数
#define MAX_BATTLE_ROW_CHARS   5		//每行最大战斗角色数
#else
#define BATTLESIDENUM 2					//战斗队伍数量
#ifdef _BATTLE_PK_PLAYER_FOR_6VS6
#define BATTLKPKPLYAERNUM 24			//最大参战角色数
#define MAX_BATTLE_ROW_CHARS   6		//每行最大战斗角色数
#else
#define BATTLKPKPLYAERNUM 20			//最大参战角色数
#define MAX_BATTLE_ROW_CHARS   5		//每行最大战斗角色数
#endif
#endif

#define _NEW_SYSTEM_MENU				// 新系统菜单 2011/12/21
#define _PTTERN_SEPARATION_BIN			// 分离补丁加载
//#define _SFUMATO						// 二次渲染 未完善，需美术资源配合
#define _SPECIAL_LOGO					// 新登陆图
#define _CHANNEL_ALL_SERV				// 增加星球频道 2011/12/21
#define _EXTEND_FONT_PAL				// 扩展字体颜色色板 2011/12/22
//#define _FONT_PARAGRAPH_COLOR			// 字体分离颜色 2011/12/22
#define _NEW_CLIENT_LOGIN				// 登陆记录MAC地址及线路选择 2011/12/22
//#define _NEW_SHOP_FRAME  				// 新商店框架 2011/12/22

#define _HI_COLOR_16				// (可开) ANDY (8bit to 16bit) 含图档
#define _HI_COLOR_32				// (可开) longzoro (32 bit) 含图档
#define _NEW_RESOMODE				 //800 600模式
#define _MOUSE_SHOW_INFO_FOR_HEAD	// (可开) longzoro 鼠标显示信息
//#define _NEWLOGO				   // (可开放) Syu ADD 6.0登入画面新logo 要关_v4_0 
/* -------------------------------------------------------------------
* 新功能：石器 800 * 600
* 人员：旭诚,镇玮
* 说明：
*/

extern int DISPLACEMENT_X;
extern int DISPLACEMENT_Y;
extern int viewOffsetX;
extern int viewOffsetY;

//#define _NEWDRAWBATTLEMAP_FIX		   // (不可开)ROG ADD 战斗地图修改

// 版本分割
#ifdef _SA_VERSION_85
#define _SA_VERSION_80
#endif

#ifdef _SA_VERSION_80
#define _SA_VERSION_75
#endif

#ifdef _SA_VERSION_75
#define _SA_VERSION_70
#endif

#ifdef _SA_VERSION_70
#define _SA_VERSION_60
#endif

#ifdef _SA_VERSION_60
#define _SA_VERSION_50
#endif

#ifdef _SA_VERSION_50
#define _SA_VERSION_40
#endif



/* 请将控制专案的 definition 放在这个档案
* 然后藉由 remark/unmark 来开关功能
*/



/* -------------------------------------------------------------------
* 专案：族人职务与家族功能补充
* 人员：小瑜、彦勋、志伟、小民
* 说明：
*/
#define  _FMVER21                      // (可开放) 家族职掌功能
#define _PERSONAL_FAME                 // (可开放) Arminius 8.30 家族个人声望


// -------------------------------------------------------------------
// 专案：伊甸大陆新NPC交谈介面
// 人员：晓威
// 说明：__EDEN_EFFECT:赌场的银行NPC'委托交易的NPC
//			 __EDEN_AUCTION:委托NPC拍卖物品介面
#define __EDEN_EFFECT			// (可开放) 但测试时要打开
#define __EDEN_AUCTION		    // (可开放) 但测试时要打开

// -------------------------------------------------------------------
// 专案：攻击性咒术的制作
// 人员：笃志
// 说明：
#define __ATTACK_MAGIC

// -------------------------------------------------------------------
// 专案：吓年兽的鞭炮
// 人员：晓威
// 说明：
#define _ITEM_FIRECREAKER

/* -------------------------------------------------------------------
* 新功能：PK星系
* 人员：彦勋
* 说明：
*/
//#define _PKSERVER // (不可开放)


// -------------------------------------------------------------------
// 专案：传送手机简讯的玩家界面实作
// 人员：昌磊  (LeiBoy)
// 说明：让玩家可以传简讯给特定的ASP网页处理
// 主要相关档 : PhoneMsg.cpp
// 完成日期 : 2002年01月28日
// 开放日期 : 2002年03月04日
// Status : Ready!
//#define __PHONEMESSAGE    
#define _FIX_URLENCODE             // WON ADD (可 开 放) 北京修正简讯无法加空格的问题


// -------------------------------------------------------------------
// 专案：传送手机简讯的二期工程
// 人员：(原定)昌磊  (接班人未定)
// 说明：让玩家可以传简讯到名片中好友的手机中
// 开放日期 : 禁止开放!!
// Status : 建构中...
//#define __CARDMESSAGE   //(禁用)!!


//#define __NEW_CLIENT_ONLY_WRITE	//Cary(禁用,未测过)
//#define CHINA_VER			//Cary (禁用)
//#define __ALBUM_4
//#define __ALBUM_6			//Cary 加入新的宠物相簿(含年兽)
//#define __TEST_SERVER		//Cary 3.0的测试server

#define __HELP				//Cary 线上说明
#define __NEW_PROTOCOL		//Cary 新的底层

#define __NEW_CLIENT_MEM	//Cary
#define __SKYISLAND			//Cary 加入天空岛的功能
#define __HI_COLOR			//Cary 加入高彩的功能(debug version专用)
#define __TALK_TO_NPC		//Cary 使用左键和npc说话
#define __NEW_BATTLE_EFFECT	//Cary 2002.2.21 新的战斗特效，如:替身娃娃
#define _AFTER_TRADE_PETWAIT_	   // (可 开 放) set pet status to wait after trade :vincent



#ifdef _SA_VERSION_40
/* -------------------------------------------------------------------
* 新功能：伊甸4
* 人员：志旺
* 说明：
*/
#define _FIX_DEL_MAP			   // (可开放) WON ADD 修正玩家抽地图

#define _FIX_9_FMPOINT			   // (可开放) WON ADD 九大庄园
#define _FIX_10_FMPOINT			//(可开) Change 十大庄园 20050404
#ifdef _FIX_10_FMPOINT
#define fm_point_num	10		   // 庄园数
#else
#define fm_point_num	9		   // (可开放) WON ADD 庄园数
#endif

#define _ADD_POOL_ITEM				// (可开放) WON ADD 增加寄放道具
//#define _ADD_FAMILY_TAX				// (不可开) WON ADD 增加庄园税收

#define _LOST_FOREST_FOG			// (可开放) the lostforest's fog. code:shan 
#define _MIND_ICON					// (可开放) show icon from char mind. code:shan
#define _MAGIC_WEAKEN				// (可开放) show magic(weaken). code:shan

#define _BLACK_MARKET            // (不可开) a black market. code:shan 

// -------------------------------------------------------------------
// vincent define
#define _MAGIC_DEEPPOISION         // (可开放) show magic(deeppoison). code:vincent
#define _MAGIC_BARRIER             // (可开放) show magic(barrier). code:vincent
#define _MAGIC_NOCAST              // (可开放) show magic(nocast). code:vincent
#define _ITEM_CRACKER              // (可开放) 道具:拉炮 code:vincent
#define _SKILL_ROAR                // (可开放) vincent  宠技:大吼(克年兽)
#define _SKILL_SELFEXPLODE         // (可开放) vincent  宠技:自爆

#define _NEW_RIDEPETS			   // (可开放) 新骑宠

#define _ATTDOUBLE_ATTACK	       // (可开放) 属性强化攻击
#define __TOCALL_MAGIC			   // (可开放) kjl 02/06/24	
#define _VARY_WOLF                 // (可开放) pet skill : vary wolf. code:shan
#define _PIRATE_ANM                // (可开放) monster move(海盗王). code: shan
#define _TRADE_PETCOLOR            // (可开放) Syu ADD 改变交易时宠物转生颜色
#define _FIXSHOWMPERR			   // (可开放) Syu ADD 修正补气时动画显示错误
#define _LIZARDPOSITION			   // (可开放) Syu ADD 修正龙蜥显示位置过低


/* -------------------------------------------------------------------
* 新功能：
* 更新日期2002/12/18 14:00 更新
*/

#define _TEAM_KICKPARTY				// (可开放) ANDY 队长踢人
#define _MONEYINPUT					// (可开放) Syu ADD 手动输入金钱量

#define _PETS_SELECTCON				// (可开放) ANDY 宠物选择

#endif		// _SA_VERSION_40



#ifdef _SA_VERSION_60
/* -------------------------------------------------------------------
* 新功能：石器6.0
* 更新日期
*/
#define _OBJSEND_C					// (可开放) ANDY 6.0
#define _ADD_SHOW_ITEMDAMAGE		// (可开放) WON ADD 显示道具的耐久度
#define _WATERANIMATION				// (可开放) Syu ADD 泪之海动画层
#define _NEWDRAWBATTLEMAP			// (可开放) Syu ADD 水世界战斗地表与战斗特效 
#define _AniCharBubble				// (可开放) Syu ADD 动画层人物吐出气泡
#define _AniCrossFrame				// (可开放) Syu ADD 动画层游过画面生物
#define _AniImmobile				// (可开放) Syu ADD 定点产生特定动画
#define _AniRandom					// (可开放) Syu ADD 随机产生环境动画
//#define _DELBORNPLACE				// (可开放) Syu ADD 6.0 统一出生于新手村
#define _NEWMUSICFILE6_0			// (可开放) Syu ADD 6.0 新地图音乐
#define _TRADESHOWNAME				// (可开放) Syu ADD 交易显示对方名称
#define _TRADEINPUTMONEY			// (可开放) Syu ADD 交易手动输入金钱
#define _TRADE_SHOWHP				// (可开放) Syu ADD 交易显示血量
#define _CHANGETRADERULE			// (可开放) Syu ADD 交易规则修订
#define _PETSKILL_CANNEDFOOD		// (可开放) ANDY 宠物技能罐头
#define _SPECIALSPACEANIM			// (可开放) Syu ADD 特殊场景动画配置
#define _SHOOTCHESTNUT				// (可开放) Syu ADD 宠技：丢栗子
//#define _NEWLOGO				   // (可开放) Syu ADD 6.0登入画面新logo 要关_v4_0 
#endif		//SA_VERSION_60

#define _TRADESYSTEM2				// (可开放) Syu ADD 新交易系统

#ifdef _SA_VERSION_70
/* -------------------------------------------------------------------
* 新功能：石器7.0
* 更新日期
*/
//已开放
#define _SARS						// (可开放) WON ADD 毒煞蔓延
#define _FIX_MAXGOLD			    // (可开放) WON ADD 增加人物金钱上限

#define _ITEM_PILENUMS				// (可开放) ANDY 道具素材堆叠

#define _ITEM_EQUITSPACE			// (可开放) ANDY 新增装备栏位
#define _EQUIT_ARRANGE				// (可开放) ANDY 格挡
#define _EQUIT_NEWGLOVE				// (可开放) ANDY 手套栏位

#define _LOCKPARTYFLOOR				// (可开放) Syu ADD 增加不可组队的地图编号
#define _LOCKHELP_OK				// (可开放) Syu ADD 锁定不可加入战斗

#define _TRADETALKWND				// (可开放) Syu ADD 交易新增对话框架
#define _TRADELOCKBTN				// (可开放) Syu ADD 增加锁定键

#define _WAVETRACK					// (可开放) Syu ADD 音波攻击轨道修正

#define _CHAR_NEWLOGOUT				// (可开放) ANDY 新登出机制

//新功能：人物职业
#define _TABLEOFSKILL				// (可开放) Syu ADD 7.0 新增人物技能表单
//#define _NEWPANEL_71				// (可开放) ROG ADD 新login画面
//#define	_NEWPANEL_70				// (可开放) Syu ADD 石器7.0 LOGIN 画面
#define _CHAR_PROFESSION			// (可开放) WON ADD 人物职业
#define _SKILLSORT					// (可开放) Syu ADD 职业技能排列分类
#define _NEWREQUESTPROTOCOL			// (可开放) Syu ADD 新增Protocol要求能力细项
#define _OUTOFBATTLESKILL			// (可开放) Syu ADD 非战斗时技能Protocol
#define _NEWSOUNDEFFECT				// (可开放) Syu ADD 动画音效档
#define _FIXSTATUS					// (可开放) Syu ADD 新战斗状态显示方式
#define _ALLDOMAN					// (可开放) Syu ADD 排行榜NPC
#define _CHATROOMPROTOCOL			// (可开放) Syu ADD 聊天室频道
#define _FRIENDCHANNEL				// (可开放) ROG ADD 好友频道
#define	_TELLCHANNEL				// (可开放) ROG ADD 密语频道

#define _DROPPETWND					// (可开放) Syu ADD 丢弃宠物确认

#define _MAILSHOWPLANET				// (可开放) Syu ADD 显示名片星球
#define _MOON_FAIRYLAND				// (可开放) ROG ADD 月之仙境
#define _SPECIALSPACEANIM_FIX		// (可开放) ROG ADD 修正动画
//#define _TIMEBAR_FUNCTION			// (不可开) ROG ADD 时间bar函式


//10/23
#define _FM_MODIFY				// 家族功能修改

#define _FIREHUNTER_SKILL			// (可开放) ROG ADD 朱雀技能_火线猎杀
#define _PETSKILL_ACUPUNCTURE       // (可开放) Change 宠技:针刺外皮
#define _PETSKILL_DAMAGETOHP	    // (可开放) Change 宠技:暗月狂狼(嗜血技的变体) 
#define _PETSKILL_BECOMEFOX         // (可开放) Change 宠技:媚惑术
#define _PETSKILL_BECOMEPIG         // (可开) Change 宠技:乌力化


// 新功能：连接新资料库(新付费机制)
#define _NEW_WGS_MSG				// (可开) WON ADD WGS的新视窗


#define _CHANNEL_MODIFY		// 频道功能整合


#define _CHANGEGALAXY // Robin 星系移民
#define _ERROR301 // Robin AP回传301错误,产品尚未注册

#define _STANDBYPET // Robin 送待机宠列表到Server
#endif		//SA_VERSION_70


#define	DIRECTINPUT_VERSION 0x0700	// Robin 增加DirectX SDK的相容性

#define _CLOUD_EFFECT				// (不可开) ROG ADD	云层效果
//#define _NPC_WELFARE_2			// (不可开) WON ADD 职业NPC-2

//#define _NPC_SELLSTH				// (可开放) ANDY 拍卖
//#define _SYUTEST
//#define _SYUTEST2
//#define _SYUTEST3
//#define _AUCPROTOCOL				// (不可开) Syu ADD 拍卖频道开关Protocol

/* -------------------------------------------------------------------
* 新功能：PK服务器
* 更新日期
*/
//测试中
//制作中

//#define _PKSERVERCHARSEL			// (不可开) Syu ADD PK服务器选择星系人物
// -------------------------------------------------------------------
// 以下不可开
/* -------------------------------------------------------------------
* 新功能：real.bin分图
* 更新日期
*/
//测试中F
//#define _SAHOOK				    // (不可开) Syu ADD Hook程式
//#define _REALDIVISION			    // (不可开) Syu ADD real.bin分图，要有realtxt.txt档
// -------------------------------------------------------------------
//#define _OMIT_WGS					// (不可开) 略过帐号、密码至WGS. code:shan
// -------------------------------------------------------------------
#ifdef _SA_VERSION_50
/* -------------------------------------------------------------------
* 新功能：石器5.0
* 说明：
*/
//#define _CANT_ATK					// (不可开) can't attack(own team). code: shan
//#define _SETTICK_COUNT			// (不可开) ANDY 重复发话
//#define _GM_MARK					// (不可开) WON ADD GM 识别
//#define _GM_IDENTIFY		        // (不可开)Rog ADD GM识别的gm指令

//#define _LOGINKICK				// (不可开) Syu ADD 登入画面超过30秒踢回星系列表
#endif		//SA_VERSION_50
#define MALLOC(s) malloc(s)
#define CALLOC(c,s) calloc(c,s)
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)
#define FREE(p) free(p)

//#define _PET_TRACE_MOUSE
//#define __EDEN_DICE  				// (不可开) 赌场骰子
#define _AI_OTHER						// AI其它功能
#define __AI							// Cary 加入自动战斗功能
#ifdef _AI_OTHER
#define _AI_CAPTURE
#endif
//-----------------------
unsigned int TimeGetTime(void);

#ifdef _SA_VERSION_70
#ifdef _BACK_VERSION
#define _REMAKE_20						// 石器复刻功能控制(防外挂)
#define _BACK_LOGO
#define _BACK_WINDOW //复刻版视窗化(debug版才可开,内部人员使用)
#define _READ16BITBMP					// Terry 可读入及显示高彩图
#else
#define _ATTACK_AI						// 内挂攻击AI
#define _AI_NEW_FIRST					// AI 首次可以用职业技能
#define _BATTLESKILL					// (可开放) Syu ADD 战斗技能介面
#define _NEWPANEL						// (可开放) Syu ADD 7.0 新人物状态介面
#endif
#endif

#ifdef _SA_VERSION_75
/* -------------------------------------------------------------------
* 新功能：石器7.5
* 更新日期
*/

#define _ANGEL_SUMMON			// 天使的招唤 Robin
#define _PROFESSION_ADDSKILL  // (可开) Change 追加职业技能
#define _TRANS_6				// (可开) Change 人物6转
#define _PET_2TRANS			// (可开) Change 宠物2转
//#define _75_LOGIN					// 7.5版login画面 
#define _STREET_VENDOR		// 摆摊功能
#define _NEW_MANOR_LAW		// 新庄园规则

#define _PETSKILL_LER			// 雷尔技能
//#define _PK_SERVER            //百万PK活动
//#define _8_TEST					//8.0测试server

// -------------------------------------------------------------------
// 专案：线上回报系统
// 人员：逸俊
// 说明：
//#define __ONLINEGM
//--------------------------------------------------------------------

#define _STREET_VENDOR_CHANGE_ICON	// 修改摆摊图示

#define _TALK_WINDOW				// 对话视窗
#define _SHOW_FUSION				// (可开) Change 融合宠物在CLIENT端只显示转生数,修改为融合宠显示为融合
#define _NPC_FUSION					// (□宮□) ANDY NPC□□芯シ

#define _FONT_SIZE					// Robin (要开) 可改字型大小
#define _CONTRACT					// Robin (要开) 契约功能
#define _HALLOWEEN_EFFECT			// 万圣节特效
#define _ADD_STATUS_2				// 新增传送人物状态第二类
#define _PETSKILL_BATTLE_MODEL		// 宠物技能战斗模组
#define _RACEMAN					// cyg 猎宠大会新npc
#define _PETSKILL_ANTINTER			// (可开) Change 宠技:蚁之葬
#define _PETSKILL_EXPLODE			// (可开) Change 宠技:爆裂攻击
#define _ITEM_FIREWORK				// 烟火功能
#define _FIREWORK_SOUND				// cyg 新增音效(烟火用)
#define _GET_HOSTNAME				// (可开) Change 取得电脑名称
#define _SKILL_ADDBARRIER			// (可开) Change 宠技:为魔障增加异常抗性功能
#define _PETSKILL_PEEL				// (可开) Change 宠技:金蝉脱壳

//#define _MOUSE_DBL_CLICK			// Robin 滑鼠右键双击功能
#endif		//SA_VERSION_75

#ifdef _SA_VERSION_80
//--------------------------------------------------------------------------------------------------------
//专案 8.0 失落的世界 新加功能定义区 人员：彦勋 奇铨 晓威 逸俊
//--------------------------------------------------------------------------------------------------------
#define _PET_ITEM				// Terry 宠物可装备道具
#define _NPC_PICTURE			// (可开) Change 新赌场功能(随时变身的NPC)
#define _JOBDAILY				// cyg 任务日志功能
#define _ITEM_PATH				// (可开) Change 可踏上不可走区域的道具
#define _ALCHEMIST				// Robin 精炼功能 
#define _ALCHEPLUS				// Robin 鉴定功能 
#define _EXTEND_AB				// cyg 扩充名片功能
#define _ITEM_INTENSIFY			// (可开) Change 宠技:强化道具
#define _TEACHER_SYSTEM			// Terry 导师系统
#define _ITEM_UPINSLAY			// (可开) Change 宠技:凿孔(让道具可镶上更多宝石)
#define _MAG_MENU				// (可开) Change 魔法熟练度、抗性由文字显示改为图示化
#define _NPC_MAGICCARD			// cyg 魔法牌游戏
#define _THEATER				// Terry 剧场功能
#define _MOVE_SCREEN			// Terry 移动荧幕到指定的座标
//	#define _80_LOGIN				// Terry 8.0 登入画面
#define _8_RIDEPETS				// (可开) Change 8.0骑宠
#define _8BATTLEMAP				// Change 8.0 战斗地表
#define _PETKILL_COST_FAME		// Terry 宠物技能店学技能要花费声望
#define _DRAGON_FUSION			// Robin 人龙进化

#define _MACADDRESSD			// (可开) Change get MAC card
#define _NEW_EARTH				// Terry 新地球,可看见内部服务器
//#define _80_LOGIN_PLAY		// Terry 8.0 登入画面
//----------------------------------------------------------------------

//#define _ITEM_JIGSAW			// (可开) Change 拼图道具

#define _NPC_DANCE				// cyg 动一动npc
#define _ATTACK_EFFECT			// 角色攻击完后会播放攻击特效

//#define _2005_ValentineDay		// Terry 2005 情人节活动
#define _PETSKILL_ADDATTCRAZED	// (可开) Change 宠技:狂乱暴走增加功能
#define _PETSKILL_JUSTICE		// (可开) Change 宠技:审判
#define _CALLBUS				// (可开) Change 让公车具有被呼叫的功能

//#define _NPC_ITEMUP				// (可开) Change 让道具升级的NPC
//#define _ITEM_COUNTDOWN			// cyg 道具倒数功能

#define _PRO_KILLME				// Change 白狼职技  拾身诱敌
#define _PETSKILL_EVOLUTION		// (可开) Change 北极熊宠技:进化
#define _PETSKILL_PROVOKEFIGHT	// (可开) Change 犬年宠技:挑衅反击
#define _FIX_ACUPUNCTURE		// (可开) Change 针刺外皮的修改
#define _PETSKILL_RIDE			// (可开) Change 宠技:上马术

#endif		// _SA_VERSION_80

#ifdef _SA_VERSION_85
//--------------------------------------------------------------------------------------------------------
//专案 8.5 魔域大冒险 新加功能定义区 人员：奇铨 逸俊
//--------------------------------------------------------------------------------------------------------
#define _PRO_ABSOLUTE_DEFENSE	// (可开) Change 白狼职技  绝对防御
#define _PRO3_ADDSKILL		// (可开) Change 追加8.5职业技能
#define _NPC_DAYACTION		// cyg 每日活动
#define _EVIL_KILL			// cyg 讨伐魔军系统
#define _HUNDRED_KILL			// cyg 百人无双
#define _NEW_CLOUD			// (可开) Change 新的地图的云效果
//#define _85_LOGIN			// (可开) Change 8.5登入画面
//----------------------------------------------------------------------
#define _AI_SKILL_UPDATE		//新AI技能
#define _TEACHER_SYSTEM_2		// cyg 新导师系统
#endif
#endif


/*
新增宏增加
*/
#define _FIX_MAXGOLD			    // (可开放) WON ADD 增加人物金钱上限
#define _MOON_FAIRYLAND				// (可开放) ROG ADD 月之仙境
#define _SPECIALSPACEANIM_FIX		// (可开放) ROG ADD 修正动画
#define _LOGINKICK					// (不可开) Syu ADD 登入画面超过30秒踢回星系列表
//#define _CHECK_GAMESPEED		    // (不可开) ANDY 检测加速
#define _NEW_CLOUD					// (可开) Change 新的地图的云效果
#define _FIX_BATTLE_AI				// 修复战斗AI转换的BUG
#define _WIN_LUAJIT_				//增加LUAJIT扩展功能     支持中文接口和中文路径LUA文件
#define _NEW_COLOR_					//补丁支持16位RGB565高彩图片
#define _NEW_WIN_POS_				//800窗口偏移设置
#define _NEW_ALPHA_					//新建半透明处理
//#define _CACHE_SURFACE_				//缓存表面
//#define _OPTIMIZATIONFLIP_  		//优化Flip
#define _KEY_  "AEB3A729E71F8870D4611FBDC01D0A4B"      //所有加密
//#define _NEWFONT_


#define _MO_SIGN_IN					//客户端签到按钮
#define _CHANGE_HELP_BUTTON  		//修改帮助按钮图形
#define _PETCOM_					//宠物成长自动计算
#define _NEW_SPEED_					//新游戏速度

#define _PETCOLOR_					//1级宠物变色
#define _PETKILL_COST_FAME			// Terry 宠物技能店学技能要花费声
#define _AIDENGLU_					//自动登陆
#define _ZHENGSHIKAIFU_				//正式开服打开
#define _ZENGJIASHUO_				//增加一个锁道具的图标，同时被锁的道具不能选中
#define _BUDING_ "554CDB1A99DF514B2F0B09DBDA0A6E9D"       //补丁加密
#define _TIME_GET_TIME				// 用QueryPerformanceCounter取代timeGetTime
#define _FAMILYBADGE_				//家族徽章
#define _SHOWFAMILYBADGE_			//显示家族徽章
#define _NEWNET_					//新封包机制
//#define _LOGIP_					//登陆时获取IP
//#define _SHOWIPSLEEP_				//显示线路延时
//#define _MEMORYLEAK_CHECK			//内存泄漏检测
//#define _COMFIRM_TRADE_REQUEST		//交易请求确认
#define _DIEJIA_					//叠加功能
#define _FANILYTEXIAOY_  45			//家族徽章偏移Y 越大则越靠窗口顶部
#define _OTHERTEXIAOY_  70			//其它图标偏移Y 越大则越靠窗口顶部
#define _ATTACK_EFFECT				// 角色攻击完后会播放攻击特效
#define _SA_VERSION 'N'
#define _SA_MAC_VERSION_CONTROL		//MAC版本号控制
#define FAMILY_MANOR_				//庄园光环
#define _CHAR_MANOR_                //人物脚下光环
//#define _CHAR_MANOR_DEBUG                //人物脚下光环DEBUG
#define _ANNOUNCEMENT_				//公告系统
//#define _EFFECT_MAP_				//塔特效
#ifndef _DEBUG__
#define _VMP_						//VMP功能开启
//#define _NODEBUG_					//反调试   测试的时候把这个屏蔽了，正式使用则把这个开启
#else
#define _CMD_MODE					//调试模式
//#define _STONDEBUG__PACKET_       //调试封包
//#define _MO_SHOW_FPS				//显示fps
#define _RELUA_						//DEBUG模式下启用按个人信息则重读LUA
#endif
#define SHOW_VERSION "www.shiqi.hk"
//#define _TRADE_BUG_LOG				//交易bug日志
#define _SECONDARY_WINDOW_DATA_		//二级窗口内容
#define LABEL_STR_                 //标记重定义输出[普][密]

#ifdef _SA_VERSION_25
#define _DEFAULT_PKEY "shiqi"		// 默认的密钥
#define _RUNNING_KEY  "shiqi.hk"		// 运行中密钥
#define _SYSTEM_SLEEP_   6  //游戏速度设置
#define _SURFACE_ANIM				// 动态地上物显示
#define _DEFENSETOOENNUM_ 5			//限几开
#define DENGLUKEY1 "6D27AEB8326A2A8E7D04B9F9B44C59C7"
#define _LEADERRIDE	 				   // (可开放) 四大家族骑暴功能 Robin 0921
//#define _MO_IMAGE_EXTENSION			//形象扩展
#define _NEW_ITEM_					//新道具栏
#ifdef _WIN_LUAJIT_
#define _LUA_PATCH_ ".//lua"  //LUA路径
#define _LUA_KEY_ "6A68673107B8C5B829815A507FF5C274"   //LUA加密密钥
#endif
#define _MAGIC_ITEM_             //法宝功能
#define _ICONBUTTONS_				//转盘
#define _BATTLE_TIME_  99000    
#define _PET_2TRANS			// (可开) Change 宠物2转
#define _DEBUGVER_    "2.2"    //DEBUG版本号
#define _NEWSHOP_         //商城
#define _SHOWPETTRN_       //显示宠物转数
#ifdef _SA_MAC_VERSION_CONTROL
#define _SA_MAC_VERSION		"7sa2"
#endif
#define _CHANGETEAM_                //队长切换
#define SA_MUTE      "sa25.exe"
#define _NEW_CHARDATA_				//新人物资料
#define _PETBLESS_					//宠物祝福
#define _RIDEPET_					//骑宠开关
#define DEF_APPNAME		"STONEAGE"	// 游戏标题
#define _RIDEQUERY_					//骑宠查询
#define _CHAT_SPACING 24            //聊天行距设置 20
#define __EMOTION			//Cary 特殊的表情，只做client
//#define _PET_SKINS					//宠物皮肤
#define _PET_2TRANS			// (可开) Change 宠物2转
#define	_NPC_EVENT_NOTICE			//npc任务信息提示
#define _SHOWPETTRN_       //显示宠物转数
//#define _PET_VAL_COLOR_             //宠物属性上色
#define _MO_CHAT_EXPRESSION			//聊天表情功能
#define _SA_LIAOTIAN_				//聊天删减按钮
#define _CHARTITLE_STR_             //文字称号     该称号开启,图形称号失效
#define _FONT_STYLE_                //文字樣式解析
#define _MESSAGE_FRONT_             //聊天信息前面加个符号
#define _TRADITIONAL_LONG_         //繁体系统创建人物登陆
#define _FONTDATALEN_ 100           //聊天信息长度 112
#define MAX_AIRPLANENUM				8
#define _FIX_CG_ATR_ICON
#define _PETSKILL_LER
//#define _CANCEL_FANTI				//取消繁体切换
//#define _RED_MEMOY_				//红包
#endif

#ifdef _MO_CHAT_EXPRESSION
#define CG_FIELD_CHAT_BTN_ON					55102
#define CG_FIELD_CHAT_BTN_OFF					55102
//#define EXPRESSION_NOID_START                   55300 //表情地图号起始ID
//#define EXPRESSION_NOID_END                     55421 //表情地图号结束ID
#define EXPRESSION_NOID_START                   55117 //表情地图号起始ID
#define EXPRESSION_NOID_END                     55215 //表情地图号结束ID
#define EXPRESSION_NOID_NUM       EXPRESSION_NOID_END-EXPRESSION_NOID_START
#define	_SUNDAY_STR_SEARCH			//SUNDAY字符串搜索算法  比KMP更牛速度更快的算法
#endif

