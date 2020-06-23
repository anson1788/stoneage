#include "version.h"
#include <string.h>
#include <time.h>
#include "char.h"
#include "object.h"
#include "char_base.h"
#include "npcutil.h"
#include "lssproto_serv.h"
#include "saacproto_cli.h"
#include "map_deal.h"
#include "readmap.h"
#include "battle.h"
#include "log.h"
#include "enemy.h"
#include "handletime.h"
#include "npc_eventaction.h"
#include "family.h"
#include "configfile.h"
#include "npc_transmigration.h"
#include "npc_exchangeman.h"
#ifdef _ANGEL_SUMMON
extern struct MissionInfo missionlist[MAXMISSION];
extern struct MissionTable missiontable[MAXMISSIONTABLE];
#endif

extern char* CHAR_setintdata[CHAR_DATAINTNUM];
extern char* CHAR_setchardata[CHAR_DATACHARNUM];
#ifdef _PROFESSION_SKILL			// WON ADD 人物职业技能
#include "profession_skill.h"
#include "chatmagic.h"
#endif

enum{
	NPC_EVENTMODE_NONE,
	NPC_EVENTMODE_OPEN,
	NPC_EVENTMODE_EVENT,
};

typedef struct {
	char	arg[32];
	int		born;
	int 	dead;
}NPC_TimeMan;

#define YOAKE 700
#define NICHIBOTU 300
#define SHOUGO 125
#define YONAKA 500

static NPC_TimeMan	TimeTble[] = {
	{"ALLNIGHT",	NICHIBOTU+1,	YOAKE},
	{"ALLNOON",		YOAKE+1,		NICHIBOTU},
	{"AM",			YONAKA+1,		SHOUGO},
	{"PM",			SHOUGO+1,		YONAKA},
	{"FORE",		YOAKE+1,		SHOUGO},
	{"AFTER",		SHOUGO+1,		NICHIBOTU},
	{"EVNING",		NICHIBOTU+1,	YONAKA},
	{"MORNING",		YONAKA+1,		YOAKE},
	{"FREE",		0,				1024},
};

extern void BATTLE_changeRideImage( int index );
#ifdef _NEW_ITEM_
extern int CheckCharMaxItem(int charindex);
#endif

#define MAXNPCPOINT 10

BOOL ActionNpc_WarpPoint(int meindex,int talker,char *npcarg)
{
	char data[1024], buf1[256], buf2[256];
	int i=1, j=0;
	BOOL EvFlg=FALSE;

	struct	{
		int FLOOR;
		int X;
		int Y;
	}Points[MAXNPCPOINT];
	
	for( i=0;i<MAXNPCPOINT;i++)	{
		Points[i].FLOOR = -1;
		Points[i].X = -1;
		Points[i].Y = -1;
	}
	//NPC_POINT
	if( NPC_Util_GetStrFromStrWithDelim( npcarg, "NPC_POINT", data, sizeof( data)) != NULL ) {
		int where=0;
		i=1;
		j=0;
		while( getStringFromIndexWithDelim( data, ";", i, buf1, sizeof( buf1)) != FALSE )	{
			i++;
			getStringFromIndexWithDelim( buf1, ",", 1, buf2, sizeof( buf2));
			Points[j].FLOOR = atoi( buf2);
			getStringFromIndexWithDelim( buf1, ",", 2, buf2, sizeof( buf2));
			Points[j].X = atoi( buf2);
			getStringFromIndexWithDelim( buf1, ",", 3, buf2, sizeof( buf2));
			Points[j].Y = atoi( buf2);
			j++;
			if( j >= MAXNPCPOINT )
				break;
		}
		where = RAND( 0, j-1);
		if( MAP_IsValidCoordinate( Points[where].FLOOR,	Points[where].X, Points[where].Y ) == TRUE ){
			CHAR_warpToSpecificPoint( meindex, Points[where].FLOOR,	Points[where].X, Points[where].Y);
			EvFlg = TRUE;
		}else	{
			for( i=0; i<10; i++)	{
				print("%d.[%d,%d,%d]\n", i, Points[i].FLOOR, Points[i].X, Points[i].Y);
			}
		}
	}

	if( NPC_Util_GetStrFromStrWithDelim( npcarg, "WARPPOINT", data, sizeof( data)) != NULL ) {
		int where=0;
		i=1;
		j=0;
		while( getStringFromIndexWithDelim( data, ";", i, buf1, sizeof( buf1)) != FALSE )	{
			i++;
			getStringFromIndexWithDelim( buf1, ",", 1, buf2, sizeof( buf2));
			Points[j].FLOOR = atoi( buf2);
			getStringFromIndexWithDelim( buf1, ",", 2, buf2, sizeof( buf2));
			Points[j].X = atoi( buf2);
			getStringFromIndexWithDelim( buf1, ",", 3, buf2, sizeof( buf2));
			Points[j].Y = atoi( buf2);
			j++;
			if( j >= MAXNPCPOINT )
				break;
		}
		where = RAND( 0, j-1);
		if( MAP_IsValidCoordinate( Points[where].FLOOR,	Points[where].X, Points[where].Y ) == TRUE ){
			CHAR_warpToSpecificPoint( talker, Points[where].FLOOR,	Points[where].X, Points[where].Y);
			EvFlg = TRUE;
		}else	{
			for( i=0; i<10; i++)	{
				print("%d.[%d,%d,%d]\n", i, Points[i].FLOOR, Points[i].X, Points[i].Y);
			}
		}

	}
#if _ATTESTAION_ID == 35
	if( NPC_Util_GetStrFromStrWithDelim( npcarg, "WARPIPPOINT", data, sizeof( data)) != NULL ) {
		int fl,x,y, ipnum;
		i=1;
		j=0;
		if( getStringFromIndexWithDelim( data, ";", i, buf1, sizeof( buf1)) != FALSE )	{
			getStringFromIndexWithDelim( buf1, ",", 1, buf2, sizeof( buf2));
			fl = atoi( buf2);
			getStringFromIndexWithDelim( buf1, ",", 2, buf2, sizeof( buf2));
			x = atoi( buf2);
			getStringFromIndexWithDelim( buf1, ",", 3, buf2, sizeof( buf2));
			y = atoi( buf2);
			getStringFromIndexWithDelim( buf1, ",", 4, buf2, sizeof( buf2));
			ipnum = atoi( buf2);
		}

    int playernum = CHAR_getPlayerMaxNum();
    int num=0;
    for(i=0;i<playernum;i++){
			if(CHAR_CHECKINDEX(i) == FALSE) continue;
			if(CHAR_getInt( i, CHAR_FLOOR) == fl)
			{
				if(CONNECT_get_userip(getfdFromCharaIndex(i)) == CONNECT_get_userip(getfdFromCharaIndex(talker))){
					num++;
					if(num>=ipnum){
						CHAR_talkToCli( talker, -1, "你的IP数进入已满", CHAR_COLORYELLOW);
						return EvFlg;
					}
				}
			}
		}

		if( MAP_IsValidCoordinate( fl,	x, y ) == TRUE ){
			CHAR_warpToSpecificPoint( talker, fl,	x, y);
			EvFlg = TRUE;
		}else	{
			for( i=0; i<10; i++)	{
				print("%d.[%d,%d,%d]\n", ipnum, fl,	x, y);
			}
		}

	}
#endif
	return EvFlg;
}

BOOL ActionNpc_CheckMenuFree( int meindex, int toindex, char *npcarg, int nums)
{
	char buf1[1024], buf2[256];
	if( nums < 1 ) return FALSE;

	if( NPC_Util_GetStrFromStrWithDelim( npcarg, "CHECKPARTY", buf1, sizeof( buf1) ) != NULL){
		if( strstr( buf1, "TRUE") != 0 )	{
			if( Action_PartyCheck( meindex, toindex) == FALSE)	{
				CHAR_talkToCli( toindex, meindex, "请一个一个来！", CHAR_COLORYELLOW);
				return FALSE;
			}
		}
	}

#ifdef _NPC_TALK_CHECK
	if( NPC_Util_GetStrFromStrWithDelim( npcarg, "TALKCHECK", buf1, sizeof( buf1) ) != NULL){
		if(CHAR_getWorkInt(meindex, CHAR_WORK_TALKCHECK)==0){
			CHAR_setWorkInt(meindex, CHAR_WORK_TALKCHECK, getTalkCheck());
		}

		if(CHAR_getWorkInt(meindex, CHAR_WORK_TALKCHECK) != CHAR_getWorkInt(toindex, CHAR_WORK_TALKCHECK)){
			char buf[256];
			CHAR_setWorkInt(meindex, CHAR_WORK_TALKCHECK, getTalkCheck());
			sprintf(buf, "请输入验证数字:%d", CHAR_getWorkInt(meindex, CHAR_WORK_TALKCHECK));
			CHAR_talkToCli( toindex, meindex, buf, CHAR_COLORYELLOW);
			return FALSE;
		}else{
			CHAR_setWorkInt(meindex, CHAR_WORK_TALKCHECK, getTalkCheck());
		}
	}
#endif

	
	if( getStringFromIndexWithDelim( npcarg,"}", nums, buf1, sizeof( buf1)) == FALSE )
		return FALSE;

	if( NPC_Util_GetStrFromStrWithDelim( buf1, "FREE", buf2, sizeof( buf2) ) == NULL){
		strcpy(buf2, "LV>0");
	}


	if( NPC_ActionPassCheck( meindex, toindex, buf2) == FALSE )	{
		{
			CHAR_talkToCli( toindex, meindex, "条件不足！", CHAR_COLORYELLOW);
			return FALSE;
		}
	}
	return Action_RunDoEventAction( meindex, toindex, buf1);
}

BOOL ActionNpc_CheckFree( int meindex, int toindex, char *npcarg)
{
	char buf1[1024], buf2[256];
	if( NPC_Util_GetStrFromStrWithDelim( npcarg, "CHECKPARTY", buf1, sizeof( buf1) ) != NULL){
		if( strstr( buf1, "TRUE") != 0 )	{
			if( Action_PartyCheck( meindex, toindex) == FALSE)	{
				CHAR_talkToCli( toindex, meindex, "请一个一个来！", CHAR_COLORYELLOW);
				return FALSE;
			}
		}
	}
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "FREE", buf2, sizeof( buf2) ) == NULL){
		strcpy(buf2, "LV>0");
	}

	if( NPC_ActionPassCheck( meindex, toindex, buf2) == FALSE )	
	{
		CHAR_talkToCli( toindex, meindex, "条件不足！", CHAR_COLORYELLOW);
		return FALSE;
	}
	return Action_RunDoEventAction( meindex, toindex, buf1);
}

#ifdef _CHANNEL_MODIFY
extern int *piOccChannelMember;
#endif

BOOL Action_RunDoEventAction( int meindex, int toindex, char *buf1)
{
	char buf2[1256];

#if 1 // Robin NPC执行动作前再一次检查FREE条件
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "FREE", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionPassCheck( meindex, toindex, buf2 ) == FALSE && buf2[0] == '\n' )	{
			print("\n 改封包!?跳过NPC的FREE检查!!:%s:%d,%d,%d ",
					CHAR_getChar(toindex, CHAR_CDKEY),
					CHAR_getInt(toindex, CHAR_FLOOR),
					CHAR_getInt(toindex, CHAR_X),
					CHAR_getInt(toindex, CHAR_Y) );
			CHAR_warpToSpecificPoint(toindex,117,225,13);
			return FALSE;
		}
	}
#endif

#ifdef _NPC_INT_SET
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "INTSET", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionIntSet( toindex, buf2) == FALSE )
			return FALSE;
	}
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "INTCHANGE", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionIntChange( toindex, buf2) == FALSE )
			return FALSE;
	}
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "INTSHOW", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionIntShow( toindex, buf2) == FALSE )
			return FALSE;
	}
#endif

#ifdef _PLAYER_DIY_MAP
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "PLAYERDIYMAPSHOW", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionPlayerDiyMap( toindex, buf2) == FALSE )
			return FALSE;
	}
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "PLAYERDIYMAPBUY", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionPlayerDiyMapBuy( toindex, buf2) == FALSE )
			return FALSE;
	}
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "PLAYERDIYMAPLETTER", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionPlayerDiyMapLetter( toindex, buf2) == FALSE )
			return FALSE;
	}
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "PLAYERDIYMAPSEND", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionPlayerDiyMapSend( toindex, buf2) == FALSE )
			return FALSE;
	}
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "PLAYERDIYMAPADDNUM", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionPlayerDiyMapAddNum( toindex, buf2) == FALSE )
			return FALSE;
	}
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "PLAYERDIYMAPADDEXP", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionPlayerDiyMapAddExp( toindex, buf2) == FALSE )
			return FALSE;
	}
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "PLAYERDIYMAPADDDAY", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionPlayerDiyMapAddDay( toindex, buf2) == FALSE )
			return FALSE;
	}
#endif

#ifdef _LOTTERY_SYSTEM
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "LOTTERYCHANGE", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionLotteryChange( toindex, buf2 ) == FALSE )
			return FALSE;
	}
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "LOTTERYBUY", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionLotteryBuy( toindex, buf2 ) == FALSE )
			return FALSE;
	}
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "LOTTERYADDTIME", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionLotteryAddTime( toindex, buf2 ) == FALSE )
			return FALSE;
	}
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "LOTTERYSHOW", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionLotteryShow( toindex ) == FALSE )
			return FALSE;
	}
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "LOTTERYPLAYERSHOW", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionLotteryPlayerShow( toindex ) == FALSE )
			return FALSE;
	}
#endif
#ifdef _SHOW_ITEM_NAME
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "SHOWITEMNAME", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionShowItemName( toindex, buf2 ) == FALSE )
			return FALSE;
	}
#endif
#ifdef _SQL_BUY_FUNC
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "ONLINEBUY", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionOnlineBuy( toindex ) == FALSE )
			return FALSE;
	}
#endif

#ifdef _ONLINE_COST	
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "ONLINECOST", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionOnlineCost( toindex ) == FALSE )
			return FALSE;
	}
#endif

#ifdef _DEL_FAME
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "DelFame", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionDelFame( toindex, buf2) == FALSE )
			return FALSE;
	}
#endif
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "AddGold", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionAddGold2( toindex, buf2) == FALSE )
			return FALSE;
	}

#ifdef _GLORY_POINT
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "AddGlory", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionAddGlory( toindex, buf2) == FALSE )
			return FALSE;
	}
#endif

	if( NPC_Util_GetStrFromStrWithDelim( buf1, "DelGold", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionDelGold( toindex, buf2) == FALSE )
			return FALSE;
	}
#ifdef _MOMENTUM_NPC
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "DelMom", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionMomentum( toindex, buf2) == FALSE )
			return FALSE;
	}
#endif
#ifdef _DP_NPC
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "DelDP", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionDP( toindex, buf2) == FALSE )
			return FALSE;
	}
#endif
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "DelItem", buf2, sizeof( buf2)) != NULL ) {
		//print("\n DelItem:%s:%d ", __FILE__, __LINE__);
		if( NPC_ActionDelItem( toindex, buf2) == FALSE ) {
			//print("\n 改封包??NPC_ActionDelItem:%s:%s ", CHAR_getChar(toindex, CHAR_CDKEY), buf2);
			//CHAR_warpToSpecificPoint(toindex,117,225,13);
			return FALSE;
		}
	}

	//条件成立後npc所给的道具
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "AddItem", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionAddItem( toindex, buf2) == FALSE )
			return FALSE;
	}
#ifdef _VIP_SHOP
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "VipItem", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionVipItem( toindex, buf2) == FALSE )
			return FALSE;
	}
	
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "VipPet", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionVipPet( toindex, buf2) == FALSE )
			return FALSE;
	}
#endif	

#ifdef _NEW_VIP_SHOP
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "Vip_Item", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionNewVipItem( toindex, buf2) == FALSE )
			return FALSE;
	}
	
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "Vip_Pet", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionNewVipPet( toindex, buf2) == FALSE )
			return FALSE;
	}
#endif

#ifdef _TRANS_7_NPC
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "TRANS7", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionTrans7( toindex, buf2) == FALSE )
			return FALSE;
	}
#endif
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "DelPet", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionDelPet( toindex, buf2) == FALSE )
			return FALSE;
	}

#ifdef _NPC_NewDelPet
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "NewDelPet", buf2, sizeof( buf2)) != NULL ) {
        if( NPC_ActionNewDelPet( toindex, buf2) == FALSE )
			return FALSE;
	}
#endif

	if( NPC_Util_GetStrFromStrWithDelim( buf1, "AddPet", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionAddPet( toindex, buf2) == FALSE )
			return FALSE;
	}
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "EvEnd", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionSetEend( toindex, buf2) == FALSE )
			return FALSE;
	}
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "EvNow", buf2, sizeof( buf2)) != NULL ) {
		if( NPC_ActionSetNow( toindex, buf2) == FALSE )
			return FALSE;
	}

	if( NPC_Util_GetStrFromStrWithDelim( buf1, "Event_End", buf2, sizeof( buf2) ) != NULL) {
		if( NPC_ActionSetEend( toindex, buf2) == FALSE )	{
			return FALSE;
		}
	}

	if( NPC_Util_GetStrFromStrWithDelim( buf1, "Event_Now", buf2, sizeof( buf2) ) != NULL) {
		if( NPC_ActionSetNow( toindex, buf2) == FALSE )
			return FALSE;
	}


	if( NPC_Util_GetStrFromStrWithDelim( buf1, "EvClr", buf2, sizeof( buf2) ) != NULL) {
		if( NPC_ActionClearEvent( toindex, buf2) == FALSE )
			return FALSE;
	}

#ifdef _NPCCHANGE_PLAYERIMG
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "CHANGEBBI", buf2, sizeof( buf2) ) != NULL) {
		if( NPC_ActionChangePlayerBBI( meindex, toindex, buf2) == FALSE )
			return FALSE;
	}
#endif

	if( NPC_Util_GetStrFromStrWithDelim( buf1, "SetLastTalkelder", buf2, sizeof( buf2) ) != NULL) {
		CHAR_setInt( toindex, CHAR_LASTTALKELDER, atoi( buf2));
	}

#ifdef _TYPE_TOXICATION
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "TOXICATION", buf2, sizeof( buf2) ) != NULL) {
		int fd = getfdFromCharaIndex( toindex );
		if( strstr( buf2, "TRUE") != 0 ){
			setToxication( fd, 1);
			CHAR_setWorkInt( toindex, CHAR_WORKTOXICATION, 0 );
			if( NPC_Util_GetStrFromStrWithDelim( buf1, "TOXICATIONMSG", buf2, sizeof( buf2) ) != NULL) {
				CHAR_talkToCli( toindex, -1, buf1,  CHAR_COLORYELLOW);
			}
		}else	{
			CHAR_setWorkInt( toindex, CHAR_WORKTOXICATION, 0 );
			setToxication( fd, 0);
			if( NPC_Util_GetStrFromStrWithDelim( buf1, "DETOXIFYMSG", buf2, sizeof( buf2) ) != NULL) {
				CHAR_talkToCli( toindex, -1, buf2,  CHAR_COLORYELLOW);
			}
		}
	}
#endif

	if(NPC_Util_GetStrFromStrWithDelim( buf1, "GetRandItem", buf2, sizeof( buf2) ) != NULL ){
		int j=1, rand_cnt=1;
		char buf3[256];
		while( getStringFromIndexWithDelim( buf2 , "," , j, buf3, sizeof( buf3)) != FALSE ){
			j++;
			rand_cnt++;
		}
		NPC_ActionTreasureRandItemGet( meindex, toindex, rand_cnt, buf2);
	}

#ifdef _PROFESSION_SKILL			// WON ADD 人物职业技能
	//andy_add 2003/07/17
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "AddPFSkillPoint", buf2, sizeof( buf2) ) != NULL) {
		if( NPC_ActionAddProfessionSkillPoint( toindex, buf2 ) == FALSE ){
			return FALSE;
		}
	}
	//andy_add 2003/07/17
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "CleanProfession", buf2, sizeof( buf2) ) != NULL) {
		if( NPC_ActionCleanProfession( toindex, buf2 ) == FALSE ){
			sprintf( buf2, "处理职业参数失败!!" );
			CHAR_talkToCli( toindex, -1, buf2,  CHAR_COLORYELLOW);
			return FALSE;
		}
	}

	if( NPC_Util_GetStrFromStrWithDelim( buf1, "PROFESSION", buf2, sizeof( buf2) ) != NULL) {
		int p_class = atoi(buf2);
		// 判断职业
		if( CHAR_getInt( toindex, PROFESSION_CLASS ) != PROFESSION_CLASS_NONE && (p_class != 0) ){
			sprintf( buf2, "你已经有职业了!!" );
			CHAR_talkToCli( toindex, -1, buf2,  CHAR_COLORYELLOW);
			return FALSE;
		}

		if( CHAR_getInt( toindex, CHAR_SKILLUPPOINT ) >0 ){
			sprintf( buf2, "请将升级点数加完!!" );
			CHAR_talkToCli( toindex, -1, buf2,  CHAR_COLORYELLOW);
			return FALSE;
		}

		if( p_class > PROFESSION_CLASS_NONE && p_class < PROFESSION_CLASS_NUM ) //	1:战士 2:巫师 3:猎人
		{
			int i, value = 0;
			int skill_id = -1;

			// 还原点数
			if( PROFESSION_RESTORE_POINT( toindex ) != 1 )	return FALSE;

			// 设定职业
			CHAR_setInt( toindex, PROFESSION_CLASS, p_class );
#ifdef _CHANNEL_MODIFY
			if(CHAR_getInt(toindex,PROFESSION_CLASS) > 0){
				int i,pclass = CHAR_getInt(toindex,PROFESSION_CLASS) - 1;
				for(i=0;i<getFdnum();i++){
					if(*(piOccChannelMember + (pclass * getFdnum()) + i) == -1){
						*(piOccChannelMember + (pclass * getFdnum()) + i) = toindex;
						break;
					}
				}
			}
#endif			

			// 设定职业等级 1
			CHAR_setInt( toindex, PROFESSION_LEVEL, 1 );	
		
			switch( p_class ){
				case PROFESSION_CLASS_FIGHTER:	value = 0;	skill_id = 37; break;	// 勇士
				case PROFESSION_CLASS_WIZARD:	value = 10;	skill_id = 1;  break;	// 巫师
				case PROFESSION_CLASS_HUNTER:	value = 25;	skill_id = 47; break;	// 猎人
				default: break;
			}
/*
			// 给第一个职业技能
			if( skill_id != -1 ){
				if( PROFESSION_SKILL_ADDSK( toindex, skill_id, 10 ) == -1 ) return FALSE;
			}
*/
			// 给1点技能点数
			CHAR_setInt(toindex, PROFESSION_SKILL_POINT, 1 );
			CHAR_send_P_StatusString(  toindex, CHAR_P_STRING_DUELPOINT );


			// 增加抗性
			for( i=0; i<3; i++ )
				CHAR_setInt( toindex, PROFESSION_FIRE_R+i, CHAR_getInt( toindex, PROFESSION_FIRE_R+i) + value );

			// 巫师增加MP上限
			if( p_class == PROFESSION_CLASS_WIZARD ){
				CHAR_setInt( toindex , CHAR_MAXMP , 150 );
			}

			// 欢迎讯息
			if( NPC_Util_GetStrFromStrWithDelim( buf1, "OK_MSG", buf2, sizeof( buf2) ) == NULL){
				sprintf( buf2, "欢迎加入" );
				CHAR_talkToCli( toindex, -1, buf2,  CHAR_COLORYELLOW);
			}

			CHAR_sendCToArroundCharacter( CHAR_getWorkInt( toindex , CHAR_WORKOBJINDEX ));
		}
#if 1 // Robin add 消除职业
		else if( p_class == PROFESSION_CLASS_NONE)
		{
			int i;

			print(" ====清除职业==== ");
			
			// 还原点数
			//if( PROFESSION_RESTORE_POINT( toindex ) != 1 )	return FALSE;

#ifdef _CHANNEL_MODIFY
			if(CHAR_getInt(toindex,PROFESSION_CLASS) > 0){
				int i,pclass = CHAR_getInt(toindex,PROFESSION_CLASS) - 1;
				for(i=0;i<getFdnum();i++){
					if(*(piOccChannelMember + (pclass * getFdnum()) + i) == toindex){
						*(piOccChannelMember + (pclass * getFdnum()) + i) = -1;
						break;
					}
				}
			}
#endif
			// 设定职业归零
			CHAR_setInt( toindex, PROFESSION_CLASS, p_class );
			
			// 设定职业等级 0
			CHAR_setInt( toindex, PROFESSION_LEVEL, 0 );

			// 技能点数归零
			CHAR_setInt(toindex, PROFESSION_SKILL_POINT, 0 );

			// 还原抗性
			for( i=0; i<3; i++ )
				CHAR_setInt( toindex, PROFESSION_FIRE_R+i, /*CHAR_getInt( toindex, PROFESSION_FIRE_R+i) + value*/0 );

			// 还原巫师MP上限
			CHAR_setInt( toindex , CHAR_MAXMP , 100 );

			// 删除所有技能
			CHAR_CHAT_DEBUG_delsk( toindex, "all" );

			// ?? Andy add
			CHAR_setInt( toindex , ATTACHPILE, 0);

			// 清除任务旗标
			NPC_NowEndEventSetFlgCls( toindex, 145);
			NPC_NowEndEventSetFlgCls( toindex, 146);
			NPC_NowEndEventSetFlgCls( toindex, 147);

			CHAR_sendStatusString( toindex , "S");
			
			CHAR_sendCToArroundCharacter( CHAR_getWorkInt( toindex , CHAR_WORKOBJINDEX ));
		}
#endif
		else
		{
			return FALSE;
		}
	}
