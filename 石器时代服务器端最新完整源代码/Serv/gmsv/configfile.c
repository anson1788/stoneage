#define __CONFIGFILE_C__
#include "version.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "util.h"
//#include "configfile.h"
//ttom
#include "lssproto_util.h"
#include "configfile.h"
#include "net.h"
//ttom end
#include "npcutil.h"
// Arminius 7.12 login announce
#include "char.h"
#include "char_data.h"
// CoolFish: add 
#include "lssproto_serv.h"
#include "autil.h"

#ifdef _ANGEL_SUMMON
extern struct MissionInfo missionlist[MAXMISSION];
extern struct MissionTable missiontable[MAXMISSIONTABLE];
#endif

extern char* CHAR_setintdata[CHAR_DATAINTNUM];
extern char* CHAR_setchardata[CHAR_DATACHARNUM];

/* É¬ÀÃÃ«âç  ÔÊÔÂÑáÕ°   */
typedef struct tagConfig
{
    /*Ãó·òºë·ÂØ©  (·ß  ¶Á±å·Æ»§Ð×ÖÐØêÉýÒý·ÖòÍ  */
  char    progname[8];
  char    configfilename[32]; /* config°×ÑëÄÌ»ï   */
    unsigned char debuglevel;   /* ·¸ÌïÓÀºëÒÁÃ¬»ï */
  unsigned int  usememoryunit;    /*¶ªÆ¹Øø¼°½»ÍßÓÀÐþÈÓÄÌÊõ */
  unsigned int  usememoryunitnum; /*¶ªÆ¹Øø¼°½»ÍßÓÀÐþÐÑ */
  char    asname[32];         /*Ê§ÊÐËü¼þÐþÈÓ¡õÌï¼°  ó¡*/
  unsigned short  acservport; /*Ê§ÊÐËü¼þÐþÈÓ¡õÌï¼°ºÌ¡õÐþ  */
  char    acpasswd[32];       /*Ê§ÊÐËü¼þÐþÈÓ¡õÌï³ß¼°ÓÉµ©·¥¡õÓñ*/
  char    gsnamefromas[32];   /*
                                 * Ê§ÊÐËü¼þÐþÈÓ¡õÌï¾®ÈÕÎ­ÒüÔÂ
                                 * ±Ø¡õØ©ÈÓ¡õÌïÎçØÆ»¯¼°  ó¡
                                 */
                                 
    // Arminius 7.24 manor pk
  char gsid[32];	// game server chinese id
  unsigned short allowmanorpk;	// is this server allow manor pk
  unsigned short port;        /* ÈÓ¡õÌï¼°½÷ÇÐ°¾ØêºÌ¡õÐþ */
	int				servernumber;	/* ±Ø¡õØ©ÈÓ¡õÌï¼°  Ä¯ */
	int				reuseaddr;	/* Address already used... »¥÷±ÒýÈÕØ¦ÖÐÁÝ¼°Ð×»§±å */
  int             do_nodelay;     /* TCP_NODELAY ±åÔÊÔÂ¾®Éýµ¤¾® */
  int             log_write_time; /* Ì¤Îå³ðÐÄ·òºëÃ«ÔÊÔÂ¾®Éýµ¤¾®£Û */
  int             log_io_time;    /* I/Oòå  ¼°ÁÝÃÞ·´¾®ÔÂ¾®Éýµ¤¾®£Û */
  int             log_game_time;  /* ±Ø¡õØ©¼°ÖÊ  òå  ¼°ÁÝÃÞÃ«·´¾®ÔÂ */
  int             log_netloop_faster; /* netloop_faster ¼°·òºë */
	int				saacwritenum;	/* Ê§ÊÐËü¼þÐþÈÓ¡õÌï³ß¼°ÖÏÚÐ  ±åwriteÔÊÔÂ¾® */
	int				saacreadnum;	/* Ê§ÊÐËü¼þÐþÈÓ¡õÌï¾®ÈÕ¼°dispatch Ã«ÖÏ¼ÔÔÊÔÂ¾® */
  unsigned short fdnum;           /*ÉýÄ¾·ÖØêÎìÉúÛÍÆËÒà¼þÃ«âç  ÔÊÔÂ¾® */
  unsigned short charnum;           /*ÉýÄ¾·ÖØêÎìÉúÛÍÆËÒà¼þÃ«âç  ÔÊÔÂ¾® */
  unsigned int   othercharnum;    /*  ¹«¼°Ö°¼°Æ½ÅÒ·Â¼°ÐÑ  */
  unsigned int objnum;            /* ×óÆ¤³âÄáÛÍÐþ¼°    ÐÑ*/
  unsigned int   petcharnum;   	/* Ê¸ÓÀÐþ¼°ÐÑ    */
  unsigned int itemnum;           /* Ê§ÄÌ  Ø©¼°    ÐÑ*/
  unsigned int battlenum;         /* ÌïÐþ»ï¼°    */
#ifdef _GET_BATTLE_EXP
  unsigned int battleexp;         /* ÌïÐþ»ï¼°    */
#endif
    char    topdir[64];         	/* ÐþÓÀÃó·¸Å«ÒÁÛÍÐþØø  */

    char    mapdir[64];         	/* Ñ¨ÓÀÃó·¸Å«ÒÁÛÍÐþØø  */
    char    maptilefile[64];    	/* Ñ¨ÓÀÃóÉ¬ÀÃ°×ÑëÄÌ»ï  */
    char    battlemapfile[64];    	/* ÌïÐþ»ïÑ¨ÓÀÃóÉ¬ÀÃ°×ÑëÄÌ»ï  */
    char    itemfile[64];       	/* Ê§ÄÌ  Ø©É¬ÀÃ°×ÑëÄÌ»ï  */
    char    invfile[64];        	/*   ³ÄÉ¬ÀÃ°×ÑëÄÌ»ï  */
    char    appearfile[64];     	/* ÇëòØÞË  É¬ÀÃ°×ÑëÄÌ»ï  */
	char	titlenamefile[64];		/* ±¹Ä¯°×ÑëÄÌ»ï   */
	char	titleconfigfile[64];	/* ±¹Ä¯É¬ÀÃ°×ÑëÄÌ»ï   */
	char	encountfile[64];		/* ¾Þ¼þÊÐËü¼þÐþÉ¬ÀÃ°×ÑëÄÌ»ï   */
	char	enemybasefile[64];		/* ³ÄÏæ  É¬ÀÃ°×ÑëÄÌ»ï   */
	char	enemyfile[64];			/* ³ÄÉ¬ÀÃ°×ÑëÄÌ»ï   */
	char	groupfile[64];			/* ºë»ï¡õÃóÉ¬ÀÃ°×ÑëÄÌ»ï   */
	char	magicfile[64];			/* ÈÈÖîÉ¬ÀÃ°×ÑëÄÌ»ï   */
#ifdef _ATTACK_MAGIC
	char  attmagicfile[64];       // ¹¥»÷ÐÔÖäÊõ
#endif

	char	petskillfile[64];		/* Ê¸ÓÀÐþ  ÈÈÖîÉ¬ÀÃ°×ÑëÄÌ»ï   */
    char    itematomfile[64];       /* Ê§ÄÌ  Ø©¼°¼ã    °×ÑëÄÌ»ï */
    char    effectfile[64];     	/* ÉÒÇëÉ¬ÀÃ°×ÑëÄÌ»ï  */
    char    quizfile[64];     		/* ÛÍÄÌÊõÉ¬ÀÃ°×ÑëÄÌ»ï  */


    char    lsgenlog[64];       /*ÈÓ¡õÌï¼°lsgen Ê§ËüÐþÃóÓÀÐþ°×ÑëÄÌ»ï  */

    char    storedir[64];       /*µ©ÐþÊ§·¸Å«ÒÁÛÍÐþØø    */
    char    npcdir[64];         /*NPC¼°É¬ÀÃ°×ÑëÄÌ»ïÃ«  ÈÊ·¸Å«ÒÁÛÍÐþØø   */

    char    logdir[64];         /*
                                 * ·òºë·¸Å«ÒÁÛÍÐþØø
                                 */
    char    logconfname[64];    /*
                                 * ·òºëÉ¬ÀÃ°×ÑëÄÌ»ï
                                 */
    char	chatmagicpasswd[64];	/* ÃñÅÒÓÀÐþ  Ü·ÓÉµ©·¥¡õÓñ */

#ifdef _STORECHAR
    char	storechar[64];
#endif
	unsigned int 	chatmagiccdkeycheck;	/* ÃñÅÒÓÀÐþ  Ü·Æ¥CDKEYÃ«ÃñÄáÓÀÛÍÔÊÔÂ¾® */
  unsigned int    filesearchnum;     /*°×ÑëÄÌ»ïÃ«¸¹³ñÆ¥ÎåÔÂ°×ÑëÄÌ»ï¼°ÐÑ*/
  unsigned int    npctemplatenum;     /*NPC¼°  ¼þÃóÒÁ¡õÐþ°×ÑëÄÌ»ï¼°ÐÑ*/
  unsigned int    npccreatenum;       /*NPC¼°Ï·Ç²°×ÑëÄÌ»ï¼°ÐÑ*/
  unsigned int    walksendinterval;   /* ÐÚÈÊ¼°Ã«ËªÔÂÃÞØÊ */
  unsigned int    CAsendinterval_ms;     /* CAÃ«ËªÔÂÃÞØÊ (ms)*/
  unsigned int    CDsendinterval_ms;     /* CDÃ«ËªÔÂÃÞØÊ (ms)*/
  unsigned int    Onelooptime_ms;     	/* 1»ï¡õÃó±å¾®ØêÔÂÁÝÃÞ */
	unsigned int	Petdeletetime;		/* Ê¸ÓÀÐþ»¥ÛÕ  ÔÊÔÂÁÝÃÞ */
	unsigned int	Itemdeletetime;		/* Ê§ÄÌ  Ø©»¥ÛÕ  ÔÊÔÂÁÝÃÞ */
    /* ·òºëÄÌ¼þ  ¼°Æ½ÅÒ·Â¼°±¾¡õÆ¤Ã«ÔÊÔÂÃÞØÊ */
  unsigned int    CharSavesendinterval;
  unsigned int    addressbookoffmsgnum;  /*
                                          * Ê§ÓñÒÁµ©Æ¤ÓÀÛÍ±å×ó°×·ÂÄÌ¼þ
                                          * ¶ªÓÀ±¾¡õ³âÃ«
                                          * ÖÏ¶ªÓÀ±¾¡õ³âËáÁùÔÂ¾®
                                          */
  unsigned int    protocolreadfrequency;  /*
                                           * Ãó·òÐþÎì»ïÃ«ÖÏÁ¢Øø
                                           * ±å  ¸ê¾®
                                           */
  unsigned int    allowerrornum;          /*
                                           * ¾Þ·Â¡õÃ«ÖÏòÛÒýÆ¥¸¤ÔÊ¾®
                                           */
  unsigned int    loghour;          		/*
                                           * ·òºëÃ«âçÐåÔÊÔÂÁÝ¶Ô  £¢ÁÝ
                                           */
  unsigned int    battledebugmsg;    		/*
                                           * ÌïÐþ»ï  ¼°·¸ÌïÓÀºë¶ªÓÀ±¾¡õ³âÃ«ÇëÔÊ¾®£Û¨ßØ¦ÈÕÇë½ñØ¦ÖÐ
                                           */
  //ttom add this because the second had this                                         
  unsigned int    encodekey;              
  unsigned int    acwbsize;             
  unsigned int    acwritesize;
  unsigned int    ErrUserDownFlg;
  //ttom end
#ifdef _GMRELOAD
	char	gmsetfile[64];				/* GMÕÊºÅ¡¢È¨ÏÞÉè¶¨µµ */
#endif

#ifdef _PROFESSION_SKILL			// WON ADD ÈËÎïÖ°Òµ¼¼ÄÜ
	char profession[32];
#endif
#ifdef _ITEM_QUITPARTY
    char itemquitparty[32];
#endif

#ifdef _DEL_DROP_GOLD	
	unsigned int	Golddeletetime;
#endif
#ifdef _NEW_PLAYER_CF
	int	newplayertrans;
	int	newplayerlv;
	int	newplayergivepet[5];
	int newplayergiveitem[15];
	int	newplayerpetlv;
	int newplayergivegold;
	int ridepetlevel;
#ifdef _VIP_SERVER
	int	newplayerpetvip;
#endif
#endif
#ifdef _USER_EXP_CF
	char	expfile[64];
#endif
#ifdef _UNLAW_WARP_FLOOR
	char	unlawwarpfloor[512];
#endif
#ifdef _NO_JOIN_FLOOR
	char	nojoinfloor[512];
#endif
#ifdef _WATCH_FLOOR
	int	watchfloor[6];
#endif
#ifdef _BATTLE_FLOOR
	int	battlefloor;
	char battlefloorcf[512];
#endif
#ifdef _UNREG_NEMA
	char	unregname[5][16];
#endif
#ifdef _TRANS_LEVEL_CF
	int chartrans;
	int pettrans;
	int yblevel;
	int maxlevel;
#endif
#ifdef _POINT
int point;
	int transpoint[8];
#endif
#ifdef _VIP_SERVER
	int vippoint;
#endif
#ifdef _PET_AND_ITEM_UP
	int petup;
	int itemup;
#endif
#ifdef _LOOP_ANNOUNCE
	char loopannouncepath[32];
	int loopannouncetime;
	char loopannounce[10][1024];
	int loopannouncemax;
#endif
#ifdef _SKILLUPPOINT_CF
	int skup;
#endif
#ifdef _RIDELEVEL
	int ridelevel;
	int ridetrans;
#endif
#ifdef _REVLEVEL
	int revlevel;
#endif
#ifdef _NEW_PLAYER_RIDE
	int npride;
#endif
#ifdef _FIX_CHARLOOPS
	int charloops;
#endif
#ifdef _PLAYER_ANNOUNCE
	int pannounce;
#endif
#ifdef _PLAYER_MOVE
	int pmove;
#endif
	int recvbuffer;
	int sendbuffer;
	int recvlowatbuffer;
	int runlevel;
#ifdef _SHOW_VIP_CF
	int showvip;
#endif
#ifdef _PLAYER_NUM
	int playernum;
#endif
#ifdef _BATTLE_GOLD
	int battlegold;
#endif
#ifdef _ANGEL_TIME
	int angelplayertime;
	int angelplayermun;
#endif
#ifdef _RIDEMODE_20
	int ridemode;
#endif
#ifdef _FM_POINT_PK
	int fmpointpk;
#endif
#ifdef _ENEMY_ACTION
	int	enemyact;
#endif
#ifdef _FUSIONBEIT_TRANS
	char	fusionbeittrans;
#endif
#ifdef _CHECK_PEPEAT
	int	CheckRepeat;
#endif
	int	cpuuse;
#ifdef _FM_JOINLIMIT
	int	joinfamilytime;
#endif
#ifdef _MAP_HEALERALLHEAL
	char	mapheal[256];
#endif
#ifdef _THE_WORLD_SEND
	int	thewordtrans;
	int	thewordlevel;
	int	thewordsend;
#endif
#ifdef _LOGIN_DISPLAY
	int	logindisplay;
#endif
#ifdef _VIP_POINT_PK
	char	vippointpk[256];
	int vippointpkcost;
#endif
#ifdef _SPECIAL_MAP
	char specialmap[256];
#endif
#ifdef _NEW_AUTO_PK
	int	autopk;
	int	autopktrans;
	int	autopklv;
#ifdef _FORMULATE_AUTO_PK
	int	autopkpoint;
	int	killpoint;
#endif
#endif
#ifdef _AUTO_DEL_PET
	char autodelpet[256];
#endif
#ifdef _AUTO_DEL_ITEM
	char autodelitem[256];
#endif
#ifdef _BT_PET
	int btpet;
#endif
#ifdef _BT_ITEM
	int btitem;
#endif
#ifdef _LUCK_STAR
	int luckstartime;
	int luckstarchances;
#endif
#ifdef _BATTLE_GETITEM_RATE
	char battlegetitemrate[256];
	int battlegetitemratemap;
#endif
#ifdef _BOUND_TIME_EXPANSION
	char unboundtimemap[64];
	int unboundtimelevel;
	int unboundtimetrans;
#endif

#ifdef _UNLAW_THIS_LOGOUT
	char unlawthislogout[256];
#endif

#ifdef _TRANS_POINT_UP
	char transpointup[256];
#endif
#ifdef _OPEN_STW_SEND
	int stwsendtype;
	int stwsendpoint;
#endif
#ifdef _POOL_ITEM_BUG
	int poolitembug;
	char poolitem[256];
#endif
#ifdef _NO_STW_ENEMY
	int nostwenemy;
	int nostwenemypoint;
#endif
#ifdef _NEW_STREET_VENDOR
	char streetvendorpoint[256];
#endif
#ifdef _ITEM_PET_LOCKED
	int itampetlocked;
#endif
#ifdef _TALK_SAVE
	int savefame;
#endif
#ifdef _TALK_CHECK
	int talkcheckmax;
	int talkcheckmin;
#endif
#ifdef _DISABLE_PROFESSION_SKILL
	char disableprofessionskill[256];
#endif
#ifdef _ALL_SERV_SEND
	int	allservtrans;
	int	allservlevel;
	int	allservsend;
#endif
#ifdef _PET_TRANS_ABILITY
	int	pettransability;
	int	pettransability1;
	int	pettransability2;
#endif
#ifdef _NEED_ITEM_ENEMY
	int delneeditem;
#endif
#ifdef _NOT_ESCAPE
	char notescape[256];
#endif
#ifdef _PLAYER_OVERLAP_PK
	char playeroverlappk[256];
#endif
#ifdef _FIMALY_PK_TIME
	int fimalypktime;
#endif
#ifdef _PETSKILL_SHOP_LUA
	char freepetskillshoppath[256];
#endif
#ifdef _CANCEL_ANGLE_TRANS
	int cancelanlgetrans;
#endif
#ifdef _VIP_BATTLE_EXP
	int vipbattleexp;
#endif
#ifdef _NO_HELP_MAP
	char nohelpmap[256];
#endif
#ifdef _BATTLE_TIME
	int battletime;
#endif
#ifdef _SAME_IP_ONLINE_NUM
	int sameiponlinenum;
#endif
#ifdef _STREET_VENDOR_TRANS
	int streetvendortrans;
#endif
#ifdef _CHECK_SEVER_IP
	char serverip[256];
#endif
#ifdef _DAMMAGE_CALC
	int dammagecalc;
#endif
#ifdef _PET_ENEMY_DEVELOP_UP
	int PetEnemyDevelopUp;
#endif
#ifdef _FIRST_LOCK_ITEM
	char FirstLockItem[256];
#endif
	int Connectnum;
#ifdef _PETSKILL_NEW_PASSIVE
int autogetskill;
int getskillpos;
char skillinfolv[5][100];
int skillfusion;
char fusionrange[50];
int skillcount;
#endif
#ifdef _SHARE_EXP
	int expshare;
#endif
#ifdef _DEFEND_BIGBAO
  int bigbao;
	int bigbao2;
#endif
#ifdef _MO_SHOW_DEBUG
  int isdebug;
#endif
#ifdef _CHAR_LOOP_TIME
	int charlooptime;
#endif
#ifdef _MO_RELOAD_NPC
	int reloadnpctime;
	int reloadnpctype;
#endif
#ifdef _JZ_NEWSCRIPT_LUA
	char luafile[256];
#endif
#ifdef _ITEM_LUA
char itemluafile[256];
#endif
#ifdef _MO_LNS_NLSUOXU
  int mapstart;
#endif
#ifdef _ROOKIE_ITEM
  unsigned int rookieitem[5];
#endif
#ifdef _NO_TRANS_ITEM
  unsigned int notransitem;
#endif

#ifdef _MAX_MERGE_LEVEL
  unsigned int maxmergelevel;
#endif

#ifdef _NO_ATTACK
	int atttime;
	int attsafetime;
	int attcnt;
	int latetime;
	int attdmetime;
	int attdmecnt;
#endif
	char noattip[5][18];
#ifdef _NO_FULLPLAYER_ATT
	int nofullplayer;
	int nofull2player;
	int nocdkeyplayer;
	int nocdkeymode;
	int nocdkeytype;
	int nofulltime;
	int fengtype;
	int nofullendplayer;
	int nofullendtime;
	int manrennum;
#endif
	int locktype;
#ifdef _NEW_FUNC_DECRYPT
	int allowerrornum2;
#endif
#ifdef _MO_LOGIN_NO_KICK
  	 int loginnokick;
#endif
#ifdef _MO_ILLEGAL_NAME
	char illegalname[256];
#endif
#ifdef _NO_USE_PACKET_MAP
	char nousepacketmap[256];
#endif
#ifdef _NO_USE_MAGIC_MAP
	char nousemagicmap[256];
#endif
#ifdef _SOME_PETMAIL
	int petmailflg;
	char somepetmail[256];
#endif
#ifdef _CTRL_TRANS_DEVELOP
	int ctrltrans;
#endif
#ifdef	_PETMAIL_TIME
	int petmailtime;
#endif

#ifdef _UP_BBPETPROB
	int upbbprob;
#endif
}Config;

