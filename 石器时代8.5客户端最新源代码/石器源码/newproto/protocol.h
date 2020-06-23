#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

int SaDispatchMessage(int fd, char *encoded);

#define SEPARATOR	";"

#define LSSPROTO_W_SEND 	0
#define LSSPROTO_W2_SEND 	1
#define LSSPROTO_XYD_RECV 	2
#define LSSPROTO_EV_SEND 	3
#define LSSPROTO_EV_RECV 	4
#define LSSPROTO_EN_SEND 	5
#define LSSPROTO_DU_SEND 	6
#define LSSPROTO_EN_RECV 	7
#define LSSPROTO_EO_SEND 	8
#define LSSPROTO_BU_SEND 	9
#define LSSPROTO_JB_SEND 	10
#define LSSPROTO_LB_SEND 	11
#define LSSPROTO_RS_RECV 	12
#define LSSPROTO_RD_RECV 	13
#define LSSPROTO_B_SEND 	14
#define LSSPROTO_B_RECV 	15
#define LSSPROTO_SKD_SEND 	16
#define LSSPROTO_ID_SEND 	17
#define LSSPROTO_PI_SEND 	18
#define LSSPROTO_DI_SEND 	19
#define LSSPROTO_DG_SEND 	20
#define LSSPROTO_DP_SEND 	21
#define LSSPROTO_I_RECV 	22
#define LSSPROTO_MI_SEND 	23
#define LSSPROTO_SI_RECV 	24
#define LSSPROTO_MSG_SEND 	25
#define LSSPROTO_MSG_RECV 	26
#define LSSPROTO_PMSG_SEND 	27
#define LSSPROTO_PME_RECV 	28
#define LSSPROTO_AB_SEND 	29
#define LSSPROTO_AB_RECV 	30
#define LSSPROTO_ABI_RECV 	31
#define LSSPROTO_DAB_SEND 	32
#define LSSPROTO_AAB_SEND 	33
#define LSSPROTO_L_SEND 	34
#define LSSPROTO_TK_SEND 	35
#define LSSPROTO_TK_RECV 	36
#define LSSPROTO_MC_RECV 	37
#define LSSPROTO_M_SEND 	38
#define LSSPROTO_M_RECV 	39
#define LSSPROTO_C_SEND 	40
#define LSSPROTO_C_RECV 	41
#define LSSPROTO_CA_RECV 	42
#define LSSPROTO_CD_RECV 	43
#define LSSPROTO_R_RECV 	44
#define LSSPROTO_S_SEND 	45
#define LSSPROTO_S_RECV 	46
#define LSSPROTO_D_RECV 	47
#define LSSPROTO_FS_SEND 	48
#define LSSPROTO_FS_RECV 	49
#define LSSPROTO_HL_SEND 	50
#define LSSPROTO_HL_RECV 	51
#define LSSPROTO_PR_SEND 	52
#define LSSPROTO_PR_RECV 	53
#define LSSPROTO_KS_SEND 	54
#define LSSPROTO_KS_RECV 	55
#define LSSPROTO_AC_SEND 	56
#define LSSPROTO_MU_SEND 	57
#define LSSPROTO_PS_SEND 	58
#define LSSPROTO_PS_RECV 	59
#define LSSPROTO_ST_SEND 	60
#define LSSPROTO_DT_SEND 	61
#define LSSPROTO_FT_SEND 	62
#define LSSPROTO_SKUP_RECV 	63
#define LSSPROTO_SKUP_SEND 	64
#define LSSPROTO_KN_SEND 	65
#define LSSPROTO_WN_RECV 	66
#define LSSPROTO_WN_SEND 	67
#define LSSPROTO_EF_RECV 	68
#define LSSPROTO_SE_RECV 	69
#define LSSPROTO_SP_SEND 	70
#define LSSPROTO_CLIENTLOGIN_SEND 	71
#define LSSPROTO_CLIENTLOGIN_RECV 	72
#define LSSPROTO_CREATENEWCHAR_SEND 	73
#define LSSPROTO_CREATENEWCHAR_RECV 	74
#define LSSPROTO_CHARDELETE_SEND 	75
#define LSSPROTO_CHARDELETE_RECV 	76
#define LSSPROTO_CHARLOGIN_SEND 	77
#define LSSPROTO_CHARLOGIN_RECV 	78
#define LSSPROTO_CHARLIST_SEND 	79
#define LSSPROTO_CHARLIST_RECV 	80
#define LSSPROTO_CHARLOGOUT_SEND 	81
#define LSSPROTO_CHARLOGOUT_RECV 	82
#define LSSPROTO_PROCGET_SEND 	83
#define LSSPROTO_PROCGET_RECV 	84
#define LSSPROTO_PLAYERNUMGET_SEND 	85
#define LSSPROTO_PLAYERNUMGET_RECV 	86
#define LSSPROTO_ECHO_SEND 	87
#define LSSPROTO_ECHO_RECV 	88
#define LSSPROTO_SHUTDOWN_SEND 	89
#define LSSPROTO_NU_RECV 	90
#define LSSPROTO_TD_SEND 	91
#define LSSPROTO_TD_RECV 	92
#define LSSPROTO_FM_RECV 	93
#define LSSPROTO_FM_SEND 	94
#define LSSPROTO_WO_RECV	95
#define LSSPROTO_PETST_SEND	96
#define LSSPROTO_BM_SEND    97   // _BLACK_MARKET
#ifdef _MIND_ICON 
#define LSSPROTO_MA_SEND    98
#endif
#ifdef _FIX_DEL_MAP
#define LSSPROTO_DM_SEND    99   // 玩家抽地图送监狱
#endif
#ifdef _ITEM_CRACKER
#define LSSPROTO_IC_RECV    100   
#endif
#ifdef _MAGIC_NOCAST//沉默
#define LSSPROTO_NC_RECV    101   
#endif
#ifdef _CHECK_GAMESPEED
#define LSSPROTO_CS_SEND	103
#define LSSPROTO_CS_RECV	104
#endif