#endif
#ifdef _TREASURE_BOX
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "TREASURE_EVENT", buf2, sizeof( buf2) ) != NULL) {
		if( strstr( buf2, "OPEN") != NULL){
			if( NPC_Util_GetStrFromStrWithDelim( buf1, "TREASURE_TIME", buf2, sizeof( buf2) ) == NULL)
				return FALSE;
			//NPC_TIME_EVENTMODE = CHAR_NPCWORKINT6,// 1
			//NPC_TIME_EVENTTIME = CHAR_NPCWORKINT7,
			CHAR_setWorkInt( meindex, CHAR_NPCWORKINT6, NPC_EVENTMODE_OPEN );
			CHAR_setWorkInt( meindex, CHAR_NPCWORKINT7, NowTime.tv_sec+atoi( buf2) );
			//变图
			//NPC_TIME_EVENONBBI = CHAR_NPCWORKINT8,
			CHAR_setInt( meindex, CHAR_BASEBASEIMAGENUMBER,
				CHAR_getWorkInt( meindex, CHAR_NPCWORKINT8));

			CHAR_setInt( meindex, CHAR_BASEIMAGENUMBER,
				CHAR_getWorkInt( meindex, CHAR_NPCWORKINT8));
			CHAR_sendCToArroundCharacter( CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX ) );
		}
	}
#endif
#ifdef  _NPC_ADDLEVELUP
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "SetLevel", buf2, sizeof( buf2) ) != NULL) {
		NPC_ActionLevelAndTransUp( meindex, toindex, atoi(buf2), 0, 0, -1);
	}
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "AddExps", buf2, sizeof( buf2) ) != NULL) {
		NPC_ActionLevelAndTransUp( meindex, toindex, 0, 0, atoi(buf2), -1);
	}
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "AddSkillPoint", buf2, sizeof( buf2) ) != NULL) {
		NPC_ActionLevelAndTransUp( meindex, toindex, 0, atoi(buf2), 0, -1);
	}
	if( NPC_Util_GetStrFromStrWithDelim( buf1, "SetRideType", buf2, sizeof( buf2) ) != NULL) {
		NPC_ActionLevelAndTransUp( meindex, toindex, 0, 0, 0, atoi(buf2));
	}
#endif

#ifdef _NPC_ADDWARPMAN1
    if( NPC_Util_GetStrFromStrWithDelim( buf1, "PartyCount", buf2, sizeof( buf2) ) != NULL) {
	    //取得组队人数
		int partys=0;
	    int leaderindex=-1;
        int flg=0; //0:等於 1:大於等於 2:小於等於 3:大於 4:小於 5:不等於
	    char Head[32],buf3[32];
		leaderindex = toindex;
	    if( CHAR_getWorkInt( toindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_NONE ){
		    partys = 0;
		}else if( CHAR_getWorkInt( toindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_CLIENT ){
		    leaderindex = CHAR_getWorkInt( toindex, CHAR_WORKPARTYINDEX1);
		
	        if( CHAR_getWorkInt( leaderindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_LEADER ){
		        int i;
		        for( i=0; i<5; i++)	{
			        int pindex = CHAR_getWorkInt( leaderindex, CHAR_WORKPARTYINDEX1+i);
			        if( !CHAR_CHECKINDEX( pindex) )
				        partys++;
				}
			}
		}
	   
		memset( Head, 0, sizeof( Head));
		memset( buf3, 0, sizeof( buf3));
	    if( strstr( buf2, "=" ) != NULL ){
		    strcpy( Head, "=");
		    flg = 0;
		    if( strstr( buf2, ">") != NULL ){
			    strcat( Head, ">");
			    flg = 1;
			}else if( strstr( buf2, "<") != NULL ){
			    strcat( Head, "<");
			    flg = 2;
			}else if( strstr( buf2, "!") != NULL )	{
			    strcat( Head, "!");
			    flg = 5;
			}
		}else if(strstr( buf2, "<") != NULL){
		    strcpy( Head, "<");
		    flg = 4;
		}else if(strstr( buf2, ">") != NULL){
		    strcpy( Head, ">");
		    flg = 3;
		}
		Head[ strlen( Head)+1] = 0;
		if( NPC_Util_GetStrFromStrWithDelim( buf2, Head, buf3, sizeof( buf3) ) != NULL) {
		    if( flg == 0 ){
           if( atoi(buf3) != partys ) {
             return FALSE;  
           }
        }else if( flg == 1 ){
           if( atoi(buf3) < partys ) {
           	return FALSE;
          }
		    }else if( flg == 2 ){
			    if( atoi(buf3) > partys ) 
			    	return FALSE;
        }else if( flg == 3 ){
			    if( atoi(buf3) <= partys ){
			    	return FALSE;
			    }
		    }else if( flg == 4 ){
			    if( atoi(buf3) >= partys ) {
			    	return FALSE;
			    }
		    }else if( flg == 5 ){
			    if( atoi(buf3) == partys ) {
			    	return FALSE;
			    }
			  }
		}
		return TRUE;
	}
#endif

	ActionNpc_WarpPoint( meindex, toindex, buf1);
	return TRUE;
}
BOOL Action_PartyCheck( int meindex, int talker)
{
	if(CHAR_getWorkInt(talker,CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE){
		return FALSE;
	}
	return TRUE;
}

BOOL NPC_ActionAddGold( int talker, int Golds)
{
	CHAR_AddGold( talker, Golds);
	return TRUE;
}

#ifdef _NPC_INT_SET
BOOL NPC_ActionIntSet( int talker,char *buf)
{
	char key[128];
	char value[64];
	getStringFromIndexWithDelim(buf,",",1,key,sizeof(key));
	int i;
	for(i=0;i<CHAR_DATAINTNUM;i++){
		if(strcmp(key, CHAR_setintdata[i]) == 0){
			getStringFromIndexWithDelim(buf,",",2,value,sizeof(value));
		  CHAR_setInt( talker, i, atoi(value));
		  char token[128];
			sprintf(token, "你当前值为:%d", CHAR_getInt( talker, i));
			CHAR_talkToCli( talker, -1, token,  CHAR_COLORYELLOW);
		  break;
		}
	}
	
	return TRUE;
}

BOOL NPC_ActionIntChange( int talker,char *buf)
{
	char key[128];
	char value[64];
	getStringFromIndexWithDelim(buf,",",1,key,sizeof(key));
	int i;
	for(i=0;i<CHAR_DATAINTNUM;i++){
		if(strcmp(key, CHAR_setintdata[i]) == 0){
			getStringFromIndexWithDelim(buf,",",2,value,sizeof(value));
		  CHAR_setInt( talker, i, CHAR_getInt( talker, i) + atoi(value));
		  char token[128];
			sprintf(token, "你当前值为:%d", CHAR_getInt( talker, i));
			CHAR_talkToCli( talker, -1, token,  CHAR_COLORYELLOW);
		  break;
		}
	}
	
	return TRUE;
}

BOOL NPC_ActionIntShow( int talker,char *buf)
{
	int i;
	for(i=0;i<CHAR_DATAINTNUM;i++){
		if(strcmp(buf, CHAR_setintdata[i]) == 0){
			char token[128];
			sprintf(token, "你当前值为%d", CHAR_getInt( talker, i));
			CHAR_talkToCli( talker, -1, token,  CHAR_COLORYELLOW);
		  break;
		}
	}
	
	return TRUE;
}


#endif

BOOL NPC_ActionDelGold( int talker,char *buf)
{
	int Golds=0;
	int PGold=0;
	if( buf == NULL ) return FALSE;
	Golds = atoi( buf);
	PGold = CHAR_getInt( talker, CHAR_GOLD);
	if( PGold < Golds )	{
		CHAR_talkToCli( talker, -1, "你身上的钱不够喔！", CHAR_COLORYELLOW);
		return FALSE;
	}else	{
		char buf1[256];
		sprintf( buf1, "交出%d石币", Golds);
		CHAR_talkToCli( talker, -1, buf1, CHAR_COLORYELLOW);
	}
	CHAR_setInt( talker, CHAR_GOLD, PGold-Golds);
	CHAR_send_P_StatusString( talker , CHAR_P_STRING_GOLD);
	return TRUE;
}
#ifdef _DEL_FAME
BOOL NPC_ActionDelFame( int talker,char *buf)
{
	int Fame=0;
	int PFame=0;
	if( buf == NULL ) return FALSE;
	Fame = atoi( buf)*100;
	PFame = CHAR_getInt( talker, CHAR_FAME);

	char buf1[256];
	sprintf( buf1, "扣除个人声望：%d", Fame/100);
	CHAR_talkToCli( talker, -1, buf1, CHAR_COLORYELLOW);
	
	CHAR_setInt( talker, CHAR_FAME, PFame-Fame);
/*
	int fd = getfdFromCharaIndex(talker);
	sprintf(buf1,"%d",PFame-Fame);
	
	saacproto_ACFixFMData_send(acfd,
		CHAR_getChar(talker,CHAR_FMNAME),CHAR_getInt(talker,CHAR_FMINDEX),CHAR_getWorkInt(talker,CHAR_WORKFMINDEXI),
		FM_FIX_FMFEED,buf1,"",CHAR_getWorkInt(talker,CHAR_WORKFMCHARINDEX),CONNECT_getFdid(fd));
*/
	return TRUE;
}
#endif

#ifdef _MOMENTUM_NPC
BOOL NPC_ActionMomentum( int talker,char *buf)
{
	int Momentums=0;
	int PMomentum=0;
	if( buf == NULL ) return FALSE;
	Momentums = atoi( buf)*100;
	PMomentum = CHAR_getInt( talker, CHAR_MOMENTUM);
	if( PMomentum < Momentums )	{
		CHAR_talkToCli( talker, -1, "你的个人气势值不够喔！", CHAR_COLORYELLOW);
		return FALSE;
	}else	{
		char buf1[256];
		sprintf( buf1, "扣除个人气势值%d", Momentums/100);
		CHAR_talkToCli( talker, -1, buf1, CHAR_COLORYELLOW);
	}
	CHAR_setInt( talker, CHAR_MOMENTUM, PMomentum-Momentums);
	int fd = getfdFromCharaIndex(talker);
	sprintf(buf,"%d",PMomentum-Momentums);
	saacproto_ACFixFMData_send(acfd,
		CHAR_getChar(talker,CHAR_FMNAME),CHAR_getInt(talker,CHAR_FMINDEX),CHAR_getWorkInt(talker,CHAR_WORKFMINDEXI),
		FM_FIX_FMMOMENTUM,buf,"",CHAR_getWorkInt(talker,CHAR_WORKFMCHARINDEX),CONNECT_getFdid(fd));
	return TRUE;
}
#endif
#ifdef _DP_NPC
BOOL NPC_ActionDP( int talker,char *buf)
{
	int DPs=0;
	int PDP=0;
	if( buf == NULL ) return FALSE;
	DPs = atoi( buf);
	PDP = CHAR_getInt( talker, CHAR_DUELPOINT);
	if( PDP < DPs )	{
		CHAR_talkToCli( talker, -1, "你的DP不够哦！", CHAR_COLORYELLOW);
		return FALSE;
	}else	{
		char buf1[256];
		sprintf( buf1, "扣除DP%d", DPs);
		CHAR_talkToCli( talker, -1, buf1, CHAR_COLORYELLOW);
	}
 
	CHAR_setInt( talker, CHAR_DUELPOINT, PDP-DPs);
	CHAR_send_P_StatusString( talker ,CHAR_P_STRING_DUELPOINT );
	
			
	char szBuffer[64];
	{
		char szAdd[64], szNow[64];
		cnv10to62( DPs, szAdd, sizeof( szAdd ) );
		cnv10to62( PDP, szNow, sizeof( szNow ) );
		snprintf( szBuffer, sizeof( szBuffer ),
			"%s|%s|", szAdd, szNow );
	}
	
	lssproto_RD_send( getfdFromCharaIndex(talker), szBuffer );
	
	CHAR_send_DpDBUpdate( talker );
#ifndef _NET_REDUCESEND
	CHAR_send_DpDBUpdate_AddressBook( talker, TRUE );
#endif
	
	return TRUE;
}
#endif

#ifdef _BOUND_TIME
BOOL NPC_ActionBoundTimeCheck(int meindex,int talker,int value,int flg)
{
  if(value>0){
		int myboundtime = CHAR_getInt(talker, CHAR_BOUNDTIME);
		if(myboundtime > 0 && myboundtime > time(NULL)){
			return TRUE;
		}
  }
	return FALSE;
}
#endif

#ifdef _PLAYER_DIY_MAP
extern Player_Diy_Map PlayerDiyMap[Player_Diy_Map_NUM];
BOOL NPC_ActionDiyMapCheck(int meindex,int talker,int diymap,int flg)
{
  int i;
	for(i=0;i<Player_Diy_Map_NUM;i++){
		if(PlayerDiyMap[i].ff==diymap){
			if(strcmp(CHAR_getChar(talker, CHAR_CDKEY), PlayerDiyMap[i].cdkey)==0){
				return TRUE;
			}
		}
	}
	CHAR_talkToCli( talker, -1, "很抱歉，你不是该地图的主人，无法为你服务~", CHAR_COLORRED);
	return FALSE;
}
BOOL NPC_ActionPlayerDiyMap( int talker, char *buf)
{
	int fd = getfdFromCharaIndex( talker);
	if( fd == - 1 ) return FALSE;
	char token[1024]="";
	int i;
	int page = atoi(buf) * 7;
	if(page>Player_Diy_Map_NUM)page=Player_Diy_Map_NUM;
	strcpy(token," “地图编号”“有效时间”“占　据　人”\n");
	for(i=0;i<page;i++){
		if(PlayerDiyMap[i].ff>0){
			char tmp[128];
			if(PlayerDiyMap[i].datetime==-1){
				sprintf(tmp, "　%-10d　永久有效　%16s\n", PlayerDiyMap[i].ff, PlayerDiyMap[i].cdkey);
			}else{
				if(PlayerDiyMap[i].datetime>time(NULL)){
					struct tm tm1;
					memcpy(&tm1, localtime((time_t *)&PlayerDiyMap[i].datetime), sizeof(tm1));
					sprintf(tmp, "　%-10d %02d-%02d-%02d %16s\n", PlayerDiyMap[i].ff,  tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday, PlayerDiyMap[i].cdkey);
				}else{
					sprintf(tmp, "　%-10d　已经失效　%16s\n", PlayerDiyMap[i].ff, PlayerDiyMap[i].cdkey);
				}
			}
			strcat(token, tmp);
		}
	}
	
	lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE, 
												WINDOW_BUTTONTYPE_OKCANCEL,
												-1,
												-1,
												token);
	return TRUE;
}
BOOL NPC_ActionPlayerDiyMapBuy( int talker, char *buf)
{
	int ff=0;
	int day=0;
	int playernum=0;
	int point=0;
	int fame=0;
	char token[256];
	if(getStringFromIndexWithDelim(buf,",",1,token,sizeof(token))==TRUE){
		ff=atoi(token);
	}
	if(getStringFromIndexWithDelim(buf,",",2,token,sizeof(token))==TRUE){
		day=max(0,atoi(token));
	}
	if(getStringFromIndexWithDelim(buf,",",3,token,sizeof(token))==TRUE){
		playernum=max(0,atoi(token));
	}
	if(getStringFromIndexWithDelim(buf,",",4,token,sizeof(token))==TRUE){
		point=max(0,atoi(token));
	}
	if(getStringFromIndexWithDelim(buf,",",5,token,sizeof(token))==TRUE){
		fame=max(0,atoi(token)*100);
	}
	if(CHAR_getInt(talker, CHAR_AMPOINT)>=point && CHAR_getInt(talker, CHAR_FAME)>=fame){
		int i;
		int find=FALSE;
		for(i=0;i<Player_Diy_Map_NUM;i++){
		  if(PlayerDiyMap[i].ff==ff){
		  	if(PlayerDiyMap[i].datetime==-1){
					CHAR_talkToCli( talker, -1, "很抱歉，该地图玩家永久使用中，无法购买~", CHAR_COLORRED);
					return TRUE;
				}
		  	if(PlayerDiyMap[i].datetime<time(NULL)){
		  		find=TRUE;
				}else{
					CHAR_talkToCli( talker, -1, "很抱歉，该地图已有玩家使用中，请在失效期后购买~", CHAR_COLORRED);
					return TRUE;
				}
				break;
		  }
	  }
		if(find==FALSE){
			for(i=0;i<Player_Diy_Map_NUM;i++){
			  if(PlayerDiyMap[i].ff==0){
			  	memset( &PlayerDiyMap[i], 0, sizeof( PlayerDiyMap[i] ) );
			
			  	PlayerDiyMap[i].ff=ff;
			  	find=TRUE;
					break;
			  }
		  }
		}
	  if(find==TRUE){
	  	char buf1[256];
		  if(point>0){
		  	CHAR_setInt(talker, CHAR_AMPOINT, CHAR_getInt(talker, CHAR_AMPOINT)-point);
#ifdef _AMPOINT_LOG
		  	LogAmPoint(CHAR_getChar( talker, CHAR_NAME ),
									         	CHAR_getChar( talker, CHAR_CDKEY ),
									          -point,
									   				CHAR_getInt( talker, CHAR_AMPOINT ),
									         	"(DIY地图)",
									         	CHAR_getInt( talker,CHAR_FLOOR),
									         	CHAR_getInt( talker,CHAR_X ),
									         	CHAR_getInt( talker,CHAR_Y ));
#endif
		  	sprintf(buf1, "扣除积分点 %d", point);
		  	CHAR_talkToCli( talker, -1, buf1, CHAR_COLORYELLOW);
		  }
		  if(fame>0){
		  	CHAR_setInt(talker, CHAR_FAME, CHAR_getInt(talker, CHAR_FAME)-fame);
		  	sprintf(buf1, "扣除声望 %d", fame);
		  	CHAR_talkToCli( talker, -1, buf1, CHAR_COLORYELLOW);
				int fd = getfdFromCharaIndex(talker);
				sprintf(buf,"%d",CHAR_getInt(talker, CHAR_FAME));
				saacproto_ACFixFMData_send(acfd,
							CHAR_getChar(talker,CHAR_FMNAME),CHAR_getInt(talker,CHAR_FMINDEX),CHAR_getWorkInt(talker,CHAR_WORKFMINDEXI),
							FM_FIX_FMFEED,buf,"",CHAR_getWorkInt(talker,CHAR_WORKFMCHARINDEX),CONNECT_getFdid(fd));
			}

			PlayerDiyMap[i].datetime = time(NULL) + day * 24 * 60 * 60;
			PlayerDiyMap[i].playernum = playernum;
			strcpy( PlayerDiyMap[i].cdkey, CHAR_getChar(talker, CHAR_CDKEY));

			sprintf(buf1, "恭喜你，获得地图 %d DIY权限 %d 天，最高支持人数 %d 人", point, day, playernum);
		  CHAR_talkToCli( talker, -1, buf1, CHAR_COLORGREEN);

		  char filename[128];
		  char map[Player_Diy_Map_SIZE];
	 		memcpy( &map , &PlayerDiyMap[i], sizeof(PlayerDiyMap[i]));
			sprintf( filename, "playermap/%d",  ff);
			FILE *fp   =   fopen(filename,   "wb");

		  fwrite(map, sizeof(map), 1,fp);
		  fclose(fp);
	  }else if(find==FALSE){
	  	CHAR_talkToCli( talker, -1, "很抱歉，已经达到DIY地图使用权上限，操作不成功~", CHAR_COLORRED);
	  }
	}else{
		CHAR_talkToCli( talker, -1, "很抱歉，你无法满足条件购买DIY地图使用权~", CHAR_COLORRED);
	}
	return TRUE;
}

BOOL NPC_ActionPlayerDiyMapLetter( int talker, char *buf)
{
  int itemID=0, itemindex=-1;
  char  token[256];
  int ff=0;
  if(getStringFromIndexWithDelim(buf,",",1,token,sizeof(token))==TRUE){
		ff=atoi(token);
	}
	if(getStringFromIndexWithDelim(buf,",",2,token,sizeof(token))==TRUE){
		itemID=atoi(token);
	}
  int i;
	for(i=0;i<Player_Diy_Map_NUM;i++){
		if(PlayerDiyMap[i].ff==ff){
			if(strcmp(CHAR_getChar(talker, CHAR_CDKEY), PlayerDiyMap[i].cdkey)==0){
				break;
			}
		}
	}
  
  if(i==Player_Diy_Map_NUM){
  	CHAR_talkToCli( talker, -1, "很抱歉，该地图并非你所有~", CHAR_COLORRED);
  	return FALSE;
  }
  
	itemindex = ITEM_makeItemAndRegist( itemID);
	if(itemindex == -1)
		return FALSE;
		
	ITEM_setChar( itemindex, ITEM_ARGUMENT, CHAR_getChar(talker, CHAR_CDKEY));
	ITEM_setChar( itemindex, ITEM_USEFUNC, "PLAYERDIYMAPLETTER");
	sprintf( token, "由 %s 签发的地图 %d 邀请函", PlayerDiyMap[i].cdkey, PlayerDiyMap[i].ff);
	ITEM_setChar( itemindex, ITEM_EFFECTSTRING, token);
	int ret = CHAR_addItemSpecificItemIndex( talker, itemindex);
	if( ret < 0 || ret >= CheckCharMaxItem(talker)  ){
		ITEM_endExistItemsOne( itemindex);
		print ("\n ret error!!");
		return FALSE;
	}

	sprintf( token,"拿到%s",ITEM_getChar( itemindex, ITEM_NAME));
	CHAR_talkToCli( talker, -1, token, CHAR_COLORYELLOW );
	CHAR_sendItemDataOne( talker, ret);

	return TRUE;	
}

BOOL NPC_ActionPlayerDiyMapSend( int talker, char *buf)
{
	char token[256];
	int ff=0, x=0, y=0, i=0;

	if( !CHAR_CHECKINDEX( talker) ) return FALSE;

	if( getStringFromIndexWithDelim( buf, ",", 1, token, sizeof(token)) == TRUE ){
		ff = atoi( token);
	}
	if( getStringFromIndexWithDelim( buf, ",", 2, token, sizeof(token)) == TRUE ){
		x = atoi( token);
	}
	if( getStringFromIndexWithDelim( buf, ",", 3, token, sizeof(token)) == TRUE ){
		y = atoi( token);
	}
	for(i=0;i<Player_Diy_Map_NUM;i++){
		if(PlayerDiyMap[i].ff==ff){
			int j;
			int itemindex;
			for( j = CHAR_STARTITEMARRAY ;  j < CheckCharMaxItem(talker)  ; j++){
				itemindex = CHAR_getItemIndex( talker ,j);
				if( ITEM_CHECKINDEX(itemindex) ){
					if( strcmp(ITEM_getChar( itemindex, ITEM_USEFUNC), "PLAYERDIYMAPLETTER")==0){
						if( strcmp(ITEM_getChar( itemindex, ITEM_ARGUMENT), PlayerDiyMap[i].cdkey)==0){
							int k;
							int playernum = 0;
							for(k=0;k<CHAR_getPlayerMaxNum();k++){
								if(CHAR_CHECKINDEX(k) == FALSE) continue;
								if(CHAR_getInt( k, CHAR_FLOOR) == ff)
								{
									playernum++;
									if(playernum>PlayerDiyMap[i].playernum)break;
								}
							}
							if(k<CHAR_getPlayerMaxNum()){
								CHAR_talkToCli( talker, -1, "你无法进入，地图人数已满，请稍候再来~", CHAR_COLORRED);
								return TRUE;
							}
							LogItem(
								CHAR_getChar( talker, CHAR_NAME ),
								CHAR_getChar( talker, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD 在item的log中增加item名称
								itemindex,
#else
								ITEM_getInt( itemindex, ITEM_ID),
#endif
								"WarpManDelItem",
								CHAR_getInt( talker,CHAR_FLOOR),
								CHAR_getInt( talker,CHAR_X ),
								CHAR_getInt( talker,CHAR_Y ),
								ITEM_getChar( itemindex, ITEM_UNIQUECODE),
								ITEM_getChar( itemindex, ITEM_NAME),
								ITEM_getInt( itemindex, ITEM_ID)
	
							);
							sprintf( token, "交出%s。",
											ITEM_getChar( itemindex, ITEM_NAME));
							CHAR_talkToCli( talker, -1, token, CHAR_COLORYELLOW);
							CHAR_DelItem( talker, j);
							CHAR_warpToSpecificPoint( talker, ff, x, y );
							
							break;
						}
					}
				}
			}
			if(j ==  CheckCharMaxItem(talker) ){
				CHAR_talkToCli( talker, -1, "你身上并无该地图使用者的邀请函~", CHAR_COLORRED);
				return TRUE;
			}
		}
	}
	return TRUE;
}
BOOL NPC_ActionPlayerDiyMapAddNum( int talker, char *buf)
{
  char token[256];
  int ff=0;
  int playernum=0;
  int point=0;
  if(getStringFromIndexWithDelim(buf,",",1,token,sizeof(token))==TRUE){
		ff=atoi(token);
	}
	if(getStringFromIndexWithDelim(buf,",",2,token,sizeof(token))==TRUE){
		playernum=atoi(token);
	}
	if(getStringFromIndexWithDelim(buf,",",3,token,sizeof(token))==TRUE){
		point=atoi(token);
	}
  int i;
	for(i=0;i<Player_Diy_Map_NUM;i++){
		if(PlayerDiyMap[i].ff==ff){
			if(strcmp(CHAR_getChar(talker, CHAR_CDKEY), PlayerDiyMap[i].cdkey)==0){
				if(CHAR_getInt(talker, CHAR_AMPOINT)>=point){
					CHAR_setInt(talker, CHAR_AMPOINT, CHAR_getInt(talker, CHAR_AMPOINT)-point);
#ifdef _AMPOINT_LOG
					LogAmPoint(CHAR_getChar( talker, CHAR_NAME ),
									         	CHAR_getChar( talker, CHAR_CDKEY ),
									          -point,
									   				CHAR_getInt( talker, CHAR_AMPOINT ),
									         	"(DIY地图)",
									         	CHAR_getInt( talker,CHAR_FLOOR),
									         	CHAR_getInt( talker,CHAR_X ),
									         	CHAR_getInt( talker,CHAR_Y ));
#endif
					sprintf(token, "扣除积分点 %d", point);
					CHAR_talkToCli( talker, -1, token, CHAR_COLORYELLOW);
					PlayerDiyMap[i].playernum += playernum;
					sprintf( token,"恭喜你，你的DIY地图最大支持人数提升至 %d 人~", PlayerDiyMap[i].playernum);
					CHAR_talkToCli( talker, -1, token, CHAR_COLORYELLOW );
					
				  char filename[128];
				  char map[Player_Diy_Map_SIZE];
			 		memcpy( &map , &PlayerDiyMap[i], sizeof(PlayerDiyMap[i]));
					sprintf( filename, "playermap/%d",  ff);
					FILE *fp   =   fopen(filename,   "wb");
		
				  fwrite(map, sizeof(map), 1,fp);
				  fclose(fp);
					
				}else{
					CHAR_talkToCli( talker, -1, "很抱歉，你的积分点数不足，无法提升人数~", CHAR_COLORRED);
  				return FALSE;
				}
				
				return TRUE;
			}
		}
	}
  
  if(i==Player_Diy_Map_NUM){
  	CHAR_talkToCli( talker, -1, "很抱歉，该地图并非你所有~", CHAR_COLORRED);
  }
  return FALSE;
}
BOOL NPC_ActionPlayerDiyMapAddExp( int talker, char *buf)
{
  int point=0;
  int exp=0;
  char token[256];
  int ff=0;
  if(getStringFromIndexWithDelim(buf,",",1,token,sizeof(token))==TRUE){
		ff=atoi(token);
	}
	if(getStringFromIndexWithDelim(buf,",",2,token,sizeof(token))==TRUE){
		exp=atoi(token);
	}
	if(getStringFromIndexWithDelim(buf,",",3,token,sizeof(token))==TRUE){
		point=atoi(token);
	}
  int i;
	for(i=0;i<Player_Diy_Map_NUM;i++){
		if(PlayerDiyMap[i].ff==ff){
			if(strcmp(CHAR_getChar(talker, CHAR_CDKEY), PlayerDiyMap[i].cdkey)==0){
				if(CHAR_getInt(talker, CHAR_AMPOINT)>=point){
					CHAR_setInt(talker, CHAR_AMPOINT, CHAR_getInt(talker, CHAR_AMPOINT)-point);
#ifdef _AMPOINT_LOG
					LogAmPoint(CHAR_getChar( talker, CHAR_NAME ),
									         	CHAR_getChar( talker, CHAR_CDKEY ),
									          -point,
									   				CHAR_getInt( talker, CHAR_AMPOINT ),
									         	"(DIY地图)",
									         	CHAR_getInt( talker,CHAR_FLOOR),
									         	CHAR_getInt( talker,CHAR_X ),
									         	CHAR_getInt( talker,CHAR_Y ));
#endif	         	
					sprintf(token, "扣除积分点 %d", point);
					CHAR_talkToCli( talker, -1, token, CHAR_COLORYELLOW);
					PlayerDiyMap[i].exp += exp;
					sprintf( token,"恭喜你，你的DIY地图经验获取提升至 %d ~", PlayerDiyMap[i].exp);
					CHAR_talkToCli( talker, -1, token, CHAR_COLORYELLOW );
					
					char filename[128];
				  char map[Player_Diy_Map_SIZE];
			 		memcpy( &map , &PlayerDiyMap[i], sizeof(PlayerDiyMap[i]));
					sprintf( filename, "playermap/%d",  ff);
					FILE *fp   =   fopen(filename,   "wb");
		
				  fwrite(map, sizeof(map), 1,fp);
				  fclose(fp);
				}else{
					CHAR_talkToCli( talker, -1, "很抱歉，你的积分点数不足，无法提升地图经验获取~", CHAR_COLORRED);
  				return FALSE;
				}
				
				return TRUE;
			}
		}
	}
  
  if(i==Player_Diy_Map_NUM){
  	CHAR_talkToCli( talker, -1, "很抱歉，该地图并非你所有~", CHAR_COLORRED);
  }
  return FALSE;
}
BOOL NPC_ActionPlayerDiyMapAddDay( int talker, char *buf)
{
  char token[256];
	int ff=0,day=0,point=0;
  if(getStringFromIndexWithDelim(buf,",",1,token,sizeof(token))==TRUE){
		ff=atoi(token);
	}
	if(getStringFromIndexWithDelim(buf,",",2,token,sizeof(token))==TRUE){
		day=atoi(token);
	}
	if(getStringFromIndexWithDelim(buf,",",3,token,sizeof(token))==TRUE){
		point=atoi(token);
	}
  int i;
	for(i=0;i<Player_Diy_Map_NUM;i++){
		if(PlayerDiyMap[i].ff==ff){
			if(strcmp(CHAR_getChar(talker, CHAR_CDKEY), PlayerDiyMap[i].cdkey)==0){
				if(CHAR_getInt(talker, CHAR_AMPOINT)>=point){
					CHAR_setInt(talker, CHAR_AMPOINT, CHAR_getInt(talker, CHAR_AMPOINT)-point);
#ifdef _AMPOINT_LOG
					LogAmPoint(CHAR_getChar( talker, CHAR_NAME ),
									         	CHAR_getChar( talker, CHAR_CDKEY ),
									          -point,
									   				CHAR_getInt( talker, CHAR_AMPOINT ),
									         	"(DIY地图)",
									         	CHAR_getInt( talker,CHAR_FLOOR),
									         	CHAR_getInt( talker,CHAR_X ),
									         	CHAR_getInt( talker,CHAR_Y ));
#endif	         	
					sprintf(token, "扣除积分点 %d", point);
					CHAR_talkToCli( talker, -1, token, CHAR_COLORYELLOW);
					PlayerDiyMap[i].datetime += day * 24 * 60 * 60;
					sprintf( token,"恭喜你，你的DIY地图使用权增加 %d 天~", day);
					CHAR_talkToCli( talker, -1, token, CHAR_COLORYELLOW );
					
					char filename[128];
				  char map[Player_Diy_Map_SIZE];
			 		memcpy( &map , &PlayerDiyMap[i], sizeof(PlayerDiyMap[i]));
					sprintf( filename, "playermap/%d",  ff);
					FILE *fp   =   fopen(filename,   "wb");
		
				  fwrite(map, sizeof(map), 1,fp);
				  fclose(fp);
				}else{
					CHAR_talkToCli( talker, -1, "很抱歉，你的积分点数不足，无法提升地图增加使用权天数~", CHAR_COLORRED);
  				return FALSE;
				}
				
				return TRUE;
			}
		}
	}
  
  if(i==Player_Diy_Map_NUM){
  	CHAR_talkToCli( talker, -1, "很抱歉，该地图并非你所有~", CHAR_COLORRED);
  }
  return FALSE;
}
#endif

#ifdef _LOTTERY_SYSTEM
int todayaward[7]={-1};
BOOL NPC_ActionLotteryChange( int talker, char *buf)
{
	int fd = getfdFromCharaIndex( talker);
	if( fd == - 1 ) return FALSE;
	int award[7];
	char token[256];

	struct tm now;
	time_t timep;
	time(&timep);
	memcpy(&now, localtime(&timep), sizeof(now));
	struct tm tm1;
	int lotterydate = CHAR_getInt(talker, CHAR_LOTTERY_DATE);
	int lotterytime = CHAR_getInt(talker, CHAR_LOTTERY_TIME);
	memcpy(&tm1, localtime((time_t *)&lotterydate), sizeof(tm1));

	if(now.tm_year==tm1.tm_year && now.tm_mon==tm1.tm_mon && now.tm_mday==(tm1.tm_mday + 1)){
		int i,j;
		int value[7];
		int num=0;
		char *lotteryvalue=CHAR_getChar(talker, CHAR_LOTTERY_VALUE);
		
		for(i=0;i<7;i++){
			if(getStringFromIndexWithDelim(lotteryvalue,",",i+1,token,sizeof(token))==TRUE){
				value[i]=atoi(token);
				int k;
				for(j=0;j<7;j++){
					if(todayaward[j]==value[i]){
						for(k=0;k<i;k++){
							if(value[k]==value[i]){
								break;
							}
						}
						if(k==i){
							num++;
						}
						break;
					}
				}
			}
		}
		char tmp[7][4]={"一","二","三","四","五","六","七"};
		if(num>0){
			for(i=0;i<7;i++){
				if(getStringFromIndexWithDelim(buf,",",i+1,token,sizeof(token))==TRUE){
					award[i]=atoi(token);
				}else{
					award[i]=0;
				}
			}
			sprintf(token, "恭喜你中得 %s等奖 ,奖励积分为 %d 点", tmp[7-num], award[7-num]);
			lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE, 
											WINDOW_BUTTONTYPE_OKCANCEL,	-1,	-1,	token);

			lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE, 
											WINDOW_BUTTONTYPE_OKCANCEL,	-1,	-1,	token);
			CHAR_setInt(talker, CHAR_AMPOINT, CHAR_getInt(talker, CHAR_AMPOINT) + award[7-num]*lotterytime);
#ifdef _AMPOINT_LOG
			LogAmPoint(CHAR_getChar( talker, CHAR_NAME ),
									         	CHAR_getChar( talker, CHAR_CDKEY ),
									          award[7-num],
									   				CHAR_getInt( talker, CHAR_AMPOINT ),
									         	"(彩票中奖)",
									         	CHAR_getInt( talker,CHAR_FLOOR),
									         	CHAR_getInt( talker,CHAR_X ),
									         	CHAR_getInt( talker,CHAR_Y ));
#endif
			CHAR_setInt(talker, CHAR_LOTTERY_DATE, 0);
			CHAR_setInt(talker, CHAR_LOTTERY_TIME, 0);
			CHAR_setChar(talker, CHAR_LOTTERY_VALUE, "");
			
			int playernum = CHAR_getPlayerMaxNum();
			sprintf(token, "恭喜玩家 %s 中得 %s等奖 ,奖励积分为 %d 点", CHAR_getChar(talker, CHAR_NAME), tmp[7-num], award[7-num]);
			for( i = 0 ; i < playernum ; i++) {
				if( CHAR_getCharUse(i) != FALSE ) {
					CHAR_talkToCli( i, -1, token, CHAR_COLORBLUE);
				}
			}
			
			FILE * f1 = fopen("todayaward.txt","a+");
			sprintf(token, "账号 %s 玩家 %s 中得 %s等奖, 奖励积分为 %d × %d = %d 点\n", CHAR_getChar(talker, CHAR_CDKEY),
																																				CHAR_getChar(talker, CHAR_NAME), 
																																				tmp[7-num], 
																																				award[7-num],
																																				lotterytime,
																																				award[7-num] * lotterytime);
			fputs(token,f1);
			fclose(f1);
		}else{
			lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE, 
											WINDOW_BUTTONTYPE_OKCANCEL,	-1,	-1,	"很抱歉，你没有中奖！");
		}
	}else{
		lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE, 
											WINDOW_BUTTONTYPE_OKCANCEL,	-1,	-1,	"很抱歉，你还未购买这期开奖的彩票！");
	}
																					
	return TRUE;
}

