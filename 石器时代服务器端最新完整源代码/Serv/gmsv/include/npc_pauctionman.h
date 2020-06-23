#ifndef _PAUCTION_H
#define _PAUCTION_H

BOOL NPC_PauctionInit( int meindex );
void NPC_PauctionLoop( int meindex);
void NPC_PauctionTalked( int meindex, int talkerindex, char *msg, int color );
void NPC_PauctionWindowTalked( int meindex, int talkerindex, int seqno,	int select, char *data);
#endif
