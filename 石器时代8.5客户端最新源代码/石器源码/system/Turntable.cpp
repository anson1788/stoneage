#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/Turntable.h"
#include "../newproto/autil.h"
#include "../systeminc/menu.h"
#include "../systeminc/shop.h"
#include "../systeminc/t_music.h"
#include "../newproto/protocol.h"
#include "../other/caryIme.h"
#include "../systeminc/field.h"
#include "../systeminc/netmain.h"
char 二级窗口内容[1024];
#ifdef _ICONBUTTONS_
extern unsigned int sockfd;
extern int nowGx, nowGy;
extern int indexWN;
extern int idWN;
ACTION* 转盘动作地址=0;

typedef struct {
	int 形像;
	char 名[64];
	char 数据[512];
}转盘道具结构;

typedef struct {
	int x;
	int y;
}转盘坐标结构;

int 转盘道具位置[10][2]={
						{0x0,-0x74},
						{0x53,-0x5B},
						{0x86,-0x13},
						{0x86,0x43},
						{0x57,0x8A},
						{0x2,0xA6},
						{-0x51,0x8A},
						{-0x84,0x46},
						{-0x85,-0x10},
						{-0x52,-0x58},
						};

转盘坐标结构 转盘坐标[10];

typedef struct {
	int x;
	int y;
	int 选中索引;
	BOOL 二级窗口;
	int 转动;//(0未转动 1转动中 2转动完了)
	int 指针位置;
	int 指针次数;
	int 指针圈数;
	转盘道具结构 道具[10];
}转盘窗口结构;

转盘窗口结构 转盘窗口;

void 转盘窗口初始化(char * 内容)
{
	转盘坐标[0].x=0;
	转盘坐标[0].y=-0x30;
	转盘坐标[1].x=0x2C;
	转盘坐标[1].y=-0x21;
	转盘坐标[2].x=0x49;
	转盘坐标[2].y=0x7;
	转盘坐标[3].x=0x48;
	转盘坐标[3].y=0x2F;
	转盘坐标[4].x=0x2D;
	转盘坐标[4].y=0x57;
	转盘坐标[5].x=0x4;
	转盘坐标[5].y=0x66;
	转盘坐标[6].x=-0x27;
	转盘坐标[6].y=0x5A;
	转盘坐标[7].x=-0x48;
	转盘坐标[7].y=0x32;
	转盘坐标[8].x=-0x47;
	转盘坐标[8].y=0x8;
	转盘坐标[9].x=-0x2A;
	转盘坐标[9].y=-0x20;
	memset(&转盘窗口,0,sizeof(转盘窗口结构));
	memset(二级窗口内容,0,1024);
	if(转盘动作地址) DeathAction(转盘动作地址);
	转盘动作地址=0;
	char *临时=new char [513];
	char *临时1=new char [513];
	int i;
	for(i=0;i<10;i++){
		memset(临时,0,512);
		memset(临时1,0,20);
		getStringFromIndexWithDelim_body((char *) 内容,(char *)"#@",i+1,临时,512);
		getStringFromIndexWithDelim_body((char *) 临时,(char *)"|",1,临时1,512);
		转盘窗口.道具[i].形像 = atoi(临时1);
		getStringFromIndexWithDelim_body((char *) 临时,(char *)"|",2,转盘窗口.道具[i].名,64);
		getStringFromIndexWithDelim_body((char *) 临时,(char *)"|",3,转盘窗口.道具[i].数据,512);
	}
	delete 临时;
	delete 临时1;
	int 窗口宽=424;
	int 窗口高=424;
	转盘窗口.x=(800-窗口宽)/2+窗口宽/2;
	转盘窗口.y=(600-窗口高)/2-40+窗口高/2;
}

ACTION* 创建转盘窗口()
{
	if(转盘动作地址) DeathAction(转盘动作地址);
	转盘动作地址=0;
	转盘动作地址 = MakeWindowDisp(转盘窗口.x,转盘窗口.y,0,0,40166,0,FALSE);
	play_se(202,320,240);
	return 转盘动作地址;
}

void 转动后没给道具()
{
	if(转盘窗口.转动==1){
		转盘窗口.转动=2;
		lssproto_WN_send(sockfd,nowGx,nowGy,indexWN,idWN,0,"2");
	}
}
extern short wnCloseFlag;
extern int windowTypeWN;
void 清除转盘动作()
{
	if(转盘动作地址) DeathAction(转盘动作地址);
	转动后没给道具();
	转盘动作地址=0;
	wnCloseFlag=0;
	windowTypeWN = -1;
	wnCloseFlag = 1;
}
extern int cloasewindows;

