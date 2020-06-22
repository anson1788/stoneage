#ifndef _NETMAIN_H_
#define _NETMAIN_H_

#include "../systeminc/version.h"

struct gameserver
{
	char used;
	char ipaddr[127];
	char port[64];
	char name[64];
#ifdef _SHOW_COUNT						// WON ADD 秀服务器流量
	char count[64];
#endif
#ifdef _SHOWIPSLEEP_
	int delay;
#endif
};

struct gamegroup{
	char	used;
	char	num;
	char	startindex;
	char	name[61];
};

#define SUCCESSFULSTR	"successful"        // ??????????
#define FAILEDSTR		"failed" 
#define OKSTR           "ok"
#define CANCLE          "cancle"

#define TIMEOUT (600*1000)                   //  ????????????????????????

//#define NETBUFSIZ (1024*16)
//#define NETBUFSIZ (1024*8)
#define NETBUFSIZ (1024*64) // Robin 2004.05.04

extern char *net_writebuf;
extern char *net_readbuf;
extern int net_readbuflen;
extern int net_writebuflen;

extern unsigned int sockfd;
extern int server_choosed;
extern int init_net;

extern struct gameserver gmsv[];

#ifdef _LOG_MSG
extern char debugLogFileName[];
#endif

typedef int                 BOOL;
extern BOOL disconnectServerFlag;
extern BOOL oldDisconnectServerFlag;


int getServerInfo( int, char *, short * );


void networkLoop(void);
void networkMycharWalk( int gx , int gy , char *direction );

int initNet(void);
void cleanupNetwork( void );
int appendReadBuf(  char *buf , int size );
int appendWriteBuf( int index , char *buf , int size );
int shiftReadBuf(  int size );
int shiftWriteBuf( int size );
int getLineFromReadBuf( char *output  , int maxlen );
int sendn( unsigned int s , char *buffer , int len );



int getServerInfoByServerName( char *servername , char *hostname , short *port );


// ????????
#define NET_ERRMSG_SOCKLIBERROR		"网路尚未准备好。"
#define NET_ERRMSG_BADNAME          "服务器名称不正确。"
#define NET_ERRMSG_SOCKETERROR      "无法执行socket的TCP。"
#define NET_ERRMSG_NOTGETADDR		"网络故障,请联系客服。"
#define NET_ERRMSG_NOTCONNECT_S		"无法开始进行服务器的连接。"
#define NET_ERRMSG_NOTCONNECT		"服务器尚未开启。请尝试其他连线"
#define NET_ERRMSG_CONNECTTIMEOUT	"服务器连接时间已到。"
#define NET_ERRMSG_LOGINTIMEOUT		"登入处理时间已到。"
#define NET_ERRMSG_CHARLISTTIMEOUT	"读取人物表时间已到。"
#define NET_ERRMSG_LOGOUTTIMEOUT	"登出处理时间已到。"
#define NET_ERRMSG_LOGINFAIL		"无法登入服务器。"
#define NET_ERRMSG_CREATECHARTIMEOUT	"人物制作时间已到。"
#define NET_ERRMSG_DELETECHARTIMEOUT	"人物削除时间已到。"
#define NET_ERRMSG_VERSIONERROR		"您当前的版本太旧，请使用更新器更新游戏后登陆！"

#define GAMESTATE_ERRMSG_LOGOUTDENY	"服务器拒绝处理登出。"


#endif