BOOL NPC_ActionLotteryBuy( int talker, char *buf)
{
	int fd = getfdFromCharaIndex( talker);
	if( fd == - 1 ) return FALSE;
	if(CHAR_getInt(talker, CHAR_AMPOINT)<atoi(buf)){
		lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE, 
											WINDOW_BUTTONTYPE_OKCANCEL,	-1,	-1,	"很抱歉，你的积分不足！");
		return FALSE;
	}
	CHAR_setInt(talker, CHAR_AMPOINT, CHAR_getInt(talker, CHAR_AMPOINT) - atoi(buf));
	CHAR_setInt(talker, CHAR_LOTTERY_TIME, 1);
	char token[256];
	sprintf(token, "已扣除 %d 积分", atoi(buf));
	CHAR_talkToCli( talker, -1, token, CHAR_COLORBLUE);
	
	lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGEANDLINEINPUT, 
												WINDOW_BUTTONTYPE_OKCANCEL,
												CHAR_WINDOWTYPE_LOTTERYBUY,
												-1,
												"请输入您要购买的彩票号，格式如:\n"
												"1,2,3,4,5,6,7\n"
												"每个号码最大数为36，一共7组数字\n"
												"PS:如已购买了票彩则会覆盖已购买的号码及倍数\n");
	
	return TRUE;
}


BOOL NPC_ActionLotteryAddTime( int talker, char *buf)
{
	int fd = getfdFromCharaIndex( talker);
	if( fd == - 1 ) return FALSE;

	if(CHAR_getInt(talker, CHAR_AMPOINT)<atoi(buf)){
		lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE, 
											WINDOW_BUTTONTYPE_OKCANCEL,	-1,	-1,	"很抱歉，你的积分不足！");
		return FALSE;
	}
	
	struct tm tm1;
	int lotterydate = CHAR_getInt(talker, CHAR_LOTTERY_DATE);
	int lotterytime = CHAR_getInt(talker, CHAR_LOTTERY_TIME);
	memcpy(&tm1, localtime((time_t *)&lotterydate), sizeof(tm1));

	int value[7];
	char *lotteryvalue=CHAR_getChar(talker, CHAR_LOTTERY_VALUE);
	if(strcmp(lotteryvalue,"")==0 || lotterydate==0 || lotterytime==0){
		lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE, WINDOW_BUTTONTYPE_OKCANCEL,	-1,	-1,	"你未购买彩票");
	}else{
		char token[256];
		
		int i;
		for(i=0;i<7;i++){
			if(getStringFromIndexWithDelim(lotteryvalue,",",i+1,token,sizeof(token))==TRUE){
				value[i]=atoi(token);
			}else{
				value[i]=0;
			}
		}
		{
			int i;
			int playernum = CHAR_getPlayerMaxNum();
			char token[256];
			sprintf(token, "玩家 %s 在投注站增加 %d 倍彩票", CHAR_getChar(talker, CHAR_NAME), atoi(buf));
			for( i = 0 ; i < playernum ; i++) {
				if( CHAR_getCharUse(i) != FALSE ) {
					CHAR_talkToCli( i, -1, token, CHAR_COLORBLUE);
				}
			}
		}
		
		CHAR_setInt(talker, CHAR_LOTTERY_TIME, CHAR_getInt(talker, CHAR_LOTTERY_TIME) + atoi(buf));
		
		CHAR_setInt(talker, CHAR_AMPOINT, CHAR_getInt(talker, CHAR_AMPOINT) - atoi(buf));
		
		lotterytime = CHAR_getInt(talker, CHAR_LOTTERY_TIME);
		
		sprintf(token, "你购买的%04d%02d%02d期的彩票 × %d;号码为：\n %0d,%0d,%0d,%0d,%0d,%0d,%0d", tm1.tm_year+1900,
																																													tm1.tm_mon+1,
																																													tm1.tm_mday,
																																													lotterytime,
																																													value[0],
																																													value[1],
																																													value[2],
																																													value[3],
																																													value[4],
																																													value[5],
																																													value[6]);
																																													
		lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE, WINDOW_BUTTONTYPE_OKCANCEL,	-1,	-1,	token);
		
	}
	return TRUE;
}


BOOL NPC_ActionLotteryShow( int talker)
{
	int fd = getfdFromCharaIndex( talker);
	if( fd == - 1 ) return FALSE;
	char token[256];
	struct tm now;
	time_t timep;
	time(&timep);
	memcpy(&now, localtime(&timep), sizeof(now));

	sprintf(token, "%04d%02d%02d期的中奖号码为：\n %0d,%0d,%0d,%0d,%0d,%0d,%0d", 	now.tm_year+1900,
																																								now.tm_mon+1,
																																								now.tm_mday,
																																								todayaward[0],
																																								todayaward[1],
																																								todayaward[2],
																																								todayaward[3],
																																								todayaward[4],
																																								todayaward[5],
																																								todayaward[6]);
																																		 
	lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE, WINDOW_BUTTONTYPE_OKCANCEL,	-1,	-1,	token);
		
	return TRUE;
}