#ifdef _TEAM_KICKPARTY
#define LSSPROTO_KTEAM_SEND	106
#endif

#ifdef _PETS_SELECTCON
#define LSSPROTO_PETST_RECV	107
#endif
#ifdef _NEWREQUESTPROTOCOL			// (不可开) Syu ADD 新增Protocol要求细项
#define LSSPROTO_RESIST_SEND 108
#define LSSPROTO_RESIST_RECV 109
#endif
#ifdef _OUTOFBATTLESKILL			// (不可开) Syu ADD 非战斗时技能Protocol
#define LSSPROTO_BATTLESKILL_SEND 110
#define LSSPROTO_BATTLESKILL_RECV 111
#endif
#ifdef _CHATROOMPROTOCOL			// (不可开) Syu ADD 聊天室频道
#define LSSPROTO_CHATROOM_SEND 112
#define LSSPROTO_CHATROOM_RECV 113
#endif


#define LSSPROTO_SPET_SEND 	114		// Robin 待机宠
#define LSSPROTO_SPET_RECV 	115

#ifdef _STREET_VENDOR
#define LSSPROTO_STREET_VENDOR_SEND 	116		// 摆摊功能
#define LSSPROTO_STREET_VENDOR_RECV 	117
#endif
		
#ifdef _JOBDAILY
#define LSSPROTO_JOBDAILY_SEND 	121		// CYG　任务日志功能
#define LSSPROTO_JOBDAILY_RECV 	120
#endif

#ifdef _TEACHER_SYSTEM
#define LSSPROTO_TEACHER_SYSTEM_SEND 	122		// 导师功能
#define LSSPROTO_TEACHER_SYSTEM_RECV 	123
#endif

#ifdef _ADD_STATUS_2
#define LSSPROTO_S2_SEND 	124
#define LSSPROTO_S2_RECV 	125
#endif