Config config;

#ifdef _USER_EXP_CF
int NeedLevelUpTbls[200];
#endif

/*
 *  Îì¼þ°×Å«ºë°×ÑëÄÌ»ïÃ«  ¸êÁÝ±åÒøµ¤ÑáÕ°
 *  xxxx=yyyy ÐÄÐ×ÖÐØ¦¼°Ã«  ¸ê
 */

typedef struct tagReadConf
{
    char    name[32];       /*xxxx±åØ¤Ð×ÔÂ°À*/

    /*ÆÝ¼°2¹´·´NULLÃ«  Ä¾ÔÂÎçÖÏÊÖÖÊ  ØÆØ¦ÖÐ*/
    char  *charvalue;      /*yyyyÃ«¹«¼°ÒýÒýÒ½  ÔÊÔÂÁÝ¼°Ò½  ÛÆ*/
    size_t  charsize;       /*charvalue¼°ÈÓÄÌÊõ*/

    /*
     *     ¿á  ÎçØÆ»¯=¼°  »¥ "ON"·ÖÔÈÐ×ÈÕ intvalue ±å·´1Ã«Ò½  ÔÊÔÂ
     * ¹«Ä¾¶¯Â½·´ atoi ¼°çø°í
     */
    void*    value;       /*yyyyÃ«  ¾§ÔÊÔÂÒ½  ÔÊÔÂÁÝ¼°Ò½  ÛÆ*/
    CTYPE    valuetype;
}ReadConf;

