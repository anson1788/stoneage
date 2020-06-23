#ifndef __NPC_PETSHOP_H__
#define __NPC_PETSHOP_H__

void NPC_PetShopTalked( int meindex , int talkerindex , char *msg ,
                     int color );
BOOL NPC_PetShopInit( int meindex );
void NPC_PetShopWindowTalked( int meindex, int talkerindex, int seqno, int select, char *data);
void NPC_PetShopLooked( int meindex , int lookedindex);
void NPC_PetShop_selectWindow( int meindex, int toindex, int num,int select);

#endif 
/*__NPC_PETSHOP_H__*/
