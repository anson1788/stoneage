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
#include "../systeminc/VMProtectSDK.h"
int getfilesize(FILE* fp)
{
	if( fp == NULL) return 0;
	int size;
	fseek(fp,0,SEEK_END);
	size = ftell(fp);
	fseek(fp,0,SEEK_SET);
	return size;
}
#ifdef _NEWSHOP_

void 商城结算(short x,short y,UCHAR 层次);
void 清除商城动作();
void 购物车商品删除(short 索引);

extern void getStrSplit( char *, char *, int, int, int );
void StockFontBufferSplit(int x, int y, char unk1, int color, char* data, int unk2,int strLen){

	char token[310]={0};
	getStrSplit(token,(char *)data,0x64,3,strLen);
	char * b = token;
	int c = 20;
	int d = 0;
	do{
		if(strlen(b)!=1) StockFontBuffer(x,y+c*d,unk1,color,b,unk2);
		++d;
		b+=100;
	}while(d < 3);
}

typedef int                 BOOL;
extern int indexWN;
extern int idWN;
extern int cloasewindows;
typedef struct {
	char 商品名[32];
	unsigned int 商品形像;
	unsigned int 详细商品形像;
	unsigned int 商品价格;
	char 商品说明[500];
	short 序号;

}商品数据结构;


商城文件_ 商城文件[5];

typedef struct {
  short 当前;
  short 终点;
} 滚动条结构;

typedef struct {
	BOOL 使用;
	unsigned int 数量;
	商品数据结构 * 商品;
}购物车商品结构;

typedef struct {
  购物车商品结构 购物栏[15];
  short 商品总数;
  short 商品类型个数;
  short 当前页数;
  short 总页数;
  unsigned int 总价格;
} 购物车结构;

typedef struct {
  滚动条结构 滚动条;
  购物车结构 购物车;
  short 道具空;
  short 窗口ID;
  short 提示类型;
  short 商品数量;
  BOOL 结算窗口;
  unsigned int 玩家彩币;
  ACTION * 宠物动作结构;
  ACTION * 窗口动作结构;
  short 宽;
  short 高;
  short X;
  short Y;
  short 宠物位置;
  BOOL 宠物购物栏状态;
} 商城结构;



商品数据结构 商品数据[200];
商城结构 商城数据;

short 商城道具坐标[][2] = {
							{90,100},
							{245,100},
							{90,193},
							{245,193},
							{90,282},
							{242,282},
							{90,369},
							{242,369}
						  };

short 道具内容坐标[][2]={
							{0,5},
							{1,9},
							{2,11},
							{3,14},
							{4,17},
							{5,19}
						 };

unsigned short 窗口显示[][4] ={
								{40041,40046,40066,35},
								{40042,40047,40067,106},
								{40043,40048,40068,176},
								{40044,40049,40069,246},
								{40045,40050,40070,316},
							   }; 

void 在线商城数据初始化()
{
	memset(&商品数据,0,sizeof(商品数据结构)*200);
	memset(&商城数据,0,sizeof(商城结构));
}

int 字符串切割( char *arr[], char *str, const char *del)//字符分割函数的简单定义搜索和实现
{
	int num = 0;
	char *s =NULL;
	char *p =NULL;
	s=strtok_s(str,del,&p); 
	while(s != NULL) {
		arr[num] = s;
		s = strtok_s(NULL,del,&p);
		num++;
	}
	num--;
	return num;
}


