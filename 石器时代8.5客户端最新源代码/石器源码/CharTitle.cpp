#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/font.h"
#include "../systeminc/action.h"
#include "systeminc\sprdisp.h"
#include "systeminc\menu.h"
#include "systeminc\mouse.h"
#include "newproto\autil.h"
#include "newproto\protocol.h"
#ifdef _CHARTITLE_STR_
extern unsigned int sockfd;
typedef struct{
	BOOL 窗口显示;
	BOOL 确定窗口显示;
	int 当前页数;
	int 确定图档索引;
	char 介绍[100][32];
	int 称号索引[100];
	TITLE_STR title;//当前称号
	TITLE_STR titledata[100];//称号数据
	BOOL 关闭窗口;
}称号数据结构;
extern int play_se(int tone, int x, int y);
称号数据结构 称号;

void 打开称号窗口初始化()
{
	if(称号.窗口显示){
		称号.窗口显示=FALSE;
		称号.确定窗口显示=FALSE;
	}else{
		称号.当前页数=0;
		称号.窗口显示=TRUE;
		称号.确定窗口显示=FALSE;
	}
}

extern void PutTitleText(int x,int y,char fontPrio,TITLE_STR str,BOOL hitFlag);

BOOL 获取称号取消窗口()
{
	return 称号.关闭窗口;
}

void 设置称号取消窗口()
{
	if(称号.关闭窗口) 称号.关闭窗口=FALSE;
	else 称号.关闭窗口=TRUE;
}

BOOL 获取当前称号是否存在()
{
	return 称号.title.flg;
}

void 称号展示()
{
	if(称号.title.flg){
		PutTitleText(78-称号.title.len/2,49,FONT_PRIO_FRONT,称号.title,0);
	}
	int bnt;
	bnt = StockFontBuffer(18,49,1,FONT_PAL_GREEN,"                             ",2);
	if(mouse.onceState & MOUSE_LEFT_CRICK>0){
		if(HitFontNo == bnt){
			if(称号.关闭窗口) 称号.关闭窗口=FALSE;
			else 称号.关闭窗口=TRUE;
			play_se(203, 320, 240);
		}
	}
}

