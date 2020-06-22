#include "version.h"
#include "common.h"

#include "char.h"
#include "char_data.h"
#include "item.h"
#include "battle.h"
#include "object.h"
#include "magic_base.h"
#ifdef _MO_ADD_LUADX
#include "enemy.h"
#endif
//0-1999 CHAR_getInt
//2000-3999 CHAR_getChar
//4000-5999 CHAR_getWorkInt
//6000-7999 CHAR_getWorkChar
//8000-9999 CHAR_getFlg
#define LUA_DATALINE0 0
#define LUA_DATALINE1 2000
#define LUA_DATALINE2 4000
#define LUA_DATALINE3 6000
#define LUA_DATALINE4 8000
#define LUA_DATALINE5 10000
#ifdef _JZ_NEWSCRIPT_LUA

lua_const M_Lua_RegConstList[] = {
	{"对像_VIP%", sizeof("对像_VIP%"), LUA_DATALINE5 + 1},
	{"对像_序%", sizeof("对像_序%"), LUA_DATALINE0 + CHAR_DATAPLACENUMBER},
	{"对像_形象%", sizeof("对像_形象%"), LUA_DATALINE0 + CHAR_BASEIMAGENUMBER},
	{"对像_原形%", sizeof("对像_原形%"), LUA_DATALINE0 + CHAR_BASEBASEIMAGENUMBER},
	{"对像_脸%", sizeof("对像_脸%"), LUA_DATALINE0 + CHAR_FACEIMAGENUMBER},
	{"对像_地图%", sizeof("对像_地图%"), LUA_DATALINE0 + CHAR_FLOOR},
	{"对像_X%", sizeof("对像_X%"), LUA_DATALINE0 + CHAR_X},
	{"对像_Y%", sizeof("对像_Y%"), LUA_DATALINE0 + CHAR_Y},
	{"对像_方向%", sizeof("对像_方向%"), LUA_DATALINE0 + CHAR_DIR},
	{"对像_等级%", sizeof("对像_等级%"), LUA_DATALINE0 + CHAR_LV},
	{"对像_石币%", sizeof("对像_石币%"), LUA_DATALINE0 + CHAR_GOLD},
	{"对像_血%", sizeof("对像_血%"), LUA_DATALINE0 + CHAR_HP},
	{"对像_气%", sizeof("对像_气%"), LUA_DATALINE0 + CHAR_MP},
	{"对像_M气%", sizeof("对像_M气%"), LUA_DATALINE0 + CHAR_MAXMP},
	{"对像_体力%", sizeof("对像_体力%"), LUA_DATALINE0 + CHAR_VITAL},
	{"对像_腕力%", sizeof("对像_腕力%"), LUA_DATALINE0 + CHAR_STR},
	{"对像_耐力%", sizeof("对像_耐力%"), LUA_DATALINE0 + CHAR_TOUGH},
	{"对像_速度%", sizeof("对像_速度%"), LUA_DATALINE0 + CHAR_DEX},
	{"对像_魅力%", sizeof("对像_魅力%"), LUA_DATALINE0 + CHAR_CHARM},
	{"对像_运%", sizeof("对像_运%"), LUA_DATALINE0 + CHAR_LUCK},
	{"对像_地%", sizeof("对像_地%"), LUA_DATALINE0 + CHAR_EARTHAT},
	{"对像_水%", sizeof("对像_水%"), LUA_DATALINE0 + CHAR_WATERAT},
	{"对像_火%", sizeof("对像_火%"), LUA_DATALINE0 + CHAR_FIREAT},
	{"对像_风%", sizeof("对像_风%"), LUA_DATALINE0 + CHAR_WINDAT},
	{"对像_战宠%", sizeof("对像_战宠%"), LUA_DATALINE0 + CHAR_DEFAULTPET},
	{"对像_CL%", sizeof("对像_CL%"), LUA_DATALINE0 + CHAR_CRITIAL},
	{"对像_CR%", sizeof("对像_CR%"), LUA_DATALINE0 + CHAR_COUNTER},
	{"对像_珍稀%", sizeof("对像_珍稀%"), LUA_DATALINE0 + CHAR_RARE},
	{"对像_RH%", sizeof("对像_RH%"), LUA_DATALINE0 + CHAR_RADARSTRENGTH},
	{"对像_CE%", sizeof("对像_CE%"), LUA_DATALINE0 + CHAR_CHATVOLUME},
	{"对像_ML%", sizeof("对像_ML%"), LUA_DATALINE0 + CHAR_MERCHANTLEVEL},
	{"对像_记录%", sizeof("对像_记录%"), LUA_DATALINE0 + CHAR_SAVEPOINT},
	{"对像_DM%", sizeof("对像_DM%"), LUA_DATALINE0 + CHAR_DETERMINEITEM},
	{"对像_称号%", sizeof("对像_称号%"), LUA_DATALINE0 + CHAR_INDEXOFEQTITLE},
	{"对像_计毒%", sizeof("对像_计毒%"), LUA_DATALINE0 + CHAR_POISON},
	{"对像_计麻%", sizeof("对像_计麻%"), LUA_DATALINE0 + CHAR_PARALYSIS},
	{"对像_计默%", sizeof("对像_计默%"), LUA_DATALINE0 + CHAR_SLEEP},
	{"对像_计石%", sizeof("对像_计石%"), LUA_DATALINE0 + CHAR_STONE},
	{"对像_计暗%", sizeof("对像_计暗%"), LUA_DATALINE0 + CHAR_DRUNK},
	{"对像_计乱%", sizeof("对像_计乱%"), LUA_DATALINE0 + CHAR_CONFUSION},
	{"对像_登数%", sizeof("对像_登数%"), LUA_DATALINE0 + CHAR_LOGINCOUNT},
	{"对像_死次%", sizeof("对像_死次%"), LUA_DATALINE0 + CHAR_DEADCOUNT},
	{"对像_走次%", sizeof("对像_走次%"), LUA_DATALINE0 + CHAR_WALKCOUNT},
	{"对像_说次%", sizeof("对像_说次%"), LUA_DATALINE0 + CHAR_TALKCOUNT},
	{"对像_伤次%", sizeof("对像_伤次%"), LUA_DATALINE0 + CHAR_DAMAGECOUNT},
	{"对像_捕次%", sizeof("对像_捕次%"), LUA_DATALINE0 + CHAR_GETPETCOUNT},
	{"对像_杀怪%", sizeof("对像_杀怪%"), LUA_DATALINE0 + CHAR_KILLPETCOUNT},
	{"对像_宠死%", sizeof("对像_宠死%"), LUA_DATALINE0 + CHAR_DEADPETCOUNT},
	{"对像_邮数%", sizeof("对像_邮数%"), LUA_DATALINE0 + CHAR_SENDMAILCOUNT},
	{"对像_合数%", sizeof("对像_合数%"), LUA_DATALINE0 + CHAR_MERGEITEMCOUNT},
	{"对像_PK数%", sizeof("对像_PK数%"), LUA_DATALINE0 + CHAR_DUELBATTLECOUNT},
	{"对像_P赢数%", sizeof("对像_P赢数%"), LUA_DATALINE0 + CHAR_DUELWINCOUNT},
	{"对像_P输数%", sizeof("对像_P输数%"), LUA_DATALINE0 + CHAR_DUELLOSECOUNT},
	{"对像_P连数%", sizeof("对像_P连数%"), LUA_DATALINE0 + CHAR_DUELSTWINCOUNT},
	{"对像_P最高%", sizeof("对像_P最高%"), LUA_DATALINE0 + CHAR_DUELMAXSTWINCOUNT},
	{"对像_类型%", sizeof("对像_类型%"), LUA_DATALINE0 + CHAR_WHICHTYPE},
	{"对像_移间%", sizeof("对像_移间%"), LUA_DATALINE0 + CHAR_WALKINTERVAL},
	{"对像_循时%", sizeof("对像_循时%"), LUA_DATALINE0 + CHAR_LOOPINTERVAL},
#ifdef _NEWOPEN_MAXEXP
	{"对像_旧经%", sizeof("对像_旧经%"), LUA_DATALINE0 + CHAR_OLDEXP},
	{"对像_经验%", sizeof("对像_经验%"), LUA_DATALINE0 + CHAR_EXP},
#else
	{"对像_经验%", sizeof("对像_经验%"), LUA_DATALINE0 + CHAR_EXP},
#endif

#ifdef _MO_ADD_LUADX
	{"对像_血MAX%", sizeof("对像_血MAX%"), LUA_DATALINE2 + CHAR_WORKMAXHP},
	{"对像_气MAX%", sizeof("对像_气MAX%"), LUA_DATALINE2 + CHAR_WORKMAXMP},
	{"对像_成长%", sizeof("对像_成长%"), LUA_DATALINE0 + CHAR_ALLOCPOINT},
	{"对像_PETRANK%", sizeof("对像_PETRANK%"), LUA_DATALINE0 + CHAR_PETRANK},
	{"对像_ENEMYBASE编号%", sizeof("对像_ENEMYBASE编号%"), LUA_DATALINE0 + CHAR_PETID},
	{"对像_摆摊%", sizeof("对像_摆摊%"), LUA_DATALINE2 + CHAR_WORKSTREETVENDOR},
	{"对像_看摊者%", sizeof("对像_看摊者%"), LUA_DATALINE2 + CHAR_WORKSTREETVENDOR_WHO},
	{"对像_摊名%", sizeof("对像_摊名%"), LUA_DATALINE0 + CHAR_STREETVENDOR_NAME},
	{"对像_婚否%", sizeof("对像_婚否%"), LUA_DATALINE0 + CHAR_LOVE},
	{"对像_配偶帐%", sizeof("对像_配偶帐%"), LUA_DATALINE0 + CHAR_LOVERID},
	{"对像_配偶名%", sizeof("对像_配偶名%"), LUA_DATALINE0 + CHAR_LOVERNAME},
	{"成长_速度%", sizeof("成长_速度%"), 0},
	{"成长_耐力%", sizeof("成长_耐力%"), 1},
	{"成长_腕力%", sizeof("成长_腕力%"), 2},
	{"成长_体力%", sizeof("成长_体力%"), 3},
	{"对像_速度成长%", sizeof("对像_速度成长%"), 0},
	{"对像_耐力成长%", sizeof("对像_耐力成长%"), 1},
	{"对像_腕力成长%", sizeof("对像_腕力成长%"), 2},
	{"对像_体力成长%", sizeof("对像_体力成长%"), 3},
	{"宠模_编号%", sizeof("宠模_编号%"), LUA_DATALINE0 + E_T_TEMPNO},
	{"宠模_初始%", sizeof("宠模_初始%"), LUA_DATALINE0 + E_T_INITNUM},
	{"宠模_升级点%", sizeof("宠模_升级点%"), E_T_LVUPPOINT},
	{"宠模_体力%", sizeof("宠模_体力%"), LUA_DATALINE0 + E_T_BASEVITAL},
	{"宠模_腕力%", sizeof("宠模_腕力%"), LUA_DATALINE0 + E_T_BASESTR},
	{"宠模_耐力%", sizeof("宠模_耐力%"), LUA_DATALINE0 + E_T_BASETGH},
	{"宠模_速度%", sizeof("宠模_速度%"), LUA_DATALINE0 + E_T_BASEDEX},
	{"宠模_AI%", sizeof("宠模_AI%"), LUA_DATALINE0 + E_T_MODAI},
	{"宠模_GET%", sizeof("宠模_GET%"), LUA_DATALINE0 + E_T_GET},
	{"宠模_地%", sizeof("宠模_地%"), LUA_DATALINE0 + E_T_EARTHAT},
	{"宠模_水%", sizeof("宠模_水%"), LUA_DATALINE0 + E_T_WATERAT},
	{"宠模_火%", sizeof("宠模_火%"), LUA_DATALINE0 + E_T_FIREAT},
	{"宠模_风%", sizeof("宠模_风%"), LUA_DATALINE0 + E_T_WINDAT},
	{"宠模_毒抗%", sizeof("宠模_毒抗%"), LUA_DATALINE0 + E_T_POISON},
	{"宠模_麻抗%", sizeof("宠模_麻抗%"), LUA_DATALINE0 + E_T_PARALYSIS},
	{"宠模_默抗%", sizeof("宠模_默抗%"), LUA_DATALINE0 + E_T_SLEEP},
	{"宠模_石抗%", sizeof("宠模_石抗%"), LUA_DATALINE0 + E_T_STONE},
	{"宠模_醉抗%", sizeof("宠模_醉抗%"), LUA_DATALINE0 + E_T_DRUNK},
	{"宠模_乱抗%", sizeof("宠模_乱抗%"), LUA_DATALINE0 + E_T_CONFUSION},
	{"宠模_宠技1%", sizeof("宠模_宠技1%"), LUA_DATALINE0 + E_T_PETSKILL1},
	{"宠模_宠技2%", sizeof("宠模_宠技2%"), LUA_DATALINE0 + E_T_PETSKILL2},
	{"宠模_宠技3%", sizeof("宠模_宠技3%"), LUA_DATALINE0 + E_T_PETSKILL3},
	{"宠模_宠技4%", sizeof("宠模_宠技4%"), LUA_DATALINE0 + E_T_PETSKILL4},
	{"宠模_宠技5%", sizeof("宠模_宠技5%"), LUA_DATALINE0 + E_T_PETSKILL5},
	{"宠模_宠技6%", sizeof("宠模_宠技6%"), LUA_DATALINE0 + E_T_PETSKILL6},
	{"宠模_宠技7%", sizeof("宠模_宠技7%"), LUA_DATALINE0 + E_T_PETSKILL7},
	{"宠模_珍稀%", sizeof("宠模_珍稀%"), LUA_DATALINE0 + E_T_RARE},
	{"宠模_会心%", sizeof("宠模_会心%"), LUA_DATALINE0 + E_T_CRITICAL},
	{"宠模_反击%", sizeof("宠模_反击%"), LUA_DATALINE0 + E_T_COUNTER},
	{"宠模_技格%", sizeof("宠模_技格%"), LUA_DATALINE0 + E_T_SLOT},
	{"宠模_图档%", sizeof("宠模_图档%"), LUA_DATALINE0 + E_T_IMGNUMBER},
	{"宠模_FLG%", sizeof("宠模_FLG%"), LUA_DATALINE0 + E_T_PETFLG},
	{"宠模_尺寸%", sizeof("宠模_尺寸%"), LUA_DATALINE0 + E_T_SIZE},
	{"宠模_级限%", sizeof("宠模_级限%"), LUA_DATALINE0 + E_T_LIMITLEVEL},
#ifdef _PET_FUSION	
	{"宠模_融合码%", sizeof("宠模_融合码%"), LUA_DATALINE0 + E_T_FUSIONCODE},
#endif	
	{"宠模_原名%", sizeof("宠模_原名%"), LUA_DATALINE1 + E_T_NAME},
#endif

	{"对像_出生%", sizeof("对像_出生%"), LUA_DATALINE0 + CHAR_LASTTALKELDER},
	{"对像_配点%", sizeof("对像_配点%"), LUA_DATALINE0 + CHAR_SKILLUPPOINT},
	{"对像_LT%", sizeof("对像_LT%"), LUA_DATALINE0 + CHAR_LEVELUPPOINT},
	{"对像_图类%", sizeof("对像_图类%"), LUA_DATALINE0 + CHAR_IMAGETYPE},
	{"对像_CLR%", sizeof("对像_CLR%"), LUA_DATALINE0 + CHAR_NAMECOLOR},
	{"对像_话色%", sizeof("对像_话色%"), LUA_DATALINE0 + CHAR_POPUPNAMECOLOR},
	{"对像_设运%", sizeof("对像_设运%"), LUA_DATALINE0 + CHAR_LASTTIMESETLUCK},
	{"对像_DP%", sizeof("对像_DP%"), LUA_DATALINE0 + CHAR_DUELPOINT},
	{"对像_转数%", sizeof("对像_转数%"), LUA_DATALINE0 + CHAR_TRANSMIGRATION},
	{"对像_转参%", sizeof("对像_转参%"), LUA_DATALINE0 + CHAR_TRANSEQUATION},
	{"对像_IA%", sizeof("对像_IA%"), LUA_DATALINE0 + CHAR_INITDATA},
	{"对像_禁时%", sizeof("对像_禁时%"), LUA_DATALINE0 + CHAR_SILENT},
	{"对像_族引%", sizeof("对像_族引%"), LUA_DATALINE0 + CHAR_FMINDEX},
	{"对像_族职%", sizeof("对像_族职%"), LUA_DATALINE0 + CHAR_FMLEADERFLAG},
	{"对像_守精%", sizeof("对像_守精%"), LUA_DATALINE0 + CHAR_FMSPRITE},
	{"对像_个银%", sizeof("对像_个银%"), LUA_DATALINE0 + CHAR_BANKGOLD},
	{"对像_骑宠%", sizeof("对像_骑宠%"), LUA_DATALINE0 + CHAR_RIDEPET},
	{"对像_骑等%", sizeof("对像_骑等%"), LUA_DATALINE0 + CHAR_LEARNRIDE},
	{"对像_咒等%", sizeof("对像_咒等%"), LUA_DATALINE0 + CHAR_LIMITLEVEL},
#ifdef _PET_FUSION
	{"对像_物码%", sizeof("对像_物码%"), LUA_DATALINE0 + CHAR_FUSIONCODE},
	{"对像_孵宠%", sizeof("对像_孵宠%"), LUA_DATALINE0 + CHAR_FUSIONINDEX},
	{"对像_养次%", sizeof("对像_养次%"), LUA_DATALINE0 + CHAR_FUSIONRAISE},
	{"对像_蛋标%", sizeof("对像_蛋标%"), LUA_DATALINE0 + CHAR_FUSIONBEIT},
	{"对像_养时%", sizeof("对像_养时%"), LUA_DATALINE0 + CHAR_FUSIONTIMELIMIT},
#endif
#ifdef _DEATH_CONTEND
	{"对像_队标%", sizeof("对像_队标%"), LUA_DATALINE0 + CHAR_PKLISTTEAMNUM},
	{"对像_领标%", sizeof("对像_领标%"), LUA_DATALINE0 + CHAR_PKLISTLEADER},
#endif
#ifdef _PERSONAL_FAME
	{"对像_声望%", sizeof("对像_声望%"), LUA_DATALINE0 + CHAR_FAME},
#endif
#ifdef _MO_Mercenary
  {"对像_佣兵经验%",       sizeof("对像_佣兵经验%"), LUA_DATALINE0 + CHAR_MERCENARY},  //佣兵经验
#endif
#ifdef _VIP_RIDE
	{"对像_VIP类型%", sizeof("对像_VIP类型%"), LUA_DATALINE0 + CHAR_VIPRIDE},
	{"对像_VIP时间%", sizeof("对像_VIP时间%"), LUA_DATALINE0 + CHAR_VIPTIME},
#endif
#ifdef _VIP_SERVER
	{"对像_积分%", sizeof("对像_积分%"), LUA_DATALINE0 + CHAR_AMPOINT},
#endif
#ifdef _EV_NUM
	{"对像_跑环%", sizeof("对像_跑环%"), LUA_DATALINE0 + CHAR_EVNUM},
#endif
  {"对像_任务链%",     sizeof("对像_任务链%"), LUA_DATALINE0 + CHAR_MISSIONTRAIN_NUM},
  {"对像_任务链时间%", sizeof("对像_任务链时间%"), LUA_DATALINE0 + CHAR_MISSIONTRAIN_TIME},
  {"对像_环数%", sizeof("对像_环数%"), LUA_DATALINE0 + CHAR_MISSIONTRAIN_NUM}, // 跑环数
  {"对像_进化%",       sizeof("对像_进化%"), LUA_DATALINE0 + CHAR_EVOLVE},
  {"对像_在线时间%",   sizeof("对像_在线时间%"), LUA_DATALINE0 + CHAR_ONLINE_TIME},
  {"对像_楼层时间%",   sizeof("对像_楼层时间%"), LUA_DATALINE0 + CHAR_FLOORDIFI_TIME},
  {"对像_极品%",       sizeof("对像_极品%"), LUA_DATALINE0 + CHAR_SUPER},
  {"对像_称号等级%",   sizeof("对像_称号等级%"), LUA_DATALINE0 + CHAR_TITLE_LV},
  {"对像_称号时间%",   sizeof("对像_称号时间%"), LUA_DATALINE0 + CHAR_TITLE_TIME},
  {"对像_任务计时%",   sizeof("对像_任务计时%"), LUA_DATALINE0 + CHAR_MISSION_TIME},

#ifdef _ATTACK_MAGIC
	{"对像_地魔熟%", sizeof("对像_地魔熟%"), LUA_DATALINE0 + CHAR_EARTH_EXP},
	{"对像_水魔熟%", sizeof("对像_水魔熟%"), LUA_DATALINE0 + CHAR_WATER_EXP},
	{"对像_火魔熟%", sizeof("对像_火魔熟%"), LUA_DATALINE0 + CHAR_FIRE_EXP},
	{"对像_风魔熟%", sizeof("对像_风魔熟%"), LUA_DATALINE0 + CHAR_WIND_EXP},
	{"对像_地魔抗%", sizeof("对像_地魔抗%"), LUA_DATALINE0 + CHAR_EARTH_RESIST},
	{"对像_水魔抗%", sizeof("对像_水魔抗%"), LUA_DATALINE0 + CHAR_WATER_RESIST},
	{"对像_火魔抗%", sizeof("对像_火魔抗%"), LUA_DATALINE0 + CHAR_FIRE_RESIST},
	{"对像_风魔抗%", sizeof("对像_风魔抗%"), LUA_DATALINE0 + CHAR_WIND_RESIST},
	{"对像_地熟经%", sizeof("对像_地熟经%"), LUA_DATALINE0 + CHAR_EARTH_ATTMAGIC_EXP},
	{"对像_水熟经%", sizeof("对像_水熟经%"), LUA_DATALINE0 + CHAR_WATER_ATTMAGIC_EXP},
	{"对像_火熟经%", sizeof("对像_火熟经%"), LUA_DATALINE0 + CHAR_FIRE_ATTMAGIC_EXP},
	{"对像_风熟经%", sizeof("对像_风熟经%"), LUA_DATALINE0 + CHAR_WIND_ATTMAGIC_EXP},
	{"对像_地抗经%", sizeof("对像_地抗经%"), LUA_DATALINE0 + CHAR_EARTH_DEFMAGIC_EXP},
	{"对像_水抗经%", sizeof("对像_水抗经%"), LUA_DATALINE0 + CHAR_WATER_DEFMAGIC_EXP},
	{"对像_火抗经%", sizeof("对像_火抗经%"), LUA_DATALINE0 + CHAR_FIRE_DEFMAGIC_EXP},
	{"对像_风抗经%", sizeof("对像_风抗经%"), LUA_DATALINE0 + CHAR_WIND_DEFMAGIC_EXP},
#endif
#ifdef _GAMBLE_BANK
	{"对像_赌银%", sizeof("对像_赌银%"), LUA_DATALINE0 + CHAR_PERSONAGOLD},
#endif
#ifdef _DROPSTAKENEW
	{"对像_赌分%", sizeof("对像_赌分%"), LUA_DATALINE0 + CHAR_GAMBLENUM},
#endif
#ifdef _AUCTIONEER
	{"对像_拍得%", sizeof("对像_拍得%"), LUA_DATALINE0 + CHAR_AUCGOLD},
#endif
#ifdef _PET_EVOLUTION
	{"对像_血药数%", sizeof("对像_血药数%"), LUA_DATALINE0 + CHAR_EVOLUTIONBASEVTL},
	{"对像_攻药数%", sizeof("对像_攻药数%"), LUA_DATALINE0 + CHAR_EVOLUTIONBASESTR},
	{"对像_防药数%", sizeof("对像_防药数%"), LUA_DATALINE0 + CHAR_EVOLUTIONBASETGH},
	{"对像_敏药数%", sizeof("对像_敏药数%"), LUA_DATALINE0 + CHAR_EVOLUTIONBASEDEX},
#endif
#ifdef _FAMILYBANKSTONELOG
	{"对像_族银%", sizeof("对像_族银%"), LUA_DATALINE0 + CHAR_FMBANKGOLD},
#endif
#ifdef _FM_JOINLIMIT
	{"对像_加族间%", sizeof("对像_加族间%"), LUA_DATALINE0 + CHAR_FMTIMELIMIT},
#endif
#ifdef _CHAR_PROFESSION			// WON ADD 人物职业
	{"对像_职业%", sizeof("对像_职业%"), LUA_DATALINE0 + PROFESSION_CLASS},
	{"对像_职等%", sizeof("对像_职等%"), LUA_DATALINE0 + PROFESSION_LEVEL},
	//{"对像_职经", sizeof("对像_职经"), LUA_DATALINE0 + PROFESSION_EXP},
	{"对像_职技点%", sizeof("对像_职技点%"), LUA_DATALINE0 + PROFESSION_SKILL_POINT},
	{"对像_职增叠%", sizeof("对像_职增叠%"), LUA_DATALINE0 + ATTACHPILE},
	{"对像_职火熟%", sizeof("对像_职火熟%"), LUA_DATALINE0 + PROFESSION_FIRE_P},
	{"对像_职冰熟%", sizeof("对像_职冰熟%"), LUA_DATALINE0 + PROFESSION_ICE_P},
	{"对像_职雷熟%", sizeof("对像_职雷熟%"), LUA_DATALINE0 + PROFESSION_THUNDER_P},
	{"对像_职火抗%", sizeof("对像_职火抗%"), LUA_DATALINE0 + PROFESSION_FIRE_R},
	{"对像_职冰抗%", sizeof("对像_职冰抗%"), LUA_DATALINE0 + PROFESSION_ICE_R},
	{"对像_职雷抗%", sizeof("对像_职雷抗%"), LUA_DATALINE0 + PROFESSION_THUNDER_R},
#endif
#ifdef _ALLDOMAN // (不可开) Syu ADD 排行榜NPC
	{"对像_战层%", sizeof("对像_战层%"), LUA_DATALINE0 + CHAR_HEROFLOOR},
#endif
#ifdef _PETSKILL_BECOMEPIG
	{"对像_乌力时%", sizeof("对像_乌力时%"), LUA_DATALINE0 + CHAR_BECOMEPIG},
	{"对像_乌力样%", sizeof("对像_乌力样%"), LUA_DATALINE0 + CHAR_BECOMEPIG_BBI},
#endif
	{"对像_离线时%", sizeof("对像_离线时%"), LUA_DATALINE0 + CHAR_LASTLEAVETIME},
#ifdef _NEW_MANOR_LAW
	{"对像_气势%", sizeof("对像_气势%"), LUA_DATALINE0 + CHAR_MOMENTUM},
#endif
#ifdef _ANGEL_SUMMON
	{"对像_勇数%", sizeof("对像_勇数%"), LUA_DATALINE0 + CHAR_HEROCNT},
#endif
#ifdef _TEACHER_SYSTEM
	{"对像_导声望%", sizeof("对像_导声望%"), LUA_DATALINE0 + CHAR_TEACHER_FAME},
#endif
#ifdef _MO_ESC_REPORT
	{"对像_签到%", sizeof("对像_签到%"), LUA_DATALINE0 + CHAR_REPORT},
#endif
#ifdef _RACEMAN
	{"对像_CKN%", sizeof("对像_CKN%"), LUA_DATALINE0 + CHAR_CHECKIN},
	{"对像_CN1%", sizeof("对像_CN1%"), LUA_DATALINE0 + CHAR_CATCHCNT1},
	{"对像_CN2%", sizeof("对像_CN2%"), LUA_DATALINE0 + CHAR_CATCHCNT2},
	{"对像_CN3%", sizeof("对像_CN3%"), LUA_DATALINE0 + CHAR_CATCHCNT3},
	{"对像_CN4%", sizeof("对像_CN4%"), LUA_DATALINE0 + CHAR_CATCHCNT4},
	{"对像_CN5%", sizeof("对像_CN5%"), LUA_DATALINE0 + CHAR_CATCHCNT5},
	{"对像_KN1%", sizeof("对像_KN1%"), LUA_DATALINE0 + CHAR_KINDCNT1},
	{"对像_KN2%", sizeof("对像_KN2%"), LUA_DATALINE0 + CHAR_KINDCNT2},
	{"对像_KN3%", sizeof("对像_KN3%"), LUA_DATALINE0 + CHAR_KINDCNT3},
	{"对像_KN4%", sizeof("对像_KN4%"), LUA_DATALINE0 + CHAR_KINDCNT4},
	{"对像_KN5%", sizeof("对像_KN5%"), LUA_DATALINE0 + CHAR_KINDCNT5},
	{"对像_KN6%", sizeof("对像_KN6%"), LUA_DATALINE0 + CHAR_KINDCNT6},
	{"对像_KN7%", sizeof("对像_KN7%"), LUA_DATALINE0 + CHAR_KINDCNT7},
	{"对像_KN8%", sizeof("对像_KN8%"), LUA_DATALINE0 + CHAR_KINDCNT8},
	{"对像_KN9%", sizeof("对像_KN9%"), LUA_DATALINE0 + CHAR_KINDCNT9},
	{"对像_KN10%", sizeof("对像_KN10%"), LUA_DATALINE0 + CHAR_KINDCNT10},
#endif
  {"对像_祝福%", sizeof("对像_祝福%"), LUA_DATALINE0 + CHAR_BEATITUDE},
#ifdef	_ACTIVE_GAME
	{"对像_活力%", sizeof("对像_活力%"), LUA_DATALINE0 + CHAR_ACTIVE},
#endif
  {"对像_整数缓存1%",   sizeof("对像_整数缓存1%"), LUA_DATALINE0 + CHAR_LUASAVE_WORKINT_1},  //lua存储 玩家下线清零
  {"对像_整数缓存2%",   sizeof("对像_整数缓存2%"), LUA_DATALINE0 + CHAR_LUASAVE_WORKINT_2},  //lua存储 玩家下线清零
  {"对像_整数缓存3%",   sizeof("对像_整数缓存3%"), LUA_DATALINE0 + CHAR_LUASAVE_WORKINT_3},  //lua存储 玩家下线清零
  {"对像_整数存储1%", sizeof("对像_整数存储1%"), LUA_DATALINE0 + CHAR_LUASAVE_INT_1},      //整数存储，保存至存档内
  {"对像_整数存储2%", sizeof("对像_整数存储2%"), LUA_DATALINE0 + CHAR_LUASAVE_INT_2},      //整数存储，保存至存档内
  {"对像_整数存储3%", sizeof("对像_整数存储3%"), LUA_DATALINE0 + CHAR_LUASAVE_INT_3},      //整数存储，保存至存档内
 // {"对像_IP%",sizeof("对像_IP%"),LUA_DATALINE1 + CHAR_CHARPLAYIP},
 
  {"对像_会员等级%", sizeof("对像_会员等级%"), LUA_DATALINE0 + CHAR_VIPRIDE},
	{"对像_会员时间%", sizeof("对像_会员时间%"), LUA_DATALINE0 + CHAR_VIPTIME},
	
//	{"对像_NPC时间%", sizeof("对像_NPC时间%"), LUA_DATALINE0 + CHAR_NPCTIME},

	{"对像_原名%", sizeof("对像_原名%"), LUA_DATALINE1 + CHAR_NAME},
	{"对像_名称%", sizeof("对像_名称%"), LUA_DATALINE1 + CHAR_USERPETNAME},
	{"对像_帐号%", sizeof("对像_帐号%"), LUA_DATALINE1 + CHAR_CDKEY},
	{"对像_属帐%", sizeof("对像_属帐%"), LUA_DATALINE1 + CHAR_OWNERCDKEY},
	{"对像_属人%", sizeof("对像_属人%"), LUA_DATALINE1 + CHAR_OWNERCHARANAME},
	{"对像_家族名%", sizeof("对像_家族名%"), LUA_DATALINE1 + CHAR_FMNAME},
#ifdef _TEACHER_SYSTEM
	{"对像_导师帐%", sizeof("对像_导师帐%"), LUA_DATALINE1 + CHAR_TEACHER_ID},
	{"对像_导师名%", sizeof("对像_导师名%"), LUA_DATALINE1 + CHAR_TEACHER_NAME},
#endif
#ifdef _MO_LUA_BETT01
	{"对像_战接1%", sizeof("对像_战接1%"), LUA_DATALINE2 + CHAR_WORKBATTLECOM1},
	{"对像_战接2%", sizeof("对像_战接2%"), LUA_DATALINE2 + CHAR_WORKBATTLECOM2},
	{"对像_战模%", sizeof("对像_战模%"), LUA_DATALINE2 + CHAR_WORKBATTLEMODE},
#endif
	{"对像_队模式%", sizeof("对像_队模式%"), LUA_DATALINE2 + CHAR_WORKPARTYMODE},
#ifdef _ATTACK_EFFECT
	{"对像_攻击特效%", sizeof("对像_攻击特效%"), LUA_DATALINE0 + CHAR_ATTACK_EFFECT},
#endif
	{"对像_物索引%", sizeof("对像_物索引%"), LUA_DATALINE2 + CHAR_WORKOBJINDEX},
#ifdef _ADD_ACTION          //npc动作
	{"对像_动作%", sizeof("对像_动作%"), LUA_DATALINE2 + CHAR_WORKACTION},
#endif
//	{"对像_LUA%", sizeof("对像_LUA%"), LUA_DATALINE2 + CHAR_WORKLUANPCINT},
//	{"对像_1LUA%", sizeof("对像_1LUA%"), LUA_DATALINE2 + CHAR_WORKLUANPCINT1},

#ifdef _CHARSIGNDAY_
	{"对像_签到状态%", sizeof("对像_签到状态%"), LUA_DATALINE0 + CHAR_QIANDAOZT},
	{"对像_签到月%", sizeof("对像_签到月%"), LUA_DATALINE0 + CHAR_QIANDAOYUE},
	{"对像_签到日%", sizeof("对像_签到日%"), LUA_DATALINE0 + CHAR_QIANDAORI},
#endif
	{"对像_IAK%", sizeof("对像_IAK%"), LUA_DATALINE4 + CHAR_ISATTACK},
	{"对像_战令%", sizeof("对像_战令%"), LUA_DATALINE4 + CHAR_ISATTACKED},
	{"对像_IOR%", sizeof("对像_IOR%"), LUA_DATALINE4 + CHAR_ISOVER},
	{"对像_档路%", sizeof("对像_档路%"), LUA_DATALINE4 + CHAR_ISOVERED},
	{"对像_HHT%", sizeof("对像_HHT%"), LUA_DATALINE4 + CHAR_HAVEHEIGHT},
	{"对像_隐身%", sizeof("对像_隐身%"), LUA_DATALINE4 + CHAR_ISVISIBLE},
	{"对像_ITP%", sizeof("对像_ITP%"), LUA_DATALINE4 + CHAR_ISTRANSPARENT},
	{"对像_IFG%", sizeof("对像_IFG%"), LUA_DATALINE4 + CHAR_ISFLYING},
	{"对像_死亡%", sizeof("对像_死亡%"), LUA_DATALINE4 + CHAR_ISDIE},
	{"对像_IBIG%", sizeof("对像_IBIG%"), LUA_DATALINE4 + CHAR_ISBIG},
	{"对像_显战%", sizeof("对像_显战%"), LUA_DATALINE4 + CHAR_ISSHOWBATTLEMSG},
	{"对像_组队标%", sizeof("对像_组队标%"), LUA_DATALINE4 + CHAR_ISPARTY},
	{"对像_允许记%", sizeof("对像_允许记%"), LUA_DATALINE4 + CHAR_ISWARP},
	{"对像_族战标%", sizeof("对像_族战标%"), LUA_DATALINE4 + CHAR_ISDUEL},
	{"对像_队聊标%", sizeof("对像_队聊标%"), LUA_DATALINE4 + CHAR_ISPARTYCHAT},
	{"对像_名片标%", sizeof("对像_名片标%"), LUA_DATALINE4 + CHAR_ISTRADECARD},
	{"对像_交易标%", sizeof("对像_交易标%"), LUA_DATALINE4 + CHAR_ISTRADE},
#ifdef _CHANNEL_MODIFY
	{"对像_密频标%", sizeof("对像_密频标%"), LUA_DATALINE4 + CHAR_ISTELL},
	{"对像_族频标%", sizeof("对像_族频标%"), LUA_DATALINE4 + CHAR_ISFM},
	{"对像_职频标%", sizeof("对像_职频标%"), LUA_DATALINE4 + CHAR_ISOCC},
	{"对像_对话存%", sizeof("对像_对话存%"), LUA_DATALINE4 + CHAR_ISSAVE},
	{"对像_聊频标%", sizeof("对像_聊频标%"), LUA_DATALINE4 + CHAR_ISCHAT},
#endif

	{"道具_ID%", sizeof("道具_ID%"), LUA_DATALINE0 + ITEM_ID},
	{"道具_图号%", sizeof("道具_图号%"), LUA_DATALINE0 + ITEM_BASEIMAGENUMBER},
	{"道具_成本%", sizeof("道具_成本%"), LUA_DATALINE0 + ITEM_COST},
	{"道具_类型%", sizeof("道具_类型%"), LUA_DATALINE0 + ITEM_TYPE},
	{"道具_AUF%", sizeof("道具_AUF%"), LUA_DATALINE0 + ITEM_ABLEUSEFIELD},
	{"道具_TARGET%", sizeof("道具_TARGET%"), LUA_DATALINE0 + ITEM_TARGET},
	{"道具_等级%", sizeof("道具_等级%"), LUA_DATALINE0 + ITEM_LEVEL},
#ifdef _ITEM_MAXUSERNUM
	{"道具_次数%", sizeof("道具_次数%"), LUA_DATALINE0 + ITEM_DAMAGEBREAK},
#endif
#ifdef _ITEMSET4_TXT
	{"道具_堆叠%", sizeof("道具_堆叠%"), LUA_DATALINE0 + ITEM_USEPILENUMS},
	{"道具_可叠%", sizeof("道具_可叠%"), LUA_DATALINE0 + ITEM_CANBEPILE},
	{"道具_STR%", sizeof("道具_STR%"), LUA_DATALINE0 + ITEM_NEEDSTR},
	{"道具_DEX%", sizeof("道具_DEX%"), LUA_DATALINE0 + ITEM_NEEDDEX},
	{"道具_转数%", sizeof("道具_转数%"), LUA_DATALINE0 + ITEM_NEEDTRANS},
	{"道具_职业%", sizeof("道具_职业%"), LUA_DATALINE0 + ITEM_NEEDPROFESSION},
#endif
#ifdef _TAKE_ITEMDAMAGE
	{"道具_耐久%", sizeof("道具_耐久%"), LUA_DATALINE0 + ITEM_DAMAGECRUSHE},
	{"道具_MAX耐久%", sizeof("道具_MAX耐久%"), LUA_DATALINE0 + ITEM_MAXDAMAGECRUSHE},
#endif
#ifdef _ADD_DEAMGEDEFC
	{"道具_伤%", sizeof("道具_伤%"), LUA_DATALINE0 + ITEM_OTHERDAMAGE},
	{"道具_吸%", sizeof("道具_吸%"), LUA_DATALINE0 + ITEM_OTHERDEFC},
#endif
#ifdef _SUIT_ITEM
	{"道具_套%", sizeof("道具_套%"), LUA_DATALINE0 + ITEM_SUITCODE},
#endif
	{"道具_AKMIN%", sizeof("道具_AKMIN%"), LUA_DATALINE0 + ITEM_ATTACKNUM_MIN},
	{"道具_AKMAX%", sizeof("道具_AKMAX%"), LUA_DATALINE0 + ITEM_ATTACKNUM_MAX},
	{"道具_附攻%", sizeof("道具_附攻%"), LUA_DATALINE0 + ITEM_MODIFYATTACK},
	{"道具_附防%", sizeof("道具_附防%"), LUA_DATALINE0 + ITEM_MODIFYDEFENCE},
	{"道具_附敏%", sizeof("道具_附敏%"), LUA_DATALINE0 + ITEM_MODIFYQUICK},
	{"道具_附体%", sizeof("道具_附体%"), LUA_DATALINE0 + ITEM_MODIFYHP},
	{"道具_附气%", sizeof("道具_附气%"), LUA_DATALINE0 + ITEM_MODIFYMP},
	{"道具_附运%", sizeof("道具_附运%"), LUA_DATALINE0 + ITEM_MODIFYLUCK},
	{"道具_附魅%", sizeof("道具_附魅%"), LUA_DATALINE0 + ITEM_MODIFYCHARM},
	{"道具_附避%", sizeof("道具_附避%"), LUA_DATALINE0 + ITEM_MODIFYAVOID},
	{"道具_属性%", sizeof("道具_属性%"), LUA_DATALINE0 + ITEM_MODIFYATTRIB},
	{"道具_属性值%", sizeof("道具_属性值%"), LUA_DATALINE0 + ITEM_MODIFYATTRIBVALUE},
	{"道具_精灵%", sizeof("道具_精灵%"), LUA_DATALINE0 + ITEM_MAGICID},
	{"道具_精灵率%", sizeof("道具_精灵率%"), LUA_DATALINE0 + ITEM_MAGICPROB},
	{"道具_精灵耗%", sizeof("道具_精灵耗%"), LUA_DATALINE0 + ITEM_MAGICUSEMP},
#ifdef _ITEMSET5_TXT
	{"道具_格档率%", sizeof("道具_格档率%"), LUA_DATALINE0 + ITEM_MODIFYARRANGE},
	{"道具_攻序%", sizeof("道具_攻序%"), LUA_DATALINE0 + ITEM_MODIFYSEQUENCE},
	{"道具_负重%", sizeof("道具_负重%"), LUA_DATALINE0 + ITEM_ATTACHPILE},
	{"道具_命中%", sizeof("道具_命中%"), LUA_DATALINE0 + ITEM_HITRIGHT},
#endif
#ifdef _ITEMSET6_TXT
	{"道具_忽防%", sizeof("道具_忽防%"), LUA_DATALINE0 + ITEM_NEGLECTGUARD},
#endif
	{"道具_毒耐%", sizeof("道具_毒耐%"), LUA_DATALINE0 + ITEM_POISON},
	{"道具_麻耐%", sizeof("道具_麻耐%"), LUA_DATALINE0 + ITEM_PARALYSIS},
	{"道具_睡耐%", sizeof("道具_睡耐%"), LUA_DATALINE0 + ITEM_SLEEP},
	{"道具_石耐%", sizeof("道具_石耐%"), LUA_DATALINE0 + ITEM_STONE},
	{"道具_酒耐%", sizeof("道具_酒耐%"), LUA_DATALINE0 + ITEM_DRUNK},
	{"道具_混耐%", sizeof("道具_混耐%"), LUA_DATALINE0 + ITEM_CONFUSION},
	{"道具_附一击%", sizeof("道具_附一击%"), LUA_DATALINE0 + ITEM_CRITICAL},
	{"道具_动作%", sizeof("道具_动作%"), LUA_DATALINE0 + ITEM_USEACTION},
	{"道具_登消%", sizeof("道具_登消%"), LUA_DATALINE0 + ITEM_DROPATLOGOUT},
	{"道具_丢消%", sizeof("道具_丢消%"), LUA_DATALINE0 + ITEM_VANISHATDROP},
	{"道具_档路%", sizeof("道具_档路%"), LUA_DATALINE0 + ITEM_ISOVERED},
	{"道具_邮寄%", sizeof("道具_邮寄%"), LUA_DATALINE0 + ITEM_CANPETMAIL},
	{"道具_从合成%", sizeof("道具_从合成%"), LUA_DATALINE0 + ITEM_CANMERGEFROM},
	{"道具_合成到%", sizeof("道具_合成到%"), LUA_DATALINE0 + ITEM_CANMERGETO},
	{"道具_份量0%", sizeof("道具_份量0%"), LUA_DATALINE0 + ITEM_INGVALUE0},
	{"道具_份量1%", sizeof("道具_份量1%"), LUA_DATALINE0 + ITEM_INGVALUE1},
	{"道具_份量2%", sizeof("道具_份量2%"), LUA_DATALINE0 + ITEM_INGVALUE2},
	{"道具_份量3%", sizeof("道具_份量3%"), LUA_DATALINE0 + ITEM_INGVALUE3},
	{"道具_份量4%", sizeof("道具_份量4%"), LUA_DATALINE0 + ITEM_INGVALUE4},
	{"道具_丢时%", sizeof("道具_丢时%"), LUA_DATALINE0 + ITEM_PUTTIME},
	{"道具_LL%", sizeof("道具_LL%"), LUA_DATALINE0 + ITEM_LEAKLEVEL},
	{"道具_曾合%", sizeof("道具_曾合%"), LUA_DATALINE0 + ITEM_MERGEFLG},
	{"道具_损等%", sizeof("道具_损等%"), LUA_DATALINE0 + ITEM_CRUSHLEVEL},
//	{"道具_限时%", sizeof("道具_限时%"), LUA_DATALINE0 + ITEM_TIME},
	{"道具_VAR1%", sizeof("道具_VAR1%"), LUA_DATALINE0 + ITEM_VAR1},
	{"道具_VAR2%", sizeof("道具_VAR2%"), LUA_DATALINE0 + ITEM_VAR2},
	{"道具_VAR3%", sizeof("道具_VAR3%"), LUA_DATALINE0 + ITEM_VAR3},
	{"道具_VAR4%", sizeof("道具_VAR4%"), LUA_DATALINE0 + ITEM_VAR4},

	{"道具_物索%", sizeof("道具_物索%"), LUA_DATALINE2 + ITEM_WORKOBJINDEX},
	{"道具_索引%", sizeof("道具_索引%"), LUA_DATALINE2 + ITEM_WORKCHARAINDEX},
#ifdef _ITEM_TIME_LIMIT
	{"道具_倒时%", sizeof("道具_倒时%"), LUA_DATALINE2 + ITEM_WORKTIMELIMIT},
#endif

	{"道具_原名%", sizeof("道具_原名%"), LUA_DATALINE1 + ITEM_NAME},
	{"道具_名%", sizeof("道具_名%"), LUA_DATALINE1 + ITEM_SECRETNAME},
	{"道具_说明%", sizeof("道具_说明%"), LUA_DATALINE1 + ITEM_EFFECTSTRING},
	{"道具_参%", sizeof("道具_参%"), LUA_DATALINE1 + ITEM_ARGUMENT},
	{"道具_帐%", sizeof("道具_帐%"), LUA_DATALINE1 + ITEM_CDKEY},
	{"道具_爱帐%", sizeof("道具_爱帐%"), LUA_DATALINE1 + ITEM_FORUSERCDKEY},
	{"道具_份0%", sizeof("道具_份0%"), LUA_DATALINE1 + ITEM_INGNAME0},
	{"道具_份1%", sizeof("道具_份1%"), LUA_DATALINE1 + ITEM_INGNAME1},
	{"道具_份2%", sizeof("道具_份2%"), LUA_DATALINE1 + ITEM_INGNAME2},
	{"道具_份3%", sizeof("道具_份3%"), LUA_DATALINE1 + ITEM_INGNAME3},
	{"道具_份4%", sizeof("道具_份4%"), LUA_DATALINE1 + ITEM_INGNAME4},

	{"道栏_头位%", sizeof("道栏_头位%"), CHAR_HEAD},
	{"道栏_身位%", sizeof("道栏_身位%"), CHAR_BODY},
	{"道栏_右手%", sizeof("道栏_右手%"), CHAR_ARM},
	{"道栏_饰1%", sizeof("道栏_饰1%"), CHAR_DECORATION1},
	{"道栏_饰2%", sizeof("道栏_饰2%"), CHAR_DECORATION2},
#ifdef _ITEM_EQUITSPACE
	{"道栏_左手%", sizeof("道栏_左手%"), CHAR_EQSHIELD},
	{"道栏_腰位%", sizeof("道栏_腰位%"), CHAR_EQBELT},
	{"道栏_脚位%", sizeof("道栏_脚位%"), CHAR_EQSHOES},
#endif
#ifdef _EQUIT_NEWGLOVE
	{"道栏_手套%", sizeof("道栏_手套%"), CHAR_EQGLOVE},
#endif
	{"道栏_1%", sizeof("道栏_1%"), CHAR_STARTITEMARRAY + 0},
	{"道栏_2%", sizeof("道栏_2%"), CHAR_STARTITEMARRAY + 1},
	{"道栏_3%", sizeof("道栏_3%"), CHAR_STARTITEMARRAY + 2},
	{"道栏_4%", sizeof("道栏_4%"), CHAR_STARTITEMARRAY + 3},
	{"道栏_5%", sizeof("道栏_5%"), CHAR_STARTITEMARRAY + 4},
	{"道栏_6%", sizeof("道栏_6%"), CHAR_STARTITEMARRAY + 5},
	{"道栏_7%", sizeof("道栏_7%"), CHAR_STARTITEMARRAY + 6},
	{"道栏_8%", sizeof("道栏_8%"), CHAR_STARTITEMARRAY + 7},
	{"道栏_9%", sizeof("道栏_9%"), CHAR_STARTITEMARRAY + 8},
	{"道栏_10%", sizeof("道栏_10%"), CHAR_STARTITEMARRAY + 9},
	{"道栏_11%", sizeof("道栏_11%"), CHAR_STARTITEMARRAY + 10},
	{"道栏_12%", sizeof("道栏_12%"), CHAR_STARTITEMARRAY + 11},
	{"道栏_13%", sizeof("道栏_13%"), CHAR_STARTITEMARRAY + 12},
	{"道栏_14%", sizeof("道栏_14%"), CHAR_STARTITEMARRAY + 13},
	{"道栏_15%", sizeof("道栏_15%"), CHAR_STARTITEMARRAY + 14},

	{"宠栏_1%", sizeof("宠栏_1%"), 0},
	{"宠栏_2%", sizeof("宠栏_2%"), 1},
	{"宠栏_3%", sizeof("宠栏_3%"), 2},
	{"宠栏_4%", sizeof("宠栏_4%"), 3},
	{"宠栏_5%", sizeof("宠栏_5%"), 4},
	
	{"队员_1%", sizeof("队员_1%"), 1},
	{"队员_2%", sizeof("队员_2%"), 2},
	{"队员_3%", sizeof("队员_3%"), 3},
	{"队员_4%", sizeof("队员_4%"), 4},

	{"IT_爪%", sizeof("IT_爪%"), ITEM_FIST},
	{"IT_斧头%", sizeof("IT_斧头%"), ITEM_AXE},
	{"IT_棍棒%", sizeof("IT_棍棒%"), ITEM_CLUB},
	{"IT_枪%", sizeof("IT_枪%"), ITEM_SPEAR},
	{"IT_弓%", sizeof("IT_弓%"), ITEM_BOW},
	{"IT_SHIELD%", sizeof("IT_SHIELD%"), ITEM_SHIELD},
	{"IT_HELM%", sizeof("IT_HELM%"), ITEM_HELM},
	{"IT_ARMOUR%", sizeof("IT_ARMOUR%"), ITEM_ARMOUR},
	{"IT_手环%", sizeof("IT_手环%"), ITEM_BRACELET},
	{"IT_乐器%", sizeof("IT_乐器%"), ITEM_MUSIC},
	{"IT_项链%", sizeof("IT_项链%"), ITEM_NECKLACE},
	{"IT_环%", sizeof("IT_环%"), ITEM_RING},
	{"IT_BELT%", sizeof("IT_BELT%"), ITEM_BELT},
	{"IT_耳环%", sizeof("IT_耳环%"), ITEM_EARRING},
	{"IT_鼻环%", sizeof("IT_鼻环%"), ITEM_NOSERING},
	{"IT_护身符%", sizeof("IT_护身符%"), ITEM_AMULET},
	{"IT_其他%", sizeof("IT_其他%"), ITEM_OTHER},
	{"IT_回旋标%", sizeof("IT_回旋标%"), ITEM_BOOMERANG},
	{"IT_投掷斧%", sizeof("IT_投掷斧%"), ITEM_BOUNDTHROW},
	{"IT_投掷石%", sizeof("IT_投掷石%"), ITEM_BREAKTHROW},
	{"IT_DISH%", sizeof("IT_DISH%"), ITEM_DISH},
#ifdef _ITEM_INSLAY
	{"IT_金属%", sizeof("IT_金属%"), ITEM_METAL},
	{"IT_首饰%", sizeof("IT_首饰%"), ITEM_JEWEL},
#endif
#ifdef _ITEM_CHECKWARES
	{"IT_货物%", sizeof("IT_货物%"), ITEM_WARES},
#endif
#ifdef _ITEM_EQUITSPACE
	{"IT_腰带%", sizeof("IT_腰带%"), ITEM_WBELT},
	{"IT_盾牌%", sizeof("IT_盾牌%"), ITEM_WSHIELD},
	{"IT_鞋子%", sizeof("IT_鞋子%"), ITEM_WSHOES},
#endif
#ifdef _EQUIT_NEWGLOVE 
	{"IT_手套%", sizeof("IT_手套%"), ITEM_WGLOVE},
#endif
#ifdef _JZ_PETEQU
	{"IT_宠头套%", sizeof("IT_宠头套%"), ITEM_PET_HEAD},
	{"IT_宠翅膀%", sizeof("IT_宠翅膀%"), ITEM_PET_WING},
	{"IT_宠牙套%", sizeof("IT_宠牙套%"), ITEM_PET_TOOTH},
	{"IT_宠胸甲%", sizeof("IT_宠胸甲%"), ITEM_PET_BREAST},
	{"IT_宠背甲%", sizeof("IT_宠背甲%"), ITEM_PET_BACK},
	{"IT_宠爪套%", sizeof("IT_宠爪套%"), ITEM_PET_CLAW},
	{"IT_宠护脚2%", sizeof("IT_宠护脚2%"), ITEM_PET_FEET},
	{"IT_宠护脚4%", sizeof("IT_宠护脚4%"), ITEM_PET_FEET4},
	{"IT_宠鳍套%", sizeof("IT_宠鳍套%"), ITEM_PET_FIN},
#endif

	{"家族_没%", sizeof("家族_没%"), FMMEMBER_NONE},
	{"家族_一般%", sizeof("家族_一般%"), FMMEMBER_MEMBER},
	{"家族_申请%", sizeof("家族_申请%"), FMMEMBER_APPLY},
	{"家族_族长%", sizeof("家族_族长%"), FMMEMBER_LEADER},
	{"家族_长老%", sizeof("家族_长老%"), FMMEMBER_ELDER},
	//{"家族_祭司", sizeof("家族_祭司"), FMMEMBER_INVITE},
	//{"家族_财务", sizeof("家族_财务"), FMMEMBER_BAILEE},
	//{"家族_副族", sizeof("家族_副族"), FMMEMBER_VICELEADER},

	{"战类_普%", sizeof("战类_普%"), BATTLE_TYPE_NONE},
	{"战类_人怪%", sizeof("战类_人怪%"), BATTLE_TYPE_P_vs_E},
	{"战类_人人%", sizeof("战类_人人%"), BATTLE_TYPE_P_vs_P},
	{"战类_怪怪%", sizeof("战类_怪怪%"), BATTLE_TYPE_E_vs_E},
	{"战类_观%", sizeof("战类_观%"), BATTLE_TYPE_WATCH},
	{"战类_DP%", sizeof("战类_DP%"), BATTLE_TYPE_DP_BATTLE},
	{"战类_BOSS%", sizeof("战类_BOSS%"), BATTLE_TYPE_BOSS_BATTLE},

	{"战斗_无%", sizeof("战斗_无%"), BATTLE_MODE_NONE},
	{"战斗_初%", sizeof("战斗_初%"), BATTLE_MODE_INIT},
	{"战斗_中%", sizeof("战斗_中%"), BATTLE_MODE_BATTLE},
	{"战斗_结束%", sizeof("战斗_结束%"), BATTLE_MODE_FINISH},
	{"战斗_结束中%", sizeof("战斗_结束中%"), BATTLE_MODE_STOP},
	{"战斗_观初%", sizeof("战斗_观初%"), BATTLE_MODE_WATCHBC},
	{"战斗_观1%", sizeof("战斗_观1%"), BATTLE_MODE_WATCHPRE},
	{"战斗_观2%", sizeof("战斗_观2%"), BATTLE_MODE_WATCHWAIT},
	{"战斗_观3%", sizeof("战斗_观3%"), BATTLE_MODE_WATCHMOVIE},
	{"战斗_观4%", sizeof("战斗_观4%"), BATTLE_MODE_WATCHAFTER},

	{"女孩%", sizeof("女孩%"), CHAR_IMAGETYPE_GIRL},
	{"男孩%", sizeof("男孩%"), CHAR_IMAGETYPE_BOY},
	{"男童%", sizeof("男童%"), CHAR_IMAGETYPE_CHILDBOY},
	{"女童%", sizeof("女童%"), CHAR_IMAGETYPE_CHILDGIRL},
	{"男人%", sizeof("男人%"), CHAR_IMAGETYPE_MAN},
	{"妇女%", sizeof("妇女%"), CHAR_IMAGETYPE_WOMAN},
	{"老人%", sizeof("老人%"), CHAR_IMAGETYPE_OLDMAN},
	{"狗%", sizeof("狗%"), CHAR_IMAGETYPE_DOG},

	{"对像_玩%", sizeof("对像_玩%"), CHAR_TYPEPLAYER},
	{"对像_怪%", sizeof("对像_怪%"), CHAR_TYPEENEMY},
	{"对像_宠%", sizeof("对像_宠%"), CHAR_TYPEPET},
	{"对像_NPC1%", sizeof("对像_NPC1%"), CHAR_TYPEDOOR},
	{"对像_NPC2%", sizeof("对像_NPC2%"), CHAR_TYPEBOX},
	{"对像_NPC3%", sizeof("对像_NPC3%"), CHAR_TYPEMSG},
	{"对像_NPC4%", sizeof("对像_NPC4%"), CHAR_TYPEWARP},
	{"对像_NPC5%", sizeof("对像_NPC5%"), CHAR_TYPESHOP},
	{"对像_NPC6%", sizeof("对像_NPC6%"), CHAR_TYPEHEALER},
	{"对像_NPC7%", sizeof("对像_NPC7%"), CHAR_TYPEOLDMAN},
	{"对像_NPC8%", sizeof("对像_NPC8%"), CHAR_TYPEROOMADMIN},
	{"对像_NPC9%", sizeof("对像_NPC9%"), CHAR_TYPETOWNPEOPLE},
	{"对像_NPC10%", sizeof("对像_NPC10%"), CHAR_TYPEDENGON},
	{"对像_NPC11%", sizeof("对像_NPC11%"), CHAR_TYPEADM},
	{"对像_NPC12%", sizeof("对像_NPC12%"), CHAR_TYPETEMPLE},
	{"对像_NPC13%", sizeof("对像_NPC13%"), CHAR_TYPESTORYTELLER},
	{"对像_NPC14%", sizeof("对像_NPC14%"), CHAR_TYPERANKING},
	{"对像_NPC15%", sizeof("对像_NPC15%"), CHAR_TYPEOTHERNPC},
	{"对像_NPC16%", sizeof("对像_NPC16%"), CHAR_TYPEPRINTPASSMAN},
	{"对像_NPC17%", sizeof("对像_NPC17%"), CHAR_TYPENPCENEMY},
	{"对像_NPC18%", sizeof("对像_NPC18%"), CHAR_TYPEACTION},
	{"对像_NPC19%", sizeof("对像_NPC19%"), CHAR_TYPEWINDOWMAN},
	{"对像_NPC20%", sizeof("对像_NPC20%"), CHAR_TYPESAVEPOINT},
	{"对像_NPC21%", sizeof("对像_NPC21%"), CHAR_TYPEWINDOWHEALER},
	{"对像_NPC22%", sizeof("对像_NPC22%"), CHAR_TYPEITEMSHOP},
	{"对像_NPC23%", sizeof("对像_NPC23%"), CHAR_TYPESTONESHOP},
	{"对像_NPC24%", sizeof("对像_NPC24%"), CHAR_TYPEDUELRANKING},
	{"对像_NPC25%", sizeof("对像_NPC25%"), CHAR_TYPEWARPMAN},
	{"对像_NPC26%", sizeof("对像_NPC26%"), CHAR_TYPEEVENT},
	{"对像_NPC27%", sizeof("对像_NPC27%"), CHAR_TYPEMIC},
	{"对像_NPC28%", sizeof("对像_NPC28%"), CHAR_TYPELUCKYMAN},
	{"对像_NPC29%", sizeof("对像_NPC29%"), CHAR_TYPEBUS},
	{"对像_NPC30%", sizeof("对像_NPC30%"), CHAR_TYPECHARM},
	{"对像_NPC31%", sizeof("对像_NPC31%"), CHAR_TYPECHECKMAN},
	{"对像_NPC32%", sizeof("对像_NPC32%"), CHAR_TYPEJANKEN},
	{"对像_NPC33%", sizeof("对像_NPC33%"), CHAR_TYPETRANSMIGRATION},
	{"对像_NPC34%", sizeof("对像_NPC34%"), CHAR_TYPEFMWARPMAN},
	{"对像_NPC35%", sizeof("对像_NPC35%"), CHAR_TYPEFMSCHEDULEMAN},
	{"对像_NPC36%", sizeof("对像_NPC36%"), CHAR_TYPEMANORSCHEDULEMAN},
#ifdef _SERVICE
	{"对像_NPC37%", sizeof("对像_NPC37%"), CHAR_TYPESTONESERVICEMAN},
#endif
#ifdef _GAMBLE_BANK
	{"对像_NPC38%", sizeof("对像_NPC38%"), CHAR_GAMBLEBANK},
#endif
#ifdef _NEW_WARPMAN
	{"对像_NPC39%", sizeof("对像_NPC39%"), CHAR_NEWNPCMAN},
#endif
#ifdef _MARKET_TRADE
	{"对像_NPC40%", sizeof("对像_NPC40%"), CHAR_MAPTRADEMAN},
#endif
#ifdef _GAMBLE_ROULETTE
	{"对像_NPC41%", sizeof("对像_NPC41%"), CHAR_GAMBLEROULETTE},
	{"对像_NPC42%", sizeof("对像_NPC42%"), CHAR_GAMBLEMASTER},
#endif
#ifdef _TRANSER_MAN
	{"对像_NPC43%", sizeof("对像_NPC43%"), CHAR_TRANSERMANS},
#endif
#ifdef _JZ_NEW_ADDMEMBERPOINT
	{"对像_NPC44%", sizeof("对像_NPC44%"), CHAR_MEMBERMANS},
#endif
#ifdef _NPC_SELLSTH
	{"对像_NPC45%", sizeof("对像_NPC45%"), CHAR_SELLSTHMAN},
#endif
#ifdef _NPC_MAKEPAIR
	{"对像_NPC46%", sizeof("对像_NPC46%"), CHAR_MAKEPAIR},
#endif
#ifdef _NPC_FUSION
	{"对像_NPC47%", sizeof("对像_NPC47%"), CHAR_PETFUSIONMAN},
#endif
#ifdef _PAUCTION_MAN
//	{"对像_NPC48%", sizeof("对像_NPC48%"), CHAR_PAUCTIONMAN},
#endif
#ifdef _ITEM_NPCCHANGE
	{"对像_NPC49%", sizeof("对像_NPC49%"), CHAR_ITEMCHANGENPC},
#endif
#ifdef _CFREE_petskill
	{"对像_NPC50%", sizeof("对像_NPC50%"), CHAR_FREESKILLSHOP},
#endif
#ifdef _PETRACE
	{"对像_NPC51%", sizeof("对像_NPC51%"), CHAR_PETRACEMASTER},
	{"对像_NPC52%", sizeof("对像_NPC52%"), CHAR_PETRACEPET},
#endif
#ifdef _AUCTIONEER
	{"对像_NPC53%", sizeof("对像_NPC53%"), CHAR_TYPEAUCTIONEER},
#endif
#ifdef _ALLDOMAN // (不可开) Syu ADD 排行榜NPC
	{"对像_NPC54%", sizeof("对像_NPC54%"), CHAR_TYPEALLDOMAN},
#endif
	{"对像_NPC55%", sizeof("对像_NPC55%"), CHAR_TYPEPETMAKER},
#ifdef _NPC_WELFARE
	{"对像_NPC56%", sizeof("对像_NPC56%"), CHAR_TYPEWELFARE},
#endif
#ifdef _NPC_VERYWELFARE
	{"对像_NPC57%", sizeof("对像_NPC57%"), CHAR_TYPEVERYWELFARE},
#endif
#ifdef _NPC_WELFARE_2				// WON ADD 职业NPC-2
	{"对像_NPC58%", sizeof("对像_NPC58%"), CHAR_TYPEWELFARE2},
#endif
#ifdef _RACEMAN
	{"对像_NPC59%", sizeof("对像_NPC59%"), CHAR_RACEMAN},
#endif

	{"物件_没%", sizeof("物件_没%"), OBJTYPE_NOUSE},
	{"物件_人%", sizeof("物件_人%"), OBJTYPE_CHARA},
	{"物件_道%", sizeof("物件_道%"), OBJTYPE_ITEM},
	{"物件_币%", sizeof("物件_币%"), OBJTYPE_GOLD},
#ifdef _NPCSERVER_NEW
	{"物件_NPC%", sizeof("物件_NPC%"), OBJTYPE_NPCSCHARA},
#endif
#ifdef _MAP_WARPPOINT
	{"物件_传%", sizeof("物件_传%"), OBJTYPE_WARPPOINT},
#endif

	{"按钮_没%", sizeof("按钮_没%"), WINDOW_BUTTONTYPE_NONE},
	{"按钮_确定%", sizeof("按钮_确定%"), WINDOW_BUTTONTYPE_OK},
	{"按钮_关闭%", sizeof("按钮_关闭%"), WINDOW_BUTTONTYPE_CANCEL},
	{"按钮_是%", sizeof("按钮_是%"), WINDOW_BUTTONTYPE_YES},
	{"按钮_否%", sizeof("按钮_否%"), WINDOW_BUTTONTYPE_NO},
	{"按钮_上一页%", sizeof("按钮_上一页%"), WINDOW_BUTTONTYPE_PREV},
	{"按钮_下一页%", sizeof("按钮_下一页%"), WINDOW_BUTTONTYPE_NEXT},

	{"组队_没队%", sizeof("组队_没队%"), CHAR_PARTY_NONE},
	{"组队_队长%", sizeof("组队_队长%"), CHAR_PARTY_LEADER},
	{"组队_队员%", sizeof("组队_队员%"), CHAR_PARTY_CLIENT},

	{"左上%", sizeof("左上%"), 0},
	{"上%", sizeof("上%"), 1},
	{"右上%", sizeof("右上%"), 2},
	{"右%", sizeof("右%"), 3},
	{"右下%", sizeof("右下%"), 4},
	{"下%", sizeof("下%"), 5},
	{"左下%", sizeof("左下%"), 6},
	{"左%", sizeof("左%"), 7},

	{"白色%", sizeof("白色%"), CHAR_COLORWHITE},
	{"青色%", sizeof("青色%"), CHAR_COLORCYAN},
	{"紫色%", sizeof("紫色%"), CHAR_COLORPURPLE},
	{"蓝色%", sizeof("蓝色%"), CHAR_COLORBLUE},
	{"黄色%", sizeof("黄色%"), CHAR_COLORYELLOW},
	{"绿色%", sizeof("绿色%"), CHAR_COLORGREEN},
	{"红色%", sizeof("红色%"), CHAR_COLORRED},
	{"灰色%", sizeof("灰色%"), CHAR_COLORGRAY},
	{"深蓝色%", sizeof("深蓝色%"), CHAR_COLORBLUE2},
	{"深绿色%", sizeof("深绿色%"), CHAR_COLORGREEN2},
//	{"黑色%", sizeof("黑色%"), CHAR_COLORBLACK},

	{"队更新_OBJ%", sizeof("队更新_OBJ%"), CHAR_N_STRING_OBJINDEX},
	{"队更新_LV%", sizeof("队更新_LV%"), CHAR_N_STRING_LV},
	{"队更新_MAXHP%", sizeof("队更新_MAXHP%"), CHAR_N_STRING_MAXHP},
	{"队更新_HP%", sizeof("队更新_HP%"), CHAR_N_STRING_HP},
	{"队更新_MP%", sizeof("队更新_MP%"), CHAR_N_STRING_MP},
	{"队更新_名字%", sizeof("队更新_名字%"), CHAR_N_STRING_NAME},
	
	{"更新3_HP%", sizeof("更新3_HP%"), CHAR_P_STRING_HP},
	{"更新3_MAXHP%", sizeof("更新3_MAXHP%"), CHAR_P_STRING_MAXHP},
	{"更新3_MP%", sizeof("更新3_MP%"), CHAR_P_STRING_MP},
	{"更新3_MAXMP%", sizeof("更新3_MAXMP%"), CHAR_P_STRING_MAXMP},
	{"更新3_体%", sizeof("更新3_体%"), CHAR_P_STRING_VITAL},
	{"更新3_腕%", sizeof("更新3_腕%"), CHAR_P_STRING_STR},
	{"更新3_耐%", sizeof("更新3_耐%"), CHAR_P_STRING_TOUGH},
	{"更新3_速%", sizeof("更新3_速%"), CHAR_P_STRING_DEX},
	{"更新3_经验%", sizeof("更新3_经验%"), CHAR_P_STRING_EXP},
	{"更新3_下一级经验%", sizeof("更新3_下一级经验%"), CHAR_P_STRING_NEXTEXP},
	{"更新3_等级%", sizeof("更新3_等级%"), CHAR_P_STRING_LV},
	{"更新3_攻击%", sizeof("更新3_攻击%"), CHAR_P_STRING_ATK},
	{"更新3_防御%", sizeof("更新3_防御%"), CHAR_P_STRING_DEF},
	{"更新3_敏捷%", sizeof("更新3_敏捷%"), CHAR_P_STRING_QUICK},
	{"更新3_魅力%", sizeof("更新3_魅力%"), CHAR_P_STRING_CHARM},
	{"更新3_运气%", sizeof("更新3_运气%"), CHAR_P_STRING_LUCK},
	{"更新3_地属性%", sizeof("更新3_地属性%"), CHAR_P_STRING_EARTH},
	{"更新3_水属性%", sizeof("更新3_水属性%"), CHAR_P_STRING_WATER},
	{"更新3_火属性%", sizeof("更新3_火属性%"), CHAR_P_STRING_FIRE},
	{"更新3_风属性%", sizeof("更新3_风属性%"), CHAR_P_STRING_WIND},
	{"更新3_石币%", sizeof("更新3_石币%"), CHAR_P_STRING_GOLD},
	{"更新3_称号%", sizeof("更新3_称号%"), CHAR_P_STRING_TITLE},
	{"更新3_DP%", sizeof("更新3_DP%"), CHAR_P_STRING_DUELPOINT},
	{"更新3_转数%", sizeof("更新3_转数%"), CHAR_P_STRING_TRANSMIGRATION},
	{"更新3_名称%", sizeof("更新3_名称%"), CHAR_P_STRING_NAME},
	{"更新3_昵称%", sizeof("更新3_昵称%"), CHAR_P_STRING_OWNTITLE},
	{"更新3_骑宠%", sizeof("更新3_骑宠%"), CHAR_P_STRING_RIDEPET},
	{"更新3_学骑等级%", sizeof("更新3_学骑等级%"), CHAR_P_STRING_LEARNRIDE},
	{"更新3_形象%", sizeof("更新3_形象%"), CHAR_P_STRING_BASEBASEIMAGENUMBER},
	{"更新3_天行者模式%", sizeof("更新3_天行者模式%"), CHAR_P_STRING_SKYWALKER},
	{"更新3_调试模式%", sizeof("更新3_调试模式%"), CHAR_P_STRING_DEBUGMODE},
	{"精灵_名%", sizeof("精灵_名%"), LUA_DATALINE1 + MAGIC_NAME},
	{"精灵_描述%", sizeof("精灵_描述%"), LUA_DATALINE1 + MAGIC_COMMENT},
	{"精灵_函数名%", sizeof("精灵_函数名%"), LUA_DATALINE1 + MAGIC_FUNCNAME},
	{"精灵_选项%", sizeof("精灵_选项%"), LUA_DATALINE1 + MAGIC_OPTION},
	{"精灵_ID%", sizeof("精灵_ID%"), LUA_DATALINE0 + MAGIC_ID},
	{"精灵_FIELD%", sizeof("精灵_FIELD%"), LUA_DATALINE0 + MAGIC_FIELD},
	{"精灵_目标%", sizeof("精灵_目标%"), LUA_DATALINE0 + MAGIC_TARGET},
	{"精灵_死目标%", sizeof("精灵_死目标%"), LUA_DATALINE0 + MAGIC_TARGET_DEADFLG},
	{"精灵_IDX%", sizeof("精灵_IDX%"), LUA_DATALINE0 + MAGIC_IDX},
	{NULL, 0, 0},
};

#endif
