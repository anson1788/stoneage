#ifndef __NPC_MAGICCARD_H__
#define __NPC_MAGICCARD_H__


#define	BANKERTOTAL			2   // °­©J¹Ö¡õ

struct BANKER
{
	short starttotal;
	short playertotal;
	short state;
	int waittime;					//Æû¡õ¼ª¶µ¡õ¹©
	int turntime;					//¡õÓÞ¡õ¹©
	short turn;
	int  hp[7]; 
	char cdkey[6][CDKEYLEN];
	char name[6][CHARNAMELEN];	
	unsigned int hand;
	unsigned int history;
	short select;
	short action;
	short process;
	short pauseflag[7];
};

extern struct BANKER banker[];
extern int Bettotal[BANKERTOTAL];
extern int Wintotal[BANKERTOTAL];
extern int tickettotal[BANKERTOTAL];

BOOL NPC_Magiccard_Init( int meindex );
void NPC_Magiccard_Loop( int meindex );
void NPC_Magiccard_WindowTalked( int meindex, int talkerindex, int seqno, int select, char *data);
void NPC_Magiccard_Talked( int meindex , int talkerindex , char *szMes , int color );

#endif