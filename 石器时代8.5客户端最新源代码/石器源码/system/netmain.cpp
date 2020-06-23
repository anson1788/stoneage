#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include <winsock.h>
#include "../systeminc/netmain.h"
#include "../systeminc/lssproto_cli.h"
#include "../wgs/message.h"
#include "../systeminc/login.h"
#include <time.h>
#include "zlib.h"
#pragma comment(lib,"zlib.lib")
// Nuke 0615: Avoid 7's lock
int isWGS7=0;

unsigned int sockfd;	// SOCKET ? unsigned int?
int init_net;			// WSAStartup??????????????????server_choosed??????
int server_choosed;		// ????????????????????

char *net_writebuf = NULL;
char *net_readbuf = NULL;
int net_readbuflen;
int net_writebuflen;

BOOL disconnectServerFlag = FALSE;
BOOL oldDisconnectServerFlag = FALSE;

#ifdef _LOG_MSG
char debugLogFileName[256] = "recvdata.txt";	// ?????????????????
#endif


struct gameserver gmsv[MAX_GMSV];

// ???????????
//  ??:	int index		: ???????0 ??getServerMax?-1??????
//  		char *hostname	: IP?????
//			short *port		: ????
//  ??:	 0 ... ?
//			-1 ... 
//
//  ?????I/O????
int getServerInfo( int index, char *hostname, short *port )
{
	if( index < 0 || index >= sizeof( gmsv)/sizeof( gmsv[0] ) )
		return -1;

	strcpy( hostname,gmsv[index].ipaddr );
	*port = atoi( gmsv[index].port );

	return 0;
}

/*
	????????????????????????????

  ??????????????????????


  ?????I/O??????

 */
char rpc_linebuffer[NETBUFSIZ];     // rpc?

extern int testCnt;

#ifdef _STONDEBUG_
int CheckNetErrror( void )
{
	int error = WSAGetLastError();
	// ??????
	switch ( error ){
		case WSANOTINITIALISED:
			return error;
		case WSAENETDOWN:
			return error;
		case WSAEACCES:
			return error;
		case WSAEINTR:
			return error;
		case WSAEINPROGRESS:
			return error;
		case WSAEFAULT:
			return error;
		case WSAENETRESET:
			return error;
		case WSAENOBUFS:
			return error;
		case WSAENOTCONN:
			return error;
		case WSAENOTSOCK:
			return error;
		case WSAEOPNOTSUPP:
			return error;
		case WSAESHUTDOWN:
			return error;
		case WSAEWOULDBLOCK:
			return error;
		case WSAEMSGSIZE:
			return error;
		case WSAEINVAL:
			return error;
		case WSAECONNABORTED:
			return error;
		case WSAECONNRESET:
			return error;
		case WSAETIMEDOUT:
			return error;
		default:
			return error;
	}
	return error;
}
#endif

#ifdef _STONDEBUG_

/*
  ???Web???????????
  
  do_http_request_forever()

  static ??????????????????????

  char *ip : IP????(???????????????????)
  unsigned short port : ?????????80?
  char *obj : ??????????????
  int sec : ???????

  ????： 0??????????????
 */
static int http_sock;
static int http_call_counter = 0;
static time_t http_call_last_time;
static int http_connecting = 0;
static int http_sent_request = 0;

