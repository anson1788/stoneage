#ifndef _PC_H_ 
#define _PC_H_

#include "action.h"
#ifdef _MORECHARACTERS_
#define MAXCHARACTER			4
#else
#define MAXCHARACTER			2
#endif
#define CHAR_NAME_LEN			16
#define CHAR_FREENAME_LEN		32
#define MAGIC_NAME_LEN			28
#define MAGIC_MEMO_LEN			72
#define ITEM_NAME_LEN			28
#define ITEM_NAME2_LEN			16
#define ITEM_MEMO_LEN			84
#define PET_NAME_LEN			16
#define PET_FREENAME_LEN		32
#define CHAR_FMNAME_LEN			33      // 家族名称

#ifdef _CHAR_PROFESSION			// WON ADD 人物职业
#define PROFESSION_MEMO_LEN		84
#endif

#ifdef _GM_IDENTIFY		// Rog ADD GM识别
#define GM_NAME_LEN		        32
#endif

//#define CHARNAMELEN				256		// ???????

#define MAX_PET					5

#define MAX_MAGIC				9

#define MAX_PARTY				5

#define	MAX_ADR_BOOK_COUNT		4
#ifdef _EXTEND_AB
	#define	MAX_ADR_BOOK_PAGE		20//20  //10   20050214 cyg 10 add to 20
#else
	#define	MAX_ADR_BOOK_PAGE		10
#endif
#define MAX_ADR_BOOK			(MAX_ADR_BOOK_COUNT*MAX_ADR_BOOK_PAGE)

#ifdef _ITEM_EQUITSPACE
typedef enum
{
    CHAR_HEAD,               
    CHAR_BODY, 
    CHAR_ARM,
    CHAR_DECORATION1,
    CHAR_DECORATION2,

#ifdef _ITEM_EQUITSPACE
	CHAR_EQBELT,
	CHAR_EQSHIELD,
	CHAR_EQSHOES,
#endif

#ifdef _EQUIT_NEWGLOVE
	CHAR_EQGLOVE,
#endif
    CHAR_EQUIPPLACENUM,
#ifdef _PET_ITEM
	PET_HEAD = 0,	// 头
	PET_WING,		// 翼
	PET_TOOTH,		// 牙
	PET_PLATE,		// 身体
	PET_BACK,		// 背
	PET_CLAW,		// 爪
	PET_FOOT,		// 脚(鳍)
	PET_EQUIPNUM
#endif
}CHAR_EquipPlace;

#ifdef _PET_ITEM
typedef enum
{
// 宠物道具,共九种
	ITEM_PET_HEAD = 29,		// 头
	ITEM_PET_WING,			// 翼
	ITEM_PET_TOOTH,			// 牙
	ITEM_PET_PLATE,			// 身体护甲
	ITEM_PET_BACK,			// 背部护甲
	ITEM_PET_CLAW,			// 爪
	ITEM_PET_1_FOOT,		// 脚部,双足
	ITEM_PET_2_FOOT,		// 脚部,四足
	ITEM_PET_FIN,			// 脚部,鳍
	ITEM_CATEGORYNUM  
}ITEM_CATEGORY;
#define MAX_PET_ITEM	7
#endif

#define MAX_ITEMSTART CHAR_EQUIPPLACENUM
#define MAX_MAXHAVEITEM	15
#ifdef _NEW_ITEM_
#define MAX_ITEM (MAX_ITEMSTART+MAX_MAXHAVEITEM*3)
int getItemNum();//判断玩家道具数量();
#else
#define MAX_ITEM (MAX_ITEMSTART+MAX_MAXHAVEITEM)
#endif
#else
#define MAX_ITEMSTART 5
#define MAX_ITEM				20
#endif

#define RESULT_ITEM_COUNT		3
#define RESULT_ITEM_NAME_LEN	24
//#define RESULT_CHR_EXP			4
#define RESULT_CHR_EXP			5

#define SKILL_NAME_LEN			24
#define SKILL_MEMO_LEN			72
#define MAX_SKILL				7



#define MAX_GOLD				1000000
#define MAX_BANKGOLD			10000000
#define MAX_FMBANKGOLD			100000000


#define MAX_PERSONAL_BANKGOLD 50000000

#ifdef _FMVER21
#define FAMILY_MAXMEMBER                100     // 家族人数
#else
#define FAMILY_MAXMEMBER                50     // 家族人数
#endif