#ifdef _ITEM_FIREWORK
#define LSSPROTO_FIREWORK_RECV 			126
#endif
#ifdef _PET_ITEM
#define LSSPROTO_PET_ITEM_EQUIP_SEND 	127
#endif
#ifdef _MOVE_SCREEN
#define LSSPROTO_MOVE_SCREEN_RECV		128
#endif
#ifdef _GET_HOSTNAME
#define LSSPROTO_HOSTNAME_SEND			129
#define LSSPROTO_HOSTNAME_RECV			130
#endif
#ifdef _THEATER
#define LSSPROTO_THEATER_DATA_RECV		131
#define LSSPROTO_THEATER_DATA_SEND		132
#endif

#ifdef _NPC_MAGICCARD
#define LSSPROTO_MAGICCARD_ACTION_RECV		133
#define LSSPROTO_MAGICCARD_DAMAGE_RECV		134
#endif


#ifdef _SECONDARY_WINDOW_DATA_
#define LSSPROTO_SECONDARY_WINDOW_RECV 	137
#endif

#ifdef _ICONBUTTONS_
#define LSSPROTO_TRUNTABLE_RECV 	138
#endif


#ifdef _ALCHEPLUS
#define LSSPROTO_ALCHEPLUS_SEND 135
#define LSSPROTO_ALCHEPLUS_RECV 136
#endif

#ifdef _NPC_DANCE
#define LSSPROTO_DANCEMAN_OPTION_RECV	137
#endif

#ifdef _HUNDRED_KILL
#define LSSPROTO_HUNDREDKILL_RECV       138
#endif

#ifdef _PK2007
#define LSSPROTO_PKLIST_SEND 	139
#define LSSPROTO_PKLIST_RECV 	140
#endif

#ifdef _CHARSIGNDAY_
#define LSSPROTO_SIGNDAY_SEND 141
#endif

#define LSSPROTO_CHAREFFECT_RECV 146
#ifdef _RED_MEMOY_
#define LSSPROTO_REDMEMOY_SEND 147
#define LSSPROTO_REDMEMOY_RECV 148
#endif
#ifdef _ANNOUNCEMENT_
#define LSSPROTO_DENGON_RECV	200
#endif
#ifdef _NEW_SYSTEM_MENU
#define LSSPROTO_SAMENU_RECV	201
#define LSSPROTO_SAMENU_SEND	202
#endif
#ifdef _NEWSHOP_
#define LSSPROTO_SHOPOK_SEND	208
#define LSSPROTO_SHOPOK_RECV	209
#endif
#ifdef _FAMILYBADGE_
#define	LSSPROTO_FAMILYBADGE_SEND 210
#define	LSSPROTO_FAMILYBADGE_RECV 211
#endif

#ifdef _CHARTITLE_
#define	LSSPROTO_CHARTITLE_SEND 212
#define	LSSPROTO_CHARTITLE_RECV 213
#endif

#ifdef _CHARTITLE_STR_
#define	LSSPROTO_CHARTITLE_SEND 212
#define	LSSPROTO_CHARTITLE_RECV 213
#endif

#ifdef _PETBLESS_
#define	LSSPROTO_VB_SEND 218
#define	LSSPROTO_VB_RECV 219
#endif

#ifdef _RIDEQUERY_//骑宠查询
#define LSSPROTO_RIDEQUERY_SEND 220
#endif

#ifdef _PET_SKINS
#define LSSPROTO_PETSKINS_SEND  221
#define LSSPROTO_PETSKINS_RECV  222
#endif