ReadConf readconf[]=
{
    { "debuglevel"      , NULL ,0 , (void*)&config.debuglevel      ,CHAR},

    { "usememoryunit"   , NULL ,0 , (void*)&config.usememoryunit   ,INT},
    { "usememoryunitnum", NULL ,0 , (void*)&config.usememoryunitnum,INT},

    { "acserv",			config.asname,sizeof(config.asname) ,NULL , 0},
    { "acservport",		NULL ,0 , (void*)&config.acservport     ,SHORT},
    { "acpasswd",		config.acpasswd,sizeof( config.acpasswd),NULL,0},
    { "gameservname",	config.gsnamefromas,sizeof(config.gsnamefromas),
     NULL,0},

    // Arminius 7.24 manor pk
    { "gameservid", config.gsid, sizeof(config.gsid), NULL, 0}, 
    { "allowmanorpk", NULL, 0, (void*)&config.allowmanorpk, SHORT},

    { "port",			NULL ,0 , (void*)&config.port           ,SHORT},
    { "servernumber",	NULL ,0 , (void*)&config.servernumber           ,INT},

    { "reuseaddr",			NULL ,0 , (void*)&config.reuseaddr  ,		INT},
    { "nodelay",			NULL , 0 , (void*)&config.do_nodelay ,		INT},
    { "log_write_time", 	NULL, 0 , (void*)&config.log_write_time,	INT},
    { "log_io_time", 		NULL, 0 , (void*)&config.log_io_time, 		INT},
    { "log_game_time",		NULL, 0 , (void*)&config.log_game_time,		INT},
    { "log_netloop_faster", NULL,0,(void*)&config.log_netloop_faster,	INT},
    { "saacwritenum",		NULL,0,(void*)&config.saacwritenum, 		INT},
    { "saacreadnum",		NULL,0,(void*)&config.saacreadnum, 			INT},
    { "fdnum",				NULL ,0 , (void*)&config.fdnum,				SHORT},
    { "charnum",				NULL ,0 , (void*)&config.charnum,				SHORT},
    { "petnum",				NULL ,0 , (void*)&config.petcharnum,		INT},
    { "othercharnum",		NULL ,0 , (void*)&config.othercharnum,		INT},

    { "objnum",			NULL ,0 , (void*)&config.objnum,				INT},
    { "itemnum",		NULL ,0 , (void*)&config.itemnum,				INT},
    { "battlenum",		NULL ,0 , (void*)&config.battlenum,				INT},
#ifdef _GET_BATTLE_EXP
    { "battleexp",		NULL ,0 , (void*)&config.battleexp,				INT},
#endif
    { "topdir"          , config.topdir,sizeof(config.topdir),NULL,0},
    { "mapdir"          , config.mapdir,sizeof(config.mapdir),NULL,0},
    { "maptilefile"     , config.maptilefile,sizeof(config.maptilefile),NULL,0},
    { "battlemapfile"   , config.battlemapfile,sizeof(config.battlemapfile),NULL,0},

#ifdef _ITEMSET6_TXT
	{ "itemset6file",	config.itemfile,	sizeof(config.invfile),	NULL,	0},
#else
#ifdef _ITEMSET5_TXT
	{ "itemset5file",	config.itemfile,	sizeof(config.invfile),	NULL,	0},
#else
#ifdef _ITEMSET4_TXT
	{ "itemset4file"  , config.itemfile,sizeof(config.invfile),NULL,0},
#else
#ifdef _ITEMSET3_ITEM
	{ "itemset3file"  , config.itemfile,sizeof(config.invfile),NULL,0},
#endif
#endif
#endif
#endif
  { "invinciblefile"  , config.invfile,sizeof(config.invfile),NULL,0},
  { "appearpositionfile"  , config.appearfile,sizeof(config.appearfile),NULL,0},
	{ "titlenamefile", config.titlenamefile, sizeof( config.titlenamefile),NULL,0},
	{ "titleconfigfile", config.titleconfigfile, sizeof( config.titleconfigfile),NULL,0},
	{ "encountfile", config.encountfile, sizeof( config.encountfile),NULL,0},
	{ "enemyfile", config.enemyfile, sizeof( config.enemyfile),NULL,0},
	{ "enemybasefile", config.enemybasefile, sizeof( config.enemybasefile),NULL,0},
	{ "groupfile", config.groupfile, sizeof( config.groupfile),NULL,0},
	{ "magicfile", config.magicfile, sizeof( config.magicfile),NULL,0},
#ifdef _ATTACK_MAGIC
	{ "attmagicfile" , config.attmagicfile , sizeof( config.attmagicfile )  , NULL , 0 },
#endif

#ifdef _PETSKILL2_TXT
	{ "petskillfile2", config.petskillfile, sizeof( config.petskillfile),NULL,0},
#else
	{ "petskillfile1", config.petskillfile, sizeof( config.petskillfile),NULL,0},
#endif

    { "itematomfile" , config.itematomfile, sizeof( config.itematomfile),NULL,0},
    { "effectfile"  , config.effectfile,sizeof(config.effectfile),NULL,0},
    { "quizfile"  , config.quizfile,sizeof(config.quizfile),NULL,0},

    { "lsgenlogfilename", config.lsgenlog,sizeof(config.lsgenlog),NULL,0},
#ifdef _GMRELOAD
	{ "gmsetfile", config.gmsetfile, sizeof( config.gmsetfile),NULL,0},
#endif

    { "storedir"        ,config.storedir,sizeof(config.storedir),NULL,0},
    { "npcdir"          ,config.npcdir,sizeof(config.npcdir),NULL,0},
    { "logdir"          ,config.logdir,sizeof(config.logdir),NULL,0},
    { "logconfname"     ,config.logconfname,sizeof(config.logconfname),NULL,0},
    { "chatmagicpasswd", config.chatmagicpasswd, sizeof( config.chatmagicpasswd),NULL,0},
#ifdef _STORECHAR
    { "storechar", config.storechar, sizeof( config.storechar),NULL,0},
#endif
    { "chatmagiccdkeycheck",  NULL,0, &config.chatmagiccdkeycheck,INT},
    { "filesearchnum",  NULL,0, &config.filesearchnum,INT},
    { "npctemplatenum",  NULL,0, &config.npctemplatenum,INT},
    { "npccreatenum",    NULL,0, &config.npccreatenum,INT},
    { "walkinterval" ,NULL,0,(void*)&config.walksendinterval,INT},
    { "CAinterval" ,NULL,0,(void*)&config.CAsendinterval_ms,INT},
    { "CDinterval" ,NULL,0,(void*)&config.CDsendinterval_ms,INT},
    { "CharSaveinterval" ,NULL,0,(void*)&config.CharSavesendinterval,INT},
    { "Onelooptime" ,NULL,0,(void*)&config.Onelooptime_ms,INT},
    { "Petdeletetime" ,NULL,0,(void*)&config.Petdeletetime,INT},
    { "Itemdeletetime" ,NULL,0,(void*)&config.Itemdeletetime,INT},
	 { "addressbookoffmesgnum" ,NULL,0,
      (void*)&config.addressbookoffmsgnum,INT},

    { "protocolreadfrequency" ,NULL,0,
      (void*)&config.protocolreadfrequency,INT},

    { "allowerrornum" ,NULL,0,(void*)&config.allowerrornum,INT},
    { "loghour" ,NULL,0,(void*)&config.loghour,INT},
    { "battledebugmsg" ,NULL,0,(void*)&config.battledebugmsg,INT},
    //ttom add because the second had
    { "encodekey" ,NULL,0,(void*)&config.encodekey,INT},
    { "acwritesize" ,NULL,0,(void*)&config.acwritesize,INT},
    { "acwbsize" ,NULL,0,(void*)&config.acwbsize,INT},
    { "erruser_down" ,NULL,0,(void*)&config.ErrUserDownFlg,INT},    

#ifdef _PROFESSION_SKILL			// WON ADD ÈËÎïÖ°Òµ¼¼ÄÜ
    { "profession",		config.profession, sizeof(config.profession) ,NULL , 0},
#endif

#ifdef _ITEM_QUITPARTY
    { "itemquitparty",	config.itemquitparty, sizeof(config.itemquitparty) ,NULL , 0},
#endif

#ifdef _DEL_DROP_GOLD
	{ "Golddeletetime" ,NULL,0,(void*)&config.Golddeletetime,	INT},
#endif

#ifdef _NEW_PLAYER_CF
	{ "TRANS" ,NULL,0,(void*)&config.newplayertrans,	INT},
	{ "LV" ,NULL,0,(void*)&config.newplayerlv,	INT},	
	{ "PET1" ,NULL,0,(void*)&config.newplayergivepet[1],	INT},
	{ "PET2" ,NULL,0,(void*)&config.newplayergivepet[2],	INT},	
	{ "PET3" ,NULL,0,(void*)&config.newplayergivepet[3],	INT},	
	{ "PET4" ,NULL,0,(void*)&config.newplayergivepet[4],	INT},	
	{ "ITEM1" ,NULL,0,(void*)&config.newplayergiveitem[0],	INT},
	{ "ITEM2" ,NULL,0,(void*)&config.newplayergiveitem[1],	INT},	
	{ "ITEM3" ,NULL,0,(void*)&config.newplayergiveitem[2],	INT},	
	{ "ITEM4" ,NULL,0,(void*)&config.newplayergiveitem[3],	INT},	
	{ "ITEM5" ,NULL,0,(void*)&config.newplayergiveitem[4],	INT},
	{ "ITEM6" ,NULL,0,(void*)&config.newplayergiveitem[5],	INT},	
	{ "ITEM7" ,NULL,0,(void*)&config.newplayergiveitem[6],	INT},	
	{ "ITEM8" ,NULL,0,(void*)&config.newplayergiveitem[7],	INT},	
	{ "ITEM9" ,NULL,0,(void*)&config.newplayergiveitem[8],	INT},
	{ "ITEM10" ,NULL,0,(void*)&config.newplayergiveitem[9],	INT},	
	{ "ITEM11" ,NULL,0,(void*)&config.newplayergiveitem[10],	INT},	
	{ "ITEM12" ,NULL,0,(void*)&config.newplayergiveitem[11],	INT},	
	{ "ITEM13" ,NULL,0,(void*)&config.newplayergiveitem[12],	INT},	
	{ "ITEM14" ,NULL,0,(void*)&config.newplayergiveitem[13],	INT},	
	{ "ITEM15" ,NULL,0,(void*)&config.newplayergiveitem[14],	INT},
	{ "PETLV" ,NULL,0,(void*)&config.newplayerpetlv,	INT},	
	{ "GOLD" ,NULL,0,(void*)&config.newplayergivegold,	INT},
	{ "RIDEPETLEVEL" ,NULL,0,(void*)&config.ridepetlevel,	INT},
#ifdef _VIP_SERVER
	{ "GIVEVIPPOINT" ,NULL,0,(void*)&config.newplayerpetvip,	INT},
#endif
#endif

#ifdef _USER_EXP_CF
	{ "USEREXP", config.expfile, sizeof( config.expfile),NULL,0},
#endif

#ifdef _UNLAW_WARP_FLOOR
	{ "UNLAWWARPFLOOR" ,config.unlawwarpfloor, sizeof( config.unlawwarpfloor),NULL,0},
#endif

#ifdef _NO_JOIN_FLOOR
	{ "NOJOINFLOOR" ,config.nojoinfloor, sizeof( config.nojoinfloor),NULL,0},
#endif

#ifdef _WATCH_FLOOR
	{ "WATCHFLOOR" ,NULL,0,(void*)&config.watchfloor[0],	INT},
	{ "WATCHFLOOR1" ,NULL,0,(void*)&config.watchfloor[1],	INT},
	{ "WATCHFLOOR2" ,NULL,0,(void*)&config.watchfloor[2],	INT},	
	{ "WATCHFLOOR3" ,NULL,0,(void*)&config.watchfloor[3],	INT},	
	{ "WATCHFLOOR4" ,NULL,0,(void*)&config.watchfloor[4],	INT},	
	{ "WATCHFLOOR5" ,NULL,0,(void*)&config.watchfloor[5],	INT},
#endif

#ifdef _BATTLE_FLOOR
	{ "BATTLEFLOOR" ,NULL,0,(void*)&config.battlefloor,	INT},
	{ "BATTLEFLOORCF" ,config.battlefloorcf, sizeof( config.battlefloorcf),NULL,0},
#endif

#ifdef _UNREG_NEMA
	{ "NAME1" ,config.unregname[0], sizeof( config.unregname[0]),NULL,0},
	{ "NAME2" ,config.unregname[1], sizeof( config.unregname[1]),NULL,0},
	{ "NAME3" ,config.unregname[2], sizeof( config.unregname[2]),NULL,0},
	{ "NAME4" ,config.unregname[3], sizeof( config.unregname[3]),NULL,0},
	{ "NAME5" ,config.unregname[4], sizeof( config.unregname[4]),NULL,0},
#endif
#ifdef _TRANS_LEVEL_CF
	{ "CHARTRANS" ,NULL,0,(void*)&config.chartrans,	INT},
	{ "PETTRANS" ,NULL,0,(void*)&config.pettrans,	INT},	
	{ "LEVEL" ,NULL,0,(void*)&config.yblevel,	INT},	
	{ "MAXLEVEL" ,NULL,0,(void*)&config.maxlevel,	INT},	
#endif
#ifdef _POINT
	{ "POINT" ,NULL,0,(void*)&config.point,	INT},
	{ "TRANS0" ,NULL,0,(void*)&config.transpoint[0],	INT},	
	{ "TRANS1" ,NULL,0,(void*)&config.transpoint[1],	INT},	
	{ "TRANS2" ,NULL,0,(void*)&config.transpoint[2],	INT},	
	{ "TRANS3" ,NULL,0,(void*)&config.transpoint[3],	INT},	
	{ "TRANS4" ,NULL,0,(void*)&config.transpoint[4],	INT},	
	{ "TRANS5" ,NULL,0,(void*)&config.transpoint[5],	INT},	
	{ "TRANS6" ,NULL,0,(void*)&config.transpoint[6],	INT},	
	{ "TRANS7" ,NULL,0,(void*)&config.transpoint[7],	INT},	
#endif

#ifdef _PET_AND_ITEM_UP
	{ "PETUP" ,NULL,0,(void*)&config.petup,	INT},
	{ "ITEMUP" ,NULL,0,(void*)&config.itemup,	INT},
#endif
#ifdef _LOOP_ANNOUNCE
	{ "ANNOUNCEPATH" ,config.loopannouncepath, sizeof( config.loopannouncepath),NULL,0},
	{ "ANNOUNCETIME" ,NULL,0,(void*)&config.loopannouncetime,	INT},
#endif
#ifdef _SKILLUPPOINT_CF
	{ "SKILLUPPOINT" ,NULL,0,(void*)&config.skup,	INT},	
#endif
#ifdef _RIDELEVEL
	{ "RIDELEVEL" ,NULL,0,(void*)&config.ridelevel,	INT},	
	{ "RIDETRANS" ,NULL,0,(void*)&config.ridetrans,	INT},	
#endif

#ifdef _REVLEVEL
	{ "REVLEVEL" ,NULL,0,(void*)&config.revlevel,	INT},	
#endif
#ifdef _NEW_PLAYER_RIDE
	{ "NPRIDE" ,NULL,0,(void*)&config.npride,	INT},	
#endif
#ifdef _FIX_CHARLOOPS
	{ "CHARLOOPS" ,NULL,0,(void*)&config.charloops,	INT},	
#endif
#ifdef _PLAYER_ANNOUNCE
	{ "PANNOUNCE" ,NULL,0,(void*)&config.pannounce,	INT},
#endif
#ifdef _PLAYER_MOVE
	{ "PMOVE" ,NULL,0,(void*)&config.pmove,	INT},
#endif

	{ "recvbuffer" ,NULL,0,(void*)&config.recvbuffer,	INT},
	{ "sendbuffer" ,NULL,0,(void*)&config.sendbuffer,	INT},
	{ "recvlowatbuffer" ,NULL,0,(void*)&config.recvlowatbuffer,	INT},
	{ "runlevel" ,NULL,0,(void*)&config.runlevel,	INT},
	
#ifdef _SHOW_VIP_CF
	{ "SHOWVIP" ,NULL,0,(void*)&config.showvip,	INT},
#endif

#ifdef _PLAYER_NUM
	{ "PLAYERNUM" ,NULL,0,(void*)&config.playernum,	INT},
#endif

#ifdef _BATTLE_GOLD
	{ "BATTLEGOLD" ,NULL,0,(void*)&config.battlegold,	INT},
#endif
#ifdef _ANGEL_TIME
	{ "ANGELPLAYERTIME" ,NULL,0,(void*)&config.angelplayertime,	INT},
	{ "ANGELPLAYERMUN" ,NULL,0,(void*)&config.angelplayermun,	INT},
#endif
#ifdef _RIDEMODE_20
	{ "RIDEMODE" ,NULL,0,(void*)&config.ridemode,	INT},
#endif
#ifdef _FM_POINT_PK
	{ "FMPOINTPK" ,NULL,0,(void*)&config.fmpointpk,	INT},
#endif
#ifdef _ENEMY_ACTION
	{ "ENEMYACTION" ,NULL,0,(void*)&config.enemyact,	INT},
#endif
#ifdef _FUSIONBEIT_TRANS
	{ "FUSIONBEIT" ,NULL,0,(void*)&config.fusionbeittrans,	INT},
#endif
#ifdef _CHECK_PEPEAT
	{ "CHECKPEPEAT" ,NULL,0,(void*)&config.CheckRepeat,	INT},
#endif
	{ "CPUUSE" ,NULL,0,(void*)&config.cpuuse,	INT},
#ifdef _FM_JOINLIMIT
	{ "JOINFAMILYTIME" ,NULL,0,(void*)&config.joinfamilytime,	INT},
#endif
#ifdef _MAP_HEALERALLHEAL
	{ "MAPHEAL" ,config.mapheal, sizeof( config.mapheal),NULL,0},
#endif

#ifdef _THE_WORLD_SEND
	{ "THEWORLDTRANS" ,NULL,0,(void*)&config.thewordtrans,	INT},
	{ "THEWORLDLEVEL" ,NULL,0,(void*)&config.thewordlevel,	INT},
	{ "THEWORLDSNED" ,NULL,0,(void*)&config.thewordsend,	INT},
#endif
#ifdef _LOGIN_DISPLAY
	{ "LOGINDISPLAY" ,NULL,0,(void*)&config.logindisplay,	INT},
#endif
#ifdef _VIP_POINT_PK
	{ "PKMAP" ,config.vippointpk, sizeof( config.vippointpk),NULL,0},
	{ "PKMAPCOST" ,NULL,0,(void*)&config.vippointpkcost,	INT},
#endif
#ifdef _SPECIAL_MAP
	{ "SPECIALMAP" ,config.specialmap, sizeof( config.specialmap),NULL,0},
#endif
#ifdef _NEW_AUTO_PK
	{ "AUTOPK" ,NULL,0,(void*)&config.autopk,	INT},
	{ "AUTOPKTRANS" ,NULL,0,(void*)&config.autopktrans,	INT},
	{ "AUTOPKLV" ,NULL,0,(void*)&config.autopklv,	INT},
#ifdef _FORMULATE_AUTO_PK
	{ "AUTOPKPOINT" ,NULL,0,(void*)&config.autopkpoint,	INT},
	{ "KILLPOINT" ,NULL,0,(void*)&config.killpoint,	INT},
#endif
#endif
#ifdef _AUTO_DEL_PET
	{ "AUTODELPET" ,config.autodelpet, sizeof( config.autodelpet),NULL,0},
#endif
#ifdef _AUTO_DEL_ITEM
	{ "AUTODELITEM" ,config.autodelitem, sizeof( config.autodelitem),NULL,0},
#endif
#ifdef _BT_PET
	{ "BTPET" ,NULL,0,(void*)&config.btpet,	INT},
#endif
#ifdef _BT_ITEM
	{ "BTITEM" ,NULL,0,(void*)&config.btitem,	INT},
#endif
#ifdef _LUCK_STAR
	{ "LUCKSTARTIME" ,NULL,0,(void*)&config.luckstartime,	INT},
	{ "LUCKSTARCHANCES" ,NULL,0,(void*)&config.luckstarchances,	INT},
#endif
#ifdef _BATTLE_GETITEM_RATE
	{ "BATTLEGETITEMRATE" ,config.battlegetitemrate, sizeof( config.battlegetitemrate),NULL,0},
	{ "BATTLEGETITEMRATEMAP" ,NULL,0,(void*)&config.battlegetitemratemap,	INT},
#endif
#ifdef _UNLAW_THIS_LOGOUT
	{ "UNLAWTHISLOGOUT" ,config.unlawthislogout, sizeof( config.unlawthislogout),NULL,0},
#endif
#ifdef _TRANS_POINT_UP
	{ "TRANSPOINTUP" ,config.transpointup, sizeof( config.transpointup),NULL,0},
#endif
#ifdef _OPEN_STW_SEND
	{ "STWSENDTYPE" ,NULL,0,(void*)&config.stwsendtype,	INT},
	{ "STWSENDPOINT" ,NULL,0,(void*)&config.stwsendpoint,	INT},
#endif
#ifdef _POOL_ITEM_BUG
	{ "POOLITEMBUG" ,NULL,0,(void*)&config.poolitembug,	INT},
	{ "POOLITEM" ,config.poolitem, sizeof( config.poolitem),NULL,0},
#endif
#ifdef _NO_STW_ENEMY
	{ "NOSTWENEMY" ,NULL,0,(void*)&config.nostwenemy,	INT},
	{ "NOSTWENEMYGOLD" ,NULL,0,(void*)&config.nostwenemypoint,	INT},
#endif
#ifdef _NEW_STREET_VENDOR
	{ "STREETVENDORPOINT" ,config.streetvendorpoint, sizeof( config.streetvendorpoint),NULL,0},
#endif
#ifdef _ITEM_PET_LOCKED
	{ "ITEMPETLOCKED" ,NULL,0,(void*)&config.itampetlocked,	INT},
#endif
#ifdef _TALK_SAVE
	{ "SAVEFAME" ,NULL,0,(void*)&config.savefame,	INT},
#endif
#ifdef _TALK_CHECK
	{ "TALKCHECKMAX" ,NULL,0,(void*)&config.talkcheckmax,	INT},
	{ "TALKCHECKMIN" ,NULL,0,(void*)&config.talkcheckmin,	INT},
#endif
#ifdef _DISABLE_PROFESSION_SKILL
	{ "DISABLEPROFESSION" ,config.disableprofessionskill, sizeof( config.disableprofessionskill),NULL,0},
#endif
#ifdef _ALL_SERV_SEND
	{ "ALLSERVTRANS" ,NULL,0,(void*)&config.allservtrans,	INT},
	{ "ALLSERVLEVEL" ,NULL,0,(void*)&config.allservlevel,	INT},
	{ "ALLSERVSNED" ,NULL,0,(void*)&config.allservsend,	INT},
#endif
#ifdef _PET_TRANS_ABILITY
	{ "PETTRANSABILITY" ,NULL,0,(void*)&config.pettransability,	INT},
	{ "PETTRANSABILITY1" ,NULL,0,(void*)&config.pettransability1,	INT},
	{ "PETTRANSABILITY2" ,NULL,0,(void*)&config.pettransability2,	INT},
#endif
#ifdef _NEED_ITEM_ENEMY
	{ "DELNEEDITEM" ,NULL,0,(void*)&config.delneeditem,	INT},
#endif
#ifdef _NOT_ESCAPE
	{ "NOTESCAPE" ,config.notescape, sizeof( config.notescape),NULL,0},
#endif
#ifdef _PLAYER_OVERLAP_PK
	{ "PLAYEROVERLAPPK" ,config.playeroverlappk, sizeof( config.playeroverlappk),NULL,0},
#endif
#ifdef _FIMALY_PK_TIME
	{ "FIMALYPKTIME" ,NULL,0,(void*)&config.fimalypktime,	INT},
#endif
#ifdef _PETSKILL_SHOP_LUA
	{ "PETSKILLSHOPPATH" ,config.freepetskillshoppath, sizeof( config.freepetskillshoppath),NULL,0},
#endif
#ifdef _CANCEL_ANGLE_TRANS
	{ "CANCELANGLETRANS" ,NULL,0,(void*)&config.cancelanlgetrans,	INT},
#endif
#ifdef _VIP_BATTLE_EXP
	{ "vipbattleexp" ,NULL,0,(void*)&config.vipbattleexp,	INT},
#endif
#ifdef _NO_HELP_MAP
	{ "nohelpmap" ,config.nohelpmap, sizeof( config.nohelpmap),NULL,0},
#endif
#ifdef _BATTLE_TIME
	{ "battletime" ,NULL,0,(void*)&config.battletime,	INT},
#endif
#ifdef _SAME_IP_ONLINE_NUM
	{ "sameiponlinenum" ,NULL,0,(void*)&config.sameiponlinenum,	INT},
#endif
#ifdef _STREET_VENDOR_TRANS
	{ "streetvendortrans" ,NULL,0,(void*)&config.streetvendortrans,	INT},
#endif
#ifdef _CHECK_SEVER_IP
	{ "serverip" ,config.serverip, sizeof( config.serverip),NULL,0},
#endif
#ifdef _DAMMAGE_CALC
	{ "dammagecalc" ,NULL,0,(void*)&config.dammagecalc,	INT},
#endif
#ifdef _PET_ENEMY_DEVELOP_UP
	{ "PetEnemyDevelopUp" ,NULL,0,(void*)&config.PetEnemyDevelopUp,	INT},
#endif
#ifdef _FIRST_LOCK_ITEM
	{ "FirstLockItem" ,config.FirstLockItem, sizeof( config.FirstLockItem),NULL,0},
#endif
	{ "Connectnum" ,NULL,0,(void*)&config.Connectnum,	INT},
#ifdef _PETSKILL_NEW_PASSIVE
		{ "AUTUGETSKILL",		NULL ,0 , (void*)&config.autogetskill,				INT},
		{ "GETSKILLPOS",		NULL ,0 , (void*)&config.getskillpos,				INT},
		{ "SKILLINFOLV1", config.skillinfolv[0], sizeof( config.skillinfolv[0]),NULL,0},
		{ "SKILLINFOLV2", config.skillinfolv[1], sizeof( config.skillinfolv[1]),NULL,0},
		{ "SKILLINFOLV3", config.skillinfolv[2], sizeof( config.skillinfolv[2]),NULL,0},
		{ "SKILLINFOLV4", config.skillinfolv[3], sizeof( config.skillinfolv[3]),NULL,0},
		{ "SKILLINFOLV5", config.skillinfolv[4], sizeof( config.skillinfolv[4]),NULL,0},
		{ "FUSIONRANGE", config.fusionrange, sizeof( config.fusionrange),NULL,0},
		{ "SKILLFUSION",		NULL ,0 , (void*)&config.skillfusion,				INT},
		{ "SKILLCOUNT",		NULL ,0 , (void*)&config.skillcount,				INT},
#endif
#ifdef _SHARE_EXP
		{ "EXPSHARE"      , NULL ,0 , (void*)&config.expshare      ,INT},
#endif
#ifdef _DEFEND_BIGBAO
		{ "BIGBAO" ,NULL,0,(void*)&config.bigbao,	INT},
		{ "BIGBAO2" ,NULL,0,(void*)&config.bigbao2,	INT},
#endif
#ifdef _MO_SHOW_DEBUG
    { "ISDEBUG",		NULL ,0 , (void*)&config.isdebug,				INT},
#endif
#ifdef _CHAR_LOOP_TIME
	{ "charlooptime" ,NULL,0,(void*)&config.charlooptime,	INT},
#endif
#ifdef _MO_RELOAD_NPC
	{ "RELOADNPCTIME" ,NULL,0,(void*)&config.reloadnpctime,	INT},
	{ "RELOADNPCTYPE" ,NULL,0,(void*)&config.reloadnpctype,	INT},
#endif
#ifdef _JZ_NEWSCRIPT_LUA
	{ "LUAFILE"  , config.luafile,sizeof(config.luafile),NULL,0},
#endif
#ifdef _ITEM_LUA
	{ "ITEMLUAFILE"  , config.itemluafile,sizeof(config.itemluafile),NULL,0},
#endif
#ifdef _ROOKIE_ITEM
    { "ROOKIEITEM",		NULL ,0 , (void*)&config.rookieitem[0],				INT},
    { "ROOKIEITEM2",		NULL ,0 , (void*)&config.rookieitem[1],				INT},
    { "ROOKIEITEM3",		NULL ,0 , (void*)&config.rookieitem[2],				INT},
    { "ROOKIEITEM4",		NULL ,0 , (void*)&config.rookieitem[3],				INT},
    { "ROOKIEITEM5",		NULL ,0 , (void*)&config.rookieitem[4],				INT},
#endif
#ifdef _NO_TRANS_ITEM
    { "NOTRANSITEM",		NULL ,0 , (void*)&config.notransitem,				INT},
#endif
#ifdef _MAX_MERGE_LEVEL
    { "MAXMERGELEVEL",		NULL ,0 , (void*)&config.maxmergelevel,				INT},
#endif
#ifdef _NO_ATTACK
	{ "ATTTIME" ,NULL,0,(void*)&config.atttime,	INT},
	{ "ATTSAFETIME" ,NULL,0,(void*)&config.attsafetime,	INT},
	{ "ATTCNT" ,NULL,0,(void*)&config.attcnt,	INT},
	{ "LATETIME" ,NULL,0,(void*)&config.latetime,	INT},
	{ "ATTDMETIME" ,NULL,0,(void*)&config.attdmetime,	INT},
	{ "ATTDMECNT" ,NULL,0,(void*)&config.attdmecnt,	INT},
#endif
	{ "NOATTIP1" ,config.noattip[0], sizeof( config.noattip[0]),NULL,0},
	{ "NOATTIP2" ,config.noattip[1], sizeof( config.noattip[1]),NULL,0},
	{ "NOATTIP3" ,config.noattip[2], sizeof( config.noattip[2]),NULL,0},
	{ "NOATTIP4" ,config.noattip[3], sizeof( config.noattip[3]),NULL,0},
	{ "NOATTIP5" ,config.noattip[4], sizeof( config.noattip[4]),NULL,0},
#ifdef _NO_FULLPLAYER_ATT
	{ "NOFULLPLAYER" ,NULL,0,(void*)&config.nofullplayer,	INT},
	{ "NOFULL2PLAYER" ,NULL,0,(void*)&config.nofull2player,	INT},
	{ "NOCDKEYPLAYER" ,NULL,0,(void*)&config.nocdkeyplayer,	INT},
	{ "NOCDKEYMODE" ,NULL,0,(void*)&config.nocdkeymode,	INT},
	{ "NOCDKEYTYPE" ,NULL,0,(void*)&config.nocdkeytype,	INT},
	{ "NOFULLTIME" ,NULL,0,(void*)&config.nofulltime,	INT},
	{ "FENGTYPE" ,NULL,0,(void*)&config.fengtype,	INT},
	{ "NOFULLENDPLAYER" ,NULL,0,(void*)&config.nofullendplayer,	INT},
	{ "NOFULLENDTIME" ,NULL,0,(void*)&config.nofullendtime,	INT},
	{ "MANRENNUM" ,NULL,0,(void*)&config.manrennum,	INT},
#endif
	{ "LOCKTYPE" ,NULL,0,(void*)&config.locktype,	INT},
#ifdef _NEW_FUNC_DECRYPT
	{ "ALLOWERRORNUM2" ,NULL,0,(void*)&config.allowerrornum2,	INT},
#endif
#ifdef _MO_LOGIN_NO_KICK
    { "LOGINNOKICK",		NULL ,0 , (void*)&config.loginnokick,				INT},
#endif
#ifdef _MO_ILLEGAL_NAME
		{ "ILLEGALNAME" ,config.illegalname, sizeof( config.illegalname),NULL,0},
#endif
#ifdef _NO_USE_PACKET_MAP
		{ "NOPACKETMAP" ,config.nousepacketmap, sizeof( config.nousepacketmap),NULL,0},
#endif
#ifdef _NO_USE_MAGIC_MAP
		{ "NOMAGICMAP" ,config.nousemagicmap, sizeof( config.nousemagicmap),NULL,0},
#endif
#ifdef _SOME_PETMAIL
    { "PETMAILFLG",		NULL ,0 , (void*)&config.petmailflg,				INT},
    { "SOMEPETMAIL" ,config.somepetmail, sizeof( config.somepetmail),NULL,0},
#endif
#ifdef _CTRL_TRANS_DEVELOP
    { "CTRLTRANS",		NULL ,0 , (void*)&config.ctrltrans,				INT},
#endif
#ifdef	_PETMAIL_TIME
			{ "PETMAILTIME" ,NULL,0,(void*)&config.petmailtime,	INT},
#endif
#ifdef _UP_BBPETPROB
			{ "UPBBPROB" ,NULL,0,(void*)&config.upbbprob,	INT},
#endif

};