void 在线商城窗口初始化(char * 内容)
{
	if(商城数据.窗口动作结构) DeathAction(商城数据.窗口动作结构);
	商城数据.窗口动作结构=0;
	if(商城数据.宠物动作结构) DeathAction(商城数据.宠物动作结构);
	商城数据.宠物动作结构=0;
	商城数据.宠物位置=-1;
	在线商城数据初始化();
	商城数据.道具空 = getIntegerToken(内容,'|',1);
	商城数据.玩家彩币 = getIntegerToken(内容,'|',2);
	商城数据.窗口ID = getIntegerToken(内容,'|',3);
	商城数据.滚动条.当前=1;
	if(商城文件[商城数据.窗口ID-1].商城数据!=NULL){
		int strlena=strlen(商城文件[商城数据.窗口ID-1].商城数据);
		char *临时商城内容 = new char [strlena+1];
		memset(临时商城内容,0,strlena+1);
		memcpy(临时商城内容,商城文件[商城数据.窗口ID-1].商城数据,strlena);
		char *临时全部道具数据[200] ={0};
		char *临时单个道具数据[5]={0};
		商城数据.商品数量 = 字符串切割(临时全部道具数据,临时商城内容,"#");
		int i=0;
		for(;i<=商城数据.商品数量;i++){
			字符串切割(临时单个道具数据,临时全部道具数据[i],"|");
			sprintf_s(商品数据[i].商品名,"%s",临时单个道具数据[0]);
			商品数据[i].商品形像 = atoi( 临时单个道具数据[1]);
			商品数据[i].详细商品形像 = atoi( 临时单个道具数据[2]);
			商品数据[i].商品价格 =  atoi( 临时单个道具数据[3]);
			sprintf_s(商品数据[i].商品说明,"%s",临时单个道具数据[4]);
			商品数据[i].商品名[strlen(商品数据[i].商品名)]=0;
			商品数据[i].商品说明[strlen(商品数据[i].商品说明)]=0;
			商品数据[i].序号=i;
		}
		商城数据.商品数量++;
		if(商城数据.商品数量 > 8)
			商城数据.滚动条.终点= 商城数据.商品数量%8 ? 商城数据.商品数量/8+1 : 商城数据.商品数量/8;
		else 商城数据.滚动条.终点= 1;
		商城数据.宠物位置=-1;
		商城数据.宠物购物栏状态=FALSE;
		delete 临时商城内容;
	}
	商城数据.宽=628;
	商城数据.高=386;
	商城数据.X=(800-商城数据.宽)/2;
	商城数据.Y=(600-商城数据.高)/2-40;
}


ACTION* 创建商城窗口()
{
	
	if(商城数据.窗口动作结构) DeathAction(商城数据.窗口动作结构);
	商城数据.窗口动作结构 = MakeWindowDisp(商城数据.X,商城数据.Y,0,0,窗口显示[商城数据.窗口ID-1][0],0,FALSE);
	play_se(202,320,240);
	return 商城数据.窗口动作结构;
}



