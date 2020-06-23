/************************/
/*	ime.cpp				*/
/************************/
#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "winnls32.h"
#include "../systeminc/font.h"
#include "../other/caryIme.h"
#include "../systeminc/menu.h"
#include "../systeminc/battleMenu.h"
#ifdef _TALK_WINDOW
#include "../systeminc/talkwindow.h"
#endif
#include "../systeminc/DirectDraw.h"
DWORD dwInfo;
void ShowBottomLineString(int iColor,LPSTR lpstr)
{
	LPSTR lpstr1=GetImeString();
	dwInfo = 0;
	if(!lpstr1 && lpstr[0]!=0){
		dwInfo = 1;
		StockFontBuffer( 8, 460 + DISPLACEMENT_Y, FONT_PRIO_FRONT, iColor, lpstr, 0 );
	}
}

// Terry add 2003/12/16 for 交易视窗开启时,显示物品说明不显示输入法
extern BOOL bShowItemExplain;
#ifdef _MO_SHOW_FPS
extern int framesToShow;
extern int skipFramesToShow;
#endif
// end
//	输入法的处理
void ImeProc()
{
	// 显示输入法的名称
// Terry fix 2003/12/16 for 交易视窗开启时,显示物品说明不显示输入法
	//if( TaskBarFlag == FALSE ){ 这行改成下一行
	if(TaskBarFlag == FALSE && bShowItemExplain == FALSE){
// end
		LPSTR lpstr=GetImeString();
		LPSTR lpstr1;
		if(lpstr){
#ifdef _TELLCHANNEL
			StockFontBuffer(8,420 + DISPLACEMENT_Y ,FONT_PRIO_FRONT,FONT_PAL_WHITE,lpstr,0);
#else
			StockFontBuffer(8,460 + DISPLACEMENT_Y ,FONT_PRIO_FRONT,FONT_PAL_WHITE,lpstr,0);
#endif
#ifdef _TALK_WINDOW
		if(g_bTalkWindow) TalkWindow.Update();
#endif
		}
		lpstr1=GetImeDescString();
		if( 1!=dwInfo){
			if( lpstr1){
				char* BIG5ToGB2312(const char* szBIG5String);
				extern int 编码;
				extern int 繁体开关;
				if(繁体开关){
					char 繁体[1024]={0};
					LCMapString(0x804,0x4000000,lpstr1, strlen(lpstr1),繁体,1024);
					if(编码==950)
						StockFontBuffer(530-strlen(lpstr1)*(FONT_SIZE>>1) + DISPLACEMENT_X,460 + DISPLACEMENT_Y ,FONT_PRIO_FRONT,0,BIG5ToGB2312((const char *)繁体),0);
					else
						StockFontBuffer(530-strlen(lpstr1)*(FONT_SIZE>>1) + DISPLACEMENT_X,460 + DISPLACEMENT_Y ,FONT_PRIO_FRONT,0,繁体,0);
				}else{
					if(编码==950)
						StockFontBuffer(530-strlen(lpstr1)*(FONT_SIZE>>1) + DISPLACEMENT_X,460 + DISPLACEMENT_Y ,FONT_PRIO_FRONT,0,BIG5ToGB2312((const char *)lpstr1),0);
					else
						StockFontBuffer(530-strlen(lpstr1)*(FONT_SIZE>>1) + DISPLACEMENT_X,460 + DISPLACEMENT_Y ,FONT_PRIO_FRONT,0,lpstr1,0);
				}
			}

			char tmp[64];
			extern DWORD dwPingTime, dwPingState;
			if( dwPingState & 0x80000000){
				wsprintf( tmp, "*%d", dwPingTime);
				dwPingState++;
				if( (dwPingState&0xff) > 40)
					dwPingState = 0;
			}else
				wsprintf( tmp, " %d", dwPingTime);
			StockFontBuffer( 605 + DISPLACEMENT_X, 460 + DISPLACEMENT_Y , FONT_PRIO_FRONT, FONT_PAL_WHITE, tmp, 0 );
#ifndef _REMAKE_20
			lpstr = "玩家模式";
			switch( AI){
			case AI_ATTACK:
				lpstr = "强制攻击";
				break;
			case AI_GUARD:
				lpstr = "强制防御";
				break;
			case AI_SELECT:
				lpstr = "ＡＩ模式";
				break;
			}
			StockFontBuffer( 540 + DISPLACEMENT_X, 460 + DISPLACEMENT_Y , FONT_PRIO_FRONT, FONT_PAL_WHITE, lpstr, 0 );
#endif
#ifdef _CHANNEL_MODIFY
			int FontColor = 0 ;
			switch(TalkMode){
			case 0:
				lpstr1 = "一般频道";
				break;
			case 1:
				FontColor = FONT_PAL_GREEN;
				lpstr1 = "密语频道";
				break;
			case 2:
				FontColor = FONT_PAL_AQUA;
				lpstr1 = "队伍频道";
				break;
			case 3:
				FontColor = FONT_PAL_PURPLE;
				lpstr1 = "家族频道";
				break;
#ifdef _CHAR_PROFESSION
			case 4:
				FontColor = FONT_PAL_BLUE2;
				lpstr1 = "职业频道";
				break;
#else
			case 4:
				FontColor = FONT_PAL_PURPLE;
				lpstr1 = "聊天室频道";
				break;
#endif
#ifdef _CHANNEL_WORLD  
			case 5:
				FontColor = FONT_PAL_YELLOW;
				lpstr1 = "世界频道";
				break;
#endif
#ifdef _CHANNEL_ALL_SERV
			case 6:
				FontColor = FONT_PAL_GREEN2;
				lpstr1 = "星球频道";
				break;
#endif
			}
			StockFontBuffer( 10, 460 + DISPLACEMENT_Y, FONT_PRIO_FRONT, FontColor, lpstr1, 0 );
#else
	#ifdef _TELLCHANNEL 
			int FontColor = 0 ;
			switch(TalkMode){
			case 0:
				lpstr1 = "一般模式";
				break;
			case 1:
				FontColor = FONT_PAL_GREEN;
				lpstr1 = "密语模式";
				break;
	#ifdef _FRIENDCHANNEL
			case 2:
				FontColor = FONT_PAL_PURPLE;
				lpstr1 = "频道模式";
				break;
	#endif
#ifdef _CHANNEL_WORLD
			case 6:
				FontColor = FONT_PAL_BLUE;
				lpstr1 = "世界模式";
				break;
#endif
#ifdef _CHANNEL_ALL_SERV
			case 7:
				FontColor = FONT_PAL_BLUE;
				lpstr1 = "星球模式";
				break;
#endif
			}
			StockFontBuffer( 10, 460 + DISPLACEMENT_Y, FONT_PRIO_FRONT, FontColor, lpstr1, 0 );
	#endif
#endif
#ifdef _MO_SHOW_FPS
			
			wsprintf(tmp, "当前FPS:%3d帧/秒 , 跳帧数: %d帧/秒", framesToShow, skipFramesToShow);
			StockFontBuffer( 270, 460 + DISPLACEMENT_Y, FONT_PRIO_FRONT, FONT_PAL_YELLOW, tmp, 0 );
#endif
		}
	}
// Terry add 2003/12/16 for 交易视窗开启时,显示物品说明不显示输入法
	bShowItemExplain = FALSE;
// end
}

