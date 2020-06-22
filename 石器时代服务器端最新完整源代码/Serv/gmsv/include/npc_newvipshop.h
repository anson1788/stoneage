#ifndef _NEW_VIP_SHOP_H
#define _NEW_VIP_SHOP_H

void NPC_NewVipShopTalked( int meindex, int talkerindex, char *msg, int color );
BOOL NPC_NewVipShopInit( int meindex );
void NPC_NewVipShopWindowTalked( int meindex, int talkerindex, int seqno, int select, char *data);
void NPC_NewVipShopLoop( int meindex);

#endif
