#include "../systeminc/version.h"
#include"../systeminc/system.h"
#include"../systeminc/loadrealbin.h"
#include"../systeminc/anim_tbl.h"
#include"../systeminc/login.h"
#include"../systeminc/menu.h"
#include"../systeminc/map.h"
#include"../other/caryIme.h"
#include "../systeminc/ime_sa.h"
#include "../systeminc/t_music.h"
#include "../systeminc/field.h"
#include"../systeminc/netmain.h"
#include "../systeminc/lssproto_cli.h"

#ifdef _ANGEL_SUMMON
short jumpHelpPage = 0;
short jumpHelpSeg = 0;
#endif

#ifdef _JOBDAILY
extern JOBDAILY jobdaily[MAXMISSION];
extern int JobdailyGetMax;
#endif
short HelpProcNo;
//  return: 0 ... 处理中
//          !0 ... 处理完毕
ACTION *ptActMenuWinHelp = NULL;
int HelpProc()
{
	static int btnId[3], btnState[3];
	static int x, y, w, h;
	static int page = 0;
	static int pageMax=0;
	static int dwPressTime=0;
	//初始化
	if( ptActMenuWinHelp ==NULL){
		dwPressTime=0;
		page=0;
		w = 420;
		h = 380;
		x = (lpDraw->xSize -w)/2;
		y = (lpDraw->ySize -h)/2;
		ptActMenuWinHelp = MakeWindowDisp( x, y, w, h, CG_FIELD_HELP_WND, -1, FALSE);
		if(ptActMenuWinHelp){
			int i;
			for(i=0;i<3;i++){
				btnId[i]=-2;
				btnState[i]=0;
			}
#ifdef _JOBDAILY
			char look[10];
			strcpy(look,"dyedye");
			lssproto_JOBDAILY_send(sockfd,look);
#endif
		}
	}
	//初始化了
	if( ptActMenuWinHelp != NULL){
		if(ptActMenuWinHelp->hp>0){
			pageMax = JobdailyGetMax%10?JobdailyGetMax/10:JobdailyGetMax/10-1;
			if( CheckMenuFlag()||((joy_trg[ 0 ] & JOY_ESC) && GetImeString() == NULL)||HelpProcNo == 1000){
				DeathAction( ptActMenuWinHelp);
				ptActMenuWinHelp = NULL;
				play_se( 203, 320, 240);	//视窗关闭声

				return 1;
			}
			//秀视窗底图
			StockDispBuffer( ((WINDOW_DISP *)ptActMenuWinHelp->pYobi)->mx, ((WINDOW_DISP *)ptActMenuWinHelp->pYobi)->my, DISP_PRIO_MENU, CG_FIELD_HELP_WND, 1);
			int i;

			if(page>0)
				btnId[0] = StockDispBuffer( x+207+32, y+352+10, DISP_PRIO_IME3, CG_FIELD_HELP_PREPAGE+btnState[0], 2);
			else btnId[0]=-2;
			if(page < pageMax)
				btnId[1] = StockDispBuffer( x+277+32, y+352+10, DISP_PRIO_IME3, CG_FIELD_HELP_NEXTPAGE+btnState[1], 2);
			else btnId[1]=-2;
			btnId[2] = StockDispBuffer( x+347+32, y+352+10, DISP_PRIO_IME3, CG_FIELD_HELP_EXIT+btnState[2], 2);

			int id = selGraId( btnId, sizeof(btnId)/sizeof(int));
			if(id==0){
				dwPressTime = TimeGetTime();
				btnState[0]=1;
				play_se( 217, 320, 240);
			}
			if(id==1){
				dwPressTime = TimeGetTime();
				btnState[1]=1;
				play_se( 217, 320, 240);
			}
			if(id==2){
				dwPressTime = TimeGetTime();
				btnState[2]=1;
				play_se( 203, 320, 240);

				return 1;
			}
			if( dwPressTime){
				if( TimeGetTime()>(dwPressTime+100)){
					dwPressTime=0;
					if(btnState[0]){
						btnState[0]=0;
						page--;
					}
					if(btnState[1]){
						btnState[1]=0;
						page++;
					}
					if(btnState[2]){
						btnState[2]=0;
						DeathAction( ptActMenuWinHelp);
						ptActMenuWinHelp = NULL;
					}
				}
			}
			StockFontBuffer( x+20, y+35, FONT_PRIO_FRONT, FONT_PAL_GREEN, "任务日志", 0);
			StockFontBuffer(x+34     ,y+70,FONT_PRIO_FRONT,FONT_PAL_YELLOW,"编号              ─ 任务说明 ─                状态",0);
			for( i = 0; i < 10; i++){
				char buf[10];
				sprintf_s(buf,"%d",jobdaily[i+page*10].JobId);
				if(!atoi(buf))	buf[0] = '\0'; //不印0出来
				StockFontBuffer(x+34     ,y+(i+1)*25+70,FONT_PRIO_FRONT,FONT_PAL_WHITE,buf,0);
				StockFontBuffer(x+34+30  ,y+(i+1)*25+70,FONT_PRIO_FRONT,FONT_PAL_WHITE,jobdaily[i+page*10].explain,0);
				StockFontBuffer(x+34+328 ,y+(i+1)*25+70,FONT_PRIO_FRONT,FONT_PAL_WHITE,jobdaily[i+page*10].state,0);
			}
			return 0;
		}
	}
	if( CheckMenuFlag()||((joy_trg[ 0 ] & JOY_ESC) && GetImeString() == NULL)||HelpProcNo == 1000){
		DeathAction( ptActMenuWinHelp);
		ptActMenuWinHelp = NULL;
		play_se( 203, 320, 240);	//视窗关闭声

		return 1;
	}
}