enum
{
	PC_ETCFLAG_PARTY		= (1 << 0),
	PC_ETCFLAG_DUEL			= (1 << 1),
	PC_ETCFLAG_CHAT_MODE	= (1 << 2),		//队伍频道开关
	PC_ETCFLAG_MAIL			= (1 << 3),       //名片频道
	PC_ETCFLAG_TRADE		= (1 << 4)
#ifdef _CHANNEL_MODIFY
	,PC_ETCFLAG_CHAT_TELL = (1 << 5)			//密语频道开关
	,PC_ETCFLAG_CHAT_FM		= (1 << 6)			//家族频道开关
#ifdef _CHAR_PROFESSION
	,PC_ETCFLAG_CHAT_OCC	= (1 << 7)			//职业频道开关
#endif
	,PC_ETCFLAG_CHAT_SAVE	=	(1 << 8)			//对话储存开关
#ifdef _CHATROOMPROTOCOL
	,PC_ETCFLAG_CHAT_CHAT	=	(1 << 9)			//聊天室开关
#endif
#endif
#ifdef _CHANNEL_WORLD
	,PC_ETCFLAG_CHAT_WORLD	=	(1 << 10)			//世界频道开关
#endif
#ifdef _CHANNEL_ALL_SERV
	,PC_ETCFLAG_ALL_SERV	=	(1 << 11)			//星球频道开关
#endif
	,PC_AI_MOD = (1 << 12)
};

enum
{
	PC_ETCFLAG_CHAT_MODE_ID = 0
	
#ifdef _CHANNEL_MODIFY
	,PC_ETCFLAG_CHAT_TELL_ID		//密语频道
	,PC_ETCFLAG_CHAT_PARTY_ID		//队伍频道
	,PC_ETCFLAG_CHAT_FM_ID			//家族频道
#ifdef _CHAR_PROFESSION
	,PC_ETCFLAG_CHAT_OCC_ID			//职业频道
#endif
#ifdef _CHATROOMPROTOCOL
	,PC_ETCFLAG_CHAT_CHAT_ID		//聊天室
#endif
#else
	,PC_ETCFLAG_CHAT_PARTY_ID		//队伍频道
#endif
#ifdef _CHANNEL_WORLD
	,PC_ETCFLAG_CHAT_WORLD_ID			//世界频道
#endif
#ifdef _CHANNEL_ALL_SERV
	,PC_ETCFLAG_ALL_SERV_ID			//星球频道开关
#endif
	,PC_ETCFLAG_CHAT_WORLD_NUM
};

#define ITEM_FLAG_PET_MAIL		( 1 << 0 )
#define ITEM_FLAG_MIX			( 1 << 1 )
#define ITEM_FLAG_COOKING_MIX	( 1 << 2 )
#define ITEM_FLAG_METAL_MIX		( 1 << 3 )	//金属
#define ITEM_FLAG_JEWEL_MIX		( 1 << 4 )	//宝石
#define ITEM_FLAG_FIX_MIX		( 1 << 5 )	//修复
#ifdef _ITEM_INTENSIFY
#define ITEM_FLAG_INTENSIFY_MIX ( 1 << 6 )	//强化
#endif
#ifdef _ITEM_UPINSLAY
#define ITEM_FLAG_UPINSLAY_MIX ( 1 << 7 )	//凿孔
#endif

typedef struct
{
	int color;						// ?
	int graNo;						// ???
	int level;						// ???????
#ifdef _ITEM_PILENUMS
	int pile;
#endif
#ifdef _ALCHEMIST //#ifdef _ITEMSET7_TXT
	char alch[4+200];
#endif
	short useFlag;					// ????
	short field;					// ?????
	short target;					// 
	short deadTargetFlag;			// ??????????
	short sendFlag;					// ??????????
	char name[ITEM_NAME_LEN*2+1];		// ????
	char name2[ITEM_NAME2_LEN*2+1];	// ?????
	char memo[ITEM_MEMO_LEN*2+1];		// ??
	char damage[20];
#ifdef _PET_ITEM
	char type;
#endif
#ifdef _ITEM_JIGSAW
	char jigsaw[50];
#endif
#ifdef _NPC_ITEMUP
	int itemup;
#endif
#ifdef _ITEM_COUNTDOWN
	int counttime;
#endif
#ifdef _MAGIC_ITEM_
	int itemType;//道具类型;
#endif
} ITEM;

