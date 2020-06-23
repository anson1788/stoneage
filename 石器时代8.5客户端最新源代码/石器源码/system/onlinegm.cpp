//cyg  实作第一个系统 (线上回报系统)
#include"../systeminc/version.h"
#include"../systeminc/system.h"
#include"../systeminc/loadrealbin.h"
#include"../systeminc/loadsprbin.h"
#include"../systeminc/anim_tbl.h"
#include"../systeminc/login.h"
#include"../systeminc/menu.h"
#include"../systeminc/map.h"
#include"../systeminc/lssproto_cli.h"
#include"../systeminc/pc.h"
#include"../systeminc/netmain.h"
#include"../systeminc/handletime.h"
#include"../systeminc/character.h"
#include"../other/caryIme.h"
#include"../systeminc/ime_sa.h"
#include"../systeminc/t_music.h"
#include"../systeminc/netproc.h"
#include"../systeminc/tool.h"
#include "time.h"
#include "../wgs/descrypt.h"
#include"../systeminc/onlinegm.h"

char ProblemClass[DEF_CLASS_TOTAL][16]={
	"异常问题",
	"一般问题"
};
char ProblemC1[DEF_C1_TOTAL][30]={
	"卡人卡石",
    "网页指正",
	"客服产品活动",
	"安装与更新",
	"会员帐号",
	"产品序号",		
	"赠品序号",
	"游戏点数卡购买及储值",
	"包月相关",
	"超级/炫风来吉卡/周边商品",
	"服务器/连线/网路状况反应",
    "游戏操作询问",
	"非法检举投拆",
	"其他"	
};
char ProblemC1Help[DEF_C1_TOTAL][60]={
	"请注明异常帐号、座标、地点、错误讯息、星系",
	"请提供相关连结",
	"请先查询游戏专属网站以及过往客服公告",
	"请尽量留下您的电脑配备",
	"请详述遇到的状况",
	"请提供完整的序号",
	"请提供完整的序号",
	"登入游戏发现扣点不正常，请改选择问题类别为(游戏扣点问题)",
	"",
	"请先查询专属网页相关活动",
	"",
	"请先查询网页FAQ",
	"",
	""
};
char ProblemC2[DEF_C2_TOTAL][16]={
	"人物异常",
	"道具异常",
	"宠物异常",
	"交易问题",
	"疑似盗用",
	"家族问题",
	"宠物＆道具问题",
	"ＷＧＳ扣点问题",
	"其他",  
	"会员升级",
	"星系移民",	
};
//人物异常
char ProblemC2_1[3][16]={
	"人物消失",
	"人物资料异常",
	"其他"
};
//道具异常
char ProblemC2_2[3][16]={
	"道具消失",
	"其他",
	"道具功能异常"
};
//宠物异常
char ProblemC2_3[5][16]={
	"宠物消失",
	"宠物邮件问题",
	"溜宠问题",
	"宠物蛋问题",
	"其他"
};
//交易问题
char ProblemC2_4[2][16]={
	"交易后消失",
	"其他"
};
//疑似盗用
char ProblemC2_5[2][16]={
	"疑似盗用查询",
	"其他"
};
//家族问题
char ProblemC2_6[3][16]={
	"家族被解散",
	"声望异常",
	"其他"
};
//星系移民
char ProblemC2_7[3][16]={
	"道具消失",
	"人物消失",
	"其他"
};

//HELP中使用到的页资料结构
typedef struct PAGE{
	short	desc_index;			//说明结构开始的索引号(此desc_index为根据seg_index在help_index取得DESC的阵列的index)
	int		desc_num;//BYTE	desc_num;			//说明的结构数
}*LPPAGE;

//注意事项
//HELP中使用到说明的文字资料结构
typedef struct DESC{
	BYTE	color;				//颜色
	char	description[60];	//说明文字
}*LPDESC;


// 问题参数储存结构 cyg  
typedef struct{
	char no[20];
	char state[40];	  							
	char okdate[10];
	int  okH;
	char wrongdate[10];
	int  wrongH;
	int	 class0;
	char class1[40];
	char cont[4000];
	char answer[4000];
	char error[2000];
	BOOL delflag;
}PRODATA;


DESC Prohelp_desc1[]={
			//颜色与说明 //fix				
			{ 4 , "1. 使用说明"},
			{ 0 , "		线上留单上限为20笔，若已达上限须先将旧有问题单删除，才能"},
			{ 0 , "		继续留单或至网页留单。异常问题须输入正常日期与错误日期。"},
			{ 0 , "		※ 最新问题单会显示在最上方。"},
			{ 4 , "2. 宠物蛋异常"},
			{ 0 , "		于2003/12/31日服务器更新时，己放置修正宠物蛋设定的程式"},
			{ 0 , "		，即日起客服中心将不再受理玩家宠物蛋能力异常的情况，修正"},
			{ 0 , "		后的宠物蛋设定如下："},
			{ 0 , "		※ 人物在线上喂养设定和以往一样，超过一小时会有能力下"},
			{ 0 , "		   降的情况。人物离线或宠物蛋放置在宠店中，再次登入会"},
			{ 0 , "		   重新计算喂养的时间。"},
			{ 4 , "3. 溜宠消失"},
			{ 0 , "		游戏程式设定己可在同一颗服务器内，与萨村门口的[工程人员]"},						
};

DESC Prohelp_desc2[]={
			{ 0 , "		取回；如无法领取请提供以下资料宠物名称、等级、宠物原持有"},
			{ 0 , "		帐号、宠物持有日期、宠物消失日期。"},
			{ 4 , "4. 宠邮消失"},
			{ 0 , "		游戏程式设定己可在同一颗服务器内，与萨村门口的[工程人员]"},
			{ 0 , "		取回；如无法领取请提供以下资料宠物名称、等级、宠物原持有"},
			{ 0 , "		帐号、宠物持有日期、宠物消失日期。"},
			//{ 4 , "4. 宠物战斗脱逃-无法处理"},
			//{ 0 , "		宠物的忠诚度低于20以下，在战斗中就会跑走。此为原游戏设定"},
			//{ 0 , "		，客服中心不受理回覆宠物。"},
			{ 4 , "5. 交易后物品消失-请提供以下资料"},
			{ 0 , "		交易方的帐号或人物名称、交易日期、交易的宠物名称以及等级"},
			{ 0 , "		、道具名称以及数量。"},
			{ 4 , "6. 宠物或道具卖给NPC-无法处理"},
			{ 0 , "		此为原游戏设定，客服中心不受理回覆宠物或道具。"},
			{ 4 , "7. 宠物或道具丢出后被系统清除-无法处理"},
			{ 0 , "		此为原游戏设定，客服中心不受理回覆宠物或道具；玩家可在游"},			
};

DESC Prohelp_desc3[]={
			{ 0 , "		戏中透过交易介面转移。"},
			{ 4 , "8. 家族异常-请提供以下资料"},
			{ 0 , "		家族异常请族长本人帐号留单，注明家族名称、家族编号、异常"},
			{ 0 , "		状况。如因为家族人数不足，系统自动解散的家族，连同家族银"},
			{ 0 , "		行存款客服中心无法受理。"},
			{ 0 , "		※ 对于家族解散与成员因过久未上线而自动退出家族设定，系"},
			{ 0 , "		   统每天会不定时的扫描、侦测。"},
			{ 0 , "		※ 目前家族成员设定，在退出后7日内无法再加入家族。"},
			{ 4 , "9. 人物DP异常-无法处理	"},
			{ 0 , "		由于DP对于游戏中并不会有影响，并无开放此功能供玩家查询。"},
			{ 4 , "10. 人物名片消失-无法处理"},
			{ 0 , "		由于名片的对应会有一对多的情况(一个人物拥有多位玩家的名"},
			{ 0 , "		片)，若单方面仅处理一名玩家并无法使资料正确，并无开放此"},
};

DESC Prohelp_desc4[]={
			{ 0 , "		功能供玩家查询。"},
			{ 4 , "11. 盗用&诈骗案件-请提供以下资料"},
			{ 0 , "		宠物名称、等级以及道具名称、数量、疑似盗用日期 工程人员"},
			{ 0 , "		可查询人物删除纪录，需请玩家不要创立新人物以便回覆空人物"},
			{ 0 , "		(没有宠物、道具以及石币)。 工程人员仅以玩家提供之道具、"},
			{ 0 , "		宠物进行查询的动作。"},
			{ 4 , "12. 朱雀转生设定  2003/06/21"},
			{ 0 , "		活动赠送之朱雀能力值，于转生后仍有小幅成长空间，但原能力"},
			{ 0 , "		值部份不一定能维持转生前高点，在此提醒玩家要转生朱雀时，"},
			{ 0 , "		请多加考虑。客服中心不受理回覆转生前朱雀之留单案件。"},
};

//DESC Prohelp_desc5[]={
//};

LPDESC Prohelp_Index[] ={
	&Prohelp_desc1[0],
	&Prohelp_desc2[0],
	&Prohelp_desc3[0],
	&Prohelp_desc4[0]
//	&Prohelp_desc5[0]
};

PAGE ProHelp_page[]={
			{ 0 , sizeof(Prohelp_desc1)/sizeof(Prohelp_desc1[0])},
			{ 0 , sizeof(Prohelp_desc2)/sizeof(Prohelp_desc2[0])},
			{ 0 , sizeof(Prohelp_desc3)/sizeof(Prohelp_desc3[0])},
			{ 0 , sizeof(Prohelp_desc4)/sizeof(Prohelp_desc4[0])}
//			{ 0 , sizeof(Prohelp_desc5)/sizeof(Prohelp_desc5[0])}
};


INPUT_HISTORY  InputHistory ;	// 用来记忆每一行的内容，往后才能拿来索引

STR_BUFFER  SubBuffer ;		//  每一行储存的buf

#ifdef __ONLINEGM  // 全关掉

char DataBuffer[500];		//  储存组合好的问题回报一连串的参数
char InputBuffer[8050];		//  储存玩家的输入内容   
char ContentBuf[8050];		//  储存组合好的问题内容

PRODATA  Prodata[100];		//  假设玩家最多可以建100笔

int		PutKey,PushKey ; 

int		ProNoSelect;        //	现在正选择的单号索引
int     HistorySelect;      //  上次选择查询的单号索引
BOOL	ProNoSelectFlag;	//  有没有选单号的旗标
int		ProNoIndex;			//  用来秀问题单号
int		ProNoValue;			//  表示有几笔问题单号

