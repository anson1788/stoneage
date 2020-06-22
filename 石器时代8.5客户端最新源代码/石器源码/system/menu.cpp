#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/font.h"
#include "../other/caryIme.h"
#include "../systeminc/anim_tbl.h"
#include "../systeminc/pc.h"
#include "../systeminc/battleMenu.h"
#include "../systeminc/battleProc.h"
#include "../systeminc/lssproto_cli.h"
#include "../systeminc/netmain.h"
#include "../systeminc/loadsprbin.h"
#include "../systeminc/savedata.h"
#include "../systeminc/t_music.h"
#include "../systeminc/menu.h"
#include "../systeminc/tool.h"
#include "../systeminc/map.h"
#include "../systeminc/field.h"
#include "../systeminc/pet_skillinfo.h"
#include "../wgs/descrypt.h"
#include "../systeminc/character.h"
#include "../redMemoy.h"
#ifdef _TALK_WINDOW
#include "../systeminc/talkwindow.h"
#endif
#ifdef _MAGIC_ITEM_
ACTION* 道具光环Act=NULL;
extern ACTION* MagicItemActAddr;
extern int MagicItemCombinData[3];
extern int MagicItemPosState[4];
#endif
#ifdef _RENWU_
BOOL 任务查询开关=FALSE;
#endif
extern int MessageBoxNew(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
int 繁体开关 = FALSE;
int 经验开关 = TRUE;
int 人物屏蔽开关 = FALSE;
int 右键攻击 = FALSE;

#ifdef _PETBLESS_
int 祝福窗口开关 = FALSE;
char 祝福窗口内容[128];
int 祝福窗口选中;
#endif


#ifdef _AIDENGLU_
extern void ShowBottomLineString(int iColor, LPSTR lpstr);
extern Landed PcLanded;
#endif
#ifdef _NEWPANEL //Syu ADD 7.0 新人物状态介面
#ifdef _SYUTEST3
#define MENU_STATUS_0   25
#else
#define MENU_STATUS_0   17
#endif
#else
#define MENU_STATUS_0	12
#endif
#ifdef _DROPPETWND					// (可开放) Syu ADD 丢弃宠物确认
#define MENU_PET_0		24
#else
#define MENU_PET_0		20
#endif
#define MENU_ITEM_0		40
#define MENU_MAIL_0		40
#define MENU_ALBUM_0	20
#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
#define MENU_TRADE_0	22
#endif
#define MENU_BANK_0		20
#define MENU_BTL_RESULT_0	1
//Terry add 2003/11/19
extern bool g_bTradesystemOpen;
//end
#ifdef _READ16BITBMP
//extern BOOL g_bUseAlpha;
#endif
#ifdef _NEW_COLOR_
extern BOOL g_bUseAlpha;
#endif
// ????

#define MAX_CHAT_REGISTY_STR		8		// ?????????
#define MAX_CHAT_REGISTY_STR_LEN	26 		// ??????????

#ifdef _DROPPETWND					// (可开放) Syu ADD 丢弃宠物确认
bool DropPetWndflag = false;			//丢弃宠物视窗显示与否旗标
short DropI = -1;						//暂存i值
#endif
#ifdef _NEW_ITEM_
int 道具栏页数 = 0;
int 判断玩家道具数量()
{
	int ret = MAX_MAXHAVEITEM + MAX_ITEMSTART;
	if (pc.道具栏状态 & 1 << 1){
		ret += MAX_MAXHAVEITEM;
		if (pc.道具栏状态 & 1 << 2){
			ret += MAX_MAXHAVEITEM;
		}
	}
	return ret;
}
#endif
#ifdef _DIEJIA_
//物品是否能堆叠
BOOL ItemCanPile(int flg)
{
	return (flg & 1 << 6);
}
#endif
#ifdef _AniCrossFrame	  // Syu ADD 动画层游过画面生物
#define RAND(x,y)   ((x-1)+1+ (int)((double)(y-(x-1))*rand()/(RAND_MAX+1.0)))
int UpDownflag = 0;
#endif

#define CAHT_REGISTY_STR_FILE_NAME 	"data\\chatreg.dat" 	// ???????????
static int systemWndFontNo[MENU_SYSTEM_0]; 			// ????
unsigned int  systemWndNo;						// ??????
STR_BUFFER chatRegistryStr[MAX_CHAT_REGISTY_STR];		// ??????????????
int MouseCursorFlag = FALSE;							// ???????

#ifdef _TRADETALKWND				// (不可开) Syu ADD 交易新增对话框架
char talkmsg[4][256];					//四行的暂存Buffer
int talkwndx = 300, talkwndy = 350;   //起始位置
bool talkwndflag = false;				//是否可以拖曳旗标
bool tradetalkwndflag = false;			//是否产生对话窗旗标
#endif

#ifdef _MONEYINPUT //Syu ADD 手动输入金钱量
STR_BUFFER MymoneyBuffer;
bool Moneyflag = false;
#endif
STR_BUFFER TradeBuffer;
bool Tradeflag = false;    //输入位置Focus旗标
bool TradeBtnflag = false; //金额放置钮显示开关
// ???
int mapWndFontNo[MENU_MAP_0]; 	// ????
static unsigned int  mapWndNo;		// ??????
int MapWmdFlagBak;					// ???????????????

// ?????
static int statusWndFontNo[MENU_STATUS_0]; 	// ????
static int statusWndBtnFlag[MENU_STATUS_0];	// ???????
static unsigned int statusWndNo;				// ??????
STR_BUFFER shougouChange;						// ??????????
int StatusUpPoint;								// ????????????

#ifdef _TRADESYSTEM2	
static int tradeWndFontNo[MENU_TRADE_0]; 	// ????
static int tradeWndBtnFlag[MENU_TRADE_0];	// ???????
#endif
static unsigned int tradeWndNo = 0;				// ??????

int showindex[7] = { 0, 0, 0, 0, 0, 0, 0 };

static char opp_sockfd[128] = "-1";
static char opp_name[128] = "";
static char trade_command[128];
static char trade_kind[128];
static int opp_showindex;
static char opp_goldmount[1024];
static char opp_itemgraph[1024];
static char opp_itemname[1024];
static char opp_itemeffect[1024];
static char opp_itemindex[1024];
static char opp_itemdamage[1024];

struct showitem {
	char name[128];
	char freename[256];
	char graph[128];
	char effect[1024];
	char color[128];
	char itemindex[128];
	char damage[128];
};
static int tradePetIndex = -1;
#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
//１～１５为我方道具，１６～２０为我方宠物，２１为我方金钱
//２２～３６为对方道具，３７～４１为对方宠物，４２为对方金钱

static showitem opp_item[MAX_MAXHAVEITEM];	//交易道具阵列增为15个
struct showpet {
	char opp_petname[128];
	char opp_petfreename[128];
	char opp_petgrano[128];
	char opp_petlevel[128];
	char opp_petatk[128];
	char opp_petdef[128];
	char opp_petquick[128];
	char opp_petindex[128];
	char opp_pettrans[128];
	char opp_petshowhp[128];
	char opp_petslot[128];
	char opp_petskill1[128];
	char opp_petskill2[128];
	char opp_petskill3[128];
	char opp_petskill4[128];
	char opp_petskill5[128];
	char opp_petskill6[128];
	char opp_petskill7[128];
#ifdef _SHOW_FUSION
	char opp_fusion[64];
#endif
#ifdef _PET_ITEM
	PetItemInfo oPetItemInfo[MAX_PET_ITEM];			// 宠物身上的道具
#endif
};											//对方交易宠物能力暂存
static showpet opp_pet[5];
#ifdef _NEW_ITEM_
int itemflag[MAX_MAXHAVEITEM * 3];
#else
int itemflag[MAX_MAXHAVEITEM];
#endif
static ACTION *SecondActPet;				//第二视窗宠物Action
bool MainTradeWndflag = true;				//主视窗显示与否的flag
bool SecondTradeWndflag = false;			//第二视窗显示与否的flag
ACTION *SecondTradeWnd;						//第二视窗
ACTION *TradeTalkWnd = NULL;						//对话视窗
int ShowPetNum = 0;						//检视正在显示的宠物号
static int SecondtradeWndFontNo[6];		//第二视窗的按钮
int mytradelist[51] = { -1 };				//我方卷页内容顺序清单
int opptradelist[51] = { -1 };				//对方卷页内容顺序清单
int drag1Y = 67, drag2Y = 257;			//拖曳钮初始位置
int tmpdrag1Y, tmpdrag2Y;					//拖拽开始位置
bool dragflag1 = false, dragflag2 = false; //拖曳钮启动与否旗标
int locknum = -1, locknum2 = -1;
#endif
static PET tradePet[2];
static char tradepetindexget[128] = "-1";
#ifdef _PET_ITEM
static bool g_bPetItemWndFlag = false;
#endif

static ACTION *pActPet3 = NULL; //交易时主视窗我方Pet Action
static ACTION *pActPet4 = NULL;
static ACTION *pActPet5 = NULL;

struct tradelist {
	char kind;
	int data;
	char name[256];
	char damage[256];
	char freename[256];
	int level;
	int trns;
#ifdef _ITEM_PILENUMS
	int pilenum;
#endif
#ifdef _SHOW_FUSION
	int fusion;
#endif
#ifdef _NPC_ITEMUP
	int itemup;
#endif
};
#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
static tradelist tradeList[45];
#endif
static int mine_itemindex[2] = { -1, -1 };
static char mine_itemname[2][128];

static int bankGold;
static int bankWndFontNo[MENU_TRADE_0]; 	// ????
static int bankWndBtnFlag[MENU_TRADE_0];	// ???????
static int cashGold = 0;
static int totalGold = 0;
static int bankGoldInc = 0;
static int bankGoldCnt = 0;

#ifdef _TELLCHANNEL				//ROG ADD 密语频道
BOOL MultiTells = FALSE;
ACTION *pActMsgWnd;
int CharNum = 0;
char TellInfo[10][128];
char name[10][32];
int index[10];
char reSendMsg[STR_BUFFER_SIZE];
int  TalkMode = 0;						//0:一般 1:密语 2: 队伍 3:家族 4:职业 
char secretName[32] = { "" };				//要密之人的名称
#endif

#ifdef _FRIENDCHANNEL				//ROG ADD 好友频道
typedef struct{
	char	roomNo[4];
	char	chiefName[CHAR_NAME_LEN+1];
	int		chiefIndex;
	int		chiefFlag;
	int		memberNum;
	char    memberName[MAX_ROOM_NUM][CHAR_NAME_LEN+1];
	int     memberIndex[MAX_ROOM_NUM];
	char    roomName[32];
	char    nickName[MAX_ROOM_NUM][CHAR_FREENAME_LEN+1];
}CHATINFO;
CHATINFO chatInfo;

bool secretFlag = FALSE;				//密语旗标
bool BtnType = FALSE;					//按钮显示用
bool setRoomFlag = FALSE;
bool assentFlag = FALSE;				//同意视窗旗标
bool scrollFlag = FALSE;
char roomInfo[MAX_ROOM_NUM][64];		//选择聊天室用
char memInfo[64];						//member information
int  scrlBtnIndex = 0;					//scroll位移
int  BtnNo = 0;							//按钮编号
int  firMemNo = 0;						//第一个成员
int  selChar = -1;						//要密之人的编号
int  closeBtn ,leaveBtn ,delBtn,chaBtn,outBtn,scrlHBtn,scrlLBtn; //按钮图档
int  roomIndex[MAX_ROOM_NUM];
int roomNum = 0;
int memIndex = 0;						//member index
static int ChatRoomBtn[ 16 ];			//聊天室按钮
STR_BUFFER chatRoomName;				//设定聊天室名称用
ACTION *pSetRoomWnd;					//设定聊天室名称用
ACTION *pAssentWnd;						//同意视窗用
ACTION *pChtChanlWnd;					//聊天室视窗用
ACTION *pSelChanlWnd;					//选择聊天室视窗用
#endif

#ifdef _TIMEBAR_FUNCTION			//时间bar函式
ACTION *pTimeBarWnd;
char   timeBarTitle[32];
int    timeBarRange;
int    timeBarCurPos;
bool   timeBarFlag = FALSE;
bool   barHolder[2];                //用来切换时间条
void DrawTimeBar();
/////////TEST/////////////////
int StartTime = 0;
int timBarIdent = -1;
//////////////////////////////
#endif

// ???
static int petWndFontNo[MENU_PET_0]; 	// ????
static int petWndBtnFlag[MENU_PET_0]; // ???????
static unsigned int  petWndNo;			// ?????????
int  petStatusNo;				// ?????????????????
int  mixPetNo;					// ?????????
static ACTION *pActPet;					// ?????????????
BOOL BattlePetReceiveFlag;				// ?????????????
#ifdef _STANDBYPET
BOOL StandbyPetSendFlag = FALSE;
#endif
STR_BUFFER petNameChange;				// ????????
int BattlePetReceivePetNo = -1;			// ?????????
int SelectWazaNo;						// ????

// ????
static int itemWndFontNo[MENU_ITEM_0]; 	// ????
static int itemWndBtnFlag[MENU_ITEM_0];	// ???????
static unsigned int  itemWndNo;
static int  jujutuNo = -1;					// ??????
int  itemNo = -1;					// ??????????
static int  itemWndDropGold;				// ????
static int  itemWndDropGoldInc;				// ?????
static int  itemWndDropGoldCnt;				// ??????????

static int  tradeWndDropGold = 0;				// trade????
static int  tradeWndDropGoldInc = 0;			// trade?????
static int  tradeWndDropGoldCnt = 0;			// trade??????????
static int  tradeWndDropGoldSend = 0;
static int  tradeWndDropGoldGet = 0;
#ifdef _PET_ITEM
static int	nSelectPet;						// 打开道具栏时记录目前作用的宠物
#endif

ACTION *pActPet2;					// ??????????????
BOOL ItemMixRecvFlag;						// ??????
int ItemMixPetNo = -1;				// ??????????
static unsigned int ItemUseTime = 0;		// ???????????

// ?????????
ITEM_BUFFER ItemBuffer[MAX_ITEM];

#ifdef _PET_ITEM		
// 每一项的第一个是能装备时的底图图号,第二个是不能装时的底图图号
int nPetItemEquipBmpNumber[PET_EQUIPNUM][2] = 
{
	{26463, 26470}, {26460, 26467}, {26458, 26465}, {26461, 26468}, {26459, 26466},
	{26457, 26464}, {26462, 26469}
};
#endif
// ???
#define MAIL_HISTORY_FILE_NAME 	"data\\mail.dat" 	// ??????
#ifdef _TRANS_6
char *TransmigrationStr[ 7 ] = { "","壹","贰","参","肆","伍","陆" };
#else
char *TransmigrationStr[6] = { "零", "壹", "贰", "参", "肆", "伍" };

int Transmigrationcolor[6] = {0,4,5,3,6,2};


#endif
static int mailWndFontNo[MENU_MAIL_0];
static int mailWndBtnFlag[MENU_MAIL_0];			// ???????
static MAIL_WND_TYPE mailWndNo;						// ????????????
static MAIL_WND_TYPE mailWndNoBak;					// ??????????
static int mailViewWndPageNo;						// ????????????
static int mailSendWndPageNo;						// ????????????
static int mailPetSendWndPageNo;					// ??????????????
static int mailItemNo;								// ???????????????
static int mailWndSendFlag[MAX_ADR_BOOK]; 		// ???????
int mailHistoryWndPageNo;							// ????????????
int mailHistoryWndSelectNo;							// ??????
static int mailHistoryWndNowPageNo;					// ???????
STR_BUFFER MailStr;									// ??
MAIL_HISTORY MailHistory[MAX_ADR_BOOK];			// ????????
ACTION *pActLetter[4];							// ?????????????????
ACTION *pActMailItem = NULL;								// ????????????????

// ????
#define ALBUM_FILE_NAME 	"data\\album.dat" 	// ????????
#define ALBUM_FILE_NAME_4 	"data\\album_4.dat" 	// ????????
#define ALBUM_FILE_NAME_5 	"data\\album_5.dat" 	// ????????
#define ALBUM_FILE_NAME_6 	"data\\album_6.dat" 	// ????????
#define ALBUM_FILE_NAME_7   "data\\album_7.dat" 	// ????????
#define ALBUM_FILE_NAME_8   "data\\album_8.dat" 	// ????????
#define ALBUM_FILE_NAME_9   "data\\album_9.dat" 	// ????????
#define ALBUM_FILE_NAME_10  "data\\album_10.dat" 	// ????????
#define ALBUM_FILE_NAME_11  "data\\album_11.dat" 	// ????????
#define ALBUM_FILE_NAME_12  "data\\album_12.dat"
#define ALBUM_FILE_NAME_13	"data\\album_13.dat"
#define ALBUM_FILE_NAME_14	"data\\album_14.dat"
#define ALBUM_FILE_NAME_15  "data\\album_14.dat"
#define ALBUM_FILE_NAME_16  "data\\album_15.dat"
#define ALBUM_FILE_NAME_17  "data\\album_16.dat"
#define ALBUM_FILE_NAME_18  "data\\album_17.dat"
#define ALBUM_FILE_NAME_19  "data\\album_18.dat"
#define ALBUM_FILE_NAME_20  "data\\album_19.dat"
#define ALBUM_FILE_NAME_21  "data\\album_20.dat"
#define ALBUM_FILE_NAME_22  "data\\album_21.dat"
#define ALBUM_FILE_NAME_23  "data\\album_22.dat"
#define ALBUM_FILE_NAME_24  "data\\album_23.dat"
#define ALBUM_FILE_NAME_25  "data\\album_24.dat"
#define ALBUM_FILE_NAME_26  "data\\album_25.dat"
#define ALBUM_FILE_NAME_27  "data\\album_26.dat"
#define ALBUM_FILE_NAME_28  "data\\album_27.dat"
#define ALBUM_FILE_NAME_29  "data\\album_28.dat"
#define ALBUM_FILE_NAME_30  "data\\album_29.dat"
#define ALBUM_FILE_NAME_31  "data\\album_30.dat"
#define ALBUM_FILE_NAME_32  "data\\album_31.dat"
#define ALBUM_FILE_NAME_33  "data\\album_32.dat"
#define ALBUM_FILE_NAME_34  "data\\album_33.dat"
#define ALBUM_FILE_NAME_35  "data\\album_34.dat"
#define ALBUM_FILE_NAME_36  "data\\album_35.dat"
#define ALBUM_FILE_NAME_37  "data\\album_36.dat"
#define ALBUM_FILE_NAME_38  "data\\album_37.dat"
#define ALBUM_FILE_NAME_39  "data\\album_38.dat"
#define ALBUM_FILE_NAME_40  "data\\album_39.dat"
#define ALBUM_FILE_NAME_41  "data\\album_40.dat"
#define ALBUM_FILE_NAME_42  "data\\album_41.dat"
#define ALBUM_FILE_NAME_43  "data\\album_42.dat"
#define ALBUM_FILE_NAME_44  "data\\album_43.dat"
#define ALBUM_FILE_NAME_45  "data\\album_44.dat"
#define ALBUM_FILE_NAME_46  "data\\album_45.dat"
#define ALBUM_FILE_NAME_47  "data\\album_46.dat"
#define ALBUM_FILE_NAME_48  "data\\album_47.dat"

static int albumWndFontNo[MENU_ALBUM_0];
static int albumWndBtnFlag[MENU_ALBUM_0];
static unsigned int albumWndNo;
static int albumWndPageNo;					// ????
static int albumNo;						// ?????
// ????????????????
PET_ALBUM_TBL PetAlbumTbl[] = {
#include "../systeminc/petName.h"	// ???????????
};
PET_ALBUM PetAlbum[MAX_PET_KIND];		// ?????
int AlbumIdCnt = 0;					// ???????????????????
// ?????????
int IdEncryptionTbl[16] = { 48, 158, 98, 23, 134, 29, 92, 67,
70, 28, 235, 20, 189, 48, 57, 125 };

// ???????
int resultWndFontNo[MENU_BTL_RESULT_0];	// ????
int ResultWndTimer;							// ???????????

// ?????
#define TASK_BAR	7
#define TASK_BAR_X 320
#define TASK_BAR_Y 468 + DISPLACEMENT_Y 
BOOL TaskBarFlag = FALSE;	// ????????
// Terry add 2003/12/16 for 交易视窗开启时,显示物品说明不显示输入法
BOOL bShowItemExplain = FALSE;
// end
static int taskBarFontNo[TASK_BAR];
static int taskBarX = TASK_BAR_X, taskBarY = TASK_BAR_Y + 24;

// ??????
unsigned int MenuToggleFlag;
#ifdef _NEWREQUESTPROTOCOL			// (不可开) Syu ADD 新增Protocol要求细项
#define CHAR_MAX_DETAIL 8
char CharDetail[CHAR_MAX_DETAIL][16] ; 
char DetailDesc[CHAR_MAX_DETAIL][64] = {
	"地魔法抗性　：" ,
	"水魔法抗性　：" ,
	"火魔法抗性　：" ,	
	"风魔法抗性　：" ,
	"地魔法熟练度：" ,
	"水魔法熟练度：" ,
	"火魔法熟练度：" ,	
	"风魔法熟练度：" 
};
#endif
#ifdef _MAG_MENU
int DetailDescicon[CHAR_MAX_DETAIL]={26479, 26475, 26477, 26481, 26478, 26474, 26476, 26480};
char DetailDescchar[4][4]={"地","水","火","风"};
#endif

#ifdef _ALCHEPLUS
int iCharAlchePlus[25];
char sAlchePlus_list[25][16] =
{
	"石", "木", "骨", "牙", "皮",
	"贝壳", "壳", "爪", "花", "叶",
	"线", "黏土", "鳞", "毒", "石化",
	"混乱", "酒醉", "睡眠", "回复", "复活",
	"水晶", "地", "水", "火", "风"
};

int iAlchePlusIcon[25] = { 26536, 26529, 26545, 26534, 26535,	/*"石", "木",	"骨", "牙",	"皮"*/
26540, 26548, 26533, 26541, 26549,	/*"贝壳", "壳", "爪",	"花", "叶"*/
26551, 26552, 26553, 26542, 26537,	/*"线", "黏土", "鳞", "毒", "石化"*/
26546, 26544, 26550, 26538, 26547,	/*"混乱",	"酒醉",	"睡眠",	"回复",	"复活"*/
26531, 26539, 26530, 26532, 26543 };	/*"水晶",	"地", "水",	"火", "风"*/
#endif

// ??????????????
ACTION *pActMenuWnd;
ACTION *pActMenuWnd2;
ACTION *pActMenuWnd3;
ACTION *pActMenuWnd4;
ACTION *pActYesNoWnd;
ACTION *pActMenuWnd5;


void checkRidePet(int);
//andy_add 2002/06/24
int RIDEPET_getNOindex(int baseNo);
int RIDEPET_getPETindex(int PetNo, int learnCode);
int RIDEPET_getRIDEno(int index, int ti);

#ifdef RIDE_PET_LIMIT
int RIDEPET_getPETindex_New(int PetNo, int learnCode);
#endif

static char *monoStereoStr[] = { "       单声道       ",
"       立体声       " };

static char *mouseCursor[] = { "     正  常     ",
"     平  滑     " };

#define WINDOW_CREATE_FRAME 10	// ??????????????

// ????????????????
extern unsigned char crs_change_tbl[];
// ????????????????
extern unsigned char crs_change_tbl2[];
// ????
extern unsigned char crs_bound_tbl[][32];

#ifdef _STREET_VENDOR
extern short sStreetVendorBuyBtn;
#endif

#ifdef __ONLINEGM
extern BOOL OnlineGmFlag;
#endif

#ifdef _PET_ITEM
/*	0x01:PET_HEAD	// 头
	0x02:PET_WING	// 翼
	0x04:PET_TOOTH	// 牙
	0x08:PET_PLATE	// 身体
	0x10:PET_BACK	// 背
	0x20:PET_CLAW	// 爪
	0x40:PET_FOOT	// 脚(鳍)
	*/
// 不同种的宠物会有能装和不能装的部位
unsigned char byShowPetItemBackground[MAX_PET_SPECIES + 1] =
{
	/*	乌力	布比	拳击手	克克尔	凯比	布克	蝙蝠	乌宝宝	飞天蛙	飞龙*/
	0x5d,	0x5d,	0x7d,	0x5d,	0x5d,	0x7d,	0x5f,	0x5d,	0x5f,	0x7f,
	/*	大舌头	乌龟	鲨鱼	海主人	猪鱼龙	呼拔拔	人龙	三角龙	雷龙	暴龙*/
	0x5d,	0x5d,	0x5d,	0x5d,	0x5d,	0x7d,	0x7d,	0x5d,	0x5d,	0x7d,
	/*	水龙	猩猩	巴克	老虎	长毛象	鸟系	羊系	穿山甲	特殊系	软体系*/
	0x5d,	0x7d,	0x7d,	0x5d,	0x5d,	0x5f,	0x5d,	0x5d,	0x09,	0x1d,
	/*	小鱼	贝壳	棘皮	鱼龙	虾蟹	鲸豚	魟系	海牛系	大鲨鱼	旗鱼系*/
	0x5d,	0x79,	0x1d,	0x5d,	0x79,	0x5d,	0x5d,	0x5d,	0x5d,	0x5d,
	/*	大鱼系	鳄龙系	融合宠*/
	0x5d,	0x5d,	0x00
};
#endif

int charDetailPage = 3;

#ifdef _ITEM_JIGSAW
bool DrawJigsawFlag = false;
int JigsawIdx = 0;
int JigsawImg[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
int JigsawOK[][3] = {//道具图号,第一块图号,底图图号
	{ 25151, 25261, 25241 }, { 25152, 25270, 25242 }, { 25153, 25279, 25243 }, { 25154, 25288, 25244 }, { 25155, 25297, 25245 }, { 25156, 25306, 25246 }, { 25157, 25315, 25247 }, { 25158, 25324, 25248 }, { 25159, 25333, 25249 }, { 25150, 25252, 25250 }
};
void SetJigsaw(int img, char *str)
{
	int i, start;
	for (i = 0; i < 10; i++){
		if (img == JigsawOK[i][0]){
			JigsawImg[0] = JigsawOK[i][2];
			start = JigsawOK[i][1];
			break;
		}
	}
	for (i = 0; i < 9; i++){
		if (str[i] == '1')
			JigsawImg[i + 1] = start + i;
		else
			JigsawImg[i + 1] = -1;
	}
}
bool CheckJigsaw(int img)
{
	if (img >= 25151 && img <= 25240)
		return true;
	return false;
}
#endif

#ifdef _NEWPANEL //Syu ADD 7.0 新人物状态介面
bool SkillWndflag = false ; 
short SkillWnd = 0 ; 
ACTION *pActSkillMenuWnd;
void SkillWndfunc2() {
	int x =0, y=0, j=0 ; 
	char buf[256];
	char msg[256];
	char msg2[256];
	if( pActSkillMenuWnd == NULL ) {
		pActSkillMenuWnd = MakeWindowDisp( 354 , 0, 300, 456, 0, -1 );
	}else{
		if( pActSkillMenuWnd->hp > 0 ){			
			StockDispBuffer( ( ( WINDOW_DISP *)pActSkillMenuWnd->pYobi )->mx - 10, ( ( WINDOW_DISP *)pActSkillMenuWnd->pYobi )->my - 5 , DISP_PRIO_MENU, CG_FIELD_SKILL_PANEL, 1 );
			x = pActSkillMenuWnd->x + 2; 
			y = pActSkillMenuWnd->y - 5; 
			statusWndFontNo[ 15 ] = StockDispBuffer( x + 201, y + 442, DISP_PRIO_IME3, CG_CLOSE_BTN , 2 )  ;
			StockFontBuffer( x + 58 , y + 40  , FONT_PRIO_FRONT, 1 , pc.profession_class_name , 0 );
			sprintf_s( msg , "%d" , pc.profession_skill_point ) ; 
			StockFontBuffer( x + 228 , y + 40  , FONT_PRIO_FRONT, 1 , msg , 0 );
			memset( msg, -1, sizeof( msg ) );
		}
	}
	for ( int i = 0 ; i < 3 ; i ++ ) {
#ifdef _READ16BITBMP
		if(g_bUseAlpha){
			if( MakeHitBox( x + 254 , y + 15 + i * 78, x + 284  , y + 93 + i * 78 , DISP_PRIO_IME3 ) == TRUE ){
				if( mouse.onceState & MOUSE_LEFT_CRICK ) {
					SkillWnd = i ; 
					play_se( 217, 320, 240 );
					break;
				}
			}
		}
		else
#endif
		if( MakeHitBox( x + 250 , y + 18 + i * 79, x + 280  , y + 96 + i * 79 , DISP_PRIO_IME3 ) == TRUE ) {
			if( mouse.onceState & MOUSE_LEFT_CRICK ) {
				SkillWnd = i ; 
				play_se( 217, 320, 240 );
				break;
			}
		}
	}
	if( mouse.onceState & MOUSE_LEFT_CRICK ) {
		if( HitDispNo == statusWndFontNo[ 15 ] ) {
			SkillWndflag = false ;  
			play_se( 217, 320, 240 );
		}
	}
	extern int BattleSkill[20];
	extern int AssitSkill[20];
	extern int AdvanceSkill[20];
	for ( int i = 0 ; i < 4 ; i ++ ) {
		for ( j = 0 ; j < 4 ; j ++ ) {
			StockDispBuffer( x + 143 + j * 60 , y + 210 + i * 57 , DISP_PRIO_IME3, CG_FIELD_SKILL_TILE , 0 );
		}
	}
	sprintf_s( msg , " ");
	switch ( SkillWnd ) {
	case 0 :
#ifdef _READ16BITBMP
		if(g_bUseAlpha) StockDispBuffer( x + 147, y + 225, DISP_PRIO_IME3, CG_FIELD_SKILL_ASSIT , 0 );
		else
#endif
			StockDispBuffer( x + 138, y + 228, DISP_PRIO_IME3, CG_FIELD_SKILL_ASSIT , 0 );
		for ( int i = 0 ; i < 4 ; i ++ ) {
			for ( int j = 0 ; j < 4 ; j ++ ) {
				if ( AssitSkill [ j + i * 4 ] == -1 ) 
					break;
				if ( HitDispNo == StockDispBuffer( x + 29 + j * 60 , y + 102 + i * 57 , DISP_PRIO_IME3 + 1 , profession_skill[ AssitSkill [ j + i * 4 ] ].icon  , 2 ) ) {

					int use_color = 0;
					if( pc.mp >= profession_skill[AssitSkill [ j + i * 4 ]].costmp && profession_skill[AssitSkill [ j + i * 4 ]].costmp != 0 ){
						use_color = FONT_PAL_WHITE;
					}else{
						use_color = FONT_PAL_GRAY;									
					}

					sprintf_s( msg , "%s" , profession_skill[AssitSkill [ j + i * 4 ]].name );
					StockFontBuffer( 355, 310 + DISPLACEMENT_Y, FONT_PRIO_FRONT, use_color, msg, 0 );

					sprintf_s( msg , "%d％", profession_skill[AssitSkill [ j + i * 4 ]].skill_level ) ; 
					StockFontBuffer( 440, 310 + DISPLACEMENT_Y, FONT_PRIO_FRONT, use_color, msg, 0 );

					sprintf_s( msg , "(耗费MP:%d)" , profession_skill[AssitSkill [ j + i * 4 ]].costmp );
					StockFontBuffer( 500, 310 + DISPLACEMENT_Y, FONT_PRIO_FRONT, use_color, msg, 0 );

					sprintf_s( msg , "%s" , profession_skill[AssitSkill [ j + i * 4 ]].memo ) ; 
#ifdef _OUTOFBATTLESKILL			// (不可开) Syu ADD 非战斗时技能Protocol
					if ( mouse.onceState & MOUSE_LEFT_CRICK && profession_skill[ AssitSkill [ j + i * 4 ] ].useFlag == 0 ) {
						if( pc.mp >= profession_skill[AssitSkill [ j + i * 4 ]].costmp && profession_skill[AssitSkill [ j + i * 4 ]].costmp != 0 ) {							
							lssproto_BATTLESKILL_send ( sockfd , AssitSkill [ j + i * 4 ] ) ; 
							play_se( 217, 320, 240 );
						}
					}
#endif
				}
				sprintf_s( msg2 , "%8s" , profession_skill[AssitSkill [ j + i * 4 ]].name );
				StockFontBuffer(x + j * 60 , y + 110 + i * 57 , FONT_PRIO_FRONT, profession_skill[ AssitSkill [ j + i * 4 ] ].useFlag + 1 , msg2 , 2 );
			}
		}
		break;
	case 1 :
#ifdef _READ16BITBMP
		if(g_bUseAlpha) StockDispBuffer( x + 147, y + 225, DISP_PRIO_IME3, CG_FIELD_SKILL_BATTLE , 0 );
		else
#endif
			StockDispBuffer( x + 138, y + 228, DISP_PRIO_IME3, CG_FIELD_SKILL_BATTLE , 0 );
		for ( int i = 0 ; i < 4 ; i ++ ) {
			for ( j = 0 ; j < 4 ; j ++ ) {	
				if ( BattleSkill [ j + i * 4 ] == -1 ) 
					break;
				if ( HitDispNo == StockDispBuffer( x + 29 + j * 60 , y + 102 + i * 57 , DISP_PRIO_IME3 + 1 , profession_skill[ BattleSkill [ j + i * 4 ] ].icon  , 2 ) ) {

					int use_color = 0;
					if( pc.mp >= profession_skill[BattleSkill [ j + i * 4 ]].costmp && profession_skill[BattleSkill [ j + i * 4 ]].costmp != 0 ){
						use_color = FONT_PAL_WHITE;
					}else{
						use_color = FONT_PAL_GRAY;									
					}

					sprintf_s( msg , "%s" , profession_skill[BattleSkill [ j + i * 4 ]].name );
					StockFontBuffer( 355, 310 + DISPLACEMENT_Y, FONT_PRIO_FRONT, use_color, msg, 0 );

					sprintf_s( msg , "%d％", profession_skill[BattleSkill [ j + i * 4 ]].skill_level ) ; 
					StockFontBuffer( 440, 310 + DISPLACEMENT_Y, FONT_PRIO_FRONT, use_color, msg, 0 );

					sprintf_s( msg , "(耗费MP:%d)" , profession_skill[BattleSkill [ j + i * 4 ]].costmp );
					StockFontBuffer( 500, 310 + DISPLACEMENT_Y, FONT_PRIO_FRONT, use_color, msg, 0 );

					sprintf_s( msg , "%s" , profession_skill[BattleSkill [ j + i * 4 ]].memo ) ; 
#ifdef _OUTOFBATTLESKILL			// (不可开) Syu ADD 非战斗时技能Protocol
					if ( mouse.onceState & MOUSE_LEFT_CRICK && profession_skill[ BattleSkill [ j + i * 4 ]].useFlag == 0 ) {
						if( pc.mp >= profession_skill[BattleSkill [ j + i * 4 ]].costmp && profession_skill[BattleSkill [ j + i * 4 ]].costmp != 0 ) {
							lssproto_BATTLESKILL_send ( sockfd , BattleSkill [ j + i * 4 ] ) ; 
							play_se( 217, 320, 240 );
						}
					}
#endif
				}
				sprintf_s( msg2 , "%8s" , profession_skill[BattleSkill [ j + i * 4 ]].name );
				StockFontBuffer(x + j * 60 , y + 110 + i * 57 , FONT_PRIO_FRONT, profession_skill[ BattleSkill [ j + i * 4 ]].useFlag + 1 , msg2 , 0 );
			}
		}
		break;
	case 2:
#ifdef _READ16BITBMP
		if(g_bUseAlpha) StockDispBuffer( x + 147, y + 225, DISP_PRIO_IME3, CG_FIELD_SKILL_ADVSK , 0 );
		else
#endif
			StockDispBuffer( x + 138, y + 228, DISP_PRIO_IME3, CG_FIELD_SKILL_ADVSK , 0 );
		for ( int i = 0 ; i < 4 ; i ++ ) {
			for ( j = 0 ; j < 4 ; j ++ ) {
				if ( AdvanceSkill [ j + i * 4 ] == -1 ) 
					break;
				if ( HitDispNo == StockDispBuffer( x + 29 + j * 60 , y + 102 + i * 57 , DISP_PRIO_IME3 + 1 , profession_skill[ AdvanceSkill [ j + i * 4 ] ].icon  , 2 ) ) {

					int use_color = 0;
					if( pc.mp >= profession_skill[AdvanceSkill [ j + i * 4 ]].costmp && profession_skill[AdvanceSkill [ j + i * 4 ]].costmp != 0 ){
						use_color = FONT_PAL_WHITE;
					}else{
						use_color = FONT_PAL_GRAY;									
					}

					sprintf_s( msg , "%s" , profession_skill[AdvanceSkill [ j + i * 4 ]].name );
					StockFontBuffer( 355, 310 + DISPLACEMENT_Y, FONT_PRIO_FRONT, use_color, msg, 0 );

					sprintf_s( msg , "%d％", profession_skill[AdvanceSkill [ j + i * 4 ]].skill_level ) ; 
					StockFontBuffer( 440, 310 + DISPLACEMENT_Y, FONT_PRIO_FRONT, use_color, msg, 0 );

					sprintf_s( msg , "(耗费MP:%d)" , profession_skill[AdvanceSkill [ j + i * 4 ]].costmp );
					StockFontBuffer( 500, 310 + DISPLACEMENT_Y, FONT_PRIO_FRONT, use_color, msg, 0 );

					sprintf_s( msg , "%s" , profession_skill[AdvanceSkill [ j + i * 4 ]].memo ) ; 
#ifdef _OUTOFBATTLESKILL			// (不可开) Syu ADD 非战斗时技能Protocol
					if ( mouse.onceState & MOUSE_LEFT_CRICK && profession_skill[AdvanceSkill [ j + i * 4 ]].useFlag == 0 ) {
						if( pc.mp >= profession_skill[AdvanceSkill [ j + i * 4 ]].costmp && profession_skill[AdvanceSkill [ j + i * 4 ]].costmp != 0 ) {							
							lssproto_BATTLESKILL_send ( sockfd , AdvanceSkill [ j + i * 4 ] ) ; 
							play_se( 217, 320, 240 );
						}
					}
#endif
				}
				sprintf_s( msg2 , "%8s" , profession_skill[AdvanceSkill [ j + i * 4 ]].name );
				StockFontBuffer(x + j * 60 , y + 110 + i * 57 , FONT_PRIO_FRONT, profession_skill[AdvanceSkill [ j + i * 4 ]].useFlag + 1 , msg2 , 0 );
			}
		}
		break;
	}
	char *splitPoint =  msg  ; 
	sprintf_s( buf , "%s" , msg ) ; 
	x = 355 ; 
	y = 340 ; 
	while( 1 ){
		if( strlen( splitPoint ) > 34 ) {
			strncpy_s( msg, splitPoint, 34 );
			buf[ 34 ] = NULL;
			if( GetStrLastByte( buf ) == 3 ){
				buf[ 33 ] = NULL;
				splitPoint += 33;
			}else{
				buf[ 34 ] = NULL;
				splitPoint += 34;
			}
			StockFontBuffer( x, y , FONT_PRIO_FRONT, 0, buf, 0 ); y += 24;
		}else{
			strcpy( buf, splitPoint );
			StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, buf, 0 );
			break;
		}
	}
}
#endif

#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
#ifdef _TRADETALKWND				// (不可开) Syu ADD 交易新增对话框架
void TradeTalk(char *msg) {
	char buf[256];
	if (pActMenuWnd4 != NULL || SecondTradeWnd != NULL) {
		sprintf_s(buf, "%s", msg);
		//判断是否由买卖双方说出
		if (strstr(buf, pc.name) || strstr(buf, opp_name)) {
			//内容转移
			while (1){
				if (strlen(msg) > 44) {
					strncpy_s(buf, msg, 44);
					buf[44] = NULL;
					if (GetStrLastByte(buf) == 3){
						buf[43] = NULL;
						msg += 43;
					}
					else{
						buf[44] = NULL;
						msg += 44;
					}
					for (int i = 0; i < 3; i++)
						strcpy(talkmsg[i], talkmsg[i + 1]);
					sprintf_s(talkmsg[3], "%s", buf);
				}
				else{
					for (int i = 0; i < 3; i++)
						strcpy(talkmsg[i], talkmsg[i + 1]);
					strcpy(buf, msg);
					sprintf_s(talkmsg[3], "%s", buf);
					break;
				}
			}
			tradetalkwndflag = true;
		}
	}
}
#endif
void LockAndOkfunction()
{
	char buffer[1024];
	char myitembuff[1024];
	char mypetbuff[1024];
	char oppitembuff[1024];
	char opppetbuff[1024];
	// 双方皆按下确定键状态
	if (pc.trade_confirm == 4) {
		// 处理按下确定键
		if ((MainTradeWndflag == true && HitDispNo == tradeWndFontNo[0]) ||
			//andy_reEdit 2003/04/27
			(SecondTradeWndflag == true && HitDispNo == SecondtradeWndFontNo[2])){
			// 对双方交易物品做成buffer	
			sprintf_s(myitembuff, "T|%s|%s|K|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|"
				, opp_sockfd, opp_name
				, tradeList[1].data, tradeList[2].data
				, tradeList[3].data, tradeList[4].data
				, tradeList[5].data, tradeList[6].data
				, tradeList[7].data, tradeList[8].data
				, tradeList[9].data, tradeList[10].data
				, tradeList[11].data, tradeList[12].data
				, tradeList[13].data, tradeList[14].data
				, tradeList[15].data);
			sprintf_s(mypetbuff, "P|%d|P|%d|P|%d|P|%d|P|%d|G|%d|"
				, tradeList[16].data, tradeList[17].data
				, tradeList[18].data, tradeList[19].data
				, tradeList[20].data, tradeList[21].data);
			sprintf_s(oppitembuff, "I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|"
				, tradeList[22].data, tradeList[23].data
				, tradeList[24].data, tradeList[25].data
				, tradeList[26].data, tradeList[27].data
				, tradeList[28].data, tradeList[29].data
				, tradeList[30].data, tradeList[31].data
				, tradeList[32].data, tradeList[33].data
				, tradeList[34].data, tradeList[35].data
				, tradeList[36].data);
			sprintf_s(opppetbuff, "P|%d|P|%d|P|%d|P|%d|P|%d|G|%d|"
				, tradeList[37].data, tradeList[38].data
				, tradeList[39].data, tradeList[40].data
				, tradeList[41].data, tradeList[42].data);
			sprintf_s(buffer, "%s%s%s%s", myitembuff, mypetbuff, oppitembuff, opppetbuff);
			lssproto_TD_send(sockfd, buffer);
			tradeStatus = 2;
		}
	}
	// 对方按下Lock或我方要按下Lock键 
	if (pc.trade_confirm == 1 || pc.trade_confirm == 3) {
		if ((MainTradeWndflag == true && HitDispNo == tradeWndFontNo[0]) ||
			//andy_reEdit 2003/04/27
			(SecondTradeWndflag == true && HitDispNo == SecondtradeWndFontNo[2])) {
			//对方尚未Lock进入状态 2
			if (pc.trade_confirm == 1) pc.trade_confirm = 2;
			//对方已经Lock进入状态 4
			if (pc.trade_confirm == 3)	pc.trade_confirm = 4;
			sprintf_s(buffer, "T|%s|%s|C|confirm", opp_sockfd, opp_name);
			lssproto_TD_send(sockfd, buffer);
		}
	}
	// 处理 Lock 、 确定键 End
}
#endif

// ?????????? ***************************************************/
void WindowDisp(ACTION *pAct)
{
	// ???????
	WINDOW_DISP *pYobi = (WINDOW_DISP *)pAct->pYobi;
	int i, j;
	int x = pAct->x + 32, y = pAct->y + 24;
	// ????
	switch (pAct->actNo){

	case 0:	// ????????

		StockBoxDispBuffer(pYobi->mx - pYobi->nowX,
			pYobi->my - pYobi->nowY,
			pYobi->mx + pYobi->nowX,
			pYobi->my + pYobi->nowY,
			pYobi->boxDispPrio, SYSTEM_PAL_BLACK, 0);
		// ????
		pYobi->nowX += pAct->dx;
		pYobi->nowY += pAct->dy;
		// ????????
		pYobi->cnt++;
		// ????????
		if (pYobi->cnt >= WINDOW_CREATE_FRAME){
			// ?????
			if (pYobi->wndType == -1) pAct->actNo = 1;
			else
				// ????????
			if (pYobi->wndType == -2) pAct->actNo = 3;
			else pAct->actNo = 2;
		}
		break;
	case 1:	// ?????
		pAct->hp = 1;
		break;
	case 2:	// ??????
		for (j = 0; j < pYobi->sizeY; j++){
			// 
			if (j == 0){
				for (i = 0; i < pYobi->sizeX; i++){
					// ?
					if (i == 0) StockDispBuffer(x, y, DISP_PRIO_MENU, pYobi->wndType + 0, pYobi->hitFlag);
					// ?
					else if (i == pYobi->sizeX - 1) StockDispBuffer(x, y, DISP_PRIO_MENU, pYobi->wndType + 2, pYobi->hitFlag);
					// ??
#ifdef _READ16BITBMP
					else if(g_bUseAlpha){
						if(pYobi->wndType == CG_WND_G_0) StockDispBuffer( x, y, DISP_PRIO_MENU, pYobi->wndType + 1, pYobi->hitFlag );
						else if(pYobi->wndType == CG_WND2_G_0)StockDispBuffer( x, y+1, DISP_PRIO_MENU, pYobi->wndType + 1, pYobi->hitFlag );
						else StockDispBuffer( x, y, DISP_PRIO_MENU, pYobi->wndType + 1, pYobi->hitFlag );
					}
#endif
					else StockDispBuffer(x, y, DISP_PRIO_MENU, pYobi->wndType + 1, pYobi->hitFlag);
					x += 64; // ????
				}
			}
			else
				// ?
			if (j == pYobi->sizeY - 1){
				for (i = 0; i < pYobi->sizeX; i++){
					// ?
					if (i == 0) StockDispBuffer(x, y, DISP_PRIO_MENU, pYobi->wndType + 6, pYobi->hitFlag);
					// ?
					else if (i == pYobi->sizeX - 1) StockDispBuffer(x, y, DISP_PRIO_MENU, pYobi->wndType + 8, pYobi->hitFlag);
					// ??
#ifdef _READ16BITBMP
					else if(g_bUseAlpha){
						if(pYobi->wndType == CG_WND_G_0) StockDispBuffer( x, y, DISP_PRIO_MENU, pYobi->wndType + 7, pYobi->hitFlag );
						else if(pYobi->wndType == CG_WND2_G_0) StockDispBuffer( x, y-1, DISP_PRIO_MENU, pYobi->wndType + 7, pYobi->hitFlag );
						else StockDispBuffer( x, y, DISP_PRIO_MENU, pYobi->wndType + 7, pYobi->hitFlag );
					}
#endif
					else StockDispBuffer(x, y, DISP_PRIO_MENU, pYobi->wndType + 7, pYobi->hitFlag);
					x += 64; // ????
				}
			}
			else
				// ??
			for (i = 0; i < pYobi->sizeX; i++){
				// ?
				if (i == 0) StockDispBuffer(x, y, DISP_PRIO_MENU, pYobi->wndType + 3, pYobi->hitFlag);
				// ?
#ifdef _READ16BITBMP
				else if(g_bUseAlpha){
					if( i == pYobi->sizeX - 1 ){
						if(pYobi->wndType == CG_WND_G_0) StockDispBuffer( x, y, DISP_PRIO_MENU, pYobi->wndType + 5, pYobi->hitFlag );
						else if(pYobi->wndType == CG_WND2_G_0) StockDispBuffer( x-1, y, DISP_PRIO_MENU, pYobi->wndType + 5, pYobi->hitFlag );
					}
					else if( i == pYobi->sizeX - 1 ) StockDispBuffer( x, y, DISP_PRIO_MENU, pYobi->wndType + 5, pYobi->hitFlag );
					else StockDispBuffer( x, y, DISP_PRIO_MENU, pYobi->wndType + 4, pYobi->hitFlag );
				}
#endif
				else if (i == pYobi->sizeX - 1) StockDispBuffer(x, y, DISP_PRIO_MENU, pYobi->wndType + 5, pYobi->hitFlag);
				// ??
				else StockDispBuffer(x, y, DISP_PRIO_MENU, pYobi->wndType + 4, pYobi->hitFlag);
				x += 64; // ????
			}
			x = pAct->x + 32; 	// ????
			y += 48; 		// ????
		}
		// ?????
		if (pYobi->titleNo != 0) StockDispBuffer(pYobi->titleX, pYobi->titleY, DISP_PRIO_IME3, pYobi->titleNo, pYobi->hitFlag);
		pAct->hp = 1;
		break;
	case 3:	// ??????
		// ????????
		if (mouse.onceState & MOUSE_LEFT_CRICK){
			// ????
			if (HitDispNo == pYobi->yesDispNo){
				pYobi->yesNoResult = TRUE;
			}
			// ?????
			if (HitDispNo == pYobi->noDispNo){
				pYobi->yesNoResult = FALSE;
			}
		}
		// ??????????
		StockDispBuffer(pYobi->mx, pYobi->my, DISP_PRIO_YES_NO_WND, CG_COMMON_WIN_YORO, 1);
		// ?????
		pYobi->yesDispNo = StockDispBuffer(pYobi->mx, pYobi->my, DISP_PRIO_YES_NO_BTN, CG_COMMON_YES_BTN, 2);
		// ??????
		pYobi->noDispNo = StockDispBuffer(pYobi->mx, pYobi->my, DISP_PRIO_YES_NO_BTN, CG_COMMON_NO_BTN, 2);
		break;
	}
}

// ????????? **************************************************************/
//	??：	int x,y 			????
//			int sizeX,sizeY 	??????????????????????????
//			int titleNo 		?????????????????????????
//			int wndType 		?：?????????????
//			 					?：??????????????
//								?：????????????
//								?：?????????????
//								?：?????????????
//								-1：???
//****************************************************************************************/
ACTION *MakeWindowDisp(int x, int y, int sizeX, int sizeY, int titleNo, int wndType
#ifdef _NEW_RESOMODE  //800 600模式
	, BOOL fixType /*= TRUE*/
#endif
	)
{
	ACTION *pAct;
	WINDOW_DISP *pYobi;
#ifdef _NEW_RESOMODE  //800 600模式
	if (fixType == TRUE){
		if (x > 320){
			x += DISPLACEMENT_X;
		}
		else if (x > 40)
		{
			x += DISPLACEMENT_X / 2;
		}
		if (y > 40)
			y += DISPLACEMENT_Y / 2;
	}
#endif
	pAct = GetAction(PRIO_JIKI, sizeof(WINDOW_DISP));
	if (pAct == NULL) return NULL;

	// ???
	pAct->func = WindowDisp;
	// ?
	pAct->dispPrio = DISP_PRIO_MENU;
	// ?????
	pAct->atr |= ACT_ATR_HIT;
	// ???
	pAct->atr |= ACT_ATR_HIDE;

	// ???????
	pYobi = (WINDOW_DISP *)pAct->pYobi;
	// ????
	pAct->x = x;
	pAct->y = y;
	pYobi->titleX = x + (sizeX * 64) / 2;
	pYobi->titleY = pAct->y + 27;
	// ??????
	pYobi->sizeX = sizeX;
	pYobi->sizeY = sizeY;
	// ???????
	pYobi->titleNo = titleNo;
	pYobi->boxDispPrio = DISP_PRIO_MENU;
	// ??????????
	if (wndType == 0) pYobi->wndType = CG_WND_G_0;
	else if (wndType == 1) pYobi->wndType = CG_WND_G_0;
	else if (wndType == 2) pYobi->wndType = CG_WND_G_0;
	else if (wndType == 3) pYobi->wndType = CG_WND_G_0;
	else if (wndType == 4){
		pYobi->wndType = -2;
		// ??????
		pYobi->yesDispNo = -2;
		pYobi->noDispNo = -2;
		pYobi->boxDispPrio = DISP_PRIO_YES_NO_WND;
	}
	else pYobi->wndType = -1;

	// YesNo ??????
	pYobi->yesNoResult = -1;

	// ??????
	if (wndType >= 2) pYobi->hitFlag = FALSE;
	else pYobi->hitFlag = TRUE;

	// ???
	// ????????????????
	if (wndType == -1 || wndType == 4){
		pYobi->mx = sizeX / 2 + pAct->x;
		pYobi->my = sizeY / 2 + pAct->y;
	}
	else{	// ????????
		pYobi->mx = (sizeX * 64) / 2 + pAct->x;
		pYobi->my = (sizeY * 48) / 2 + pAct->y;
	}
	// ??
	pAct->dx = (pYobi->mx - pAct->x) / WINDOW_CREATE_FRAME;
	pAct->dy = (pYobi->my - pAct->y) / WINDOW_CREATE_FRAME;

	return pAct;
}

// ???????????? ***********************************************/
void AnimDisp(ACTION *pAct)
{
	int flag = FALSE;
	int x, y, i;

	// ?????
	switch (pAct->actNo){

	case ANIM_DISP_PET:	// ???

		// ????????
		if (pAct->hitDispNo == HitDispNo && mouse.autoState & MOUSE_LEFT_CRICK)
		{
			// ??
			pAct->anim_ang++;
			// ????????
			if (pAct->anim_ang >= 8) pAct->anim_ang = 0;
			play_se(217, 320, 240); // ?????
		}
		// ???????
		pattern(pAct, ANM_NOMAL_SPD, ANM_LOOP);

		break;

	case ANIM_DISP_PET_SEND:	// ?????????????

		// ??
		pAct->anim_no = ANIM_WALK;
		// ?????????( ??? )( ??????? )
		pAct->anim_ang = 2;
		// ?
		pAct->crs = crs_change_tbl2[pAct->anim_ang];
		// ?????
		pAct->dx++;
		// ?????????
		if (pAct->dx % 3 == 0){
			// ?????
			if (pAct->spd < 20){
				pAct->spd++;
			}
		}
		// ?????????
		if (pAct->dx % 5 == 0){
			if (pAct->dy >= 2) pAct->dy--;
		}
		// ???
		gemini(pAct);

		// ???????
		if (pActMailItem != NULL){
			// ??????????
			if (pAct->x <= pActMailItem->x){
				pActMailItem->x = pAct->x;
			}
		}
		// ?????????????
		if (pAct->x <= -100){
			// ??????
			DeathAction(pActMenuWnd);
			pActMenuWnd = NULL;
			// ????
			DeathAction(pActPet);
			pActPet = NULL;
			// ????????
			DeathAction(pActMailItem);
			pActMailItem = NULL;
			// ????????
			mailWndNo = MAIL_WND_VIEW;
			// ????????
			play_se(203, 320, 240);
			// ????????
			GetKeyInputFocus(&MyChatBuffer);
		}
		// ???????
		pattern(pAct, pAct->dy, ANM_LOOP);

		break;

	case ANIM_DISP_PET_RECIEVE:	// ???????????

		// ??
		pAct->anim_no = ANIM_WALK;
		// ?????????( ??? )( ??????? )
		pAct->anim_ang = 6;
		// ?
		pAct->crs = crs_change_tbl2[pAct->anim_ang];
		// ?????
		pAct->dx++;
		// ?????
		if (pAct->spd > 0){
			// ?????????
			if (pAct->dx % 3 == 0){
				pAct->spd--;
				// ??????????
				if (pAct->spd <= 0){
					pAct->actNo = ANIM_DISP_PET_MAIN;
					// ???????
					if (pActMailItem != NULL){
						pActMailItem->dispPrio = DISP_PRIO_BOX3;
					}
				}
			}
		}
		// ?????????
		if (pAct->dx % 8 == 0){
			// ????????
			if (pAct->dy < 12) pAct->dy++;
		}
		// ???
		gemini(pAct);

		// ???????
		if (pActMailItem != NULL){
			// ??????????
			if (pAct->x >= pActMailItem->x){
				pActMailItem->x = pAct->x;
			}
		}
		// ???????
		pattern(pAct, pAct->dy, ANM_LOOP);

		break;

	case ANIM_DISP_PET_MAIN:	// ???????

		// ?????
		if (Rnd(0, 4) >= 2){
			pAct->actNo = ANIM_DISP_PET_WALK;
			// ?????
			pAct->anim_ang = Rnd(0, 7);
			// ???
			pAct->crs = crs_change_tbl2[pAct->anim_ang];
			// ?????????
			pAct->spd = Rnd(1, 12);
			// ???????????
			pAct->dy = 13 - pAct->spd;
		}
		else pAct->actNo = Rnd(ANIM_DISP_PET_STAND, ANIM_DISP_PET_DEAD);

		// ?????????????
		pAct->anim_no_bak = -1;

		break;

	case ANIM_DISP_PET_WALK:	// ?

		// ??
		pAct->anim_no = ANIM_WALK;

		// ???
		gemini(pAct);

		// ???????
		if (pattern(pAct, pAct->dy, ANM_NO_LOOP)){
			// ???????
			if (Rnd(0, 3) == 0){
				pAct->actNo = ANIM_DISP_PET_MAIN;
			}
			else{
				// ?????????????
				pAct->anim_no_bak = -1;
			}
		}
		if (pActPet == NULL || pActMenuWnd == NULL){
			return;
		}
		// ????????
		// 
		if (pActPet->y < pActMenuWnd->y + 64){
			pActPet->y = pActMenuWnd->y + 64;
			// ???
			pAct->crs = crs_bound_tbl[0][pAct->crs];
			// ??
			pAct->anim_ang = crs_change_tbl[pAct->crs];
		}
		// ?
		if (pActPet->y > pActMenuWnd->y + 424 - 32){
			pActPet->y = pActMenuWnd->y + 424 - 32;
			// ???
			pAct->crs = crs_bound_tbl[2][pAct->crs];
			// ??
			pAct->anim_ang = crs_change_tbl[pAct->crs];
		}
		// ?
		if (pActPet->x < pActMenuWnd->x + 32){
			pActPet->x = pActMenuWnd->x + 32;
			// ???
			pAct->crs = crs_bound_tbl[3][pAct->crs];
			// ??
			pAct->anim_ang = crs_change_tbl[pAct->crs];
		}
		// ?
		if (pActPet->x > pActMenuWnd->x + 272 - 32){
			pActPet->x = pActMenuWnd->x + 272 - 32;
			// ???
			pAct->crs = crs_bound_tbl[1][pAct->crs];
			// ??
			pAct->anim_ang = crs_change_tbl[pAct->crs];
		}
		break;

	case ANIM_DISP_PET_STAND:	// ?

		// ??
		pAct->anim_no = ANIM_STAND;

		// ???????
		if (pattern(pAct, ANM_NOMAL_SPD, ANM_NO_LOOP)){
			// ???????
			pAct->actNo = ANIM_DISP_PET_MAIN;
		}

		break;

	case ANIM_DISP_PET_ATTACK:	// ??

		// ??
		pAct->anim_no = ANIM_ATTACK;

		// ???????
		if (pattern(pAct, ANM_NOMAL_SPD, ANM_NO_LOOP)){
			// ???????
			if (Rnd(0, 1) == 0){
				pAct->actNo = ANIM_DISP_PET_MAIN;
			}
			else{
				// ?????????????
				pAct->anim_no_bak = -1;
			}
		}

		break;

	case ANIM_DISP_PET_GUARD:	// ???

		// ??
		pAct->anim_no = ANIM_GUARD;

		// ???????
		if (pattern(pAct, ANM_NOMAL_SPD, ANM_NO_LOOP)){
			// ???????
			if (Rnd(0, 1) == 0){
				pAct->actNo = ANIM_DISP_PET_MAIN;
			}
		}

		break;

	case ANIM_DISP_PET_DAMAGE:	// ????

		// ??
		pAct->anim_no = ANIM_DAMAGE;

		// ???????
		if (pattern(pAct, ANM_NOMAL_SPD, ANM_NO_LOOP)){
			// ???????
			if (Rnd(0, 1) == 0){
				pAct->actNo = ANIM_DISP_PET_MAIN;
			}
			else{
				// ?????????????
				pAct->anim_no_bak = -1;
			}
		}

		break;

	case ANIM_DISP_PET_DEAD:	// ?

		// ??
		pAct->anim_no = ANIM_DEAD;

		// ???????
		if (pattern(pAct, ANM_NOMAL_SPD, ANM_NO_LOOP)){
			// ???????
			if (Rnd(0, 2) == 0){
				pAct->actNo = ANIM_DISP_PET_MAIN;
			}
		}

		break;

	case ANIM_DISP_PET_ITEM:	// ????????

		// ???????
		pattern(pAct, ANM_NOMAL_SPD, ANM_LOOP);

		break;

	case ANIM_DISP_LETTER_MOVE:	// ??

		// ?
		if (pAct->x < 245) pAct->x += 5;
		// ?????????
		else{
			pAct->actNo = ANIM_DISP_LETTER;
			// ???????
			pattern(pAct, ANM_NOMAL_SPD, ANM_LOOP);
			pAct->anim_cnt += 8;
		}
		break;

	case ANIM_DISP_LETTER:		// ????????

		// ???????
		pattern(pAct, ANM_NOMAL_SPD, ANM_LOOP);

		break;

	case ANIM_DISP_MIX_PET_INIT:	// ???????

		// ?????????
		pAct->spd = 11;//Rnd( 6, 12 );
		// ???????????
		pAct->dy = 14 - pAct->spd;

		// ??
		pAct->anim_no = ANIM_WALK;

		// ?????????
		for (i = MAX_ITEMSTART; i < MAX_ITEM; i++){
			if (ItemBuffer[i].mixFlag >= 1){
				pAct->dx = i;
				break;
			}
		}
		// ??


			pAct->actNo = ANIM_DISP_MIX_PET_MOVE;
		break;

	case ANIM_DISP_MIX_PET_MOVE:	// ?????????

		// ?佋
		x = ItemBuffer[pAct->dx].defX;
		y = ItemBuffer[pAct->dx].defY;

		// ????
		radar(pAct, &x, &y);

		// ??
		pAct->anim_no = ANIM_WALK;

		// ?????????
		if (y >= pAct->spd / 2){
			// ???
			pAct->crs = x;
			// ????佋????????
			if (pAct->level == FALSE){
				pAct->anim_ang = crs_change_tbl[pAct->crs];
				// ??????????
				pAct->level = TRUE;
			}

			// ???
			gemini(pAct);
			// ???????
			pattern(pAct, pAct->dy, ANM_LOOP);
		}
		// ?????
		else{
			// ???????????
			pAct->level = FALSE;
			// ??????
			play_se(204, 320, 240);

			// ??????????????
			if (pAct->delta == 1){
				// ???????
				pAct->actNo = ANIM_DISP_MIX_PET_MOJI;
				// ??
				pAct->anim_no = ANIM_STAND;

				// ?????????
				pAct->spd = 10;//Rnd( 6, 12 );
				// ???????????
				pAct->dy = 14 - pAct->spd;

				// ??????
				for (i = MAX_ITEMSTART; i < MAX_ITEM; i++){
					if (ItemBuffer[i].mixFlag >= 1){
						ItemBuffer[i].mixFlag = 4;
					}
				}

				// ????????????
				pAct->delta = Rnd(2, 15);
				pAct->mp = 15;
				break;
			}

			// ????????
			pAct->spd = (int)((double)pAct->spd * 0.79);
			// ????????
			if (pAct->spd < 2) pAct->spd = 2;
			// ???????????
			pAct->dy = 13 - pAct->spd;

			// ??????????????
#ifdef _ITEM_UPINSLAY
			if( ItemBuffer[ pAct->dx ].mixFlag != 12 
				&& ItemBuffer[ pAct->dx ].mixFlag != 13 ) 
#endif
				ItemBuffer[pAct->dx].mixFlag = 3;

			// ????????
			for (i = pAct->dx + 1; i < MAX_ITEM; i++){
				if (ItemBuffer[i].mixFlag >= 1){
					pAct->dx = i;
					break;
				}
			}

			// ????????
			if (i >= MAX_ITEM){
				// ????????????
				for (i = MAX_ITEMSTART; i < MAX_ITEM; i++){
					// ???????????
					if (pc.item[i].useFlag == FALSE){
						// ????
						pAct->dx = i;
						break;
					}
					else
						// ????????????
					if (ItemBuffer[i].mixFlag >= 1){
						// ????
						pAct->dx = i;
						break;
					}
				}
				// ????????????
				pAct->delta = 1;
			}
		}
		break;

	case ANIM_DISP_MIX_PET_MOJI:	// ?????????????

		// ???????
		pattern(pAct, 1, ANM_LOOP);
		// ??????
		if (pAct->maxHp >= pAct->mp) pAct->maxHp = 0;
		else{
			pAct->maxHp++;
			break;
		}
		// ??????
		switch (pAct->hp){

			// ???
		case 0:
		case 3:
			// ??
			pAct->anim_ang++;
			// ????????
			if (pAct->anim_ang >= 8) pAct->anim_ang = 0;
			// ?????
			if (pAct->hp == 0){
				// ??????
				play_se(216, 320, 240);
			}
			// ???????
			if (pAct->hp == 3){
				pAct->dir++;
				pAct->hp = 0;
				// ??????
				pAct->mp--;
				if (pAct->mp < 4) pAct->mp = 4;
			}
			else pAct->hp++;
			break;

			// ???
		case 1:
		case 2:
			// ??
			pAct->anim_ang--;
			// ????????
			if (pAct->anim_ang < 0) pAct->anim_ang = 7;
			pAct->hp++;
			break;
		}

		// ???????
		if (pAct->dir >= pAct->delta){
			// ???
			if (pAct->dir == pAct->delta){
				pAct->dir++;
				// ????
				char moji[256];
				// ????
				moji[0] = NULL;
				// ????????????
				for (i = MAX_ITEMSTART; i < MAX_ITEM; i++){
					// ??????????
					if (ItemBuffer[i].mixFlag >= TRUE){//ttom
						char work[256];
						// ???
						sprintf_s(work, "%d|", i);
						strcat_s(moji, work);
						// ??????
					}//ttom
				}
				// ???|????????
				moji[strlen(moji) - 1] = NULL;

				// ??????????????????
				if (bNewServer)
					lssproto_PS_send(sockfd, mixPetNo, SelectWazaNo, 0, moji);
				else
					old_lssproto_PS_send(sockfd, mixPetNo, SelectWazaNo, 0, moji);
				// ??????
				ItemMixRecvFlag = TRUE;
			}

			// ??????
			if (ItemMixRecvFlag == FALSE){
				// ??????
				play_se(115, 320, 240);
				// ????????
				for (i = MAX_ITEMSTART; i < MAX_ITEM; i++){
					ItemBuffer[i].mixFlag = FALSE;
				}
				// ?????????
				pAct->spd = 11;
				// ???????????
				pAct->dy = 14 - pAct->spd;
				// ???
				pAct->actNo = ANIM_DISP_MIX_PET_END;
			}
		}

		break;

	case ANIM_DISP_MIX_PET_END:	// ???????

		// ?佋
		x = 750;
		y = 240;

		// ????
		radar(pAct, &x, &y);

		// ??
		pAct->anim_no = ANIM_WALK;

		// ?????????
		if (y >= pAct->spd / 2){
			// ???
			pAct->crs = x;

			// ????佋????????
			if (pAct->level == FALSE){
				pAct->anim_ang = crs_change_tbl[pAct->crs];
				// ??????????
				pAct->level = TRUE;
			}

			// ???
			gemini(pAct);
			// ???????
			pattern(pAct, pAct->dy, ANM_LOOP);
		}
		// ?????
		else{
			// ??????
			DeathAction(pAct);
			pActPet2 = NULL;
			// ????????
			ItemMixPetNo = -1;
		}
		break;
#ifdef _AniCrossFrame	  // Syu ADD 动画层游过画面生物
	case ANIM_DISP_CROSSFRAME:
	{
								 extern bool delFlag;
								 extern bool flag22;
								 pAct->anim_no = ANIM_WALK;
								 pAct->anim_ang = 2;
								 pAct->crs = crs_change_tbl2[pAct->anim_ang];
								 //移动速度
								 pAct->spd = 1;
								 //显示层级
								 pAct->dispPrio = DISP_PRIO_JIKI;
								 if (pAct->dx % 5 == 0){
									 if (pAct->dy >= 2) pAct->dy--;
								 }
								 gemini(pAct);
								 //各种方向位移
								 if (((pAct->anim_chr_no > 101511) && (pAct->anim_chr_no < 101516)) ||
									 (pAct->anim_chr_no == 101517) || (pAct->anim_chr_no == 101519))
								 {
									 pAct->x--;
									 if (pAct->y == 0)
										 UpDownflag = 4;
									 if (UpDownflag == 0)
										 UpDownflag = RAND(1, 5);
									 if (UpDownflag > 3)
										 pAct->y++;
									 else
										 pAct->y = pAct->y;
								 }
								 else if ((pAct->anim_chr_no == 101516) || (pAct->anim_chr_no == 101518))
								 {
									 pAct->x++;
									 if (pAct->y == 0)
										 UpDownflag = 4;
									 if (UpDownflag == 0)
										 UpDownflag = RAND(1, 5);
									 if (UpDownflag > 3)
										 pAct->y++;
									 else
										 pAct->y = pAct->y;
								 }
								 else if ((pAct->anim_chr_no == 101520) || (pAct->anim_chr_no == 101522) ||
									 (pAct->anim_chr_no <= 101593 && pAct->anim_chr_no >= 101588)) //右上
								 {
									 pAct->x++;
									 pAct->y--;
								 }
								 else if ((pAct->anim_chr_no == 101521) || (pAct->anim_chr_no == 101523) ||
									 (pAct->anim_chr_no <= 101587 && pAct->anim_chr_no >= 101582)) //左上
								 {
									 pAct->x--;
									 pAct->y--;
								 }
								 else
								 {
									 pAct->x++;
									 pAct->y++;
								 }
								 //超出荧幕范围消灭
								 if ((pAct->x <= -100 || pAct->y <= -100 || pAct->x >= 740 || pAct->y >= 580) && delFlag == true && flag22 == true)
								 {
									 flag22 = false;
									 UpDownflag = 0;
								 }
								 pattern(pAct, pAct->dy, ANM_LOOP);
	}
		break;
#endif
#ifdef _THEATER
	case ANIM_DISP_THEATER_NPC:		// 剧场NPC显示
	{
										float	fX, fY;

										camMapToGamen((float)pAct->gx * GRID_SIZE, (float)pAct->gy * GRID_SIZE, &fX, &fY);
										ATR_H_POS(pAct) = (int)fX;
										ATR_V_POS(pAct) = (int)fY;
										setCharPrio(pAct->bmpNo, pAct->x, pAct->y, 0, 0, pAct->mx, pAct->my
#ifdef _SFUMATO
											, pAct->sfumato
#endif
											);
										switch (ATR_CHR_ACT(pAct))
										{
										case 0:		// 攻击
										case 1:		// 受伤
										case 2:		// 晕倒
										case 5:		// 坐下
										case 10:	// 防御
										case 12:	// 投掷
											pattern(pAct, ANM_NOMAL_SPD, ANM_NO_LOOP);	// 不重覆动作
											break;
										default:
											pattern(pAct, ANM_NOMAL_SPD, ANM_LOOP);
											break;
										}
	}
		break;
#endif
	}

	// ??????????
	if (ANIM_DISP_PET_MAIN <= pAct->actNo && pAct->actNo <= ANIM_DISP_PET_DEAD){
		// ????????????
		if (pAct->hitDispNo == HitDispNo &&
			mouse.autoState & MOUSE_LEFT_CRICK){
			// ????
			pAct->actNo = ANIM_DISP_PET_WALK;
			// ?????
			pAct->anim_ang = Rnd(0, 7);
			// ???
			pAct->crs = crs_change_tbl2[pAct->anim_ang];
			// ?????????
			pAct->spd = 15;

			pAct->dy = 2;
			play_se(217, 320, 240); // ?????
		}
	}
}

// ??????????? ***********************************************/
ACTION *MakeAnimDisp(int x, int y, int sprNo, int mode)
{
	ACTION *pAct;

	// ?????????
	pAct = GetAction(PRIO_JIKI, NULL);
	if (pAct == NULL) return NULL;

	// ???
	pAct->func = AnimDisp;
	// ?????
	pAct->atr |= ACT_ATR_HIT;
	pAct->anim_chr_no = sprNo;
	// ??
	pAct->anim_no = ANIM_STAND;
	// ?????????( ??? )( ??????? )
	pAct->anim_ang = 1;
	/* ? */
	pAct->dispPrio = DISP_PRIO_BOX3;
	/* ??? */
	pAct->x = x;
	pAct->y = y;
	// ???
	pAct->actNo = mode;
	// ??????????
	if (pAct->actNo == ANIM_DISP_PET){
		// ???????????
		pAct->dy = 12;
	}
	else
	if (pAct->actNo == ANIM_DISP_PET_RECIEVE){
		pAct->dy = 1;
		pAct->spd = 30;
	}
	// ???????
	pattern(pAct, ANM_NOMAL_SPD, ANM_LOOP);

	return pAct;
}

// ??????? ****************************************************************/
BOOL SaveMailHistory(int no)
{
	FILE *fp;

	// ????????????
	if ((fp = fopen(MAIL_HISTORY_FILE_NAME, "r+b")) == NULL){
		return FALSE;
	}
#ifdef _MORECHARACTERS_
	// ?????????????????????????
	extern int 多人物当前页数;
	fseek(fp, sizeof(MAIL_HISTORY)* MAX_ADR_BOOK * (selectPcNo + 多人物当前页数 * 2), SEEK_SET);
#else
	fseek(fp, sizeof(MAIL_HISTORY)* MAX_ADR_BOOK * (selectPcNo), SEEK_SET);
#endif
	// ???????????????????????
	fseek(fp, sizeof(MAIL_HISTORY)* no, SEEK_CUR);

	// ???????
	if (fwrite(&MailHistory[no], sizeof(MAIL_HISTORY), 1, fp) < 1){

		fclose(fp);// ????????
		return FALSE;
	}

	fclose(fp);

	return TRUE;
}

// ?????????? ****************************************************************/
BOOL LoadMailHistory(void)
{
	FILE *fp;
	MAIL_HISTORY work[MAX_ADR_BOOK * 2];	// ?????????

	// ???????????
	if ((fp = fopen(MAIL_HISTORY_FILE_NAME, "rb")) == NULL){
		// ???????
		if ((fp = fopen(MAIL_HISTORY_FILE_NAME, "wb")) != NULL){
			// ???????
			fwrite(MailHistory, sizeof(MAIL_HISTORY), MAX_ADR_BOOK, fp);
			fwrite(MailHistory, sizeof(MAIL_HISTORY), MAX_ADR_BOOK, fp);
			fclose(fp);	// ????????
		}

		return FALSE;
	}

	// ????????????
	if (fread(work, sizeof(MAIL_HISTORY), MAX_ADR_BOOK * 2, fp) < MAX_ADR_BOOK * 2){
		// ???????
		// ???????
		if ((fp = fopen(MAIL_HISTORY_FILE_NAME, "wb")) != NULL){
#ifdef _STONDEBUG_		
			MessageBoxNew( hWnd, "删除以前的mail资料。", "确定", MB_OK | MB_ICONSTOP );
#endif
			// ???????
			fwrite(MailHistory, sizeof(MAIL_HISTORY), MAX_ADR_BOOK, fp);
			fwrite(MailHistory, sizeof(MAIL_HISTORY), MAX_ADR_BOOK, fp);
			fclose(fp);	// ????????
		}

		return FALSE;
	}

#ifdef _MORECHARACTERS_
	// ??????????????????
	extern int 多人物当前页数;
	fseek(fp, sizeof(MAIL_HISTORY)* MAX_ADR_BOOK * (selectPcNo + 多人物当前页数 * 2), SEEK_SET);
#else
	fseek(fp, sizeof(MAIL_HISTORY)* MAX_ADR_BOOK * (selectPcNo), SEEK_SET);
#endif
	// ??????
	if (fread(MailHistory, sizeof(MAIL_HISTORY), MAX_ADR_BOOK, fp) < MAX_ADR_BOOK){

		fclose(fp);	// ????????
		return FALSE;
	}
	// ????????
	fclose(fp);

	return TRUE;
}

// ???????? ****************************************************************/
BOOL SaveChatRegistyStr(void)
{
	FILE *fp;

	// ????????????
	if ((fp = fopen(CAHT_REGISTY_STR_FILE_NAME, "r+b")) == NULL){
#ifdef _STONDEBUG_		
		MessageBoxNew( hWnd, "记录聊天的登录文字失败！１", "确定", MB_OK | MB_ICONSTOP );
#endif
		return FALSE;
	}

	// ???????
	if (fwrite(chatRegistryStr, sizeof(STR_BUFFER), MAX_CHAT_REGISTY_STR, fp) < MAX_CHAT_REGISTY_STR){

#ifdef _STONDEBUG_		
		MessageBoxNew( hWnd, "记录聊天的登录文字失败！２", "确定", MB_OK | MB_ICONSTOP );
#endif
		fclose(fp);// ????????
		return FALSE;
	}
#ifdef _TALK_WINDOW
	fwrite(&g_bTalkWindow,sizeof(BOOL),1,fp);
#endif
	// ????????
	fclose(fp);

	return TRUE;
}

// ??????????? ****************************************************************/
BOOL LoadChatRegistyStr(void)
{
	FILE *fp;
	int i;

	// ???????????
	if ((fp = fopen(CAHT_REGISTY_STR_FILE_NAME, "rb")) == NULL){
		// ???????
		if ((fp = fopen(CAHT_REGISTY_STR_FILE_NAME, "wb")) != NULL){
#ifdef _STONDEBUG_		
			MessageBoxNew( hWnd, "建立聊天的登录文字档案！", "确定", MB_OK );
#endif
			// ??????????????????
			for (i = 0; i < 8; i++){
				// ??? ??
				chatRegistryStr[i].buffer[0] = NULL;
				chatRegistryStr[i].cnt = 0;
				chatRegistryStr[i].cursor = 0;
				// ????
				chatRegistryStr[i].len = MAX_CHAT_REGISTY_STR_LEN;
				// ????
				chatRegistryStr[i].lineLen = 0;
				// ?????
				chatRegistryStr[i].lineDist = 20;
				// ???
				chatRegistryStr[i].color = 0;
				// ????
				// ??????
				chatRegistryStr[i].fontPrio = FONT_PRIO_FRONT;
			}
			// ???????
			fwrite(chatRegistryStr, sizeof(STR_BUFFER), MAX_CHAT_REGISTY_STR, fp);
#ifdef _TALK_WINDOW
			fwrite(&g_bTalkWindow,sizeof(BOOL),1,fp);
#endif
			fclose(fp);	// ????????
		}
		return FALSE;
	}

	// ??????
	if (fread(chatRegistryStr, sizeof(STR_BUFFER), MAX_CHAT_REGISTY_STR, fp) < MAX_CHAT_REGISTY_STR){

#ifdef _STONDEBUG_		
		MessageBoxNew( hWnd, "载入聊天的登录文字资料失败！１", "确定", MB_OK | MB_ICONSTOP );
#endif
		fclose(fp);	// ????????
		return FALSE;
	}
#ifdef _TALK_WINDOW
	fread(&g_bTalkWindow,sizeof(BOOL),1,fp);
	if(g_bTalkWindow) TalkWindow.Create();
#endif
	// ????????
	fclose(fp);

	return TRUE;
}
#ifdef __ALBUM_47
int MAX_PET_TBL;
#endif
// ????? ****************************************************************/
BOOL SaveAlbum(int no)
{
	FILE	*fp;

#if defined(__ALBUM_47)
	BOOL	ret = FALSE;
	if (0 <= no && no < MAX_PET_TBL){
		if (fp = fopen(ALBUM_FILE_NAME_48, "r+b")){
			if (0 != fseek(fp, (16 + sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				MessageBoxNew(hWnd, "储存相簿资料失败！１", "确定", MB_OK);
			if (0 != fseek(fp, 16 + sizeof(PET_ALBUM)*no, SEEK_CUR))
				MessageBoxNew(hWnd, "储存相簿资料失败！２", "确定", MB_OK);
			if (fwrite(&PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = TRUE;
			fclose(fp);
		}
		else
			MessageBoxNew(hWnd, "储存相簿资料失败！３", "确定", MB_OK);
	}
	return ret;
#elif defined(__ALBUM_46)
	BOOL	ret = FALSE;
	if( 0 <= no && no < MAX_PET_TBL ){
		if(!fopen_s(&fp, ALBUM_FILE_NAME_47, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				MessageBoxNew( hWnd, "储存相簿资料失败！１", "确定", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				MessageBoxNew( hWnd, "储存相簿资料失败！２", "确定", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = TRUE;
			fclose( fp );
		}else
			MessageBoxNew( hWnd, "储存相簿资料失败！３", "确定", MB_OK );
	}
	return ret;

#elif defined(__ALBUM_45)
	BOOL	ret = FALSE;
	if( 0 <= no && no < MAX_PET_TBL ){
		if(!fopen_s(&fp, ALBUM_FILE_NAME_46, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				MessageBoxNew( hWnd, "储存相簿资料失败！１", "确定", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				MessageBoxNew( hWnd, "储存相簿资料失败！２", "确定", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = TRUE;
			fclose( fp );
		}else
			MessageBoxNew( hWnd, "储存相簿资料失败！３", "确定", MB_OK );
	}
	return ret;

#elif defined(__ALBUM_44)
	BOOL	ret = FALSE;
	if( 0 <= no && no < MAX_PET_TBL ){
		if( !fopen_s( &fp,ALBUM_FILE_NAME_45, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				MessageBoxNew( hWnd, "储存相簿资料失败！１", "确定", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				MessageBoxNew( hWnd, "储存相簿资料失败！２", "确定", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = TRUE;
			fclose( fp );
		}else
			MessageBoxNew( hWnd, "储存相簿资料失败！３", "确定", MB_OK );
	}
	return ret;

#elif defined(__ALBUM_43)
	BOOL	ret = FALSE;
	if( 0 <= no && no < MAX_PET_TBL ){
		if(!fopen_s(&fp, ALBUM_FILE_NAME_44, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				MessageBoxNew( hWnd, "储存相簿资料失败！１", "确定", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				MessageBoxNew( hWnd, "储存相簿资料失败！２", "确定", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = TRUE;
			fclose( fp );
		}else
			MessageBoxNew( hWnd, "储存相簿资料失败！３", "确定", MB_OK );
	}
	return ret;

#elif defined(__ALBUM_42)
	BOOL	ret = FALSE;
	if( 0 <= no && no < MAX_PET_TBL ){
		if(!fopen_s( &fp,ALBUM_FILE_NAME_43, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				MessageBoxNew( hWnd, "储存相簿资料失败！１", "确定", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				MessageBoxNew( hWnd, "储存相簿资料失败！２", "确定", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = TRUE;
			fclose( fp );
		}else
			MessageBoxNew( hWnd, "储存相簿资料失败！３", "确定", MB_OK );
	}
	return ret;

#elif defined(__ALBUM_41)
	BOOL	ret = FALSE;
	if( 0 <= no && no < MAX_PET_TBL ){
		if(!fopen_s( &fp,ALBUM_FILE_NAME_42, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				MessageBoxNew( hWnd, "储存相簿资料失败！１", "确定", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				MessageBoxNew( hWnd, "储存相簿资料失败！２", "确定", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = TRUE;
			fclose( fp );
		}else
			MessageBoxNew( hWnd, "储存相簿资料失败！３", "确定", MB_OK );
	}
	return ret;

#elif defined(__ALBUM_40)
	BOOL	ret = FALSE;
	if( 0 <= no && no < MAX_PET_TBL ){
		if(!fopen_s(&fp, ALBUM_FILE_NAME_41, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				MessageBoxNew( hWnd, "储存相簿资料失败！１", "确定", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				MessageBoxNew( hWnd, "储存相簿资料失败！２", "确定", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = TRUE;
			fclose( fp );
		}else
			MessageBoxNew( hWnd, "储存相簿资料失败！３", "确定", MB_OK );
	}
	return ret;

#elif defined(__ALBUM_39)
	BOOL	ret = FALSE;
	if( 0 <= no && no < MAX_PET_TBL ){
		if( !fopen_s( &fp,ALBUM_FILE_NAME_40, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				MessageBoxNew( hWnd, "储存相簿资料失败！１", "确定", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				MessageBoxNew( hWnd, "储存相簿资料失败！２", "确定", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = TRUE;
			fclose( fp );
		}else
			MessageBoxNew( hWnd, "储存相簿资料失败！３", "确定", MB_OK );
	}
	return ret;

#elif defined(__ALBUM_38)
	BOOL	ret = FALSE;
	if( 0 <= no && no < MAX_PET_TBL ){
		if(!fopen_s( &fp,ALBUM_FILE_NAME_39, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				MessageBoxNew( hWnd, "储存相簿资料失败！１", "确定", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				MessageBoxNew( hWnd, "储存相簿资料失败！２", "确定", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = TRUE;
			fclose( fp );
		}else
			MessageBoxNew( hWnd, "储存相簿资料失败！３", "确定", MB_OK );
	}
	return ret;

#elif defined(__ALBUM_37)
	BOOL	ret = FALSE;
	if( 0 <= no && no < MAX_PET_TBL ){
		if(!fopen_s( &fp,ALBUM_FILE_NAME_38, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				MessageBoxNew( hWnd, "储存相簿资料失败！１", "确定", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				MessageBoxNew( hWnd, "储存相簿资料失败！２", "确定", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = TRUE;
			fclose( fp );
		}else
			MessageBoxNew( hWnd, "储存相簿资料失败！３", "确定", MB_OK );
	}
	return ret;

#elif defined(__ALBUM_36)
	BOOL	ret = FALSE;
	if( 0 <= no && no < MAX_PET_TBL ){
		if(!fopen_s( &fp,ALBUM_FILE_NAME_37, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				MessageBoxNew( hWnd, "储存相簿资料失败！１", "确定", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				MessageBoxNew( hWnd, "储存相簿资料失败！２", "确定", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = TRUE;
			fclose( fp );
		}else
			MessageBoxNew( hWnd, "储存相簿资料失败！３", "确定", MB_OK );
	}
	return ret;

#elif defined(__ALBUM_35)
	BOOL	ret = FALSE;
	if( 0 <= no && no < MAX_PET_TBL ){
		if( !fopen_s( &fp,ALBUM_FILE_NAME_36, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				MessageBoxNew( hWnd, "储存相簿资料失败！１", "确定", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				MessageBoxNew( hWnd, "储存相簿资料失败！２", "确定", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = TRUE;
			fclose( fp );
		}else
			MessageBoxNew( hWnd, "储存相簿资料失败！３", "确定", MB_OK );
	}
	return ret;
#elif defined(__ALBUM_34)
	BOOL	ret = FALSE;
	if( 0 <= no && no < MAX_PET_TBL ){
		if(!fopen_s(&fp, ALBUM_FILE_NAME_35, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				MessageBoxNew( hWnd, "储存相簿资料失败！１", "确定", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				MessageBoxNew( hWnd, "储存相簿资料失败！２", "确定", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = TRUE;
			fclose( fp );
		}else
			MessageBoxNew( hWnd, "储存相簿资料失败！３", "确定", MB_OK );
	}
	return ret;
#elif defined(__ALBUM_33)
	BOOL	ret = FALSE;
	if( 0 <= no && no < MAX_PET_TBL ){
		if( !fopen_s(&fp, ALBUM_FILE_NAME_34, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				MessageBoxNew( hWnd, "储存相簿资料失败！１", "确定", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				MessageBoxNew( hWnd, "储存相簿资料失败！２", "确定", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = TRUE;
			fclose( fp );
		}else
			MessageBoxNew( hWnd, "储存相簿资料失败！３", "确定", MB_OK );
	}
	return ret;
#elif defined(__ALBUM_32)
	BOOL	ret = FALSE;
	if( 0 <= no && no < MAX_PET_TBL ){
		if(!fopen_s(&fp, ALBUM_FILE_NAME_33, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				MessageBoxNew( hWnd, "储存相簿资料失败！１", "确定", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				MessageBoxNew( hWnd, "储存相簿资料失败！２", "确定", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = TRUE;
			fclose( fp );
		}else
			MessageBoxNew( hWnd, "储存相簿资料失败！３", "确定", MB_OK );
	}
	return ret;
#else
	// ????????????
	if ((fopen_s(&fp, ALBUM_FILE_NAME, "r+b")) != NULL){
#ifdef _STONDEBUG_		
		MessageBoxNew( hWnd, "储存相簿资料失败！１", "确定", MB_OK );
#endif
		return FALSE;
	}
	// ???????
	for (int i = 0; i < AlbumIdCnt; i++){
		// ????????????
		fseek(fp, 16, SEEK_CUR);
		fseek(fp, sizeof(PET_ALBUM)* MAX_PET_KIND, SEEK_CUR);
	}
	// ???????
	fseek(fp, 16, SEEK_CUR);
	// ???????????????
	fseek(fp, sizeof(PET_ALBUM)* no, SEEK_CUR);

	// ???????
	if (fwrite(&PetAlbum[no], sizeof(PET_ALBUM), 1, fp) < 1){

#ifdef _STONDEBUG_		
		MessageBoxNew( hWnd, "储存相簿资料失败！２", "确定", MB_OK );
#endif
		fclose(fp);// ????????
		return FALSE;
	}
	// ????????
	fclose(fp);

	return TRUE;
#endif
}

#if defined(__ALBUM_4)

BOOL LoadAlbum_4( char *user)
{
	FILE *fp;
	BOOL ret = FALSE;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "data\\album_2.dat");
	DeleteFile( "data\\album_3.dat");
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fopen_s( &fp,ALBUM_FILE_NAME_4, "r+b")) != NULL){
		if( (fopen_s(&fp, ALBUM_FILE_NAME_4, "wb")) == NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			// ??????????
			// ????????????
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return FALSE;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return FALSE;
			}
		}else{
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return FALSE;
			}
			//
			fclose( fp);
			return TRUE;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

BOOL ConvertAlbum1_4(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fopen_s( &fp,ALBUM_FILE_NAME, "r+b" ) ) != NULL )
		return FALSE;
	while( 1 ){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//找下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//读入资料
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return FALSE;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}							
				}
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}

#elif defined(__ALBUM_47)
BOOL LoadAlbum_47(char *user)
{
	FILE *fp;
	BOOL ret = FALSE;
	char id[16];
	char id2[16];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy(id, user, 16);
	DeleteFile("data\\album_2.dat");
	DeleteFile("data\\album_3.dat");
	DeleteFile("data\\album_10.dat");
	for (i = 0; i < MAX_PET_KIND; i++){
		for (int j = 0; j < PET_NAME_LEN + 1; j++){
			PetAlbum[i].name[j] = Rnd(1, 255);
			PetAlbum[i].freeName[j] = Rnd(1, 255);
		}
		PetAlbum[i].flag = 0;
		PetAlbum[i].faceGraNo = Rnd(1, 60000);
		PetAlbum[i].level = Rnd(1, 30000);
		PetAlbum[i].faceGraNo = Rnd(1, 30000);
		PetAlbum[i].maxHp = Rnd(1, 30000);
		PetAlbum[i].str = Rnd(1, 30000);
		PetAlbum[i].quick = Rnd(1, 30000);
		PetAlbum[i].def = Rnd(1, 30000);
		PetAlbum[i].earth = Rnd(1, 30000);
		PetAlbum[i].water = Rnd(1, 30000);
		PetAlbum[i].fire = Rnd(1, 30000);
		PetAlbum[i].wind = Rnd(1, 30000);
	}
	if ((fp = fopen(ALBUM_FILE_NAME_48, "r+b")) == NULL){
		if ((fp = fopen(ALBUM_FILE_NAME_48, "wb")) != NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd(1, 100);
			for (i = 0; i < 16; i++)
				id[i] ^= IdEncryptionTbl[i];
			if (fwrite(id, sizeof(id), 1, fp) >= 1){
				fwrite(PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose(fp);
		}
		return ret;
	}
	while (1){
		if (fread(id2, sizeof(id2), 1, fp) < 1){
			//找不到此帐号的记录
			id[15] &= 0xf;
			id[id[15]] = Rnd(1, 100);
			for (i = 0; i < 16; i++)
				id[i] ^= IdEncryptionTbl[i];
			if (fwrite(id, sizeof(id), 1, fp) >= 1){
				fwrite(PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose(fp);
			return FALSE;
		}
		for (i = 0; i < 16; i++)
			id2[i] ^= IdEncryptionTbl[i];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if (strcmp(id, id2) != 0){
			//不是此帐号的记录
			if (fseek(fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek(fp, (sizeof(PET_ALBUM)*MAX_PET_KIND + 16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd(1, 100);
				for (i = 0; i < 16; i++)
					id[i] ^= IdEncryptionTbl[i];
				if (fwrite(id, sizeof(id), 1, fp) >= 1){
					fwrite(PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose(fp);
				return FALSE;
			}
		}
		else{
			//找到此帐号的记录
			if (fread(PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite(PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose(fp);
				return FALSE;
			}
			fclose(fp);
			return TRUE;
		}
		AlbumIdCnt++;
	}
	fclose(fp);
	return ret;
}

BOOL ConvertAlbumTo47(char *user, char *oldfile, DWORD num)
{
	PET_ALBUM OldAlbum[MAX_PET_KIND];
	char id2[32];
	int i;
	FILE *fp;
	if ((fp = fopen(oldfile, "r+b")) == NULL)
		return FALSE;
	while (1){
		//读入ID
		if (fread(id2, 16, 1, fp) < 1){
			fclose(fp);
			return FALSE;
		}
		//ID解码
		for (i = 0; i < 16; i++){
			id2[i] ^= IdEncryptionTbl[i];
		}
		id2[id2[15]] = NULL;
		if (strcmp(user, id2) != 0){
			//ID不符
			if (fseek(fp, sizeof(PET_ALBUM)* num, SEEK_CUR) != 0){//下一个
				fclose(fp);
				return FALSE;
			}
		}
		else{
			//找到ID纪录
			if (fread(OldAlbum, sizeof(PET_ALBUM), num, fp) < num){
				fclose(fp);
				return FALSE;
			}
			for (DWORD i = 0; i < num; i++){
				if (440 <= i && i <= 461) continue;		//修正乌力Q 问题
				CopyMemory(&PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum(i);
			}
			break;
		}
	}
	fclose(fp);
	return TRUE;
}

BOOL ConvertAlbum1_47(char *user)
{
	PET_ALBUM OldAlbum[MAX_PET_KIND_1];
	char id2[16];
	int i;
	FILE *fp;
	if ((fp = fopen(ALBUM_FILE_NAME, "r+b")) == NULL)
		return FALSE;
	while (1){
		//读入ID
		if (fread(id2, sizeof(id2), 1, fp) < 1){
			fclose(fp);
			return FALSE;
		}
		//ID解码
		for (i = 0; i < 16; i++){
			id2[i] ^= IdEncryptionTbl[i];
		}
		id2[id2[15]] = NULL;
		if (strcmp(user, id2) != 0){
			//ID不符
			if (fseek(fp, sizeof(PET_ALBUM)* MAX_PET_KIND_1, SEEK_CUR) != 0){//找下一个
				fclose(fp);
				return FALSE;
			}
		}
		else{
			//读入资料
			if (fread(OldAlbum, sizeof(PET_ALBUM), MAX_PET_KIND_1, fp) < MAX_PET_KIND_1){
				fclose(fp);
				return FALSE;
			}
			for (int i = 0; i < MAX_PET_KIND_1; i++){
				for (int j = 0; j < MAX_PET_TBL; j++){
					if (!OldAlbum[i].flag)
						break;
					if (OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory(&PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum(PetAlbumTbl[j].albumNo);
						break;
					}
				}
			}
			break;
		}
	}
	fclose(fp);
	return TRUE;
}
#elif defined(__ALBUM_46)
BOOL LoadAlbum_46( char *user)
{
	FILE *fp;
	BOOL ret = FALSE;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "data\\album_2.dat");
	DeleteFile( "data\\album_3.dat");
	DeleteFile( "data\\album_10.dat");		
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fopen_s( &fp,ALBUM_FILE_NAME_47, "r+b")) != NULL){
		if( (fopen_s(&fp, ALBUM_FILE_NAME_47, "wb")) == NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//找不到此帐号的记录
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return FALSE;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//不是此帐号的记录
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return FALSE;
			}
		}else{
			//找到此帐号的记录
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return FALSE;
			}
			fclose( fp);
			return TRUE;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

BOOL ConvertAlbumTo46(  char *user, char *oldfile, DWORD num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( (  fopen_s(&fp, oldfile, "r+b" ) ) != NULL )
		return FALSE;
	while( 1){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//找到ID纪录
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return FALSE;
			}
			for( DWORD i = 0; i < num; i++){
				if( 440 <= i && i <= 461 ) continue;		//修正乌力Q 问题
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}

BOOL ConvertAlbum1_46(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( (  fopen_s( &fp,ALBUM_FILE_NAME, "r+b" ) ) != NULL )
		return FALSE;
	while( 1 ){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//找下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//读入资料
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return FALSE;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}							
				}
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}
#elif defined(__ALBUM_45)
BOOL LoadAlbum_45( char *user)
{
	FILE *fp;
	BOOL ret = FALSE;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "data\\album_2.dat");
	DeleteFile( "data\\album_3.dat");
	DeleteFile( "data\\album_10.dat");		
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fopen_s(&fp, ALBUM_FILE_NAME_46, "r+b")) != NULL){
		if( (fopen_s( &fp,ALBUM_FILE_NAME_46, "wb")) == NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//找不到此帐号的记录
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return FALSE;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//不是此帐号的记录
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return FALSE;
			}
		}else{
			//找到此帐号的记录
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return FALSE;
			}
			fclose( fp);
			return TRUE;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

BOOL ConvertAlbumTo45(  char *user, char *oldfile, DWORD num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fopen_s( &fp,oldfile, "r+b" ) ) != NULL )
		return FALSE;
	while( 1){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//找到ID纪录
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return FALSE;
			}
			for( DWORD i = 0; i < num; i++){
				if( 440 <= i && i <= 461 ) continue;		//修正乌力Q 问题
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}

BOOL ConvertAlbum1_45(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( (  fopen_s(&fp, ALBUM_FILE_NAME, "r+b" ) ) != NULL )
		return FALSE;
	while( 1 ){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//找下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//读入资料
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return FALSE;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}							
				}
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}
#elif defined(__ALBUM_44)
BOOL LoadAlbum_44( char *user)
{
	FILE *fp;
	BOOL ret = FALSE;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "data\\album_2.dat");
	DeleteFile( "data\\album_3.dat");
	DeleteFile( "data\\album_10.dat");		
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fopen_s(&fp, ALBUM_FILE_NAME_45, "r+b"))!= NULL){
		if( (fopen_s( &fp,ALBUM_FILE_NAME_45, "wb"))== NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//找不到此帐号的记录
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return FALSE;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//不是此帐号的记录
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return FALSE;
			}
		}else{
			//找到此帐号的记录
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return FALSE;
			}
			fclose( fp);
			return TRUE;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

BOOL ConvertAlbumTo44(  char *user, char *oldfile, DWORD num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fopen_s( &fp,oldfile, "r+b" ) ) != NULL )
		return FALSE;
	while( 1){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//找到ID纪录
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return FALSE;
			}
			for( DWORD i = 0; i < num; i++){
				if( 440 <= i && i <= 461 ) continue;		//修正乌力Q 问题
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}

BOOL ConvertAlbum1_44(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fopen_s(&fp, ALBUM_FILE_NAME, "r+b" ) ) != NULL )
		return FALSE;
	while( 1 ){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//找下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//读入资料
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return FALSE;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}							
				}
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}
#elif defined(__ALBUM_43)
BOOL LoadAlbum_43( char *user)
{
	FILE *fp;
	BOOL ret = FALSE;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "data\\album_2.dat");
	DeleteFile( "data\\album_3.dat");
	DeleteFile( "data\\album_10.dat");		
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fopen_s( &fp,ALBUM_FILE_NAME_44, "r+b"))!= NULL){
		if( (fopen_s( &fp,ALBUM_FILE_NAME_44, "wb")) == NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//找不到此帐号的记录
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return FALSE;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//不是此帐号的记录
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return FALSE;
			}
		}else{
			//找到此帐号的记录
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return FALSE;
			}
			fclose( fp);
			return TRUE;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

BOOL ConvertAlbumTo43(  char *user, char *oldfile, DWORD num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fopen_s( &fp,oldfile, "r+b" ) ) != NULL )
		return FALSE;
	while( 1){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//找到ID纪录
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return FALSE;
			}
			for( DWORD i = 0; i < num; i++){
				if( 440 <= i && i <= 461 ) continue;		//修正乌力Q 问题
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}

BOOL ConvertAlbum1_43(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( (  fopen_s(&fp, ALBUM_FILE_NAME, "r+b" ) ) != NULL )
		return FALSE;
	while( 1 ){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//找下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//读入资料
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return FALSE;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}							
				}
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}
#elif defined(__ALBUM_42)
BOOL LoadAlbum_42( char *user)
{
	FILE *fp;
	BOOL ret = FALSE;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "data\\album_2.dat");
	DeleteFile( "data\\album_3.dat");
	DeleteFile( "data\\album_10.dat");		
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fopen_s( &fp,ALBUM_FILE_NAME_43, "r+b"))!= NULL){
		if( (fopen_s(&fp, ALBUM_FILE_NAME_43, "wb")) == NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//找不到此帐号的记录
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return FALSE;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//不是此帐号的记录
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return FALSE;
			}
		}else{
			//找到此帐号的记录
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return FALSE;
			}
			fclose( fp);
			return TRUE;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

BOOL ConvertAlbumTo42(  char *user, char *oldfile, DWORD num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( (  fopen_s( &fp,oldfile, "r+b" ) ) != NULL )
		return FALSE;
	while( 1){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//找到ID纪录
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return FALSE;
			}
			for( DWORD i = 0; i < num; i++){
				if( 440 <= i && i <= 461 ) continue;		//修正乌力Q 问题
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}

BOOL ConvertAlbum1_42(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( (  fopen_s( &fp,ALBUM_FILE_NAME, "r+b" ) ) != NULL )
		return FALSE;
	while( 1 ){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//找下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//读入资料
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return FALSE;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}							
				}
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}
#elif defined(__ALBUM_41)
BOOL LoadAlbum_41( char *user)
{
	FILE *fp;
	BOOL ret = FALSE;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "data\\album_2.dat");
	DeleteFile( "data\\album_3.dat");
	DeleteFile( "data\\album_10.dat");		
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fopen_s( &fp,ALBUM_FILE_NAME_42, "r+b")) != NULL){
		if( (fopen_s(&fp, ALBUM_FILE_NAME_42, "wb")) == NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//找不到此帐号的记录
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return FALSE;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//不是此帐号的记录
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return FALSE;
			}
		}else{
			//找到此帐号的记录
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return FALSE;
			}
			fclose( fp);
			return TRUE;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

BOOL ConvertAlbumTo41(  char *user, char *oldfile, DWORD num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fopen_s( &fp,oldfile, "r+b" ) ) != NULL )
		return FALSE;
	while( 1){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//找到ID纪录
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return FALSE;
			}
			for( DWORD i = 0; i < num; i++){
				if( 440 <= i && i <= 461 ) continue;		//修正乌力Q 问题
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}

BOOL ConvertAlbum1_41(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fopen_s( &fp,ALBUM_FILE_NAME, "r+b" ) ) != NULL )
		return FALSE;
	while( 1 ){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//找下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//读入资料
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return FALSE;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}							
				}
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}
#elif defined(__ALBUM_40)
BOOL LoadAlbum_40( char *user)
{
	FILE *fp;
	BOOL ret = FALSE;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "data\\album_2.dat");
	DeleteFile( "data\\album_3.dat");
	DeleteFile( "data\\album_10.dat");		
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fopen_s( &fp,ALBUM_FILE_NAME_41, "r+b")) != NULL){
		if( (fopen_s( &fp,ALBUM_FILE_NAME_41, "wb")) == NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//找不到此帐号的记录
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return FALSE;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//不是此帐号的记录
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return FALSE;
			}
		}else{
			//找到此帐号的记录
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return FALSE;
			}
			fclose( fp);
			return TRUE;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

BOOL ConvertAlbumTo40(  char *user, char *oldfile, DWORD num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fopen_s( &fp,oldfile, "r+b" ) ) != NULL )
		return FALSE;
	while( 1){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//找到ID纪录
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return FALSE;
			}
			for( DWORD i = 0; i < num; i++){
				if( 440 <= i && i <= 461 ) continue;		//修正乌力Q 问题
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}

BOOL ConvertAlbum1_40(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fopen_s(&fp, ALBUM_FILE_NAME, "r+b" ) ) != NULL )
		return FALSE;
	while( 1 ){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//找下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//读入资料
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return FALSE;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}							
				}
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}
#elif defined(__ALBUM_39)
BOOL LoadAlbum_39( char *user)
{
	FILE *fp;
	BOOL ret = FALSE;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "data\\album_2.dat");
	DeleteFile( "data\\album_3.dat");
	DeleteFile( "data\\album_10.dat");		
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fopen_s(&fp, ALBUM_FILE_NAME_40, "r+b")) != NULL){
		if( (fopen_s( &fp,ALBUM_FILE_NAME_40, "wb")) == NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//找不到此帐号的记录
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return FALSE;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//不是此帐号的记录
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return FALSE;
			}
		}else{
			//找到此帐号的记录
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return FALSE;
			}
			fclose( fp);
			return TRUE;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

BOOL ConvertAlbumTo39(  char *user, char *oldfile, DWORD num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( (fopen_s( &fp,oldfile, "r+b" ) ) != NULL )
		return FALSE;
	while( 1){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//找到ID纪录
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return FALSE;
			}
			for( DWORD i = 0; i < num; i++){
				if( 440 <= i && i <= 461 ) continue;		//修正乌力Q 问题
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}

BOOL ConvertAlbum1_39(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( (  fopen_s( &fp,ALBUM_FILE_NAME, "r+b" ) ) != NULL )
		return FALSE;
	while( 1 ){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//找下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//读入资料
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return FALSE;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}							
				}
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}
#elif defined(__ALBUM_38)
BOOL LoadAlbum_38( char *user)
{
	FILE *fp;
	BOOL ret = FALSE;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "data\\album_2.dat");
	DeleteFile( "data\\album_3.dat");
	DeleteFile( "data\\album_10.dat");		
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fopen_s(&fp, ALBUM_FILE_NAME_39, "r+b")) != NULL){
		if( (fopen_s( &fp,ALBUM_FILE_NAME_39, "wb")) == NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//找不到此帐号的记录
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return FALSE;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//不是此帐号的记录
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return FALSE;
			}
		}else{
			//找到此帐号的记录
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return FALSE;
			}
			fclose( fp);
			return TRUE;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

BOOL ConvertAlbumTo38(  char *user, char *oldfile, DWORD num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fopen_s(&fp, oldfile, "r+b" ) ) != NULL )
		return FALSE;
	while( 1){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//找到ID纪录
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return FALSE;
			}
			for( DWORD i = 0; i < num; i++){
				if( 440 <= i && i <= 461 ) continue;		//修正乌力Q 问题
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}

BOOL ConvertAlbum1_38(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fopen_s( &fp,ALBUM_FILE_NAME, "r+b" ) ) != NULL )
		return FALSE;
	while( 1 ){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//找下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//读入资料
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return FALSE;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}							
				}
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}
#elif defined(__ALBUM_37)
BOOL LoadAlbum_37( char *user)
{
	FILE *fp;
	BOOL ret = FALSE;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "data\\album_2.dat");
	DeleteFile( "data\\album_3.dat");
	DeleteFile( "data\\album_10.dat");		
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fopen_s( &fp,ALBUM_FILE_NAME_38, "r+b")) != NULL){
		if( (fopen_s(&fp, ALBUM_FILE_NAME_38, "wb")) == NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//找不到此帐号的记录
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return FALSE;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//不是此帐号的记录
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return FALSE;
			}
		}else{
			//找到此帐号的记录
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return FALSE;
			}
			fclose( fp);
			return TRUE;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

BOOL ConvertAlbumTo37(  char *user, char *oldfile, DWORD num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( (  fopen_s( &fp,oldfile, "r+b" ) ) != NULL )
		return FALSE;
	while( 1){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//找到ID纪录
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return FALSE;
			}
			for( DWORD i = 0; i < num; i++){
				if( 440 <= i && i <= 461 ) continue;		//修正乌力Q 问题
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}

BOOL ConvertAlbum1_37(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( (  fopen_s( &fp,ALBUM_FILE_NAME, "r+b" ) ) != NULL )
		return FALSE;
	while( 1 ){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//找下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//读入资料
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return FALSE;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}							
				}
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}
#elif defined(__ALBUM_36)
BOOL LoadAlbum_36( char *user)
{
	FILE *fp;
	BOOL ret = FALSE;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "data\\album_2.dat");
	DeleteFile( "data\\album_3.dat");
	DeleteFile( "data\\album_10.dat");		
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fopen_s( &fp,ALBUM_FILE_NAME_37, "r+b")) != NULL){
		if( (fopen_s( &fp,ALBUM_FILE_NAME_37, "wb")) == NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//找不到此帐号的记录
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return FALSE;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//不是此帐号的记录
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return FALSE;
			}
		}else{
			//找到此帐号的记录
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return FALSE;
			}
			fclose( fp);
			return TRUE;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

BOOL ConvertAlbumTo36(  char *user, char *oldfile, DWORD num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fopen_s(&fp, oldfile, "r+b" ) ) != NULL )
		return FALSE;
	while( 1){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//找到ID纪录
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return FALSE;
			}
			for( DWORD i = 0; i < num; i++){
				if( 440 <= i && i <= 461 ) continue;		//修正乌力Q 问题
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}

BOOL ConvertAlbum1_36(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fopen_s( &fp,ALBUM_FILE_NAME, "r+b" ) ) != NULL )
		return FALSE;
	while( 1 ){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//找下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//读入资料
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return FALSE;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}							
				}
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}
#elif defined(__ALBUM_35)
BOOL LoadAlbum_35( char *user)
{
	FILE *fp;
	BOOL ret = FALSE;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "data\\album_2.dat");
	DeleteFile( "data\\album_3.dat");
	DeleteFile( "data\\album_10.dat");
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fopen_s( &fp,ALBUM_FILE_NAME_36, "r+b")) != NULL){
		if( (fopen_s(&fp, ALBUM_FILE_NAME_36, "wb")) == NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//找不到此帐号的记录
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return FALSE;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//不是此帐号的记录
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return FALSE;
			}
		}else{
			//找到此帐号的记录
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return FALSE;
			}
			fclose( fp);
			return TRUE;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

BOOL ConvertAlbumTo35(  char *user, char *oldfile, DWORD num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fopen_s(&fp, oldfile, "r+b" ) ) != NULL )
		return FALSE;
	while( 1){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//找到ID纪录
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return FALSE;
			}
			for( DWORD i = 0; i < num; i++){
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}

BOOL ConvertAlbum1_35(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( (  fopen_s(&fp, ALBUM_FILE_NAME, "r+b" ) ) != NULL )
		return FALSE;
	while( 1 ){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//找下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//读入资料
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return FALSE;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}							
				}
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}
#elif defined(__ALBUM_34)
BOOL LoadAlbum_34( char *user)
{
	FILE *fp;
	BOOL ret = FALSE;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "data\\album_2.dat");
	DeleteFile( "data\\album_3.dat");
	DeleteFile( "data\\album_10.dat");
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fopen_s(&fp, ALBUM_FILE_NAME_35, "r+b"))!= NULL){
		if( (fopen_s(&fp, ALBUM_FILE_NAME_35, "wb")) == NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//找不到此帐号的记录
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return FALSE;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//不是此帐号的记录
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return FALSE;
			}
		}else{
			//找到此帐号的记录
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return FALSE;
			}
			fclose( fp);
			return TRUE;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

BOOL ConvertAlbumTo34(  char *user, char *oldfile, DWORD num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fopen_s( &fp,oldfile, "r+b" ) ) != NULL )
		return FALSE;
	while( 1){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//找到ID纪录
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return FALSE;
			}
			for( DWORD i = 0; i < num; i++){
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}

BOOL ConvertAlbum1_34(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fopen_s(&fp, ALBUM_FILE_NAME, "r+b" ) ) != NULL )
		return FALSE;
	while( 1 ){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//找下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//读入资料
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return FALSE;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}							
				}
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}
#elif defined(__ALBUM_33)
BOOL LoadAlbum_33( char *user)
{
	FILE *fp;
	BOOL ret = FALSE;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "data\\album_2.dat");
	DeleteFile( "data\\album_3.dat");
	DeleteFile( "data\\album_10.dat");
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fopen_s( &fp,ALBUM_FILE_NAME_34, "r+b")) != NULL){
		if( (fopen_s( &fp,ALBUM_FILE_NAME_34, "wb")) == NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//找不到此帐号的记录
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return FALSE;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//不是此帐号的记录
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return FALSE;
			}
		}else{
			//找到此帐号的记录
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return FALSE;
			}
			fclose( fp);
			return TRUE;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

BOOL ConvertAlbumTo33(  char *user, char *oldfile, DWORD num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( (  fopen_s( &fp,oldfile, "r+b" ) ) != NULL )
		return FALSE;
	while( 1){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//找到ID纪录
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return FALSE;
			}
			for( DWORD i = 0; i < num; i++){
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}

BOOL ConvertAlbum1_33(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( ALBUM_FILE_NAME, "r+b" ) ) == NULL )
		return FALSE;
	while( 1 ){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//找下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//读入资料
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return FALSE;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}							
				}
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}
#elif defined(__ALBUM_32)
BOOL LoadAlbum_32( char *user)
{
	FILE *fp;
	BOOL ret = FALSE;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "data\\album_2.dat");
	DeleteFile( "data\\album_3.dat");
	DeleteFile( "data\\album_10.dat");
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fp=fopen( ALBUM_FILE_NAME_4, "r+b")) == NULL){
		if( (fp=fopen( ALBUM_FILE_NAME_4, "wb")) != NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//找不到此帐号的记录
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return FALSE;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//不是此帐号的记录
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return FALSE;
			}
		}else{
			//找到此帐号的记录
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return FALSE;
			}
			fclose( fp);
			return TRUE;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

BOOL ConvertAlbumTo32(  char *user, char *oldfile, DWORD num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( oldfile, "r+b" ) ) == NULL )
		return FALSE;
	while( 1){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//找到ID纪录
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return FALSE;
			}
			for( DWORD i = 0; i < num; i++){
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}

BOOL ConvertAlbum1_32(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( ALBUM_FILE_NAME, "r+b" ) ) == NULL )
		return FALSE;
	while( 1 ){
		//读入ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return FALSE;
		}
		//ID解码
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID不符
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//找下一个
				fclose( fp );
				return FALSE;
			}
		}else{
			//读入资料
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return FALSE;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}							
				}
			}
			break;
		}
	}
	fclose( fp );
	return TRUE;
}
#endif


// ???????? ****************************************************************/
BOOL LoadAlbum(void)
{
	char id[32];

	// ?????
	extern char szUser[];
	CopyMemory(id, szUser, 32);
	ecb_crypt("f;encor1c", id, 32, DES_DECRYPT);
	// ????
	id[15] = strlen(id);

#if defined(__ALBUM_47)
	if (LoadAlbum_47(id))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_47, MAX_PET_KIND_46))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_46, MAX_PET_KIND_45))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_45, MAX_PET_KIND_44))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_44, MAX_PET_KIND_43))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_43, MAX_PET_KIND_42))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_42, MAX_PET_KIND_41))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_41, MAX_PET_KIND_40))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_40, MAX_PET_KIND_39))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_39, MAX_PET_KIND_38))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_38, MAX_PET_KIND_37))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_37, MAX_PET_KIND_36))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_36, MAX_PET_KIND_35))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5))
		return TRUE;
	if (ConvertAlbumTo47(id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4))
		return TRUE;
	if (ConvertAlbum1_47(id))
		return TRUE;
	return FALSE;
#elif defined(__ALBUM_46)
	if( LoadAlbum_46( id) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_46, MAX_PET_KIND_45) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_45, MAX_PET_KIND_44) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_44, MAX_PET_KIND_43) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_43, MAX_PET_KIND_42) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_42, MAX_PET_KIND_41) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_41, MAX_PET_KIND_40) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_40, MAX_PET_KIND_39) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_39, MAX_PET_KIND_38) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_38, MAX_PET_KIND_37) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_37, MAX_PET_KIND_36) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_36, MAX_PET_KIND_35) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return TRUE;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return TRUE;
	if( ConvertAlbum1_46( id) )
		return TRUE;
	return FALSE;
#elif defined(__ALBUM_45)
	if( LoadAlbum_45( id) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_45, MAX_PET_KIND_44) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_44, MAX_PET_KIND_43) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_43, MAX_PET_KIND_42) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_42, MAX_PET_KIND_41) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_41, MAX_PET_KIND_40) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_40, MAX_PET_KIND_39) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_39, MAX_PET_KIND_38) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_38, MAX_PET_KIND_37) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_37, MAX_PET_KIND_36) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_36, MAX_PET_KIND_35) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return TRUE;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return TRUE;
	if( ConvertAlbum1_45( id) )
		return TRUE;
	return FALSE;
#elif defined(__ALBUM_44)
	if( LoadAlbum_44( id) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_44, MAX_PET_KIND_43) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_43, MAX_PET_KIND_42) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_42, MAX_PET_KIND_41) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_41, MAX_PET_KIND_40) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_40, MAX_PET_KIND_39) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_39, MAX_PET_KIND_38) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_38, MAX_PET_KIND_37) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_37, MAX_PET_KIND_36) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_36, MAX_PET_KIND_35) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return TRUE;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return TRUE;
	if( ConvertAlbum1_44( id) )
		return TRUE;
	return FALSE;
#elif defined(__ALBUM_43)
	if( LoadAlbum_43( id) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_43, MAX_PET_KIND_42) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_42, MAX_PET_KIND_41) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_41, MAX_PET_KIND_40) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_40, MAX_PET_KIND_39) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_39, MAX_PET_KIND_38) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_38, MAX_PET_KIND_37) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_37, MAX_PET_KIND_36) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_36, MAX_PET_KIND_35) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return TRUE;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return TRUE;
	if( ConvertAlbum1_43( id) )
		return TRUE;
	return FALSE;
#elif defined(__ALBUM_42)
	if( LoadAlbum_42( id) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_42, MAX_PET_KIND_41) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_41, MAX_PET_KIND_40) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_40, MAX_PET_KIND_39) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_39, MAX_PET_KIND_38) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_38, MAX_PET_KIND_37) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_37, MAX_PET_KIND_36) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_36, MAX_PET_KIND_35) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return TRUE;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return TRUE;
	if( ConvertAlbum1_42( id) )
		return TRUE;
	return FALSE;
#elif defined(__ALBUM_41)
	if( LoadAlbum_41( id) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_41, MAX_PET_KIND_40) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_40, MAX_PET_KIND_39) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_39, MAX_PET_KIND_38) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_38, MAX_PET_KIND_37) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_37, MAX_PET_KIND_36) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_36, MAX_PET_KIND_35) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return TRUE;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return TRUE;
	if( ConvertAlbum1_41( id) )
		return TRUE;
	return FALSE;
#elif defined(__ALBUM_40)
	if( LoadAlbum_40( id) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_40, MAX_PET_KIND_39) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_39, MAX_PET_KIND_38) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_38, MAX_PET_KIND_37) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_37, MAX_PET_KIND_36) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_36, MAX_PET_KIND_35) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return TRUE;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return TRUE;
	if( ConvertAlbum1_40( id) )
		return TRUE;
	return FALSE;
#elif defined(__ALBUM_39)
	if( LoadAlbum_39( id) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_39, MAX_PET_KIND_38) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_38, MAX_PET_KIND_37) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_37, MAX_PET_KIND_36) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_36, MAX_PET_KIND_35) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return TRUE;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return TRUE;
	if( ConvertAlbum1_39( id) )
		return TRUE;
	return FALSE;
#elif defined(__ALBUM_38)
	if( LoadAlbum_38( id) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_38, MAX_PET_KIND_37) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_37, MAX_PET_KIND_36) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_36, MAX_PET_KIND_35) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return TRUE;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return TRUE;
	if( ConvertAlbum1_38( id) )
		return TRUE;
	return FALSE;
#elif defined(__ALBUM_37)
	if( LoadAlbum_37( id) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_37, MAX_PET_KIND_36) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_36, MAX_PET_KIND_35) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return TRUE;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return TRUE;
	if( ConvertAlbum1_37( id) )
		return TRUE;
	return FALSE;
#elif defined(__ALBUM_36)
	if( LoadAlbum_36( id) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_36, MAX_PET_KIND_35) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return TRUE;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return TRUE;
	if( ConvertAlbum1_36( id) )
		return TRUE;
	return FALSE;
#elif defined(__ALBUM_35)
	if( LoadAlbum_35( id) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return TRUE;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return TRUE;
	if( ConvertAlbum1_35( id) )
		return TRUE;
	return FALSE;
#elif defined(__ALBUM_34)
	if( LoadAlbum_34( id) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return TRUE;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return TRUE;
	if( ConvertAlbum1_34( id) )
		return TRUE;
	return FALSE;
#elif defined(__ALBUM_33)
	if( LoadAlbum_33( id) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return TRUE;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return TRUE;
	if( ConvertAlbum1_33( id) )
		return TRUE;
	return FALSE;
#elif defined(__ALBUM_32)
	if( LoadAlbum_32( id) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return TRUE;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return TRUE;
	if( ConvertAlbum1_32( id) )
		return TRUE;
	return FALSE;
#elif defined(__ALBUM_15)
	if( LoadAlbum_15( id) )
		return TRUE;
	if( ConvertAlbumTo15( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return TRUE;
	if( ConvertAlbumTo15( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return TRUE;
	if( ConvertAlbumTo15( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return TRUE;
	if( ConvertAlbumTo15( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return TRUE;
	if( ConvertAlbumTo15( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return TRUE;
	if( ConvertAlbumTo15( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return TRUE;
	if( ConvertAlbumTo15( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return TRUE;
	if( ConvertAlbumTo15( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return TRUE;
	if( ConvertAlbumTo15( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return TRUE;
	if( ConvertAlbumTo15( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return TRUE;
	if( ConvertAlbumTo15( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return TRUE;
	if( ConvertAlbum1_15( id) )
		return TRUE;
	return FALSE;	
#else
	char id2[16];
	int i;
	FILE *fp;
	// ?????????
	AlbumIdCnt = 0;
	// ???????????
	if ((fp = fopen(ALBUM_FILE_NAME, "r+b")) == NULL){

		// ???????
		if ((fp = fopen(ALBUM_FILE_NAME, "wb")) != NULL){
			// ????????????
			id[id[15]] = Rnd(1, 100);
			// ??????
			for (i = 0; i < 16; i++){
				id[i] ^= IdEncryptionTbl[i];

			}
			// ?????????
			fwrite(id, sizeof(id), 1, fp);

			// ???????????
			for (i = 0; i < MAX_PET_KIND; i++){
				int j;
				// ???
				for (j = 0; j < PET_NAME_LEN + 1; j++){
					PetAlbum[i].name[j] = Rnd(1, 255);
					PetAlbum[i].freeName[j] = Rnd(1, 255);
				}

				PetAlbum[i].faceGraNo = Rnd(1, 60000);
				PetAlbum[i].level = Rnd(1, 30000);
				PetAlbum[i].faceGraNo = Rnd(1, 30000);
				PetAlbum[i].maxHp = Rnd(1, 30000);
				PetAlbum[i].str = Rnd(1, 30000);
				PetAlbum[i].quick = Rnd(1, 30000);
				PetAlbum[i].def = Rnd(1, 30000);

				PetAlbum[i].earth = Rnd(1, 30000);
				PetAlbum[i].water = Rnd(1, 30000);
				PetAlbum[i].fire = Rnd(1, 30000);
				PetAlbum[i].wind = Rnd(1, 30000);
			}

			// ???????
			fwrite(PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			fclose(fp);	// ????????
		}

		return FALSE;
	}
	// ????????? ????? ?
	while (1){
		// ??????
		if (fread(id2, sizeof(id2), 1, fp) < 1){
			// ??????????
			// ????????????
			id[id[15]] = Rnd(1, 100);
			// ??????
			for (i = 0; i < 16; i++){
				id[i] ^= IdEncryptionTbl[i];
			}
			// ?????????
			fwrite(id, sizeof(id), 1, fp);
			// ???????
			fwrite(PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			fclose(fp);	// ????????
			return FALSE;
		}
		// ?????????
		for (i = 0; i < 16; i++){
			id2[i] ^= IdEncryptionTbl[i];
		}
		// ?????????
		id2[id2[15]] = NULL;
		// ??????
		if (strcmp(id, id2) != 0){
			// ????????????????????
			if (fseek(fp, sizeof(PET_ALBUM)* MAX_PET_KIND, SEEK_CUR) != 0){
				fclose(fp);	// ????????
				return FALSE;
			}
		}
		else{// ????
			// ??????????
			if (fread(PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fclose(fp);	// ????????
				return FALSE;
			}
			break;
		}
		// ??????
		AlbumIdCnt++;
	}
	// ????????
	fclose(fp);

	return TRUE;
#endif
}

// ???????????? **************************************************/
void InitOhtaParam(void)
{
	LoadMailHistory();					// ??????????
	LoadChatHistoryStr();				// ????????????
	//LoadReadNameShield();
	//LoadReadSayShield();
	mailLamp = CheckMailNoReadFlag();	// ????
	StatusUpPoint = 0;					// ???????????????
	battleResultMsg.useFlag = 0;		// ????????
	MapWmdFlagBak = 0;
}

// ??????? ****************************************************************/
BOOL CheckMailNoReadFlag(void)
{
	int i, j;

	// ???
	for (i = 0; i < MAX_ADR_BOOK; i++){
		// ?????
		for (j = 0; j < MAIL_MAX_HISTORY; j++){
			// ???????
			if (MailHistory[i].noReadFlag[j] >= TRUE) return TRUE;
		}
	}
	return FALSE;
}

// ???????? ****************************************************************/
void CheckNewPet(int sprNo)
{
	int tblNo = sprNo - 100250; // ??????
	int albumNo;
	// ?????
#if defined(__ALBUM_47)
	if (tblNo > 1800){					//小恶魔
		tblNo -= 1208;
	}
	else if (tblNo > 1755){					//间隔南瓜魔王后的 狐猴
		tblNo -= 1201;
	}
	else if (tblNo > 1739){ //1710 ){			//	间隔骑宠后的 飞蛇
		tblNo -= 1200;
	}
	else if (tblNo > 1686){			//海底融合宠
		tblNo -= 1175;
	}
	else if (tblNo > 1641){			//鸡年兽4 甲虫2	8.0第一次整合测试
		tblNo -= 1167;
	}
	else if (tblNo > 1635){	//麒麟
		tblNo -= 1148;
	}
	else if (tblNo > 1634){	//猫女1 猫女2 
		tblNo -= 1149;
	}
	else if (tblNo > 1616){//布里萨尔  蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
		tblNo -= 1146;
	}
	else if (tblNo > 1568){//机人龙
		tblNo -= 1103;
	}
	else if (tblNo > 1564){//黄色飞龙
		tblNo -= 1101;
	}
	else if (tblNo > 1516){
		tblNo -= 1055;
	}
	else if (tblNo == 1516){//修正乌力王
		tblNo = 455;
	}
	else if (tblNo > 1509){
		tblNo -= 1054;
	}
	else if (tblNo == 1497){
		tblNo = 395;
	}
	else if (tblNo > 1495){
		tblNo -= 1044;
	}
	else if (tblNo > 1490){
		tblNo -= 1043;
	}
	else if (tblNo > 1485){
		tblNo -= 1040;
	}
	else if (tblNo > 1454) {
		tblNo -= 1034;
	}
	else if (tblNo == 1214) {
		tblNo = 331;
	}
	else if (tblNo == 1217) {
		tblNo = 328;
	}
	else if (tblNo > 1361){
		tblNo -= 952;
	}
	else if (tblNo > 1356){
		tblNo -= 999;
	}
	else if (tblNo > 1281){
		tblNo -= 919;
	}
	else if (tblNo > 1239){
		tblNo -= 889;
	}
	else if (tblNo > 1186){
		tblNo -= 886;
	}
	else if (tblNo > 1173){
		tblNo -= 877;
	}
	else if (tblNo > 1172){
		tblNo -= 877;
	}
	else if (tblNo > 1161){
		tblNo -= 868;
	}
	else if (tblNo > 930){
		tblNo -= 745;
	}
	else if (tblNo == 927){
		tblNo -= 634;
	}
	else if (tblNo > 929){
		tblNo -= 644;
	}
	else if (tblNo > 927){
		tblNo -= 643;
	}
	else if (tblNo > 900){
		tblNo -= 621;
	}
	else if (tblNo > 180){
		tblNo -= (333 + 56);
	}
#elif defined(__ALBUM_46)
	if( tblNo > 1800 ){					//小恶魔
		tblNo -= 1208;
	}else if( tblNo > 1755 ){					//间隔南瓜魔王后的 狐猴
		tblNo -= 1201;
	}else if (tblNo > 1739 ){ //1710 ){			//	间隔骑宠后的 飞蛇
		tblNo -= 1200;
	}else if (tblNo > 1686 ){			//海底融合宠
		tblNo -= 1175;
	}else if (tblNo > 1641 ){			//鸡年兽4 甲虫2	8.0第一次整合测试
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//麒麟
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//猫女1 猫女2 
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//布里萨尔  蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//机人龙
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//黄色飞龙
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//修正乌力王
		tblNo = 455;
	}else if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_45)
	if( tblNo > 1800 ){					//小恶魔
		tblNo -= 1208;
	}else if( tblNo > 1755 ){					//间隔南瓜魔王后的 狐猴
		tblNo -= 1201;
	}else if (tblNo > 1739 ){ //1710 ){			//	间隔骑宠后的 飞蛇
		tblNo -= 1200;
	}else if (tblNo > 1686 ){			//海底融合宠
		tblNo -= 1175;
	}else if (tblNo > 1641 ){			//鸡年兽4 甲虫2	8.0第一次整合测试
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//麒麟
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//猫女1 猫女2 
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//布里萨尔  蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//机人龙
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//黄色飞龙
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//修正乌力王
		tblNo = 455;
	}else if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_44)
	if( tblNo > 1800 ){					//小恶魔
		tblNo -= 1208;
	}else if( tblNo > 1755 ){					//间隔南瓜魔王后的 狐猴
		tblNo -= 1201;
	}else if (tblNo > 1739 ){ //1710 ){			//	间隔骑宠后的 飞蛇
		tblNo -= 1200;
	}else if (tblNo > 1686 ){			//海底融合宠
		tblNo -= 1175;
	}else if (tblNo > 1641 ){			//鸡年兽4 甲虫2	8.0第一次整合测试
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//麒麟
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//猫女1 猫女2 
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//布里萨尔  蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//机人龙
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//黄色飞龙
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//修正乌力王
		tblNo = 455;
	}else if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_43)
	if( tblNo > 1800 ){					//小恶魔
		tblNo -= 1208;
	}else if( tblNo > 1755 ){					//间隔南瓜魔王后的 狐猴
		tblNo -= 1201;
	}else if (tblNo > 1739 ){ //1710 ){			//	间隔骑宠后的 飞蛇
		tblNo -= 1200;
	}else if (tblNo > 1686 ){			//海底融合宠
		tblNo -= 1175;
	}else if (tblNo > 1641 ){			//鸡年兽4 甲虫2	8.0第一次整合测试
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//麒麟
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//猫女1 猫女2 
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//布里萨尔  蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//机人龙
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//黄色飞龙
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//修正乌力王
		tblNo = 455;
	}else if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_42)
	if( tblNo > 1800 ){					//小恶魔
		tblNo -= 1208;
	}else if( tblNo > 1755 ){					//间隔南瓜魔王后的 狐猴
		tblNo -= 1201;
	}else if (tblNo > 1739 ){ //1710 ){			//	间隔骑宠后的 飞蛇
		tblNo -= 1200;
	}else if (tblNo > 1686 ){			//海底融合宠
		tblNo -= 1175;
	}else if (tblNo > 1641 ){			//鸡年兽4 甲虫2	8.0第一次整合测试
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//麒麟
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//猫女1 猫女2 
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//布里萨尔  蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//机人龙
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//黄色飞龙
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//修正乌力王
		tblNo = 455;
	}else if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_41)
	if( tblNo > 1800 ){					//小恶魔
		tblNo -= 1208;
	}else if( tblNo > 1755 ){					//间隔南瓜魔王后的 狐猴
		tblNo -= 1201;
	}else if (tblNo > 1739 ){ //1710 ){			//	间隔骑宠后的 飞蛇
		tblNo -= 1200;
	}else if (tblNo > 1686 ){			//海底融合宠
		tblNo -= 1175;
	}else if (tblNo > 1641 ){			//鸡年兽4 甲虫2	8.0第一次整合测试
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//麒麟
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//猫女1 猫女2 
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//布里萨尔  蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//机人龙
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//黄色飞龙
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//修正乌力王
		tblNo = 455;
	}else if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_40)
	if( tblNo > 1800 ){					//小恶魔
		tblNo -= 1208;
	}else if( tblNo > 1755 ){					//间隔南瓜魔王后的 狐猴
		tblNo -= 1201;
	}else if (tblNo > 1739 ){ //1710 ){			//	间隔骑宠后的 飞蛇
		tblNo -= 1200;
	}else if (tblNo > 1686 ){			//海底融合宠
		tblNo -= 1175;
	}else if (tblNo > 1641 ){			//鸡年兽4 甲虫2	8.0第一次整合测试
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//麒麟
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//猫女1 猫女2 
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//布里萨尔  蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//机人龙
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//黄色飞龙
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//修正乌力王
		tblNo = 455;
	}else if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_39)
	if( tblNo > 1800 ){					//小恶魔
		tblNo -= 1208;
	}else if( tblNo > 1755 ){					//间隔南瓜魔王后的 狐猴
		tblNo -= 1201;
	}else if (tblNo > 1739 ){ //1710 ){			//	间隔骑宠后的 飞蛇
		tblNo -= 1200;
	}else if (tblNo > 1686 ){			//海底融合宠
		tblNo -= 1175;
	}else if (tblNo > 1641 ){			//鸡年兽4 甲虫2	8.0第一次整合测试
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//麒麟
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//猫女1 猫女2 
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//布里萨尔  蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//机人龙
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//黄色飞龙
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//修正乌力王
		tblNo = 455;
	}else if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_38)

	if( tblNo > 1755 ){					//间隔南瓜魔王后的 狐猴
		tblNo -= 1201;
	}else if (tblNo > 1739 ){ //1710 ){			//	间隔骑宠后的 飞蛇
		tblNo -= 1200;
	}else if (tblNo > 1686 ){			//海底融合宠
		tblNo -= 1175;
	}else if (tblNo > 1641 ){			//鸡年兽4 甲虫2	8.0第一次整合测试
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//麒麟
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//猫女1 猫女2 
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//布里萨尔  蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//机人龙
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//黄色飞龙
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//修正乌力王
		tblNo = 455;
	}else if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_37)

	if (tblNo > 1710 ){			//	间隔骑宠后的 飞蛇
		tblNo -= 1200;
	}else if (tblNo > 1686 ){			//海底融合宠
		tblNo -= 1175;
	}else if (tblNo > 1641 ){			//鸡年兽4 甲虫2	8.0第一次整合测试
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//麒麟
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//猫女1 猫女2 
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//布里萨尔  蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//机人龙
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//黄色飞龙
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//修正乌力王
		tblNo = 455;
	}else if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_36)

	if (tblNo > 1686 ){			//鸡年兽4 甲虫2	8.0第一次整合测试
		tblNo -= 1175;
	}else if (tblNo > 1641 ){			//鸡年兽4 甲虫2	8.0第一次整合测试
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//麒麟
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//猫女1 猫女2 
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//布里萨尔  蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//机人龙
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//黄色飞龙
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//修正乌力王
		tblNo = 455;
	}else if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_35)

	if (tblNo > 1641 ){			//鸡年兽4 甲虫2 8.0第一次整合测试
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//麒麟
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//猫女1 猫女2 
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//布里萨尔  蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//机人龙
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//黄色飞龙
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//修正乌力王
		tblNo = 455;
	}else if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_34)

	if (tblNo > 1639 ){			//鸡年兽4
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//麒麟
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//猫女1 猫女2 麒麟
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//布里萨尔  蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//机人龙
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//黄色飞龙
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//修正乌力王
		tblNo = 455;
	}else if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_33)

	if (tblNo > 1635 ){
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//猫女1 猫女2 麒麟
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//布里萨尔  蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//机人龙
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//黄色飞龙
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//修正乌力王
		tblNo = 455;
	}else if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_32)

	if ( tblNo > 1634 ){	//猫女1 猫女2
		tblNo -= 1149;
	}else if ( tblNo > 1628 ){	//火蚁1 火蚁2 牛人1 牛人2
		tblNo -= 1147;
	}else if ( tblNo > 1616 ){//布里萨尔  蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//机人龙
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//黄色飞龙
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//修正乌力王
		tblNo = 455;
	}else if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_31)

	if ( tblNo > 1628 ){	//火蚁1 火蚁2 牛人1 牛人2
		tblNo -= 1147;
	}else if ( tblNo > 1616 ){//布里萨尔  蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//机人龙
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//黄色飞龙
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//修正乌力王
		tblNo = 455;
	}else if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_30)

	if ( tblNo > 1616 ){//布里萨尔  蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//机人龙
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//黄色飞龙
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//修正乌力王
		tblNo = 455;
	}else if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_29)

	if ( tblNo > 1616 ){//布里萨尔  蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//机人龙
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//黄色飞龙
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//修正乌力王
		tblNo = 455;
	}else if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_28)

	if ( tblNo > 1616 ){//布里萨尔  蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//机人龙
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//黄色飞龙
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//修正乌力王
		tblNo = 455;
	}else if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}

#elif defined(__ALBUM_27)

	if ( tblNo > 1616 ){//布里萨尔  蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//机人龙
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//黄色飞龙
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//修正乌力王
		tblNo = 455;
	}else if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}

#elif defined(__ALBUM_26)

	if ( tblNo > 1616 ){//布里萨尔  蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//机人龙
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//黄色飞龙
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//修正乌力王
		tblNo = 455;
	}else if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}

#elif defined(__ALBUM_25)
	if( tblNo > 1616 ){//布里萨尔
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//机人龙
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//黄色飞龙
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//修正乌力王
		tblNo = 455;
	}else if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}

#elif defined(__ALBUM_24)
	if( tblNo > 1568 ){//机人龙
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//黄色飞龙
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//修正乌力王
		tblNo = 455;
	}else if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}

#elif defined(__ALBUM_23)
	if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//修正乌力王
		tblNo = 455;
	}else if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}

#elif defined(__ALBUM_22)
	if ( tblNo == 1516 ){//修正乌力王
		tblNo = 455;
	}else if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}

#elif defined(__ALBUM_21)
	if ( tblNo > 1509 ){
		tblNo -= 1054;
	}else if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}

#elif defined(__ALBUM_20)
	if ( tblNo == 1497 ){
		tblNo = 395;
	}else if ( tblNo > 1495 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
		tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}

#elif defined(__ALBUM_19)
	if ( tblNo > 1490 ){
		tblNo -= 1044 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}

#elif defined(__ALBUM_18)
	if ( tblNo > 1485 ){
		tblNo -= 1040 ; 
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}

#elif defined(__ALBUM_17)
	if ( tblNo > 1454 ) {
		tblNo -= 1034 ; 
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}

#elif defined(__ALBUM_16)
	if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_15)
	if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ; 
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#endif
	// ????????????
	if (0 <= tblNo && tblNo < MAX_PET_TBL){
		// ???????????
		albumNo = PetAlbumTbl[tblNo].albumNo;
		// ????????
		if (albumNo != -1 ){
			// ???????????
			if (PetAlbum[albumNo].flag == 0){
				// ???????
				PetAlbum[albumNo].flag = 1;
				// ???
				strcpy(PetAlbum[albumNo].name, PetAlbumTbl[tblNo].name);
				// ?????
				SaveAlbum(albumNo);
			}
		}
	}
}

// ?????????? **********************************************************/
void CheckBattleNewPet(void)
{
	int i;

	// ?????
	for (i = 0; i < BATTLKPKPLYAERNUM; i++){
		// ??????????
		if (p_party[i] == NULL) continue;
		// ??????????
		if (p_party[i]->func == NULL) continue;
		// ?????????
		CheckNewPet(p_party[i]->anim_chr_no);
	}
}

// ?????????? *****************************************************/
void CenteringStr(char *inStr, char *outStr, int max)
{
	int len, space, amari;

	// ?????
	len = strlen(inStr);

	// ????????
	if (len >= max){
		// ???????
		strncpy_s(outStr, max + 1, inStr, max);
		outStr[max] = NULL;	// ??????
		return;
	}

	// ???????
	amari = (max - len) % 2;
	// ?????
	space = (max - len) / 2;

	// ?????????
	if (space != 0){
		// ???
		sprintf(outStr, "%*c%s%*c", space, ' ', inStr, space + amari, ' ');
	}
	else{
		// ???
		sprintf(outStr, "%s ", inStr);
	}
}

// ????????????????? *************************************/
int CheckPetSkill(int skillId)
{
	int i, j;

	// ????????
	for (j = 0; j < MAX_PET; j++){
		// ???????????
		if (pet[j].useFlag == TRUE){
			// ?????
			for (i = 0; i < pet[j].maxSkill; i++){
				// ????????
				if (petSkill[j][i].useFlag == TRUE){
					// ??????
					if (petSkill[j][i].skillId == skillId){
						return TRUE;
					}
				}
			}
		}
	}
	return FALSE;
}

// ?????????? *******************************************************/
void InitItem(int x, int y, BOOL bPetItemFlag)
{
	int i, j = 0, k = 0;

#ifdef _ITEM_EQUITSPACE

#ifdef _SA_VERSION_25
	struct _tagInitXY{
		int x;
		int y;
	}InitXY[CHAR_EQUIPPLACENUM] = {
		{ x + 84, y + 51 }, { x + 85, y + 119 }, { x + 33, y + 122 },
		{ x + 137, y + 63 }, { x + 33, y + 63 }, { x + 137, y + 122 },
		{ x + 137, y + 122 }, { x + 136, y + 130 }
#ifdef _EQUIT_NEWGLOVE
		, { x + 137, y + 122 }
#endif
	};
#endif
#ifdef _PET_ITEM
	if (bPetItemFlag)		// 目前显示的是宠物道具栏
	{
		// seting x
		InitXY[0].x = InitXY[5].x = x + 63;
		InitXY[1].x = InitXY[6].x = x + 113;
		InitXY[2].x = x + 36;
		InitXY[3].x = x + 87;
		InitXY[4].x = x + 138;
		// seting y
		InitXY[0].y = InitXY[1].y = y + 34;
		InitXY[2].y = InitXY[3].y = InitXY[4].y = y + 83;
		InitXY[5].y = InitXY[6].y = y + 132;
	}
#endif
	for (i = 0; i < MAX_ITEMSTART; i++)
	{
		ItemBuffer[i].defX = InitXY[i].x;
		ItemBuffer[i].defY = InitXY[i].y;
		ItemBuffer[i].x = ItemBuffer[0].defX;
		ItemBuffer[i].y = ItemBuffer[0].defY;
		ItemBuffer[i].bmpNo = 20000 + 0;
		ItemBuffer[i].dispPrio = DISP_PRIO_ITEM;
	}
#else
	ItemBuffer[ 0 ].defX = x + 136;
	ItemBuffer[ 0 ].defY = y + 59;
	ItemBuffer[ 0 ].x = ItemBuffer[ 0 ].defX;
	ItemBuffer[ 0 ].y = ItemBuffer[ 0 ].defY;
	ItemBuffer[ 0 ].bmpNo = 20000 + 0;
	ItemBuffer[ 0 ].dispPrio = DISP_PRIO_ITEM;

	ItemBuffer[ 1 ].defX = x + 134;
	ItemBuffer[ 1 ].defY = y + 129;
	ItemBuffer[ 1 ].x = ItemBuffer[ 1 ].defX;
	ItemBuffer[ 1 ].y = ItemBuffer[ 1 ].defY;
	ItemBuffer[ 1 ].bmpNo = 20200 + 1;
	ItemBuffer[ 1 ].dispPrio = DISP_PRIO_ITEM;

	ItemBuffer[ 2 ].defX = x + 61;
	ItemBuffer[ 2 ].defY = y + 129;
	ItemBuffer[ 2 ].x = ItemBuffer[ 2 ].defX;
	ItemBuffer[ 2 ].y = ItemBuffer[ 2 ].defY;
	ItemBuffer[ 2 ].bmpNo = 20400 + 2;
	ItemBuffer[ 2 ].dispPrio = DISP_PRIO_ITEM;

	ItemBuffer[ 3 ].defX = x + 32;
	ItemBuffer[ 3 ].defY = y + 68;
	ItemBuffer[ 3 ].x = ItemBuffer[ 3 ].defX;
	ItemBuffer[ 3 ].y = ItemBuffer[ 3 ].defY;
	ItemBuffer[ 3 ].bmpNo = 20600 + 3;
	ItemBuffer[ 3 ].dispPrio = DISP_PRIO_ITEM;

	ItemBuffer[ 4 ].defX = x + 83;
	ItemBuffer[ 4 ].defY = y + 68;
	ItemBuffer[ 4 ].x = ItemBuffer[ 4 ].defX;
	ItemBuffer[ 4 ].y = ItemBuffer[ 4 ].defY;
	ItemBuffer[ 4 ].bmpNo = 20800 + 4;
	ItemBuffer[ 4 ].dispPrio = DISP_PRIO_ITEM;
#endif

	for (i = MAX_ITEMSTART; i < MAX_ITEM; i++){
		ItemBuffer[i].defX = x + 32 + j;
		ItemBuffer[i].defY = y + 56 + 48 + 48 + 48 + k;
		ItemBuffer[i].x = ItemBuffer[i].defX;
		ItemBuffer[i].y = ItemBuffer[i].defY;
		ItemBuffer[i].bmpNo = 20000 + j / 48;
		ItemBuffer[i].dispPrio = DISP_PRIO_ITEM;
		ItemBuffer[i].mixFlag = FALSE;
		j += 51;
		if (j >= 48 * 5){
			j = 0;
#ifdef _NEW_ITEM_
			if (k >= 48 * 2){
				k = 0;
			}
			else k += 48;
#else
			k += 48;
#endif
		}
	}
}

// CoolFish: Trade 04/14
void InitItem3(int x, int y)
{
	int i, j = 0, k = 0;
	// ????????
	for (i = MAX_ITEMSTART; i < MAX_ITEM; i++){
		ItemBuffer[i].defX = x + 32 + j;
		ItemBuffer[i].defY = y + 56 + 48 + 48 + 48 - 160 + k;
		ItemBuffer[i].x = ItemBuffer[i].defX;
		ItemBuffer[i].y = ItemBuffer[i].defY;
		ItemBuffer[i].bmpNo = 20000 + j / 48;
		ItemBuffer[i].dispPrio = DISP_PRIO_ITEM;
		j += 51; // ?????
		// ????????
		if (j >= 48 * 5){
			j = 0;
#ifdef _NEW_ITEM_
			if (k >= 48 * 2){
				k = 0;
			}
			else k += 48;
#else
			k += 48;
#endif
		}
	}
}
// CoolFish: End 

// ???????? ******************************************************/
BOOL MakeHitBox(int x1, int y1, int x2, int y2, int dispPrio)
{
	// ?????
	if (mouse.nowPoint.x <= x2 && x1 <= mouse.nowPoint.x &&
		mouse.nowPoint.y <= y2 && y1 <= mouse.nowPoint.y){
		// ???????
		if (dispPrio >= 0){
			// ????????????????
			StockBoxDispBuffer(x1, y1, x2, y2, dispPrio, BoxColor, 0);
		}
		return TRUE;
	}
	return FALSE;
}

// ????????? **************************************************************/
void DeathLetterAction(void)
{
	int i;

	for (i = 0; i < 4; i++){
		DeathAction(pActLetter[i]);
		pActLetter[i] = NULL;
	}
}

// ???????? ***************************************************************/
void InitMailSendFlag(void)
{
	int i;

	for (i = 0; i < MAX_ADR_BOOK; i++) mailWndSendFlag[i] = 0;
}

#ifdef _PET_SKINS
extern 	int originalPetSkin;
extern void initPetSkinWin();
extern void petSkinProc();
extern void petChange(int graphNo);
#endif


// ????????????? *****************************************************/
void DeathMenuAction(void)
{

#ifdef _MAGIC_ITEM_
	if(道具光环Act) DeathAction(道具光环Act);
	道具光环Act=NULL;
#endif
	if(pActMenuWnd)
		DeathAction(pActMenuWnd);
	pActMenuWnd = NULL;
#ifdef _FRIENDCHANNEL			//ROG ADD 好友频道
	chatRoomBtn = 0;
	assentFlag = FALSE;
	DeathAction( pAssentWnd );
	pAssentWnd = NULL;
	setRoomFlag = FALSE;
	DeathAction( pSetRoomWnd );
	pSetRoomWnd = NULL;
#endif
	//新增 动作表情切换地图BUG
	extern ACTION * ptActMenuWin1;
	if (ptActMenuWin1){
		DeathAction(ptActMenuWin1);
		ptActMenuWin1 = NULL;
	}
	if(pActMenuWnd3)
		DeathAction(pActMenuWnd3);
	pActMenuWnd3 = NULL;
	// ????????
	GetKeyInputFocus(&MyChatBuffer);
	if(pActPet)
		DeathAction(pActPet);
	pActPet = NULL;
	if(pActMailItem)
		DeathAction(pActMailItem);
	pActMailItem = NULL;
	if(pActYesNoWnd)
		DeathAction(pActYesNoWnd);
	pActYesNoWnd = NULL;
	// ?????????
	DeathLetterAction();

	// Robin 04/14 trade
	if (pActMenuWnd4){
		DeathAction(pActMenuWnd4);
		pActMenuWnd4 = NULL;
	}
#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
	//清除所使用Action
	if (SecondTradeWnd){
		DeathAction(SecondTradeWnd);
		SecondTradeWnd = NULL;
	}
	if (SecondActPet){
		DeathAction(SecondActPet);
		SecondActPet = NULL;
	}
	if (TradeTalkWnd){
		DeathAction(TradeTalkWnd);
		TradeTalkWnd = NULL;
	}
#endif
	if (pActPet3){
		DeathAction(pActPet3);
		pActPet3 = NULL;
	}

	if (pActPet4){
		DeathAction(pActPet4);
		pActPet4 = NULL;
	}
	if (pActPet5){
		DeathAction(pActPet5);
		pActPet5 = NULL;
	}
	if (pActMenuWnd5){
		DeathAction(pActMenuWnd5);
		pActMenuWnd5 = NULL;
	}
#ifdef _PET_SKINS
	initPetSkinWin();
	petWndNo = 0;
#endif
}
// ????????????? *****************************************************/
void DeathMenuAction2(void)
{
#ifdef _MAGIC_ITEM_
	if(道具光环Act) DeathAction(道具光环Act);
	道具光环Act=NULL;
	extern void ClearMagicItemWin();
	ClearMagicItemWin();
#endif
	DeathAction(pActMenuWnd2);
#ifdef _MONEYINPUT //Syu ADD 手动输入金钱量
	GetKeyInputFocus(&MyChatBuffer);
	Moneyflag = false;
#endif
	pActMenuWnd2 = NULL;
	// ?????
	DeathAction(pActPet2);
	pActPet2 = NULL;
	// ????????????
	ItemMixPetNo = -1;

	// Robin 04/14 trade
	DeathAction(pActMenuWnd4);
#ifdef _FRIENDCHANNEL			//ROG ADD 好友频道
	chatRoomBtn = 0;
	assentFlag = FALSE;
	DeathAction( pAssentWnd );
	pAssentWnd = NULL;
	setRoomFlag = FALSE;
	DeathAction( pSetRoomWnd );
	pSetRoomWnd = NULL;
#endif

#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
	//清除所使用Action
	DeathAction(SecondTradeWnd);
	SecondTradeWnd = NULL;
	DeathAction(SecondActPet);
	SecondActPet = NULL;
	DeathAction(TradeTalkWnd);
	TradeTalkWnd = NULL;
#endif
	//视窗消灭时还原设定
	Tradeflag = false;
	pActMenuWnd4 = NULL;
	DeathAction(pActPet3);
	pActPet3 = NULL;
	DeathAction(pActPet4);
	pActPet4 = NULL;
	DeathAction(pActPet5);
	pActPet5 = NULL;
	DeathAction(pActMenuWnd5);
	pActMenuWnd5 = NULL;

}

// ?????????? *******************************************************/
void InitMenu(void)
{

	int i;

	// ????
	for (i = 0; i < MENU_SYSTEM_0; i++) systemWndFontNo[i] = -2;
	// ???
	for (i = 0; i < MENU_MAP_0; i++) mapWndFontNo[i] = -2;
	mapWndNo = 0;
	// ?????
	for (i = 0; i < MENU_STATUS_0; i++) statusWndFontNo[i] = -2;
	for (i = 0; i < MENU_STATUS_0; i++) statusWndBtnFlag[i] = 0;

	// Robin 04/14 trade
	for (i = 0; i < MENU_TRADE_0; i++) tradeWndFontNo[i] = -2;
	for (i = 0; i < MENU_TRADE_0; i++) tradeWndBtnFlag[i] = 0;

	// ???
	for (i = 0; i < MENU_PET_0; i++) petWndFontNo[i] = -2;
	petWndNo = 0;
	// ????
	for (i = 0; i < MENU_ITEM_0; i++) itemWndFontNo[i] = -2;
	for (i = 0; i < MENU_ITEM_0; i++) itemWndBtnFlag[i] = 0;
	itemWndNo = 0;
	// ???
	for (i = 0; i < MENU_MAIL_0; i++) mailWndFontNo[i] = -2;
	for (i = 0; i < MAX_ADR_BOOK; i++) mailWndSendFlag[i] = 0;
	mailWndNo = MAIL_WND_VIEW;
	// ????
	for (i = 0; i < MENU_ALBUM_0; i++) albumWndFontNo[i] = -2;
	albumWndNo = 0;
	// ?????
	for (i = 0; i < TASK_BAR; i++) taskBarFontNo[i] = -2;
	// ???????
	for (i = 0; i < MENU_BTL_RESULT_0; i++) resultWndFontNo[i] = -2;
	// ??????
	MenuToggleFlag = 0;
#ifdef _ITEM_JIGSAW 
	JigsawIdx = 0;
#endif
#ifdef _TELLCHANNEL				//ROG ADD 密语频道
	MultiTells = FALSE;
#endif

	// ?????????????
	DeathMenuAction();
	// ?????????????
	DeathMenuAction2();
	// ???????????
	mouse.itemNo = -1;
	// ????????
	itemNo = -1;
	// ???????
	jujutuNo = -1;
	// ????????
	ItemMixPetNo = -1;
#ifdef __AI
	void AI_CloseWnd();
	AI_CloseWnd();
#endif
}

// ??????????? *******************************************************/
void InitMenu2(void)
{
	// ??????
	MenuToggleFlag = 0;
	// ?????????????
	DeathMenuAction();
	// ?????????????
	DeathMenuAction2();
	// ???????????
	mouse.itemNo = -1;
	// ????????
	itemNo = -1;
	// ???????
	jujutuNo = -1;
	// ???????????
	BattleResultWndFlag = FALSE;

}

// ??????????? *******************************************************/
BOOL CheckMenuFlag(void)
{
	BOOL flag = FALSE;

	// ??????
	if (MenuToggleFlag != 0) flag = TRUE;
	// ???????????
	if (BattleResultWndFlag >= 1) flag = TRUE;
	// ?????????????????
	if (MapWmdFlagBak >= TRUE) flag = TRUE;
#ifdef _FRIENDCHANNEL
	if( assentFlag ) flag = assentFlag;
	if( setRoomFlag ) flag = setRoomFlag;
#endif

	return flag;
}

#ifdef __PHONEMESSAGE
void CleanSMS()
{
	extern STR_BUFFER ptext;
	extern HANDLE MHandle;
	ptext.buffer[0]='\0';
	if(MHandle)
	{
		TerminateThread(MHandle,1);
		CloseHandle(MHandle);
	}
}
#endif


#ifdef _AIDENGLU_
BOOL 自动登陆窗口 = FALSE;
ACTION *p自动登陆窗口 = NULL;
ACTION * 方向动作;
ACTION * 人物动作;
extern struct gameserver gmsv[];
extern struct gamegroup  gmgroup[];
void 自动登陆窗口回调()
{
	static int 方向窗口是否打开;
	static int btnId[15], btnState[15];
	static int x = 0, y = 0;
	static DWORD dwPressTime;
	static char msg[][8] = {
		"队长",
		"队员",
		"单人"
	};
	static char  chardir[][8] = {
		"下",
		"左下",
		"左",
		"左上",
		"上",
		"右上",
		"右",
		"右下"
	};
	if (p自动登陆窗口 == NULL){
		方向窗口是否打开 = FALSE;
		方向动作 = NULL;
		人物动作 = NULL;
		int w = 412;
		int h = 301;
		x = (lpDraw->xSize - w) / 2;
		y = (lpDraw->ySize - h) / 2;
		p自动登陆窗口 = MakeWindowDisp(x, y, w, h, 55241, -1, FALSE);
		for (int i = 0; i < 14; i++){
			btnId[i] = -2;
			if (i < 10)
				btnState[i] = 0;
		}
		dwPressTime = 0;
	}
	else{
		if (p自动登陆窗口->hp >= 1){
			if (joy_trg[0] & JOY_ESC){
				自动登陆窗口 = FALSE;
				DeathAction(p自动登陆窗口);
				DeathAction(方向动作);
				DeathAction(人物动作);
				人物动作 = NULL;
				方向窗口是否打开 = FALSE;
				方向动作 = NULL;
				p自动登陆窗口 = NULL;
				actBtn = 0;
				return;
			}
			if (dwPressTime){
				if (TimeGetTime() > (dwPressTime + 100)){
					dwPressTime = 0;
					if (btnState[0] == 1){//大区减
						btnState[0] = 0;
						play_se(217, 320, 240);
						while (TRUE){
							PcLanded.大区--;
							if (PcLanded.大区 < 0){
								PcLanded.大区 = 18;
							}
							if (gmgroup[PcLanded.大区].used){
								break;
							}
						}
					}
					if (btnState[1] == 1){//大区加
						btnState[1] = 0;
						play_se(217, 320, 240);
						while (TRUE){
							PcLanded.大区++;
							if (PcLanded.大区 > 18){
								PcLanded.大区 = 0;
							}
							if (gmgroup[PcLanded.大区].used){
								break;
							}
						}
					}

					if (btnState[2] == 1){//小区减
						btnState[2] = 0;
						play_se(217, 320, 240);
						if (PcLanded.大区 != -1)
						{
							while (TRUE){
								PcLanded.小区--;
								if (PcLanded.小区 < 0){
									PcLanded.小区 = gmgroup[PcLanded.大区].num - 1;
								}
								if (gmsv[gmgroup[PcLanded.大区].startindex + PcLanded.小区].used){
									break;
								}
							}
						}
					}
					if (btnState[3] == 1){//小区加
						btnState[3] = 0;
						play_se(217, 320, 240);
						if (PcLanded.大区 != -1)
						{
							while (TRUE){
								PcLanded.小区++;
								if (PcLanded.小区 >= gmgroup[PcLanded.大区].num){
									PcLanded.小区 = 0;
								}
								if (gmsv[gmgroup[PcLanded.大区].startindex + PcLanded.小区].used){
									break;
								}
							}
						}
					}
					if (btnState[4] == 1){//队模减
						btnState[4] = 0;
						play_se(217, 320, 240);
						PcLanded.队模--;
						if (PcLanded.队模 < 0){
							PcLanded.队模 = 2;
						}
						if (PcLanded.队模 == 0 || PcLanded.队模 == 2){
							PcLanded.是否自动喊话 = FALSE;
						}
						if (PcLanded.队模 == 1){
							PcLanded.是否自动遇敌 = FALSE;
						}
					}
					if (btnState[5] == 1){//队模加
						btnState[5] = 0;
						play_se(217, 320, 240);
						PcLanded.队模++;
						if (PcLanded.队模 > 2){
							PcLanded.队模 = 0;
						}
						if (PcLanded.队模 == 0 || PcLanded.队模 == 2){
							PcLanded.是否自动喊话 = FALSE;
						}
						if (PcLanded.队模 == 1){
							PcLanded.是否自动遇敌 = FALSE;
						}
					}
					if (btnState[6] == 1){//角色减
						btnState[6] = 0;
						play_se(217, 320, 240);
						while (TRUE){
							PcLanded.人物--;
							if (PcLanded.人物 < 0){
								PcLanded.人物 = 3;
							}
							if (PcLanded.登陆人物名称[PcLanded.人物][0]) break;
						}
					}
					if (btnState[7] == 1){//角色加
						btnState[7] = 0;
						play_se(217, 320, 240);
						while (TRUE){
							PcLanded.人物++;
							if (PcLanded.人物 > 3){
								PcLanded.人物 = 0;
							}
							if (PcLanded.登陆人物名称[PcLanded.人物][0]) break;
						}
					}
					if (btnState[8] == 1){//确定
						btnState[8] = 0;
						play_se(217, 320, 240);
						DeathAction(p自动登陆窗口);
						p自动登陆窗口 = NULL;
						自动登陆窗口 = FALSE;
						if (方向动作){
							DeathAction(方向动作);
							方向动作 = NULL;
							DeathAction(人物动作);
							人物动作 = NULL;
						}
						extern short actBtn;
						actBtn = 0;
						return;
					}
					if (btnState[9] == 1){//取消
						btnState[9] = 0;
						play_se(217, 320, 240);
						DeathAction(p自动登陆窗口);
						p自动登陆窗口 = NULL;
						自动登陆窗口 = FALSE;
						if (方向动作){
							DeathAction(方向动作);
							方向动作 = NULL;
							DeathAction(人物动作);
							人物动作 = NULL;
						}
						extern short actBtn;
						actBtn = 0;
						return;
					}
					if (btnState[10] == 1){//方向
						btnState[10] = 0;
						play_se(217, 320, 240);
						DeathAction(方向动作);
						方向动作 = NULL;
						DeathAction(人物动作);
						人物动作 = NULL;
						方向窗口是否打开 = FALSE;
					}
				}
			}
			else{
				for (int i = 0; i < 10; i++){
					if (HitDispNo == btnId[i]){
						if (i == 0 || i == 1) ShowBottomLineString(FONT_PAL_WHITE, "设置大区。");
						if (i == 2 || i == 3) ShowBottomLineString(FONT_PAL_WHITE, "设置线路,需设置大区方可设置。");
						if (i == 4 || i == 5) ShowBottomLineString(FONT_PAL_WHITE, "队长：可设自动遇敌 队员：可设喊话和自动组队 单人：可设自动遇敌 ps：人物掉线重登自动开启AI模式。");
						if (i == 6 || i == 7) ShowBottomLineString(FONT_PAL_WHITE, "设置登陆人物。");
						if (mouse.onceState & MOUSE_LEFT_CRICK){
							dwPressTime = TimeGetTime();
							btnState[i] = 1;
							break;
						}
					}
				}
				if (HitDispNo == btnId[14]){
					if (mouse.onceState & MOUSE_LEFT_CRICK){
						dwPressTime = TimeGetTime();
						btnState[10] = 1;
					}
				}
			}

			extern int 自动登陆是否开启;
			if (HitFontNo == btnId[10]){
				ShowBottomLineString(FONT_PAL_WHITE, "登陆游戏中可按F9开启和关闭,队员模式登陆游戏后喊话可关闭该功能停止喊话。");
				if (mouse.onceState & MOUSE_LEFT_CRICK){
					if (PcLanded.大区 != -1 && PcLanded.小区 != -1 && PcLanded.人物 != -1){
						自动登陆是否开启 = !自动登陆是否开启;
						if (!自动登陆是否开启){
							extern int 是否重登组队, 是否重登喊话, 是否重开登组队_1;
							是否重登组队 = FALSE;
							是否重登喊话 = FALSE;
							是否重开登组队_1 = FALSE;
						}
					}
					else{
						StockChatBufferLine("请把大区、线路和角色选择后才能操作！", FONT_PAL_RED);
					}
				}
			}
			if (HitFontNo == btnId[12]){
				if (mouse.onceState & MOUSE_LEFT_CRICK){
					ShowBottomLineString(FONT_PAL_WHITE, "队长和单人可设置该功能。");
					if (PcLanded.队模 == 0 || PcLanded.队模 == 2)
						PcLanded.是否自动遇敌 = !PcLanded.是否自动遇敌;
					else{
						PcLanded.是否自动遇敌 = FALSE;
						StockChatBufferLine("遇敌模式只有队长和单人可开启！", FONT_PAL_RED);
					}
				}
			}
			if (HitFontNo == btnId[13]){
				if (mouse.onceState & MOUSE_LEFT_CRICK){
					ShowBottomLineString(FONT_PAL_WHITE, "只有队员可设置该功能。");
					if (PcLanded.队模 == 1)
						PcLanded.是否自动喊话 = !PcLanded.是否自动喊话;
					else{
						PcLanded.是否自动喊话 = FALSE;
						StockChatBufferLine("自动喊话只有队员模式可开启！", FONT_PAL_RED);
					}
				}
			}
			if (HitFontNo == btnId[11]){
				ShowBottomLineString(FONT_PAL_WHITE, "掉线人物登陆后人物的方向。");
				if (mouse.onceState & MOUSE_LEFT_CRICK){
					if (方向窗口是否打开){
						方向窗口是否打开 = FALSE;
						if (方向动作){
							DeathAction(方向动作);
							方向动作 = NULL;
							DeathAction(人物动作);
							人物动作 = NULL;
						}
					}
					else 方向窗口是否打开 = TRUE;
				}
			}
			int 方向偏移X = 0;
			if (方向窗口是否打开) 方向偏移X = -90;
			if (方向窗口是否打开){
				if (!方向动作){
					方向动作 = MakeWindowDisp(607 + 方向偏移X, 147, 192, 148, 55242, -1, FALSE);
					人物动作 = MakeAnimDisp(660 + 方向偏移X, 240, pc.graNo, 0);
					人物动作->anim_ang = PcLanded.人物方向;
					pattern(人物动作, ANM_NOMAL_SPD, ANM_LOOP);
					btnId[14] = -2;
					btnState[10] = 0;
				}
				else if (方向动作->hp >= 1){
					StockDispBuffer(((WINDOW_DISP *)方向动作->pYobi)->mx, ((WINDOW_DISP *)方向动作->pYobi)->my, DISP_PRIO_MENU, 55242, 1);
					PcLanded.人物方向 = 人物动作->anim_ang;
					btnId[14] = StockDispBuffer(((WINDOW_DISP *)方向动作->pYobi)->mx - 55, ((WINDOW_DISP *)方向动作->pYobi)->my - 90, DISP_PRIO_IME3, 26262 + btnState[10], 2);
				}
			}
			StockDispBuffer(((WINDOW_DISP *)p自动登陆窗口->pYobi)->mx + 方向偏移X, ((WINDOW_DISP *)p自动登陆窗口->pYobi)->my, DISP_PRIO_MENU, 55241, 1);
			btnId[0] = StockDispBuffer(x + 128 + 方向偏移X, y + 92 + 70, DISP_PRIO_IME3, 26064 + btnState[0], 2);
			btnId[1] = StockDispBuffer(x + 109 + 方向偏移X, y + 107 + 71, DISP_PRIO_IME3, 26066 + btnState[1], 2);

			btnId[2] = StockDispBuffer(x + 273 + 方向偏移X, y + 92 + 70, DISP_PRIO_IME3, 26064 + btnState[2], 2);
			btnId[3] = StockDispBuffer(x + 254 + 方向偏移X, y + 107 + 71, DISP_PRIO_IME3, 26066 + btnState[3], 2);

			btnId[4] = StockDispBuffer(x + 128 + 方向偏移X, y + 92 + 106, DISP_PRIO_IME3, 26064 + btnState[4], 2);
			btnId[5] = StockDispBuffer(x + 109 + 方向偏移X, y + 107 + 107, DISP_PRIO_IME3, 26066 + btnState[5], 2);

			btnId[6] = StockDispBuffer(x + 128 + 方向偏移X, y + 92 + 142, DISP_PRIO_IME3, 26064 + btnState[6], 2);
			btnId[7] = StockDispBuffer(x + 109 + 方向偏移X, y + 107 + 143, DISP_PRIO_IME3, 26066 + btnState[7], 2);

			btnId[8] = StockDispBuffer(x + 207 + 方向偏移X, y + 152, DISP_PRIO_IME3, 26262 + btnState[8], 2);
			btnId[9] = StockDispBuffer(x + 207 + 方向偏移X, y + 152, DISP_PRIO_IME3, 26264 + btnState[9], 2);

			char moji[256];
			if (PcLanded.大区 != -1){
				CenteringStr(gmgroup[PcLanded.大区].name, moji, CHAR_NAME_LEN);
				StockFontBuffer(x + 105 + 方向偏移X, y + 64, FONT_PRIO_FRONT, FONT_PAL_YELLOW, moji, 0);
			}
			if (PcLanded.队模 != -1){
				StockFontBuffer(x + 150 + 方向偏移X, y + 100, FONT_PRIO_FRONT, FONT_PAL_YELLOW, msg[PcLanded.队模], 0);
			}
			if (PcLanded.人物 != -1){
				CenteringStr(PcLanded.登陆人物名称[PcLanded.人物], moji, CHAR_NAME_LEN);
				StockFontBuffer(x + 105 + 方向偏移X, y + 136, FONT_PRIO_FRONT, FONT_PAL_YELLOW, moji, 0);
			}

			if (自动登陆是否开启)
				btnId[10] = StockFontBuffer(x + 150 + 方向偏移X, y + 171, FONT_PRIO_FRONT, FONT_PAL_YELLOW, "开启", 2);
			else
				btnId[10] = StockFontBuffer(x + 150 + 方向偏移X, y + 171, FONT_PRIO_FRONT, FONT_PAL_YELLOW, "关闭", 2);
			if (PcLanded.小区 != -1){
				StockFontBuffer(x + 305 + 方向偏移X, y + 64, FONT_PRIO_FRONT, FONT_PAL_YELLOW, gmsv[gmgroup[PcLanded.大区].startindex + PcLanded.小区].name, 0);
			}
			btnId[11] = StockFontBuffer(x + 320 + 方向偏移X, y + 100, FONT_PRIO_FRONT, FONT_PAL_YELLOW, chardir[PcLanded.人物方向], 2);
			if (PcLanded.是否自动遇敌)
				btnId[12] = StockFontBuffer(x + 320 + 方向偏移X, y + 136, FONT_PRIO_FRONT, FONT_PAL_YELLOW, "开启", 2);
			else
				btnId[12] = StockFontBuffer(x + 320 + 方向偏移X, y + 136, FONT_PRIO_FRONT, FONT_PAL_YELLOW, "关闭", 2);
			if (PcLanded.是否自动喊话)
				btnId[13] = StockFontBuffer(x + 320 + 方向偏移X, y + 171, FONT_PRIO_FRONT, FONT_PAL_YELLOW, "开启", 2);
			else
				btnId[13] = StockFontBuffer(x + 320 + 方向偏移X, y + 171, FONT_PRIO_FRONT, FONT_PAL_YELLOW, "关闭", 2);
		}
	}
}

#endif




#ifdef _AI_OTHER
extern int AI_Other_State;
extern void AI_OtherProc();
#endif
void MenuProc(void)
{
	int x, y, i;
	x = y = i = 0;
	char moji[256];
#ifdef _ITEM_JIGSAW 
	DrawJigsawFlag = false;
#endif
#ifdef __AI
	extern int AI_State;
	if (AI_State){
		extern BOOL AI_SettingProc();
		if (AI_SettingProc()){
			AI_State = 0;
			extern short actBtn;
			actBtn = 0;
		}
	}
#endif
#ifdef _AIDENGLU_
	if (自动登陆窗口){
		自动登陆窗口回调();
	}
#endif
#ifdef _AI_OTHER
	if (AI_Other_State) AI_OtherProc();
#endif

#ifdef _TELLCHANNEL				//ROG ADD 密语频道
	if (MultiTells == TRUE)
	{
		SelectChar();
		if (MenuToggleFlag & JOY_ESC)
			DeathTellChannel();
	}
#endif

#ifdef _PET_SKINS
	
	if (petWndNo == 1){
		petSkinProc();
	}
	else
	{
		initPetSkinWin();
	}
	
#endif

#ifdef _TIMEBAR_FUNCTION
	if(timeBarFlag == TRUE)
	{
		DrawTimeBar();
	}
#endif

	// ??????????????
#ifdef __AI
	if (joy_trg[0] & JOY_ESC && checkFieldMenuFlag() == FALSE && !AI_State&&!自动登陆窗口


		){
#else
	if( joy_trg[ 0 ] & JOY_ESC && checkFieldMenuFlag() == FALSE ){
#endif
		// ?????????????
		if (GetImeString() == NULL){
			// ???????
			if (MenuToggleFlag != 0 || BattleResultWndFlag >= 1){
				MenuToggleFlag = 0;
				// ???????????
				BattleResultWndFlag = FALSE;
				// ???????????
				mouse.itemNo = -1;
				// ????????
				itemNo = -1;
				// ???????
				jujutuNo = -1;
				// ????????????
				if (MapWmdFlagBak != TRUE){
					// ????????
					play_se(203, 320, 240);
				}
			}
			else{  // ??????????
				if (GetImeString() == NULL){

					MenuToggleFlag ^= JOY_ESC;	// ?????
					// ????????
					play_se(202, 320, 240);
					// ????
					for (i = 0; i < MENU_SYSTEM_0; i++) systemWndFontNo[i] = -2;
					systemWndNo = 0;		// ?????????

					// ????????
					if (MenuToggleFlag & JOY_ESC){
						MenuToggleFlag &= JOY_CTRL_I | JOY_CTRL_M;	// ??????????????????
						MenuToggleFlag |= JOY_ESC;	// ?????
						// ???????????
						BattleResultWndFlag = FALSE;
						// ????????
						play_se(202, 320, 240);
					}
					else{
						// ????????
						play_se(203, 320, 240);
					}
				}
			}
			// ??????????
			DeathMenuAction();
			DeathMenuAction2();
			// ????????
			saveUserSetting();
		}
	}
	// ??????????????
#ifdef __AI
	if (TaskBarFlag == TRUE && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[3] && !AI_State&&!自动登陆窗口
			
		){
#else
	if( TaskBarFlag == TRUE && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[ 3 ] ){ 
#endif
#ifdef _BATTLESKILL
		extern int wonflag ;
		if ( wonflag == 1 ) 
			play_se( 220, 320, 240 );
		else {
#endif
			MenuToggleFlag ^= JOY_ESC;	// ?????
			// ????
			for (i = 0; i < MENU_SYSTEM_0; i++) systemWndFontNo[i] = -2;
			systemWndNo = 0;		// ?????????
			// ??????????
			DeathMenuAction();
			// ????????
			saveUserSetting();

			// ????????
			if (MenuToggleFlag & JOY_ESC){
				MenuToggleFlag &= JOY_CTRL_I | JOY_CTRL_M;	// ??????????????????
				MenuToggleFlag |= JOY_ESC;	// ?????
				// ???????????
				BattleResultWndFlag = FALSE;
				// ????????
				play_se(202, 320, 240);
			}
			else{
				// ????????
				play_se(203, 320, 240);
			}
#ifdef _BATTLESKILL
		}
#endif
	}

	// ??????????
#ifdef __AI
	if (!AI_State&&!自动登陆窗口 && ((joy_trg[0] & JOY_CTRL_S && GetImeString() == NULL)
		|| (TaskBarFlag == TRUE && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[1]))
		

		){
#else
	if( ( joy_trg[ 0 ] & JOY_CTRL_S && GetImeString() == NULL ) 
		|| ( TaskBarFlag == TRUE && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[ 1 ] ) ){ 
#endif
		// ???
		if (ProcNo == PROC_BATTLE){
			// ???
			play_se(220, 320, 240);
		}
		else{

			MenuToggleFlag ^= JOY_CTRL_S;	// CTRL + S ??
			// ?????
			for (i = 0; i < MENU_STATUS_0; i++) statusWndFontNo[i] = -2;
			for (i = 0; i < MENU_STATUS_0; i++) statusWndBtnFlag[i] = 0;
			statusWndNo = 0;
			// ??????????
			DeathMenuAction();
			// ????????
			saveUserSetting();

			// ????????
			if (MenuToggleFlag & JOY_CTRL_S){
				MenuToggleFlag &= JOY_CTRL_I | JOY_CTRL_M;	// ??????????????????
				MenuToggleFlag |= JOY_CTRL_S;	// ??????????
				// ???????????
				BattleResultWndFlag = FALSE;
				// ????????
				play_se(202, 320, 240);
			}
			else{
				// ????????
				play_se(203, 320, 240);
			}
		}
	}

	// Nuke 0413: Trade
#ifdef __AI
	if (!AI_State&&!自动登陆窗口 && joy_trg[0] & JOY_CTRL_T && GetImeString() == NULL) {
#else
	if ( joy_trg[ 0 ] & JOY_CTRL_T && GetImeString() == NULL ) {
#endif
		joy_trg[0] &= ~JOY_CTRL_T;

		// ???
		if (ProcNo == PROC_BATTLE){
			// ???
			play_se(220, 320, 240);
		}
		else{

#if 0
			MenuToggleFlag ^= JOY_CTRL_T;	// CTRL + T ??

			for (i = 0; i < MENU_TRADE_0; i++) tradeWndFontNo[i] = -2;
			for (i = 0; i < MENU_TRADE_0; i++) tradeWndBtnFlag[i] = 0;

			tradeWndNo = 0;

			tradeInit();

			DeathMenuAction();
			DeathMenuAction2();
			// ????????
			saveUserSetting();

			// ????????
			if (MenuToggleFlag & JOY_CTRL_T){
				MenuToggleFlag &= 0;
				MenuToggleFlag |= JOY_CTRL_T;	// ??????????
				// ???????????
				BattleResultWndFlag = FALSE;

				// ????????
				play_se(202, 320, 240);
			}
			else{
				// ????????
				play_se(203, 320, 240);
			}
#else
			int dx, dy;
			int flag;

			// ???????????
			getRouteData(pc.dir, &dx, &dy);
			flag = checkCharObjPoint(nowGx + dx, nowGy + dy, CHAROBJ_TYPE_USER_NPC);
			// ??????????????????
			if (partyModeFlag == 0 && flag == TRUE && eventWarpSendFlag == 0 && eventEnemySendFlag == 0
				&& sendEnFlag == 0 && tradeFlag == 0
#ifdef _STREET_VENDOR
				&& (pc.iOnStreetVendor == 0 || sStreetVendorBuyBtn == 2)
#endif
#ifdef _THEATER
				&& pc.iTheaterMode == 0
#endif
				)
			{
				if (!(MenuToggleFlag & JOY_CTRL_T))// ??
				{
					if (bNewServer)
						lssproto_TD_send(sockfd, "D|D");
					else
						old_lssproto_TD_send(sockfd, "D|D");
				}
			}
#ifdef _STREET_VENDOR
			if (pc.iOnStreetVendor == 1)
				StockChatBufferLine("摆摊中不得进行交易", FONT_PAL_RED);
#endif
#ifdef _THEATER
			if (pc.iTheaterMode > 0)
				StockChatBufferLine("请专心表演", FONT_PAL_RED);
#endif
			play_se(217, 320, 240);	// ?????

#endif

		}
	}

	// ????????
#ifdef __AI
	if (!AI_State&&!自动登陆窗口 && ((joy_trg[0] & JOY_CTRL_P && GetImeString() == NULL)
		|| (TaskBarFlag == TRUE && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[2]))
			

		){
#else
	if( ( joy_trg[ 0 ] & JOY_CTRL_P && GetImeString() == NULL )
		|| ( TaskBarFlag == TRUE && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[ 2 ] ) ){ 
#endif
		// ???????????????
		if (ProcNo == PROC_BATTLE){
			// ???
			play_se(220, 320, 240);
		}
		else{
			{
				MenuToggleFlag ^= JOY_CTRL_P;	// CTRL + P ??
				// ???
				for (i = 0; i < MENU_PET_0; i++) petWndFontNo[i] = -2;
				petWndNo = 0;		// ?????????

				// ??????????
				DeathMenuAction();
				// ????????
				saveUserSetting();

				// ????????
				if (MenuToggleFlag & JOY_CTRL_P){
					MenuToggleFlag &= JOY_CTRL_I | JOY_CTRL_M;	// ??????????????????
					MenuToggleFlag |= JOY_CTRL_P;
					// ???????????
					BattleResultWndFlag = FALSE;
					// ????????
					play_se(202, 320, 240);
				}
				else{
					// ????????
					play_se(203, 320, 240);
				}
			}
		}
	}
	// ?????????
#ifdef __AI
	if (!AI_State&&!自动登陆窗口 && ((joy_trg[0] & JOY_CTRL_I && GetImeString() == NULL)
		|| (TaskBarFlag == TRUE && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[4]))
				
		){
#else
	if( ( joy_trg[ 0 ] & JOY_CTRL_I && GetImeString() == NULL ) 
		|| ( TaskBarFlag == TRUE && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[ 4 ] ) ){ 
#endif
		// ???
		if (ProcNo == PROC_BATTLE){
			// ???
			play_se(220, 320, 240);
		}
		else{
#ifdef _STREET_VENDOR
			if (pc.iOnStreetVendor == 1 || sStreetVendorBuyBtn == 2) StockChatBufferLine("摆摊中不得使用道具", FONT_PAL_RED);
			else
#endif
#ifdef _THEATER
			if (pc.iTheaterMode & 0x00000002)
				StockChatBufferLine("请专心表演", FONT_PAL_RED);
			else if (pc.iTheaterMode & 0x00000001)
				StockChatBufferLine("请专心看表演", FONT_PAL_RED);
			else
#endif
			{
				MenuToggleFlag ^= JOY_CTRL_I;	// CTRL + I ??
				// ????????????
				for (i = 0; i < MENU_ITEM_0; i++) itemWndFontNo[i] = -2;
				for (i = 0; i < MENU_ITEM_0; i++) itemWndBtnFlag[i] = 0;
				itemWndNo = 0;
				mouse.itemNo = -1;
				itemWndDropGold = 0;

				// ??????????
				if (BattleResultWndFlag >= 1) DeathMenuAction();
				// ??????????
				DeathMenuAction2();
				// ????????			
				saveUserSetting();
				// ????????
				if (MenuToggleFlag & JOY_CTRL_I){
					MenuToggleFlag &= ~JOY_CTRL_M;	// ???????????
					MenuToggleFlag &= ~JOY_CTRL_T;
					MenuToggleFlag &= ~JOY_B;
					MenuToggleFlag |= JOY_CTRL_I;
					// ???????????
					BattleResultWndFlag = FALSE;
					// ?????????
					MapWmdFlagBak = FALSE;
					// ????????
					play_se(202, 320, 240);

				}
				else{
					// ????????
					play_se(203, 320, 240);
				}
			}
		}
	}

	// ????????
#ifdef __AI
	if (!AI_State&&!自动登陆窗口 && ((joy_trg[0] & JOY_CTRL_M && GetImeString() == NULL)
		|| (TaskBarFlag == TRUE && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[0])
		|| (MapWmdFlagBak == TRUE && BattleResultWndFlag == FALSE && EncountFlag == FALSE && ProcNo == PROC_GAME && SubProcNo == 3				

		)
		)){
#else
	if( ( joy_trg[ 0 ] & JOY_CTRL_M && GetImeString() == NULL ) 
		|| ( TaskBarFlag == TRUE && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[ 0 ] )
		|| ( MapWmdFlagBak == TRUE && BattleResultWndFlag == FALSE && EncountFlag == FALSE && ProcNo == PROC_GAME && SubProcNo == 3 )
		){  
#endif
		// ???
		if (ProcNo == PROC_BATTLE){
			// ???
			play_se(220, 320, 240);
		}
		else{

			MenuToggleFlag ^= JOY_CTRL_M;	// CTRL + M ??
			// ???
			for (i = 0; i < MENU_PET_0; i++) petWndFontNo[i] = -2;
			mapWndNo = 0;		// ?????????

			// ??????????
			if (BattleResultWndFlag >= 1) DeathMenuAction();
			DeathMenuAction2();
			// ????????
			// ????????
			if (MenuToggleFlag & JOY_CTRL_M){
				MenuToggleFlag &= ~JOY_CTRL_I;	// ????????????
				MenuToggleFlag &= ~JOY_CTRL_T;
				MenuToggleFlag &= ~JOY_B;
				MenuToggleFlag |= JOY_CTRL_M;
				// ???????????
				BattleResultWndFlag = FALSE;
				// ?????????
				MapWmdFlagBak = FALSE;
				// ????????
				play_se(202, 320, 240);
			}
			else{
				// ????????	
				play_se(203, 320, 240);
			}
		}
	}

	// ????????
#ifdef __AI
	if (!AI_State&&!自动登陆窗口 && ((joy_trg[0] & JOY_CTRL_E && GetImeString() == NULL)
		|| (TaskBarFlag == TRUE && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[5]))
			
		){
#else
	if( ( joy_trg[ 0 ] & JOY_CTRL_E && GetImeString() == NULL )
		|| ( TaskBarFlag == TRUE && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[ 5 ] ) ){ 
#endif
#ifdef _BATTLESKILL
		extern int wonflag ;
		if ( wonflag == 1 ) 
			play_se( 220, 320, 240 );
		else {
#endif
			MenuToggleFlag ^= JOY_CTRL_E;	// CTRL + E ??
			// ???
			for (i = 0; i < MENU_MAIL_0; i++) mailWndFontNo[i] = -2;
			mailWndNo = MAIL_WND_VIEW;	// ?????????
			mailViewWndPageNo = 0;		// ???????????????
			mailItemNo = -1;			// ??????????????
			InitMailSendFlag();			// ????????
			// ??????????
			DeathMenuAction();
			// ????????
			saveUserSetting();

			// ????????
			if (MenuToggleFlag & JOY_CTRL_E){
				MenuToggleFlag &= JOY_CTRL_I | JOY_CTRL_M;	// ??????????????????
				MenuToggleFlag |= JOY_CTRL_E;
				// ???????????
				BattleResultWndFlag = FALSE;
				// ????????
				play_se(202, 320, 240);
			}
			else{
				// ????????
				play_se(203, 320, 240);
			}
#ifdef _BATTLESKILL
		}
#endif
		//}
	}

	// ?????????
#ifdef __AI
	if (!AI_State&&!自动登陆窗口 && ((joy_trg[0] & JOY_CTRL_A && GetImeString() == NULL)
		|| (TaskBarFlag == TRUE && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[6]))
	

		){
#else
	if( ( joy_trg[ 0 ] & JOY_CTRL_A && GetImeString() == NULL )
		|| ( TaskBarFlag == TRUE && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[ 6 ] ) ){
#endif
#ifdef _BATTLESKILL
		extern int wonflag ;
		if ( wonflag == 1 ) 
			play_se( 220, 320, 240 );
		else {
#endif
			MenuToggleFlag ^= JOY_CTRL_A;	// CTRL + A ??
			// ???
			for (i = 0; i < MENU_ALBUM_0; i++) albumWndFontNo[i] = -2;
			mapWndNo = 0;		// ?????????

			// ??????????
			DeathMenuAction();
			// ????????
			saveUserSetting();

			// ????????
			if (MenuToggleFlag & JOY_CTRL_A){
				MenuToggleFlag &= JOY_CTRL_I | JOY_CTRL_M;	// ??????????????????
				MenuToggleFlag |= JOY_CTRL_A;
				// ???????????
				BattleResultWndFlag = FALSE;
				// ????????
				play_se(202, 320, 240);
			}
			else{
				// ????????
				play_se(203, 320, 240);
			}
			//}
#ifdef _BATTLESKILL
		}
#endif
	}

#ifdef _SA_LIAOTIAN_
	static int 聊天状态1=0;
	static int 聊天状态2=0;
	static int 聊天状态3=0;
	char 聊天提示[128];
	StockDispBuffer(785, 505, DISP_PRIO_IME3, 55260+聊天状态1, 1);
	if( MakeHitBox(785-10, 505-10,785+10,505+10, DISP_PRIO_IME4 ) == TRUE ){
		sprintf(聊天提示,"聊天信息上拉,当前聊天信息位置%d！",NowChatLine);
		ShowBottomLineString(FONT_PAL_WHITE, 聊天提示);	
		if(mouse.state & MOUSE_LEFT_CRICK){
			聊天状态1 = 1;
			if(mouse.onceState & MOUSE_LEFT_CRICK){
				if(NowChatLine > NowMaxChatLine){
					NowChatLine--;
					if (NowChatLine <= 0){
						NowChatLine = 1;
					}
				}
				play_se(203, 320, 240);
			}
		}else 聊天状态1=0;
	}else 聊天状态1=0;
	StockDispBuffer(785, 530, DISP_PRIO_IME3, 55262+聊天状态2, 1);
	if( MakeHitBox(785-10, 530-10,785+10,530+10, DISP_PRIO_IME4 ) == TRUE ){
		sprintf(聊天提示,"聊天信息下拉,当前聊天信息位置%d！",NowChatLine);
		ShowBottomLineString(FONT_PAL_WHITE, 聊天提示);	
		if( (mouse.state & MOUSE_LEFT_CRICK)){
			聊天状态2 = 1;
			if(mouse.onceState & MOUSE_LEFT_CRICK){
				if(*ChatBuffer[ NowChatLine].buffer){
					NowChatLine++;
					if (NowChatLine > MAX_CHAT_LINE)
						NowChatLine = MAX_CHAT_LINE;
				}
				play_se(203, 320, 240);
			}
		}else 聊天状态2=0;
	}else 聊天状态2=0;
	StockDispBuffer(785, 555, DISP_PRIO_IME3, 55264+聊天状态3, 1);
	if( MakeHitBox(785-10, 555-10,785+10,555+10, DISP_PRIO_IME4 ) == TRUE ){
		sprintf(聊天提示,"聊天信息正常显示,当前聊天信息位置%d！",NowChatLine);
		ShowBottomLineString(FONT_PAL_WHITE, 聊天提示);	
		if( (mouse.state & MOUSE_LEFT_CRICK)){
			聊天状态3 = 1;
			if(mouse.onceState & MOUSE_LEFT_CRICK){
				extern int NowChatLine_Bak;
				NowChatLine=NowChatLine_Bak;
				play_se(203, 320, 240);
			}
		}else 聊天状态3=0;
	}else 聊天状态3=0;
#endif
#ifdef _EFFECT_MAP_
	if(ProcNo==PROC_GAME){
		StockDispBuffer(400, 288, DISP_PRIO_MENU-1, 55256, 0);
	}
#endif
#ifdef _RED_MEMOY_
	RedMemoyCall();

#endif
	StockDispBuffer(TASK_BAR_X, TASK_BAR_Y, DISP_PRIO_MENU, CG_TASK_BAR_BACK, 1);
	if (mouse.nowPoint.y >= 456 + DISPLACEMENT_Y){
		if (taskBarY > TASK_BAR_Y) taskBarY--;
		TaskBarFlag = TRUE;
#ifdef __ONLINEGM
		if(OnlineGmFlag==TRUE){
			TaskBarFlag = FALSE;
		}
#endif 
	}
	else{
		if (TaskBarFlag == TRUE){
			if (taskBarY < TASK_BAR_Y + 24) taskBarY++;
			if (taskBarY == TASK_BAR_Y + 24){
				for (i = 0; i < TASK_BAR; i++) albumWndFontNo[i] = -2;
				TaskBarFlag = FALSE;
			}
		}
	}

	if (TaskBarFlag == TRUE){
		x = taskBarX, y = taskBarY;

		taskBarFontNo[0] = StockDispBuffer(x, y, DISP_PRIO_IME3, CG_TASK_BAR_MAP_UP + ((MenuToggleFlag & JOY_CTRL_M) ? 1 : 0), 2);
		taskBarFontNo[1] = StockDispBuffer(x + 24, y, DISP_PRIO_IME3, CG_TASK_BAR_STATUS_UP + ((MenuToggleFlag & JOY_CTRL_S) ? 1 : 0), 2);
		taskBarFontNo[2] = StockDispBuffer(x + 48, y, DISP_PRIO_IME3, CG_TASK_BAR_PET_UP + ((MenuToggleFlag & JOY_CTRL_P) ? 1 : 0), 2);
		taskBarFontNo[4] = StockDispBuffer(x + 72, y, DISP_PRIO_IME3, CG_TASK_BAR_ITEM_UP + ((MenuToggleFlag & JOY_CTRL_I) ? 1 : 0), 2);
		taskBarFontNo[5] = StockDispBuffer(x + 95, y, DISP_PRIO_IME3, CG_TASK_BAR_MAIL_UP + ((MenuToggleFlag & JOY_CTRL_E) ? 1 : 0), 2);
		taskBarFontNo[6] = StockDispBuffer(x + 118, y, DISP_PRIO_IME3, CG_TASK_BAR_ALBUM_UP + ((MenuToggleFlag & JOY_CTRL_A) ? 1 : 0), 2);
		taskBarFontNo[3] = StockDispBuffer(x + 140, y, DISP_PRIO_IME3, CG_TASK_BAR_SYSTEM_UP + ((MenuToggleFlag & JOY_ESC) ? 1 : 0), 2);
	}

	if (MenuToggleFlag & JOY_ESC
	
		){
		int w = 7;
		int h = 8;
		// ????????
		switch (systemWndNo){

		case 0:	// ???? ??????

			if (pActMenuWnd == NULL){
#ifdef _NEW_SYSTEM_MENU
				x = (lpDraw->xSize - w * 64) / 2;
				y = (lpDraw->ySize - h * 48) / 2;

				pActMenuWnd = MakeWindowDisp(x, y, w, h, CG_WND_TITLE_SYSTEM, 1, FALSE);
#else
				pActMenuWnd = MakeWindowDisp( 4, 4, 3, 7, CG_WND_TITLE_SYSTEM, 1 );
#endif
				for (i = 0; i < MENU_SYSTEM_0; i++) systemWndFontNo[i] = -2;
				break;
			}
			else{
				if (pActMenuWnd->hp <= 0) break;
			}

			// ????????
			if (mouse.onceState & MOUSE_LEFT_CRICK){
				// ???????
				if (HitFontNo == systemWndFontNo[0]){
					// ??????
#ifdef __PHONEMESSAGE
					CleanSMS();
#endif
					DeathAction(pActMenuWnd);
					pActMenuWnd = NULL;
					systemWndNo = 1;
					// ????????
					play_se(202, 320, 240);
					break;
				}
				// ??????
				if (HitFontNo == systemWndFontNo[1]){
					// ??????
					DeathAction(pActMenuWnd);
					pActMenuWnd = NULL;
					systemWndNo = 2;
					// ????????
					play_se(202, 320, 240);
					break;
				}
				// ?????
				if (HitFontNo == systemWndFontNo[4]){
					// ??????
					DeathAction(pActMenuWnd);
					pActMenuWnd = NULL;
					systemWndNo = 4;
					// ????????
					play_se(202, 320, 240);
					break;
				}
				// ?????
				if (HitFontNo == systemWndFontNo[3]){
					// ??????
					DeathAction(pActMenuWnd);
					pActMenuWnd = NULL;
					systemWndNo = 3;
					// ????????
					play_se(202, 320, 240);
					break;
				}
				// ??????
				if (HitFontNo == systemWndFontNo[5]){
					// ??????
					DeathAction(pActMenuWnd);
					pActMenuWnd = NULL;
					systemWndNo = 6;
					// ????????
					play_se(202, 320, 240);
					break;
				}
				//原地登出
				if (HitFontNo == systemWndFontNo[6]){
					// ??????
#ifdef __PHONEMESSAGE
					CleanSMS();
#endif
					DeathAction(pActMenuWnd);
					pActMenuWnd = NULL;
					systemWndNo = 7;
					// ????????
					play_se(202, 320, 240);

					break;
				}
				// ???
				if (HitFontNo == systemWndFontNo[2]){
					// ??????
					DeathAction(pActMenuWnd);
					pActMenuWnd = NULL;
					// ???????????
					MenuToggleFlag ^= JOY_ESC;
					// ????????
					play_se(203, 320, 240);
					break;
				}
#ifdef __AI
				//自动战斗设定
				if (HitFontNo == systemWndFontNo[7]){
					// ??????
					extern int AI_State;
					AI_State = 1;
					extern short actBtn;
					actBtn = 1;
					closeEtcSwitch();
					closeJoinChannelWN();
					MenuToggleFlag = 0;
					DeathMenuAction2();
					DeathMenuAction();
					// ????????
					play_se(202, 320, 240);
					break;
				}
				if (HitFontNo == systemWndFontNo[24]){
					if (AI == AI_SELECT){
						AI = AI_NONE;
						StockChatBufferLine("关闭ＡＩ模式！", FONT_PAL_RED);
						pc.etcFlag &= (~PC_AI_MOD);
						lssproto_FS_send(sockfd, pc.etcFlag);
					}
					else {
						AI = AI_SELECT;
						StockChatBufferLine("开启ＡＩ模式！", FONT_PAL_RED);
						pc.etcFlag |= PC_AI_MOD;
						lssproto_FS_send(sockfd, pc.etcFlag);
					}
					play_se(202, 320, 240);
					break;
				}
#endif
				static BOOL stopBGMClicked = FALSE;
				if (HitFontNo == systemWndFontNo[8]){
					if (MuteFlag){
						StockChatBufferLine("开启声音！", FONT_PAL_RED);
						MuteFlag = FALSE;
						if (stopBGMClicked)
						{
							play_bgm(map_bgm_no);
						}
					}
					else{
						stopBGMClicked = TRUE;
						StockChatBufferLine("关闭声音！", FONT_PAL_RED);
						play_se(202, 320, 240);
						stop_bgm();
						MuteFlag = TRUE;				
					}		
					saveUserSetting();
					break;
				}
#ifdef _AIDENGLU_
				if (HitFontNo == systemWndFontNo[9]){
					DeathAction(pActMenuWnd);
					pActMenuWnd = NULL;
					play_se(202, 320, 240);
					closeEtcSwitch();
					closeJoinChannelWN();
					MenuToggleFlag = 0;
					DeathMenuAction2();
					DeathMenuAction();
					自动登陆窗口 = TRUE;
					extern short actBtn;
					actBtn = 1;
					break;
				}
#endif
				if (HitFontNo == systemWndFontNo[10]){
					DeathAction(pActMenuWnd);
					pActMenuWnd = NULL;
					MenuToggleFlag ^= JOY_ESC;
#ifdef _CANCEL_FANTI
					StockChatBufferLine("功能开发中！", FONT_PAL_RED);
					play_se(202, 320, 240);
#else
					if (繁体开关)
					{
						繁体开关 = FALSE;
						StockChatBufferLine("切换简体！", FONT_PAL_RED);
					}
					else{
						繁体开关 = TRUE;
						StockChatBufferLine("切换繁体！", FONT_PAL_RED);
					}
					play_se(202, 320, 240);
					break;
#endif

				}
				if (HitFontNo == systemWndFontNo[15]){
					DeathAction(pActMenuWnd);
					pActMenuWnd = NULL;
					MenuToggleFlag ^= JOY_ESC;
					右键攻击 = !右键攻击;
					if (右键攻击)
						StockChatBufferLine("开启右键攻击！", FONT_PAL_RED);
					else
						StockChatBufferLine("关闭右键攻击！", FONT_PAL_RED);

					play_se(202, 320, 240);
					break;
				}

				if (HitFontNo == systemWndFontNo[23]){
					play_se(202, 320, 240);
					经验开关 = !经验开关;
					if (经验开关)
						StockChatBufferLine("开启经验显示！", FONT_PAL_RED);
					else
						StockChatBufferLine("关闭经验显示！", FONT_PAL_RED);

					break;
				}
				if (HitFontNo == systemWndFontNo[22]){
					play_se(202, 320, 240);
					人物屏蔽开关 = !人物屏蔽开关;
					if (人物屏蔽开关)
						StockChatBufferLine("屏蔽周边人物！", FONT_PAL_RED);
					else{
						//extern void 设置静止的动作为站立( void );
						//设置静止的动作为站立();
						StockChatBufferLine("显示周边人物！", FONT_PAL_RED);
					}


					break;
				}
#ifdef _RENWU_
				if (HitFontNo == systemWndFontNo[21]){
					closeEtcSwitch();
					closeJoinChannelWN();
					if (CheckMenuFlag())
						InitMenu2();
					extern short HelpProcNo;
					HelpProcNo = 0;
					任务查询开关=TRUE;

					break;
				}
#endif
#ifdef _NEW_SYSTEM_MENU
				for (int i = 11; i < MENU_SYSTEM_0; i++){
					if (i != 24 && i != 15 && i != 23 && i != 22
#ifdef _RENWU_
						&& i != 21
#endif
						)
					if (HitFontNo == systemWndFontNo[i]){
#ifdef _RELUA_
						extern void ReLoadStoneAgeLUA(char *filename);
						if (i == 16){
							extern int windowTypeWN;
							extern short wnCloseFlag;
							extern int cloasewindows;
							extern SA_WINDOWS windows;
							DeathAction(windows.ptActMenuWin);
							windows.ptActMenuWin = 0;
							wnCloseFlag = 0;
							windowTypeWN = -1;
							wnCloseFlag = 1;
							cloasewindows = 1;
							ReLoadStoneAgeLUA("");
						}
#endif
						DeathAction(pActMenuWnd);
						pActMenuWnd = NULL;
						MenuToggleFlag ^= JOY_ESC;
						lssproto_SaMenu_send(sockfd, i);
						play_se(202, 320, 240);
						break;
					}
				}

#endif
				break;
			}
			// ?
			x = pActMenuWnd->x + 28;

			// ?????????
			y = pActMenuWnd->y + 54;
#ifdef _CHAR_NEWLOGOUT
			systemWndFontNo[0] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "    回记录点    ", 2);	y += 34;//y += 40;
#else
			systemWndFontNo[ 0 ] = StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, 	"    登    出    ", 2 );	y += 34;//y += 40;
#endif
			systemWndFontNo[6] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "    原地登出    ", 2);	y += 34;//y += 40;
			systemWndFontNo[1] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "    聊天设定    ", 2);	y += 34;//y += 40;
			systemWndFontNo[4] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "    背景音乐  ", 2);	y += 34;//y += 40;
			systemWndFontNo[3] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "    音效设定    ", 2);	y += 34;//y += 40;
			systemWndFontNo[5] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "    滑鼠设定    ", 2);	y += 34;//y += 52;
			systemWndFontNo[7] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "    战斗设定  ", 2);	y += 34;//y += 40;
			if (MuteFlag){
				systemWndFontNo[8] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "    开启声音  ", 2);	y += 34;//y += 40;
			}
			else{	
				systemWndFontNo[8] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "    关闭声音  ", 2);	y += 34;//y += 40;
			}
#ifdef _NEW_SYSTEM_MENU
			y = pActMenuWnd->y + 54;
			systemWndFontNo[17] = StockFontBuffer(x + 140, y, FONT_PRIO_FRONT, 0, "    官方主页    ", 2);	y += 34;//y += 40;
			systemWndFontNo[18] = StockFontBuffer(x + 140, y, FONT_PRIO_FRONT, 0, "    我的邮箱    ", 2);	y += 34;//y += 40;

			systemWndFontNo[11] = StockFontBuffer(x + 140, y, FONT_PRIO_FRONT, 0, "    原地遇敌    ", 2);	y += 34;//y += 40;
			systemWndFontNo[12] = StockFontBuffer(x + 140, y, FONT_PRIO_FRONT, 0, "    取消原地    ", 2);	y += 34;//y += 40;
			systemWndFontNo[13] = StockFontBuffer(x + 140, y, FONT_PRIO_FRONT, 0, "    支票制作    ", 2);	y += 34;//y += 40;
			systemWndFontNo[14] = StockFontBuffer(x + 140, y, FONT_PRIO_FRONT, 0, "    任务查询    ", 2);	y += 34;//y += 40;
			systemWndFontNo[15] = StockFontBuffer(x + 140, y, FONT_PRIO_FRONT, 0, "    右键攻击    ", 2);	y += 34;//y += 40;
			systemWndFontNo[16] = StockFontBuffer(x + 140, y, FONT_PRIO_FRONT, 0, "    个人信息    ", 2);	y += 34;//y += 40;
			y = pActMenuWnd->y + 54;
			systemWndFontNo[19] = StockFontBuffer(x + 280, y, FONT_PRIO_FRONT, 0, "    在线充值    ", 2);	y += 34;//y += 40;
			systemWndFontNo[20] = StockFontBuffer(x + 280, y, FONT_PRIO_FRONT, 0, "    卡密使用    ", 2);	y += 34;//y += 40;
			systemWndFontNo[21] = StockFontBuffer(x + 280, y, FONT_PRIO_FRONT, 0, "    快捷传送    ", 2);	y += 34;//y += 40;
			systemWndFontNo[9] = StockFontBuffer(x + 280, y, FONT_PRIO_FRONT, 0, "    掉线重连    ", 2);	y += 34;//y += 40;
			if (繁体开关){
				systemWndFontNo[10] = StockFontBuffer(x + 280, y, FONT_PRIO_FRONT, 0, "    切换简体    ", 2);	y += 34;//y += 40;
			}
			else{
				systemWndFontNo[10] = StockFontBuffer(x + 280, y, FONT_PRIO_FRONT, 0, "    切换繁体    ", 2);	y += 34;//y += 40;
			}
			if (人物屏蔽开关){
				systemWndFontNo[22] = StockFontBuffer(x + 280, y, FONT_PRIO_FRONT, 0, "    人物显示    ", 2);	y += 34;//y += 40;
			}
			else{
				systemWndFontNo[22] = StockFontBuffer(x + 280, y, FONT_PRIO_FRONT, 0, "    人物屏蔽    ", 2);	y += 34;//y += 40;
			}
			if (经验开关){
				systemWndFontNo[23] = StockFontBuffer(x + 280, y, FONT_PRIO_FRONT, 0, "    经验关闭  ", 2);	y += 34;//y += 40;
			}
			else{
				systemWndFontNo[23] = StockFontBuffer(x + 280, y, FONT_PRIO_FRONT, 0, "    经验显示  ", 2);	y += 34;//y += 40;
			}
			if (AI == AI_SELECT){
				systemWndFontNo[24] = StockFontBuffer(x + 280, y, FONT_PRIO_FRONT, 0, "    关闭战斗    ", 2);	y += 34;//y += 40;
			}
			else{
				systemWndFontNo[24] = StockFontBuffer(x + 280, y, FONT_PRIO_FRONT, 0, "    自动战斗    ", 2);	y += 34;//y += 40;
			}

			systemWndFontNo[25] = StockFontBuffer(x + 0  , y, FONT_PRIO_FRONT, 0, "    战力详情    ", 2);	//y += 40;
			systemWndFontNo[26] = StockFontBuffer(x + 140, y, FONT_PRIO_FRONT, 0, "    捕鱼达人    ", 2);	//y += 40;
			systemWndFontNo[27] = StockFontBuffer(x + 280, y, FONT_PRIO_FRONT, 0, "    成就排行    ", 2);	y += 34;//y += 40;

			systemWndFontNo[2] = StockFontBuffer(x + 140, y, FONT_PRIO_FRONT, FONT_PAL_AQUA, "    关    闭    ", 2);
#else
			systemWndFontNo[ 2 ] = StockFontBuffer( x, y, FONT_PRIO_FRONT, FONT_PAL_AQUA, 	"    关    闭    ", 2 );
#endif
			break;

		case 1:	//
			if (pActMenuWnd == NULL){
				x = (lpDraw->xSize - 3 * 64) / 2;
				y = (lpDraw->ySize - 3 * 48) / 2;
				pActMenuWnd = MakeWindowDisp(x, y, 3, 3, CG_WND_TITLE_LOGOUT, 1, FALSE);
				for (i = 0; i < MENU_SYSTEM_0; i++) systemWndFontNo[i] = -2;
				break;
			}
			else{
				if (pActMenuWnd->hp <= 0) break;
			}

			// ????????
			if (mouse.onceState & MOUSE_LEFT_CRICK){
				// ????
				if (HitFontNo == systemWndFontNo[0]){
					// ??????
					DeathAction(pActMenuWnd);
					pActMenuWnd = NULL;
#ifdef _CHAR_NEWLOGOUT
					systemWndNo = 0;
					lssproto_CharLogout_send(sockfd, 1);
#else
					GameState = GAME_LOGIN;
					ChangeProc2( PROC_CHAR_LOGOUT );
#endif
					play_se(206, 320, 240);
					break;
				}
				// ?????
				if (HitFontNo == systemWndFontNo[1]){
					// ??????
					DeathAction(pActMenuWnd);
					pActMenuWnd = NULL;
					// ??????
					systemWndNo = 0;
					play_se(203, 320, 240);
					break;
				}
			}

			// ?
			x = pActMenuWnd->x + 38;
			y = pActMenuWnd->y + 56;

#ifdef _NEWFONT_
			systemWndFontNo[0] = StockFontBuffer(x+20, y, FONT_PRIO_FRONT, 0, "     确  定     ", 2);	y += 40;
			systemWndFontNo[1] = StockFontBuffer(x+20, y, FONT_PRIO_FRONT, 0, "     不  要     ", 2);	y += 40;
#else
			systemWndFontNo[0] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "     确  定     ", 2);	y += 40;
			systemWndFontNo[1] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "     不  要     ", 2);	y += 40;
#endif
			break;

		case 2:	// ????? ??????

			// ?????????
			if (pActMenuWnd == NULL){
				// ?????????
#ifndef _TALK_WINDOW
				x = (lpDraw->xSize - 4 * 64) / 2;
				y = (lpDraw->ySize - 8 * 48) / 2;
				pActMenuWnd = MakeWindowDisp(x, y, 4, 8, CG_WND_TITLE_CHAT, 1, FALSE);
#else
				x = (lpDraw->xSize - 4 * 64) / 2;
				y = (lpDraw->ySize - 9 * 48) / 2;
				pActMenuWnd = MakeWindowDisp( x, y, 4, 9, CG_WND_TITLE_CHAT, 1, FALSE );
#endif
				for (i = 0; i < MENU_SYSTEM_0; i++) systemWndFontNo[i] = -2;
				break;
			}
			else{
				if (pActMenuWnd->hp <= 0) break;
			}

			// ?????????????
			if (mouse.autoState & MOUSE_LEFT_CRICK){
				// ???????  ????
				if (HitFontNo == systemWndFontNo[0]){
					NowMaxChatLine++;
					if (NowMaxChatLine > DEF_CHAT_LINE){
						NowMaxChatLine = DEF_CHAT_LINE;
						play_se(220, 320, 240);
					}
					else{
						// ?????
						play_se(217, 320, 240);
					}
				}
				// ???????  ????
				if (HitFontNo == systemWndFontNo[1]){
					// ???
					NowMaxChatLine--;
					// ????????
					if (NowMaxChatLine <= -1){
						NowMaxChatLine = 0;
						// ???
						play_se(220, 320, 240);
					}
					else{
						// ?????
						play_se(217, 320, 240);
					}
				}
				// ????????
				if (HitFontNo == systemWndFontNo[2]){
					// ?
					MyChatBuffer.color++;
					// ????????
					if (MyChatBuffer.color >= FONT_PAL_NUM) MyChatBuffer.color = 0;
					// ?????
					play_se(217, 320, 240);
				}

				// ??
				if (HitFontNo == systemWndFontNo[4]){
					// ??????
					DeathAction(pActMenuWnd);
					pActMenuWnd = NULL;
					systemWndNo = 5;
					// ????????
					play_se(202, 320, 240);
					break;
				}
				// ???????  ????
				if (HitFontNo == systemWndFontNo[5]){
					// ???
					NowMaxVoice++;
					// ????????
					if (NowMaxVoice > MAX_VOICE){
						NowMaxVoice = MAX_VOICE;
						// ???
						play_se(220, 320, 240);
					}
					else{
						// ?????
						play_se(217, 320, 240);
					}
				}
				// ???????  ????
				if (HitFontNo == systemWndFontNo[6]){
					// ???
					NowMaxVoice--;
					// ????????
					if (NowMaxVoice <= 0){
						NowMaxVoice = 1;
						// ???
						play_se(220, 320, 240);
					}
					else{
						// ?????
						play_se(217, 320, 240);
					}
				}
#ifdef _TALK_WINDOW
				if(HitFontNo == systemWndFontNo[7]){
					g_bTalkWindow = !g_bTalkWindow;
					if(!WindowMode && g_bTalkWindow){
						StockChatBufferLine("全荧幕模式下无法使用本功能",FONT_PAL_RED);
						g_bTalkWindow = FALSE;
					}
					play_se(217,320,240);
					if(g_bTalkWindow) TalkWindow.Create();
					else TalkWindow.Visible(FALSE);
				}
#endif
			}
			// ????????
			if (mouse.onceState & MOUSE_LEFT_CRICK){
				// ???
				if (HitFontNo == systemWndFontNo[3]){
					// ??????
					DeathAction(pActMenuWnd);
					pActMenuWnd = NULL;
					// ??????????
					systemWndNo = 0;
					// ????????
					play_se(203, 320, 240);
					// ????????
					saveUserSetting();
					break;
				}
			}
			// ?
			x = pActMenuWnd->x + 56;
			y = pActMenuWnd->y + 56;
			sprintf_s(moji, "◆目前显示的行数%3d 行◆", NowMaxChatLine);
			StockFontBuffer(x - 16, y, FONT_PRIO_FRONT, FONT_PAL_YELLOW, moji, 0);				y += 32;
			systemWndFontNo[0] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "      增    加      ", 2);	y += 32;
			systemWndFontNo[1] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "      减    少      ", 2);	y += 32;
			systemWndFontNo[2] = StockFontBuffer(x, y, FONT_PRIO_FRONT, MyChatBuffer.color, "    改变文字颜色    ", 2);	y += 32;
			systemWndFontNo[4] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "      记录文字      ", 2);	y += 44;

			sprintf_s(moji, "◆目前的音量%3d ◆", NowMaxVoice);
			StockFontBuffer(x - 16, y, FONT_PRIO_FRONT, FONT_PAL_YELLOW, moji, 0);				y += 32;
			systemWndFontNo[5] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "     增    加     ", 2);	y += 32;
			systemWndFontNo[6] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "     减    少     ", 2);	y += 44;

#ifdef _TALK_WINDOW
			StockFontBuffer(x - 16,y,FONT_PRIO_FRONT,FONT_PAL_YELLOW,"◆聊天视窗设定◆",0);y += 32;
			systemWndFontNo[7] = StockFontBuffer(x + 20,y,FONT_PRIO_FRONT,0,g_bTalkWindow ? "关闭聊天视窗":"打开聊天视窗",2);y += 32;
#endif																																						

			systemWndFontNo[3] = StockFontBuffer(x, y, FONT_PRIO_FRONT, FONT_PAL_AQUA, "     回上一页     ", 2);	y += 40;

			break;

		case 3:	// ???? ??????

			// ?????????
			if (pActMenuWnd == NULL){
				// ?????????
				x = (lpDraw->xSize - 4 * 64) / 2;
				y = (lpDraw->ySize - 6 * 48) / 2;
				pActMenuWnd = MakeWindowDisp(x, y, 4, 6, CG_WND_TITLE_SE, 1, FALSE);
				for (i = 0; i < MENU_SYSTEM_0; i++) systemWndFontNo[i] = -2;
				break;
			}
			else{
				if (pActMenuWnd->hp <= 0) break;
			}

			// ?????????????
			if (mouse.autoState & MOUSE_LEFT_CRICK){
				// ???????
				if (HitFontNo == systemWndFontNo[0]){
					// ???
					t_music_se_volume++;
					// ????????
					if (t_music_se_volume > 15){
						t_music_se_volume = 15;
						// ???
						play_se(220, 320, 240);
					}
					else{
						// ?????
						play_se(217, 320, 240);
					}
				}
				// ???????
				if (HitFontNo == systemWndFontNo[1]){
					t_music_se_volume--;
					if (t_music_se_volume <= 0){
						t_music_se_volume = 1;
						play_se(220, 320, 240);
					}
					else{
						play_se(217, 320, 240);
					}
				}
				// ??????????????
				if (HitFontNo == systemWndFontNo[2]){
					// ?????????
					stereo_flg = !stereo_flg;
					// ?????
					play_se(217, 320, 240);
				}
			}
			// ????????
			if (mouse.onceState & MOUSE_LEFT_CRICK){
				// ???
				if (HitFontNo == systemWndFontNo[3]){
					// ??????
					DeathAction(pActMenuWnd);
					pActMenuWnd = NULL;
					// ??????????
					systemWndNo = 0;
					// ????????
					play_se(203, 320, 240);
					// ????????
					saveUserSetting();
					break;
				}
			}
			// ?
			x = pActMenuWnd->x + 56;
			y = pActMenuWnd->y + 64;
			sprintf_s(moji, "◆  目前的音量%3d   ◆", t_music_se_volume);
			StockFontBuffer(x - 8, y, FONT_PRIO_FRONT, FONT_PAL_YELLOW, moji, 0);				y += 40;
			systemWndFontNo[0] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "     增     加     ", 2);	y += 40;
			systemWndFontNo[1] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "     减     少     ", 2);	y += 40;
			systemWndFontNo[2] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, monoStereoStr[stereo_flg], 2);	y += 52;
			systemWndFontNo[3] = StockFontBuffer(x, y, FONT_PRIO_FRONT, FONT_PAL_AQUA, "      回上一页      ", 2);	y += 40;

			break;

		case 4:	// ???? ??????
			// ?????????
			if (pActMenuWnd == NULL){
				// ?????????
				x = (lpDraw->xSize - 4 * 64) / 2;
				y = (lpDraw->ySize - 8 * 48) / 2;
				pActMenuWnd = MakeWindowDisp(x, y, 4, 8, CG_WND_TITLE_BGM, 1, FALSE);
				for (i = 0; i < MENU_SYSTEM_0; i++) systemWndFontNo[i] = -2;
				break;
			}
			else{
				if (pActMenuWnd->hp <= 0) break;
			}

			// ?????????????
			if (mouse.autoState & MOUSE_LEFT_CRICK){
				// ???????
				if (HitFontNo == systemWndFontNo[0]){
					// ???
					t_music_bgm_volume++;
					// ????????
					if (t_music_bgm_volume > 15){
						t_music_bgm_volume = 15;
						// ???
						play_se(220, 320, 240);
					}
					else{
						play_se(217, 320, 240);	// ?????
						bgm_volume_change();		// ???
					}
				}
				// ???????
				if (HitFontNo == systemWndFontNo[1]){
					// ???
					t_music_bgm_volume--;
					// ????????
					if (t_music_bgm_volume <= 0){
						t_music_bgm_volume = 1;
						// ???
						play_se(220, 320, 240);
					}
					else{
						play_se(217, 320, 240);	// ?????
						bgm_volume_change();		// ???
					}
				}
				// ????????
				if (HitFontNo == systemWndFontNo[3]){
					// ??????
					t_music_bgm_pitch[t_music_bgm_no]++;
					set_gbm_pitch();
					// ????????
					if (t_music_bgm_pitch[t_music_bgm_no] > 8){
						t_music_bgm_pitch[t_music_bgm_no] = 8;
						// ???
						play_se(220, 320, 240);
					}
					else{
						play_se(217, 320, 240);	// ?????
						set_gbm_pitch();			// ????
					}
				}
				// ???????
				if (HitFontNo == systemWndFontNo[4]){
					// ??????
					t_music_bgm_pitch[t_music_bgm_no]--;
					// ????????
					if (t_music_bgm_pitch[t_music_bgm_no] < -8){
						t_music_bgm_pitch[t_music_bgm_no] = -8;
						// ???
						play_se(220, 320, 240);
					}
					else{
						play_se(217, 320, 240);	// ?????
						set_gbm_pitch();			// ????
					}
				}
			}
			// ????????
			if (mouse.onceState & MOUSE_LEFT_CRICK){
				// ???
				if (HitFontNo == systemWndFontNo[2]){
					// ??????
					DeathAction(pActMenuWnd);
					pActMenuWnd = NULL;
					// ??????????
					systemWndNo = 0;
					// ????????
					play_se(203, 320, 240);
					// ????????
					saveUserSetting();
					break;
				}
			}
			// ?
			x = pActMenuWnd->x + 56;
			y = pActMenuWnd->y + 64;
			sprintf_s(moji, "◆  目前的音量 %3d  ◆", t_music_bgm_volume);
			StockFontBuffer(x - 8, y, FONT_PRIO_FRONT, FONT_PAL_YELLOW, moji, 0);				y += 40;
			systemWndFontNo[0] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "     增      加     ", 2);	y += 40;
			systemWndFontNo[1] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "     减      少     ", 2);	y += 48;
			if (t_music_bgm_pitch[t_music_bgm_no] == 0){
				sprintf_s(moji, "◆ 目前的节奏   0 ◆");
			}
			else
				sprintf_s(moji, "◆ 目前的节奏 %+3d ◆", t_music_bgm_pitch[t_music_bgm_no]);
			StockFontBuffer(x - 8, y, FONT_PRIO_FRONT, FONT_PAL_YELLOW, moji, 0);				y += 40;
			systemWndFontNo[3] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "      加    快      ", 2);	y += 40;
			systemWndFontNo[4] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "      减    慢      ", 2);	y += 52;
			systemWndFontNo[2] = StockFontBuffer(x, y, FONT_PRIO_FRONT, FONT_PAL_AQUA, "      回上一页      ", 2);	y += 40;

			break;

		case 5:	// ????

			// ?????????
			if (pActMenuWnd == NULL){
				// ?????????
				x = (lpDraw->xSize - 272) / 2;
				y = (lpDraw->ySize - 430) / 2;
				pActMenuWnd = MakeWindowDisp(x, y, 272, 430, CG_WND_TITLE_CHAT, -1, FALSE);
				for (i = 0; i < MENU_SYSTEM_0; i++) systemWndFontNo[i] = -2;
				// ???????
				GetKeyInputFocus(&chatRegistryStr[0]);

				break;
			}
			else{
				if (pActMenuWnd->hp <= 0) break;
			}

			// ???????
			StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my, DISP_PRIO_MENU, CG_CHAT_REGISTY_WND, 1);
			// ????????
			if (mouse.onceState & MOUSE_LEFT_CRICK){
				// ???
				if (HitDispNo == systemWndFontNo[3]){
					// ??????
					DeathAction(pActMenuWnd);
					pActMenuWnd = NULL;
					// ????????
					GetKeyInputFocus(&MyChatBuffer);
					// ??????????
					systemWndNo = 0;
					// ????????
					play_se(203, 320, 240);
					// ????????
					saveUserSetting();
					// ????????
					SaveChatRegistyStr();
					break;
				}
			}
			// ?
			x = pActMenuWnd->x + 18;
			y = pActMenuWnd->y + 58;

			// ?????
			for (i = 0; i < 8; i++){
				// ????
				chatRegistryStr[i].x = x + 2;
				chatRegistryStr[i].y = y;
				StockFontBuffer2(&chatRegistryStr[i]); y += 43;
				// ???????
				if (MakeHitBox(chatRegistryStr[i].x - 4, chatRegistryStr[i].y - 3,
					chatRegistryStr[i].x + 234 + 3, chatRegistryStr[i].y + 16 + 3, DISP_PRIO_IME3) == TRUE){
					// ????????
					if (mouse.onceState & MOUSE_LEFT_CRICK){
						// ???????
						GetKeyInputFocus(&chatRegistryStr[i]);
						play_se(217, 320, 240); // ?????
					}
				}
			}
			// ?????
			systemWndFontNo[3] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, y + 5, DISP_PRIO_IME3, CG_RETURN_BTN, 2);

			break;

		case 6:	// ?????

			// ?????????
			if (pActMenuWnd == NULL){
				// ?????????
				x = (lpDraw->xSize - 3 * 64) / 2;
				y = (lpDraw->ySize - 4 * 48) / 2;
				pActMenuWnd = MakeWindowDisp(x, y, 3, 4, CG_WND_TITLE_MOUSE, 1, FALSE);
				for (i = 0; i < MENU_SYSTEM_0; i++) systemWndFontNo[i] = -2;
				break;
			}
			else{
				if (pActMenuWnd->hp <= 0) break;
			}

			// ??????????
			if (HitFontNo == systemWndFontNo[0]){
				// ???
				if (MouseCursorFlag == TRUE){
					// ??????
					strcpy(OneLineInfoStr, "两色的滑鼠游标，反应较快。");

					// ????????
					if (mouse.onceState & MOUSE_LEFT_CRICK){
						// ??????????
#ifdef _TALK_WINDOW
						g_iCursorCount = ShowCursor( FALSE );
#else
						ShowCursor(FALSE);
#endif
						MouseCursorFlag = FALSE;
						play_se(217, 320, 240);	// ?????
					}
				}
				else{
					// ??????
					strcpy(OneLineInfoStr, "普通的滑鼠游标。");
					// ????????
					if (mouse.onceState & MOUSE_LEFT_CRICK){
						// ??????????
#ifdef _TALK_WINDOW
						g_iCursorCount = ShowCursor( TRUE );
#else
						//ShowCursor( TRUE );
#endif
						MouseCursorFlag = FALSE;
						play_se(217, 320, 240);	// ?????
					}
				}
			}
			// ????????
			if (mouse.onceState & MOUSE_LEFT_CRICK){
				// ???
				if (HitFontNo == systemWndFontNo[1]){
					// ??????
					DeathAction(pActMenuWnd);
					pActMenuWnd = NULL;
					// ??????????
					systemWndNo = 0;
					// ????????
					play_se(203, 320, 240);
					// ????????
					saveUserSetting();
					break;
				}
			}

			// ?
			x = pActMenuWnd->x + 38;
			y = pActMenuWnd->y + 56;

			// ?????????
			StockFontBuffer(x, y, FONT_PRIO_FRONT, FONT_PAL_YELLOW, "◆  游标设定  ◆", 0);	y += 40;
			systemWndFontNo[0] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, mouseCursor[MouseCursorFlag], 2);	y += 40;
			systemWndFontNo[1] = StockFontBuffer(x, y, FONT_PRIO_FRONT, FONT_PAL_AQUA, "    回上一页    ", 2);	y += 40;
			break;

		case 7:	// ????? ??????

			// ?????????
			if (pActMenuWnd == NULL){
				// ?????????
				x = (lpDraw->xSize - 3 * 64) / 2;
				y = (lpDraw->ySize - 3 * 48) / 2;
				pActMenuWnd = MakeWindowDisp(x, y, 3, 3, CG_WND_TITLE_LOGOUT, 1, FALSE);
				for (i = 0; i < MENU_SYSTEM_0; i++) systemWndFontNo[i] = -2;
				break;
			}
			else{
				if (pActMenuWnd->hp <= 0) break;
			}

			// ????????
			if (mouse.onceState & MOUSE_LEFT_CRICK){
				// ????
				if (HitFontNo == systemWndFontNo[0]){
					// ??????
					DeathAction(pActMenuWnd);
					pActMenuWnd = NULL;
					GameState = GAME_LOGIN;
					ChangeProc2(PROC_CHAR_LOGOUT);

					play_se(206, 320, 240);
					break;
				}
				// ?????
				if (HitFontNo == systemWndFontNo[1]){
					// ??????
					DeathAction(pActMenuWnd);
					pActMenuWnd = NULL;
					// ??????
					systemWndNo = 0;
					play_se(203, 320, 240);
					break;
				}
			}

			// ?
			x = pActMenuWnd->x + 38;
			y = pActMenuWnd->y + 56;

#ifdef _NEWFONT_

			systemWndFontNo[0] = StockFontBuffer(x+20, y, FONT_PRIO_FRONT, 0, "     确  定     ", 2);	y += 40;
			systemWndFontNo[1] = StockFontBuffer(x+20, y, FONT_PRIO_FRONT, 0, "     不  要     ", 2);	y += 40;
#else

			systemWndFontNo[0] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "     确  定     ", 2);	y += 40;
			systemWndFontNo[1] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "     不  要     ", 2);	y += 40;
#endif
			break;
		}
	}

	//? ???????????? *****************************************************/
#ifdef _FRIENDCHANNEL				//ROG ADD 好友频道
	if(setRoomFlag == TRUE)
	{
		setRoomName();
	}

	if(assentFlag == TRUE)
	{
		AssentWnd();
	}
#endif


	if (MenuToggleFlag & JOY_CTRL_S){

		// ????????
		switch (statusWndNo){

		case 0:	// ??????

			// ?????????
			if (pActMenuWnd == NULL){
				// ?????????

				pActMenuWnd = MakeWindowDisp(4, 4, 272, 360, 0, -1);
#ifdef _CHARTITLE_STR_
				extern void 设置称号取消窗口();
				extern BOOL 获取称号取消窗口();
				if(获取称号取消窗口()) 设置称号取消窗口();
#endif
				// ??????
				for (i = 0; i < MENU_STATUS_0; i++) statusWndFontNo[i] = -2;
				for (i = 0; i < MENU_STATUS_0; i++) statusWndBtnFlag[i] = 0;
#ifdef _NEWPANEL //Syu ADD 7.0 新人物状态介面
				SkillWndflag = false ; 
#endif
			}
			else{
				// ??????????????
				if (pActMenuWnd->hp > 0){
					// ???????????????
#ifdef _NEWPANEL //Syu ADD 7.0 新人物状态介面
					if ( SkillWndflag == true ) {
						SkillWndfunc2();
					}
					else {
						if ( pActSkillMenuWnd != NULL ) {
							DeathAction( pActSkillMenuWnd ) ; 
							pActSkillMenuWnd = NULL ; 
						}
					}
					StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx-1, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my+34, DISP_PRIO_MENU, CG_NEW_STATUS_WND, 1 );
#else
#ifdef _NEW_CHARDATA_
					StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my + 20, DISP_PRIO_MENU,	55266, 1);
#else
					StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my, DISP_PRIO_MENU, CG_STATUS_WND, 1);
#endif
#endif
					// ????????
					if (mouse.onceState & MOUSE_LEFT_CRICK){
						// 队伍
						if (HitDispNo == statusWndFontNo[0]){
							DeathAction(pActMenuWnd);
							pActMenuWnd = NULL;
							statusWndNo = 1;
							play_se(202, 320, 240);
						}
						// 关闭
						if (HitDispNo == statusWndFontNo[1]){
							DeathAction(pActMenuWnd);
							pActMenuWnd = NULL;
							MenuToggleFlag ^= JOY_CTRL_S;
							play_se(203, 320, 240);
						}
#ifdef _NEWPANEL //Syu ADD 7.0 新人物状态介面
						if( HitDispNo == statusWndFontNo[ 12 ] ){
							DeathAction( pActMenuWnd);
							pActMenuWnd = NULL;
							statusWndNo = 3;
							play_se( 202, 320, 240);
#ifdef _NEWREQUESTPROTOCOL			// (不可开) Syu ADD 新增Protocol要求细项
							lssproto_RESIST_send ( sockfd , "" ) ; 
#endif
#ifdef _ALCHEPLUS
							lssproto_ALCHEPLUS_send ( sockfd , "" ) ; 
#endif
						}
						if( HitDispNo == statusWndFontNo[ 14 ] ){
							play_se( 202, 320 , 240 ) ; 
							SkillWndflag = true ; 
							MenuToggleFlag &= ~JOY_CTRL_I;
							MenuToggleFlag &= ~JOY_CTRL_M;
							if( pActMenuWnd3 != NULL ) {
								DeathAction ( pActMenuWnd3 ) ; 
								pActMenuWnd3 = NULL ; 
								GetKeyInputFocus( &MyChatBuffer );
							}
						}
#endif

					}
					// 改变称号
					if (HitDispNo == statusWndFontNo[6]){
						// ????????
						if (mouse.onceState & MOUSE_LEFT_CRICK){
#ifdef _CHARTITLE_
							statusWndBtnFlag[6] = TRUE;
							extern void 打开称号窗口初始化();
							打开称号窗口初始化();
							if (pActMenuWnd3 == NULL){
								pActMenuWnd3 = MakeWindowDisp(363, 105, 0, 0, 55249, -1, 0);
								play_se(202, 320, 240);
							}
							else{
								DeathAction(pActMenuWnd3);
								pActMenuWnd3 = NULL;
								play_se(203, 320, 240);
							}
#else
#ifdef _CHARTITLE_STR_
							statusWndBtnFlag[6] = TRUE;
							extern void 打开称号窗口初始化();
							打开称号窗口初始化();
							if (pActMenuWnd3 == NULL){
								pActMenuWnd3 = MakeWindowDisp(363, 105, 0, 0, 55249, -1, 0);
								play_se(202, 320, 240);
							}
							else{
								DeathAction(pActMenuWnd3);
								pActMenuWnd3 = NULL;
								play_se(203, 320, 240);
							}
#else
#ifdef _NEWPANEL //Syu ADD 7.0 新人物状态介面
							SkillWndflag = false ; 
							MenuToggleFlag &= ~JOY_CTRL_I;
							//MenuToggleFlag &= ~JOY_CTRL_M;
#endif
#ifdef _TAIKEN			
							// ???
							play_se( 220, 320, 240 );
							// ???????
							sprintf_s( moji,"体验版不能选择！" );
							// ??????????????????
							StockChatBufferLine( moji, FONT_PAL_WHITE );
#else
							if (pActMenuWnd3 == NULL){
								// ?????????
#ifdef _NEWPANEL //Syu ADD 7.0 新人物状态介面
								pActMenuWnd3 = MakeWindowDisp( 304, 16 + 280 + 48, 272, 88, 0, -1 );
#else
#ifdef _NEW_CHARDATA_
								pActMenuWnd3 = MakeWindowDisp(4, 16 + 280 + 78 + 35, 272, 88, 0, -1, 0);
#else
								pActMenuWnd3 = MakeWindowDisp( 4, 16 + 280 + 78, 272, 88, 0, -1 ,0);
#endif
#endif
								// ????????
								play_se(202, 320, 240);
								// ????
								shougouChange.buffer[0] = NULL;
								shougouChange.cnt = 0;
								shougouChange.cursor = 0;
								// ????
								shougouChange.len = 12;
								// ???
								shougouChange.color = 0;
								// ????
								shougouChange.x = pActMenuWnd3->x + 38;
								shougouChange.y = pActMenuWnd3->y + 25;
								// ??????
								shougouChange.fontPrio = FONT_PRIO_FRONT;
								// ????
								statusWndBtnFlag[6] = TRUE;
							}
							else{
								// ??????
								DeathAction(pActMenuWnd3);
								pActMenuWnd3 = NULL;
								// ????????
								GetKeyInputFocus(&MyChatBuffer);
								// ????????
								play_se(203, 320, 240);
							}
							statusWndBtnFlag[6] = TRUE;
#endif
#endif
#endif
						}
						// ?????????
						if (mouse.state & MOUSE_LEFT_CRICK && statusWndBtnFlag[6] == TRUE){
							// ????
							statusWndBtnFlag[6] = TRUE;
						}
						else{
							// ???
							statusWndBtnFlag[6] = FALSE;
						}
					}
					else{
						// ???
						statusWndBtnFlag[6] = FALSE;
					}
					// ??????????
					if (StatusUpPoint != 0){
						// ????????
						for (i = 2; i < 6; i++){
							// ?????
							if (HitDispNo == statusWndFontNo[i]){
								// ????
								// ????????
								if (mouse.onceState & MOUSE_LEFT_CRICK){
									// ???????
									if (bNewServer)
										lssproto_SKUP_send(sockfd, i - 2);
									else
										old_lssproto_SKUP_send(sockfd, i - 2);


#ifndef _CHAR_PROFESSION			// WON ADD 人物职业
									StatusUpPoint--;
#endif

									play_se(211, 320, 240);

									statusWndBtnFlag[i] = TRUE;
								}
								// ?????????
								if (mouse.state & MOUSE_LEFT_CRICK && statusWndBtnFlag[i] == TRUE){
									// ????
									statusWndBtnFlag[i] = TRUE;
								}
								else{
									// ???
									statusWndBtnFlag[i] = FALSE;
								}
							}
							else{
								// ???
								statusWndBtnFlag[i] = FALSE;
							}
						}
					}
					// ????????
					if (pActMenuWnd != NULL){
#define PET_WND_ATTR_X 159
#define PET_WND_ATTR_Y 5
						int x2;
						x = pActMenuWnd->x + 20;
						y = pActMenuWnd->y + 28;
						CenteringStr(pc.name, moji, CHAR_NAME_LEN);
						StockFontBuffer(x - 6, y - 2, FONT_PRIO_FRONT, 0, moji, 0);
						sprintf_s(moji, "%8d", pc.dp);
						StockFontBuffer(x + 170, y - 2, FONT_PRIO_FRONT, 0, moji, 0); y += 21;
						// ??????????
#ifndef _CHARTITLE_
#ifndef _CHARTITLE_STR_
						CenteringStr(pc.freeName, moji, 32);	// ????
#endif
#endif
#ifdef _NEWPANEL //Syu ADD 7.0 新人物状态介面
						//位移称号
						StockFontBuffer( x + 77, y + 2 , FONT_PRIO_FRONT, 0, moji, 0 );
#ifdef _ALLDOMAN // (不可开) Syu ADD 排行榜NPC
						//英雄战场称号
						if ( pc.herofloor == 132 )		sprintf_s( moji , "%s" , "尼斯大陆英雄" ) ;
						else if ( pc.herofloor >= 130 )	sprintf_s( moji , "%s" , "尼斯大陆战士" ) ; 
						else if ( pc.herofloor >= 125 )	sprintf_s( moji , "%s" , "尼斯大陆勇士" ) ; 
						else if ( pc.herofloor >= 120 )	sprintf_s( moji , "%s" , "萨伊那斯英雄" ) ; 
						else if ( pc.herofloor >= 115 )	sprintf_s( moji , "%s" , "斯巴达战士" ) ; 
						else if ( pc.herofloor >= 110 )	sprintf_s( moji , "%s" , "萨姆吉尔战士" ) ; 
						else if ( pc.herofloor >= 100 )	sprintf_s( moji , "%s" , "玛丽那丝战士" ) ; 
						else if ( pc.herofloor >=  80 )	sprintf_s( moji , "%s" , "卡坦战士" ) ; 
						else if ( pc.herofloor >=  60 )	sprintf_s( moji , "%s" , "霍特尔战士" ) ; 
						else if ( pc.herofloor >=  40 )	sprintf_s( moji , "%s" , "降魔勇士" ) ; 
						else if ( pc.herofloor >=  20 )	sprintf_s( moji , "%s" , "圣灵勇士" ) ; 
						else if ( pc.herofloor >=   1 )	sprintf_s( moji , "%s" , "初犊勇士" ) ; 
						else							sprintf_s( moji , " " );
						StockFontBuffer( x + 72, y + 28 , FONT_PRIO_FRONT, 5, moji, 0 ); 
						sprintf_s( moji , "%d" , pc.profession_level);
						StockFontBuffer( x + 157, y + 53 , FONT_PRIO_FRONT, 0, moji, 0 ); 
#endif
#else
#ifndef _CHARTITLE_
#ifndef _CHARTITLE_STR_
#ifdef _NEWFONT_
						StockFontBuffer( x-20, y, FONT_PRIO_FRONT, 0, moji, 0 ); 
#else
						StockFontBuffer(x - 60, y, FONT_PRIO_FRONT, 0, moji, 0);
#endif
#endif
#endif
#endif
#ifdef _TRANS_6
						if( pc.transmigration >= 1 && pc.transmigration <= 6 ){
#else
						if (pc.transmigration >= 1 && pc.transmigration <= 5){
#endif
#ifndef _NEW_CHARDATA_
							sprintf_s(moji, "转生%s", TransmigrationStr[pc.transmigration]);
							StockFontBuffer(x + 178 + 12, y + 2, FONT_PRIO_FRONT, FONT_PAL_AQUA, moji, 0);
#else
							;
#endif
						}
#ifdef _CHARTITLE_
						y += 32;
						
#else
#ifdef _CHARTITLE_STR_
						y += 19;
#else
						y += 21;
#endif
#endif
						sprintf_s(moji, "%3d", pc.level);
#ifdef _NEWPANEL //Syu ADD 7.0 新人物状态介面
						y += 35;
						StockFontBuffer( x + 38, y - 2, FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						sprintf_s( moji, "%8d", pc.exp );
						StockFontBuffer( x + 48, y + 2 , FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						sprintf_s( moji, "%8d",pc.maxExp );
						StockFontBuffer( x + 48, y + 4, FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						sprintf_s( moji, "%4d", pc.hp );
						StockFontBuffer( x + 48, y + 8, FONT_PRIO_FRONT, 0, moji, 0 );
						sprintf_s( moji, "%4d", pc.maxHp );
						StockFontBuffer( x + 98, y + 8, FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						sprintf_s( moji, "%3d", pc.mp );
						StockFontBuffer( x + 50, y + 12 , FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						sprintf_s( moji, "%3d", pc.atk );
						StockFontBuffer( x + 50, y + 16, FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						sprintf_s( moji, "%3d", pc.def );
						StockFontBuffer( x + 50, y + 20, FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						sprintf_s( moji, "%3d", pc.quick );
						StockFontBuffer( x + 50, y + 24, FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						sprintf_s( moji, "%3d", pc.charm );
						StockFontBuffer( x + 50, y + 28, FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						y += 29;
						sprintf_s( moji, "%3d", pc.vital );
						StockFontBuffer( x + 55, y + 34 , FONT_PRIO_FRONT, 0, moji, 0 );
						sprintf_s( moji, "%3d", pc.str );
						StockFontBuffer( x + 55 + 120, y + 34 , FONT_PRIO_FRONT, 0, moji, 0 ); y += 20;
						sprintf_s( moji, "%3d", pc.tgh );
						StockFontBuffer( x + 55, y + 34 , FONT_PRIO_FRONT, 0, moji, 0 );
						sprintf_s( moji, "%3d", pc.dex );
						StockFontBuffer( x + 55 + 120, y + 34 , FONT_PRIO_FRONT, 0, moji, 0 ); y += 20;
#else
#ifdef _NEW_CHARDATA_

						/*
						static int 人物头像左状态=0,人物头像右状态=0;

						StockDispBuffer( pActMenuWnd->x + 187, pActMenuWnd->y + 118+72, DISP_PRIO_IME3,CG_PREV_BTN+人物头像左状态, 1 );
						if( MakeHitBox( pActMenuWnd->x + 187-18, pActMenuWnd->y + 118+72-10, pActMenuWnd->x + 187+18,pActMenuWnd->y + 118+72+10, DISP_PRIO_IME4 ) == TRUE ){
						ShowBottomLineString(FONT_PAL_WHITE, "修改人物头像");
						if(mouse.state & MOUSE_LEFT_CRICK){
						人物头像左状态 = 1;
						if(mouse.onceState & MOUSE_LEFT_CRICK){
						play_se(203, 320, 240);
						//人物头像封包
						}
						}else 人物头像左状态=0;
						}else 人物头像左状态=0;
						StockDispBuffer( pActMenuWnd->x + 232, pActMenuWnd->y + 118+72, DISP_PRIO_IME3,CG_NEXT_BTN+人物头像右状态, 1 );
						if( MakeHitBox( pActMenuWnd->x + 232-18, pActMenuWnd->y + 118+72-10, pActMenuWnd->x + 232+18,pActMenuWnd->y + 118+72+10, DISP_PRIO_IME4 ) == TRUE ){
						ShowBottomLineString(FONT_PAL_WHITE, "修改人物头像");
						if(mouse.state & MOUSE_LEFT_CRICK){
						人物头像右状态 = 1;
						if(mouse.onceState & MOUSE_LEFT_CRICK){
						play_se(203, 320, 240);
						//人物头像封包
						}
						}else 人物头像右状态=0;
						}else 人物头像右状态=0;

						*/


						StockFontBuffer(x + 50, y + 44, FONT_PRIO_FRONT, 0, moji, 0);
						sprintf_s(moji, "%s", TransmigrationStr[pc.transmigration]);
						StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, Transmigrationcolor[pc.transmigration], moji, 0);
						sprintf_s(moji, "%s", pc.chusheng);
						StockFontBuffer(x + 50, y + 21, FONT_PRIO_FRONT, 0, moji, 0);
						y += 21 + 44;
						sprintf_s(moji, "%8d", pc.exp);
						StockFontBuffer(x + 48, y, FONT_PRIO_FRONT, 0, moji, 0); y += 21;
						sprintf_s(moji, "%8d", pc.maxExp);
						StockFontBuffer(x + 48, y, FONT_PRIO_FRONT, 0, moji, 0); y += 21;
						sprintf_s(moji, "%4d", pc.hp);
						StockFontBuffer(x + 48, y, FONT_PRIO_FRONT, 0, moji, 0);
						sprintf_s(moji, "%4d", pc.maxHp);
						StockFontBuffer(x + 98, y, FONT_PRIO_FRONT, 0, moji, 0); y += 21;
						sprintf_s(moji, "%3d", pc.mp);
						StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, 0, moji, 0); y += 21;
						sprintf_s(moji, "%3d", pc.atk);
						StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, 0, moji, 0); y += 21;
						sprintf_s(moji, "%3d", pc.def);
						StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, 0, moji, 0); y += 21;
						sprintf_s(moji, "%3d", pc.quick);
						StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, 0, moji, 0); y += 21;
						sprintf_s(moji, "%3d", pc.charm);
						StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, 0, moji, 0); y += 21;
						y += 29;
						sprintf_s(moji, "%3d", pc.vital);
						StockFontBuffer(x + 61, y, FONT_PRIO_FRONT, 0, moji, 0);
						sprintf_s(moji, "%3d", pc.str);
						StockFontBuffer(x + 61 + 120, y, FONT_PRIO_FRONT, 0, moji, 0); y += 20;
						sprintf_s(moji, "%3d", pc.tgh);
						StockFontBuffer(x + 61, y, FONT_PRIO_FRONT, 0, moji, 0);
						sprintf_s(moji, "%3d", pc.dex);
						StockFontBuffer(x + 61 + 120, y, FONT_PRIO_FRONT, 0, moji, 0); y += 20;
#else
						StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, 0, moji, 0); y += 21;
						sprintf_s(moji, "%8d", pc.exp);
						StockFontBuffer(x + 48, y, FONT_PRIO_FRONT, 0, moji, 0); y += 21;
						sprintf_s(moji, "%8d", pc.maxExp);
						StockFontBuffer(x + 48, y, FONT_PRIO_FRONT, 0, moji, 0); y += 21;
						sprintf_s(moji, "%4d", pc.hp);
						StockFontBuffer(x + 48, y, FONT_PRIO_FRONT, 0, moji, 0);
						sprintf_s(moji, "%4d", pc.maxHp);
						StockFontBuffer(x + 98, y, FONT_PRIO_FRONT, 0, moji, 0); y += 21;
						sprintf_s(moji, "%3d", pc.mp);
						StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, 0, moji, 0); y += 21;
						sprintf_s(moji, "%3d", pc.atk);
						StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, 0, moji, 0); y += 21;
						sprintf_s(moji, "%3d", pc.def);
						StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, 0, moji, 0); y += 21;
						sprintf_s(moji, "%3d", pc.quick);
						StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, 0, moji, 0); y += 21;
						sprintf_s(moji, "%3d", pc.charm);
						StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, 0, moji, 0); y += 21;
						y += 29;
						sprintf_s(moji, "%3d", pc.vital);
						StockFontBuffer(x + 61, y, FONT_PRIO_FRONT, 0, moji, 0);
						sprintf_s(moji, "%3d", pc.str);
						StockFontBuffer(x + 61 + 120, y, FONT_PRIO_FRONT, 0, moji, 0); y += 20;
						sprintf_s(moji, "%3d", pc.tgh);
						StockFontBuffer(x + 61, y, FONT_PRIO_FRONT, 0, moji, 0);
						sprintf_s(moji, "%3d", pc.dex);
						StockFontBuffer(x + 61 + 120, y, FONT_PRIO_FRONT, 0, moji, 0); y += 20;
#endif
#endif

#ifdef _NEWPANEL //Syu ADD 7.0 新人物状态介面
						y += 50;
#else
						y += 17;
#endif
						statusWndFontNo[0] = StockDispBuffer(x + 59, y, DISP_PRIO_IME3, CG_STATUS_WND_GROUP_BTN, 2);
						statusWndFontNo[1] = StockDispBuffer(x + 173, y, DISP_PRIO_IME3, CG_CLOSE_BTN, 2);
						// ???????
#ifdef _NEWPANEL //Syu ADD 7.0 新人物状态介面
						statusWndFontNo[ 12 ] = StockDispBuffer( x + 164 , y - 102, DISP_PRIO_IME3, CG_STATUS_DETAIL, 2 );
						if ( pc.profession_class != 0 )
							statusWndFontNo[ 14 ] = StockDispBuffer( x + 213 , y - 318 , DISP_PRIO_IME3, CG_WAR_ICON_BIG + pc.profession_class - 1, 2 );
						//else //找时间放空图
						//	statusWndFontNo[ 14 ] = StockDispBuffer( x + 213 , y - 318 , DISP_PRIO_IME3, CG_WAR_ICON_BIG + pc.profession_class - 1, 2 );


						StockDispBuffer( pActMenuWnd->x + 216, pActMenuWnd->y + 168, DISP_PRIO_IME3, pc.faceGraNo, 0 );
#else
#ifdef _CHARTITLE_
						StockDispBuffer(pActMenuWnd->x + 209, pActMenuWnd->y + 118 + 11, DISP_PRIO_IME3, pc.faceGraNo, 0);
#else
#ifdef _CHARTITLE_STR_
						StockDispBuffer(pActMenuWnd->x + 209+1, pActMenuWnd->y + 118 + 13, DISP_PRIO_IME3, pc.faceGraNo, 0);
#else
#ifdef _NEW_CHARDATA_
						StockDispBuffer(pActMenuWnd->x + 210, pActMenuWnd->y + 118 + 13, DISP_PRIO_IME3, pc.faceGraNo, 0);
#else
						StockDispBuffer( pActMenuWnd->x + 209, pActMenuWnd->y + 118, DISP_PRIO_IME3, pc.faceGraNo, 0 );
#endif
#endif
#endif
#endif					
						if (StatusUpPoint != 0){
							// ??????????
#ifdef _NEWPANEL //Syu ADD 7.0 新人物状态介面
							StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx + 14 , ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my + 67 , DISP_PRIO_IME3, CG_STATUS_WND_LV_UP_POINT, 0 );
							sprintf_s( moji, "%2d", StatusUpPoint );
							StockFontBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx + 70, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my + 156, FONT_PRIO_FRONT, FONT_PAL_YELLOW, moji, 0 );
							statusWndFontNo[ 2 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx  , ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my + 66 , DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[ 2 ], 2 );
							statusWndFontNo[ 3 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx + 120, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my + 66 , DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[ 3 ], 2 );
							statusWndFontNo[ 4 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx  , ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my + 86, DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[ 4 ], 2 );
							statusWndFontNo[ 5 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx + 120, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my + 86, DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[ 5 ], 2 );
#else
#ifdef _CHARTITLE_
							StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my + 11, DISP_PRIO_IME3, CG_STATUS_WND_LV_UP_POINT, 0);
							sprintf_s(moji, "%2d", StatusUpPoint);
							StockFontBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx + 50, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my + 88 + 11, FONT_PRIO_FRONT, FONT_PAL_YELLOW, moji, 0);

							// ??????
							statusWndFontNo[2] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my + 11, DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[2], 2);
							statusWndFontNo[3] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx + 120, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my + 11, DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[3], 2);
							statusWndFontNo[4] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my + 20 + 11, DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[4], 2);
							statusWndFontNo[5] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx + 120, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my + 20 + 11, DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[5], 2);
#else
#ifdef _CHARTITLE_STR_
							StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my + 11+31, DISP_PRIO_IME3, CG_STATUS_WND_LV_UP_POINT, 0);
							sprintf_s(moji, "%2d", StatusUpPoint);
							StockFontBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx + 50, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my + 88 + 11+31, FONT_PRIO_FRONT, FONT_PAL_YELLOW, moji, 0);

							// ??????
							statusWndFontNo[2] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my + 11+31, DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[2], 2);
							statusWndFontNo[3] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx + 120, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my + 11+31, DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[3], 2);
							statusWndFontNo[4] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my + 20 + 11+31, DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[4], 2);
							statusWndFontNo[5] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx + 120, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my + 20 + 11+31, DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[5], 2);
#else
#ifdef _NEW_CHARDATA_
							StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my + 42, DISP_PRIO_IME3, CG_STATUS_WND_LV_UP_POINT, 0);
							sprintf_s(moji, "%2d", StatusUpPoint);
							StockFontBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx + 50, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my + 88 + 42, FONT_PRIO_FRONT, FONT_PAL_YELLOW, moji, 0);
							statusWndFontNo[2] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my + 42, DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[2], 2);
							statusWndFontNo[3] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx + 120, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my + 42, DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[3], 2);
							statusWndFontNo[4] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my + 20 + 42, DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[4], 2);
							statusWndFontNo[5] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx + 120, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my + 20 + 42, DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[5], 2);
#else
							StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my, DISP_PRIO_IME3, CG_STATUS_WND_LV_UP_POINT, 0 );
							sprintf_s( moji, "%2d", StatusUpPoint );
							StockFontBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx + 50, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my + 88, FONT_PRIO_FRONT, FONT_PAL_YELLOW, moji, 0 );
							// ??????
							statusWndFontNo[ 2 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my, DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[ 2 ], 2 );
							statusWndFontNo[ 3 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx + 120, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my, DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[ 3 ], 2 );
							statusWndFontNo[ 4 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my + 20, DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[ 4 ], 2 );
							statusWndFontNo[ 5 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx + 120, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my + 20, DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[ 5 ], 2 );
#endif
#endif
#endif
#endif
						}
						// ??
#ifdef _NEWPANEL //Syu ADD 7.0 新人物状态介面
						//位移改变称号钮
						statusWndFontNo[ 6 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx - 124 , ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my, DISP_PRIO_IME3, CG_STATUS_WND_SHOUGOU_BTN_UP + statusWndBtnFlag[ 6 ], 2 );
#else
#ifdef _CHARTITLE_
						extern void 称号展示();
						称号展示();
						statusWndFontNo[6] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx - 125, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my + 3, DISP_PRIO_IME3, CG_STATUS_WND_SHOUGOU_BTN_UP + statusWndBtnFlag[6], 2);
#else
#ifdef _CHARTITLE_STR_
						extern void 称号展示();
						extern BOOL 获取当前称号是否存在();
						extern BOOL 获取称号取消窗口();
						extern void 设置称号取消窗口();
						if(获取称号取消窗口()){
							StockDispBuffer(138,450,DISP_PRIO_MENU,55259,1);//确定窗口底图
							int bnt;
							StockFontBuffer(70,425,1,0,"是否取消称号显示？",0);
							bnt = StockFontBuffer(90,462,1,FONT_PAL_YELLOW,"确定",2);
							if(mouse.onceState & MOUSE_LEFT_CRICK>0){
								if(HitFontNo == bnt){
									设置称号取消窗口();
									if(获取当前称号是否存在()){
										int checksum=0;
										char buf[1024*4];
										memset(buf,0,1024*4);
										checksum += util_mkint(buf, -1);
										util_mkint(buf, checksum);
										util_SendMesg(sockfd, LSSPROTO_CHARTITLE_SEND, buf);
									}
									play_se(203, 320, 240);
								}
							}
							bnt = StockFontBuffer(155,462,1,FONT_PAL_YELLOW,"取消",2);
							if(mouse.onceState & MOUSE_LEFT_CRICK>0){
								if(HitFontNo == bnt){
									设置称号取消窗口();
									play_se(203, 320, 240);
								}
							}
						}
						称号展示();
						statusWndFontNo[6] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my, DISP_PRIO_IME3, CG_STATUS_WND_SHOUGOU_BTN_UP + statusWndBtnFlag[6], 2);
#else
						statusWndFontNo[6] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my, DISP_PRIO_IME3, CG_STATUS_WND_SHOUGOU_BTN_UP + statusWndBtnFlag[6], 2);
#endif
#endif
#endif

						// ?
						x = pActMenuWnd->x + 19;
						y = pActMenuWnd->y + 174;
#ifdef _NEWPANEL //Syu ADD 7.0 新人物状态介面
						x -= 4;
						y += 43;
#endif
#ifdef _CHARTITLE_
						y += 11;
#endif
#ifdef _NEW_CHARDATA_
						y += 42;
#endif
						if (pc.earth > 0){
							x2 = (int)(pc.earth * 0.8);
							StockBoxDispBuffer(x + PET_WND_ATTR_X + 0, y + PET_WND_ATTR_Y + 0, x + PET_WND_ATTR_X + 0 + x2, y + PET_WND_ATTR_Y + 8, DISP_PRIO_IME4, SYSTEM_PAL_GREEN, 1);
							StockBoxDispBuffer(x + PET_WND_ATTR_X + 1, y + PET_WND_ATTR_Y + 1, x + PET_WND_ATTR_X + 1 + x2, y + PET_WND_ATTR_Y + 9, DISP_PRIO_IME3, SYSTEM_PAL_GREEN2, 0);
							StockBoxDispBuffer(x + PET_WND_ATTR_X + 2, y + PET_WND_ATTR_Y + 2, x + PET_WND_ATTR_X + 2 + x2, y + PET_WND_ATTR_Y + 10, DISP_PRIO_IME3, SYSTEM_PAL_GREEN2, 0);
						}
						y += 20;

						// ????
						if (pc.water > 0){
							x2 = (int)(pc.water * 0.8);

							StockBoxDispBuffer(x + PET_WND_ATTR_X + 0, y + PET_WND_ATTR_Y + 0, x + PET_WND_ATTR_X + 0 + x2, y + PET_WND_ATTR_Y + 8, DISP_PRIO_IME4, SYSTEM_PAL_AQUA, 1);
							StockBoxDispBuffer(x + PET_WND_ATTR_X + 1, y + PET_WND_ATTR_Y + 1, x + PET_WND_ATTR_X + 1 + x2, y + PET_WND_ATTR_Y + 9, DISP_PRIO_IME3, SYSTEM_PAL_AQUA2, 0);
							StockBoxDispBuffer(x + PET_WND_ATTR_X + 2, y + PET_WND_ATTR_Y + 2, x + PET_WND_ATTR_X + 2 + x2, y + PET_WND_ATTR_Y + 10, DISP_PRIO_IME3, SYSTEM_PAL_AQUA2, 0);
						}
						y += 20;

						// ????
						if (pc.fire > 0){
							x2 = (int)(pc.fire * 0.8);
							StockBoxDispBuffer(x + PET_WND_ATTR_X + 0, y + PET_WND_ATTR_Y + 0, x + PET_WND_ATTR_X + 0 + x2, y + PET_WND_ATTR_Y + 8, DISP_PRIO_IME4, SYSTEM_PAL_RED, 1);
							StockBoxDispBuffer(x + PET_WND_ATTR_X + 1, y + PET_WND_ATTR_Y + 1, x + PET_WND_ATTR_X + 1 + x2, y + PET_WND_ATTR_Y + 9, DISP_PRIO_IME3, SYSTEM_PAL_RED2, 0);
							StockBoxDispBuffer(x + PET_WND_ATTR_X + 2, y + PET_WND_ATTR_Y + 2, x + PET_WND_ATTR_X + 2 + x2, y + PET_WND_ATTR_Y + 10, DISP_PRIO_IME3, SYSTEM_PAL_RED2, 0);
						}
						y += 20;

						// ????
						if (pc.wind > 0){
							x2 = (int)(pc.wind * 0.8);
							StockBoxDispBuffer(x + PET_WND_ATTR_X + 0, y + PET_WND_ATTR_Y + 0, x + PET_WND_ATTR_X + 0 + x2, y + PET_WND_ATTR_Y + 8, DISP_PRIO_IME4, SYSTEM_PAL_YELLOW, 1);
							StockBoxDispBuffer(x + PET_WND_ATTR_X + 1, y + PET_WND_ATTR_Y + 1, x + PET_WND_ATTR_X + 1 + x2, y + PET_WND_ATTR_Y + 9, DISP_PRIO_IME3, SYSTEM_PAL_YELLOW2, 0);
							StockBoxDispBuffer(x + PET_WND_ATTR_X + 2, y + PET_WND_ATTR_Y + 2, x + PET_WND_ATTR_X + 2 + x2, y + PET_WND_ATTR_Y + 10, DISP_PRIO_IME3, SYSTEM_PAL_YELLOW2, 0);
						}

					}
				}
			}
			// ???????????
			if (pActMenuWnd3 != NULL){
				// ??????????????
				if (pActMenuWnd3->hp > 0){
					// ?????????
#ifdef _NEWPANEL //Syu ADD 7.0 新人物状态介面
					StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd3->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd3->pYobi )->my, DISP_PRIO_MENU, CG_NAME_CHANGE_WND, 1 );
#else
#ifdef _CHARTITLE_
					StockDispBuffer(((WINDOW_DISP *)pActMenuWnd3->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd3->pYobi)->my + 27, DISP_PRIO_MENU, 55249, 1);
#else
#ifdef _CHARTITLE_STR_
					StockDispBuffer(((WINDOW_DISP *)pActMenuWnd3->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd3->pYobi)->my + 27, DISP_PRIO_MENU, 55249, 1);
#else
					StockDispBuffer(((WINDOW_DISP *)pActMenuWnd3->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd3->pYobi)->my, DISP_PRIO_MENU, CG_NAME_CHANGE_WND, 1);
#endif
#endif
#endif
#ifdef _CHARTITLE_
					extern BOOL 人物称号处理(ACTION * pct);
					if (人物称号处理(pActMenuWnd3)){
						pActMenuWnd3 = NULL;

					}
#else
#ifdef _CHARTITLE_STR_
					extern BOOL 人物称号处理(ACTION * pct);
					if (人物称号处理(pActMenuWnd3)){
						pActMenuWnd3 = NULL;

					}
#else
					GetKeyInputFocus(&shougouChange);
					// ????????
					if (mouse.onceState & MOUSE_LEFT_CRICK){
						// ??ＯＫ???
						if (HitDispNo == statusWndFontNo[7]){
							// ???????? 
							KeyboardReturn();
						}
						// ??????????
						if (HitDispNo == statusWndFontNo[8]){
							// ??????
							DeathAction(pActMenuWnd3);
							pActMenuWnd3 = NULL;
							// ????????
							GetKeyInputFocus(&MyChatBuffer);
							// ????????
							play_se(203, 320, 240);
						}
					}
					// ??????????????
					if (pActMenuWnd3 != NULL){
						// ???
						StockFontBuffer2(&shougouChange);

						// ?
						x = pActMenuWnd3->x + 20;
						y = pActMenuWnd3->y + 60;
						statusWndFontNo[7] = StockDispBuffer(x + 60, y + 7, DISP_PRIO_IME3, CG_OK_BTN, 2);
						statusWndFontNo[8] = StockDispBuffer(x + 170, y + 7, DISP_PRIO_IME3, CG_CANCEL_BTN, 2);
					}
#endif
#endif
				}
			}
			break;

		case 1:	// ??????????

			// ?????????
			if (pActMenuWnd == NULL){
				// ?????????
				pActMenuWnd = MakeWindowDisp(4, 0, 256, 456, 0, -1);
				// ??????
				for (i = 0; i < MENU_STATUS_0; i++) statusWndFontNo[i] = -2;
				for (i = 0; i < MENU_STATUS_0; i++) statusWndBtnFlag[i] = 0;
			}
			else{
				// ??????????????
				if (pActMenuWnd->hp > 0){
					// ????????????
					StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my, DISP_PRIO_MENU, CG_STATUS_WND_GROUP_WND, 1);
					// ????????
					if (mouse.onceState & MOUSE_LEFT_CRICK){
						// ???
						if (HitDispNo == statusWndFontNo[11]){
							// ??????
							DeathAction(pActMenuWnd);
							pActMenuWnd = NULL;
							statusWndNo = 0;
							// ????????
							play_se(203, 320, 240);
						}
#ifdef _TEAM_KICKPARTY
						for (i = 0; i < 5; i++){
							if (HitDispNo == statusWndFontNo[i + 12]){
								lssproto_KTEAM_send(sockfd, i);
								play_se(203, 320, 240);
							}
						}
#endif
					}
					// ????????
					if (pActMenuWnd != NULL){
						int selectFlag;	// ??????????
						// ?
						x = pActMenuWnd->x + 17;
						y = pActMenuWnd->y + 25;

						if (itemNo != -1 || jujutuNo != -1) selectFlag = 2;
						else selectFlag = FALSE;

						CenteringStr(pc.name, moji, CHAR_NAME_LEN);
						statusWndFontNo[0] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, moji, selectFlag); y += 19;
						sprintf_s(moji, "%4d", pc.mp);
#ifdef _NEWFONT_
						StockFontBuffer(x + 79, y, FONT_PRIO_FRONT, 0, moji, 0);
						sprintf_s(moji, "%d", pc.hp);
						StockFontBuffer(x + 142, y, FONT_PRIO_FRONT, 0, moji, 0);
						sprintf_s(moji, "%d",pc.maxHp);
						StockFontBuffer(x + 190, y, FONT_PRIO_FRONT, 0, moji, 0);
						y += 20;
#else
						StockFontBuffer(x + 77, y, FONT_PRIO_FRONT, 0, moji, 0);
						sprintf_s(moji, "%4d   %4d", pc.hp, pc.maxHp);
						StockFontBuffer(x + 142, y, FONT_PRIO_FRONT, 0, moji, 0); y += 20;
#endif
						
						for (i = 0; i < 5; i++){
							if (pet[i].useFlag == TRUE){
								if (pet[i].freeName[0] != NULL) CenteringStr(pet[i].freeName, moji, PET_NAME_LEN);
								else CenteringStr(pet[i].name, moji, PET_NAME_LEN);
#ifdef _NEWFONT_
								statusWndFontNo[i + 1] = StockFontBuffer(x, y-2, FONT_PRIO_FRONT, 0, moji, selectFlag); y += 20;
								sprintf_s(moji, "%d", pet[i].hp);
								StockFontBuffer(x + 142, y, FONT_PRIO_FRONT, 0, moji, 0);
								sprintf_s(moji, "%d", pet[i].maxHp);
								StockFontBuffer(x + 190, y, FONT_PRIO_FRONT, 0, moji, 0);
								y += 20;
#else
								statusWndFontNo[i + 1] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, moji, selectFlag); y += 20;
								sprintf_s(moji, "%4d   %4d", pet[i].hp, pet[i].maxHp);
								StockFontBuffer(x + 142, y, FONT_PRIO_FRONT, 0, moji, 0); y += 20;
#endif
							}
						}
						y = 268;
						for (i = 0; i < 5; i++){
							if (pc.id != party[i].id && party[i].useFlag == TRUE){
								CenteringStr(party[i].name, moji, CHAR_NAME_LEN);
								statusWndFontNo[i + 6] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, moji, selectFlag); y += 20;
#ifdef _TEAM_KICKPARTY
								//andy_add 队长踢人
								statusWndFontNo[i + 12] = StockDispBuffer(x + 187, y - 10, DISP_PRIO_IME3, CG_MAIL_WND_DELETE_BTN, 2);
#endif
								sprintf_s(moji, "%4d", party[i].mp);
								StockFontBuffer(x + 77, y, FONT_PRIO_FRONT, 0, moji, 0);
#ifdef _NEWFONT_
								sprintf_s(moji, "%d", party[i].hp);
								StockFontBuffer(x + 142, y, FONT_PRIO_FRONT, 0, moji, 0); 
								sprintf_s(moji, "%d", party[i].maxHp);
								StockFontBuffer(x + 190, y, FONT_PRIO_FRONT, 0, moji, 0); 
								y += 20;
#else
								sprintf_s(moji, "%4d %4d", party[i].hp, party[i].maxHp);
								StockFontBuffer(x + 142, y, FONT_PRIO_FRONT, 0, moji, 0); y += 20;
#endif
							}
						}
						statusWndFontNo[11] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, 441, DISP_PRIO_IME3, CG_RETURN_BTN, 2);
					}
				}
			}
			break;
#ifdef _NEWPANEL
		case 3:
			if( pActMenuWnd == NULL ){
				pActMenuWnd = MakeWindowDisp( 4, 0, 256, 456, 0, -1 );
				for( i = 0 ; i < MENU_STATUS_0 ; i++ ) statusWndFontNo[ i ] = -2;
				for( i = 0 ; i < MENU_STATUS_0 ; i++ ) statusWndBtnFlag[ i ] = 0;
			}else{
				if( pActMenuWnd->hp > 0 ){
					x = pActMenuWnd->x ; 
					y = pActMenuWnd->y ; 
#ifdef _MAG_MENU
					if( charDetailPage == 1 ) {
						StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx+28, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my-5, DISP_PRIO_MENU, 26528, 1 );
						StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx+28, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my-5, DISP_PRIO_IME3 + 1, 26555, 1 );
						if( MakeHitBox( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx+140, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my-135, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx+170, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my-60, DISP_PRIO_IME3) == TRUE)
						if( mouse.onceState & MOUSE_LEFT_CRICK )
							charDetailPage = 2;
						if( MakeHitBox( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx+140, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my-60, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx+170, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my+40, DISP_PRIO_IME3) == TRUE)
						if( mouse.onceState & MOUSE_LEFT_CRICK )
							charDetailPage = 3;
					}
					if( charDetailPage == 2 ) {
						StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx+28, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my-5, DISP_PRIO_MENU, 26528, 1 );
						StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx+28, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my-5, DISP_PRIO_IME3 + 1, 26556, 1 );
						if( MakeHitBox( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx+140, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my-215, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx+170, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my-136, DISP_PRIO_IME3) == TRUE)
						if( mouse.onceState & MOUSE_LEFT_CRICK )
							charDetailPage = 1;
						if( MakeHitBox( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx+140, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my-60, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx+170, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my+40, DISP_PRIO_IME3) == TRUE)
						if( mouse.onceState & MOUSE_LEFT_CRICK )
							charDetailPage = 3;
					}

					if( charDetailPage == 1 || charDetailPage == 2 ) {
						int index;
						for ( i = 0 ; i < 4 ; i ++ ) {
							for ( int j = 0 ; j < 4 ; j ++ ) {	
								index = j+i*4 + (charDetailPage-1)*(4*4);
								if( index >= 25 ) break;
								if( iCharAlchePlus[index] <= 0 ) continue;
								if( HitDispNo == StockDispBuffer( x + 45 + j * 60 , y + 96 + i * 57 , DISP_PRIO_IME3 + 1, iAlchePlusIcon[index], 2 ) ) {
									sprintf_s( moji , "%s 的材料鉴定熟度", sAlchePlus_list[index]); 
									StockFontBuffer( 25, 311, FONT_PRIO_FRONT, FONT_PAL_WHITE, moji, 0 );
									sprintf_s( moji , "%d %%", iCharAlchePlus[index] / 1000 ); 
									StockFontBuffer( 220, 311, FONT_PRIO_FRONT, FONT_PAL_WHITE, moji, 0 );

									sprintf_s( moji , "对于 %s 材料的鉴定熟度，可提高", sAlchePlus_list[index] ); 
									StockFontBuffer( 25, 340, FONT_PRIO_FRONT, FONT_PAL_WHITE, moji, 0 );
									sprintf_s( moji , "素材合成成功率。"); 	
									StockFontBuffer( 25, 360, FONT_PRIO_FRONT, FONT_PAL_WHITE, moji, 0 );

								}
							}
						}
					}


					if( charDetailPage == 3 ) {
						StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx+28, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my-5, DISP_PRIO_MENU, CG_SKILL_TABLE, 1 );
						StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx+28, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my-5, DISP_PRIO_IME3 + 1, 26486, 1 );
						if( MakeHitBox( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx+140, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my-215, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx+170, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my-136, DISP_PRIO_IME3) == TRUE)
						if( mouse.onceState & MOUSE_LEFT_CRICK )
							charDetailPage = 1;
						if( MakeHitBox( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx+140, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my-135, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx+170, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my-60, DISP_PRIO_IME3) == TRUE)
						if( mouse.onceState & MOUSE_LEFT_CRICK )
							charDetailPage = 2;

						for ( i = 0 ; i < 2 ; i ++ ) {
							for ( int j = 0 ; j < 4 ; j ++ ) {	
								if ( HitDispNo == StockDispBuffer( x + 45 + j * 60 , y + 102 + i * 57 , DISP_PRIO_IME3 + 1, DetailDescicon[ j + i * 4 ], 2 ) ) {
									sprintf_s( moji , "%s%d" , DetailDesc[j + i * 4] , atoi ( CharDetail[j + i * 4] ) ) ; 
									StockFontBuffer( 25, 310, FONT_PRIO_FRONT, FONT_PAL_WHITE, moji, 0 );
									if( i == 0 ){
										sprintf_s( moji , "对于%s精灵魔法的抗性，可减少受到", DetailDescchar[j] ); 
										StockFontBuffer( 25, 330, FONT_PRIO_FRONT, FONT_PAL_WHITE, moji, 0 );
										sprintf_s( moji , "%s精灵魔法的伤害。", DetailDescchar[j] ); 	
										StockFontBuffer( 25, 350, FONT_PRIO_FRONT, FONT_PAL_WHITE, moji, 0 );
									}
									else if( i == 1 ){
										sprintf_s( moji , "使用%s精灵魔法的熟练度，可增加使用", DetailDescchar[j] ); 
										StockFontBuffer( 25, 330, FONT_PRIO_FRONT, FONT_PAL_WHITE, moji, 0 );
										sprintf_s( moji , "%s精灵魔法时的威力。", DetailDescchar[j] ) ; 
										StockFontBuffer( 25, 350, FONT_PRIO_FRONT, FONT_PAL_WHITE, moji, 0 );
									}

								}
							}
						}

					}
#else
					StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx+8, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my-5, DISP_PRIO_MENU, CG_SKILL_TABLE, 1 );
					statusWndFontNo[ 13 ] = StockDispBuffer( x+139, y+423, DISP_PRIO_IME3, CG_TRADE_BACK_BTN, 2 );
#endif
					statusWndFontNo[ 13 ] = StockDispBuffer( x+215, y+437, DISP_PRIO_IME3, CG_TRADE_BACK_BTN, 2 );
#ifdef _SYUTEST3
					for ( i = 14 ; i < 22 ; i ++ ) 
						statusWndFontNo[ i ] = StockDispBuffer( x+139 , y+ 200 + ( i - 14 ) * 20 , DISP_PRIO_IME3, CG_TRADE_BACK_BTN, 2 );
#endif
#ifndef _MAG_MENU
					for ( i = 0 ; i < CHAR_MAX_DETAIL ; i ++ ) {
						sprintf_s( moji , "%s%d" , DetailDesc[i] , atoi ( CharDetail[i] ) ) ; 
						StockFontBuffer( 20 , 35 + i * 20  , FONT_PRIO_FRONT, 1, moji, 0 );		
					}
#endif
					if( mouse.onceState & MOUSE_LEFT_CRICK ){
						if( HitDispNo == statusWndFontNo[ 13 ] ){
							DeathAction( pActMenuWnd );
							pActMenuWnd = NULL;
							statusWndNo = 0;
							play_se( 202, 320, 240);
						}
#ifdef _SYUTEST3
						if( HitDispNo == statusWndFontNo[ 14 ] )
							lssproto_CHATROOM_send ( sockfd , "C|123456798" ) ; 
						if( HitDispNo == statusWndFontNo[ 15 ] )
							lssproto_CHATROOM_send ( sockfd , "D|123456798" ) ; 
						if( HitDispNo == statusWndFontNo[ 16 ] )
							lssproto_CHATROOM_send ( sockfd , "J|0" ) ; 
						if( HitDispNo == statusWndFontNo[ 17 ] )
							lssproto_CHATROOM_send ( sockfd , "L|123456798" ) ; 
						if( HitDispNo == statusWndFontNo[ 18 ] )
							lssproto_CHATROOM_send ( sockfd , "K|123456798" ) ; 
						if( HitDispNo == statusWndFontNo[ 19 ] )
							lssproto_CHATROOM_send ( sockfd , "M|123456798" ) ; 
						if( HitDispNo == statusWndFontNo[ 20 ] )
							lssproto_CHATROOM_send ( sockfd , "T|123456798" ) ; 
						if( HitDispNo == statusWndFontNo[ 21 ] )
							lssproto_CHATROOM_send ( sockfd , "B|123456798" ) ; 
#endif
					}
				}
			}
			break;
#endif
#ifdef _ITEM_JIGSAW
		case 4:
			if (JigsawImg[0] != -1){
				StockDispBuffer(190, 220, DISP_PRIO_MENU, 25251, 1);
				StockDispBuffer(189, 219, DISP_PRIO_MENU, JigsawImg[0], 0);
				if (JigsawImg[1] != -1)
					StockDispBuffer(103, 138, DISP_PRIO_IME3 + 1, JigsawImg[1], 1);
				if (JigsawImg[2] != -1)
					StockDispBuffer(189, 138, DISP_PRIO_IME3 + 1, JigsawImg[2], 1);
				if (JigsawImg[3] != -1)
					StockDispBuffer(275, 138, DISP_PRIO_IME3 + 1, JigsawImg[3], 1);
				if (JigsawImg[4] != -1)
					StockDispBuffer(103, 219, DISP_PRIO_IME3 + 1, JigsawImg[4], 1);
				if (JigsawImg[5] != -1)
					StockDispBuffer(189, 219, DISP_PRIO_IME3 + 1, JigsawImg[5], 1);
				if (JigsawImg[6] != -1)
					StockDispBuffer(275, 219, DISP_PRIO_IME3 + 1, JigsawImg[6], 1);
				if (JigsawImg[7] != -1)
					StockDispBuffer(103, 300, DISP_PRIO_IME3 + 1, JigsawImg[7], 1);
				if (JigsawImg[8] != -1)
					StockDispBuffer(189, 300, DISP_PRIO_IME3 + 1, JigsawImg[8], 1);
				if (JigsawImg[9] != -1)
					StockDispBuffer(275, 300, DISP_PRIO_IME3 + 1, JigsawImg[9], 1);
				DrawJigsawFlag = true;
			}
			break;
#endif
		}

	}

	//? ?????????? ********************************************************/

	if (MenuToggleFlag & JOY_CTRL_P){
#ifdef _STREET_VENDOR
		if (pc.iOnStreetVendor == 1 || sStreetVendorBuyBtn == 2){
			StockChatBufferLine("摆摊中不得更换宠物状态", FONT_PAL_RED);
			MenuToggleFlag ^= JOY_CTRL_P;
			// 为了不执行宠物视窗所以设定为 3,不作事
			petWndNo = 3;
		}
#endif
#ifdef _THEATER
		if (pc.iTheaterMode > 0)
		{
			if (pc.iTheaterMode & 0x00000002)
				StockChatBufferLine("请专心表演", FONT_PAL_RED);
			else if (pc.iTheaterMode & 0x00000001)
				StockChatBufferLine("请专心看表演", FONT_PAL_RED);
			MenuToggleFlag ^= JOY_CTRL_P;
			// 为了不执行宠物视窗所以设定为 3,不作事
			petWndNo = 3;
		}
#endif
#ifdef _NPC_DANCE
		if (pc.iDanceMode)
		{
			StockChatBufferLine("特殊状态无法查看宠物", FONT_PAL_RED);
			MenuToggleFlag ^= JOY_CTRL_P;
			petWndNo = 3;
		}
#endif
		// ????????
		switch (petWndNo){

		case 0:	// ??????????

			// ?????????
			if (pActMenuWnd == NULL){
				// ?????????
				pActMenuWnd = MakeWindowDisp(4, 4, 272, 320, 0, -1);

				// ??????
				for (i = 0; i < MENU_PET_0; i++) petWndFontNo[i] = -2;
				// ????
				BattlePetReceivePetNo = -1;
#ifdef _DROPPETWND					// (可开放) Syu ADD 丢弃宠物确认
				DropPetWndflag = false;
				DropI = -1;
#endif
			}
			else{
				// ??????????????
				if (pActMenuWnd->hp > 0){
					// ??????????
					StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my, DISP_PRIO_MENU, CG_PET_WND_VIEW, 1);

					// ??????????
					for (i = 0; i < 5; i++){
						// ????????????
						if (pet[i].useFlag == FALSE){
							// ??????
							if (i == pc.battlePetNo)
								pc.battlePetNo = -1;
							// ?????
							if (i == pc.mailPetNo)
								pc.mailPetNo = -1;
							// ??????????
							pc.selectPetNo[i] = FALSE;
						}
					}
					// ???????????
					BattlePetStMenCnt = 0;
					// ????????
					for (i = 0; i < 5; i++){
						if (pc.selectPetNo[i] == TRUE) BattlePetStMenCnt++;
					}

					// ????????
					if (mouse.onceState & MOUSE_LEFT_CRICK){
						// ????????
						for (i = 0; i < 5; i++){
							// ???????????????????
							if (pet[i].useFlag == TRUE && BattlePetReceivePetNo != i){
								// ??????
								if (HitDispNo == petWndFontNo[i]){
									// ride Pet
#if 1
									if (i == pc.mailPetNo){
										pc.mailPetNo = -1;
										// shan
										if ((bNewServer & 0xf000000) == 0xf000000)
											lssproto_PETST_send(sockfd, i, 0);
#ifndef  _RIDEPET_
										checkRidePet(i);
#endif
										play_se(217, 320, 240); // ?????
									}
									else
										// when ride Pet
									if (i == pc.ridePetNo && pc.graNo != SPR_pet021
										&& pc.graNo != 100362){//金飞
										char buf[64];
										sprintf_s(buf, "R|P|-1");
										if (bNewServer)
											lssproto_FM_send(sockfd, buf);
										else
											lssproto_FM_send(sockfd, buf);
										play_se(217, 320, 240); // ?????
										// shan
										if ((bNewServer & 0xf000000) == 0xf000000)
											lssproto_PETST_send(sockfd, i, 0);
									}
									else
										// ??
									if (i == pc.battlePetNo && BattlePetReceiveFlag == FALSE){
										// ?????
										if (bNewServer)
											lssproto_KS_send(sockfd, -1);
										else
											old_lssproto_KS_send(sockfd, -1);
										// ??????
										BattlePetReceiveFlag = TRUE;
										// ???????????
										BattlePetReceivePetNo = i;

										// ?????
										pc.selectPetNo[i] = FALSE;
										BattlePetStMenCnt--; // ?????????
										// ?????????
										if (pc.mailPetNo == -1){
											// ??????
											pc.mailPetNo = i;
											// shan
											if ((bNewServer & 0xf000000) == 0xf000000)
												lssproto_PETST_send(sockfd, i, 4);
										}
										else{
#ifndef  _RIDEPET_
										checkRidePet(i);
#endif
											// shan
											if ((bNewServer & 0xf000000) == 0xf000000)
												lssproto_PETST_send(sockfd, i, 0);
										}
										play_se(217, 320, 240); // ?????
									}
									else
										// ??????
									if (pc.selectPetNo[i] == TRUE){
										// ???????
										if (pc.battlePetNo == -1 && BattlePetReceiveFlag == FALSE){
											// ?????
											if (pet[i].hp > 0){
												// ?
												if (bNewServer)
													lssproto_KS_send(sockfd, i);
												else
													old_lssproto_KS_send(sockfd, i);
												// ??????
												BattlePetReceiveFlag = TRUE;
												// ???????????
												BattlePetReceivePetNo = i;
												play_se(217, 320, 240); // ?????
											}
											else{
												play_se(220, 320, 240); // ???
											}
										}
										else{
											// ?????????
											if (pc.mailPetNo == -1){
												pc.mailPetNo = i;
												// shan
												if ((bNewServer & 0xf000000) == 0xf000000)
													lssproto_PETST_send(sockfd, i, 4);
											}
											// ride Pet
											else
											{
#ifndef  _RIDEPET_
												checkRidePet(i);
#endif
												// shan
												if ((bNewServer & 0xf000000) == 0xf000000)
													lssproto_PETST_send(sockfd, i, 0);
											}

											pc.selectPetNo[i] = FALSE;
											BattlePetStMenCnt--;
											play_se(217, 320, 240);
										}

									}
									else
									if (pc.selectPetNo[i] == FALSE){
										// ?????????
										if (BattlePetStMenCnt < 4){
											// ???????
											pc.selectPetNo[i] = TRUE;
											BattlePetStMenCnt++; // ????????
											play_se(217, 320, 240); // ?????
											// shan
											if ((bNewServer & 0xf000000) == 0xf000000)
												lssproto_PETST_send(sockfd, i, 1);
										}
										else{
											// ?????????
											if (pc.mailPetNo == -1){
												pc.mailPetNo = i;
												play_se(217, 320, 240); // ?????
												// shan
												if ((bNewServer & 0xf000000) == 0xf000000)
													lssproto_PETST_send(sockfd, i, 4);
											}
											// ride Pet
											else
											{
#ifndef  _RIDEPET_
												checkRidePet(i);
#endif
											}
										}
									}
#endif

								}
								// ?????????
								if (HitFontNo == petWndFontNo[i + 5]){
									petStatusNo = i; // ??????
									petWndNo = 1;
									// ??????
									DeathAction(pActMenuWnd);
									pActMenuWnd = NULL;
									// ????????
									play_se(202, 320, 240);
								}

							}
						}
						// ??
						if (HitDispNo == petWndFontNo[10]){
							// ??????
							DeathAction(pActMenuWnd);
							pActMenuWnd = NULL;
							// ??????????
							MenuToggleFlag ^= JOY_CTRL_P;
							// ????????
							play_se(203, 320, 240);
							// ????????

							saveUserSetting();
						}
						// ????
						if (HitDispNo == petWndFontNo[11]){
							petStatusNo = 0; // ??????
							// ??????????
							for (i = 0; i < 5; i++){
								// ???????????
								if (pet[i].useFlag == TRUE){
									petStatusNo = i;
									break;
								}
							}
							petWndNo = 1;
							// ??????
							DeathAction(pActMenuWnd);
							pActMenuWnd = NULL;
							// ????????
							play_se(202, 320, 240);
						}
#ifdef _DROPPETWND					// (可开放) Syu ADD 丢弃宠物确认
						if (HitDispNo == petWndFontNo[21]) {
							i = DropI;
							DropI = -1;
							lssproto_DP_send(sockfd, nowGx, nowGy, i);
							if (pc.selectPetNo[i] == TRUE) {
								pc.selectPetNo[i] = FALSE;
								BattlePetStMenCnt--;
							}
							if (i == pc.battlePetNo)
								lssproto_KS_send(sockfd, -1);
							if (pc.mailPetNo == i) {
								pc.mailPetNo = -1;
							}
							play_se(217, 320, 240);
							DropPetWndflag = false;
						}
						if (HitDispNo == petWndFontNo[22]) {
							DropPetWndflag = false;
							DropI = -1;
							play_se(217, 320, 240);
						}
#endif
					}
					// ????????
					if (mouse.onceState & MOUSE_RIGHT_CRICK){
						// ????????
						for (i = 0; i < 5; i++){
							// ???????????
							if (pet[i].useFlag == TRUE && eventWarpSendFlag == FALSE){
								// ??????
								if (HitFontNo == petWndFontNo[i + 5]){
									// ?????????? or ride Pet
									if (ItemMixPetNo != i
										&& pc.ridePetNo != i)
									{
#ifdef _DROPPETWND					// (可开放) Syu ADD 丢弃宠物确认
										DropPetWndflag = true;
										DropI = i;
#else


										// ??????
										if( bNewServer)
											lssproto_DP_send( sockfd, nowGx, nowGy, i );
										else
											old_lssproto_DP_send( sockfd, nowGx, nowGy, i );
										// ??????
										if( pc.selectPetNo[ i ] == TRUE ){
											pc.selectPetNo[ i ] = FALSE; // ????
											BattlePetStMenCnt--; // ?????????
										}
										// ??
										if( i == pc.battlePetNo ){
											// ?????
											if( bNewServer)
												lssproto_KS_send( sockfd, -1 );
											else
												old_lssproto_KS_send( sockfd, -1 );
										}
										// ?????
										if( pc.mailPetNo == i ){
											pc.mailPetNo = -1;
										}
										play_se( 217, 320, 240 ); // ?????
#endif
									}
									else
									{
										// ???
										play_se(220, 320, 240);
									}
								}
							}
						}
					}
#ifdef _DROPPETWND					// (可开放) Syu ADD 丢弃宠物确认
					if (DropPetWndflag == true) {
						StockFontBuffer(245, 220, FONT_PRIO_AFRONT, 3, "确定要丢出你的宠物吗？", 0); y += 40;
						StockDispBuffer(320, 240, DISP_PRIO_YES_NO_WND, CG_DROPWND, 0);
						petWndFontNo[21] = StockDispBuffer(320, 240, DISP_PRIO_YES_NO_BTN, CG_COMMON_YES_BTN, 2);
						petWndFontNo[22] = StockDispBuffer(320, 240, DISP_PRIO_YES_NO_BTN, CG_COMMON_NO_BTN, 2);
					}
#endif
					// ????????
					if (pActMenuWnd != NULL){

						int flag = FALSE;
						int color;
						int btnNo;
						int atrFlag = FALSE;
						int atrGraNo[4];

						x = pActMenuWnd->x + 16 + 50, y = pActMenuWnd->y + 31;
						// ??????
						for (i = 0; i < 5; i++){
							// ???????????
							if (pet[i].useFlag == TRUE){

								color = FONT_PAL_WHITE;		// ?????
								btnNo = 0;					// ???????
								// ???????
								if (pc.selectPetNo[i] == TRUE){
									color = FONT_PAL_AQUA;
									btnNo = 2;
								}
								// ??
								if (i == pc.battlePetNo){
									color = FONT_PAL_YELLOW;
									btnNo = 1;
								}
								// ?????
								if (i == pc.mailPetNo){
									color = FONT_PAL_GREEN;
									btnNo = 3;
								}
								// ride Pet
								if (i == pc.ridePetNo){
									color = FONT_PAL_YELLOW;
									btnNo = 195;
								}
								if (pet[i].freeName[0] != NULL){
#ifdef _NEWFONT_
									sprintf(moji,"    %-26s",pet[i].freeName);
									//CenteringStr(pet[i].freeName, moji, PET_NAME_LEN);
#else
									CenteringStr(pet[i].freeName, moji, PET_NAME_LEN);
#endif
									
								}
								else{
#ifdef _NEWFONT_
									sprintf(moji,"    %-26s",pet[i].name);
									//CenteringStr(pet[i].name, moji, PET_NAME_LEN);
#else
									CenteringStr(pet[i].name, moji, PET_NAME_LEN);
#endif
								}
								petWndFontNo[i + 5] = StockFontBuffer(x + 3, y, FONT_PRIO_FRONT, color, moji, 2); y += 24;
#ifdef _SHOWPETTRN_
								sprintf(moji, "%d转", pet[i].trn);
								StockFontBuffer(x + 122, y - 24, FONT_PRIO_FRONT, color, moji, 2);
#endif
								atrFlag = FALSE;
								// 
								if (pet[i].earth > 0){	// 佋
									// ??
									if (pet[i].earth > 50) atrGraNo[atrFlag] = CG_ATR_ICON_EARTH_BIG;
									// ??
									else atrGraNo[atrFlag] = CG_ATR_ICON_EARTH_SML;
									atrFlag++; // ?????
								}
								if (pet[i].water > 0){	// ?
									// ??
									if (pet[i].water > 50) atrGraNo[atrFlag] = CG_ATR_ICON_WATER_BIG;
									// ??
									else atrGraNo[atrFlag] = CG_ATR_ICON_WATER_SML;
									atrFlag++; // ?????
								}
								if (pet[i].fire > 0){	// ?
									// ??
									if (pet[i].fire > 50) atrGraNo[atrFlag] = CG_ATR_ICON_FIRE_BIG;
									// ??
									else atrGraNo[atrFlag] = CG_ATR_ICON_FIRE_SML;
									atrFlag++; // ?????
								}
								if (pet[i].wind > 0){	// ?
									// ??
									if (pet[i].wind > 50) atrGraNo[atrFlag] = CG_ATR_ICON_WIND_BIG;
									// ??
									else atrGraNo[atrFlag] = CG_ATR_ICON_WIND_SML;
									atrFlag++; // ?????
								}
								// ???
								if (atrFlag > 0) StockDispBuffer(pActMenuWnd->x + 228, y - 16, DISP_PRIO_IME3, atrGraNo[0], 0);
								// ??
								if (atrFlag > 1) StockDispBuffer(pActMenuWnd->x + 228 + 16, y - 16, DISP_PRIO_IME3, atrGraNo[1], 0);

								petWndFontNo[i] = StockDispBuffer(x - 27, y - 14, DISP_PRIO_IME3, CG_PET_WND_REST_BTN + btnNo, 2);

#ifdef  _RIDEPET_
								if (pet[i].rideflg == 1 && pet[i].ai == 100){
									if ((i == pc.ridePetNo) || pc.ridePetNo < 0){
										int rideid = StockDispBuffer(x - 27, y + 11, DISP_PRIO_IME3, 55257, 2);
										if (mouse.onceState & MOUSE_LEFT_CRICK){
											if (HitDispNo == rideid){
												checkRidePet(i);
											}
										}
									}
								}
#endif
#ifdef _NEWFONT_
								sprintf_s(moji, "%3d", pet[i].level);
								StockFontBuffer(x + 15 + 27, y, FONT_PRIO_FRONT, color, moji, 0);

								sprintf_s(moji, "%4d",pet[i].hp);
								StockFontBuffer(x + 70 + 32, y, FONT_PRIO_FRONT, color, moji, 0);
								sprintf_s(moji, "%4d", pet[i].maxHp);
								StockFontBuffer(x + 130 + 27, y, FONT_PRIO_FRONT, color, moji, 0);
								 y += 27;
#else
								sprintf_s(moji, "%3d     %4d   %4d", pet[i].level, pet[i].hp, pet[i].maxHp);
								StockFontBuffer(x + 26 + 27, y, FONT_PRIO_FRONT, color, moji, 0); y += 27;
#endif
								flag = TRUE;
							}
						}
						// ????????
						if (flag != TRUE){
							// ??
							petWndFontNo[10] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, pActMenuWnd->y + 299, DISP_PRIO_IME3, CG_CLOSE_BTN, 2);
							StockFontBuffer(x + 10, y, FONT_PRIO_FRONT, 0, "你没有宠物", 0); y += 40;
						}
						else{
							// ??
							petWndFontNo[10] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx + 56, pActMenuWnd->y + 299, DISP_PRIO_IME3, CG_CLOSE_BTN, 2);
							petWndFontNo[11] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx - 56, pActMenuWnd->y + 299, DISP_PRIO_IME3, CG_PET_WND_STATUS_BTN, 2);
						}
					}
				}
			}
			break;

		case 1: // ??????????
			if (pActMenuWnd == NULL){
				// ?????????

				pActMenuWnd = MakeWindowDisp(4, 4, 272, 332, 0, -1);
#ifdef _PETBLESS_
				祝福窗口开关 = 0;
				memset(祝福窗口内容, 0, 128);
#endif
				// ??????
				for (i = 0; i < MENU_PET_0; i++) petWndFontNo[i] = -2;

			}
			else{
				// ??????????????
				if (pActMenuWnd->hp > 0){
					// ?????????
					StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my, DISP_PRIO_MENU, CG_PET_WND_DETAIL, 1);
					// ????????????????
					if (pActPet == NULL)
					{
#ifdef _LIZARDPOSITION			   // (可开放) Syu ADD 修正龙蜥显示位置过低
						if ((pet[petStatusNo].graNo == 101493) || (pet[petStatusNo].graNo == 101494) ||
							(pet[petStatusNo].graNo == 101495) || (pet[petStatusNo].graNo == 101496))
						{
							pActPet = MakeAnimDisp(pActMenuWnd->x + 220, pActMenuWnd->y + 124, pet[petStatusNo].graNo, ANIM_DISP_PET);
						}
						else
							pActPet = MakeAnimDisp(pActMenuWnd->x + 200, pActMenuWnd->y + 144, pet[petStatusNo].graNo, ANIM_DISP_PET);
#else
						pActPet = MakeAnimDisp( pActMenuWnd->x + 200, pActMenuWnd->y + 144, pet[ petStatusNo ].graNo, ANIM_DISP_PET );
#endif				
					}
#ifdef _PET_SKINS
					originalPetSkin = pet[petStatusNo].graNo;
#endif

					// ????????
					if (mouse.onceState & MOUSE_LEFT_CRICK){
						// ?????????????
						if (HitDispNo == petWndFontNo[0]){
							DeathAction(pActMenuWnd);
							pActMenuWnd = NULL;
							// ????
							DeathAction(pActPet);
							pActPet = NULL;
							// ?????????????
							petWndNo = 0;
							// ???????
							DeathAction(pActMenuWnd3);
							pActMenuWnd3 = NULL;
							// ????????
							GetKeyInputFocus(&MyChatBuffer);
							// ????????
							play_se(203, 320, 240);
						}
						// ????????
						if (HitDispNo == petWndFontNo[4]){
							// ???????
							petWndNo = 2;
							DeathAction(pActMenuWnd);
							pActMenuWnd = NULL;
							// ????
							DeathAction(pActPet);
							pActPet = NULL;
							// ?????????????
							petWndNo = 2;
							// ???????
							DeathAction(pActMenuWnd3);
							pActMenuWnd3 = NULL;
							// ????????
							GetKeyInputFocus(&MyChatBuffer);
							// ????????
							play_se(202, 320, 240);
						}
					}
					// ??????????
					if (HitDispNo == petWndFontNo[1] || joy_con[0] & JOY_A){
						// ????????
						if (mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[0] & JOY_A){
							// ??????????
							if (pActMenuWnd3 != NULL){
								// ??????
								DeathAction(pActMenuWnd3);
								pActMenuWnd3 = NULL;
								// ????????
								GetKeyInputFocus(&MyChatBuffer);
								// ????????
								play_se(203, 320, 240);
							}
							while (1){
								petStatusNo--;
								// ????????
								if (petStatusNo <= -1) petStatusNo = 4;
								if (pet[petStatusNo].useFlag == TRUE) break;
							}
							// ?????????
#ifdef _LIZARDPOSITION			   // (可开放) Syu ADD 修正龙蜥显示位置过低
							if ((pet[petStatusNo].graNo == 101493) || (pet[petStatusNo].graNo == 101494) ||
								(pet[petStatusNo].graNo == 101495) || (pet[petStatusNo].graNo == 101496))
							{
								pActPet->x = pActMenuWnd->x + 220;
								pActPet->y = pActMenuWnd->y + 124;
							}
							else
							{
								pActPet->x = pActMenuWnd->x + 200;
								pActPet->y = pActMenuWnd->y + 144;
							}
#endif
							pActPet->anim_chr_no = pet[petStatusNo].graNo;
#ifdef _PET_SKINS
							petChange(pActPet->anim_chr_no);
#endif
							// ?????
							play_se(217, 320, 240);
							petWndBtnFlag[1] = TRUE;
						}
						// ?????????
						if ((mouse.state & MOUSE_LEFT_CRICK || joy_con[0] & JOY_A) && petWndBtnFlag[1] == TRUE){
							// ????
							petWndBtnFlag[1] = TRUE;
						}
						else{
							// ???
							petWndBtnFlag[1] = FALSE;
						}
					}
					else{
						// ???
						petWndBtnFlag[1] = FALSE;
					}
					// ???????????
					if (HitDispNo == petWndFontNo[2] || joy_con[0] & JOY_B){
						// ????????
						if (mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[0] & JOY_B){
							// ??????????
							if (pActMenuWnd3 != NULL){
								// ??????
								DeathAction(pActMenuWnd3);
								pActMenuWnd3 = NULL;
								// ????????
								GetKeyInputFocus(&MyChatBuffer);
								// ????????
								play_se(203, 320, 240);
							}
							while (1){
								petStatusNo++;
								// ????????
								if (petStatusNo >= 5) petStatusNo = 0;
								if (pet[petStatusNo].useFlag == TRUE) break;
							}
							// ????????
#ifdef _LIZARDPOSITION			   // (可开放) Syu ADD 修正龙蜥显示位置过低
							if ((pet[petStatusNo].graNo == 101493) || (pet[petStatusNo].graNo == 101494) ||
								(pet[petStatusNo].graNo == 101495) || (pet[petStatusNo].graNo == 101496))
							{
								pActPet->x = pActMenuWnd->x + 220;
								pActPet->y = pActMenuWnd->y + 124;
							}
							else
							{
								pActPet->x = pActMenuWnd->x + 200;
								pActPet->y = pActMenuWnd->y + 144;
							}
#endif
							pActPet->anim_chr_no = pet[petStatusNo].graNo;
#ifdef _PET_SKINS
							petChange(pActPet->anim_chr_no);
#endif
							// ?????
							play_se(217, 320, 240);
							petWndBtnFlag[2] = TRUE;
						}
						// ?????????
						if ((mouse.state & MOUSE_LEFT_CRICK || joy_con[0] & JOY_B) && petWndBtnFlag[2] == TRUE){
							// ????
							petWndBtnFlag[2] = TRUE;
						}
						else{
							// ???
							petWndBtnFlag[2] = FALSE;
						}
					}
					else{
						// ???
						petWndBtnFlag[2] = FALSE;
					}
					// ????
					if (HitDispNo == petWndFontNo[3]){
						// ????????
						if (mouse.onceState & MOUSE_LEFT_CRICK){
							// ???????
							if (pet[petStatusNo].changeNameFlag == TRUE){
								if (pActMenuWnd3 == NULL){
									// ?????????
									pActMenuWnd3 = MakeWindowDisp(4, 4 + 280 + 56, 272, 88, 0, -1, 0);
									// ????????
									play_se(202, 320, 240);
									// ????
									petNameChange.buffer[0] = NULL;
									petNameChange.cnt = 0;
									petNameChange.cursor = 0;
									// ????
									petNameChange.len = 16;
									// ???
									petNameChange.color = 0;
									// ????
									petNameChange.x = pActMenuWnd3->x + 22;
									petNameChange.y = pActMenuWnd3->y + 25;
									// ??????
									petNameChange.fontPrio = FONT_PRIO_FRONT;
								}
								else{	// ???????????
									// ??????
									DeathAction(pActMenuWnd3);
									pActMenuWnd3 = NULL;
									// ????????
									GetKeyInputFocus(&MyChatBuffer);
									// ????????
									play_se(203, 320, 240);
								}
								petWndBtnFlag[3] = TRUE;

							}
							else{
								// ???
								play_se(220, 320, 240);
							}
						}
						// ?????????
						if (mouse.state & MOUSE_LEFT_CRICK && petWndBtnFlag[3] == TRUE){
							// ????
							petWndBtnFlag[3] = TRUE;
						}
						else{
							// ???
							petWndBtnFlag[3] = FALSE;
						}
					}
					else{
						// ???
						petWndBtnFlag[3] = FALSE;
					}
#ifndef _PET_SKINS
					if (HitDispNo == petWndFontNo[7]){
						// ????????
						if (pet[petStatusNo].graNo != 100451 && pet[petStatusNo].graNo != 100432 &&
							pet[petStatusNo].graNo != 101280 && pet[petStatusNo].graNo != 101281 &&
							pet[petStatusNo].graNo != 100015 && pet[petStatusNo].graNo != 100018 &&
							pet[petStatusNo].graNo != 101279 && pet[petStatusNo].graNo != 100401 &&
							pet[petStatusNo].graNo != 101414 && pet[petStatusNo].graNo != 101167 &&
							pet[petStatusNo].graNo != 101172 && pet[petStatusNo].graNo != 102011 &&
							pet[petStatusNo].graNo != 102012)	// fix 哪些宠物不能照宠照
						if (mouse.onceState & MOUSE_LEFT_CRICK){
#ifdef _TAIKEN			
							// ???
							play_se( 220, 320, 240 );
							// ???????
							sprintf_s( moji,"体验版不能选择！" );
							// ??????????????????
							StockChatBufferLine( moji, FONT_PAL_WHITE );
#else
							int tblNo = pet[petStatusNo].graNo - 100250; // ??????
#if defined(__ALBUM_47)
							if (tblNo > 1800){					//小恶魔
								tblNo -= 1208;
							}
							else if (tblNo > 1755){					//间隔南瓜魔王后的 狐猴
								tblNo -= 1201;
							}
							else if (tblNo > 1739){			//飞蛇
								tblNo -= 1200;
							}
							else if (tblNo > 1686){			//8.0宠物
								tblNo -= 1175;
							}
							else if (tblNo > 1641){			//鸡年兽4 甲虫2	8.0第一次整合测试
								tblNo -= 1167;
							}
							else if (tblNo > 1635){   //麒麟
								tblNo -= 1148;
							}
							else if (tblNo > 1634){	//猫女1 猫女2
								tblNo -= 1149;
							}
							else if (tblNo > 1628){	//火蚁1 火蚁2 牛人1 牛人2
								tblNo -= 1147;
							}
							else if (tblNo > 1615){//布里萨尔 蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
								tblNo -= 1145;
							}
							else if (tblNo > 1568){//机人龙
								tblNo -= 1103;
							}
							else if (tblNo > 1564){//黄色飞龙
								tblNo -= 1101;
							}
							else if (tblNo > 1516){
								tblNo -= 1055;
							}
							else if (tblNo == 1516){//修正乌力王
								tblNo = 455;
							}
							else if (tblNo > 1509){
								tblNo -= 1054;
							}
							else if (tblNo == 1497){
								tblNo = 395;
							}
							else if (tblNo > 1495){
								tblNo -= 1044;
							}
							else if (tblNo > 1490){
								tblNo -= 1043;
							}
							else if (tblNo > 1485){
								tblNo -= 1040;
							}
							else if (tblNo > 1454) {
								tblNo -= 1034;
							}
							else if (tblNo == 1214) {
								tblNo = 331;
							}
							else if (tblNo == 1217) {
								tblNo = 328;
							}
							else if (tblNo > 1361){
								tblNo -= 952;
							}
							else if (tblNo > 1356){
								tblNo -= 999;
							}
							else if (tblNo > 1281){
								tblNo -= 919;
							}
							else if (tblNo > 1239){
								tblNo -= 889;
							}
							else if (tblNo > 1186){
								tblNo -= 886;
							}
							else if (tblNo > 1173){
								tblNo -= 877;
							}
							else if (tblNo > 1172){
								tblNo -= 877;
							}
							else if (tblNo > 1161){
								tblNo -= 868;
							}
							else if (tblNo > 930){
								tblNo -= 745;
							}
							else if (tblNo == 927){
								tblNo -= 634;
							}
							else if (tblNo > 929){
								tblNo -= 644;
							}
							else if (tblNo > 927){
								tblNo -= 643;
							}
							else if (tblNo > 900){
								tblNo -= 621;
							}
							else if (tblNo > 180){
								tblNo -= (333 + 56);
							}
#elif defined(__ALBUM_46)
							if( tblNo > 1800 ){					//小恶魔
								tblNo -= 1208;
							}else if( tblNo > 1755 ){					//间隔南瓜魔王后的 狐猴
								tblNo -= 1201 ;
							}else if (tblNo > 1739 ){			//飞蛇
								tblNo -= 1200;
							}else if (tblNo > 1686 ){			//8.0宠物
								tblNo -= 1175;
							}else if (tblNo > 1641 ){			//鸡年兽4 甲虫2	8.0第一次整合测试
								tblNo -= 1167;
							}else if (tblNo > 1635 ){   //麒麟
								tblNo -= 1148;
							}else if ( tblNo > 1634 ){	//猫女1 猫女2
								tblNo -= 1149;
							}else if ( tblNo > 1628 ){	//火蚁1 火蚁2 牛人1 牛人2
								tblNo -= 1147;
							}else if( tblNo > 1615 ){//布里萨尔 蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
								tblNo -= 1145;
							}else if( tblNo > 1568 ){//机人龙
								tblNo -= 1103;
							}else if( tblNo > 1564 ){//黄色飞龙
								tblNo -= 1101;
							}else if( tblNo > 1516 ){
								tblNo -= 1055;
							}else if ( tblNo == 1516 ){//修正乌力王
								tblNo = 455 ;
							}else if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}
#elif defined(__ALBUM_45)
							if( tblNo > 1800 ){					//小恶魔
								tblNo -= 1208;
							}else if( tblNo > 1755 ){					//间隔南瓜魔王后的 狐猴
								tblNo -= 1201 ;
							}else if (tblNo > 1739 ){			//飞蛇
								tblNo -= 1200;
							}else if (tblNo > 1686 ){			//8.0宠物
								tblNo -= 1175;
							}else if (tblNo > 1641 ){			//鸡年兽4 甲虫2	8.0第一次整合测试
								tblNo -= 1167;
							}else if (tblNo > 1635 ){   //麒麟
								tblNo -= 1148;
							}else if ( tblNo > 1634 ){	//猫女1 猫女2
								tblNo -= 1149;
							}else if ( tblNo > 1628 ){	//火蚁1 火蚁2 牛人1 牛人2
								tblNo -= 1147;
							}else if( tblNo > 1615 ){//布里萨尔 蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
								tblNo -= 1145;
							}else if( tblNo > 1568 ){//机人龙
								tblNo -= 1103;
							}else if( tblNo > 1564 ){//黄色飞龙
								tblNo -= 1101;
							}else if( tblNo > 1516 ){
								tblNo -= 1055;
							}else if ( tblNo == 1516 ){//修正乌力王
								tblNo = 455 ;
							}else if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}
#elif defined(__ALBUM_44)
							if( tblNo > 1800 ){					//小恶魔
								tblNo -= 1208;
							}else if( tblNo > 1755 ){					//间隔南瓜魔王后的 狐猴
								tblNo -= 1201 ;
							}else if (tblNo > 1739 ){			//飞蛇
								tblNo -= 1200;
							}else if (tblNo > 1686 ){			//8.0宠物
								tblNo -= 1175;
							}else if (tblNo > 1641 ){			//鸡年兽4 甲虫2	8.0第一次整合测试
								tblNo -= 1167;
							}else if (tblNo > 1635 ){   //麒麟
								tblNo -= 1148;
							}else if ( tblNo > 1634 ){	//猫女1 猫女2
								tblNo -= 1149;
							}else if ( tblNo > 1628 ){	//火蚁1 火蚁2 牛人1 牛人2
								tblNo -= 1147;
							}else if( tblNo > 1615 ){//布里萨尔 蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
								tblNo -= 1145;
							}else if( tblNo > 1568 ){//机人龙
								tblNo -= 1103;
							}else if( tblNo > 1564 ){//黄色飞龙
								tblNo -= 1101;
							}else if( tblNo > 1516 ){
								tblNo -= 1055;
							}else if ( tblNo == 1516 ){//修正乌力王
								tblNo = 455 ;
							}else if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}
#elif defined(__ALBUM_43)
							if( tblNo > 1800 ){					//小恶魔
								tblNo -= 1208;
							}else if( tblNo > 1755 ){					//间隔南瓜魔王后的 狐猴
								tblNo -= 1201 ;
							}else if (tblNo > 1739 ){			//飞蛇
								tblNo -= 1200;
							}else if (tblNo > 1686 ){			//8.0宠物
								tblNo -= 1175;
							}else if (tblNo > 1641 ){			//鸡年兽4 甲虫2	8.0第一次整合测试
								tblNo -= 1167;
							}else if (tblNo > 1635 ){   //麒麟
								tblNo -= 1148;
							}else if ( tblNo > 1634 ){	//猫女1 猫女2
								tblNo -= 1149;
							}else if ( tblNo > 1628 ){	//火蚁1 火蚁2 牛人1 牛人2
								tblNo -= 1147;
							}else if( tblNo > 1615 ){//布里萨尔 蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
								tblNo -= 1145;
							}else if( tblNo > 1568 ){//机人龙
								tblNo -= 1103;
							}else if( tblNo > 1564 ){//黄色飞龙
								tblNo -= 1101;
							}else if( tblNo > 1516 ){
								tblNo -= 1055;
							}else if ( tblNo == 1516 ){//修正乌力王
								tblNo = 455 ;
							}else if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}
#elif defined(__ALBUM_42)
							if( tblNo > 1800 ){					//小恶魔
								tblNo -= 1208;
							}else if( tblNo > 1755 ){					//间隔南瓜魔王后的 狐猴
								tblNo -= 1201 ;
							}else if (tblNo > 1739 ){			//飞蛇
								tblNo -= 1200;
							}else if (tblNo > 1686 ){			//8.0宠物
								tblNo -= 1175;
							}else if (tblNo > 1641 ){			//鸡年兽4 甲虫2	8.0第一次整合测试
								tblNo -= 1167;
							}else if (tblNo > 1635 ){   //麒麟
								tblNo -= 1148;
							}else if ( tblNo > 1634 ){	//猫女1 猫女2
								tblNo -= 1149;
							}else if ( tblNo > 1628 ){	//火蚁1 火蚁2 牛人1 牛人2
								tblNo -= 1147;
							}else if( tblNo > 1615 ){//布里萨尔 蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
								tblNo -= 1145;
							}else if( tblNo > 1568 ){//机人龙
								tblNo -= 1103;
							}else if( tblNo > 1564 ){//黄色飞龙
								tblNo -= 1101;
							}else if( tblNo > 1516 ){
								tblNo -= 1055;
							}else if ( tblNo == 1516 ){//修正乌力王
								tblNo = 455 ;
							}else if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}
#elif defined(__ALBUM_41)
							if( tblNo > 1800 ){					//小恶魔
								tblNo -= 1208;
							}else if( tblNo > 1755 ){					//间隔南瓜魔王后的 狐猴
								tblNo -= 1201 ;
							}else if (tblNo > 1739 ){			//飞蛇
								tblNo -= 1200;
							}else if (tblNo > 1686 ){			//8.0宠物
								tblNo -= 1175;
							}else if (tblNo > 1641 ){			//鸡年兽4 甲虫2	8.0第一次整合测试
								tblNo -= 1167;
							}else if (tblNo > 1635 ){   //麒麟
								tblNo -= 1148;
							}else if ( tblNo > 1634 ){	//猫女1 猫女2
								tblNo -= 1149;
							}else if ( tblNo > 1628 ){	//火蚁1 火蚁2 牛人1 牛人2
								tblNo -= 1147;
							}else if( tblNo > 1615 ){//布里萨尔 蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
								tblNo -= 1145;
							}else if( tblNo > 1568 ){//机人龙
								tblNo -= 1103;
							}else if( tblNo > 1564 ){//黄色飞龙
								tblNo -= 1101;
							}else if( tblNo > 1516 ){
								tblNo -= 1055;
							}else if ( tblNo == 1516 ){//修正乌力王
								tblNo = 455 ;
							}else if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}
#elif defined(__ALBUM_40)
							if( tblNo > 1800 ){					//小恶魔
								tblNo -= 1208;
							}else if( tblNo > 1755 ){					//间隔南瓜魔王后的 狐猴
								tblNo -= 1201 ;
							}else if (tblNo > 1739 ){			//飞蛇
								tblNo -= 1200;
							}else if (tblNo > 1686 ){			//8.0宠物
								tblNo -= 1175;
							}else if (tblNo > 1641 ){			//鸡年兽4 甲虫2	8.0第一次整合测试
								tblNo -= 1167;
							}else if (tblNo > 1635 ){   //麒麟
								tblNo -= 1148;
							}else if ( tblNo > 1634 ){	//猫女1 猫女2
								tblNo -= 1149;
							}else if ( tblNo > 1628 ){	//火蚁1 火蚁2 牛人1 牛人2
								tblNo -= 1147;
							}else if( tblNo > 1615 ){//布里萨尔 蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
								tblNo -= 1145;
							}else if( tblNo > 1568 ){//机人龙
								tblNo -= 1103;
							}else if( tblNo > 1564 ){//黄色飞龙
								tblNo -= 1101;
							}else if( tblNo > 1516 ){
								tblNo -= 1055;
							}else if ( tblNo == 1516 ){//修正乌力王
								tblNo = 455 ;
							}else if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}
#elif defined(__ALBUM_39)
							if( tblNo > 1800 ){					//小恶魔
								tblNo -= 1208;
							}else if( tblNo > 1755 ){					//间隔南瓜魔王后的 狐猴
								tblNo -= 1201 ;
							}else if (tblNo > 1739 ){			//飞蛇
								tblNo -= 1200;
							}else if (tblNo > 1686 ){			//8.0宠物
								tblNo -= 1175;
							}else if (tblNo > 1641 ){			//鸡年兽4 甲虫2	8.0第一次整合测试
								tblNo -= 1167;
							}else if (tblNo > 1635 ){   //麒麟
								tblNo -= 1148;
							}else if ( tblNo > 1634 ){	//猫女1 猫女2
								tblNo -= 1149;
							}else if ( tblNo > 1628 ){	//火蚁1 火蚁2 牛人1 牛人2
								tblNo -= 1147;
							}else if( tblNo > 1615 ){//布里萨尔 蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
								tblNo -= 1145;
							}else if( tblNo > 1568 ){//机人龙
								tblNo -= 1103;
							}else if( tblNo > 1564 ){//黄色飞龙
								tblNo -= 1101;
							}else if( tblNo > 1516 ){
								tblNo -= 1055;
							}else if ( tblNo == 1516 ){//修正乌力王
								tblNo = 455 ;
							}else if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}
#elif defined(__ALBUM_38)
							if( tblNo > 1755 ){					//间隔南瓜魔王后的 狐猴
								tblNo -= 1201 ;
							}else if (tblNo > 1739 ){			//飞蛇
								tblNo -= 1200;
							}else if (tblNo > 1686 ){			//8.0宠物
								tblNo -= 1175;
							}else if (tblNo > 1641 ){			//鸡年兽4 甲虫2	8.0第一次整合测试
								tblNo -= 1167;
							}else if (tblNo > 1635 ){   //麒麟
								tblNo -= 1148;
							}else if ( tblNo > 1634 ){	//猫女1 猫女2
								tblNo -= 1149;
							}else if ( tblNo > 1628 ){	//火蚁1 火蚁2 牛人1 牛人2
								tblNo -= 1147;
							}else if( tblNo > 1615 ){//布里萨尔 蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
								tblNo -= 1145;
							}else if( tblNo > 1568 ){//机人龙
								tblNo -= 1103;
							}else if( tblNo > 1564 ){//黄色飞龙
								tblNo -= 1101;
							}else if( tblNo > 1516 ){
								tblNo -= 1055;
							}else if ( tblNo == 1516 ){//修正乌力王
								tblNo = 455 ;
							}else if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}
#elif defined(__ALBUM_37)
							if (tblNo > 1739 ){			//飞蛇
								tblNo -= 1200;
							}else if (tblNo > 1686 ){			//8.0宠物
								tblNo -= 1175;
							}else if (tblNo > 1641 ){			//鸡年兽4 甲虫2	8.0第一次整合测试
								tblNo -= 1167;
							}else if (tblNo > 1635 ){   //麒麟
								tblNo -= 1148;
							}else if ( tblNo > 1634 ){	//猫女1 猫女2
								tblNo -= 1149;
							}else if ( tblNo > 1628 ){	//火蚁1 火蚁2 牛人1 牛人2
								tblNo -= 1147;
							}else if( tblNo > 1615 ){//布里萨尔 蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
								tblNo -= 1145;
							}else if( tblNo > 1568 ){//机人龙
								tblNo -= 1103;
							}else if( tblNo > 1564 ){//黄色飞龙
								tblNo -= 1101;
							}else if( tblNo > 1516 ){
								tblNo -= 1055;
							}else if ( tblNo == 1516 ){//修正乌力王
								tblNo = 455 ;
							}else if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}
#elif defined(__ALBUM_36)
							if (tblNo > 1686 ){			//鸡年兽4 甲虫2	8.0第一次整合测试
								tblNo -= 1175;
							}else if (tblNo > 1641 ){			//鸡年兽4 甲虫2	8.0第一次整合测试
								tblNo -= 1167;
							}else if (tblNo > 1635 ){   //麒麟
								tblNo -= 1148;
							}else if ( tblNo > 1634 ){	//猫女1 猫女2
								tblNo -= 1149;
							}else if ( tblNo > 1628 ){	//火蚁1 火蚁2 牛人1 牛人2
								tblNo -= 1147;
							}else if( tblNo > 1615 ){//布里萨尔 蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
								tblNo -= 1145;
							}else if( tblNo > 1568 ){//机人龙
								tblNo -= 1103;
							}else if( tblNo > 1564 ){//黄色飞龙
								tblNo -= 1101;
							}else if( tblNo > 1516 ){
								tblNo -= 1055;
							}else if ( tblNo == 1516 ){//修正乌力王
								tblNo = 455 ;
							}else if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}
#elif defined(__ALBUM_35)
							if (tblNo > 1641 ){			//鸡年兽4 甲虫2	8.0第一次整合测试
								tblNo -= 1167;
							}else if (tblNo > 1635 ){   //麒麟
								tblNo -= 1148;
							}else if ( tblNo > 1634 ){	//猫女1 猫女2
								tblNo -= 1149;
							}else if ( tblNo > 1628 ){	//火蚁1 火蚁2 牛人1 牛人2
								tblNo -= 1147;
							}else if( tblNo > 1615 ){//布里萨尔 蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
								tblNo -= 1145;
							}else if( tblNo > 1568 ){//机人龙
								tblNo -= 1103;
							}else if( tblNo > 1564 ){//黄色飞龙
								tblNo -= 1101;
							}else if( tblNo > 1516 ){
								tblNo -= 1055;
							}else if ( tblNo == 1516 ){//修正乌力王
								tblNo = 455 ;
							}else if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}

#elif defined(__ALBUM_34)
							if (tblNo > 1639 ){			//鸡年兽4
								tblNo -= 1167;
							}else if (tblNo > 1635 ){   //麒麟
								tblNo -= 1148;
							}else if ( tblNo > 1634 ){	//猫女1 猫女2
								tblNo -= 1149;
							}else if ( tblNo > 1628 ){	//火蚁1 火蚁2 牛人1 牛人2
								tblNo -= 1147;
							}else if( tblNo > 1615 ){//布里萨尔 蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
								tblNo -= 1145;
							}else if( tblNo > 1568 ){//机人龙
								tblNo -= 1103;
							}else if( tblNo > 1564 ){//黄色飞龙
								tblNo -= 1101;
							}else if( tblNo > 1516 ){
								tblNo -= 1055;
							}else if ( tblNo == 1516 ){//修正乌力王
								tblNo = 455 ;
							}else if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}
#elif defined(__ALBUM_33)
							if (tblNo > 1635 ){
								tblNo -= 1148;
							}else if ( tblNo > 1634 ){	//猫女1 猫女2 麒麟
								tblNo -= 1149;
							}else if ( tblNo > 1628 ){	//火蚁1 火蚁2 牛人1 牛人2
								tblNo -= 1147;
							}else if( tblNo > 1615 ){//布里萨尔 蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
								tblNo -= 1145;
							}else if( tblNo > 1568 ){//机人龙
								tblNo -= 1103;
							}else if( tblNo > 1564 ){//黄色飞龙
								tblNo -= 1101;
							}else if( tblNo > 1516 ){
								tblNo -= 1055;
							}else if ( tblNo == 1516 ){//修正乌力王
								tblNo = 455 ;
							}else if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}
#elif defined(__ALBUM_32)
							if ( tblNo > 1634 ){	//猫女1 猫女2
								tblNo -= 1149;
							}else if ( tblNo > 1628 ){	//火蚁1 火蚁2 牛人1 牛人2
								tblNo -= 1147;
							}else if( tblNo > 1615 ){//布里萨尔 蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
								tblNo -= 1145;
							}else if( tblNo > 1568 ){//机人龙
								tblNo -= 1103;
							}else if( tblNo > 1564 ){//黄色飞龙
								tblNo -= 1101;
							}else if( tblNo > 1516 ){
								tblNo -= 1055;
							}else if ( tblNo == 1516 ){//修正乌力王
								tblNo = 455 ;
							}else if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}
#elif defined(__ALBUM_31)
							if ( tblNo > 1628 ){	//火蚁1 火蚁2 牛人1 牛人2
								tblNo -= 1147;
							}else if( tblNo > 1615 ){//布里萨尔 蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
								tblNo -= 1145;
							}else if( tblNo > 1568 ){//机人龙
								tblNo -= 1103;
							}else if( tblNo > 1564 ){//黄色飞龙
								tblNo -= 1101;
							}else if( tblNo > 1516 ){
								tblNo -= 1055;
							}else if ( tblNo == 1516 ){//修正乌力王
								tblNo = 455 ;
							}else if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}
#elif defined(__ALBUM_30)
							if( tblNo > 1615 ){//布里萨尔 蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼 布伊酷
								tblNo -= 1145;
							}else if( tblNo > 1568 ){//机人龙
								tblNo -= 1103;
							}else if( tblNo > 1564 ){//黄色飞龙
								tblNo -= 1101;
							}else if( tblNo > 1516 ){
								tblNo -= 1055;
							}else if ( tblNo == 1516 ){//修正乌力王
								tblNo = 455 ;
							}else if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}
#elif defined(__ALBUM_29)
							if( tblNo > 1615 ){//布里萨尔 蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔 白狼
								tblNo -= 1145;
							}else if( tblNo > 1568 ){//机人龙
								tblNo -= 1103;
							}else if( tblNo > 1564 ){//黄色飞龙
								tblNo -= 1101;
							}else if( tblNo > 1516 ){
								tblNo -= 1055;
							}else if ( tblNo == 1516 ){//修正乌力王
								tblNo = 455 ;
							}else if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}

#elif defined(__ALBUM_28)
							if( tblNo > 1615 ){//布里萨尔 蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2 灰人熔
								tblNo -= 1145;
							}else if( tblNo > 1568 ){//机人龙
								tblNo -= 1103;
							}else if( tblNo > 1564 ){//黄色飞龙
								tblNo -= 1101;
							}else if( tblNo > 1516 ){
								tblNo -= 1055;
							}else if ( tblNo == 1516 ){//修正乌力王
								tblNo = 455 ;
							}else if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}

#elif defined(__ALBUM_27)
							if( tblNo > 1615 ){//布里萨尔 蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王 狮人1 狮人2
								tblNo -= 1145;
							}else if( tblNo > 1568 ){//机人龙
								tblNo -= 1103;
							}else if( tblNo > 1564 ){//黄色飞龙
								tblNo -= 1101;
							}else if( tblNo > 1516 ){
								tblNo -= 1055;
							}else if ( tblNo == 1516 ){//修正乌力王
								tblNo = 455 ;
							}else if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}

#elif defined(__ALBUM_26)
							if( tblNo > 1615 ){//布里萨尔 蜜蜂1 蜜蜂2 蝴蝶1 蝴蝶2 暗黑乌力王
								tblNo -= 1145;
							}else if( tblNo > 1568 ){//机人龙
								tblNo -= 1103;
							}else if( tblNo > 1564 ){//黄色飞龙
								tblNo -= 1101;
							}else if( tblNo > 1516 ){
								tblNo -= 1055;
							}else if ( tblNo == 1516 ){//修正乌力王
								tblNo = 455 ;
							}else if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}

#elif defined(__ALBUM_25)
							if( tblNo > 1615 ){//布里萨尔
								tblNo -= 1145;
							}else if( tblNo > 1568 ){//机人龙
								tblNo -= 1103;
							}else if( tblNo > 1564 ){//黄色飞龙
								tblNo -= 1101;
							}else if( tblNo > 1516 ){
								tblNo -= 1055;
							}else if ( tblNo == 1516 ){//修正乌力王
								tblNo = 455 ;
							}else if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}

#elif defined(__ALBUM_24)
							if( tblNo > 1568 ){//机人龙
								tblNo -= 1103;
							}else if( tblNo > 1564 ){//黄色飞龙
								tblNo -= 1101;
							}else if( tblNo > 1516 ){
								tblNo -= 1055;
							}else if ( tblNo == 1516 ){//修正乌力王
								tblNo = 455 ;
							}else if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}
#elif defined(__ALBUM_23)
							if( tblNo > 1518 ){
								tblNo -= 1057;
							}else if( tblNo > 1516 ){
								tblNo -= 1055;
							}else if ( tblNo == 1516 ){//修正乌力王
								tblNo = 455 ;
							}else if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}

#elif defined(__ALBUM_22)
							if ( tblNo == 1516 ){//修正乌力王
								tblNo = 455 ;
							}else if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}

#elif defined(__ALBUM_21)
							if ( tblNo > 1509 ){
								tblNo -= 1054;
							}else if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}

#elif defined(__ALBUM_20)
							if ( tblNo == 1497 ){
								tblNo = 395;
							}else if ( tblNo > 1495 ){
								tblNo -= 1044 ;
							}else if ( tblNo > 1490 ){
								tblNo -= 1043 ;
							}else if ( tblNo > 1485 ){
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}

#elif defined(__ALBUM_19)
							if ( tblNo > 1485 ) {
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}
#elif defined(__ALBUM_18)
							if ( tblNo > 1490 ){
								tblNo -= 1044;
							}else if ( tblNo > 1485 ) {
								tblNo -= 1040 ; 
							}else if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}
#elif defined(__ALBUM_17)
							if ( tblNo > 1454 ) {
								tblNo -= 1034 ; 
							}else if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}
#elif defined(__ALBUM_16)
							if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}
#elif defined(__ALBUM_15)
							if ( tblNo == 1214 ) {
								tblNo = 331 ;
							}else if ( tblNo == 1217 ) {
								tblNo = 328 ; 
							}else if ( tblNo > 1361){
								tblNo -= 952;
							}else if( tblNo > 1356){
								tblNo -= 999;
							}else if( tblNo > 1281){
								tblNo -= 919;
							}else if( tblNo > 1239){
								tblNo -= 889;
							}else if( tblNo > 1186 ){
								tblNo -= 886;
							}else if( tblNo > 1173 ){
								tblNo -= 877;
							}else if( tblNo > 1172){
								tblNo -= 877;
							}else if( tblNo > 1161){
								tblNo -= 868;
							}else if( tblNo > 930){
								tblNo -= 745;
							}else if( tblNo == 927){
								tblNo -= 634;
							}else if( tblNo > 929){
								tblNo -= 644;
							}else if( tblNo > 927){
								tblNo -= 643;
							}else if( tblNo > 900){
								tblNo -= 621;
							}else if( tblNo > 180){
								tblNo -= (333+56);
							}
#endif
							int albumNo;
							// ???????
							if (0 <= tblNo && tblNo < MAX_PET_TBL){
								// ???????

								if (tblNo == 442)
									albumNo = 0;

								albumNo = PetAlbumTbl[tblNo].albumNo;
								// ????????
								if (albumNo != -1){
									// ???????
									PetAlbum[albumNo].flag = 2;

									// ??????
									if (pet[petStatusNo].freeName[0] != NULL){
										strcpy(PetAlbum[albumNo].freeName, pet[petStatusNo].freeName);
									}
									else{
										// ???
										strcpy(PetAlbum[albumNo].freeName, PetAlbumTbl[tblNo].name);
									}
									// ???
									strcpy(PetAlbum[albumNo].name, PetAlbumTbl[tblNo].name);
									// ????????
									PetAlbum[albumNo].faceGraNo = PetAlbumTbl[tblNo].faceGraNo;
									// ???
									PetAlbum[albumNo].level = pet[petStatusNo].level;
									// ???
									PetAlbum[albumNo].maxHp = pet[petStatusNo].maxHp;
									// ??
									PetAlbum[albumNo].str = pet[petStatusNo].atk;
									// ??
									PetAlbum[albumNo].quick = pet[petStatusNo].quick;
									// ?
									PetAlbum[albumNo].def = pet[petStatusNo].def;

									// 佋
									PetAlbum[albumNo].earth = pet[petStatusNo].earth;
									// ?
									PetAlbum[albumNo].water = pet[petStatusNo].water;
									// ?
									PetAlbum[albumNo].fire = pet[petStatusNo].fire;
									// ?
									PetAlbum[albumNo].wind = pet[petStatusNo].wind;

									// ???
									play_se(201, 320, 240);
									// ???????
									sprintf_s(moji, "%s 照相完成！", PetAlbum[albumNo].freeName);
									// ??????????????????
									StockChatBufferLine(moji, FONT_PAL_WHITE);

									petWndBtnFlag[7] = TRUE;
									// ????? 
									SaveAlbum(albumNo);

								}
								else{
									// ???
									play_se(220, 320, 240);
#ifdef _STONDEBUG_		
									sprintf_s( moji,"图形编号很奇怪 %d",pet[ petStatusNo ].graNo );
									MessageBoxNew( hWnd, moji, "确定", MB_OK | MB_ICONSTOP );
#endif
									//	}
								}
							}
							else{
								// ???
								play_se(220, 320, 240);
#ifdef _STONDEBUG_		
								sprintf_s( moji,"宠物的table编号很奇怪 %d",tblNo );
								MessageBoxNew( hWnd, moji, "确定", MB_OK | MB_ICONSTOP );
#endif
							}
#endif
						}
						// ?????????
						if (mouse.state & MOUSE_LEFT_CRICK && petWndBtnFlag[7] == TRUE){
							// ????
							petWndBtnFlag[7] = TRUE;
						}
						else{
							// ???
							petWndBtnFlag[7] = FALSE;
						}
					}
					else{
						// ???
						petWndBtnFlag[7] = FALSE;
					}
#endif


					if (pActMenuWnd != NULL){
#define PET_WND_ATTR_X 159
#define PET_WND_ATTR_Y 5
						int x2;
						x = pActMenuWnd->x + 20;
						y = pActMenuWnd->y + 35;
#ifdef _PETBLESS_
					/*
						if (pet[petStatusNo].blessflg){
							int blessNoid;
							if (pet[petStatusNo].blesshp != 2){
								blessNoid = StockDispBuffer(x + 140, y + 128, DISP_PRIO_IME3, 55258, 2);
								if (HitDispNo == blessNoid) ShowBottomLineString(FONT_PAL_WHITE, "宠物祝福。");
								if ((mouse.state & MOUSE_LEFT_CRICK && HitDispNo == blessNoid)){
									祝福窗口选中 = 1;
									if (!祝福窗口开关)
										lssproto_petbless_send(sockfd, petStatusNo, -1);
									祝福窗口开关 = TRUE;
								}
							}
							if (pet[petStatusNo].blessatk != 2){
								blessNoid = StockDispBuffer(x + 92, y + 150, DISP_PRIO_IME3, 55258, 2);
								if (HitDispNo == blessNoid) ShowBottomLineString(FONT_PAL_WHITE, "宠物祝福。");
								if ((mouse.state & MOUSE_LEFT_CRICK && HitDispNo == blessNoid)){

									祝福窗口选中 = 2;
									if (!祝福窗口开关)
										lssproto_petbless_send(sockfd, petStatusNo, -2);
									祝福窗口开关 = TRUE;

								}
							}
							if (pet[petStatusNo].blessdef != 2){
								blessNoid = StockDispBuffer(x + 92, y + 175, DISP_PRIO_IME3, 55258, 2);
								if (HitDispNo == blessNoid) ShowBottomLineString(FONT_PAL_WHITE, "宠物祝福。");
								if ((mouse.state & MOUSE_LEFT_CRICK && HitDispNo == blessNoid)){

									祝福窗口选中 = 3;
									if (!祝福窗口开关)
										lssproto_petbless_send(sockfd, petStatusNo, -3);
									祝福窗口开关 = TRUE;
								}
							}
							if (pet[petStatusNo].blessquick != 2){
								blessNoid = StockDispBuffer(x + 92, y + 200, DISP_PRIO_IME3, 55258, 2);
								if (HitDispNo == blessNoid) ShowBottomLineString(FONT_PAL_WHITE, "宠物祝福。");
								if ((mouse.state & MOUSE_LEFT_CRICK && HitDispNo == blessNoid)){

									祝福窗口选中 = 4;
									if (!祝福窗口开关)
										lssproto_petbless_send(sockfd, petStatusNo, -4);
									祝福窗口开关 = TRUE;
								}
							}
							if (祝福窗口开关){
								StockDispBuffer(x + 115, y + 345, DISP_PRIO_MENU, 55259, 1);
								StockFontBuffer(x - 10, y + 325, FONT_PRIO_FRONT, 0, 祝福窗口内容, 0);
								blessNoid = StockDispBuffer(x + 65, y + 367, DISP_PRIO_IME3, 55233, 2);
								if ((mouse.state & MOUSE_LEFT_CRICK && HitDispNo == blessNoid)){
									if (祝福窗口开关) lssproto_petbless_send(sockfd, petStatusNo, 祝福窗口选中);
									祝福窗口开关 = FALSE;

								}
								blessNoid = StockDispBuffer(x + 165, y + 367, DISP_PRIO_IME3, 55235, 2);
								if ((mouse.state & MOUSE_LEFT_CRICK && HitDispNo == blessNoid)){
									祝福窗口开关 = FALSE;
								}
							}
						}
						*/
#endif
#ifdef _PETCOM_
						extern void ShowBottomLineString(int iColor, LPSTR lpstr);
						static int 宠算图片索引 = 0;
						static int 宠算窗口按钮ID = 0;
						宠算窗口按钮ID = StockDispBuffer(x + 193, y + 5, DISP_PRIO_IME3, CG_PETCOM_CHANGE_BTN + 宠算图片索引, 2);
						if (HitDispNo == 宠算窗口按钮ID)
							ShowBottomLineString(FONT_PAL_WHITE, "宠物成长计算。");
						if ((mouse.state & MOUSE_LEFT_CRICK && HitDispNo == 宠算窗口按钮ID)){
							宠算图片索引 = 1;
							if (mouse.onceState & MOUSE_LEFT_CRICK){
								petWndNo = 4;
								DeathAction(pActMenuWnd);
								pActMenuWnd = NULL;
								DeathAction(pActPet);
								pActPet = NULL;
								DeathAction(pActMenuWnd3);
								pActMenuWnd3 = NULL;
								GetKeyInputFocus(&MyChatBuffer);
								play_se(202, 320, 240);
							}
						}
						else{
							宠算图片索引 = 0;
						}
#endif

						if (pet[petStatusNo].freeName[0] != NULL)
							CenteringStr(pet[petStatusNo].freeName, moji, PET_NAME_LEN);
						else
							CenteringStr(pet[petStatusNo].name, moji, PET_NAME_LEN);
						StockFontBuffer(x - 7, y, FONT_PRIO_FRONT, 0, moji, 0); y += 24;
#ifdef _SHOWPETTRN_
						sprintf(moji, "[%d转]", pet[petStatusNo].trn);
						StockFontBuffer(x + 108, y - 24, FONT_PRIO_FRONT, 5, moji, 0);
#endif
						petWndFontNo[3] = StockDispBuffer(x + 66, y + 7, DISP_PRIO_IME3, CG_NAME_CHANGE_BTN + petWndBtnFlag[3], 2);
						y += 23;
						sprintf_s(moji, "%3d", pet[petStatusNo].level);
						StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, 0, moji, 0); y += 24;
						sprintf_s(moji, "%8d", pet[petStatusNo].exp);
						StockFontBuffer(x + 48, y, FONT_PRIO_FRONT, 0, moji, 0); y += 24;
						sprintf_s(moji, "%8d", pet[petStatusNo].maxExp);
						StockFontBuffer(x + 48, y, FONT_PRIO_FRONT, 0, moji, 0); y += 24;
#ifdef _PETBLESS_
						if (pet[petStatusNo].blesshp){
							sprintf_s(moji, "%4d", pet[petStatusNo].hp);
							StockFontBuffer(x + 46, y, FONT_PRIO_FRONT, 2, moji, 0);
							sprintf_s(moji, "%4d", pet[petStatusNo].maxHp);
							StockFontBuffer(x + 48 + 45, y, FONT_PRIO_FRONT, 2, moji, 0); y += 24;
						}else{
							sprintf_s(moji, "%4d", pet[petStatusNo].hp);
							StockFontBuffer(x + 46, y, FONT_PRIO_FRONT, 0, moji, 0);
							sprintf_s(moji, "%4d", pet[petStatusNo].maxHp);
							StockFontBuffer(x + 48 + 45, y, FONT_PRIO_FRONT, 0, moji, 0); y += 24;
						}
#else
#ifdef _PET_VAL_COLOR_
						sprintf_s(moji, "%4d", pet[petStatusNo].hp);
						StockFontBuffer(x + 46, y, FONT_PRIO_FRONT, 1, moji, 0);
						sprintf_s(moji, "%4d", pet[petStatusNo].maxHp);
						StockFontBuffer(x + 48 + 45, y, FONT_PRIO_FRONT, 1, moji, 0); y += 24;
#else
						sprintf_s(moji, "%4d", pet[petStatusNo].hp);
						StockFontBuffer(x + 46, y, FONT_PRIO_FRONT, 0, moji, 0);
						sprintf_s(moji, "%4d", pet[petStatusNo].maxHp);
						StockFontBuffer(x + 48 + 45, y, FONT_PRIO_FRONT, 0, moji, 0); y += 24;
#endif
#endif
						// 佋???
						if (pet[petStatusNo].earth > 0){
							x2 = (int)(pet[petStatusNo].earth * 0.8);
							StockBoxDispBuffer(x + PET_WND_ATTR_X + 0, y + PET_WND_ATTR_Y + 0, x + PET_WND_ATTR_X + 0 + x2, y + PET_WND_ATTR_Y + 8, DISP_PRIO_IME4, SYSTEM_PAL_GREEN, 1);
							StockBoxDispBuffer(x + PET_WND_ATTR_X + 1, y + PET_WND_ATTR_Y + 1, x + PET_WND_ATTR_X + 1 + x2, y + PET_WND_ATTR_Y + 9, DISP_PRIO_IME3, SYSTEM_PAL_GREEN2, 0);
							StockBoxDispBuffer(x + PET_WND_ATTR_X + 2, y + PET_WND_ATTR_Y + 2, x + PET_WND_ATTR_X + 2 + x2, y + PET_WND_ATTR_Y + 10, DISP_PRIO_IME3, SYSTEM_PAL_GREEN2, 0);
						}

#ifdef _PETBLESS_
						if (pet[petStatusNo].blessatk){
							sprintf_s(moji, "%3d", pet[petStatusNo].atk);
							StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, 2, moji, 0); y += 24;
						}
						else{
							sprintf_s(moji, "%3d", pet[petStatusNo].atk);
							StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, 0, moji, 0); y += 24;
						}
#else
#ifdef _PET_VAL_COLOR_
						sprintf_s(moji, "%3d", pet[petStatusNo].atk);
						StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, 6, moji, 0); y += 24;
#else
						sprintf_s(moji, "%3d", pet[petStatusNo].atk);
						StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, 0, moji, 0); y += 24;
#endif
#endif
						// ????
						if (pet[petStatusNo].water > 0){
							x2 = (int)(pet[petStatusNo].water * 0.8);

							StockBoxDispBuffer(x + PET_WND_ATTR_X + 0, y + PET_WND_ATTR_Y + 0, x + PET_WND_ATTR_X + 0 + x2, y + PET_WND_ATTR_Y + 8, DISP_PRIO_IME4, SYSTEM_PAL_AQUA, 1);
							StockBoxDispBuffer(x + PET_WND_ATTR_X + 1, y + PET_WND_ATTR_Y + 1, x + PET_WND_ATTR_X + 1 + x2, y + PET_WND_ATTR_Y + 9, DISP_PRIO_IME3, SYSTEM_PAL_AQUA2, 0);
							StockBoxDispBuffer(x + PET_WND_ATTR_X + 2, y + PET_WND_ATTR_Y + 2, x + PET_WND_ATTR_X + 2 + x2, y + PET_WND_ATTR_Y + 10, DISP_PRIO_IME3, SYSTEM_PAL_AQUA2, 0);
						}

#ifdef _PETBLESS_
						if (pet[petStatusNo].blessdef){
							sprintf_s(moji, "%3d", pet[petStatusNo].def);
							StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, 2, moji, 0); y += 24;
						}
						else{
							sprintf_s(moji, "%3d", pet[petStatusNo].def);
							StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, 0, moji, 0); y += 24;
						}
#else
#ifdef _PET_VAL_COLOR_
						sprintf_s(moji, "%3d", pet[petStatusNo].def);
						StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, 5, moji, 0); y += 24;
#else
						sprintf_s(moji, "%3d", pet[petStatusNo].def);
						StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, 0, moji, 0); y += 24;
#endif
#endif
						// ????
						if (pet[petStatusNo].fire > 0){
							x2 = (int)(pet[petStatusNo].fire * 0.8);
							StockBoxDispBuffer(x + PET_WND_ATTR_X + 0, y + PET_WND_ATTR_Y + 0, x + PET_WND_ATTR_X + 0 + x2, y + PET_WND_ATTR_Y + 8, DISP_PRIO_IME4, SYSTEM_PAL_RED, 1);
							StockBoxDispBuffer(x + PET_WND_ATTR_X + 1, y + PET_WND_ATTR_Y + 1, x + PET_WND_ATTR_X + 1 + x2, y + PET_WND_ATTR_Y + 9, DISP_PRIO_IME3, SYSTEM_PAL_RED2, 0);
							StockBoxDispBuffer(x + PET_WND_ATTR_X + 2, y + PET_WND_ATTR_Y + 2, x + PET_WND_ATTR_X + 2 + x2, y + PET_WND_ATTR_Y + 10, DISP_PRIO_IME3, SYSTEM_PAL_RED2, 0);
						}
#ifdef _PETBLESS_
						if (pet[petStatusNo].blessquick){
							sprintf_s(moji, "%3d", pet[petStatusNo].quick);
							StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, 2, moji, 0); y += 24;
						}
						else{
							sprintf_s(moji, "%3d", pet[petStatusNo].quick);
							StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, 0, moji, 0); y += 24;
						}
#else
#ifdef _PET_VAL_COLOR_
						sprintf_s(moji, "%3d", pet[petStatusNo].quick);
						StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, 4, moji, 0); y += 24;
#else
						sprintf_s(moji, "%3d", pet[petStatusNo].quick);
						StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, 0, moji, 0); y += 24;
#endif
#endif
						// ????
						if (pet[petStatusNo].wind > 0){
							x2 = (int)(pet[petStatusNo].wind * 0.8);
							StockBoxDispBuffer(x + PET_WND_ATTR_X + 0, y + PET_WND_ATTR_Y + 0, x + PET_WND_ATTR_X + 0 + x2, y + PET_WND_ATTR_Y + 8, DISP_PRIO_IME4, SYSTEM_PAL_YELLOW, 1);
							StockBoxDispBuffer(x + PET_WND_ATTR_X + 1, y + PET_WND_ATTR_Y + 1, x + PET_WND_ATTR_X + 1 + x2, y + PET_WND_ATTR_Y + 9, DISP_PRIO_IME3, SYSTEM_PAL_YELLOW2, 0);
							StockBoxDispBuffer(x + PET_WND_ATTR_X + 2, y + PET_WND_ATTR_Y + 2, x + PET_WND_ATTR_X + 2 + x2, y + PET_WND_ATTR_Y + 10, DISP_PRIO_IME3, SYSTEM_PAL_YELLOW2, 0);
						}
						sprintf_s(moji, "%3d", pet[petStatusNo].ai);
						StockFontBuffer(x + 50, y, FONT_PRIO_FRONT, 0, moji, 0); y += 24;
						y += 12;
						// ?????

#ifndef _PET_SKINS
						petWndFontNo[7] = StockDispBuffer(x + 52, y, DISP_PRIO_IME3, CG_ALBUM_WND_SNAP_BTN_UP + petWndBtnFlag[7], 2);
#endif
						petWndFontNo[4] = StockDispBuffer(x + 178, y, DISP_PRIO_IME3, CG_PET_WND_WAZA_BTN, 2); y += 31;
						petWndFontNo[0] = StockDispBuffer(x + 100 + 70, y, DISP_PRIO_IME3, CG_RETURN_BTN, 2);
						petWndFontNo[1] = StockDispBuffer(x + 16 + 16 + 8, y, DISP_PRIO_IME3, CG_PREV_BTN + petWndBtnFlag[1], 2);
						petWndFontNo[2] = StockDispBuffer(x + 50 + 16 + 18, y, DISP_PRIO_IME3, CG_NEXT_BTN + petWndBtnFlag[2], 2);
					}
				}
			}
			if (pActMenuWnd3 != NULL){
				if (pActMenuWnd3->hp > 0){
					StockDispBuffer(((WINDOW_DISP *)pActMenuWnd3->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd3->pYobi)->my, DISP_PRIO_MENU, CG_NAME_CHANGE_WND, 1);
					// ??????
					GetKeyInputFocus(&petNameChange);
					// ????????
					if (mouse.onceState & MOUSE_LEFT_CRICK){
						// ?ＯＫ???
						if (HitDispNo == petWndFontNo[5]){
							// ????????
							KeyboardReturn();

						}
						// ?????????
						if (HitDispNo == petWndFontNo[6]){
							// ??????
							DeathAction(pActMenuWnd3);
							pActMenuWnd3 = NULL;
							// ????????
							GetKeyInputFocus(&MyChatBuffer);
							// ????????
							play_se(203, 320, 240);
						}
					}
					// ??????????????
					if (pActMenuWnd3 != NULL){
						// ???
						StockFontBuffer2(&petNameChange);

						// ?
						x = pActMenuWnd3->x + 20;
						y = pActMenuWnd3->y + 60;
						petWndFontNo[5] = StockDispBuffer(x + 60, y + 7, DISP_PRIO_IME3, CG_OK_BTN, 2);
						petWndFontNo[6] = StockDispBuffer(x + 170, y + 7, DISP_PRIO_IME3, CG_CANCEL_BTN, 2);
					}
				}
			}
			break;

		case 2: // ??????????

			// ?????????
			if (pActMenuWnd == NULL){
				// ?????????
				pActMenuWnd = MakeWindowDisp(4, 4, 272, 348, 0, -1);
				// ??????
				for (i = 0; i < MENU_PET_0; i++) petWndFontNo[i] = -2;

			}
			else{
				// ??????????????
				if (pActMenuWnd->hp > 0){
					// ?????????
					StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my, DISP_PRIO_MENU, CG_PET_WAZA_WND, 1);
					// ?????????
					for (i = 0; i < pet[petStatusNo].maxSkill; i++){
						// ?????
						StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my, DISP_PRIO_IME3, CG_PET_WAZA_BAR_1 + i, 1);
					}
					// ????????
					if (mouse.onceState & MOUSE_LEFT_CRICK){
						// ????????????
						if (HitDispNo == petWndFontNo[7]){
							// ??????
							DeathAction(pActMenuWnd);
							pActMenuWnd = NULL;
							// ?????????????
							petWndNo = 1;
							// ????????
							play_se(203, 320, 240);
						}
					}
					// ??????????
					if (HitDispNo == petWndFontNo[8] || joy_con[0] & JOY_A){
						// ????????
						if (mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[0] & JOY_A){
							while (1){
								petStatusNo--;
								// ????????
								if (petStatusNo <= -1) petStatusNo = 4;
								if (pet[petStatusNo].useFlag == TRUE) break;
							}
							// ??????
							for (i = 0; i < MENU_PET_0; i++) petWndFontNo[i] = -2;
							// ?????
							play_se(217, 320, 240);
							petWndBtnFlag[8] = TRUE;
						}
						// ?????????
						if ((mouse.state & MOUSE_LEFT_CRICK || joy_con[0] & JOY_A) && petWndBtnFlag[8] == TRUE){
							// ????
							petWndBtnFlag[8] = TRUE;
						}
						else{
							// ???
							petWndBtnFlag[8] = FALSE;
						}
					}
					else{
						// ???
						petWndBtnFlag[8] = FALSE;
					}
					// ???????????
					if (HitDispNo == petWndFontNo[9] || joy_con[0] & JOY_B){
						// ????????
						if (mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[0] & JOY_B){
							while (1){
								petStatusNo++;
								// ????????
								if (petStatusNo >= 5) petStatusNo = 0;
								if (pet[petStatusNo].useFlag == TRUE) break;
							}
							// ??????
							for (i = 0; i < MENU_PET_0; i++) petWndFontNo[i] = -2;
							// ?????
							play_se(217, 320, 240);
							petWndBtnFlag[9] = TRUE;
						}
						// ?????????
						if ((mouse.state & MOUSE_LEFT_CRICK || joy_con[0] & JOY_B) && petWndBtnFlag[9] == TRUE){
							// ????
							petWndBtnFlag[9] = TRUE;
						}
						else{
							// ???
							petWndBtnFlag[9] = FALSE;
						}
					}
					else{
						// ???
						petWndBtnFlag[9] = FALSE;
					}

					// ????????
					if (pActMenuWnd != NULL){

						// ?
						x = pActMenuWnd->x + 32;
						y = pActMenuWnd->y + 252;

						// ?
						for (i = 0; i < pet[petStatusNo].maxSkill; i++){
							// ????????
							if (petSkill[petStatusNo][i].useFlag == TRUE){
								// ???????
								if (HitFontNo == petWndFontNo[i]){
									// ??????
									char *splitPoint = petSkill[petStatusNo][i].memo;
									// ?????
									while (1){
										// ?????????
										if (strlen(splitPoint) > 24){
											strncpy_s(moji, splitPoint, 24);
											moji[24] = NULL;	// ??????
											// ??????
											if (GetStrLastByte(moji) == 3){
												moji[23] = NULL;
												splitPoint += 23;
											}
											else{
												moji[24] = NULL;
												splitPoint += 24;
											}
											StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, moji, 0); y += 20;
										}
										else{
											strcpy(moji, splitPoint);
											StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, moji, 0);
											break;
										}
									}
#ifdef _STONDEBUG_						
									// ????????
									if( mouse.onceState & MOUSE_RIGHT_CRICK ){
										// ?????????
										if( petSkill[ petStatusNo ][ i ].field != PETSKILL_FIELD_BATTLE ){
											int j;
											int cnt = 0;
											switch( petSkill[ petStatusNo ][ i ].skillId ){

											case PETSKILL_MERGE: 	// ??????
												// ?????????????????
												for( j = MAX_ITEMSTART ; j < MAX_ITEMSTART ; j++ ){
													if( ItemBuffer[ j ].mixFlag == 1 ) cnt++;
												}
												break;

											case PETSKILL_MERGE2: 	// ???????
												// ?????????????????
												for( j = MAX_ITEMSTART ; j < MAX_ITEM ; j++ ){
													if( ItemBuffer[ j ].mixFlag == 2 ) cnt++;
												}
												break;
											}

											// ????????
											if( cnt >= 2 ){
												// ????
												moji[ 0 ] = NULL;
												// ????????????
												for( j = MAX_ITEMSTART ; j <  MAX_ITEM ; j++ ){
													// ??????????
													if( ItemBuffer[ j ].mixFlag >= TRUE ){
														char work[ 256 ];
														// ???
														sprintf_s( work,"%d|", j );
														strcat_s( moji, work );
														// ??????
														ItemBuffer[ j ].mixFlag = FALSE;
													}
												}
												// ???|????????
												moji[ strlen( moji ) - 1 ] = NULL;

												// ??????????????????
												if( bNewServer)
													lssproto_PS_send( sockfd, mixPetNo, i, 0, moji );
												else
													old_lssproto_PS_send( sockfd, mixPetNo, i, 0, moji );

												// ??????
												play_se(212, 320, 240);
											}
											else{
												// ???
												play_se(220, 320, 240);
											}
										}
										else{
											// ???
											play_se(220, 320, 240);
										}
									}
#endif									
									// ????????
									if (mouse.onceState & MOUSE_LEFT_CRICK){
										// ????????????????????????????????????
										if (petSkill[petStatusNo][i].field != PETSKILL_FIELD_BATTLE && pActPet2 == NULL
											/* && MenuToggleFlag & JOY_CTRL_I*/){
											int j;
											int cnt = 0;
											// ?????
											switch (petSkill[petStatusNo][i].skillId){

											case PETSKILL_MERGE: 	// ??????
												// ?????????????????
												for (j = MAX_ITEMSTART; j < MAX_ITEM; j++){
													if (ItemBuffer[j].mixFlag == 1) cnt++;
												}
												break;

											case PETSKILL_MERGE2: 	// ???????
												// ?????????????????
												for (j = MAX_ITEMSTART; j < MAX_ITEM; j++){
													if (ItemBuffer[j].mixFlag == 2) cnt++;
												}
												break;
											case PETSKILL_INSLAY: //andy_add inslay
												for (j = MAX_ITEMSTART; j < MAX_ITEM; j++){
													if (ItemBuffer[j].mixFlag == 10) cnt++;
												}
												break;
											case PETSKILL_FIXITEM:
											case PETSKILL_FIXITEM2:
												for (j = MAX_ITEMSTART; j < MAX_ITEM; j++){
													if (ItemBuffer[j].mixFlag == 11) cnt++;
												}
												break;
#ifdef _ITEM_INTENSIFY
											case PETSKILL_INTENSIFY:
												for (j = MAX_ITEMSTART; j < MAX_ITEM; j++){
													if (ItemBuffer[j].mixFlag == 12) cnt++;
												}
												break;
#endif
#ifdef _ITEM_UPINSLAY
											case PETSKILL_UPINSLAY:
												for (j = MAX_ITEMSTART; j < MAX_ITEM; j++){
													if (ItemBuffer[j].mixFlag == 13) cnt++;
												}
												break;
#endif
#ifdef _ALCHEMIST
											case PETSKILL_ALCHEMIST:
												for (j = MAX_ITEMSTART; j < MAX_ITEM; j++){
													if (ItemBuffer[j].mixFlag == 9) cnt++;
												}
												break;
#endif
#ifdef _CALLBUS 
											case PETSKILL_CALLBUS:
												lssproto_PS_send(sockfd, petStatusNo, i, 0, "");
												break;
#endif
											}
											// ????????
											if (cnt >= 2){

												// ????????????????
												if (pActPet2 == NULL){
													// ?????????
													pActPet2 = MakeAnimDisp(750, 240, pet[petStatusNo].graNo, ANIM_DISP_MIX_PET_INIT);
													// ???????
													ItemMixPetNo = petStatusNo;
												}
												// ????
												SelectWazaNo = i;
												// ???????????
												mixPetNo = petStatusNo;

												// ??????
												play_se(212, 320, 240);
											}
											else{
#ifdef _CALLBUS
												if (petSkill[petStatusNo][i].skillId != PETSKILL_CALLBUS){
#endif
													// ?????????????????
													if (!(MenuToggleFlag & JOY_CTRL_I) || (MenuToggleFlag & JOY_CTRL_I && itemWndNo != 0)){
														int j;
														MenuToggleFlag |= JOY_CTRL_I;	// CTRL + I ??
														// ????????????
														for (j = 0; j < MENU_ITEM_0; j++) itemWndFontNo[j] = -2;
														for (j = 0; j < MENU_ITEM_0; j++) itemWndBtnFlag[j] = 0;
														itemWndNo = 0;
														mouse.itemNo = -1;
														itemWndDropGold = 0;

														// ??????????
														if (BattleResultWndFlag >= 1) DeathMenuAction();
														// ??????????
														DeathMenuAction2();
														// ????????
														saveUserSetting();

														MenuToggleFlag &= ~JOY_CTRL_M;	// ???????????
														// ???????????
														BattleResultWndFlag = FALSE;
														// ?????????
														MapWmdFlagBak = FALSE;
														// ????????
														play_se(202, 320, 240);
													}
													else
														// ???
														play_se(220, 320, 240);
#ifdef _CALLBUS
												}
#endif
											}
										}
										else{
											// ???
											play_se(220, 320, 240);
										}
									}
								}
							}
						}

						x = pActMenuWnd->x + 40;
						y = pActMenuWnd->y + 32;
						if (pet[petStatusNo].freeName[0] != NULL)
							CenteringStr(pet[petStatusNo].freeName, moji, PET_NAME_LEN);
						else
							CenteringStr(pet[petStatusNo].name, moji, PET_NAME_LEN);
						StockFontBuffer(x - 28, y, FONT_PRIO_FRONT, 0, moji, 0); y += 26; x += 18;
						for (i = 0; i < pet[petStatusNo].maxSkill; i++){
							if (petSkill[petStatusNo][i].useFlag == TRUE){
								int color = FONT_PAL_GRAY;
#ifdef _NEWFONT_
								sprintf_s(moji, "              %-42s", petSkill[petStatusNo][i].name);
#else
								sprintf_s(moji, "       %-22s", petSkill[petStatusNo][i].name);
#endif
								if (petSkill[petStatusNo][i].field != PETSKILL_FIELD_BATTLE) color = FONT_PAL_WHITE;
								petWndFontNo[i] = StockFontBuffer(x - 43, y, FONT_PRIO_FRONT, color, moji, 2);
							}
							y += 25;
						}

						// ?
						x = pActMenuWnd->x + 20;
						y = pActMenuWnd->y + 330;

						// ?????????
						petWndFontNo[7] = StockDispBuffer(x + 100 + 70, y, DISP_PRIO_IME3, CG_RETURN_BTN, 2);
						petWndFontNo[8] = StockDispBuffer(x + 16 + 16 + 8, y, DISP_PRIO_IME3, CG_PREV_BTN + petWndBtnFlag[8], 2);
						petWndFontNo[9] = StockDispBuffer(x + 50 + 16 + 18, y, DISP_PRIO_IME3, CG_NEXT_BTN + petWndBtnFlag[9], 2);
					}
				}
			}
			break;
#ifdef _STREET_VENDOR
		case 3: petWndNo = 0; break;
#endif
#ifdef _PETCOM_
		case 4:
			if (pActMenuWnd == NULL){
				pActMenuWnd = MakeWindowDisp(4, 4, 272, 320, 0, -1);
			}
			else{
				if (pActMenuWnd->hp > 0){
					StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my, DISP_PRIO_MENU, CG_PETCOM_WND, 1);
					int 按钮ID = StockDispBuffer(pActMenuWnd->x + 190, pActMenuWnd->y + 300, DISP_PRIO_IME3, CG_RETURN_BTN, 2);
					if ((mouse.onceState & MOUSE_LEFT_CRICK) && (HitDispNo == 按钮ID)){
						DeathAction(pActMenuWnd);
						pActMenuWnd = NULL;
						petWndNo = 1;
						play_se(203, 320, 240);
						break;
					}
					static int 宠算左按钮索引 = 0;
					static int 宠算右按钮索引 = 0;
					按钮ID = StockDispBuffer(pActMenuWnd->x + 60, pActMenuWnd->y + 300, DISP_PRIO_IME3, CG_PREV_BTN + 宠算左按钮索引, 2);
					if ((mouse.state & MOUSE_LEFT_CRICK && HitDispNo == 按钮ID)){
						宠算左按钮索引 = 1;
						if (mouse.onceState & MOUSE_LEFT_CRICK){
							while (1){
								petStatusNo--;
								if (petStatusNo <= -1) petStatusNo = 4;
								if (pet[petStatusNo].useFlag == TRUE) break;
							}
							play_se(203, 320, 240);
						}
					}
					else{
						宠算左按钮索引 = 0;
					}
					按钮ID = StockDispBuffer(pActMenuWnd->x + 104, pActMenuWnd->y + 300, DISP_PRIO_IME3, CG_NEXT_BTN + 宠算右按钮索引, 2);
					if ((mouse.state & MOUSE_LEFT_CRICK && HitDispNo == 按钮ID)){
						宠算右按钮索引 = 1;
						if (mouse.onceState & MOUSE_LEFT_CRICK){
							while (1){
								petStatusNo++;
								// ????????
								if (petStatusNo >= 5) petStatusNo = 0;
								if (pet[petStatusNo].useFlag == TRUE) break;
							}
							play_se(203, 320, 240);
						}
					}
					else{
						宠算右按钮索引 = 0;
					}
#define _PETCMOX 10
					if (pet[petStatusNo].freeName[0] != NULL)
						CenteringStr(pet[petStatusNo].freeName, moji, PET_NAME_LEN);
					else
						CenteringStr(pet[petStatusNo].name, moji, PET_NAME_LEN);
					StockFontBuffer(85, 36, FONT_PRIO_FRONT, 0, moji, 0);

					int atrFlag = FALSE;
					int atrGraNo[4];
					atrFlag = 0;
					if (pet[petStatusNo].earth > 0){
						if (pet[petStatusNo].earth > 50) atrGraNo[atrFlag] = CG_ATR_ICON_EARTH_BIG;
						else atrGraNo[atrFlag] = CG_ATR_ICON_EARTH_SML;
						atrFlag++;
					}
					if (pet[petStatusNo].water > 0){
						if (pet[petStatusNo].water > 50) atrGraNo[atrFlag] = CG_ATR_ICON_WATER_BIG;
						else atrGraNo[atrFlag] = CG_ATR_ICON_WATER_SML;
						atrFlag++;
					}
					if (pet[petStatusNo].fire > 0){
						if (pet[petStatusNo].fire > 50) atrGraNo[atrFlag] = CG_ATR_ICON_FIRE_BIG;
						else atrGraNo[atrFlag] = CG_ATR_ICON_FIRE_SML;
						atrFlag++;
					}
					if (pet[petStatusNo].wind > 0){
						if (pet[petStatusNo].wind > 50) atrGraNo[atrFlag] = CG_ATR_ICON_WIND_BIG;
						else atrGraNo[atrFlag] = CG_ATR_ICON_WIND_SML;
						atrFlag++;
					}
					if (atrFlag > 0) StockDispBuffer(232, 44, DISP_PRIO_IME3, atrGraNo[0], 0);
					if (atrFlag > 1) StockDispBuffer(232 + 16, 44, DISP_PRIO_IME3, atrGraNo[1], 0);

					sprintf_s(moji, "%3d", pet[petStatusNo].oldlevel);
					StockFontBuffer(80, 98, FONT_PRIO_FRONT, 0, moji, 0);
					sprintf_s(moji, "%3d", pet[petStatusNo].oldhp);
					StockFontBuffer(80, 120, FONT_PRIO_FRONT, 0, moji, 0);
					sprintf_s(moji, "%3d", pet[petStatusNo].oldatk);
					StockFontBuffer(80, 142, FONT_PRIO_FRONT, 0, moji, 0);
					sprintf_s(moji, "%3d", pet[petStatusNo].olddef);
					StockFontBuffer(80, 164, FONT_PRIO_FRONT, 0, moji, 0);
					sprintf_s(moji, "%3d", pet[petStatusNo].oldquick);
					StockFontBuffer(80, 186, FONT_PRIO_FRONT, 0, moji, 0);
					float  a, b, c, d, e, f;
					float a1, b1, c1, d1, e1;

					if (pet[petStatusNo].level != pet[petStatusNo].oldlevel && pet[petStatusNo].oldlevel){
						f = (float)(pet[petStatusNo].level - pet[petStatusNo].oldlevel);
						a = (pet[petStatusNo].maxHp - pet[petStatusNo].oldhp) / f;
						b = (pet[petStatusNo].atk - pet[petStatusNo].oldatk) / f;
						c = (pet[petStatusNo].def - pet[petStatusNo].olddef) / f;
						d = (pet[petStatusNo].quick - pet[petStatusNo].oldquick) / f;

						f = 140.00 - pet[petStatusNo].oldlevel;

						a1 = f*a + pet[petStatusNo].oldhp;
						b1 = f*b + pet[petStatusNo].oldatk;
						c1 = f*c + pet[petStatusNo].olddef;
						d1 = f*d + pet[petStatusNo].oldquick;
					}
					else{
						a = b = c = d = 1.0000;
						a1 = b1 = c1 = d1 = 0;
					}
					e = b + c + d;
					e1 = pet[petStatusNo].maxHp*0.25 + pet[petStatusNo].atk + pet[petStatusNo].def + pet[petStatusNo].quick;

					sprintf_s(moji, "%3d", 140);

					StockFontBuffer(155, 98, FONT_PRIO_FRONT, 0, moji, 0);
					sprintf_s(moji, "%3.2f", a1);
					StockFontBuffer(143, 120, FONT_PRIO_FRONT, 0, moji, 0);
					sprintf_s(moji, "%3.2f", b1);
					StockFontBuffer(143, 142, FONT_PRIO_FRONT, 0, moji, 0);
					sprintf_s(moji, "%3.2f", c1);
					StockFontBuffer(143, 164, FONT_PRIO_FRONT, 0, moji, 0);
					sprintf_s(moji, "%3.2f", d1);
					StockFontBuffer(143, 186, FONT_PRIO_FRONT, 0, moji, 0);

					sprintf_s(moji, "%3.4f", a);
					StockFontBuffer(215, 120, FONT_PRIO_FRONT, 0, moji, 0);
					sprintf_s(moji, "%3.4f", b);
					StockFontBuffer(215, 142, FONT_PRIO_FRONT, 0, moji, 0);
					sprintf_s(moji, "%3.4f", c);
					StockFontBuffer(215, 164, FONT_PRIO_FRONT, 0, moji, 0);
					sprintf_s(moji, "%3.4f", d);
					StockFontBuffer(215, 186, FONT_PRIO_FRONT, 0, moji, 0);
					sprintf_s(moji, "%3.4f", e);
					StockFontBuffer(80, 210, FONT_PRIO_FRONT, 0, moji, 0);

					sprintf_s(moji, "%3.2f", e1);
					StockFontBuffer(210, 210, FONT_PRIO_FRONT, 0, moji, 0);


				}
			}
			break;
#endif
		}
	}

	//? ??????????? *******************************************************/
	if (MenuToggleFlag & JOY_CTRL_I){
		int x1 = 0, y1 = 0;
#ifdef _NEWPANEL //Syu ADD 7.0 新人物状态介面
		SkillWndflag = false ; 
		/*		if( pActMenuWnd3 != NULL ) {
					DeathAction ( pActMenuWnd3 ) ; 
					pActMenuWnd3 = NULL ; 
					GetKeyInputFocus( &MyChatBuffer );
					}
					*/
#endif
#ifdef _NPC_DANCE
		if (pc.iDanceMode)
		{
			StockChatBufferLine("特殊状态无法使用道具", FONT_PAL_RED);
			MenuToggleFlag ^= JOY_CTRL_I;
			itemWndNo = 3;
		}
#endif

		switch (itemWndNo){
		case 0:	
			if (pActMenuWnd2 == NULL)
			{
				// ?????????
#ifndef _PET_ITEM
				pActMenuWnd2 = MakeWindowDisp(365, 4, 271, 440, 0, -1);
#ifdef _NEW_ITEM_
				道具栏页数 = 0;
#endif
#ifdef _MAGIC_ITEM_
				道具光环Act=NULL;
#endif
				InitItem(pActMenuWnd2->x, pActMenuWnd2->y, 0);
#else
				pActMenuWnd2 = MakeWindowDisp(351, 4, 271, 440, 0, -1);
#ifdef _NEW_ITEM_
				道具栏页数 = 0;
#endif
				pActMenuWnd2->x += 14;
				((WINDOW_DISP*)pActMenuWnd2->pYobi)->mx = 271 / 2 + pActMenuWnd2->x;
				InitItem(pActMenuWnd2->x, pActMenuWnd2->y, g_bPetItemWndFlag);
#endif
				// ???????????
				itemNo = -1;
				// ????????
				ItemMixPetNo = -1;
				// ??????????????????
				if (MenuToggleFlag & JOY_CTRL_E && mailWndNo == MAIL_WND_ITEM){
					// ???????????
					MenuToggleFlag &= ~JOY_CTRL_E;
					DeathMenuAction();
				}
#ifdef _MONEYINPUT //Syu ADD 手动输入金钱量
				MymoneyBuffer.buffer[0] = NULL;
				MymoneyBuffer.cnt = 0;
				MymoneyBuffer.cursor = 0;
				MymoneyBuffer.len = 8;
				MymoneyBuffer.color = 0;
				MymoneyBuffer.x = pActMenuWnd2->x + 191;
				MymoneyBuffer.y = pActMenuWnd2->y + 138;
				MymoneyBuffer.fontPrio = FONT_PRIO_FRONT;
				x1 = pActMenuWnd2->x + 173;
				y1 = pActMenuWnd2->y + 133;
#endif
#ifdef _PET_ITEM
				nSelectPet = -1;

				for (i = 0; i < MAX_PET; i++)
				{
					// 有这只宠
					if (pet[i].useFlag)
					{
						nSelectPet = i;
						break;
					}
				}
#endif
			}
			else
			{
				//andy_log 装备栏位修改处
				if (pActMenuWnd2->hp > 0)
				{
#ifdef _PET_ITEM
					// 检查是否按下了道具视窗左边的标签
					x1 = pActMenuWnd2->x - 21;
					y1 = pActMenuWnd2->y + 12;
					if (g_bPetItemWndFlag)
					{
						if (MakeHitBox(x1, y1, x1 + 23, y1 + 60, DISP_PRIO_IME3) == TRUE)	// 按下了人物装备
						if (mouse.onceState & MOUSE_LEFT_CRICK)
						{
							g_bPetItemWndFlag = false;
							InitItem(pActMenuWnd2->x, pActMenuWnd2->y, g_bPetItemWndFlag);
						}
					}
					else
					{
						if (MakeHitBox(x1, y1 + 78, x1 + 23, y1 + 142, DISP_PRIO_IME3) == TRUE)	// 按下了宠物装备
						if (mouse.onceState & MOUSE_LEFT_CRICK)
						{
							g_bPetItemWndFlag = true;
							InitItem(pActMenuWnd2->x, pActMenuWnd2->y, g_bPetItemWndFlag);
						}
					}	
					if (g_bPetItemWndFlag)
						StockDispBuffer(((WINDOW_DISP*)pActMenuWnd2->pYobi)->mx - 14, ((WINDOW_DISP*)pActMenuWnd2->pYobi)->my, DISP_PRIO_MENU, 26456, 1);
					else
						StockDispBuffer(((WINDOW_DISP*)pActMenuWnd2->pYobi)->mx - 14, ((WINDOW_DISP*)pActMenuWnd2->pYobi)->my, DISP_PRIO_MENU, CG_NEWITEM_WND, 1);
#else
#ifdef _ITEM_EQUITSPACE
					StockDispBuffer(((WINDOW_DISP *)pActMenuWnd2->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd2->pYobi)->my, DISP_PRIO_MENU, CG_NEWITEM_WND, 1);
#else
					StockDispBuffer(((WINDOW_DISP *)pActMenuWnd2->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd2->pYobi)->my, DISP_PRIO_MENU, CG_ITEM_WND_0, 1);

					StockDispBuffer(((WINDOW_DISP *)pActMenuWnd2->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd2->pYobi)->my, DISP_PRIO_MENU, CG_ITEM_WND_1, 1);
#endif
#endif

#ifdef _MONEYINPUT //Syu ADD 手动输入金钱量
#ifdef _PET_ITEM
					if (!g_bPetItemWndFlag)	// 人物装备栏才要显示以下的东西
#endif
					{
						x1 = pActMenuWnd2->x + 175;
						y1 = pActMenuWnd2->y + 133;
						if (MakeHitBox(x1, y1, x1 + 86, y1 + 25, DISP_PRIO_IME3) == TRUE)
						{
							if (mouse.onceState & MOUSE_LEFT_CRICK)
							{
								strcpy(MymoneyBuffer.buffer, "");
								MymoneyBuffer.buffer[0] = NULL;
								MymoneyBuffer.cursor = 0;
								MymoneyBuffer.cnt = 0;
								Moneyflag = true;
								GetKeyInputFocus(&MymoneyBuffer);
								play_se(217, 320, 240);
							}
						}
					}
					if (Moneyflag == true)
					{
						MymoneyBuffer.x = pActMenuWnd2->x + 240 - strlen(MymoneyBuffer.buffer) * 7;
						StockFontBuffer2(&MymoneyBuffer);
						itemWndDropGold = atoi(MymoneyBuffer.buffer);
						if (atoi(MymoneyBuffer.buffer) >= pc.gold)
						{
							sprintf_s(MymoneyBuffer.buffer, "%d", pc.gold);
							itemWndDropGold = pc.gold;
						}
					}
#endif
					// ????????
					if (mouse.onceState & MOUSE_LEFT_CRICK)
					{
						// 按下关闭钮
						if (HitDispNo == itemWndFontNo[0]){
#ifdef _MAGIC_ITEM_
							if(道具光环Act) DeathAction(道具光环Act);
							道具光环Act=NULL;
							extern void ClearMagicItemWin();
							ClearMagicItemWin();
#endif
							DeathAction(pActMenuWnd2);

							pActMenuWnd2 = NULL;
							// ????
							DeathAction(pActPet2);
							pActPet2 = NULL;
							// ????????
							ItemMixPetNo = -1;
							// ???????????
							MenuToggleFlag ^= JOY_CTRL_I;
							// ??????????????????
							if (MenuToggleFlag & JOY_CTRL_S){

								MenuToggleFlag ^= JOY_CTRL_S;	// CTRL + S ??
								// ??????????
								DeathMenuAction();
							}
							// ????????
							play_se(203, 320, 240);
							itemNo = -1;
#ifdef _MONEYINPUT //Syu ADD 手动输入金钱量
							GetKeyInputFocus(&MyChatBuffer);
							Moneyflag = false;
#endif
						}
						// 按了咒术
						if (HitDispNo == itemWndFontNo[1]){
							itemWndNo = 1;
							DeathAction(pActMenuWnd2);
#ifdef _MONEYINPUT //Syu ADD 手动输入金钱量
							GetKeyInputFocus(&MyChatBuffer);
							Moneyflag = false;
#endif
							pActMenuWnd2 = NULL;
							DeathAction(pActPet2);
							pActPet2 = NULL;
							ItemMixPetNo = -1;
							play_se(202, 320, 240);
							if (!(MenuToggleFlag & JOY_CTRL_S)){
								MenuToggleFlag ^= JOY_CTRL_S;	// CTRL + S ??
								for (i = 0; i < MENU_STATUS_0; i++) statusWndFontNo[i] = -2;
								for (i = 0; i < MENU_STATUS_0; i++) statusWndBtnFlag[i] = 0;
								statusWndNo = 1;
								DeathMenuAction();
								saveUserSetting();
								MenuToggleFlag &= JOY_CTRL_I;
								MenuToggleFlag |= JOY_CTRL_S;
							}
							else{
								if (statusWndNo != 1){
									DeathAction(pActMenuWnd);
									pActMenuWnd = NULL;
									statusWndNo = 1;
								}
							}
						}
						if (itemNo != -1 && MenuToggleFlag & JOY_CTRL_S && statusWndNo == 1){
							for (i = 0; i < 11; i++){
								if (HitFontNo == statusWndFontNo[i] && eventWarpSendFlag == FALSE){
									if (bNewServer)
										lssproto_ID_send(sockfd, nowGx, nowGy, itemNo, i);
									else
										old_lssproto_ID_send(sockfd, nowGx, nowGy, itemNo, i);
									play_se(212, 320, 240);
									itemNo = -1;

								}
							}
						}
#ifdef _PET_ITEM
						// 目前处在宠物装备视窗
						if (g_bPetItemWndFlag)
						{
							// 按下了左箭头
							if (HitDispNo == itemWndFontNo[5])
							{
								if (nSelectPet != -1)
								{
									do
									{
										nSelectPet--;
										if (nSelectPet == pc.ridePetNo)
											nSelectPet--;
										if (nSelectPet < 0)
											nSelectPet = MAX_PET - 1;
									}
									while (!pet[nSelectPet].useFlag);
								}
							}
							// 按下了右箭头
							if (HitDispNo == itemWndFontNo[6])
							{
								if (nSelectPet != -1)
								{
									do
									{
										nSelectPet++;
										if (nSelectPet == pc.ridePetNo)
											nSelectPet++;
										if (nSelectPet >= MAX_PET)
											nSelectPet = 0;
									}
									while (!pet[nSelectPet].useFlag);
								}
							}
						}
#endif
					}

					if (HitDispNo == itemWndFontNo[2]){
						if (mouse.onceState & MOUSE_LEFT_CRICK){
							if (itemWndDropGold > 0 && eventWarpSendFlag == FALSE){
								itemWndBtnFlag[2] = TRUE;
								play_se(217, 320, 240);
							}
							else{
								play_se(220, 320, 240);
							}
						}
						if (mouse.onceState & MOUSE_LEFT_CRICK_UP && itemWndBtnFlag[2] == TRUE){
							itemWndBtnFlag[2] = FALSE;
							play_se(212, 320, 240);
							if (bNewServer)
								lssproto_DG_send(sockfd, nowGx, nowGy, itemWndDropGold);
							else
								old_lssproto_DG_send(sockfd, nowGx, nowGy, itemWndDropGold);
							itemWndDropGold = 0;
#ifdef _MONEYINPUT //Syu ADD 手动输入金钱量
							Moneyflag = false;
#endif

						}
					}
					else
						itemWndBtnFlag[2] = FALSE;

					if (HitDispNo == itemWndFontNo[3])
					{
						if (mouse.onceState & MOUSE_LEFT_CRICK_UP && itemWndBtnFlag[3] == TRUE){

							itemWndBtnFlag[3] = FALSE;
						}
						// ??????
						if (itemWndBtnFlag[3] == TRUE){
#ifdef _MONEYINPUT //Syu ADD 手动输入金钱量
							GetKeyInputFocus(&MyChatBuffer);
							Moneyflag = false;
#endif
							// ???
							itemWndDropGold += itemWndDropGoldInc;
							// ?????????????
							itemWndDropGoldCnt++;
							// ????
							if (itemWndDropGoldCnt >= 30){
								// ?????????????
								itemWndDropGoldCnt = 0;
								// ????????
								if (itemWndDropGoldInc == 0) itemWndDropGoldInc = 1;
								else{
									// ?????
									itemWndDropGoldInc *= 5;
									// ????????
									if (itemWndDropGoldInc > 10000) itemWndDropGoldInc = 10000;
								}
							}
							// ????????
							if (itemWndDropGold >= pc.gold){
								itemWndDropGold = pc.gold;
								// ???
								play_se(220, 320, 240);
							}
						}
						// ????????
						if (mouse.onceState & MOUSE_LEFT_CRICK){
							// ???
							itemWndDropGold++;
							// ????????
							if (itemWndDropGold >= pc.gold){
								itemWndDropGold = pc.gold;
								// ???
								play_se(220, 320, 240);
							}
							else{
								// ?????
								itemWndDropGoldInc = 0;
								// ?????????????
								itemWndDropGoldCnt = 0;
								// ???????
								itemWndBtnFlag[3] = TRUE;
								// ?????
								play_se(217, 320, 240);
							}
						}

					}
					else
						// ??????
						itemWndBtnFlag[3] = FALSE;

					// ??????????
					if (HitDispNo == itemWndFontNo[4])
					{
						// ???????????????
						if (mouse.onceState & MOUSE_LEFT_CRICK_UP && itemWndBtnFlag[4] == TRUE){
							itemWndBtnFlag[4] = FALSE;
						}
						// ??????
						if (itemWndBtnFlag[4] == TRUE){
							// ????
#ifdef _MONEYINPUT //Syu ADD 手动输入金钱量
							GetKeyInputFocus(&MyChatBuffer);
							Moneyflag = false;
#endif
							itemWndDropGold -= itemWndDropGoldInc;
							// ?????????????
							itemWndDropGoldCnt++;
							// ????
							if (itemWndDropGoldCnt >= 30){
								// ?????????????
								itemWndDropGoldCnt = 0;
								// ????????
								if (itemWndDropGoldInc == 0) itemWndDropGoldInc = 1;
								else{
									// ?????
									itemWndDropGoldInc *= 5;
									// ????????
									if (itemWndDropGoldInc > 10000) itemWndDropGoldInc = 10000;
								}
							}
							// ????????
							if (itemWndDropGold < 0){
								itemWndDropGold = 0;
								// ???
								play_se(220, 320, 240);
							}
						}
						// ????????
						if (mouse.onceState & MOUSE_LEFT_CRICK){
							// ????
							itemWndDropGold--;
							// ????????
							if (itemWndDropGold <= 0){
								itemWndDropGold = 0;
								// ???
								play_se(220, 320, 240);
							}
							else{
								// ?????
								itemWndDropGoldInc = 0;
								// ?????????????
								itemWndDropGoldCnt = 0;
								// ???????
								itemWndBtnFlag[4] = TRUE;
								// ?????
								play_se(217, 320, 240);

							}
						}
					}
					else
						itemWndBtnFlag[4] = FALSE;

					if (pActMenuWnd2 != NULL)
					{
#ifdef _NEW_ITEM_
						//道具栏页数
						for (i = 0; i < 3; i++){
							if (i == 道具栏页数){
								StockDispBuffer(513, 188 + i * 56, DISP_PRIO_IME3, 55113 + i, 1);
							}
							else{
								BOOL flg = FALSE;
								if (i){
									if (pc.道具栏状态 & 1 << i){
										flg = TRUE;
									}
								}
								else flg = TRUE;
								if (flg){
									StockDispBuffer(518, 188 + i * 56, DISP_PRIO_IME3, 55110 + i, 1);
									if (MakeHitBox(508, 160 + i * 56, 508 + 20, 157 + i * 56 + 60, DISP_PRIO_IME4)){
										if (mouse.onceState & MOUSE_LEFT_CRICK){
											道具栏页数 = i;
										}
										if (mouse.itemNo != -1) 道具栏页数 = i;
									}
								}
								else StockDispBuffer(518, 188 + i * 56, DISP_PRIO_IME3, 55107 + i, 1);
							}
						}
#endif
						x = pActMenuWnd2->x + 16;
						y = pActMenuWnd2->y + 351 + 7;
						if (itemNo != -1)
						{
							StockBoxDispBuffer(ItemBuffer[itemNo].defX - 24, ItemBuffer[itemNo].defY - 24,
								ItemBuffer[itemNo].defX + 26, ItemBuffer[itemNo].defY + 23,
								DISP_PRIO_IME4, SYSTEM_PAL_AQUA, 0);
						}

						for (i = MAX_ITEM - 1; i >= 0; i--)
						{
#ifdef _NEW_ITEM_
							int 道具起始 = MAX_ITEMSTART + MAX_MAXHAVEITEM*道具栏页数;
							int 道具结束 = 道具起始 + MAX_MAXHAVEITEM;
							if (i >= MAX_ITEMSTART){
								if (i < 道具起始 || i >= 道具结束) continue;
							}
#endif
#ifdef _MAGIC_ITEM_
							if(pc.道具光环效果 > 100000){
								if(道具光环Act==NULL){
									道具光环Act = MakeAnimDisp(ItemBuffer[i].defX, ItemBuffer[i].defY,pc.道具光环效果, 0);
								}
							}
#endif
#ifdef _PET_ITEM
							// 当显示宠物道具栏时,宠物身上的装备只显示七个,多的就跳过不处理
							if (g_bPetItemWndFlag && (i >= PET_EQUIPNUM && i < MAX_ITEMSTART))
								continue;
#endif
#ifdef _ZENGJIASHUO_
#ifdef _MAGIC_ITEM_
#ifdef _SA_VERSION_25
							if ( i == 8 || i == 5 || i==7)
#endif
#else
							if ( i == 5 || i == 6 || i==7)
#endif
							{
								continue;
							}
#endif
							if (MakeHitBox(ItemBuffer[i].defX - 24, ItemBuffer[i].defY - 24,
								ItemBuffer[i].defX + 26, ItemBuffer[i].defY + 23, DISP_PRIO_IME3) == TRUE)
							{
#ifdef _PET_ITEM
								// 处理显示宠物装备
								if (g_bPetItemWndFlag && (i >= PET_HEAD && i < PET_EQUIPNUM) && nSelectPet > -1)
								{
									if (pet[nSelectPet].item[i].useFlag == TRUE && (ItemBuffer[i].mixFlag <= 2 || ItemBuffer[i].mixFlag == 10))
									{
										char *splitPoint = pet[nSelectPet].item[i].memo;
										char damage_msg[256];
										int color = pet[nSelectPet].item[i].color;

										StockFontBuffer(pActMenuWnd2->x + 16, pActMenuWnd2->y + 331, FONT_PRIO_FRONT, color, pet[nSelectPet].item[i].name, 0);

										// 显示物品耐久度
										sprintf_s(damage_msg, "耐久度(%s)", pet[nSelectPet].item[i].damage);
										StockFontBuffer(pActMenuWnd2->x + 150, pActMenuWnd2->y + 331, FONT_PRIO_FRONT, color, damage_msg, 0);

#ifdef	_NPC_ITEMUP
										ShowItemup(pet[nSelectPet].item[i].itemup,mouse.nowPoint.x,mouse.nowPoint.y);// 显示物品升级状态	
#endif									
#ifdef _ITEM_COUNTDOWN
										ShowCounttime(pet[nSelectPet].item[i].counttime,mouse.nowPoint.x,mouse.nowPoint.y);// 显示物品倒数计时状态	
#endif
										while (1)
										{
											if (strlen(splitPoint) > 28)
											{
												strncpy_s(moji, splitPoint, 28);
												moji[28] = NULL;
												if (GetStrLastByte(moji) == 3)
												{
													moji[27] = NULL;
													splitPoint += 27;
												}
												else
												{
													moji[28] = NULL;
													splitPoint += 28;
												}
												StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, moji, 0);
												y += 24;
											}
											else
											{
												strcpy(moji, splitPoint);
												StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, moji, 0);
												break;
											}
										}
									}
								}
								else
									// 人物装备
#endif
								{
#ifdef _ALCHEMIST
									if (pc.item[i].useFlag == TRUE)
#else
									if (pc.item[i].useFlag == TRUE && (ItemBuffer[i].mixFlag <= 2 || ItemBuffer[i].mixFlag == 10))
#endif
									{
										char *splitPoint = pc.item[i].memo;
										int color = pc.item[i].color;
										if (pc.transmigration == 0 && pc.level < pc.item[i].level) color = FONT_PAL_RED;
										StockFontBuffer(pActMenuWnd2->x + 16, pActMenuWnd2->y + 331, FONT_PRIO_FRONT, color, pc.item[i].name, 0);
										{

											// 显示物品耐久度
											char damage_msg[256];
											sprintf_s(damage_msg, "耐久度(%s)", pc.item[i].damage);
											StockFontBuffer(pActMenuWnd2->x + 150, pActMenuWnd2->y + 331, FONT_PRIO_FRONT, color, damage_msg, 0);
										}
#ifdef	_NPC_ITEMUP
										ShowItemup(pc.item[i].itemup, mouse.nowPoint.x, mouse.nowPoint.y);// 显示物品升级状态	
#endif
#ifdef _ITEM_COUNTDOWN
										ShowCounttime(pc.item[i].counttime, mouse.nowPoint.x, mouse.nowPoint.y);// 显示物品倒数计时状态	
#endif
										while (1){
											if (strlen(splitPoint) > 28){
												strncpy_s(moji, splitPoint, 28);
												moji[28] = NULL;
												if (GetStrLastByte(moji) == 3){
													moji[27] = NULL;
													splitPoint += 27;
												}
												else{
													moji[28] = NULL;
													splitPoint += 28;
												}
												StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, moji, 0); y += 24;
											}
											else{
												strcpy(moji, splitPoint);
												StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, moji, 0);
												break;
											}
										}
									}
								}
								if (pActPet2 == NULL)
								{
									if (mouse.onceState & MOUSE_LEFT_CRICK && mouse.itemNo == -1)
									{
#ifdef _MAGIC_ITEM_
										extern int MagicItemCombinData[3];
										if(MagicItemCombinData[0]==i) MagicItemCombinData[0]=0;
										if(MagicItemCombinData[1]==i) MagicItemCombinData[1]=0;
										if(MagicItemCombinData[2]==i) MagicItemCombinData[2]=0;
#endif
										ItemBuffer[i].mixFlag = FALSE;
#ifdef _PET_ITEM
										if (g_bPetItemWndFlag && (i >= PET_HEAD && i < PET_EQUIPNUM) && nSelectPet > -1)
										{
											if (pet[nSelectPet].item[i].useFlag == TRUE)
											{
												ItemBuffer[i].dragFlag = TRUE;
												mouse.itemNo = i;
												itemNo = -1;
											}
										}
										else
#endif
										if (pc.item[i].useFlag == TRUE){
											ItemBuffer[i].dragFlag = TRUE;
											mouse.itemNo = i;
											itemNo = -1;
										}
									}
									if (mouse.onceState & MOUSE_LEFT_CRICK_UP)
									{
										if (mouse.itemNo != -1)
										{
											if (mouse.itemNo != i && ItemBuffer[i].mixFlag == 0)
											{
#ifdef _PET_ITEM
												if (g_bPetItemWndFlag)
												{
													if (nSelectPet > -1 && !(mouse.itemNo >= CHAR_EQUIPPLACENUM && i >= CHAR_EQUIPPLACENUM))
													{
														if (i < CHAR_EQUIPPLACENUM && nSelectPet == pc.ridePetNo)	// 若是要装上去,检查是不是骑宠
															StockChatBufferLine("骑宠不可装装备！", FONT_PAL_YELLOW);
														else
															lssproto_PetItemEquip_send(sockfd, nowGx, nowGy, nSelectPet, mouse.itemNo, i);
													}
													else
													{
														if (bNewServer)
															lssproto_MI_send(sockfd, mouse.itemNo, i);
														else
															old_lssproto_MI_send(sockfd, mouse.itemNo, i);
													}
												}
												else
#endif
												{
													if (bNewServer)
														lssproto_MI_send(sockfd, mouse.itemNo, i);
													else
														old_lssproto_MI_send(sockfd, mouse.itemNo, i);
												}
												play_se(217, 320, 240);
											}
											ItemBuffer[mouse.itemNo].dragFlag = FALSE;
											mouse.itemNo = -1;
										}
									}
									if (mouse.onceState & MOUSE_LEFT_DBL_CRICK && ItemUseTime < TimeGetTime() - 500)
									{
										ItemUseTime = TimeGetTime();
#ifdef _PET_ITEM
										// 若目前是在人物装备栏且想装备宠物装备时,自动切到宠物装备栏
										if (!g_bPetItemWndFlag)
										{
											if (pc.item[i].useFlag && pc.item[i].type >= ITEM_PET_HEAD && pc.item[i].type < ITEM_CATEGORYNUM)
											{
												g_bPetItemWndFlag = true;
												InitItem(pActMenuWnd2->x, pActMenuWnd2->y, g_bPetItemWndFlag);
												break;
											}
										}
										// 若目前是在宠物装备栏且想装备人物装备时,自动切到人物装备栏
										else
										{
											if (pc.item[i].useFlag && pc.item[i].type < ITEM_PET_HEAD && i >= MAX_ITEMSTART)
											{
												g_bPetItemWndFlag = false;
												InitItem(pActMenuWnd2->x, pActMenuWnd2->y, g_bPetItemWndFlag);
												break;
											}
										}
#endif
										if (pc.item[i].useFlag != TRUE || pc.item[i].field == ITEM_FIELD_BATTLE || pc.transmigration == 0 && pc.level < pc.item[i].level){
											play_se(220, 320, 240);
										}
										else{ // ????
											mouse.itemNo = -1;
											switch (pc.item[i].target){
											case ITEM_TARGET_MYSELF:	// ??
												if (eventWarpSendFlag == FALSE){
#ifdef _ITEM_JIGSAW
													if (strlen(pc.item[i].jigsaw)){
														if (pc.item[i].graNo >= 25151 && pc.item[i].graNo <= 25159 //底版道具图
															&& strcmp(pc.item[i].jigsaw, "111111111")
															|| pc.item[i].graNo == 25150){
															SetJigsaw(pc.item[i].graNo, pc.item[i].jigsaw);
															if (!(MenuToggleFlag & JOY_CTRL_S)){
																int j;
																MenuToggleFlag ^= JOY_CTRL_S;	// CTRL + S ??
																for (j = 0; j < MENU_STATUS_0; j++) statusWndFontNo[j] = -2;
																for (j = 0; j < MENU_STATUS_0; j++) statusWndBtnFlag[j] = 0;
																statusWndNo = 4;
																DeathMenuAction();
																saveUserSetting();
																play_se(202, 320, 240);
																MenuToggleFlag &= JOY_CTRL_I;
																MenuToggleFlag |= JOY_CTRL_S;
															}
															else{
																if (statusWndNo != 4){
																	DeathAction(pActMenuWnd);
																	pActMenuWnd = NULL;
																	statusWndNo = 4;
																	play_se(202, 320, 240);
																}
															}
															JigsawIdx = i;
															break;
														}
														else{
															if (!DrawJigsawFlag){
																if (pc.item[i].graNo >= 25151 && pc.item[i].graNo <= 25159)
																	StockChatBufferLine("请先开启大拼图底版", FONT_PAL_YELLOW);
																else
																	StockChatBufferLine("请先开启小拼图底版", FONT_PAL_YELLOW);
																break;
															}
															if (statusWndNo == 4 && CheckJigsaw(pc.item[i].graNo))
																lssproto_ID_send(sockfd, nowGx, nowGy, i, JigsawIdx);
															break;
														}
													}
													if (CheckJigsaw(pc.item[i].graNo)){
														if (!DrawJigsawFlag){
															if (pc.item[i].graNo >= 25151 && pc.item[i].graNo <= 25159){
																StockChatBufferLine("请先开启大拼图底版", FONT_PAL_YELLOW);
															}
															else{
																StockChatBufferLine("请先开启小拼图底版", FONT_PAL_YELLOW);
															}
															break;
														}
														if (statusWndNo == 4)
															lssproto_ID_send(sockfd, nowGx, nowGy, i, JigsawIdx);
														break;
													}
#endif
													if (bNewServer)
														lssproto_ID_send(sockfd, nowGx, nowGy, i, 0);
													else
														old_lssproto_ID_send(sockfd, nowGx, nowGy, i, 0);
													play_se(212, 320, 240);
												}
												break;
											case ITEM_TARGET_OTHER:	// ????)
												itemNo = i;
												play_se(217, 320, 240);
												if (!(MenuToggleFlag & JOY_CTRL_S)){
													int j;
													MenuToggleFlag ^= JOY_CTRL_S;	// CTRL + S ??
													for (j = 0; j < MENU_STATUS_0; j++) statusWndFontNo[j] = -2;
													for (j = 0; j < MENU_STATUS_0; j++) statusWndBtnFlag[j] = 0;
													statusWndNo = 1;
													DeathMenuAction();
													saveUserSetting();
													play_se(202, 320, 240);
													MenuToggleFlag &= JOY_CTRL_I;
													MenuToggleFlag |= JOY_CTRL_S;
												}
												else{
													if (statusWndNo != 1){
														DeathAction(pActMenuWnd);
														pActMenuWnd = NULL;
														statusWndNo = 1;
														play_se(202, 320, 240);
													}
												}
												break;
#ifdef _PET_ITEM
											case ITEM_TARGET_PET:
												if (eventWarpSendFlag == FALSE)
												{
													// 若是在已装备的装备上连点二下则不动作
													if (i >= PET_HEAD && i < PET_EQUIPNUM)
														break;
													if (pc.ridePetNo != -1 && nSelectPet == pc.ridePetNo)	// 检查是不是骑宠
														StockChatBufferLine("骑宠不可装装备！", FONT_PAL_YELLOW);
													else
														lssproto_PetItemEquip_send(sockfd, nowGx, nowGy, nSelectPet, i, -1);
													play_se(212, 320, 240);
												}
												break;
#endif
											}
										}
									}
									//#ifdef _STONDEBUG_		
									// ??????????????????????
									if (mouse.onceState & MOUSE_RIGHT_CRICK  && mouse.itemNo == -1)
									{
										int j;
										int cnt = 0;
										// ?????????????????????????????
										if (pc.item[i].useFlag == TRUE && i >= MAX_ITEMSTART /*&& pc.item[ i ].sendFlag & ITEM_FLAG_MIX */)
										{
											int j;
											int flag = 0;
											// ????????
											if (itemNo != -1) itemNo = -1;
											// ??????
											for (j = MAX_ITEMSTART; j < MAX_ITEM; j++)
											{
												// ??????????
												if (ItemBuffer[j].mixFlag >= TRUE){
													flag = ItemBuffer[j].mixFlag;
													break;
												}
											}
											// ?????????
											if (ItemBuffer[i].mixFlag == FALSE)
											{
												// ????????????
												if (flag == 0){
													// ????
													if (pc.item[i].sendFlag & ITEM_FLAG_COOKING_MIX){
														// ??????????????
														if (CheckPetSkill(PETSKILL_MERGE2) == TRUE){
															ItemBuffer[i].mixFlag = 2;
															// ?????
															play_se(217, 320, 240);
														}
														else{
															// ???
															play_se(220, 320, 240);
														}
													}
													//andy_add
													else if (pc.item[i].sendFlag & ITEM_FLAG_METAL_MIX ||
														pc.item[i].sendFlag & ITEM_FLAG_JEWEL_MIX){
														if (CheckPetSkill(PETSKILL_INSLAY)){
															ItemBuffer[i].mixFlag = 10;
															play_se(217, 320, 240);
														}
														else{
															play_se(220, 320, 240);
														}
													}
													else if (pc.item[i].sendFlag & ITEM_FLAG_FIX_MIX){
														if (CheckPetSkill(PETSKILL_FIXITEM) || CheckPetSkill(PETSKILL_FIXITEM2)){
															ItemBuffer[i].mixFlag = 11;
															play_se(217, 320, 240);
														}
														else{
															play_se(220, 320, 240);
														}
													}
#ifdef _ITEM_INTENSIFY
													else if (pc.item[i].sendFlag & ITEM_FLAG_INTENSIFY_MIX){
														if (CheckPetSkill(PETSKILL_INTENSIFY)){
															ItemBuffer[i].mixFlag = 12;
															play_se(217, 320, 240);
														}
														else{
															play_se(220, 320, 240);
														}
													}
#endif
#ifdef _ITEM_UPINSLAY
													else if (pc.item[i].sendFlag & ITEM_FLAG_UPINSLAY_MIX){
														if (CheckPetSkill(PETSKILL_UPINSLAY)){
															ItemBuffer[i].mixFlag = 13;
															play_se(217, 320, 240);
														}
														else{
															play_se(220, 320, 240);
														}
													}
#endif
													else{
#ifdef _ALCHEMIST // 第一个是否精炼物
														if (CheckPetSkill(PETSKILL_ALCHEMIST) == TRUE &&
															pc.item[i].sendFlag & ITEM_FLAG_MIX &&
															strcmp(pc.item[i].alch, "杂") != NULL){
															ItemBuffer[i].mixFlag = 9;
														}
														else
#endif
															// ??????????????
														if (CheckPetSkill(PETSKILL_MERGE) == TRUE &&
															pc.item[i].sendFlag & ITEM_FLAG_MIX){
															ItemBuffer[i].mixFlag = 1;
															// ?????
															play_se(217, 320, 240);
														}
														else{
															// ???
															play_se(220, 320, 240);
														}
													}
												}
												else{

													// ????
													if (pc.item[i].sendFlag & ITEM_FLAG_COOKING_MIX){
														if (flag == 2){
															ItemBuffer[i].mixFlag = 2;
															// ?????
															play_se(217, 320, 240);
														}
														else{
															// ???
															play_se(220, 320, 240);
														}
													}

													else{
														if (flag == 1){
															ItemBuffer[i].mixFlag = 1;
															// ?????
															play_se(217, 320, 240);
														}
#ifdef _ALCHEMIST // 第一个之后的精炼物
														else if (flag == 9){
															if (strcmp(pc.item[i].alch, pc.item[j].alch) == FALSE) {
																ItemBuffer[i].mixFlag = 9;
															}
															play_se(217, 320, 240);
														}
#endif
														else if (flag == 10){	//andy_add inslay
															ItemBuffer[i].mixFlag = 10;
															play_se(217, 320, 240);
														}
														else if (flag == 11){	//andy_add fixitem
															ItemBuffer[i].mixFlag = 11;
															play_se(217, 320, 240);
														}
#ifdef _ITEM_INTENSIFY
														else if (flag == 12){
															ItemBuffer[i].mixFlag = 12;
															play_se(217, 320, 240);
														}
#endif
#ifdef _ITEM_UPINSLAY
														else if (flag == 13){
															ItemBuffer[i].mixFlag = 13;
															play_se(217, 320, 240);
														}
#endif

														else{
															// ???
															play_se(220, 320, 240);
														}

													}
												}
											}
											else
											{
#ifdef _ALCHEMIST // 取消一个合成物时...
												if (ItemBuffer[i].mixFlag == 9) {
													int k;
													ItemBuffer[i].mixFlag = 1;
													for (k = MAX_ITEMSTART; k < MAX_ITEM; k++){
														if (ItemBuffer[k].mixFlag >= TRUE && (k != i)){
															ItemBuffer[i].mixFlag = FALSE;
															break;
														}
													}
												}
#ifdef _MAGIC_ITEM_
												else if (ItemBuffer[i].mixFlag == 14) {
													extern int MagicItemCombinData[3],MagicItemPosState[4];
													MagicItemCombinData[0]=-1;
													
													ItemBuffer[i].mixFlag = FALSE;
												}
												else if (ItemBuffer[i].mixFlag == 15) {
													extern int MagicItemCombinData[3],MagicItemPosState[4];
													MagicItemCombinData[1]=-1;
													
													ItemBuffer[i].mixFlag = FALSE;
												}
												else if (ItemBuffer[i].mixFlag == 16) {
													extern int MagicItemCombinData[3],MagicItemPosState[4];
													MagicItemCombinData[2]=-1;
													
													ItemBuffer[i].mixFlag = FALSE;
												}
#endif
												else {
													ItemBuffer[i].mixFlag = FALSE;
												}
#else
												ItemBuffer[i].mixFlag = FALSE;
#endif
												play_se(217, 320, 240);
											}
										}
										else
											play_se(220, 320, 240);

										for (j = MAX_ITEMSTART; j < MAX_ITEM; j++)
										{
											if (ItemBuffer[j].mixFlag >= 1) cnt++;
										}
										if (cnt >= 2)
										{
											if (!(MenuToggleFlag & JOY_CTRL_P))
											{

												MenuToggleFlag ^= JOY_CTRL_P;	// CTRL + S ??
												for (j = 0; j < MENU_PET_0; j++) petWndFontNo[j] = -2;
												for (j = 0; j < MENU_PET_0; j++) petWndBtnFlag[j] = 0;
												petWndNo = 0;
												DeathMenuAction();
												saveUserSetting();
												play_se(202, 320, 240);

												MenuToggleFlag &= JOY_CTRL_I;	// ??????????????
												MenuToggleFlag |= JOY_CTRL_P;	// ??????????
											}
										}
									}
								}
							}
#ifdef _PET_ITEM
							if (g_bPetItemWndFlag && nSelectPet > -1 && (i >= PET_HEAD && i < PET_EQUIPNUM) && pet[nSelectPet].useFlag == TRUE)
								StockDispBuffer(ItemBuffer[i].defX, ItemBuffer[i].defY, ItemBuffer[i].dispPrio, pet[nSelectPet].item[i].graNo, 0);
							else
#endif
							if (pc.item[i].useFlag == TRUE)
							{
#ifdef _PET_ITEM
								if (!(g_bPetItemWndFlag && i < CHAR_EQUIPPLACENUM))
#endif
								{
#if 0
									if (ItemBuffer[i].mixFlag <= 9)
#else
									if (ItemBuffer[i].mixFlag <= 2)
#endif
									{
										StockDispBuffer(ItemBuffer[i].defX, ItemBuffer[i].defY, ItemBuffer[i].dispPrio, pc.item[i].graNo, 0);
#ifdef _ITEM_PILENUMS
										if (pc.item[i].pile > 1)
										{
											char pile[256];
											sprintf_s(pile, "%d", pc.item[i].pile);
											StockFontBuffer(ItemBuffer[i].defX + 10, ItemBuffer[i].defY, FONT_PRIO_FRONT, FONT_PAL_WHITE, pile, 0);
										}
#endif
									}
#ifdef _ALCHEMIST
									else if (ItemBuffer[i].mixFlag == 9)
									{
										StockDispBuffer(ItemBuffer[i].defX, ItemBuffer[i].defY, ItemBuffer[i].dispPrio, pc.item[i].graNo, 0);
									}
#endif
									else if (ItemBuffer[i].mixFlag == 10)
									{ //andy_add inslay
										StockDispBuffer(ItemBuffer[i].defX, ItemBuffer[i].defY, ItemBuffer[i].dispPrio, pc.item[i].graNo, 0);
									}
									else if (ItemBuffer[i].mixFlag == 11)
									{ //andy_add fixitem
										StockDispBuffer(ItemBuffer[i].defX, ItemBuffer[i].defY, ItemBuffer[i].dispPrio, pc.item[i].graNo, 0);
									}
#ifdef _ITEM_INTENSIFY
									else if (ItemBuffer[i].mixFlag == 12)
									{ //andy_add fixitem
										StockDispBuffer(ItemBuffer[i].defX, ItemBuffer[i].defY, ItemBuffer[i].dispPrio, pc.item[i].graNo, 0);
									}
#endif
#ifdef _ITEM_UPINSLAY
									else if (ItemBuffer[i].mixFlag == 13)
									{ //andy_add fixitem
										StockDispBuffer(ItemBuffer[i].defX, ItemBuffer[i].defY, ItemBuffer[i].dispPrio, pc.item[i].graNo, 0);
									}
#endif
									else if (ItemBuffer[i].mixFlag == 3)
									{	// ??????
										// ????
										StockDispBuffer(pActPet2->x, pActPet2->y - 16, DISP_PRIO_DRAG, pc.item[i].graNo, 0);
									}
									else if (ItemBuffer[i].mixFlag == 4)
									{	// ????????
										// ????
										StockDispBuffer(pActPet2->x, pActPet2->y, ItemBuffer[i].dispPrio, pc.item[i].graNo, 0);
									}
#ifdef _MAGIC_ITEM_
									if (ItemBuffer[i].mixFlag == 14 || ItemBuffer[i].mixFlag == 15 || ItemBuffer[i].mixFlag == 16){
										StockDispBuffer(ItemBuffer[i].defX, ItemBuffer[i].defY, ItemBuffer[i].dispPrio, pc.item[i].graNo, 0);
									}
									
#endif



								}
							}
							// 显示合成文字
							if (ItemBuffer[i].mixFlag >= TRUE)
							{
								// ?????????????
								if (pc.item[i].useFlag == TRUE){
									// 普通合成的时候
									if (ItemBuffer[i].mixFlag == 1){
										StockFontBuffer(ItemBuffer[i].defX - 17, ItemBuffer[i].defY, FONT_PRIO_FRONT, 0, "合成", 0);
									}
									else
										// 料理合成的时候
									if (ItemBuffer[i].mixFlag == 2){
										StockFontBuffer(ItemBuffer[i].defX - 17, ItemBuffer[i].defY, FONT_PRIO_FRONT, 0, "料理", 0);
									}
#ifdef _ALCHEMIST
									if (ItemBuffer[i].mixFlag == 9){
										StockFontBuffer(ItemBuffer[i].defX - 17, ItemBuffer[i].defY, FONT_PRIO_FRONT, 0, "精炼", 0);
									}
#endif
									if (ItemBuffer[i].mixFlag == 10){// PETSKILL_INSLAY ANDY_ADD
										StockFontBuffer(ItemBuffer[i].defX - 17, ItemBuffer[i].defY, FONT_PRIO_FRONT, 0, "精工", 0);
									}
									if (ItemBuffer[i].mixFlag == 11){// PETSKILL_FIXITEM ANDY_ADD
										StockFontBuffer(ItemBuffer[i].defX - 17, ItemBuffer[i].defY, FONT_PRIO_FRONT, 0, "修复", 0);
									}
#ifdef _ITEM_INTENSIFY
									if (ItemBuffer[i].mixFlag == 12){
										StockFontBuffer(ItemBuffer[i].defX - 17, ItemBuffer[i].defY, FONT_PRIO_FRONT, 0, "强化", 0);
									}
#endif
#ifdef _ITEM_UPINSLAY
									if (ItemBuffer[i].mixFlag == 13){
										StockFontBuffer(ItemBuffer[i].defX - 17, ItemBuffer[i].defY, FONT_PRIO_FRONT, 0, "凿孔", 0);
									}
#endif
#ifdef _MAGIC_ITEM_
									if (ItemBuffer[i].mixFlag == 14){
										StockFontBuffer(ItemBuffer[i].defX - 17, ItemBuffer[i].defY, FONT_PRIO_FRONT, 0, "主体", 0);
									}
									if (ItemBuffer[i].mixFlag == 15){
										StockFontBuffer(ItemBuffer[i].defX - 17, ItemBuffer[i].defY, FONT_PRIO_FRONT, 0, "宝石", 0);
									}
									if (ItemBuffer[i].mixFlag == 16){
										StockFontBuffer(ItemBuffer[i].defX - 17, ItemBuffer[i].defY, FONT_PRIO_FRONT, 0, "祝福", 0);
									}
#endif

								}
								else{
									// ???????
									ItemBuffer[i].mixFlag = TRUE;
								}
							}
						}
						// ?????????
						if (mouse.onceState & MOUSE_LEFT_CRICK_UP)
						{

#ifdef _MAGIC_ITEM_

							if(MagicItemActAddr){
								if(MagicItemPosState[0]){
									if(MagicItemCombinData[0]) ItemBuffer[MagicItemCombinData[0]].mixFlag= 0;
							//		if(mouse.itemNo!=-1 && pc.item[mouse.itemNo].道具类型==25) {
									if(mouse.itemNo!=-1 && pc.item[mouse.itemNo].道具类型>=0&&pc.item[mouse.itemNo].道具类型<20 && pc.item[mouse.itemNo].道具类型!=16) {
										MagicItemCombinData[0] = mouse.itemNo;
										ItemBuffer[mouse.itemNo].mixFlag=14;
									}
									else MagicItemCombinData[0]=-1;
								}
								else if(MagicItemPosState[1]){
									if(MagicItemCombinData[1]) ItemBuffer[MagicItemCombinData[1]].mixFlag= 0;
									if(mouse.itemNo!=-1 && pc.item[mouse.itemNo].道具类型==38){
										ItemBuffer[mouse.itemNo].mixFlag=15;
										MagicItemCombinData[1] = mouse.itemNo;
									}else MagicItemCombinData[1]=-1;
		
								}
								else if(MagicItemPosState[2]){
									if(MagicItemCombinData[2]) ItemBuffer[MagicItemCombinData[2]].mixFlag= 0;
									if(mouse.itemNo!=-1  && pc.item[mouse.itemNo].道具类型==39) {
										ItemBuffer[mouse.itemNo].mixFlag=16;
										MagicItemCombinData[2] = mouse.itemNo;
									}else MagicItemCombinData[2]=-1;
								}
								mouse.itemNo=-1;
							}
#endif

							// ???????????

							if (mouse.itemNo != -1){
								// ????????
								if (mouse.nowPoint.x <= pActMenuWnd2->x && mouse.itemNo != -1 && eventWarpSendFlag == FALSE){
#ifdef _PET_ITEM
									if (g_bPetItemWndFlag)
										lssproto_PetItemEquip_send(sockfd, nowGx, nowGy, nSelectPet, mouse.itemNo, -2);
									else
#endif
										// ???????
									if (bNewServer) {
										lssproto_DI_send(sockfd, nowGx, nowGy, mouse.itemNo);
									}
									else
										old_lssproto_DI_send(sockfd, nowGx, nowGy, mouse.itemNo);

								}
								// ??????????
								ItemBuffer[mouse.itemNo].dragFlag = FALSE;
								// ????????????
								mouse.itemNo = -1;
								// ?????
								play_se(217, 320, 240);
							}
						}
						// ?????????
						if (mouse.itemNo != -1)
						{
							// ??????????????
							if (mouse.onceState & MOUSE_RIGHT_CRICK)
							{
								// ??????
								// ??????????
								ItemBuffer[mouse.itemNo].dragFlag = FALSE;
								// ????????????
								mouse.itemNo = -1;
								// ?????
								play_se(217, 320, 240);
							}
							// ????
#ifdef _PET_ITEM
							if (g_bPetItemWndFlag && nSelectPet > -1 && (mouse.itemNo >= PET_HEAD && mouse.itemNo < PET_EQUIPNUM) && pet[nSelectPet].useFlag == TRUE)
								StockDispBuffer(mouse.nowPoint.x, mouse.nowPoint.y, DISP_PRIO_DRAG, pet[nSelectPet].item[mouse.itemNo].graNo, 0);
							else
#endif
								StockDispBuffer(mouse.nowPoint.x, mouse.nowPoint.y, DISP_PRIO_DRAG, pc.item[mouse.itemNo].graNo, 0);
						}
						// ????????
						if (pActMenuWnd2 != NULL)
						{
#ifdef _DIEJIA_
							static int 叠加时间 = 0;
							int 按钮ID = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd2->pYobi)->mx - 60, pActMenuWnd2->y + 422, DISP_PRIO_IME3, 55246, 2);
							if (mouse.onceState & MOUSE_LEFT_CRICK){
								if (HitDispNo == 按钮ID){
									if (TimeGetTime() > 叠加时间){
										叠加时间 = TimeGetTime() + 4000;
										int itemMax = CHAR_EQUIPPLACENUM + MAX_MAXHAVEITEM;
#ifdef _NEW_ITEM_
										if (pc.道具栏状态 & 1 << 1){
											itemMax += MAX_MAXHAVEITEM;
										}
										if (pc.道具栏状态 & 1 << 2){
											itemMax += MAX_MAXHAVEITEM;
										}
#endif
										chatStrSendForServer("/叠加",0);
									}
									else{
										StockChatBufferLine("你点击的太频繁啦！", FONT_PAL_YELLOW);
									}
								}
							}
							itemWndFontNo[0] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd2->pYobi)->mx + 60, pActMenuWnd2->y + 422, DISP_PRIO_IME3, CG_CLOSE_BTN, 2);
#else
							itemWndFontNo[ 0 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->mx, pActMenuWnd2->y + 422, DISP_PRIO_IME3, CG_CLOSE_BTN, 2 );
#endif



#ifdef _PET_ITEM
							if (g_bPetItemWndFlag)
							{
								itemWndFontNo[1] = -2;
								// 显示左箭头
								itemWndFontNo[5] = StockDispBuffer(pActMenuWnd2->x + 188, pActMenuWnd2->y + 142, DISP_PRIO_IME3, 26047, 2);
								// 显示右箭头
								itemWndFontNo[6] = StockDispBuffer(pActMenuWnd2->x + 236, pActMenuWnd2->y + 142, DISP_PRIO_IME3, 26048, 2);

								// 显示宠物资料
								if (nSelectPet > -1 && pet[nSelectPet].useFlag)
								{
									char szTemp[16];
									WORD wSpecies = HIWORD(pet[nSelectPet].fusion);

									if (wSpecies < 0 || wSpecies >= MAX_PET_SPECIES)
										wSpecies = MAX_PET_SPECIES;
									// 显示宠物装备栏的底图
									for (i = 0; i < PET_EQUIPNUM; i++)
									{
										if (byShowPetItemBackground[wSpecies] & (1 << i))
											StockDispBuffer(ItemBuffer[i].defX, ItemBuffer[i].defY, DISP_PRIO_IME3, nPetItemEquipBmpNumber[i][0], 0);
										else
											StockDispBuffer(ItemBuffer[i].defX, ItemBuffer[i].defY, DISP_PRIO_IME3, nPetItemEquipBmpNumber[i][1], 0);
									}

									// 显示宠物名称
									if (strlen(pet[nSelectPet].freeName) > 0)
										StockFontBuffer(pActMenuWnd2->x + 148, pActMenuWnd2->y + 17, FONT_PRIO_FRONT, 0, pet[nSelectPet].freeName, 0);
									else
										StockFontBuffer(pActMenuWnd2->x + 148, pActMenuWnd2->y + 17, FONT_PRIO_FRONT, 0, pet[nSelectPet].name, 0);
									// 显示宠物的属性
									_itoa_s(pet[nSelectPet].maxHp, szTemp, 10);
									StockFontBuffer(pActMenuWnd2->x + 222, pActMenuWnd2->y + 40, FONT_PRIO_FRONT, FONT_PAL_WHITE, szTemp, 0);
									_itoa_s(pet[nSelectPet].atk, szTemp, 10);
									StockFontBuffer(pActMenuWnd2->x + 222, pActMenuWnd2->y + 40 + 25, FONT_PRIO_FRONT, FONT_PAL_WHITE, szTemp, 0);
									_itoa_s(pet[nSelectPet].def, szTemp, 10);
									StockFontBuffer(pActMenuWnd2->x + 222, pActMenuWnd2->y + 40 + 49, FONT_PRIO_FRONT, FONT_PAL_WHITE, szTemp, 0);
									_itoa_s(pet[nSelectPet].quick, szTemp, 10);
									StockFontBuffer(pActMenuWnd2->x + 222, pActMenuWnd2->y + 40 + 73, FONT_PRIO_FRONT, FONT_PAL_WHITE, szTemp, 0);
								}
								else
								{
									nSelectPet = -1;
									for (i = 0; i < MAX_PET; i++)
									{
										// 有这只宠
										if (pet[i].useFlag)
										{
											nSelectPet = i;
											break;
										}
									}
								}
							}
							else
#endif
							{
								itemWndFontNo[1] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd2->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd2->pYobi)->my, DISP_PRIO_IME3, CG_ITEM_WND_JUJUTU_BTN, 2);
								// ??
								sprintf_s(moji, "%7d", pc.gold);
								StockFontBuffer(pActMenuWnd2->x + 32 + 48 * 3 + 16, pActMenuWnd2->y + 87, FONT_PRIO_FRONT, 0, moji, 0);
								// ?????
								sprintf_s(moji, "%7d", itemWndDropGold);
#ifdef _MONEYINPUT //Syu ADD 手动输入金钱量
								if (Moneyflag == false)
									StockFontBuffer(pActMenuWnd2->x + 32 + 48 * 3 + 16, pActMenuWnd2->y + 138, FONT_PRIO_FRONT, 0, moji, 0);
#else
								StockFontBuffer(pActMenuWnd2->x + 32 + 48 * 3 + 16, pActMenuWnd2->y + 138, FONT_PRIO_FRONT, 0, moji, 0);
#endif
								// ????????
								itemWndFontNo[2] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd2->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd2->pYobi)->my, DISP_PRIO_IME3, CG_ITEM_WND_GOLD_DROP_BTN_UP + itemWndBtnFlag[2], 2);
								// ????????
								itemWndFontNo[3] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd2->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd2->pYobi)->my, DISP_PRIO_IME3, CG_ITEM_WND_GOLD_INC_BTN_UP + itemWndBtnFlag[3], 2);
								// ?????????
								itemWndFontNo[4] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd2->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd2->pYobi)->my, DISP_PRIO_IME3, CG_ITEM_WND_GOLD_DEC_BTN_UP + itemWndBtnFlag[4], 2);
							}
						}
					}
				}
			}
			break;

		case 1:	// ?????

			// ?????????
			if (pActMenuWnd2 == NULL){
				pActMenuWnd2 = MakeWindowDisp(364, 4, 272, 280, 0, -1);
				// ????
				for (i = 0; i < MENU_ITEM_0; i++) itemWndFontNo[i] = -2;
				for (i = 0; i < MENU_ITEM_0; i++) itemWndBtnFlag[i] = 0;
				jujutuNo = -1;	// ???????

			}
			else{
				// ??????????????
				if (pActMenuWnd2->hp > 0){
					x = pActMenuWnd2->x + 74;
					y = pActMenuWnd2->y + 208;
					// ?????????
#ifdef _READ16BITBMP
					if (g_bUseAlpha) StockDispBuffer(((WINDOW_DISP *)pActMenuWnd2->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd2->pYobi)->my + 7, DISP_PRIO_MENU, CG_JUJUTU_WND, 1);
					else
#endif
						StockDispBuffer(((WINDOW_DISP *)pActMenuWnd2->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd2->pYobi)->my + 10, DISP_PRIO_MENU, CG_JUJUTU_WND, 1);
					// ???
					for (i = 0; i < 5; i++){
						if (HitFontNo == itemWndFontNo[i]){
							// ??????
							char *splitPoint = magic[i].memo;
							sprintf_s(moji, "%2d/%2d", magic[i].mp, pc.mp);
							StockFontBuffer(pActMenuWnd2->x + 74 + 81, pActMenuWnd2->y + 183, FONT_PRIO_FRONT, 0, moji, 0);
							while (1){
								// ?????????
								if (strlen(splitPoint) > 22){
									strncpy_s(moji, splitPoint, 22);
									moji[22] = NULL;	// ??????
									// ??????
									if (GetStrLastByte(moji) == 3){
										moji[21] = NULL;
										splitPoint += 21;
									}
									else{
										moji[22] = NULL;
										splitPoint += 22;
									}
									StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, moji, 0); y += 24;
								}
								else{
									strcpy(moji, splitPoint);
									StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, moji, 0);
									break;
								}
							}
#if 0
							// ??????
							if( magic[ i ].memo[ 0 ] != NULL ){
								strncpy_s(moji, magic[i].memo, 22);
								moji[22] = NULL;
								StockFontBuffer(pActMenuWnd2->x + 74, pActMenuWnd2->y + 204, FONT_PRIO_FRONT, 0, moji, 0);

								if (magic[i].memo[22] != NULL){
									strncpy_s(moji, magic[i].memo + 22, 22);
									moji[22] = NULL;
									StockFontBuffer(pActMenuWnd2->x + 74, pActMenuWnd2->y + 224, FONT_PRIO_FRONT, 0, moji, 0);
								}
							}
#endif
							// ????
							StockDispBuffer(pActMenuWnd2->x + 37, pActMenuWnd2->y + 220, DISP_PRIO_ITEM, pc.item[i].graNo, 0);

							// ????????
							if (mouse.onceState & MOUSE_LEFT_CRICK){
								// ????????
								if (magic[i].field == MAGIC_FIELD_BATTLE || magic[i].mp > pc.mp){
									// ???
									play_se(220, 320, 240);
								}
								else{ // ????
									// ???????
									switch (magic[i].target){

									case MAGIC_TARGET_MYSELF:	// ??

										// ????????????
										if (eventWarpSendFlag == FALSE){
											// ??
											if (bNewServer)
												lssproto_MU_send(sockfd, nowGx, nowGy, i, 0);
											else
												old_lssproto_MU_send(sockfd, nowGx, nowGy, i, 0);
											play_se(100, 320, 240);
										}
										break;

									case MAGIC_TARGET_OTHER:	// ????)
										jujutuNo = i;
										// ?????
										play_se(217, 320, 240);
										//itemNo = -1;
										break;
#if 0
									case MAGIC_TARGET_ALLMYSIDE:	// 

										// ??
										if( bNewServer)
											lssproto_MU_send( sockfd, nowGx, nowGy, i, 0 );
										else
											old_lssproto_MU_send( sockfd, nowGx, nowGy, i, 0 );
										// ?
										play_se( 100, 320, 240 );
										break;

									case MAGIC_TARGET_ALLOTHERSIDE:	// ?

										// ??
										if( bNewServer)
											lssproto_MU_send( sockfd, nowGx, nowGy, i, 0 );
										else
											old_lssproto_MU_send( sockfd, nowGx, nowGy, i, 0 );
										// ?
										play_se( 100, 320, 240 );
										break;

									case MAGIC_TARGET_ALL:	// ?

										// ??
										if (bNewServer)
											lssproto_MU_send(sockfd, nowGx, nowGy, i, 0);
										else
											old_lssproto_MU_send(sockfd, nowGx, nowGy, i, 0);
										// ?
										play_se(100, 320, 240);
										break;
#endif									
									}
								}
							}
						}
					}
					// ????????
					if (mouse.onceState & MOUSE_LEFT_CRICK){
						// ???
						if (HitDispNo == itemWndFontNo[12]){
							// ??????
							DeathAction(pActMenuWnd2);
#ifdef _MONEYINPUT //Syu ADD 手动输入金钱量
							GetKeyInputFocus(&MyChatBuffer);
							Moneyflag = false;
#endif
							pActMenuWnd2 = NULL;
							itemWndNo = 0;
							// ????????
							play_se(203, 320, 240);
							// ???????
							jujutuNo = -1;

							// ??????????????????
							if (MenuToggleFlag & JOY_CTRL_S){

								MenuToggleFlag ^= JOY_CTRL_S;	// CTRL + S ??
								// ??????????
								DeathMenuAction();
							}
						}
						// ?????
						if (jujutuNo != -1 && MenuToggleFlag & JOY_CTRL_S && statusWndNo == 1){
							// ????
							for (i = 0; i < 11; i++){
								// ?
								if (HitFontNo == statusWndFontNo[i] && eventWarpSendFlag == FALSE){
									// ??
									if (bNewServer)
										lssproto_MU_send(sockfd, nowGx, nowGy, jujutuNo, i);
									else
										old_lssproto_MU_send(sockfd, nowGx, nowGy, jujutuNo, i);
									// ?
									play_se(100, 320, 240);
									// ????
									if (magic[jujutuNo].mp > pc.mp - magic[jujutuNo].mp) jujutuNo = -1;;	// ????

								}
							}
						}

					}
					// ????????
					if (pActMenuWnd2 != NULL){
						int flag = FALSE;
						int col;
						// ?
						x = pActMenuWnd2->x + 30;
						y = pActMenuWnd2->y + 35;

						// 
						for (i = 0; i < 5; i++){
							if (magic[i].useFlag == TRUE){
								col = FONT_PAL_WHITE;	// ??????
								if (jujutuNo == i) col = FONT_PAL_AQUA;	// ??
								//cary 2001.12.3
								if (pc.familySprite == 0){
									if ((magic[i].mp * 80 / 100) > pc.mp) col = FONT_PAL_RED;	// ????
								}
								else{
									if (magic[i].mp > pc.mp) col = FONT_PAL_RED;	// ????
								}
								if (magic[i].field == MAGIC_FIELD_BATTLE) col = FONT_PAL_GRAY;	// ?????
								CenteringStr(magic[i].name, moji, MAGIC_NAME_LEN);
								itemWndFontNo[i] = StockFontBuffer(x, y, FONT_PRIO_FRONT, col, moji, 2); y += 28;
								flag = TRUE;
							}
						}
						if (flag == FALSE) StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "没有咒术", 0);
						// ?????????
						itemWndFontNo[12] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd2->pYobi)->mx, pActMenuWnd2->y + 262 + 10, DISP_PRIO_IME3, CG_RETURN_BTN, 2);
					}
				}
			}
			break;

		case 2:	// ??????????

			// ?????????
			if (pActMenuWnd2 == NULL){
				pActMenuWnd2 = MakeWindowDisp(368, 4, 192, 304, 0, -1);
				// ????
				for (i = 0; i < MENU_ITEM_0; i++) itemWndFontNo[i] = -2;
				for (i = 0; i < MENU_ITEM_0; i++) itemWndBtnFlag[i] = 0;

			}
			else{
				// ??????????????
				if (pActMenuWnd2->hp > 0){
					// ?????????
					StockDispBuffer(((WINDOW_DISP *)pActMenuWnd2->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd2->pYobi)->my, DISP_PRIO_MENU, CG_ITEM_WND_SELECT_WND, 1);
					// ????????
					if (mouse.onceState & MOUSE_LEFT_CRICK){
						// ????
						for (i = 0; i < 11; i++){
							if (HitFontNo == itemWndFontNo[i] && eventWarpSendFlag == FALSE){
								// ??
								if (jujutuNo != -1){
									// ??
									if (bNewServer)
										lssproto_MU_send(sockfd, nowGx, nowGy, jujutuNo, i);
									else
										old_lssproto_MU_send(sockfd, nowGx, nowGy, jujutuNo, i);
									// ?
									play_se(100, 320, 240);
									// ?????????
									if (magic[jujutuNo].mp > pc.mp - magic[jujutuNo].mp){
										// ??????
										DeathAction(pActMenuWnd2);
#ifdef _MONEYINPUT //Syu ADD 手动输入金钱量
										GetKeyInputFocus(&MyChatBuffer);
										Moneyflag = false;
#endif
										pActMenuWnd2 = NULL;
										itemWndNo = 1;
										// ????????
										play_se(203, 320, 240);
									}
								}
								else{	// ??????
									//??????
									if (bNewServer)
										lssproto_ID_send(sockfd, nowGx, nowGy, itemNo, i);
									else
										old_lssproto_ID_send(sockfd, nowGx, nowGy, itemNo, i);
									// ??????
									play_se(212, 320, 240);
									// ??????
									DeathAction(pActMenuWnd2);
#ifdef _MONEYINPUT //Syu ADD 手动输入金钱量
									GetKeyInputFocus(&MyChatBuffer);
									Moneyflag = false;
#endif
									pActMenuWnd2 = NULL;
									itemWndNo = 0;
									// ??????????????????
									if (MenuToggleFlag & JOY_CTRL_S){

										MenuToggleFlag ^= JOY_CTRL_S;	// CTRL + S ??
										// ??????????
										DeathMenuAction();
									}
									// ????????
									play_se(203, 320, 240);
								}
							}
						}
						// ???
						if (HitDispNo == itemWndFontNo[11]){
							// ??????
							DeathAction(pActMenuWnd2);
#ifdef _MONEYINPUT //Syu ADD 手动输入金钱量
							GetKeyInputFocus(&MyChatBuffer);
							Moneyflag = false;
#endif
							pActMenuWnd2 = NULL;
							// ??
							if (jujutuNo != -1) itemWndNo = 1;
							else{
								// ????????????
								itemWndNo = 0;
								// ??????????????????
								if (MenuToggleFlag & JOY_CTRL_S){

									MenuToggleFlag ^= JOY_CTRL_S;	// CTRL + S ??
									// ??????????
									DeathMenuAction();
								}
							}
							// ????????
							play_se(203, 320, 240);
						}
					}
					// ????????
					if (pActMenuWnd2 != NULL){
						// ?
						x = pActMenuWnd2->x + 25;
						y = pActMenuWnd2->y + 30;

						// ?????????
						// ?
						sprintf_s(moji, "%-16s", pc.name);
						itemWndFontNo[0] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, moji, 2); y += 23;
						// ???
						for (i = 0; i < 5; i++){
							// ???????????
							if (pet[i].useFlag == TRUE){
								// ??????
								if (pet[i].freeName[0] != NULL)
									sprintf_s(moji, "%-16s", pet[i].freeName);
								else
									sprintf_s(moji, "%-16s", pet[i].name);
								itemWndFontNo[i + 1] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, moji, 2); y += 23;
							}
						}
						y = 183;
						// ??
						for (i = 0; i < 5; i++){
							// ????????????????
							if (pc.id != party[i].id && party[i].useFlag == TRUE){
								sprintf_s(moji, "%-16s", party[i].name);
								itemWndFontNo[i + 6] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, moji, 2); y += 23;
							}
						}
						// ?????????
						itemWndFontNo[11] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd2->pYobi)->mx, pActMenuWnd2->y + 287, DISP_PRIO_IME3, CG_RETURN_BTN, 2);
					}
				}
			}
			break;
		}
	}

	//? ?????????? *******************************************************/

	if (MenuToggleFlag & JOY_CTRL_M){
#ifdef _NEWPANEL //Syu ADD 7.0 新人物状态介面
		SkillWndflag = false;
#endif
		// ????????
		switch (mapWndNo){

		case 0:	// ????????

			// ?????????
			if (pActMenuWnd2 == NULL){
				pActMenuWnd2 = MakeWindowDisp(388, 4, 248, 240, 0, -1);
				for (i = 0; i < MENU_MAP_0; i++) mapWndFontNo[i] = -2;

			}
			else{
				// ??????????????
				if (pActMenuWnd2->hp > 0){
					// ????????
					StockDispBuffer(((WINDOW_DISP *)pActMenuWnd2->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd2->pYobi)->my, DISP_PRIO_MENU, CG_MAP_WND, 0);
					// ????????
					if (mouse.onceState & MOUSE_LEFT_CRICK){
						// ???
						if (HitDispNo == mapWndFontNo[0]){
							// ??????
							DeathAction(pActMenuWnd2);
#ifdef _MONEYINPUT //Syu ADD 手动输入金钱量
							GetKeyInputFocus(&MyChatBuffer);
							Moneyflag = false;
#endif
							pActMenuWnd2 = NULL;
							// ???????
							MenuToggleFlag ^= JOY_CTRL_M;
							// ????????
							play_se(203, 320, 240);
						}
					}

					// ????????
					if (pActMenuWnd2 != NULL){
						// ?
						x = pActMenuWnd2->x + 61;
						y = pActMenuWnd2->y + 186;

						// ????
						StockFontBuffer(pActMenuWnd2->x + 22, pActMenuWnd2->y + 31, FONT_PRIO_FRONT, 0, nowFloorName, 0);

						// ?????????
						sprintf_s(moji, "东 %3d", nowGx);
						StockFontBuffer(x, y, FONT_PRIO_FRONT, FONT_PAL_YELLOW, moji, 0);
						sprintf_s(moji, "南 %3d", nowGy);
						StockFontBuffer(x + 73, y, FONT_PRIO_FRONT, FONT_PAL_YELLOW, moji, 0); y += 36;
						// ?????
						mapWndFontNo[0] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd2->pYobi)->mx, y, DISP_PRIO_IME3, CG_CLOSE_BTN, 2);
					}
				}
			}
			break;
		}
	}

	//? ?????????? *******************************************************/

	if (MenuToggleFlag & JOY_CTRL_E){
		// ????????
		switch (mailWndNo){

		case MAIL_WND_VIEW:	// ???????

			// ?????????
			if (pActMenuWnd == NULL){
				// ?????????
				pActMenuWnd = MakeWindowDisp(4, 4, 272, 440, 0, -1);
				// ??????
				for (i = 0; i < MENU_MAIL_0; i++) mailWndFontNo[i] = -2;
				// ????????????????????????
				for (i = 0; i < MAX_ADR_BOOK; i++){
					int j;
					// ????????
					for (j = 0; j < MAIL_MAX_HISTORY; j++){
						// ????
						if (MailHistory[i].noReadFlag[j] >= TRUE){
							// ????????
							mailViewWndPageNo = i / MAX_ADR_BOOK_COUNT;
							i = MAX_ADR_BOOK;
							break;
						}
					}
				}
			}
			else{
				// ??????????????
				if (pActMenuWnd->hp > 0){
					static int nowDelNo;
					// ???????
					StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my, DISP_PRIO_MENU, CG_MAIL_WND, 1);

					// ????????
					if (mouse.onceState & MOUSE_LEFT_CRICK && pActYesNoWnd == NULL){

						int nowNo = mailViewWndPageNo * MAX_ADR_BOOK_COUNT;
						// ??????
						for (i = 0; i < 12; i += 3, nowNo++){
							// ????????????????
#ifdef _EXTEND_AB
							if (addressBook[nowNo].useFlag == TRUE
								|| addressBook[nowNo].useFlag == 2
								|| addressBook[nowNo].useFlag == 3){
#else
							if (addressBook[nowNo].useFlag == TRUE){
#endif	
								if (HitFontNo == mailWndFontNo[i]){
									mailHistoryWndSelectNo = nowNo;
									// ??????
									DeathAction(pActMenuWnd);
									pActMenuWnd = NULL;
									// ?????????
									DeathLetterAction();
									// ????????
									mailWndNo = MAIL_WND_HISTORY;
									// ????????
									play_se(202, 320, 240);
								}

								// ?????
								if (HitDispNo == mailWndFontNo[i + 1]){
									// ?????????
									if (pActYesNoWnd == NULL){
										// ????????
										play_se(202, 320, 240);
										// ??????????
										pActYesNoWnd = MakeWindowDisp(pActMenuWnd->x + 42, i * 32 + 54, 176, 56, 0, 4);

										// ????????
										nowDelNo = nowNo;
									}
								}
								// ??????
								if (HitDispNo == mailWndFontNo[i + 2]){
									// ??????
									DeathAction(pActMenuWnd);
									pActMenuWnd = NULL;
									// ?????????
									DeathLetterAction();
									// ????????
									InitMailSendFlag();
									// ??????????
									mailWndSendFlag[nowNo] = TRUE;
									// ????????????
									mailWndNoBak = mailWndNo;
									// ???
									if (ProcNo == PROC_BATTLE){
										// ??????????
										mailWndNo = MAIL_WND_SEND;
									}
									else{
										// ?????????????
										if (pc.mailPetNo != -1){
											// ??????
											mailWndNo = MAIL_WND_SELECT;
										}
										else{
											// ??????????
											mailWndNo = MAIL_WND_SEND;
										}
									}
									// ????????
									play_se(202, 320, 240);
								}
							}
						}
						// ???
						if (HitDispNo == mailWndFontNo[19]){
							// ??????
							DeathAction(pActMenuWnd);
							pActMenuWnd = NULL;
							// ?????????
							DeathLetterAction();
							MenuToggleFlag ^= JOY_CTRL_E;
							// ????????
							play_se(203, 320, 240);
						}
					}

					// ???
					if ((HitDispNo == mailWndFontNo[26] || joy_con[0] & JOY_A) && pActYesNoWnd == NULL){
						// ????????
						if (mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[0] & JOY_A){
							mailViewWndPageNo--; // ?????
							if (mailViewWndPageNo < 0) mailViewWndPageNo = MAX_ADR_BOOK_PAGE - 1;
							// ?????
							play_se(217, 320, 240);
							mailWndBtnFlag[26] = TRUE;
							// ?????????
							DeathLetterAction();
						}
						// ?????????
						if ((mouse.state & MOUSE_LEFT_CRICK || joy_con[0] & JOY_A) && mailWndBtnFlag[26] == TRUE){
							// ????
							mailWndBtnFlag[26] = TRUE;
						}
						else{
							// ???
							mailWndBtnFlag[26] = FALSE;
						}
					}
					else{
						// ???
						mailWndBtnFlag[26] = FALSE;
					}
					// ???
					if ((HitDispNo == mailWndFontNo[27] || joy_con[0] & JOY_B) && pActYesNoWnd == NULL){
						// ????????
						if (mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[0] & JOY_B){
							mailViewWndPageNo++; // ??????
							if (mailViewWndPageNo >= MAX_ADR_BOOK_PAGE) mailViewWndPageNo = 0;
							// ?????
							play_se(217, 320, 240);
							mailWndBtnFlag[27] = TRUE;
							// ?????????
							DeathLetterAction();
						}
						// ?????????
						if ((mouse.state & MOUSE_LEFT_CRICK || joy_con[0] & JOY_B) && mailWndBtnFlag[27] == TRUE){
							// ????
							mailWndBtnFlag[27] = TRUE;
						}
						else{
							// ???
							mailWndBtnFlag[27] = FALSE;
						}
					}
					else{
						// ???
						mailWndBtnFlag[27] = FALSE;
					}

					// ????????
					if (pActMenuWnd != NULL){
						int nowNo = mailViewWndPageNo * MAX_ADR_BOOK_COUNT;
						// ?
						x = pActMenuWnd->x;
						y = pActMenuWnd->y;

						// ?????????????????????
						int graNo[7] = { CG_MAIL_WND_OFF_LINE_BTN,
							CG_MAIL_WND_ON_LINE_SUN_BTN,
							CG_MAIL_WND_ON_LINE_MOON_BTN,
							CG_MAIL_WND_ON_LINE_MERCURY_BTN,
							CG_MAIL_WND_ON_LINE_VINUS_BTN,
							CG_MAIL_WND_ON_LINE_EARTH_BTN,
							CG_MAIL_WND_ON_LINE_MARS_BTN
						};

						for (i = 0; i < 12; i += 3, nowNo++){
							// ????
#ifdef _EXTEND_AB
							if (addressBook[nowNo].useFlag == TRUE
								|| addressBook[nowNo].useFlag == 2
								|| addressBook[nowNo].useFlag == 3){
#else
							if (addressBook[nowNo].useFlag == TRUE){
#endif
								int j = 0;
								// ????????
								for (j = 0; j < MAIL_MAX_HISTORY; j++){
									// ??????????
									if (pActLetter[i / 3] == NULL){
										if (MailHistory[nowNo].noReadFlag[j] >= TRUE){
											// ???????????
											pActLetter[i / 3] = MakeAnimDisp(x - 20, y + 38, SPR_mail, ANIM_DISP_LETTER_MOVE);
										}
									}
								}
								// ?
#ifdef _EXTEND_AB
								if (addressBook[nowNo].useFlag == 2)
									StockFontBuffer(x + 15, y + 85, FONT_PRIO_FRONT, 5, "精灵使者", 0);
								if (addressBook[nowNo].useFlag == 3)
									StockFontBuffer(x + 15, y + 85, FONT_PRIO_FRONT, 5, "精灵勇者", 0);
#endif
								StockDispBuffer(x + 44, y + 68, DISP_PRIO_IME3, addressBook[nowNo].graNo, 0);
								CenteringStr(addressBook[nowNo].name, moji, CHAR_NAME_LEN);
								mailWndFontNo[i] = StockFontBuffer(x + 80, y + 30, FONT_PRIO_FRONT, 0, moji, 2);
								sprintf_s(moji, "%3d", addressBook[nowNo].level);
								StockFontBuffer(x + 104, y + 60, FONT_PRIO_FRONT, 0, moji, 0);
#ifdef _MAILSHOWPLANET				// (可开放) Syu ADD 显示名片星球
								sprintf_s(moji, "%8s", addressBook[nowNo].planetname);
								StockFontBuffer(x + 198, y + 30, FONT_PRIO_FRONT, 5, moji, 0);
#endif
								//
#ifdef _TRANS_6	
								if (addressBook[nowNo].transmigration < 0 || addressBook[nowNo].transmigration >= 7) addressBook[nowNo].transmigration = 0;
#else								
								if (addressBook[nowNo].transmigration < 0 || addressBook[nowNo].transmigration >= 6) addressBook[nowNo].transmigration = 0;
#endif
								StockFontBuffer(x + 152, y + 59, FONT_PRIO_FRONT, FONT_PAL_AQUA, TransmigrationStr[addressBook[nowNo].transmigration], 0);
								sprintf_s(moji, "%8d", addressBook[nowNo].dp);
								StockFontBuffer(x + 103, y + 88, FONT_PRIO_FRONT, 0, moji, 0);
								StockDispBuffer(x + 225, y + 60, DISP_PRIO_IME3, graNo[(addressBook[nowNo].onlineFlag == 0) ? 0 : 1], 0);
								mailWndFontNo[i + 2] = StockDispBuffer(x + 225, y + 80, DISP_PRIO_IME3, CG_MAIL_WND_MAIL_BTN, 2);
								mailWndFontNo[i + 1] = StockDispBuffer(x + 225, y + 100, DISP_PRIO_IME3, CG_MAIL_WND_DELETE_BTN, 2);

							}
#ifdef _EXTEND_AB
							else{
								if (nowNo == MAX_ADR_BOOK - 1)
								{
									StockFontBuffer(x + 100, y + 30, FONT_PRIO_FRONT, FONT_PAL_RED, "精灵召唤用", 0);
								}
							}
#endif
							y += 96;
						}
						// ???
						mailWndFontNo[26] = StockDispBuffer(pActMenuWnd->x + 25 + 28 - 8, pActMenuWnd->y + 421, DISP_PRIO_IME3, CG_PREV_BTN + mailWndBtnFlag[26], 2);
						// ????
						sprintf_s(moji, "%2d", mailViewWndPageNo + 1);
						StockFontBuffer(pActMenuWnd->x + 46 + 28 - 8, pActMenuWnd->y + 413, FONT_PRIO_FRONT, 0, moji, 0);
						// ????
						mailWndFontNo[27] = StockDispBuffer(pActMenuWnd->x + 75 + 28, pActMenuWnd->y + 421, DISP_PRIO_IME3, CG_NEXT_BTN + mailWndBtnFlag[27], 2);
						mailWndFontNo[19] = StockDispBuffer(pActMenuWnd->x + 220 - 28, pActMenuWnd->y + 421, DISP_PRIO_IME3, CG_CLOSE_BTN, 2);
					}

					// ?????????
					if (pActYesNoWnd != NULL){
						// ????
						if (((WINDOW_DISP *)pActYesNoWnd->pYobi)->yesNoResult == TRUE){
							// ??????
							if (bNewServer)
								lssproto_DAB_send(sockfd, nowDelNo);
							else
								old_lssproto_DAB_send(sockfd, nowDelNo);
							play_se(217, 320, 240);
							// ???????
							DeathAction(pActYesNoWnd);
							pActYesNoWnd = NULL;
							// ????????
							play_se(203, 320, 240);
						}
						else
							// ?????
						if (((WINDOW_DISP *)pActYesNoWnd->pYobi)->yesNoResult == FALSE){
							// ???????
							DeathAction(pActYesNoWnd);
							pActYesNoWnd = NULL;
							// ????????
							play_se(203, 320, 240);
						}
					}
				}

			}
			break;

		case MAIL_WND_SELECT:	// ?????
			// ?????????
			if (pActMenuWnd == NULL){
				// ?????????
				pActMenuWnd = MakeWindowDisp(40, 40, 3, 3, NULL, 0);
				// ??????
				for (i = 0; i < MENU_MAIL_0; i++) mailWndFontNo[i] = -2;
				// ??????????
				for (i = 0; i < 5; i++){
					// ????????????
					if (pet[i].useFlag == FALSE){
						// ?????
						if (i == pc.mailPetNo){
							pc.mailPetNo = -1;
							// ????????
							saveUserSetting();
						}
					}
				}
			}
			else{
				// ??????????????
				if (pActMenuWnd->hp > 0){
					// ????????
					if (mouse.onceState & MOUSE_LEFT_CRICK){
						// ??????
						if (HitFontNo == mailWndFontNo[0]){
							// ??????
							DeathAction(pActMenuWnd);
							pActMenuWnd = NULL;
							// ???????????
							mailWndNo = MAIL_WND_SEND;
							// ????????
							play_se(202, 320, 240);
						}
						// ????????
						if (HitFontNo == mailWndFontNo[1]){
#ifdef _TAIKEN			
							// ???
							play_se(220, 320, 240);
							// ???????
							sprintf_s(moji, "体验版不能选择！");
							// ??????????????????
							StockChatBufferLine( moji, FONT_PAL_WHITE );
#else
#ifdef _STREET_VENDOR
							if (pc.iOnStreetVendor == 1){
								play_se(220, 320, 240);
								StockChatBufferLine("摆摊中不得使用宠物邮件", FONT_PAL_RED);
							}
							else
#endif
#ifdef _THEATER
							if (pc.iTheaterMode & 0x00000002)
								StockChatBufferLine("请专心表演", FONT_PAL_RED);
							else if (pc.iTheaterMode & 0x00000001)
								StockChatBufferLine("请专心看表演", FONT_PAL_RED);
							else
#endif
								// ???????????
							if (pc.mailPetNo != -1){
								// ??????
								DeathAction(pActMenuWnd);
								pActMenuWnd = NULL;
								// ?????????????
								mailWndNo = MAIL_WND_PET_SEND;
								// ????????
								play_se(202, 320, 240);
							}
							else{
								// ???
								play_se(220, 320, 240);
							}
#endif
						}
						// ???
						if (HitDispNo == mailWndFontNo[2]){
							// ??????
							DeathAction(pActMenuWnd);
							pActMenuWnd = NULL;
							// ????????
							mailWndNo = MAIL_WND_VIEW;
							// ????????
							play_se(203, 320, 240);
						}
					}

					// ????????
					if (pActMenuWnd != NULL){
						int color = FONT_PAL_WHITE;

						// ???????????
						if (pc.mailPetNo == -1)
							color = FONT_PAL_RED;

						// ?
						x = pActMenuWnd->x + 28;
						y = pActMenuWnd->y + 32;

						// ?????????
						mailWndFontNo[0] = StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, "  普 通 邮 件   ", 2);	y += 32;
						mailWndFontNo[1] = StockFontBuffer(x, y, FONT_PRIO_FRONT, color, "  宠 物 邮 件   ", 2);	y += 48;
						mailWndFontNo[2] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, y, DISP_PRIO_IME3, CG_RETURN_BTN, 2);
					}
				}
			}
			break;

		case MAIL_WND_SEND:	// ??????????

			// ?????????
			if (pActMenuWnd == NULL){
				pActMenuWnd = MakeWindowDisp(4, 4, 272, 304, 0, -1);
				// ??????
				for (i = 0; i < MENU_MAIL_0; i++) mailWndFontNo[i] = -2;
				mailPetSendWndPageNo = mailViewWndPageNo;		// ??????????

				MailStr.len = 140;
				// ????
				MailStr.lineLen = 28;
				// ?????
				MailStr.lineDist = 20;
				// ???
				MailStr.color = 0;
				// ????
				MailStr.x = pActMenuWnd->x + 12;
				MailStr.y = pActMenuWnd->y + 280 - 136;
				// ??????
				MailStr.fontPrio = FONT_PRIO_FRONT;

			}
			else{
				// ??????????????
				if (pActMenuWnd->hp > 0){
					// ??????
					StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my, DISP_PRIO_MENU, CG_MAIL_WND_SEND_WND, 1);
					// ????????
					if (mouse.onceState & MOUSE_LEFT_CRICK){
						int nowNo = mailPetSendWndPageNo * MAX_ADR_BOOK_COUNT;
						// ??????
						for (i = 0; i < MAX_ADR_BOOK_COUNT; i++){
							// ?????
							if (HitFontNo == mailWndFontNo[i]){
								// ??????
#ifdef _EXTEND_AB
								if (addressBook[nowNo + i].useFlag == TRUE
									|| addressBook[nowNo + i].useFlag == 2
									|| addressBook[nowNo + i].useFlag == 3){
#else								??????????
								if (addressBook[nowNo + i].useFlag == TRUE){
#endif
									// ?????????
									if (mailWndSendFlag[nowNo + i] == FALSE) mailWndSendFlag[nowNo + i] = TRUE;
									else mailWndSendFlag[nowNo + i] = FALSE;
									// ?????
									play_se(217, 320, 240);
								}
							}

						}
						// ???
						if (HitDispNo == mailWndFontNo[19]){
							// ??????
							DeathAction(pActMenuWnd);
							pActMenuWnd = NULL;
							// ?????????
							mailWndNo = mailWndNoBak;
							// ????????
							play_se(203, 320, 240);
							// ????????
							GetKeyInputFocus(&MyChatBuffer);
						}
					}
					// ????
					if (HitDispNo == mailWndFontNo[18]){
						// ??????????????????0.5???
						if (mouse.onceState & MOUSE_LEFT_CRICK && ItemUseTime < TimeGetTime() - 5000){
							int flag = FALSE;
							// ?????
							ItemUseTime = TimeGetTime();
							// ??????????
							for (i = 0; i < MAX_ADR_BOOK; i++){
								if (mailWndSendFlag[i] == TRUE) flag = TRUE;
							}
							// ??????
							if (MailStr.buffer[0] != NULL && flag == TRUE){
								// ?????
								for (i = 0; i < MAX_ADR_BOOK; i++){
									// ????????
									if (mailWndSendFlag[i] == TRUE){
										char moji2[256];
										// ????????
										strcpy(moji2, MailStr.buffer);
										// ???????
										makeEscapeString(moji2, moji, sizeof(moji));
										// ????
										if (bNewServer)
											lssproto_MSG_send(sockfd, i, moji, FONT_PAL_WHITE);
										else
											old_lssproto_MSG_send(sockfd, i, moji, FONT_PAL_WHITE);

									}
								}
								play_se(101, 320, 240);
								mailWndBtnFlag[18] = TRUE;
							}
							else{
								// ???
								play_se(220, 320, 240);
							}
						}
						// ?????????
						if (mouse.state & MOUSE_LEFT_CRICK && mailWndBtnFlag[18] == TRUE){
							// ????
							mailWndBtnFlag[18] = TRUE;
						}
						else{
							// ???
							mailWndBtnFlag[18] = FALSE;
						}
					}
					else{
						// ???
						mailWndBtnFlag[18] = FALSE;
					}

					// ???
					if (HitDispNo == mailWndFontNo[16] || joy_con[0] & JOY_A){
						// ????????
						if (mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[0] & JOY_A){
							mailPetSendWndPageNo--; // ?????
							if (mailPetSendWndPageNo < 0) mailPetSendWndPageNo = MAX_ADR_BOOK_PAGE - 1;
							// ?????
							play_se(217, 320, 240);
							mailWndBtnFlag[16] = TRUE;
						}
						// ?????????
						if ((mouse.state & MOUSE_LEFT_CRICK || joy_con[0] & JOY_A) && mailWndBtnFlag[16] == TRUE){
							// ????
							mailWndBtnFlag[16] = TRUE;
						}
						else{
							// ???
							mailWndBtnFlag[16] = FALSE;
						}
					}
					else{
						// ???
						mailWndBtnFlag[16] = FALSE;
					}
					// ???
					if (HitDispNo == mailWndFontNo[17] || joy_con[0] & JOY_B){
						// ????????
						if (mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[0] & JOY_B){
							mailPetSendWndPageNo++; // ??????
							if (mailPetSendWndPageNo >= MAX_ADR_BOOK_PAGE) mailPetSendWndPageNo = 0;
							// ?????
							play_se(217, 320, 240);
							mailWndBtnFlag[17] = TRUE;
						}
						// ?????????
						if ((mouse.state & MOUSE_LEFT_CRICK || joy_con[0] & JOY_B) && mailWndBtnFlag[17] == TRUE){
							// ????
							mailWndBtnFlag[17] = TRUE;
						}
						else{
							// ???
							mailWndBtnFlag[17] = FALSE;
						}
					}
					else{
						// ???
						mailWndBtnFlag[17] = FALSE;
					}

					// ??????
					if (HitDispNo == mailWndFontNo[14]){
						// ????????
						if (mouse.onceState & MOUSE_LEFT_CRICK){
							// ??? ??
							MailStr.buffer[0] = NULL;
							MailStr.cnt = 0;
							MailStr.cursor = 0;
							// ?????
							play_se(217, 320, 240);
							mailWndBtnFlag[14] = TRUE;
						}
						// ?????????
						if (mouse.state & MOUSE_LEFT_CRICK && mailWndBtnFlag[14] == TRUE){
							// ????
							mailWndBtnFlag[14] = TRUE;
						}
						else{
							// ???
							mailWndBtnFlag[14] = FALSE;
						}
					}
					else{
						// ???
						mailWndBtnFlag[14] = FALSE;
					}
					// ??????
					if (HitDispNo == mailWndFontNo[15]){
						// ????????
						if (mouse.onceState & MOUSE_LEFT_CRICK){
							// ????????
							InitMailSendFlag();
							// ?????
							play_se(217, 320, 240);
							mailWndBtnFlag[15] = TRUE;
						}
						// ?????????
						if (mouse.state & MOUSE_LEFT_CRICK && mailWndBtnFlag[15] == TRUE){
							// ????
							mailWndBtnFlag[15] = TRUE;
						}
						else{
							// ???
							mailWndBtnFlag[15] = FALSE;
						}
					}
					else{
						// ???
						mailWndBtnFlag[15] = FALSE;
					}

					// ????????
					if (pActMenuWnd != NULL){
						int nowNo = mailPetSendWndPageNo * MAX_ADR_BOOK_COUNT;
						int color;

						// ?
						x = pActMenuWnd->x;
						y = pActMenuWnd->y;
						// ??????
						GetKeyInputFocus(&MailStr);
						// ???
						StockFontBuffer2(&MailStr);

						for (i = 0; i < MAX_ADR_BOOK_COUNT; i++){
							// ????????????????
#ifdef _EXTEND_AB
							if (addressBook[nowNo + i].useFlag == TRUE
								|| addressBook[nowNo + i].useFlag == 2
								|| addressBook[nowNo + i].useFlag == 3){
#else
							if (addressBook[nowNo + i].useFlag == TRUE){
#endif
								color = FONT_PAL_GRAY;	// ?????
								// ???????
								if (addressBook[nowNo + i].onlineFlag > 0){
									// ?
									color = FONT_PAL_WHITE;
								}
								// ??????
								if (mailWndSendFlag[nowNo + i] == TRUE){
									// ?
									color = FONT_PAL_AQUA;
								}
								// 
								CenteringStr(addressBook[nowNo + i].name, moji, CHAR_NAME_LEN);
								mailWndFontNo[i] = StockFontBuffer(x + 111, y + 177 - 136, FONT_PRIO_FRONT, color, moji, 2);

							}
							y += 23;
						}
						// ??????
						mailWndFontNo[15] = StockDispBuffer(pActMenuWnd->x + 51, pActMenuWnd->y + 211 - 136, DISP_PRIO_IME3, CG_MAIL_WND_CLEAR_BTN_UP + mailWndBtnFlag[15], 2);
						// ???
						mailWndFontNo[16] = StockDispBuffer(pActMenuWnd->x + 26 - 8 + 4, pActMenuWnd->y + 241 - 132, DISP_PRIO_IME3, CG_PREV_BTN + mailWndBtnFlag[16], 2);
						// ????
						sprintf_s(moji, "%2d", mailPetSendWndPageNo + 1);
						StockFontBuffer(pActMenuWnd->x + 47 - 8 + 4, pActMenuWnd->y + 233 - 132, FONT_PRIO_FRONT, 0, moji, 0);
						// ????
						mailWndFontNo[17] = StockDispBuffer(pActMenuWnd->x + 76 + 4, pActMenuWnd->y + 241 - 132, DISP_PRIO_IME3, CG_NEXT_BTN + mailWndBtnFlag[17], 2);
						// ??????
						mailWndFontNo[14] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx + 60, pActMenuWnd->y + 390 - 136, DISP_PRIO_IME3, CG_MAIL_WND_CLEAR_BTN_UP + mailWndBtnFlag[14], 2);
						// ????
						mailWndFontNo[18] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx - 60, pActMenuWnd->y + 421 - 136, DISP_PRIO_IME3, CG_SEND_BTN + mailWndBtnFlag[18] * 75, 2);
						// ??
						mailWndFontNo[19] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx + 60, pActMenuWnd->y + 421 - 136, DISP_PRIO_IME3, CG_RETURN_BTN, 2);
					}
				}
			}
			break;

		case MAIL_WND_PET_SEND:	// ????????????

			// ?????????
			if (pActMenuWnd == NULL){
				pActMenuWnd = MakeWindowDisp(4, 4, 272, 440, 0, -1);
				// ??????
				for (i = 0; i < MENU_MAIL_0; i++) mailWndFontNo[i] = -2;
				mailPetSendWndPageNo = mailViewWndPageNo;		// ??????????
				MailStr.len = MAIL_STR_LEN;
				// ????
				MailStr.lineLen = 28;
				// ?????
				MailStr.lineDist = 20;
				// ???
				MailStr.color = 0;
				// ????
				MailStr.x = pActMenuWnd->x + 12;
				MailStr.y = pActMenuWnd->y + 280;
				// ??????
				MailStr.fontPrio = FONT_PRIO_FRONT;
			}
			else{
				// ??????????????
				if (pActMenuWnd->hp > 0){
					// ????????????????
					if (pActPet == NULL){
						// ?????????
						pActPet = MakeAnimDisp(pActMenuWnd->x + 190, pActMenuWnd->y + 120, pet[pc.mailPetNo].graNo, ANIM_DISP_PET);
					}
					// ?????????????????????????
					if (pActMailItem == NULL && mailItemNo != -1){
						// ??????????
						pActMailItem = MakeAnimDisp(pActMenuWnd->x + 56, pActMenuWnd->y + 96, pc.item[mailItemNo].graNo, ANIM_DISP_PET_ITEM);
						pActMailItem->atr |= ACT_ATR_INFO;
						pActMailItem->atr |= ACT_ATR_TYPE_ITEM;
						pActMailItem->dispPrio = DISP_PRIO_DRAG;
						// ???????
						strcpy(pActMailItem->name, pc.item[mailItemNo].name);
					}
					// ??????
					StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my, DISP_PRIO_MENU, CG_MAIL_WND_PET_SEND_WND, 1);
					// ????????
					if (mouse.onceState & MOUSE_LEFT_CRICK){
						int nowNo = mailPetSendWndPageNo * MAX_ADR_BOOK_COUNT;
						// ??????
						for (i = 0; i < MAX_ADR_BOOK_COUNT; i++){
							// ??????????????????????
							if (HitFontNo == mailWndFontNo[i]){
								// ????????
#ifdef _EXTEND_AB
								if (addressBook[nowNo + i].useFlag == TRUE
									|| addressBook[nowNo + i].useFlag == 2
									|| addressBook[nowNo + i].useFlag == 3){
#else								????????
								if (addressBook[nowNo + i].useFlag == TRUE){
#endif
									int j, flag = FALSE;
									// ??????????
									for (j = 0; j < MAX_ADR_BOOK; j++){
										if (mailWndSendFlag[j] == TRUE) flag = TRUE;
									}

									// ???????
									if (flag == FALSE){
										mailWndSendFlag[nowNo + i] = TRUE;
										// ?????
										play_se(217, 320, 240);
									}
									else{ // ????????
										// ????????????????
										if (mailWndSendFlag[nowNo + i] == TRUE){
											mailWndSendFlag[nowNo + i] = FALSE;
											// ?????
											play_se(217, 320, 240);
										}
										else{ // ?????????????????
											// ???
											play_se(220, 320, 240);
										}
									}
								}
							}
						}
						// ???
						if (HitDispNo == mailWndFontNo[19]){
							// ??????
							DeathAction(pActMenuWnd);
							pActMenuWnd = NULL;
							// ????
							DeathAction(pActPet);
							pActPet = NULL;
							// ?????
							DeathAction(pActMailItem);
							pActMailItem = NULL;
							// ????????
							mailWndNo = MAIL_WND_VIEW;
							// ????????
							play_se(203, 320, 240);
							// ????????
							GetKeyInputFocus(&MyChatBuffer);
						}
						// ???????
						if (HitDispNo == mailWndFontNo[15]){
							// ??????
							DeathAction(pActMenuWnd);
							pActMenuWnd = NULL;
							// ????
							DeathAction(pActPet);
							pActPet = NULL;
							// ?????
							DeathAction(pActMailItem);
							pActMailItem = NULL;
							// ?????????
							mailWndNo = MAIL_WND_ITEM;
							// ????????
							play_se(202, 320, 240);
							// ????????
							GetKeyInputFocus(&MyChatBuffer);
						}
					}
					// ????
					if (HitDispNo == mailWndFontNo[18]){

						// ????????
						if (mouse.onceState & MOUSE_LEFT_CRICK){
							int flag = FALSE;
							// ??????????
							for (i = 0; i < MAX_ADR_BOOK; i++){
								if (mailWndSendFlag[i] == TRUE) flag = TRUE;
							}
							// ??????
							if (MailStr.buffer[0] != NULL && flag == TRUE
								&& pActPet != NULL
								&& pActPet->actNo == ANIM_DISP_PET
								&& pc.mailPetNo != ItemMixPetNo){
								for (i = 0; i < MAX_ADR_BOOK; i++){
									// ????????
									if (mailWndSendFlag[i] == TRUE){
										// Terry add fix can send mail to offline character 2004/2/5
										if (addressBook[i].onlineFlag == 0) StockChatBufferLine("该玩家不在线上！", FONT_PAL_RED);
										else
											// end
										{
											char moji2[256];
											// ????????
											strcpy(moji2, MailStr.buffer);
											// ???????
											makeEscapeString(moji2, moji, sizeof(moji));
											// ???????
											if (bNewServer)
												lssproto_PMSG_send(sockfd, i, pc.mailPetNo, mailItemNo, moji2, FONT_PAL_WHITE);
											else
												old_lssproto_PMSG_send(sockfd, i, pc.mailPetNo, mailItemNo, moji2, FONT_PAL_WHITE);
											pc.mailPetNo = -1;
											// ??????????
											ItemBuffer[mailItemNo].mixFlag = 0;
											// ????????
											saveUserSetting();
											// ????????????????
											if (pActPet != NULL){
												// ??????
												pActPet->actNo = 1;
											}
										}
									}
								}
								play_se(101, 320, 240);
								mailWndBtnFlag[18] = TRUE;
							}
							else{
								// ???
								play_se(220, 320, 240);
							}
						}
						// ?????????
						if (mouse.state & MOUSE_LEFT_CRICK && mailWndBtnFlag[18] == TRUE){
							// ????
							mailWndBtnFlag[18] = TRUE;
						}
						else{
							// ???
							mailWndBtnFlag[18] = FALSE;
						}
					}
					else{
						// ???
						mailWndBtnFlag[18] = FALSE;
					}

					// ???
					if (HitDispNo == mailWndFontNo[16] || joy_con[0] & JOY_A){
						// ????????
						if (mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[0] & JOY_A){
							mailPetSendWndPageNo--; // ?????
							if (mailPetSendWndPageNo < 0) mailPetSendWndPageNo = MAX_ADR_BOOK_PAGE - 1;
							// ?????
							play_se(217, 320, 240);
							mailWndBtnFlag[16] = TRUE;
						}
						// ?????????
						if ((mouse.state & MOUSE_LEFT_CRICK || joy_con[0] & JOY_A) && mailWndBtnFlag[16] == TRUE){
							// ????
							mailWndBtnFlag[16] = TRUE;
						}
						else{
							// ???
							mailWndBtnFlag[16] = FALSE;
						}
					}
					else{
						// ???
						mailWndBtnFlag[16] = FALSE;
					}
					// ???
					if (HitDispNo == mailWndFontNo[17] || joy_con[0] & JOY_B){
						// ????????
						if (mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[0] & JOY_B){
							mailPetSendWndPageNo++; // ??????
							if (mailPetSendWndPageNo >= MAX_ADR_BOOK_PAGE) mailPetSendWndPageNo = 0;
							// ?????
							play_se(217, 320, 240);
							mailWndBtnFlag[17] = TRUE;
						}
						// ?????????
						if ((mouse.state & MOUSE_LEFT_CRICK || joy_con[0] & JOY_B) && mailWndBtnFlag[17] == TRUE){
							// ????
							mailWndBtnFlag[17] = TRUE;
						}
						else{
							// ???
							mailWndBtnFlag[17] = FALSE;
						}
					}
					else{
						// ???
						mailWndBtnFlag[17] = FALSE;
					}

					// ??????
					if (HitDispNo == mailWndFontNo[14]){
						// ????????
						if (mouse.onceState & MOUSE_LEFT_CRICK){
							// ??? ??
							MailStr.buffer[0] = NULL;
							MailStr.cnt = 0;
							MailStr.cursor = 0;
							// ?????
							play_se(217, 320, 240);
							mailWndBtnFlag[14] = TRUE;
						}
						// ?????????
						if (mouse.state & MOUSE_LEFT_CRICK && mailWndBtnFlag[14] == TRUE){
							// ????
							mailWndBtnFlag[14] = TRUE;
						}
						else{
							// ???
							mailWndBtnFlag[14] = FALSE;
						}
					}
					else{
						// ???
						mailWndBtnFlag[14] = FALSE;
					}

					// ????????
					if (pActMenuWnd != NULL){
						int nowNo = mailPetSendWndPageNo * MAX_ADR_BOOK_COUNT;
						int color;

						// ?
						x = pActMenuWnd->x;
						y = pActMenuWnd->y;
						// ??????
						GetKeyInputFocus(&MailStr);

						if (pet[pc.mailPetNo].freeName[0] == NULL){
							CenteringStr(pet[pc.mailPetNo].name, moji, PET_NAME_LEN);
						}
						else{
							CenteringStr(pet[pc.mailPetNo].freeName, moji, PET_NAME_LEN);
						}
						StockFontBuffer(x + 120, y + 14, FONT_PRIO_FRONT, 0, moji, 0);
						StockFontBuffer2(&MailStr);
						for (i = 0; i < MAX_ADR_BOOK_COUNT; i++){
							// ????????????????
#ifdef _EXTEND_AB
							if (addressBook[nowNo + i].useFlag == TRUE
								|| addressBook[nowNo + i].useFlag == 2
								|| addressBook[nowNo + i].useFlag == 3){
#else
							if (addressBook[nowNo + i].useFlag == TRUE){
#endif
								color = FONT_PAL_GRAY;	// ?????
								// ???????
								if (addressBook[nowNo + i].onlineFlag > 0){
									// ?
									color = FONT_PAL_WHITE;
								}
								// ??????
								if (mailWndSendFlag[nowNo + i] == TRUE){
									// ?
									color = FONT_PAL_AQUA;
								}
								// 
								CenteringStr(addressBook[nowNo + i].name, moji, CHAR_NAME_LEN);
								mailWndFontNo[i] = StockFontBuffer(x + 111, y + 177, FONT_PRIO_FRONT, color, moji, 2);

							}
							y += 23;
						}
						// ???????
						mailWndFontNo[15] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my, DISP_PRIO_IME3, CG_MAIL_WND_ITEM_BTN, 2);
						// ???
						mailWndFontNo[16] = StockDispBuffer(pActMenuWnd->x + 25 - 8 + 4, pActMenuWnd->y + 241, DISP_PRIO_IME3, CG_PREV_BTN + mailWndBtnFlag[16], 2);
						// ????
						sprintf_s(moji, "%2d", mailPetSendWndPageNo + 1);
						StockFontBuffer(pActMenuWnd->x + 46 - 8 + 4, pActMenuWnd->y + 233, FONT_PRIO_FRONT, 0, moji, 0);
						// ????
						mailWndFontNo[17] = StockDispBuffer(pActMenuWnd->x + 75 + 4, pActMenuWnd->y + 241, DISP_PRIO_IME3, CG_NEXT_BTN + mailWndBtnFlag[17], 2);
						// ??????
						mailWndFontNo[14] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx + 60, pActMenuWnd->y + 390, DISP_PRIO_IME3, CG_MAIL_WND_CLEAR_BTN_UP + mailWndBtnFlag[14], 2);
						// ????
						mailWndFontNo[18] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx - 60, pActMenuWnd->y + 421, DISP_PRIO_IME3, CG_SEND_BTN + mailWndBtnFlag[18] * 75, 2);
						// ??
						mailWndFontNo[19] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx + 60, pActMenuWnd->y + 421, DISP_PRIO_IME3, CG_RETURN_BTN, 2);
					}
				}
			}
			break;

		case MAIL_WND_ITEM:	// ?????????
			// ?????????
			if (pActMenuWnd == NULL){
#ifdef _NEW_ITEM_
				道具栏页数 = 0;
#endif
				pActMenuWnd = MakeWindowDisp(4, 30, 271, 281, 0, -1);
				// ??????
				for (i = 0; i < MENU_MAIL_0; i++) mailWndFontNo[i] = -2;
				mailSendWndPageNo = 0;		// ?????????
				// ??????????
				InitItem(pActMenuWnd->x, pActMenuWnd->y - 1, FALSE);
				// ???????????
				if (MenuToggleFlag & JOY_CTRL_I && itemWndNo == 0){
					// ???????????
					MenuToggleFlag &= ~JOY_CTRL_I;
					DeathMenuAction2();
				}
			}
			else{
				// ??????????????
				if (pActMenuWnd->hp > 0){
					StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my, DISP_PRIO_IME3, CG_ITEM_WND_1, 1);
					StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my-80, DISP_PRIO_IME3, CG_BTL_ITEM_WND_TITLE, 1);
					if (mouse.onceState & MOUSE_LEFT_CRICK){
						// ???
						if (HitDispNo == mailWndFontNo[19]){
							// ??????
							DeathAction(pActMenuWnd);
							pActMenuWnd = NULL;
							// ????????
							mailWndNo = MAIL_WND_PET_SEND;
							// ????????
							play_se(203, 320, 240);
						}
					}
					// ????????
					if (pActMenuWnd != NULL){
						// ?
						x = pActMenuWnd->x + 16;
						y = pActMenuWnd->y + 191 + 6;
#ifdef _NEW_ITEM_
						for (i = 0; i < 3; i++){
							if (i == 道具栏页数){
								StockDispBuffer(287, 39 + i * 56, DISP_PRIO_BOX2, 55223 + i, 1);
							}
							else{
								BOOL flg = FALSE;
								if (i){
									if (pc.道具栏状态 & 1 << i){
										flg = TRUE;
									}
								}
								else flg = TRUE;
								if (flg){
									StockDispBuffer(271 + 10, 39 + i * 56, DISP_PRIO_IME3, 55226 + i, 1);
									if (MakeHitBox(261 + 10, 11 + i * 56, 281 + 10, 8 + i * 56 + 60, DISP_PRIO_IME3)){
										if (mouse.onceState & MOUSE_LEFT_CRICK){
											道具栏页数 = i;
										}
									}
								}
								else StockDispBuffer(271 + 10, 39 + i * 56, DISP_PRIO_IME3, 55229 + i, 1);
							}
						}
#endif
						for (i = MAX_ITEM - 1; i >= MAX_ITEMSTART; i--){
#ifdef _NEW_ITEM_
							//这里是邮件
							int 道具起始 = MAX_ITEMSTART + MAX_MAXHAVEITEM*道具栏页数;
							int 道具结束 = 道具起始 + MAX_MAXHAVEITEM;
							if (i >= MAX_ITEMSTART){
								if (i < 道具起始 || i >= 道具结束) continue;
							}
#endif
							if (MakeHitBox(ItemBuffer[i].defX - 24, ItemBuffer[i].defY - 24 - 160,
								ItemBuffer[i].defX + 25, ItemBuffer[i].defY + 23 - 160, DISP_PRIO_IME4) == TRUE){
								// ?????????????
								if (pc.item[i].useFlag == TRUE){
									// ??????
									char *splitPoint = pc.item[i].memo;
									int color = pc.item[i].color;

									// ?????????
									if (pc.level < pc.item[i].level) color = FONT_PAL_RED;
									// ?????
									StockFontBuffer(pActMenuWnd->x + 16, pActMenuWnd->y + 332 - 160 - 1, FONT_PRIO_FRONT, color, pc.item[i].name, 0);
									// ?????
									// ?????
									while (1){
										// ?????????
										if (strlen(splitPoint) > 28){
											strncpy_s(moji, splitPoint, 28);
											moji[28] = NULL;	// ??????
											// ??????
											if (GetStrLastByte(moji) == 3){
												moji[27] = NULL;
												splitPoint += 27;
											}
											else{
												moji[28] = NULL;
												splitPoint += 28;
											}
											StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, moji, 0); y += 24;
										}
										else{
											strcpy(moji, splitPoint);
											StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, moji, 0);
											break;
										}
									}
								}

								// ??????????????
								if (mouse.onceState & MOUSE_LEFT_DBL_CRICK){
									if (pc.item[i].useFlag == TRUE && pc.item[i].sendFlag & ITEM_FLAG_PET_MAIL){
										mailItemNo = i;	// ???????
										// ??????
										DeathAction(pActMenuWnd);
										pActMenuWnd = NULL;
										// ????????
										mailWndNo = MAIL_WND_PET_SEND;
										// ????????
										play_se(203, 320, 240);
										// ????????
										GetKeyInputFocus(&MyChatBuffer);
									}
									else{
										// ???
										play_se(220, 320, 240);
									}
								}
							}
							if (pc.item[i].useFlag == TRUE){
								// ????
								StockDispBuffer(ItemBuffer[i].defX, ItemBuffer[i].defY - 160, ItemBuffer[i].dispPrio, pc.item[i].graNo, 0);
							}
						}
						// ?????????
						if (pActMenuWnd != NULL){
							// ?????????
							mailWndFontNo[19] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, pActMenuWnd->y + 262, DISP_PRIO_IME4, CG_CLOSE_BTN, 2);
						}
					}
				}
			}
			break;

		case MAIL_WND_HISTORY:	// ???????
			// ?????????
			if (pActMenuWnd == NULL){
				pActMenuWnd = MakeWindowDisp(4, 4, 272, 424, 0, -1);
				// ??????
				for (i = 0; i < MENU_MAIL_0; i++) mailWndFontNo[i] = -2;

				// ?????????????????
				// ?????		
				for (mailHistoryWndPageNo = MAIL_MAX_HISTORY - 1; mailHistoryWndPageNo > 0; mailHistoryWndPageNo--){
					// ????????
					if (mailHistoryWndPageNo < 0) mailHistoryWndPageNo = MAIL_MAX_HISTORY - 1;
					// ???????
					mailHistoryWndNowPageNo = (MailHistory[mailHistoryWndSelectNo].newHistoryNo + mailHistoryWndPageNo) % MAIL_MAX_HISTORY;
					// ????????????
					if (MailHistory[mailHistoryWndSelectNo].noReadFlag[mailHistoryWndNowPageNo] >= TRUE) break;
				}
#if 0
				// ????????????????
				if( pActPet == NULL ){
					// ?????????
					pActPet = MakeAnimDisp( pActMenuWnd->x - 204, pActMenuWnd->y + 160, pet[ pc.mailPetNo ].graNo, ANIM_DISP_PET_RECIEVE );
					pActPet->atr |= ACT_ATR_INFO;
					pActPet->atr |= ACT_ATR_TYPE_PET;
					pActPet->level = 32;
					strcpy( pActPet->name, "测试宠物" );
				}
				// ?????????????????????????
				if (pActMailItem == NULL && MailHistory[mailHistoryWndSelectNo].itemGraNo[mailHistoryWndNowPageNo] != -1){
					// ??????????
					pActMailItem = MakeAnimDisp(pActMenuWnd->x - 204, pActMenuWnd->y + 128, pc.item[5].graNo, ANIM_DISP_PET_ITEM);
					pActMailItem->atr |= ACT_ATR_INFO;
					pActMailItem->atr |= ACT_ATR_TYPE_ITEM;
					pActMailItem->dispPrio = DISP_PRIO_DRAG;
					// ???????
					strcpy(pActMailItem->name, "道具完成");
				}
#endif
			}
			else{
				// ??????????????
				if (pActMenuWnd->hp > 0){
					static int nowDelNo;
					// ???????
					StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my, DISP_PRIO_MENU, CG_MAIL_WND_HISTORY_WND, 1);
					// ????????
					if (mouse.onceState & MOUSE_LEFT_CRICK){
						// ??????
						if (HitDispNo == mailWndFontNo[18]){
							// ??????
							DeathAction(pActMenuWnd);
							pActMenuWnd = NULL;
							// ?????????
							DeathLetterAction();
							// ????
							DeathAction(pActPet);
							pActPet = NULL;
							// ?????
							DeathAction(pActMailItem);
							pActMailItem = NULL;
							// ????????????
							mailWndNoBak = mailWndNo;
							// ????????
							InitMailSendFlag();
							// ??????????
							mailWndSendFlag[mailHistoryWndSelectNo] = TRUE;
							// ???
							if (ProcNo == PROC_BATTLE){
								// ??????????
								mailWndNo = MAIL_WND_SEND;
							}
							else{
								// ?????????????
								if (pc.mailPetNo != -1){
									// ??????
									mailWndNo = MAIL_WND_SELECT;
								}
								else{
									// ??????????
									mailWndNo = MAIL_WND_SEND;
								}
							}
							// ????????
							play_se(202, 320, 240);
						}
						// ???
						if (HitDispNo == mailWndFontNo[19]){
							// ??????
							DeathAction(pActMenuWnd);
							pActMenuWnd = NULL;
							// ?????????
							DeathLetterAction();
							// ????
							DeathAction(pActPet);
							pActPet = NULL;
							// ?????
							DeathAction(pActMailItem);
							pActMailItem = NULL;
							// ????????
							mailWndNo = MAIL_WND_VIEW;
							// ????????
							play_se(203, 320, 240);
						}
					}
					// ???
					if (HitDispNo == mailWndFontNo[16] || joy_con[0] & JOY_A){
						// ????????
						if (mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[0] & JOY_A){
							mailHistoryWndPageNo--; // ?????
							if (mailHistoryWndPageNo < 0) mailHistoryWndPageNo = MAIL_MAX_HISTORY - 1;
							// ?????????
							DeathLetterAction();
							// ????
							DeathAction(pActPet);
							pActPet = NULL;
							// ?????
							DeathAction(pActMailItem);
							pActMailItem = NULL;
							// ?????
							play_se(217, 320, 240);
							mailWndBtnFlag[16] = TRUE;
						}
						// ?????????
						if ((mouse.state & MOUSE_LEFT_CRICK || joy_con[0] & JOY_A) && mailWndBtnFlag[16] == TRUE){
							// ????
							mailWndBtnFlag[16] = TRUE;
						}
						else{
							// ???
							mailWndBtnFlag[16] = FALSE;
						}
					}
					else{
						// ???
						mailWndBtnFlag[16] = FALSE;
					}
					// ???
					if (HitDispNo == mailWndFontNo[17] || joy_con[0] & JOY_B){
						// ????????
						if (mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[0] & JOY_B){
							mailHistoryWndPageNo++; // ??????
							if (mailHistoryWndPageNo >= MAIL_MAX_HISTORY) mailHistoryWndPageNo = 0;
							// ?????????
							DeathLetterAction();
							// ????
							DeathAction(pActPet);
							pActPet = NULL;
							// ?????
							DeathAction(pActMailItem);
							pActMailItem = NULL;
							// ?????
							play_se(217, 320, 240);
							mailWndBtnFlag[17] = TRUE;
						}
						// ?????????
						if ((mouse.state & MOUSE_LEFT_CRICK || joy_con[0] & JOY_B) && mailWndBtnFlag[17] == TRUE){
							// ????
							mailWndBtnFlag[17] = TRUE;
						}
						else{
							// ???
							mailWndBtnFlag[17] = FALSE;
						}
					}
					else{
						// ???
						mailWndBtnFlag[17] = FALSE;
					}

					// ????????
					if (pActMenuWnd != NULL){
						// ?
						x = pActMenuWnd->x;
						y = pActMenuWnd->y;
						char *splitPoint;
						// ???????
						mailHistoryWndNowPageNo = (MailHistory[mailHistoryWndSelectNo].newHistoryNo + mailHistoryWndPageNo) % MAIL_MAX_HISTORY;
						// ?????????
						if (MailHistory[mailHistoryWndSelectNo].noReadFlag[mailHistoryWndNowPageNo] >= TRUE){
							// ????????
							if (MailHistory[mailHistoryWndSelectNo].noReadFlag[mailHistoryWndNowPageNo] > TRUE){
								// ????????????????
								if (pActPet == NULL){
									// ?????????
									pActPet = MakeAnimDisp(pActMenuWnd->x - 204, pActMenuWnd->y + 160, MailHistory[mailHistoryWndSelectNo].noReadFlag[mailHistoryWndNowPageNo], ANIM_DISP_PET_RECIEVE);
									pActPet->atr |= ACT_ATR_INFO;
									pActPet->atr |= ACT_ATR_TYPE_PET;
									// ??????
									pActPet->level = MailHistory[mailHistoryWndSelectNo].petLevel[mailHistoryWndNowPageNo];
									// ????
									strcpy(pActPet->name, MailHistory[mailHistoryWndSelectNo].petName[mailHistoryWndNowPageNo]);
								}
								// ?????????????????????????
								if (pActMailItem == NULL && MailHistory[mailHistoryWndSelectNo].itemGraNo[mailHistoryWndNowPageNo] != -1){
									// ??????????
									pActMailItem = MakeAnimDisp(pActMenuWnd->x - 204, pActMenuWnd->y + 128, MailHistory[mailHistoryWndSelectNo].itemGraNo[mailHistoryWndNowPageNo], ANIM_DISP_PET_ITEM);
									pActMailItem->atr |= ACT_ATR_INFO;
									pActMailItem->atr |= ACT_ATR_TYPE_ITEM;
									pActMailItem->dispPrio = DISP_PRIO_DRAG;
									// ???????
									strcpy(pActMailItem->name, "道具完成");
								}
							}
							// ???????????
							if (pActLetter[0] == NULL){
								// ???????????
								pActLetter[0] = MakeAnimDisp(x + 28, y + 244, SPR_mail, ANIM_DISP_LETTER);
							}
							// ????
							MailHistory[mailHistoryWndSelectNo].noReadFlag[mailHistoryWndNowPageNo] = FALSE;
							// ???????
							SaveMailHistory(mailHistoryWndSelectNo);
							// ????
							mailLamp = CheckMailNoReadFlag();

						}
						// ??????
						splitPoint = MailHistory[mailHistoryWndSelectNo].str[mailHistoryWndNowPageNo];
						// 
						CenteringStr(addressBook[mailHistoryWndSelectNo].name, moji, CHAR_NAME_LEN);
						StockFontBuffer(x + 64, y + 34, FONT_PRIO_FRONT, 0, moji, 0);
						// 
						StockFontBuffer(x + 148, y + 236, FONT_PRIO_FRONT, 0, MailHistory[mailHistoryWndSelectNo].dateStr[mailHistoryWndNowPageNo], 0);
						// 
						while (1){
							// ?????????
							if (strlen(splitPoint) > 28){
								strncpy_s(moji, splitPoint, 28);
								moji[28] = NULL;	// ??????
								// ??????
								if (GetStrLastByte(moji) == 3){
									moji[27] = NULL;
									splitPoint += 27;
								}
								else{
									moji[28] = NULL;
									splitPoint += 28;
								}
								StockFontBuffer(x + 12, y + 260, FONT_PRIO_FRONT, 0, moji, 0); y += 20;
							}
							else{
								strcpy(moji, splitPoint);
								StockFontBuffer(x + 12, y + 260, FONT_PRIO_FRONT, 0, moji, 0);
								break;
							}
						}
						// ???
						mailWndFontNo[16] = StockDispBuffer(pActMenuWnd->x + 25 + 16 + 8, pActMenuWnd->y + 405, DISP_PRIO_IME3, CG_PREV_BTN + mailWndBtnFlag[16], 2);
						// ????
						sprintf_s(moji, "%2d", mailHistoryWndPageNo + 1);
						StockFontBuffer(pActMenuWnd->x + 46 + 16 + 9, pActMenuWnd->y + 397, FONT_PRIO_FRONT, 0, moji, 0);
						// ????
						mailWndFontNo[17] = StockDispBuffer(pActMenuWnd->x + 75 + 16 + 9 + 8, pActMenuWnd->y + 405, DISP_PRIO_IME3, CG_NEXT_BTN + mailWndBtnFlag[17], 2);
						// ??????
						mailWndFontNo[18] = StockDispBuffer(pActMenuWnd->x + 196, pActMenuWnd->y + 371, DISP_PRIO_IME3, CG_MAIL_WND_MAIL_BTN, 2);
						// ?????
						mailWndFontNo[19] = StockDispBuffer(pActMenuWnd->x + 220 - 16 - 8, pActMenuWnd->y + 405, DISP_PRIO_IME3, CG_RETURN_BTN, 2);
					}
				}
			}
			break;


		}
	}

	//? ??????????? *******************************************************/

	if (MenuToggleFlag & JOY_CTRL_A){

		// ????????
		switch (albumWndNo){

		case 0:	// ??????

			// ?????????
			if (pActMenuWnd == NULL){
				// ?????????
				pActMenuWnd = MakeWindowDisp(4, 4, 272, 448, 0, -1);
				// ??????
				for (i = 0; i < MENU_ALBUM_0; i++) albumWndFontNo[i] = -2;
				// ???????
				albumWndPageNo = 0;
				// ????????
				albumNo = -1;
			}
			else{
				// ??????????????
				if (pActMenuWnd->hp > 0){
					// ?????????
					StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd->pYobi)->my, DISP_PRIO_MENU, CG_ALBUM_WND, 1);
					// ????????
					if (mouse.onceState & MOUSE_LEFT_CRICK){
						// ?????????
						int nowTopNo = albumWndPageNo * 8;
						// ???????
						for (i = 0; i < 8; i++){
							// ????????????????
							if (HitFontNo == albumWndFontNo[i] && PetAlbum[nowTopNo + i].flag >= 2){
								// ???????
								albumNo = nowTopNo + i;
								// ??????
								PetAlbum[albumNo].flag = 3;
								// ?????
								SaveAlbum(albumNo);
								// ?????
								play_se(217, 320, 240);
							}
						}
						// ???
						if (HitDispNo == albumWndFontNo[19]){
							// ??????
							DeathAction(pActMenuWnd);
							pActMenuWnd = NULL;
							// ???????
							MenuToggleFlag ^= JOY_CTRL_A;
							// ????????
							play_se(203, 320, 240);
						}
					}

					// ???
					if (HitDispNo == albumWndFontNo[16] || joy_con[0] & JOY_A){
						// ????????
						if (mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[0] & JOY_A){
							int limitPage = ((MAX_PET_KIND + MAX_PET_PAGE_SIZE - 1) / MAX_PET_PAGE_SIZE) - 1;
							albumWndPageNo--; // ?????
							// ????????
							if (albumWndPageNo < 0) albumWndPageNo = limitPage;
							// ?????
							play_se(217, 320, 240);
							albumWndBtnFlag[16] = TRUE;
						}
						// ?????????
						if ((mouse.state & MOUSE_LEFT_CRICK || joy_con[0] & JOY_A) && albumWndBtnFlag[16] == TRUE){
							// ????
							albumWndBtnFlag[16] = TRUE;
						}
						else{
							// ???
							albumWndBtnFlag[16] = FALSE;
						}
					}
					else{
						// ???
						albumWndBtnFlag[16] = FALSE;
					}
					// ???
					if (HitDispNo == albumWndFontNo[17] || joy_con[0] & JOY_B){
						// ????????
						if (mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[0] & JOY_B){
							int limitPage = ((MAX_PET_KIND + MAX_PET_PAGE_SIZE - 1) / MAX_PET_PAGE_SIZE) - 1;
							albumWndPageNo++; // ??????
							// ????????
							if (albumWndPageNo > limitPage) albumWndPageNo = 0;
							// ?????
							play_se(217, 320, 240);
							albumWndBtnFlag[17] = TRUE;
						}
						// ?????????
						if ((mouse.state & MOUSE_LEFT_CRICK || joy_con[0] & JOY_B) && albumWndBtnFlag[17] == TRUE){
							// ????
							albumWndBtnFlag[17] = TRUE;
						}
						else{
							// ???
							albumWndBtnFlag[17] = FALSE;
						}
					}
					else{
						// ???
						albumWndBtnFlag[17] = FALSE;
					}


					// ????????
					if (pActMenuWnd != NULL){
#define ALBUM_WND_ATTR_X 34
#define ALBUM_WND_ATTR_Y 0
						int x2;
						int nowTopNo;	// ?????????
						// ?
						x = pActMenuWnd->x;
						y = pActMenuWnd->y + 28;

						// ????????
						if (albumNo != -1){
							// ??????????
							CenteringStr(PetAlbum[albumNo].freeName, moji, PET_NAME_LEN);
							StockFontBuffer(x + 13, y, FONT_PRIO_FRONT, 0, moji, 0); y += 22;

							sprintf_s(moji, "%3d", PetAlbum[albumNo].level);
							StockFontBuffer(x + 36, y, FONT_PRIO_FRONT, 0, moji, 0); y += 24;

							sprintf_s(moji, "%4d", PetAlbum[albumNo].maxHp);
							StockFontBuffer(x + 36, y, FONT_PRIO_FRONT, 0, moji, 0);

							sprintf_s(moji, "%3d", PetAlbum[albumNo].str);
							StockFontBuffer(x + 36 + 72, y, FONT_PRIO_FRONT, 0, moji, 0); y += 24;

							sprintf_s(moji, "%4d", PetAlbum[albumNo].quick);
							StockFontBuffer(x + 36, y, FONT_PRIO_FRONT, 0, moji, 0);

							sprintf_s(moji, "%3d", PetAlbum[albumNo].def);
							StockFontBuffer(x + 36 + 72, y, FONT_PRIO_FRONT, 0, moji, 0); y += 29;

							// 佋???
							if (PetAlbum[albumNo].earth > 0){
								x2 = (int)(PetAlbum[albumNo].earth * 0.8);

								StockBoxDispBuffer(x + ALBUM_WND_ATTR_X + 0, y + ALBUM_WND_ATTR_Y + 0, x + ALBUM_WND_ATTR_X + 0 + x2, y + ALBUM_WND_ATTR_Y + 8, DISP_PRIO_IME4, SYSTEM_PAL_GREEN, 1);
								StockBoxDispBuffer(x + ALBUM_WND_ATTR_X + 1, y + ALBUM_WND_ATTR_Y + 1, x + ALBUM_WND_ATTR_X + 1 + x2, y + ALBUM_WND_ATTR_Y + 9, DISP_PRIO_IME3, SYSTEM_PAL_GREEN2, 0);
								StockBoxDispBuffer(x + ALBUM_WND_ATTR_X + 2, y + ALBUM_WND_ATTR_Y + 2, x + ALBUM_WND_ATTR_X + 2 + x2, y + ALBUM_WND_ATTR_Y + 10, DISP_PRIO_IME3, SYSTEM_PAL_GREEN2, 0);
							}
							y += 20;

							// ????
							if (PetAlbum[albumNo].water > 0){
								x2 = (int)(PetAlbum[albumNo].water * 0.8);

								StockBoxDispBuffer(x + ALBUM_WND_ATTR_X + 0, y + ALBUM_WND_ATTR_Y + 0, x + ALBUM_WND_ATTR_X + 0 + x2, y + ALBUM_WND_ATTR_Y + 8, DISP_PRIO_IME4, SYSTEM_PAL_AQUA, 1);
								StockBoxDispBuffer(x + ALBUM_WND_ATTR_X + 1, y + ALBUM_WND_ATTR_Y + 1, x + ALBUM_WND_ATTR_X + 1 + x2, y + ALBUM_WND_ATTR_Y + 9, DISP_PRIO_IME3, SYSTEM_PAL_AQUA2, 0);
								StockBoxDispBuffer(x + ALBUM_WND_ATTR_X + 2, y + ALBUM_WND_ATTR_Y + 2, x + ALBUM_WND_ATTR_X + 2 + x2, y + ALBUM_WND_ATTR_Y + 10, DISP_PRIO_IME3, SYSTEM_PAL_AQUA2, 0);
							}
							y += 20;

							// ????
							if (PetAlbum[albumNo].fire > 0){
								x2 = (int)(PetAlbum[albumNo].fire * 0.8);

								StockBoxDispBuffer(x + ALBUM_WND_ATTR_X + 0, y + ALBUM_WND_ATTR_Y + 0, x + ALBUM_WND_ATTR_X + 0 + x2, y + ALBUM_WND_ATTR_Y + 8, DISP_PRIO_IME4, SYSTEM_PAL_RED, 1);
								StockBoxDispBuffer(x + ALBUM_WND_ATTR_X + 1, y + ALBUM_WND_ATTR_Y + 1, x + ALBUM_WND_ATTR_X + 1 + x2, y + ALBUM_WND_ATTR_Y + 9, DISP_PRIO_IME3, SYSTEM_PAL_RED2, 0);
								StockBoxDispBuffer(x + ALBUM_WND_ATTR_X + 2, y + ALBUM_WND_ATTR_Y + 2, x + ALBUM_WND_ATTR_X + 2 + x2, y + ALBUM_WND_ATTR_Y + 10, DISP_PRIO_IME3, SYSTEM_PAL_RED2, 0);
							}
							y += 20;

							// ????
							if (PetAlbum[albumNo].wind > 0){
								x2 = (int)(PetAlbum[albumNo].wind * 0.8);

								StockBoxDispBuffer(x + ALBUM_WND_ATTR_X + 0, y + ALBUM_WND_ATTR_Y + 0, x + ALBUM_WND_ATTR_X + 0 + x2, y + ALBUM_WND_ATTR_Y + 8, DISP_PRIO_IME4, SYSTEM_PAL_YELLOW, 1);
								StockBoxDispBuffer(x + ALBUM_WND_ATTR_X + 1, y + ALBUM_WND_ATTR_Y + 1, x + ALBUM_WND_ATTR_X + 1 + x2, y + ALBUM_WND_ATTR_Y + 9, DISP_PRIO_IME3, SYSTEM_PAL_YELLOW2, 0);
								StockBoxDispBuffer(x + ALBUM_WND_ATTR_X + 2, y + ALBUM_WND_ATTR_Y + 2, x + ALBUM_WND_ATTR_X + 2 + x2, y + ALBUM_WND_ATTR_Y + 10, DISP_PRIO_IME3, SYSTEM_PAL_YELLOW2, 0);
							}

							StockDispBuffer(pActMenuWnd->x + 151 + 48, pActMenuWnd->y + 65 + 52, DISP_PRIO_IME3, PetAlbum[albumNo].faceGraNo, 0);
						}

						y = pActMenuWnd->y + 215;

						// ?????????
						nowTopNo = albumWndPageNo * 8;
						// ???????
						for (i = 0; i < 8; i++){
							// ????????
							if (nowTopNo + i >= MAX_PET_KIND) break;

							// ??????????
							if (PetAlbum[nowTopNo + i].flag == 2){
								// ?
								StockDispBuffer(x + 24, y + 9, DISP_PRIO_IME3, CG_ALBUM_WND_NEW_ICON, 0);
							}
							// ??????
							sprintf_s(moji, "No.%3d", nowTopNo + i + 1);
							StockFontBuffer(x + 46, y + 1, FONT_PRIO_FRONT, 0, moji, 0);

							if (PetAlbum[nowTopNo + i].flag != 0){
								// ????
								if (PetAlbum[nowTopNo + i].flag == 1){
									CenteringStr(PetAlbum[nowTopNo + i].name, moji, PET_NAME_LEN);
									StockFontBuffer(x + 112, y, FONT_PRIO_FRONT, FONT_PAL_GRAY, moji, 0);
									// ???????
									albumWndFontNo[i] = -2;
								}
								else
									// ???
								if (PetAlbum[nowTopNo + i].flag >= 2){
									CenteringStr(PetAlbum[nowTopNo + i].name, moji, PET_NAME_LEN);
									albumWndFontNo[i] = StockFontBuffer(x + 112, y, FONT_PRIO_FRONT, 0, moji, 2);
								}
							}
							else{
								// ???????
								albumWndFontNo[i] = -2;
							}
							y += 22;
						}

						// ???
						albumWndFontNo[16] = StockDispBuffer(pActMenuWnd->x + 25 + 32 + 10 + 40, pActMenuWnd->y + 400, DISP_PRIO_IME3, CG_PREV_BTN + albumWndBtnFlag[16], 2);
						albumWndFontNo[17] = StockDispBuffer(pActMenuWnd->x + 75 + 32 + 9 + 10 + 40, pActMenuWnd->y + 400, DISP_PRIO_IME3, CG_NEXT_BTN + albumWndBtnFlag[17], 2);
						// ?????
						albumWndFontNo[19] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, pActMenuWnd->y + 448 - 19, DISP_PRIO_IME3, CG_CLOSE_BTN, 2);
					}
				}
			}
			break;
		}
	}

	//? ??????? ***********************************************************/
	if (BattleResultWndFlag >= 1 && ProcNo == PROC_GAME && SubProcNo == 3){
		// ??????
		if (--ResultWndTimer <= 0){
			// ??????
			DeathAction(pActMenuWnd);
			pActMenuWnd = NULL;
			// ???????????
			BattleResultWndFlag = FALSE;
			// ????????????
			if (MapWmdFlagBak != TRUE){
				// ????????
				play_se(203, 320, 240);
			}
		}

		static int 宠物经验判断 = FALSE;
		switch (BattleResultWndFlag){

		case 1:	// ?????????
			if (pActMenuWnd == NULL){
				宠物经验判断 = FALSE;
				int flag = 0;
				// ?????????
				pActMenuWnd = MakeWindowDisp(320 - 160, 240 - 120, 5, 6, CG_WND_TITLE_RESULT, 2);
				// ??????
				for (i = 0; i < MENU_BTL_RESULT_0; i++) resultWndFontNo[i] = -2;
				// ??????????
				for (i = 0; i < 4; i++){
					// ?????????????
					if (battleResultMsg.resChr[i].levelUp == TRUE) flag = TRUE;
				}
				if (flag == TRUE){
					// ???????
					play_se(211, 320, 240);
				}
				else{
					// ????????
					play_se(202, 320, 240);
				}
			}
			else{
				// ??????????????
				if (pActMenuWnd->hp > 0){
					// ????????
					if (mouse.onceState & MOUSE_LEFT_CRICK){
						// ???
						if (HitDispNo == resultWndFontNo[0]){
							// ??????
							DeathAction(pActMenuWnd);
							pActMenuWnd = NULL;
							// ???????????
							BattleResultWndFlag = FALSE;
							// ????????????
							if (MapWmdFlagBak != TRUE){
								// ????????
								play_se(203, 320, 240);
							}
						}
					}

					// ????????
					if (pActMenuWnd != NULL){
						int color;
						int flag, j;
						// ?
						x = pActMenuWnd->x + 12;
						y = pActMenuWnd->y + 44;
						
						// ?
						CenteringStr( pc.name, moji, CHAR_NAME_LEN );
						StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, moji, 0 ); 
						color = FONT_PAL_GRAY;	// ?????
						// ????????
						if( battleResultMsg.resChr[ 0 ].levelUp == TRUE ){
							// ??????
							StockFontBuffer( x + 152, y, FONT_PRIO_FRONT, FONT_PAL_YELLOW, "LvUp!", 0 ); 
						}
						// ??
						sprintf( moji,"Exp %+5d", battleResultMsg.resChr[ 0 ].exp );
						StockFontBuffer( x + 148 + 58, y, FONT_PRIO_FRONT, 0, moji, 0 ); 
						y += 20;
						
						// ??????
						for( i = 0 ; i < 5 ; i++ ){
							// ??????????? ?? ??????
							if( pet[ i ].useFlag == TRUE
									&& (pc.selectPetNo[ i ] == TRUE || pc.ridePetNo == i ) ){
								// ????
								// ??????
								if( pet[ i ].freeName[ 0 ] != NULL )
									CenteringStr( pet[ i ].freeName, moji, PET_NAME_LEN );
								else
									CenteringStr( pet[ i ].name, moji, PET_NAME_LEN );
								StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, moji, 0 );
								color = FONT_PAL_GRAY;	// ?????
								flag = FALSE;			// ??????
								// ?????????
								for( j = 1 ; j < RESULT_CHR_EXP ; j++ ){
									// ????
									if( battleResultMsg.resChr[ j ].petNo == i ){
										// ????????
										if( battleResultMsg.resChr[ j ].levelUp == TRUE ){	// ????????
											// ??????
											StockFontBuffer( x + 152, y, FONT_PRIO_FRONT, FONT_PAL_YELLOW,"LvUp!", 0 ); 
										}
										// ??
										sprintf( moji,"Exp %+5d", battleResultMsg.resChr[ j ].exp );
										StockFontBuffer( x + 148 + 58, y, FONT_PRIO_FRONT, 0, moji, 0 ); 
										flag = TRUE;
									}
								}
								// ????
								if( flag == FALSE ){
									sprintf( moji,"Exp %+4d", battleResultMsg.resChr[ j ].exp );
									StockFontBuffer( x + 148 + 58, y, FONT_PRIO_FRONT, 0, "Exp    +0", 0 ); 
								}
								y += 20;
							}
						}
						flag = FALSE;
						y = pActMenuWnd->y + 134;
						// 	????: 
						StockFontBuffer( x, y+30, FONT_PRIO_FRONT, 0, "  道具 ：", 0 ); 
						char itemName[128];
						memset(itemName,0,128);
						for( i = 0 ; i < 3 ; i++ ){
							// ?????????
							if( battleResultMsg.item[ i ][ 0 ] != NULL ){
								sprintf_s(itemName + strlen(itemName), sizeof(itemName)-strlen(itemName), " %s ", battleResultMsg.item[i]);
								StockFontBuffer( x + 85, y+30, FONT_PRIO_FRONT, 0, battleResultMsg.item[ i ], 0 ); 
								y += 22;
								flag = TRUE;
							}
						}
						if( flag == FALSE ){
							StockFontBuffer( x + 85, y+30, FONT_PRIO_FRONT, 0, "没有得到任何道具。", 0 ); 
						}
						resultWndFontNo[ 0 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, pActMenuWnd->y + 216+30, DISP_PRIO_IME3, CG_CLOSE_BTN, 2 );

						if (!宠物经验判断&&经验开关){
							int petexp[2];
							int petlevel[2];
							int petindex[2];
							petexp[0] = petexp[1] = -1;
							petindex[0] = petindex[1] = -1;
							petlevel[0] = petlevel[1] = 0;
							int num = 1;
							for(num;num < RESULT_CHR_EXP;num++){
								if(battleResultMsg.resChr[num].petNo>=0){
									if(pet[battleResultMsg.resChr[num].petNo].useFlag){
										if(pc.battlePetNo==battleResultMsg.resChr[num].petNo){
											petexp[1] = battleResultMsg.resChr[num].exp;
											petlevel[1] = battleResultMsg.resChr[num].levelUp;
											petindex[1] = battleResultMsg.resChr[num].petNo;
										}
										if(pc.ridePetNo==battleResultMsg.resChr[num].petNo){
											petexp[0] = battleResultMsg.resChr[num].exp;
											petlevel[0] = battleResultMsg.resChr[num].levelUp;
											petindex[0] = battleResultMsg.resChr[num].petNo;
										}
									}
								}
							}
							宠物经验判断 = !宠物经验判断;
							char token[256];
							if(battleResultMsg.resChr[0].levelUp)
								sprintf_s(token, "得到经验：[自己：%d UpLv!] ", battleResultMsg.resChr[0].exp);
							else
								sprintf_s(token, "得到经验：[自己：%d] ", battleResultMsg.resChr[0].exp);
							if(petindex[0]>=0){
								if (petlevel[0])
									sprintf_s(token + strlen(token), sizeof(token)-strlen(token), "[%s：%d UpLv!] ",pet[ petindex[0] ].name , petexp[0]);
								else
									sprintf_s(token + strlen(token), sizeof(token)-strlen(token), "[%s：%d] ", pet[ petindex[0] ].name , petexp[0]);
							}
							if(petindex[1]>=0){
								if (petlevel[1])
									sprintf_s(token + strlen(token), sizeof(token)-strlen(token), "[%s：%d UpLv!] ",pet[ petindex[1] ].name , petexp[1]);
								else
									sprintf_s(token + strlen(token), sizeof(token)-strlen(token), "[%s：%d] ", pet[ petindex[1] ].name , petexp[1]);
							}
							if (itemName[0]) sprintf_s(token + strlen(token), sizeof(token)-strlen(token), "得到物品：[%s]", itemName);
							StockChatBufferLine(token, FONT_PAL_YELLOW);
						}
					}
				}
			}
			break;

		case 2:	// ??????????

			// ?????????
			if (pActMenuWnd == NULL){
				int flag = 0;
				// ?????????
				pActMenuWnd = MakeWindowDisp(320 - 96, 240 - 72, 3, 3, CG_WND_TITLE_RESULT, 2);
				// ??????
				for (i = 0; i < MENU_BTL_RESULT_0; i++) resultWndFontNo[i] = -2;
				// ????????
				play_se(202, 320, 240);
			}
			else{
				// ??????????????
				if (pActMenuWnd->hp > 0){
					// ????????
					if (mouse.onceState & MOUSE_LEFT_CRICK){
						// ???
						if (HitDispNo == resultWndFontNo[0]){
							// ??????
							DeathAction(pActMenuWnd);
							pActMenuWnd = NULL;
							// ???????????
							BattleResultWndFlag = FALSE;
							// ????????????
							if (MapWmdFlagBak != TRUE){
								// ????????
								play_se(203, 320, 240);
							}
						}
					}

					// ????????
					if (pActMenuWnd != NULL){
						int color = FONT_PAL_WHITE;
						// ?
						x = pActMenuWnd->x + 17;
						y = pActMenuWnd->y + 48;
						// ??
						sprintf_s(moji, " Ｄ  Ｐ  %+8d", battleResultMsg.resChr[0].exp);
						StockFontBuffer(x, y, FONT_PRIO_FRONT, color, moji, 0);
						y += 28;
						// ??
						sprintf_s(moji, " 合  计  %8d", battleResultMsg.resChr[1].exp);
						StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, moji, 0);

						// ?????????
						resultWndFontNo[0] = StockDispBuffer(((WINDOW_DISP *)pActMenuWnd->pYobi)->mx, pActMenuWnd->y + 120, DISP_PRIO_IME3, CG_CLOSE_BTN, 2);
					}
				}
			}
			break;
		}
	}

	//以下开始为交易视窗部分
	if (MenuToggleFlag & JOY_CTRL_T)
	{
		char buffer[1024];
#ifdef _TRADESYSTEM2	// Syu ADD 新交易系统
		char buffer2[1024];
		int j;
#endif
#ifdef _PET_ITEM
		static BOOL	bViewPetEquip = FALSE;
#endif
		switch (tradeWndNo){
#ifdef _COMFIRM_TRADE_REQUEST
		case 1:
		{
			static int btnYes = -1, btnNo = -1;
			char questStr[200];
			sprintf(questStr, "确定接受玩家[style c=5]%s[/style]的交易请求吗？", opp_name);
			StockFontBuffer(245, 220, FONT_PRIO_AFRONT, 3, questStr, 0); 
			StockDispBuffer(320, 240, DISP_PRIO_YES_NO_WND, CG_DROPWND, 0);
			btnYes = StockDispBuffer(320, 240, DISP_PRIO_YES_NO_BTN, CG_COMMON_YES_BTN, 2);
			btnNo = StockDispBuffer(320, 240, DISP_PRIO_YES_NO_BTN, CG_COMMON_NO_BTN, 2);

			if (mouse.onceState & MOUSE_LEFT_CRICK)
			{
				if (btnYes == HitDispNo)
				{
					sprintf_s(buffer, "C|%s|%s|1", opp_sockfd, opp_name);
					//送出取消讯息给Server通知对方
					lssproto_TD_send(sockfd, buffer);
					play_se(203, 320, 240);
					btnYes = -1;
					btnNo = -1;
					tradeWndNo = 0;
				}
				else if (btnNo == HitDispNo)
				{
					sprintf_s(buffer, "C|%s|%s|0", opp_sockfd, opp_name);
					//送出取消讯息给Server通知对方
					lssproto_TD_send(sockfd, buffer);
					play_se(203, 320, 240);
					btnYes = -1;
					btnNo = -1;
					tradeWndNo = 0;
				}
			}
			break;
		}
#endif
		default:
			//视窗为产生时初始化
			if (pActMenuWnd4 == NULL)
			{
				DeathMenuAction();
				DeathMenuAction2();
				//手动输入金额buff初始化
				TradeBuffer.buffer[0] = NULL;
				TradeBuffer.cnt = 0;
				TradeBuffer.cursor = 0;
				TradeBuffer.len = 8;
				TradeBuffer.color = 0;
				TradeBuffer.x = 600;
				TradeBuffer.y = 120;
				TradeBuffer.fontPrio = FONT_PRIO_FRONT;

				x = (lpDraw->xSize - 620) / 2;
				y = (lpDraw->ySize - 456) / 2;
				pActMenuWnd4 = MakeWindowDisp(x, y, 620, 456, 0, -1, FALSE);
#ifdef _NEW_ITEM_
				道具栏页数 = 0;
#endif
#ifdef _CHANGETRADERULE		   // (不可开) Syu ADD 交易规则修订
				TradeBtnflag = false;
#endif


#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
				//状态初始化
				locknum = -1;
				locknum2 = -1;
				MainTradeWndflag = true;
				SecondTradeWndflag = false;

				//andy_add 20030610
				for (i = 0; i < MENU_TRADE_0; i++) tradeWndFontNo[i] = -2;
				for (i = 0; i < 6; i++) SecondtradeWndFontNo[i] = -2;

				for (i = 0; i < 43; i++)
					tradeList[i].data = -1;
#ifdef _TRADETALKWND				// (不可开) Syu ADD 交易新增对话框架
				//视窗开启清空内容
				tradetalkwndflag = false;
				for (i = 0; i < 4; i++)
					sprintf_s(talkmsg[i], "");
#endif
				drag1Y = 67;
				drag2Y = 257;

				x = (lpDraw->xSize - 620) / 2;
				y = (lpDraw->ySize - 456) / 2;
				SecondTradeWnd = MakeWindowDisp(x, y, 620, 456, 0, -1, FALSE);
				for (i = 0; i < 43; i++) {   //清理交易列表
					tradeList[i].data = -1;
					tradeList[i].kind = 'S';
				}
				for (i = 0; i < 21; i++){	//清理交易列表
					mytradelist[i] = -1;
					opptradelist[i] = -1;
				}
#ifdef _NEW_ITEM_
				for (i = 0; i < MAX_MAXHAVEITEM * 3; i++) {
#ifdef _ITEM_PILENUMS
					itemflag[i] = pc.item[i + MAX_ITEMSTART].pile;
#else
					itemflag[i] = 0 ; 
#endif

#ifdef _NPC_ITEMUP
#endif
				}
#endif


				for (i = 0; i < MAX_MAXHAVEITEM; i++) {
#ifndef _NEW_ITEM_
#ifdef _ITEM_PILENUMS
					itemflag[i] = pc.item[i + MAX_ITEMSTART].pile;
#else
					itemflag[i] = 0;
#endif
#endif
					strcpy(opp_item[i].itemindex, "-1");
					strcpy(opp_item[i].damage, "");
					strcpy(opp_item[i].effect, "");
					strcpy(opp_item[i].name, "");
#ifdef _NPC_ITEMUP
#endif
				}

#endif

				InitItem3(325, 230);
				//Terry add 2003/11/19
				g_bTradesystemOpen = true;
				//end

			}
			else
			{

#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
				// 主视窗内容
				if (MainTradeWndflag == true)
				{
					SecondTradeWndflag = false;
#endif

					if (pActMenuWnd4->hp > 0)
					{


						//产生交易主视窗
#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
						//回主视窗时清除检视视窗PetAction
						if (SecondActPet != NULL)
						{
							DeathAction(SecondActPet);
							SecondActPet = NULL;
						}


						StockDispBuffer(((WINDOW_DISP *)pActMenuWnd4->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd4->pYobi)->my - 20, DISP_PRIO_MENU, CG_TRADE_WND, 1);
#endif


						// 以下为滑鼠左键被按下时 Start
						if (mouse.onceState & MOUSE_LEFT_CRICK)
						{
							// 处理取消键 Start
							//处理按下取消键
							if (HitDispNo == tradeWndFontNo[1])
							{
								//关闭交易视窗
								MenuToggleFlag &= ~JOY_CTRL_T;

								//视窗关闭音效
								play_se(203, 320, 240);
								sprintf_s(buffer, "W|%s|%s", opp_sockfd, opp_name);
								//送出取消讯息给Server通知对方
								lssproto_TD_send(sockfd, buffer);
								tradeStatus = 0;
								tradeInit();
								pc.trade_confirm = 1;
							}
							// 处理取消键 End						


							// 处理 Lock 、 确定键 Start
#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
							//主视窗与检视视窗共用Lock跟确定function
							LockAndOkfunction();
							// 处理 Lock 、 确定键 End

							//主副视窗切换钮
							if (HitDispNo == tradeWndFontNo[21])
							{
								MainTradeWndflag = false;
								SecondTradeWndflag = true;
								//andy_add 20030610
								for (i = 0; i < MENU_TRADE_0; i++) tradeWndFontNo[i] = -2;
								for (i = 0; i < 6; i++) SecondtradeWndFontNo[i] = -2;

							}
							//对方物品向上卷动钮
							if (HitDispNo == tradeWndFontNo[18])
							{
								if (drag1Y >= 67)
									drag1Y -= 4;
								if (drag1Y < 67)
									drag1Y = 67;
							}
							//对方物品向下卷动钮
							if (HitDispNo == tradeWndFontNo[19])
							{
								if (drag1Y <= 151)
									drag1Y += 4;
								if (drag1Y > 151)
									drag1Y = 151;
							}
							//我方物品向上卷动钮
							if (HitDispNo == tradeWndFontNo[15])
							{
								if (drag2Y >= 257)
									drag2Y -= 4;
								if (drag2Y < 257)
									drag2Y = 257;
							}
							//我方物品向下卷动钮
							if (HitDispNo == tradeWndFontNo[16])
							{
								if (drag2Y <= 341)
									drag2Y += 4;
								if (drag2Y > 341)
									drag2Y = 341;
							}
							//我方物品拖曳启动
							if (HitDispNo == tradeWndFontNo[20])
							{
								if (!dragflag1)
								{
									tmpdrag1Y = drag1Y;
									dragflag1 = true;
								}
							}
							//对方物品拖曳启动
							if (HitDispNo == tradeWndFontNo[17])
							{
								if (!dragflag2)
								{
									tmpdrag2Y = drag2Y;
									dragflag2 = true;
								}
							}
#ifdef _TRADETALKWND				// (不可开) Syu ADD 交易新增对话框架
							//拖曳钮
							if (HitDispNo == tradeWndFontNo[14])
								talkwndflag = true;
							//关闭钮

#endif
#endif						

						}
						// 以上为滑鼠左键被按下时 End


#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
						//我方拖曳钮界线限制
						if (dragflag1 == true &&
							(tmpdrag1Y - mouse.crickLeftDownPoint.y + mouse.nowPoint.y) <= 151 &&
							(tmpdrag1Y - mouse.crickLeftDownPoint.y + mouse.nowPoint.y) >= 67){
							drag1Y = tmpdrag1Y - mouse.crickLeftDownPoint.y + mouse.nowPoint.y;
						}

						//对方拖曳钮界线限制
						if (dragflag2 == true &&
							(tmpdrag2Y - mouse.crickLeftDownPoint.y + mouse.nowPoint.y) <= 341 &&
							(tmpdrag2Y - mouse.crickLeftDownPoint.y + mouse.nowPoint.y) >= 257){
							drag2Y = tmpdrag2Y - mouse.crickLeftDownPoint.y + mouse.nowPoint.y;
						}
#ifdef _TRADETALKWND				// (不可开) Syu ADD 交易新增对话框架
						//对话方框拖曳位移
						if (talkwndflag == true)
						{
							talkwndx = mouse.nowPoint.x;
							talkwndy = mouse.nowPoint.y;
						}
						if (mouse.onceState & MOUSE_RIGHT_CRICK)
							tradetalkwndflag = false;
#endif
#endif

						// 处理宠物选取左键 Start
						// 处理宠物选取左键
						if (HitDispNo == tradeWndFontNo[2])
						{
							if (mouse.onceState & MOUSE_LEFT_CRICK)
							{
								tradeWndBtnFlag[2] = TRUE;
								int i = 0;
								while (1)
								{
									tradePetIndex++;
									if (tradePetIndex >= 5) tradePetIndex = 0;
									if (pet[tradePetIndex].useFlag != NULL
										&& pc.ridePetNo != tradePetIndex)
										break;
									if ((i++) > 5)
									{
										tradePetIndex = -1;
										break;
									}
								}
								if (pActPet3 != NULL)
								{
									DeathAction(pActPet3);
									pActPet3 = NULL;
								}

							}
						}
						// 处理宠物选取左键 End

						// 处理宠物选取右键 Start
						// 处理宠物选取右键
						if (HitDispNo == tradeWndFontNo[3])
						{
							if (mouse.onceState & MOUSE_LEFT_CRICK)
							{
								tradeWndBtnFlag[3] = TRUE;
								int i = 0;
								while (1)
								{
									tradePetIndex--;
									if (tradePetIndex <0) tradePetIndex = 4;
									if (pet[tradePetIndex].useFlag != NULL
										&& pc.ridePetNo != tradePetIndex)
										break;
									if ((i++) >5)
									{
										tradePetIndex = -1;
										break;
									}
								}
								if (pActPet3 != NULL)
								{
									DeathAction(pActPet3);
									pActPet3 = NULL;
								}

							}
						}

						// 处理宠物选取右键 End

						// 处理金额增加键 Start
						if (HitDispNo == tradeWndFontNo[4])
						{
							if (mouse.onceState & MOUSE_LEFT_CRICK_UP && tradeWndBtnFlag[4] == TRUE)
								tradeWndBtnFlag[4] = FALSE;
							if (tradeWndBtnFlag[4] == TRUE)
							{
								// 按下增加时将Focus还给ChatBuffer
								GetKeyInputFocus(&MyChatBuffer);
								// 视窗消灭时还原设定
								Tradeflag = false;
								tradeWndDropGold += tradeWndDropGoldInc;
								tradeWndDropGoldCnt++;
								if (tradeWndDropGoldCnt >= 30){
									tradeWndDropGoldCnt = 0;
									if (tradeWndDropGoldInc == 0)
										tradeWndDropGoldInc = 1;
									else {
										tradeWndDropGoldInc *= 5;
										if (tradeWndDropGoldInc > 10000)
											tradeWndDropGoldInc = 10000;
									}
								}
								//金额上限确认
								if (tradeWndDropGold >= pc.gold)
								{
									tradeWndDropGold = pc.gold;
									play_se(220, 320, 240);
								}
							}
							if (mouse.onceState & MOUSE_LEFT_CRICK)
							{
								tradeWndDropGold++;
								if (tradeWndDropGold >= pc.gold)
								{
									tradeWndDropGold = pc.gold;
									play_se(220, 320, 240);
								}
								else
								{
									tradeWndDropGoldInc = 0;
									tradeWndDropGoldCnt = 0;
									tradeWndBtnFlag[4] = TRUE;
									play_se(217, 320, 240);
								}
							}
						}
						else
							tradeWndBtnFlag[4] = FALSE;
						// 处理金额增加键 End

						// 处理金额减少键 Start
						if (HitDispNo == tradeWndFontNo[5])
						{
							if (mouse.onceState & MOUSE_LEFT_CRICK_UP && tradeWndBtnFlag[5] == TRUE)
								tradeWndBtnFlag[5] = FALSE;
							if (tradeWndBtnFlag[5] == TRUE)
							{
								// 按下减少时将Focus还给ChatBuffer
								GetKeyInputFocus(&MyChatBuffer);
								// 视窗消灭时还原设定
								Tradeflag = false;
								tradeWndDropGold -= tradeWndDropGoldInc;
								tradeWndDropGoldCnt++;
								if (tradeWndDropGoldCnt >= 30)
								{
									tradeWndDropGoldCnt = 0;
									if (tradeWndDropGoldInc == 0)
									{
										tradeWndDropGoldInc = 1;
									}
									else
									{
										tradeWndDropGoldInc *= 5;
										if (tradeWndDropGoldInc > 10000)
											tradeWndDropGoldInc = 10000;
									}
								}
								if (tradeWndDropGold < 0)
								{
									tradeWndDropGold = 0;
									play_se(220, 320, 240);
								}
							}
							if (mouse.onceState & MOUSE_LEFT_CRICK) {
								tradeWndDropGold--;
								if (tradeWndDropGold <= 0)
								{
									tradeWndDropGold = 0;
									play_se(220, 320, 240);
								}
								else
								{
									tradeWndDropGoldInc = 0;
									tradeWndDropGoldCnt = 0;
									tradeWndBtnFlag[5] = TRUE;
									play_se(217, 320, 240);
								}
							}
						}
						else
							tradeWndBtnFlag[5] = FALSE;
						// 处理金额减少键 End

						// 处理金额放置键 Start
						if (HitDispNo == tradeWndFontNo[6])
						{
							if (mouse.onceState & MOUSE_LEFT_CRICK)
							{
								// 按下放置时将Focus还给ChatBuffer
								GetKeyInputFocus(&MyChatBuffer);
								// 视窗消灭时还原设定
								Tradeflag = false;
								if (tradeWndDropGold > 0 && eventWarpSendFlag == FALSE)
								{
									tradeWndBtnFlag[6] = TRUE;
									play_se(217, 320, 240);
								}
								else
									play_se(220, 320, 240);
							}
							if (mouse.onceState & MOUSE_LEFT_CRICK_UP && tradeWndBtnFlag[6] == TRUE)
							{
								if (tradeStatus != 1)	return;
								if (pc.trade_confirm == 2 || pc.trade_confirm == 4) return;
								tradeWndBtnFlag[6] = FALSE;
								int TradeGoldIndex = 0;

#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
								if ((tradeList[21].data == -1))
								{
									tradeList[21].data = tradeWndDropGold;
									tradeList[21].kind = 'G';
									TradeGoldIndex = 3;
								}
#endif

								if (TradeGoldIndex != 0)
								{
#ifdef _CHANGETRADERULE		   // (不可开) Syu ADD 交易规则修订
									TradeBtnflag = true;
#endif
									play_se(212, 320, 240);
									tradeWndDropGoldSend = tradeWndDropGold;
									sprintf_s(buffer, "T|%s|%s|G|%d|%d", opp_sockfd, opp_name, TradeGoldIndex, tradeWndDropGoldSend);
									lssproto_TD_send(sockfd, buffer);
									tradeWndDropGold = 0;
								}
							}
						}
						else
							tradeWndBtnFlag[6] = FALSE;
						// 处理金额放置键 End

						// 处理宠物放置键 Start		
						if (HitDispNo == tradeWndFontNo[7])
						{
#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
							tradeWndBtnFlag[7] = TRUE;
#endif
							if (mouse.onceState & MOUSE_LEFT_CRICK_UP)
							{
								if (tradeStatus != 1)	return;
								if (pc.trade_confirm == 2 || pc.trade_confirm == 4) return;

								if (pet[tradePetIndex].useFlag
									&&  pc.ridePetNo != tradePetIndex)
								{

									if (pet[tradePetIndex].freeName[0] != NULL)
										strcpy(tradePet[0].name, pet[tradePetIndex].freeName);
									else
										strcpy(tradePet[0].name, pet[tradePetIndex].name);
									tradePet[0].level = pet[tradePetIndex].level;
									tradePet[0].atk = pet[tradePetIndex].atk;
									tradePet[0].def = pet[tradePetIndex].def;
									tradePet[0].quick = pet[tradePetIndex].quick;
									tradePet[0].graNo = pet[tradePetIndex].graNo;
									tradePet[0].trn = pet[tradePetIndex].trn;
									tradePet[0].maxHp = pet[tradePetIndex].maxHp;

									tradePet[0].index = tradePetIndex;
#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
									pet[tradePetIndex].useFlag = NULL;
									if (pActPet3)
									{
										DeathAction(pActPet3);
										pActPet3 = NULL;
									}
#endif


									DeathAction(pActPet4);
									pActPet4 = NULL;
#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
									tradeList[tradePetIndex + 16].data = tradePetIndex;
									tradeList[tradePetIndex + 16].kind = 'P';
									strcpy(tradeList[tradePetIndex + 16].name, pet[tradePetIndex].name);
									tradeList[tradePetIndex + 16].level = pet[tradePetIndex].level;
									tradeList[tradePetIndex + 16].trns = pet[tradePetIndex].trn;
#ifdef _SHOW_FUSION
									tradeList[tradePetIndex + 16].fusion = pet[tradePetIndex].fusion;
#endif									

									if (pet[tradePetIndex].freeName[0] != NULL)
										strcpy(tradeList[tradePetIndex + 16].freename, pet[tradePetIndex].freeName);
									else
										strcpy(tradeList[tradePetIndex + 16].freename, "");// change fix 防止未改名的宠物显示出上一个丢掉宠物的改名bug
#endif

									if (tradeStatus)
									{
#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
										//增加送出欲交易宠物的技能、原名、更改名
										sprintf_s(buffer2, "%s|%s|%s|%s|%s|%s|%s|%s|%s",
											petSkill[tradePetIndex][0].name,
											petSkill[tradePetIndex][1].name,
											petSkill[tradePetIndex][2].name,
											petSkill[tradePetIndex][3].name,
											petSkill[tradePetIndex][4].name,
											petSkill[tradePetIndex][5].name,
											petSkill[tradePetIndex][6].name,
											pet[tradePetIndex].name,
											pet[tradePetIndex].freeName
											);
										sprintf_s(buffer, "T|%s|%s|P|3|%d|%s", opp_sockfd, opp_name, tradePetIndex, buffer2);
#endif
										lssproto_TD_send(sockfd, buffer);
									}
								}
							}
						}
						// 处理宠物放置键 End

#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
						//滑鼠左键放掉取消所有拖曳旗标
						if (mouse.onceState & MOUSE_LEFT_CRICK_UP)
						{
							dragflag1 = false;
							dragflag2 = false;
#ifdef _TRADETALKWND				// (不可开) Syu ADD 交易新增对话框架
							talkwndflag = false;
#endif
						}
#endif
						//未被按下的钮全部还原
						for (i = 2; i <= 8; i++)
						{
							if (mouse.state & MOUSE_LEFT_CRICK && tradeWndBtnFlag[i] == TRUE)
								tradeWndBtnFlag[i] = TRUE;
							else
								tradeWndBtnFlag[i] = FALSE;
						}
						// 产生主视窗各零件
						if (pActMenuWnd4 != NULL)
						{
							// 取得视窗基准X , Y座标
							x = pActMenuWnd4->x;
							y = pActMenuWnd4->y;
							// 个人金额最大值显示

#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
#ifdef _TRADETALKWND				// (不可开) Syu ADD 交易新增对话框架
							//对话框内容
							if (tradetalkwndflag != false)
							{
								for (i = 3; i >= 0; i--)
								{
									sprintf_s(moji, "%s", talkmsg[i]);
									StockFontBuffer(talkwndx - 154, talkwndy - 26 + i * 20, FONT_PRIO_AFRONT, FONT_PAL_YELLOW, moji, 0);
								}
								tradeWndFontNo[14] = StockDispBuffer(talkwndx, talkwndy, DISP_PRIO_BOX3, CG_TRADE_TALK_WND, 2);
							}
#endif						
							int ShowPoint;
							int j = 0;
							//对方的交易清单呈现开始位置  
							ShowPoint = (drag2Y - 257) / 4;
							//建立我方List清单    
							for (i = 0; i < 21; i++)				//修正交易列表  xiezi
							{
								if (tradeList[i + 1].data != -1)
								{
									mytradelist[j] = i + 1;
									j++;
								}
							}
							j = 0;
							//建立对方List清单
							for (i = 21; i < 42; i++)				//修正交易列表  xiezi
							{
								if (tradeList[i + 1].data != -1)
								{
									opptradelist[j] = i + 1;
									j++;
								}
							}
							j = 0;
							//显示我方交易清单      
							for (i = ShowPoint; i < ShowPoint + 5 && i < 21; i++)   //交易清单修正 xiezi
							{
								if (mytradelist[i] == -1) break;
								//说明框用的HitBox暂时不做
								//								if( MakeHitBox( x + 10 , y + 226 + j * 29, x + 280  , y + 244 + j * 29 , DISP_PRIO_IME3 ) == TRUE ){
								//								}
								sprintf_s(moji, "%c", tradeList[mytradelist[i]].kind);
								//显示道具
								if (strcmp(moji, "I") == 0)
								{
									sprintf_s(moji, "%s", tradeList[mytradelist[i]].name);
									StockFontBuffer(x + 12, y + 228 + j * 29, FONT_PRIO_FRONT, 0, moji, 0);

#ifdef _ITEM_PILENUMS
									sprintf_s(moji, "x%d", tradeList[mytradelist[i]].pilenum);
									StockFontBuffer(x + 180, y + 228 + j * 29, FONT_PRIO_FRONT, 0, moji, 0);
#endif
#ifdef _NPC_ITEMUP
									//if( MakeHitBox( x + 10 , y + 226 + j * 29, x + 280  , y + 244 + j * 29 , DISP_PRIO_IME3 ) == TRUE )
									//	ShowItemup(tradeList[mytradelist[i]].itemup,x+160,y + 246 + j * 29);
#endif					
									sprintf_s(moji, "%s", tradeList[mytradelist[i]].damage);
									StockFontBuffer(x + 220, y + 228 + j * 29, FONT_PRIO_FRONT, 0, moji, 0);
									j++;
								}
								//显示宠物
								else if (strcmp(moji, "P") == 0)
								{
									int colors = 0;
									int mylist = mytradelist[i];
									sprintf_s(moji, "%s", tradeList[mylist].name);
									StockFontBuffer(x + 12, y + 228 + j * 29, FONT_PRIO_FRONT, 0, moji, 0);

									if (strcmp(tradeList[mylist].freename, tradeList[mylist].name))
									{
										sprintf_s(moji, "[%s]", tradeList[mylist].freename);
										colors = FONT_PAL_RED;
										StockFontBuffer(x + 102, y + 228 + j * 29, FONT_PRIO_FRONT, colors, moji, 0);
									}
#ifdef _PET_2TRANS
									sprintf_s( moji , "Lv:%d%s" , tradeList[mylist].level, "");
									if (tradeList[mylist].trns == 1)
										sprintf_s(moji, "Lv:%d%s", tradeList[mylist].level, "一转");
									else if (tradeList[mylist].trns == 2)
										sprintf_s(moji, "Lv:%d%s", tradeList[mylist].level, "二转");
#ifdef _SHOW_FUSION
									if (LOWORD(tradeList[mylist].fusion) == 1)
										sprintf_s(moji, "Lv:%d%s", tradeList[mylist].level, "融合");
#endif
#else
									sprintf_s(moji, "Lv:%d%s", tradeList[mylist].level, (tradeList[mylist].trns == 0) ? "" : "转");
#endif
									StockFontBuffer(x + 220, y + 228 + j * 29, FONT_PRIO_FRONT, 0, moji, 0);

									j++;
								}
								//显示金钱
								else if (strcmp(moji, "G") == 0)
								{
									StockFontBuffer(x + 12, y + 228 + j * 29, FONT_PRIO_FRONT, 0, "石币", 0);
									sprintf_s(moji, "%d", tradeList[mytradelist[i]].data);
									StockFontBuffer(x + 102, y + 228 + j * 29, FONT_PRIO_FRONT, 0, moji, 0);
									sprintf_s(moji, "%s", "Gold");
									StockFontBuffer(x + 220, y + 228 + j * 29, FONT_PRIO_FRONT, 0, moji, 0);
									j++;
								}
								if (j == 5)
									break;
							}
							j = 0;
							//对方的交易清单呈现开始位置       
							ShowPoint = (drag1Y - 67) / 4;
							//显示对方交易清单
							for (i = ShowPoint; i < ShowPoint + 5 && i < 21; i++)   //交易清单修正 xiezi
							{
								if (opptradelist[i] == -1) break;
								//说明框用的HitBox暂时不做
								//								if( MakeHitBox( x + 10 , y + 37 + j * 29, x + 280  , y + 55 + j * 29 , DISP_PRIO_IME3 ) == TRUE ){
								//								}
								sprintf_s(moji, "%c", tradeList[opptradelist[i]].kind);
								//显示道具
								if (strcmp(moji, "I") == 0)
								{
									int colors = 0;
									int opptradlist = opptradelist[i];
									// Terry modify for 都用原名 2004/6/9
									sprintf_s(moji, "%s", tradeList[opptradlist].name);
									//									sprintf_s( moji , "%s" , tradeList[ opptradlist].freename ) ; 
									//									if( strcmp( tradeList[opptradlist].name, tradeList[opptradlist].freename ) ){
									//										sprintf_s( moji , "[%s]" , tradeList[ opptradlist].freename ) ; 
									//										colors = 6;
									//									}
									StockFontBuffer(x + 12, y + 40 + j * 29, FONT_PRIO_FRONT, colors, moji, 0);
#ifdef _ITEM_PILENUMS
									sprintf_s(moji, "x%d", tradeList[opptradelist[i]].pilenum);
									StockFontBuffer(x + 180, y + 40 + j * 29, FONT_PRIO_FRONT, 0, moji, 0);
#endif
#ifdef _NPC_ITEMUP 
									//if( MakeHitBox( x + 10 , y + 37 + j * 29, x + 280  , y + 55 + j * 29 , DISP_PRIO_IME3 ) == TRUE )
									//	ShowItemup(tradeList[opptradelist[i - 22]].itemup,x+160,y + 57 + j * 29);
#endif								

									sprintf_s(moji, "%s", tradeList[opptradelist[i]].damage);
									StockFontBuffer(x + 220, y + 40 + j * 29, FONT_PRIO_FRONT, 0, moji, 0);
									j++;
								}
								else if (strcmp(moji, "P") == 0)
								{//显示宠物
									int colors = 0; //FONT_PAL_RED
									int opplist = opptradelist[i];
									sprintf_s(moji, "%s", tradeList[opplist].name);
									StockFontBuffer(x + 12, y + 40 + j * 29, FONT_PRIO_FRONT, 0, moji, 0);
#ifdef _SHOW_FUSION									
									// change fix 只为了颢示-----> []
									if (strcmp(tradeList[opplist].freename, tradeList[opplist].name) == 0)
									{
										sprintf_s(moji, "[]");
										colors = FONT_PAL_RED;
										StockFontBuffer(x + 102, y + 40 + j * 29, FONT_PRIO_FRONT, colors, moji, 0);
									}
#endif
									if (strcmp(tradeList[opplist].freename, tradeList[opplist].name))
									{
										sprintf_s(moji, "[%s]", tradeList[opplist].freename);
										colors = FONT_PAL_RED;
										StockFontBuffer(x + 102, y + 40 + j * 29, FONT_PRIO_FRONT, colors, moji, 0);
									}
#ifdef _PET_2TRANS
									sprintf_s( moji , "Lv:%d%s" , tradeList[opplist].level, (tradeList[opplist].trns==0)?"":"");
									if (tradeList[opplist].trns == 1)
										sprintf_s(moji, "Lv:%d%s", tradeList[opplist].level, "一转");
									else if (tradeList[opplist].trns == 2)
										sprintf_s(moji, "Lv:%d%s", tradeList[opplist].level, "二转");
#ifdef _SHOW_FUSION
									if (LOWORD(tradeList[opplist].fusion) == 1)
										sprintf_s(moji, "Lv:%d%s", tradeList[opplist].level, "融合");
#endif
#else
									sprintf_s(moji, "Lv:%d%s", tradeList[opplist].level, (tradeList[opplist].trns == 0) ? "" : "转");
#endif
									StockFontBuffer(x + 220, y + 40 + j * 29, FONT_PRIO_FRONT, 0, moji, 0);
									j++;
								}
								//显示金钱
								else if (strcmp(moji, "G") == 0)
								{
									StockFontBuffer(x + 12, y + 40 + j * 29, FONT_PRIO_FRONT, 0, "石币", 0);
									sprintf_s(moji, "%d", tradeList[opptradelist[i]].data);
									StockFontBuffer(x + 102, y + 40 + j * 29, FONT_PRIO_FRONT, 0, moji, 0);
									sprintf_s(moji, "%s", "Gold");
									StockFontBuffer(x + 220, y + 40 + j * 29, FONT_PRIO_FRONT, 0, moji, 0);

									j++;
								}
								if (j == 5)
									break;
							}
#endif
							sprintf_s(moji, "%7d", pc.gold);
							StockFontBuffer(x + 550, y + 65, FONT_PRIO_FRONT, 0, moji, 0);
							// Focus不在手动输入时显示原数值
							if (Tradeflag == false)
							{
								sprintf_s(moji, "%7d", tradeWndDropGold);
								StockFontBuffer(x + 550, y + 120, FONT_PRIO_FRONT, 0, moji, 0);
							}
							if (pet[tradePetIndex].useFlag
								&& pc.ridePetNo != tradePetIndex)
							{
								if (pActPet3 == NULL)
								{
#ifdef _LIZARDPOSITION			   // (可开放) Syu ADD 修正龙蜥显示位置过低
									if ((pet[tradePetIndex].graNo == 101493) || (pet[tradePetIndex].graNo == 101494) ||
										(pet[tradePetIndex].graNo == 101495) || (pet[tradePetIndex].graNo == 101496))
									{
										pActPet3 = MakeAnimDisp(480, 230, pet[tradePetIndex].graNo, ANIM_DISP_PET);
									}
									else
										pActPet3 = MakeAnimDisp(480, 230, pet[tradePetIndex].graNo, ANIM_DISP_PET);
#else 
									pActPet3 = MakeAnimDisp(400, /*175*/145, pet[tradePetIndex].graNo, ANIM_DISP_PET);
#endif
								}

								if (pet[tradePetIndex].freeName[0] != NULL)
									CenteringStr(pet[tradePetIndex].freeName, moji, PET_NAME_LEN);
								else
									CenteringStr(pet[tradePetIndex].name, moji, PET_NAME_LEN);

								//有转生时颜色显示蓝色
#ifdef _TRADESYSTEM2			// (不可开) Syu ADD 新交易系统
								//新系统位置偏移修正
								if (pet[tradePetIndex].trn == 1)
									StockFontBuffer(x + 330, y + 33, FONT_PRIO_FRONT, 1, moji, 0);
								else
									StockFontBuffer(x + 330, y + 33, FONT_PRIO_FRONT, 0, moji, 0);

								sprintf_s(moji, "%3d", pet[tradePetIndex].level);
								StockFontBuffer(x + 485, y + 105, FONT_PRIO_FRONT, 0, moji, 0);
								sprintf_s(moji, "%3d", pet[tradePetIndex].atk);
								StockFontBuffer(x + 485, y + 129, FONT_PRIO_FRONT, 0, moji, 0);
								sprintf_s(moji, "%3d", pet[tradePetIndex].def);
								StockFontBuffer(x + 485, y + 153, FONT_PRIO_FRONT, 0, moji, 0);
								sprintf_s(moji, "%3d", pet[tradePetIndex].quick);
								StockFontBuffer(x + 485, y + 177, FONT_PRIO_FRONT, 0, moji, 0);
								sprintf_s(moji, "%3d", pet[tradePetIndex].maxHp);
								StockFontBuffer(x + 485, y + 201, FONT_PRIO_FRONT, 0, moji, 0);
#endif
							}


#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
							// 以交易进行到的状态决定Button样式
							if (pc.trade_confirm == 1 || pc.trade_confirm == 3)
								tradeWndFontNo[0] = StockDispBuffer(x + 59, y + 390, DISP_PRIO_IME3, CG_TRADE_LOCK_BTN, 2);
							if (pc.trade_confirm == 4)
								tradeWndFontNo[0] = StockDispBuffer(x + 59, y + 390, DISP_PRIO_IME3, CG_TRADE_OK_BTN, 2);
							//偏移
							tradeWndFontNo[1] = StockDispBuffer(x + 240, y + 390, DISP_PRIO_IME3, CG_TRADE_CANCEL_BTN, 2);
#endif

							tradeWndFontNo[2] = StockDispBuffer(x + 452 + 20, y + 63 + 8, DISP_PRIO_IME3, CG_TRADE_LEFT_BTN_UP + tradeWndBtnFlag[2], 2);
							tradeWndFontNo[3] = StockDispBuffer(x + 486 + 20, y + 63 + 8, DISP_PRIO_IME3, CG_TRADE_RIGHT_BTN_UP + tradeWndBtnFlag[3], 2);
							tradeWndFontNo[4] = StockDispBuffer(x + 554 - 94, y + 93 + 106, DISP_PRIO_IME3, CG_TRADE_UP_BTN_UP + tradeWndBtnFlag[4], 2);
							tradeWndFontNo[5] = StockDispBuffer(x + 560 - 94, y + 93 + 106, DISP_PRIO_IME3, CG_TRADE_DOWN_BTN_UP + tradeWndBtnFlag[5], 2);
#ifdef _CHANGETRADERULE		   // (不可开) Syu ADD 交易规则修订
							if (TradeBtnflag == false)
								tradeWndFontNo[6] = StockDispBuffer(x + 562 - 62 + 25, y + 148 + 108 + 8, DISP_PRIO_IME3, CG_TRADE_PUT_BTN_UP + tradeWndBtnFlag[6], 2);
#else
							tradeWndFontNo[6] = StockDispBuffer(x + 562 - 62 + 25, y + 148 + 108 + 8, DISP_PRIO_IME3, CG_TRADE_PUT_BTN_UP + tradeWndBtnFlag[6], 2);
#endif
#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
							//偏移
							tradeWndFontNo[7] = StockDispBuffer(x + 365 - 62 + 25, y + 190 + 108 + 8, DISP_PRIO_IME3, CG_TRADE_PUT_BTN_UP + tradeWndBtnFlag[7], 2);
#endif
#ifndef _CHANGETRADERULE		   // (不可开) Syu ADD 交易规则修订
							tradeWndFontNo[8] = StockDispBuffer(x + 55 + 25, y + 190 + 18, DISP_PRIO_IME3, CG_MAIL_WND_CLEAR_BTN_UP + tradeWndBtnFlag[8], 2);
#endif

#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
							//偏移
							//交易显示双方名称
							StockFontBuffer(x + 5, y + 193, FONT_PRIO_FRONT, FONT_PAL_PURPLE, pc.name, 0);
							StockFontBuffer(x + 5, y + 6, FONT_PRIO_FRONT, FONT_PAL_PURPLE, opp_name, 0);
							//检视视窗钮
							tradeWndFontNo[21] = StockDispBuffer(x + 150, y + 390, DISP_PRIO_IME3, CG_TRADE_VIEW_BTN, 2);
							//我方卷轴拖曳钮
							tradeWndFontNo[18] = StockDispBuffer(x + 302, y + 42, DISP_PRIO_IME3, CG_TRADE_SCROLL_UP, 2);
							//我方卷轴上移钮
							tradeWndFontNo[19] = StockDispBuffer(x + 302, y + 176, DISP_PRIO_IME3, CG_TRADE_SCROLL_DOWN, 2);
							//我方卷轴下移钮
							tradeWndFontNo[20] = StockDispBuffer(x + 302, y + drag1Y, DISP_PRIO_IME3, CG_TRADE_SCROLL_BTN, 2);
							//对方卷轴拖曳钮
							tradeWndFontNo[15] = StockDispBuffer(x + 302, y + 232, DISP_PRIO_IME3, CG_TRADE_SCROLL_UP, 2);
							//对方卷轴上移钮
							tradeWndFontNo[16] = StockDispBuffer(x + 302, y + 366, DISP_PRIO_IME3, CG_TRADE_SCROLL_DOWN, 2);
							//对方卷轴下移钮
							tradeWndFontNo[17] = StockDispBuffer(x + 302, y + drag2Y, DISP_PRIO_IME3, CG_TRADE_SCROLL_BTN, 2);
#endif

							// 假设游标在金额位置时
							if (MakeHitBox(x + 530, y + 115, x + +530 + 86, y + 115 + 25, DISP_PRIO_IME3) == TRUE)
							{
								// 按下滑鼠取得focus及初始化
								if (mouse.onceState & MOUSE_LEFT_CRICK)
								{
									strcpy(TradeBuffer.buffer, "");
									TradeBuffer.buffer[0] = NULL;
									TradeBuffer.cursor = 0;
									TradeBuffer.cnt = 0;
									Tradeflag = true;
									GetKeyInputFocus(&TradeBuffer);
									play_se(217, 320, 240);
								}
							}
							// focus在手动输入时显示buffer
							if (Tradeflag == true)
							{
								TradeBuffer.x = pActMenuWnd4->x + 600 - strlen(TradeBuffer.buffer) * 7;
								TradeBuffer.y = pActMenuWnd4->y + 120;
								StockFontBuffer2(&TradeBuffer);
								tradeWndDropGold = atoi(TradeBuffer.buffer);
								if (atoi(TradeBuffer.buffer) >= pc.gold)
								{
									sprintf_s(TradeBuffer.buffer, "%d", pc.gold);
									tradeWndDropGold = pc.gold;
								}
							}

							if (pc.trade_confirm == 2)
							{
#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
								//偏移
								StockFontBuffer(x + 220, y + 194, FONT_PRIO_FRONT, FONT_PAL_RED, "锁定交易", 0);
							}
							if (pc.trade_confirm == 3)
							{
								StockFontBuffer(x + 210, y + 194, FONT_PRIO_FRONT, FONT_PAL_RED, "对方锁定交易", 0);
							}
							if (pc.trade_confirm == 4)
							{
								if (tradeStatus == 2)
									StockFontBuffer(x + 220, y + 194, FONT_PRIO_FRONT, FONT_PAL_RED, "交易确认", 0);
								else
									StockFontBuffer(x + 210, y + 194, FONT_PRIO_FRONT, FONT_PAL_RED, "最后交易确认", 0);
							}
#endif
						}
#ifdef _NEW_ITEM_
						for (i = 0; i < 3; i++){
							if (i == 道具栏页数){
								StockDispBuffer(722, 335 + i * 56, DISP_PRIO_IME2, 55223 + i, 1);
							}
							else{
								BOOL flg = FALSE;
								if (i){
									if (pc.道具栏状态 & 1 << i){
										flg = TRUE;
									}
								}
								else flg = TRUE;
								if (flg){
									StockDispBuffer(727 - 11, 335 + i * 56, DISP_PRIO_IME2, 55226 + i, 1);
									if (MakeHitBox(717 - 11, 307 + i * 56, 717 + 20 - 11, 304 + i * 56 + 60, DISP_PRIO_IME4)){
										if (mouse.onceState & MOUSE_LEFT_CRICK){
											道具栏页数 = i;
										}
									}
								}
								else StockDispBuffer(727 - 11, 335 + i * 56, DISP_PRIO_IME2, 55229 + i, 1);
							}
						}
#endif
						for (i = MAX_ITEM - 1; i >= MAX_ITEMSTART; i--)
						{
#ifdef _NEW_ITEM_
							int 道具起始 = MAX_ITEMSTART + MAX_MAXHAVEITEM*道具栏页数;
							int 道具结束 = 道具起始 + MAX_MAXHAVEITEM;
							if (i >= MAX_ITEMSTART){
								if (i < 道具起始 || i >= 道具结束) continue;
							}
#endif
#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
							//已选取交易的盖杖印章
#ifdef _ITEM_PILENUMS
							if (itemflag[i - MAX_ITEMSTART] <= 0)
							{
#else
							if (itemflag[i - MAX_ITEMSTART] == 1)
							{
#endif
								//原本为显示原道具并盖上章
								//StockDispBuffer( ItemBuffer[ i ].defX, ItemBuffer[ i ].defY, ItemBuffer[ i ].dispPrio, CG_TRADE_SEAL , 0 );
								//修正为道具隐藏，直接跳过不做后续显示

								continue;

							}
#endif

							if (MakeHitBox(x + ItemBuffer[i].defX - 35, y + ItemBuffer[i].defY - 22,
								x + ItemBuffer[i].defX + 15, y + ItemBuffer[i].defY + 28, DISP_PRIO_IME3) == TRUE)
							{
#ifdef _TELLCHANNEL
								// Terry fix 2003/12/16 for 交易视窗开启时,显示物品说明不显示输入法
								//TaskBarFlag = TRUE;这行不要
								bShowItemExplain = TRUE; // 新增这行
								// end
#endif
								// 该栏位有道具的情况
								if (pc.item[i].useFlag == TRUE && ItemBuffer[i].mixFlag <= 2)
								{
									char *splitPoint = pc.item[i].memo;
									int color = pc.item[i].color;
									// 交易物品能否使用Check
									if (pc.transmigration == 0 && pc.level < pc.item[i].level)
										color = FONT_PAL_RED;
									// 道具名称显示
									StockFontBuffer(0, 460 + DISPLACEMENT_Y, FONT_PRIO_FRONT, color, pc.item[i].name, 0);
									// 道具说明
									StockFontBuffer(160, 460 + DISPLACEMENT_Y, FONT_PRIO_FRONT, 0, splitPoint, 0);
#ifdef _NPC_ITEMUP
									ShowItemup(pc.item[i].itemup, mouse.nowPoint.x, mouse.nowPoint.y);
#endif
								}
								// 在道具上点两下的情况
								if (mouse.onceState & MOUSE_LEFT_DBL_CRICK)
								{
									if (tradeStatus != 1)	return;
									if (pc.trade_confirm == 2 || pc.trade_confirm == 4) return;
									// 合成旗标关闭
									ItemBuffer[i].mixFlag = FALSE;
									// 确认该位置有道具
									if (pc.item[i].useFlag == TRUE){
										ItemBuffer[i].dragFlag = TRUE;
										mouse.itemNo = i;
										// 道具选择初期化
										itemNo = -1;
									}
#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
									if (mouse.itemNo != -1)
									{
										//道具栏页数
										char buf[1024];

										int chkindex = 0, frontempIndex = 0;
										//检查道具tradeList空间是否已满  修正  xiezi
										for (int scanindex = 1; scanindex < 16; scanindex++)
										{
											if (tradeList[chkindex].data == i)
											{
												chkindex = scanindex;
												break;
											}
											if (tradeList[scanindex].data == -1 && frontempIndex < 1)
											{
												frontempIndex = scanindex;
											}
										}
										chkindex = chkindex > 0 ? chkindex : frontempIndex;

										if (chkindex > 0)
										{
											tradeList[chkindex].kind = 'I';
											tradeList[chkindex].data = i;
											strcpy(tradeList[chkindex].name, pc.item[i].name);
											strcpy(tradeList[chkindex].damage, pc.item[i].damage);
#ifdef _ITEM_PILENUMS
											itemflag[i - MAX_ITEMSTART]--;
#else
											itemflag[i - MAX_ITEMSTART] = 1;
#endif
#ifdef _ITEM_PILENUMS
											tradeList[chkindex].pilenum = pc.item[i].pile - itemflag[i - MAX_ITEMSTART];
#endif
#ifdef _NPC_ITEMUP 
											tradeList[i - 4].itemup = pc.item[i].itemup;
#endif
											sprintf_s(buf, "T|%s|%s|I|1|%d", opp_sockfd, opp_name, i);
											lssproto_TD_send(sockfd, buf);
										}
									}
#endif
									mouse.itemNo = -1;
								}
							}
							if (pc.item[i].useFlag == TRUE)
							{
								// 非合成物品的情况
								if (ItemBuffer[i].mixFlag <= 2) {
									char buf[256];
									// 道具显示
									StockDispBuffer(x + ItemBuffer[i].defX - 8, y + ItemBuffer[i].defY, ItemBuffer[i].dispPrio, pc.item[i].graNo, 0);


#ifdef _ITEM_PILENUMS
									sprintf_s(buf, "%d", itemflag[i - MAX_ITEMSTART]);
									StockFontBuffer(x + ItemBuffer[i].defX - 8, y + ItemBuffer[i].defY, FONT_PRIO_FRONT, FONT_PAL_RED, buf, 0);
#endif

								}
								else if (ItemBuffer[i].mixFlag == 10)
									StockDispBuffer(x + ItemBuffer[i].defX - 8, y + ItemBuffer[i].defY, ItemBuffer[i].dispPrio, pc.item[i].graNo, 0);
								else if (ItemBuffer[i].mixFlag == 11)
									StockDispBuffer(x + ItemBuffer[i].defX - 8, y + ItemBuffer[i].defY, ItemBuffer[i].dispPrio, pc.item[i].graNo, 0);
								else if (ItemBuffer[i].mixFlag == 3)
									StockDispBuffer(pActPet2->x, pActPet2->y - 16, DISP_PRIO_DRAG, pc.item[i].graNo, 0);
								else if (ItemBuffer[i].mixFlag == 4)
									StockDispBuffer(pActPet2->x, pActPet2->y, ItemBuffer[i].dispPrio, pc.item[i].graNo, 0);

							}

						}
					}
#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
				}
				// 检视视窗内容
				else  if (SecondTradeWndflag == true)
				{
					MainTradeWndflag = false;
					if (pActMenuWnd4 != NULL && SecondTradeWnd != NULL)
					{
						x = SecondTradeWnd->x;
						y = SecondTradeWnd->y;
#ifdef _TRADETALKWND				// (不可开) Syu ADD 交易新增对话框架
						//对话框内容
						if (tradetalkwndflag != false)
						{
							for (i = 3; i >= 0; i--)
							{
								sprintf_s(moji, "%s", talkmsg[i]);
								StockFontBuffer(talkwndx - 154, talkwndy - 26 + i * 20, FONT_PRIO_AFRONT, FONT_PAL_YELLOW, moji, 0);
							}
							SecondtradeWndFontNo[4] = StockDispBuffer(talkwndx, talkwndy, DISP_PRIO_BOX3, CG_TRADE_TALK_WND, 2);
						}
#endif						
						SecondtradeWndFontNo[0] = StockDispBuffer(x + 254, y + 380, DISP_PRIO_IME3, CG_TRADE_CANCEL_BTN, 2);
						SecondtradeWndFontNo[1] = StockDispBuffer(x + 159, y + 380, DISP_PRIO_IME3, CG_TRADE_BACK_BTN, 2);
						//交易进行状态
						if (pc.trade_confirm == 1 || pc.trade_confirm == 3)
							SecondtradeWndFontNo[2] = StockDispBuffer(x + 63, y + 380, DISP_PRIO_IME3, CG_TRADE_LOCK_BTN, 2);
						if (pc.trade_confirm == 4)
							SecondtradeWndFontNo[2] = StockDispBuffer(x + 63, y + 380, DISP_PRIO_IME3, CG_TRADE_OK_BTN, 2);

						sprintf_s(moji, "%7d", tradeWndDropGoldGet);

#ifdef _PET_ITEM
						// 显示宠物装备资料
						if (locknum != -1 && tradeList[locknum + 37].data != -1)
						{
							if (bViewPetEquip)
								SecondtradeWndFontNo[5] = StockDispBuffer(x + 259, y + 310, DISP_PRIO_IME3, 26472, 2);
							else
								SecondtradeWndFontNo[5] = StockDispBuffer(x + 259, y + 310, DISP_PRIO_IME3, 26471, 2);
						}
#endif

						//显示金钱
						if (tradeWndDropGoldGet > 0)
							StockFontBuffer(x + 95, y + 336, FONT_PRIO_FRONT, 0, moji, 0);
						for (i = 0; i < 5; i++)
						{
							if (MakeHitBox(x + 320, y + 12 + i * 42, x + 600, y + 51 + i * 42, DISP_PRIO_IME3) == TRUE)
							{
#ifdef _TRADELOCKBTN				// (不可开) Syu ADD 增加锁定键
								if (mouse.onceState & MOUSE_LEFT_CRICK)
								{
#ifdef _PET_ITEM
									bViewPetEquip = FALSE;
									HitDispNo = 0;
#endif
									locknum = i;
									locknum2 = -1;
								}
								if (locknum == -1)
								{
									locknum = i;
									locknum2 = -1;
								}
							}
							if (locknum != -1)
							{
								if (tradeList[locknum + 37].data != -1)
								{
#ifdef _PET_ITEM
									if (bViewPetEquip)
									{
										int		iY = 251, iColor;
										char	*splitPoint;

										// 显示宠物装备栏的底图及装备
										for (int iCount = 0; iCount < PET_EQUIPNUM; iCount++)
										{	
											StockDispBuffer(ItemBuffer[iCount].defX, ItemBuffer[iCount].defY, DISP_PRIO_IME3, nPetItemEquipBmpNumber[iCount][0], 0);
											if (opp_pet[locknum].oPetItemInfo[iCount].bmpNo > 0)
												StockDispBuffer(ItemBuffer[iCount].defX, ItemBuffer[iCount].defY, DISP_PRIO_ITEM, opp_pet[locknum].oPetItemInfo[iCount].bmpNo, 0);
										}
										for (int iCount = 0; iCount < PET_EQUIPNUM; iCount++)
										{
											// 显示装备内容
											if (MakeHitBox( ItemBuffer[iCount].defX - 26, ItemBuffer[iCount].defY - 26,
												ItemBuffer[iCount].defX + 26, ItemBuffer[iCount].defY + 23, DISP_PRIO_IME3) == TRUE)
											{
												if (opp_pet[locknum].oPetItemInfo[iCount].bmpNo > 0)
												{
													iColor = opp_pet[locknum].oPetItemInfo[iCount].color;
													splitPoint = opp_pet[locknum].oPetItemInfo[iCount].memo;
													// 装备名称
													StockFontBuffer(x + 25, y + 195, FONT_PRIO_FRONT, iColor, opp_pet[locknum].oPetItemInfo[iCount].name,0);
													// 耐久度
													sprintf_s(moji, "耐久度(%s)", opp_pet[locknum].oPetItemInfo[iCount].damage);
													StockFontBuffer(x + 25, y + 215, FONT_PRIO_FRONT, iColor, moji, 0);

													// 显示道具叙述
													while (1)
													{
														if (strlen(splitPoint) > 28)
														{
															sprintf_s(moji, 28, "%s", splitPoint);
															moji[28] = NULL;
															if (GetStrLastByte(moji) == 3)
															{
																moji[27] = NULL;
																splitPoint += 27;
															}
															else
															{
																moji[28] = NULL;
																splitPoint += 28;
															}
															StockFontBuffer(x + 25, iY, FONT_PRIO_FRONT, 0, moji, 0);
															iY += 24;
														}
														else
														{
															strcpy(moji, splitPoint);
															StockFontBuffer(x + 25, iY, FONT_PRIO_FRONT, 0, moji, 0);
															break;
														}
													}
												}
											}
										}
									}
									else
#endif
									{
										//产生宠物Action
										if (SecondActPet == NULL) {
											ShowPetNum = atoi(opp_pet[locknum].opp_petgrano);
											SecondActPet = MakeAnimDisp(x + 215, y + 130, ShowPetNum, ANIM_DISP_PET);
										}
										//已存在显示的宠物
										else if (SecondActPet != NULL && ShowPetNum != atoi(opp_pet[locknum].opp_petgrano)) {
											DeathAction(SecondActPet);
											SecondActPet = NULL;
											ShowPetNum = atoi(opp_pet[locknum].opp_petgrano);
											SecondActPet = MakeAnimDisp(x + 215, y + 130, atoi(opp_pet[locknum].opp_petgrano), ANIM_DISP_PET);
										}

										//opp_petfreename
										//显示数值、技能 ( 左方 )
										StockDispBuffer(x + 95, y + 40, DISP_PRIO_IME3, CG_TRADE_LINE, 0);
										sprintf_s(moji, "%s", opp_pet[locknum].opp_petname);
										StockFontBuffer(x + 55, y + 30, FONT_PRIO_FRONT, 0, moji, 0);

										if (strcmp(opp_pet[locknum].opp_petfreename, opp_pet[locknum].opp_petname)){
											sprintf_s(moji, "[%s]", opp_pet[locknum].opp_petfreename);
											StockFontBuffer(x + 155, y + 30, FONT_PRIO_FRONT, 6, moji, 0);
										}

										StockDispBuffer(x + 55, y + 70, DISP_PRIO_IME3, CG_TRADE_LV_LINE, 0);

										sprintf_s(moji, "%3d", atoi(opp_pet[locknum].opp_petlevel));
										StockFontBuffer(x + 55, y + 62, FONT_PRIO_FRONT, 0, moji, 0);

#ifdef _PET_2TRANS
										sprintf_s( moji,"%s", "" );
										if (atoi(opp_pet[locknum].opp_pettrans) == 1)
											sprintf_s(moji, "%s", "一转");
										else if (atoi(opp_pet[locknum].opp_pettrans) == 2)
											sprintf_s(moji, "%s", "二转");
#ifdef _SHOW_FUSION
										if (LOWORD(atoi(opp_pet[locknum].opp_fusion)) == 1)
											sprintf_s(moji, "%s", "融合");
#endif
#else
										sprintf_s(moji, "%s", (atoi(opp_pet[locknum].opp_pettrans) == 0) ? "" : "转");
#endif
										StockFontBuffer(x + 90, y + 62, FONT_PRIO_FRONT, 2, moji, 0);


										StockDispBuffer(x + 55, y + 100, DISP_PRIO_IME3, CG_TRADE_HP_LINE, 0);
#ifdef _PETBLESS_
										sprintf_s(moji, "%3d", abs(atoi(opp_pet[locknum].opp_petshowhp)));
										if (atoi(opp_pet[locknum].opp_petshowhp) < 0)
											StockFontBuffer(x + 55, y + 92, FONT_PRIO_FRONT, FONT_PAL_RED, moji, 0);
										else
											StockFontBuffer(x + 55, y + 92, FONT_PRIO_FRONT, 0, moji, 0);

										StockDispBuffer(x + 55, y + 130, DISP_PRIO_IME3, CG_TRADE_ATK_LINE, 0);

										sprintf_s(moji, "%3d", abs(atoi(opp_pet[locknum].opp_petatk)));
										if (atoi(opp_pet[locknum].opp_petatk) < 0)
											StockFontBuffer(x + 55, y + 124, FONT_PRIO_FRONT, FONT_PAL_RED, moji, 0);
										else
											StockFontBuffer(x + 55, y + 124, FONT_PRIO_FRONT, 0, moji, 0);

										StockDispBuffer(x + 55, y + 160, DISP_PRIO_IME3, CG_TRADE_DEF_LINE, 0);
										sprintf_s(moji, "%3d", abs(atoi(opp_pet[locknum].opp_petdef)));

										if(atoi(opp_pet[locknum].opp_petdef) < 0)
											StockFontBuffer(x + 55, y + 154, FONT_PRIO_FRONT, FONT_PAL_RED, moji, 0);
										else
											StockFontBuffer(x + 55, y + 154, FONT_PRIO_FRONT, 0, moji, 0);
										StockDispBuffer(x + 55, y + 190, DISP_PRIO_IME3, CG_TRADE_DEX_LINE, 0);
										sprintf_s(moji, "%3d",abs( atoi(opp_pet[locknum].opp_petquick)));
										if(atoi(opp_pet[locknum].opp_petquick) < 0)
											StockFontBuffer(x + 55, y + 184, FONT_PRIO_FRONT, FONT_PAL_RED, moji, 0);
										else
											StockFontBuffer(x + 55, y + 184, FONT_PRIO_FRONT, 0, moji, 0);
#else
										sprintf_s(moji, "%3d", atoi(opp_pet[locknum].opp_petshowhp));
										StockFontBuffer(x + 55, y + 92, FONT_PRIO_FRONT, 0, moji, 0);

										StockDispBuffer(x + 55, y + 130, DISP_PRIO_IME3, CG_TRADE_ATK_LINE, 0);
										sprintf_s(moji, "%3d", atoi(opp_pet[locknum].opp_petatk));
										StockFontBuffer(x + 55, y + 124, FONT_PRIO_FRONT, 0, moji, 0);

										StockDispBuffer(x + 55, y + 160, DISP_PRIO_IME3, CG_TRADE_DEF_LINE, 0);
										sprintf_s(moji, "%3d", atoi(opp_pet[locknum].opp_petdef));
										StockFontBuffer(x + 55, y + 154, FONT_PRIO_FRONT, 0, moji, 0);

										StockDispBuffer(x + 55, y + 190, DISP_PRIO_IME3, CG_TRADE_DEX_LINE, 0);
										sprintf_s(moji, "%3d", atoi(opp_pet[locknum].opp_petquick));
										StockFontBuffer(x + 55, y + 184, FONT_PRIO_FRONT, 0, moji, 0);

#endif
										for (j = 0; j < 4; j++)
											StockDispBuffer(x + 100, y + 220 + j * 30, DISP_PRIO_IME3, CG_TRADE_SK1_LINE + j, 0);
										for (j = 4; j < atoi(opp_pet[locknum].opp_petslot); j++)
											StockDispBuffer(x + 230, y + 220 + (j - 4) * 30, DISP_PRIO_IME3, CG_TRADE_SK5_LINE + j - 4, 0);
										sprintf_s(moji, "%s", opp_pet[locknum].opp_petskill1);
										if (strcmp(moji, "(null)") != 0)
											StockFontBuffer(x + 65, y + 212, FONT_PRIO_FRONT, 0, moji, 0);
										sprintf_s(moji, "%s", opp_pet[locknum].opp_petskill2);
										if (strcmp(moji, "(null)") != 0)
											StockFontBuffer(x + 65, y + 242, FONT_PRIO_FRONT, 0, moji, 0);
										sprintf_s(moji, "%s", opp_pet[locknum].opp_petskill3);
										if (strcmp(moji, "(null)") != 0)
											StockFontBuffer(x + 65, y + 272, FONT_PRIO_FRONT, 0, moji, 0);
										sprintf_s(moji, "%s", opp_pet[locknum].opp_petskill4);
										if (strcmp(moji, "(null)") != 0)
											StockFontBuffer(x + 65, y + 302, FONT_PRIO_FRONT, 0, moji, 0);
										sprintf_s(moji, "%s", opp_pet[locknum].opp_petskill5);
										if (strcmp(moji, "(null)") != 0)
											StockFontBuffer(x + 195, y + 212, FONT_PRIO_FRONT, 0, moji, 0);
										sprintf_s(moji, "%s", opp_pet[locknum].opp_petskill6);
										if (strcmp(moji, "(null)") != 0)
											StockFontBuffer(x + 195, y + 242, FONT_PRIO_FRONT, 0, moji, 0);
										sprintf_s(moji, "%s", opp_pet[locknum].opp_petskill7);
										if (strcmp(moji, "(null)") != 0)
											StockFontBuffer(x + 195, y + 272, FONT_PRIO_FRONT, 0, moji, 0);
									}
								}
								else if (SecondActPet != NULL) {
									DeathAction(SecondActPet);
									SecondActPet = NULL;
								}
#endif
							}
							//显示数值、技能 ( 右方 )
							if (tradeList[i + 37].data != -1)
							{
								sprintf_s(moji, "%s", opp_pet[i].opp_petname);
								StockFontBuffer(x + 322, y + 13 + i * 42, FONT_PRIO_FRONT, atoi(opp_pet[i].opp_pettrans), moji, 0);

								if (strcmp(opp_pet[i].opp_petname, opp_pet[i].opp_petfreename))
								{
									sprintf_s(moji, "[%s]", opp_pet[i].opp_petfreename);
									StockFontBuffer(x + 442, y + 13 + i * 42, FONT_PRIO_FRONT, 6, moji, 0);
								}

#ifdef _PET_2TRANS
								sprintf_s( moji,"%s", "" );
								if (atoi(opp_pet[i].opp_pettrans) == 1)
									sprintf_s(moji, "%s", "一转");
								else if (atoi(opp_pet[i].opp_pettrans) == 2)
									sprintf_s(moji, "%s", "二转");
#ifdef _SHOW_FUSION
								if (LOWORD(atoi(opp_pet[i].opp_fusion)) == 1)
									sprintf_s(moji, "%s", "融合");
#endif
#else
								sprintf_s(moji, "%s", (atoi(opp_pet[i].opp_pettrans) == 0) ? "" : "转");
#endif
								StockFontBuffer(x + 590, y + 13 + i * 42, FONT_PRIO_FRONT, 2, moji, 0);

								sprintf_s(moji, "%4d", abs(atoi(opp_pet[i].opp_petshowhp)));
								StockFontBuffer(x + 504, y + 33 + i * 42, FONT_PRIO_FRONT, atoi(opp_pet[i].opp_pettrans), moji, 0);
								sprintf_s(moji, "%3d", atoi(opp_pet[i].opp_petlevel));
								StockFontBuffer(x + 558, y + 33 + i * 42, FONT_PRIO_FRONT, atoi(opp_pet[i].opp_pettrans), moji, 0);
								sprintf_s(moji, "%3d", abs(atoi(opp_pet[i].opp_petatk)));
								StockFontBuffer(x + 345, y + 33 + i * 42, FONT_PRIO_FRONT, atoi(opp_pet[i].opp_pettrans), moji, 0);
								sprintf_s(moji, "%3d", abs(atoi(opp_pet[i].opp_petdef)));
								StockFontBuffer(x + 398, y + 33 + i * 42, FONT_PRIO_FRONT, atoi(opp_pet[i].opp_pettrans), moji, 0);
								sprintf_s(moji, "%3d", abs(atoi(opp_pet[i].opp_petquick)));
								StockFontBuffer(x + 451, y + 33 + i * 42, FONT_PRIO_FRONT, atoi(opp_pet[i].opp_pettrans), moji, 0);
							}
						}
						//显示道具
						for (i = 0; i < 3; i++)
						{
							for (j = 0; j < 5; j++)
							{
								if (MakeHitBox(x + 368 + 51 * j - 35, y + 265 + 47 * i - 23,
									x + 368 + 51 * j + 18, y + 265 + 47 * i + 23, DISP_PRIO_IME3) == TRUE)
								{
#ifdef _TRADELOCKBTN				// (不可开) Syu ADD 增加锁定键
									if (mouse.onceState & MOUSE_LEFT_CRICK) {
										locknum2 = i * 5 + j;
										locknum = -1;
									}
									if (locknum2 == -1) {
										locknum2 = i * 5 + j;
										locknum = -1;
									}
									if (SecondActPet != NULL) {
										DeathAction(SecondActPet);
										SecondActPet = NULL;
									}
#ifdef _PET_ITEM
									bViewPetEquip = FALSE;
#endif
								}
								if (locknum2 != -1)
								{
									x = SecondTradeWnd->x;
									y = SecondTradeWnd->y;
									if (tradeList[locknum2 + 22].data != -1)
									{
										//说明过长换行
										char *splitPoint = opp_item[locknum2].effect;
										while (1)
										{
											if (y > 40)
												break;
											if (strlen(splitPoint) > 28)
											{
												strncpy_s(moji, splitPoint, 28);
												moji[28] = NULL;
												if (GetStrLastByte(moji) == 3)
												{
													moji[27] = NULL;
													splitPoint += 27;
												}
												else
												{
													moji[28] = NULL;
													splitPoint += 28;
												}
												StockFontBuffer(x + 50, y + 160, FONT_PRIO_FRONT, 0, moji, 0); y += 40;
											}
											else
											{
												strcpy(moji, splitPoint);
												StockFontBuffer(x + 50, y + 160, FONT_PRIO_FRONT, 0, moji, 0);
												break;
											}
										}
										StockDispBuffer(x + 80, y + 90, DISP_PRIO_IME3, atoi(opp_item[locknum2].graph), 0);

										sprintf_s(moji, "%s ", opp_item[locknum2].name);
										StockFontBuffer(x + 50, y + 120, FONT_PRIO_FRONT, 0, moji, 0);
										if (strcmp(opp_item[locknum2].name, opp_item[locknum2].freename))
										{
											sprintf_s(moji, "[%s]", opp_item[locknum2].freename);
											StockFontBuffer(x + 50, y + 140, FONT_PRIO_FRONT, 6, moji, 0);
										}
#ifdef _ITEM_PILENUMS
										sprintf_s(moji, "x%d", tradeList[locknum2 + 22].pilenum);
#else
										sprintf_s(moji, "%s", opp_item[locknum2].name);
#endif
										StockFontBuffer(x + 140, y + 80, FONT_PRIO_FRONT, 0, moji, 0);
#ifdef _NPC_ITEMUP
										ShowItemup(tradeList[locknum2 + 22].itemup, 240, 120);
#endif


										if (strcmp(opp_item[locknum2].damage, "") != 0)
											sprintf_s(moji, "耐久度：%s", opp_item[locknum2].damage);
										StockFontBuffer(x + 50, y + 240, FONT_PRIO_FRONT, 0, moji, 0);
									}
#endif
								}
								if (tradeList[i * 5 + j + 22].data != -1)
								{
									StockDispBuffer(x + 368 + 47 * j, y + 265 + 47 * i, DISP_PRIO_IME3, atoi(opp_item[i * 5 + j].graph), 0);
#ifdef _ITEM_PILENUMS
									if (tradeList[i * 5 + j + 22].pilenum > 1)
									{
										sprintf_s(moji, "%d", tradeList[i * 5 + j + 22].pilenum);
										StockFontBuffer(x + 370 + 51 * j + 10, y + 265 + 47 * i + 2, FONT_PRIO_FRONT, 0, moji, 0);
									}
#endif

								}
							}
						}
					}
					if (SecondTradeWnd != NULL) {

						if (SecondTradeWnd->hp > 0)
						{
							//产生交易第二视窗
							if (pActPet3 != NULL)
							{
								DeathAction(pActPet3);
								pActPet3 = NULL;
							}
							StockDispBuffer(((WINDOW_DISP *)SecondTradeWnd->pYobi)->mx, ((WINDOW_DISP *)SecondTradeWnd->pYobi)->my - 25, DISP_PRIO_MENU, CG_TRADE_VIEWWND, 1);

							if (mouse.onceState & MOUSE_LEFT_CRICK)
							{
								if (HitDispNo == SecondtradeWndFontNo[0]) {
									//关闭交易视窗
									MenuToggleFlag &= ~JOY_CTRL_T;
									//视窗关闭音效
									play_se(203, 320, 240);
									sprintf_s(buffer, "W|%s|%s", opp_sockfd, opp_name);
									//送出取消讯息给Server通知对方
									lssproto_TD_send(sockfd, buffer);
									tradeStatus = 0;
									tradeInit();
									pc.trade_confirm = 1;
								}
								else if (HitDispNo == SecondtradeWndFontNo[1])
								{
									MainTradeWndflag = true;
									SecondTradeWndflag = false;
									//andy_add 20030610
									for (i = 0; i < MENU_TRADE_0; i++) tradeWndFontNo[i] = -2;
									for (i = 0; i < 6; i++) SecondtradeWndFontNo[i] = -2;
#ifdef _PET_ITEM
									InitItem3( 325, 230 );
									bViewPetEquip = FALSE;
#endif
								}
#ifdef _TRADETALKWND				// (不可开) Syu ADD 交易新增对话框架
								else if (HitDispNo == SecondtradeWndFontNo[4])
									talkwndflag = true;
#endif
#ifdef _PET_ITEM
								else if (HitDispNo == SecondtradeWndFontNo[5])
								{
									bViewPetEquip = !bViewPetEquip;
									InitItem(SecondTradeWnd->x, SecondTradeWnd->y + 20, bViewPetEquip);
								}
#endif
								else
									LockAndOkfunction();
							}
#ifdef _TRADETALKWND				// (不可开) Syu ADD 交易新增对话框架
							if (talkwndflag == true)
							{
								talkwndx = mouse.nowPoint.x;
								talkwndy = mouse.nowPoint.y;
							}
							if (mouse.onceState & MOUSE_LEFT_CRICK_UP)
								talkwndflag = false;
							if (mouse.onceState & MOUSE_RIGHT_CRICK)
								tradetalkwndflag = false;
#endif
						}
					}
				}

#endif
			}
			break;
		}
	}
	else if (tradeStatus != 0)
	{
		char buffer[1024] = "";
		tradeStatus = 0;
		MenuToggleFlag &= ~JOY_CTRL_T;
		sprintf_s(buffer, "W|%s|%s", opp_sockfd, opp_name);
		lssproto_TD_send(sockfd, buffer);
		tradeInit();
		pc.trade_confirm = 1;
	}

	// 交易视窗部分到此结束


	// show Bank Window
	if (MenuToggleFlag & JOY_B){

		char buffer[1024];
		int x, y, w, h;

		if (checkPcWalkFlag() == 1) closeBankman();


		if (joy_trg[0] & JOY_ESC
			|| actBtn == 1
			|| menuBtn == 1
			|| disconnectServerFlag == TRUE
			)
		{
			closeBankman();
			return;
		}

		// ?????????
		if (pActMenuWnd5 == NULL){
			w = 270; h = 160;
			x = (lpDraw->xSize - w) / 2;
			y = (lpDraw->ySize - h) / 2;

			pActMenuWnd5 = MakeWindowDisp(x, y, w, h, NULL, -1, FALSE);
			play_se(202, 320, 240);	// ????????									

		}
		else{
			// ??????????????
			if (pActMenuWnd5->hp > 0){

				if (mouse.onceState & MOUSE_LEFT_CRICK){

					// ???
					if (HitDispNo == bankWndFontNo[0]){
						closeBankman();
						return;
					}

					if (HitDispNo == bankWndFontNo[1]) {
						sprintf_s(buffer, "B|G|%d", pc.gold - cashGold);
						if (bNewServer)
							lssproto_FM_send(sockfd, buffer);
						else
							old_lssproto_FM_send(sockfd, buffer);
						closeBankman();
						return;
					}

				}

				// Add Gold
				if (HitDispNo == bankWndFontNo[2]){

					if (mouse.onceState & MOUSE_LEFT_CRICK_UP && bankWndBtnFlag[2] == TRUE){

						bankWndBtnFlag[2] = FALSE;
					}

					if ((cashGold < CHAR_getMaxHaveGold()) && (totalGold - cashGold > 0)) {

						if (bankWndBtnFlag[2] == TRUE){

							cashGold += bankGoldInc;
							bankGoldCnt++;

							// ????
							if (bankGoldCnt >= 30){
								// ?????????????
								bankGoldCnt = 0;
								// ????????
								if (bankGoldInc == 0) {
									bankGoldInc = 1;
								}
								else{
									// ?????
									bankGoldInc *= 5;
									// ????????
									if (bankGoldInc > 10000) {
										bankGoldInc = 10000;
									}
								}
							}
							// ????????
							if (cashGold >= CHAR_getMaxHaveGold()) {
								cashGold = CHAR_getMaxHaveGold();
								play_se(220, 320, 240);
							}
							if (cashGold >= totalGold) {
								cashGold = totalGold;
								play_se(220, 320, 240);
							}

						}

						// ????????
						if (mouse.onceState & MOUSE_LEFT_CRICK) {
							// ???
							cashGold++;
							// ????????
							if (cashGold >= CHAR_getMaxHaveGold()) {
								cashGold = CHAR_getMaxHaveGold();
								// ???
								play_se(220, 320, 240);
							}
							else {
								// ?????
								bankGoldInc = 0;
								// ?????????????
								bankGoldCnt = 0;
								// ???????
								bankWndBtnFlag[2] = TRUE;
								// ?????
								play_se(217, 320, 240);
							}

						}
					}
				}
				else{
					// ??????
					bankWndBtnFlag[2] = FALSE;
				}
				// ??????????
				if (HitDispNo == bankWndFontNo[3]){
					// ???????????????
					if (mouse.onceState & MOUSE_LEFT_CRICK_UP && bankWndBtnFlag[3] == TRUE){
						bankWndBtnFlag[3] = FALSE;
						// ??????
						//play_se( 212, 320, 240 );
					}
					if ((cashGold > 0) && (totalGold - cashGold < MAX_BANKGOLD)) {
						// ??????
						if (bankWndBtnFlag[3] == TRUE){

							// ????
							cashGold -= bankGoldInc;
							// ?????????????
							bankGoldCnt++;

							// ????
							if (bankGoldCnt >= 30){
								// ?????????????
								bankGoldCnt = 0;
								// ????????
								if (bankGoldInc == 0) {
									bankGoldInc = 1;
								}
								else{
									// ?????
									bankGoldInc *= 5;
									// ????????
									if (bankGoldInc > 10000) {
										bankGoldInc = 10000;
									}
								}
							}
							// ????????
							if (cashGold <= 0){
								cashGold = 0;
								play_se(220, 320, 240);
							}
							if (totalGold - cashGold >= MAX_BANKGOLD){
								cashGold = totalGold - MAX_BANKGOLD;
								play_se(220, 320, 240);
							}

						}
						// ????????
						if (mouse.onceState & MOUSE_LEFT_CRICK) {

							cashGold--;

							if (cashGold <= 0){
								cashGold = 0;
								// ???
								play_se(220, 320, 240);
							}
							else if (totalGold - cashGold >= MAX_BANKGOLD) {
								cashGold = totalGold - MAX_BANKGOLD;
								play_se(220, 320, 240);
							}
							else{
								// ?????
								bankGoldInc = 0;
								// ?????????????
								bankGoldCnt = 0;
								// ???????
								bankWndBtnFlag[3] = TRUE;
								// ?????
								play_se(217, 320, 240);

							}

						}
					}
					else {
						bankGoldInc = 0;
						bankGoldCnt = 0;
					}

				}
				else{
					// ??????
					bankWndBtnFlag[3] = FALSE;
				}
				// ?????????
				for (int i = 2; i <= 8; i++) {
					if (mouse.state & MOUSE_LEFT_CRICK && tradeWndBtnFlag[i] == TRUE){
						// ????
						tradeWndBtnFlag[i] = TRUE;
					}
					else{
						// ???
						tradeWndBtnFlag[i] = FALSE;
					}
				}
				if (pActMenuWnd5 != NULL) {
					// ?
					x = pActMenuWnd5->x;
					y = pActMenuWnd5->y + 5;

					bankWndFontNo[0] = StockDispBuffer(x + 200, y + 133, DISP_PRIO_IME3, CG_TRADE_CANCEL_BTN, 2);
					bankWndFontNo[1] = StockDispBuffer(x + 75, y + 133, DISP_PRIO_IME3, CG_TRADE_OK_BTN, 2);
					bankWndFontNo[2] = StockDispBuffer(x + 140, y + 70, DISP_PRIO_IME3, CG_UP_BTN + bankWndBtnFlag[2], 2);
					bankWndFontNo[3] = StockDispBuffer(x + 200, y + 70, DISP_PRIO_IME3, CG_DOWN_BTN + bankWndBtnFlag[3], 2);
					sprintf_s(moji, "%7d", cashGold);
					StockFontBuffer(x + 180, y + 40, FONT_PRIO_FRONT, 0, moji, 0);
					sprintf_s(moji, "%7d", totalGold - cashGold);
					StockFontBuffer(x + 180, y + 86, FONT_PRIO_FRONT, 0, moji, 0);

					StockDispBuffer(((WINDOW_DISP *)pActMenuWnd5->pYobi)->mx, ((WINDOW_DISP *)pActMenuWnd5->pYobi)->my, DISP_PRIO_IME3, CG_FAMILY_BANK_WIN, 1);
				}
			}
		}
	}

#ifdef _STANDBYPET
	if (StandbyPetSendFlag == FALSE){
		int s_pet = 0;
		int i;

		for (i = 0; i < MAX_PET; i++) {
			if (pc.selectPetNo[i])
				s_pet |= (1 << i);
		}
		if (s_pet != pc.standbyPet)
			send_StandBy_Pet();
	}
#endif


}
#ifdef _NEWREQUESTPROTOCOL			// (不可开) Syu ADD 新增Protocol要求细项
void lssproto_RESIST_recv ( int fd, char *data)
{
	for ( int  i = 0 ; i < CHAR_MAX_DETAIL ; i ++ ) 
		getStringToken( data, '|', i + 1 , sizeof( CharDetail[i] ) - 1 , CharDetail[i] );
}
#endif

#ifdef _ALCHEPLUS
void lssproto_ALCHEPLUS_recv(int fd, char *data)
{
	char token[64];
	int i;
	for (i = 0; i < 25; i++) {
		getStringToken(data, '|', i + 1, sizeof(token)-1, token);
		iCharAlchePlus[i] = atoi(token);
	}
}
#endif

#ifdef _OUTOFBATTLESKILL			// (不可开) Syu ADD 非战斗时技能Protocol
void lssproto_BATTLESKILL_recv(int fd, char *data) {
	setCharMind(pc.ptAct, atoi(data));
}
#endif

void lssproto_CHAREFFECT_recv(int fd, char *data) {
	if (data[0] == '1')
		setCharFamily(pc.ptAct, atoi(data + 2));
	else if (data[0] == '2')
		setCharMind(pc.ptAct, atoi(data + 2));
	else if (data[0] == '3')
		setCharmFamily(pc.ptAct, atoi(data + 2));
#ifdef _CHARTITLE_
	else if (data[0] == '4'){
		setCharmTitle(pc.ptAct, atoi(data + 2));
	}
#endif
#ifdef _CHAR_MANOR_
	else if (data[0] == '5')
		setCharmManor(pc.ptAct, atoi(data + 2));
#endif
}

#ifdef _TRADE_BUG_LOG
bool __writeFirstTime;
#endif


void lssproto_TD_recv(int fd, char *data)
{
	char Head[2] = "";
	char buf_sockfd[128] = "";
	char buf_name[128] = "";
	char buf[128] = "";
#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
	char opp_index[128];
	int  index;
	char realname[256];
	char freename[256];
#endif


	getStringToken(data, '|', 1, sizeof(char), Head);

	// 交易开启资料初始化	
	if (strcmp(Head, "C") == 0) {

#ifdef _TRADE_BUG_LOG
		__writeFirstTime = true;
#endif

		memset(opp_sockfd, 0, sizeof(opp_sockfd));
		memset(opp_name, 0, sizeof(opp_name));
		memset(trade_command, 0, sizeof(trade_command));

		getStringToken(data, '|', 2, sizeof(opp_sockfd)-1, opp_sockfd);
		getStringToken(data, '|', 3, sizeof(opp_name)-1, opp_name);
		getStringToken(data, '|', 4, sizeof(trade_command)-1, trade_command);

		if (strcmp(trade_command, "0") == 0) {
			return;
		}
		else if (strcmp(trade_command, "1") == 0) {
			tradeStatus = 1;
			MenuToggleFlag = JOY_CTRL_T;
			pc.trade_confirm = 1;
		}
#ifdef _COMFIRM_TRADE_REQUEST
		else if (strcmp(trade_command, "2") == 0)
		{
			tradeStatus = 1;
			MenuToggleFlag = JOY_CTRL_T;
			pc.trade_confirm = 1;
			tradeWndNo = 1;
		}
#endif

	}
	//处理物品交易资讯传递
	else if (strcmp(Head, "T") == 0) {

		if (tradeStatus == 0)	return;
		char buf_showindex[128];

		//andy_add mttrade
		getStringToken(data, '|', 4, sizeof(trade_kind)-1, trade_kind);
		if (strcmp(trade_kind, "S") == 0) {
			char buf1[256];
			int objno = -1, showno = -1;
			if (pActMenuWnd4 == NULL){
				DeathMenuAction();
				DeathMenuAction2();

				int w = 620; int h = 456;
				int x = (lpDraw->xSize - w) / 2;
				int y = (lpDraw->ySize - h) / 2;

				pActMenuWnd4 = MakeWindowDisp(x, y, w, h, NULL, -1, FALSE);
				InitItem3(325, 230);
#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
				if (SecondTradeWnd == NULL)
					SecondTradeWnd = MakeWindowDisp(10, 0, 620, 456, 0, -1);
#endif
			}
			getStringToken(data, '|', 6, sizeof(buf1)-1, buf1);
			objno = atoi(buf1);
			getStringToken(data, '|', 7, sizeof(buf1)-1, buf1);
			showno = atoi(buf1);
			getStringToken(data, '|', 5, sizeof(buf1)-1, buf1);

			ItemBuffer[objno].mixFlag = FALSE;
			if (pc.item[objno].useFlag == TRUE){
				ItemBuffer[objno].dragFlag = TRUE;
				mouse.itemNo = objno;
			}

			if (!strcmp(buf1, "I"))	{	//I
			}
			else {	//P
				tradePetIndex = objno;
				tradePet[0].index = objno;

				if (pet[objno].useFlag &&  pc.ridePetNo != objno){
					if (pet[objno].freeName[0] != NULL)
						strcpy(tradePet[0].name, pet[objno].freeName);
					else
						strcpy(tradePet[0].name, pet[objno].name);
					tradePet[0].level = pet[objno].level;
					tradePet[0].atk = pet[objno].atk;
					tradePet[0].def = pet[objno].def;
					tradePet[0].quick = pet[objno].quick;
					tradePet[0].graNo = pet[objno].graNo;

					showindex[3] = 3;
					DeathAction(pActPet4);
					pActPet4 = NULL;
				}
			}

			mouse.itemNo = -1;

			return;
		}

		getStringToken(data, '|', 2, sizeof(buf_sockfd)-1, buf_sockfd);
		getStringToken(data, '|', 3, sizeof(buf_name)-1, buf_name);
		getStringToken(data, '|', 4, sizeof(trade_kind)-1, trade_kind);
		getStringToken(data, '|', 5, sizeof(buf_showindex)-1, buf_showindex);
		opp_showindex = atoi(buf_showindex);

		if ((strcmp(buf_sockfd, opp_sockfd) != 0) || (strcmp(buf_name, opp_name) != 0))
			return;

		if (strcmp(trade_kind, "G") == 0) {

			getStringToken(data, '|', 6, sizeof(opp_goldmount)-1, opp_goldmount);
			int mount = atoi(opp_goldmount);
#ifdef _CHANGETRADERULE		   // (不可开) Syu ADD 交易规则修订
			if (tradeWndDropGoldGet != 0) {
				MenuToggleFlag ^= JOY_CTRL_T;
				play_se(203, 320, 240);
				sprintf_s(buf, "W|%s|%s", opp_sockfd, opp_name);
				if (bNewServer)
					lssproto_TD_send(sockfd, buf);
				else
					old_lssproto_TD_send(sockfd, buf);
				sprintf_s(buf, "%s以不正常方式修改交易金钱，系统强制关闭交易视窗！", opp_name);
				StockChatBufferLine(buf, FONT_PAL_RED);
				return;
			}
#endif

#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
			//andy_reEdit
			if (mount != -1) {
				tradeList[42].kind = 'G';
				tradeList[42].data = mount;
				tradeWndDropGoldGet = mount;
			}
			else {
				tradeList[42].data = 0;
				tradeWndDropGoldGet = 0;
			}
			/*
			if( opp_showindex == 3 ) {
			if( mount != -1 ) {
			tradeList[42].kind = 'G' ;
			tradeList[42].data = mount ;
			tradeWndDropGoldGet = mount;
			}else {
			tradeList[42].data = 0;
			tradeWndDropGoldGet = 0;
			}
			}else
			*/

#else
			if( opp_showindex == 1 ) {
				if( mount != -1 ) {
					showindex[4] = 2;
					tradeWndDropGoldGet = mount;
				}else {
					showindex[4] = 0;
					tradeWndDropGoldGet = 0;
				}
			}
			else if (opp_showindex == 2) {
				if (mount != -1) {
					showindex[5] = 2;
					tradeWndDropGoldGet = mount;
				}
				else {
					showindex[5] = 0;
					tradeWndDropGoldGet = 0;
				}
			}
			else return;
#endif

		}

		if (strcmp(trade_kind, "I") == 0) {
			char pilenum[256], item_freename[256];

			getStringToken(data, '|', 6, sizeof(opp_itemgraph)-1, opp_itemgraph);

			getStringToken(data, '|', 7, sizeof(opp_itemname)-1, opp_itemname);
			getStringToken(data, '|', 8, sizeof(item_freename)-1, item_freename);

			getStringToken(data, '|', 9, sizeof(opp_itemeffect)-1, opp_itemeffect);
			getStringToken(data, '|', 10, sizeof(opp_itemindex)-1, opp_itemindex);
			getStringToken(data, '|', 11, sizeof(opp_itemdamage)-1, opp_itemdamage);// 显示物品耐久度

#ifdef _ITEM_PILENUMS
			getStringToken(data, '|', 12, sizeof(pilenum)-1, pilenum);//pilenum
#endif
#ifdef _NPC_ITEMUP
			getStringToken(data, '|', 13, sizeof(itemup)-1, itemup);
#endif

#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
			if (strcmp(opp_itemgraph, "-1") == 0)
				return;

			int i = atoi(opp_itemindex);


			int chkindex = -1, frontempIndex = -1;
			//检查道具opp_item空间是否已满  修正  xiezi
			for (int scanindex = 0; scanindex < 15; scanindex++)
			{
				if (strcmp(opp_item[chkindex].itemindex, opp_itemindex) == 0)
				{
					chkindex = scanindex;
					break;
				}
				if ((opp_item[scanindex].itemindex[0] == NULL || strcmp(opp_item[scanindex].itemindex, "-1") == 0) && frontempIndex < 0)
				{
					frontempIndex = scanindex;
				}
			}
			chkindex = chkindex > -1 ? chkindex : frontempIndex;
			if (chkindex > -1)
			{
				strcpy(opp_item[chkindex].name, makeStringFromEscaped(opp_itemname));
				strcpy(opp_item[chkindex].freename, makeStringFromEscaped(item_freename));
				strcpy(opp_item[chkindex].graph, opp_itemgraph);
				strcpy(opp_item[chkindex].effect, makeStringFromEscaped(opp_itemeffect));
				strcpy(opp_item[chkindex].itemindex, opp_itemindex);
				strcpy(opp_item[chkindex].damage, makeStringFromEscaped(opp_itemdamage));

				chkindex = 0, frontempIndex = 0;
				//检查道具tradeList空间是否已满  修正  xiezi
				for (int scanindex = 22; scanindex < 37; scanindex++)
				{
					if (tradeList[chkindex].data == i)
					{
						chkindex = scanindex;
						break;
					}
					if (tradeList[scanindex].data == -1 && frontempIndex < 1)
					{
						frontempIndex = scanindex;
					}
				}
				chkindex = chkindex > 0 ? chkindex : frontempIndex;

				if (chkindex > 0){
					tradeList[chkindex].kind = 'I';
					tradeList[chkindex].data = atoi(opp_itemindex);
					strcpy(tradeList[chkindex].name, makeStringFromEscaped(opp_itemname));

					strcpy(tradeList[chkindex].freename, makeStringFromEscaped(item_freename));

					strcpy(tradeList[chkindex].damage, makeStringFromEscaped(opp_itemdamage));
#ifdef _ITEM_PILENUMS
					tradeList[chkindex].pilenum = atoi(pilenum);
#endif
#ifdef _NPC_ITEMUP
					tradeList[chkindex].itemup = atoi(itemup);
#endif

#endif  //_TRADESYSTEM2
				}
			}
		}

		if (strcmp(trade_kind, "P") == 0) {
#ifdef _PET_ITEM
			int		iItemNo;
			char	szData[256];
#endif

#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
			getStringToken(data, '|', 12, sizeof(opp_index)-1, opp_index);
			index = -1;

			//检查道具opp_item空间是否已满  修正  xiezi
			for (int scanindex = 0; scanindex < 5; scanindex++)
			{
				if (opp_pet[scanindex].opp_petindex[0] == NULL || strcmp(opp_pet[scanindex].opp_petindex, "-1") == 0)
				{
					index = scanindex;
					break;
				}
			}

			if (index > -1){

				getStringToken(data, '|', 6, sizeof(opp_pet[index].opp_petgrano) - 1, opp_pet[index].opp_petgrano);
				getStringToken(data, '|', 7, sizeof(opp_pet[index].opp_petname) - 1, opp_pet[index].opp_petname);
				getStringToken(data, '|', 8, sizeof(opp_pet[index].opp_petlevel) - 1, opp_pet[index].opp_petlevel);
				getStringToken(data, '|', 9, sizeof(opp_pet[index].opp_petatk) - 1, opp_pet[index].opp_petatk);
				getStringToken(data, '|', 10, sizeof(opp_pet[index].opp_petdef) - 1, opp_pet[index].opp_petdef);
				getStringToken(data, '|', 11, sizeof(opp_pet[index].opp_petquick) - 1, opp_pet[index].opp_petquick);
				getStringToken(data, '|', 12, sizeof(opp_pet[index].opp_petindex) - 1, opp_pet[index].opp_petindex);
				getStringToken(data, '|', 13, sizeof(opp_pet[index].opp_pettrans) - 1, opp_pet[index].opp_pettrans);
				getStringToken(data, '|', 14, sizeof(opp_pet[index].opp_petshowhp) - 1, opp_pet[index].opp_petshowhp);
				getStringToken(data, '|', 15, sizeof(opp_pet[index].opp_petslot) - 1, opp_pet[index].opp_petslot);
				getStringToken(data, '|', 16, sizeof(opp_pet[index].opp_petskill1) - 1, opp_pet[index].opp_petskill1);
				getStringToken(data, '|', 17, sizeof(opp_pet[index].opp_petskill2) - 1, opp_pet[index].opp_petskill2);
				getStringToken(data, '|', 18, sizeof(opp_pet[index].opp_petskill3) - 1, opp_pet[index].opp_petskill3);
				getStringToken(data, '|', 19, sizeof(opp_pet[index].opp_petskill4) - 1, opp_pet[index].opp_petskill4);
				getStringToken(data, '|', 20, sizeof(opp_pet[index].opp_petskill5) - 1, opp_pet[index].opp_petskill5);
				getStringToken(data, '|', 21, sizeof(opp_pet[index].opp_petskill6) - 1, opp_pet[index].opp_petskill6);
				getStringToken(data, '|', 22, sizeof(opp_pet[index].opp_petskill7) - 1, opp_pet[index].opp_petskill7);
				getStringToken(data, '|', 23, sizeof(realname), realname);
				getStringToken(data, '|', 24, sizeof(freename), freename);
#ifdef _SHOW_FUSION
				getStringToken(data, '|', 25, sizeof(opp_pet[index].opp_fusion) - 1, opp_pet[index].opp_fusion);
#endif
				strcpy(opp_pet[index].opp_petname, realname);
				strcpy(opp_pet[index].opp_petfreename, freename);
#endif

#ifdef _PET_ITEM
				for (int i = 0;; i++){
					if (getStringToken(data, '|', 26 + i * 6, sizeof(szData), szData))
						break;
					iItemNo = atoi(szData);
					getStringToken(data, '|', 27 + i * 6, sizeof(opp_pet[index].oPetItemInfo[iItemNo].name), opp_pet[index].oPetItemInfo[iItemNo].name);
					getStringToken(data, '|', 28 + i * 6, sizeof(opp_pet[index].oPetItemInfo[iItemNo].memo), opp_pet[index].oPetItemInfo[iItemNo].memo);
					getStringToken(data, '|', 29 + i * 6, sizeof(opp_pet[index].oPetItemInfo[iItemNo].damage), opp_pet[index].oPetItemInfo[iItemNo].damage);
					getStringToken(data, '|', 30 + i * 6, sizeof(szData), szData);
					opp_pet[index].oPetItemInfo[iItemNo].color = atoi(szData);
					getStringToken(data, '|', 31 + i * 6, sizeof(szData), szData);
					opp_pet[index].oPetItemInfo[iItemNo].bmpNo = atoi(szData);
				}
#endif

#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
				tradeList[index + 37].data = atoi(opp_pet[index].opp_petindex);
				tradeList[index + 37].kind = 'P';
				strcpy(tradeList[index + 37].freename, freename);
				strcpy(tradeList[index + 37].name, realname);
				tradeList[index + 37].level = atoi(opp_pet[index].opp_petlevel);
				tradeList[index + 37].trns = atoi(opp_pet[index].opp_pettrans);
#ifdef _SHOW_FUSION
				tradeList[index + 37].fusion = atoi(opp_pet[index].opp_fusion);
#endif
#endif

				if (opp_showindex == 3) {
#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
					if (strcmp(opp_pet[index].opp_petgrano, "-1") == 0) {
#endif
						showindex[6] = 0;
					}
					else {
						showindex[6] = 3;
					}
					DeathAction(pActPet5);
					pActPet5 = NULL;
				}
			}
		}


		// shan trade(DoubleCheck) begin
		if (strcmp(trade_kind, "C") == 0) {
			if (pc.trade_confirm == 1) pc.trade_confirm = 3;
			if (pc.trade_confirm == 2)	pc.trade_confirm = 4;
		}
		// end

		if (strcmp(trade_kind, "A") == 0)
			tradeStatus = 2;

	}
	else if (strcmp(Head, "W") == 0) {//取消交易
		getStringToken(data, '|', 2, sizeof(buf_sockfd)-1, buf_sockfd);
		getStringToken(data, '|', 3, sizeof(buf_name)-1, buf_name);
		if ((strcmp(buf_sockfd, opp_sockfd) == 0) && (strcmp(buf_name, opp_name) == 0)) {
			tradeStatus = 0;
			MenuToggleFlag &= ~JOY_CTRL_T;
			tradeInit();
			pc.trade_confirm = 1;
			play_se(203, 320, 240);
		}
	}
}

void tradeInit(void)
{
#ifdef _TRADESYSTEM2	// (不可开) Syu ADD 新交易系统
	//初始化时清空Action
	DeathAction(SecondTradeWnd);
	SecondTradeWnd = NULL;
	DeathAction(SecondActPet);
	SecondActPet = NULL;
	DeathAction(TradeTalkWnd);
	TradeTalkWnd = NULL;
	//Terry add 2003/11/25
	g_bTradesystemOpen = false;
	//end
#endif
	DeathAction(pActMenuWnd4);
	// 交易视窗初始化时将focus还给chat
	GetKeyInputFocus(&MyChatBuffer);
	Tradeflag = false;
	pActMenuWnd4 = NULL;
	DeathAction(pActPet3);
	pActPet3 = NULL;
	DeathAction(pActPet4);
	pActPet4 = NULL;
	DeathAction(pActPet5);
	pActPet5 = NULL;
	for (int i = 0; i <= 6; i++)
		showindex[i] = 0;
	strcpy(opp_sockfd, "-1");
	strcpy(opp_name, "");
	tradePetIndex = 0;
	strcpy(tradepetindexget, "-1");
	mine_itemindex[0] = -1;
	mine_itemindex[1] = -1;

	tradeWndDropGoldSend = 0;
	tradeWndDropGoldGet = 0;
	tradeWndDropGold = 0;
	//strcpy(opp_item[0].itemindex, "-1");
	//strcpy(opp_item[1].itemindex, "-1");

	memset(opp_pet, 0, sizeof(showpet)* 5);
	for (int i = 0; i < MAX_MAXHAVEITEM; i++)
	{
		strcpy(opp_item[i].itemindex, "-1");
	}
	memset(tradeList, 0, sizeof(tradelist)* 45);
}

void BankmanInit(char *data)
{
	char token1[1024];
	getStringToken(data, '|', 3, sizeof(token1)-1, token1);

	bankGold = atoi(token1);
	cashGold = pc.gold;
	totalGold = bankGold + cashGold;

	MenuToggleFlag = JOY_B;
	DeathAction(pActMenuWnd5);
	pActMenuWnd5 = NULL;
}

void closeBankman(void) {

	MenuToggleFlag ^= JOY_B;
	play_se(203, 320, 240);
	DeathAction(pActMenuWnd5);
	pActMenuWnd5 = NULL;

}
#ifdef  _RIDEPET_
void checkRidePet(int pindex)
{
	char buf[128];
	sprintf_s(buf, "R|P|%d", pindex);
	if (bNewServer)
		lssproto_FM_send(sockfd, buf);
	else
		old_lssproto_FM_send(sockfd, buf);
	return;
}
#else
void checkRidePet(int pindex)
{
	int j;
#ifdef _PET_ITEM
	BOOL	bHavePetItem = FALSE;
#endif
	
	if (!bNewServer)
		return;

#ifdef _PET_ITEM
	// 宠身上有装备不可骑
	for (j = 0; j < MAX_PET_ITEM; ++j){
		if (pet[pindex].item[j].useFlag){	// 身上有装装备
			bHavePetItem = TRUE;
			break;
		}
	}
#endif
	if( pc.ridePetNo < 0 
		&& pc.learnride >= pet[pindex].level //Change fix 这里被注解掉了 20050801打开
		&& (pc.level + 5) > pet[pindex].level
		&& pet[pindex].ai >= 100 
		&& pc.graNo != SPR_pet021 
		&& pc.graNo != 100362 //金飞
#ifdef _PETSKILL_BECOMEPIG // 乌力化中不可骑
		&& pc.graNo != 100250
#endif
#ifdef _THEATER
		&& pc.graNo != 101989	// 穿布偶装时不能骑宠
#endif
		)
	{
		pc.ridePetNo = -1;
		for (j = 0; j < sizeof(ridePetTable) / sizeof(tagRidePetTable); j++){
			int baseimageNo = pc.graNo - (pc.graNo % 5);
			int leaderimageNo = 100700 + ((baseimageNo - 100000) / 20) * 10 + (pc.familySprite) * 5;
//			//andy_edit
//			if (((ridePetTable[j].charNo == pc.graNo) || (ridePetTable[j].charNo == pc.baseGraNo)) /*&& ridePetTable[j].petNo == pet[pindex].graNo*/){
//				char buf[64];
//#ifdef _PET_ITEM
//				if (bHavePetItem){	// 有装备不可骑
//					StockChatBufferLine("宠物身上有装备不可骑乘！", FONT_PAL_YELLOW);
//					pc.selectPetNo[pindex] = 0;
//					return;
//				}
//#endif
//				sprintf(buf, "R|P|%d", pindex);
//				if (bNewServer)
//					lssproto_FM_send(sockfd, buf);
//				else
//					old_lssproto_FM_send(sockfd, buf);
//				pc.ridePetNo = pindex;
//				return;
			char buf[64];
			sprintf(buf, "R|P|%d", pindex);
			if (bNewServer)
				lssproto_FM_send(sockfd, buf);
			else
				old_lssproto_FM_send(sockfd, buf);
			pc.ridePetNo = pindex;
			return;

			
#ifdef _LEADERRIDE
			if (ridePetTable[j].charNo == leaderimageNo && ridePetTable[j].petNo == pet[pindex].graNo &&
				pc.big4fm != 0 && pc.familyleader != FMMEMBER_APPLY && pc.familyleader != FMMEMBER_NONE){	
				char buf[64];
#ifdef _PET_ITEM
				if (bHavePetItem){	// 有装备不可骑
					StockChatBufferLine("宠物身上有装备不可骑乘！", FONT_PAL_YELLOW);
					pc.selectPetNo[pindex] = 0;
					return;
				}
#endif
				sprintf(buf, "R|P|%d", pindex);
				if (bNewServer)
					lssproto_FM_send(sockfd, buf);
				else
					old_lssproto_FM_send(sockfd, buf);
				pc.ridePetNo = pindex;
				return;
			}
#endif
		}
		{//andy_add 新骑宠
			int ti = -1, index;
			unsigned int LRCode = 1<<30;
			if ((ti = RIDEPET_getPETindex(pet[pindex].graNo, pc.lowsride)) < 0)
			{
#ifdef RIDE_PET_LIMIT
				if (pc.lowsride >= LRCode)
				{
					if ((ti = RIDEPET_getPETindex_New(pet[pindex].graNo, LRCode)) < 0)
					{
						return;
					}
				}
				else
				{
					return;
				}
#endif
			}
			if ((index = RIDEPET_getNOindex(pc.baseGraNo)) >= 0){
				char buf[64];	
				if (RIDEPET_getRIDEno(index, ti) >= 0){
#ifdef _PET_ITEM
					if (bHavePetItem){	// 有装备不可骑
						StockChatBufferLine("宠物身上有装备不可骑乘！", FONT_PAL_YELLOW);
						pc.selectPetNo[pindex] = 0;
						return;
					}
#endif
					sprintf(buf, "R|P|%d", pindex);
					if (bNewServer)
						lssproto_FM_send(sockfd, buf);
					else
						old_lssproto_FM_send(sockfd, buf);
					pc.ridePetNo = pindex;
					return;
				}
			}
		}
	}
}
#endif

//andy_add 2002/06/24
int RIDEPET_getNOindex(int baseNo)
{
	int i;
	for (i = 0; i < sizeof(RPlistMode) / sizeof(tagRidePetList); i++)	{
		if (RPlistMode[i].charNo == baseNo)
			return RPlistMode[i].Noindex;
	}
	return -1;
}
//andy_add 2002/06/24
int RIDEPET_getPETindex(int PetNo, int learnCode)
{
	int i;
	for (i = 0; i < sizeof(RideCodeMode) / sizeof(tagRideCodeMode); i++){
		if (RideCodeMode[i].petNo == PetNo &&
			(RideCodeMode[i].learnCode & learnCode))
			return i;
	}
	return -1;
}
#ifdef RIDE_PET_LIMIT
int RIDEPET_getPETindex_New(int PetNo, int learnCode)
{
	int i;
	for (i = 0; i < sizeof(RideCodeMode) / sizeof(tagRideCodeMode); i++)	{
		if (RideCodeMode[i].petNo == PetNo/* &&
			(RideCodeMode[i].learnCode & learnCode)*/)	{
			return i;
		}
	}
	return -1;
}
#endif

//andy_add 2002/06/24
int RIDEPET_getRIDEno(int index, int ti)
{
	if (index < 0 || index >= sizeof(RideNoList) / sizeof(tagRideNoList))
		return -1;
	if (ti < 0 || ti >= MAXNOINDEX)
		return -1;
	return RideNoList[index].RideNo[ti];
}

#ifdef _TELLCHANNEL				//ROG ADD 密语频道
void InitSelectChar(char *msg, BOOL endFlag)
{
	int turn, level;
	char name1[32];
	if (MultiTells == TRUE)
		DeathTellChannel();

	if (!endFlag){
#ifndef _CHANNEL_MODIFY
		index[CharNum] = getIntegerToken(msg, '|', 2);
		turn = getIntegerToken(msg, '|', 3);
		level = getIntegerToken(msg, '|', 4);
		getStringToken(msg, '|', 5, sizeof(name[CharNum]) - 1, name[CharNum]);
		getStringToken(msg, '|', 6, sizeof(name1)-1, name1);
#else
		index[CharNum] = getIntegerToken(msg, '|', 3);
		turn = getIntegerToken(msg, '|', 4);
		level = getIntegerToken(msg, '|', 5);
		getStringToken(msg, '|', 6, sizeof(name[CharNum]) - 1, name[CharNum]);
		getStringToken(msg, '|', 7, sizeof(name1)-1, name1);
#endif
		sprintf_s(TellInfo[CharNum], " %15s %15s  %5d  %5d   ", name[CharNum], name1, turn, level);
		CharNum++;
	}
	else{
#ifndef _CHANNEL_MODIFY
		getStringToken(msg, '|', 2, sizeof(reSendMsg)-1, reSendMsg);
#else
		getStringToken(msg, '|', 3, sizeof(reSendMsg)-1, reSendMsg);
#endif
		MultiTells = TRUE;
	}
	//test//////////////
#ifdef _TIMEBAR_FUNCTION
	StartTime = TimeGetTime();
	timBarIdent = SetTimeBar("aaaaaa", 50);
#endif
	//////////////////
}

void SelectChar(void)
{
	char tmpMsg[1024];
	int x, y;
	if (pActMsgWnd == NULL){
		pActMsgWnd = MakeWindowDisp(80, 200, 6, CharNum / 2 + 1, 2, 0);
	}
	else if (pActMsgWnd->hp > 0){
		x = pActMsgWnd->x;
		y = pActMsgWnd->y;
		char title[] = { "           名 字           昵 称  转 生  等 级 " };
		StockFontBuffer(x + 10, y + 15, FONT_PRIO_FRONT, 4, title, 0);
		for (int i = 0; i < CharNum; i++){
			StockFontBuffer(x + 10, y + 40 + i * 20, FONT_PRIO_FRONT, 5, TellInfo[i], 0);
			if (MakeHitBox(x + 10, y + 37 + i * 20, x + 360, y + 56 + i * 20, DISP_PRIO_BOX2)){
				if (mouse.onceState & MOUSE_LEFT_CRICK){
					sprintf_s(tmpMsg, "%s %s /T%d  ", name[i], reSendMsg, index[i]);
					TalkMode = 1;
					chatStrSendForServer(tmpMsg, 0);
					DeathTellChannel();
				}
			}
		}
	}

	//test//////////////
#ifdef _TIMEBAR_FUNCTION
	int Now = TimeGetTime();
	Now -= StartTime;
	int rsut = SetTimeBarPos(timBarIdent, Now / 1000);
	if (rsut == 1)
		StartTime += 50000;
	else if (rsut == -1){
		DeathAction(pTimeBarWnd);
		pTimeBarWnd = NULL;
	}

#endif
	////////////////////
}

void DeathTellChannel(void)
{
	DeathAction(pActMsgWnd);
	pActMsgWnd = NULL;
	MultiTells = FALSE;
	CharNum = 0;
}

#endif

#ifdef _FRIENDCHANNEL				//ROG ADD 好友频道

void initSetRoomName()
{
	DeathMenuAction();
	DeathMenuAction2();
	DeathAction(pSetRoomWnd);
	pSetRoomWnd = NULL;
	GetKeyInputFocus( &MyChatBuffer );
	setRoomFlag = TRUE;
	SelRoomBtn = 0;
}

void setRoomName(void)
{
	static int setRoomBtn[2];				//设定聊天室名称用

	if(pSetRoomWnd == NULL){
		pSetRoomWnd = MakeWindowDisp( 270, 0, 3, 2, NULL, 0 );
		play_se( 202, 320, 240 );
		chatRoomName.buffer[ 0 ] = NULL;
		chatRoomName.cnt = 0;
		chatRoomName.cursor=0;
		chatRoomName.len = 16;
		chatRoomName.color = 0;
		chatRoomName.x = pSetRoomWnd->x + 22;
		chatRoomName.y = pSetRoomWnd->y + 35;
		chatRoomName.fontPrio = FONT_PRIO_FRONT;
	}
	else if(pSetRoomWnd->hp > 0 ){
		int x = pSetRoomWnd->x;
		int y = pSetRoomWnd->y;

		setRoomBtn[0] = StockDispBuffer( pSetRoomWnd->x + 53, pSetRoomWnd->y + 70, DISP_PRIO_IME3, CG_OK_BTN, 2 );
		setRoomBtn[1] = StockDispBuffer( pSetRoomWnd->x + 140, pSetRoomWnd->y + 70, DISP_PRIO_IME3, CG_CANCEL_BTN, 2 );

		char title[] = {"请输入频道名称"};
		StockFontBuffer( x + 22, y + 10, FONT_PRIO_FRONT,4 , title, 0 );
		GetKeyInputFocus( &chatRoomName );
		StockFontBuffer2( &chatRoomName );

		if( mouse.onceState & MOUSE_LEFT_CRICK ){
			if( HitDispNo == setRoomBtn[0] )
				KeyboardReturn();
			else if( HitDispNo == setRoomBtn[1] ){
				GetKeyInputFocus( &MyChatBuffer );
#ifdef _CHATROOMPROTOCOL
				lssproto_CHATROOM_send ( sockfd , "B|" ) ; 
#endif
				DeathAction(pSetRoomWnd);
				pSetRoomWnd = NULL;
				setRoomFlag = FALSE;
			}
		}
	}
}

void InitSelChatRoom(char *msg)
{
	char tempRoomNum[32],RoomName[32];
	int memberNum = 0;
	char chiefName[128] = {""};
	char temp[16],*temp1;
	int i = 2, k=0;
	roomNum = 0;

	while( getStringToken( msg, '|', i , sizeof(tempRoomNum) -1 ,tempRoomNum )!=1){
		if( tempRoomNum[0] == 'r' ){
			temp1 = tempRoomNum;
			temp1++;
			roomNum = atoi( temp1);

			getStringToken( msg, '|', i+1 , sizeof(RoomName) -1 ,RoomName );
			getStringToken( msg, '|', i+3 , sizeof(chiefName) -1 ,chiefName );
			getStringToken( msg, '|', i+4 , sizeof(temp) -1 ,temp );
			char *sss;
			if( (temp1 = strtok_s(temp,"p",&sss))!= NULL )
				memberNum = atoi(temp1);

			roomIndex[k] = roomNum;
			sprintf_s(roomInfo[k],"  %16s  %16s   %2d ",RoomName,chiefName,memberNum);
			k++;
			roomNum ++;
			i += 5;	
		}
		else
			break;			
	}
	SelRoomBtn = 1;
}

#ifdef _CHATROOMPROTOCOL
void SelectChatRoom(void)
{
	char tmpMsg[128] = {""};

	if(pSelChanlWnd == NULL){
		pSelChanlWnd = MakeWindowDisp( 120, 100, 7, roomNum / 2 + 2 , 2, 0 );
	}
	else if(pSelChanlWnd->hp > 0 ){
		int x = pSelChanlWnd->x;
		int y = pSelChanlWnd->y;
		char title[] = {"          频道名称          队长名称  人数 "};
		StockFontBuffer( x + 10, y + 15, FONT_PRIO_FRONT,4 , title, 0 );
		int i;
		for(i = 0 ; i < roomNum ; i++ ){
			StockFontBuffer( x + 10, y + 40 + i * 20, FONT_PRIO_FRONT,5 , roomInfo[i], 0 );
			if( MakeHitBox( x +10, y+ 37 + i * 20 , x + 320 , y + 58 + i * 20, DISP_PRIO_BOX2 )){
				if( mouse.onceState & MOUSE_LEFT_CRICK ){
					sprintf_s(tmpMsg,"J|%d",roomIndex[i]);
					lssproto_CHATROOM_send ( sockfd , tmpMsg ) ; 
					SelRoomBtn = 0;         //关闭选择频道视窗
				}
			}
		}

		StockFontBuffer( x + 30, y + 50 + i * 20, FONT_PRIO_FRONT,5 , "  建立新的聊天室", 0 );
		if( MakeHitBox(x + 30 ,y + 47 + i * 20 , x + 340 , y + 68 + i * 20, DISP_PRIO_BOX2 )){
			if( mouse.onceState & MOUSE_LEFT_CRICK )
				initSetRoomName();
		}
	}
}
#endif

void SwapOrder(int pos1,int pos2 )
{
	char    tempName[CHAR_NAME_LEN+1];
	int     tempIndex;
	char    tempNick[CHAR_FREENAME_LEN+1];

	strcpy(tempName,chatInfo.memberName[pos1]);
	strcpy(tempNick,chatInfo.nickName[pos1]);
	tempIndex = chatInfo.memberIndex[pos1];

	strcpy(chatInfo.memberName[pos1],chatInfo.memberName[pos2]);
	strcpy(chatInfo.nickName[pos1],chatInfo.nickName[pos2]);
	chatInfo.memberIndex[pos1] = chatInfo.memberIndex[pos2];

	strcpy(chatInfo.memberName[pos2],tempName);
	strcpy(chatInfo.nickName[pos2],tempNick);
	chatInfo.memberIndex[pos2] = tempIndex;
}

void InitRoomInfo()
{
	for (int i=0; i<50; i++){
		chatInfo.memberIndex[i] = 0;
		strcpy(chatInfo.memberName[i],"");
		strcpy(chatInfo.nickName[i],"");
	}
	chatInfo.chiefFlag = 0;
	chatInfo.chiefIndex = 0;
	chatInfo.memberNum = 0;
	firMemNo = 0;
}

void InitCreateChatRoom(char *msg)		//初始化聊天室视窗
{
	char temp[64],*temp1;
	chatInfo.chiefFlag = 0;
	InitRoomInfo();					    //初始化参数
	chatRoomBtn = 1;
	getStringToken( msg, '|', 2 , sizeof(temp) -1 ,temp );
	char *sss;
	if((temp1 = strtok_s(temp,"r",&sss))!= NULL ){
		strcpy(chatInfo.roomNo,temp1);
		strcpy(pc.chatRoomNum,chatInfo.roomNo);

		getStringToken( msg, '|', 3 , sizeof(chatInfo.roomName) -1 ,chatInfo.roomName );

		chatInfo.chiefIndex = getIntegerToken( msg, '|', 4 );

		getStringToken( msg, '|', 5 , sizeof(temp) -1 ,temp );
		strcpy(chatInfo.chiefName,temp);

		getStringToken( msg, '|', 6 , sizeof(temp) -1 ,temp );


		char *sss;
		if( (temp1 = strtok_s(temp,"p",&sss))!= NULL )
			chatInfo.memberNum = atoi(temp1);
		int i;
		for(i = 0; i< chatInfo.memberNum; i ++){
			getStringToken( msg, '|', 7+i*3, sizeof(temp) -1 ,temp );
			if ((temp1 = strchr(temp,'I')) == NULL )
				break;
			chatInfo.memberIndex[i] = atoi(temp1+1);		
			getStringToken( msg, '|', 8+i*3, sizeof(chatInfo.memberName[i]) -1 
				,chatInfo.memberName[i] );
			getStringToken( msg, '|', 9+i*3, sizeof(chatInfo.nickName[i]) -1 
				,chatInfo.nickName[i] );
			if(i > 0 && chatInfo.memberIndex[i] == chatInfo.chiefIndex)
				SwapOrder(i, 0);                 //室长排序
			secretFlag = FALSE;			  
			selChar = -1;
		}
		chatInfo.chiefFlag = getIntegerToken( msg, '|', 9 + i * 3 - 2 );
	}
#ifdef _CHANNEL_MODIFY
	pc.etcFlag |= PC_ETCFLAG_CHAT_CHAT;
#endif
}

#ifdef _CHATROOMPROTOCOL
void ChatRoomWnd( void )			//聊天室视窗
{
	char tmpMsg[STR_BUFFER_SIZE];
	int nameColor;
	int scrlLenth = 183;

	if(pChtChanlWnd == NULL){
		pChtChanlWnd = MakeWindowDisp( 350, 60, 540, 456, 0, -1 );
	}
	else if( pChtChanlWnd->hp > 0 ){
		int	x = pChtChanlWnd->x + 2; 
		int y = pChtChanlWnd->y - 5; 

		int scrTop = y + 69;           //Scroll高度

		StockDispBuffer(  ( ( WINDOW_DISP *)pChtChanlWnd->pYobi )->mx - 120, ( ( WINDOW_DISP *)pChtChanlWnd->pYobi )->my -28, DISP_PRIO_MENU, CG_FIELD_CHATROOM_PANEL, 1 );
		char title[32] = {""};

		sprintf_s(title,"%s" ,chatInfo.roomName);

		StockFontBuffer( x + 140 - strlen(title) * 4 , y + 50, FONT_PRIO_FRONT,4 , title, 0 );

		if(!BtnType){
			closeBtn = CG_FIELD_CLOSE_BTN_UP;
			leaveBtn = CG_FIELD_LEAVE_BTN_UP;
			scrlHBtn = CG_FIELD_SCROLL_HUP;
			scrlLBtn = CG_FIELD_SCROLL_LUP;

			if(chatInfo.chiefFlag == 1){									//队长专有按钮
				delBtn = CG_FIELD_DELETE_BTN_UP;
				if(secretFlag && chatInfo.chiefIndex != chatInfo.memberIndex[selChar]){
					outBtn = CG_FIELD_OUTMEMBER_BTN_UP;
					chaBtn = CG_FIELD_CHANGECHIEF_BTN_UP;
				}
				else{
					outBtn = CG_FIELD_OUTMEMBER_BTN_DISABLE;
					chaBtn = CG_FIELD_CHANGECHIEF_BTN_DISABLE;
				}
			}else{
				outBtn = CG_FIELD_OUTMEMBER_BTN_DISABLE;
				chaBtn = CG_FIELD_CHANGECHIEF_BTN_DISABLE;
				delBtn = CG_FIELD_DELETE_BTN_DISABLE;
			}
		}
		ChatRoomBtn[0] = StockDispBuffer( x + 70 , y + 335, DISP_PRIO_IME3, closeBtn, 2 );
		ChatRoomBtn[1] = StockDispBuffer( x + 230, y + 335, DISP_PRIO_IME3, leaveBtn, 2 );
		ChatRoomBtn[2] = StockDispBuffer( x + 150, y + 305, DISP_PRIO_IME3, outBtn, 2 );
		ChatRoomBtn[3] = StockDispBuffer( x + 230, y + 305, DISP_PRIO_IME3, chaBtn, 2 );
		ChatRoomBtn[4] = StockDispBuffer( x + 70, y + 305, DISP_PRIO_IME3, delBtn, 2 );
		ChatRoomBtn[5] = StockDispBuffer( x + 267, scrTop + scrlBtnIndex, DISP_PRIO_IME3, CG_FIELD_CHANNEL_SCROLL ,2);
		ChatRoomBtn[6] = StockDispBuffer( x + 267, scrTop -20, DISP_PRIO_IME3, scrlHBtn ,2);
		ChatRoomBtn[7] = StockDispBuffer( x + 267, y + 274, DISP_PRIO_IME3, scrlLBtn ,2);

		for(int i = 0 ; i < 10 ; i++ ){	
			if(chatInfo.memberNum - 1 < i + firMemNo )
				break;
			//人名变色
			if( MakeHitBox(x + 20 ,y + 77 + i * 20 , x + 250 , y + 96+ i * 20, DISP_PRIO_BOX2 )){
				if( mouse.onceState & MOUSE_LEFT_CRICK ){
					if(strcmp(chatInfo.memberName[i + firMemNo], pc.name) != 0 || strcmp(chatInfo.nickName[i + firMemNo], pc.freeName) != 0){
						if(secretFlag == TRUE && selChar == i){
							strcpy(secretName,"");
							pNowStrBuffer->buffer[ 0 ] = NULL;
							pNowStrBuffer->cursor=0;
							pNowStrBuffer->cnt = 0;
							StrToNowStrBuffer1(secretName);
							secretFlag = FALSE;
							selChar = -1;
							TalkMode = 0;
						}else{
							sprintf_s(secretName,"%s ",chatInfo.memberName[i+ firMemNo]);
							pNowStrBuffer->buffer[ 0 ] = NULL;
							pNowStrBuffer->cursor=0;
							pNowStrBuffer->cnt = 0;
							StrToNowStrBuffer1(secretName);
							secretFlag = TRUE;
							TalkMode = 1;
							selChar = i + firMemNo;
						}
					}
				}
			}

			if(secretFlag && selChar == i + firMemNo)
				nameColor = 4;
			else
				nameColor = 5;

			StockFontBuffer( x + 30, y + 80 + i * 20, FONT_PRIO_FRONT,nameColor , chatInfo.memberName[i + firMemNo], 0 );
			StockFontBuffer( x + 160, y + 80 + i * 20, FONT_PRIO_FRONT,nameColor , chatInfo.nickName[i + firMemNo], 0 );

		}

		for( int i = 0; i < 8; i++){
			if( i == 2 &&  chatInfo.chiefFlag == 0 )		//不是队长则跳过三个按钮
				i = 5;
			else if( i == 2 && chatInfo.chiefFlag == 1 && selChar == -1)			//是队长未选人
				i = 4;

			if( HitDispNo == ChatRoomBtn[ i ] ) {
				if( mouse.onceState & MOUSE_LEFT_CRICK ) {
					switch(i){
					case 0:
						closeBtn = CG_FIELD_CLOSE_BTN_DOWN;
						break;
					case 1:
						leaveBtn = CG_FIELD_LEAVE_BTN_DOWN;
						break;
					case 2:
						outBtn = CG_FIELD_OUTMEMBER_BTN_DOWN;
						break;
					case 3:
						chaBtn = CG_FIELD_CHANGECHIEF_BTN_DOWN;
						break;
					case 4:
						delBtn = CG_FIELD_DELETE_BTN_DOWN;
						break;
					case 5:
						scrollFlag = TRUE;
						break;
					case 6:
						if(firMemNo >0)
						{
							firMemNo --;
							scrlBtnIndex = (scrlLenth/(chatInfo.memberNum -10))  * firMemNo;
							scrlHBtn = CG_FIELD_SCROLL_HDOWN;
						}
						break;
					case 7:
						if( chatInfo.memberNum > firMemNo + 10 && chatInfo.memberNum > 10)
						{
							firMemNo ++;
							scrlBtnIndex = (scrlLenth/(chatInfo.memberNum -10))  * firMemNo;
							scrlLBtn = CG_FIELD_SCROLL_LDOWN;
						}
						break;
					}
					BtnNo = i;
					BtnType = TRUE;

				}else if(mouse.onceState & MOUSE_LEFT_CRICK_UP){
					switch(i){
					case 0:						//关闭视窗
						DeathMenuAction();
						break;

					case 1:						//离开频道
						if(chatInfo.chiefFlag == 1)
							StockChatBufferLine(  "室长不得离开聊天室,如欲离开请换别人当室长！" , FONT_PAL_RED);
						else{
							lssproto_CHATROOM_send ( sockfd , "L|" ) ;
							strcpy(pc.chatRoomNum,""); 
							DeathMenuAction();
							StockChatBufferLine(  "你已离开聊天室" , FONT_PAL_BLUE);
							TalkMode = 0;
#ifdef _CHANNEL_MODIFY
							pc.etcFlag &= ~PC_ETCFLAG_CHAT_CHAT;
#endif
						}
						break;

					case 2:						//踢人
						sprintf_s(tmpMsg,"K|%d", chatInfo.memberIndex[selChar]);
						lssproto_CHATROOM_send ( sockfd , tmpMsg ) ;
						break;

					case 3:					//换队长
						sprintf_s(tmpMsg,"M|%d", chatInfo.memberIndex[selChar]);
						lssproto_CHATROOM_send ( sockfd , tmpMsg );
						TalkMode = 0;
						break;

					case 4:					//删除频道
						lssproto_CHATROOM_send ( sockfd ,"D|") ;
						strcpy(pc.chatRoomNum,"");
						TalkMode = 0;
#ifdef _CHANNEL_MODIFY
						pc.etcFlag &= ~PC_ETCFLAG_CHAT_CHAT;
#endif
						break;
					}
					secretFlag = FALSE;
					selChar = -1;

				}
			}
		}

		if(mouse.onceState & MOUSE_LEFT_CRICK_UP || HitDispNo != ChatRoomBtn[ BtnNo ])
			BtnType = FALSE;

		if(mouse.onceState & MOUSE_LEFT_CRICK_UP || mouse.nowPoint.x < x )
			scrollFlag =FALSE;

		if(scrollFlag){
			if(	mouse.nowPoint.y > scrTop -2 && mouse.nowPoint.y < (scrTop + scrlLenth + 4) ){
				if(chatInfo.memberNum > 10){
					firMemNo = (mouse.nowPoint.y - scrTop) / (scrlLenth/(chatInfo.memberNum -10));
					scrlBtnIndex = mouse.nowPoint.y - scrTop;	
				}else
					scrlBtnIndex = 0;
			}
		}
	}
}
#endif

void initAssentWnd(char *data)			//要求加入视窗
{
	assentFlag = TRUE;
	char temp[64];
	getStringToken( data, '|', 2 , sizeof(temp) -1 ,temp );
	sprintf_s(memInfo,"%s 申请加入",temp);
	memIndex = getIntegerToken( data, '|', 3);
}

#ifdef _CHATROOMPROTOCOL
void AssentWnd(void)                    //要求加入视窗
{
	int i = 0;
	char tmpMsg[128] = {""};

	int AssentBtn[2];
	int x,y;
	if(pAssentWnd == NULL)
		pAssentWnd = MakeWindowDisp( 270, 0, 3, 2 , 2, 0 );
	else if( pAssentWnd->hp > 0){
		x = pAssentWnd->x;
		y = pAssentWnd->y;

		StockFontBuffer( x + 20 , y + 30, FONT_PRIO_FRONT,5 , memInfo, 0 );
		AssentBtn[0] = StockDispBuffer( x + 90, y + 60, DISP_PRIO_IME3, CG_FIELD_AGREE_BTN, 2 );
		AssentBtn[1] = StockDispBuffer( x + 100, y + 60, DISP_PRIO_IME3, CG_FIELD_DISAGREE_BTN, 2 );

		for( i = 0; i < 2; i++){
			if( HitDispNo == AssentBtn[ i ] ){
				if( mouse.onceState & MOUSE_LEFT_CRICK ){
					if(i == 0){
						sprintf_s(tmpMsg,"A|%d|1", memIndex);
						lssproto_CHATROOM_send ( sockfd ,tmpMsg) ; 
					}
					else{
						sprintf_s(tmpMsg,"A|%d|0", memIndex);
						lssproto_CHATROOM_send ( sockfd , tmpMsg ) ; 
					}
					DeathAction(pAssentWnd);
					pAssentWnd = NULL;
					assentFlag = FALSE;
				}				
			}
		}
	}
}
#endif

#ifdef _CHANNEL_MODIFY
void SaveChatData(char *msg,char KindOfChannel,bool bCloseFile);
#endif

void InitRecvMsg(char *data)
{
	char temp[STR_BUFFER_SIZE];
	char msg[STR_BUFFER_SIZE];
	getStringToken( data, '|', 2 , sizeof(temp) -1 ,temp );
#ifndef _CHANNEL_MODIFY
	sprintf_s(msg,"[频道]%s",temp);
#else
	sprintf_s(msg,"[聊]%s",temp);
	TradeTalk(msg);
	SaveChatData(msg,'R',false);
#endif
	StockChatBufferLine( msg, 2);
}

#ifdef _CHATROOMPROTOCOL			// (不可开) Syu ADD 聊天室频道
void lssproto_CHATROOM_recv ( int fd, char *data)
{
	char type[3] = {""};
	int i = 0;
	getStringToken( data, '|', 1, sizeof( type ) - 1, type);
	switch(type[0]){
	case 'B':
		InitSelChatRoom(data);
		break;
	case 'D':						//Delete
		chatRoomBtn = 0;
		strcpy(pc.chatRoomNum,"");
		StockChatBufferLine(  "聊天室已被删除！" , FONT_PAL_RED);
#ifdef _CHANNEL_MODIFY
		pc.etcFlag &= ~PC_ETCFLAG_CHAT_CHAT;
#endif
		break;
	case 'T':
		InitRecvMsg(data);    //处理讯息
		break;
	case 'K'://剔除
		chatRoomBtn = 0;
		strcpy(pc.chatRoomNum,"");
		StockChatBufferLine( "你已被室长踢出聊天室！" , FONT_PAL_RED);
#ifdef _CHANNEL_MODIFY
		pc.etcFlag &= ~PC_ETCFLAG_CHAT_CHAT;
#endif
		break;
	case 'J'://加入申请
		if(assentFlag == FALSE)
		{
			DeathAction(pAssentWnd);
			pAssentWnd = NULL;
			initAssentWnd(data);
		}
		break;
	case 'R':			//更新聊天室资讯
		InitCreateChatRoom(data);
		break;
	}
}
#endif

#endif

#ifdef _TIMEBAR_FUNCTION
/***********************************************************
参数:
title :  TimeBar 标头  范围32bit
range :  TimeBar 范围
回传值:	 TimeBar 识别用.用以设定正确的值

barHolder[timBarIdent] : 可用以判断现在是否为此函式的拥有者
***********************************************************/
int SetTimeBar(char *title, int range)
{
	int timBarIdent = 0;
	if(strlen(title) > 32)
		return -1;
	if(timeBarFlag = TRUE){
		DeathAction(pTimeBarWnd);
		pTimeBarWnd = NULL;
		if(barHolder[0]){
			barHolder[0] = FALSE;
			barHolder[1] = TRUE;
			timBarIdent = 1;
		}
		else{
			barHolder[1] = FALSE;
			barHolder[0] = TRUE;
			timBarIdent = 0;
		}
	}
	else{
		barHolder[0] = TRUE;
		timBarIdent = 0;
	}
	strcpy(timeBarTitle , title);
	timeBarRange = range;
	timeBarFlag  = TRUE;

	return timBarIdent;
}
/*******************************************
return -1: 注意!!代表无法设定现在的位址,可能被其他程式占用
return  1: 已经完成 100 %
*******************************************/
int SetTimeBarPos(int timBarIdent, int CurPos)
{
	if(!barHolder[timBarIdent] )//|| CurPos > timeBarRange)
		return -1;
	timeBarCurPos = 20 * CurPos / timeBarRange;  //20为bar的单位总数.
	if(timeBarCurPos > 20){
		timeBarCurPos = 20;
		return 1;				//已经到达100 %了
	}
	return 0;
}

void DrawTimeBar()
{
	if(pTimeBarWnd == NULL){
		pTimeBarWnd = MakeWindowDisp( 270, 0, 3, 2 , 2, 0 );
	}
	else if( pTimeBarWnd->hp > 0){
		int x = pTimeBarWnd->x;
		int y = pTimeBarWnd->y;
		char percent[4] = { "" };
		sprintf_s(percent, "%d", (timeBarCurPos * 5));
		StockFontBuffer(x + 20, y + 30, FONT_PRIO_FRONT, 5, timeBarTitle, 0);
		StockFontBuffer(x + 50, y + 50, FONT_PRIO_FRONT, 5, percent, 0);
		for (int i = 0; i < timeBarCurPos; i++)
			StockDispBuffer(x + i * 10, y + 60, DISP_PRIO_IME3, CG_TIMEBAR_UNIT, 2);
	}
}
#endif

#ifdef _STANDBYPET
void send_StandBy_Pet(void)
{
	int	standbypet = 0;
	int i, cnt = 0;
	for (i = 0; i <MAX_PET; i++){
		if (pc.selectPetNo[i] == TRUE) {
			cnt++;
			if (cnt > 4)
				break;
			standbypet |= (1 << i);
		}
	}
	lssproto_SPET_send(sockfd, standbypet);
	StandbyPetSendFlag = TRUE;
}
#endif
/* =========================================

注意!!
这个档案已经超过1万5千多行了，造成VC编辑器效率低落，
请尽量不要在后面增加新Code了。

~Robin~

所以请加到menu2.cpp  by  Change
========================================= */

