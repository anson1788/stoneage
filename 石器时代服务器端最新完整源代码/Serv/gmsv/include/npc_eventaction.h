#ifndef _PRO_NPCFIX_H
#define _PRO_NPCFIX_H
//ANDY_Edit
BOOL ActionNpc_CheckMenuFree( int meindex, int toindex, char *npcarg, int nums);
BOOL ActionNpc_CheckFree( int meindex, int toindex, char *npcarg);
BOOL Action_RunDoEventAction( int meindex, int toindex, char *buf1);
BOOL ActionNpc_WarpPoint(int meindex,int talker,char *npcarg);
BOOL Action_PartyCheck( int meindex, int talker);
BOOL NPC_ActionAddGold( int talker, int Golds);
BOOL NPC_ActionDelGold( int talker,char *buf);
#ifdef _MOMENTUM_NPC
BOOL NPC_ActionMomentum( int talker,char *buf);
#endif
#ifdef _DEL_FAME
BOOL NPC_ActionDelFame( int talker,char *buf);
#endif
#ifdef _DP_NPC
BOOL NPC_ActionDP( int talker,char *buf);
#endif
#ifdef _SQL_BUY_FUNC
BOOL NPC_ActionOnlineBuy( int talker );
#endif
#ifdef _ONLINE_COST	
BOOL NPC_ActionOnlineCost( int talker );
#endif
#ifdef _VIP_SHOP
BOOL NPC_ActionDelVipPoint( int talker,char *buf);
#endif
#ifdef _TRANS_7_NPC
BOOL NPC_ActionTrans7( int talker, char *buf);
#endif

#ifdef _VIP_SHOP
BOOL NPC_ActionVipItem( int talker, char *buf);
BOOL NPC_ActionVipPet( int talker, char *buf);
#endif

#ifdef _NEW_VIP_SHOP
BOOL NPC_ActionNewVipItem( int talker, char *buf);
BOOL NPC_ActionNewVipPet( int talker, char *buf);
#endif


// WON ADD
BOOL NPC_ActionAddGold2( int talker,char *buf);
BOOL NPC_ActionDelPet( int toindex,char *buf);
#ifdef _NPC_NewDelPet
BOOL NPC_ActionNewDelPet( int talker,char *msg);
#endif
BOOL NPC_ActionAddPet( int talker, char *buf);
BOOL NPC_ActionDelItem( int talker,char *buf);
BOOL NPC_ActionAddItem( int talker, char *buf);
BOOL NPC_ActionSetEend( int talkerindex, char * buf);
BOOL NPC_ActionSetNow( int toindex, char * buf);
BOOL NPC_ActionClearEvent( int toindex, char * buf);

BOOL NPC_ActionCheckTrans(int meindex,int talker,int trans,int flg);
#ifdef _NPC_CHECKTIME
BOOL NPC_ActionTimeCheck(int meindex, int talker, char *buf, int date,int flg);
#endif
#ifdef _MISSION_TRAIN
BOOL NPC_ActionMissionTrainCheck(int meindex,int talker,int mt,int flg);
#endif
#ifdef _FAME_CHECK_NPC
BOOL NPC_ActionFameCheck(int meindex,int talker,int fame,int flg);
#endif

#ifdef _VIP_SHOP
BOOL NPC_ActionVipPointCheck(int meindex,int talker,int point,int flg);
#endif
#ifdef _VIP_RIDE
BOOL NPC_ActionVipCheck(int meindex,int talker,int vip,int flg);
#endif
#ifdef _MOMENTUM_NPC
BOOL NPC_ActionMomentumCheck(int meindex,int talker,int Momentum,int flg);
#endif
#ifdef _DP_NPC
BOOL NPC_ActionDPCheck(int meindex,int talker,int DP,int flg);
#endif
BOOL NPC_ActionLevelCheck(int meindex,int talker,int level,int flg);

#ifdef _PROFESSION_SKILL			// WON ADD 人物职业技能
BOOL NPC_ActionClassCheck( int meindex,int talker,int p_class,int flg);

BOOL NPC_ActionSkillCheck( int meindex,int talker,int skillId,int flg);
BOOL NPC_ActionSkillNumCheck( int meindex,int talker,int sknum,int flg);

BOOL NPC_ActionSkillPointAndSkillCheck( int meindex,int talker,int sknum,int flg);


BOOL NPC_ActionCleanProfession( int toindex, char *buf);
BOOL NPC_ActionAddProfessionSkillPoint( int toindex, char *buf );
#endif

BOOL NPC_ActionGoldCheck(int meindex,int talker,int Golds,int flg);
BOOL NPC_ActionPartyCheck( int toindex, int nums, int flg);
BOOL NPC_ActionReItemCheck( int toindex, int nums, int flg);
BOOL NPC_ActionRePetCheck( int toindex, int nums, int flg);
BOOL ActionCheckMyPet( int meindex, int talker, int petLv, int flg, int petid);
BOOL NPC_ActionBigSmallLastCheck(int point1,int mypoint,int flg);