BOOL NPC_ActionLotteryPlayerShow( int talker)
{
	int fd = getfdFromCharaIndex( talker);
	if( fd == - 1 ) return FALSE;
	char token[256];
	struct tm tm1;
	int lotterydate = CHAR_getInt(talker, CHAR_LOTTERY_DATE);
	int lotterytime = CHAR_getInt(talker, CHAR_LOTTERY_TIME);
	memcpy(&tm1, localtime((time_t *)&lotterydate), sizeof(tm1));

	int value[7];
	char *lotteryvalue=CHAR_getChar(talker, CHAR_LOTTERY_VALUE);
	if(strcmp(lotteryvalue,"")==0 || lotterydate==0 || lotterytime==0){
		lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE, WINDOW_BUTTONTYPE_OKCANCEL,	-1,	-1,	"你未购买彩票");
	}else{
		int i;
		for(i=0;i<7;i++){
			if(getStringFromIndexWithDelim(lotteryvalue,",",i+1,token,sizeof(token))==TRUE){
				value[i]=atoi(token);
			}else{
				value[i]=0;
			}
		}
	
	
		sprintf(token, "你购买的%04d%02d%02d期的彩票 × %d;号码为：\n %0d,%0d,%0d,%0d,%0d,%0d,%0d", tm1.tm_year+1900,
																																													tm1.tm_mon+1,
																																													tm1.tm_mday,
																																													lotterytime,
																																													value[0],
																																													value[1],
																																													value[2],
																																													value[3],
																																													value[4],
																																													value[5],
																																													value[6]);
																																			 
		lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE, WINDOW_BUTTONTYPE_OKCANCEL,	-1,	-1,	token);
	}
	return TRUE;
}
#endif

#ifdef _SHOW_ITEM_NAME
BOOL NPC_ActionShowItemName( int talker ,char *buf)
{
	char token[256];
	char buf1[256];
	getStringFromIndexWithDelim(buf,";",1,buf1,sizeof(buf1));
	int id=atoi(buf1);

	getStringFromIndexWithDelim(buf,";",2,buf1,sizeof(buf1));

	int itemindex = CHAR_getItemIndex( talker, CHAR_EQUIPPLACENUM);
	if( !ITEM_CHECKINDEX( itemindex ) ) {
		CHAR_talkToCli( talker, -1, "请将道具放在第一栏上",  CHAR_COLORRED);
		return TRUE;
	}
	if(ITEM_getInt(itemindex ,ITEM_ID ) == id){
		ITEM_setChar(itemindex, ITEM_FORUSERNAME, CHAR_getChar(talker, CHAR_NAME));
		ITEM_setChar(itemindex, ITEM_FORUSERCDKEY, CHAR_getChar(talker, CHAR_CDKEY));
		
		sprintf(token,  buf1, CHAR_getChar(talker, CHAR_NAME));
		ITEM_setChar(itemindex, ITEM_EFFECTSTRING, token);
		CHAR_sendItemDataOne( talker, CHAR_EQUIPPLACENUM);
		CHAR_talkToCli( talker, -1, "已成功为你道具刻名",  CHAR_COLORGREEN);
	}else{
		CHAR_talkToCli( talker, -1, "很抱歉,你的道具不符合刻名",  CHAR_COLORRED);
	}
	
	return TRUE;
}
#endif

#ifdef _SQL_BUY_FUNC
BOOL NPC_ActionOnlineBuy( int talker )
{
	char	buf[256];
	int fd = getfdFromCharaIndex( talker);
	if( fd == - 1 ) return FALSE;
	
	lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGEANDLINEINPUT, 
					WINDOW_BUTTONTYPE_OKCANCEL,
					CHAR_WINDOWTYPE_ONLINE_BUY,
					-1,
					makeEscapeString( "请输入你的提货卡密码串！\n", buf, sizeof(buf)));
	return TRUE;
}
#endif

#ifdef _ONLINE_COST	
BOOL NPC_ActionOnlineCost( int talker )
{
	char	buf[256];
	int fd = getfdFromCharaIndex( talker);
	if( fd == - 1 ) return FALSE;
	
	lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGEANDLINEINPUT, 
						WINDOW_BUTTONTYPE_OKCANCEL,
						CHAR_WINDOWTYPE_ONLINE_COST,
						-1,
						makeEscapeString( "请输入你的充值卡密码串！\n", buf, sizeof(buf)));
						
	return TRUE;
}
#endif

// WON ADD
BOOL NPC_ActionAddGold2( int talker,char *buf)
{
	int Golds=0;
	int PGold=0;
	int MaxGold = 0;
	int total_gold = 0;
	if( buf == NULL ) return FALSE;
	Golds = atoi( buf);
	PGold = CHAR_getInt( talker, CHAR_GOLD);
	MaxGold = CHAR_getMaxHaveGold( talker );

	total_gold = PGold + Golds;
	if( total_gold > MaxGold ) total_gold = MaxGold;
	
	CHAR_setInt( talker, CHAR_GOLD, total_gold );
	CHAR_send_P_StatusString( talker , CHAR_P_STRING_GOLD);
	return TRUE;
}