BOOL 人物称号处理(ACTION * pct)
{
	int x,y;
	x = pct->x;
	y = pct->y + 133;
	int 按钮左=-1;
	int 按钮右=-1;
	int 按钮关闭=-1;
	int 按钮ID;
	按钮左 = StockDispBuffer( x -55, y + 7, DISP_PRIO_IME3, 55250, 2 );
	按钮右 = StockDispBuffer( x -20, y + 7, DISP_PRIO_IME3, 55251, 2 );
	按钮关闭 = StockDispBuffer( x+40, y + 7, DISP_PRIO_IME3, 55252, 2 );
	if( mouse.onceState & MOUSE_LEFT_CRICK ){
		if(!称号.确定窗口显示){
			if(按钮左 == HitDispNo){
				if(称号.当前页数){
					称号.当前页数--;
				}
			}
			if(按钮右 == HitDispNo){
				if(称号.当前页数!=19 && 称号.titledata[(称号.当前页数+1)*5].flg)
				{
					称号.当前页数++;
				}
			}
			if(按钮关闭 == HitDispNo){
				DeathAction(pct);
				称号.窗口显示=FALSE;
				称号.确定窗口显示=FALSE;
				return TRUE;
			}
		}
	}

	int i=0;
	int 称号图档索引;
	for(;i<5;i++){
		称号图档索引 = 称号.当前页数*5+i;
		if(称号.titledata[称号图档索引].flg)
		{
			PutTitleText(380-称号.titledata[称号图档索引].len/2,43+i*0x20,FONT_PRIO_FRONT,称号.titledata[称号图档索引],0);
			char 称号序号[128];
			sprintf_s(称号序号,"%03d",称号图档索引+1);
			StockFontBufferExt(0x130,0x2D+i*0x20,1,FONT_PAL_GREEN,称号序号,0,19);
			if(!称号.确定窗口显示){
				if(MakeHitBox(0x124,0x26+i*0x20,0x1B8,0x44+i*0x20,0x6D)==1){
					StockFontBuffer(0x123,0xD1,2,0,称号.介绍[称号图档索引],0);
					if(mouse.onceState & MOUSE_LEFT_CRICK>0){
						称号.确定图档索引=称号.称号索引[ 称号图档索引];
						称号.确定窗口显示=TRUE;
					}
				}
			}
		}else break;
	}
	if(称号.确定窗口显示){
		StockFontBuffer(0x123,0xD1,2,0,称号.介绍[称号.确定图档索引],0);
		StockFontBuffer(0x130,0x118,2,0,"确定使用该称号吗？",0);
		StockDispBuffer(0x16D,0x130,0x6C,55253,1);//确定窗口底图
		按钮ID = StockDispBuffer(0x194,0x148,109,55252,2);//关闭
		if(mouse.onceState & MOUSE_LEFT_CRICK>0){
			if(按钮ID == HitDispNo){
				称号.确定窗口显示=FALSE;
				称号.确定图档索引=0;
			}
		}
		按钮ID = StockDispBuffer(0x148,0x148,109,55254,2);//确定
		if(mouse.onceState & MOUSE_LEFT_CRICK>0){
			if(按钮ID == HitDispNo){
				int checksum=0;
				char buf[1024*4];
				memset(buf,0,1024*4);
				checksum += util_mkint(buf, 称号.确定图档索引);
				util_mkint(buf, checksum);
				util_SendMesg(sockfd, LSSPROTO_CHARTITLE_SEND, buf);
				称号.确定窗口显示=FALSE;
				称号.确定图档索引=0;
			}
		}
	}
	return FALSE;
}
extern void getCharTitleSplit( char *str,TITLE_STR* title);
void 人物称号数据更新(char *数据)
{
	char 基础内容[126],图档内容[1024],图档索引[1024],内容介绍[4000],临时[126];
	memset(基础内容,0,126);
	memset(图档内容,0,1024);
	memset(图档索引,0,1024);
	memset(内容介绍,0,4000);
	getStringFromIndexWithDelim_body((char *) 数据,(char *)"#@",1,基础内容,126);
	getStringFromIndexWithDelim_body((char *) 基础内容,(char *)"|",1,临时,126);
	int titleindex = atoi(临时);
	extern char* FreeGetTitleStr(int id);
	if(titleindex>0) getCharTitleSplit(FreeGetTitleStr(titleindex),&称号.title);
	else memset(&称号.title,0,sizeof(TITLE_STR));
	getStringFromIndexWithDelim_body((char *) 基础内容,(char *)"|",2,临时,126);
	int 数量 = atoi(临时);
	if(数量!=0){
		memset(&称号.titledata,0,sizeof(称号.titledata[0])*100);
		memset(&称号.称号索引,0,sizeof(int)*100);
		memset(&称号.介绍,0,32*100);
		getStringFromIndexWithDelim_body((char *) 数据,(char *)"#@",2,图档内容,1024);
		getStringFromIndexWithDelim_body((char *) 数据,(char *)"#@",3,内容介绍,4000);
		getStringFromIndexWithDelim_body((char *) 数据,(char *)"#@",4,图档索引,1024);
		int i=0;
		extern char* FreeGetTitleStr(int id);
		for(i;i<数量;i++){
			getStringFromIndexWithDelim_body((char *)图档内容,(char *)"|",i+1,临时,126);
			titleindex = atoi(临时);
			getCharTitleSplit(FreeGetTitleStr(titleindex),&称号.titledata[i]);
			getStringFromIndexWithDelim_body((char *)图档索引,(char *)"|",i+1,临时,126);
			称号.称号索引[i]=atoi(临时);
			getStringFromIndexWithDelim_body((char *)内容介绍,(char *)"|",i+1,临时,126);
			sprintf_s(称号.介绍[i],32,"%s",临时);
		}
	}
}

void 初始化称号结构()
{
	memset(&称号,0,sizeof(称号数据结构));
}
#endif