BOOL    TitleFlag ;			
int     SegmentIndex ;		//  一段一段内容的索引
int     SegmentTotal ;		//  纪录总共有几段
char    SegmentBuf[2][2500];//  纪录某笔单号的内容和客服回应

int		okY,okM,okD,okH,wrongY,wrongM,wrongD,wrongH;
int 	ClassIndex , ClassIndex1 , ClassIndex2 ;  //  问题类别索引

BOOL	DeleteBtnFlag;      //要有选到某单号才能删除的旗标
      

//OnlineGm 的Log 专用区 Begin
#ifdef _STONDEBUG_
char    inlogstr[512];
#define filename     "onlinegm.log"
#endif
//OnlineGm 的Log 专用区 End

//线回系统 测试Server IP : 10.1.0.17 

BOOL QueryOnlineGmIP(char HostName[]);
char *OnlineGmIP;	   //使用domain name
//char OnlineGmIP[20];   //使用直接ip
#define SMSDomainName   "ingame.wayi.com.tw"//"sms.hwaei.com.tw"

int     ResultCode;  // 传输产生的侦错码 

static char *pContent=NULL;

BOOL ResetFlag = TRUE;
BOOL DirFlag = TRUE;
DWORD  LastTime = 0;
int  iReturnNo;  //纪录回传的单号

int	 TotalAddValue;   //纪录玩家目前留单总数

int  ProblemType ;  //纪录按传送键时问题单的类型  一般0异常1
int  SendType    ;  //纪录传送给server前的问题类型

unsigned int  ichecklooptime = 0;    //检查连线回圈时间

//*************************************
//	初始资料
//*************************************
void InitOnlineGm( void )
{
	int i;
	
	PutKey = 0;
	PushKey = 0;
	ProNoSelectFlag = FALSE;
	ProNoSelect = 0;
	HistorySelect = 5000;  //无意义 区别用
	ProNoIndex = 0;
	ProNoValue = 0;
	SegmentIndex =0;
	SegmentTotal = 0;
	okY=okM=okD=okH=wrongY=wrongM=wrongD=wrongH=0;
	ClassIndex = 1;
	ClassIndex1 = ClassIndex2 =0;
	
	ResetFlag = TRUE ;
	DirFlag = TRUE ;
	DeleteBtnFlag = FALSE ;

	DataBuffer[0] = '\0';
	InputBuffer[0] = '\0';
	ContentBuf[0] = '\0';

	SegmentBuf[0][0]='\0';
	SegmentBuf[1][0]='\0';

	for( i=0;i<DEF_MAX_ADD_VALUE;i++){
		Prodata[i].no[0] = '\0';
		Prodata[i].state[0] = '\0';
		Prodata[i].okdate[0] = '\0';	
	    Prodata[i].okH = 0;
		Prodata[i].wrongdate[0] = '\0';
		Prodata[i].wrongH = 0;	
		Prodata[i].class0 = 0;	
		Prodata[i].class1[0] = '\0';
		Prodata[i].cont[0] = '\0';
		Prodata[i].answer[0] = '\0';	
		Prodata[i].error[0] = '\0';
		Prodata[i].delflag = FALSE;		
	}
	LastTime = 0;
	iReturnNo = 0;
	TotalAddValue = 0;
	ProblemType = 0;
	SendType = 0;
}

//************************************************
//函式 : 产生Log档(于Local端目录下)的专用函式
//回传值 : None
//附  注 : 只有Debug mode才在硬碟写入log档!!
//************************************************
#ifdef _STONDEBUG_
void WriteInLog( char cData[], char cFile[])
{
	char totals[65536];//[1024];
	struct tm nowTime;
	time_t longTime;
	time( &longTime );
	localtime_s(&nowTime, &longTime );
	sprintf_s(totals,"%02d/%02d/%02d %02d:%02d:%02d ",(nowTime.tm_year % 100), nowTime.tm_mon+1, nowTime.tm_mday,
				nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec);
	lstrcat(totals,cData);
	lstrcat(totals,"\r\n\r\n\r\n");
	FILE *fp;
	fp = fopen(filename, "a" );
	if(fp)
	{
		fprintf(fp,totals);
		fclose(fp);
	}
}
#endif