BOOL NPC_ActionDelPet( int talker, char *buf)
{
	int petsel;	 
	int petindex=0, k=1;
	int fd = getfdFromCharaIndex( talker );
	char msgbuf[64], szPet[128],buff2[256],buff3[256];
	char buf2[256],buff1[256];
	int defpet, ridepet;
	int petno,kNum,find=0,petLv=0,flg=-1;

	while( getStringFromIndexWithDelim(buf , "," , k, buff1, sizeof( buff1)) !=FALSE )	{ 
		k++;
    	if( strstr( buff1,"-") != NULL )	{
		getStringFromIndexWithDelim(buff1,"-",2,buff3,sizeof(buff3));
    		getStringFromIndexWithDelim(buff1,"-",1,buf2,sizeof(buf2));		

			if( strstr( buf2,"!") != NULL )	{
				flg = 0;
				getStringFromIndexWithDelim(buf2,"!=",2,buff2,sizeof(buff2));
			}else if( strstr( buf,"<") != NULL )	{
				flg = 1;
				getStringFromIndexWithDelim(buf2,"<",2,buff2,sizeof(buff2));
			}else if( strstr( buf,">") != NULL )	{
				flg = 2;
				getStringFromIndexWithDelim(buf2,">",2,buff2,sizeof(buff2));
			}else if( strstr( buf,"=") != NULL)	{
				flg = 3;
				getStringFromIndexWithDelim(buf2,"=",2,buff2,sizeof(buff2));
			}else
				return FALSE;
			petLv = atoi( buff2);
    	}
		continue;

		if(strstr(buff3,"*")!=NULL){									
			getStringFromIndexWithDelim(buff3,"*",1,buf2,sizeof(buf2));
			petno = atoi(buf2);
			getStringFromIndexWithDelim(buff3,"*",2,buf2,sizeof(buf2));
			kNum = atoi(buf2);
		}else	{
			petno = atoi( buff3);
			kNum = 30;
		}
		//andy_log
		print("petno:%d kNum:%d \n ", petno, kNum);

		find =0;

		for(petsel=0 ; petsel < CHAR_MAXPETHAVE ; petsel++)	{
			petindex = CHAR_getCharPet( talker, petsel);
			if( !CHAR_CHECKINDEX(petindex))	continue;
			if(CHAR_getInt( petindex, CHAR_PETID) !=  petno )continue;

	  		switch( flg)	{
	  		case 0:
	  			if( CHAR_getInt( petindex, CHAR_LV )  != petLv )	{
	  			}else	{
	  				continue;
	  			}
	  		break;
	  		case 1:
				if( CHAR_getInt( petindex, CHAR_LV )  <  petLv )	{
				}else	{
					continue;
				}
			break;
	  		case 2:
				if( CHAR_getInt( petindex, CHAR_LV )  > petLv )		{
				}else	{
					continue;
				}
	  		break;
	  		case 3:
				if( CHAR_getInt( petindex, CHAR_LV ) != petLv )continue;
				break;
	  		}
	  		
 			petindex = CHAR_getCharPet( talker, petsel);
			if( !CHAR_CHECKINDEX(petindex) )
				continue;
			if( CHAR_getWorkInt( CONNECT_getCharaindex( fd), CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE )
				continue;
			defpet = CHAR_getInt( talker, CHAR_DEFAULTPET);
			
			if(defpet == petsel)	{
				CHAR_setInt( talker, CHAR_DEFAULTPET, -1);
				lssproto_KS_send( fd, -1, TRUE);
			}
			
			ridepet = CHAR_getInt( talker, CHAR_RIDEPET);
			
			if(ridepet == petsel)	{
				CHAR_setInt( talker , CHAR_RIDEPET, -1 );
				CHAR_setInt( talker , CHAR_BASEIMAGENUMBER , CHAR_getInt( talker , CHAR_BASEBASEIMAGENUMBER) );
				CHAR_complianceParameter( talker );
				CHAR_sendCToArroundCharacter( CHAR_getWorkInt( talker, CHAR_WORKOBJINDEX ));
				CHAR_send_P_StatusString( talker , CHAR_P_STRING_RIDEPET);
			}

			snprintf( msgbuf,sizeof( msgbuf), "交出%s。", CHAR_getChar( petindex, CHAR_NAME));
			CHAR_talkToCli( talker, -1, msgbuf,  CHAR_COLORYELLOW);
				LogPet(			
					CHAR_getChar( talker, CHAR_NAME ),
					CHAR_getChar( talker, CHAR_CDKEY ),
					CHAR_getChar( petindex, CHAR_NAME),
					CHAR_getInt( petindex, CHAR_LV),
					"TenseiDel",
					CHAR_getInt( talker,CHAR_FLOOR),
					CHAR_getInt( talker,CHAR_X ),
					CHAR_getInt( talker,CHAR_Y ),
					CHAR_getChar( petindex, CHAR_UNIQUECODE)   // shan 2001/12/14
				);
			CHAR_setCharPet( talker, petsel, -1);
			CHAR_endCharOneArray( petindex );
			snprintf( szPet, sizeof( szPet ), "K%d", petsel);
			CHAR_sendStatusString( talker, szPet );

			find++;
			if( find >= kNum )
				break;	//条件数目皆删除完
		}
	}
    return TRUE;
}

#ifdef _NPC_NewDelPet
BOOL NPC_ActionNewDelPet( int toindex, char *msg)
{
   		int i,petindex=0,petlev,petnum,petcount,mypetlevel,petid;
		char buf[32],buf2[32];
		int flg=0,count=0;
		
		//比较方式
		if ( strstr( msg, "=" ) )
			flg = 0;
		else if( strstr( msg, "<" ) )
			flg = 1;
		else
			flg = 2;

		//等级
		if ( getStringFromIndexWithDelim( msg, "-", 1, buf, sizeof( buf )) ){
			char cmp[12];
			if( flg == 0 )
				strcpy(cmp,"=");
			else if( flg == 1 )
				strcpy(cmp,"<");
			else 
				strcpy(cmp,">");
			if ( getStringFromIndexWithDelim( buf, cmp, 2, buf2, sizeof( buf2 )) ){
			    petlev = atoi(buf2);
			}
			else
			    return FALSE;
		}
		else
            return FALSE;

		//宠物编号
        if ( getStringFromIndexWithDelim( msg, "-", 2, buf, sizeof( buf )) ){
			if ( getStringFromIndexWithDelim( buf, "*", 1, buf2, sizeof( buf2 )) ){
                petnum = atoi( buf2 );
			}
			else
				return FALSE;
		}
		else
			return FALSE;

		//数量
        if ( getStringFromIndexWithDelim( msg, "-", 2, buf, sizeof( buf )) ){
			if ( getStringFromIndexWithDelim( buf, "*", 2, buf2, sizeof( buf2 )) ){
			    petcount = atoi( buf2 );
			}
			else
				return FALSE;
		}
		else
			return FALSE;

	    for( i = 0; i < CHAR_MAXPETHAVE; i ++ ){
			BOOL cmp=FALSE;
			petindex = CHAR_getCharPet( toindex, i);
			petid = CHAR_getInt( petindex, CHAR_PETID);
			mypetlevel = CHAR_getInt( petindex, CHAR_LV);
        		
			if( flg == 0 )
				if( mypetlevel == petlev ) cmp = TRUE;
			if( flg == 1 )
				if( mypetlevel < petlev ) cmp = TRUE;
			if( flg == 2 )
				if( mypetlevel > petlev ) cmp = TRUE;

			if( petnum == petid && cmp ){//删除宠物
	            char szPet[128];
	            int defpet;
	            char msgbuf[64];
	            int fd = getfdFromCharaIndex( toindex );

                if( !CHAR_CHECKINDEX(petindex) ) return FALSE;

	            if( CHAR_getInt( toindex, CHAR_RIDEPET) == i ) {

		//CHAR_talkToCli( talker, -1, "骑乘中的宠物无法交出！", CHAR_COLORYELLOW );
    	//return	FALSE;

		            CHAR_setInt( toindex, CHAR_RIDEPET, -1);
		            CHAR_send_P_StatusString( toindex, CHAR_P_STRING_RIDEPET );
		            CHAR_complianceParameter( toindex );
		            CHAR_sendCToArroundCharacter( CHAR_getWorkInt( toindex , CHAR_WORKOBJINDEX ));
				}

	            //公及平乓仿互爵    卅日｝矢永玄毛丹日内蔽
	            if( CHAR_getWorkInt( CONNECT_getCharaindex( fd ), CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE) 
					return FALSE;
	            //及矢永玄互田玄伙井升丹井及民尼永弁
	            defpet = CHAR_getInt( toindex, CHAR_DEFAULTPET);
	            if(defpet == i){
		            CHAR_setInt( toindex, CHAR_DEFAULTPET, -1);
		            lssproto_KS_send( fd, -1, TRUE);
				}

	            snprintf( msgbuf,sizeof( msgbuf), "交出%s。",
						  CHAR_getChar( petindex, CHAR_NAME));
	            CHAR_talkToCli( toindex, -1, msgbuf,  CHAR_COLORWHITE);

	            // 矢永玄毛澎  仄凶夫弘
	            LogPet( CHAR_getChar( toindex, CHAR_NAME ), // 平乓仿
		                CHAR_getChar( toindex, CHAR_CDKEY ),
		                CHAR_getChar( petindex, CHAR_NAME),
		                CHAR_getInt( petindex, CHAR_LV),
		                "EvnetDell(任务删除)",
		                CHAR_getInt( toindex,CHAR_FLOOR),
		                CHAR_getInt( toindex,CHAR_X ),
		                CHAR_getInt( toindex,CHAR_Y ),
		                CHAR_getChar( petindex, CHAR_UNIQUECODE)   // shan 2001/12/14
	            );


	            //   切  井日壅允
	            CHAR_setCharPet( toindex, i, -1);

	            CHAR_endCharOneArray( petindex );

	            snprintf( szPet, sizeof( szPet ), "K%d", i);
	            // 蕙仄中矢永玄旦  □正旦霜曰勾仃月
	            CHAR_sendStatusString( toindex, szPet );

				++count;
                if( count >= petcount )
				    break;
			}
		}

    return TRUE;
}
#endif

BOOL NPC_ActionAddPet( int talker, char *buf)
{
	int	ret;
	char msgbuf[64], buff3[64];
	int	enemynum;
	int	i,j,k=1;
	int petindex, petindex2, petid;

  k=1;
  while( getStringFromIndexWithDelim(buf , "," , k, buff3, sizeof(buff3)) !=FALSE ){
  	k++;	
	//检查宠物栏是否有空位
	for( i = 0 ;i < CHAR_MAXPETHAVE ; i++) {
		petindex = CHAR_getCharPet( talker, i);
		if( petindex == -1  )
			break;
	}

  if( i == CHAR_MAXPETHAVE ){
		snprintf( msgbuf,sizeof( msgbuf), "宠物已满！！");
		CHAR_talkToCli( talker, -1, msgbuf,  CHAR_COLORYELLOW);
		return FALSE;
	}
#ifdef _PET_MM
	if(strstr(buff3,"MM1") || strstr(buff3,"MM3"))
		petid = 1479;
	else if(strstr(buff3,"MM2") || strstr(buff3,"MM4"))
		petid = 2547;
	else
		petid = atoi( buff3);
#else
	petid = atoi( buff3);
#endif


#ifdef _NPC_GET_PETABI
	{
		char buf1[32];
		easyGetTokenFromString( buf , 1 , buf1, sizeof(buf1));
		if(strstr(buf1,"PETABI")){
			easyGetTokenFromString( buf , 2 , buf1, sizeof(buf1));
			petid = atoi(buf1);
		}
	}
#endif


	enemynum = ENEMY_getEnemyNum();
	for( i = 0; i < enemynum; i ++ ) {
		if( ENEMY_getInt( i, ENEMY_ID ) == petid )
		//if( ENEMY_getInt( i, ENEMY_TEMPNO ) == petid )
			break;
	}
	
	if( i == enemynum )
		continue;
		
	ret = ENEMY_createPetFromEnemyIndex( talker, i);
	for( i = 0; i < CHAR_MAXPETHAVE; i ++ )	{
		if( CHAR_getCharPet( talker, i ) == ret )
			break;
	}
	if( i == CHAR_MAXPETHAVE )
		i = 0;	

	if( CHAR_CHECKINDEX( ret ) == TRUE ){
#ifdef _PET_MM	
	if(strstr(buff3,"MM1") || strstr(buff3,"MM2")){
		int LevelUpPoint;
		LevelUpPoint = ( 50<< 24) + ( 50<< 16) + ( 50<< 8) + ( 50<< 0);
		CHAR_setInt( ret, CHAR_ALLOCPOINT, LevelUpPoint);
		}else if(strstr(buff3,"MM3") || strstr(buff3,"MM4")){
			while(CHAR_getInt( ret, CHAR_LV)<79){	//升级
				int LevelUpPoint;
				LevelUpPoint = ( 50<< 24) + ( 50<< 16) + ( 50<< 8) + ( 50<< 0);
				CHAR_setInt( ret, CHAR_ALLOCPOINT, LevelUpPoint);
				CHAR_PetLevelUp( ret );
				CHAR_PetAddVariableAi( ret, AI_FIX_PETLEVELUP );
				CHAR_setInt( ret, CHAR_LV, CHAR_getInt( ret, CHAR_LV) +1 );
			}
		}
#endif


#ifdef _NPC_GET_PETABI
	{
		char buf1[32];
		easyGetTokenFromString( buf , 1 , buf1, sizeof(buf1));
		if(strstr(buf,"PETABI")){
			int lv, trans, vital, str, tough, dex;
			easyGetTokenFromString( buf , 3 , buf1, sizeof(buf1));
			lv=atoi(buf1);
			easyGetTokenFromString( buf , 4 , buf1, sizeof(buf1));
			trans=atoi(buf1);
			easyGetTokenFromString( buf , 5 , buf1, sizeof(buf1));
			vital=atoi(buf1);
			easyGetTokenFromString( buf , 6 , buf1, sizeof(buf1));
			str=atoi(buf1);
			easyGetTokenFromString( buf , 7 , buf1, sizeof(buf1));
			tough=atoi(buf1);
			easyGetTokenFromString( buf , 8 , buf1, sizeof(buf1));
			dex=atoi(buf1);
			
			CHAR_setInt( ret, CHAR_LV, lv);
			CHAR_setInt( ret, CHAR_TRANSMIGRATION, trans);
			CHAR_setInt( ret, CHAR_VITAL, vital*20);
			CHAR_setInt( ret, CHAR_STR, str*80);
			CHAR_setInt( ret, CHAR_TOUGH, tough*80);
			CHAR_setInt( ret, CHAR_DEX, dex*100);
			CHAR_setMaxExpFromLevel( ret, CHAR_getInt( ret, CHAR_LV ));
			CHAR_setInt(ret,CHAR_VARIABLEAI,10000);

		}
	}
#endif


		CHAR_setInt( ret, CHAR_HP, CHAR_getWorkInt( ret, CHAR_WORKMAXHP ));
		CHAR_setMaxExpFromLevel( ret, CHAR_getInt( ret, CHAR_LV ));
		CHAR_setInt(ret,CHAR_VARIABLEAI,10000);
	}
	
	petindex2 = CHAR_getCharPet(talker, i);
	if( !CHAR_CHECKINDEX( petindex2) )
		continue;
	snprintf( msgbuf,sizeof( msgbuf), "拿到%s。", CHAR_getChar(petindex2,CHAR_NAME));
	CHAR_talkToCli( talker, -1, msgbuf,  CHAR_COLORYELLOW);
	for(j = 0; j < CHAR_MAXPETHAVE; j++){
		petindex = CHAR_getCharPet(talker, j);
		if( !CHAR_CHECKINDEX( petindex) )
			continue;
		CHAR_complianceParameter( petindex );
		snprintf( msgbuf, sizeof( msgbuf ), "K%d", j );
		CHAR_sendStatusString( talker, msgbuf );
		snprintf( msgbuf, sizeof( msgbuf ), "W%d", j );
		CHAR_sendStatusString( talker, msgbuf );
	}

	LogPet(
		CHAR_getChar( talker, CHAR_NAME ),
		CHAR_getChar( talker, CHAR_CDKEY ),
		CHAR_getChar( petindex2, CHAR_NAME),
		CHAR_getInt( petindex2, CHAR_LV),
		"TenseiGet",
		CHAR_getInt( talker,CHAR_FLOOR),
		CHAR_getInt( talker,CHAR_X ),
		CHAR_getInt( talker,CHAR_Y ),
		CHAR_getChar( petindex2, CHAR_UNIQUECODE)   // shan 2001/12/14
	);
  }

	return TRUE;
}

#ifdef _ITEM_PILENUMS
BOOL NPC_DelItem( int toindex, int ti, int num)
{
	int itemindex, pilenum;

	itemindex = CHAR_getItemIndex( toindex , ti);
	if( !ITEM_CHECKINDEX(itemindex) ) return FALSE;
	pilenum = ITEM_getInt( itemindex, ITEM_USEPILENUMS);
	if( pilenum < num ) return FALSE;

	pilenum = pilenum - num;
	if( pilenum <= 0 ){
	{
		LogItem(
			CHAR_getChar( toindex, CHAR_NAME ),
			CHAR_getChar( toindex, CHAR_CDKEY ),
#ifdef _add_item_log_name
			itemindex,
#else
	   		ITEM_getInt( itemindex, ITEM_ID ),
#endif
			"Sell(卖道具)",
			CHAR_getInt( toindex,CHAR_FLOOR),
			CHAR_getInt( toindex,CHAR_X ),
			CHAR_getInt( toindex,CHAR_Y ),
			ITEM_getChar( itemindex, ITEM_UNIQUECODE),
			ITEM_getChar( itemindex, ITEM_NAME),
			ITEM_getInt( itemindex, ITEM_ID)

		);
	}
		CHAR_setItemIndex( toindex, ti, -1);
		ITEM_endExistItemsOne( itemindex);
	}else{
		ITEM_setInt( itemindex, ITEM_USEPILENUMS, pilenum);
	}
	CHAR_sendItemDataOne( toindex, ti);
	return TRUE;
}


void NPC_ActionDoPileClearItem( int toindex, int itemID)
{
	int i, itemindex;
	char token[256];

	for( i = CHAR_STARTITEMARRAY ; i <  CheckCharMaxItem(toindex) ; i++ ){
		itemindex = CHAR_getItemIndex( toindex , i );
		if( !ITEM_CHECKINDEX(itemindex) ) continue;
		if( ITEM_getInt(itemindex, ITEM_ID) != itemID ) continue;
		sprintf( token, "交出道具%s",ITEM_getChar( itemindex, ITEM_NAME));
		CHAR_talkToCli( toindex, -1, token, CHAR_COLORYELLOW);

						LogItem(
							CHAR_getChar( toindex, CHAR_NAME ),
							CHAR_getChar( toindex, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD 在item的log中增加item名称
							itemindex,
#else
							ITEM_getInt( itemindex, ITEM_ID),
#endif
							"任务收取",
							CHAR_getInt( toindex,CHAR_FLOOR),
							CHAR_getInt( toindex,CHAR_X ),
							CHAR_getInt( toindex,CHAR_Y ),
							ITEM_getChar( itemindex, ITEM_UNIQUECODE),
							ITEM_getChar( itemindex, ITEM_NAME),
							ITEM_getInt( itemindex, ITEM_ID)

						);
			CHAR_setItemIndex( toindex, i, -1);
			ITEM_endExistItemsOne( itemindex);
			CHAR_sendItemDataOne( toindex, i);
	}
}

BOOL NPC_ActionDoPileDelItem( int toindex, int itemID, int num)
{
	int i, itemindex, finditem=0, pilenum=0, onenum=0;
	char token[256];

	for( i = CHAR_STARTITEMARRAY ; i < CheckCharMaxItem(toindex) && (finditem<num || num==-1); i++ ){
		itemindex = CHAR_getItemIndex( toindex , i );
		onenum = 0;
		if( !ITEM_CHECKINDEX(itemindex) ) continue;
		if( ITEM_getInt(itemindex, ITEM_ID) != itemID ) continue;
		pilenum = ITEM_getInt( itemindex, ITEM_USEPILENUMS);
		if( pilenum >= (num-finditem) ){
			pilenum -= (num-finditem);
			onenum += (num-finditem);
		}else {
			onenum = pilenum;
			pilenum=0;
		}
		finditem += onenum;
		
		ITEM_setInt( itemindex, ITEM_USEPILENUMS, pilenum);
		sprintf( token, "交出%d个%s", onenum, ITEM_getChar( itemindex, ITEM_NAME));
		CHAR_talkToCli( toindex, -1, token, CHAR_COLORYELLOW);
		if( pilenum <= 0 ){
						LogItem(
							CHAR_getChar( toindex, CHAR_NAME ),
							CHAR_getChar( toindex, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD 在item的log中增加item名称
							itemindex,
#else
							ITEM_getInt( itemindex, ITEM_ID),
#endif
							"任务收取",
							CHAR_getInt( toindex,CHAR_FLOOR),
							CHAR_getInt( toindex,CHAR_X ),
							CHAR_getInt( toindex,CHAR_Y ),
							ITEM_getChar( itemindex, ITEM_UNIQUECODE),
							ITEM_getChar( itemindex, ITEM_NAME),
							ITEM_getInt( itemindex, ITEM_ID)

						);


			CHAR_setItemIndex( toindex, i, -1);
			ITEM_endExistItemsOne( itemindex);
		}
		CHAR_sendItemDataOne( toindex, i);
	}

	if( finditem != num && num != -1 ) return FALSE;
	return TRUE;
}

BOOL NPC_ActionDelItem( int talker, char *buf)
{
	char buff3[128], buf2[32];
	int k=1, itemid, neednum;
	if( !CHAR_CHECKINDEX( talker) ) return FALSE;
	while( getStringFromIndexWithDelim(buf , "," , k++, buff3, sizeof(buff3)) !=FALSE )	{
		neednum = 1;
		if( getStringFromIndexWithDelim( buff3, "*", 1, buf2, sizeof(buf2)) == FALSE ) continue;
		itemid = atoi( buf2);
		if( getStringFromIndexWithDelim( buff3, "*", 2, buf2, sizeof(buf2)) == TRUE ){
			neednum = atoi( buf2);
			if( NPC_ActionDoPileDelItem( talker, itemid, neednum) == FALSE ) return FALSE;
		}else{
			NPC_ActionDoPileClearItem( talker, itemid);
		}
	}
	return TRUE;
}
#else
BOOL NPC_ActionDelItem( int talker, char *buf)
{
	int i = 1, j = 1,k = 1;
	char buff3[128];
	char buf2[32];
	int itemindex;
	char token[256];
	
	if( !CHAR_CHECKINDEX( talker) ) return FALSE;
	while( getStringFromIndexWithDelim(buf , "," , k, buff3, sizeof(buff3)) !=FALSE )	{
		k++;
		if(strstr(buff3,"*")!=NULL){
			int itemno, kosuu=0, id, cnt=0;
			struct _tagItemArray {
				int index;
				int itemindex;
			};
			struct _tagItemArray ItemArray[20];
			for( i=0; i<20 ; i++){
				ItemArray[i].itemindex = -1;
				ItemArray[i].index = -1;
			}
			//取得字串
			getStringFromIndexWithDelim(buff3,"*",1,buf2,sizeof(buf2));
			itemno = atoi(buf2);
			getStringFromIndexWithDelim(buff3,"*",2,buf2,sizeof(buf2));
			kosuu = atoi(buf2);
			//取得 item index
			for( i = CHAR_STARTITEMARRAY ; i < CheckCharMaxItem(talker) ; i++ ){
				itemindex=CHAR_getItemIndex( talker , i );
				if( ITEM_CHECKINDEX(itemindex) ){
					id=ITEM_getInt(itemindex ,ITEM_ID );
					if(itemno==id){
						ItemArray[cnt].itemindex = itemindex;
						ItemArray[cnt].index = i;
						cnt++;
					}
				}
				if(cnt == kosuu)
					break;
			}
			if(cnt != kosuu){
				CHAR_talkToCli( talker, -1,"条件不足", CHAR_COLORYELLOW);
				return FALSE;
			}
			//收取物品
			for( i=0; i<20; i++)	{
				itemindex = ItemArray[i].itemindex;
				if( !ITEM_CHECKINDEX(itemindex) ) break;
				sprintf(token,"交出%s",ITEM_getChar( itemindex, ITEM_NAME));
				CHAR_talkToCli( talker, -1, token, CHAR_COLORYELLOW);
				CHAR_setItemIndex( talker, ItemArray[i].index ,-1);
				ITEM_endExistItemsOne( itemindex);
				CHAR_sendItemDataOne( talker, ItemArray[i].index);
				LogItem(
					CHAR_getChar( talker, CHAR_NAME ), CHAR_getChar( talker, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD 在item的log中增加item名称
					itemindex,
#else
					ITEM_getInt( itemindex, ITEM_ID),
#endif
					"WarpManDelItem",
					CHAR_getInt( talker, CHAR_FLOOR), CHAR_getInt( talker, CHAR_X ),
					CHAR_getInt( talker, CHAR_Y ), ITEM_getChar( itemindex, ITEM_UNIQUECODE),
					ITEM_getChar( itemindex, ITEM_NAME),
					ITEM_getInt( itemindex, ITEM_ID)
					);
			}
		}else{
			for( j = CHAR_STARTITEMARRAY ;  j < CheckCharMaxItem(talker) ; j++){
				itemindex = CHAR_getItemIndex( talker ,j);
				if( ITEM_CHECKINDEX(itemindex) ){
					if( atoi( buff3) == ITEM_getInt(itemindex,ITEM_ID)){
						LogItem(
							CHAR_getChar( talker, CHAR_NAME ),
							CHAR_getChar( talker, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD 在item的log中增加item名称
							itemindex,
#else
							ITEM_getInt( itemindex, ITEM_ID),
#endif
							"WarpManDelItem",
							CHAR_getInt( talker,CHAR_FLOOR),
							CHAR_getInt( talker,CHAR_X ),
							CHAR_getInt( talker,CHAR_Y ),
							ITEM_getChar( itemindex, ITEM_UNIQUECODE),
							ITEM_getChar( itemindex, ITEM_NAME),
							ITEM_getInt( itemindex, ITEM_ID)

						);
						sprintf( token, "交出%s。",
										ITEM_getChar( itemindex, ITEM_NAME));
						CHAR_talkToCli( talker, -1, token, CHAR_COLORYELLOW);
						CHAR_DelItem( talker, j);
					}
				}
			}
		}
	}
	return TRUE;
}
#endif

#ifdef _VIP_SHOP
BOOL NPC_ActionVipItem( int talker, char *buf)
{
  int itemindex=-1;
  int pVipPoint=0, VipPoints=0;
  char buff[256], token[256];
  int ret;
	int itemID=0;
	if(getStringFromIndexWithDelim(buf , "," , 1, buff, sizeof(buff)) !=FALSE)
		itemID = atoi(buf);

  itemindex = CHAR_findEmptyItemBox( talker );
	if( itemindex < 0 )	{
		CHAR_talkToCli( talker, -1, "物品栏空间不足！！",  CHAR_COLORYELLOW);
		return FALSE;
	}
	
	if(getStringFromIndexWithDelim(buf , "," , 2, buff, sizeof(buff)) !=FALSE)
		VipPoints=atoi(buff);
	if(VipPoints<0)VipPoints=0;

  pVipPoint = CHAR_getInt( talker, CHAR_AMPOINT);
	if( pVipPoint < VipPoints )	{
		CHAR_talkToCli( talker, -1, "你身上的积分点数不够哦", CHAR_COLORYELLOW);
		return FALSE;
	}
	
	itemindex = ITEM_makeItemAndRegist( itemID );
  if( itemindex != -1 ){
		ret = CHAR_addItemSpecificItemIndex( talker, itemindex);
		if( ret < 0 || ret >= CheckCharMaxItem(talker) ) {
			ITEM_endExistItemsOne( itemindex);
			print ("\n ret error!!");
			return FALSE;
		}

		sprintf( token,"拿到%s",ITEM_getChar( itemindex, ITEM_NAME));
		CHAR_talkToCli( talker, -1, token, CHAR_COLORYELLOW );
		
		CHAR_setInt( talker, CHAR_AMPOINT, pVipPoint-VipPoints);
#ifdef _AMPOINT_LOG
		LogAmPoint(CHAR_getChar( talker, CHAR_NAME ),
									         	CHAR_getChar( talker, CHAR_CDKEY ),
									          -VipPoints,
									   				CHAR_getInt( talker, CHAR_AMPOINT ),
									         	"(积分购物)",
									         	CHAR_getInt( talker,CHAR_FLOOR),
									         	CHAR_getInt( talker,CHAR_X ),
									         	CHAR_getInt( talker,CHAR_Y ));
#endif		
		sprintf( token, "扣除积分点数%d", VipPoints);
		CHAR_talkToCli( talker, -1, token, CHAR_COLORYELLOW);

		CHAR_sendItemDataOne( talker, ret);
  }

	return TRUE;	                                                                                                                                                                      
}

BOOL NPC_ActionVipPet( int talker, char *buf)
{
	int	ret;
	char msgbuf[64], buf1[8], buf2[10];
	int	enemynum;
	int	i,j;
	int petindex, petindex2, petid;
  int pVipPoint=0, VipPoints=0;
		
 	if(getStringFromIndexWithDelim(buf , "," , 1, buf1, sizeof(buf1)) ==FALSE)return FALSE;
#ifdef _PET_MM
	if(strstr(buf1,"MM1") || strstr(buf1,"MM3")){
		petid = 1479;
	}else if(strstr(buf1,"MM2") || strstr(buf1,"MM4")){
		petid = 2547;
	}else{
		petid = atoi( buf1 );
	}
#else
	petid = atoi( buf1 );
#endif
		
#ifdef _NPC_GET_PETABI
	{
		char buf1[32];
		easyGetTokenFromString( buf , 1 , buf1, sizeof(buf1));
		//检查是否PETABI字段
		if(strstr(buf1,"PETABI")){
			easyGetTokenFromString( buf , 2 , buf1, sizeof(buf1));
			petid = atoi(buf1);
		}
	}
#endif
		
	if(getStringFromIndexWithDelim(buf , "," , 2, buf2, sizeof(buf2)) !=FALSE)
		VipPoints=atoi(buf2);
	if(VipPoints<0)VipPoints=0;

  pVipPoint = CHAR_getInt( talker, CHAR_AMPOINT);
	if( pVipPoint < VipPoints )	{
		CHAR_talkToCli( talker, -1, "你身上的积分点数不够哦", CHAR_COLORYELLOW);
		return FALSE;
	}

	for( i = 0 ;i < CHAR_MAXPETHAVE ; i++) {
		petindex = CHAR_getCharPet( talker, i);
		if( petindex == -1  )
			break;
	}

  if( i == CHAR_MAXPETHAVE )      {
		snprintf( msgbuf,sizeof( msgbuf), "宠物已满！！");
		CHAR_talkToCli( talker, -1, msgbuf,  CHAR_COLORYELLOW);
		return FALSE;
	}

	enemynum = ENEMY_getEnemyNum();
	for( i = 0; i < enemynum; i ++ ) {
		if( ENEMY_getInt( i, ENEMY_ID) == petid) {
				break;
			}
	}

	if( i == enemynum )
		return FALSE;

	ret = ENEMY_createPetFromEnemyIndex( talker, i);
	for( i = 0; i < CHAR_MAXPETHAVE; i ++ )	{
		if( CHAR_getCharPet( talker, i ) == ret )
			break;
	}
	if( i == CHAR_MAXPETHAVE )
		CHAR_talkToCli( talker, -1, "领取宠物失败！", CHAR_COLORYELLOW);

	if( CHAR_CHECKINDEX( ret ) == TRUE ){
#ifdef _PET_MM
		if(strstr(buf1,"MM1") || strstr(buf1,"MM2")){
			int LevelUpPoint;
			LevelUpPoint = ( 50<< 24) + ( 50<< 16) + ( 50<< 8) + ( 50<< 0);
			CHAR_setInt( ret, CHAR_ALLOCPOINT, LevelUpPoint);
		}else if(strstr(buf1,"MM3") || strstr(buf1,"MM4")){
			while(CHAR_getInt( ret, CHAR_LV)<79){	//升级
				int LevelUpPoint;
				LevelUpPoint = ( 50<< 24) + ( 50<< 16) + ( 50<< 8) + ( 50<< 0);
				CHAR_setInt( ret, CHAR_ALLOCPOINT, LevelUpPoint);
				CHAR_PetLevelUp( ret );
				CHAR_PetAddVariableAi( ret, AI_FIX_PETLEVELUP );
				CHAR_setInt( ret, CHAR_LV, CHAR_getInt( ret, CHAR_LV) +1 );
			}
		}
#endif

#ifdef _NPC_GET_PETABI
	{
		char buf1[32];
		easyGetTokenFromString( buf , 1 , buf1, sizeof(buf1));
		if(strstr(buf,"PETABI")){
			int lv, trans, vital, str, tough, dex;
			//获取等级
			easyGetTokenFromString( buf , 3 , buf1, sizeof(buf1));
			lv=atoi(buf1);
			//获取转生
			easyGetTokenFromString( buf , 4 , buf1, sizeof(buf1));
			trans=atoi(buf1);
			//获取血
			easyGetTokenFromString( buf , 5 , buf1, sizeof(buf1));
			vital=atoi(buf1);
			//获取攻
			easyGetTokenFromString( buf , 6 , buf1, sizeof(buf1));
			str=atoi(buf1);
			//获取防
			easyGetTokenFromString( buf , 7 , buf1, sizeof(buf1));
			tough=atoi(buf1);
			//获取敏
			easyGetTokenFromString( buf , 8 , buf1, sizeof(buf1));
			dex=atoi(buf1);
			
			//设置等级
			CHAR_setInt( ret, CHAR_LV, lv);
			//设置转数
			CHAR_setInt( ret, CHAR_TRANSMIGRATION, trans);
			//设置血
			CHAR_setInt( ret, CHAR_VITAL, vital*20);
			//设置攻
			CHAR_setInt( ret, CHAR_STR, str*80);
			//设置防
			CHAR_setInt( ret, CHAR_TOUGH, tough*80);
			//设置敏
			CHAR_setInt( ret, CHAR_DEX, dex*100);
			//设置经验
			CHAR_setMaxExpFromLevel( ret, CHAR_getInt( ret, CHAR_LV ));
			//设置忠
			CHAR_setInt(ret,CHAR_VARIABLEAI,10000);

		}
	}
#endif

		CHAR_setInt( ret, CHAR_HP, CHAR_getWorkInt( ret, CHAR_WORKMAXHP ));
		CHAR_setMaxExpFromLevel( ret, CHAR_getInt( ret, CHAR_LV ));
		CHAR_setInt(ret,CHAR_VARIABLEAI,10000);
	}
	
	petindex2 = CHAR_getCharPet(talker, i);
	if( !CHAR_CHECKINDEX( petindex2) )
		return FALSE;
	snprintf( msgbuf,sizeof( msgbuf), "拿到%s。", CHAR_getChar(petindex2,CHAR_NAME));
	CHAR_talkToCli( talker, -1, msgbuf,  CHAR_COLORYELLOW);
	CHAR_setInt( talker, CHAR_AMPOINT, pVipPoint-VipPoints);
#ifdef _AMPOINT_LOG
	LogAmPoint(CHAR_getChar( talker, CHAR_NAME ),
									         	CHAR_getChar( talker, CHAR_CDKEY ),
									          -VipPoints,
									   				CHAR_getInt( talker, CHAR_AMPOINT ),
									         	"(积分购宠)",
									         	CHAR_getInt( talker,CHAR_FLOOR),
									         	CHAR_getInt( talker,CHAR_X ),
									         	CHAR_getInt( talker,CHAR_Y ));
#endif	
	snprintf( msgbuf,sizeof( msgbuf), "扣除积分点数%d", VipPoints);
	CHAR_talkToCli( talker, -1, msgbuf, CHAR_COLORYELLOW);
	for(j = 0; j < CHAR_MAXPETHAVE; j++){
		petindex = CHAR_getCharPet(talker, j);
		if( !CHAR_CHECKINDEX( petindex) )
			continue;
		CHAR_complianceParameter( petindex );
		snprintf( msgbuf, sizeof( msgbuf ), "K%d", j );
		CHAR_sendStatusString( talker, msgbuf );
		snprintf( msgbuf, sizeof( msgbuf ), "W%d", j );
		CHAR_sendStatusString( talker, msgbuf );
	}
	
	return TRUE;
}
#endif

#ifdef _NEW_VIP_SHOP
BOOL NPC_ActionNewVipItem( int talker, char *buf)
{
	char buf1[1024];
  int itemindex = CHAR_findEmptyItemBox( talker );
	if( itemindex < 0 )	{
		CHAR_talkToCli( talker, -1, "物品栏空间不足！！",  CHAR_COLORYELLOW);
		return FALSE;
	}
	
	CHAR_talkToCli( talker, -1, "取获数据中，请稍待...",  CHAR_COLORRED);
	int Points=0;
	if(getStringFromIndexWithDelim(buf , "," , 2, buf1, sizeof(buf1)) !=FALSE)
		Points=atoi(buf1);
	if(Points<0)Points=0;
	int fd = getfdFromCharaIndex( talker );
	char *id = CHAR_getChar(talker,CHAR_CDKEY);
#ifdef _SQL_VIPPOINT_LOG
	LogSqlVipPoint(CHAR_getChar( talker, CHAR_NAME ),
									         	CHAR_getChar( talker, CHAR_CDKEY ),
									         	"(消费道具)",
									         	-Points,
									         	CHAR_getInt( talker,CHAR_FLOOR),
									         	CHAR_getInt( talker,CHAR_X ),
									         	CHAR_getInt( talker,CHAR_Y ));
#endif
#ifdef _OTHER_SAAC_LINK
	if(osfd == -1){
		OtherSaacConnect();
		CHAR_talkToCli( talker, -1, "点卷服务器未正常连接!", CHAR_COLORRED );
		return FALSE;
	}
	saacproto_NewVipShop_send(osfd, fd, id, - Points, buf, 0);
#else
	saacproto_NewVipShop_send(acfd, fd, id, - Points, buf, 0);
#endif
	return TRUE;

}

BOOL NPC_ActionNewVipPet( int talker, char *buf)
{
	int i;
	char buf1[1024];
	for( i = 0 ;i < CHAR_MAXPETHAVE ; i++) {
		int petindex = CHAR_getCharPet( talker, i);
		if( petindex == -1  )
			break;
	}

  if( i == CHAR_MAXPETHAVE )      {
		snprintf( buf1,sizeof( buf1), "宠物已满！！");
		CHAR_talkToCli( talker, -1, buf1,  CHAR_COLORYELLOW);
		return FALSE;
	}
	
	CHAR_talkToCli( talker, -1, "取获数据中，请稍待...",  CHAR_COLORRED);
	int Points=0;
	if(getStringFromIndexWithDelim(buf , "," , 2, buf1, sizeof(buf1)) !=FALSE)
		Points=atoi(buf1);
	if(Points<0)Points=0;
	int fd = getfdFromCharaIndex( talker );
	char *id = CHAR_getChar(talker,CHAR_CDKEY);
#ifdef _SQL_VIPPOINT_LOG
	LogSqlVipPoint(CHAR_getChar( talker, CHAR_NAME ),
									         	CHAR_getChar( talker, CHAR_CDKEY ),
									         	"(消费购宠)",
									         	-Points,
									         	CHAR_getInt( talker,CHAR_FLOOR),
									         	CHAR_getInt( talker,CHAR_X ),
									         	CHAR_getInt( talker,CHAR_Y ));
#endif
#ifdef _OTHER_SAAC_LINK
	if(osfd == -1){
		OtherSaacConnect();
		CHAR_talkToCli( talker, -1, "点卷服务器未正常连接!", CHAR_COLORRED );
		return FALSE;
	}
	saacproto_NewVipShop_send(osfd, fd, id, - Points, buf, 1);
#else
	saacproto_NewVipShop_send(acfd, fd, id, - Points, buf, 1);
#endif
	return TRUE;
}
#endif

#ifdef _TRANS_7_NPC
BOOL NPC_ActionTrans7( int talker, char *buf)
{
	char token[128];
	int work[10];
	int Trans=CHAR_getInt(talker,CHAR_TRANSMIGRATION)+1;
#ifdef _ADD_POOL_ITEM			   // WON ADD 增加可寄放的道具
	int tran_pool_item[5] = { 4, 4, 4, 4, 4 };		// 增加的道具寄放数
	int tran_pool_pet[5] = { 2, 2, 2, 2, 2};		// 增加的宠物寄放数
	int j,item_sum=0, pet_sum=0;
	
	if(Trans>7 || CHAR_getInt(talker,CHAR_LV)<80){
		sprintf( token, "由于你已七转或等级小于80级，所以转生失败!");
		CHAR_talkToCli( talker, -1, token, CHAR_COLORYELLOW );
		return FALSE;
	}
	
	for(j=0; j<5 ;j++){
		item_sum += tran_pool_item[j] ;			
		pet_sum += tran_pool_pet[j];		
	}

	item_sum += 10;								// 限制最大的寄道具数
	pet_sum  += 5;							    // 限制最大的寄宠数

#endif
	NPC_TransmigrationStatus(talker, talker, work);
	NPC_TransmigrationFlg_CLS(talker, talker);
	CHAR_setInt(talker ,CHAR_TRANSMIGRATION, Trans);
	CHAR_setInt(talker, CHAR_LV ,1);
	CHAR_setMaxExp( talker, 0);
  CHAR_setInt( talker,CHAR_SKILLUPPOINT,CHAR_getInt( talker, CHAR_TRANSMIGRATION)*10);
  CHAR_Skillupsend( talker );
  CHAR_setInt( talker,CHAR_RIDEPET, -1 );
  CHAR_setInt( talker , CHAR_BASEIMAGENUMBER , CHAR_getInt( talker , CHAR_BASEBASEIMAGENUMBER) );
	CHAR_sendStatusString( talker , "P");
	return TRUE;
}
#endif

BOOL NPC_ActionAddItem( int talker, char *buf)
{
  int itemID,k=0,itemindex=-1;
  int spaceNum=5,i;
  char buff3[256], msgbuf[64], token[256];
  int ret;
  k=1;
  while( getStringFromIndexWithDelim(buf , "," , k, buff3, sizeof(buff3)) !=FALSE ){
	k++;
		for( i = spaceNum ; i < CheckCharMaxItem(talker) ; i++ ){
			itemindex=CHAR_getItemIndex( talker , i );
			if( itemindex == -1 )	{
				spaceNum = i+1;
				break;
			}
		}                                                                                                                                                                                                                                                                                                                                      
		if( i == CheckCharMaxItem(talker) )	{
			snprintf( msgbuf,sizeof( msgbuf), "物品栏空间不足！！");
			CHAR_talkToCli( talker, -1, msgbuf,  CHAR_COLORYELLOW);
			return FALSE;
		}
  }
  k=1;
  while( getStringFromIndexWithDelim(buf , "," , k, buff3, sizeof(buff3)) !=FALSE ){
	k++;
        itemID = atoi( buff3);
        if( itemID) itemindex = ITEM_makeItemAndRegist( itemID);
		if(itemindex == -1)
			continue;
		ret = CHAR_addItemSpecificItemIndex( talker, itemindex);
		if( ret < 0 || ret >= CheckCharMaxItem(talker) ) {
			ITEM_endExistItemsOne( itemindex);
			print ("\n ret error!!");
			return FALSE;
		}
			LogItem(
				CHAR_getChar( talker, CHAR_NAME ),
				CHAR_getChar( talker, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD 在item的log中增加item名称
				itemindex,
#else
				atoi( message),
#endif
				"AddItem(制作道具NPC)",
		      CHAR_getInt( talker,CHAR_FLOOR),
					CHAR_getInt( talker,CHAR_X ),
		      CHAR_getInt( talker,CHAR_Y ),
		      ITEM_getChar( itemindex, ITEM_UNIQUECODE),
					ITEM_getChar( itemindex, ITEM_NAME),
					ITEM_getInt( itemindex, ITEM_ID)
		
			);
		sprintf( token,"拿到%s",ITEM_getChar( itemindex, ITEM_NAME));
		CHAR_talkToCli( talker, -1, token, CHAR_COLORYELLOW );
		CHAR_sendItemDataOne( talker, ret);
  }

	return TRUE;	                                                                                                                                                                      
}

BOOL NPC_ActionSetEend( int talkerindex, char * buf)
{
	int eveno;
	int talkNo=1, iflg=0;
	char buf1[256];

	while( getStringFromIndexWithDelim( buf,",",talkNo,buf1,sizeof( buf1)) != FALSE)	{ 
		talkNo++;
		eveno = atoi( buf);
		NPC_EventSetFlg( talkerindex, eveno);
		if( ++iflg > 20 ) break;
	}
	return TRUE;
}

BOOL NPC_ActionClearEvent( int toindex, char * buf)
{
	int eveno;
	int talkNo=1, iflg=0;
	char buf1[256];

	while( getStringFromIndexWithDelim( buf,",",talkNo,buf1,sizeof( buf1)) != FALSE)	{ 
		talkNo++;
		eveno = atoi( buf);
		NPC_NowEndEventSetFlgCls( toindex, eveno);
		if( ++iflg > 20 ) break;
	}
	return TRUE;
}

BOOL NPC_ActionSetNow( int toindex, char * buf)
{
	int eveno;
	int talkNo=1, iflg=0;
	char buf1[256];

	while( getStringFromIndexWithDelim( buf,",",talkNo,buf1,sizeof( buf1)) != FALSE)	{ 
		talkNo++;
		eveno = atoi( buf);
		NPC_NowEventSetFlg( toindex, eveno);
		if( ++iflg > 20 ) break;
	}
	return TRUE;
}


BOOL NPC_ActionPassCheck( int meindex, int talker, char *buf)
{

	char buff2[256];
	char buff3[128];
	int i=1,j=1;
	int loop=0;
	while( getStringFromIndexWithDelim(buf,",",i,buff2,sizeof(buff2)) !=FALSE )	{	
		i++;
		if(strstr(buff2,"&")!=NULL){
			j=1;
			loop=0;
			while( getStringFromIndexWithDelim(buff2,"&",j,buff3,sizeof(buff3)) !=FALSE )	{
				j++;
				if(NPC_ActionBigSmallCheck( meindex, talker, buff3)==FALSE)	{
					loop=1;
					break;
				}
			}
			if(loop==0) {
				CHAR_setWorkInt( talker, CHAR_WORKWARPCHECK, TRUE );
				return TRUE;
			}
		}else{
			if( NPC_ActionBigSmallCheck( meindex, talker, buff2) == TRUE){
				CHAR_setWorkInt( talker, CHAR_WORKWARPCHECK, TRUE );
				return TRUE;

			}
		}
	}
	CHAR_setWorkInt( talker, CHAR_WORKWARPCHECK, FALSE );
	return FALSE;

}

BOOL NPC_ActionBigSmallCheck(int meindex,int talker,char* buf)
{

	char buff2[128], buff3[128];
	int kosuu,temp=-1,flg=0;

#ifdef _NEW_WARPMAN
	char buff1[128];
	if(strstr( buf, "-") != NULL)	{
		getStringFromIndexWithDelim( buf, "-", 2, buff3, sizeof(buff3));
		temp = atoi( buff3);
		getStringFromIndexWithDelim( buf, "-", 1, buff1, sizeof(buff1));
		strcpy( buf, buff1);
	}                         
#endif

	if( strstr( buf, "<") != NULL ){
		getStringFromIndexWithDelim( buf, "<", 2, buff3, sizeof(buff2));
		kosuu = atoi( buff3);
		getStringFromIndexWithDelim( buf, "<", 1, buff2, sizeof(buff2));

		if( NPC_ActionFreeIfCheck( meindex, talker, buff2, buff3, kosuu, 1, temp)==TRUE){
			return TRUE;
		}
	}else if(strstr( buf, ">") != NULL){
		getStringFromIndexWithDelim( buf, ">", 2, buff3, sizeof(buff2));
		kosuu = atoi(buff3);
		getStringFromIndexWithDelim( buf, ">" ,1, buff2, sizeof(buff2));

		if( NPC_ActionFreeIfCheck( meindex, talker, buff2, buff3, kosuu, 2, temp) == TRUE ){
			return TRUE;
		}

	}else if( strstr( buf, "!" ) != NULL){
		getStringFromIndexWithDelim( buf, "!=", 2, buff3, sizeof(buff2));
		kosuu = atoi( buff3);
		getStringFromIndexWithDelim( buf, "!=", 1, buff2, sizeof(buff2));
		if( NPC_ActionFreeIfCheck( meindex, talker, buff2, buff3, kosuu, 0, temp) == TRUE ){
			return FALSE;
		}else{
			return TRUE;
		}
	}else if( strstr( buf, "=") != NULL){
		getStringFromIndexWithDelim( buf, "=", 2, buff3, sizeof(buff2));
		kosuu = atoi( buff3);
		getStringFromIndexWithDelim( buf, "=", 1, buff2, sizeof(buff2));
		flg = 0;
		if( strstr( buf, "*") != NULL){
			if( NPC_ActionWarpManReduce( meindex, talker, buf)==TRUE){
				return TRUE;
			}
		}
#ifdef _NPC_ActionFreeCmp
		else if( strstr( buf, "^") != NULL){
		    if( NPC_ActionWarpManReduce2( meindex, talker, buf)==TRUE){
				return TRUE;
			}
		}
#endif
		else if( NPC_ActionFreeIfCheck( meindex, talker, buff2, buff3, kosuu, flg, temp) == TRUE )	{
			return TRUE;
		}
	}
	return FALSE;
}

#ifdef _NPC_ActionFreeCmp 
//其实功能和原本的NPC_ActionWarpManReduce差不多,但为了不影响以前的设定,所以不去修改原本的
BOOL NPC_ActionWarpManReduce2(int meindex,int talker,char *buf)
{
	char buf2[512];
	char buf3[256];
	int i, itemindex, itemno, kosuu;
	int id=0;
	int nums = 0;

	getStringFromIndexWithDelim(buf,"=",2,buf2,sizeof(buf2));
	getStringFromIndexWithDelim(buf2,"^",1,buf3,sizeof(buf3));
	itemno = atoi(buf3);
	getStringFromIndexWithDelim(buf2,"^",2,buf3,sizeof(buf3));
	kosuu = atoi(buf3);

	for( i=CHAR_STARTITEMARRAY ; i < CheckCharMaxItem(talker);i++ ){
		itemindex=CHAR_getItemIndex( talker , i );
		if( ITEM_CHECKINDEX(itemindex) ){
			id=ITEM_getInt(itemindex ,ITEM_ID );
			if( itemno == id ){
				nums += ITEM_getInt( itemindex, ITEM_USEPILENUMS);//数量
			}
		}
	}

	if( nums == kosuu ) return TRUE;

	return FALSE;
}
#endif

#ifdef _NEW_MANOR_LAW
extern struct FM_POINTLIST  fmpointlist;
#endif

BOOL NPC_ActionFreeIfCheck(int meindex,int talker, char* buf, char *opt, int kosuu,int flg, int temp)
{
#ifdef _MISSION_TRAIN
	if(strcmp(buf,"MT")==0){
		if(NPC_ActionMissionTrainCheck(meindex,talker,kosuu,flg)==TRUE)
				return TRUE;
	}
#endif	
#ifdef _BOUND_TIME
	if(strcmp(buf,"BOUNDTIME")==0){
		if(NPC_ActionBoundTimeCheck(meindex,talker,kosuu,flg)==TRUE)
				return TRUE;
	}
#endif
#ifdef _NPC_CHECKTIME
	if(strcmp(buf,"YEAR")==0 || strcmp(buf,"MON")==0 || strcmp(buf,"DAY")==0 
		|| strcmp(buf,"HOUR")==0 || strcmp(buf,"MIN")==0 || strcmp(buf,"SEC")==0 ){
		if(NPC_ActionTimeCheck(meindex,talker, buf, kosuu, flg)==TRUE)
				return TRUE;
	}
#endif
#ifdef _PLAYER_DIY_MAP
	if(strcmp(buf,"DIYMAP")==0){
		if(NPC_ActionDiyMapCheck(meindex,talker,kosuu,flg)==TRUE)
				return TRUE;
	}
#endif
#ifdef _FAME_CHECK_NPC
	if(strcmp(buf,"FAME")==0){
		if(NPC_ActionFameCheck(meindex,talker,kosuu,flg)==TRUE)
				return TRUE;
	}
#endif
#ifdef _VIP_SHOP
	if(strcmp(buf,"VIPPOINT")==0){
		if(NPC_ActionVipPointCheck(meindex,talker,kosuu,flg)==TRUE)
				return TRUE;
	}
#endif
#ifdef _VIP_RIDE
	if(strcmp(buf,"VIP")==0){
		if(NPC_ActionVipCheck(meindex,talker,kosuu,flg)==TRUE)
				return TRUE;
	}
#endif
	if(strcmp(buf,"LV")==0){
		if(NPC_ActionLevelCheck(meindex,talker,kosuu,flg)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"GOLD")==0){
		if(NPC_ActionGoldCheck(meindex,talker,kosuu,flg)==TRUE)
				return TRUE;
	}
#ifdef _GLORY_POINT
	if(strcmp(buf,"GLORY")==0){
		if(NPC_ActionGloryCheck(meindex,talker,kosuu,flg)==TRUE)
				return TRUE;
	}
#endif
	if( strcmp(buf,"TRANS") == 0 )	{
		if( NPC_ActionCheckTrans( meindex, talker, kosuu, flg) == TRUE )
			return TRUE;
	}

	if( !strcmp( buf, "GTIME") ){
		if( NPC_CheckTimeDefine( opt) == TRUE )
			return TRUE;
	}

	if(strcmp( buf, "TIME") == 0) {
		if(NPC_TiemCheck( meindex, talker, kosuu, flg) == TRUE)
			return TRUE;
	}

	if( strcmp( buf, "PET") == 0 )	{
		if( temp > 0 )	{
			if( ActionCheckMyPet( meindex, talker, kosuu, flg, temp) == TRUE )
				return TRUE;
		}else
			return FALSE;
	}
	if(strcmp(buf,"ITEM")==0){
		if(NPC_ActionItemCheck(meindex,talker,kosuu, flg)==TRUE)
				return TRUE;
	}

	if(strcmp(buf,"EQUIT")==0){
		if(NPC_ActioneEquitCheck(meindex,talker,kosuu, flg)==TRUE)
				return TRUE;
	}

	if( !strcmp(buf,"ENDEV") || !strcmp(buf,"EVEND") ){
		if(NPC_EventCheckFlg( talker, kosuu)==TRUE)
				return TRUE;
	}
	if( !strcmp(buf,"NOWEV") || !strcmp(buf,"EVNOW") ){
		if(NPC_NowEventCheckFlg( talker, kosuu)==TRUE)
				return TRUE;
	}

	if(strcmp(buf,"PARTY")==0){
		if(NPC_ActionPartyCheck( talker, kosuu, flg)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"reITEM")==0){
		if( NPC_ActionReItemCheck( talker, kosuu, flg)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"rePET")==0){
		if( NPC_ActionRePetCheck( talker, kosuu, flg)==TRUE)
				return TRUE;
	}

#ifdef _NPCCHANGE_PLAYERIMG
	if(strcmp( buf, "BBI")==0)	{
		if( NPC_CheckPlayerBBI( meindex, talker, kosuu, flg)== TRUE)	{
			return TRUE;
		}
	}
#endif
	if( strcmp( buf, "FM") == 0 ){
		if( CHAR_getInt( talker, CHAR_FMLEADERFLAG ) > 0 && 
			CHAR_getInt( talker, CHAR_FMLEADERFLAG ) != FMMEMBER_APPLY)
		if( CHAR_getWorkInt( talker, CHAR_WORKFMFLOOR) == kosuu ){
			return TRUE;
		}
	}
	if(strcmp(buf,"DR")==0){
		if(NPC_ActionDuelRankCheck(meindex,talker,kosuu,flg)==TRUE){
				return TRUE;
		}
	}
	if(strcmp(buf,"DP")==0){
		if(NPC_ActionDuelPointCheck(meindex,talker,kosuu,flg)==TRUE){
				return TRUE;
		}
	}
#ifdef _PROFESSION_SKILL			// WON ADD 人物职业技能
	if( strcmp( buf, "CLASS") == 0 ){
		if(NPC_ActionClassCheck(meindex,talker,kosuu,flg)==TRUE)
				return TRUE;
	}
	if( strcmp( buf, "Skill") == 0 ){
		if( NPC_ActionSkillCheck( meindex, talker, kosuu, flg)==TRUE)
				return TRUE;
	}
	if( strcmp( buf, "SkNum") == 0 ){
		if( NPC_ActionSkillNumCheck( meindex, talker, kosuu, flg)==TRUE)
				return TRUE;
	}
	if( strcmp( buf, "SKCP") == 0 ){
		if( NPC_ActionSkillPointAndSkillCheck( meindex, talker, kosuu, flg)==TRUE)
				return TRUE;
	}
#endif
	

#ifdef _TREASURE_BOX
	if( strcmp( buf, "BOX") == 0 ){
		if( NPC_ActionTreasureTypeCheck( meindex, kosuu,
			//NPC_TIME_EVENTMODE = CHAR_NPCWORKINT6,// 1
			CHAR_getWorkInt( meindex, CHAR_NPCWORKINT6) ,flg)==TRUE)
				return TRUE;
	}
#endif

#ifdef _NPC_ADDWARPMAN1
    if( strcmp( buf, "PARTYCOUNT") == 0){
	    if(NPC_ActionPartyCountCheck( talker, kosuu, flg)==TRUE)
			return TRUE;
	}
	if( strcmp( buf, "MANCOUNT" ) == 0 ){
        if(NPC_ActionManCountCheck( talker, kosuu, flg ) == TRUE )
			return TRUE;
	}
	if( strcmp( buf, "WOMANCOUNT" ) == 0 ){
        if(NPC_ActionWomanCountCheck( talker, kosuu, flg ) == TRUE )
			return TRUE;
	}
#endif

#ifdef _ANGEL_SUMMON
	if(strcmp(buf,"ANGEL_NOW")==0){
		if( NPC_ActionMissionAngelCheck(meindex,talker,kosuu, MISSION_DOING, 0)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"HERO_NOW")==0){
		if( NPC_ActionMissionHeroCheck(meindex,talker,kosuu, MISSION_DOING, 0)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"ANGEL_OVER")==0){
		if( NPC_ActionMissionAngelCheck(meindex,talker,kosuu, MISSION_HERO_COMPLETE, 0)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"HERO_OVER")==0){
		if( NPC_ActionMissionHeroCheck(meindex,talker,kosuu, MISSION_HERO_COMPLETE, 0)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"ANGEL_OUT")==0){
		if( NPC_ActionMissionAngelCheck(meindex,talker,kosuu, MISSION_TIMEOVER, 0)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"HERO_OUT")==0){
		if( NPC_ActionMissionHeroCheck(meindex,talker,kosuu, MISSION_TIMEOVER, 0)==TRUE)
				return TRUE;
	}

	if(strcmp(buf,"ANGEL_I_NOW")==0){
		if( NPC_ActionMissionAngelCheck(meindex,talker,kosuu, MISSION_DOING, TRUE)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"HERO_I_NOW")==0){
		if( NPC_ActionMissionHeroCheck(meindex,talker,kosuu, MISSION_DOING, TRUE)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"ANGEL_I_OVER")==0){
		if( NPC_ActionMissionAngelCheck(meindex,talker,kosuu, MISSION_HERO_COMPLETE, TRUE)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"HERO_I_OVER")==0){
		if( NPC_ActionMissionHeroCheck(meindex,talker,kosuu, MISSION_HERO_COMPLETE, TRUE)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"ANGEL_I_OUT")==0){
		if( NPC_ActionMissionAngelCheck(meindex,talker,kosuu, MISSION_TIMEOVER, TRUE)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"HERO_I_OUT")==0){
		if( NPC_ActionMissionHeroCheck(meindex,talker,kosuu, MISSION_TIMEOVER, TRUE)==TRUE)
				return TRUE;
	}
	if(strcmp( buf, "HEROCNT") == 0) {
		//if(NPC_EventLevelCheck( meindex, talker, kosuu, flg) == TRUE) {
		if(NPC_HeroCompleteCountCheck( meindex, talker, kosuu, flg) == TRUE)
			return TRUE;
	}

#endif
#ifdef _NEW_MANOR_LAW
	if(strcmp(buf,"MANOR") == 0){
		// 大於-1表示要检查是否为庄园家族成员才能warp
		if(kosuu > -1){
			int iFmIndex = -1,iHadFmindex = -1;
			char token[256];
			
			iFmIndex = CHAR_getInt(talker,CHAR_FMINDEX);
			getStringFromIndexWithDelim(fmpointlist.pointlistarray[kosuu],"|",5,token,sizeof(token));
			iHadFmindex = atoi(token);
			if(iFmIndex > -1){
				// 是庄园拥有家族的成员
				if(iFmIndex == iHadFmindex){
					// 必须有家族且是正式族员
					if(CHAR_getInt(talker,CHAR_FMLEADERFLAG) != FMMEMBER_NONE &&
						 CHAR_getInt(talker,CHAR_FMLEADERFLAG) != FMMEMBER_APPLY) return TRUE;
				}
			}
		}
	}
#endif

#ifdef _NPC_CHECK_SCRIPT
{
	int i;
	for(i=0;i<CHAR_DATAINTNUM;i++){
		if(strcmp(buf, CHAR_setintdata[i]) == 0){
			if(NPC_ActionBigSmallLastCheck( kosuu, CHAR_getInt(talker, i), flg) == TRUE){
				return TRUE;
			}
		}
	}
}
#endif

	return FALSE;
}

BOOL NPC_ActioneEquitCheck(int meindex,int talker,int itemNo, int flg)
{
	int i;
	int itemindex=-1;
	int id;
	for( i=0;i<CHAR_STARTITEMARRAY;i++ ){
		itemindex = CHAR_getItemIndex( talker , i );
		if( ITEM_CHECKINDEX( itemindex) )	{
			id=ITEM_getInt(itemindex ,ITEM_ID );
			if( NPC_ActionBigSmallLastCheck(itemNo,id,flg) == TRUE )
				return TRUE;
		}
	}

	return FALSE;
}

BOOL NPC_ActionItemCheck(int meindex,int talker,int itemNo, int flg)
{
	int i;
	int itemindex=-1;
	int id;
	for( i=CHAR_STARTITEMARRAY;i<CheckCharMaxItem(talker);i++ ){
		itemindex = CHAR_getItemIndex( talker , i );
		if( ITEM_CHECKINDEX( itemindex) )	{
			id=ITEM_getInt(itemindex ,ITEM_ID );
			if( NPC_ActionBigSmallLastCheck(itemNo,id,flg) == TRUE )
				return TRUE;
		}
	}

	return FALSE;
}

BOOL NPC_ActionBigSmallLastCheck(int point1,int mypoint,int flg)
{
	if(flg==0){
		if(point1==mypoint)
			return TRUE;
	}else if(flg==1){
		if(mypoint < point1)
			return TRUE;
	}else if(flg==2){
		if(mypoint > point1)
			return TRUE;
	}

	return FALSE;
}

BOOL ActionCheckMyPet( int meindex, int talker, int petLv, int flg, int petid)
{
	int petsel,petindex=-1;	
	int lv=-1;
	for(petsel=0 ; petsel < CHAR_MAXPETHAVE ; petsel++)     {
		petindex = CHAR_getCharPet( talker, petsel);
		if( petindex == -1  )
			continue;
	        if(CHAR_getInt( petindex, CHAR_PETID) !=  petid )
			continue;
		break;
	}
	if( petsel  == CHAR_MAXPETHAVE )	{
		return FALSE;
	}else	{	//找到条件宠
		lv = CHAR_getInt( petindex, CHAR_LV);
		if( NPC_ActionBigSmallLastCheck( petLv, lv, flg) == TRUE)	{
			return TRUE;
		}else	{
			return FALSE;
		}
	}
	return FALSE;                                             
}


BOOL NPC_ActionCheckTrans(int meindex,int talker,int trans,int flg)
{
	int myTrans;
	myTrans=CHAR_getInt(talker, CHAR_TRANSMIGRATION);
	if( NPC_ActionBigSmallLastCheck( trans, myTrans, flg ) == TRUE )	{
             return TRUE;
        }                   
	return FALSE;
}


#ifdef _NPC_CHECKTIME
BOOL NPC_ActionTimeCheck(int meindex, int talker, char *buf, int date,int flg)
{
	struct tm *p;
	time_t timep;
	time(&timep);
	p=localtime(&timep); /*取得当地时间*/
	int mydate=0;

	if(strcmp(buf,"YEAR")==0 ){
		mydate=p->tm_year + 1900;
	}else if(strcmp(buf,"MON")==0){
		mydate=p->tm_mon + 1;
	}else if(strcmp(buf,"DAY")==0){
		mydate=p->tm_mday;
	}else if(strcmp(buf,"HOUR")==0){
		mydate=p->tm_hour;
	}else if(strcmp(buf,"MIN")==0){
		mydate=p->tm_min;
	}else if(strcmp(buf,"SEC")==0 ){
		mydate=p->tm_sec;
	}

	if(NPC_ActionBigSmallLastCheck( date, mydate, flg) == TRUE){
		return TRUE;
	}
	return FALSE;
}
#endif

#ifdef _MISSION_TRAIN
BOOL NPC_ActionMissionTrainCheck(int meindex,int talker,int mt,int flg)
{
	int mymt;
	mymt=CHAR_getInt(talker, CHAR_MISSIONTRAIN_NUM);
	if(NPC_ActionBigSmallLastCheck( mt, mymt, flg) == TRUE){
		return TRUE;
	}
	return FALSE;
}
#endif

#ifdef _FAME_CHECK_NPC
BOOL NPC_ActionFameCheck(int meindex,int talker,int fame,int flg)
{
	int myfame;
	myfame=CHAR_getInt(talker, CHAR_FAME);
	if(NPC_ActionBigSmallLastCheck( fame, myfame, flg) == TRUE){
		return TRUE;
	}
	return FALSE;
}
#endif

#ifdef _VIP_SHOP
BOOL NPC_ActionVipPointCheck(int meindex,int talker,int point,int flg)
{
	int mypoint;
	mypoint=CHAR_getInt(talker,CHAR_AMPOINT);
	if(NPC_ActionBigSmallLastCheck( point, mypoint, flg) == TRUE){
		return TRUE;
	}
	return FALSE;
}
#endif
#ifdef _VIP_RIDE
BOOL NPC_ActionVipCheck(int meindex,int talker,int vip,int flg)
{
	if(NPC_ActionBigSmallLastCheck( CHAR_getInt(talker,CHAR_VIPRIDE), vip, flg) == TRUE){
		return TRUE;
	}
	return FALSE;
}
#endif

BOOL NPC_ActionLevelCheck(int meindex,int talker,int level,int flg)
{
	int mylevel;
	mylevel=CHAR_getInt(talker,CHAR_LV);
	if(NPC_ActionBigSmallLastCheck( level, mylevel, flg) == TRUE){
		return TRUE;
	}
	return FALSE;
}

#ifdef _PROFESSION_SKILL			// WON ADD 人物职业技能
BOOL NPC_ActionClassCheck(int meindex,int talker,int p_class,int flg)
{
	int myclass;
	myclass= CHAR_getInt( talker, PROFESSION_CLASS );

	if(NPC_ActionBigSmallLastCheck( p_class, myclass, flg) == TRUE){
		return TRUE;
	}
	return FALSE;
}

BOOL NPC_ActionSkillPointAndSkillCheck( int meindex,int talker,int sknum,int flg)
{
	int myskillpoint, myskillnum=0, myskilllevel;
	int mysknum=0, j;
	myskillpoint= CHAR_getInt( talker, PROFESSION_SKILL_POINT );
	myskilllevel= CHAR_getInt( talker, PROFESSION_LEVEL );

	for( j=0; j<PROFESSION_MAX_LEVEL; j++ ){
		if( CHAR_getCharSkill( talker, j) <= 0 ) continue;
		myskillnum++;
	}
	mysknum = myskilllevel - (myskillpoint + myskillnum);
	//andy_log
	print("mysknum%d sknum:%d \n", mysknum, sknum);
	return NPC_ActionBigSmallLastCheck( sknum, mysknum, flg);
}

BOOL NPC_ActionSkillNumCheck( int meindex, int talker, int sknum, int flg)
{
	int j, mynum=0;

	for( j=0; j<PROFESSION_MAX_LEVEL; j++ ){
		if( CHAR_getCharSkill( talker, j) <= 0 ) continue;
		mynum++;
	}
	return NPC_ActionBigSmallLastCheck( sknum, mynum, flg);
}

BOOL NPC_ActionSkillCheck( int meindex,int talker,int skillId,int flg)
{
	int sID, j;
		//andy_log
		print("NPC_ActionSkillCheck( skillId:%d ) \n", skillId);
	for( j=0; j<PROFESSION_MAX_LEVEL; j++ ){
		if( (sID = CHAR_getCharSkill( talker, j)) == 0 ) continue;

		//andy_log
		print("sID:%d skillId:%d flg:%d\n", sID, skillId, flg);
		if( NPC_ActionBigSmallLastCheck( skillId, sID, flg) == TRUE){
			return TRUE;
		}
	}
	return FALSE;
}

BOOL NPC_ActionAddProfessionSkillPoint( int toindex, char *buf )
{
	char token[256];
	int skillpoint=0, addsp=0;
	addsp = atoi( buf);
	skillpoint = CHAR_getInt( toindex , PROFESSION_SKILL_POINT);
	CHAR_setInt( toindex , PROFESSION_SKILL_POINT, skillpoint + addsp );
	CHAR_sendCToArroundCharacter( CHAR_getWorkInt( toindex , CHAR_WORKOBJINDEX ));
	sprintf( token, "技能学习点数增加%d点。", addsp);
	CHAR_talkToCli( toindex, -1, token, CHAR_COLORYELLOW);
	return TRUE;
}

BOOL NPC_ActionCleanProfession( int toindex, char *buf)
{
	int k=1,i;
	char buf1[256], buf2[256];
	CHAR_HaveSkill *pSkil;

	k=1;
	while( getStringFromIndexWithDelim( buf , "," , k, buf1, sizeof( buf1)) !=FALSE ){
		k++;
		if( getStringFromIndexWithDelim( buf1 , "=" , 1, buf2, sizeof( buf2)) ==FALSE ) continue;

		if( !strcmp( buf2, "SKILL") ){
			int psID=-1;
			if( getStringFromIndexWithDelim( buf1 , "=" , 2, buf2, sizeof( buf2)) ==FALSE ) continue;
			if( strstr( buf2, "ALL") != NULL ){
			}else{
				psID = atoi( buf2);
			}
			for( i=0; i<PROFESSION_MAX_LEVEL; i++ ){
				if( psID != -1 && psID != CHAR_getIntPSkill( toindex, i, SKILL_IDENTITY) ) continue;
					pSkil = CHAR_getCharHaveSkill( toindex, i );
					SKILL_makeSkillData( &pSkil->skill, 0, 0 );	
					pSkil->use = 0;
			}
		}else if( !strcmp( buf2, "CLASS") ){
			if( getStringFromIndexWithDelim( buf1 , "=" , 2, buf2, sizeof( buf2)) ==FALSE ) continue;
			if( strstr( buf2, "ALL") != NULL ){
				CHAR_setInt( toindex , PROFESSION_CLASS, 0);
#ifdef _CHANNEL_MODIFY
				if(CHAR_getInt(toindex,PROFESSION_CLASS) > 0){
					int i,pclass = CHAR_getInt(toindex,PROFESSION_CLASS) - 1;
					for(i=0;i<getFdnum();i++){
						if(*(piOccChannelMember + (pclass * getFdnum()) + i) == toindex){
							*(piOccChannelMember + (pclass * getFdnum()) + i) = -1;
							break;
						}
					}
				}
#endif
				CHAR_setInt( toindex , PROFESSION_LEVEL, 0);
				CHAR_setInt( toindex , PROFESSION_SKILL_POINT, 0);
				CHAR_setInt( toindex , ATTACHPILE, 0);
			}else{
				print("尚未设定参数!! (%s %d)\n", __FILE__, __LINE__);
			}
		}
	}
	CHAR_sendStatusString( toindex , "S");
	CHAR_sendCToArroundCharacter( CHAR_getWorkInt( toindex , CHAR_WORKOBJINDEX ));
	return TRUE;	                                                                                                                                                                      
}
#endif


BOOL NPC_ActionPartyCheck( int toindex, int nums, int flg)
{
	int partys=0;
	int leaderindex=-1;
	if( CHAR_getWorkInt( toindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_NONE ){
		partys = 0;
		leaderindex = toindex;
	}else if( CHAR_getWorkInt( toindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_CLIENT ){
		leaderindex = CHAR_getWorkInt( toindex, CHAR_WORKPARTYINDEX1);
	}

	if( CHAR_getWorkInt( leaderindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_LEADER ){
		int i;
		for( i=0; i<5; i++)	{
			int pindex = CHAR_getWorkInt( leaderindex, CHAR_WORKPARTYINDEX1+i);
			if( !CHAR_CHECKINDEX( pindex) ) continue;
				partys++;
		}
	}

	if(NPC_ActionBigSmallLastCheck( nums, partys, flg) == TRUE){
		return TRUE;
	}
	return FALSE;
}

BOOL NPC_ActionReItemCheck( int toindex, int nums, int flg)
{
	int remnants=0;
	remnants = CHAR_findSurplusItemBox( toindex);
	if(NPC_ActionBigSmallLastCheck( nums, remnants, flg) == TRUE){
		return TRUE;
	}
	return FALSE;
}
BOOL NPC_ActionRePetCheck( int toindex, int nums, int flg)
{
	int remnants=0;
	remnants = CHAR_findSurplusPetBox( toindex);
	if(NPC_ActionBigSmallLastCheck( nums, remnants, flg) == TRUE){
		return TRUE;
	}
	return FALSE;
}

BOOL NPC_ActionGoldCheck(int meindex,int talker,int Golds,int flg)
{
	int myGold;
	myGold=CHAR_getInt(talker,CHAR_GOLD);
	if(NPC_ActionBigSmallLastCheck( Golds, myGold, flg) == TRUE){
		return TRUE;
	}
	return FALSE;
}
#ifdef _GLORY_POINT
BOOL NPC_ActionGloryCheck(int meindex,int talker,int Golds,int flg)
{
	int myGold;
	myGold=sasql_getGloryPoint(talker);
//	print("\n%d-%d",Golds,myGold);
	if(NPC_ActionBigSmallLastCheck( Golds, myGold, flg) == TRUE){
		return TRUE;
	}
	return FALSE;
}

BOOL NPC_ActionAddGlory( int talker,char *buf)
{
	int Golds=0;
	int PGold=0;
	int total_gold = 0;
	if( buf == NULL ) return FALSE;
	Golds = atoi( buf);
	PGold = sasql_getGloryPoint( talker);

	total_gold = PGold + Golds;
	sasql_setGloryPoint(talker,total_gold);
	return TRUE;
}
#endif

BOOL NPC_ActionWarpManReduce(int meindex,int talker,char *buf)
{
	char buf2[512];
	char buf3[256];
	int i, itemindex, itemno, kosuu;
	int id=0;
#ifdef _ITEM_PILENUMS
	int nums = 0;
#else
	int cnt =0;
#endif
	getStringFromIndexWithDelim(buf,"=",2,buf2,sizeof(buf2));
	getStringFromIndexWithDelim(buf2,"*",1,buf3,sizeof(buf3));
	itemno = atoi(buf3);
	getStringFromIndexWithDelim(buf2,"*",2,buf3,sizeof(buf3));
	kosuu = atoi(buf3);

	for( i=CHAR_STARTITEMARRAY ; i < CheckCharMaxItem(talker);i++ ){
		itemindex=CHAR_getItemIndex( talker , i );
		if( ITEM_CHECKINDEX(itemindex) ){
			id=ITEM_getInt(itemindex ,ITEM_ID );
			if( itemno == id ){
#ifdef _ITEM_PILENUMS
				nums += ITEM_getInt( itemindex, ITEM_USEPILENUMS);//数量
#else
				cnt++;
				if(cnt==kosuu){
					return TRUE;
				}
#endif
			}
		}
	}

#ifdef _ITEM_PILENUMS
	if( nums >= kosuu ) return TRUE;
#endif

	return FALSE;

}

#ifdef _NPCCHANGE_PLAYERIMG	//andy
BOOL NPC_ActionChangePlayerBBI( int meindex, int charindex, char *Img)
{
	int cBBI;
	char buf1[256];
	memset( buf1, 0, sizeof( buf1));
	if( !CHAR_CHECKINDEX( charindex) )
		return FALSE;
	if( !CHAR_CHECKINDEX( meindex) )
		return FALSE;
#ifdef _FIXBUG_ATTACKBOW
	{
		int armtype;
		int itemindex = CHAR_getItemIndex( charindex, CHAR_ARM);
		if( ITEM_CHECKINDEX(itemindex) ){
			armtype = ITEM_getInt( itemindex, ITEM_TYPE );
			if( armtype == ITEM_BOW || armtype == ITEM_BOUNDTHROW ||
				armtype == ITEM_BREAKTHROW || armtype == ITEM_BOOMERANG)	{
				CHAR_talkToCli( charindex, -1, "使用远距离武器无法变身！", CHAR_COLORYELLOW );
				return FALSE;
			}
		}

	}
#endif
#ifdef _PETSKILL_BECOMEPIG
    if( CHAR_getInt( charindex, CHAR_BECOMEPIG) > -1 ){//变成乌力了
		CHAR_talkToCli( charindex, -1, "无法变身，乌力化中不能变身！", CHAR_COLORYELLOW );
	    return FALSE;
	}
#endif
	if( CHAR_getInt( charindex, CHAR_RIDEPET) != -1 )	{//骑宠不换图
		CHAR_setInt( charindex, CHAR_RIDEPET, -1 );
		BATTLE_changeRideImage( charindex );
		CHAR_talkToCli( charindex, -1, "取消骑宠状态。", CHAR_COLORYELLOW);
		
		//change -----> 修改bug(落马後,宠物选项的状态依然为"骑乘",这里修正过来
		CHAR_complianceParameter( charindex );
		CHAR_send_P_StatusString( charindex, CHAR_P_STRING_RIDEPET);
		
		//return FALSE;
	}
#ifdef _ITEM_METAMO
	CHAR_setWorkInt( charindex, CHAR_WORKITEMMETAMO, 0);
#endif

	if( Img == NULL || !strcmp( Img, "\0"))
		return FALSE;
//=======================
	CHAR_setWorkInt( charindex, CHAR_WORKNPCMETAMO, meindex);
	if( strstr( Img, "NPCBBI") != NULL )	{
		cBBI = CHAR_getInt( meindex, CHAR_BASEBASEIMAGENUMBER);
		sprintf( buf1, "你化身成了%s的样子。", CHAR_getChar( meindex, CHAR_NAME));
	}else if( strstr( Img, "MYBBI") != NULL )	{
		cBBI = CHAR_getInt( charindex, CHAR_BASEBASEIMAGENUMBER);
		CHAR_setWorkInt( charindex, CHAR_WORKNPCMETAMO, 0);
		sprintf( buf1, "变回了原来的样子。");
	}else if( strstr( Img, ",") != NULL )	{
		int imgindex=1, maxImg=0;
		while( getStringFromIndexWithDelim( Img, ",", imgindex, buf1, sizeof( buf1)) != FALSE )	{
			imgindex++;
			maxImg++;
		}
		if( (imgindex = (rand()%maxImg)) > maxImg) imgindex = 1;
		if( imgindex < 1 ) imgindex = 1;

		if( getStringFromIndexWithDelim( Img, ",", imgindex, buf1, sizeof( buf1)) == FALSE )
			return FALSE;
		cBBI = atoi( buf1);
		sprintf( buf1, "你样子改变了。");
	}else	{
		cBBI = atoi( Img);
		sprintf( buf1, "你样子改变了。");
	}
//=======================
	if( cBBI < 0 )	{
		return FALSE;
	}
	CHAR_talkToCli( charindex, -1, buf1, CHAR_COLORYELLOW);
	//CHAR_BASEBASEIMAGENUMBER	CHAR_BASEIMAGENUMBER
	CHAR_setInt( charindex, CHAR_BASEIMAGENUMBER, cBBI);
	CHAR_complianceParameter( charindex );
	CHAR_sendCToArroundCharacter( CHAR_getWorkInt( charindex , CHAR_WORKOBJINDEX ));
	CHAR_send_P_StatusString( charindex , CHAR_P_STRING_BASEBASEIMAGENUMBER);
	//CHAR_K_STRING_BASEIMAGENUMBER
	//CHAR_P_STRING_BASEBASEIMAGENUMBER
	return TRUE;
}
#endif

#ifdef _NPCCHANGE_PLAYERIMG
BOOL NPC_CheckPlayerBBI( int meindex, int charindex, int BBI, int flg)
{
	int MyBBI;
	if( !CHAR_CHECKINDEX( charindex))
		return FALSE;
	if( BBI < 0 )
		return FALSE;
	MyBBI = CHAR_getInt( charindex, CHAR_BASEIMAGENUMBER);
	return NPC_ActionBigSmallLastCheck( BBI, MyBBI,flg);
}
#endif

BOOL showString( int meindex, char *showstr,int flg)
{	//广播
	char buf1[256];
	char argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	int toindex = -1;
	int floor;
	int i;
	if( NPC_Util_GetArgStr( meindex, argstr, sizeof( argstr)) == NULL )	{
		print("\n not found argstr ");
		return FALSE;
	}
	if( NPC_Util_GetStrFromStrWithDelim( argstr, showstr,buf1, sizeof(buf1)) == NULL ) {
		strcpy( buf1, showstr);
	}
	toindex = -1;
	floor = CHAR_getInt( meindex , CHAR_FLOOR);
	while( toindex < 1000)	{
		toindex ++;
		if( !CHAR_CHECKINDEX( toindex) )
			continue;
		if( CHAR_getInt( toindex , CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER )
			continue;
		if( CHAR_getInt( toindex , CHAR_FLOOR) != floor )
			continue;
		CHAR_talkToCli( toindex, meindex, buf1,  CHAR_COLORYELLOW);
		if( flg == 1 )	{
			CHAR_setWorkInt( toindex, CHAR_WORKSTAKEFLAG, FALSE );
			for( i=0; i<5; i++)	{
				CHAR_setWorkInt( toindex, i+CHAR_WORKSTAKETYPE1, 0);
			}
		}
	}
	return TRUE;
}

BOOL NPC_getTimeXYPoint( int meindex, int Mode)
{
	char npcarg[NPC_UTIL_GETARGSTR_BUFSIZE];
	char buf[256], buf1[256], buf2[256];
	int fl=-1, x, y;
	int GraNo = -1;
	int points = 1;	//条件成立为1
	if( Mode <= 0 )	{
		points = 2;
	}
	memset( npcarg, 0, sizeof( npcarg));
	if( !CHAR_CHECKINDEX( meindex) ) return FALSE;
	if( NPC_Util_GetArgStr( meindex, npcarg, sizeof(npcarg)) == NULL ){
		print("WarpMan:GetArgStrErr");
		return FALSE;
	}
	memset( buf, 0, sizeof( buf));
	if( NPC_Util_GetStrFromStrWithDelim( npcarg, "TimeXYPoint", buf, sizeof( buf)) != NULL ){
		if( getStringFromIndexWithDelim(buf , "&" , points, buf1, sizeof( buf1)) != FALSE ){
			memset( buf2, 0, sizeof( buf2));
			if( getStringFromIndexWithDelim(buf1 , "," , 1, buf2, sizeof( buf2)) == FALSE ){
				return FALSE;
			}
			fl = atoi( buf2);
			if( getStringFromIndexWithDelim(buf1 , "," , 2, buf2, sizeof( buf2)) == FALSE ){
				return FALSE;
			}
			x = atoi( buf2);
			if( getStringFromIndexWithDelim(buf1 , "," , 3, buf2, sizeof( buf2)) == FALSE ){
				return FALSE;
			}
			y = atoi( buf2);
			if( MAP_IsValidCoordinate( fl, x, y ) == FALSE ){
				char filename[256];
				NPC_Util_CheckAssignArgFile( meindex, filename);
				return FALSE;
			}
			if( CHAR_getInt( meindex, CHAR_FLOOR) != fl ||
				CHAR_getInt( meindex, CHAR_X) != x ||
				CHAR_getInt( meindex, CHAR_Y) != y ){
				CHAR_warpToSpecificPoint( meindex, fl , x , y);
			}
		}
	}

	if( NPC_Util_GetStrFromStrWithDelim( npcarg, "GraNo", buf, sizeof( buf)) != NULL ){
		if( getStringFromIndexWithDelim(buf , "&" , points, buf1, sizeof( buf1)) != FALSE ){
			GraNo = atoi( buf1);
			CHAR_setInt( meindex, CHAR_BASEIMAGENUMBER, GraNo);
		}
	}
	return TRUE;
}

BOOL NPC_getTimeData( int meindex, char *npcarg, int *born, int *dead, int loopspeed)
{
	int i;
	char buf[256];
	if( NPC_Util_GetStrFromStrWithDelim( npcarg, "NEWTIME", buf, sizeof( buf)) != NULL ){
		for(i=0 ; i<9 ; i++){
			if( strstr( buf, TimeTble[i].arg ) != NULL){
				*born = TimeTble[i].born;
				*dead = TimeTble[i].dead;
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL NPC_CheckTimeDefine( char *timetype)
{
	LSTIME nowlstime;
	int born=-1, dead=-1, finds=0, i;

	RealTimeToLSTime( NowTime.tv_sec, &nowlstime);

	for(i=0 ; i<9 ; i++){
		if( strstr( timetype, TimeTble[i].arg ) != NULL){
			born = TimeTble[i].born;
			dead = TimeTble[i].dead;
			finds = 1;
			break;
		}
	}
	if( finds == 0 ) return FALSE;

	if( born < dead ){
		if( (born < nowlstime.hour) && (dead > nowlstime.hour) ){
			return TRUE;
		}
	}else{
		if( (born < nowlstime.hour && 1024 > nowlstime.hour) ||
			( 0 < nowlstime.hour && dead > nowlstime.hour)){
			return TRUE;
		}
	}

	return FALSE;
}

void NPC_TimeDefineDo( int meindex, int born, int dead, int typeMode)
{
	LSTIME nowlstime;
	RealTimeToLSTime( NowTime.tv_sec, &nowlstime);
	if(born < dead){
		if( (born < nowlstime.hour) && (dead > nowlstime.hour) ){
			CHAR_setWorkInt(meindex,typeMode,0);		
		}else{
			CHAR_setWorkInt(meindex,typeMode,1);
		}
	}else{
		if( (born < nowlstime.hour && 1024 > nowlstime.hour) || ( 0 < nowlstime.hour && dead > nowlstime.hour)){
			CHAR_setWorkInt(meindex,typeMode,0);
		}else{
			CHAR_setWorkInt(meindex,typeMode,1);
		}
	}
	if( NPC_getTimeXYPoint( meindex, CHAR_getWorkInt( meindex, typeMode)) == FALSE )
		return;
}


void GetRand_WarpPoint( char *buf, int *fl, int *x, int *y )	
{
	char buff1[256],buff2[256];
	int i=0,point=1;
	struct {
		int m_fl;
		int m_x;
		int m_y;
	} RandPoint[]={
		{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
		{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
		{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
		{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
	};
		while( getStringFromIndexWithDelim( buf,";",point,buff1,sizeof( buff1)) != FALSE) {
			point++;
			getStringFromIndexWithDelim(buff1,",",1,buff2,sizeof(buff2));
			RandPoint[i].m_fl = atoi(buff2);
			getStringFromIndexWithDelim(buff1,",",2,buff2,sizeof(buff2));
			RandPoint[i].m_x = atoi(buff2);
			getStringFromIndexWithDelim(buff1,",",3,buff2,sizeof(buff2));
			RandPoint[i].m_y = atoi(buff2);
			i++;
			if( i >= arraysizeof( RandPoint) )
				break;
		}
		
		point = RAND( 0, i);
		if( RandPoint[point].m_fl <= 0 || point > arraysizeof( RandPoint) -1)	{
			point = 0;
		}
		*fl = RandPoint[point].m_fl;
		*x = RandPoint[point].m_x;
		*y = RandPoint[point].m_y;

	return;
}

#ifdef _EMENY_CHANCEMAN
int NPC_EmenyChanceCheck( int meindex, int charaindex, char *arg)
{
	char buf1[256];
	int sex=2, fl=-1,x=-1,y=-1;
	memset( buf1, 0, sizeof( buf1));

	if( NPC_Util_GetStrFromStrWithDelim( arg, "CHECKSEX", buf1, sizeof( buf1)) == NULL ) {
		return charaindex;
	}
	if( strstr( buf1, "male") != 0 ){
		sex = 1;
	}
	if( NPC_Util_GetStrFromStrWithDelim( arg, "NOBATTLEWARP", buf1, sizeof( buf1)) != NULL ) {
		char data[256];
		if( getStringFromIndexWithDelim( buf1,",", 1, data, sizeof(data)) == FALSE ) return charaindex;
		fl = atoi( data);
		if( getStringFromIndexWithDelim( buf1,",", 2, data, sizeof(data)) == FALSE ) return charaindex;
		x = atoi( data);
		if( getStringFromIndexWithDelim( buf1,",", 3, data, sizeof(data)) == FALSE ) return charaindex;
		y = atoi( data);
	}
	return NPC_DischargePartyForSex( meindex, charaindex, fl, x, y, sex);
}

int NPC_DischargePartyForSex( int meindex, int charaindex, int fl, int x, int y, int fsex)
{
	int Party[CHAR_PARTYMAX+3] = {-1,-1,-1,-1,-1,-1,-1,-1};
	int masterindex =-1, i, j, playerNo;
	if( CHAR_getWorkInt( charaindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_LEADER ){
		playerNo = CHAR_getInt( charaindex, CHAR_BASEBASEIMAGENUMBER);
		if( CHAR_getSexInt( playerNo ) == fsex )	{
			if( MAP_IsValidCoordinate( fl, x, y) != FALSE ){
				CHAR_warpToSpecificPoint( charaindex, fl, x, y);
				return -1;
			}
		}else	{
			return charaindex;
		}
	}
	CHAR_talkToCli( charaindex, -1, "团队重组！", CHAR_COLORYELLOW);

	for( i = 0; i < getPartyNum(charaindex); i ++ ) {
		int index = CHAR_getWorkInt( charaindex, CHAR_WORKPARTYINDEX1+i);
		if( !CHAR_CHECKINDEX( index)) continue;
		Party[i] = index;//纪录所有人
	}
	CHAR_DischargePartyNoMsg( charaindex);//解散团队
	
	for( i = 0; i < getPartyNum(charaindex); i ++ ) {
		int index = Party[i];
		playerNo = CHAR_getInt( index, CHAR_BASEBASEIMAGENUMBER);
		if( CHAR_getSexInt( playerNo ) == fsex )	{
			if( MAP_IsValidCoordinate( fl, x, y) != FALSE ){
				CHAR_warpToSpecificPoint( index, fl, x, y);
			}
			Party[i] = -1;
		}
	}
	j = CHAR_WORKPARTYINDEX1;
	for( i = 0; i < getPartyNum(charaindex); i ++ ) {
		int index = Party[i];
		if( !CHAR_CHECKINDEX( index ))continue;
		if( !CHAR_CHECKINDEX( masterindex )){
			masterindex = index;
			CHAR_setWorkInt( masterindex, CHAR_WORKPARTYMODE, CHAR_PARTY_LEADER);
			CHAR_setWorkInt( masterindex, j++, masterindex);
		}else	{
			CHAR_setWorkInt( masterindex, j++, index);
			CHAR_setWorkInt( index, CHAR_WORKPARTYMODE, CHAR_PARTY_CLIENT);
			CHAR_setWorkInt( index, CHAR_WORKPARTYINDEX1, masterindex);
		}
	}
	return masterindex;
}
#endif

BOOL NPC_ActionDuelRankCheck(int meindex,int talker,int rank,int flg)
{
	int myrank;
	myrank=CHAR_getWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC);
	if( NPC_ActionBigSmallLastCheck( rank, myrank, flg)==TRUE){
		return TRUE;
	}
	
	return FALSE;
}

BOOL NPC_ActionDuelPointCheck(int meindex,int talker,int rank,int flg)
{
	int myduel;
	myduel = CHAR_getInt( talker, CHAR_DUELPOINT);
	if( NPC_ActionBigSmallLastCheck( rank, myduel, flg)==TRUE){
		return TRUE;
	}
	
	return FALSE;
}

BOOL NPC_ActionTreasureTypeCheck(int meindex,int level, int type, int flg)
{
	int mylevel;

//	if( CHAR_getWorkInt( meindex, NPC_TIME_EVENTMODE) == NPC_EVENTMODE_EVENT ){
	mylevel = (type == NPC_EVENTMODE_EVENT)?1:0;
	if( NPC_ActionBigSmallLastCheck( level, mylevel, flg)==TRUE)
		return TRUE;
	return FALSE;
}

BOOL NPC_ActionTreasureRandItemGet(int meidex,int talker,int rand_j,char *buf)
{
	char buff2[64];
	int randitem;
	int ret;
	int itemindex;
	char token[128];

	if(rand_j == 0) {
		print("Event:由於０的介入，出现错误。");
 		return FALSE;
 	}
 	
	randitem = rand()%rand_j;
	if(randitem == 0) randitem = rand_j;

	getStringFromIndexWithDelim(buf , "," , randitem, buff2, sizeof(buff2)) ;

	itemindex = ITEM_makeItemAndRegist( atoi( buff2));

	if(itemindex == -1) return FALSE;
	/*失奶  丞及馨笛(  涛失奶  丞  卞中木化仄引丹  */
	ret = CHAR_addItemSpecificItemIndex( talker, itemindex);
	if( ret < 0 || ret >= CheckCharMaxItem(talker) ) {
		print( "npc_exchangeman.c: additem error itemindex[%d]\n", itemindex);
		ITEM_endExistItemsOne( itemindex);
		return FALSE;
	}

	if(itemindex != -1) {
		LogItem(
			CHAR_getChar( talker, CHAR_NAME ), /* 平乓仿   */
			CHAR_getChar( talker, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD 在item的log中增加item名称
			itemindex,
#else
			ITEM_getInt( itemindex, ITEM_ID),  /* 失奶  丞  寞 */
#endif
			"EventAddItem(任务需求所得到的道具)",
			CHAR_getInt( talker,CHAR_FLOOR),
			CHAR_getInt( talker,CHAR_X ),
 			CHAR_getInt( talker,CHAR_Y ),
            ITEM_getChar( itemindex, ITEM_UNIQUECODE),
			ITEM_getChar( itemindex, ITEM_NAME),
			ITEM_getInt( itemindex, ITEM_ID)
		);
	}
					
	sprintf(token,"收下了%s",ITEM_getChar( itemindex, ITEM_NAME));
	CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);

	CHAR_sendItemDataOne( talker, ret);
	return TRUE;

}

#ifdef  _NPC_ADDLEVELUP				// (不可开) ANDY 外部测试机用来增加玩家等级
extern  tagRidePetTable ridePetTable[296];
void NPC_ActionLevelAndTransUp( int meindex, int charindex, int level, int skillpoint, int exp, int ridepet)
{
	char szBuffer[256]="";
	if( !CHAR_CHECKINDEX( charindex) )
		return;
//int CHAR_LevelUpCheck( int charaindex , int toindex)
	if( exp > 0 ){
		int LevelUp=0;
		int myexp = CHAR_getInt( charindex, CHAR_EXP);
		myexp += exp;
		if( myexp > 300000000 ) myexp = 300000000;
		CHAR_setInt( charindex, CHAR_EXP, myexp);
		snprintf( szBuffer, sizeof(szBuffer),
			"(%s) 得到 EXP %d", CHAR_getUseName( charindex ), exp);
		CHAR_talkToCli( charindex, -1, szBuffer, CHAR_COLORYELLOW);
		LevelUp = CHAR_LevelUpCheck( charindex , -1);
		if( LevelUp > 0 ){
			snprintf( szBuffer, sizeof(szBuffer),
				"(%s) 升级至 %d",
				CHAR_getUseName( charindex ),
				CHAR_getInt( charindex, CHAR_LV )
			);
			CHAR_talkToCli( charindex, -1, szBuffer, CHAR_COLORYELLOW);
			CHAR_setInt( charindex, CHAR_SKILLUPPOINT,
				CHAR_getInt( charindex, CHAR_SKILLUPPOINT) +  LevelUp*3 );
			CHAR_complianceParameter( charindex );
		}
	}
	if( level > 0 ){
		CHAR_setInt( charindex, CHAR_LV, level);
		snprintf( szBuffer, sizeof(szBuffer),
			"(%s) 等级设定为 %d",
			CHAR_getUseName( charindex ),	CHAR_getInt( charindex, CHAR_LV )
		);
		CHAR_talkToCli( charindex, -1, szBuffer, CHAR_COLORYELLOW);
	}

	if( skillpoint > 0 ){
		int MySp = CHAR_getInt( charindex, CHAR_SKILLUPPOINT);
		MySp += skillpoint;
		CHAR_setInt( charindex, CHAR_SKILLUPPOINT, MySp);
	}
	if( ridepet >= 0 ){
		int basepet[2][4]={	{2066,2067,2068,2069},
							{2072,2073,2074,2075}};
		int petTemp[]={-1,-1,-1,-1,-1};
		char msgbuf[256];
		int petNum=0;
		int i,j=0;
		int k=0, petindex,enemyarray;
		petTemp[j++] = basepet[0][rand()%4];
		petTemp[j++] = basepet[1][rand()%4];

		for( i=0; i< arraysizeof(ridePetTable) ; i++ ){
			if( CHAR_getInt( charindex, CHAR_BASEBASEIMAGENUMBER) == ridePetTable[i].charNo ){
				petTemp[j]= ridePetTable[i].petId;
				j++;
				if( j >= arraysizeof( petTemp) )
					break;
			}
		}
		j=0;
		for( petNum=0; petNum<arraysizeof( petTemp); petNum++)	{
			enemyarray = ENEMY_getEnemyArrayFromId( petTemp[ petNum]); //白虎
			petindex = ENEMY_createPetFromEnemyIndex( charindex, enemyarray);
			if( petindex == -1 ) {
				continue;
			}
			j++;
			for( k = 1; k < 120; k ++ ){	//升级
				CHAR_PetLevelUp( petindex );
				CHAR_PetAddVariableAi( petindex, AI_FIX_PETLEVELUP );
				CHAR_setInt( petindex, CHAR_LV, CHAR_getInt( petindex, CHAR_LV) +1 );
			}
			CHAR_complianceParameter( petindex );
			CHAR_setInt( petindex , CHAR_HP , CHAR_getWorkInt( petindex , CHAR_WORKMAXHP ) );
			snprintf( szBuffer, sizeof(szBuffer), "得到%s", CHAR_getUseName( petindex) );
			CHAR_talkToCli( charindex, -1, szBuffer, CHAR_COLORYELLOW);
		}
		for(j = 0; j < CHAR_MAXPETHAVE; j++){
			petindex = CHAR_getCharPet(charindex, j);
			if( !CHAR_CHECKINDEX( petindex) )
				continue;
			CHAR_complianceParameter( petindex );
			snprintf( msgbuf, sizeof( msgbuf ), "K%d", j );
			CHAR_sendStatusString( charindex, msgbuf );
			snprintf( msgbuf, sizeof( msgbuf ), "W%d", j );
			CHAR_sendStatusString( charindex, msgbuf );
		}
		CHAR_setInt( charindex, CHAR_LEARNRIDE, ridepet);
	}
	CHAR_send_P_StatusString(  charindex, CHAR_P_STRING_DUELPOINT|
		CHAR_P_STRING_TRANSMIGRATION| CHAR_P_STRING_RIDEPET|
		CHAR_P_STRING_BASEBASEIMAGENUMBER| CHAR_P_STRING_GOLD|
		CHAR_P_STRING_EXP| CHAR_P_STRING_LV| CHAR_P_STRING_HP|CHAR_P_STRING_LEARNRIDE);
	CHAR_Skillupsend( charindex);
}
#endif


#ifdef _PROFESSION_SKILL			// WON ADD 人物职业技能
// 还原点数
int PROFESSION_RESTORE_POINT( int charaindex )
{
	int vital, str, tough, dex, MySp;
	int sum	= 0, sum2=0, fd=-1;
	char msg[256];
	//andy_log 修先判断可能中断的条件可减少做不必要的动作
	if( !CHAR_CHECKINDEX( charaindex ) ) return -1;
	if( (fd = getfdFromCharaIndex(charaindex) ) == -1 ) return -1;

	vital = CHAR_getInt( charaindex, CHAR_VITAL );		// 体
	str   = CHAR_getInt( charaindex, CHAR_STR );		// 攻
	tough = CHAR_getInt( charaindex, CHAR_TOUGH );		// 防
	dex	  = CHAR_getInt( charaindex, CHAR_DEX );		// 敏
	MySp = CHAR_getInt( charaindex, CHAR_SKILLUPPOINT);
	sum = vital + str + tough + dex;
	if( sum < 0 )	return -1;

	//andy_reEdit 2003.06.25 19:30
	vital = 20 * (vital / sum);
	str   = 20 * (str   / sum);
	tough = 20 * (tough / sum);
	dex	  = 20 * (dex   / sum);

/*	大点视为浮点数 浮点数会误判 1950 + 50 = 2000 但转换後/100 = 19 + 0 = 19
	vital = 2000 * vital / sum;
	str   = 2000 * str   / sum;
	tough = 2000 * tough / sum;
	dex	  = 2000 * dex   / sum;
*/	
	sum2 = vital + str + tough + dex;
    if( sum2 < 20)	vital = vital + (20 - sum2);
	CHAR_setInt( charaindex, CHAR_VITAL, vital *100);	// 体
	CHAR_setInt( charaindex, CHAR_STR,	 str *100);		// 攻
	CHAR_setInt( charaindex, CHAR_TOUGH, tough *100);	// 防
	CHAR_setInt( charaindex, CHAR_DEX,   dex *100);		// 敏
	
    sum /= 100;
	MySp = (sum + CHAR_getInt( charaindex, CHAR_SKILLUPPOINT )) - 20;
	CHAR_setInt( charaindex, CHAR_SKILLUPPOINT, MySp);

	lssproto_SKUP_send( fd, MySp );

	snprintf( msg, sizeof(msg), "还原点数得到 %d", MySp );
	CHAR_talkToCli( charaindex, -1, msg, CHAR_COLORYELLOW);	

	CHAR_complianceParameter( charaindex );
	CHAR_send_P_StatusString(  charaindex, 
		CHAR_P_STRING_DUELPOINT|
		CHAR_P_STRING_VITAL|
		CHAR_P_STRING_STR|
		CHAR_P_STRING_TOUGH|
		CHAR_P_STRING_DEX
	);

	return 1;
}
#endif

#ifdef _NPC_ADDWARPMAN1
BOOL NPC_ActionPartyCountCheck( int toindex, int nums, int flg)
{
	int partys=0;
	int i;

	partys = 0;
	for( i=0 ; i < getPartyNum(toindex  ) ;i++)
		if(CHAR_getWorkInt(toindex,CHAR_WORKPARTYINDEX1+i) != -1)
			partys++;

	if(NPC_ActionBigSmallLastCheck( nums, partys, flg) == TRUE)
		return TRUE;

	return FALSE;
}
//检查男生人数
BOOL NPC_ActionManCountCheck( int toindex, int nums, int flg)
{
	int i,tempcount = 0;
	if( CHAR_getWorkInt( toindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_NONE ){
	    if( IsMale(toindex) == 1 )
		    tempcount++;
	}
	else{
	    for( i=0 ; i < getPartyNum(toindex  ) ;i++)
	        if(CHAR_getWorkInt(toindex,CHAR_WORKPARTYINDEX1+i) != -1)
			    if( IsMale( CHAR_getWorkInt(toindex,CHAR_WORKPARTYINDEX1+i) ) == 1 )
				    tempcount++;
	}

	print("\n检查男生人数:%d",tempcount);
	if(NPC_ActionBigSmallLastCheck( nums, tempcount, flg) == TRUE)
		return TRUE;

	return FALSE;
}
//检查女生人数
BOOL NPC_ActionWomanCountCheck( int toindex, int nums, int flg)
{
	int i,tempcount = 0;
	if( CHAR_getWorkInt( toindex, CHAR_WORKPARTYMODE) == CHAR_PARTY_NONE ){
	    if( IsFemale(toindex) == 1 )
		    tempcount++;
	}
    else{
	    for( i=0 ; i < getPartyNum(toindex  ) ;i++)
	        if(CHAR_getWorkInt(toindex,CHAR_WORKPARTYINDEX1+i) != -1)
		        if( IsFemale( CHAR_getWorkInt(toindex,CHAR_WORKPARTYINDEX1+i) ) == 1 )
				    tempcount++;
	}
    print("\n检查女生人数:%d",tempcount);
	if(NPC_ActionBigSmallLastCheck( nums, tempcount, flg) == TRUE)
		return TRUE;

	return FALSE;
}
#endif


#ifdef _ANGEL_SUMMON
BOOL NPC_ActionMissionAngelCheck(int meindex,int talker,int mission,int flg,int checkitem)
{
	int mindex;
	char nameinfo[64];
	int i, itemindex, itemid;

	mindex = checkIfAngel( talker);
	if( mindex <= -1 ) return FALSE;
	if( missiontable[mindex].mission != mission)	return FALSE;
	getMissionNameInfo( talker, nameinfo);
	if( strcmp( nameinfo, missiontable[mindex].angelinfo ))	return FALSE;
	if( missiontable[mindex].flag != flg )	return FALSE;

	if( checkitem ) {	// 有无信物
		for( i=CHAR_STARTITEMARRAY;i<CheckCharMaxItem(talker);i++ ){
			itemindex = CHAR_getItemIndex( talker ,i );
			if( ITEM_CHECKINDEX( itemindex) )	{
				itemid=ITEM_getInt(itemindex ,ITEM_ID );
				if( itemid == ANGELITEM ) {
					if( !strcmp( ITEM_getChar( itemindex, ITEM_ANGELINFO), missiontable[mindex].angelinfo) 
						&& !strcmp( ITEM_getChar( itemindex, ITEM_HEROINFO), missiontable[mindex].heroinfo) )
						break;
				}
			}
		}
		if( i >= CheckCharMaxItem(talker) )
			return FALSE;
	}
	return TRUE;
}

BOOL NPC_ActionMissionHeroCheck(int meindex,int talker,int mission,int flg,int checkitem)
{
	int mindex;
	char nameinfo[64];
	int i, itemindex, itemid;

	mindex = checkIfAngel( talker);
	if( mindex <= -1 ) return FALSE;
	if( missiontable[mindex].mission != mission)	return FALSE;
	getMissionNameInfo( talker, nameinfo);
	if( strcmp( nameinfo, missiontable[mindex].heroinfo ))	return FALSE;
	if( missiontable[mindex].flag != flg )	return FALSE;

	if( checkitem ) {	// 有无信物
		for( i=CHAR_STARTITEMARRAY;i<CheckCharMaxItem(talker);i++ ){
			itemindex = CHAR_getItemIndex( talker ,i );
			if( ITEM_CHECKINDEX( itemindex) )	{
				itemid=ITEM_getInt(itemindex ,ITEM_ID );
				if( itemid == HEROITEM ) {
					if( !strcmp( ITEM_getChar( itemindex, ITEM_ANGELINFO), missiontable[mindex].angelinfo) 
						&& !strcmp( ITEM_getChar( itemindex, ITEM_HEROINFO), missiontable[mindex].heroinfo) )
						break;
				}
			}
		}
		if( i >= CheckCharMaxItem(talker) )
			return FALSE;
	}

	return TRUE;
}


BOOL NPC_HeroCompleteCountCheck(int meindex,int talker,int count,int flg)
{

	int herocnt;
	herocnt = CHAR_getInt(talker,CHAR_HEROCNT);

	if(NPC_EventBigSmallLastCheck( count, herocnt, flg) == TRUE)
	{
		if(flg == 3) return FALSE;
		return TRUE;
	}

	if(flg == 3) return TRUE;
	return FALSE;

}



#endif