int
do_http_request_forever( char *ip, unsigned short port,
                         char *obj, int sec )
{
    fd_set rfds,wfds;
    int r;
    struct timeval tm;

    /* ??????????????????
       1?1?????????? */
    if( http_call_last_time != time(NULL )){
        http_call_last_time = time(NULL);
    } else {
        return 0;
    }

    //fprintf(stderr,".");
    
    if( http_call_counter == 0 ){
        struct sockaddr_in sin;

        /* sec?1?connect?? */
        if( time(NULL) % sec != 0 )return 0;
        
        http_sock = socket( AF_INET, SOCK_STREAM, 0 );
        if( http_sock < 0 )return -1;

		unsigned long flg = 1;
		ioctlsocket( http_sock, FIONBIO, &flg );
#if 0
        fprintf(stderr,"socket()\n" );
        flags = fcntl( http_sock, F_GETFL,0);
        if( fcntl( http_sock, F_SETFL, flags|O_NONBLOCK )< 0){
            close( http_sock );
            return -2;
        }
#endif
        memset( &sin, 0 , sizeof( sin ));
        sin.sin_addr.s_addr = inet_addr( ip );
        sin.sin_port = htons( port );
        sin.sin_family = AF_INET;
        
        if( connect( http_sock, (struct sockaddr*)&sin,
                     sizeof(sin))== SOCKET_ERROR ){
	
			if( WSAGetLastError() == WSAEWOULDBLOCK )
			{
				// ?????????????????
            }
			else
			{
				closesocket( http_sock );
				return -5;
			}
		}

        http_call_counter ++;
        http_sent_request = 0;
        //fprintf(stderr,"connected\n" );
        return 0;
    }

    FD_ZERO( &rfds );
    FD_SET( http_sock, &rfds );
    FD_ZERO( &wfds );
    FD_SET( http_sock, &wfds );    

    tm.tv_usec = tm.tv_sec = 0;
    r = select( http_sock+1, &rfds, &wfds,(fd_set*)NULL,&tm);

    if( r > 0 && FD_ISSET( http_sock, &rfds ) ){
        char buf[1000];
        r = recv( http_sock, buf,sizeof(buf), 0);
        if( r <= 0 ){
            closesocket( http_sock );
            /* ????????? */
            http_call_counter = 0;
        }
        //fprintf(stderr,"read %d\n",r );        
    }
    
    if( r > 0 && FD_ISSET( http_sock, &wfds ) && http_sent_request == 0 ){
        /* HTTP????MSS?????????
           1??write?????????????
           ???????????????????????????
           ????? */
        int r;
        char fuck[1000];
        sprintf_s( fuck, "GET %s HTTP/1.0\r\n\r\n" , obj );

        r = send( http_sock, fuck, strlen( fuck ), 0 );

        if( r <= 0 ){
            closesocket(r );
            http_call_counter = 0;
            return -10;
        }
        //fprintf(stderr,"wrote %d\n",r );
        http_sent_request = 1;
    }
    return 0;
}
#endif

void networkLoop(void)
{
	if( init_net == FALSE )
		return;

	if( disconnectServerFlag && !oldDisconnectServerFlag){
		// ??????????
		ChangeProc( PROC_DISCONNECT_SERVER );
	}
	oldDisconnectServerFlag = disconnectServerFlag;
	if( disconnectServerFlag)
		return;

    if( server_choosed == 0)return;
	fd_set rfds , wfds, efds;
	
	struct timeval tm;
	tm.tv_sec = 0;
	tm.tv_usec = 0;

	FD_ZERO( &rfds );
	FD_ZERO( &wfds );
	FD_ZERO( &efds );

	FD_SET( sockfd , &rfds );
	FD_SET( sockfd , &wfds );
	FD_SET( sockfd , &efds );

	int a = select(  2 , &rfds , &wfds , (fd_set*)NULL, &tm );
#if 0
	if( FD_ISSET( sockfd , &efds )){
		char buf[256]; 
		memset( buf , 0 , sizeof(buf));
		int len = recv( sockfd , buf , sizeof( buf ) -1 , 0 );
		if( len == SOCKET_ERROR ){
#ifdef _STONDEBUG_
			CheckNetErrror();
#endif
			closesocket( sockfd );
			dwServer = NULL;
			// ??????????????
			disconnectServerFlag = TRUE;
		}
	}
#endif
	
	// ???
	int len = SOCKET_ERROR;
#ifdef __NEW_CLIENT_MEM
	BOOL ret;
	DWORD oldprotect;
#ifndef __NEW_CLIENT_ONLY_WRITE
	ret = VirtualProtect( net_readbuf, NETBUFSIZ, PAGE_READWRITE, &oldprotect);
#endif
#endif
	if( FD_ISSET( sockfd , &rfds) ){
		len = recv( sockfd , rpc_linebuffer , (NETBUFSIZ >> 1) - 1, 0 );
		if (isWGS7) {
			if ((len>1400)&&(len<=1460)) {
				Sleep(500);
				len += recv( sockfd , rpc_linebuffer+len , (NETBUFSIZ >> 1) - 1, 0 );
			}
			isWGS7=0;
		}
		if( len == SOCKET_ERROR ){
			if( WSAGetLastError() != WSAEWOULDBLOCK ){
#ifdef _STONDEBUG_
				CheckNetErrror();
#endif
				closesocket(sockfd);
				dwServer = NULL;
				// ??????????????
				disconnectServerFlag = TRUE;
			}
		} else appendReadBuf( rpc_linebuffer , len );
	}
	int aaaaaaaa=0;
	while( len!=SOCKET_ERROR && net_readbuflen>0){
		// get line from read buffer
		if(GS==dwServer){
			if(!getLineFromReadBuf( rpc_linebuffer  ,sizeof(rpc_linebuffer) )){

				if( bNewServer)
					SaDispatchMessage( sockfd , rpc_linebuffer );
				else
					lssproto_ClientDispatchMessage( sockfd , rpc_linebuffer );
			}else break;
		}else ReadWGSMessage(net_readbuf,net_readbuflen);
	}
#ifdef __NEW_CLIENT_MEM
#ifndef __NEW_CLIENT_ONLY_WRITE
	ret = VirtualProtect( net_readbuf, NETBUFSIZ, PAGE_NOACCESS, &oldprotect);
#endif
#endif

	static unsigned int writetime = TimeGetTime();
	
	// ????
	if( FD_ISSET( sockfd , &wfds)){
#ifdef __NEW_CLIENT_MEM
		ret = VirtualProtect( net_writebuf, NETBUFSIZ, PAGE_READWRITE, &oldprotect);
#endif
		len = 0;
		if( net_writebuflen) len = send( sockfd, net_writebuf , net_writebuflen , 0 );
		/*???????*/
		if(len > 0) writetime = TimeGetTime();

		if( len == SOCKET_ERROR ){
			if( WSAGetLastError() != WSAEWOULDBLOCK ){
#ifdef _STONDEBUG_
				CheckNetErrror();
#endif
     		closesocket(sockfd);
				dwServer = NULL;
				// ??????????????
				disconnectServerFlag = TRUE;
			}
		} else {
			if( len) shiftWriteBuf( len );
		}
#ifdef __NEW_CLIENT_MEM
		ret = VirtualProtect( net_writebuf, NETBUFSIZ, PAGE_NOACCESS, &oldprotect);
#endif
	}
	if((GS==dwServer) && (writetime + 30*1000 <TimeGetTime())){
		if(	init_net == TRUE){
			if( bNewServer)
				lssproto_Echo_send(sockfd, "hoge");
			else
				old_lssproto_Echo_send(sockfd, "hoge");
			testCnt++;
		}
	}
}