#ifdef _CHARTITLE_
extern unsigned int sockfd;
typedef struct{
	int 称号窗口结构;
	ACTION *称号结构[5];
	ACTION * 当前称号结构;
	int 当前称号图档;
	BOOL 窗口显示;
	int 图档[100];
	int 称号索引[100];
	char 介绍[100][32];
	int 当前页数;
	BOOL 确定窗口显示;
	int 确定图档索引;
}称号数据结构;

称号数据结构 称号;

void 结束称号展示动画()
{
	int i=0;
	for(i;i<5;i++){
		if(称号.称号结构[i]){
			if(称号.称号结构[i]) DeathAction(称号.称号结构[i]);
			称号.称号结构[i]=0;
		}					
	}
}

void 打开称号窗口初始化()
{
	if(称号.窗口显示){
		结束称号展示动画();
		称号.窗口显示=FALSE;
		称号.确定窗口显示=FALSE;
	}else{
		称号.当前页数=0;
		称号.窗口显示=TRUE;
		称号.确定窗口显示=FALSE;
	}
}


void 称号展示()
{

	if(称号.当前称号图档)
		if(!称号.当前称号结构){
			if(称号.当前称号图档 < 100000){
				称号.当前称号结构 = 0;
				StockDispBuffer(0x7F,0x40,109,称号.当前称号图档,0);
			}else{
				称号.当前称号结构 = MakeAnimDisp(0x7F,0x59,称号.当前称号图档,0);
			}
		}else if(*(int *)(称号.当前称号结构+0x140)!= 称号.当前称号图档){
			DeathAction(称号.当前称号结构);
			if(称号.当前称号图档 < 100000){
				称号.当前称号结构 = 0;
				StockDispBuffer(0x7F,0x40,109,称号.当前称号图档,0);
			}else{
				称号.当前称号结构 = MakeAnimDisp(0x7F,0x59,称号.当前称号图档,0);
			}
		}
}

BOOL 人物称号处理(ACTION * pct)
{
	int x,y;
	x = pct->x;
	y = pct->y + 133;
	int 按钮左=-1;
	int 按钮右=-1;
	int 按钮关闭=-1;
	int 按钮ID;
	按钮左 = StockDispBuffer( x -55, y + 7, DISP_PRIO_IME3, 55250, 2 );
	按钮右 = StockDispBuffer( x -20, y + 7, DISP_PRIO_IME3, 55251, 2 );
	按钮关闭 = StockDispBuffer( x+40, y + 7, DISP_PRIO_IME3, 55252, 2 );
	if( mouse.onceState & MOUSE_LEFT_CRICK ){
		if(!称号.确定窗口显示){
			if(按钮左 == HitDispNo){
				if(称号.当前页数){
					称号.当前页数--;
					结束称号展示动画();
				}
			}
			if(按钮右 == HitDispNo){
				if(称号.当前页数!=19 && 称号.图档[(称号.当前页数+1)*5]){
					称号.当前页数++;
					结束称号展示动画();
				}
			}
			if(按钮关闭 == HitDispNo){
				DeathAction(pct);
				结束称号展示动画();
				称号.窗口显示=FALSE;
				称号.确定窗口显示=FALSE;
				return TRUE;
			}
		}
	}

	int i=0;
	int 称号图档索引;
	for(;i<5;i++){
		称号图档索引 = 称号.当前页数*5+i;
		if(称号.图档[称号图档索引]){
			if(!称号.称号结构[i]){
				if(称号.图档[称号图档索引] < 100000){
					称号.称号结构[i] = 0;
					StockDispBuffer(0x16D+20,0x35+i*0x20,109,称号.图档[称号图档索引],0);
				}else{
					称号.称号结构[i] = MakeAnimDisp(0x16D+20,0x35+i*0x20,称号.图档[称号图档索引],0);
				}
			}else if(称号.当前称号结构==0) StockDispBuffer(0x16D,0x35+i*0x20,109,称号.图档[称号图档索引],0);
			char 称号序号[128];
			sprintf_s(称号序号,"%03d",称号图档索引+1);

			StockFontBufferExt(0x130,0x2D+i*0x20,1,FONT_PAL_GREEN,称号序号,0,16);
			if(!称号.确定窗口显示){
				if(MakeHitBox(0x124,0x26+i*0x20,0x1B8,0x44+i*0x20,0x6D)==1){
					StockFontBuffer(0x123,0xD1,2,0,称号.介绍[称号图档索引],0);
					if(mouse.onceState & MOUSE_LEFT_CRICK>0){
						称号.确定图档索引=称号.称号索引[ 称号图档索引];
						称号.确定窗口显示=TRUE;
					}
				}
			}
		}else break;
	}
	if(称号.确定窗口显示){
		StockFontBuffer(0x123,0xD1,2,0,称号.介绍[称号.确定图档索引],0);
		StockFontBuffer(0x130,0x118,2,0,"确定使用该称号吗？",0);
		StockDispBuffer(0x16D,0x130,0x6C,55253,1);//确定窗口底图
		按钮ID = StockDispBuffer(0x194,0x148,109,55252,2);//关闭
		if(mouse.onceState & MOUSE_LEFT_CRICK>0){
			if(按钮ID == HitDispNo){
				称号.确定窗口显示=FALSE;
				称号.确定图档索引=0;
			}
		}
		按钮ID = StockDispBuffer(0x148,0x148,109,55254,2);//确定
		if(mouse.onceState & MOUSE_LEFT_CRICK>0){
			if(按钮ID == HitDispNo){
				int checksum=0;
				char buf[1024*4];
				memset(buf,0,1024*4);
				checksum += util_mkint(buf, 称号.确定图档索引);
				util_mkint(buf, checksum);
				util_SendMesg(sockfd, LSSPROTO_CHARTITLE_SEND, buf);
				称号.确定窗口显示=FALSE;
				称号.确定图档索引=0;
			}
		}
	}
	return FALSE;
}