typedef struct
{
	int graNo;
	int faceGraNo;
	int id;
	int dir;
	int hp, maxHp;
	int mp, maxMp;
	int vital;
	int str, tgh, dex;
	int exp, maxExp;
	int level;
	int atk, def;
	int quick, charm, luck;
	int earth, water, fire, wind;
	int gold;
#ifdef _NEW_MANOR_LAW
	int fame;
#endif
	int titleNo;
	int dp;
	char name[CHAR_NAME_LEN+1];
	char freeName[CHAR_FREENAME_LEN+1];
	short nameColor;
#ifdef _ANGEL_SUMMON
	unsigned status;
#else
	unsigned short status;
#endif
	unsigned short etcFlag;
	short battlePetNo;
	short selectPetNo[MAX_PET];
	short mailPetNo;
#ifdef _STANDBYPET
	short standbyPet;
#endif
	int battleNo;
	short sideNo;
	short helpMode;
	ITEM item[MAX_ITEM];
	ACTION *ptAct;
	int pcNameColor;
	short transmigration;
	char chusheng[61];
	char familyName[CHAR_NAME_LEN+1];
	int familyleader;
	int channel;
	int quickChannel;
	int personal_bankgold;
	int ridePetNo;//宠物形像
	int learnride;
	unsigned int lowsride;
	char ridePetName[CHAR_FREENAME_LEN+1];
	int ridePetLevel;
	int familySprite;
	int baseGraNo;
	ITEM itempool[MAX_ITEM];
	int big4fm;
	int trade_confirm;         // 1 -> 初始值
	                           // 2 -> 慬我方按下确定键
	                           // 3 -> 仅对方按下确定键
	                           // 4 -> 双方皆按下确定键

#ifdef _CHAR_PROFESSION			// WON ADD 人物职业
	int profession_class;
	int profession_level;
//	int profession_exp;
	int profession_skill_point;	
	char profession_class_name[32];
#endif	
#ifdef _ALLDOMAN // (不可开) Syu ADD 排行榜NPC
	int herofloor;
#endif

#ifdef _GM_IDENTIFY		// Rog ADD GM识别
	char gm_name[GM_NAME_LEN+1];
#endif

#ifdef _FRIENDCHANNEL  // ROG ADD 好友频道
	char  chatRoomNum[4];
#endif
#ifdef _STREET_VENDOR
	int iOnStreetVendor;		// 摆摊模式
#endif
	int skywalker; // GM天行者??
#ifdef _MOVE_SCREEN
	BOOL	bMoveScreenMode;	// 移动荧幕模式
	BOOL	bCanUseMouse;		// 是否可以使用滑鼠移动
	int		iDestX;				// 目标点 X 座标
	int		iDestY;				// 目标点 Y 座标
#endif
#ifdef _THEATER
	int		iTheaterMode;		// 剧场模式
	int		iSceneryNumber;		// 记录剧院背景图号
	ACTION	*pActNPC[5];		// 记录剧场中临时产生出来的NPC
#endif
#ifdef _NPC_DANCE
	int     iDanceMode;			// 动一动模式
#endif
#ifdef _EVIL_KILL
	int     newfame; // 讨伐魔军积分
	short   ftype;
#endif

	int debugmode;
#ifdef _SFUMATO
	int sfumato;		// 二次渲染图层色彩
#endif
#ifdef _NEW_ITEM_
	int itemKitStatus//道具栏状态;
#endif
#ifdef _CHARSIGNADY_NO_
	int 签到标记;
#endif
#ifdef _MAGIC_ITEM_
	int magicItemStatus//法宝道具状态;
	int itemRightEffect //道具光环效果;
#endif
} PC;

#ifdef _FMVER21
enum
{
	FMMEMBER_NONE   = -1,  // 未加入任何家族
    FMMEMBER_MEMBER = 1,   // 一般成员
	FMMEMBER_APPLY,        // 申请加入家族
    FMMEMBER_LEADER,       // 家族族长        
	FMMEMBER_ELDER,        // 长老
    //FMMEMBER_INVITE,     // 祭司
    //FMMEMBER_BAILEE,     // 财务长
    //FMMEMBER_VICELEADER, // 副族长
	FMMEMBER_NUMBER,
};
#endif

enum
{
	MAGIC_FIELD_ALL,
	MAGIC_FIELD_BATTLE,
	MAGIC_FIELD_MAP
};

enum
{
	MAGIC_TARGET_MYSELF,
	MAGIC_TARGET_OTHER,
	MAGIC_TARGET_ALLMYSIDE,
	MAGIC_TARGET_ALLOTHERSIDE,
	MAGIC_TARGET_ALL,
	MAGIC_TARGET_NONE,
	MAGIC_TARGET_OTHERWITHOUTMYSELF,
	MAGIC_TARGET_WITHOUTMYSELFANDPET,
	MAGIC_TARGET_WHOLEOTHERSIDE,
	#ifdef __ATTACK_MAGIC
	MAGIC_TARGET_SINGLE,				// 针对敌方某一方
	MAGIC_TARGET_ONE_ROW,				// 针对敌方某一列
	MAGIC_TARGET_ALL_ROWS,				// 针对敌方所有人
	#endif
};

