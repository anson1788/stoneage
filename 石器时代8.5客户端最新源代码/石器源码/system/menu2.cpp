/*
	menu.cpp 档案太大了,新功能写在这
	*/
#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/font.h"
#include "../other/caryIme.h"
#include "../systeminc/anim_tbl.h"
#include "../systeminc/pc.h"
#include "../systeminc/battleMenu.h"
#include "../systeminc/battleProc.h"
#include "../systeminc/lssproto_cli.h"
#include "../systeminc/netmain.h"
#include "../systeminc/loadsprbin.h"
#include "../systeminc/savedata.h"
#include "../systeminc/t_music.h"
#include "../systeminc/menu.h"
#include "../systeminc/tool.h"
#include "../systeminc/map.h"
#include "../systeminc/field.h"
#include "../systeminc/pet_skillinfo.h"
#include "../wgs/descrypt.h"
#include "../systeminc/character.h"
#ifdef _TALK_WINDOW
#include "../systeminc/talkwindow.h"
#endif

#ifdef _NPC_ITEMUP
void ShowItemup(int value, int x, int y)
{
	char itemup_msg[256]="";
	int sx=-50,sy=-20;
	int s = ((((value)&0xff000000)>>24)&0x000000ff);//攻
	int t = ((((value)&0x00ff0000)>>16)&0x000000ff);//防
	int d = ((((value)&0x0000ff00)>>8 )&0x000000ff);//敏
	int h = ((value)&0x000000ff);//保留
	if( s > 0 ){
		sprintf_s( itemup_msg, "攻增加 %d", s); 
		StockFontBuffer( x+sx , y+sy, FONT_PRIO_FRONT, FONT_PAL_WHITE, itemup_msg, 0 );
		sy-=20;
	}
	if( t > 0 ){
		sprintf_s( itemup_msg, "防增加 %d", t); 
		StockFontBuffer( x+sx , y+sy, FONT_PRIO_FRONT, FONT_PAL_WHITE, itemup_msg, 0 );
		sy-=20;	
	}
	if( d > 0 ){
		sprintf_s( itemup_msg, "敏增加 %d", d); 
		StockFontBuffer( x+sx , y+sy, FONT_PRIO_FRONT, FONT_PAL_WHITE, itemup_msg, 0 );
		sy-=20;
	}

}
#endif

#ifdef _ITEM_COUNTDOWN
void ShowCounttime(int sectime, int x, int y)
{
	char buf[256];
	int  day;
	int  hour;
	int  min;

	if( sectime <= 0 ) return;
	if (sectime > 86400)
	{
		day = sectime / 86400;
		hour = (sectime / 3600) - (day * 24);
		min = (sectime / 60) - (day * 1440) - (hour * 60);
		sprintf_s( buf, "约%d天%d时%d分", day, hour, min); 
	}
	else if( sectime > 3600 ){
		hour = sectime/3600;
		min  = (sectime/60)-(hour*60);
		sprintf_s( buf,"约%d时%d分",hour,min); 
	}else if(sectime > 60 ){
		min  = sectime/60;
		sprintf_s( buf,"  约%d分",min); 
	}else{
		sprintf_s( buf,"  约1分"); 
	}
	//StockFontBuffer( x-20 , y-20, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf , 0 );	
	StockFontBuffer(x + 30 , y + 10, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf , 0);
}
#endif




#ifdef _PET_SKINS

static int btnShowSkinWin = -2;
static int btnShowSkinWinState = FALSE;
ACTION *petActSkins = NULL;
int originalPetSkin;

void initPetSkinWin()
{
	if (btnShowSkinWin != -2)
	{
		btnShowSkinWin = -2;
		btnShowSkinWinState = FALSE;
		if (petActSkins != NULL)
		{
			DeathAction(petActSkins);
			petActSkins = NULL;
		}
	}
}

void petChange(int graphNo)
{
	if (petActSkins != NULL)
	{
		petActSkins->anim_chr_no = graphNo;
	}
}

void petSkinProc()
{
	static int btnPrev;
	static int btnNext;
	static int btnPrevState;
	static int btnNextState;
	static int btnSkinSet;
	static int btnExit;
	static int x = 293, y = 120;

	btnShowSkinWin = StockDispBuffer(76, 290, DISP_PRIO_IME3, CG_PET_SKINS_SHOW_BTN + btnShowSkinWinState, 2);

	if (petActSkins == NULL && btnShowSkinWinState)
	{
		petActSkins = MakeAnimDisp(x + 65, y + 35, originalPetSkin, ANIM_DISP_PET);
	}

	if (HitDispNo == btnShowSkinWin){
		extern void ShowBottomLineString(int iColor, LPSTR lpstr);
		ShowBottomLineString(FONT_PAL_WHITE, "更换宠物皮肤。");
		if (mouse.onceState & MOUSE_LEFT_CRICK){

			if (btnShowSkinWinState){
				initPetSkinWin();
				play_se(203, 320, 240);
			}
			else
			{
				play_se(202, 320, 240);
				btnShowSkinWinState = TRUE;
			}
		}
	}

	if (btnShowSkinWinState)
	{
		//上一个皮肤
		if (HitDispNo == btnPrev){
			if (mouse.onceState & MOUSE_LEFT_CRICK){
				btnPrevState = TRUE;
			}
		}

		//下一个皮肤
		if (HitDispNo == btnNext){
			if (mouse.onceState & MOUSE_LEFT_CRICK){
				btnNextState = TRUE;
			}
		}

		//设定皮肤
		if (HitDispNo == btnSkinSet){
			if (mouse.onceState & MOUSE_LEFT_CRICK){

			}
		}

		//关闭预览
		if (HitDispNo == btnExit){
			if (mouse.onceState & MOUSE_LEFT_CRICK){
				initPetSkinWin();
			}
		}

		if (mouse.onceState & MOUSE_LEFT_CRICK_UP)
		{
			btnPrevState = FALSE;
			btnNextState = FALSE;
		}

		StockDispBuffer(358, 133, DISP_PRIO_MENU, CG_PET_SKINS_WIN, 0);
		btnPrev = StockDispBuffer(x, y, DISP_PRIO_IME3, CG_PET_SKINS_PREV_BTN + btnPrevState, 2);
		btnNext = StockDispBuffer(x + 132, y, DISP_PRIO_IME3, CG_PET_SKINS_NEXT_BTN + btnNextState, 2);
		btnSkinSet = StockDispBuffer(323, 246, DISP_PRIO_IME3, CG_PET_SKINS_SET, 2);
		btnExit = StockDispBuffer(395, 246, DISP_PRIO_IME3, 55252, 2);
	}
}
#endif


void CloseMagicItemWin()
{
	extern ACTION* pActMenuWnd2,*pActPet2;
	DeathAction(pActMenuWnd2);

	pActMenuWnd2 = NULL;
							// ????
	DeathAction(pActPet2);
	pActPet2 = NULL;
	extern int ItemMixPetNo,itemNo;
							// ????????
	ItemMixPetNo = -1;
							// ???????????
	MenuToggleFlag ^= JOY_CTRL_I;
	// ??????????????????
	if (MenuToggleFlag & JOY_CTRL_S){

		MenuToggleFlag ^= JOY_CTRL_S;	// CTRL + S ??
								// ??????????
		extern void DeathMenuAction(void);
		DeathMenuAction();
	}
							// ????????
	play_se(203, 320, 240);

	itemNo = -1;
#ifdef _MONEYINPUT //Syu ADD 手动输入金钱量
	extern bool Moneyflag  ;
	GetKeyInputFocus(&MyChatBuffer);
	Moneyflag = false;
#endif
}