#ifndef __TRADE_H__
#define __TRADE_H__
#include "version.h"
#include "common.h"
#include "util.h"
#include "net.h"
#ifdef _TRADE_PK
typedef struct _tagTradeList{
	int charaindex;
	int fd;
	int PetTi[5];
	int ItemTi[15];
	int ItemNum[15];
	int Golds;
	int use;
}STradeList;
#endif
BOOL TRADE_Search(int fd, int meindex, char* message);
    
void CHAR_Trade(int fd, int index, char* message);

#ifdef _ITEM_PILEFORTRADE
void TRADE_InitTradeList( void);
#endif

#ifdef _TRADE_PK
BOOL TRADE_HandleTradeForPK( STradeList *temp1, STradeList *temp2 );
#endif
void TRADE_CheckTradeListUser( void);

#endif 
