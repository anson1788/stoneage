#include "systeminc\version.h"
#include "systeminc/system.h"
#include "redMemoy.h"
#ifdef _RED_MEMOY_
RED_MEMOY redMemoy;
extern BOOL MakeHitBox(int x1, int y1, int x2, int y2, int dispPrio);
extern void lssproto_redMemoy_send(int fd,int index);
extern unsigned int sockfd;
int MemoyColor[]={
					0,1,2,3,4,5,6,7,8,9
				};
void RedMemoyInit()
{
	memset(&redMemoy,0,sizeof(RED_MEMOY));
}

void RedMemoySetup(int type,int time,int vip,char *name,int index)
{
	redMemoy.flg=1;
	redMemoy.type=type;
	redMemoy.time=TimeGetTime()+time;
	redMemoy.vip=vip;
	sprintf(redMemoy.name,"%s",name);
	redMemoy.index=index;
}

void RedMemoyCall()
{
	if(redMemoy.flg){
		if(TimeGetTime() < redMemoy.time){
			char str[128];
			int id;
			sprintf(str,"[%s]玩家 %s 分享红包%d金币！",
				redMemoy.type?"全服":"家族",redMemoy.name,redMemoy.vip);
			StockDispBuffer(400, 70, DISP_PRIO_ITEM, 60001, 1);//底图
			if(MakeHitBox(630-16,75-16,630+16,75+16,DISP_PRIO_ITEM+2)==1){
				StockDispBuffer(630, 75, DISP_PRIO_ITEM+1, 60003, 1);//按钮
				if( (mouse.onceState & MOUSE_LEFT_CRICK)){
					redMemoy.flg=0;
					extern void lssproto_redMemoy_send(int fd,int index,char *str);
					char str[128];
					sprintf(str,"%d",redMemoy.index);
					lssproto_redMemoy_send(sockfd,2,str);
				}
			}else StockDispBuffer(630, 75, DISP_PRIO_ITEM+1, 60002, 1);//按钮
			static int colorindex = 0;

			if(TimeGetTime() > redMemoy.colortime){
				redMemoy.colortime = TimeGetTime()+300;
				colorindex++;
				int num = sizeof(MemoyColor)/4;
				if(colorindex == num) colorindex = 0;
			}
			StockFontBuffer(220, 65, FONT_PRIO_FRONT, MemoyColor[colorindex], str, 0);
		}
	}
}

#endif