ACTION* 转盘数据显示()
{
	//底图
	StockDispBuffer(转盘窗口.x,转盘窗口.y+28,104,40166,1);

	//关闭
	if(MakeHitBox(转盘窗口.x+0x61,转盘窗口.y-0x99,转盘窗口.x+0x80,转盘窗口.y-0x7D,1)==1){
		StockDispBuffer(转盘窗口.x+0x71-2,转盘窗口.y-0x8B,105,40165,1);
		if( (mouse.onceState & MOUSE_LEFT_CRICK)  && !转盘窗口.二级窗口){
			windowTypeWN = -1;
			wnCloseFlag = 1;
		}
	}
	//开始按钮
	if(转盘窗口.转动)
		StockDispBuffer(转盘窗口.x,转盘窗口.y+0x1B+1,105,40164,1);
	else{
		if(MakeHitBox(转盘窗口.x-0x30,转盘窗口.y-0x14,转盘窗口.x+0x30,转盘窗口.y+0x4A,1)==1){
			StockDispBuffer(转盘窗口.x,转盘窗口.y+0x1B+1,105,40164,1);
			if( (mouse.onceState & MOUSE_LEFT_CRICK) && !转盘窗口.二级窗口){
				lssproto_WN_send(sockfd,nowGx,nowGy,indexWN,idWN,0,"1");
			}
		}
	}
	if(转盘窗口.转动==1){
		if(转盘窗口.指针次数>2){
			转盘窗口.指针次数 = 0;
			if(转盘窗口.指针位置==9) {
				转盘窗口.指针圈数++;
				转盘窗口.指针位置=0;
			}
			else 转盘窗口.指针位置++;
		}else 转盘窗口.指针次数++;
		if(转盘窗口.指针圈数 == 8 && 转盘窗口.指针位置==(转盘窗口.选中索引-1)) {
			转盘窗口.转动=2;
			转盘窗口.二级窗口=TRUE;
			lssproto_WN_send(sockfd,nowGx,nowGy,indexWN,idWN,0,"2");
		}
	}
	StockDispBuffer(转盘窗口.x+转盘坐标[转盘窗口.指针位置].x,转盘窗口.y+转盘坐标[转盘窗口.指针位置].y,106,40153+转盘窗口.指针位置,1);
	//道具显示
	int i;
	for(i=0;i<10;i++){
		StockDispBuffer(转盘窗口.x+转盘道具位置[i][0],转盘窗口.y+转盘道具位置[i][1],106,转盘窗口.道具[i].形像,1);
		if(!转盘窗口.二级窗口)
		if(MakeHitBox(转盘窗口.x+转盘道具位置[i][0]-30,转盘窗口.y+转盘道具位置[i][1]-30,转盘窗口.x+转盘道具位置[i][0]+30,转盘窗口.y+转盘道具位置[i][1]+30,1)==1){
			//extern void 显示道具数据(short x,short y,UCHAR 层次,char* 内容,int 形像 ,char*名字);
			显示道具数据(转盘窗口.x+转盘道具位置[i][0],转盘窗口.y+转盘道具位置[i][1],107,转盘窗口.道具[i].数据,0,转盘窗口.道具[i].名);
		}
	}
	//窗口提示
	if(转盘窗口.二级窗口){
		StockDispBuffer(转盘窗口.x-0xC1,转盘窗口.y-0x32,107,40088,1);
		extern int getTextLength(char * str);
		StockFontBuffer(转盘窗口.x-getTextLength(二级窗口内容)/2,转盘窗口.y+5,2,0,二级窗口内容,1);
		int 按钮ID = StockDispBuffer(转盘窗口.x,转盘窗口.y+50,109,26042,2);
		if( mouse.onceState & MOUSE_LEFT_CRICK ){
			if( HitDispNo == 按钮ID ){
				转盘窗口.二级窗口=0;
				windowTypeWN = -1;
				wnCloseFlag = 1;
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
		|| cloasewindows == 1) 清除转盘动作();
	return 转盘动作地址;
}

void 置转盘数据(int 选中索引)
{
	转盘窗口.转动=1;
	转盘窗口.选中索引=选中索引;
}
#endif





