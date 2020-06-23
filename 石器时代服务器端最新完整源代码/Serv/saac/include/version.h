#ifndef __VERSION_H__
#define __VERSION_H__

/* 请将控制专案的 definition 放在这个档案
 * 然後藉由 remark/unmark 来开关功能
 */

//特殊功能控制----------------------------------------

#define _SAVE_ZIP                //自动备份
#define _IP_VIP                  //IP认证
#define _FIX_FMPOINTTIME						//修正家族占领时间
#define _FAMILYBADGE_								//家族徽章
#define _MORECHARACTERS_     			//多页人物
#define _SQL_BUY_FUNC_ONE_CDKEY		//每个帐号仅能领取一次道具

#define _ATTESTAION_ID 1

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2
	//#define _VERSION_25
#endif

#ifndef _VERSION_25
	#define _TEACHER_SYSTEM   // 导师系统(未开放)
	#define _ALLDOMAN				// (可开放) Syu ADD 排行榜NPC
#endif
	#define _CHANNEL_MODIFY		// 频道功能整合
	#define _NEW_MANOR_LAW		// 新庄园规则
/* -------------------------------------------------------------------
 * 专案：族人职务与家族功能补充
 * 人员：小瑜、彦勋、志伟、小民
 * 说明：
 */
#define _LEADERFLAG_VER		// 嗯、ㄟ、这个.... (可开放)
#define _FAMILY			//  (可开放)
#define _PERSONAL_FAME		// Arminius 8.30 家族个人声望  (可开放)
#define _FMVER21			// CoolFish: 家族族长职务代理人 2001/8/30 (可开放)
#define _CHAR_POOLITEM			// (可开放) ANDY 共用仓库
#define _CHAR_POOLPET			// (可开放) Robin 共用宠物仓库

/* -------------------------------------------------------------------
 * 专案：当机不回朔之人物资料存档方式修正
 * 人员：小瑜、彦勋
 * 说明：修正人物资料存档方式
 */
#define _NewSave		// CoolFish: 2001/10/16 (可开放)

/* -------------------------------------------------------------------
 * 专案：家族ＢＵＧ修正
 * 人员：小瑜
 * 说明：
 */
#define _LEADERFUNCHECK	// CoolFish: 2002/02/06 族长功能错误、禅让错误修正(可开放)

/* -------------------------------------------------------------------
 * 专案：跨星球踢人
 * 人员：彦豪
 * 说明：
 */
#define _WAEI_KICK				// (可开放)(北京不必开)

/* -------------------------------------------------------------------
 * 专案：九大庄园(4.0)
 * 人员：志旺
 * 说明：
 */
#ifndef _VERSION_25
#define _FIX_10_FMPOINT			// (可开放) WON ADD 九大庄园
#endif
#define _FIX_LEADER_ERR			// (可开放) WON ADD 修正族长问题
#define _REMOVE_FM_LOG          // (可开放) WON ADD 移除不必要的家族 LOG

/* -------------------------------------------------------------------
 * 专案：石器6.0
 * 人员：
 * 说明：
 */
#define _SEND_EFFECT		    // (可开放) WON ADD 送下雪、下雨等特效

// -------------------------------------------------------------------
#define _AC_SEND_FM_PK			// (可开放) WON ADD 庄园对战列表储存在AC
#define _ACFMPK_LIST			// (可开放) ANDY 庄园对战列表
/* -------------------------------------------------------------------
 * 专案：石器7.0
 * 人员：
 * 说明：
 */

#define _LOCK_ADD_NAME		// LockNode 增加名字栏位(未开放)
// -------------------------------------------------------------------
#define _SLEEP_CHAR		// Robin 2004/02/12	活跃人物与非活跃人物分开储存

// Terry define start -------------------------------------------------------------------
#define _FM_MODIFY				// 家族功能修改

// Terry define end   -------------------------------------------------------------------

#define _ANGEL_SUMMON	// Robin 精灵的召唤

//--------------------------------------------------------
//by longzoro
#define _SASQL                   // 石器资料库
#ifdef _SASQL
	#define _SQL_BACKGROUND        //网页后台功能   需同时开放_SASQL1
	#define _SQL_REGISTER          //自动注册功能   需同时开放_SASQL1
#endif

#if _ATTESTAION_ID != 11
#define _LOGNZORO_FUNC_ALL       // 龙zoro新增功能
#endif
#ifdef _LOGNZORO_FUNC_ALL
	#define _NEW_VIP_SHOP            // 会员商店
	
	#if _ATTESTAION_ID != 8 && _ATTESTAION_ID != 10 && _ATTESTAION_ID != 21 
		#define _ITEM_PET_LOCKED         // 物品宠物锁
	#endif
	
	#if _ATTESTAION_ID != 3
		#define _ONLINE_COST             // 在线充值
		#define _SQL_BUY_FUNC            // SQL购买功能
		#define _VIPPOINT_OLD_TO_NEW     // 会员点旧转新
	#endif
	
	#if _ATTESTAION_ID == 5
		#define _FORMULATE_AUTO_PK       // 制定自动化PK系统
	#else
		#define _OLDPS_TO_MD5PS         // 把旧的密码转成MD5密码
		#ifdef _NEW_VIP_SHOP
			#if _ATTESTAION_ID != 3
				#define _COST_ITEM             // 充值道具
			#endif
		#endif
		#if _ATTESTAION_ID != 21
			#define _AUTO_BACKUP           // 自动备份
		#endif
		#define _FIX_CAHR_VERSION      // 存档版本更改
		#if _ATTESTAION_ID != 38 && _ATTESTAION_ID != 49
			#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 21 || _ATTESTAION_ID == 23 || _ATTESTAION_ID == 28 || _ATTESTAION_ID >= 30
				#define _LOTTERY_SYSTEM          // 彩票系统
			#endif	
		#endif	
	#endif
#endif

#if _ATTESTAION_ID != 5 && _ATTESTAION_ID != 11
	#define FAMILY_MAX_MEMBER 			100 	// 家族人数
#else
	#define FAMILY_MAX_MEMBER       50		// 家族人数
#endif

#if _ATTESTAION_ID != 11
	#define _CHARADATA_SAVE_SQL      //将数据写入SQL
#endif
#if 1
	#define _ONE_SERVER              // 单机绑定
#endif

#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 21 || _ATTESTAION_ID ==31 || _ATTESTAION_ID ==33 || _ATTESTAION_ID >=35
	#define _ALL_SERV_SEND       // 星球频道
#endif
#if _ATTESTAION_ID == 1 || _ATTESTAION_ID == 2 || _ATTESTAION_ID == 3 || _ATTESTAION_ID == 21 || _ATTESTAION_ID ==31 || _ATTESTAION_ID ==33 || _ATTESTAION_ID ==35 || _ATTESTAION_ID >=42
	#if _ATTESTAION_ID != 43 && _ATTESTAION_ID != 47 && _ATTESTAION_ID != 49 
		//#define _DEATH_FAMILY_LOGIN_CHECK			//删除人数不足之家族或太久未上线之成员
	#endif
#endif


#if _ATTESTAION_ID == 1
	//#define _SET_FMPOINT
	//#define _FAMILY_TOTEM
	#define _NEWCLISETMAC
	#define _FAMILY_PAY_POINT			//家族充值积分
#endif

#if _ATTESTAION_ID == 0
	#define _DEBUG
#endif

#endif



