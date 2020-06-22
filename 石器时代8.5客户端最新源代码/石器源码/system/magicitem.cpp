/************************/
#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/pc.h"
#include "../systeminc/shop.h"
#include "../systeminc/tool.h"
#include "../systeminc/menu.h"
#include "../systeminc/t_music.h"
#include "../systeminc/netmain.h"
#include "../systeminc/map.h"
#include "../NewProto/protocol.h"
#include "../systeminc/login.h"
#include "../other/caryIme.h"
#include "../systeminc/field.h"
#include "../wgs/tea.h"


#ifdef _MAGIC_ITEM_
ACTION* MagicItemActAddr;
int MagicItemCombinData[3];
int MagicItemPosState[4];

void InitMagicItemWin(char * 内容)
{

	if(!(MenuToggleFlag & JOY_CTRL_I))
		MenuToggleFlag |= JOY_CTRL_I;
	DeathAction(MagicItemActAddr);
	MagicItemActAddr=NULL;
	memset(MagicItemCombinData,0,sizeof(MagicItemPosState));
	memset(MagicItemPosState,0,sizeof(MagicItemPosState));
}



ACTION* CreateMagicItemWin()
{
#ifdef _SA_VERSION_25
	MagicItemActAddr = MakeWindowDisp(140,157,0,0,40177,0,FALSE);
#endif

	play_se(202,320,240);
	return MagicItemActAddr;
}
extern int cloasewindows;
extern ACTION* pActMenuWnd;
void ClearMagicItemWin()
{
	if(MagicItemActAddr){
		wnCloseFlag = 0;
		cloasewindows = 0;
		DeathAction(MagicItemActAddr);
		MagicItemActAddr=NULL;
		windowTypeWN = -1;
		wnCloseFlag = 1;
	}
}



ACTION* ShowMagicItemData()
{
	int 位置[3][2]={
		{105+35,44+35},
		{30+35,150+35},
		{180+35,150+35}
	};
	if(MakeHitBox(100+5,30+14,170+5,100+14,106)==1){
		MagicItemPosState[0] = 1;
	}else MagicItemPosState[0] = 0;
	if(MakeHitBox(30,150,100,220,106)==1){
		MagicItemPosState[1] = 1;
	}else MagicItemPosState[1] = 0;
	if(MakeHitBox(180,150,250,220,106)==1){
		MagicItemPosState[2] = 1;
	}else MagicItemPosState[2] = 0;

	if(MakeHitBox(2,2,274,362,-1)==1){
		MagicItemPosState[3]=1;
	}else MagicItemPosState[3]=0;
	int flg=TRUE;
	int i=0;
	for(;i<3;i++){
		if(MagicItemCombinData[i]>0) {
			StockDispBuffer(位置[i][0],位置[i][1], DISP_PRIO_DRAG,pc.item[MagicItemCombinData[i]].graNo, 0);
			if(MagicItemPosState[i]){
				char *splitPoint = pc.item[MagicItemCombinData[i]].memo;
				int color = pc.item[MagicItemCombinData[i]].color;
				if (pc.transmigration == 0 && pc.level < pc.item[MagicItemCombinData[i]].level) color = FONT_PAL_RED;
				StockFontBuffer(20, 250, FONT_PRIO_FRONT, color, pc.item[MagicItemCombinData[i]].name, 0);
				char damage_msg[256];
				sprintf_s(damage_msg, "耐久度(%s)", pc.item[MagicItemCombinData[i]].damage);
				StockFontBuffer(150, 250, FONT_PRIO_FRONT, color, damage_msg, 0);
				char moji[256];
				int x=20;
				int y=272;
				flg=FALSE;
				while (1){
					if (strlen(splitPoint) > 28){
						strncpy_s(moji, splitPoint, 28);
						moji[28] = NULL;
						if (GetStrLastByte(moji) == 3){
							moji[27] = NULL;
							splitPoint += 27;
						}else{
							moji[28] = NULL;
							splitPoint += 28;
						}
						StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, moji, 0); y += 22;
					}else{
						strcpy(moji, splitPoint);
						StockFontBuffer(x, y, FONT_PRIO_FRONT, 0, moji, 0);
						break;
					}
				}
			}
		}
	}
	if(flg){
		char *str="上方放入待强化装备，左下需放入强化石/精灵石，右下可选择性放入祝福石";
		char *splitPoint = str;
		char moji[256];
		int x=20;
		int y=272;
		while (1){
			if (strlen(splitPoint) > 34){
				strncpy_s(moji, splitPoint, 34);
				moji[34] = NULL;
				if (GetStrLastByte(moji) == 3){
					moji[33] = NULL;
					splitPoint += 33;
				}else{
					splitPoint += 34;
				}
				StockFontBuffer(x, y, FONT_PRIO_FRONT, 5, moji, 0); y += 22;
			}else{
				strcpy(moji, splitPoint);
				StockFontBuffer(x, y, FONT_PRIO_FRONT, 5, moji, 0);
				break;
			}
		}			
	}



	int id ;
	id = StockFontBuffer( 70,338, FONT_PRIO_FRONT,FONT_PAL_YELLOW, " 确  定 ", 2 );
	if(id ==HitFontNo){
		if( (mouse.onceState & MOUSE_LEFT_CRICK)){
		//	if(MagicItemCombinData[0]>0 && MagicItemCombinData[1]>0 && MagicItemCombinData[2]>0){
			if(MagicItemCombinData[0]>0 && MagicItemCombinData[1]>0 ){
				char 临时内容[10]={0};
				sprintf_s(临时内容,"%d|%d|%d",MagicItemCombinData[0],MagicItemCombinData[1],MagicItemCombinData[2]);
				extern int indexWN,idWN;
				lssproto_WN_send(sockfd,nowGx,nowGy,indexWN,idWN,0,临时内容);
				windowTypeWN = -1;
				wnCloseFlag = 1;
			}else{
				StockChatBufferLine("系统：您未放入合成材料!",6);
			}
		}
	}
	id = StockFontBuffer( 160,338, FONT_PRIO_FRONT, FONT_PAL_YELLOW, " 关  闭 ", 2 );
	if(id ==HitFontNo){
		if( (mouse.onceState & MOUSE_LEFT_CRICK)){
			windowTypeWN = -1;
			wnCloseFlag = 1;
		}
	}
	if( CheckMenuFlag()
		|| ((joy_trg[ 0 ] & JOY_ESC) && GetImeString() == NULL)
		|| actBtn == 1
		|| menuBtn == 1
		|| disconnectServerFlag == TRUE
		|| wnCloseFlag == 1 
		|| cloasewindows == 1)
	{
		if(!pActMenuWnd && wnCloseFlag==0){
			return MagicItemActAddr;
		}
		for(i=0;i<3;i++){
			if(MagicItemCombinData[i]>0) {
				ItemBuffer[MagicItemCombinData[i]].mixFlag=0;
				MagicItemCombinData[i]=0;
			}
		}
		extern void CloseMagicItemWin();
		CloseMagicItemWin();
		ClearMagicItemWin();
	}
	return MagicItemActAddr;
}
#endif