//************************************************
//函式一 : 处理主程式与ASP间沟通的专用函式
//回传值 :  0 --- 处理完毕 & 问题回报成功
//         1 --- 问题回报发送失败
//         2 --- 手机号码参数错误(时间或类别错误)
//         3 --- 手机讯息参数错误(过长)
//         4 --- 网路端错误
//         5 --- WGS点数(或通数)已不足!!(小于五点)
//         6 --- 连线逾时(或DNS失败)
//         7 --- ASP回传错误
//************************************************
// pid:id ppw:password pdb:内部资料 pinput:玩家输入的内容
int SendProblemMsg(char pid[16], char ppw[16], char pdb[] , char pinput[])
{
	char returndata[20] ;  //纪录回传的字串
	int nRet,wait=1;

	if(!QueryOnlineGmIP(SMSDomainName)) return 6;
	//if((pdb[0] =='\0') || (lstrlen(pdb) != 23)) return 2;    
	//if(lstrlen(pc.name)>16)	return 3;

#ifdef _FIX_URLENCODE
		char OutTmp[8000];
		char *pOutTmp ;
		char *pInTmp ;
		char Hex[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
		//  do  encoding
		pInTmp=pinput;
		pOutTmp=OutTmp;
		while  (*pInTmp)
		{
			if ( (*pInTmp>= '0') && (*pInTmp <= '9') 
				||	(*pInTmp>= 'a') && (*pInTmp <= 'z')
				||	(*pInTmp>= 'A') && (*pInTmp <= 'Z') 
				)
				*pOutTmp++  =  *pInTmp;
			else
			{
				if(*pInTmp==0x20&&(!IsDBCSLeadByte(*pInTmp)))
					*pOutTmp++  =  '+';
				else
				{
					*pOutTmp++  =  '%';
					*pOutTmp++  =  Hex[(*pInTmp>>4)&0x0f];
					*pOutTmp++  =  Hex[(*pInTmp)&0x0f];
				}
			}
				
				pInTmp++;
				
		}
		*pOutTmp  =  '\0';
		sprintf_s(pinput,8050,"&qcontent=%s",OutTmp);
#endif

	SOCKET	hSock;
	SOCKADDR_IN		stName;
	fd_set	        rfds,wfds;
	timeval	        tmTimeOut;
	char  *result;
	int   len, cnt, ret ;
	
	tmTimeOut.tv_usec=0;
	tmTimeOut.tv_sec=5;
	if( INVALID_SOCKET != ( hSock = socket( AF_INET, SOCK_STREAM, 0) ) )
	{
		DWORD flg = 1;
		if( SOCKET_ERROR != ioctlsocket( hSock, FIONBIO, &flg))
		{
			ZeroMemory( &stName, sizeof(struct sockaddr));
			stName.sin_family = PF_INET;
			stName.sin_port = htons(80);
			stName.sin_addr.s_addr = inet_addr(OnlineGmIP);
			ret = connect( hSock, (LPSOCKADDR)&stName, sizeof(struct sockaddr) );
			if( SOCKET_ERROR != ret || WSAEWOULDBLOCK == WSAGetLastError() )
			{
				cnt = 0;
				char  buf[3000];  
				if(ProblemType==0){   
					SendType=0;		//一般问题
					sprintf_s(buf,"GET //normal_check.asp?wgsid=%s&gamepwd=%s&game=1%s%s",pid,ppw,pdb,pinput);   
				}else{
					SendType=1;		//异常问题
					sprintf_s(buf,"GET //handle_check.asp?wgsid=%s&gamepwd=%s&game=1%s%s",pid,ppw,pdb,pinput);   
				}
				strcat_s(buf," HTTP/1.1\r\nAccept: text/*\r\nUser-Agent: StoneAge\r\n");
				sprintf_s(buf,"%sHost: %s\r\n\r\n",buf,OnlineGmIP);
#ifdef _STONDEBUG_   //写入onlinegm.log
			WriteInLog( buf , filename);
#endif
				len = lstrlen(buf);
				ichecklooptime = TimeGetTime();
				while(1)
				{
					FD_ZERO( &wfds);
					FD_SET( hSock, &wfds);
					nRet = select( NULL, (fd_set*)NULL, &wfds, (fd_set*)NULL, &tmTimeOut);
					if(nRet == SOCKET_ERROR||nRet==-1){
						closesocket(hSock);
						return 4;
					}else if(nRet == 0){
						closesocket(hSock);
						return 8;
					}
					if( FD_ISSET( hSock, &wfds) ){
						ret = send( hSock, &buf[cnt], len, 0);
						cnt += ret;
						len -= ret;
						if(len<1) break;  //ok
					}
					if( TimeGetTime() - ichecklooptime > 3000 ){
						closesocket(hSock);
						return 8;
					}
				}
				Sleep(500);
				len = (InputHistory.newNo*70)+500 ; //1024;
				cnt = 0;
				ichecklooptime = TimeGetTime();  
				while(1)
				{
					if(wait>=3){
						closesocket(hSock);
						return 8;
					}
					FD_ZERO( &rfds);
					FD_SET( hSock, &rfds);
					select( NULL, &rfds, (fd_set*)NULL, (fd_set*)NULL, &tmTimeOut);
					if(nRet == SOCKET_ERROR||nRet==-1){
						closesocket(hSock);
						return 4;
					}
					if(nRet == 0){
						wait++;
						continue;
					}
					if( FD_ISSET( hSock, &rfds) ){
				 		ret = recv( hSock, &buf[cnt], len, 0);
						if(ret<0){
							closesocket(hSock);
							return 4;
						}
					}
					buf[cnt+ret] = 0;
					result = strstr( buf, "\r\n\r\n") + 4;
					getStringToken(result ,',',1,sizeof(returndata)-1,returndata);
					if(atoi(returndata)!=1){  //如果回传负数表示有误，1表示ok
						closesocket(hSock);
						return atoi(returndata);
					}else{ 
						getStringToken(result ,':',2,sizeof(returndata)-1,returndata);
						iReturnNo = atoi(returndata);  //读出单号
					}
					if(result)  break;
					cnt += ret;
					len -= ret;
					if( TimeGetTime() > ichecklooptime + 3000 ){
						closesocket(hSock);
						return 8;
					}
				}
			}else{
				closesocket(hSock);
				return 6;
			}
		}	
		closesocket(hSock);
		return 1;
	}
	return 4;			
}

//************************************************
// 传送要删除单号的主程式
// 回传值 :0 --- 处理完毕 & 问题删除成功
//		   3 --- 参数传递错误
//         4 --- 网路端错误
//         6 --- 连线逾时(或DNS失败)
//************************************************
int SendDeleteMsg(char pid[16], char ppw[16] , char pno[16] )
{
	int nRet;
	if(!QueryOnlineGmIP(SMSDomainName)) return 6;

	int ret,len,cnt,icheck,wait=1;
	SOCKET	hSock;
	SOCKADDR_IN		stName;
	fd_set	        rfds,wfds;
	timeval	        tmTimeOut;
	char  *result;
	tmTimeOut.tv_usec=0;
	tmTimeOut.tv_sec=5;
	if( INVALID_SOCKET != ( hSock = socket( AF_INET, SOCK_STREAM, 0) ) )
	{
		DWORD flg = 1;
		if( SOCKET_ERROR != ioctlsocket( hSock, FIONBIO, &flg))
		{
			ZeroMemory( &stName, sizeof(struct sockaddr));
			stName.sin_family = PF_INET;
			stName.sin_port = htons(80); 
			stName.sin_addr.s_addr = inet_addr(OnlineGmIP);
			ret = connect( hSock, (LPSOCKADDR)&stName, sizeof(struct sockaddr) );
			if( SOCKET_ERROR != ret || WSAEWOULDBLOCK == WSAGetLastError() )
			{
				cnt = 0;
				char  buf[1024];
				sprintf_s(buf,"GET //DelForm.asp?wgsid=%s&gamepwd=%s&rcount=1&idno=%s",pid,ppw,pno);				
				strcat_s(buf," HTTP/1.1\r\nAccept: text/*\r\nUser-Agent: StoneAge\r\n");
				sprintf_s(buf,"%sHost: %s\r\n\r\n",buf,OnlineGmIP);
				len = lstrlen(buf);
				ichecklooptime = TimeGetTime();
				while(1)
				{
					FD_ZERO( &wfds);
					FD_SET( hSock, &wfds);
					nRet = select( NULL, (fd_set*)NULL, &wfds, (fd_set*)NULL, &tmTimeOut);
					if(nRet == SOCKET_ERROR||nRet==-1){
						closesocket(hSock);
						return 4;
					}else if(nRet == 0){
						closesocket(hSock);
						return 8;
					}
					if( FD_ISSET( hSock, &wfds) ){
						ret = send( hSock, &buf[cnt], len, 0);
						cnt += ret;
						len -= ret;
						if(len<1) break;  //ok
					}
					if( TimeGetTime() > ichecklooptime + 3000 ){
						closesocket(hSock);
						return 8;
					}
				}
				Sleep(500);
				len = 1024;
				cnt = 0;
				ichecklooptime = TimeGetTime();
				while(1)
				{
					if(wait>=3){
						closesocket(hSock);
						return 8;
					}
					FD_ZERO( &rfds);
					FD_SET( hSock, &rfds);
					nRet =  select( NULL, &rfds, (fd_set*)NULL, (fd_set*)NULL, &tmTimeOut);
					if(nRet == SOCKET_ERROR||nRet==-1){
						closesocket(hSock);
						return 4;
					}
					if(nRet == 0){
						wait++;
						continue;
					}
					if( FD_ISSET( hSock, &rfds) ){
						ret = recv( hSock, &buf[cnt], len, 0);							
						if(ret<0){
							closesocket(hSock);
							return 4;
						}
						buf[cnt+ret] = 0;
						result = strstr( buf, "\r\n\r\n") + 4;
						char *sss;
						strtok_s(result,",",&sss);
						icheck=atoi(result);
						if(icheck != 1){
							closesocket(hSock);
							return icheck;
						}
						if(result)  break;
						cnt += ret;
						len -= ret;
					}
					if( TimeGetTime() > ichecklooptime + 3000 ){
						closesocket(hSock);
						return 8;
					}
				}
			}else{
				closesocket(hSock);
				return 6;
			}
		}
		closesocket(hSock);
		return 1 ;
	}
	return 4 ;
}

//************************************************
//	向Server要玩家的问题单所有内容
//  回传值 0: 正确无误
//         2: 玩家无任何问题单
//         4: 网路端错误
//         6: 连线逾时(或DNS失败)
//************************************************
int SendDirMsg(char pid[16], char ppw[16] )
{
	if(!QueryOnlineGmIP(SMSDomainName)) return 6;

	int nRet,i=1;
	int wait=1,j=1;		
	char alldata[65536],getdata[6000],perdata[3000],statedata[100];  //全部的,每笔,每栏,处理状态
	char returndata[20] ;
	BOOL Okflag = FALSE , Firstflag = TRUE;
#ifdef _STONDEBUG_
	char logbuf[50];
#endif
	int ret,len,cnt; 
	SOCKET	hSock;
	SOCKADDR_IN		stName;
	fd_set	        rfds,wfds;
	timeval	        tmTimeOut;
	char  *result;

	tmTimeOut.tv_usec=0;
	tmTimeOut.tv_sec=5;
	memset(alldata,0,sizeof(alldata));
	if( INVALID_SOCKET != ( hSock = socket( AF_INET, SOCK_STREAM, 0) ) )
	{
		DWORD flg = 1;
		if( SOCKET_ERROR != ioctlsocket( hSock, FIONBIO, &flg))
		{
			ZeroMemory( &stName, sizeof(struct sockaddr));
			stName.sin_family = PF_INET;
			stName.sin_port = htons(80); 
			stName.sin_addr.s_addr = inet_addr(OnlineGmIP);
			ret = connect( hSock, (LPSOCKADDR)&stName, sizeof(struct sockaddr) );
			if( SOCKET_ERROR != ret || WSAEWOULDBLOCK == WSAGetLastError() )
			{
				cnt = 0;
				char  buf[65536];
				sprintf_s(buf,"GET //userFormList.asp?wgsid=%s&gamepwd=%s",pid,ppw);				
				strcat_s(buf," HTTP/1.1\r\nAccept: text/*\r\nUser-Agent: StoneAge\r\n");
				sprintf_s(buf,"%sHost: %s\r\n\r\n",buf,OnlineGmIP);
				len = lstrlen(buf);
				ichecklooptime = TimeGetTime();
				while(1)
				{
					FD_ZERO( &wfds);
					FD_SET( hSock, &wfds);
					nRet = select( NULL, (fd_set*)NULL, &wfds, (fd_set*)NULL, &tmTimeOut);
					if(nRet == SOCKET_ERROR||nRet==-1){
						closesocket(hSock);
						return -400;
					}else if(nRet == 0){
						closesocket(hSock);
						return -800;
					}
					if( FD_ISSET( hSock, &wfds) ){
						ret = send( hSock, &buf[cnt], len, 0);
						cnt += ret;
						len -= ret;
						if(len<1) break;  //ok
					}
					if( TimeGetTime() > ichecklooptime + 3000 ){
						closesocket(hSock);
						return -800;
					}
				}				
				Sleep(500);
				len = 65536;//8000;
				cnt = 0;
				ichecklooptime = TimeGetTime();
				while(1)
				{
					if(wait>=3){
						closesocket(hSock);
						return -800;
					}
					FD_ZERO( &rfds);
					FD_SET( hSock, &rfds);
					nRet = select( NULL, &rfds, (fd_set*)NULL, (fd_set*)NULL, &tmTimeOut);
					if(nRet == SOCKET_ERROR||nRet==-1){
						closesocket(hSock);
						return -400;
					}
					if(nRet == 0){
						wait++;
						continue;
					}
					if( FD_ISSET( hSock, &rfds) ){
						ret = recv( hSock, &buf[cnt], len, 0);							
						if(ret<0){
							closesocket(hSock);
							return -400;
						}								
						buf[cnt+ret] = 0;   
						result = strstr( buf, "\r\n\r\n") + 4;
						if(strcmp(result,"0")==0){
							closesocket(hSock);
							return -200;
						}
						strcpy(alldata,result);
						cnt += ret;
						len -= ret;
					}
					if(strstr(alldata,"&#")){ //ok
						break;
					}
					if( TimeGetTime() > ichecklooptime + 3000 ){
						closesocket(hSock);
						return -800;
					}
				}
				getStringToken(result ,',',1,sizeof(returndata)-1,returndata);
				if(atoi(returndata)<0){  //如果回传负数表示有误，大于0数子代表单子数量，同时也表示ok
					closesocket(hSock);
					return atoi(returndata);
				}else{
					TotalAddValue = atoi(returndata);
				}
				result = alldata;
				result = strstr( result , ",") + 1;
				strcpy(alldata,result);	
			}else{
#ifdef _STONDEBUG_
				sprintf_s(logbuf,"Get last error:%d",WSAGetLastError());
				WriteInLog( logbuf , filename);
#endif
				return -600;
			}
		}
		closesocket(hSock);
		ProNoValue = 0;
		ProNoSelectFlag = FALSE;
#ifdef _STONDEBUG_   //写入onlinegm.log
			WriteInLog( alldata , filename);
#endif
		perdata[0] = '\0';
		getdata[0] = '\0';
		//sprintf_s(alldata,"21|测试|*|*|*|*|18|卡人卡石|亲爱的维护部阿俊您好谢谢您的用心与努力~感谢您的支持与爱护！也再次的预祝您修改愉快！顺心如意！|亲爱的维护部阿俊您好谢谢您的用心与努力~感谢您的支持与爱护！也再次的预祝您修改愉快！顺心如意！|&#");
		while(getStringToken( alldata,'&',i,sizeof(getdata)-1,getdata)!=1){
			while(getStringToken(getdata,'|',j,sizeof(perdata)-1,perdata)!=1){
				if( strcmp( perdata , "*")){
					switch(j){
					case 1:sprintf_s(Prodata[ProNoValue].no,"%s",perdata); break;
					case 2:
						getStringToken(perdata,'>',2,sizeof(statedata)-1,statedata);
						getStringToken(statedata,'<',1,sizeof(statedata)-1,statedata);
						if(lstrlen(statedata) == NULL)
							sprintf_s(Prodata[ProNoValue].state,"%s",perdata);
						else
							sprintf_s(Prodata[ProNoValue].state,"%s",statedata);
						break;
					case 3:sprintf_s(Prodata[ProNoValue].okdate,"%s",perdata);break;
					case 4:Prodata[ProNoValue].okH=atoi(perdata); break;
					case 5:sprintf_s(Prodata[ProNoValue].wrongdate,"%s",perdata);break;
					case 6:Prodata[ProNoValue].wrongH=atoi(perdata); break;
					case 7:Prodata[ProNoValue].class0=atoi(perdata);break;
					case 8:sprintf_s(Prodata[ProNoValue].class1,"%s",perdata);break;
					case 9:sprintf_s(Prodata[ProNoValue].cont,"%s",perdata);break;
					case 10:sprintf_s(Prodata[ProNoValue].answer,"%s",perdata);break;
					default: sprintf_s(Prodata[ProNoValue].error,"%s",perdata);break;
					}
				}
				perdata[0] = '\0';
				j++;		
			}
			getdata[0] = '\0';
			ProNoValue++;
			j=1; 
			i++;
		}
		//TotalAddValue = ProNoValue; 
		return 1 ; // ok
	}
	return 4 ;
}
//************************************************
//	查询问题单的内容
//************************************************
void vReadContent( int pno )
{
	static char *pRecv ;
	char date[10];
	char data[40];
	char classbuf1[40],classbuf2[40];
	int i=1;
		
	ClassIndex=Prodata[pno].class0 - 18;

	if(ClassIndex == 0 ){   //异常问题
		while(getStringToken(Prodata[pno].okdate,'/',i,sizeof(date)-1,date)!=1){ 
			if( i==1 )	okY = atoi(date);
			else if( i==2)	okM = atoi(date);
			i ++;
		}
		okD = atoi(date);
		okH = Prodata[pno].okH;
		i = 1;
		date[0] = '\0';
		while(getStringToken(Prodata[pno].wrongdate,'/',i,sizeof(date)-1,date)!=1){ 
			if( i==1 )	wrongY = atoi(date);
			else if( i==2)	wrongM = atoi(date);
			i ++;
		}
		wrongD = atoi(date);
		wrongH = Prodata[pno].wrongH;
	}

	if(getStringToken(Prodata[pno].class1,',',1,sizeof(data)-1,data)!=1){ 
		sprintf_s(classbuf1,"%s",data);
		data[0] = '\0';
		if(getStringToken(Prodata[pno].class1,',',2,sizeof(data)-1,data)==1) 
			sprintf_s(classbuf2,"%s",data);
	}else{
		sprintf_s(classbuf1,"%s",data);
	}

	if(ClassIndex){ // 一般问题
		for(i=0;i<DEF_C1_TOTAL;i++){
			if(strcmp(ProblemC1[i],classbuf1)==NULL)
			break;
		}
		ClassIndex1 = i;
	}else{			// 异常问题
		for(i=0;i<DEF_C2_TOTAL;i++){
			if(strcmp(ProblemC2[i],classbuf1)==NULL)
			break;
		}
		ClassIndex1 = i;
		switch(ClassIndex1){
		case 0: for(i=0;i<3;i++){
					if(strcmp(ProblemC2_1[i],classbuf2)==NULL)
					break;
				}break;
		case 1: for(i=0;i<3;i++){
					if(strcmp(ProblemC2_2[i],classbuf2)==NULL)
					break;
				}break;
		case 2: for(i=0;i<5;i++){
					if(strcmp(ProblemC2_3[i],classbuf2)==NULL) 
					break;
				}break;
		case 3: for(i=0;i<2;i++){
					if(strcmp(ProblemC2_4[i],classbuf2)==NULL) 
					break;
				}break;
		case 4: for(i=0;i<2;i++){ 
					if(strcmp(ProblemC2_5[i],classbuf2)==NULL) 
					break;
				}break;
		case 5: for(i=0;i<3;i++){
					if(strcmp(ProblemC2_6[i],classbuf2)==NULL)
					break;
				}break;
		case 10:for(i=0;i<3;i++){
					if(strcmp(ProblemC2_7[i],classbuf2)==NULL)
					break;
				}break;
			default: i=0;
		}
		ClassIndex2=i;
	}
		
	if (lstrlen(Prodata[pno].cont)!=NULL){
		sprintf_s(SegmentBuf[SegmentTotal],"%s",Prodata[pno].cont);
		SegmentTotal++;
		if(lstrlen(Prodata[pno].answer)!=NULL){
			sprintf_s(SegmentBuf[SegmentTotal],"%s",Prodata[pno].answer);
			SegmentTotal++;
		}
	}
}

//************************************************
//函式	: 资料输入栏位的初始化函式
//回传值	: None
//************************************************
void InitOnlineGmInput(int x, int y)
{
	int i;

	HistorySelect = 5000;

	SubBuffer.buffer[0]='\0';
	SubBuffer.x = x + 20;  
	SubBuffer.y = y + 400; 
	SubBuffer.len = 70;				//字数
	SubBuffer.color = FONT_PAL_AQUA;
	SubBuffer.fontPrio = FONT_PRIO_FRONT;
	SubBuffer.lineLen=70;			//每行几个字
	SubBuffer.lineDist=25;		//行与行间隔几个pixel

	InputHistory.newNo = 0;
	InputHistory.nowNo = 0;
	InputHistory.addNo = 0;
	InputHistory.lockNo = 0;
	for ( i=0;i<115;i++ )  
	{
		InputHistory.str[i][0]='\0';
		InputHistory.color[i]=FALSE; 
	}

	int len = lstrlen(SubBuffer.buffer);
	if(len){
		SubBuffer.cursor = len;
		SubBuffer.cnt= len;
	}else{
		SubBuffer.cnt=0;
		SubBuffer.cursor=0;
	}
	TitleFlag = TRUE;
}

//************************************************
//函式   : 处理问题单号的主函式
//回传值 : None
//************************************************
void ShowProblemNo( int x, int y, int Key )
{
	int i,k,word = 0;
	int Keyid;
	char buf[100]= "/0";
	static int fontId[5];
	static char *pChar ;
	
	if (Key==1){  //上箭头
		ProNoIndex -- ;
		if (ProNoIndex < 0 ) 
			ProNoIndex = 0 ;
	}
	else if(Key==2){	//下箭头
		ProNoIndex ++ ;
		if (ProNoIndex == ProNoValue)
		ProNoIndex = ProNoValue-1;
	}

	for ( i=0;i<5;i++ ){ // 总共五行 
		if (ProNoIndex+i>=ProNoValue)
			break ;
		if (ProNoIndex+i==ProNoSelect && ProNoSelectFlag == TRUE){
			sprintf_s(buf,"%s                 %s",Prodata[ProNoIndex+i].no,Prodata[ProNoIndex+i].state);  
			fontId[i] = StockFontBuffer( x + 20 , y+7+(i*20) , FONT_PRIO_FRONT, 1 , buf , 2 );
		}else{
			sprintf_s(buf,"%s                 %s",Prodata[ProNoIndex+i].no,Prodata[ProNoIndex+i].state);  
			fontId[i] = StockFontBuffer( x + 20 , y+7+(i*20) , FONT_PRIO_FRONT, 0 , buf, 2 );
		}
		if(Prodata[ProNoIndex+i].delflag == TRUE){
			sprintf_s(buf,"已删除");
			StockFontBuffer( x + 70 , y+7+(i*20) , FONT_PRIO_FRONT, 4 , buf, 2 );
		}
	}

	Keyid = selFontId( fontId, sizeof( fontId )/sizeof( int ) );
	if ( 0 <= Keyid && Keyid < 5 ){
		if (ProNoIndex + Keyid < ProNoValue){   
			//查询某比单号的内容
			ProNoSelectFlag = TRUE;
			ProNoSelect = ProNoIndex + Keyid;
			if ( ProNoSelect != HistorySelect ){
				HistorySelect = ProNoSelect;
				InitOnlineGmInput(x, y);
				SegmentIndex = 0;
				SegmentTotal = 0;
				SegmentBuf[0][0] = '\0';
				SegmentBuf[1][0] = '\0';
				vReadContent( ProNoSelect );
			}
		}
	}
	
	if( SegmentTotal > SegmentIndex  ){  //   秀出内容
		InputHistory.addNo =0;  // 非玩家输入所以不用加
		GetKeyInputFocus(&SubBuffer);
		if( SegmentIndex %2 == 0 ){
			if(TitleFlag==TRUE){ 
				StrToNowStrBuffer1("前次问题内容：");
				for ( k=0;k<60+10;k++ )   
					StrToNowStrBuffer(" ");
				InputHistory.color[InputHistory.newNo]=TRUE;
				pContent = SegmentBuf[SegmentIndex];
				TitleFlag = FALSE;	
			}else{
				if(pContent!=NULL){
					word = StrToNowStrBuffer2(pContent);
					pContent+=word ;
					if(word<70){
						for ( k=0;k<(80-word);k++ )   
							StrToNowStrBuffer(" ");
					}
					if(*pContent == NULL){
						pContent = NULL;
						SegmentIndex++;			
						TitleFlag = TRUE;
					}				
					/*
					if (i>70){
						StrToNowStrBuffer(pContent);
						pContent+=70 ;
					}else{
						StrToNowStrBuffer(pContent);
						pContent+=i ;
						for ( k=0;k<(70-i);k++ )   //cyg 补空格
							StrToNowStrBuffer(" ");
					}
					if(*pContent == NULL){
						pContent = NULL;
						SegmentIndex++;			
						TitleFlag = TRUE;
					}
					*/
				}
			}
		}else{
			if(TitleFlag==TRUE){ 
				StrToNowStrBuffer1("客服解答：");
				for ( k=0;k<60+10;k++ )   //cyg 补空格
					StrToNowStrBuffer(" ");
				InputHistory.color[InputHistory.newNo]=TRUE;
				pContent = SegmentBuf[SegmentIndex];
				TitleFlag = FALSE;
			}else{
				if(pContent!=NULL){
					word = StrToNowStrBuffer2(pContent);
					pContent+=word ;
					if(word<70){
						for ( k=0;k<(80-word);k++ ){   
							StrToNowStrBuffer(" ");
						}
					}
					if(*pContent == NULL){
						pContent = NULL;
						SegmentIndex++;			
						TitleFlag = TRUE;
					}
				}
			}
		}
	}else{ 
		if( TitleFlag == TRUE ){
			GetKeyInputFocus(&SubBuffer);
			StrToNowStrBuffer1("新增问题内容：");
			for ( k=0;k<56+10;k++ )   
				StrToNowStrBuffer(" ");
			InputHistory.color[InputHistory.newNo]=TRUE;
			TitleFlag = FALSE;
			InputHistory.addNo = 0;
			InputHistory.lockNo = InputHistory.newNo;
			
		}
	}
}

//************************************************
//函式   : 处理资料输入栏位的主函式
//回传值 : None
//************************************************
void InputOnlineGmData( int x, int y, int Key )
{
	int Changelineflag = 0;
	int i;

	if ( InputHistory.newNo - InputHistory.nowNo <= 4)
		GetKeyInputFocus(&SubBuffer);
	else
		pNowStrBuffer = NULL ;
	
	if (Key == 1)  //上箭头
	{
		if (InputHistory.newNo >4 && InputHistory.nowNo ){
			InputHistory.nowNo -- ; 
			if (InputHistory.newNo - (InputHistory.nowNo+4) == 1 ){
				strcpy( InputHistory.str[ InputHistory.newNo ],SubBuffer.buffer);
				SubBuffer.buffer[0] = NULL ;
			}
		}
	}
	if (Key == 2) // 下箭头
	{
		if (InputHistory.newNo >4){
			if (InputHistory.nowNo+4<InputHistory.newNo){
				InputHistory.nowNo ++ ; 
				if (InputHistory.nowNo+4==InputHistory.newNo){
				GetKeyInputFocus(&SubBuffer);
				strcpy( SubBuffer.buffer,InputHistory.str[ InputHistory.newNo ] );
				}
			}
		}
	}

	Changelineflag = StockFontBuffer3( &SubBuffer );
	if (Changelineflag){ // 换行 
		if(InputHistory.addNo < 30 ){  //0~29
			SubBuffer.cnt=0;
			SubBuffer.cursor=0;
			if (SubBuffer.y< y + 410)
				SubBuffer.y += SubBuffer.lineDist ; 
			strcpy( InputHistory.str[ InputHistory.newNo ],SubBuffer.buffer);
			SubBuffer.buffer[0]='\0';				
			InputHistory.newNo++;
			InputHistory.addNo++;
			if (InputHistory.newNo >= 5)
				InputHistory.nowNo = InputHistory.newNo -4; 
		}else{
			//SubBuffer.cursor
		}
	}
	
	for ( i=0;i<5;i++ ) // 总共五行 
	{
		if ((InputHistory.nowNo+i)!=InputHistory.newNo){
			if(InputHistory.color[InputHistory.nowNo+i]==FALSE)
				StockFontBuffer( SubBuffer.x , y+300+(i*(30-5)) , FONT_PRIO_FRONT, 0 ,InputHistory.str[ InputHistory.nowNo+i ], 0 );
			else
				StockFontBuffer(  SubBuffer.x , y+300+(i*(30-5)) , FONT_PRIO_FRONT, 5 ,InputHistory.str[ InputHistory.nowNo+i ], 0 );
		}
	}
}

HANDLE GMHandle;
extern char szUser[],szPassword[];
//************************************************
//函式   : 处理查询单号的主函式
//回传值 : OL
//************************************************
DWORD WINAPI DirThread(LPVOID param)
{
	ResultCode = -1;
	char id[32], password[32];
	CopyMemory(id,szUser,32);
	ecb_crypt("f;encor1c",id,32,DES_DECRYPT);
	CopyMemory(password,szPassword,32);
	ecb_crypt("f;encor1c",password,32,DES_DECRYPT);

	ResultCode = SendDirMsg( id,password );

	switch(ResultCode)
	{
		case -200: 
			StockChatBufferLine("无任何问题单纪录!!",FONT_PAL_RED);
			StockChatBufferLine("请新增问题单!",FONT_PAL_RED);
			ProNoValue = 0;
			break;
		case -400:
			StockChatBufferLine("网路发生错误!!",FONT_PAL_RED);
			StockChatBufferLine("无法查询问题单!",FONT_PAL_RED);
			break;
		case -600:
			StockChatBufferLine("线上回报服务器无法连结!!",FONT_PAL_RED);
			StockChatBufferLine("请您稍后再试...",FONT_PAL_WHITE);
			break;
		case -800:
			StockChatBufferLine("网路连线逾时!!",FONT_PAL_RED);
			StockChatBufferLine("请您稍后再试...",FONT_PAL_WHITE);
			break;
		case 1:    // 查询成功
			break;
		case -999:
			StockChatBufferLine("网路忙线中!!",FONT_PAL_RED);
			StockChatBufferLine("请您稍后再试...",FONT_PAL_RED);
			break;
		default:
			StockChatBufferLine("抱歉程式内部错误!!!",FONT_PAL_RED);
			StockChatBufferLine("问题单查询失败!",FONT_PAL_RED);
			break;
	}
#ifdef _STONDEBUG_   //写入onlinegm.log
	if( ResultCode != 1 ){
		sprintf_s(inlogstr,"查询失败错误码%d",ResultCode);
		WriteInLog(inlogstr,filename);
	}
#endif
	CloseHandle(GMHandle);
	ExitThread(0);	
	return 0L;
}

//************************************************
//函式   : 处理删除单号的主函式
//回传值 : OL
//************************************************
DWORD WINAPI DeleteThread(LPVOID param)
{
	ResultCode = -1;
	char id[32], password[32] ;
	CopyMemory(id,szUser,32);
	ecb_crypt("f;encor1c",id,32,DES_DECRYPT);
	CopyMemory(password,szPassword,32);
	ecb_crypt("f;encor1c",password,32,DES_DECRYPT);
#ifdef _STONDEBUG_
	inlogstr[0] = 0;
#endif

	ResultCode = SendDeleteMsg(id,password,Prodata[ProNoSelect].no);

	switch(ResultCode)
	{
		case 4:
			StockChatBufferLine("网路发生错误!!",FONT_PAL_RED);
			StockChatBufferLine("无法删除本则单号!",FONT_PAL_RED);
			break;
		case 6:
			StockChatBufferLine("线上回报服务器无法连结!!",FONT_PAL_RED);
			StockChatBufferLine("请您稍后再试...",FONT_PAL_WHITE);
			break;
		case 8:
			StockChatBufferLine("网路连线逾时!!",FONT_PAL_RED);
			StockChatBufferLine("请您稍后再试...",FONT_PAL_WHITE);
			break;
		case 1: 
			StockChatBufferLine("问题单号已删除了!",FONT_PAL_YELLOW);
			Prodata[ProNoSelect].delflag = TRUE ;
			TotalAddValue --;
			break;
		case -999:
			StockChatBufferLine("网路忙线中!!",FONT_PAL_RED);
			StockChatBufferLine("请您稍后再试...",FONT_PAL_RED);
			break;
		default:
			StockChatBufferLine("抱歉程式内部错误!!!",FONT_PAL_RED);
			StockChatBufferLine("本则单号删除失败!",FONT_PAL_RED);
			break;
	}
#ifdef _STONDEBUG_   //写入onlinegm.log
	if(	ResultCode != 1){	
		sprintf_s(inlogstr,"删除失败错误码%d",ResultCode);
		WriteInLog(inlogstr,filename);
	}
#endif

	CloseHandle(GMHandle);
	ExitThread(0);	
	return 0L;
}

//************************************************
//函式   : 处理Multithread的主函式
//回传值 : OL
//************************************************
DWORD WINAPI OnlineGmThread(LPVOID param)
{
	ResultCode = -1;
	char id[32], password[32] ,buf[50];
	CopyMemory(id,szUser,32);
	ecb_crypt("f;encor1c",id,32,DES_DECRYPT);
	CopyMemory(password,szPassword,32);
	ecb_crypt("f;encor1c",password,32,DES_DECRYPT);
#ifdef _STONDEBUG_
	inlogstr[0] = 0;
#endif
	
	ResultCode = SendProblemMsg(id,password,DataBuffer,InputBuffer);

	switch(ResultCode)
	{
		case 4:
			StockChatBufferLine("网路发生错误!!",FONT_PAL_RED);
			StockChatBufferLine("无法发送本则问题!",FONT_PAL_RED);
			break;
		case 6:
			StockChatBufferLine("线上回报服务器无法连结!!",FONT_PAL_RED);
			StockChatBufferLine("请您稍后再试...",FONT_PAL_WHITE);
			break;
		case 8:
			StockChatBufferLine("网路连线逾时!!",FONT_PAL_RED);
			StockChatBufferLine("请您稍后再试...",FONT_PAL_WHITE);
			break;
		case -9:
			if(SendType==0){
				StockChatBufferLine("每次发送一般问题单",FONT_PAL_RED);
				StockChatBufferLine("请间隔三十分钟谢谢...",FONT_PAL_RED);
			}else{
				StockChatBufferLine("每次发送异常问题单",FONT_PAL_RED);
				StockChatBufferLine("请间隔一小时谢谢...",FONT_PAL_RED);
			}
			break;
		case -999:
			StockChatBufferLine("网路忙线中!!",FONT_PAL_RED);
			StockChatBufferLine("请您稍后再试...",FONT_PAL_RED);
			break;
		case 1:
			sprintf_s(buf,"问题单号%d已送出!",iReturnNo);
			StockChatBufferLine( buf ,FONT_PAL_YELLOW);
			TotalAddValue ++;
			ResetFlag = TRUE ; // 只有成功才reset
			DirFlag = TRUE ;
			break;
		default:
			StockChatBufferLine("抱歉程式内部错误!!!",FONT_PAL_RED);
			StockChatBufferLine("本则单号传送失败!",FONT_PAL_RED);
			break;
	}
#ifdef _STONDEBUG_   //写入onlinegm.log
	if( ResultCode != 1 ){
		sprintf_s(inlogstr,"问题单类型:%d 建单失败错误码:%d",SendType,ResultCode);
		WriteInLog(inlogstr,filename);
	}
#endif
	CloseHandle(GMHandle);
	ExitThread(0);	
	return 0L;
}

extern int nServerGroup;
extern short selectServerIndex;
extern short onlinegmProcNo;
extern int sCharSide ;
//**************************************************
//主函式:
//回传值:0 表示视窗开启中 1 表示完成
//**************************************************
int iOnlineGmProc()
{
	int i , id ; 
	static DWORD dwPressTime;
	static int btnId[50] , btnState[50] , HelpId[10] , HelpidState[10] ;
	static int SureId[2];
	char buf[100]= "/0" ; // test
	static ACTION *pActOnlineGmWnd = NULL;
	static ACTION *pActSureWnd = NULL;
	static struct tm nowTime;
	time_t longTime;
	DWORD  PushdirTime;
	static LPPAGE page;
	static LPDESC desc;
	static int desc_index,page_index;
	int x, y;
	if (onlinegmProcNo == 0)
	{
		// 打开一个视窗	
		if (pActOnlineGmWnd == NULL )
		{
			x = (lpDraw->xSize - 600) / 2;
			y = (lpDraw->ySize - 450) / 2;

			pActOnlineGmWnd = MakeWindowDisp(x,y,600,450,NULL,-1, FALSE);

			x = pActOnlineGmWnd->x;
			y = pActOnlineGmWnd->y;

			if(ResetFlag==TRUE){
				InitOnlineGmInput(x, y);
				InitOnlineGm(); 
				dwPressTime = 0;
				for ( i = 0 ; i < sizeof(btnId)/sizeof(int) ; i++ )
				{	
					btnId[i] = 0;
					btnState[i] = 0;
				}
				for ( i = 0 ; i < sizeof(HelpId)/sizeof(int) ; i++ )
				{
					HelpId[i] = 0;
					HelpidState[i] = 0;
				}
				time( &longTime );
				localtime_s(&nowTime, &longTime );
				wrongY = okY = nowTime.tm_year+1900 ;
				/*//fix
				wrongM = okM = nowTime.tm_mon+1 ;
				wrongD = okD = nowTime.tm_mday ;
				wrongH = okH = nowTime.tm_hour ;
				*/
				//画面reset
				ProNoIndex = 0;
				desc_index = 0;
				page_index = 0; 
				PushKey = 3;  // 自动查询
			}

		}
		else	
		{
			x = pActOnlineGmWnd->x;
			y = pActOnlineGmWnd->y;

			if (DeleteBtnFlag==FALSE)
			{
				id = selGraId( btnId, sizeof( btnId )/sizeof( int ) );
				if(dwPressTime)
				{
					if( TimeGetTime()>(dwPressTime+100))
					{
						for( i = 0 ; i < DEF_BUTTON_TOTAL ; i ++ )
						{
							if(btnState[i])
							{
								btnState[i] = 0;
								switch (i)
								{
								case 0://查询
									PushdirTime = TimeGetTime();
									if ( DirFlag==TRUE || (PushdirTime-LastTime)>5000 ){   //(关闭)有查询过就不能在查
										LastTime = TimeGetTime();
										PushKey = 3;
										DirFlag = FALSE;
									}
									break;
								case 1://删除
									if (ProNoSelectFlag){	//要有选到某单号才能删除
										DeleteBtnFlag = TRUE ;										
									}
									break;						
								case 2://传送
									DeathAction( pActOnlineGmWnd );
									pActOnlineGmWnd = NULL;
									onlinegmProcNo = 2;
									return 0;								
								case 3://清除
									InitOnlineGmInput(x, y);	break;
								case 4://注意事项
									DeathAction( pActOnlineGmWnd );
									pActOnlineGmWnd = NULL;
									onlinegmProcNo = 1;
									return 0;
								case 5://关闭
									ResetFlag = TRUE;
									DeathAction( pActOnlineGmWnd);
									pActOnlineGmWnd = NULL;
									return 1 ;
								case 6://讯息框上卷
									if(ProNoValue){
										PushKey = 1;
									}
									break;
								case 7://讯息框下卷
									if(ProNoValue){
										PushKey = 2;
									}
									break;
								case 8:PutKey = 1;  break;//输入框上卷
								case 9:PutKey = 2;  break;//输入框下卷
								case 10://正常年月上卷
									okM--;
									if (okM<=0){  //fix
										okY--;
										okM=12;
									}
									break;
								case 11://正常年月下卷
									okM++;
									if (okM>12){
										okY++;
										okM=1;
									}
									break;
								case 12://正常日上卷
									okD--;
									if (okD<=0) okD = 31 ;   //fix
									break;
								case 13://正常日下卷
									okD++;
									if (okD>31) okD = 1;
									break;
								case 14://正常时上卷
									okH--;
									if (okH<0)	okH = 23;
									break;
								case 15://正常时下卷
									okH++;
									if (okH>23) okH=0;
									break;
								case 16://错误年月上卷
									wrongM--;
									if (wrongM<=0){ //fix
										wrongY--;
										wrongM=12;
									}
									break;
								case 17://错误年月下卷
									wrongM++;
									if (wrongM>12){
										wrongY++;
										wrongM=1;
									}
									break;
								case 18://错误日上卷
									wrongD--;
									if (wrongD<=0) wrongD = 31;  //fix
									break;								
								case 19://错误日下卷
									wrongD++;
									if (wrongD > 31) wrongD = 1;
									break;
								case 20://错误时上卷
									wrongH--;
									if (wrongH<0) wrongH = 23;
									break;
								case 21://错误时下卷
									wrongH++;
									if (wrongH>23) wrongH = 0;
									break;
								case 22://类别一上卷
									ClassIndex -- ;
									ClassIndex1 = 0 ;
									ClassIndex2 = 0 ;
									if (ClassIndex<0) ClassIndex = DEF_CLASS_TOTAL-1;
									break;
								case 23://类别一下卷
									ClassIndex ++ ;
									ClassIndex1 = 0 ;
									ClassIndex2 = 0 ;
									if (ClassIndex==DEF_CLASS_TOTAL) ClassIndex = 0;
									break;							
								case 24://类别二上卷
									ClassIndex1 -- ;
									ClassIndex2 = 0 ;
									if (ClassIndex==1){   // 一般问题
										if (ClassIndex1<0) ClassIndex1 = DEF_C1_TOTAL-1;
									}else{
										if (ClassIndex1<0) ClassIndex1 = DEF_C2_TOTAL-1;
									}
									break;
								case 25://类别二下卷
									ClassIndex1 ++ ;
									ClassIndex2 = 0 ;
									if (ClassIndex==1){		// 一般问题
										if (ClassIndex1==DEF_C1_TOTAL) ClassIndex1=0;
									}else{
										if (ClassIndex1==DEF_C2_TOTAL) ClassIndex1=0;
									}
									break;
								case 26://类别三上卷
									ClassIndex2 -- ;
									switch (ClassIndex1)
									{
									case 0:if (ClassIndex2<0) ClassIndex2=2;break;
									case 1:if (ClassIndex2<0) ClassIndex2=2;break;
									case 2:if (ClassIndex2<0) ClassIndex2=4;break;
									case 3:if (ClassIndex2<0) ClassIndex2=1;break;
									case 4:if (ClassIndex2<0) ClassIndex2=1;break;
									case 5:if (ClassIndex2<0) ClassIndex2=2;break;
									case 10:if (ClassIndex2<0) ClassIndex2=2;break;
									default: ClassIndex2=0;break;
									}
									break;
								case 27://类别三下卷
									ClassIndex2 ++ ;
									switch (ClassIndex1)
									{
									case 0:if (ClassIndex2>2) ClassIndex2=0;break;
									case 1:if (ClassIndex2>2) ClassIndex2=0;break;
									case 2:if (ClassIndex2>4) ClassIndex2=0;break;
									case 3:if (ClassIndex2>1) ClassIndex2=0;break;
									case 4:if (ClassIndex2>1) ClassIndex2=0;break;
									case 5:if (ClassIndex2>2) ClassIndex2=0;break;
									case 10:if (ClassIndex2>2) ClassIndex2=0;break;
									default: ClassIndex2=0;break;
									}
									break;
								}
							}
						}
						dwPressTime = 0;
					}
				}
				else
				{
					if ( 0 <= id && id < DEF_BUTTON_TOTAL )
					{
						btnState[id] = 1;
						dwPressTime = TimeGetTime(); 
					}
				}
			}else{ 
				if (pActSureWnd == NULL){
					pActSureWnd = MakeWindowDisp( 200+100+20 , 100-63 , 4-1 , 6-4 , NULL , -1);				
				}
				else if(pActSureWnd->hp > 0)
				{
					StockDispBuffer(416+15+15,100,DISP_PRIO_MENU,ONLINEGM_YES_OR_NO,1);
					MakeHitBox( 338+15+15,100,405+15+15,124, DISP_PRIO_BOX2 );
					MakeHitBox( 426+15+15,100,494+15+15,124, DISP_PRIO_BOX2 );
					selGraId( SureId, sizeof( SureId )/sizeof( int ) );
					SureId[0] = StockDispBuffer(  372+15+15 , 112 , DISP_PRIO_IME3, ONLINEGM_SURE_U , 1);
					SureId[1] = StockDispBuffer(  460+15+15 , 112 , DISP_PRIO_IME3, ONLINEGM_CHANCEL , 1);
					if(Prodata[ProNoSelect].delflag == TRUE){
						sprintf_s(buf,"此笔单号已删除");
						StockFontBuffer( 365+15+15 , 81 , FONT_PRIO_FRONT, 2 , buf , 0 );
					}else{
						sprintf_s(buf,"删除单号 %s 吗?",Prodata[ProNoSelect].no);
						StockFontBuffer( 355+15+15 , 81 , FONT_PRIO_FRONT, 5 , buf , 0 );
					}

					if((mouse.onceState & MOUSE_LEFT_CRICK) && HitDispNo == SureId[0]){ //确定
						if(Prodata[ProNoSelect].delflag == FALSE){
							DWORD dwThreadID,dwThrdParam = 1;
							GMHandle = CreateThread(NULL, 0, DeleteThread,&dwThrdParam,0,&dwThreadID);
							if(GMHandle == NULL)
							{
								StockChatBufferLine("您的作业系统资源已不足!!",FONT_PAL_RED);
								StockChatBufferLine("本则单号删除失败!",FONT_PAL_RED);
								ResetFlag = TRUE;
								DeathAction( pActOnlineGmWnd);
								pActOnlineGmWnd = NULL;
								return 1;
							}				
						}
						DeathAction( pActSureWnd);
						pActSureWnd = NULL;			
						DeleteBtnFlag = FALSE ;
					}
					if((mouse.onceState & MOUSE_LEFT_CRICK) && HitDispNo == SureId[1]){ //取消
						DeathAction( pActSureWnd);
						pActSureWnd = NULL;			
						DeleteBtnFlag = FALSE ;
					}
				}
			}
			if (pActOnlineGmWnd->hp > 0)
			{
				x = pActOnlineGmWnd->x;
				y = pActOnlineGmWnd->y;
				if(PushKey != 3){
					ShowProblemNo(x, y, PushKey);
				}else{ //处理查询功能
					for( i=0;i<DEF_MAX_ADD_VALUE;i++){
						Prodata[i].no[0] = '\0';
						Prodata[i].state[0] = '\0';
						Prodata[i].okdate[0] = '\0';	
						Prodata[i].okH = 0;
						Prodata[i].wrongdate[0] = '\0';
						Prodata[i].wrongH = 0;	
						Prodata[i].class0 = 0;	
						Prodata[i].class1[0] = '\0';
						Prodata[i].cont[0] = '\0';
						Prodata[i].answer[0] = '\0';	
						Prodata[i].error[0] = '\0';
						Prodata[i].delflag = FALSE;		
					}
					DWORD dwThreadID,dwThrdParam = 1;
					GMHandle = CreateThread(NULL, 0, DirThread,&dwThrdParam,0,&dwThreadID);
					if(GMHandle == NULL)
					{
						StockChatBufferLine("您的作业系统资源已不足!!",FONT_PAL_RED);
						StockChatBufferLine("问题单号查询失败!",FONT_PAL_RED);
						ResetFlag = TRUE;
						DeathAction( pActOnlineGmWnd);
						pActOnlineGmWnd = NULL;
						return 1;
					}
					
				}
				PushKey = 0;


				InputOnlineGmData(x, y, PutKey);	
				PutKey = 0;
				/*  for test
				sprintf_s(buf,"new%d",InputHistory.newNo);
				StockFontBuffer( 212 , 250 , FONT_PRIO_FRONT, 1 , buf , 0 );
				sprintf_s(buf,"now%d",InputHistory.nowNo);
				StockFontBuffer( 350 , 250 , FONT_PRIO_FRONT, 1 , buf , 0 );
				sprintf_s(buf,"add%d",InputHistory.addNo);
				StockFontBuffer( 480 , 250 , FONT_PRIO_FRONT, 1 , buf , 0 );
				*/
				//sprintf_s(buf,"Total  %d",TotalAddValue);
				//StockFontBuffer( 480 , 250 , FONT_PRIO_FRONT, 1 , buf , 0 );
				/*
				sprintf_s(buf,"ProNoValue   %d",ProNoValue);
				StockFontBuffer( 490 , 250 , FONT_PRIO_FRONT, 1 , buf , 0 );
				sprintf_s(buf,"ProNoSelect  %d",ProNoSelect);
				StockFontBuffer( 350 , 250 , FONT_PRIO_FRONT, 1 , buf , 0 );
				*/
				StockDispBuffer( x + 320 , y + 228 , DISP_PRIO_MENU , ONLINEGM_BACKGROUND , 1);//背景图
				StockFontBuffer( x + 80, y + 180, FONT_PRIO_FRONT, 0,"    年  月       日       时",0);
				StockFontBuffer( x + 387+21, y + 180, FONT_PRIO_FRONT, 0,"    年  月      日       时",0);
				StockFontBuffer( x + 12 , y + 180, FONT_PRIO_FRONT, 5, "正常日期", 0 );
				StockFontBuffer( x + 315+21 ,y + 180, FONT_PRIO_FRONT, 5, "错误日期", 0 );
				StockFontBuffer( x + 12 , y + 215, FONT_PRIO_FRONT, 5, "问题类别", 0 );
				//显示时间
				sprintf_s( buf ,"%d  %02d       %02d       %02d",okY,okM,okD,okH );
				StockFontBuffer( x + 100-20 , y + 180, FONT_PRIO_FRONT, 1 , buf , 0 );
				sprintf_s( buf ,"%d  %02d      %02d       %02d",wrongY,wrongM,wrongD,wrongH );
				StockFontBuffer( x + 387+21 , y + 180, FONT_PRIO_FRONT, 1 , buf , 0 );
				//显示问题类别
				StockFontBuffer( x + 90 , y + 215, FONT_PRIO_FRONT, 1, ProblemClass[ClassIndex], 0 );
				if (ClassIndex==1) //一般问题 
				{
					StockFontBuffer( x + 230-30, y + 215, FONT_PRIO_FRONT, 1, ProblemC1[ClassIndex1], 0 );
					StockFontBuffer(  x + 20, y + 250, FONT_PRIO_FRONT, 1, ProblemC1Help[ClassIndex1], 0 );
				}else{	//异常问题
					StockFontBuffer( x + 230, y + 215, FONT_PRIO_FRONT, 1, ProblemC2[ClassIndex1], 0 );
					switch (ClassIndex1)
					{
					case 0:StockFontBuffer( x + 460, y + 215, FONT_PRIO_FRONT, 1, ProblemC2_1[ClassIndex2], 0 );break;
					case 1:StockFontBuffer( x + 460, y + 215, FONT_PRIO_FRONT, 1, ProblemC2_2[ClassIndex2], 0 );break;
					case 2:StockFontBuffer( x + 460, y + 215, FONT_PRIO_FRONT, 1, ProblemC2_3[ClassIndex2], 0 );break;
					case 3:StockFontBuffer( x + 460, y + 215, FONT_PRIO_FRONT, 1, ProblemC2_4[ClassIndex2], 0 );break;
					case 4:StockFontBuffer( x + 460, y + 215, FONT_PRIO_FRONT, 1, ProblemC2_5[ClassIndex2], 0 );break;
					case 5:StockFontBuffer( x + 460, y + 215, FONT_PRIO_FRONT, 1, ProblemC2_6[ClassIndex2], 0 );break;
					case 10:StockFontBuffer( x + 460, y + 215, FONT_PRIO_FRONT, 1, ProblemC2_7[ClassIndex2], 0 );break;
					default: break;
					}	
					if ((ClassIndex1==2&&ClassIndex2==0)||ClassIndex1==6)
						StockFontBuffer( x + 20,y + 250, FONT_PRIO_FRONT,1,"选择此项目需注明【宠物等级】",0);
				}
				//如果user有按下按钮 按钮显示按下
				for ( i = 0 ; i < DEF_BUTTON_TOTAL ; i ++ )
				{
					if (btnState[i]) 
						btnId[i] = StockDispBuffer(  x + BUTTON_XY[i][0], y + BUTTON_XY[i][1]  , DISP_PRIO_IME3, DOWN_ANIM_ID[i] , 2);
					else
						btnId[i] = StockDispBuffer(  x + BUTTON_XY[i][0], y + BUTTON_XY[i][1]  , DISP_PRIO_IME3, UP_ANIM_ID[i] , 2);
				}
			}
		}
	}
	else if (onlinegmProcNo == 1) // 注意事项
	{
		if (pActOnlineGmWnd==NULL){
			x = (lpDraw->xSize - 7*64)/2;
			y = (lpDraw->ySize - 8*48)/2;

			pActOnlineGmWnd = MakeWindowDisp( x , y , 4+3 , 6+2 ,NULL,0, FALSE);//( 210 , 120, 10 , 10 , NULL , 0 );
			pNowStrBuffer = NULL ;
		}
		else if (pActOnlineGmWnd->hp > 0)
		{
			x = pActOnlineGmWnd->x;
			y = pActOnlineGmWnd->y;
			page = &ProHelp_page[page_index];
			desc = Prohelp_Index[page_index]+page->desc_index;
			desc_index = page->desc_index;

			id = selGraId( HelpId, sizeof( HelpId )/sizeof( int ) );
			if (HelpidState[0]>=0)
				HelpId[0] = StockDispBuffer( x+207+32-160, y+352, DISP_PRIO_IME3, CG_FIELD_HELP_PREPAGE+btnState[1], 2);
			else
				StockDispBuffer( x+207+32-160, y+352, DISP_PRIO_IME3, CG_FIELD_HELP_PREPAGE+btnState[1], 1);

			if (HelpidState[1]>=0)
				HelpId[1] = StockDispBuffer( x+277+32-100, y+352, DISP_PRIO_IME3, CG_FIELD_HELP_NEXTPAGE+btnState[2], 2);
			else
				StockDispBuffer( x+277+32-100, y+352, DISP_PRIO_IME3, CG_FIELD_HELP_NEXTPAGE+btnState[2], 1);			

			HelpId[2] = StockDispBuffer( x+347+32-40, y+352, DISP_PRIO_IME3, CG_FIELD_HELP_EXIT+btnState[3], 2);

			if((mouse.onceState & MOUSE_LEFT_CRICK) && HitDispNo == HelpId[0]){  //上一页
				if (page_index){
					page_index--;
					page = &ProHelp_page[page_index];			//指向上一页
					desc_index = page->desc_index;
					desc = Prohelp_Index[page_index]+desc_index;//指向新一页的desc开头
				}
			}			
			if((mouse.onceState & MOUSE_LEFT_CRICK) && HitDispNo == HelpId[1]){	//下一页
				if (page_index < 4-1)
				{
					page_index++;
					page = &ProHelp_page[page_index];			//指向下一页
					desc_index = page->desc_index;
					desc = Prohelp_Index[page_index]+desc_index;//指向新一页的desc开头
				}
			}
			HelpidState[0] = page_index==0 ? -2 : 0 ;
			HelpidState[1] = page_index==4-1 ? -2 : 0 ;

			if((mouse.onceState & MOUSE_LEFT_CRICK) && HitDispNo == HelpId[2])
			{
				DeathAction( pActOnlineGmWnd);
				pActOnlineGmWnd = NULL;
				onlinegmProcNo = 0 ;
				ResetFlag = FALSE;
				return 0 ;
			}			
			for( i = 0; i < page->desc_num; i++){
				StockFontBuffer( x+30+5 , y+i*25+10, FONT_PRIO_FRONT, desc->color, desc->description, 0);			
				desc++;
			}
		}
	}
	else if (onlinegmProcNo == 2) //cyg 准备传送的确定视窗
	{
		if (pActOnlineGmWnd==NULL){
			x = (lpDraw->xSize - 5*64)/2;
			y = (lpDraw->ySize - 5*48)/2;

			pActOnlineGmWnd =  MakeWindowDisp( x , y , 5 , 5 ,NULL,0, FALSE);
			pNowStrBuffer = NULL ;
		}
		else if (pActOnlineGmWnd->hp > 0)
		{
			x = pActOnlineGmWnd->x;
			y = pActOnlineGmWnd->y;
			MakeHitBox( x + 36,y + 238-85-35,x + 40,y  + 238-85+35, DISP_PRIO_BOX );
			MakeHitBox( x + 156+18,y + 238-85-35,x  + 156+18,y  + 238-85+35, DISP_PRIO_BOX );
			id = selGraId( btnId, sizeof( btnId )/sizeof( int ) );
			int SureBtn[2];
			SureBtn[0] = StockDispBuffer(  x + 70 , y + 250-85+35 , DISP_PRIO_IME3, ONLINEGM_SURE_U , 1);
			SureBtn[1] = StockDispBuffer(  x + 190+18 , y + 250-85+35 , DISP_PRIO_IME3, ONLINEGM_CHANCEL , 1);
			sprintf_s( buf ,"错误讯息->");			
			StockFontBuffer( x + 20 , y+ 20+60 , FONT_PRIO_FRONT, 4 , buf , 0 );

			if( TotalAddValue >= DEF_MAX_ADD_VALUE ){  // 有没有达最大建单数
				sprintf_s( buf ,"您的问题单已达上限20笔!!");
				StockFontBuffer( x + 235 , y + 150 , FONT_PRIO_FRONT, 4 , buf , 0 );
				sprintf_s( buf ,"请删除问题单后再留单谢谢",TotalAddValue );
				StockFontBuffer( x + 235 , y + 185 , FONT_PRIO_FRONT, 4 , buf , 0 );
				if((mouse.onceState & MOUSE_LEFT_CRICK) && HitDispNo == SureBtn[0])
				{
					DeathAction( pActOnlineGmWnd);
					pActOnlineGmWnd = NULL;
					onlinegmProcNo = 0 ;
					ResetFlag = FALSE;
					return 0 ;
				}
			}else{
				if (ClassIndex==1){
					sprintf_s( buf ,"问题类别-> %s",ProblemClass[ClassIndex]);
					StockFontBuffer( x + 20 , y + 20+10, FONT_PRIO_FRONT, 1 , buf , 0 );
					sprintf_s( buf ,"问题种类-> %s",ProblemC1[ClassIndex1]);
					StockFontBuffer( x + 20 , y + 20+35, FONT_PRIO_FRONT, 1 , buf , 0 );
				}else{
					sprintf_s( buf ,"问题类别-> %s",ProblemClass[ClassIndex]);
					StockFontBuffer( x + 20 , y + 20+10, FONT_PRIO_FRONT, 1 , buf , 0 );
					sprintf_s( buf ,"问题种类-> %s",ProblemC2[ClassIndex1]);	
					StockFontBuffer( x + 20 , y + 20+35, FONT_PRIO_FRONT, 1 , buf , 0 );
					switch (ClassIndex1)
					{
					case 0: sprintf_s( buf,"种类细项-> %s",ProblemC2_1[ClassIndex2] );break;
					case 1: sprintf_s( buf,"种类细项-> %s",ProblemC2_2[ClassIndex2] );break;
					case 2: sprintf_s( buf,"种类细项-> %s",ProblemC2_3[ClassIndex2] );break;
					case 3: sprintf_s( buf,"种类细项-> %s",ProblemC2_4[ClassIndex2] );break;
					case 4: sprintf_s( buf,"种类细项-> %s",ProblemC2_5[ClassIndex2] );break;
					case 5: sprintf_s( buf,"种类细项-> %s",ProblemC2_6[ClassIndex2] );break;
					case 10: sprintf_s( buf,"种类细项-> %s",ProblemC2_7[ClassIndex2] );break;
					default: buf[0] = '\0'; 
					}
					StockFontBuffer( x + 220-25 , y + 120+60, FONT_PRIO_FRONT, 1 , buf , 0 );
					sprintf_s( buf ,"正常日期-> %4d年%02d月%02d日%02d时",okY,okM,okD,okH);
					StockFontBuffer( x + 220-25 , y + 120+85, FONT_PRIO_FRONT, 1 , buf , 0 );
					sprintf_s( buf ,"错误日期-> %4d年%02d月%02d日%02d时",wrongY,wrongM,wrongD,wrongH );
					StockFontBuffer( x + 220-25 , y + 120+110, FONT_PRIO_FRONT, 1 , buf , 0 );				
				}
			
				if( lstrlen(InputHistory.str[InputHistory.lockNo+1]) == 0 && lstrlen(SubBuffer.buffer) == 0 ){
					sprintf_s( buf ,"请输入问题内容");
					StockFontBuffer( x + 20 , y + 120, FONT_PRIO_FRONT, 4 , buf , 0 );
					if((mouse.onceState & MOUSE_LEFT_CRICK) && HitDispNo == SureBtn[0])
					{
						DeathAction( pActOnlineGmWnd);
						pActOnlineGmWnd = NULL;
						onlinegmProcNo = 0 ;
						ResetFlag = FALSE;
						return 0 ;
					}
				}else if(ClassIndex==0&&(!okY||!okM||!okD||!wrongY||!wrongM||!wrongD)){ //fix
					sprintf_s( buf ,"请填写[正常日期]与[错误日期]");
					StockFontBuffer( x + 280-10+2 , y + 260 , FONT_PRIO_FRONT, 4 , buf , 0 );
					if((mouse.onceState & MOUSE_LEFT_CRICK) && HitDispNo == SureBtn[0])
					{
						DeathAction( pActOnlineGmWnd);
						pActOnlineGmWnd = NULL;
						onlinegmProcNo = 0 ;
						ResetFlag = FALSE;
						return 0 ;
					}
				}else{
					if((mouse.onceState & MOUSE_LEFT_CRICK) && HitDispNo == SureBtn[0])
					{
						ResetFlag = FALSE;
						ContentBuf[0] = '\0';
						for ( i=0;i<InputHistory.addNo-1;i++)
							strcat_s(ContentBuf,InputHistory.str[InputHistory.newNo-(InputHistory.addNo-1)+i]); //组合每一行
						strcat_s(ContentBuf,SubBuffer.buffer);
						sprintf_s( InputBuffer,"%s",ContentBuf);

						if (ClassIndex==1){  // 一般问题   没有日期和时间
							ProblemType=0;
							sprintf_s( DataBuffer,"&star=%s&earth=%s&qtype1=%s&msource=19",gmgroup[nServerGroup].name,gmsv[selectServerIndex].name,ProblemC1[ClassIndex1]);					
						}else{	// 异常问题
							ProblemType=1;
							char tempbuf[60]; 
							sprintf_s( tempbuf,"%s",ProblemC2[ClassIndex1]);
							switch (ClassIndex1)
							{
							case 0: sprintf_s( tempbuf,"%s,%s",tempbuf,ProblemC2_1[ClassIndex2] );break;
							case 1: sprintf_s( tempbuf,"%s,%s",tempbuf,ProblemC2_2[ClassIndex2] );break;
							case 2: sprintf_s( tempbuf,"%s,%s",tempbuf,ProblemC2_3[ClassIndex2] );break;
							case 3: sprintf_s( tempbuf,"%s,%s",tempbuf,ProblemC2_4[ClassIndex2] );break;
							case 4: sprintf_s( tempbuf,"%s,%s",tempbuf,ProblemC2_5[ClassIndex2] );break;
							case 5: sprintf_s( tempbuf,"%s,%s",tempbuf,ProblemC2_6[ClassIndex2] );break;
							case 10: sprintf_s( tempbuf,"%s,%s",tempbuf,ProblemC2_7[ClassIndex2] );break;
							default: break;
							}
#ifdef _CHAR_PROFESSION
							if(lstrlen(pc.profession_class_name) == NULL){
								strcpy(pc.profession_class_name,"无");
							}
#endif
							char side[10];
							if(sCharSide == 1)
								strcpy(side,"左");
							else
								strcpy(side,"右");
#ifdef _CHAR_PROFESSION
							sprintf_s( DataBuffer,"&SY=%d&SM=%d&SD=%d&EY=%d&EM=%d&ED=%d&datarighttime=%d&datawrongtime=%d&msource=18&exceptcont=%s&G1grade1=%d&G1grade2=%d&G1Loc1=%s&G1Job1=%s&star=%s&G1gname1=%s",okY,okM,okD,wrongY,wrongM,wrongD,okH,wrongH,tempbuf,pc.transmigration,pc.level,side,pc.profession_class_name,gmgroup[nServerGroup].name,pc.name);
#else
							sprintf_s( DataBuffer,"&SY=%d&SM=%d&SD=%d&EY=%d&EM=%d&ED=%d&datarighttime=%d&datawrongtime=%d&msource=18&exceptcont=%s&G1grade1=%d&G1grade2=%d&G1Loc1=%s&G1Job1=%s&G1gname1=%s",okY,okM,okD,wrongY,wrongM,wrongD,okH,wrongH,tempbuf,pc.transmigration,pc.level,side,gmgroup[nServerGroup].name,pc.name);
#endif
						}
						DeathAction( pActOnlineGmWnd);
						pActOnlineGmWnd = NULL;
						DWORD dwThreadID,dwThrdParam = 1;
						GMHandle = CreateThread(NULL, 0, OnlineGmThread,&dwThrdParam,0,&dwThreadID);
						if(GMHandle == NULL)
						{
							StockChatBufferLine("您的作业系统资源已不足!!",FONT_PAL_RED);
							StockChatBufferLine("本则单号传送失败!",FONT_PAL_RED);
							ResetFlag = TRUE;
							DeathAction( pActOnlineGmWnd);
							pActOnlineGmWnd = NULL;
							return 1;
						}				
						onlinegmProcNo = 0 ;
						return 0 ;
					}	
				}
			}
			if((mouse.onceState & MOUSE_LEFT_CRICK) && HitDispNo == SureBtn[1])  //取消钮
			{
				DeathAction( pActOnlineGmWnd);
				pActOnlineGmWnd = NULL;
				onlinegmProcNo = 0 ;
				ResetFlag = FALSE;
				return 0 ;
			}
		}
	}
	if(CheckMenuFlag()){
		ResetFlag = TRUE;
		DeathAction( pActOnlineGmWnd);
		pActOnlineGmWnd = NULL;
		return 1;
	}
	return 0 ;
}
//************************************************
//函式   : DNS专用函式
//回传值 : TRUE  --- 转换成功
//         FALSE --- 转换失败
//************************************************
BOOL QueryOnlineGmIP(char HostName[])
{
	PHOSTENT info;

//	strcpy(OnlineGmIP,"10.0.1.60");    // 直接给ip,保留转换code
//	return TRUE;

	if( (info = gethostbyname(HostName)) == NULL)
		return FALSE;
	else{
		OnlineGmIP = inet_ntoa(*(struct in_addr *)*info->h_addr_list);		
	}
	return TRUE;

}
#endif