ACTION* 商城数据显示()
{
	if(商城数据.窗口ID!=5){
		if(商城数据.宠物动作结构){
			DeathAction(商城数据.宠物动作结构);
			商城数据.宠物动作结构=0;
			商城数据.宠物位置=-1;
		}
	}

	int 按钮ID;
	StockDispBuffer(商城数据.X,商城数据.Y+28,104,窗口显示[商城数据.窗口ID-1][0],1);
	//类型按钮
	int i=0;
	for(;i<5;i++){
		if(i!=(商城数据.窗口ID-1)){
			if(MakeHitBox(商城数据.X+1,商城数据.Y+窗口显示[i][3],商城数据.X+29,商城数据.Y+窗口显示[i][3]+69,-1)==1){
				if(!商城数据.结算窗口){
					StockDispBuffer(商城数据.X+1,商城数据.Y+窗口显示[i][3],106,窗口显示[i][2],1);
					if( (mouse.onceState & MOUSE_LEFT_CRICK)){
						char 临时内容[10]={0};
						sprintf_s(临时内容,"%d",i+1);
						lssproto_WN_send(sockfd,nowGx,nowGy,indexWN,idWN,0,临时内容);
						windowTypeWN = -1;
						wnCloseFlag = 1;
					}else StockDispBuffer(商城数据.X+1,商城数据.Y+窗口显示[i][3],106,窗口显示[i][1],1);
				}
			}else StockDispBuffer(商城数据.X+1,商城数据.Y+窗口显示[i][3],106,窗口显示[i][1],1);
		}
	}


	//关闭按钮
	if(MakeHitBox(商城数据.X+595,商城数据.Y+27,商城数据.X+626,商城数据.Y+53,-1)==1){
		StockDispBuffer(商城数据.X+594,商城数据.Y+27,105,40054,1);
		if( (mouse.onceState & MOUSE_LEFT_CRICK)){
			cloasewindows = 1;
		}
	}else StockDispBuffer(商城数据.X+595,商城数据.Y+27,105,40061,1);
	if(商城数据.结算窗口) 商城结算(商城数据.X+130,商城数据.Y+140,110);
	
	//结算按钮

	if(MakeHitBox(商城数据.X+534,商城数据.Y+372,商城数据.X+591,商城数据.Y+391,-1)==1){
		if(!商城数据.结算窗口){
			StockDispBuffer(商城数据.X+534,商城数据.Y+372,105,40072,1);
			if( (mouse.onceState & MOUSE_LEFT_CRICK)){
				if(商城数据.购物车.商品总数){
					商城数据.提示类型=1;
					商城数据.结算窗口=TRUE;
				}else{
					商城数据.提示类型=2;
					商城数据.结算窗口=TRUE;
				}
			}
		}else StockDispBuffer(商城数据.X+534,商城数据.Y+372,105,40058,1);
	}else StockDispBuffer(商城数据.X+534,商城数据.Y+372,105,40058,1);

	//清空功能
	if(MakeHitBox(商城数据.X+435,商城数据.Y+372,商城数据.X+435+58,商城数据.Y+372+19,-1)==1){
		if(!商城数据.结算窗口){
			StockDispBuffer(商城数据.X+435,商城数据.Y+372,105,40059,1);
			if( (mouse.onceState & MOUSE_LEFT_CRICK)){
				商城数据.购物车.当前页数=0;
				商城数据.购物车.总页数=0;
				商城数据.购物车.商品类型个数=0;
				商城数据.购物车.商品总数=0;
				商城数据.购物车.总价格=0;
				商城数据.购物车.总页数=0;
				memset(商城数据.购物车.购物栏,0,sizeof(购物车商品结构)*15);
			}
		}else StockDispBuffer(商城数据.X+435,商城数据.Y+372,105,40073,1);
	}else StockDispBuffer(商城数据.X+435,商城数据.Y+372,105,40073,1);

	
	//商品上页
	if(MakeHitBox(商城数据.X+379,商城数据.Y+39,商城数据.X+405,商城数据.Y+66,-1)==1){
		if(!商城数据.结算窗口){
			StockDispBuffer(商城数据.X+379,商城数据.Y+39,105,40055,1);
			if( (mouse.onceState & MOUSE_LEFT_CRICK)){
				if(商城数据.滚动条.当前>1) 商城数据.滚动条.当前-=1;
			}
		}else StockDispBuffer(商城数据.X+379,商城数据.Y+39,105,40062,1);
	}else StockDispBuffer(商城数据.X+379,商城数据.Y+39,105,40062,1);

	//商品下页
	if(MakeHitBox(商城数据.X+379,商城数据.Y+370,商城数据.X+405,商城数据.Y+397,-1)==1){
		if(!商城数据.结算窗口){
			StockDispBuffer(商城数据.X+379,商城数据.Y+370,105,40056,1);
			if( (mouse.onceState & MOUSE_LEFT_CRICK)){
				if(商城数据.滚动条.当前<商城数据.滚动条.终点) 商城数据.滚动条.当前+=1;
			}
		}else StockDispBuffer(商城数据.X+379,商城数据.Y+370,105,40063,1);
	}else StockDispBuffer(商城数据.X+379,商城数据.Y+370,105,40063,1);

	//printf("当前=%d  结束=%d\n",商城数据.滚动条.当前,商城数据.滚动条.终点);
	short 滚动条块位置=0;
	if(商城数据.滚动条.当前==1 || 商城数据.滚动条.当前==0) 滚动条块位置=75;
	else if(商城数据.滚动条.当前==商城数据.滚动条.终点) 滚动条块位置=361;
	else 滚动条块位置=(361-75)/(商城数据.滚动条.终点-1)*(商城数据.滚动条.当前-1)+75;
	StockDispBuffer(商城数据.X+391,商城数据.Y+滚动条块位置,105,26447,1);
	
	//商城数据显示
	
	if(商城数据.商品数量>0){
		short 开始,结束;
		开始 = (商城数据.滚动条.当前-1)*8+1;
		结束 = 开始+7;
		if(结束 >商城数据.商品数量) 结束 = 商城数据.商品数量;
		short 临时位置=0;
		BOOL 宠物判断=FALSE;
		i=开始;
		for(;i<=结束;i++){
			临时位置++;
			按钮ID = StockDispBuffer(商城数据.X+商城道具坐标[临时位置-1][0],商城数据.Y+商城道具坐标[临时位置-1][1],106,商品数据[i-1].商品形像,1);
			if(HitDispNo==按钮ID){
				if(!商城数据.结算窗口){
					if(商城数据.宠物动作结构){
						if(商城数据.宠物位置!=按钮ID){
							DeathAction(商城数据.宠物动作结构);
							商城数据.宠物动作结构=0;
							商城数据.宠物位置=-1;
						}
					}
					显示道具数据(商城数据.X+商城道具坐标[临时位置-1][0],商城数据.Y+商城道具坐标[临时位置-1][1],109,商品数据[i-1].商品说明,商品数据[i-1].详细商品形像,商品数据[i-1].商品名);
					宠物判断=TRUE;
					if(商城数据.宠物动作结构) 商城数据.宠物位置=按钮ID;
				}else{
					if(商城数据.宠物动作结构){
						DeathAction(商城数据.宠物动作结构);
						商城数据.宠物动作结构=0;
						商城数据.宠物位置=-1;
					}
				}
			}
			//购买按钮
			if(MakeHitBox(商城数据.X+商城道具坐标[临时位置-1][0]+40,商城数据.Y+商城道具坐标[临时位置-1][1],商城数据.X+商城道具坐标[临时位置-1][0]+40+59,商城数据.Y+商城道具坐标[临时位置-1][1]+22,-1)==1){
				if(!商城数据.结算窗口){
					StockDispBuffer(商城数据.X+商城道具坐标[临时位置-1][0]+40,商城数据.Y+商城道具坐标[临时位置-1][1],105,40071,1);
					if( (mouse.onceState & MOUSE_LEFT_CRICK)){
						if(商城数据.玩家彩币 < 商城数据.购物车.总价格 + 商品数据[i-1].商品价格){
							StockChatBufferLine("系统：身上的金币不足。",6);
							break;
						}
						if(商城数据.道具空 < 商城数据.购物车.商品总数+1){
							StockChatBufferLine("系统：身上的道具位不足。",6);
							break;
						}
						if(!商城数据.购物车.商品类型个数){
							memset(&商城数据.购物车.购物栏,0,sizeof(购物车商品结构)*15);
							商城数据.购物车.购物栏[0].使用=TRUE;
							商城数据.购物车.购物栏[0].数量=1;
							商城数据.购物车.当前页数=1;
							商城数据.购物车.购物栏[0].商品=&商品数据[i-1];
							商城数据.购物车.商品类型个数++;
							商城数据.购物车.商品总数++;
							商城数据.购物车.总价格+=商城数据.购物车.购物栏[0].商品->商品价格;
						}else{
							BOOL 判断是否已经购买了=FALSE;
							int k=0;
							for(;k<15;k++){
								if(商城数据.购物车.购物栏[k].使用){
									//printf("地址1=%x   地址2=%x\n",商城数据.购物车.购物栏[0].商品,&商品数据[i-1]);
									if(商城数据.购物车.购物栏[k].商品 == &商品数据[i-1]){
										判断是否已经购买了=TRUE;
										商城数据.购物车.购物栏[k].数量++;
										商城数据.购物车.商品总数++;
										商城数据.购物车.总价格+=商城数据.购物车.购物栏[k].商品->商品价格;
										break;
									}
								}
							}
							if(!判断是否已经购买了){
								if(商城数据.购物车.购物栏[0].使用){
									购物车商品结构 临时购物车数据1;
									购物车商品结构 临时购物车数据2;
									临时购物车数据1.商品 = 商城数据.购物车.购物栏[0].商品;
									临时购物车数据1.数量 = 商城数据.购物车.购物栏[0].数量;
									k=1;
									for(;k<15;k++){
										if(商城数据.购物车.购物栏[k].使用){
											临时购物车数据2.商品 = 商城数据.购物车.购物栏[k].商品;
											临时购物车数据2.数量 = 商城数据.购物车.购物栏[k].数量;
											商城数据.购物车.购物栏[k].商品=临时购物车数据1.商品;
											商城数据.购物车.购物栏[k].数量=临时购物车数据1.数量;
											临时购物车数据1.商品=临时购物车数据2.商品;
											临时购物车数据1.数量=临时购物车数据2.数量;
										}else{
											商城数据.购物车.购物栏[k].使用 = TRUE;
											商城数据.购物车.购物栏[k].商品=临时购物车数据1.商品;
											商城数据.购物车.购物栏[k].数量=临时购物车数据1.数量;
											break;
										}
									}
								}
								商城数据.购物车.购物栏[0].商品 = &商品数据[i-1];
								商城数据.购物车.购物栏[0].数量 = 1;
								商城数据.购物车.购物栏[0].使用 = TRUE;
								商城数据.购物车.商品类型个数++;
								商城数据.购物车.商品总数++;
								商城数据.购物车.总价格+=商城数据.购物车.购物栏[0].商品->商品价格;
							}
						}
					//	printf("商城数据.购物车.商品类型个数%3   = %d\n",商城数据.购物车.商品类型个数%3);
						商城数据.购物车.总页数 = 商城数据.购物车.商品类型个数%3 ? 商城数据.购物车.商品类型个数/3+1 : 商城数据.购物车.商品类型个数/3;
						if(商城数据.购物车.总页数*3-2>商城数据.购物车.商品类型个数) 商城数据.购物车.总页数--;
						if(!商城数据.购物车.总页数) 商城数据.购物车.总页数=1;
					}
				}else StockDispBuffer(商城数据.X+商城道具坐标[临时位置-1][0]+40,商城数据.Y+商城道具坐标[临时位置-1][1],105,40057,1);
			}else StockDispBuffer(商城数据.X+商城道具坐标[临时位置-1][0]+40,商城数据.Y+商城道具坐标[临时位置-1][1],105,40057,1);
			char 临时内容[128]={0};
			sprintf_s(临时内容,"%-5d金币",商品数据[i-1].商品价格);
			StockFontBuffer(商城数据.X+商城道具坐标[临时位置-1][0]+15,商城数据.Y+商城道具坐标[临时位置-1][1]-45,1,0,商品数据[i-1].商品名,0);
			StockFontBuffer(商城数据.X+商城道具坐标[临时位置-1][0]+38,商城数据.Y+商城道具坐标[临时位置-1][1]-20,1,4,临时内容,0);
		}
		if(!商城数据.宠物购物栏状态 && !宠物判断){
			if(商城数据.宠物动作结构){
				DeathAction(商城数据.宠物动作结构);
				商城数据.宠物动作结构=0;
				商城数据.宠物位置=-1;
			}
		}
	}
	//购物车处理
	if(商城数据.购物车.商品总数>0){
		short 开始,结束;
		开始 = (商城数据.购物车.当前页数-1)*3+1;
		结束 = 商城数据.购物车.当前页数*3;
		if(结束>商城数据.购物车.商品类型个数) 结束 = 商城数据.购物车.商品类型个数;
		short 临时位置 =0;
		BOOL 购物判断 = FALSE;
		int i=开始;
		for(;i<=结束;i++){
			临时位置++;
			
			StockDispBuffer(商城数据.X+412,商城数据.Y+67+((临时位置-1)*80),105,40053,1);
			//购物车左页
			if(MakeHitBox(商城数据.X+505,商城数据.Y+121+((临时位置-1)*80),商城数据.X+505+31,商城数据.Y+121+((临时位置-1)*80)+16,-1)==1){
				if(!商城数据.结算窗口){
					StockDispBuffer(商城数据.X+505,商城数据.Y+121+((临时位置-1)*80),106,40064,1);
					if( (mouse.onceState & MOUSE_LEFT_CRICK)){
						商城数据.购物车.总价格-= 商城数据.购物车.购物栏[i-1].商品->商品价格;
						商城数据.购物车.商品总数-=1;
						if(商城数据.购物车.购物栏[i-1].数量==1){
							购物车商品删除(i-1);
							商城数据.购物车.商品类型个数--;
							商城数据.购物车.总页数  = 商城数据.购物车.商品类型个数%3 ? 商城数据.购物车.商品类型个数/3+1 : 商城数据.购物车.商品类型个数/3;
							if(商城数据.购物车.总页数*3>商城数据.购物车.商品类型个数) 商城数据.购物车.总页数--;
							if(!商城数据.购物车.总页数&&商城数据.购物车.商品类型个数) 商城数据.购物车.总页数=1;
							if(商城数据.购物车.当前页数>商城数据.购物车.总页数) 商城数据.购物车.当前页数 = 商城数据.购物车.总页数;
							if(!商城数据.购物车.商品类型个数) 
							{
								商城数据.购物车.当前页数=0;
								商城数据.购物车.总页数=0;
							}
							break;
						}else 商城数据.购物车.购物栏[i-1].数量-=1;
					}
				}else StockDispBuffer(商城数据.X+505,商城数据.Y+121+((临时位置-1)*80),106,40051,1);
			}else StockDispBuffer(商城数据.X+505,商城数据.Y+121+((临时位置-1)*80),106,40051,1);



			if(MakeHitBox(商城数据.X+571,商城数据.Y+121+((临时位置-1)*80),商城数据.X+571+31,商城数据.Y+121+((临时位置-1)*80)+16,-1)==1){
				if(!商城数据.结算窗口){
					StockDispBuffer(商城数据.X+571,商城数据.Y+121+((临时位置-1)*80),106,40065,1);
					if( (mouse.onceState & MOUSE_LEFT_CRICK)){
						if(商城数据.玩家彩币 < 商城数据.购物车.总价格 + 商城数据.购物车.购物栏[i-1].商品->商品价格){
							StockChatBufferLine("系统：身上的金币不足。",6);
							break;
						}
						if(商城数据.道具空 < 商城数据.购物车.商品总数+1){
							StockChatBufferLine("系统：身上的道具位不足。",6);
							break;
						}
						商城数据.购物车.商品总数++;
						商城数据.购物车.总价格+=商城数据.购物车.购物栏[i-1].商品->商品价格;
						商城数据.购物车.购物栏[i-1].数量++;
					}
				}else StockDispBuffer(商城数据.X+571,商城数据.Y+121+((临时位置-1)*80),106,40052,1);
			}else StockDispBuffer(商城数据.X+571,商城数据.Y+121+((临时位置-1)*80),106,40052,1);
			按钮ID = StockDispBuffer(商城数据.X+455,商城数据.Y+105+((临时位置-1)*80),106,商城数据.购物车.购物栏[i-1].商品->商品形像,1);

			if(HitDispNo==按钮ID){
				if(!商城数据.结算窗口){
					if(商城数据.宠物动作结构){
						if(商城数据.宠物位置!=按钮ID){
							DeathAction(商城数据.宠物动作结构);
							商城数据.宠物动作结构=0;
							商城数据.宠物位置=-1;
						}
					}
					显示道具数据(商城数据.X+455,商城数据.Y+105+((临时位置-1)*80),109,商城数据.购物车.购物栏[i-1].商品->商品说明,商城数据.购物车.购物栏[i-1].商品->详细商品形像,商城数据.购物车.购物栏[i-1].商品->商品名);
					购物判断=TRUE;
					if(商城数据.宠物动作结构) 商城数据.宠物位置=按钮ID;
				}else if(商城数据.宠物动作结构){
					DeathAction(商城数据.宠物动作结构);
					商城数据.宠物动作结构=0;
					商城数据.宠物位置=-1;
				}
			}
			char 临时内容[128]={0};
			sprintf_s(临时内容,"%-5d金币",商城数据.购物车.购物栏[i-1].商品->商品价格);
			StockFontBuffer(商城数据.X+500,商城数据.Y+80+((临时位置-1)*80),1,0,商城数据.购物车.购物栏[i-1].商品->商品名,0);
			StockFontBuffer(商城数据.X+535,商城数据.Y+100+((临时位置-1)*80),1,4,临时内容,0);
			sprintf_s(临时内容,"%d",商城数据.购物车.购物栏[i-1].数量);
			临时内容[strlen(临时内容)]=0;
			StockFontBuffer(商城数据.X+549,商城数据.Y+121+((临时位置-1)*80),1,4,临时内容,0);
		}
		if(购物判断) 商城数据.宠物购物栏状态=TRUE;
		else 商城数据.宠物购物栏状态 = FALSE;
//购物车按钮左页
		if(MakeHitBox(商城数据.X+420,商城数据.Y+312,商城数据.X+420+31,商城数据.Y+312+16,-1)==1){
			if(!商城数据.结算窗口){
				StockDispBuffer(商城数据.X+420,商城数据.Y+312,105,40064,1);
				if( (mouse.onceState & MOUSE_LEFT_CRICK)){
					if(商城数据.购物车.当前页数!=1) 商城数据.购物车.当前页数-=1;
				}
			}else StockDispBuffer(商城数据.X+420,商城数据.Y+312,105,40051,1);
		}else StockDispBuffer(商城数据.X+420,商城数据.Y+312,105,40051,1);
//购物车按钮右页
		if(MakeHitBox(商城数据.X+575,商城数据.Y+312,商城数据.X+575+31,商城数据.Y+312+16,-1)==1){
			if(!商城数据.结算窗口){
				StockDispBuffer(商城数据.X+575,商城数据.Y+312,105,40065,1);
				if( (mouse.onceState & MOUSE_LEFT_CRICK)){
					if(商城数据.购物车.当前页数!=商城数据.购物车.总页数) 商城数据.购物车.当前页数+=1;
				}
			}else StockDispBuffer(商城数据.X+575,商城数据.Y+312,105,40052,1);
		}else StockDispBuffer(商城数据.X+575,商城数据.Y+312,105,40052,1);
	}
	char 临时内容[20]={0};
	sprintf_s(临时内容,"%-3d/ %-3d",商城数据.购物车.当前页数,商城数据.购物车.总页数);
	StockFontBuffer(商城数据.X+490,商城数据.Y+314,1,0,临时内容,0);
	memset(临时内容,0,20);
	sprintf_s(临时内容,"%d",商城数据.玩家彩币);
	StockFontBuffer(商城数据.X+462,商城数据.Y+342,1,0,临时内容,0);
	memset(临时内容,0,20);
	sprintf_s(临时内容,"%d",商城数据.购物车.总价格);
	StockFontBuffer(商城数据.X+573,商城数据.Y+342,1,0,临时内容,0);
	
	if( CheckMenuFlag()
		|| ((joy_trg[ 0 ] & JOY_ESC) && GetImeString() == NULL)
		|| actBtn == 1
		|| menuBtn == 1
		|| disconnectServerFlag == TRUE
		|| wnCloseFlag == 1 
		|| cloasewindows == 1)
	{
		wnCloseFlag = 0;
		cloasewindows = 0;
		清除商城动作();
	}
	return 商城数据.窗口动作结构;
}