BOOL NPC_ActionItemCheck(int meindex,int talker,int itemNo, int flg);
BOOL NPC_ActioneEquitCheck(int meindex,int talker,int itemNo, int flg);

BOOL NPC_ActionFreeIfCheck(int meindex,int talker, char* buf, char *opt, int kosuu,int flg, int temp);

BOOL NPC_ActionBigSmallCheck(int meindex,int talker,char* buf);
BOOL NPC_ActionPassCheck( int meindex, int talker, char *buf);
BOOL NPC_ActionWarpManReduce(int meindex,int talker,char *buf);
#ifdef _NPC_ActionFreeCmp 
BOOL NPC_ActionWarpManReduce2(int meindex,int talker,char *buf);
#endif
#ifdef _NPCCHANGE_PLAYERIMG	//andy
BOOL NPC_ActionChangePlayerBBI( int meindex, int charindex, char *Img);
BOOL NPC_CheckPlayerBBI( int meindex, int charindex, int BBI, int flg);
#endif

BOOL showString( int meindex, char *showstr,int flg);
BOOL NPC_getTimeXYPoint( int meindex, int Mode);
BOOL NPC_getTimeData( int meindex, char *npcarg, int *born, int *dead, int loopspeed);
void NPC_TimeDefineDo( int meindex, int born, int dead, int typeMode);
BOOL NPC_CheckTimeDefine( char *timetype);

void GetRand_WarpPoint( char *buf, int *fl, int *x, int *y );
#ifdef _EMENY_CHANCEMAN
int NPC_EmenyChanceCheck( int meindex, int charaindex, char *arg);
int NPC_DischargePartyForSex( int meindex, int charaindex, int fl, int x, int y, int fsex);
#endif

BOOL NPC_ActionDuelRankCheck(int meindex,int talker,int rank,int flg);
BOOL NPC_ActionDuelPointCheck(int meindex,int talker,int rank,int flg);

#ifdef _TREASURE_BOX
BOOL NPC_ActionTreasureTypeCheck( int meindex, int level, int type, int flg);
#endif
BOOL NPC_ActionTreasureRandItemGet(int meidex,int talker,int rand_j,char *buf);
void NPC_ActionLevelAndTransUp( int meindex, int charindex, int level,
							   int skillpoint, int exp, int ridepet);
#ifdef _ITEM_PILENUMS
BOOL NPC_ActionDoPileDelItem( int toindex, int itemID, int num);
BOOL NPC_DelItem( int toindex, int ti, int num);
#endif
void NPC_ActionDoPileClearItem( int toindex, int itemID);


#ifdef _PROFESSION_SKILL			// WON ADD 人物职业技能
int PROFESSION_RESTORE_POINT( int charaindex );
#endif


#ifdef _NPC_ADDWARPMAN1
BOOL NPC_ActionPartyCountCheck( int toindex, int nums, int flg);
BOOL NPC_ActionManCountCheck( int toindex, int nums, int flg);//检查男生人数
BOOL NPC_ActionWomanCountCheck( int toindex, int nums, int flg);//检查女生人数
#endif

#ifdef _ANGEL_SUMMON
BOOL NPC_ActionMissionAngelCheck(int meindex,int talker,int mission,int flg,int checkitem);
BOOL NPC_ActionMissionHeroCheck(int meindex,int talker,int mission,int flg,int checkitem);
BOOL NPC_HeroCompleteCountCheck(int meindex,int talker,int count,int flg);
#endif


#ifdef _PLAYER_DIY_MAP
BOOL NPC_ActionDiyMapCheck(int meindex,int talker,int diymap,int flg);
BOOL NPC_ActionPlayerDiyMap( int talker, char *buf);
BOOL NPC_ActionPlayerDiyMapBuy( int talker, char *buf);
BOOL NPC_ActionPlayerDiyMapLetter( int talker, char *buf);
BOOL NPC_ActionPlayerDiyMapSend( int talker, char *buf);
BOOL NPC_ActionPlayerDiyMapAddNum( int talker, char *buf);
BOOL NPC_ActionPlayerDiyMapAddExp( int talker, char *buf);
BOOL NPC_ActionPlayerDiyMapAddDay( int talker, char *buf);
#endif

#ifdef _LOTTERY_SYSTEM
BOOL NPC_ActionLotteryChange( int talker, char *buf);
BOOL NPC_ActionLotteryBuy( int talker, char *buf);
BOOL NPC_ActionLotteryAddTime( int talker, char *buf);
BOOL NPC_ActionLotteryShow( int talker);
BOOL NPC_ActionLotteryPlayerShow( int talker);
#endif
#ifdef _SHOW_ITEM_NAME
BOOL NPC_ActionShowItemName( int talker ,char *buf);
#endif
#ifdef _NPC_INT_SET
BOOL NPC_ActionIntSet( int talker,char *buf);
BOOL NPC_ActionIntChange( int talker,char *buf);
BOOL NPC_ActionIntShow( int talker,char *buf);
#endif
#ifdef _GLORY_POINT
BOOL NPC_ActionGloryCheck(int meindex,int talker,int Golds,int flg);
#endif
#endif