enum
{
	PETSKILL_FIELD_ALL,
	PETSKILL_FIELD_BATTLE,
	PETSKILL_FIELD_MAP
};

enum
{
	PETSKILL_TARGET_MYSELF,
	PETSKILL_TARGET_OTHER,
	PETSKILL_TARGET_ALLMYSIDE,
	PETSKILL_TARGET_ALLOTHERSIDE,
	PETSKILL_TARGET_ALL,
	PETSKILL_TARGET_NONE,
	PETSKILL_TARGET_OTHERWITHOUTMYSELF,
	PETSKILL_TARGET_WITHOUTMYSELFANDPET
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
	,PETSKILL_TARGET_ONE_ROW
	,PETSKILL_TARGET_ONE_LINE
	,PETSKILL_TARGER_DEATH
#endif
#ifdef _SKILL_ADDBARRIER
	,PETSKILL_TARGET_ONE_ROW_ALL //选我方的单排
#endif
};

enum
{
	ITEM_FIELD_ALL,
	ITEM_FIELD_BATTLE,
	ITEM_FIELD_MAP,
};

enum
{
	ITEM_TARGET_MYSELF,
	ITEM_TARGET_OTHER,
	ITEM_TARGET_ALLMYSIDE,
	ITEM_TARGET_ALLOTHERSIDE,
	ITEM_TARGET_ALL,
	ITEM_TARGET_NONE,
	ITEM_TARGET_OTHERWITHOUTMYSELF,
	ITEM_TARGET_WITHOUTMYSELFANDPET,
#ifdef _PET_ITEM
	ITEM_TARGET_PET
#endif
};

typedef struct
{
	int index;						//位置
	int graNo;						// ???
	int hp, maxHp;					// ??????
	int mp, maxMp;					// ??????
	int exp, maxExp;				// ?????????????
	int level;						// ???
	int atk, def;					// ??????????
	int quick;						// ?
	int ai;							// ?
	int earth, water, fire, wind;	// 佋???
	int maxSkill;					// ?????
	int trn;						// 宠物转生数
#ifdef _SHOW_FUSION
	int fusion;						// low word: 宠蛋旗标, hi word: 物种编码
#endif
#ifdef _ANGEL_SUMMON
	unsigned status;
#else
	unsigned short status;			// ?????(??????)
#endif
	char name[CHAR_NAME_LEN+1];		// ?
	char freeName[PET_NAME_LEN+1];	// ???????
	short useFlag;					// ??????????????
	short changeNameFlag;			// ?????????
#ifdef _PET_ITEM
	ITEM item[MAX_PET_ITEM];		// 宠物道具
#endif
#ifdef _PETCOM_
	int oldlevel,oldhp,oldatk,oldquick,olddef;
#endif
#ifdef _RIDEPET_
	int rideflg;
#endif
#ifdef _PETBLESS_
	int blessflg;
	int blesshp;
	int blessatk;
	int blessquick;
	int blessdef;
#endif
} PET;


typedef struct
{
	short useFlag;
	int mp;
	short field;
	short target;
	short deadTargetFlag;
	char name[MAGIC_NAME_LEN+1];
	char memo[MAGIC_MEMO_LEN+1];
} MAGIC;


typedef struct
{
	short useFlag;
	int id;
	int level;
	int maxHp;
	int hp;
	int mp;
	char name[CHAR_NAME_LEN+1];
	ACTION *ptAct;
} PARTY;


typedef struct
{
	short useFlag;
	short onlineFlag;
	int level;
	short transmigration;
	int dp;
	int graNo;
	char name[CHAR_NAME_LEN+1];
#ifdef _MAILSHOWPLANET				// (可开放) Syu ADD 显示名片星球
	char planetname[64];
#endif
} ADDRESS_BOOK;


typedef struct
{
	short petNo;
	short levelUp;
	int exp;
} BATTLE_RESULT_CHR;

typedef struct
{
	short useFlag;
	BATTLE_RESULT_CHR resChr[RESULT_CHR_EXP];
	char item[RESULT_ITEM_COUNT][RESULT_ITEM_NAME_LEN+1];
} BATTLE_RESULT_MSG;


typedef struct
{
	short useFlag;
	short skillId;
	short field;
	short target;
	char name[SKILL_NAME_LEN+1];
	char memo[SKILL_MEMO_LEN+1];
} PET_SKILL;