void 清除商城动作()
{
	if(商城数据.窗口动作结构) DeathAction(商城数据.窗口动作结构);
	商城数据.窗口动作结构=0;
	if(商城数据.宠物动作结构) DeathAction(商城数据.宠物动作结构);
	商城数据.宠物动作结构=0;
	商城数据.宠物位置=-1;
	wnCloseFlag=0;
	windowTypeWN = -1;
	wnCloseFlag = 1;
}

void 商城结算(short x,short y,UCHAR 层次)
{
	if(商城数据.提示类型==1){
		StockDispBuffer(x,y,层次,40088,1);
		StockFontBuffer(x+110,y+50,2,0,"您确定需要购买这些商品吗？",0);
		 if(MakeHitBox(x+60,y+100,x+60+105,y+100+18,-1)==1){
			StockDispBuffer(x+60,y+100,层次+1,40084,1);
			if( (mouse.onceState & MOUSE_LEFT_CRICK)){
				char 内容[256]={0};
				int i=0;
				for(;i<15;i++){
					if(商城数据.购物车.购物栏[i].使用){
						char 临时[15]={0};
						sprintf_s(临时,"%d|%d#",商城数据.购物车.购物栏[i].商品->序号+1,商城数据.购物车.购物栏[i].数量);
						strcat_s(内容,临时);
					}
				}
				lssproto_WN_send(sockfd,nowGx,nowGy,indexWN,idWN,商城数据.窗口ID,内容);
				windowTypeWN = -1;
				wnCloseFlag = 1;
			}
		 }else StockDispBuffer(x+60,y+100,层次+1,40086,1);
		 if(MakeHitBox(x+220,y+100,x+220+105,y+100+18,-1)==1){
			StockDispBuffer(x+220,y+100,层次+1,40085,1);
			if( (mouse.onceState & MOUSE_LEFT_CRICK)) 商城数据.结算窗口=FALSE;
		 }else StockDispBuffer(x+220,y+100,层次+1,40087,1);
	}else{
		StockDispBuffer(x,y,层次,40088,1);
		StockFontBuffer(x+130,y+50,2,0,"您的购物车没有商品！",0);
		if(MakeHitBox(x+140,y+100,x+140+105,y+100+18,-1)==1){
			StockDispBuffer(x+140,y+100,层次+1,40090,1);
			if( (mouse.onceState & MOUSE_LEFT_CRICK)) 商城数据.结算窗口=FALSE;
		}else StockDispBuffer(x+140,y+100,层次+1,40089,1);
	}
}