void 窗口结束处理(ACTION* ID)//结束显示动画 
{
	称号.窗口显示=FALSE;
	结束称号展示动画();
	if(称号.当前称号结构) DeathAction(称号.当前称号结构);
	称号.当前称号结构=0;
	DeathAction(ID);
}

void 人物称号数据更新(char *数据)
{
	char 基础内容[126],图档内容[1024],图档索引[1024],内容介绍[4000],临时[126];
	memset(基础内容,0,126);
	memset(图档内容,0,1024);
	memset(图档索引,0,1024);
	memset(内容介绍,0,4000);
	getStringFromIndexWithDelim_body((char *) 数据,(char *)"#@",1,基础内容,126);
	getStringFromIndexWithDelim_body((char *) 基础内容,(char *)"|",1,临时,126);
	int 值 = atoi(临时);
	if(值) 称号.当前称号图档=值;
	getStringFromIndexWithDelim_body((char *) 基础内容,(char *)"|",2,临时,126);
	int 数量 = atoi(临时);
	if(数量!=0){
		memset(&称号.图档,0,sizeof(int)*100);
		memset(&称号.称号索引,0,sizeof(int)*100);
		memset(&称号.介绍,0,32*100);
		getStringFromIndexWithDelim_body((char *) 数据,(char *)"#@",2,图档内容,1024);
		getStringFromIndexWithDelim_body((char *) 数据,(char *)"#@",3,内容介绍,4000);
		getStringFromIndexWithDelim_body((char *) 数据,(char *)"#@",4,图档索引,1024);
		int i=0;
		for(i;i<数量;i++){
			getStringFromIndexWithDelim_body((char *)图档内容,(char *)"|",i+1,临时,126);
			称号.图档[i]=atoi(临时);
			getStringFromIndexWithDelim_body((char *)图档索引,(char *)"|",i+1,临时,126);
			称号.称号索引[i]=atoi(临时);
			getStringFromIndexWithDelim_body((char *)内容介绍,(char *)"|",i+1,临时,126);
			sprintf_s(称号.介绍[i],32,"%s",临时);
		}
	}
}

void 初始化称号结构()
{
	memset(&称号,0,sizeof(称号数据结构));
}
#endif


