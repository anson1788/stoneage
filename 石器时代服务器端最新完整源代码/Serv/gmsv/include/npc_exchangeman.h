#ifndef __NPC_EXCHANGEMAN_H__
#define __NPC_EXCHANGEMAN_H__
BOOL NPC_EventAddShowItem(int meindex,int talker,char *buf);
BOOL NPC_EventAddEgg(int meindex, int talker, char *buff2,int mode);
BOOL NPC_RandItemGet(int meidex,int talker,int rand_j,char *buf);
BOOL NPC_EventAddItem(int meindex,int talker,char *buf);
#ifdef _TRANS_7_NPC
BOOL NPC_EventTRANS(int meindex, int talker, char *buff2,int mode);
#endif
void NPC_ExChangeMan_selectWindow( int meindex, int talker,int num);
BOOL NPC_TypeCheck(int meindex,int talker,char *szMes);
int NPC_ExChangeManEventCheck( int meindex, int talker, char *buff1);
BOOL NPC_EventItemCheck(int meindex,int talker,int itemNo,int flg);
BOOL NPC_EventLevelCheck(int meindex,int talker,int level,int flg);
// Arminius 8.14 move to .h (for pet talk)
//BOOL NPC_EventBigSmallCheck(int meindex,int talker,char* buf);
BOOL NPC_EventFreeIfCheck(int meindex,int talker,char* buf,int kosuu,int flg);
BOOL NPC_EventBigSmallLastCheck(int point1,int mypoint,int flg);
BOOL NPC_ENDEventNoCheck(int meindex,int talker,int shiftbit,int flg);
BOOL NPC_NOWEventNoCheck(int meindex,int talker,int shiftbit,int flg);
BOOL NPC_TiemCheck(int meindex,int talker,int time,int flg);
BOOL NPC_EventAddPet(int meindex, int talker, char *buff2,int mode);
BOOL NPC_RandItemGet(int meindex, int talker,int rand_j,char *buf);

BOOL NPC_PetLvCheckType2(int petindex,int meindex,int talker,char *buf,int mode);
BOOL NPC_PetLvCheck(int meindex,int talker,char *buf,int mode);
void NPC_RequestMain(int meindex,int talker,char *buf);
void NPC_AcceptMain(int meindex,int  talker ,char*buf);
void NPC_MsgDisp(int meindex,int talker,int num);
BOOL NPC_EventAdd(int meindex,int talker,int mode);
BOOL NPC_AcceptDel(int meindex,int talker,int mode);
BOOL NPC_SavePointCheck(int meindex,int talker,int shiftbit,int flg);

BOOL NPC_EventReduce(int meindex,int talker,char *buf);
BOOL NPC_EventDelItem(int meindex,int talker,char *buf,int breakflg);
BOOL NPC_EventDelItemEVDEL(int meindex,int talker,char *buf,char *nbuf,int breakflg);
int NPC_EventFile(int meindex,int talker,char *buf);
BOOL NPC_EventDelPet(int  meindex,int  talker, int petsel);
void NPC_PetSkillMakeStr(int meindex,int toindex,int select);
void NPC_CleanMain(int meindex,int talker,char *buf);	
void NPC_EventPetSkill( int meindex, int talker, char *data);
BOOL NPC_ItemFullCheck(int meindex,int talker,char *buf,int mode,int evcnt);
void NPC_CharmStatus(int meindex,int talker);
BOOL NPC_ImageCheck(int meindex,int talker,int kosuu,int flg);
int NPC_EventGetCost(int meindex,int talker,char *arg);

BOOL NPC_EventWarpNpc(int meindex,char *buf);

void NPC_ExChangeManTalked( int meindex , int talkerindex , char *msg ,
                     int color );
BOOL NPC_ExChangeManInit( int meindex );
void NPC_ExChangeManWindowTalked( int meindex, int talkerindex, int seqno, int select, char *data);



BOOL NPC_EventCheckFlg( int talker, int shiftbit);
BOOL NPC_NowEventCheckFlg( int talker, int shiftbit);

// Arminius 8.14 move from .c (for pet talk)
BOOL NPC_EventBigSmallCheck(int meindex,int talker,char* buf);
	
#endif


 /*__NPC_EXCHANGEMAN_H__*/