// Arminius 7.12 login announce
char announcetext[8192];
void AnnounceToPlayer(int charaindex)
{
  char *ptr,*qtr;
  
  ptr=announcetext;
  while ((qtr=strstr(ptr,"\n"))!=NULL) {
    qtr[0]='\0';
//    printf("ptr=%s\n",ptr);
    CHAR_talkToCli(charaindex, -1, ptr, CHAR_COLORYELLOW);
    qtr[0]='\n';
    ptr=qtr+1;
  }
  CHAR_talkToCli(charaindex, -1, ptr, CHAR_COLORYELLOW);
  
}

// Robin 0720
void AnnounceToPlayerWN(int fd)
{
	char buf[8192];
#ifdef _VIP_LOGOUT
	char token[8192];
	int charaindex = CONNECT_getCharaindex( fd );
	long lastleavetime = CHAR_getInt( charaindex , CHAR_LASTLEAVETIME);
	struct tm *p;
	p=localtime(&lastleavetime);
			
	sprintf( token , "Äú×îºóÀëÏßÊ±¼ä %dÄê%dÔÂ%dÈÕ %d:%d:%d\n\n%s",p->tm_year + 1900,
																																		p->tm_mon + 1,
																																		p->tm_mday,
																																		p->tm_hour,
																																		p->tm_min,
																																		p->tm_sec,
																																		announcetext);
	lssproto_WN_send( fd , WINDOW_MESSAGETYPE_LOGINMESSAGE,
			WINDOW_BUTTONTYPE_OK,
			-1,-1,
			makeEscapeString( token, buf, sizeof(buf)));
#else	
	lssproto_WN_send( fd , WINDOW_MESSAGETYPE_LOGINMESSAGE,
		WINDOW_BUTTONTYPE_OK,
		-1,-1,
		makeEscapeString( announcetext, buf, sizeof(buf)));
#endif
}


void LoadAnnounce(void)
{
    FILE *f;

    memset(announcetext, 0, sizeof(announcetext));
    if ((f=fopen("./announce.txt","r"))!=NULL) {
      fread(announcetext, sizeof(announcetext), 1, f);
      announcetext[sizeof(announcetext)-1]='\0';
      fclose(f);
    }
}
#ifdef _PET_TALKPRO
PTALK pettalktext[PETTALK_MAXID];

void LoadPetTalk(void)
{
	FILE *fp;
	char fn[256];
	char line[ 4096];
	char talkmem[4096];
	int maxid=0;
	char buf1[256], buf2[256], buf3[256];
	int talkNO=-1, mark=-1, i;
	int len = sizeof( talkmem);
	
	memset(talkmem, 0, sizeof(talkmem));
	sprintf(fn, "%s/pettalk/pettalk.menu", getNpcdir());

	for( i=0;i<PETTALK_MAXID;i++)	{
		pettalktext[i].ID = -1;
		strcpy( pettalktext[i].DATA, "\0");
	}

	print("\n×°ÔØ³èÎï¶Ô»°ÎÄ¼þ:%s...", fn);
	
#ifdef _CRYPTO_DATA		
	char realopfile[256];
	BOOL crypto = FALSE;
	sprintf(realopfile, "%s.allblues", fn);
	fp = fopen( realopfile, "r");
	if( fp != NULL ){
		crypto = TRUE;
	}else
#endif
{
	fp = fopen( fn, "r");
}
    if( fp != NULL ) {
		while( fgets( line, sizeof( line), fp)) {
#ifdef _CRYPTO_DATA		
			if(crypto==TRUE){
				DecryptKey(line);
			}
#endif
			if( strlen( talkmem) != 0 ) {
				if( talkmem[strlen( talkmem) -1] != '|' ) {
					strcatsafe( talkmem, len, "|");		
				}
			}
			chompex( line);
			strcatsafe( talkmem,len,  line);
		}
		fclose( fp);
    }else	{
		print("´íÎó:ÕÒ²»µ½ÎÄ¼þ!");
	}

	talkNO=1;
	while( getStringFromIndexWithDelim( talkmem,"END",talkNO, buf1, sizeof( buf1)) != FALSE	){
		talkNO++;
		if( NPC_Util_GetStrFromStrWithDelim( buf1, "PETTEMPNO", buf2, sizeof( buf2)) == NULL  )
			continue;
		mark=1;
		strcpy( fn,"\0");

		if( getStringFromIndexWithDelim( buf2,",", mark+1,buf3,sizeof( buf3)) != FALSE )	{
			pettalktext[maxid].ID = atoi( buf3);
			if( getStringFromIndexWithDelim( buf2,",", mark,buf3,sizeof( buf3)) != FALSE )	{
				sprintf(fn, "%s/pettalk/%s", getNpcdir(), buf3);
				//print("\n ...file:%s", fn);
#ifdef _CRYPTO_DATA		
	char realopfile[256];
	BOOL crypto = FALSE;
	sprintf(realopfile, "%s.allblues", fn);
	fp = fopen( realopfile, "r");
	if( fp != NULL ){
		crypto = TRUE;
	}else
#endif
{
				fp = fopen( fn, "r");
}
				if( fp != NULL )	{
					char line[4096];
					while( fgets( line, sizeof( line), fp ) ) {
#ifdef _CRYPTO_DATA		
						if(crypto==TRUE){
							DecryptKey(line);
						}
#endif
						if( strlen( pettalktext[maxid].DATA) != 0 ) {
							if( pettalktext[maxid].DATA[strlen( pettalktext[maxid].DATA) -1] != '|' ) {
								strcatsafe( pettalktext[maxid].DATA, sizeof( pettalktext[maxid].DATA), "|");
							}
						}
						chompex( line);
						strcatsafe( pettalktext[maxid].DATA, sizeof( pettalktext[maxid].DATA), line);
					}
					maxid++;
					fclose( fp);
				}else	{
					print("´íÎó:[%s] ÕÒ²»µ½!", fn);
					pettalktext[maxid].ID=-1;
				}
			}else	{
				pettalktext[maxid].ID=-1;
			}
		}
		print(".");
		if( maxid >= PETTALK_MAXID )
			break;
	}
	print("×î´óID=%d...", maxid);
	{
		int haveid=0;
		for( i=0;i<PETTALK_MAXID;i++)	{
			if( pettalktext[i].ID >= 0 )	{
				haveid++;
			}
		}
		print("ÔØÈë×ÜÊý=%d", haveid);
	}

}

#else
char pettalktext[4096];
void LoadPetTalk(void)
{
  FILE *fp;
  char fn[256];
  char	line[ 4096];
  int len = sizeof( pettalktext);
  
  memset(pettalktext, 0, sizeof(pettalktext));
  sprintf(fn, "%s/pettalk/pettalk.mem", getNpcdir());
#ifdef _CRYPTO_DATA		
	char realopfile[256];
	BOOL crypto = FALSE;
	sprintf(realopfile, "%s.allblues", fn);
	fp = fopen( realopfile, "r");
	if( fp != NULL ){
		crypto = TRUE;
	}else
#endif
{
  fp = fopen( fn, "r");
}
    if( fp != NULL ) {
		print("\n\n ¶ÁÈ¡ pettalk.mem");
		while( fgets( line, sizeof( line), fp)) {
#ifdef _CRYPTO_DATA		
			if(crypto==TRUE){
				DecryptKey(line);
			}
#endif
			if( strlen( pettalktext) != 0 ) {
				if( pettalktext[strlen( pettalktext) -1] != '|' ) {
					strcatsafe( pettalktext, len, "|");		
				}
			}
			chompex( line);
			strcatsafe( pettalktext,len,  line);
		}
		fclose( fp);
		print("\n %s", pettalktext);
    }else	{
		print("\n ²»ÄÜÕÒµ½ pettalk.mem");
	}
}
#endif

#ifdef _GAMBLE_BANK
GAMBLEBANK_ITEMS GB_ITEMS[GAMBLEBANK_ITEMSMAX];

void Load_GambleBankItems( void)
{

	FILE *fp;
	char filename[256];
	char buf1[256];
	char name[128];
	int num,ID,type;
	int i=0;
	sprintf(filename, "./data/gambleitems.txt" );
	print("\n¼ÓÔØ¶Ä²©ÎïÆ·ÎÄ¼þ %s ...", filename);
#ifdef _CRYPTO_DATA		
	char realopfile[256];
	BOOL crypto = FALSE;
	sprintf(realopfile, "%s.allblues", filename);
	fp = fopen( realopfile, "r");
	if( fp != NULL ){
		crypto = TRUE;
	}else
#endif
{
	fp = fopen( filename, "r");
}
    if( fp != NULL ) {
		while( fgets( buf1, sizeof( buf1), fp) != NULL )	{
#ifdef _CRYPTO_DATA		
			if(crypto==TRUE){
				DecryptKey(buf1);
			}
#endif
			if( strstr( buf1, "#") != 0 ) continue;
			sscanf( buf1,"%s %d %d %d", name, &ID, &num , &type);
			strcpy( GB_ITEMS[i].name, name);
			GB_ITEMS[i].Gnum = num;
			GB_ITEMS[i].ItemId = ID;
			GB_ITEMS[i].type = type;
			i++;
		}
		print("×î´óID: %d ", i);
		fclose( fp);
    }else	{
		print("´íÎó ÕÒ²»µ½ÎÄ¼þ %s", filename);
	}

}
#endif


#ifdef _CFREE_petskill
PETSKILL_CODES Code_skill[PETSKILL_CODE];
void Load_PetSkillCodes( void)
{
	FILE *fp;
	char filename[256];
	char buf1[256];
	char name[128];
	char type[256];
	int num,ID;
	int i=0;
	sprintf(filename, "./data/skillcode.txt" );
	print("\n¼ÓÔØ³èÎï¼¼ÄÜ±àÂëÎÄ¼þ:%s...", filename);
#ifdef _CRYPTO_DATA		
	char realopfile[256];
	BOOL crypto = FALSE;
	sprintf(realopfile, "%s.allblues", filename);
	fp = fopen( realopfile, "r");
	if( fp != NULL ){
		crypto = TRUE;
	}else
#endif
{
	fp = fopen( filename, "r");
}
    if( fp != NULL ) {
		while( fgets( buf1, sizeof( buf1), fp) != NULL )	{
#ifdef _CRYPTO_DATA		
			if(crypto==TRUE){
				DecryptKey(buf1);
			}
#endif
			sscanf( buf1,"%s %d %d %s", name, &num, &ID, type);
			strcpy( Code_skill[i].name, name);
			Code_skill[i].TempNo = num;
			Code_skill[i].PetId = ID;
			strcpy( Code_skill[i].Code, type);
			//print("\n %s|%d|%d|%s|", Code_skill[i].name, Code_skill[i].TempNo, 
			//	Code_skill[i].PetId, Code_skill[i].Code);
			i++;
			if( i >= PETSKILL_CODE ) break;
		}
		fclose( fp);
    }else	{
		print("´ò²»µ½ÎÄ¼þ %s", filename);
	}
	print("Íê³É\n");
}
#endif

#ifdef _GMRELOAD
BOOL LoadGMSet( char* filename )
{
	FILE* fp;
	int i = 0, gm_num = 0;
	
#ifdef _CRYPTO_DATA		
	char realopfile[256];
	BOOL crypto = FALSE;
	sprintf(realopfile, "%s.allblues", filename);
	fp = fopen( realopfile, "r");
	if( fp != NULL ){
		crypto = TRUE;
	}else
#endif
{
  	fp = fopen(filename,"r");
}
	if (fp == NULL)
	{
		print("ÎÞ·¨´ò¿ªÎÄ¼þ\n");
		return FALSE;
	}
	for (i = 0; i < GMMAXNUM; i++)
	{
		strcpy(gminfo[i].cdkey, "");
		gminfo[i].level = 0;
	}
	while(1){
		char	line[64], cdkey[64], level[64];
		if (fgets(line, sizeof(line), fp) == NULL)	break;
#ifdef _CRYPTO_DATA		
		if(crypto==TRUE){
			DecryptKey(line);
		}
#endif
		chop(line);
		//change Ê¹gmset.txt¿ÉÒÔÔö¼Ó×¢½â*******
		if( line[0] == '#' )
			continue;
		for( i=0; i<strlen(line); i++ ){
            if( line[i] == '#' ){
			    line[i] = '\0';
		        break;
			}
		}
		//*************************************
		gm_num = gm_num + 1;
		if (gm_num > GMMAXNUM)	break;
		easyGetTokenFromString(line, 1, cdkey, sizeof(cdkey));
		if (strcmp(cdkey, "") == 0)	break;
		strncpy(gminfo[gm_num].cdkey, cdkey, sizeof(gminfo[gm_num].cdkey));
		easyGetTokenFromString(line, 2, level, sizeof(level));
		if (strcmp(level, "") == 0)	break;
		gminfo[gm_num].level = atoi(level);
//		print("\ncdkey:%s, level:%d", gminfo[gm_num].cdkey, gminfo[gm_num].level);
	}
	fclose(fp);
	return TRUE;
}
#endif

/*------------------------------------------------------------
 * Ãó·òºë·ÂØ©  Ã«  ÔÂ
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  cahr*
 ------------------------------------------------------------*/
char* getProgname( void )
{
    return config.progname;
}
/*------------------------------------------------------------
 * configfilename Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  char*
 ------------------------------------------------------------*/
char* getConfigfilename( void )
{
    return config.configfilename;
}
/*------------------------------------------------------------
 * configfilename Ã«É¬ÀÃÔÊÔÂ£Û
 * Â¦ÐÑ
 *  newv    char*   Þ¥ØÆÖÐ°À
 * ß¯Ô»°À
 *  Ø¦ØÆ
 ------------------------------------------------------------*/
void setConfigfilename( char* newv )
{
    strcpysafe( config.configfilename, sizeof( config.configfilename ),
                newv );
}

/*------------------------------------------------------------
 * ·¸ÌïÓÀºëÒÁÃ¬»ïÃ«  ÔÂ
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getDebuglevel( void )
{
    return config.debuglevel;
}
/*------------------------------------------------------------
 * ·¸ÌïÓÀºëÒÁÃ¬»ïÃ«É¬ÀÃÔÊÔÂ
 * Â¦ÐÑ
 *  newv    int     Þ¥ØÆÖÐ°À
 * ß¯Ô»°À
 *  unsigned int    éÉ¼°°À
 ------------------------------------------------------------*/
unsigned int setDebuglevel( unsigned int newv )
{
    int old;
    old = config.debuglevel;
    config.debuglevel = newv;
    return old;
}
/*------------------------------------------------------------
 * memoryunit Ã«  ÔÂ
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getMemoryunit( void )
{
    return config.usememoryunit;
}
/*------------------------------------------------------------
 * memoryunitnum Ã«  ÔÂ
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getMemoryunitnum( void )
{
    return config.usememoryunitnum;
}

/*------------------------------------------------------------
 * Ê§ÊÐËü¼þÐþÈÓ¡õÌï¼°Ê§ÓñÒÁµ©Ã«  ÔÂ
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  char*
 ------------------------------------------------------------*/
char*   getAccountservername( void )
{
    return config.asname;
}
/*------------------------------------------------------------
 * Ê§ÊÐËü¼þÐþÈÓ¡õÌï¼°ºÌ¡õÐþÃ«  ÔÂ
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned short
 ------------------------------------------------------------*/
unsigned short   getAccountserverport( void )
{
    return config.acservport;
}
/*------------------------------------------------------------
 * Ê§ÊÐËü¼þÐþÈÓ¡õÌï³ß¼°ÓÉµ©·¥¡õÓñÃ«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned short
 ------------------------------------------------------------*/