void lssproto_W_send(int fd,int x,int y,char* direction);
void lssproto_W2_send(int fd,int x,int y,char* direction);
//Cary say 战斗结束后的大地图座标
void lssproto_XYD_recv(int fd,int x,int y,int dir);
void lssproto_EV_send(int fd,int event,int seqno,int x,int y,int dir);
void lssproto_EV_recv(int fd,int seqno,int result);
void lssproto_EN_send(int fd,int x,int y);
void lssproto_DU_send(int fd,int x,int y);
void lssproto_EN_recv(int fd,int result,int field);
void lssproto_EO_send(int fd,int dummy);
void lssproto_BU_send(int fd,int dummy);
void lssproto_JB_send(int fd,int x,int y);
void lssproto_LB_send(int fd,int x,int y);
void lssproto_RS_recv(int fd,char* data);
void lssproto_RD_recv(int fd,char* data);
void lssproto_B_send(int fd,char* command);
void lssproto_B_recv(int fd,char* command);
void lssproto_SKD_send(int fd,int dir,int index);
void lssproto_ID_send(int fd,int x,int y,int haveitemindex,int toindex);
void lssproto_PI_send(int fd,int x,int y,int dir);
void lssproto_DI_send(int fd,int x,int y,int itemindex);
void lssproto_DG_send(int fd,int x,int y,int amount);
void lssproto_DP_send(int fd,int x,int y,int petindex);
void lssproto_I_recv(int fd,char* data);
void lssproto_MI_send(int fd,int fromindex,int toindex);
void lssproto_SI_recv(int fd,int fromindex,int toindex);
void lssproto_MSG_send(int fd,int index,char* message,int color);
//Cary say 收到普通邮件或宠物邮件
void lssproto_MSG_recv(int fd,int aindex,char* text,int color);
void lssproto_PMSG_send(int fd,int index,int petindex,int itemindex,char* message,int color);
void lssproto_PME_recv(int fd,int objindex,int graphicsno,int x,int y,int dir,int flg,int no,char* cdata);
void lssproto_AB_send(int fd);
void lssproto_AB_recv(int fd,char* data);
void lssproto_ABI_recv(int fd,int num,char* data);
void lssproto_DAB_send(int fd,int index);
void lssproto_AAB_send(int fd,int x,int y);
void lssproto_L_send(int fd,int dir);
void lssproto_TK_send(int fd,int x,int y,char* message,int color,int area);
void lssproto_TK_recv(int fd,int index,char* message,int color);
void lssproto_MC_recv(int fd,int fl,int x1,int y1,int x2,int y2,int tilesum,int objsum,int eventsum,char* data);
void lssproto_M_send(int fd,int fl,int x1,int y1,int x2,int y2);
void lssproto_M_recv(int fd,int fl,int x1,int y1,int x2,int y2,char* data);
void lssproto_C_send(int fd,int index);
void lssproto_C_recv(int fd,char* data);
void lssproto_CA_recv(int fd,char* data);
void lssproto_CD_recv(int fd,char* data);
void lssproto_R_recv(int fd,char* data);
void lssproto_S_send(int fd,char* category);
void lssproto_S_recv(int fd,char* data);
void lssproto_D_recv(int fd,int category,int dx,int dy,char* data);
void lssproto_FS_send(int fd,int flg);
void lssproto_FS_recv(int fd,int flg);
void lssproto_HL_send(int fd,int flg);
//战斗中是否要Help
void lssproto_HL_recv(int fd,int flg);
void lssproto_PR_send(int fd,int x,int y,int request);
void lssproto_PR_recv(int fd,int request,int result);
void lssproto_KS_send(int fd,int petarray);
//Cary say 指定那一只宠物出场战斗
void lssproto_KS_recv(int fd,int petarray,int result);

#ifdef _STANDBYPET
void lssproto_SPET_send(int fd,int standbypet);
void lssproto_SPET_recv(int fd,int standbypet,int result);
#endif