/*
   StoneAge??????????????????


???????????????????????????select?
???????????????????????????????????
???????????????????????HTTP???????????
??????


  ??????????????

  1. ??????initNet()??     
  2. ????networkLoop????????????? 
  3. cli.cpp ???? gmsv ??????????????????
  4. ?????????????????????

*/



//***************************************
// ?????????
//***************************************
BOOL initNet( void )
{
	int ret;
	WSADATA wsadata;

	// winsock??????
    ret = WSAStartup( MAKEWORD( 1, 1 ), &wsadata );
	if( ret != 0 )
		return FALSE;

    // lssproto????
//cary test	lssproto_InitClient( appendWriteBuf, 65536, sockfd );
	lssproto_InitClient( appendWriteBuf, NETBUFSIZ, sockfd );

#ifdef _LOG_MSG
	//??????
	{
		// ????????????
		FILE *fp;
		fp = fopen( debugLogFileName, "w" );
		fclose(fp);
	}
	lssproto_SetClientLogFiles( debugLogFileName , debugLogFileName );
#endif

#ifdef _STONDEBUG_
		{	// ????????????????
			FILE *fp = fopen( "recvtime.txt", "w" );
			if( fp ) fclose( fp );
		}
#endif	

	init_net = TRUE;
	disconnectServerFlag = FALSE;
	oldDisconnectServerFlag = FALSE;

	return TRUE;
}


//***************************************
// ?????????????????
//***************************************
void cleanupNetwork( void )
{
	if( init_net == FALSE )
		return;

	init_net = FALSE;
	server_choosed = 0;

	disconnectServerFlag = FALSE;
	oldDisconnectServerFlag = FALSE;

	closesocket( sockfd );
	dwServer = NULL;
	WSACleanup();

	// lsrpc?????????????????????
	// ????????????????????????????
	lssproto_CleanupClient();
}