char*   getAccountserverpasswd( void )
{
  return config.acpasswd;
}
/*------------------------------------------------------------
 * Ê§ÊÐËü¼þÐþÈÓ¡õÌï¾®ÈÕÎ­ÒüÔÂ±Ø¡õØ©ÈÓ¡õÌïÎçØÆ»¯¼°  ó¡Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned short
 ------------------------------------------------------------*/
char*   getGameservername( void )
{
    return config.gsnamefromas;
}

// Arminius 7.24 manor pk
char* getGameserverID( void )
{
    if (config.gsid[strlen(config.gsid)-1]=='\n')
      config.gsid[strlen(config.gsid)-1]='\0';
      
    return config.gsid;
}

unsigned short getAllowManorPK( void )
{
    return config.allowmanorpk;
}

unsigned short getPortnumber( void )
{
    return config.port;
}
/*------------------------------------------------------------
 * ±Ø¡õØ©ÈÓ¡õÌï¼°  Ä¯   Ä¯Ã«  ÔÂ
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned short
 ------------------------------------------------------------*/
int getServernumber( void )
{
    return config.servernumber;
}
/*------------------------------------------------------------
 * reuseaddr ¼°°ÀÃ«  ÔÂ
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned short
 ------------------------------------------------------------*/
int getReuseaddr( void )
{
    return config.reuseaddr;
}

int getNodelay( void )
{
    return config.do_nodelay;
}
int getLogWriteTime(void)
{
    return config.log_write_time;
}
int getLogIOTime( void)
{
    return config.log_io_time;
}
int getLogGameTime(void)
{
    return config.log_game_time;
}
int getLogNetloopFaster(void)
{
    return config.log_netloop_faster;
}

/*------------------------------------------------------------
 * saacwritenum Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *   int
 ------------------------------------------------------------*/
int getSaacwritenum( void )
{
    return config.saacwritenum;
}
/*------------------------------------------------------------
 * saacwritenum Ã«É¬ÀÃÔÊÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *   int
 ------------------------------------------------------------*/
void setSaacwritenum( int num )
{
    config.saacwritenum = num;
}
/*------------------------------------------------------------
 * saacreadnum Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *   int
 ------------------------------------------------------------*/
int getSaacreadnum( void )
{
    return config.saacreadnum;
}
/*------------------------------------------------------------
 * saacreadnum Ã«É¬ÀÃÔÊÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *   int
 ------------------------------------------------------------*/
void setSaacreadnum( int num )
{
    config.saacreadnum = num;
}
/*------------------------------------------------------------
 * fdnum Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int
 ------------------------------------------------------------*/

unsigned int getFdnum( void )
{
	return config.fdnum;
}

unsigned int getPlayercharnum( void )
{
    return config.charnum;
}

/*------------------------------------------------------------
 * petcharanum Ã«  ÔÂ
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getPetcharnum( void )
{
    return config.petcharnum;
}


/*------------------------------------------------------------
 * othercharnum Ã«  ÔÂ
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getOtherscharnum( void )
{
    return config.othercharnum;
}

/*------------------------------------------------------------
 * objnum Ã«  ÔÂ
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getObjnum( void )
{
    return config.objnum;
}

/*------------------------------------------------------------
 * itemnum Ã«  ÔÂ
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getItemnum( void )
{
    return config.itemnum;
}


/*------------------------------------------------------------
 * battlenum Ã«  ÔÂ
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getBattlenum( void )
{
    return config.battlenum;
}

#ifdef _GET_BATTLE_EXP
unsigned int getBattleexp( void )
{
    return config.battleexp;
}
void setBattleexp( int exp )
{
    config.battleexp = exp;
    return;
}
#endif
/*------------------------------------------------------------
 * topdir Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  char*
 ------------------------------------------------------------*/
char* getTopdir( void )
{
    return config.topdir;
}
/*------------------------------------------------------------
 * mapdir Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  char*
 ------------------------------------------------------------*/
char* getMapdir( void )
{
    return config.mapdir;
}
/*------------------------------------------------------------
 * maptilefile Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  char*
 ------------------------------------------------------------*/
char* getMaptilefile( void )
{
    return config.maptilefile;
}
/*------------------------------------------------------------
 * battlemapfile Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  char*
 ------------------------------------------------------------*/
char* getBattleMapfile( void )
{
    return config.battlemapfile;
}
/*------------------------------------------------------------
 * itemfile Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  char*
 ------------------------------------------------------------*/
char* getItemfile( void )
{
    return config.itemfile;
}
/*------------------------------------------------------------
 * invfile Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  char*
 ------------------------------------------------------------*/
char* getInvfile( void )
{
    return config.invfile;
}
/*------------------------------------------------------------
 * appearfile Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  char*
 ------------------------------------------------------------*/
char* getAppearfile( void )
{
    return config.appearfile;
}
/*------------------------------------------------------------
 * effectfile Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  char*
 ------------------------------------------------------------*/
char* getEffectfile( void )
{
    return config.effectfile;
}
/*------------------------------------------------------------
 * titlenamefile Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  char*
 ------------------------------------------------------------*/
char* getTitleNamefile( void )
{
    return config.titlenamefile;
}
/*------------------------------------------------------------
 * titleconfigfile Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  char*
 ------------------------------------------------------------*/
char* getTitleConfigfile( void )
{
    return config.titleconfigfile;
}
/*------------------------------------------------------------
 * encountfile Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  char*
 ------------------------------------------------------------*/
char* getEncountfile( void )
{
    return config.encountfile;
}
/*------------------------------------------------------------
 * enemyfile Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  char*
 ------------------------------------------------------------*/
char* getEnemyfile( void )
{
    return config.enemyfile;
}
/*------------------------------------------------------------
 * enemybasefile Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  char*
 ------------------------------------------------------------*/
char* getEnemyBasefile( void )
{
    return config.enemybasefile;
}
/*------------------------------------------------------------
 * groupfile Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  char*
 ------------------------------------------------------------*/
char* getGroupfile( void )
{
    return config.groupfile;
}
/*------------------------------------------------------------
 * magicfile Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  char*
 ------------------------------------------------------------*/
char* getMagicfile( void )
{
    return config.magicfile;
}

#ifdef _ATTACK_MAGIC

/*------------------------------------------------------------
 * È¡µÃ¹¥»÷ÐÔµÄÖäÊõ
 * ²ÎÊý
 * None
 * ·µ»ØÖµ
 * char*
 ------------------------------------------------------------*/
char* getAttMagicfileName( void )
{
    return config.attmagicfile;
}

#endif


char* getPetskillfile( void )
{
    return config.petskillfile;
}

#ifdef _PROFESSION_SKILL			// WON ADD ÈËÎïÖ°Òµ¼¼ÄÜ
char* getProfession( void )
{
    return config.profession;
}
#endif

#ifdef _ITEM_QUITPARTY
char* getitemquitparty( void )
{
    return config.itemquitparty;
}
#endif

char *getItematomfile( void )
{
    return config.itematomfile;
}


char* getQuizfile( void )
{
    return config.quizfile;
}

/*------------------------------------------------------------
 * lsgenlogfile Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  char*
 ------------------------------------------------------------*/
char* getLsgenlogfilename( void )
{
    return config.lsgenlog;
}

#ifdef _GMRELOAD
char* getGMSetfile( void )
{
    return config.gmsetfile;
}
#endif

/*------------------------------------------------------------
 * storedir Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  char*
 ------------------------------------------------------------*/
char* getStoredir( void )
{
    return config.storedir;
}
#ifdef _STORECHAR
/*------------------------------------------------------------
 ------------------------------------------------------------*/
char* getStorechar( void )
{
    return config.storechar;
}
#endif

/*------------------------------------------------------------
 * NPC åÃ¼°·¸Å«ÒÁÛÍÐþØøÃ«  »¯ÈÊÔÂèúÐÑ
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  char*
 ------------------------------------------------------------*/
char* getNpcdir( void )
{
    return config.npcdir;
}
/*------------------------------------------------------------
 * ·òºë·¸Å«ÒÁÛÍÐþØøÃ«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  char*
 ------------------------------------------------------------*/
char* getLogdir( void )
{
    return config.logdir;
}

/*------------------------------------------------------------
 * ·òºëÉ¬ÀÃ°×ÑëÄÌ»ï  Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  char*
 ------------------------------------------------------------*/
char* getLogconffile( void )
{
    return config.logconfname;
}
/*------------------------------------------------------------
 * ÃñÅÒÓÀÐþ  Ü·ÓÉµ©·¥¡õÓñ Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  char*
 ------------------------------------------------------------*/
char* getChatMagicPasswd( void )
{
    return config.chatmagicpasswd;
}

void setChatMagicPasswd( void )
{
    sprintf(config.chatmagicpasswd,"gm");
}
/*------------------------------------------------------------
 * ·¸ÌïÓÀºëÃñÅÒÓÀÐþ  Ü·Æ¥¼°CDKEYÃñÄáÓÀÛÍÃ«ÔÊÔÂ¾®Éýµ¤¾®Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  char*
 ------------------------------------------------------------*/
unsigned getChatMagicCDKeyCheck( void )
{
    return config.chatmagiccdkeycheck;
}

void setChatMagicCDKeyCheck( void )
{
    config.chatmagiccdkeycheck = 0;
}
/*------------------------------------------------------------
 * filesearchnumÃ«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getFilesearchnum( void )
{
    return config.filesearchnum;
}
/*------------------------------------------------------------
 * npctemplatenumÃ«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getNpctemplatenum( void )
{
    return config.npctemplatenum;
}
/*------------------------------------------------------------
 * npccreatenumÃ«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getNpccreatenum( void )
{
    return config.npccreatenum;
}

/*------------------------------------------------------------
 * walksendintervalÃ«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getWalksendinterval( void )
{
    return config.walksendinterval;
}
/*------------------------------------------------------------
 * walksendintervalÃ«±¾ÓÀÐþÔÊÔÂ£Û
 * Â¦ÐÑ
 *  unsigned int 	interval	ÁÝÃÞ  Á¢Øø
 * ß¯Ô»°À
 *  void
 ------------------------------------------------------------*/
void setWalksendinterval( unsigned int interval )
{
    config.walksendinterval = interval;
}
/*------------------------------------------------------------
 * CAsendintervalÃ«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getCAsendinterval_ms( void )
{
    return config.CAsendinterval_ms;
}
/*------------------------------------------------------------
 * CAsendintervalÃ«±¾ÓÀÐþÔÊÔÂ£Û
 * Â¦ÐÑ
 *  unsigned int interval
 * ß¯Ô»°À
 *  unsigned int
 ------------------------------------------------------------*/
void setCAsendinterval_ms( unsigned int interval_ms )
{
    config.CAsendinterval_ms = interval_ms;
}
/*------------------------------------------------------------
 * CDsendintervalÃ«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getCDsendinterval_ms( void )
{
    return config.CDsendinterval_ms;
}
/*------------------------------------------------------------
 * CDsendintervalÃ«±¾ÓÀÐþÔÊÔÂ£Û
 * Â¦ÐÑ
 *  interval		unsigned int
 * ß¯Ô»°À
 * void
 ------------------------------------------------------------*/
void setCDsendinterval_ms( unsigned int interval_ms )
{
	config.CDsendinterval_ms = interval_ms;
}
/*------------------------------------------------------------
 * OnelooptimeÃ«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getOnelooptime_ms( void )
{
    return config.Onelooptime_ms;
}
/*------------------------------------------------------------
 * OnelooptimeÃ«±¾ÓÀÐþÔÊÔÂ£Û
 * Â¦ÐÑ
 *  interval		unsigned int
 * ß¯Ô»°À
 * void
 ------------------------------------------------------------*/
void setOnelooptime_ms( unsigned int interval_ms )
{
	config.Onelooptime_ms = interval_ms;
}
/*------------------------------------------------------------
 * PetdeletetimeÃ«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getPetdeletetime( void )
{
    return config.Petdeletetime;
}
/*------------------------------------------------------------
 * PetdeletetimeÃ«±¾ÓÀÐþÔÊÔÂ£Û
 * Â¦ÐÑ
 *  interval		unsigned int
 * ß¯Ô»°À
 * void
 ------------------------------------------------------------*/
void setPetdeletetime( unsigned int interval )
{
	config.Petdeletetime = interval;
}
/*------------------------------------------------------------
 * ItemdeletetimeÃ«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getItemdeletetime( void )
{
    return config.Itemdeletetime;
}
/*------------------------------------------------------------
 * ItemdeletetimeÃ«±¾ÓÀÐþÔÊÔÂ£Û
 * Â¦ÐÑ
 *  interval		unsigned int
 * ß¯Ô»°À
 * void
 ------------------------------------------------------------*/
void setItemdeletetime( unsigned int interval )
{
	config.Itemdeletetime = interval;
}

/*------------------------------------------------------------
 * CharSavesendintervalÃ«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getCharSavesendinterval( void )
{
    return config.CharSavesendinterval;
}
/*------------------------------------------------------------
 * CharSavesendintervalÃ«±¾ÓÀÐþÔÊÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int
 ------------------------------------------------------------*/
void setCharSavesendinterval( unsigned int interval)
{
	config.CharSavesendinterval = interval;
}

/*------------------------------------------------------------
 * Addressbookoffmsgnum Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getAddressbookoffmsgnum( void )
{
    return config.addressbookoffmsgnum;
}
/*------------------------------------------------------------
 * Protocolreadfrequency Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getProtocolreadfrequency( void )
{
    return config.protocolreadfrequency;
}

/*------------------------------------------------------------
 * Allowerrornum Ã«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getAllowerrornum( void )
{
    return config.allowerrornum;
}

/*------------------------------------------------------------
 * ·òºëÃ«äú  ÔÊÔÂÁÝ¶ÔÃ«  ÔÂ£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int
 ------------------------------------------------------------*/
unsigned int getLogHour( void )
{
    return config.loghour;
}

/*------------------------------------------------------------
 * ÌïÐþ»ï  ¼°·¸ÌïÓÀºë¶ªÓÀ±¾¡õ³âÃ«ÇëÔÊ¾®£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int 1Ø¦ÈÕÇëÔÊ
 ------------------------------------------------------------*/
unsigned int getBattleDebugMsg( void )
{
    return config.battledebugmsg;
}
/*------------------------------------------------------------
 * ÌïÐþ»ï  ¼°·¸ÌïÓÀºë¶ªÓÀ±¾¡õ³âÃ«ÇëÔÊ¾®£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int 1Ø¦ÈÕÇëÔÊ
 ------------------------------------------------------------*/
void setBattleDebugMsg( unsigned int num )
{
    config.battledebugmsg = num;
}



/*
 * Config¼°·¸°×ÇÉ»ïÐþ°ÀÃ«è£»§ÔÂèúÐÑ
 * Â¦ÐÑ
 *  argv0   char*   ÎìÑ¨¼þÓñ·ÂÄÌ¼þÂ¦ÐÑ¼°  âÙ
 */
void  defaultConfig( char* argv0 )
{
    char* program;                  /* program  Ã«·Æ»§ÔÂ¼°±åÒøµ¤ */

    /* ·¸°×ÇÉ»ïÐþ°ÀÃ«  Ä¾ÔÂ */

    /*Ãó·òºë·ÂØ©  */
    program = rindex(argv0, '/');
    if (program == NULL)
        program = argv0;
    else
        program++;   /* "/"¼°ÆÝ¾®ÈÕ±åØÆÐ×ÖÐ¼°Æ¥++ÔÊÔÂ*/
    strcpysafe( config.progname , sizeof( config.progname ) ,program );

    /*É¬ÀÃ°×ÑëÄÌ»ï  */
    strcpysafe( config.configfilename,
                sizeof( config.configfilename ),"setup.cf" );

}

/*
 * É¬ÀÃ°×ÑëÄÌ»ï  Ã«  ÊÏ·Ö  Æ¥¼°ÖÊ  Ã«µæµ¤£Û
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  Ø¦ØÆ
 */