void lssproto_AC_send(int fd,int x,int y,int actionno);
void lssproto_MU_send(int fd,int x,int y,int array,int toindex);
void lssproto_PS_send(int fd,int havepetindex,int havepetskill,int toindex,char* data);
//Cary say 宠物合成
void lssproto_PS_recv(int fd,int result,int havepetindex,int havepetskill,int toindex);
void lssproto_ST_send(int fd,int titleindex);
void lssproto_DT_send(int fd,int titleindex);
void lssproto_FT_send(int fd,char* data);
//Cary say 取得可加的属性点数
void lssproto_SKUP_recv(int fd,int point);
void lssproto_SKUP_send(int fd,int skillid);
void lssproto_KN_send(int fd,int havepetindex,char* data);
void lssproto_WN_recv(int fd,int windowtype,int buttontype,int seqno,int objindex,char* data);
void lssproto_WN_send(int fd,int x,int y,int seqno,int objindex,int select,char* data);
void lssproto_EF_recv(int fd,int effect,int level,char* option);
void lssproto_SE_recv(int fd,int x,int y,int senumber,int sw);
void lssproto_SP_send(int fd,int x,int y,int dir);
#ifdef _NEW_CLIENT_LOGIN
void lssproto_ClientLogin_send(int fd,char* cdkey,char* passwd,char* mac,int selectServerIndex, char *ip);
#else
void lssproto_ClientLogin_send(int fd,char* cdkey,char* passwd);
#endif
void lssproto_ClientLogin_recv(int fd,char* result);

void lssproto_CreateNewChar_send(int fd,int dataplacenum,char* charname,int imgno,int faceimgno,int vital,int str,int tgh,int dex,int earth,int water,int fire,int wind,int hometown);
void lssproto_CreateNewChar_recv(int fd,char* result,char* data);
void lssproto_CharDelete_send(int fd,char* charname);
void lssproto_CharDelete_recv(int fd,char* result,char* data);
void lssproto_CharLogin_send(int fd,char* charname);
void lssproto_CharLogin_recv(int fd,char* result,char* data);
void lssproto_CharList_send(int fd);
void lssproto_CharList_recv(int fd,char* result,char* data);
void lssproto_CharLogout_send(int fd, int Flg);
void lssproto_CharLogout_recv(int fd,char* result,char* data);
void lssproto_ProcGet_send(int fd);
void lssproto_ProcGet_recv(int fd,char* data);
void lssproto_PlayerNumGet_send(int fd);
void lssproto_PlayerNumGet_recv(int fd,int logincount,int player);
void lssproto_Echo_send(int fd,char* test);
void lssproto_Echo_recv(int fd,char* test);
void lssproto_Shutdown_send(int fd,char* passwd,int min);
void lssproto_NU_recv(int fd, int AddCount);
void lssproto_TD_send(int fd, char* data);
void lssproto_TD_recv(int fd, char* data);
void lssproto_FM_send(int fd, char* data);
void lssproto_FM_recv(int fd, char* data);
//Cary say 取得转生的特效
void lssproto_WO_recv( int fd, int effect );
void lssproto_PETST_send(int fd, int nPet, int sPet);// sPet  0:休息 1:等待 4:邮件
void lssproto_BM_send(int fd, int iindex);             // _BLACK_MARKET
#ifdef _FIX_DEL_MAP
void lssproto_DM_send(int fd);                         // WON ADD 玩家抽地图送监狱
#endif
#ifdef _MIND_ICON 
void lssproto_MA_send(int fd, int x, int y, int nMind);
#endif
#ifdef _ITEM_CRACKER 
void lssproto_IC_recv(int fd, int x, int y);
#endif
#ifdef _MAGIC_NOCAST//沉默
void lssproto_NC_recv(int fd,int flg);
#endif

#ifdef _CHECK_GAMESPEED
void lssproto_CS_send( int fd);
void lssproto_CS_recv( int fd, int deltimes);
int lssproto_getdelaytimes();
void lssproto_setdelaytimes( int delays);
#endif

#ifdef _TEAM_KICKPARTY
void lssproto_KTEAM_send( int fd,int si);
#endif