void 显示道具数据(short x,short y,UCHAR 层次,char* 内容,unsigned int 形像,char*名字 )
{
	char 临时内容[500]={0};
	sprintf_s(临时内容,"%s",内容);
	char *道具数据[30]={0};
	int 背景张数 = 0;
	if(商城数据.窗口ID!=5){
		背景张数 = 字符串切割(道具数据,临时内容,"\n");
		if(道具数据[背景张数-1][0]==0) 背景张数--;
	}
	int 背景张数1=背景张数;
	int 图片高,图片x,图片y,i=0;
	if(商城数据.窗口ID!=5){
		for(;i<6;i++){
			if(i==5) {
				背景张数 = 道具内容坐标[i][0];
				break;
			}
			if(道具内容坐标[i][1]>=背景张数){
				背景张数 = 道具内容坐标[i][0];
				break;
			}
		}
		图片高 = 背景张数*48+144;
		if((x-190) >(628+商城数据.X-x-65)) 图片x=x-190;
		else  图片x=x+35;
		图片y=y-(图片高/2);
		BOOL 判断=FALSE;
		while(true){
			if(商城数据.Y > 图片y-20){
				图片y +=10;
				判断=TRUE;
			}	
			if((商城数据.Y+386+30) < (图片y+图片高)){
				图片y -=10;
				判断=TRUE;
			}
			if(判断) 判断=FALSE;
			else break;
		}

		StockDispBuffer(图片x+80,图片y+50,层次,40171,1);
		int 高=48;
		if(背景张数 >=1){
			for(i=1;i<=背景张数;i++) StockDispBuffer(图片x+80,图片y+50+高*(i-1)+83,层次,40172,1);
		}
		StockDispBuffer(图片x+80,图片y+50+高*背景张数+83,层次,40173,1);
		int 内容x,内容y;
		内容x=图片x+6-10;
		内容y=图片y-25+50;

		for(i=1;i<=背景张数1+1;i++) StockFontBufferSplit(内容x+15,内容y+((i-1)*20),FONT_PRIO_AFRONT,0,道具数据[i-1],0,20);
	}else{
		图片高 = 1*48+144;
		if((x-190) >(628+商城数据.X-x-65)) 图片x=x-190;
		else  图片x=x+35;
		图片y=y-(图片高/2);
		BOOL 判断=FALSE;
		while(true){
			if(商城数据.Y > 图片y-20){
				图片y +=10;
				判断=TRUE;
			}	
			if((商城数据.Y+386+30) < (图片y+图片高)){
				图片y -=10;
				判断=TRUE;
			}
			if(判断) 判断=FALSE;
			else break;
		}
		StockDispBuffer(图片x+80,图片y+50,层次,40171,1);
		int 高=48;
		StockDispBuffer(图片x+80,图片y+50+83,层次,40173,1);
		if(!商城数据.宠物动作结构) 商城数据.宠物动作结构 = MakeAnimDisp(图片x+76,图片y+80,形像,0);
		StockFontBufferSplit(图片x+15,图片y+100,2,0,临时内容,0,20);
	}
}