// ?????????????
//
//  ??：	   0 ... ?
//				  -1 ... ????????
//				-100 ... ?????????
int appendReadBuf(  char *buf, int size )
{
#ifdef _NEWSHOP_
	BOOL flg=TRUE;
	while(*(DWORD *)buf == 0xC7B3CCC9){   //buf[0]==0xFFFFFFC9 && buf[1]==0xFFFFFFCC && buf[2]==0xFFFFFFB3 && buf[3]==0xFFFFFFC7
		extern int 字符串切割( char *arr[], char *str, const char *del);
		extern void 商城文件保存(char * 路径,商城文件_ *商城);
		extern 商城文件_ 商城文件[5];
		char patch[256]={0};
		int id=-1;
		buf+=4;
		char *临时单个道具数据[4]={0};
		unsigned int 总大小,压缩大小;
		字符串切割(临时单个道具数据,(char*)buf,"|");
		总大小 = atoi(临时单个道具数据[0]);
		压缩大小 = atoi(临时单个道具数据[1]);
		id = atoi(临时单个道具数据[2]);
		memcpy(商城文件[id].MD5码,临时单个道具数据[3],32);
		sprintf_s(patch,"./data/shop%d.bin",id+1);
		if(商城文件[id].商城数据!=NULL)
			free(商城文件[id].商城数据);
		商城文件[id].商城数据 = (char*)malloc(总大小+1);
		memset(商城文件[id].商城数据,0,总大小+1);
		uLongf lendata = 总大小-1;
		uncompress((Bytef*)商城文件[id].商城数据,&lendata,(Bytef*)(buf+51),(unsigned long)压缩大小);
		商城文件保存(patch,&商城文件[id]);
		memset(buf-4,0,压缩大小+55);
		size -=压缩大小+55;
		buf+=压缩大小+51;
		flg=FALSE;
	}
	if(!flg) return -1;
#endif
	if( server_choosed == 0 )
		return -100;

	if( (net_readbuflen + size) > NETBUFSIZ )
        return -1;

	memcpy( net_readbuf + net_readbuflen, buf, size );
	net_readbuflen += size;
	return 0;
}


// ?????????????
//
//  ??：	   0 ... ?
//				  -1 ... ????????
//				-100 ... ?????????
int appendWriteBuf( int index, char *buf, int size )
{
	if( server_choosed == 0 )
		return -100;
#ifndef __NEW_CLIENT_MEM
	if( (net_writebuflen + size) > NETBUFSIZ )
		return -1;
	memcpy( net_writebuf + net_writebuflen, buf, size );
	net_writebuflen += size;
#else
	DWORD oldprotect;
	BOOL ret;
	ret = VirtualProtect( net_writebuf, NETBUFSIZ, PAGE_READWRITE, &oldprotect);

	if( (net_writebuflen + size) > NETBUFSIZ )
		return -1;
	memcpy( net_writebuf + net_writebuflen, buf, size );
	net_writebuflen += size;

	ret = VirtualProtect( net_writebuf, NETBUFSIZ, PAGE_NOACCESS, &oldprotect);
#endif
	return 0;
}


// ????????????size???????
// ?????size????????????
//
//  ??：	   0 ... ?
//				  -1 ... ?????????
//				-100 ... ????????
int shiftReadBuf( int size )
{
	int i;
	if( server_choosed == 0 )
		return -100;

	if( size > net_readbuflen )
		return -1;
	for( i = size; i < net_readbuflen; i++ )
	{
		net_readbuf[i-size] = net_readbuf[i];
	}
	net_readbuflen -= size;
	return 0;
}


// ????????????size???????
// ?????size????????????
//
//  ??：	   0 ... ?
//				  -1 ... ?????????
//				-100 ... ????????
int shiftWriteBuf( int size )
{
	int i;

	if( server_choosed == 0 )
		return -100;
	if( size > net_writebuflen )
		return -1;
	for( i = size; i < net_writebuflen; i++ )
	{
		net_writebuf[i-size] = net_writebuf[i];
	}
	net_writebuflen -= size;
	return 0;
}


// ????????????????????????
//
//  ??：	   0 ... ?
//				  -1 ... ???????
//				-100 ... ?????????
int getLineFromReadBuf( char *output, int maxlen )
{
	int i;

	if( server_choosed == 0 )
		return -100;
	int j;
	for( i = 0; i < net_readbuflen && i < (maxlen-1); i++ )
	{
		if( net_readbuf[i] == '\n' )
		{
			memcpy( output, net_readbuf, i );
			output[i] = '\0';
			// ?????????????? 0x0d??????
			for( j = i+1; j >= 0; j-- )
			{
				if( output[j] == 0x0d )
				{
					output[j] = '\0';
					break;
				}
			}

			// ???????
			shiftReadBuf( i+1 );

			// ????????????????????
			// ?????????????????????????????????
			net_readbuf[net_readbuflen] = '\0';

			return 0;
		}
	}
	return -1;
}


// ???????len??????
//
//  ??：	0? ... ???????
//				 -100 ... ??????????
int sendn( SOCKET s, char *buffer, int len )
{
	int total = 0;
	int r;

	if( server_choosed == 0 )
		return -100;

	while( 1 )
	{
		r = send( s, buffer, len, 0 );
		if( r == SOCKET_ERROR )
			return  SOCKET_ERROR;
		total += r;
		if( total == len )
			return total;
	}
}