#ifdef _CHAR_PROFESSION			// WON ADD 人物职业
typedef struct
{
	short useFlag;
	short skillId;
	short target;
	short kind;
	char name[PROFESSION_MEMO_LEN+1];
	char memo[PROFESSION_MEMO_LEN+1];
	int icon;
	int costmp;
	int skill_level;
#ifdef _PRO3_ADDSKILL
	int cooltime;
#endif
} PROFESSION_SKILL;
#endif


typedef struct
{
	char name[CHAR_NAME_LEN+1];		// SJIS?????
	short level;					// ???
	int login;						// ??????

	int faceGraNo;					// ????
	int hp;							// ??
	int str;						// ???
	int def;						// ??
	int agi;						// ?
	int app;						// ?
	int attr[4];					// ?佋???????
	int dp;							// ????????
} CHARLISTTABLE;
#ifdef _AIDENGLU_
typedef struct
{
	int bigArea; //大区;
	int teamMode; //队模;
	int smallArea; //小区;
	int character;//人物;
	int isAutoChat; //是否自动喊话;
	int isAutoMeet; //是否自动遇敌;
	int characterDirection;//人物方向;
	char logonName[4][32];//登陆人物名称[4][32];
	int logonDelay; //登陆延时时间;
}Landed;
#endif

extern PC pc;
extern short maxPcNo;
extern short selectPcNo;
extern short prSendMode;
extern short prSendFlag;
extern short jbSendFlag;
extern short duelSendFlag;

extern int loginDp;

extern short helpFlag;

extern short tradeFlag;
extern short tradeStatus;

//extern short channel;

extern MAGIC magic[];

extern PET pet[];

extern PARTY party[];
extern short partyModeFlag;

extern ADDRESS_BOOK addressBook[];
extern BOOL addressBookFlag;


extern BATTLE_RESULT_MSG battleResultMsg;

extern PET_SKILL petSkill[][MAX_SKILL];

extern CHARLISTTABLE chartable[];

extern char newCharacterName[];
extern int newCharacterGraNo;
extern int newCharacterFaceGraNo;
extern int newCharacterVit;
extern int newCharacterStr;
extern int newCharacterTgh;
extern int newCharacterDex;
extern int newCharacterEarth;
extern int newCharacterWater;
extern int newCharacterFire;
extern int newCharacterWind;
extern int newCharacterHomeTown;

void initPcAll( void );
void initPc( void );
void createPc( int, int, int, int );
void resetPc( void );
void setPcWarpPoint( int, int );
void setPcPoint( void );
void setPcGraNo( int, int );
void setPcId( int );
void setPcDir( int );
void setPcAction( int );
#ifdef __EMOTION
	void setPcEmotion( int emotion );
#endif
int getPcAction( void );

#ifdef _CHAR_PROFESSION			// WON ADD 人物职业
#ifdef _PRO3_ADDSKILL
    #define MAX_PROFESSION_SKILL	30
#else
    #define MAX_PROFESSION_SKILL	26
#endif
    extern PROFESSION_SKILL profession_skill[];
//    #ifdef _GM_IDENTIFY		// Rog ADD GM识别
//      void setPcParam( char *, char *, int, char *, int, int, int, int, int, int, int, int, char *);
//        void setPcParam( char *, char *, int, char *, int, int, int, int, int, int, int, char *);    
//	#else
//      void setPcParam( char *, char *, int, char *, int, int, int, int, int, int, int, int );
#ifdef _ALLDOMAN // (不可开) Syu ADD 排行榜NPC
	void setPcParam( char *, char *, int, char *, int, int, int, int, int, int, int , int );
#else
    void setPcParam( char *, char *, int, char *, int, int, int, int, int, int, int );
#endif
//    #endif
#else
    void setPcParam( char *, char *, int, char *, int, int, int, int );
#endif

void updataPcAct( void );
void setPcLeader( void );
void delPcLeader( void );
void setPcParty( void );
void delPcParty( void );
void setPcWatch( void );
void delPcWatch( void );
void changePcAct( int, int, int, int, int, int, int );
void setPcWalkFlag( void );
void delPcWalkFlag( void );
int checkPcWalkFlag( void );
void setPcUseMagic( void );
void delPcUseMagic( void );
void setPcFukidashi( unsigned int );
void setPcNameColor( int );

void clearPartyParam( void );
void clearPtActPartyParam( void );

int existCharacterListEntry( int );
int cmpNameCharacterList( char * );
int setCharacterList( char *, char * );
int resetCharacterList( int );

void getItem( void );
void swapItem( int, int );

BOOL lookAtAround( void );


void initAddressBook( void );
void addressBookProc( void );

int CHAR_getMaxHaveGold( void);

#endif