void 购物车商品删除(short 索引)
{

	商城数据.购物车.购物栏[索引].商品=0;
	商城数据.购物车.购物栏[索引].数量=0;
	商城数据.购物车.购物栏[索引].使用=FALSE;
	int i = 索引+1;
	if(i>14) return;
	for(i;i<15;i++){
		if(!商城数据.购物车.购物栏[i].使用) break;
		商城数据.购物车.购物栏[索引].商品 = 商城数据.购物车.购物栏[i].商品;
		商城数据.购物车.购物栏[索引].数量 = 商城数据.购物车.购物栏[i].数量;
		商城数据.购物车.购物栏[索引].使用=TRUE;
		商城数据.购物车.购物栏[i].商品=0;
		商城数据.购物车.购物栏[i].数量=0;
		商城数据.购物车.购物栏[i].使用=FALSE;
		索引 = i;
	}
}

void 商城文件读取(char * 路径,商城文件_ *商城)
{
	FILE *fp;
	fopen_s(&fp,路径,"rb");	
	if( fp == NULL )
	{
		商城->MD5码[0]=NULL;
		商城->商城数据=NULL;
		return;
	}
	int size = getfilesize(fp);
	char *临时 = (char*)malloc(size);
	fread(临时,1,size,fp);
	fclose(fp);
	商城->商城数据 = (char*)malloc(size-34+1);
	memset(商城->商城数据,0,size-34+1);
	memset(商城->MD5码,0,64);
	memcpy_s(商城->MD5码,34,临时,34);
	memcpy_s(商城->商城数据,size-34,临时+34,size-34);
	TEA解密((long*)商城->商城数据,((size-34)/4),(long*)_KEY_);
	free(临时);
}


void 商城文件保存(char * 路径,商城文件_ *商城)
{
	FILE *fp;
	fopen_s(&fp,路径,"wb");
	int length=strlen(商城->商城数据);
	int length1 = length%4?length+(4-length%4):length;
	char *临时 = (char*)malloc(34+length1);
	memset(临时,0,34+length1);
	memcpy_s(临时,34,商城->MD5码,34);
	memcpy_s((临时+34),length,商城->商城数据,length);
#ifdef _VMP_
	TEA加密((long *)(临时+34),(long)(length1/4),(long*)VMProtectDecryptStringA(_KEY_));
#else
	TEA加密((long *)(临时+34),(long)(length1/4),(long*)_KEY_);
#endif
    fwrite(临时, length1+34, 1, fp);
	free(临时);
    fclose(fp);
}


void 商城初始化()
{
	商城文件读取("./data/shop1.bin",&商城文件[0]);
	商城文件读取("./data/shop2.bin",&商城文件[1]);
	商城文件读取("./data/shop3.bin",&商城文件[2]);
	商城文件读取("./data/shop4.bin",&商城文件[3]);
	商城文件读取("./data/shop5.bin",&商城文件[4]);
}
#endif














