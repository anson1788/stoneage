#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/CharSignDay.h"
#include "../newproto/autil.h"
#include "../systeminc/menu.h"
#include "../systeminc/shop.h"
#include "../systeminc/t_music.h"
#include "../newproto/protocol.h"
#include "../other/caryIme.h"
#include "../systeminc/field.h"
#include "../systeminc/netmain.h"
#ifdef _CHARSIGNDAY_
extern unsigned int sockfd;
extern int nowGx, nowGy;
extern int indexWN;
extern int idWN;
extern short wnCloseFlag;
extern int cloasewindows;
extern int windowTypeWN;
ACTION * 签到动作地址=0;
int 签到窗口X,签到窗口Y;
extern char 二级窗口内容[1024];
int 签到二级窗口;

typedef struct {
	int ID;
	char 道具名[64];
	char 数据[512];
}签到道具结构;

typedef struct {
	int 道具空;
	int 状态;
	int 当月天数;
	int 当天几号;
	签到道具结构 道具[30];
}签到结构;

签到结构 签到数据;
short 签到道具内容坐标[][2]={
							{0,3},
							{1,6},
							{2,8},
							{3,11},
							{4,13},
							{5,16}
						 };
int 签到窗口宽=588;
int 签到窗口高=396;

void 签到窗口初始化(char * 内容)
{
	memset(&签到数据,0,sizeof(签到结构));
	memset(二级窗口内容,0,1024);
	if(签到动作地址) DeathAction(签到动作地址);
	签到动作地址=0;
	char *临时=new char [512];
	char *临时数据=new char [512];
	memset(临时,0,512);
	memset(临时数据,0,512);
	getStringFromIndexWithDelim_body((char *) 内容,(char *)"|",1,临时,10);
	签到数据.道具空=atoi(临时);
	memset(临时,0,10);
	getStringFromIndexWithDelim_body((char *) 内容,(char *)"|",2,临时,10);
	签到数据.当月天数=atoi(临时);
	memset(临时,0,10);
	getStringFromIndexWithDelim_body((char *) 内容,(char *)"|",3,临时,10);
	签到数据.当天几号=atoi(临时);
	memset(临时,0,10);
	getStringFromIndexWithDelim_body((char *) 内容,(char *)"|",4,临时,30);
	签到数据.状态=atoi(临时);
	memset(临时,0,512);
	memset(临时数据,0,512);
	int i;
	for(i=0;i<签到数据.当月天数;i++){
		getStringFromIndexWithDelim_body((char *) 内容,(char *)"#@",2+i,临时,512);
		getStringFromIndexWithDelim_body((char *) 临时,(char *)"|",1,临时数据,10);
		签到数据.道具[i].ID=atoi(临时数据);
		memset(临时数据,0,10);
		getStringFromIndexWithDelim_body((char *) 临时,(char *)"|",2,临时数据,64);
		sprintf(签到数据.道具[i].道具名,"%s",临时数据);
		memset(临时数据,0,64);
		getStringFromIndexWithDelim_body((char *) 临时,(char *)"|",3,临时数据,512);
		sprintf(签到数据.道具[i].数据,"%s",临时数据);
		memset(临时,0,512);
		memset(临时数据,0,512);
	}
	delete 临时;
	delete 临时数据;
	签到二级窗口=0;
	签到窗口X=(800-签到窗口宽)/2+签到窗口宽/2;
	签到窗口Y=(600-签到窗口高)/2-40+签到窗口高/2;
}

ACTION* 创建签到窗口()
{
	if(签到动作地址) DeathAction(签到动作地址);
	签到动作地址 = MakeWindowDisp(签到窗口X,签到窗口Y,0,0,55217,0,FALSE);
	play_se(202,320,240);
	return 签到动作地址;
}

void 清除签到动作()
{
	if(签到动作地址) DeathAction(签到动作地址);
	签到动作地址=0;
	wnCloseFlag=0;
	windowTypeWN = -1;
	wnCloseFlag = 1;
}

ACTION* 签到数据显示()
{
	int 底座X=55;
	int 底座Y=58;
	int 当前底座数量=0;
	int i,b;
	for(i=0;i<5;++i){
		for(b=0;b<6;++b){
			当前底座数量++;
			StockDispBuffer(签到窗口X-225+b*底座X,签到窗口Y-65+i*底座Y,106,签到数据.道具[当前底座数量-1].ID,1);//道具
			if(MakeHitBox(签到窗口X-225+b*底座X-26,签到窗口Y-65+i*底座Y-26,签到窗口X-225+b*底座X+26,签到窗口Y-65+i*底座Y+26,1)==1){
				if(!签到二级窗口) 显示道具数据(签到窗口X-225+b*底座X,签到窗口Y-65+i*底座Y,112,签到数据.道具[当前底座数量-1].数据,0,签到数据.道具[当前底座数量-1].道具名);
			}
			if((签到数据.状态&(1<<当前底座数量)))
			//	StockDispBuffer(400, 288, DISP_PRIO_MENU-1, 55256, 0);
				StockDispBuffer(签到窗口X-225+b*底座X,签到窗口Y-65+i*底座Y,107,55269,1);//签到成功
			if(当前底座数量 >= 签到数据.当月天数) break;
		}
	}
//底图
	StockDispBuffer(签到窗口X,签到窗口Y+28,104,55217,1);
//签到
	int 按钮ID = StockDispBuffer(签到窗口X+200,签到窗口Y+207,105,55216,2);
	if( mouse.onceState & MOUSE_LEFT_CRICK ){
		if( HitDispNo == 按钮ID ){
			lssproto_WN_send(sockfd,nowGx,nowGy,indexWN,idWN,0,"1");
			签到二级窗口=1;		
		}
	
	}
	
//关闭
	if(MakeHitBox(签到窗口X+253,签到窗口Y-175,签到窗口X+283,签到窗口Y-149,1)==1){
		StockDispBuffer(签到窗口X+270,签到窗口Y-160,105,40165,1);
		if(mouse.onceState & MOUSE_LEFT_CRICK  && !签到二级窗口){
			windowTypeWN = -1;
			wnCloseFlag = 1;
		}
	}
	
	if(签到二级窗口){
		StockDispBuffer(签到窗口X-186,签到窗口Y-58,111,40088,1);
		StockFontBuffer(签到窗口X-138,签到窗口Y-16,2,0,二级窗口内容,0);
		按钮ID = StockDispBuffer(签到窗口X+5,签到窗口Y+37,112,26042,2);
		if( mouse.onceState & MOUSE_LEFT_CRICK ){
			if( HitDispNo == 按钮ID ){
				签到二级窗口=0;
				memset(二级窗口内容,0,1024);
			}
		}
	}
	if(CheckMenuFlag()
		|| ((joy_trg[ 0 ] & JOY_ESC) && GetImeString() == NULL)
		|| actBtn == 1
		|| menuBtn == 1
		|| disconnectServerFlag == TRUE
		|| wnCloseFlag == 1 
		|| cloasewindows == 1) 清除签到动作();
	return 签到动作地址;
}

void 置签到状态(int 状态)
{
	签到数据.状态=状态;
}



#endif