void lastConfig( void )
{
    char    entry[256];
    /*  Ñ¨ÓÀÃó·¸Å«ÒÁÛÍÐþØø¼°É¬ÀÃ    */
    snprintf(entry, sizeof(entry), "%s/%s", config.topdir, config.mapdir);
    strcpysafe(config.mapdir, sizeof(config.mapdir), entry);

    /*  Ñ¨ÓÀÃóÉ¬ÀÃ°×ÑëÄÌ»ï  ¼°É¬ÀÃ    */
    snprintf(entry,sizeof(entry),"%s/%s",
             config.topdir,config.maptilefile);
    strcpysafe(config.maptilefile, sizeof(config.maptilefile), entry);

    /*  ÌïÐþ»ïÑ¨ÓÀÃóÉ¬ÀÃ°×ÑëÄÌ»ï  ¼°É¬ÀÃ    */
    snprintf(entry,sizeof(entry),"%s/%s",
             config.topdir,config.battlemapfile);
    strcpysafe(config.battlemapfile, sizeof(config.battlemapfile), entry);

    /*  Ê§ÄÌ  Ø©É¬ÀÃ°×ÑëÄÌ»ï  ¼°É¬ÀÃ    */
    snprintf(entry,sizeof(entry), "%s/%s", config.topdir,config.itemfile);
    strcpysafe(config.itemfile, sizeof(config.itemfile), entry);

    /*    ³ÄÉ¬ÀÃ°×ÑëÄÌ»ï  ¼°É¬ÀÃ    */
    snprintf(entry,sizeof(entry), "%s/%s", config.topdir,config.invfile);
    strcpysafe(config.invfile, sizeof(config.invfile), entry);

    /*  ÇëòØÞË  É¬ÀÃ°×ÑëÄÌ»ï  ¼°É¬ÀÃ    */
    snprintf(entry,sizeof(entry), "%s/%s", config.topdir,config.appearfile);
    strcpysafe(config.appearfile, sizeof(config.appearfile), entry);

    /*  ÉÒÇëÉ¬ÀÃ°×ÑëÄÌ»ï  ¼°É¬ÀÃ    */
    snprintf(entry,sizeof(entry), "%s/%s", config.topdir,config.effectfile);
    strcpysafe(config.effectfile, sizeof(config.effectfile), entry);

    /*  ÛÍÄÌÊõÉ¬ÀÃ°×ÑëÄÌ»ï  ¼°É¬ÀÃ    */
    snprintf(entry,sizeof(entry), "%s/%s", config.topdir,config.quizfile);
    strcpysafe(config.quizfile, sizeof(config.quizfile), entry);

    /*  ±¹Ä¯  °×ÑëÄÌ»ï  ¼°É¬ÀÃ    */
    snprintf(entry,sizeof(entry), "%s/%s", config.topdir,config.titlenamefile);
    strcpysafe(config.titlenamefile, sizeof(config.titlenamefile), entry);

    /*  lsgen Ê§ËüÐþÃóÓÀÐþ°×ÑëÄÌ»ï      */
    snprintf(entry,sizeof(entry),"%s/%s", config.topdir,config.lsgenlog);
    strcpysafe(config.lsgenlog, sizeof(config.lsgenlog), entry);

    /*  µ©ÐþÊ§·¸Å«ÒÁÛÍÐþØø¼°É¬ÀÃ    */
/*
    snprintf(entry,sizeof(entry), "%s/%s",config.topdir,config.storedir);
    strcpysafe(config.storedir, sizeof(config.storedir), entry);
*/
    /*  NPCÉ¬ÀÃÐþÓÀÃó·¸Å«ÒÁÛÍÐþØø¼°É¬ÀÃ    */
    snprintf(entry,sizeof(entry), "%s/%s",config.topdir,config.npcdir);
    strcpysafe(config.npcdir, sizeof(config.npcdir), entry);

#ifdef _STORECHAR
    /*   */
    snprintf(entry,sizeof(entry), "%s/%s",config.topdir,config.storechar);
    strcpysafe(config.storechar, sizeof(config.storechar), entry);
#endif

}


/*
 * ºÌÄÌ¼þÕý¡õÎçØÍå©Ø¦µáÊ÷  Ã«  ÔÈ»¯Ò½  Ã«ÔÊÔÂèúÐÑ
 * Â¦ÐÑ
 *  to      void*   °ÀÃ«Ò½  ÔÊÔÂºÌÄÌ¼þÕý
 *  type    CTYPE   to¼°µáÃ«è£»§ÔÂ
 *  value   double  to±åÒ½  ÔÊÔÂ°À
 * ß¯Ô»°À
 *  Ø¦ØÆ
 */
void substitutePointerFromType( void* to , CTYPE type ,double value)
{
    switch( type  ){
    case CHAR:
        *(char*)to = (char)value;
        break;
    case SHORT:
        *(short*)to = (short)value;
        break;
    case INT:
        *(int*)to = (int)value;
        break;
    case DOUBLE:
        *(double*)to = (double)value;
        break;
    }
}

BOOL luareadconfigfile( char* data )
{
	char firstToken[256];
	int ret = getStringFromIndexWithDelim( data , "=",  1, firstToken, sizeof(firstToken) );
	if( ret == FALSE ){
    return FALSE;
  }
	int i;
  for( i = 0 ; i < arraysizeof( readconf ) ; i ++ ){
    if( strcmp( readconf[i].name ,firstToken ) == 0 ){
      /* match */
      char secondToken[256];      /*2    ¼°  Ù¯  */
      /* delim "=" Æ¥2    ¼°Ðþ¡õÛÍ¼þÃ«  ÔÂ*/
      ret = getStringFromIndexWithDelim( data , "=" , 2
                                         , secondToken ,
                                         sizeof(secondToken) );

      /* NULL  Ù¯¾®Éýµ¤¾®Ã«Æ©ÍÍÔÂ */
      if( ret == FALSE ){
        break;
      }

      if( readconf[i].charvalue != NULL )
          strcpysafe( readconf[i].charvalue
                      ,readconf[i].charsize, secondToken);
      if( readconf[i].value != NULL ) {
        if( strcmp( "ON" ,secondToken ) == 0 ) {

            substitutePointerFromType( readconf[i].value,
                                       readconf[i].valuetype,
                                       1.0);

        }else if( strcmp( "OFF" ,secondToken ) == 0 ) {

            substitutePointerFromType( readconf[i].value,
                                       readconf[i].valuetype,
                                       1.0);
        }else {
            strtolchecknum(secondToken,
                           (int*)readconf[i].value,
                           10, readconf[i].valuetype);
				}
			}
      break;
    }
  }
  return TRUE;
}


/*------------------------------------------------------------
 * É¬ÀÃ°×ÑëÄÌ»ïÃ«  ¸ê
 * Â¦ÐÑ
 *      filename            °×ÑëÄÌ»ï
 * ß¯Ô»°À
 *      TRUE(1)     ÔÀ
 *      FALSE(0)    ÁÃ      -> °×ÑëÄÌ»ï¼°×ó¡õÃó¼þ±åÁÃ  ØÆÐ×
 ------------------------------------------------------------*/
BOOL readconfigfile( char* filename )
{
    FILE* f=NULL;
    char linebuf[256];                  /* Óòµæ  ÐÄ  ¸êÌïÓÀ°×Ñë */
    int linenum=0;                      /* µæÐÑÃ«ÐÑÒüÔÂ */
    char    realopenfilename[256];      /*    ¶Ë±åopen ÔÊÔÂ°×ÑëÄÌ»ï  */

    char    hostname[128];

    /*  Ê¯µ©Ðþ  Ã«·Æ»§ÔÂ    */
    if( gethostname( hostname, sizeof(hostname) ) != -1 ){
        char*   initdot;
        initdot = index( hostname, '.' );
        if( initdot != NULL )
            *initdot = '\0';
        snprintf( realopenfilename, sizeof(realopenfilename),
                  "%s.%s" , filename, hostname);

        /* °×ÑëÄÌ»ï¼°×ó¡õÃó¼þ */
        f=fopen( realopenfilename, "r" );
        
       // if( f == NULL )
       //     print( "Can't open %s.  use %s instead\n", realopenfilename, filename );
    }
    if( f == NULL ){
        f=fopen( filename , "r" );          /* °×ÑëÄÌ»ï¼°×ó¡õÃó¼þ */
        if( f == NULL ){
            print( "Can't open %s\n", filename );
            return FALSE;
        }
    }

    /* ÓòµæÎð¹´  ÐÄ  ¸ê */
    while( fgets( linebuf , sizeof( linebuf ), f ) ){
        char firstToken[256];       /*1    ¼°  Ù¯  */
        int i;                      /*»ï¡õÃó  ÐÑ*/
        int ret;                    /*ØøÕý¡õ¼þÎì¡õÓñ*/

        linenum ++;

        deleteWhiteSpace(linebuf);          /* remove whitespace    */

        if( linebuf[0] == '#' )continue;        /* comment */
        if( linebuf[0] == '\n' )continue;       /* none    */

        chomp( linebuf );                    /* remove tail newline  */

        /* delim "=" Æ¥  âÙ(1)¼°Ðþ¡õÛÍ¼þÃ«  ÔÂ*/
        ret = getStringFromIndexWithDelim( linebuf , "=",  1, firstToken,
                                           sizeof(firstToken) );
        if( ret == FALSE ){
            print( "Find error at %s in line %d. Ignore\n",
                     filename , linenum);
            continue;
        }

        /* readconf ¼°ÈÓÄÌÊõÆ¥»ï¡õÃó */
        for( i = 0 ; i < arraysizeof( readconf ) ; i ++ ){
            if( strcmp( readconf[i].name ,firstToken ) == 0 ){
                /* match */
                char secondToken[256];      /*2    ¼°  Ù¯  */
                /* delim "=" Æ¥2    ¼°Ðþ¡õÛÍ¼þÃ«  ÔÂ*/
                ret = getStringFromIndexWithDelim( linebuf , "=" , 2
                                                   , secondToken ,
                                                   sizeof(secondToken) );

                /* NULL  Ù¯¾®Éýµ¤¾®Ã«Æ©ÍÍÔÂ */
                if( ret == FALSE ){
                    print( "Find error at %s in line %d. Ignore",
                           filename , linenum);
                    break;
                }


                /*NULL·ÖÔÈÐ×ÈÕÒ½  ØÆØ¦ÖÐ*/
                if( readconf[i].charvalue != NULL )
                    strcpysafe( readconf[i].charvalue
                                ,readconf[i].charsize, secondToken);

                /*NULL·ÖÔÈÐ×ÈÕÒ½  ØÆØ¦ÖÐ*/
                if( readconf[i].value != NULL ) {
                    if( strcmp( "ON" ,secondToken ) == 0 ) {
                        /*ON·ÖÔÈÐ×ÈÕ1Ã«  Ä¾ÔÂ*/
                        substitutePointerFromType( readconf[i].value,
                                                   readconf[i].valuetype,
                                                   1.0);

                    }else if( strcmp( "OFF" ,secondToken ) == 0 ) {
                        /*OFF·ÖÔÈÐ×ÈÕ1Ã«  Ä¾ÔÂ*/
                        substitutePointerFromType( readconf[i].value,
                                                   readconf[i].valuetype,
                                                   1.0);
                    }else {
                        strtolchecknum(secondToken,
                                       (int*)readconf[i].value,
                                       10, readconf[i].valuetype);
					}
				}
                break;
            }
        }
    }
    fclose( f );
    lastConfig();
    return TRUE;
}
//ttom add this becaus the second had this function 
/*------------------------------------------------------------
 * ¾Þ¼þÎì¡õÓñÆ½¡õÃ«É¬ÀÃÔÊÔÂ
 * Â¦ÐÑ
 *  Ø¦ØÆ
 * ß¯Ô»°À
 *  unsigned int Æ½¡õÃ«ß¯ÔÊ
------------------------------------------------------------*/
unsigned int setEncodeKey( void )
{
   JENCODE_KEY = config.encodekey;
   return JENCODE_KEY;
}
/*------------------------------------------------------------
* Ê§ÊÐËü¼þÐþÈÓ¡õÌï¡õ±åÌ¤Îå  ¸êÌïÓÀ°×Ñë¼°ÈÓÄÌÊõÃ«É¬ÀÃÔÊÔÂ
* Â¦ÐÑ
*  Ø¦ØÆ
* ß¯Ô»°À
*  unsigned int Æ½¡õÃ«ß¯ÔÊ
------------------------------------------------------------*/
unsigned int setAcWBSize( void )
{
    AC_WBSIZE = config.acwbsize;
    return AC_WBSIZE;
}
unsigned int getAcwriteSize( void )
{
    return config.acwritesize;
}
unsigned int getErrUserDownFlg( void )
{
    return config.ErrUserDownFlg;
}
    

#ifdef _DEL_DROP_GOLD
unsigned int getGolddeletetime( void )
{
    if( config.Golddeletetime > 0 )
		return config.Golddeletetime;
	else
		return config.Itemdeletetime;
}
void setIGolddeletetime( unsigned int interval )
{
	config.Golddeletetime = interval;
}
#endif

#ifdef _NEW_PLAYER_CF
int getNewplayertrans( void )
{
  if(config.newplayertrans > 7)
		return 7;
	else if(config.newplayertrans >= 0)
		return config.newplayertrans;
	else
		return 0;
}
int getNewplayerlv( void )
{
  if(config.newplayerlv > 160)
		return 160;
	else if(config.newplayerlv >0)
		return config.newplayerlv;
	else
		return 0;
}
int getNewplayerpetlv( void )
{
  if(config.newplayerpetlv > 160)
		return 160;
	else if(config.newplayerpetlv > 0)
		return config.newplayerpetlv;
	else
		return 0;
}

int getNewplayergivepet( int index )
{
  if(config.newplayergivepet[index] > 0 )
		return config.newplayergivepet[index];
	else
		return -1;
}

int getNewplayergiveitem( int index )
{
  if(config.newplayergiveitem[index] > 0 )
		return config.newplayergiveitem[index];
	else
		return -1;
}

void setNewplayergivepet( unsigned int index ,unsigned int interval)
{
	config.newplayergivepet[index] = interval;
}

int getNewplayergivegold( void )
{
  if(config.newplayergivegold > 1000000)
		return 1000000;
	else if(config.newplayergivegold < 0)
		return 0;
	else
		return config.newplayergivegold;
}
int getRidePetLevel( void )
{
  if(config.ridepetlevel > 0 )
		return config.ridepetlevel;
	else
		return -1;
}
#ifdef _VIP_SERVER
int getNewplayergivevip( void )
{
	return config.newplayerpetvip < 0?0:config.newplayerpetvip;
}
#endif
#endif

#ifdef _UNLAW_WARP_FLOOR
int getUnlawwarpfloor( unsigned int index )
{
	char unlawwarpfloor[256];
	getStringFromIndexWithDelim(config.unlawwarpfloor,",", index+1, unlawwarpfloor, sizeof(unlawwarpfloor));
	return atoi(unlawwarpfloor);
}
#endif

#ifdef _NO_JOIN_FLOOR
int getNoJoinFloor( unsigned int index )
{
	char nojoinfloor[256];
	getStringFromIndexWithDelim(config.nojoinfloor,",", index+1, nojoinfloor, sizeof(nojoinfloor));
	return atoi(nojoinfloor);
}
#endif

#ifdef _WATCH_FLOOR
int getWatchFloor( unsigned int index )
{
  if(config.watchfloor[index] > 0 )
		return config.watchfloor[index];
	else
		return -1;
}
char* getWatchFloorCF( void )
{
	return (config.watchfloor[0]>0)? "ÊÇ":"·ñ";
}
#endif

#ifdef _BATTLE_FLOOR
int getBattleFloor( unsigned int index )
{
	
	char battlefloor[256];
	if(getStringFromIndexWithDelim(config.battlefloorcf,",", index+1, battlefloor, sizeof(battlefloor))==TRUE)
		return atoi(battlefloor);
	else
		return -1;
}
char* getBattleFloorCF( void )
{
	return (config.battlefloor>0)? "ÊÇ":"·ñ";
}
#endif

#ifdef _ANGEL_SUMMON

extern int mission_num;

