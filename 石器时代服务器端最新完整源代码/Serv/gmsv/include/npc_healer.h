#ifndef __NPC_HEALER_H__
#define __NPC_HEALER_H__

void NPC_HealerTalked( int meindex , int talkerindex , char *msg ,
                     int color );
void NPC_HealerAllHeal( int talker );
BOOL NPC_HealerInit( int meindex );

#endif
 /*__NPC_HEALER_H__*/