#ifdef _PETS_SELECTCON
void lssproto_PETST_recv( int fd,int petarray,int result );
#endif
#ifdef _CHATROOMPROTOCOL			// (不可开) Syu ADD 聊天室频道
void lssproto_CHATROOM_send( int fd , char *data );
void lssproto_CHATROOM_recv( int fd , char *data ) ; 
#endif
#ifdef _NEWREQUESTPROTOCOL			// (不可开) Syu ADD 新增Protocol要求细项
void lssproto_RESIST_send( int fd , char *data );
void lssproto_RESIST_recv( int fd , char *data ) ; 
#endif

#ifdef _ALCHEPLUS
void lssproto_ALCHEPLUS_send( int fd , char *data );
void lssproto_ALCHEPLUS_recv( int fd , char *data ); 
#endif

#ifdef _OUTOFBATTLESKILL			// (不可开) Syu ADD 非战斗时技能Protocol
void lssproto_BATTLESKILL_send( int fd, int SkillNum);
void lssproto_BATTLESKILL_recv( int fd, char *data) ; 
#endif
void lssproto_CHAREFFECT_recv ( int fd, char *data);

void lssproto_SPET_send(int fd, int standbypet); // Robin 待机宠
#ifdef _STREET_VENDOR
void lssproto_STREET_VENDOR_send(int fd,char *data);	// 摆摊功能
void lssproto_STREET_VENDOR_recv(int fd,char *data);	// 摆摊功能
#endif

#ifdef _JOBDAILY
void lssproto_JOBDAILY_send(int fd,char *data);	
void lssproto_JOBDAILY_recv(int fd,char *data);	
#endif

#ifdef _FAMILYBADGE_
void lssproto_FamilyBadge_send(int fd);
void lssproto_FamilyBadge_recv(char *data);
#endif

#ifdef _TEACHER_SYSTEM
void lssproto_TEACHER_SYSTEM_send(int fd,char *data);	// 导师功能
void lssproto_TEACHER_SYSTEM_recv(int fd,char *data);
#endif

#ifdef _ADD_STATUS_2
void lssproto_S2_send(int fd,char *data);
void lssproto_S2_recv(int fd,char *data);
#endif

#ifdef _ITEM_FIREWORK
void lssproto_Firework_recv(int fd, int nCharaindex, int nType, int nActionNum);	// 烟火功能
#endif
#ifdef _PET_ITEM
void lssproto_PetItemEquip_send(int fd, int nGx, int nGy, int nPetNo, int nItemNo, int nDestNO);	// 宠物装备功能
#endif
#ifdef _THEATER
void lssproto_TheaterData_recv(int fd, char *pData);
#endif
#ifdef _MOVE_SCREEN
void lssproto_MoveScreen_recv(int fd, BOOL bMoveScreenMode, int iXY);	// client 移动荧幕
#endif
#ifdef _GET_HOSTNAME
void lssproto_HostName_send(int fd);
#endif

#ifdef _NPC_MAGICCARD
void lssproto_MagiccardAction_recv(int fd, char *data);	//魔法牌功能
void lssproto_MagiccardDamage_recv(int fd, int position , int damage , int offsetx, int offsety);
#endif

#ifdef  _NPC_DANCE
void lssproto_DancemanOption_recv(int fd, int option );	//动一动状态
#endif



#ifdef _ANNOUNCEMENT_

void lssproto_DENGON_recv(char *data, int colors, int nums);
#endif
#ifdef _HUNDRED_KILL
void lssproto_hundredkill_recv( int fd, int flag );
#endif
#ifdef _PK2007
void lssproto_pkList_send(int fd);
void lssproto_pkList_recv(int fd,int count,char* data);
#endif
#ifdef _NEW_SYSTEM_MENU
void lssproto_SaMenu_send(int fd, int index);
#endif
#endif


#ifdef _PETBLESS_
void lssproto_petbless_send(int fd ,int petpos,int type);
#endif
#ifdef _RIDEQUERY_
void lssproto_RideQuery_send(int fd);
#endif
#ifdef _PET_SKINS
void lssproto_PetSkins_recv(char *data);
#endif



#ifdef _CHARSIGNDAY_
void lssproto_SignDay_send(int fd);
#endif