BOOL LoadMissionList( void )
{
	FILE* fp;
	int i = 0;

	mission_num = 0;
#ifdef _CRYPTO_DATA		
	BOOL crypto = FALSE;
	fp = fopen( "./data/mission.txt.allblues", "r");
	if( fp != NULL ){
		crypto = TRUE;
	}else
#endif
{
	fp = fopen("./data/mission.txt", "r");
}
	if (fp == NULL)
	{
		print("ÈÎÎñÎÄ¼þ´ò¿ª´íÎó\n");
		return FALSE;
	}
	
	memset( missionlist, 0, sizeof(missionlist));

	while(1){
		char	line[1024];
		char	token[1024];
		int		mindex;
		if (fgets(line, sizeof(line), fp) == NULL)	break;
#ifdef _CRYPTO_DATA		
		if(crypto==TRUE){
			DecryptKey(line);
		}
#endif
		chop(line);
		// ÒÔ#Îª×¢½â*******
		if( line[0] == '#' )
			continue;
		for( i=0; i<strlen(line); i++ ){
            if( line[i] == '#' ){
			    line[i] = '\0';
		        break;
			}
		}
		//*************************************

		//¸ñÊ½ #ÈÎÎñ±àºÅ,±ØÒªµÈ¼¶,ÈÎÎñËµÃ÷,½±Æ·ID,ÏÞÖÆÊ±¼ä(Ð¡Ê±)
		
		getStringFromIndexWithDelim(line, ",", 1, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		mindex = atoi( token);

		if( mindex <= 0 || mindex >= MAXMISSION) 
			break;

		missionlist[mindex].id = mindex;

		getStringFromIndexWithDelim(line, ",", 2, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		missionlist[mindex].level = atoi( token);

		getStringFromIndexWithDelim(line, ",", 3, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		strcpy( missionlist[mindex].eventflag, token);

		getStringFromIndexWithDelim(line, ",", 4, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		strcpy( missionlist[mindex].detail, token);

		//getStringFromIndexWithDelim(line, ",", 4, token, sizeof(token));
		//if (strcmp(token, "") == 0)	break;
		//strcpy( missionlist[mindex].bonus, token);

		getStringFromIndexWithDelim(line, ",", 5, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		missionlist[mindex].limittime = atoi( token);
/*
		print("\nMISSION[%d] lv:%d ef:%s detail:%s limit:%d ", mindex,
			missionlist[mindex].level, missionlist[mindex].eventflag,
			missionlist[mindex].detail, missionlist[mindex].limittime );
*/
		mission_num++;
		//if (mission_num > MAXMISSION)	break;
	}
	fclose(fp);
	return TRUE;
}



BOOL LoadMissionCleanList( )
{
	// ¸ñÊ½... Ê¹Õß,ÓÂÕß,ÈÎÎñ,½±ÉÍ
	FILE* fp;
	int	listindex =0;
	int i = 0;

	memset( missiontable, 0, sizeof(missiontable));
	
#ifdef _CRYPTO_DATA		
	BOOL crypto = FALSE;
	fp = fopen( "./data/missionclean.txt.allblues", "r");
	if( fp != NULL ){
		crypto = TRUE;
	}else
#endif
{
	fp = fopen("./data/missionclean.txt", "r");
}
	
	if (fp == NULL)
	{
		print("Çå³ýÈÎÎñÎÄ¼þ´ò¿ª´íÎó\n");
		return FALSE;
	}

	while(1){
		char	line[1024];
		char	token[1024];
		
		if (fgets(line, sizeof(line), fp) == NULL)	break;
#ifdef _CRYPTO_DATA		
		if(crypto==TRUE){
			DecryptKey(line);
		}
#endif
		chop(line);
		// ÒÔ#Îª×¢½â*******
		if( line[0] == '#' )
			continue;
		for( i=0; i<strlen(line); i++ ){
            if( line[i] == '#' ){
			    line[i] = '\0';
		        break;
			}
		}
		
		getStringFromIndexWithDelim(line, ",", 1, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		strcpy( missiontable[listindex].angelinfo, token);

		getStringFromIndexWithDelim(line, ",", 2, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		strcpy( missiontable[listindex].heroinfo, token);

		getStringFromIndexWithDelim(line, ",", 3, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		missiontable[listindex].mission = atoi( token);
		
		getStringFromIndexWithDelim(line, ",", 4, token, sizeof(token));
		//if (strcmp(token, "") == 0)	break;
		missiontable[listindex].flag = atoi( token);

		getStringFromIndexWithDelim(line, ",", 5, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		missiontable[listindex].time = atoi( token);

		print("\nMISSIONCLEAN[%d] %s %s %d %d %d", listindex,
			missiontable[listindex].angelinfo,
			missiontable[listindex].heroinfo,
			missiontable[listindex].mission,
			missiontable[listindex].flag,
			missiontable[listindex].time );

		listindex++;
		if ( listindex >= MAXMISSIONTABLE)	break;
	}
	fclose(fp);
	return TRUE;
}


#endif

#ifdef _JOBDAILY
extern  DailyFileType dailyfile[MAXDAILYLIST];
BOOL LoadJobdailyfile(void)
{
	char	line[20000];
	char	token[16384];
	int		listindex =0;
	int     i;
	FILE* fp;
#ifdef _CRYPTO_DATA		
	BOOL crypto = FALSE;
	fp = fopen( "./data/jobdaily.txt.allblues", "r");
	if( fp != NULL ){
		crypto = TRUE;
	}else
#endif
{
	fp = fopen("./data/jobdaily.txt", "r");
}
	if (fp == NULL)
	{
		print("ÈÕ³£¹¤×÷ÎÄ¼þ´ò¿ª´íÎó\n");
		return FALSE;
	}

	memset( dailyfile, 0, sizeof(dailyfile));

	while(1){		
		line[0]='\0';	
		if (fgets(line, sizeof(line), fp) == NULL)	break;
#ifdef _CRYPTO_DATA		
		if(crypto==TRUE){
			DecryptKey(line);
		}
#endif
		//print("\n %s ", line);
		chop(line);

		// #Îª×¢½â
		if( line[0] == '#' )
			continue;
		for( i=0; i<strlen(line); i++ ){
            if( line[i] == '#' ){
			    line[i] = '\0';
		        break;
			}
		}
		
		getStringFromIndexWithDelim(line, "|", 1, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		dailyfile[listindex].jobid = atoi(token);

		getStringFromIndexWithDelim(line, "|", 2, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		strcpy( dailyfile[listindex].rule, token);

		getStringFromIndexWithDelim(line, "|", 3, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		if(strlen(token)>64){
			print("\nÈÎÎñËµÃ÷¹ý³¤:%d\n",strlen(token));
			return FALSE;
		}
		strcpy( dailyfile[listindex].explain, token);
		
		getStringFromIndexWithDelim(line, "|", 4, token, sizeof(token));
		if (strcmp(token, "") == 0)	break;
		strcpy( dailyfile[listindex].state, token);

		/*print("\ndailyfile[%d] %s %s %s %s", listindex,
			dailyfile[listindex].jobid,
			dailyfile[listindex].rule,
			dailyfile[listindex].explain,
			dailyfile[listindex].state);	
		*/
		listindex++;
		if ( listindex >= MAXDAILYLIST)	break;
	}
	fclose(fp);
	return TRUE;
}
#endif

#ifdef _USER_EXP_CF
BOOL LoadEXP( char* filename )
{
	FILE* fp;
	int i = 0;
	int MaxLevel=0;
	
#ifdef _CRYPTO_DATA		
	char realopfile[256];
	BOOL crypto = FALSE;
	sprintf(realopfile, "%s.allblues", filename);
	fp = fopen( realopfile, "r");
	if( fp != NULL ){
		crypto = TRUE;
	}else
#endif
{
	fp = fopen(filename, "r");
}
	if (fp == NULL)
	{
		print("ÎÞ·¨´ò¿ªÎÄ¼þ\n");
		return FALSE;
	}

	while(1){
		char	line[64], exp[64];
		if (fgets(line, sizeof(line), fp) == NULL)	break;
#ifdef _CRYPTO_DATA		
		if(crypto==TRUE){
			DecryptKey(line);
		}
#endif
		chop(line);

		if( line[0] == '#' )
			continue;
		for( i=0; i<strlen(line); i++ ){
            if( line[i] == '#' ){
			    line[i] = '\0';
		        break;
			}
		}
		//*************************************
		MaxLevel = MaxLevel + 1;
		if (MaxLevel >= 200)	break;
		easyGetTokenFromString(line, 2, exp, sizeof(exp));
		NeedLevelUpTbls[MaxLevel]=atoi(exp);
	}
	fclose(fp);
	return TRUE;
}

char* getEXPfile( void )
{
    return config.expfile;
}

int getNeedLevelUpTbls( int level )
{
    return NeedLevelUpTbls[level];
}

#endif

#ifdef _UNREG_NEMA
char* getUnregname( int index )
{
    return config.unregname[index];
}
#endif

#ifdef _TRANS_LEVEL_CF
int getChartrans( void )
{
		if(config.chartrans>6)
			config.chartrans=5;
    return config.chartrans;
}
int getPettrans( void )
{
		if(config.pettrans>2)
			return 2;
		else if(config.pettrans<-1)
			return -1;
    return config.pettrans;
}
int getYBLevel( void )
{
		if(config.yblevel>config.maxlevel)
			config.yblevel=config.maxlevel;
    return config.yblevel;
}
int getMaxLevel( void )
{
    return config.maxlevel;
}
#endif

#ifdef _POINT
char* getPoint( void )
{
		return (config.point>0)? "ÊÇ":"·ñ";
}
int getTransPoint( int index )
{
		return config.transpoint[index];
}
#endif

#ifdef _PET_AND_ITEM_UP
char* getPetup( void )
{
		return (config.petup>0)? "ÊÇ":"·ñ";
}
char* getItemup( void )
{
		return (config.itemup>0)? "ÊÇ":"·ñ";
}
#endif
#ifdef _LOOP_ANNOUNCE
char* getLoopAnnouncePath( void )
{
		return config.loopannouncepath;
}
int loadLoopAnnounce( void )
{
	FILE* fp;
	int i = 0;
	config.loopannouncemax=0;
	fp = fopen(config.loopannouncepath, "r");
	if (fp == NULL)
	{
		print("ÎÞ·¨´ò¿ªÎÄ¼þ\n");
		return FALSE;
	}
	while(1){
		char	line[1024];
		if (fgets(line, sizeof(line), fp) == NULL)	break;
		chop(line);

		if( line[0] == '#' )
			continue;
		for( i=0; i<10; i++ ){
      if( line[i] == '#' ){
			  line[i] = '\0';
		    break;
			}
		}
		//*************************************
		strcpy(config.loopannounce[config.loopannouncemax],line);
		config.loopannouncemax++;
	}
	fclose(fp);
	return TRUE;
}
int getLoopAnnounceTime( void )
{
    return (config.loopannouncetime<0)?-1:config.loopannouncetime;
}
int getLoopAnnounceMax( void )
{
    return (config.loopannouncemax>0)?config.loopannouncemax:0;
}
char* getLoopAnnounce( int index )
{
    return config.loopannounce[index];
}
#endif

#ifdef _SKILLUPPOINT_CF
int getSkup( void )
{
    return (config.skup>0)?config.skup:0;
}
#endif
#ifdef _RIDELEVEL
int getRideLevel( void )
{
    return config.ridelevel;
}
int getRideTrans( void )
{
    return config.ridetrans;
}
#endif
#ifdef _REVLEVEL
char* getRevLevel( void )
{
		return (config.revlevel>0)?"ÊÇ":"·ñ";
}
#endif
#ifdef _NEW_PLAYER_RIDE
char* getPlayerRide( void )
{
		if(config.npride>2)
			return "ÅäÌ×ËÍ»¢¼ÓÀ×";
		else if(config.npride==2)
			return "ÅäÌ×ËÍÀ×";
		else if(config.npride==1)
			return "ÅäÌ×ËÍ»¢";
		else
			return "²»ËÍÅäÌ×Æï³è";
}
#endif

#ifdef _FIX_CHARLOOPS
int getCharloops( void )
{
    return config.charloops-1;
}
#endif

#ifdef _PLAYER_ANNOUNCE
int getPAnnounce( void )
{
    return max(-1, config.pannounce);
}
#endif
#ifdef _PLAYER_MOVE
int getPMove( void )
{
    return max(-1, config.pmove);
}
#endif

int getrecvbuffer( void )
{
		if(config.recvbuffer<0)
	    return 0;
	  else if(config.recvbuffer>128)
	    return 128;
	  else
	  	return config.recvbuffer;
}

int getsendbuffer( void )
{
		if(config.sendbuffer<0)
	    return 0;
	  else if(config.sendbuffer>128)
	    return 128;
	  else
	  	return config.sendbuffer;
}

int getrecvlowatbuffer( void )
{
		if(config.recvlowatbuffer<0)
	    return 0;
	  else if(config.recvlowatbuffer>1024)
	    return 1024;
	  else
	  	return config.recvlowatbuffer;
}

int getrunlevel( void )
{
		if(config.runlevel<-20)
	    return -20;
	  else if(config.runlevel>19)
	    return 19;
	  else
	  	return config.runlevel;

}

#ifdef _SHOW_VIP_CF
int getShowVip( void )
{
		if(config.showvip>2)
			return 2;
		else if(config.showvip<0)
			return 0;
		else
			return config.showvip;
}
#endif

#ifdef _PLAYER_NUM
int getPlayerNum( void )
{
		return config.playernum;
}
void setPlayerNum( int num )
{
		config.playernum=num;
}
#endif

#ifdef _BATTLE_GOLD
int getBattleGold( void )
{
		if(config.battlegold<0)
	    return 0;
	  else if(config.battlegold>100)
	    return 100;
	  else
	  	return config.battlegold;

}
#endif

#ifdef _ANGEL_TIME
int getAngelPlayerTime( void )
{
		return (config.angelplayertime>1)?config.angelplayertime:1;
}
int getAngelPlayerMun( void )
{
		return (config.angelplayermun>2)?config.angelplayermun:2;
}
#endif

#ifdef _RIDEMODE_20
int getRideMode( void )
{
		if(config.ridemode<0)
			config.ridemode=0;
		return config.ridemode;
}
#endif
#ifdef _FM_POINT_PK
char *getFmPointPK( void )
{
		return (config.fmpointpk>0)?"ÊÇ":"·ñ";
}
#endif
#ifdef _ENEMY_ACTION
int getEnemyAction( void )
{
		if(config.enemyact>100)
			return 100;
		else if(config.enemyact<1)
			return 1;
		else
			return config.enemyact;
}
#endif

#ifdef _FUSIONBEIT_TRANS
int getFusionbeitTrans( void )
{
		if(config.fusionbeittrans>2)
			return 2;
		else if(config.fusionbeittrans<0)
			return 0;
		else
			return config.fusionbeittrans;
}
#endif
int getCpuUse( void )
{
	return config.cpuuse;
}
#ifdef _CHECK_PEPEAT
int getCheckRepeat( void )
{
		return (config.CheckRepeat>0)?1:0;
}
#endif

#ifdef _FM_JOINLIMIT
int getJoinFamilyTime( void )
{
		return config.joinfamilytime;
}
#endif

#ifdef _MAP_HEALERALLHEAL
int getMapHeal( int index )
{
	char mapheal[256];
	getStringFromIndexWithDelim(config.mapheal,",", index+1, mapheal, sizeof(mapheal));
	return atoi(mapheal);
}
#endif

#ifdef _THE_WORLD_SEND
int getTheWorldTrans()
{
		return config.thewordtrans;
}
int getTheWorldLevel()
{
		return config.thewordlevel;
}
int getTheWorldSend()
{
		return config.thewordsend;
}
#endif

#ifdef _LOGIN_DISPLAY
int getLoginDisplay()
{
		return config.logindisplay;
}
#endif

#ifdef _VIP_POINT_PK
int getVipPointPK( int index )
{
	char vippointpk[256];
	if(getStringFromIndexWithDelim(config.vippointpk,",", index+1, vippointpk, sizeof(vippointpk))==TRUE){
		return atoi(vippointpk);
	}else{
		return -1;
	}
}
float getVipPointPKCost(void)
{
	return config.vippointpkcost / 100.00;
}
#endif

#ifdef _SPECIAL_MAP
int getSpecialMap( int index )
{
	char specialmap[256];
	if(getStringFromIndexWithDelim(config.specialmap,",", index+1, specialmap, sizeof(specialmap))==TRUE){
		return atoi(specialmap);
	}else{
		return -1;
	}
}
#endif

#ifdef _NEW_AUTO_PK
int getAutoPkTime()
{	
		if(config.autopk>23)config.autopk=23;
		if(config.autopk<0)config.autopk=-1;
		return config.autopk;
}
int getAutoPkTrans()
{	
		return config.autopktrans;
}

int getAutoPkLv()
{	
		return config.autopklv;
}
#ifdef _FORMULATE_AUTO_PK
int getAutoPkPoint()
{
		return config.autopkpoint;
}
int getKillPoint()
{
		return config.killpoint;
}
#endif
#endif

#ifdef _AUTO_DEL_PET
int getAutoDelPet( int index )
{
	char autodelpet[256];
	getStringFromIndexWithDelim(config.autodelpet,",", index+1, autodelpet, sizeof(autodelpet));
	return atoi(autodelpet);
}
#endif

#ifdef _AUTO_DEL_ITEM
int getAutoDelItem( int index )
{
	char autodelitem[256];
	getStringFromIndexWithDelim(config.autodelitem,",", index+1, autodelitem, sizeof(autodelitem));
	return atoi(autodelitem);
}
#endif

#ifdef _BT_PET
int getBtPet()
{
	return config.btpet;
}
#endif

#ifdef _BT_ITEM
int getBtItem()
{
	return config.btitem;
}
#endif

#ifdef _LUCK_STAR
int getLuckStarTime()
{
	if(config.luckstartime < 1)config.luckstartime=1;
	return config.luckstartime;
}
int getLuckStarChances()
{
	return config.luckstarchances;
}
#endif

#ifdef _BATTLE_GETITEM_RATE
int getBattleGetItemRate( int index )
{
	char battlegetitemrate[256];
	getStringFromIndexWithDelim(config.battlegetitemrate,",", index+1, battlegetitemrate, sizeof(battlegetitemrate));
	return atoi(battlegetitemrate);
}
int getBattleGetItemRateMap()
{
	return config.battlegetitemratemap;
}
#endif

#ifdef _UNLAW_THIS_LOGOUT
int getUnlawThisLogout( int index )
{
	char unlawthislogout[256];
	getStringFromIndexWithDelim(config.unlawthislogout,",", index+1, unlawthislogout, sizeof(unlawthislogout));
	return atoi(unlawthislogout);
}
#endif

#ifdef _TRANS_POINT_UP
int getTransPoinUP( int index )
{
	char transpointup[256];
	getStringFromIndexWithDelim(config.transpointup,",", index+1, transpointup, sizeof(transpointup));
	return atoi(transpointup);
}
#endif

#ifdef _OPEN_STW_SEND
int getOpenStwSendType( void )
{
	if(config.stwsendtype<-1){
		config.stwsendtype=-1;
	}else if(config.stwsendtype>5){
		config.stwsendtype=5;
	}
	return config.stwsendtype;
}
int getOpenStwSendPoint( void )
{
	return max(1, config.stwsendpoint);
}

#endif

#ifdef _POOL_ITEM_BUG
int getPoolItemBug( void )
{
	return config.poolitembug;
}
int getPoolItem( int id )
{
	if(id<0)id = 0;
	char poolitem[256];
	if(getStringFromIndexWithDelim(config.poolitem,",", id + 1, poolitem, sizeof(poolitem))==TRUE){
		return atoi(poolitem);
	}else{
		return -1;
	}
}

#endif

#ifdef _NO_STW_ENEMY
int getNoSTWNenemy( void )
{
	return config.nostwenemy;
}

int getNoSTWNenemyPoint( void )
{
	return config.nostwenemypoint;
}
#endif

#ifdef _NEW_STREET_VENDOR
int getStreetVendor( int id )
{
	if(id<0)id = 0;
	char streetvendorpoint[256];
	if(getStringFromIndexWithDelim(config.streetvendorpoint,",", id + 1, streetvendorpoint, sizeof(streetvendorpoint))==TRUE){
		return atoi(streetvendorpoint);
	}else{
		return -1;
	}
}

#endif

#ifdef _ITEM_PET_LOCKED
int getItemPetLocked( void )
{
	return config.itampetlocked;
}
#endif


#ifdef _TALK_SAVE
int getSaveFame( void )
{
	return max(0, config.savefame*100);
}
#endif

#ifdef _TALK_CHECK
int getTalkCheck( void )
{
	return (rand() % max(10, config.talkcheckmax)) + max(10, config.talkcheckmin);
}
#endif

#ifdef _DISABLE_PROFESSION_SKILL
BOOL getDisableProfessionSkill( int floor )
{
	int i=1;
	char buff[64];
	while(getStringFromIndexWithDelim(config.disableprofessionskill,",", i, buff, sizeof(buff))){
		if(atoi(buff) == floor)return TRUE;
		i++;
	}
	return FALSE;
}
#endif
#ifdef _ALL_SERV_SEND
int getAllServTrans()
{
		return config.allservtrans;
}
int getAllServLevel()
{
		return config.allservlevel;
}
int getAllServSend()
{
		return config.allservsend;
}
#endif
#ifdef _PET_TRANS_ABILITY
int getPetTransAbility()
{
	if(config.pettransability<=0)config.pettransability=0;
	return config.pettransability;
}
int getPetTransAbility1()
{
	if(config.pettransability1<=0)config.pettransability1=150;
	return config.pettransability1;
}
int getPetTransAbility2()
{
	if(config.pettransability2<=0)config.pettransability2=200;
	return config.pettransability2;
}
#endif

#ifdef _NEED_ITEM_ENEMY
int getDelNeedItem()
{
		return config.delneeditem;
}
#endif

#ifdef _NOT_ESCAPE
int getNotEscape( int index)
{
	char notescape[256];
	if(getStringFromIndexWithDelim(config.notescape,",", index+1, notescape, sizeof(notescape))==TRUE){
		return atoi(notescape);
	}else{
		return -1;
	}
}
#endif

#ifdef _PLAYER_OVERLAP_PK
int getPlayerOverlapPk( int index )
{
	char playeroverlappk[256];
	if(getStringFromIndexWithDelim(config.playeroverlappk,",", index+1, playeroverlappk, sizeof(playeroverlappk))==TRUE){
		return atoi(playeroverlappk);
	}else{
		return -1;
	}
}
#endif

#ifdef _FIMALY_PK_TIME
int getFimalyPkTime()
{
		return config.fimalypktime;
}
#endif

#ifdef _PETSKILL_SHOP_LUA
char *getFreePetSkillShopPath()
{
		return config.freepetskillshoppath;
}
#endif


#ifdef _CANCEL_ANGLE_TRANS
int getCancelAngleTrans()
{
		return config.cancelanlgetrans;
}
#endif

#ifdef _VIP_BATTLE_EXP
int getVipBattleexp()
{
		return config.vipbattleexp;
}
#endif

#ifdef _NO_HELP_MAP
int getNoHelpMap( int index )
{
	char nohelpmap[256];
	if(getStringFromIndexWithDelim(config.nohelpmap,",", index+1, nohelpmap, sizeof(nohelpmap))==TRUE){
		return atoi(nohelpmap);
	}else{
		return -1;
	}
}
#endif

#ifdef _BATTLE_TIME
int getBattleTime()
{
		return config.battletime;
}
#endif

#ifdef _SAME_IP_ONLINE_NUM
int getSameIpOnlineNum()
{
		return config.sameiponlinenum;
}
#endif

#ifdef _STREET_VENDOR_TRANS
int getStreetVendorTrans()
{
		return config.streetvendortrans;
}
#endif

#ifdef _CHECK_SEVER_IP
int checkServerIp( unsigned int ip )
{
	char serverip[64];
	char cliip[64];
	int i = 1;

	int a, b, c, d;
	
	a=(ip % 0x100); ip=ip / 0x100;
	b=(ip % 0x100); ip=ip / 0x100;
	c=(ip % 0x100); ip=ip / 0x100;
	d=(ip % 0x100);

	sprintf(cliip, "%d.%d.%d.%d", a, b, c, d);

	while(getStringFromIndexWithDelim(config.serverip,",", i++, serverip, sizeof(serverip))==TRUE){
		if(strcmp(cliip, serverip) == 0){
			return TRUE;
		}
	}

	return FALSE;
}
#endif

#ifdef _DAMMAGE_CALC
int getDamageCalc()
{
	if(config.dammagecalc > 0){
		return config.dammagecalc;
	}else{
		return 70;
	}
}
#endif


#ifdef _PET_ENEMY_DEVELOP_UP
int getPetEnemyDevelopUp()
{
		return config.PetEnemyDevelopUp;
}
#endif


#ifdef _FIRST_LOCK_ITEM
int getFirstLockItem( int index )
{
	char FirstLockItem[256];
	if(getStringFromIndexWithDelim(config.FirstLockItem,",", index+1, FirstLockItem, sizeof(FirstLockItem))==TRUE){
		return atoi(FirstLockItem);
	}else{
		return -1;
	}
}
#endif
int getConnectnum()
{
		return config.Connectnum;
}

#ifdef _PETSKILL_NEW_PASSIVE
int getAutoGetSkill(void)
{
	if (config.autogetskill==1) return 1;

		return 0;
}

int getSkillFusion(void)
{
		return config.skillfusion;
}

int getSkillCount(void)
{
	if (config.skillcount<1|| config.skillcount>6) return 1;
		return config.skillcount;
}

//»ñµÃ¼¼ÄÜÉú³ÉµÄÎ»ÖÃ
int getSkillPos(void)
{
	if (config.getskillpos<1 || config.getskillpos>7) return 6;
		return config.getskillpos-1;
}

//¸ù¾Ý¼¼ÄÜµÈ¼¶»ñµÃÏ°µÃ¸ÅÂÊ
int getSkillProb(int skillLevel){
	char prob[20];
	if(getStringFromIndexWithDelim(config.skillinfolv[skillLevel-1],"|", 1, prob, sizeof(prob)) == FALSE)
		return 0;
	return atoi(prob);
}

//¸ù¾Ý¼¼ÄÜµÄµÈ¼¶»ñµÃ³õÊ¼»òÕß½áÎ²µÄ¼¼ÄÜÐòºÅ skillLevel¼¼ÄÜµÈ¼¶  start 1Ê¼2ÖÕ
int getSkillRange(int skillLevel,int start){
	char index[20];
	if(getStringFromIndexWithDelim(config.skillinfolv[skillLevel-1],"|", start+1, index, sizeof(index)) == FALSE)
		return -1;
	return atoi(index);

}

//»ñµÃÈÚºÏ³èÎïµÄ·¶Î§ PETID
int getFusionRange(int start){
	char index[20];
	if(getStringFromIndexWithDelim(config.fusionrange,"|", start, index, sizeof(index)) == FALSE)
		return -1;
	return atoi(index);

}
//ÊÇ·ñÊÇÈÚºÏ³èÎï
int isFusionPet(int petId){
	if ((petId>=getFusionRange(1)&& petId<=getFusionRange(2))|| petId==1045) return 1;
	return 0;
}

//Èç¹ûÊÇ±»¶¯¼¼ÄÜ·µ»ØÖµÎªÕýÊý ·ñÔòÎª¸´Êý
int isPassiveSkill(int skillId){
	if (skillId<0) return -1;
	char start[20];
	char end[20];
	int i;
	for (i=0;i<5;i++){
		if(getStringFromIndexWithDelim(config.skillinfolv[i],"|", 2, start, sizeof(start)) == FALSE)
			return -1;
		if(getStringFromIndexWithDelim(config.skillinfolv[i],"|", 3, end, sizeof(end)) == FALSE)
			return -1;
		if (skillId>=atoi(start) && skillId<=atoi(end))
			return 1;
	}
	return -1;
}
//Èç¹ûÊÇ±»¶¯¼¼ÄÜ·µ»ØÖµÎª¼¼ÄÜµÄµÈ¼¶   ·ñÔòÎª¸´Êý
int getSkillLevel(int skillId){
	if (skillId<0) return -1;
	char start[20];
	char end[20];
	int i;
	for (i=0;i<5;i++){
		if(getStringFromIndexWithDelim(config.skillinfolv[i],"|", 2, start, sizeof(start)) == FALSE)
			return -1;
		if(getStringFromIndexWithDelim(config.skillinfolv[i],"|", 3, end, sizeof(end)) == FALSE)
			return -1;
		if (skillId>=atoi(start) && skillId<=atoi(end))
			return i+1;
	}
	return -1;
}
//¸ù¾ÝµÈ¼¶Ëæ»ú»ñµÃ±»¶¯¼¼ÄÜ
int getRandSkillByLevel(int skillLevel){
	if (skillLevel<0||skillLevel>5) return -1;
	return RAND(getSkillRange(skillLevel,1),getSkillRange(skillLevel,2));
}

//Ëæ»ú»ñµÃÒ»Ã¶±»¶¯¼¼ÄÜ
int getRandSkill(void){
	int i,k;
	int prob[5];
	for (i=1;i<=5;i++){
		prob[i-1]=getSkillProb(i);
	}
	k=RAND(0,10000);
	if (k<=prob[0]){
		return getRandSkillByLevel(1);
	}else if(k<=(prob[0]+prob[1])){
		return getRandSkillByLevel(2);
	}else if(k<=(prob[0]+prob[1]+prob[2])){
		return getRandSkillByLevel(3);
	}else if(k<=(prob[0]+prob[1]+prob[2]+prob[3])){
		return getRandSkillByLevel(4);
	}else if(k<=(prob[0]+prob[1]+prob[2]+prob[3]+prob[4])){
		return getRandSkillByLevel(5);
	}else{
		return -1;
	}
}
#endif
#ifdef _SHARE_EXP
int getExpShare(void){
	if (config.expshare ==1)
		return 1;
	else
		return 0;
}
#endif
#ifdef _DEFEND_BIGBAO
int getBigBao(void){
	if(config.bigbao<=0) return 1024;
	return config.bigbao;
}
int getBigBao2(void){
	if(config.bigbao2<=0) return 1024;
	return config.bigbao2;
}
#endif

#ifdef _MO_SHOW_DEBUG
int isDebug(void){
		return config.isdebug;
}
#endif
#ifdef _CHAR_LOOP_TIME
int getCharLoopTime()
{
		return config.charlooptime;
}
#endif
#ifdef _MO_RELOAD_NPC
int getReloadNpcTime(void){
	if(config.reloadnpctime<=0) return 0;
	return config.reloadnpctime;
}

int getReloadNpcType(void){
	if(config.reloadnpctype<=0) return 0;
	return 1;
}
#endif
#ifdef _JZ_NEWSCRIPT_LUA
char *getLuaFile(void)
{
	return config.luafile;
}
BOOL ITEM_LuaInit(const char * _FileName){
	if (strlen( _FileName) != 0) return TRUE;
	return FALSE;
}
#endif

#ifdef _ITEM_LUA
const char *getitemluafile(void){
	return config.itemluafile;
}
#endif
#ifdef _MO_LNS_NLSUOXU
int GetConfigLineType(char *TM_ConfigName)
{
	int i;

	for( i = 0 ; i < arraysizeof( readconf ) ; i ++ ){
		if (strcmp(readconf[i].name,TM_ConfigName) == 0) break;
	}
	if (i == arraysizeof( readconf )) return -1;
	switch( readconf[i].valuetype  ){
	case CHAR:
		return 0;
		break;
	case SHORT:
		return 1;
		break;
	case INT:
		return 2;
		break;
	case DOUBLE:
		return 3;
		break;
	}
}

char* GetConfigLineVal( char* TM_ConfigName )
{
	char* filename = getConfigfilename();
	FILE* f=NULL;
	char linebuf[256];                  /* Óòµæ  ÐÄ  ¸êÌïÓÀ°×Ñë */
	int linenum=0;                      /* µæÐÑÃ«ÐÑÒüÔÂ */
	char    realopenfilename[256];      /*    ¶Ë±åopen ÔÊÔÂ°×ÑëÄÌ»ï  */
	char secondToken[256];
	char    hostname[128];


	if( f == NULL ){
		f=fopen( filename , "r" );          /* °×ÑëÄÌ»ï¼°×ó¡õÃó¼þ */
		if( f == NULL ){
			print( "Can't open %s\n", filename );
			return FALSE;
		}
	}

	/* ÓòµæÎð¹´  ÐÄ  ¸ê */
	while( fgets( linebuf , sizeof( linebuf ), f ) ){
		char firstToken[256];       /*1    ¼°  Ù¯  */
		int i;                      /*»ï¡õÃó  ÐÑ*/
		int ret;                    /*ØøÕý¡õ¼þÎì¡õÓñ*/

		linenum ++;

		deleteWhiteSpace(linebuf);          /* remove whitespace    */

		if( linebuf[0] == '#' )continue;        /* comment */
		if( linebuf[0] == '\n' )continue;       /* none    */

		chomp( linebuf );                    /* remove tail newline  */

		/* delim "=" Æ¥  âÙ(1)¼°Ðþ¡õÛÍ¼þÃ«  ÔÂ*/
		ret = getStringFromIndexWithDelim( linebuf , "=",  1, firstToken,
			sizeof(firstToken) );
		if( ret == FALSE ){
			print( "Find error at %s in line %d. Ignore\n",
				filename , linenum);
			continue;
		}
		if (strcmp(firstToken,TM_ConfigName) != 0) continue;
		/* readconf ¼°ÈÓÄÌÊõÆ¥»ï¡õÃó */
		for( i = 0 ; i < arraysizeof( readconf ) ; i ++ ){
			if( strcmp( readconf[i].name ,firstToken ) == 0 ){
				/* match */
				/* delim "=" Æ¥2    ¼°Ðþ¡õÛÍ¼þÃ«  ÔÂ*/
				ret = getStringFromIndexWithDelim( linebuf , "=" , 2
					, secondToken ,
					sizeof(secondToken) );

				/* NULL  Ù¯¾®Éýµ¤¾®Ã«Æ©ÍÍÔÂ */
				if( ret == FALSE ){
					print( "Find error at %s in line %d. Ignore",
						filename , linenum);
					break;
				}
				return secondToken;
				break;
			}
		}
		break;
	}
	fclose( f );
	lastConfig();
	return NULL;
}

int getCopymapstartingID( void )
{
	return config.mapstart;
}
#endif
#ifdef _ROOKIE_ITEM
unsigned int getRookieItem(int index)
{
	if (config.rookieitem[index]>0) return config.rookieitem[index];
	return 0;
}
#endif
#ifdef _NO_TRANS_ITEM
unsigned int getNoTransItem(void)
{
	return config.notransitem;
}
#endif
#ifdef _MAX_MERGE_LEVEL
unsigned int getMaxMergeLevel(void)
{
	return config.maxmergelevel;
}
#endif
#ifdef _NO_ATTACK
int getAttTime(void)
{
	return config.atttime;
}
int getAttSafeTime(void)
{
	return config.attsafetime;
}
int getAttCnt(void)
{
	return config.attcnt;
}
int getLateTime(void)
{
	return config.latetime;
}
int getAttDmeTime(void)
{
	return config.attdmetime;
}
int getAttDmeCnt(void)
{
	return config.attdmecnt;
}
#endif
char* getNoAttIp(int num)
{
	if(num<0) num=0;
	else if(num>4) num=4;
	return config.noattip[num];
}
#ifdef _NO_FULLPLAYER_ATT
int getNoFullPlayer(void){
	return config.nofullplayer;
}
int getNoFull2Player(void){
	return config.nofull2player;
}
int getNoCdkeyPlayer(void){
	if(config.nocdkeyplayer<0 || config.nocdkeyplayer>2000)
		return 0;
	return config.nocdkeyplayer;
}
int getNoCdkeyMode(void){
	if(config.nocdkeymode<0 || config.nocdkeymode>2)
		return 0;
	return config.nocdkeymode;
}
int getNoCdkeyType(void){
	if(config.nocdkeytype<0 || config.nocdkeytype>2)
		return 0;
	return config.nocdkeytype;
}
int getNoFullTime(void){
	return config.nofulltime;
}
int getFengType(void){
	return config.fengtype;
}
int getNoFullEndPlayer(void){
	return config.nofullendplayer;
}
int getNoFullEndTime(void){
	return config.nofullendtime;
}
int getManRenNum(void){
	return config.manrennum;
}
#endif

int getLockType(void){
	if(config.locktype<=0) return 0;
	return config.locktype;
}

#ifdef _NEW_FUNC_DECRYPT
unsigned int getAllowerrornum2( void )
{
    return config.allowerrornum2;
}
#endif
#ifdef _MO_LOGIN_NO_KICK
int getLoginNoKick(void){
		if (config.loginnokick==1) return 1;
		return 0;
}
#endif

#ifdef _MO_ILLEGAL_NAME
char* getIllegalName( int index )
{
	char illegalname[256];
	if(getStringFromIndexWithDelim(config.illegalname,",", index+1, illegalname, sizeof(illegalname))==TRUE){
		return illegalname;
	}else{
		return "\0";
	}
}
#endif


#ifdef _NO_USE_PACKET_MAP
int getNoPacketMap( int index )
{
	char NoPacketMap[256];
	if(getStringFromIndexWithDelim(config.nousepacketmap,",", index+1, NoPacketMap, sizeof(NoPacketMap))==TRUE){
		return atoi(NoPacketMap);
	}else{
		return -1;
	}
}
#endif
#ifdef _NO_USE_MAGIC_MAP
int getNoMagicMap( int index )
{
	char NoMagicMap[256];
	if(getStringFromIndexWithDelim(config.nousemagicmap,",", index+1, NoMagicMap, sizeof(NoMagicMap))==TRUE){
		return atoi(NoMagicMap);
	}else{
		return -1;
	}
}
#endif


#ifdef _SOME_PETMAIL
int getPetMailFlg(void){
		if(config.petmailflg==1){
    		return config.petmailflg;
		}
		return 0;
    }
int getPetMailPetid(unsigned int index )
{
	char petid[256];
	if(getStringFromIndexWithDelim(config.somepetmail,",", index+1, petid, sizeof(petid))==TRUE){
		return atoi(petid);
	}else{
		return -1;
	}
}
#endif
#ifdef _CTRL_TRANS_DEVELOP
int getCtrlTrans(void){
		if(config.ctrltrans==1){
    		return config.ctrltrans;
		}
		return 0;
}
#endif

#ifdef	_PETMAIL_TIME
int getPetMailTime(void){
	if(config.petmailtime<=0) return 0;
	return config.petmailtime;
}
#endif
#ifdef _UP_BBPETPROB
int getUpBBProb(void){
	if(config.upbbprob==1) return 1;
	return 0;
}
#endif
